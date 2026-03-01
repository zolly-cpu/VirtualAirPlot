//
// Database Editor
// clMain.cpp 10/03/2011 WVD
//
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "clMainWindow.h"
#include <iostream>
#include <list>


using namespace std;

int main (int paArgc, char * paArgv[])
{
  try
  {
    if (paArgc > 1)
    {
        QApplication loApplication( paArgc, paArgv );
        //CalibratieScherm
        //----------------------------------






        clMainWindow loMainWin(paArgc,paArgv);

        //loMainWin.setCentralWidget(loDatabaseEditorWidget);
        //loMainWin.resize(682,297);
        loMainWin.show();
        return loApplication.exec();
    }
    else
    {
        printf("Not enough parameters supplied to call application ...");
    }
  }
  catch ( ... )
  {
    cout << "Unknown exception" << endl;
  }
  return -1;
}
