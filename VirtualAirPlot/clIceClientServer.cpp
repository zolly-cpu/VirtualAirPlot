#define INFO_BUFFER_SIZE 32767
#include "clIceClientServer.h"


bool clIceClientServer::Ping()
{
    try
    {
        return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::Ping -> " + QString(e.what()));
        return false;
    }
}

void clIceClientServer::Shutdown()
{
    try
    {

    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::Shutdown -> " + QString(e.what()));
    }
}


bool clIceClientServer::insertIntoTableDatabase(   QString paTableName,
                                        vector<std::string>& paColumns,
                                        vector<std::string>& paValue,
                                        vector<std::string>& paTypeValue,
                                        QString &paReturnMessage)
{
    try
    {
        string loReturnMessage;


        /*
        UVServerAppServer::tyStringSequence loColumns;
        UVServerAppServer::tyStringSequence loValue;
        UVServerAppServer::tyStringSequence loTypeValue;


        copy(paColumns.begin(), paColumns.end(), back_inserter(loColumns));
        copy(paValue.begin(), paValue.end(), back_inserter(loValue));
        copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));
        */
        meServerAppServer->insertIntoTableDatabase(string(paTableName.toUtf8()), paColumns, paValue, paTypeValue, loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::insertIntoTableDatabase -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::updateIntoTableDatabase(    QString& paTableName,
                                                    QString& paId,
                                                    vector<std::string>& paColumns,
                                                    vector<std::string>& paValue,
                                                    vector<std::string>& paTypeValue,
                                                    QString &paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->updateIntoTableDatabase(string(paTableName.toUtf8()), string(paId.toUtf8()), paColumns, paValue, paTypeValue, loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::updateIntoTableDatabase -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::deleteIntoTableDatabase( QString& paTableName,
                                      QString& paId,
                                      QString& paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->deleteIntoTableDatabase(string(paTableName.toUtf8()), string(paId.toUtf8()), loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::deleteIntoTableDatabase -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::getFromTableDatabaseGeneral(   QString& paTableName,
                                            QString& paStartValue,
                                            QString& paMaxValue,
                                            vector<std::string>& paReturnId,
                                            vector<std::string>& paReturnName,											
                                            QString& paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->getFromTableDatabaseGeneral(string(paTableName.toUtf8()), string(paStartValue.toUtf8()), string(paMaxValue.toUtf8()), paReturnId, paReturnName, loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::getFromTableDatabaseGeneral -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::getFromTableDatbaseByProperty(     QString& paTableName,
                                                QString& paStartValue,
                                                QString& paMaxValue,
                                                vector<std::string>& paProperties,
                                                vector<std::string>& paValue,
                                                vector<std::string>& paTypeValue,
                                                vector<std::string>& paLogExp,
                                                vector<std::string>& paReturnId,
                                                QString& paReturnMessage)
{
    try
    {
		
		UVServerAppServer::tyStringSequence loProperties;
		UVServerAppServer::tyStringSequence loValue;
		UVServerAppServer::tyStringSequence loTypeValue;
		UVServerAppServer::tyStringSequence loLogExp;

        copy(paProperties.begin(), paProperties.end(), back_inserter(loProperties));
		copy(paValue.begin(), paValue.end(), back_inserter(loValue));
		copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));
		copy(paLogExp.begin(), paLogExp.end(), back_inserter(loLogExp));
		
		
		
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->getFromTableDatbaseByProperty(string(paTableName.toUtf8()), string(paStartValue.toUtf8()), string(paMaxValue.toUtf8()), loProperties, loValue, loTypeValue, loLogExp, paReturnId, loReturnMessage);

        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::getFromTableDatbaseByProperty -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::getFromTableDatabaseById(  QString& paTableName,
                                        QString& paId,
                                        vector<std::string>& paProperties,
                                        vector<std::string>& paReturnValue,
                                        QString& paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());

        UVServerAppServer::tyStringSequence loProperties;

        copy(paProperties.begin(), paProperties.end(), back_inserter(loProperties));


        meServerAppServer->getFromTableDatabaseById(string(paTableName.toUtf8()), string(paId.toUtf8()), loProperties, paReturnValue, loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::getFromTableDatabaseById -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::getAllTablesFromDatabase(  vector<std::string>& paTables,
                                            QString& paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->getAllTablesFromDatabase(paTables, loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::getAllTablesFromDatabase -> " + QString(e.what()));
        return false;
    }
}

bool clIceClientServer::getAllMethodsFromTable(    QString paTableName,
                                            vector<std::string>& paMethodName,
                                            vector<std::string>& paAlias,
                                            vector<std::string>& paSource,
                                            vector<std::string>& paSourceName,
                                            vector<std::string>& paReturnType,
                                            QString& paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->getAllMethodsFromTable(string(paTableName.toUtf8()), paMethodName, paAlias, paSource, paSourceName,paReturnType,loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::getAllMethodsFromTable -> " + QString(e.what()));
        return false;
    }
}
bool clIceClientServer::getAllPropertiesFromTable( QString paTableName,
                                            vector<std::string>& paPropertyName,
                                            vector<std::string>& paAlias,
                                            vector<std::string>& paType,
                                            vector<std::string>& paExtra,
                                            vector<std::string>& paReference,
                                            QString& paReturnMessage)
{
    try
    {
        string loReturnMessage;
        loReturnMessage = std::string(paReturnMessage.toUtf8());
        meServerAppServer->getAllPropertiesFromTable(string(paTableName.toUtf8()), paPropertyName, paAlias, paType, paExtra,paReference,loReturnMessage);
        paReturnMessage = QString(loReturnMessage.c_str());
        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clIceClientServer::getAllPropertiesFromTable -> " + QString(e.what()));
        return false;
    }
}
bool clIceClientServer::giveWorkstationClientInfo(	const std::string& paWorkstationName,
												const std::string& paPercentageMemUsage,
												const std::string& paPercentageDiskUsage,
												const std::string& paPercentageCpuUsage,
												std::string& paReturnMessage)
{
	try
    {
			
		meServerAppServer->giveWorkstationClientInfo(paWorkstationName, paPercentageMemUsage, paPercentageDiskUsage, paPercentageCpuUsage, paReturnMessage);
        return true;
    }
    catch(exception &e)
    {
        paReturnMessage = "clIceClientServer::giveWorkstationClientInfo -> error";
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVLogServer.exe","clIceClientServer::giveWorkstationClientInfo -> " + QString(e.what()));
        return false;
    }											
}
