#ifndef CCONNECTIONSTREE_H
#define CCONNECTIONSTREE_H

#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QTreeWidget>
#include <QXmlStreamWriter>
#include "datasource.h"
#include "tableheadercaption.h"
#include "ccustomproxymodel.h"

class CConnectionsTree
{
public:
    CConnectionsTree();
    ~CConnectionsTree();


    void InitConnectonsTree(QTreeView *connView);
    void UpdateIncludeFilter(const QString &value);
    void UpdateData(CDataSource::SocketInfo * dataSource, bool disableCleanup);


    void SetDataColumnHiden(QTreeView *tree);
    ///Export as format CSV, XML or HTML
    void Save(QWidget *parent);

    QString GetStateString(uint state);

private:
    QMap<CDataSource::ColumnData, TableHeaderCaption>   m_caption;
    QMap<CDataSource::eNetType, QString>                m_protocol;
    QMap<CDataSource::ConnectionTcpState, QString>      m_state;
    CCustomProxyModel                                   *m_pProxyModel;


    void PrepareTableCaptionString();
    void PrepareTableStateString();
    void PrepareTableProtocolString();

};

#endif // CCONNECTIONSTREE_H
