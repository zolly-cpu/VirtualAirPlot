#ifndef CLSOCKETTOBEACONTHREAD_H
#define CLSOCKETTOBEACONTHREAD_H

#include <vector>
#include <iostream>
#include <string>
#include <dlfcn.h>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>


#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDataStream>
#include <QtCore/QTextCodec>

using namespace std;

class clSocketToBeaconThread : public QThread
{
    Q_OBJECT;
public:
    explicit clSocketToBeaconThread(int iID ,QObject *parent = 0);
    ~clSocketToBeaconThread ();

	void run();
	QString convertDataFromSocket(QByteArray paByteArray);
signals:
    void error(QTcpSocket::SocketError socketerror);
    void dataReaded();
public slots:
    void readyReadSocket();
    void disconnectedSocket();
private:
    QTcpSocket *meSocket;
    int meSocketDescriptor;
public:
    float beacon_lenght_primary = 0;
    float beacon_lenght_secondary = 0;
    QString beacon_name_primary;
    QString beacon_name_secondary;

};
#endif
