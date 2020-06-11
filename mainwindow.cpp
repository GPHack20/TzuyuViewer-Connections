#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QSettings>
#include <QMessageBox>
#include <QTextEdit>
#include <QClipboard>
#include <QMenu>
#include <thread>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent) //Initialize the constructor with initial values.
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_captureEnable(false) //Record the process - without removing records - start false so the "Record button" = enable
    , m_visibleItems(0)
    , m_totalItems(0)

{

    ui->setupUi(this);

    //Set icon to the application
    setWindowIcon(QPixmap(":/data/tcpvieww.svg"));

    //Load inicial data to treeview
    m_cconnectionstree.InitConnectonsTree(ui->treeView_connection);

    //Configure Slider - Zoom
    m_ccfontsize.ConfigureSlider(ui->horizontalSlider, this);

    connect(this, SIGNAL(callUpdateGui()),   this, SLOT(updateGui())); //Connect the window with the update funtion - it allows to update the windows

    connect(ui->treeView_connection->selectionModel(), //To set an item as the current item without selecting it
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(currentSelectionChanged(const QModelIndex, const QModelIndex)));

    ui->pushButton_whois->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Pause_toggled(bool checked)  //Enable or disable the Pause button
{
    m_NetData.pauseUpdate(checked);
}

void MainWindow::on_pushButton_Record_toggled(bool checked) //Enable or disable the Record button
{
    m_captureEnable = checked;
}

void MainWindow::on_pushButton_copyToClipboard_clicked()    //Row to clipboard
{
    auto pClp = QApplication::clipboard();
    if( pClp != nullptr )
        pClp->setText(m_ClipBoardString);
}

void MainWindow::on_pushButton_SaveToFile_clicked() //Write in format XML, HTML or CSV
{
    auto pause = m_NetData.IsPause();
    m_NetData.pauseUpdate(true);
    m_cconnectionstree.Save(ui->treeView_connection);
    m_NetData.pauseUpdate(pause);
}

void MainWindow::on_pushButton_whois_clicked()  //Use the command Whois to get information about the remote address
{
    ShowWhois(m_RowText, m_whoisText);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)    //Resize objects with Slider
{
    m_ccfontsize.SetFontSize(value);
}

void MainWindow::on_lineEdit_include_textChanged(const QString &arg1)   //Filter with regex the conexions
{
    m_cconnectionstree.UpdateIncludeFilter(arg1);
}

void MainWindow::closeEvent(QCloseEvent *event) //Close the appliction and threads
{
    m_NetData.DeleteRootLoader();
    SaveAppState();
    event->accept();
}

void MainWindow::SaveAppState() //Store the information about the application - Size and Geometry
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(DEFCFG_MAINWINDOWGEOM, saveGeometry());
    settings.setValue(DEFCFG_CONNECTIONTABLE, ui->treeView_connection->header()->saveState());
    m_ccfontsize.SaveConfig();
}

void MainWindow::showEvent(QShowEvent *event)   //Even to show the application with savedstate
{
    QMainWindow::showEvent( event );

    try
    {

        if(!m_ccfontsize.Init())
            return; //avoid reinitialization on show/hide form

        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        auto testv = settings.value(DEFCFG_MAINWINDOWGEOM);

        if(testv.isValid() && !testv.isNull())
        {
            restoreGeometry(settings.value(DEFCFG_MAINWINDOWGEOM).toByteArray());
            ui->treeView_connection->header()->restoreState(settings.value(DEFCFG_CONNECTIONTABLE, "").toByteArray());
        }

        m_cconnectionstree.SetDataColumnHiden(ui->treeView_connection);

        m_NetData.setUpdateCallback([&](void)->void{ emit callUpdateGui(); }); //Lambda expresion - callback update GUI

    }
    catch(...)
    {
        QMessageBox::critical(this, windowTitle(), QObject::tr("Failed on showEvent"), QMessageBox::Ok);
    }
}

void MainWindow::updateGui()    //Update the Gui and proccess
{

    try
    {

        auto dataHelper = m_NetData.GetData();
        if(dataHelper != nullptr)
        {

            /* start update code here */
            auto captureEnable = m_captureEnable;

            auto connList = dataHelper->GetConnectionsList();
            for (auto it = connList->begin(); it != connList->end(); it++ )
            {
                m_cconnectionstree.UpdateData(&it->second, captureEnable);
            }

            if(!captureEnable)
            {
                for (auto it = connList->begin(); it != connList->end(); )
                {
                    if (it->second.deleteItem > 0)
                        it = connList->erase(it);
                    else
                        ++it;
                }
            }

            UpdateStatusText();


            m_NetData.EnableUpdateData();

            /* end update code here */

        }
    }
    catch(std::exception &e)
    {
        QMessageBox::critical(this, this->windowTitle(), e.what(), QMessageBox::Ok);
        exit(-1);
    }
    catch(...)
    {
        QMessageBox::critical(this, this->windowTitle(), "Unexpected exception", QMessageBox::Ok);
        exit(-1);
    }


}

void MainWindow::currentSelectionChanged(const QModelIndex current, const QModelIndex previous) //Enable and disable buttons
{

    Q_UNUSED(previous);

    m_whoisText = "";
    m_RowText = "";

    if(!current.isValid())
    {
        ui->pushButton_whois->setEnabled(false);
        ui->pushButton_whois->setText("Whois");
        return;
    }

    auto row = current.row();

    auto src = qobject_cast<QSortFilterProxyModel*>(ui->treeView_connection->model());
    if(src == nullptr)
    {
        ui->pushButton_whois->setEnabled(false);
        ui->pushButton_whois->setText("Whois");
        return;
    }

    QString tmpToolTipStr;

    for(auto index = 0; index < CDataSource::COLUMN_DATA_DATA; index++)
    {
        auto tmpStr = src->data(src->index(row, index)).toString();

        m_RowText += tmpStr + "\t";
        tmpToolTipStr +=  tmpStr + "\n";

        if(index == CDataSource::COLUMN_DATA_REMOTEADDRESS)
        {
           auto whoisText = src->data(src->index(row, index)).toString();
            if(QString::compare(whoisText, "0.0.0.0", Qt::CaseInsensitive) != 0 &&
               QString::compare(whoisText, "::", Qt::CaseInsensitive) != 0)
            {
                m_whoisText = whoisText;
                ui->pushButton_whois->setText("Whois \"" + whoisText + "\"");
                ui->pushButton_whois->setEnabled(true);
            }
            else
            {
                ui->pushButton_whois->setEnabled(false);
                ui->pushButton_whois->setText("Whois");
            }

        }
    }

    ui->pushButton_copyToClipboard->setToolTip(tmpToolTipStr);

    m_ClipBoardString = m_RowText;

}

void MainWindow::ShowWhois(QString rowText, QString whoisText)  //Find information about the remote host to show in dialog later
{
    rowText.replace("\t"," ");
    rowText += "\n\n";

    /*QMessageBox msgBox;
    msgBox.setText("row:**"+rowText);
    msgBox.exec();*/


    QString whoisOutput;
    QString err;
    QString commandString = "whois " + whoisText;

    try {
        QProcess exec;
        exec.start(commandString);
        exec.waitForFinished();
        whoisOutput = QString(exec.readAllStandardOutput());
    } catch(const std::exception &e) {
        err = QString(e.what());
    } catch(...) {
        err = QObject::tr("Unexpected exception");
    }

    if(whoisOutput.isEmpty())
    {
        if(err.isEmpty())
        {
            whoisOutput = QObject::tr("Command 'whois' not found,\n"
                                      "but can be installed with:\n"
                                      "sudo apt install whois");
        }
        else
        {
            whoisOutput = QObject::tr("Failed run 'whois':\n") + err;
        }
    }

    rowText += whoisOutput;
    ShowInfoDialog(commandString, rowText, true);
}

void MainWindow::ShowInfoDialog(QString title, QString dialogText, bool readonly)   //Dialog to show information about Whois command
{
    auto infoDialog = new QDialog(this);
    infoDialog->setWindowTitle(title);
    infoDialog->setWindowFlags(infoDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto infoEditor = new QTextEdit;
    infoEditor->setLineWrapMode(QTextEdit::NoWrap);
    infoEditor->setPlainText(dialogText);

    if(readonly)
    {
        infoEditor->setReadOnly(true);
     }

    QFont newfont("monospace");
    newfont.setStyleHint(QFont::Monospace);
    newfont.setPointSize((this->font()).pointSize());
    infoEditor->setFont(newfont);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoEditor );
    infoDialog->setLayout(mainLayout);

    infoDialog->setAttribute(Qt::WA_DeleteOnClose);// ?
    auto newsize = this->size();

    infoDialog->resize(newsize.width()/2, newsize.height()/2);
    infoDialog->show();
    infoDialog->raise();
    infoDialog->activateWindow();
}

void MainWindow::on_treeView_connection_customContextMenuRequested(const QPoint &pos)   //custommenu to get cell or row
{

    typedef enum{
        CopyCell = 1,
        CopyRow
    }MenuAction;

    auto selCell = ui->treeView_connection->indexAt(pos);
    if (!selCell.isValid())
        return;

    auto src = qobject_cast<QSortFilterProxyModel*>(ui->treeView_connection->model());
    if(src == nullptr)
        return;

    src->setDynamicSortFilter(false);

    auto cellText = selCell.data().toString();
    auto rowText = QString();

    for(auto index = 0; index < CDataSource::COLUMN_DATA_DATA; index++)
        rowText += src->data(src->index(selCell.row(), index)).toString() + "\t";

    src->setDynamicSortFilter(true);

    QMenu menu;

    if(!cellText.isEmpty())
    {
        auto actionCell = new QAction(QObject::tr("Copy") + " \"" + CutLongText(cellText, 80) + "\"", &menu);
        actionCell->setData(CopyCell);
        menu.addAction(actionCell);
    }

    auto actionRow = new QAction(QObject::tr("Copy row"), &menu);
    actionRow->setData(CopyRow);
    menu.addAction(actionRow);

    auto ret = menu.exec(QCursor::pos());
    if(ret == nullptr)
        return;

    if(QApplication::clipboard() == nullptr)
        return;

    if(ret->data() == CopyRow)
    {
        QApplication::clipboard()->setText(rowText);
        return;
    }

    if(ret->data() == CopyCell)
    {
        QApplication::clipboard()->setText(cellText);
        return;
    }

}


const QString MainWindow::GetAuthGuiName()  //Get admin to see the details of the proccess
{
    QStringList passwordReqApp;
    passwordReqApp << "/usr/bin/pkexec";
    passwordReqApp << "/usr/bin/kdesudo";
    passwordReqApp << "/usr/bin/gksudo";
    passwordReqApp << "/usr/bin/kdesu";
    passwordReqApp << "/usr/bin/gksu";


    foreach(auto tmpv, passwordReqApp)
    {
        auto name = tmpv.split(" ").at(0);
        if( access(name.toLatin1(), X_OK ) == 0 )
        {
            if(name.startsWith("/usr/bin/pkexec"))
                return  QString("%1 %2 --rootmodule").arg(tmpv, QCoreApplication::applicationFilePath());

            return QString("%1 \"%2 --rootmodule\"").arg(tmpv, QCoreApplication::applicationFilePath());
        }
    }

    auto errorString = QString(
                QObject::tr("Cannot start module, because \n%1\n is missing or execution access is denied."))
                .arg(passwordReqApp.join("\n"));

    QMessageBox::critical(this, QObject::tr("Run as root"), errorString, QMessageBox::Ok);

    return "";

}

void MainWindow::on_pushButton_displayNames_clicked()   //load the proccess after get admin
{

    auto cmd = GetAuthGuiName();
    if(cmd.isEmpty())
        return;

    std::cout << cmd.toStdString() << std::endl;

    auto pause = m_NetData.IsPause();
    m_NetData.pauseUpdate(true);
    setEnabled(false);
    //setDisabled(true);

    auto rootResolver = new QProcess(this);
    rootResolver->start(cmd);
    if(!rootResolver->waitForStarted(-1))
    {
        auto retErrString = rootResolver->errorString();
        std::cout << "QProcess error: " << retErrString.toStdString() << std::endl;

        m_NetData.pauseUpdate(pause);
        setEnabled(true);
        //setDisabled(false);
        QMessageBox::critical(this, QObject::tr("Run as root"), QObject::tr("Failed on start\n") + retErrString + "\n" + cmd, QMessageBox::Ok);
        return;
    }

    if(!m_NetData.InitRootLoader())
    {
        m_NetData.pauseUpdate(pause);
        setEnabled(true);
        //setDisabled(false);
        QMessageBox::critical(this, QObject::tr("Run as root"), QObject::tr("Failed on 'init' client module"), QMessageBox::Ok);
        return;
    }

    setEnabled(true);
    //setDisabled(false);
    m_NetData.pauseUpdate(pause);
    ui->pushButton_displayNames->setEnabled(false);

}

QString MainWindow::CutLongText(const QString &sourceText, int maxLength)   //Cut long Text
{

    if(sourceText.length() < maxLength)
        return sourceText;

    auto splitText = sourceText.split(" ");

    QString retText;
    foreach(auto varTmp, splitText)
    {
        if(retText.length() + varTmp.length() > maxLength)
        {
            retText += "...";
            break;
        }
        retText += " " + varTmp;
    }
    return retText;

}

void MainWindow::UpdateStatusText() //Update text according the program is updated
{
    auto visibleItems = 0;
    auto totalItems = 0;

    auto proxy = qobject_cast<QSortFilterProxyModel*>(ui->treeView_connection->model());
    if(proxy != nullptr)
    {

        visibleItems = proxy->rowCount();

        auto source = qobject_cast<QStandardItemModel*>(proxy->sourceModel());
        if(source != nullptr)
        {
            totalItems = source->rowCount();
        }

    }

    if(visibleItems == m_visibleItems && totalItems == m_totalItems)
        return;

    m_visibleItems = visibleItems;
    m_totalItems = totalItems;

    auto labelText = QString(tr("  %1 / %2  ")).arg(visibleItems).arg(totalItems);
    ui->label_status->setText(labelText);

    auto labelToolTip = QString(tr(" Visible %1 / Total %2 ")).arg(visibleItems).arg(totalItems);
    ui->label_status->setToolTip(labelToolTip);

}

void MainWindow::on_lineEdit_include_editingFinished()
{

}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionNames_triggered()
{
    on_pushButton_displayNames_clicked();
}

void MainWindow::on_actionPause_triggered()
{
}

void MainWindow::on_pushButton_Pause_clicked()
{

}

void MainWindow::on_pushButton_Record_clicked()
{

}

void MainWindow::on_actionRecord_triggered()
{

}

void MainWindow::on_actionCopy_triggered()
{
    on_pushButton_copyToClipboard_clicked();
}

void MainWindow::on_actionSave_triggered()
{
    on_pushButton_SaveToFile_clicked();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgabout;
    msgabout.setText("    <h1 style=\"color:black;\"> ОперСист (КП) 2020</h1>"
                     "    <h5 style=\"color:red;\"> by Guido Peñaloza</h5>"
                     "    <h2 style=\"color: grey;\">Kурсовой</h2>"
                     "    <p style=\"color: dodgerblue; text-align:center;\">Програмний засіб перегляду відкритих TCP / IP з'єднань Відображення всіх відкритих TCP і UDP портів на комп'ютері. "
                     "    Інформація про процеси, що відкрили порти. Можливість фільтрації списку за будьяким стовпчиком (номер порту, ім'я процесу і т.д.)."
                     "    Можливість експорту інформації в HTML та CSV.</p>");
    msgabout.setTextFormat(Qt::RichText);
    msgabout.exec();
}

void MainWindow::on_actionContent_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/GPHack20/TzuyuViewer-Connections", QUrl::TolerantMode));
}
