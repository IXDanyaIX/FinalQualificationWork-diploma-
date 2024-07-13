#ifndef INFORMATION_GOODS_H
#define INFORMATION_GOODS_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QModelIndex>
#include "goods_model.h"

namespace Ui {
class Information_goods;
}

class Information_goods : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information_goods(QWidget *parent = nullptr);
    ~Information_goods();
    void setModel(QAbstractItemModel* model);
    void setCurrentIndex(const QModelIndex& index);

signals:
    void closed();

private slots:


    void on_last_goods_triggered();

    void on_next_goods_triggered();

    void on_save_pushButton_clicked();

    void on_close_pushButton_clicked();
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
    // QSortFilterProxyModel *proxyModel;
    Ui::Information_goods *ui;
};

#endif // INFORMATION_GOODS_H
