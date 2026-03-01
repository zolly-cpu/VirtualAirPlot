#ifndef CLDATABASEACTION_H_WVD_17052011
#define CLDATABASEACTION_H_WVD_17052011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>


using namespace std;


class clDatabaseAction
{

public:
    clDatabaseAction (QString paName, QString paAlias, QString paSource, QString paSourceName, QString paReturnType);
    ~clDatabaseAction ();
public:
    //public database functions
    QString getName();
    QString getAlias();
    QString getSource();
    QString getSourceName();
    QString getReturnType();
private:
    QString meName;
    QString meAlias;
    QString meSource;
    QString meSourceName;
    QString meReturnType;


    void setName(QString paName);
    void setAlias(QString paAlias);
    void setSource(QString paSource);
    void setSourceName(QString paSourceName);
    void setReturnType(QString paReturnType);
};

#endif
