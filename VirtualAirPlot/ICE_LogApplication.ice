#ifndef _LOGAPPLICATION_INTERFACE
#define _LOGAPPLICATION_INTERFACE

module UVLogAppServer {

    interface LogAppServer
    {
      bool Ping ();
      void Shutdown ();
      void insertItem(string paLogNumber, string paComputerName, string paApplicationName, string paApplicationText);
    };
};

#endif
