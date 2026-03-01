#ifndef CLSOCKETTOBEACONS_H
#define CLSOCKETTOBEACONS_H

#include <vector>
#include <iostream>
#include <string>
#include <dlfcn.h>

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>
#include <QtCore/QFile>
#include <QtCore/QtMath>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clSocketToBeaconThread.h"





using namespace std;


class clSocketToBeacons : public QTcpServer
{
    Q_OBJECT
public:
    explicit clSocketToBeacons(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paName, int paPort, QString paIp, int paCommunication_timer, QObject * parent = 0);
    ~clSocketToBeacons();

	void StartServer();

protected:
	void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

private slots:
	void transferData();

private:
	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
public:
	QString convertDataFromSocket(QByteArray paByteArray);
	int meSocketDescriptor;
	QTcpServer *meSocket;
	QByteArray meData;
	bool mePrivilege = false;
	//Data of the sensors
	float meBeacon_lenght_primary;
	float meBeacon_lenght_secondary;
	QString meBeacon_name_primary;
	QString meBeacon_name_secondary;
	//Data of the beacons
	int mePort;
	QString meIp;
	int meCommunication_timer;
	QString meName;
	//Socket information
	clSocketToBeaconThread * meThread;

};
//! [0]

#endif
