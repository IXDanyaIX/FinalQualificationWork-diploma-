#ifndef INFORMATION_CATEGORY_H
#define INFORMATION_CATEGORY_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include "goods_model.h"

namespace Ui {
class Information_category;
}

class Information_category : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information_category(QWidget *parent = nullptr);
    ~Information_category();

    void setModel(QAbstractItemModel* model);
    void setCurrentIndex(const QModelIndex& index);

signals:
    void closed();

private slots:
    void on_save_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_last_category_triggered();

    void on_next_category_triggered();

    //my slots
    void updateButtonsAvailability();

public slots:
    void onModelUpdated();


private:

    void Update_state();
    enum class State{
        create,
        update
    };

    State state;
    QModelIndex currentIndex;
    QDataWidgetMapper* mapper;
    QAbstractItemModel *currentModel;
    QSortFilterProxyModel *proxyModel;

    Ui::Information_category *ui;



};

#endif // INFORMATION_CATEGORY_H
