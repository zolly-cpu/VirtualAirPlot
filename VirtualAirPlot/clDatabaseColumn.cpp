#include "clDatabaseColumn.h"

clDatabaseColumn::clDatabaseColumn(QString paName, QString paAlias, QString paType, QString paExtra, QString paReference)
{
    setName(paName);
    setAlias(paAlias);
    setType(paType);
    setExtra(paExtra);
    setReference(paReference);
}

clDatabaseColumn::~clDatabaseColumn()
{

}

//Getters
QString clDatabaseColumn::getName(){return meName;}
QString clDatabaseColumn::getAlias(){return meAlias;}
QString clDatabaseColumn::getType(){return meType;}
QString clDatabaseColumn::getExtra(){return meExtra;}
QString clDatabaseColumn::getReference(){return meReference;}
//Setters
void clDatabaseColumn::setName(QString paName){meName = paName;}
void clDatabaseColumn::setAlias(QString paAlias){meAlias = paAlias;}
void clDatabaseColumn::setType(QString paType){meType = paType;}
void clDatabaseColumn::setExtra(QString paExtra){meExtra = paExtra;}
void clDatabaseColumn::setReference(QString paReference){meReference = paReference;}
