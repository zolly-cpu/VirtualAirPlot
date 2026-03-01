#include "clSocketToBeacons.h"

clSocketToBeacons::clSocketToBeacons(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paName, int paPort, QString paIp, int paCommunication_timer, QObject * parent)
{
    meIceClientServer = paIceClientServer;
	meIceClientLogging = paIceClientLogging;
    mePort = paPort;
	meName = paName;
	meIp = paIp;
	meCommunication_timer = paCommunication_timer;
}

clSocketToBeacons::~clSocketToBeacons()
{

}
QString clSocketToBeacons::convertDataFromSocket(QByteArray paByteArray)
{
	char fixed[10000];
	int index = 0;
	QByteArray::iterator iter = paByteArray.begin();
	while(iter != paByteArray.end())
	{
		QChar c = *iter;
		if (c != '\0' && c != '\n') fixed[index++] = c.toLatin1();
		iter++;
	}
	fixed[index] = '\0';
	return QString(fixed);
}

void clSocketToBeacons::StartServer()
{

	//connect (this,SIGNAL (newConnection ()),this,SLOT(newConnections()));


	if(!this->listen(QHostAddress::Any,mePort))
	{
		qDebug() << "Could not start server";
	}
	else
	{
		qDebug() << "Listening...";
	}
}
void clSocketToBeacons::incomingConnection(qintptr socketDescriptor)
{
	meThread = NULL;
	qDebug() << socketDescriptor << " Connecting...";
	meThread = new clSocketToBeaconThread(socketDescriptor, this);
	connect(meThread, SIGNAL(finished()), meThread, SLOT(deleteLater()));
	connect(meThread, SIGNAL(dataReaded()), meThread, SLOT(transferData()));
	meThread->start();
}
void clSocketToBeacons::transferData()
{
	meBeacon_lenght_primary = meThread->beacon_lenght_primary;
	meBeacon_lenght_secondary = meThread->beacon_lenght_secondary;
	meBeacon_name_primary = meThread->beacon_name_primary;
	meBeacon_name_secondary = meThread->beacon_name_secondary;
}




