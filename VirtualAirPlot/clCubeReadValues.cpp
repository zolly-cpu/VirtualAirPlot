#include "clCubeReadValues.h"

clCubeReadValues::clCubeReadValues(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
	try
	{
		meIceClientLogging = paIceClientLogging;
		meIceClientServer = paIceClientServer;

		meConfigurationFile = paConfigurationFile;

		meCubeReadValues.setupUi(this);


		readXMLfile();
		fillForm();


		connect(meCubeReadValues.chkBeacon_01, SIGNAL(toggled(bool)),this, SLOT(verifyCheck_beacon01(bool)));
		connect(meCubeReadValues.chkBeacon_02, SIGNAL(toggled(bool)),this, SLOT(verifyCheck_beacon02(bool)));
		connect(meCubeReadValues.chkBeacon_03, SIGNAL(toggled(bool)),this, SLOT(verifyCheck_beacon03(bool)));
		connect(meCubeReadValues.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
		//connect(meCubeInitialise.btnGenerate, SIGNAL(clicked()),this,SLOT(slotButtonGeneratePressed()));




		meSocketToBeacon_beacon01 = NULL;
		meSocketToBeacon_beacon02 = NULL;
		meSocketToBeacon_beacon03 = NULL;



		meTimer = new QTimer(this);
		meTimer->setInterval(1000);
		meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
		meTimer->start();
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::clCubeReadValues() -> Constructor done");
	}
	catch(exception &e)
	{
		cout << e.what();
	}
}

clCubeReadValues::~clCubeReadValues()
{
}
void clCubeReadValues::slotDoIt()
{
	try
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> Perfor check");

		float loLenght_beacon01_sensor01;
		float loLenght_beacon02_sensor01;
		float loLenght_beacon03_sensor01;
		QString loName_sensor01;

		float loLenght_beacon01_sensor02;
		float loLenght_beacon02_sensor02;
		float loLenght_beacon03_sensor02;
		QString loName_sensor02;

		//Get the distance from the socket beacon
		if (meSocketToBeacon_beacon01 != NULL)
		{
			loLenght_beacon01_sensor01 = meSocketToBeacon_beacon01->meBeacon_lenght_primary;
			loLenght_beacon01_sensor02 = meSocketToBeacon_beacon01->meBeacon_lenght_secondary;
			loName_sensor01 = meSocketToBeacon_beacon01->meBeacon_name_primary;
			loName_sensor02 = meSocketToBeacon_beacon01->meBeacon_name_secondary;
		}
		if (meSocketToBeacon_beacon02 != NULL)
		{
			loLenght_beacon02_sensor01 = meSocketToBeacon_beacon02->meBeacon_lenght_primary;
			loLenght_beacon02_sensor02 = meSocketToBeacon_beacon02->meBeacon_lenght_secondary;
			loName_sensor01 = meSocketToBeacon_beacon02->meBeacon_name_primary;
			loName_sensor02 = meSocketToBeacon_beacon02->meBeacon_name_secondary;
		}
		if (meSocketToBeacon_beacon03 != NULL)
		{
			loLenght_beacon03_sensor01 = meSocketToBeacon_beacon03->meBeacon_lenght_primary;
			loLenght_beacon03_sensor02 = meSocketToBeacon_beacon03->meBeacon_lenght_secondary;
			loName_sensor01 = meSocketToBeacon_beacon03->meBeacon_name_primary;
			loName_sensor02 = meSocketToBeacon_beacon03->meBeacon_name_secondary;
		}


		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> Start query");

		if (true)
		{


			vector<std::string> loProperties;
			vector<std::string> loValue;
			vector<std::string> loTypeValue;
			vector<std::string> loLogExp;
			vector<std::string> loReturnId;
			QString loReturnMessage;

			QString loTableName = QString("VIRTUAL_AIR_PLOT_COORD");
			QString loStart = QString("0");
			QString loStop = QString("0");

			loProperties.push_back("BEACON_01_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon01_sensor01 + myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back("<");
			loProperties.push_back("BEACON_01_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon01_sensor01 - myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back(">");

			loProperties.push_back("BEACON_02_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon02_sensor01 + myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back("<");
			loProperties.push_back("BEACON_02_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon02_sensor01 - myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back(">");

			loProperties.push_back("BEACON_03_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon03_sensor01 + myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back("<");
			loProperties.push_back("BEACON_03_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon03_sensor01 - myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back(">");

			if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,loStart,loStop,loProperties,loValue,loTypeValue,loLogExp,loReturnId,loReturnMessage))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + loReturnMessage);
				return;
			}
			else
			{
				if (loReturnId.size() > 0)
				{
					//Get the coordinates
					/*
					 virtual bool getFromTableDatabaseById(  QString& paTableName,     *
					 QString& paId,
					 vector<std::string>& paProperties,
					 vector<std::string>& paReturnValue,
					 QString& paReturnMessage);
					*/
					QString loTableNameTemp = QString("VIRTUAL_AIR_PLOT_COORD");
					QString loUUID = QString(loReturnId.at(0).c_str());
					vector<std::string> loProps;
					vector<std::string> loRetVal;
					QString loRetMes;

					loProps.push_back("BEACON_01_X_COORD");
					loProps.push_back("BEACON_01_Y_COORD");
					loProps.push_back("BEACON_01_Z_COORD");
					loProps.push_back("BEACON_02_X_COORD");
					loProps.push_back("BEACON_02_Y_COORD");
					loProps.push_back("BEACON_02_Z_COORD");
					loProps.push_back("BEACON_03_X_COORD");
					loProps.push_back("BEACON_03_Y_COORD");
					loProps.push_back("BEACON_03_Z_COORD");
					loProps.push_back("BEACON_01_LENGHT");
					loProps.push_back("BEACON_02_LENGHT");
					loProps.push_back("BEACON_03_LENGHT");

					if (!meIceClientServer->getFromTableDatabaseById(loTableNameTemp, loUUID,loProps,loRetVal,loRetMes))
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + loRetMes);
					}
					else
					{
						if (loRetVal.size() > 0)
						{
							meCubeReadValues.ledSensor_01_name->setText(loName_sensor01);
							meCubeReadValues.ledSensor_01_coord->setText(QString("{*1,%2,%3}").arg(QString(loRetVal.at(0).c_str())).arg(QString(loRetVal.at(1).c_str())).arg(QString(loRetVal.at(2).c_str())));
						}
					}
				}
			}


		}
		if(true)
		{
			vector<std::string> loProperties;
			vector<std::string> loValue;
			vector<std::string> loTypeValue;
			vector<std::string> loLogExp;
			vector<std::string> loReturnId;
			QString loReturnMessage;

			QString loTableName = QString("VIRTUAL_AIR_PLOT_COORD");
			QString loStart = QString("0");
			QString loStop = QString("0");

			loProperties.push_back("BEACON_01_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon01_sensor02 + myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back("<");
			loProperties.push_back("BEACON_01_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon01_sensor02 - myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back(">");

			loProperties.push_back("BEACON_02_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon02_sensor02 + myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back("<");
			loProperties.push_back("BEACON_02_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon02_sensor02 - myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back(">");

			loProperties.push_back("BEACON_03_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon03_sensor02 + myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back("<");
			loProperties.push_back("BEACON_03_LENGHT");
			loValue.push_back(QString::number(loLenght_beacon03_sensor02 - myDim.tol).toStdString());
			loTypeValue.push_back("float8");
			loLogExp.push_back(">");

			if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,loStart,loStop,loProperties,loValue,loTypeValue,loLogExp,loReturnId,loReturnMessage))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + loReturnMessage);
				return;
			}
			else
			{
				if (loReturnId.size() > 0)
				{
					//Get the coordinates
					/*
					 *		 virtual bool getFromTableDatabaseById(  QString& paTableName,     *
					 *		 QString& paId,
					 *		 vector<std::string>& paProperties,
					 *		 vector<std::string>& paReturnValue,
					 *		 QString& paReturnMessage);
					 */
					QString loTableNameTemp = QString("VIRTUAL_AIR_PLOT_COORD");
					QString loUUID = QString(loReturnId.at(0).c_str());
					vector<std::string> loProps;
					vector<std::string> loRetVal;
					QString loRetMes;

					loProps.push_back("BEACON_01_X_COORD");
					loProps.push_back("BEACON_01_Y_COORD");
					loProps.push_back("BEACON_01_Z_COORD");
					loProps.push_back("BEACON_02_X_COORD");
					loProps.push_back("BEACON_02_Y_COORD");
					loProps.push_back("BEACON_02_Z_COORD");
					loProps.push_back("BEACON_03_X_COORD");
					loProps.push_back("BEACON_03_Y_COORD");
					loProps.push_back("BEACON_03_Z_COORD");
					loProps.push_back("BEACON_01_LENGHT");
					loProps.push_back("BEACON_02_LENGHT");
					loProps.push_back("BEACON_03_LENGHT");

					if (!meIceClientServer->getFromTableDatabaseById(loTableNameTemp, loUUID,loProps,loRetVal,loRetMes))
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + loRetMes);
					}
					else
					{
						if (loRetVal.size() > 0)
						{
							meCubeReadValues.ledSensor_02_name->setText(loName_sensor02);
							meCubeReadValues.ledSensor_02_coord->setText(QString("{*1,%2,%3}").arg(QString(loRetVal.at(0).c_str())).arg(QString(loRetVal.at(1).c_str())).arg(QString(loRetVal.at(2).c_str())));
						}
					}
				}
			}

		}





	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + QString(e.what()));
	}
}
void clCubeReadValues::slotButtonRefreshPressed()
{
    try
    {
		readXMLfile();
		fillForm();
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}
void clCubeReadValues::slotButtonGeneratePressed()
{
    try
    {
		meCubeReadValues.txtMessage->setText("Database updating wait for finisch ...");

    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotButtonGeneratePressed() -> " + QString(e.what()));
    }
}
void clCubeReadValues::handleResults(const QString &)
{
	try
	{
		meCubeReadValues.txtMessage->setText(QString("Database up to date for the dimensions {%1,%2,%3} this are a %4 records created").arg(myDim.x).arg(myDim.y).arg(myDim.z).arg(myDim.x*myDim.y*myDim.z));
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeInitialise::handleResults() -> " + QString(e.what()));
	}
}
bool clCubeReadValues::readXMLfile()
{
	try
	{
		//padatbaseClass = clDatabaseClass(paFileName);
		QString loFileName(meConfigurationFile);

		QFile loFile(loFileName);
		if ( !loFile.open( QIODevice::ReadOnly ) ) {
			QString loTemp("VirtualAirPlot::readXmlFile() -> Cound not open file '" + loFileName + "' ...");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> " + loTemp);

			return false;
		}

		//QDomDocument
		QDomDocument loDomDocument;
		if ( !loDomDocument.setContent( &loFile ) ) {
			QString loTemp("VirtualAirPlot::readXmlFile() -> Cound not open file '" + loFileName + "' ...");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> " + loTemp);

			loFile.close();
			return false;
		}

		// create the tree view out of the DOM
		QDomElement loDocElem = loDomDocument.documentElement();

		QDomNode loDomNode = loDocElem.firstChild();
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> first element:" + QString(loDomNode.nodeName()));
		while( !loDomNode.isNull() ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> " + QString(loDomNode.nodeName()));
			if(loDomNode.nodeName() == "beacon_01")
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> t");
				QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
				if( !loDocElemChild.isNull() )// the node was really an element.
				{
					QDomNode loDomNodeSub = loDocElemChild.firstChild();
					while ( !loDomNodeSub.isNull() )
					{
						if (loDomNodeSub.nodeName() == "name")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> name");
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
						if (loDomNodeSub.nodeName() == "tol")
						{
							QDomElement loSubDomNode = loDomNodeSub.toElement();
							myDim.tol = QString(loSubDomNode.text()).toFloat();
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::readXMLfile() -> " + loTemp);
		return false;
	}
}
bool clCubeReadValues::fillForm()
{
	try
	{
		meCubeReadValues.tvwObjects->clear();
		QStringList loColumnNames;
		loColumnNames << "name" << "ip" << "port" << "communication_timer" << "gpio_start" << "gpio_stop";
		meCubeReadValues.tvwObjects->setHeaderLabels(loColumnNames);

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


		meCubeReadValues.tvwObjects->addTopLevelItem(meBeacon_01);
		meCubeReadValues.tvwObjects->addTopLevelItem(meBeacon_02);
		meCubeReadValues.tvwObjects->addTopLevelItem(meBeacon_03);
		meCubeReadValues.tvwObjects->addTopLevelItem(meSensor_01);
		meCubeReadValues.tvwObjects->addTopLevelItem(meSensor_02);


		meCubeReadValues.chkBeacon_01->setCheckState(Qt::Unchecked);
		meCubeReadValues.chkBeacon_02->setCheckState(Qt::Unchecked);
		meCubeReadValues.chkBeacon_03->setCheckState(Qt::Unchecked);



		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::fillForm() -> fill UI finisched");

		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::fillForm() -> " + QString(e.what()));
		return false;
	}
}
void clCubeReadValues::verifyCheck_beacon01(bool checked)
{
		try
		{
			if (checked)
			{
				meSocketToBeacon_beacon01 = new clSocketToBeacons(meIceClientServer, meIceClientLogging, myBeacon01.name, myBeacon01.port, myBeacon01.ip, myBeacon01.communication_timer, this);
				meSocketToBeacon_beacon01->StartServer();
				//connect(meWorkstationCycle[i], &clWorkstationCycle::resultReady, this, &clWorkstationCycles::handleResults);
				//connect(meWorkstationCycle[i], &clWorkstationCycle::finished, meWorkstationCycle[i], &QObject::deleteLater);
			}
			else if(!checked)
			{
				delete meSocketToBeacon_beacon01;
				meSocketToBeacon_beacon01 = NULL;
			}
		}
		catch(exception &e)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::fillForm() -> " + QString(e.what()));
		}
}
void clCubeReadValues::verifyCheck_beacon02(bool checked)
{
	try
	{
		if (checked)
		{
			meSocketToBeacon_beacon02 = new clSocketToBeacons(meIceClientServer, meIceClientLogging, myBeacon02.name, myBeacon02.port, myBeacon02.ip, myBeacon02.communication_timer, this);
			meSocketToBeacon_beacon02->StartServer();
			//connect(meWorkstationCycle[i], &clWorkstationCycle::resultReady, this, &clWorkstationCycles::handleResults);
			//connect(meWorkstationCycle[i], &clWorkstationCycle::finished, meWorkstationCycle[i], &QObject::deleteLater);
		}
		else if(!checked)
		{
			delete meSocketToBeacon_beacon02;
			meSocketToBeacon_beacon02 = NULL;
		}
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::fillForm() -> " + QString(e.what()));
	}
}
void clCubeReadValues::verifyCheck_beacon03(bool checked)
{
	try
	{
		if (checked)
		{
			meSocketToBeacon_beacon03 = new clSocketToBeacons(meIceClientServer, meIceClientLogging, myBeacon03.name, myBeacon03.port, myBeacon03.ip, myBeacon03.communication_timer, this);
			meSocketToBeacon_beacon03->StartServer();
			//connect(meWorkstationCycle[i], &clWorkstationCycle::resultReady, this, &clWorkstationCycles::handleResults);
			//connect(meWorkstationCycle[i], &clWorkstationCycle::finished, meWorkstationCycle[i], &QObject::deleteLater);

		}
		else if(!checked)
		{
			delete meSocketToBeacon_beacon03;
			meSocketToBeacon_beacon03 = NULL;
		}
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::fillForm() -> " + QString(e.what()));
	}
}



