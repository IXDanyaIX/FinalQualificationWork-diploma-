#include "reportdialog.h"
#include "ui_reportdialog.h"
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocument>
#include <QDate>

ReportDialog::ReportDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::on_generateReportPushButton_clicked()
{
    QString id = ui->idEdit->text();
    QString fromDate = ui->fromDateEdit->date().toString("yyyy-MM-dd");
    QString toDate = ui->toDateEdit->date().toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare(R"(
            SELECT
                g.name AS "Название товара",
                SUM(sg.quantity) AS "Количество купленного товара",
                ROUND(SUM(sg.quantity * g.purchase_price)::numeric, 2) AS "Сумма покупки, руб",
                COALESCE(SUM(rgs.quantity), 0) AS "Количество проданного товара",
                ROUND(COALESCE(SUM(rgs.quantity * g.sale_price), 0)::numeric, 2) AS "Сумма продажи, руб",
                ROUND((COALESCE(SUM(rgs.quantity * g.sale_price), 0)::numeric - SUM(sg.quantity * g.purchase_price)::numeric), 2) AS "Прибыль, руб"
            FROM
                goods g
            LEFT JOIN
                shipment_goods sg ON g.ID = sg.ID_goods
            LEFT JOIN
                realization_goods_shipment rgs ON sg.ID = rgs.ID_shipment
            LEFT JOIN
                realization r ON rgs.ID_realization = r.ID AND r.date_realization BETWEEN :fromDate AND :toDate
            WHERE
                g.ID = :id
            GROUP BY
                g.name;
        )");

    query.bindValue(":id", id);
    query.bindValue(":fromDate", fromDate);
    query.bindValue(":toDate", toDate);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    ui->tableView->setModel(model);
}

void ReportDialog::on_saveReportPushButton_clicked()
{
    QAbstractItemModel *model = ui->tableView->model();
    if (!model) {
        qDebug() << "Нет данных для сохранения.";
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить отчет"), "", tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty())
        return;

    QString fromDate = ui->fromDateEdit->date().toString("yyyy-MM-dd");
    QString toDate = ui->toDateEdit->date().toString("yyyy-MM-dd");
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");

    QString reportHtml = "<html><head><title>Отчет</title></head><body>";
    reportHtml += "<h1>Отчет</h1>";
    reportHtml += "<p>Дата создания отчета: " + currentDate + "</p>";
    reportHtml += "<p>Дата с: " + fromDate + " по: " + toDate + "</p>";
    reportHtml += "<table border='1'>";
    reportHtml += "<tr>"
                  "<th>Название товара</th>"
                  "<th>Количество купленного товара</th>"
                  "<th>Сумма покупки, руб</th>"
                  "<th>Количество проданного товара</th>"
                  "<th>Сумма продажи, руб</th>"
                  "<th>Прибыль, руб</th>"
                  "</tr>";

    for (int row = 0; row < model->rowCount(); ++row) {
        reportHtml += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            reportHtml += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
        }
        reportHtml += "</tr>";
    }

    reportHtml += "</table></body></html>";

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    QTextDocument doc;
    doc.setHtml(reportHtml);
    doc.print(&printer);

    qDebug() << "Отчет успешно сохранен.";
}
