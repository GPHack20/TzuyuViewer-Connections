#include "mainwindow.h"
#include <QApplication>
#include "source/rootmodule.h"
#include "source/clanguage.h"

int main(int argc, char *argv[])
{
    /*Enable the root mode to load the information about the proccess*/
    auto runRootModule = (argc == 2 && argv[1] != nullptr && strstr(argv[1], "rootmodule") != nullptr);
    if(runRootModule)
    {
        auto root = new CRootModule(getpid());

        if(root->m_error.length() > 1)
        {
            std::cout << "CRootModule failed " << root->m_processId << " error " << root->m_error << std::endl;
            delete root;
            return -1;
        }
        root->RunServer();
        delete root;
        return 0x0;

    }

    //Load the resources: photo and languages
    Q_INIT_RESOURCE(tcpview);

    QApplication app(argc, argv);

    //Store the settings in $HOME/.config/GP/TzuyuViewer.conf
    QCoreApplication::setOrganizationDomain("guruhack20cell-47783.portmap.io");
    QCoreApplication::setOrganizationName("GP");
    QCoreApplication::setApplicationName("TzuyuViewer");
    QCoreApplication::setApplicationVersion("1.0 beta");

    CLanguage        m_lang;
    //m_lang.SetLangByLocale("en");  Change to any other language (Russian, English) with iso code
    m_lang.SetLangByLocale("es");

    MainWindow w;
    w.show();

    return app.exec();

}
