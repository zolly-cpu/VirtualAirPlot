#define INFO_BUFFER_SIZE 32767
#include "clIceClientLogging.h"

bool clIceClientLogging::Ping()
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
        return meLogAppServer->Ping();
    }
    catch(exception& e)
    {
        printf("bool clIceClientLogging::Ping() -> error ...");
        cout << e.what() << endl;
        return false;
    }
}
void clIceClientLogging::Shutdown()
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
        meLogAppServer->Shutdown();
    }
    catch(...)
    {
        printf("bool clIceClientLogging::Shutdown() -> error ...");
    }
}
void clIceClientLogging::insertItem(   	QString paLogNumber,
										QString paComputerName,
										QString paApplicationName,
										QString paApplicationText)
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);		
        meLogAppServer->insertItem(std::string(paLogNumber.toStdString()), std::string(paComputerName.toStdString()),std::string(paApplicationName.toStdString ()),std::string(paApplicationText.toStdString()));
    }
    catch(...)
    {
        printf("bool clIceClientLogging::insertItem(...) -> error ...");
    }
}
