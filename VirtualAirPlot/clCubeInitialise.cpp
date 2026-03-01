#include "clCubeInitialise.h"

clCubeInitialise::clCubeInitialise(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meConfigurationFile = paConfigurationFile;

	meCubeInitialise.setupUi(this);
    connect(meCubeInitialise.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
    connect(meCubeInitialise.btnGenerate, SIGNAL(clicked()),this,SLOT(slotButtonGeneratePressed()));
	
	readXMLfile();
	fillForm();


}

clCubeInitialise::~clCubeInitialise()
{
}
void clCubeInitialise::slotButtonRefreshPressed()
{
    try
    {
		readXMLfile();
		fillForm();
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clParameterView::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}
void clCubeInitialise::slotButtonGeneratePressed()
{
    try
    {
		meCubeInitialise.txtMessage->setText("Database updating wait for finisch ...");

		//Starting the threads//
		meUpdateDatabaseCoord = new clUpdateDatabaseCoord(meIceClientServer, meIceClientLogging, myDim.x, myDim.y, myDim.z, QString("VIRTUAL_AIR_PLOT_COORD"), this);
		connect(meUpdateDatabaseCoord, &clUpdateDatabaseCoord::resultReady, this, &clCubeInitialise::handleResults);
		//connect(meWorkstationCycle[i], &clWorkstationCycle::finished, meWorkstationCycle[i], &QObject::deleteLater);
		meUpdateDatabaseCoord->start();
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::slotButtonGeneratePressed() -> " + QString(e.what()));
    }
}
void clCubeInitialise::handleResults(const QString &)
{
	try
	{
		meCubeInitialise.txtMessage->setText(QString("Database up to date for the dimensions {%1,%2,%3} this are a %4 records created").arg(myDim.x).arg(myDim.y).arg(myDim.z).arg(myDim.x*myDim.y*myDim.z));
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::handleResults() -> " + QString(e.what()));
	}
}
bool clCubeInitialise::readXMLfile()
{
	try
	{
		//padatbaseClass = clDatabaseClass(paFileName);
		QString loFileName(meConfigurationFile);

		QFile loFile(loFileName);
		if ( !loFile.open( QIODevice::ReadOnly ) ) {
			QString loTemp("VirtualAirPlot::readXmlFile() -> Cound not open file '" + loFileName + "' ...");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> " + loTemp);

			return false;
		}

		//QDomDocument
		QDomDocument loDomDocument;
		if ( !loDomDocument.setContent( &loFile ) ) {
			QString loTemp("VirtualAirPlot::readXmlFile() -> Cound not open file '" + loFileName + "' ...");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> " + loTemp);

			loFile.close();
			return false;
		}

		// create the tree view out of the DOM
		QDomElement loDocElem = loDomDocument.documentElement();

		QDomNode loDomNode = loDocElem.firstChild();
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> first element:" + QString(loDomNode.nodeName()));
		while( !loDomNode.isNull() ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> " + QString(loDomNode.nodeName()));
			if(loDomNode.nodeName() == "beacon_01")
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> t");
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "name")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> name");
							myBeacon01.name = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "ip")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.ip = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "port")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.port = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "factor")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.factor = QString(loSubDomNode.text()).toFloat();
						}
						if (loDomNodeSub.nodeName() == "communication_timer")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.communication_timer = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "adress")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.address = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "x")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.x = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "y")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon01.y = QString(loSubDomNode.text()).toInt();
						}
						loDomNodeSub = loDomNodeSub.nextSibling();
					}
				}
			}
			else if(loDomNode.nodeName() == "beacon_02")
			{
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "name")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.name = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "ip")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.ip = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "port")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.port = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "factor")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.factor = QString(loSubDomNode.text()).toFloat();
						}
						if (loDomNodeSub.nodeName() == "communication_timer")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.communication_timer = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "adress")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.address = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "x")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.x = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "y")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon02.y = QString(loSubDomNode.text()).toInt();
						}
						loDomNodeSub = loDomNodeSub.nextSibling();
					}
				}
			}
			else if(loDomNode.nodeName() == "beacon_03")
			{
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "name")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.name = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "ip")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.ip = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "port")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.port = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "factor")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.factor = QString(loSubDomNode.text()).toFloat();
						}
						if (loDomNodeSub.nodeName() == "communication_timer")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.communication_timer = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "adress")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.address = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "x")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.x = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "y")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myBeacon03.y = QString(loSubDomNode.text()).toInt();
						}
						loDomNodeSub = loDomNodeSub.nextSibling();
					}
				}
			}
			else if(loDomNode.nodeName() == "sensor_01")
			{
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "name")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor01.name = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "factor")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor01.factor = QString(loSubDomNode.text()).toFloat();
						}
						if (loDomNodeSub.nodeName() == "communication_timer")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor01.communication_timer = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "gpio_start")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor01.gpio = QString(loSubDomNode.text()).toInt();
						}
						loDomNodeSub = loDomNodeSub.nextSibling();
					}
				}
			}
			else if(loDomNode.nodeName() == "sensor_02")
			{
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "name")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor02.name = QString(loSubDomNode.text());
						}
						if (loDomNodeSub.nodeName() == "factor")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor02.factor = QString(loSubDomNode.text()).toFloat();
						}
						if (loDomNodeSub.nodeName() == "communication_timer")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor02.communication_timer = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "gpio_stop")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							mySensor02.gpio = QString(loSubDomNode.text()).toInt();
						}
						loDomNodeSub = loDomNodeSub.nextSibling();
					}
				}
			}
			else if(loDomNode.nodeName() == "dim")
			{
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "x")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myDim.x = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "y")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myDim.y = QString(loSubDomNode.text()).toInt();
						}
						if (loDomNodeSub.nodeName() == "z")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myDim.z = QString(loSubDomNode.text()).toInt();
						}
						loDomNodeSub = loDomNodeSub.nextSibling();
					}
				}
			}
			loDomNode = loDomNode.nextSibling();
		}
		return true;
	}
	catch(...)
	{
		QString loTemp("VirtualAirPlot::readXmlFile() -> error ...");
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::readXMLfile() -> " + loTemp);
		return false;
	}
}
bool clCubeInitialise::fillForm()
{
	try
	{
		meCubeInitialise.tvwObjects->clear();
		QStringList loColumnNames;
		loColumnNames << "name" << "ip" << "port" << "communication_timer" << "gpio_start" << "gpio_stop";
		meCubeInitialise.tvwObjects->setHeaderLabels(loColumnNames);

		meBeacon_01 = new QTreeWidgetItem;
		meBeacon_02 = new QTreeWidgetItem;
		meBeacon_03 = new QTreeWidgetItem;
		meSensor_01 = new QTreeWidgetItem;
		meSensor_02 = new QTreeWidgetItem;

		const QIcon Icon_b01("./ICONS/beacon.png");
		const QIcon Icon_b02("./ICONS/beacon.png");
		const QIcon Icon_b03("./ICONS/beacon.png");
		const QIcon Icon_s01("./ICONS/sensor.png");
		const QIcon Icon_s02("./ICONS/sensor.png");

		meBeacon_01->setIcon(0,Icon_b01);
		meBeacon_02->setIcon(0,Icon_b02);
		meBeacon_03->setIcon(0,Icon_b03);
		meSensor_01->setIcon(0,Icon_s01);
		meSensor_02->setIcon(0,Icon_s02);

		meBeacon_01->setText(0,QString(myBeacon01.name));
		meBeacon_02->setText(0,QString(myBeacon02.name));
		meBeacon_03->setText(0,QString(myBeacon03.name));
		meSensor_01->setText(0,QString(mySensor01.name));
		meSensor_02->setText(0,QString(mySensor02.name));

		meBeacon_01->setText(1,QString(myBeacon01.ip));
		meBeacon_02->setText(1,QString(myBeacon02.ip));
		meBeacon_03->setText(1,QString(myBeacon03.ip));

		meBeacon_01->setText(2,QString::number(myBeacon01.port));
		meBeacon_02->setText(2,QString::number(myBeacon02.port));
		meBeacon_03->setText(2,QString::number(myBeacon03.port));

		meBeacon_01->setText(3,QString::number(myBeacon01.communication_timer));
		meBeacon_02->setText(3,QString::number(myBeacon02.communication_timer));
		meBeacon_03->setText(3,QString::number(myBeacon03.communication_timer));
		meSensor_01->setText(3,QString::number(mySensor01.communication_timer));
		meSensor_02->setText(3,QString::number(mySensor02.communication_timer));


		meSensor_01->setText(4,QString::number(mySensor01.gpio));
		meSensor_02->setText(5,QString::number(mySensor02.gpio));


		meCubeInitialise.tvwObjects->addTopLevelItem(meBeacon_01);
		meCubeInitialise.tvwObjects->addTopLevelItem(meBeacon_02);
		meCubeInitialise.tvwObjects->addTopLevelItem(meBeacon_03);
		meCubeInitialise.tvwObjects->addTopLevelItem(meSensor_01);
		meCubeInitialise.tvwObjects->addTopLevelItem(meSensor_02);

		meCubeInitialise.spbX_1->setRange(0,100000000);
		meCubeInitialise.spbX_1->setValue(myBeacon01.x);
		meCubeInitialise.spbX_2->setRange(0,100000000);
		meCubeInitialise.spbX_2->setValue(myBeacon02.x);
		meCubeInitialise.spbX_3->setRange(0,100000000);
		meCubeInitialise.spbX_3->setValue(myBeacon03.x);

		meCubeInitialise.spbY_1->setRange(0,100000000);
		meCubeInitialise.spbY_1->setValue(myBeacon01.y);
		meCubeInitialise.spbY_2->setRange(0,100000000);
		meCubeInitialise.spbY_2->setValue(myBeacon02.y);
		meCubeInitialise.spbY_3->setRange(0,100000000);
		meCubeInitialise.spbY_3->setValue(myBeacon03.y);

		meCubeInitialise.spbSteps->setRange(0,100000000);
		meCubeInitialise.spbSteps->setValue(1);

		meCubeInitialise.lblDimensions->setText(QString("{%1,%2,%3}").arg(myDim.x).arg(myDim.y).arg(myDim.z));

		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::fillForm() -> " + QString(e.what()));
		return false;
	}
}





