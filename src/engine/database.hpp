/*
 * database.hpp
 *
 *  Created on: 17-Dec-2014
 *      Author: perl
 */

#ifndef DATABASE_HPP_
#define DATABASE_HPP_
#include <QObject>
#include <bb/data/SqlDataAccess>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
#include <QtSql/QSqlQuery>
//#include "chatdatamodel.h"

using namespace bb::data;

class bb::cascades::GroupDataModel;
class bb::cascades::ArrayDataModel;
class SqlDataAcces;

class Database :
        public QObject
{

    Q_OBJECT

public:
    int state;
    QString DB_PATH;
    Database(QObject *parent);
    virtual bool initDatabase();
    void myContacts();
    void updateLocation(int uid,QString loc);
    bool insertMessage(QVariantMap message);
    bool insertAttachments(QVariantMap attach);
    bool updateContacts(QVariantMap data);
    bool insertQuery(QString query,QVariantMap bind);
    QString getUserDetails(QString UserId);
    QSqlQuery executeQuery(QString qstring);
    QVariant executeSqlQuery(QString query,QVariantMap bind);
    QVariantList getLatestChatModel(QString type);
    QVariantList getMessagesModel(int,int,QString);
    bb::cascades::GroupDataModel* getQueryModel(QString query);
    bb::cascades::GroupDataModel* getLive(int uid,int offset,QString type);
    QVariant getQueryVariant(QString query);
    void clearContactHistory(qint32,QString);
    QVariantMap getQueryMap(QString query);
    int getTableSize(QString tabname);
    int getTableSizeByQuery(QString query);
    int getFavCount();
    QVariantMap getMessageById(int id);

private:
    SqlDataAccess *sqlda;

};

#endif /* DATABASE_HPP_ */
