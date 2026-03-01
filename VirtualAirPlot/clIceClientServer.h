#ifndef _ICE_CLIENT_SERVER_H
#define _ICE_CLIENT_SERVER_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>

#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtNetwork/QHostInfo>

#include <exception>
#include <iostream>
#include <string>

using namespace std;

#include "ICE_ServerApplication.h"
#include "clIceClientLogging.h"

class clIceClientServer
{
public:
    clIceClientServer (Ice::CommunicatorPtr paCommunicator, clIceClientLogging *paIceClientLogging) : meIceClientLogging(paIceClientLogging)
    {
        try
        {
            string loServerRegistration = "ServerAppServerAdaptor";
            Ice::ObjectPrx loBaseProxy = paCommunicator->propertyToProxy ("ServerAppServerAdaptor.Proxy");
            meServerAppServer = UVServerAppServer::ServerAppServerPrx::uncheckedCast (loBaseProxy);
        }
        catch (Ice::Exception &paException)
        {
            cout << paException << endl;
        }
        catch(...)
        {
             printf("clIceClientServer(Ice::CommunicatorPtr paCommunicator)-> error");
        }
    }
private:
    UVServerAppServer::ServerAppServerPrx meServerAppServer;
    IceUtil::Mutex meMutex;
public:
    virtual bool Ping();

    virtual void Shutdown();

    virtual bool insertIntoTableDatabase(   QString paTableName,
                                            vector<std::string>& paColumns,
                                            vector<std::string>& paValue,
                                            vector<std::string>& paTypeValue,
                                            QString &paReturnMessage);

    virtual bool updateIntoTableDatabase(   QString& paTableName,
                                            QString& paId,
                                            vector<std::string>& paColumns,
                                            vector<std::string>& paValue,
                                            vector<std::string>& paTypeValue,
                                            QString &paReturnMessage);

    virtual bool deleteIntoTableDatabase( QString& paTableName,
                                          QString& paId,
                                          QString& paReturnMessage);


      bool getFromTableDatabaseGeneral( QString& paTableName,
                                        QString& paStartValue,
                                        QString& paMaxValue,
                                        vector<std::string>& paReturnId,
                                        vector<std::string>& paReturnName,										
                                        QString& paReturnMessage);

    virtual bool getFromTableDatbaseByProperty(     QString& paTableName,
                                                    QString& paStartValue,
                                                    QString& paMaxValue,
                                                    vector<std::string>& paProperties,
                                                    vector<std::string>& paValue,
                                                    vector<std::string>& paTypeValue,
                                                    vector<std::string>& paLogExp,
                                                    vector<std::string>& paReturnId,
                                                    QString& paReturnMessage);

    virtual bool getFromTableDatabaseById(  QString& paTableName,
                                            QString& paId,
                                            vector<std::string>& paProperties,
                                            vector<std::string>& paReturnValue,
                                            QString& paReturnMessage);


    virtual bool getAllTablesFromDatabase(  vector<std::string>& paTables,
                                            QString& paReturnMessage);

    virtual bool getAllMethodsFromTable(    QString paTable,
                                            vector<std::string>& paMethodName,
                                            vector<std::string>& paAlias,
                                            vector<std::string>& paSource,
                                            vector<std::string>& paSourceName,
                                            vector<std::string>& paReturnType,
                                            QString& paReturnMessage);

    virtual bool getAllPropertiesFromTable( QString paTable,
                                            vector<std::string>& paPropertyName,
                                            vector<std::string>& paAlias,
                                            vector<std::string>& paType,
                                            vector<std::string>& paExtra,
                                            vector<std::string>& paReference,
                                            QString& paReturnMessage);
											
	virtual bool giveWorkstationClientInfo(	const std::string& paWorkstationName,
											const std::string& paPercentageMemUsage,
											const std::string& paPercentageDiskUsage,
											const std::string& paPercentageCpuUsage,
											std::string& paReturnMessage);
											
private:
    clIceClientLogging *meIceClientLogging;
};

#endif
