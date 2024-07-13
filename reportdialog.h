#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QDebug>

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = nullptr);
    ~ReportDialog();

private slots:
    void on_generateReportPushButton_clicked();



    void on_saveReportPushButton_clicked();

private:
    Ui::ReportDialog *ui;
};

#endif // REPORTDIALOG_H
