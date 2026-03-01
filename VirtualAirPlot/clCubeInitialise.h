#ifndef CLCUBEINITIALISE_H
#define CLCUBEINITIALISE_H

#include <exception>
#include <string>
#include <iostream>

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
#include <QtWidgets/QMessageBox>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
//#include <QtCore/qtalgorithms>
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTextStream>

#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>




#include "wdCubeInitialise.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clUpdateDatabaseCoord.h"

using namespace std;

class clCubeInitialise : public QWidget
{

    Q_OBJECT;

public:

    clCubeInitialise (clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clCubeInitialise ();

    Ui::wdCubeInitialise meCubeInitialise;
    //QThread meThread;

private slots:
    //Slots Camera setup
    void slotButtonRefreshPressed();
    void slotButtonGeneratePressed();
public slots:
    void handleResults(const QString &);
private:

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
	bool readXMLfile();
    bool fillForm();


    clUpdateDatabaseCoord * meUpdateDatabaseCoord;
    QString meConfigurationFile;
    /*
    <beacon_01>
    <name>sensor_01<name/>
    <ip>192.168.4.1<ip/>
    <port>1234<port/>
    <factor>0.5<factor/>
    <communication_timer>200<communication_timer/>
    <adress>BC-5C-D2-DD-E0-C2<adress/>
    </beacon_01>
    */

    struct
    {
        QString name;
        QString ip;
        int port;
        float factor;
        int communication_timer;
        QString address;
        int x;
        int y;
    } myBeacon01,myBeacon02,myBeacon03;
/*
 < name>sensor_02<name/> *
 <factor>0.5<factor/>
 <communication_timer>2<communication_timer/>
 <adress>8A-5C-D2-DD-E0-C2<adress/>
 <gpio_stop><gpio_stop/>
*/
    struct
    {
        QString name;
        float factor;
        int communication_timer;
        QString address;
        int gpio;
    } mySensor01,mySensor02;
    struct
    {
        int x;
        int y;
        int z;
    } myDim;

    QTreeWidgetItem * meBeacon_01;
    QTreeWidgetItem * meBeacon_02;
    QTreeWidgetItem * meBeacon_03;
    QTreeWidgetItem * meSensor_01;
    QTreeWidgetItem * meSensor_02;
};
#endif
