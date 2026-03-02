#ifndef CLCUBEREADVALUES_H
#define CLCUBEREADVALUES_H

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
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>
#include <QtGui/QScreen>
#include <QtCore/QSize>

#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QTextureImage>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>

#include "Qt3DExtras/qt3dwindow.h"
#include "Qt3DExtras/qorbitcameracontroller.h"


#include "wdCubeReadValues.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clSocketToBeacons.h"

using namespace std;

class clCubeReadValues : public QWidget
{

    Q_OBJECT;

public:

    clCubeReadValues (clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clCubeReadValues ();

    Ui::wdCubeReadValues meCubeReadValues;
    //QThread meThread;

private slots:
    //Slots Camera setup
    void slotButtonRefreshPressed();
    void slotButtonGeneratePressed();

public slots:
    void handleResults(const QString &);
    void verifyCheck_beacon01(bool checked);
    void verifyCheck_beacon02(bool checked);
    void verifyCheck_beacon03(bool checked);
    void slotDoIt();
private:

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
	bool readXMLfile();
    bool fillForm();
    bool initialiseDisplayWidget();

    QString meConfigurationFile;

    clSocketToBeacons *meSocketToBeacon_beacon01;
    clSocketToBeacons *meSocketToBeacon_beacon02;
    clSocketToBeacons *meSocketToBeacon_beacon03;

    QTimer* meTimer;

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
    float meTol = 2;

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
        float tol;
    } myDim;

    QTreeWidgetItem * meBeacon_01;
    QTreeWidgetItem * meBeacon_02;
    QTreeWidgetItem * meBeacon_03;
    QTreeWidgetItem * meSensor_01;
    QTreeWidgetItem * meSensor_02;

    //DisplayWidget
    QWidget *widget;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    Qt3DExtras::Qt3DWindow* meViewPort;
    QWidget* meContainer;
    Qt3DCore::QEntity * rootEntity;
    Qt3DInput::QInputAspect *input;
    Qt3DExtras::QOrbitCameraController * camController;
    Qt3DRender::QCamera * camera;
    Qt3DCore::QEntity * connectionEntity;
    //Points
    Qt3DCore::QEntity* createPointEntity(Qt3DCore::QEntity* rootEntity, QString paName, QString paKind, QString paXcoord, QString paYcoord, QString paZcoord, int paId);
    Qt3DCore::QEntity* createPlane(Qt3DCore::QEntity* rootEntity, QString paPlaneImage);
    Qt3DCore::QEntity* createLineEntity(Qt3DCore::QEntity* rootEntity);
    Qt3DCore::QEntity* createLineEntityPoints(Qt3DCore::QEntity* rootEntity, QString paPoint1_X, QString paPoint1_Y, QString paPoint1_Z, QString paPoint2_X, QString paPoint2_Y, QString paPoint2_Z);
    // Sphere
    vector<Qt3DCore::QEntity *> sphereEntity;
    vector<Qt3DExtras::QSphereMesh*> sphereMesh;
    vector<Qt3DCore::QTransform*> transform;
    // Material
    vector<Qt3DExtras::QPhongMaterial*> material;
    //Extra info
    vector<QString> meName;
    vector<QString> meUUID;
    QString meDisplayedView = "";



};
#endif
