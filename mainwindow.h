#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "information_category.h"
#include "information_order.h"
#include "information_shipment.h"
#include "information_goods.h"
#include "information_realization.h"
#include "xlsxwriter.h"
#include <QFileDialog>
#include "searhform.h"
#include "reportdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Обработка нажития для ctrl
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    // Обновление модели
    void modelUpdated();

private slots:

    // Обработка нажатия на кнопки панели
    void on_contractors_triggered();

    void on_goods_triggered();

    void on_units_of_measurmant_triggered();

    void on_category_triggered();

    void on_orders_triggered();

    void on_shipment_triggered();

    void on_realization_triggered();

    void on_tabWidget_tabBarDoubleClicked(int index);

    void on_add_tab_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_add_record_triggered();

    void on_delete_record_triggered();

    void on_report_action_triggered();

    void on_status_goods_triggered();

    void on_search_triggered();


    // my slots
    // Отслеживание нажатия на таблицу
    void onTableClicked(const QModelIndex &index);

    // Закрытие форм с расширенной информацией
    void onInformationCategoryClosed();
    void onInformationOrderClosed();
    void onInformationShipmentClosed();
    void onInformationGoodsClosed();
    void onInformationRealizationClosed();

    // Улдаление вкладки
    void on_remove_tab_triggered();

    void on_status_realization_triggered();

    void on_exportExcel_triggered();



    void searchInTable(const QString &searchText);


private:
    void exportToDOCX(const QString &fileName);

    // Перечесление с активными таблицами
    enum class ActiveTable{
        None,
        Category,
        Goods,
        Order,
        Realization,
        Shipment,
        Contractors,
        Units,
        Status_realization,
        Status_goods
    };

    QTableView* currentTableData;

    void showInformationMessage(const QString &message);
    void showErrorMessage(const QString &message);

    // Создание таблицы talbeview
    QTableView* createTable();
    QTableView* createTableView();
    void setupLayout(QTableView* view);
    void setHeaderResizeMode(QTableView* view);
    void enableSorting (QTableView* view);

    // Расширенное редактирование записи
    void expandCategoryTable(const QModelIndex &index);
    void expandOrderTable(const QModelIndex &index);
    void expandShipmentTable(const QModelIndex &index);
    void expandGoodsTable(const QModelIndex &index);
    void expandRealizationTable(const QModelIndex &index);


    void showForm(QWidget *form);

    ActiveTable currentTable;
    int currentRow;
    bool ctrlPressed;

    Information_order *infOrder;
    Information_category *infCategory;
    Information_shipment *infShipment;
    Information_goods *infGoods;
    Information_realization *infRealization;
    // для поиска
    SearhForm* searchForm;
    QAbstractItemModel *originalModel;
    ActiveTable searchTable;
    bool isProxyModelActive;
    QSortFilterProxyModel *proxyModel;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
