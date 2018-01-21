/*
 * database.cpp
 *
 *  Created on: 17-Dec-2014
 *      Author: perl
 */

#include "database.hpp"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QtSql>
#include <QDate>
#include <QDebug>
#include <QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
//#include "chatdatamodel.h"
#include <bb/data/SqlDataAccess>

using namespace bb::cascades;
using namespace bb::data;

//class bb::cascades::GroupDataModel;

Database::Database(QObject *parent) :
        QObject(parent), DB_PATH("./data/Telega.db"),state(0)
{
    sqlda = new SqlDataAccess(DB_PATH);
    qDebug() << "DB name:" << DB_PATH;
}

bool Database::initDatabase()
{
    // Call this method with the name of the database with
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_PATH);
    qDebug() << "DB valid:" << db.isValid();
    qDebug() << "DB opened:" << db.open();
    bool ok = db.open();
    if (ok) {
        qDebug() << "Telega database created" << db.open();
        QString contactsTable =
                "create table if not exists contacts(id varchar primary key unique,phone varchar,type varchar,lastname varchar,firstname varchar,latitude number,longitude number,expiry date,latestmessage varchar,messagemode varchar,messagetime number,wasonline number,location varchar,favourite boolean,photoid varchar,username varchar,messageread boolean,init boolean)";
        QString messageTable =
                "create table if not exists messagesTab(id varchar primary key unique,out boolean,date varchar,fromId varchar,toId varchar,fwdDate varchar ,fwdFromId varchar,message varchar,read boolean,mediaType varchar,attachmentId varchar,type varchar)";
        QString attachmentsTable =
                "create table if not exists attachments(id varchar,mediaId varchar,date varchar,dcId varchar,accessHash varchar,fileName varchar,mimeType varchar,latitude number,longitude number,duration varchar,type varchar,addedtime number,secret varchar,volumeId number,messageid varchar)";
        QString broadCastTable =
                        "create table if not exists broadCast(usercheck varchar primary key unique,broadcastid varchar,userid varchar)";
        QString settingsTable =
                                "create table if not exists settings(type varchar primary key, value varchar)";
//        QString chatsTable =
//                        "create table if not exists chats(chatid int,mediaId int,date int,dcId int,accessHash int,fileName varchar,mimeType varchar,latitude number,longitude number,duration int)";
//
//        QString primary = "insert into settings";
        QSqlQuery queryContacts(contactsTable);
        QSqlQuery queryMessage(messageTable);
        QSqlQuery queryAttachments(attachmentsTable);
        QSqlQuery querybroadCast(broadCastTable);
        QSqlQuery querySettings(settingsTable);
        qDebug() << "Contacts created" << queryContacts.isActive();
        qDebug() << "Messages created" << queryMessage.isActive();
        qDebug() << "Attachments created" << queryAttachments.isActive();
        qDebug() << "BroadCast created" << querybroadCast.isActive();
        qDebug() << "Settings created" << querySettings.isActive();
        return true;
    } else
        return false;
}
bool Database :: updateContacts(QVariantMap data){
    QSqlQuery query;
    query.prepare("update contacts set latestmessage =:latestmessage , messagetime = :messagetime where id =:id ");
    query.bindValue(":latestmessage",data.value("latestmessage"));
    query.bindValue(":messagetime",data.value("messagetime"));
    query.bindValue(":id",data.value("id"));
    if(query.exec()) return true;
    else
    {
        qDebug() << "error updation" << query.lastError();
        return false;
    }
}
void Database::myContacts()
{
    QSqlQuery query("SELECT * FROM contacts");
    int fieldNo = query.record().indexOf("firstname");
    int dateNo = query.record().indexOf("type");
    while (query.next()) {
        QString name = query.value(fieldNo).toString();
        QString date = query.value(dateNo).toString();
//        qDebug() << "result name:" << name << "location " << date;
    }
}


QVariantList Database::getLatestChatModel(QString type)
{
    QString q;
    ArrayDataModel *model = new ArrayDataModel();
    if(type.isEmpty())
    {
       q = "select * from contacts order by messagetime desc";
    }
    else
    {
        q = "select * from contacts where type = \"";
        q.append(type);
       q.append("\" order by messagetime desc");
    }
    QVariant list = sqlda->execute(q);

    model->clear();
    model->insert(0,list.value<QVariantList>());
    return list.value<QVariantList>();
}



QString Database::getUserDetails(QString UserId)
{
    QSqlQuery query("select firstname,lastname,ID from contacts where id = " + UserId);
//    while (query.next()) {
//             QString country = query.value(0).toString();
//             return country;
//         }
    QString firstname = "firstname";
    return firstname;
}
void Database::updateLocation(int uid, QString loc)
{

    const QString sqlCommand = "UPDATE contacts "
            "    SET location = :location"
            "    WHERE id = :id";
    QVariantMap bindings;
    bindings["location"] = loc;
    bindings["id"] = uid;

    sqlda->execute(sqlCommand, bindings);
    if (!sqlda->hasError()) {
        qDebug() << "data is updated successfully";
    } else
        qDebug() << "updation is false";
}
bool Database::insertMessage(QVariantMap message)
{
    QSqlQuery query;
    query.prepare(
            "INSERT INTO messagesTab (id,date,fromId,toId,fwdDate,fwdFromId,message,read,mediaType,attachmentId) "
                    "VALUES (?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, message.value("id"));
    query.bindValue(1, message.value("date"));
    query.bindValue(2, message.value("fromId"));
    query.bindValue(3, message.value("toId"));
    query.bindValue(4, message.value("fwDate"));
    query.bindValue(5, message.value("fwdFromId"));
    query.bindValue(6, message.value("message"));
    query.bindValue(7, message.value("read"));
    query.bindValue(8, message.value("type"));
    query.bindValue(9, message.value("attachmentId"));
    query.exec();
    if (query.isActive())
        return true;
    return false;
}

bool Database::insertAttachments(QVariantMap attach)
{

    QSqlQuery query;
    query.prepare(
            "INSERT INTO attachments (id,mediaId,date,dcId,accessHash,fileName,mimeType,latitude,longitude,duration) "
                    "VALUES (?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, attach.value("id"));
    query.bindValue(1, attach.value("mediaId"));
    query.bindValue(2, attach.value("date"));
    query.bindValue(3, attach.value("dcId"));
    query.bindValue(4, attach.value("accessHash"));
    query.bindValue(5, attach.value("fileName"));
    query.bindValue(6, attach.value("mimeType"));
    query.bindValue(7, attach.value("latitude"));
    query.bindValue(8, attach.value("longitude"));
    query.bindValue(9, attach.value("duration"));
    if (query.exec())
        return true;
    else {
        qDebug() << "error in insertion::" << query.lastError();
        return false;
    }
}

QVariantList Database::getMessagesModel(int uid,int offset, QString type)
{
    QString query = "select * from messagesTab where ( fromId = \"";
    query.append(QString::number(uid));
    query.append("\" or toId = \"");
    query.append(QString::number(uid));
    query.append("\") and type = \"");
    query.append(type);
    query.append("\" order by date desc");
    query.append(" limit ");
    query.append(QString::number(30));
    query.append(" offset ");
    query.append(QString::number(offset));
    qDebug() << query;
    QVariant list = sqlda->execute(query);

     return list.value<QVariantList>();
}
GroupDataModel* Database::getLive(int uid,int offset,QString type)
{

    GroupDataModel *model = new GroupDataModel(QStringList());
    QString query = "select * from messagesTab where (fromId = \"";
    query.append(QString::number(uid));
    query.append("\" or toId = \"");
    query.append(QString::number(uid));
    query.append("\" ) and type = \"");
    query.append(type);
    query.append("\" and id > \"");
    query.append(QString::number(offset));
    query.append("\" order by date desc");
    QVariant list = sqlda->execute(query);
    model->clear();
    model->insertList(list.value<QVariantList>());
    qDebug()<< "userid in database " << uid << offset << type;
    qDebug() << query << model->size();
    return model;
}

QSqlQuery Database::executeQuery(QString qstring){
    QSqlQuery query(qstring);
    return query;
}

QVariant Database::executeSqlQuery(QString query,QVariantMap bind){
     QVariant var = sqlda->execute(query,bind);

     return var;
}

bool Database::insertQuery(QString query,QVariantMap bind){
    sqlda->execute(query,bind);
    return sqlda->hasError();
}
int Database::getTableSize(QString tabname){
    QSqlQuery q;
     q.prepare(QString("SELECT COUNT (*) FROM %1").arg(tabname));
     if(q.exec()){
         int rows= 0;
              if (q.next()) {
                     rows= q.value(0).toInt();
              }
              return rows;
     }
     else qDebug()<<q.lastError();
};
int Database::getTableSizeByQuery(QString query){
    QSqlQuery q;
     q.prepare(query);
     if(q.exec()){
         int rows= 0;
              if (q.next()) {
                     rows= q.value(0).toInt();
              }
              return rows;
     }
     else qDebug()<<q.lastError();
}
QVariantMap Database::getMessageById(int id){
    QVariantMap d;
    d.insert("id",id);
    QString q = "select * from messagesTab where id = :id";
    QVariant mess = sqlda->execute(q,d);
    QVariantList infomess = mess.value<QVariantList>();
    QVariant ke = infomess.value(0);
    QVariantMap k = ke.value<QVariantMap>();

    return k;

}

void Database::clearContactHistory(qint32 id, QString type)
{
    QVariantMap contactValues;
    contactValues["fromId"] = id;
    contactValues["toId"] = id;
    contactValues["type"] = type;
    executeSqlQuery("delete from messagesTab where (fromId = :fromId or toId = :toId) and type = :type",contactValues);

}
GroupDataModel * Database::getQueryModel(QString query)
{
    GroupDataModel *model = new GroupDataModel(QStringList());
    QVariant data = sqlda->execute(query);
    model->insertList(data.value<QVariantList>());
    return model;
}
QVariant Database::getQueryVariant(QString query)
{

    QVariant data = sqlda->execute(query);
    return data;
}

QVariantMap Database::getQueryMap(QString query)
{

    QVariant data = sqlda->execute(query);
    QVariantMap map = data.toMap();
    return map;
}

int Database::getFavCount(){
    QSqlQuery q;
     q.prepare(QString("SELECT COUNT (*) FROM contacts where favourite = \"true\""));
     if(q.exec()){
         int rows= 0;
              if (q.next()) {
                     rows= q.value(0).toInt();
              }
              return rows;
     }
     else qDebug()<<q.lastError();
};
