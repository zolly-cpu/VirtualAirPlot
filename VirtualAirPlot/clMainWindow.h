#ifndef CLMAINWINDOW_H_WVD_08102011
#define CLMAINWINDOW_H_WVD_08102011

#include <string>
#include <iostream>
#include <vector>
#include <exception>
#include <stdio.h>
//#include <windows.h>
//#include <tchar.h>
#include <stdlib.h>


#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QListWidget>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>




#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clCubeInitialise.h"
#include "clCubeReadValues.h"


using namespace std;


class clMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    clMainWindow (int paArgc, char * paArgv[], QWidget* paParent = 0, const char* paName = 0);
    ~clMainWindow ();
public:


public slots:

    void exitApplication();
    void startProductConfig();
    void readProductValues();

private:
    void createMenus();
    void createActions();




    QMdiArea *meWorkspace;


    //Products
    clCubeInitialise *meCubeInitialise;
    clCubeReadValues *meCubeReadValues;
    QString meConfigurationFile;



	QTimer * meTimer;



    //Private qt vars
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *productMenu;
    QMenu *helpMenu;

    //File actions


    QAction *meExitApplication;
    QAction *meSaveAss;
    QAction *meConfigProduct;
    QAction *meReadValues;

    //Ice communicators for logging
    clIceClientLogging *meIceClientLogging;
    Ice::CommunicatorPtr meIceCommunicatorLogging;
    void initializeLogClient(int paArgc, char * paArgv[]);

    //Ice communicator for server
    clIceClientServer *meIceClientServer;
    Ice::CommunicatorPtr meIceCommunicatorServer;
    void initializeServerClient(int paArgc, char * paArgv[]);
};

#endif
