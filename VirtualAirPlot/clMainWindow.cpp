#include "clMainWindow.h"
#define INFO_BUFFER_SIZE 32767
//Constructors
clMainWindow::clMainWindow(int paArgc, char * paArgv[], QWidget* paParent, const char* paName)
{
    try
    {
        initializeLogClient(paArgc,paArgv);

        initializeServerClient(paArgc,paArgv);
		
        meConfigurationFile = QString(paArgv[1]);
		
        meWorkspace = new QMdiArea;
        setCentralWidget(meWorkspace);
        createActions();
        createMenus();
		
    }
    catch(...)
    {
    //connect(workspace, SIGNAL(windowActivated(QWidget *)),
    //        this, SLOT(updateMenus()));
    //createActions();
    //createMenus();
    //createToolBars();
    //createStatusBar();
    //setWindowTitle(tr("MDI Editor"));
    //setWindowIcon(QPixmap(":/images/icon.png"));
    }
}
//Destructors
clMainWindow::~clMainWindow()
{
    //Clean up the memory
    //delete meDatabaseClass;
    //delete meXMLReader;
}
/*****************************
* ICE FUNCTIONS
*******************************/
void clMainWindow::initializeLogClient(int paArgc, char * paArgv[])
{
    try
    {
		/*
		int loArgc = 2;
		char * loArgv[1];
		loArgv[0] = paArgv[0];
		loArgv[1] = paArgv[1];
		*/
        Ice::InitializationData initData2;
        initData2.properties = Ice::createProperties();
        initData2.properties->load("config.client.LogServer");
        meIceCommunicatorLogging = Ice::initialize(paArgc, paArgv, initData2);
        meIceClientLogging = new clIceClientLogging(meIceCommunicatorLogging);
    }
    catch(...)
    {
        cout << "Logging not initialized" << endl;
    }
}

void clMainWindow::initializeServerClient(int paArgc, char * paArgv[])
{
    try
    {
		/*
		int loArgc = 2;
		char * loArgv[1];
		loArgv[0] = paArgv[0];
		loArgv[1] = paArgv[2];
		*/
		
        Ice::InitializationData initData2;
        initData2.properties = Ice::createProperties();
        initData2.properties->load("config.client.Server");
        meIceCommunicatorServer = Ice::initialize(paArgc, paArgv, initData2);
        printf("main() -> Server Communicator inistialized ");

        meIceClientServer = new clIceClientServer(meIceCommunicatorServer,meIceClientLogging);
    }
    catch(...)
    {
        cout << "Logging not initialized" << endl;
    }
}
/*****************************
* UI controls
*******************************/
//Create the actions
void clMainWindow::createActions()
{
    meExitApplication = new QAction(tr("&Exit"), this);
    meExitApplication->setStatusTip(tr("Exit"));
    connect(meExitApplication, SIGNAL(triggered()), this, SLOT(exitApplication()));

    meSaveAss = new QAction(tr("&SaveAs"), this);
    meSaveAss->setStatusTip(tr("Save ass ..."));
    //connect(meSaveAss, SIGNAL(triggered()), this, SLOT(saveLogging()));

    meConfigProduct = new QAction(tr("&Configure"), this);
    meConfigProduct->setStatusTip(tr("Config product"));
    connect(meConfigProduct, SIGNAL(triggered()), this, SLOT(startProductConfig()));

    meReadValues = new QAction(tr("&Read"), this);
    meReadValues->setStatusTip(tr("Read values"));
    connect(meReadValues, SIGNAL(triggered()), this, SLOT(readProductValues()));

}
//Creating the menu
void clMainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    productMenu = menuBar()->addMenu(tr("&Product"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    fileMenu->addAction(meExitApplication);
    fileMenu->addAction(meSaveAss);
    productMenu->addAction(meConfigProduct);
    productMenu->addAction(meReadValues);
}
/***********************************
* UI slots
***************************************/
//Exit the application
void clMainWindow::exitApplication()
{
        try
        {
            this->close();
        }
        catch(exception &e)
        {
			
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clMainWindow::exitApplication()" + QString(e.what()));
        }
}
//Start config product
void clMainWindow::startProductConfig()
{
        try
        {
            meCubeInitialise = new clCubeInitialise(meIceClientServer,meIceClientLogging,meConfigurationFile);
            meWorkspace->addSubWindow(meCubeInitialise);
            meCubeInitialise->show();
        }
        catch(exception &e)
        {

			
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clMainWindow::startProductConfig()" + QString(e.what()));
        }
}
void clMainWindow::readProductValues()
{
    try
    {
        meCubeReadValues = new clCubeReadValues(meIceClientServer,meIceClientLogging,meConfigurationFile);
        meWorkspace->addSubWindow(meCubeReadValues);
        meCubeReadValues->show();
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clMainWindow::readProductValues()" + QString(e.what()));
    }
}
