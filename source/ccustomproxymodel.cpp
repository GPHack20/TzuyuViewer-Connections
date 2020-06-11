#include "ccustomproxymodel.h"

CCustomProxyModel::CCustomProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_QRegExp.setCaseSensitivity(Qt::CaseInsensitive); //Insensitive case: It does not matter upper or lower case.
    m_QRegExp.setPatternSyntax(QRegExp::RegExp); //Sets the syntax mode for the regular expression. The default is QRegExp::RegExp.
}

void CCustomProxyModel::setFilterRegExpEx(const QString &val)
{
    m_QRegExp.setPattern(val); //Sets the pattern string to pattern.
    setFilterRegExp(m_QRegExp); //Filter the contents of the source model
}

bool CCustomProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const  //Filter by rows
{

    if (filterRegExp().isEmpty())
        return true;

    for(int ind = 0; ind < sourceModel()->columnCount() - 1; ind++)
    {
        QModelIndex index = sourceModel()->index(sourceRow, ind, sourceParent);
        if(sourceModel()->data(index).toString().contains(filterRegExp()))
            return true;
    }

    return false;
}


