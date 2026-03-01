#include "clSocketToBeaconThread.h"

clSocketToBeaconThread::clSocketToBeaconThread(int paID, QObject * parent) : QThread(parent)
{
    try
    {		
		this->meSocketDescriptor = paID;


		meSocket = new QTcpSocket(this);
		connect(meSocket, SIGNAL(readyRead()), this, SLOT(readyReadSocket()));
		connect(meSocket, SIGNAL(disconnected()), this, SLOT(disconnectedSocket()));
		
		qDebug() << meSocketDescriptor << " Starting thread";

		if(!meSocket->setSocketDescriptor(this->meSocketDescriptor))
		{
			emit error(meSocket->error());
			return;
		}
		qDebug() << meSocketDescriptor << " Client connected";
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clSocketBoxThread::clLogServer(QString paConfigFile) -> error ...");
    }
}

clSocketToBeaconThread::~clSocketToBeaconThread ()
{
	
}


void clSocketToBeaconThread::run()
{
	

    // make this thread a loop
	//while (meSocket->isOpen())
	//{
		
	//}
    exec();
}

void clSocketToBeaconThread::readyReadSocket()
{

		try
		{
			cout << "clSocketToBeaconThread::readyReadSocket->START" << endl;
			QByteArray loData = meSocket->readAll();

			qDebug() << meSocketDescriptor << " Data in: " << endl << convertDataFromSocket(loData) << endl;

			QDomDocument loDomDocument;
			if ( !loDomDocument.setContent( loData ) )
			{
				return;
			}

			QDomElement loDocElem = loDomDocument.documentElement();
			QDomNode loTablesNode = loDocElem.firstChild();

			//example of xml communication
			//<root><sensor_01 name="" lenght=""><sensor_02 name="" lenght="">
			while( !loTablesNode.isNull() )
			{
				if(loTablesNode.nodeName() == "sensor_01")
				{
					QDomElement loDocElemChild = loTablesNode.toElement(); // try to convert the node to an element.
					if( !loDocElemChild.isNull() )// the node was really an element.
					{
						if (loDocElemChild.hasAttribute(QString("name")))
						{
							//QString loValue = loDocElemChild.attribute(paAttribute);
							//paValue = &loValue;
							beacon_name_primary = loDocElemChild.attribute("name");
						}
						if (loDocElemChild.hasAttribute(QString("lenght")))
						{
							//QString loValue = loDocElemChild.attribute(paAttribute);
							//paValue = &loValue;
							beacon_lenght_primary = loDocElemChild.attribute("lenght").toFloat();
						}
					}
				}
				if(loTablesNode.nodeName() == "sensor_02")
				{
					QDomElement loDocElemChild = loTablesNode.toElement(); // try to convert the node to an element.
					if( !loDocElemChild.isNull() )// the node was really an element.
					{
						if (loDocElemChild.hasAttribute(QString("name")))
						{
							//QString loValue = loDocElemChild.attribute(paAttribute);
							//paValue = &loValue;
							beacon_name_secondary = loDocElemChild.attribute("name");
						}
						if (loDocElemChild.hasAttribute(QString("lenght")))
						{
							//QString loValue = loDocElemChild.attribute(paAttribute);
							//paValue = &loValue;
							beacon_lenght_secondary = loDocElemChild.attribute("lenght").toFloat();
						}
					}
				}
				loTablesNode = loTablesNode.nextSibling();
			}

			emit dataReaded ();
			/*
			meBeacon_lenght_primary = &beacon_lenght_primary;
			meBeacon_lenght_secondary = &beacon_lenght_secondary;
			meBeacon_name_primary = &beacon_name_primary;
			meBeacon_name_secondary = &beacon_name_secondary;
			*/


		}
		catch(...)
		{
			printf("clSocketBoxThread::readyReadSocket() -> error ...");
		}

}
QString clSocketToBeaconThread::convertDataFromSocket(QByteArray paByteArray)
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
void clSocketToBeaconThread::disconnectedSocket()
{
    qDebug() << meSocketDescriptor << " Disconnected";
    meSocket->deleteLater();
    exit(0);
}

