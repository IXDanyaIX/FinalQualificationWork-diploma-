#ifndef CUSTOMSORTFILTERPROXYMODEL_H
#define CUSTOMSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class CustomSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit CustomSortFilterProxyModel(QObject *parent = nullptr);

    void setSortColumn(int column, Qt::SortOrder order);
    void setFilterRegExp(const QRegExp &regExp);

};

#endif // CUSTOMSORTFILTERPROXYMODEL_H
