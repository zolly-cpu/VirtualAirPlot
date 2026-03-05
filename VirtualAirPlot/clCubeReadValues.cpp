#include "clCubeReadValues.h"

clCubeReadValues::clCubeReadValues(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
	try
	{
		meIceClientLogging = paIceClientLogging;
		meIceClientServer = paIceClientServer;

		meConfigurationFile = paConfigurationFile;

		meCubeReadValues.setupUi(this);

		widget = NULL;

		readXMLfile();
		fillForm();
		initialiseDisplayWidget();


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
bool clCubeReadValues::initialiseDisplayWidget()
{
	try
	{
		if (widget != NULL)
		{
			delete widget;
			widget = NULL;

			material.clear();
			transform.clear();
			sphereMesh.clear();
			sphereEntity.clear();
			meName.clear();
			meUUID.clear();
		}

		// Create a Qt3D window
		meViewPort = new Qt3DExtras::Qt3DWindow();
		meViewPort->defaultFrameGraph()->setClearColor(QColor(QRgb(0xFFFFFFf)));
		//meViewPort->defaultFrameGraph()->setClearColor(QColor(QRgb(0x000000)));
		//meObjectLocator.wdgObjectLocation = QWidget::createWindowContainer(meViewPort);
		meContainer = QWidget::createWindowContainer(meViewPort);
		QSize screenSize = meViewPort->screen()->size();
		meContainer->setMinimumSize(QSize(800, 600));
		meContainer->setMaximumSize(screenSize);


		input = new Qt3DInput::QInputAspect;
		meViewPort->registerAspect(input);


		widget = new QWidget;
		hLayout = new QHBoxLayout(widget);
		vLayout = new QVBoxLayout();
		vLayout->setAlignment(Qt::AlignTop);
		hLayout->addWidget(meContainer, 1);
		hLayout->addLayout(vLayout);

		widget->setWindowTitle(QStringLiteral("Location"));




		// Root entity
		rootEntity = new Qt3DCore::QEntity();

		// Add the line entity
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::showLocatorMapForLocation -> createLineEntity");
		createLineEntity(rootEntity);







		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::showLocatorMapForLocation -> createPointEntity");
		createPointEntity(rootEntity, QString("sensor_01"), QString("10"), QString("500"), QString("500"), QString("500"), 0);
		createPointEntity(rootEntity, QString("sensor_02"), QString("20"), QString("300"), QString("300"), QString("300"), 1);
		createLineEntityPoints(rootEntity,QString("500"), QString("500"), QString("500"),QString("300"), QString("300"), QString("300"));

		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::showLocatorMapForLocation -> createPlane");
		createPlane(rootEntity, QString("./ICONS/10x10.jpg"));

		// Camera setup

		camera = meViewPort->camera();
		camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		camera->setPosition(QVector3D(0, 0, 2));
		//TO REMOVE
		//camera->setUpVector(QVector3D(0, 1, 0));
		camera->setViewCenter(QVector3D(0, 0, 0));

		// Camera controller
		camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
		camController->setLinearSpeed(50.0f);
		camController->setLookSpeed(180.0f);
		camController->setCamera(camera);


		// Set root entity
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::showLocatorMapForLocation -> set root entity");
		meViewPort->setRootEntity(rootEntity);

		// Show the window
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::showLocatorMapForLocation -> show container");

		//meObjectLocator.horizontalLayout_Widgets->addWidget(meContainer,1);


		widget->show();
		widget->resize(800, 600);




		return true;
	}
	catch(exception &e)
	{
		return false;
	}
}

//Diplay an object on the map
Qt3DCore::QEntity* clCubeReadValues::createPointEntity(Qt3DCore::QEntity* rootEntity, QString paName, QString paKind, QString paXcoord, QString paYcoord, QString paZcoord, int paId)
{
	try
	{
		//
		meName.push_back(paName);
		meUUID.push_back(paName);

		// Sphere
		sphereEntity.push_back(new Qt3DCore::QEntity(rootEntity));
		sphereMesh.push_back(new Qt3DExtras::QSphereMesh);
		sphereMesh.at(sphereMesh.size()-1)->setRadius(0.02f);

		transform.push_back(new Qt3DCore::QTransform{sphereMesh.at(sphereMesh.size()-1)});
		//transform->setRotationX(xAngle);
		//transform->setRotationY(yAngle);
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createPointEntity -> setting the coord");
		transform.at(transform.size()-1)->setTranslation(QVector3D(paXcoord.toFloat()/1000, paYcoord.toFloat()/1000,paZcoord.toFloat()/1000));
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createPointEntity -> coord set");
		// Material
		material.push_back(new Qt3DExtras::QPhongMaterial(rootEntity));

		switch (paKind.toInt())
		{
			case 0:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 10:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::green));
				break;
			case 20:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::blue));
				break;
			case 30:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::black));
				break;
			case 40:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::gray));
				break;
			case 50:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::yellow));
				break;
			case 60:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::gray));
				break;
			case 70:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 80:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 90:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 100:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			default:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::white));
		}

		sphereEntity.at(sphereEntity.size()-1)->addComponent(material.at(material.size()-1));
		sphereEntity.at(sphereEntity.size()-1)->addComponent(sphereMesh.at(sphereMesh.size()-1));
		sphereEntity.at(sphereEntity.size()-1)->addComponent(transform.at(transform.size()-1));
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createPointEntity -> returning the sphereEntity");
		return sphereEntity.at(sphereEntity.size()-1);
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createPointEntity -> " + QString(e.what()));
		return NULL;
	}
}

//Display the plane on the map
Qt3DCore::QEntity* clCubeReadValues::createPlane(Qt3DCore::QEntity* rootEntity,QString paPlaneImage) {
	// Plane shape data
	Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
	planeMesh->setWidth(1);
	planeMesh->setHeight(1);

	Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
	planeTransform->setScale(1.0f);
	planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90));
	planeTransform->setTranslation(QVector3D(0.5f, 0.5f, 0.0f));

	auto textureImage = new Qt3DRender::QTextureImage;
	textureImage->setSource(QUrl::fromLocalFile(paPlaneImage));
	auto planeImage = new Qt3DRender::QTexture2D();
	planeImage->addTextureImage(textureImage);

	auto planeMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
	planeMaterial->setDiffuse(QVariant::fromValue(planeImage));
	planeMaterial->setNormal(QVariant::fromValue(planeImage));
	planeMaterial->setSpecular(QVariant::fromValue(planeImage));
	planeMaterial->setShininess(0.1f);

	auto m_planeEntity = new Qt3DCore::QEntity(rootEntity);
	m_planeEntity->addComponent(planeMesh);
	m_planeEntity->addComponent(planeMaterial);
	m_planeEntity->addComponent(planeTransform);


	return m_planeEntity;
}
//Display the axes system
Qt3DCore::QEntity* clCubeReadValues::createLineEntityPoints(Qt3DCore::QEntity* rootEntity, QString paPoint1_X, QString paPoint1_Y, QString paPoint1_Z,QString paPoint2_X, QString paPoint2_Y, QString paPoint2_Z) {

	try
	{
		// Geometry for the line
		auto geometry1 = new Qt3DRender::QGeometry(rootEntity);

		// Vertex data (start and end points of the line)
		QByteArray vertexData1;
		vertexData1.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
		float* positions1 = reinterpret_cast<float*>(vertexData1.data());
		positions1[0] = paPoint1_X.toFloat()/1000; positions1[1] = paPoint1_Y.toFloat()/1000; positions1[2] = paPoint1_Z.toFloat()/1000; // Start point
		positions1[3] = paPoint2_X.toFloat()/1000; positions1[4] = paPoint2_Y.toFloat()/1000; positions1[5] = paPoint2_Z.toFloat()/1000; // End point

		auto vertexBuffer1 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry1);
		vertexBuffer1->setData(vertexData1);

		auto positionAttribute1 = new Qt3DRender::QAttribute();
		positionAttribute1->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
		positionAttribute1->setVertexBaseType(Qt3DRender::QAttribute::Float);
		positionAttribute1->setVertexSize(3);
		positionAttribute1->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
		positionAttribute1->setBuffer(vertexBuffer1);
		positionAttribute1->setByteStride(3 * sizeof(float));
		positionAttribute1->setCount(2);

		geometry1->addAttribute(positionAttribute1);
		// Line renderer
		auto lineRenderer1 = new Qt3DRender::QGeometryRenderer();
		lineRenderer1->setGeometry(geometry1);
		lineRenderer1->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

		// Entity
		connectionEntity = new Qt3DCore::QEntity(rootEntity);
		connectionEntity->addComponent(lineRenderer1);

		// Material
		auto material = new Qt3DExtras::QPhongMaterial(rootEntity);
		material->setDiffuse(QColor(Qt::red));
		connectionEntity->addComponent(material);

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createLineEntityPoints -> OK");

		return connectionEntity;
	}

	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createLineEntityPoints -> " + QString(e.what()));
		return NULL;
	}
}
//Display the axes system
Qt3DCore::QEntity* clCubeReadValues::createLineEntity(Qt3DCore::QEntity* rootEntity) {

	try
	{
		// Geometry for the line
		auto geometry1 = new Qt3DRender::QGeometry(rootEntity);
		auto geometry2 = new Qt3DRender::QGeometry(rootEntity);
		auto geometry3 = new Qt3DRender::QGeometry(rootEntity);

		// Vertex data (start and end points of the line)
		QByteArray vertexData1;
		vertexData1.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
		float* positions1 = reinterpret_cast<float*>(vertexData1.data());
		positions1[0] = 0.0f; positions1[1] = 0.0f; positions1[2] = 0.0f; // Start point
		positions1[3] = 1.0f; positions1[4] = 0.0f; positions1[5] = 0.0f; // End point

		auto vertexBuffer1 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry1);
		vertexBuffer1->setData(vertexData1);

		// Vertex data (start and end points of the line)
		QByteArray vertexData2;
		vertexData2.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
		float* positions2 = reinterpret_cast<float*>(vertexData2.data());
		positions2[0] = 0.0f; positions2[1] = 0.0f; positions2[2] = 0.0f; // Start point
		positions2[3] = 0.0f; positions2[4] = 1.0f; positions2[5] = 0.0f; // End point

		auto vertexBuffer2 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry2);
		vertexBuffer2->setData(vertexData2);


		// Vertex data (start and end points of the line)
		QByteArray vertexData3;
		vertexData3.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
		float* positions3 = reinterpret_cast<float*>(vertexData3.data());
		positions3[0] = 0.0f; positions3[1] = 0.0f; positions3[2] = 0.0f; // Start point
		positions3[3] = 0.0f; positions3[4] = 0.0f; positions3[5] = 1.0f; // End point

		auto vertexBuffer3 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry3);
		vertexBuffer3->setData(vertexData3);







		auto positionAttribute1 = new Qt3DRender::QAttribute();
		positionAttribute1->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
		positionAttribute1->setVertexBaseType(Qt3DRender::QAttribute::Float);
		positionAttribute1->setVertexSize(3);
		positionAttribute1->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
		positionAttribute1->setBuffer(vertexBuffer1);
		positionAttribute1->setByteStride(3 * sizeof(float));
		positionAttribute1->setCount(2);

		auto positionAttribute2 = new Qt3DRender::QAttribute();
		positionAttribute2->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
		positionAttribute2->setVertexBaseType(Qt3DRender::QAttribute::Float);
		positionAttribute2->setVertexSize(3);
		positionAttribute2->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
		positionAttribute2->setBuffer(vertexBuffer2);
		positionAttribute2->setByteStride(3 * sizeof(float));
		positionAttribute2->setCount(2);

		auto positionAttribute3 = new Qt3DRender::QAttribute();
		positionAttribute3->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
		positionAttribute3->setVertexBaseType(Qt3DRender::QAttribute::Float);
		positionAttribute3->setVertexSize(3);
		positionAttribute3->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
		positionAttribute3->setBuffer(vertexBuffer3);
		positionAttribute3->setByteStride(3 * sizeof(float));
		positionAttribute3->setCount(2);


		geometry1->addAttribute(positionAttribute1);
		geometry2->addAttribute(positionAttribute2);
		geometry3->addAttribute(positionAttribute3);

		// Line renderer
		auto lineRenderer1 = new Qt3DRender::QGeometryRenderer();
		lineRenderer1->setGeometry(geometry1);
		lineRenderer1->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

		// Line renderer
		auto lineRenderer2 = new Qt3DRender::QGeometryRenderer();
		lineRenderer2->setGeometry(geometry2);
		lineRenderer2->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

		// Line renderer
		auto lineRenderer3 = new Qt3DRender::QGeometryRenderer();
		lineRenderer3->setGeometry(geometry3);
		lineRenderer3->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

		// Entity
		auto lineEntity1 = new Qt3DCore::QEntity(rootEntity);
		lineEntity1->addComponent(lineRenderer1);
		auto lineEntity2 = new Qt3DCore::QEntity(rootEntity);
		lineEntity2->addComponent(lineRenderer2);
		auto lineEntity3 = new Qt3DCore::QEntity(rootEntity);
		lineEntity3->addComponent(lineRenderer3);

		// Material
		auto material = new Qt3DExtras::QPhongMaterial(rootEntity);
		material->setDiffuse(QColor(Qt::red));
		lineEntity1->addComponent(material);
		lineEntity2->addComponent(material);
		lineEntity3->addComponent(material);

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createLineEntity -> OK");

		return lineEntity1;
	}

	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::createLineEntity -> " + QString(e.what()));
		return NULL;
	}
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



		bool loSensor01_readed = false;
		bool loSensor02_readed = false;
		QString loSensor01_X;
		QString loSensor01_Y;
		QString loSensor01_Z;
		QString loSensor02_X;
		QString loSensor02_Y;
		QString loSensor02_Z;




		if (true)
		{

			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> Propertie size:" + QString("Lenght [%1,%2,%3]").arg(QString::number(loLenght_beacon01_sensor01)).arg(QString::number(loLenght_beacon02_sensor01)).arg(QString::number(loLenght_beacon03_sensor01)));
			vector<std::string> loProperties;
			vector<std::string> loValue;
			vector<std::string> loTypeValue;
			vector<std::string> loLogExp;
			vector<std::string> loPropertiesReturn;
			vector<std::string> loValuesReturn;
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

			if (!meIceClientServer->getFromTableDatbaseByPropertyRepresentProp(loTableName,loStart,loStop,loProperties,loValue,loTypeValue,loLogExp,loPropertiesReturn,loValuesReturn,loReturnMessage))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + loReturnMessage);
				return;
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> Propertie size:" + QString::number(loPropertiesReturn.size()));
				if (loPropertiesReturn.size() > 0)
				{
							meCubeReadValues.ledSensor_01_name->setText(loName_sensor01);
							meCubeReadValues.ledSensor_01_coord->setText(QString("{%1,%2,%3}").arg(QString(loValuesReturn.at(0).c_str())).arg(QString(loValuesReturn.at(1).c_str())).arg(QString(loValuesReturn.at(2).c_str())));
							transform.at(0)->setTranslation(QVector3D(QString(loValuesReturn.at(0).c_str()).toFloat()/1000,QString(loValuesReturn.at(1).c_str()).toFloat()/1000,QString(loValuesReturn.at(2).c_str()).toFloat()/1000));
							loSensor01_readed = true;
							loSensor01_X = QString(loValuesReturn.at(0).c_str());
							loSensor01_Y = QString(loValuesReturn.at(1).c_str());
							loSensor01_Y = QString(loValuesReturn.at(2).c_str());
				}
			}


		}
		if(true)
		{
			vector<std::string> loProperties;
			vector<std::string> loValue;
			vector<std::string> loTypeValue;
			vector<std::string> loLogExp;
			vector<std::string> loPropertiesReturn;
			vector<std::string> loValuesReturn;
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

			if (!meIceClientServer->getFromTableDatbaseByPropertyRepresentProp(loTableName,loStart,loStop,loProperties,loValue,loTypeValue,loLogExp,loPropertiesReturn,loValuesReturn,loReturnMessage))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> " + loReturnMessage);
				return;
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"VirtualAirPlot","clCubeReadValues::slotDoIt() -> Propertie size:" + QString::number(loPropertiesReturn.size()));
				if (loPropertiesReturn.size() > 0)
				{
					meCubeReadValues.ledSensor_02_name->setText(loName_sensor02);
					meCubeReadValues.ledSensor_02_coord->setText(QString("{%1,%2,%3}").arg(QString(loValuesReturn.at(0).c_str())).arg(QString(loValuesReturn.at(1).c_str())).arg(QString(loValuesReturn.at(2).c_str())));
					transform.at(1)->setTranslation(QVector3D(QString(loValuesReturn.at(0).c_str()).toFloat()/1000,QString(loValuesReturn.at(1).c_str()).toFloat()/1000,QString(loValuesReturn.at(2).c_str()).toFloat()/1000));
					loSensor02_readed = true;
					loSensor02_X = QString(loValuesReturn.at(0).c_str());
					loSensor02_Y = QString(loValuesReturn.at(1).c_str());
					loSensor02_Y = QString(loValuesReturn.at(2).c_str());
				}
			}

		}
		if (loSensor01_readed && loSensor02_readed)
		{

			if (connectionEntity != NULL)
			{
				delete connectionEntity;
				connectionEntity = NULL;
				createLineEntityPoints(rootEntity,loSensor01_X, loSensor01_Y, loSensor01_Z,loSensor02_X, loSensor02_Y, loSensor02_Z);
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



