#ifndef CLUPDATEDATABASECOORD_H
#define CLUPDATEDATABASECOORD_H

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






using namespace std;


class clUpdateDatabaseCoord : public QThread
{
    Q_OBJECT
public:
    clUpdateDatabaseCoord(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, int paX, int paY, int paZ, QString paTableName, QObject * parent = 0);
    ~clUpdateDatabaseCoord();

protected:
    void run() override;
signals:
    void resultReady(const QString &s);
private:

	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
	
public:
	int x;
	int y;
	int z;
	QString meTableName;
};
//! [0]

#endif
