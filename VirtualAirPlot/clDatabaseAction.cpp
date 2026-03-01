#include "clDatabaseAction.h"

clDatabaseAction::clDatabaseAction(QString paName, QString paAlias, QString paSource, QString paSourceName, QString paReturnType)
{
    setName(paName);
    setAlias(paAlias);
    setSource(paSource);
    setSourceName(paSourceName);
    setReturnType(paReturnType);
}

clDatabaseAction::~clDatabaseAction()
{

}

//Setters
void clDatabaseAction::setName(QString paName){ meName= paName;}
void clDatabaseAction::setAlias(QString paAlias){ meAlias = paAlias;}
void clDatabaseAction::setSource(QString paSource){meSource = paSource;}
void clDatabaseAction::setSourceName(QString paSourceName){meSourceName = paSourceName;}
void clDatabaseAction::setReturnType(QString paReturnType){meReturnType = paReturnType;}

//Getters
QString clDatabaseAction::getName(){return meName;}
QString clDatabaseAction::getAlias(){return meAlias;}
QString clDatabaseAction::getSource(){return meSource;}
QString clDatabaseAction::getSourceName(){return meSourceName;}
QString clDatabaseAction::getReturnType(){return meReturnType;}
