#ifndef CLDATABASECOLUMN_H_WVD_17052011
#define CLDATABASECOLUMN_H_WVD_17052011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>


using namespace std;


class clDatabaseColumn
{

public:
    clDatabaseColumn (QString paName, QString paAlias, QString paType, QString paExtra, QString paReference);
    ~clDatabaseColumn ();
public:
    //public database functions
    QString getName();
    QString getAlias();
    QString getType();
    QString getExtra();
    QString getReference();

private:
    QString meName;
    QString meAlias;
    QString meType;
    QString meExtra;
    QString meReference;



    void setName(QString paName);
    void setAlias(QString paAlias);
    void setType(QString paType);
    void setExtra(QString paExtra);
    void setReference(QString paReference);
};

#endif
