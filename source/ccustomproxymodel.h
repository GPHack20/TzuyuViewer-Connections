#ifndef CCUSTOMPROXYMODEL_H
#define CCUSTOMPROXYMODEL_H

#include <QSortFilterProxyModel>

//Custom Proxy model and filter data for QTree View
class CCustomProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    CCustomProxyModel(QObject *parent = 0);
    void setFilterRegExpEx(const QString &val);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

    QRegExp m_QRegExp;
};

#endif // CCUSTOMPROXYMODEL_H
