#include "customsortfilterproxymodel.h"

CustomSortFilterProxyModel::CustomSortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {}


void CustomSortFilterProxyModel::setFilterRegExp(const QRegExp &regExp){
     QSortFilterProxyModel::setFilterRegExp(regExp);
}

void CustomSortFilterProxyModel::setSortColumn(int column, Qt::SortOrder order){

    sort(column, order);

}
