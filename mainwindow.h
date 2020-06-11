#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <QProcess>
#include "source/cconnectionstree.h"
#include "source/ccfontsize.h"
#include "source/updatethread.h"
#include "source/clanguage.h"
#include <QDesktopServices>

#define DEFCFG_MAINWINDOWGEOM  "base/MainWindowGeometry"
#define DEFCFG_CONNECTIONTABLE "base/TableHeaderState"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT //Allow to use signals and slots

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent( QShowEvent* event );

private slots:
    void on_pushButton_displayNames_clicked();
    void on_pushButton_Pause_toggled(bool checked);
    void on_pushButton_Record_toggled(bool checked);
    void on_pushButton_copyToClipboard_clicked();
    void on_pushButton_SaveToFile_clicked();
    void on_pushButton_whois_clicked();
    void on_lineEdit_include_textChanged(const QString &arg1);
    void on_horizontalSlider_valueChanged(int value);
    void updateGui();
    void currentSelectionChanged(const QModelIndex current, const QModelIndex previous);
    void on_treeView_connection_customContextMenuRequested(const QPoint &pos);

    void on_lineEdit_include_editingFinished();

    void on_horizontalSlider_actionTriggered(int action);

    void on_actionExit_triggered();

    void on_actionNames_triggered();

    void on_actionPause_triggered();

    void on_pushButton_Pause_clicked();

    void on_pushButton_Record_clicked();

    void on_actionRecord_triggered();

    void on_actionCopy_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();

    void on_actionContent_triggered();

signals:
    void callUpdateGui();

private:

    Ui::MainWindow  *ui;
    CConnectionsTree m_cconnectionstree;
    CCFontSize       m_ccfontsize;
    CUpdateThread    m_NetData;
    bool             m_captureEnable;
    QString          m_whoisText;
    QString          m_RowText;
    QString          m_ClipBoardString;
    int              m_visibleItems;
    int              m_totalItems;

    void SaveAppState();
    void ShowWhois(QString rowText, QString whoisText);
    void ShowInfoDialog(QString title, QString dialogText, bool readonly);
    const QString GetAuthGuiName();
    QString CutLongText(const QString &sourceText, int maxLength);
    void UpdateStatusText();
};

#endif // MAINWINDOW_H
