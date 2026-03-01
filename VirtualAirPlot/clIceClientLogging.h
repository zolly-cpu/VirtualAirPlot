#ifndef _ICE_CLIENT_LOGGING_H
#define _ICE_CLIENT_LOGGING_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>

#include <QtCore/QString>
#include <QtCore/QMutex>

#include <exception>
#include <iostream>
#include <string>

using namespace std;

#include "ICE_LogApplication.h"

class clIceClientLogging
{
public:
    clIceClientLogging (Ice::CommunicatorPtr paCommunicator)
    {
        try
        {
            string loServerRegistration = "LogAppServerAdaptor";
            Ice::ObjectPrx base = paCommunicator->stringToProxy("LogAppServer:tcp -p 5001:udp -p 5001");
            meLogAppServer = UVLogAppServer::LogAppServerPrx::uncheckedCast (base);
			
			
			if (meLogAppServer == NULL)
			{
					printf("clIceClient::clIceClient -> wouter toch");
				
			}
        }
        catch (Ice::Exception &paException)
        {
            cout << paException << endl;
        }
        catch(...)
        {
             printf("clIceClient (Ice::CommunicatorPtr paCommunicator)-> error");
        }
    }
private:
    UVLogAppServer::LogAppServerPrx meLogAppServer;
    IceUtil::Mutex meMutex;
public:
    bool Ping();
    void Shutdown();
    void insertItem(QString paLogNumber,
					QString paComputerName,
                    QString paApplicationName,
                    QString paApplicationText);
};

#endif
