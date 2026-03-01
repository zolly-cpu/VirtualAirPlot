#include "clUpdateDatabaseCoord.h"

clUpdateDatabaseCoord::clUpdateDatabaseCoord(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, int paX, int paY, int paZ, QString paTableName, QObject * parent)
{
    meIceClientServer = paIceClientServer;
	meIceClientLogging = paIceClientLogging;
    x = paX;
	y = paY;
	z = paZ;
	meTableName = paTableName;
}

clUpdateDatabaseCoord::~clUpdateDatabaseCoord()
{

}
void clUpdateDatabaseCoord::run()
{
	try
	{
		vector <float> beacon01_lenght;
		vector <float> beacon02_lenght;
		vector <float> beacon03_lenght;

		vector <int> beacon01_x_coord;
		vector <int> beacon01_y_coord;
		vector <int> beacon01_z_coord;

		vector <int> beacon02_x_coord;
		vector <int> beacon02_y_coord;
		vector <int> beacon02_z_coord;

		vector <int> beacon03_x_coord;
		vector <int> beacon03_y_coord;
		vector <int> beacon03_z_coord;

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clUpdateDatabaseCoord::run() -> Performing beacon 1");

		/*
		 * Performing beacon 1
		 * */

		for (int i = 0 ; i < x; i++)
		{
			for (int j = 0 ; j < y; j++)
			{
				for (int k = 0 ; k < z; k++)
				{
					beacon01_x_coord.push_back(i);
					beacon01_y_coord.push_back(j);
					beacon01_z_coord.push_back(k);
					float loTemp = qSqrt((i*i)+(j*j));
					beacon01_lenght.push_back(qSqrt((loTemp * loTemp) + (k*k)));
				}
			}
		}


		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clUpdateDatabaseCoord::run() -> Performing beacon 2");

		/*
		 * Performing beacon 2
		 * */
		for (int j = y - 1 ; j > -1; j--)
		{
			for (int i = 0 ; i < x; i++)
			{
				for (int k = 0 ; k < z; k++)
				{
					beacon02_x_coord.push_back(i);
					beacon02_y_coord.push_back(j);
					beacon02_z_coord.push_back(k);
					float loTemp = qSqrt((i*i)+(j*j));
					beacon02_lenght.push_back(qSqrt((loTemp * loTemp) + (k*k)));
				}
			}
		}

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clUpdateDatabaseCoord::run() -> Performing beacon 3");
		/*
		 * Performing beacon 3
		 * */
		for (int j = 0 ; j < y; j++)
		{
			for (int i = x - 1 ; i > -1; i--)
			{
				for (int k = 0 ; k < z; k++)
				{
					beacon03_x_coord.push_back(i);
					beacon03_y_coord.push_back(j);
					beacon03_z_coord.push_back(k);
					float loTemp = qSqrt((i*i)+(j*j));
					beacon03_lenght.push_back(qSqrt((loTemp * loTemp) + (k*k)));
				}
			}
		}

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clUpdateDatabaseCoord::run() -> Insert in db");


		/*
		 virtual bool insertIntoTableDatabase(   QString paTableName,      *
		 vector<std::string>& paColumns,
		 vector<std::string>& paValue,
		 vector<std::string>& paTypeValue,
		 QString &paReturnMessage);
		 *
		 * */

		vector <std::string> paColumnNames;
		vector <std::string> paTypeValues;

		QString loReturnMessage;

		paColumnNames.push_back("BEACON_01_X_COORD");
		paColumnNames.push_back("BEACON_01_Y_COORD");
		paColumnNames.push_back("BEACON_01_Z_COORD");
		paColumnNames.push_back("BEACON_02_X_COORD");
		paColumnNames.push_back("BEACON_02_Y_COORD");
		paColumnNames.push_back("BEACON_02_Z_COORD");
		paColumnNames.push_back("BEACON_03_X_COORD");
		paColumnNames.push_back("BEACON_03_Y_COORD");
		paColumnNames.push_back("BEACON_03_Z_COORD");
		paColumnNames.push_back("BEACON_01_LENGHT");
		paColumnNames.push_back("BEACON_02_LENGHT");
		paColumnNames.push_back("BEACON_03_LENGHT");


		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("int");
		paTypeValues.push_back("float8");
		paTypeValues.push_back("float8");
		paTypeValues.push_back("float8");

		//At most 4 billion elements

		for (double l = 0 ; l < beacon01_lenght.size(); l++)
		{
			vector <std::string> paValues;

			paValues.push_back(QString::number(beacon01_x_coord.at(l)).toStdString());
			paValues.push_back(QString::number(beacon01_y_coord.at(l)).toStdString());
			paValues.push_back(QString::number(beacon01_z_coord.at(l)).toStdString());

			paValues.push_back(QString::number(beacon02_x_coord.at(l)).toStdString());
			paValues.push_back(QString::number(beacon02_y_coord.at(l)).toStdString());
			paValues.push_back(QString::number(beacon02_z_coord.at(l)).toStdString());

			paValues.push_back(QString::number(beacon03_x_coord.at(l)).toStdString());
			paValues.push_back(QString::number(beacon03_y_coord.at(l)).toStdString());
			paValues.push_back(QString::number(beacon03_z_coord.at(l)).toStdString());

			paValues.push_back(QString::number(beacon01_lenght.at(l)).toStdString());
			paValues.push_back(QString::number(beacon03_lenght.at(l)).toStdString());
			paValues.push_back(QString::number(beacon03_lenght.at(l)).toStdString());

			QString loReturnMessage;

			if(!meIceClientServer->insertIntoTableDatabase(meTableName,paColumnNames,paValues,paTypeValues,loReturnMessage))
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clUpdateDatabaseCoord::run() -> " + loReturnMessage);
		}
		emit resultReady(QString("Database Updated with new values"));
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clUpdateDatabaseCoord::run() -> " + QString(e.what()));
    }
}
