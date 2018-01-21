/*
 * Copyright (c) 2017-2018 Arfeo.net.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "applicationui.hpp"
#include "loadcontacts.hpp"
#include "database.hpp"
#include <bb/cascades/Application>
#include <QDateTime>
#include <QDate>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/resources/localehandler.h>
#include <bb/cascades/resources/qmldocument.h>
#include <bb/system/SystemToast.hpp>
#include <bb/system/SystemDialog>
#include <bb/cascades/ActionBarAutoHideBehavior>
#include <bb/pim/account/Account>
#include <bb/data/DataAccessError.hpp>
#include <bb/cascades/TextInputActionItem>
#include <bb/cascades/ActionItem>
#include <bb/cascades/SubmitKey>
#include <bb/system/InvokeManager>
#include <bb/device/VibrationController>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Menu>
#include <bb/system/Clipboard>
#include <QDebug>
#include <QFile>
#include <qdir.h>
#include <QtCore/QByteArray>
#include <QtDeclarative/qdeclarativedebug.h>
#include <unistd.h>
#include "dumper.h"
#include "core/constants.h"
#include "core/dcprovider.h"
//#include "chatdatamodel.h"

using namespace bb::cascades;
using namespace bb::system;

ApplicationUI::ApplicationUI() :
        QObject(),
        m_translator(new QTranslator(this)),
        m_localeHandler(new LocaleHandler(this)),
        m_invokeManager(new InvokeManager(this)),
        db(new Database(this)),
        loggedIn(false)
{
    //connect(udp, SIGNAL(receivedData(QString)), this, SLOT(sendping()));
    connect(&mDcProvider, SIGNAL(authTransferCompleted()), this, SLOT(onauthTransferCompleted()));
    connect(&mDcProvider, SIGNAL(dcProviderReady()), this, SIGNAL(onDcProviderReady()));
    db->initDatabase();
    init();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_telega", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}

ApplicationUI::~ApplicationUI()
{
    if (m_translator)
        delete m_translator;
    if (m_localeHandler)
        delete m_localeHandler;
    if (m_invokeManager)
        delete m_invokeManager;
    if (db)
        delete db;
}

void ApplicationUI::onSystemLanguageChanged()
{
   // ..
}

void ApplicationUI::init()
{
    s = Settings::getInstance();
    s->loadSettings();
    mDcProvider.initialize();
    QList<DC *> dcsList = mDcProvider.getDcs();
    connect(&mDcProvider, SIGNAL(dcProviderReady()), this, SLOT(keyloaded()));
    //changeServer(s->workingDcNum()-1);
    switch (dcsList.value(Settings::getInstance()->workingDcNum() - 1)->state()) {
        case DC::userSignedIn:
            qDebug() << "loggedIn: true";
            mDcProvider.transferAuth();
            loggedIn = true;
            break;
        default:
            qDebug() << "loggedIn: false";
            db->executeQuery("delete from messagesTab");
            loggedIn = false;
            break;
    }
}

bool ApplicationUI::checkLogin() {
    return loggedIn;
}

void ApplicationUI::onAuthSignInError(qint64 msgid, qint32 errcode, QString errtext)
{
    Q_EMIT showerror(errtext);
}

void ApplicationUI::changeServer(qint16 number)
{
    QList<DC *> dcsList = mDcProvider.getDcs();
    Session *session = new Session(dcsList[number], this);
    mApi = new Api(session, this);
    connect(session, SIGNAL(sessionReady(DC*)), this, SLOT(onserverready()));
    connect(session, SIGNAL(sessionClosed(qint32)), this, SLOT(onsessionclosed()));
    //connect(session,SIGNAL(sessionRdisconnected(),this,SLOT() ));
    //TODO when signal destroyed/No internet access
    session->connectToServer();

    connect(mApi, SIGNAL(authSendCodeError(qint64, qint32, QString)), this,
            SLOT(onAuthSendCodeError(qint64,qint32,QString)));
    connect(mApi, SIGNAL(authSignInError(qint64,qint32,QString)), this,
            SLOT(onAuthSignInError(qint64,qint32,QString)));
    connect(mApi, SIGNAL(authSignUpError(qint64,qint32,QString)), this,
            SLOT(onAuthSignInError(qint64,qint32,QString)));
    connect(mApi, SIGNAL(authSentCode(qint64,bool,QString)), this,
            SLOT(onauthSentCode(qint64,bool,QString)));
    connect(mApi, SIGNAL(authSignInAuthorization(qint64,qint32)), this,
            SLOT(onUserAuthorized(qint64,qint32)));
    connect(mApi, SIGNAL(authSignUpAuthorization(qint64,qint32,User)), this,
            SLOT(onUserAuthorized(qint64,qint32)));
    connect(mApi, SIGNAL(contactsContacts(qint64,QList<Contact>,QList<User>)), this,
            SLOT(onContactsContacts(qint64,QList<Contact>,QList<User>)));
    connect(mApi, SIGNAL(messagesGetHistoryMessages(qint64,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SLOT(onMessagesGetHistoryMessages(qint64,QList<Message>,QList<Chat>,QList<User>)));
    connect(mApi,
            SIGNAL(
                    messagesGetHistoryMessagesSlice(qint64,qint32,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SLOT(
                    onMessagesGetHistoryMessageSlice(qint64,qint32,QList<Message>,QList<Chat>,QList<User>)));

    connect(mApi,
            SIGNAL(messagesDialogs(qint64,QList<Dialog>,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SLOT(
                    onMessagesGetDialogsAnswer(qint64,QList<Dialog>,QList<Message>,QList<Chat>,QList<User>)));
    connect(mApi, SIGNAL(usersGetUsersResult(qint64,QList<User>)), this,
            SLOT(onUsersGetUsersAnswer(qint64,QList<User>)));
    connect(mApi, SIGNAL(updatesTooLong()), this, SLOT(onUpdatesTooLong()));
    connect(mApi, SIGNAL(updateShortMessage(qint32,qint32,QString,qint32,qint32,qint32)), this,
            SLOT(onUpdateShortMessage(qint32,qint32,QString,qint32,qint32,qint32)));
    connect(mApi, SIGNAL(updateShortChatMessage(qint32,qint32,qint32,QString,qint32,qint32,qint32)),
            this,
            SLOT(onUpdateShortChatMessage(qint32,qint32,qint32,QString,qint32,qint32,qint32)));
    connect(mApi, SIGNAL(updateShort(Update,qint32)), this, SLOT(onUpdateShort(Update,qint32)));
    connect(mApi,
            SIGNAL(messagesGetMessagesMessages(qint64,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SLOT(onMessagesGetHistoryMessages(qint64,QList<Message>,QList<Chat>,QList<User>)));
    connect(mApi,
            SIGNAL(
                    messagesGetMessagesMessagesSlice(qint64,qint32,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SIGNAL(
                    onMessagesGetHistoryMessageSlice(qint64,qint32,QList<Message>,QList<Chat>,QList<User>)));

    connect(mApi,
            SIGNAL(updatesCombined(QList<Update>,QList<User>,QList<Chat>,qint32,qint32,qint32)),
            this,
            SLOT(onUpdatesCombined(QList<Update>,QList<User>,QList<Chat>,qint32,qint32,qint32)));
    connect(mApi, SIGNAL(updates(QList<Update>,QList<User>,QList<Chat>,qint32,qint32)), this,
            SLOT(onUpdates(QList<Update>,QList<User>,QList<Chat>,qint32,qint32)));
    connect(mApi,
            SIGNAL(
                    messagesSendMessageAnswer(qint64,qint32,qint32,qint32,qint32,QList<ContactsLink>)),
            this,
            SLOT(
                    onMessagesSendMessageAnswer(qint64,qint32,qint32,qint32,qint32,QList<ContactsLink>)));
    connect(mApi, SIGNAL(messagesSentMessage(qint64,qint32,qint32,qint32,qint32)), this,
            SLOT(onMessagesSentMessage(qint64,qint32,qint32,qint32,qint32)));
    connect(mApi,
            SIGNAL(messagesSentMessageLink(qint64,qint32,qint32,qint32,qint32,QList<ContactsLink>)),
            this,
            SIGNAL(
                    messagesSendMessageAnswer(qint64,qint32,qint32,qint32,qint32,QList<ContactsLink>)));

    connect(mApi, SIGNAL(contactsBlockResult(qint64,bool)), this,
            SLOT(contactsUnBlockAnswer(qint64,bool)));
    connect(mApi, SIGNAL(contactsBlocked(qint64,QList<ContactBlocked>,QList<User>)), this,
            SLOT(onContactsBlocked(qint64,QList<ContactBlocked>,QList<User>)));
    connect(mApi, SIGNAL(contactsUnblockResult(qint64,bool)), this,
            SLOT(contactsUnBlockAnswer(qint64,bool)));
    connect(mApi, SIGNAL(messagesChatFull(qint64,ChatFull,QList<Chat>,QList<User>)), this,
            SLOT(onMessagesGetFullChatAnswer(qint64,ChatFull,QList<Chat>,QList<User>)));
    connect(mApi,
            SIGNAL(
                    messagesDeleteChatUserStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)),
            this,
            SLOT(
                    onMessagesDeleteChatUserStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)));
    connect(mApi,
            SIGNAL(
                    messagesAddChatUserStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)),
            this,
            SLOT(
                    onMessagesAddChatUserStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)));
    connect(mApi, SIGNAL(messagesChats(qint64,QList<Chat>,QList<User>)), this,
            SLOT(messagesGetChatsAnswer(qint64,QList<Chat>,QList<User>)));
    connect(mApi, SIGNAL(contactsDeleteContactsResult(qint64,bool)), this,
            SIGNAL(contactsDeleteContactsAnswer(qint64,bool)));
    connect(mApi, SIGNAL(contactsDeleteContactLink(qint64,ContactsMyLink,ContactsForeignLink,User)),
            this,
            SLOT(contactsDeleteContactAnswer(qint64,ContactsMyLink,ContactsForeignLink,User)));
    connect(mApi, SIGNAL(accountUser(qint64,User)), this,
            SLOT(accountUpdateProfileAnswer(qint64,User)));
    connect(mApi, SIGNAL(authLogOutResult(qint64,bool)), this,
            SLOT(onAuthLogOutAnswer(qint64,bool)));
    connect(mApi,
            SIGNAL(
                    messagesCreateChatStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)),
            this,
            SLOT(
                    onMessagesCreateChatStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)));
    connect(mApi,
            SIGNAL(
                    messagesEditChatTitleStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)),
            this,
            SLOT(
                    onMessagesEditChatTitleStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)));
    connect(mApi, SIGNAL(uploadFile(qint64,StorageFileType,qint32,QByteArray)), this,
            SLOT(onUploadGetFileAnswer(qint64,StorageFileType,qint32,QByteArray)));
    connect(mApi,
            SIGNAL(
                    messagesForwardMsgsStatedMessages(qint64,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SLOT(onMessagesGetHistoryMessages(qint64,QList<Message>,QList<Chat>,QList<User>)));
    connect(mApi,
            SIGNAL(
                    messagesSendBroadcastStatedMessages(qint64,QList<Message>,QList<Chat>,QList<User>)),
            this,
            SLOT(onMessagesGetHistoryMessages(qint64,QList<Message>,QList<Chat>,QList<User>)));
    connect(mApi, SIGNAL(messagesSearchMessages(qint64,QList<Message>,QList<Chat>,QList<User>)),
            this, SLOT(onMessagesSearchAnswer(qint64,QList<Message>,QList<Chat>,QList<User>)));
    connect(mApi,
            SIGNAL(
                    messagesForwardMsgStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)),
            this,
            SLOT(
                    onMessagesForwardMsgStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32)));
    connect(mApi,
            SIGNAL(
                    messagesForwardMsgsStatedMessages(qint64,QList<Message>,QList<Chat>,QList<User>,qint32,qint32)),
            this,
            SLOT(
                    onMessagesForwardMsgStatedMessage(qint64,QList<Message>,QList<Chat>,QList<User>,qint32,qint32)));
    connect(mApi,
            SIGNAL(
                    contactsImportedContacts(qint64,QList<ImportedContact>,QList<qint64>,QList<User>)),
            this,
            SIGNAL(
                    contactsImportContactsAnswer(qint64,QList<ImportedContact>,QList<qint64>,QList<User>)));
}

void ApplicationUI::switchDC(QString number)
{
    //qDebug() << number;
    Settings::getInstance()->setWorkingDcNum(qint32(number.toInt()));
    Settings::getInstance()->writeAuthFile();
    //changeServer(number.toInt()-1 );
}

void ApplicationUI::onAuthSendCodeError(qint64 msgid, qint32 code, QString codetext)
{
    switch (code) {
        case 303: {
            dcchange(codetext);
            Q_EMIT movingdcinit();
            break;
        }
        case 400: {
            Q_EMIT invalidphone(codetext);
            break;
        }
    }
}

void ApplicationUI::dcchange(QString codetext)
{
    qint32 newDc = codetext.mid(codetext.lastIndexOf("_") + 1).toInt();
    qDebug() << "migrated to dc" << newDc;
    Settings::getInstance()->setWorkingDcNum(newDc);
    DC *dc = mDcProvider.getDc(newDc);
    mApi->changeMainSessionToDc(dc);
}

void ApplicationUI::onserverready()
{
    DC *dc = mDcProvider.getWorkingDc();
    if(dc->authKeyId()) {
        getCurrentUser();
        getLatestMessages();
        getcontacts();
    }
}

void ApplicationUI::authSendCode(QString ph)
{
    phone = ph;
    mApi->authSendCode(phone, 0, APP_ID, APP_HASH, "en");
}

void ApplicationUI::onauthSentCode(qint64 msg, bool regresp, QString hashs)
{
    hash = hashs;
    reg = regresp;
    emit qmlgethashpane(hash, reg);
}
void ApplicationUI::onauthTransferCompleted()
{
    qDebug() << "authtransfer";
    Q_EMIT checkserver();
}
bool ApplicationUI::isUserReg()
{
    bool reg;
}
void ApplicationUI::authUser(QString code)
{
    mApi->authSignIn(phone, hash, code);
}
void ApplicationUI::authUserSignIn(QString first, QString last, QString code)
{
    mApi->authSignUp(phone, hash, code, first, last);
}
void ApplicationUI::onUserAuthorized(qint64, qint32 expires)
{
    // change state of current dc
    qint32 workingDcNum = Settings::getInstance()->workingDcNum();
    DC *dc = mDcProvider.getDc(workingDcNum);
    dc->setState(DC::userSignedIn);
    dc->setExpires(expires);
    QList<DC *> dcsList = mDcProvider.getDcs();
    Settings::getInstance()->setDcsList(dcsList);
    Settings::getInstance()->writeAuthFile();
//    mDcProvider.transferAuth();
    Q_EMIT quitapp();
}

QVariantList ApplicationUI::getContactsmodal(QString type)
{
    return db->getLatestChatModel(type);
//    QVariantList varParams;
//    varParams << m_cachedUsers;
//    return varParams;
}

void ApplicationUI::getcontacts()
{
    QString hash;
    mApi->contactsGetContacts(hash);
}

void ApplicationUI::onContactsContacts(qint64 msgId, const QList<Contact> &contacts,
        const QList<User> &users)
{
    db->executeQuery("delete from contacts where type != \"broadcast\"");
    m_cachedContacts = contacts;
    m_cachedUsers = users;
    QVariantMap map;
    Q_FOREACH(User user, m_cachedUsers ){
    idtoname.insert(QString::number(user.id()),user.firstName());
    map["id"] = user.id();
    map["firstname"] = user.firstName();
    map["lastname"]=user.lastName();
    map["phone"] = user.phone();
    map["type"] = "contact";
    map["wasonline"] = user.status().wasOnline();
    QString qstring = "insert into contacts(id,firstname,lastname,phone,type,wasonline)values(:id,:firstname,:lastname,:phone,:type,:wasonline)";
    bool isdataInserted = db->insertQuery(qstring,map);
    if(user.photo().photoId()) {
        QFile ffile("./data/" +QString::number(user.photo().photoId())+".jpeg");
        if(ffile.exists())
        {
            QVariantMap m_map;
            m_map.insert("id",user.id());
            m_map.insert("photoid",user.photo().photoId());
            db->insertQuery("update contacts set photoid = :photoid where id = :id",m_map);
            Q_EMIT userprofile(user.id(),user.photo().photoId());
        }
        else
        {
            DownloadImageThumb("userprofile",user.photo().photoId(),user.photo().photoSmall(),user.id(),user.accessHash());
        }
    }
}

Q_EMIT
    reloadContacts();
}
void ApplicationUI::DownloadImageThumb(QString type, qint64 photoid, FileLocation ps,
        qint32 clientid, qint64 accesshash)
{
    InputFileLocation location(InputFileLocation::typeInputFileLocation);
    location.setVolumeId(ps.volumeId());
    location.setSecret(ps.secret());
    location.setLocalId(ps.localId());
    location.setAccessHash(accesshash);
    qint64 msg = uploadGetFile(location, 0, ps.dcId());
    QVariantMap mapper;
    mapper.insert("id", photoid);
    db->insertQuery("delete from attachments where id=:id", mapper);
    QVariantMap m_map;
    m_map.insert("id", clientid);
    m_map.insert("photoid", photoid);
    db->insertQuery("update contacts set photoid = :photoid where id = :id", m_map);
    mapper.insert("msgid", msg);
    mapper.insert("type", type);

    db->insertQuery("insert into attachments( id,messageid,type )values(:id,:msgid,:type)", mapper);
}
void ApplicationUI::getCurrentUser()
{
    qint64 d = 0;
    InputUser user = askForInputUser("self", d);
    QList<InputUser> input;
    input << user;
    mApi->usersGetUsers(input);
}

void ApplicationUI::keyloaded()
{
    Q_EMIT onkeyloaded(true);
}

void ApplicationUI::sendping()
{
    mApi->sendping();
}

void ApplicationUI::getmessages(qint32 id, QString type, qint32 offset)
{
    InputPeer peer = askForInputPeer(QString::number(id), type);
    mApi->messagesGetHistory(peer, offset, 0, 20);
}

qint32 ApplicationUI::workingdc()
{
    return Settings::getInstance()->workingDcNum();
}

QVariantList ApplicationUI::getMessagesModel(qint32 id, int offset, QString type)
{

    return db->getMessagesModel(id, offset, type);
}
void ApplicationUI::userdetails(QString userid)
{
    qDebug() << userid;
//    emit getuserdetails(db->getUserDetails(userid));
}
void ApplicationUI::settyping(QString userid, bool check)
{
    mApi->messagesSetTyping(askForInputPeer(userid, "contact"), check);
}

void ApplicationUI::onMessagesGetHistoryMessages(qint64 id, const QList<Message> &messages,
        const QList<Chat> &chats, const QList<User> &users)
{
    onMessagesGetHistoryAnswer(id, messages.size(), messages, chats, users);
}
void ApplicationUI::onMessagesGetHistoryMessageSlice(qint64 id, qint32 count,
        const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users)
{
    onMessagesGetHistoryAnswer(id, messages.size(), messages, chats, users);
}

void ApplicationUI::onMessagesGetHistoryAnswer(qint64 id, qint32 count,
        const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users)
{
    for (qint32 i = messages.size() - 1; i >= 0; i--) {
        insertMessage(messages.at(i));
    }
    qDebug() << messages.length() << chats.length() << users.length();
    if (messages.length() == 1) {
        Message mes = messages.at(0);
        if (currentUser.userid == mes.fromId() && mes.out() == true) {
            qDebug() << mes.fromId();
            Q_EMIT updatemessage(db->getMessageById(mes.id()));
        }
    } else {
        Q_EMIT historyloaded();
    }
}

void ApplicationUI::getLatestMessages()
{
    mApi->messagesGetDialogs(0, 0, 100);
}

void ApplicationUI::onUsersGetUsersAnswer(qint64 id, const QList<User> users)
{
    qDebug() << "usersGetUsersAnswer SIGNAL received, id:" << id;
    Q_FOREACH (User user, users){
    Dumper::dumpUser(user);
    if( User::typeUserSelf == user.classType() ) {
        currentUser.userid = user.id();
        currentUser.firstName = user.firstName();
        currentUser.lastName = user.lastName();
        currentUser.phoneNumber = user.phone();
        currentUser.username = user.username();
        Q_EMIT accountUserInfo(currentUser.firstName ,currentUser.lastName, currentUser.phoneNumber , currentUser.username ,currentUser.userid);

        QVariantMap map;
        qint64 userpid = user.photo().photoId();
        QFile photofile(QString("./data/"+QString::number(userpid)+".jpeg"));
        if(photofile.exists()) {
            qDebug()<< "checking the photo file if loop";
            Q_EMIT profileimage(userpid);
        }
        else
        {
            qDebug()<< "checking the photo file else loop";
            InputFileLocation location(InputFileLocation::typeInputFileLocation);
            FileLocation ps = user.photo().photoSmall();
            location.setVolumeId(ps.volumeId());
            location.setSecret(ps.secret());
            location.setLocalId(ps.localId());
            location.setAccessHash(user.accessHash());
            Dumper::dumpUserProfilePhoto(user.photo());
            qint64 msg = uploadGetFile(location,0,user.photo().photoSmall().dcId());
            map.insert("id",user.photo().photoId());
            qDebug()<<"user photo id "<<user.photo().photoId() << " ===="<<msg;
            db->insertQuery("delete from settings where type='userphotoid'",QVariantMap());
            db->insertQuery("insert into settings(type,value)values('userphotoid',:id)",map);
            db->insertQuery("delete from attachments where type='userphotomsgid'",QVariantMap());
            map.insert("msg",msg);
            db->insertQuery("insert into attachments(id,type,messageid)values(:id,'userphotomsgid',:msg)",map);
            foreach(QString key,map.keys())
            qDebug()<<key<<map.value(key);
        }
    }
}
}

void ApplicationUI::onMessagesGetDialogsAnswer(qint64 id, QList<Dialog> dialogs,
        QList<Message> messages, QList<Chat> chats, QList<User> users)
{

    qDebug() << "messagesGetDialogsAnswer SIGNAL received, id:" << id;
    qDebug() << "dialogs:" << dialogs.size();
    qDebug() << "last messages from dialogs:" << messages.size();
    qDebug() << "chats mentioned in dialogs:" << chats.size();
    qDebug() << "users mentioned in messages and chats:" << users.size();
    qDebug() << "-----";
    QMap<qint32, const User*> mentionedUsers;
    for (qint32 i = 0; i < users.size(); i++) {
        mentionedUsers.insert(users.at(i).id(), &(users.at(i)));
    }
    qint32 chatCounter = 0;
    QVariantMap mchats;
    QVariantMap musers;
    QVariantMap mmessages;
    for (qint32 i = 0; i < dialogs.size(); ++i) {
        mchats.clear();
        musers.clear();
        mmessages.clear();
        Dialog thisdialog = dialogs.at(i);
        Message thismessage = messages.at(i);
        Peer peer = thisdialog.peer();
        if (peer.classType() == Peer::typePeerChat) {
//            qDebug() << "chatId:" << peer.chatId();
            Chat thischat = chats.at(chatCounter++);
            qDebug() << "Chat Name: " << thischat.title();
            mchats["id"] = peer.chatId();
            mchats["firstname"] = thischat.title();
            mchats["latestmessage"] = thismessage.message();
            mchats["messagemode"] = thismessage.out();
            mchats["messagetime"] = thismessage.date();
            mchats["messageread"] = !thismessage.unread();
            mchats["type"] = "chat";
            mchats["init"] = true;
            QString query =
                    "insert into contacts(id,firstname,latestmessage,messagemode,messagetime,messageread,type,init)"
                            "values(:id,:firstname,:latestmessage,:messagemode,:messagetime,:messageread,:type,:init)";
            db->insertQuery(query, mchats);
            qDebug() << thischat.photo().photoSmall().localId();
            if (thischat.photo().photoSmall().localId()) {
                qDebug() << thischat.photo().photoSmall().localId();
                QFile ffile(
                        "./data/" + QString::number(thischat.photo().photoSmall().localId())
                                + ".jpeg");
                if (ffile.exists()) {
                    QVariantMap m_map;
                    m_map.insert("id", thischat.id());
                    m_map.insert("photoid", thischat.photo().photoSmall().localId());
                    db->insertQuery("update contacts set photoid = :photoid where id = :id", m_map);
                    Q_EMIT userprofile(thischat.id(), thischat.photo().photoSmall().localId());
                } else {
                    DownloadImageThumb("userprofile", thischat.photo().photoSmall().localId(),
                            thischat.photo().photoSmall(), thischat.id(), 0);
                }
            }
        } else if (peer.classType() == Peer::typePeerUser) {
//            qDebug() << thismessage.message() << Message::typeMessageService << Message::typeMessage << peer.classType() << Peer::typePeerUser;
            const User *u = mentionedUsers.value(peer.userId());
            musers.insert("id", u->id());
            musers.insert("firstname", u->firstName());
            musers.insert("lastname", u->lastName());
            musers.insert("phone", u->phone());
            musers.insert("latestmessage", thismessage.message());
            musers.insert("messagemode", thismessage.out());
            musers.insert("messagetime", thismessage.date());
            musers.insert("messageread", !thismessage.unread());
            musers.insert("type", "contact");
            musers.insert("init", true);
//            qDebug() << u->firstName() << u->status().wasOnline() << thismessage.date();
            QString query =
                    "insert into contacts(id,firstname,lastname,phone,latestmessage,messagemode,messagetime,messageread,type,init)"
                            "values(:id,:firstname,:lastname,:phone,:latestmessage,:messagemode,:messagetime,:messageread,:type,:init)";
            if (!db->insertQuery(query, musers)) {
            } else {
//                qDebug() << "firstname" << u->firstName() << "   ==  messagetime"
//                        << thismessage.date() << u->status().wasOnline();
                if (u->status().wasOnline() == 0) {
                    musers.insert("wasonline", thismessage.date());
                }
                QString q =
                        "update contacts set firstname=:firstname,phone=:phone,lastname=:lastname,wasonline = :wasonline,type= :type,latestmessage = :latestmessage,messagetime = :messagetime,messagemode = :messagemode,messageread = :messageread,type=:type,init=:init where id = :id";
                bool isupdated = db->insertQuery(q, musers);
            }
        }
    }
    Q_EMIT reloadContacts();
}

void ApplicationUI::flushTable(QString tablename)
{
    QString query = "delete from ";
    query.append(tablename);
    db->executeQuery(query);
    Q_EMIT reloadContacts();
}

// Utilites
InputPeer ApplicationUI::askForInputPeer(QString userid, QString sType)
{
    bool valid = false;
    InputPeer::InputPeerType type;
    while (!valid) {
        if (sType == "contact") {
            type = InputPeer::typeInputPeerContact;
            valid = true;
        } else if (sType == "chat") {
            type = InputPeer::typeInputPeerChat;
            valid = true;
        } else if (sType == "self") {
            type = InputPeer::typeInputPeerSelf;
            valid = true;
        }
    }
    InputPeer peer(type);
    if (type == InputPeer::typeInputPeerContact) {
        peer.setUserId(userid.toInt());
    } else if (type == InputPeer::typeInputPeerChat) {
        peer.setChatId(userid.toInt());
    }
    return peer;
}
//TODO Foregin User Bu
InputUser ApplicationUI::askForInputUser(QString sType, qint64 id)
{
    bool valid = false;
    InputUser::InputUserType type;
    if (sType == "contact") {
        type = InputUser::typeInputUserContact;
        valid = true;
    } else if (sType == "foreign") {
        type = InputUser::typeInputUserForeign;
        valid = true;
    } else if (sType == "self") {
        type = InputUser::typeInputUserSelf;
        valid = true;
    }
    QString sId;
    QString sAccessHash;
    qint64 accessHash;
    InputUser user(type);
    if (type != InputUser::typeInputUserSelf) {
        user.setUserId(id);
        if (type == InputUser::typeInputUserForeign) {
            accessHash = sAccessHash.toLongLong(&valid);
            user.setAccessHash(accessHash);
        }
    }
    return user;
}

void ApplicationUI::onUpdateShortMessage(qint32 id, qint32 fromId, const QString &message,
        qint32 pts, qint32 date, qint32 seq)
{
    Message m;
    m.setId(id);
    m.setFromId(fromId);
    m.setMessage(message);
    m.setDate(date);
    m.setUnread(1);
    m.setDate(date);
    insertMessage(m);
    Q_EMIT updatemessage(db->getMessageById(id));
}

void ApplicationUI::onUpdateShortChatMessage(qint32 id, qint32 fromId, qint32 chatId,
        const QString &message, qint32 pts, qint32 date, qint32 seq)
{

    qDebug() << fromId << chatId << message;
    Message m;
    m.setId(id);
    m.setDate(date);
    m.setFromId(fromId);
    m.setUnread(false);
    Peer peer(Peer::typePeerChat);
    peer.setChatId(chatId);
    m.setToId(peer);
    m.setMessage(message);
    m.setOut(false);
    insertMessage(m);
    Q_EMIT updatemessage(db->getMessageById(id));

}

void ApplicationUI::onUpdateShort(const Update &update, qint32 date)
{
//    qDebug() << "updateShort SIGNAL received" << Dumper::unixTimeToDate(date);
    liveUpdate(update);
}

void ApplicationUI::onUpdatesCombined(const QList<Update> &updates, const QList<User> &users,
        const QList<Chat> &chats, qint32 date, qint32 seqStart, qint32 seq)
{
    Q_FOREACH (Update update, updates){
    liveUpdate(update);
}
qDebug() << "Users:";
Q_FOREACH (User user, users) {
    Dumper::dumpUser(user);
}
}

void ApplicationUI::onUpdates(const QList<Update> &udts, const QList<User> &users,
        const QList<Chat> &chats, qint32 date, qint32 seq)
{
    qDebug() << "updates SIGNAL received" << "date:" << Dumper::unixTimeToDate(date);
    Q_FOREACH (Update update, udts){
    liveUpdate(update);
}
    qDebug() << "Users:";
    Q_FOREACH (User user, users){
    Dumper::dumpUser(user);
}
    qDebug() << "Chats:";
    Q_FOREACH (Chat chat, chats){
    Q_EMIT loadGroupInfo(chat.title(),chat.participantsCount(),chat.id(),chat.left());
}
}
void ApplicationUI::dumpTable(QString tablename)
{
    qDebug() << "dumping the table in dumpTable";
    QVariantMap dat;
    dat.insert("tablename", tablename);
    QString query = "select * from ";
    query.append(tablename);
    QSqlQuery data = db->executeQuery(query);

    while (data.next()) {
        for (int i = 0; i < data.record().count(); i++) {
            qDebug() << data.value(i).toString();
        }
    }

}

void ApplicationUI::setPopUserId(int id)
{
    popuserid = id;
//    qDebug() << "Settings popUserID" << id;
}
int ApplicationUI::getPopUserId()
{
//    qDebug() << "Gettings popUserID" << popuserid;
    return popuserid;
}

void ApplicationUI::setPopGroupId(int id)
{
    popgroupid = id;
//    qDebug() << "Settings popUserID" << id;
}
int ApplicationUI::getPopGroupId()
{
//    qDebug() << "Gettings popUserID" << popuserid;
    return popgroupid;
}

void ApplicationUI::sendMessage(QString msg, qint64 data)
{
    InputPeer peer = askForInputPeer(QString::number(popuserid), "contact");
    mApi->messagesSendMessage(peer, msg, data);
}
void ApplicationUI::sendMessageChat(QString msg, qint64 data)
{
    InputPeer peer = askForInputPeer(QString::number(popgroupid), "chat");
    qDebug() << peer.chatId();
    mApi->messagesSendMessage(peer, msg, data);
}
void ApplicationUI::sendTyping(QString m, bool k)
{
    InputPeer peer = askForInputPeer(m, "contact");
    mApi->messagesSetTyping(peer, k);
}

void ApplicationUI::liveUpdate(const Update &update)
{

    Update::UpdateType x = update.classType();
//    qDebug() << "#Update" << x << update.message().message() ;
    Q_ASSERT(
            x == Update::typeUpdateNewMessage || x == Update::typeUpdateMessageID
                    || x == Update::typeUpdateReadMessages || x == Update::typeUpdateDeleteMessages
                    || x == Update::typeUpdateRestoreMessages || x == Update::typeUpdateUserTyping
                    || x == Update::typeUpdateChatUserTyping
                    || x == Update::typeUpdateChatParticipants || x == Update::typeUpdateUserStatus
                    || x == Update::typeUpdateUserName || x == Update::typeUpdateUserPhoto
                    || x == Update::typeUpdateContactRegistered
                    || x == Update::typeUpdateContactLink || x == Update::typeUpdateActivation
                    || x == Update::typeUpdateNewAuthorization
                    || x == Update::typeUpdateNewGeoChatMessage
                    || x == Update::typeUpdateNewEncryptedMessage
                    || x == Update::typeUpdateEncryptedChatTyping
                    || x == Update::typeUpdateEncryption
                    || x == Update::typeUpdateEncryptedMessagesRead
                    || x == Update::typeUpdateChatParticipantAdd
                    || x == Update::typeUpdateChatParticipantDelete
                    || x == Update::typeUpdateDcOptions || x == Update::typeUpdateUserBlocked
                    || x == Update::typeUpdateNotifySettings);

    switch (x) {
        case Update::typeUpdateNewMessage: {
            insertMessage(update.message());
            Q_EMIT updatemessage(db->getMessageById(update.message().id()));
            break;
        }
        case Update::typeUpdateMessageID: {
            QList<qint32> msg;
            msg << update.id();
            mApi->messagesGetMessages(msg);
            break;
        }
        case Update::typeUpdateReadMessages: {
            QList<qint32> messages = update.messages();
            Q_FOREACH (qint32 msgId, messages){
            QVariantMap map;
            map.insert("id",msgId);
            map.insert("read",1);
            db->insertQuery("update messagesTab set read = :read where id = :id",map);
            Q_EMIT messageread(msgId);
        }
            break;
        }
        case Update::typeUpdateDeleteMessages: {
            QList<qint32> messages = update.messages();
            qDebug() << "Delete messages " << messages.size();
            Q_FOREACH (qint32 msgId, messages){
            Q_EMIT livechatdeletemessage(db->getMessageById(msgId));
            deleteMessage(msgId);
        }
            break;
        }
        case Update::typeUpdateRestoreMessages: {
            qDebug() << "type: restore messages";
            QList<qint32> messages = update.messages();
            Q_FOREACH (qint32 msgId, messages){
            qDebug() << "msgId:" << msgId;
        }
            qDebug() << "pts: " << update.pts();
            break;
        }
        case Update::typeUpdateUserTyping:
            if (popuserid == update.userId()) {
                Q_EMIT usertyping(update.userId());
            }
            break;
        case Update::typeUpdateChatUserTyping:
            if (popgroupid == update.chatId()) {
                Q_EMIT usertypinggroup(getContactInfo(update.userId()));
            }
            break;
        case Update::typeUpdateChatParticipants:
            qDebug() << "type: chat participants";
            Q_FOREACH (ChatParticipant cp, update.participants().participants()){
            qDebug() << cp.inviterId() << cp.userId();
        }
        getLatestMessages();
        break;
        case Update::typeUpdateUserStatus:
//            onStatusUpdate(update.status(), update.userId());
        break;
        case Update::typeUpdateUserName:
        currentUser.userid = update.userId();
        currentUser.firstName = update.firstName();
        currentUser.lastName = update.lastName();
        break;
        case Update::typeUpdateUserPhoto:
        qDebug() << "type: user photo";
        qDebug() << "userId:" << update.userId();
        DownloadImageThumb("userprofile", update.photo().photoId(), update.photo().photoSmall(),update.userId(),0);
        break;
        case Update::typeUpdateContactRegistered:
        qDebug() << "type: contact registered";
        qDebug() << "userId:" << update.userId();
//           qDebug() << "date:" << unixTimeToDate(update.date());
        break;
        case Update::typeUpdateContactLink:
        deletecontactlocal(update.userId());
//        Q_EMIT reloadContacts();
        break;
        case Update::typeUpdateActivation:
        qDebug() << "type: activation";
        qDebug() << "userId:" << update.userId();
        break;
        case Update::typeUpdateNewAuthorization:
        qDebug() << "type: new authorization";
        qDebug() << "authKeyId:" << update.authKeyId();
//           qDebug() << "date:" << unixTimeToDate(update.date());
        qDebug() << "device:" << update.device();
        qDebug() << "location:" << update.location();
        break;
        case Update::typeUpdateNewGeoChatMessage:
        qDebug() << "type: new geo chat message";
//           dumpGeoChatMessage(update.geoChatMessage());
        break;
        //XXX HERE must be the cases for encrypted messages when implemented
        case Update::typeUpdateChatParticipantAdd:
        qDebug() << "type: add chat participant";
        qDebug() << "chatId:" << update.chatId();
        qDebug() << "userId:" << update.userId();
        qDebug() << "inviterId:" << update.inviterId();
        qDebug() << "version:" << update.version();
        break;
        case Update::typeUpdateChatParticipantDelete:
        qDebug() << "type: delete chat participant";
        qDebug() << "chatId:" << update.chatId();
        qDebug() << "userId:" << update.userId();
        qDebug() << "version:" << update.version();
        break;
        case Update::typeUpdateDcOptions: {
            qDebug() << "type: dc options";
            QList<DcOption> dcOptions = update.dcOptions();
            Q_FOREACH (DcOption dcOption, dcOptions) {
//               dumpDcOption(dcOption);
            }
            break;
        }
        case Update::typeUpdateUserBlocked:
        qDebug() << "type: user blocked";
        qDebug() << "userId:" << update.userId();
        qDebug() << "blocked:" << update.blocked();
        break;
        case Update::typeUpdateNotifySettings:
        qDebug() << "type: notify settings";
//           dumpNotifyPeer(update.peer());
//           dumpPeerNotifySettings(update.notifySettings());
        break;
        default:
        qDebug() << "Update received in a not contemplated option";
        break;
    }
}

void ApplicationUI::insertMessage(const Message &message)
{
    const Message m = message;
    QVariantMap attach, msgmap, cmap;

    attach.insert("id", m.id());
    attach.insert("addeddate", QDateTime().currentDateTime().toTime_t());
    attach.insert("date", m.date());
    QString msg = m.message();

    if (message.media().classType() != MessageMedia::typeMessageMediaEmpty) {
        QString type = "photo";
        qint64 id;
        qint64 accessHash;
        QString attachmsg;
        switch (message.media().classType()) {
            Dumper::dumpMessageMedia(message.media());
        case MessageMedia::typeMessageMediaVideo:
            type = "video";
            id = m.media().video().id();
            accessHash = m.media().video().accessHash();
            attach.insert("duration", m.media().video().duration());
            attach.insert("dcId", m.media().video().dcId());
            attach.insert("accessHash", accessHash);
            attach.insert("mediaId", id);
            attach.insert("mimeType", type);
            attach.insert("date", m.date());
            attach.insert("type", "attachment");
            attachmsg.append("Attachment : Type Video \n");
            attachmsg.append(m.media().video().caption());
            attachmsg.append(" \n Size : ");
            attachmsg.append(QString::number(m.media().video().size()));
            attachmsg.append(" \n Duration: ");
            attachmsg.append(QString::number(m.media().video().duration()));
            attachmsg.append(" \n Dimenssion: ");
            attachmsg.append(QString::number(m.media().video().h()));
            attachmsg.append(" x ");
            attachmsg.append(QString::number(m.media().video().w()));
            attachmsg.append(" px");
            db->insertQuery(
                    "insert into attachments(id,mediaId,date,dcId,accessHash,mimeType,duration,addeddate,type)values(:id,:mediaId,:date,:dcId,:accessHash,:mimeType,:duration,:addeddate,:type)",
                    attach);
            break;
        case MessageMedia::typeMessageMediaGeo:
            type = "geopoint";
            attach.insert("latitude", m.media().geo().lat());
            attach.insert("longitude", m.media().geo().longitude());
            attach.insert("mimeType", type);
            attach.insert("type", "attachment");
            //TODO geo
            db->insertQuery(
                    "insert into attachments (id,latitude,longitude,mimeType,type,addeddate)values(insert into attachments (:id,:latitude,:longitude,:mimeType,:type,:addeddate)",
                    attach);
            break;
        case MessageMedia::typeMessageMediaContact:
            type = "contact";
            attach.insert("firstname", m.media().firstName());
            attach.insert("lastname", m.media().lastName());
            attach.insert("phone", m.media().phoneNumber());
            attach.insert("mimeType", type);
            attach.insert("type", "attachment");
            db->insertQuery(
                    "insert into attachments(id,mimeType,firstname,lastname,phone,type,addeddate)values(:id,:mimeType,:firstname,:lastname,:phone,:type,:addeddate)",
                    attach);
            break;
        case MessageMedia::typeMessageMediaUnsupported:
            type = "unsupported";
            break;
        case MessageMedia::typeMessageMediaDocument:
            type = "document";
            id = m.media().document().id();
            accessHash = m.media().document().accessHash();
            attach.insert("dcId", m.media().document().dcId());
            attach.insert("mediaId", id);
            attach.insert("mimeType", type);
            attach.insert("accessHash", accessHash);
            attach.insert("type", "attachment");
            attachmsg.append("Type :");
            attachmsg.append(type);
            db->insertQuery(
                    "insert into attachments (id,mediaId,dcId,mimeType,accessHash,type,addeddate,date)values(:id,:mediaId,:dcId,:mimeType,:accessHash,:type,:addeddate,:date)",
                    attach);
            break;
        case MessageMedia::typeMessageMediaAudio:
            type = "audio";
            id = m.media().audio().id();
            accessHash = m.media().audio().accessHash();
            attach.insert("duration", m.media().audio().duration());
            attach.insert("dcId", m.media().audio().dcId());
            attach.insert("accessHash", accessHash);
            attach.insert("mediaId", id);
            attach.insert("mimeType", type);
            attach.insert("date", m.date());
            attach.insert("type", "attachment");
            attachmsg.append("Attachment : Type Audio \n");
            attachmsg.append(QString::number(m.media().audio().duration()));
            db->insertQuery(
                    "insert into attachments(id,mediaId,date,dcId,accessHash,mimeType,duration,addeddate,type)values(:id,:mediaId,:date,:dcId,:accessHash,:mimeType,:duration,:addeddate,:type)",
                    attach);
            break;
        default:
            type = "photo";
            attach.insert("accessHash", accessHash);
            attach.insert("mediaId", id);
            attach.insert("mimeType", type);
            attach.insert("type", "attachment");
            attachmsg.append("Attachment : Type Image \n");
//                attachmsg.append(QString::number(m.media().photo().sizes().at(1).size()));
            attachmsg.append(" \n Size : ");
            attachmsg.append(QString::number(m.media().photo().sizes().at(1).h()));
            attachmsg.append(" x ");
            attachmsg.append(QString::number(m.media().photo().sizes().at(1).w()));
            attachmsg.append(" px");

            db->insertQuery(
                    "insert into attachments(id,mediaId,accessHash,mimeType,type,addeddate,date)values(:id,:mediaId,:accessHash,:mimeType,:type,:addeddate,:date)",
                    attach);

        }
        msgmap.insert("id", m.id());
        msgmap.insert("date", m.date());
        msgmap.insert("fromId", m.fromId());
        if (m.toId().userId() > 0)
            msgmap.insert("toId", m.toId().userId());
        msgmap.insert("type", "contact");
        if (m.toId().chatId() > 0) {
            msgmap.insert("toId", m.toId().chatId());
            msgmap.insert("type", "chat");
        }
        msgmap.insert("fwdDate", m.fwdDate());
        msgmap.insert("fwdFromId", m.fwdFromId());
        bool read = false;
        if (m.unread() == false)
            read = true;
        msgmap.insert("read", read);
        msgmap.insert("message", attachmsg);
        msgmap.insert("mediaType",type);
        qDebug() << attachmsg;
        db->insertQuery(
                "insert into messagesTab(id,date,out,fromId,toId,fwdDate,fwdFromId,read,message,mediaType,attachmentId,type)values(:id,:date,:out,:fromId,:toId,:fwdDate,:fwdFromId,:read,:message,:mediaType,:attachmentId,:type)",
                msgmap);
    } else {
        msgmap.insert("id", m.id());
        msgmap.insert("date", m.date());
        msgmap.insert("fromId", m.fromId());
        if (m.toId().userId() > 0)
            msgmap.insert("toId", m.toId().userId());
        msgmap.insert("type", "contact");
        if (m.toId().chatId() > 0) {
            msgmap.insert("toId", m.toId().chatId());
            msgmap.insert("type", "chat");
        }
        msgmap.insert("fwdDate", m.fwdDate());
        msgmap.insert("fwdFromId", m.fwdFromId());
        bool read = false;
        if (m.unread() == false)
            read = true;
        msgmap.insert("read", read);
        if (message.classType() == Message::typeMessageService) {
            MessageAction ma = message.action();
            if (ma.classType() == MessageAction::typeMessageActionChatCreate
                    || ma.classType() == MessageAction::typeMessageActionChatEditTitle) {
                msgmap.insert("message", ma.title().append(" title changed"));
            } else if (ma.classType() == MessageAction::typeMessageActionChatAddUser
                    || ma.classType() == MessageAction::typeMessageActionChatDeleteUser) {
                msgmap.insert("message", " user added");
            } else if (ma.classType() == MessageAction::typeMessageActionChatEditPhoto) {
//                dumpPhoto(ma.photo());
            }
        } else {
            msgmap.insert("message", m.message());
            msgmap.insert("out", m.out());
                    db->insertQuery(
                            "insert into messagesTab(id,date,out,fromId,toId,fwdDate,fwdFromId,read,message,type)values(:id,:date,:out,:fromId,:toId,:fwdDate,:fwdFromId,:read,:message,:type)",
                            msgmap);
        }

    }
    if (m.toId().chatId() == 0) {
        if (m.toId().userId() == 0) {
            QVariantMap m_map;
            m_map["latestmessage"] = m.message();
            m_map["messagetime"] = m.date();
            m_map["id"] = m.fromId();
            m_map["messageread"] = !m.unread();
            m_map["messagemode"] = m.out();

            Q_EMIT liveupatemessage(m.fromId(), m.message(), m.unread(), m.date(), m.out());
            db->insertQuery(
                    "update contacts set messageread = :messageread,messagemode=:messagemode,latestmessage = :latestmessage,messagetime=:messagetime where id = :id",
                    m_map);
        } else {
            QVariantMap m_map;
            m_map["latestmessage"] = m.message();
            m_map["messagetime"] = m.date();
            m_map["id"] = m.toId().userId();
            m_map["messageread"] = !m.unread();
            m_map["messagemode"] = m.out();
            Q_EMIT liveupatemessage(m.toId().userId(), m.message(), m.unread(), m.date(), m.out());
            bool insert =
                    db->insertQuery(
                            "update contacts set messageread = :messageread,messagemode=:messagemode,latestmessage = :latestmessage,messagetime=:messagetime where id = :id",
                            m_map);
            qDebug() << "Update tesitn " << insert;
        }
    } else {
        QVariantMap m_map;
        m_map["latestmessage"] = m.message();
        m_map["messagetime"] = m.date();
        m_map["id"] = m.toId().chatId();
        m_map["messageread"] = !m.unread();
        m_map["messagemode"] = m.out();

        Q_EMIT liveupatemessage(m.toId().chatId(), m.message(), m.unread(), m.date(), m.out());
        db->insertQuery(
                "update contacts set messageread = :messageread,messagemode=:messagemode,latestmessage = :latestmessage,messagetime=:messagetime where id = :id",
                m_map);
    }
//    if (m.toId().userId() == 0 && m.toId().chatId() == 0) {
//
//    }
//    else if(m.fromId()==currentUser.userid){
//
//    }else if(m.toId().chatId()>0){
//
//     }
}
void ApplicationUI::onMessagesSendMessageAnswer(qint64 id, qint32 messageId, qint32 date,
        qint32 pts, qint32 seq, const QList<ContactsLink> &links)
{
    Q_FOREACH (ContactsLink link, links){
    Dumper::dumpContactsLink(link);
}
QList<qint32> msg;
msg << messageId;
mApi->messagesGetMessages(msg);
}

void ApplicationUI::onMessagesSentMessage(qint64 id, qint32 msgId, qint32 date, qint32 pts,
        qint32 seq)
{
    QList<ContactsLink> links;
    Q_EMIT onMessagesSendMessageAnswer(id, msgId, date, pts, seq, links);
}

void ApplicationUI::onStatusUpdate(const UserStatus &status, qint32 userid)
{
    qDebug() << userid << status.expires() << status.wasOnline();
    switch (status.classType()) {
        case UserStatus::typeUserStatusEmpty:
            break;
        case UserStatus::typeUserStatusOffline: {
            QVariantMap map;
            map["id"] = userid;
            map["wasonline"] = status.wasOnline();
            db->insertQuery("update contacts set wasonline = :wasonline where id = :id", map);
            break;
        }
        case UserStatus::typeUserStatusOnline: {
            QVariantMap map;
            map["id"] = userid;
            map["wasonline"] = status.wasOnline();
            db->insertQuery("update contacts set wasonline = :wasonline where id = :id", map);
            break;
        }
    }
//    Q_EMIT reloadContacts();
}

bool ApplicationUI::deleteMessage(int id)
{
    QString del = "delete from messagesTab where id =:id";
    QString attachdel =
            "delete from attachments where id in (select attachmentId from messagesTab where id = :id)";
    QVariantMap map;
    map["id"] = id;
    db->insertQuery(attachdel, map);
    QList<qint32> list;
    list << id;
    mApi->messagesDeleteMessages(list);
//    Q_EMIT deleteAccountLive(id);
    return !db->insertQuery(del, map);
}
bool ApplicationUI::addToFavourites(qint32 id)
{
    int num = db->getFavCount();
    if (num < 6) {
        qDebug() << "hello im there hers ";
        QString updatequery = "update contacts set favourite = \"true\" where id = :id";
        QVariantMap map;
        map["id"] = id;
        Q_EMIT reloadContacts();
        return true;
    } else {

        qDebug() << "you have enough favourites of contacts thank you" << num;
        return false;
    }
}
GroupDataModel * ApplicationUI::getFavContactsModel()
{
    QString favquery = "select * from contacts where favourite = \"true\"";
    GroupDataModel *favmodel = new GroupDataModel(QStringList());
    QVariant favcontacts = db->executeSqlQuery(favquery, QVariantMap());
    favmodel->insertList(favcontacts.value<QVariantList>());
    return favmodel;
}

bool ApplicationUI::deleteFavContacts(qint32 id)
{
    QVariantMap map;
    map.insert("id", id);
    bool isdelete = db->insertQuery("update contacts set favourite = \"false\" where id = :id",
            map);
    Q_EMIT reloadContacts();
    return !isdelete;
}

void ApplicationUI::clearContactHistory(qint32 id, QString type)
{
    if (type == "broadcast") {
        db->clearContactHistory(id, type);
    } else {
        InputPeer peer = askForInputPeer(QString::number(id), type);
        mApi->messagesDeleteHistory(peer, 0);
        db->clearContactHistory(id, type);
    }
}

void ApplicationUI::blockUserContact(qint64 id)
{
    InputUser user = askForInputUser("contact", id);
    mApi->contactsBlock(user);

}
void ApplicationUI::onContactsBlockAnswer(bool test)
{
    qDebug() << "User Been deleted" << test;
}
void ApplicationUI::unBlockContact(qint64 id)
{
    InputUser user = askForInputUser("contact", id);
    mApi->contactsUnblock(user);
}

void ApplicationUI::copyText(QByteArray text)
{
    bb::system::Clipboard clipboard;
    clipboard.clear();
    clipboard.insert("text/plain", text);
    bb::system::SystemToast *toast = new SystemToast(this);
    toast->setBody("Copied to clipboard.");
    toast->show();
}

void ApplicationUI::forwardMessage(qint32 msgid, qint32 userid, qint32 rand, QString type)
{

    InputPeer user = askForInputPeer(QString::number(userid), type);
    mApi->messagesForwardMessage(user, msgid, rand);
}

GroupDataModel* ApplicationUI::getContactsNamesModel(QString type)
{
    QString contactquery;
    QVariantMap map;
    GroupDataModel *contactsmodel = new GroupDataModel(QStringList() << "firstname");
    QVariant favcontacts;
    if (type.isEmpty()) {
        contactquery = "select * from contacts order by firstname desc";
        favcontacts = db->executeSqlQuery(contactquery, QVariantMap());
    } else {
        contactquery = "select * from contacts  where type = :type order by firstname desc";
        map.insert("type", type);
        favcontacts = db->executeSqlQuery(contactquery, map);
    }

    contactsmodel->insertList(favcontacts.value<QVariantList>());
    return contactsmodel;
}
qint32 ApplicationUI::getStashFwdMessage()
{
    return stashfwdmessage;

}
void ApplicationUI::stashFwdMessage(qint32 msgid)
{
    stashfwdmessage = msgid;
}
void ApplicationUI::flushStashFwdMessage()
{
    stashfwdmessage = 0;
}

void ApplicationUI::contactsBlockAnswer(qint64 userid, bool tr)
{
    qDebug() << userid << tr;
}

void ApplicationUI::listofblockedUsers()
{
    mApi->contactsGetBlocked(0, 50);
}
void ApplicationUI::onContactsBlocked(qint64 id, const QList<ContactBlocked> &blocked,
        const QList<User> &users)
{
    QVariantList list;
    Q_FOREACH(User item, users){
    qDebug() << item.firstName() << item.id();
    QVariantMap map;
    map["firstName"] = item.firstName();
    map["id"] = item.id();
    map["lastName"] = item.lastName();
    list <<map;
}
    GroupDataModel* blockedContactsModel = new GroupDataModel();
    blockedContactsModel->insertList(list);
    Q_EMIT loadBlockedUsers(blockedContactsModel);
}

void ApplicationUI::contactsUnBlockAnswer(qint64 id, bool ok)
{
    qDebug() << "Unblocking user status " << id << ok;
    listofblockedUsers();
}

void ApplicationUI::createGroup(QString title, QVariant id)
{
    QList<InputUser> list;
    Q_FOREACH(QVariant ids, id.toList()){
    bool ok = true;

    InputUser user = askForInputUser("contact", ids.toInt(&ok) );
    list << user;
}
    mApi->messagesCreateChat(list, title);
}

void ApplicationUI::addGroupMember(qint32 id, qint32 limit)
{
    InputUser user = askForInputUser("contact", id);
    mApi->messagesAddChatUser(popgroupid, user, limit);
    Message m;
    m.setId(rand() % 1000000000);
    m.setFromId(popgroupid);
    m.setMessage("user added");
    m.setUnread(1);

    m.setDate(QDateTime::currentDateTime().toTime_t());
    Peer peer(Peer::typePeerChat);
    peer.setChatId(popgroupid);
    m.setToId(peer);
    insertMessage(m);
    Q_EMIT updatemessage(db->getMessageById(m.id()));

}

void ApplicationUI::onMessagesAddChatUserStatedMessage(qint64 id, Message message,
        QList<Chat> chats, QList<User> users, qint32 pts, qint32 seq)
{
    qint32 chatid = 0;
    chatid = chats.at(0).id();
    qDebug() << "on new users";
    qDebug() << chatid;
    mApi->messagesGetFullChat(chatid);
}
void ApplicationUI::leaveGroup(qint32 id)
{
    InputUser user = askForInputUser("self", currentUser.userid);
    mApi->messagesDeleteChatUser(id, user);

}

void ApplicationUI::deleteGroup(qint32 id)
{
    QVariantMap map;
    map["id"] = id;
    db->insertQuery("delete from contacts where id=:id", map);
    Q_EMIT deleteliveuser(id);
}
void ApplicationUI::getGroupMembers(qint32 id)
{
    mApi->messagesGetFullChat(id);
}

void ApplicationUI::onMessagesGetFullChatAnswer(qint64 id, const ChatFull &fullChat,
        const QList<Chat> &chats, const QList<User> &users)
{
    QVariantList list;
    Q_FOREACH(User item, users){
    qDebug() << item.firstName() << item.id() << item.lastName();
    QVariantMap map;
    map["firstName"] = item.firstName();
    map["id"] = item.id();
    map["lastName"] = item.lastName();
    list <<map;
}
    QString chattitle;
    qint32 part;
    qint32 grpid;
    bool active;

    Q_FOREACH(Chat chat, chats ){
    qDebug() << chat.title();
    chattitle = chat.title();
    part = chat.participantsCount();
    grpid = chat.id();
    active = chat.left();
}
    GroupDataModel* blockedContactsModel = new GroupDataModel();
    blockedContactsModel->insertList(list);
    Q_EMIT loadGroupMembers(blockedContactsModel);
    Q_EMIT loadGroupInfo(chattitle, part, grpid, active);
}

void ApplicationUI::removeGroupMember(qint32 groupid, qint32 id)
{
    InputUser user = askForInputUser("contact", id);
    mApi->messagesDeleteChatUser(groupid, user);
    Message m;
    m.setId(rand() % 1000000000);
    m.setFromId(10);
    m.setMessage("user removed");
    m.setUnread(1);
    m.setDate(QDateTime::currentDateTime().toTime_t());
    Peer peer(Peer::typePeerChat);
    peer.setChatId(groupid);
    m.setToId(peer);
    insertMessage(m);
    Q_EMIT updatemessage(db->getMessageById(m.id()));

}

void ApplicationUI::onMessagesDeleteChatUserStatedMessage(qint64 msgId, Message message,
        QList<Chat> chats, QList<User> users, qint32 pts, qint32 seq)
{
    qint32 chatid;
    Q_FOREACH(Chat chat, chats ){
    chatid = chat.id();
}
    getGroupMembers(chatid);

}

void ApplicationUI::getGroupInfo(qint32 id)
{
    QList<qint32> chatids;
    chatids << id;
    mApi->messagesGetChats(chatids);
}
void ApplicationUI::messagesGetChatsAnswer(qint64 id, const QList<Chat> &chats,
        const QList<User> &users)
{
    QString chattitle;
    qint32 part;
    qint32 grpid;
    bool active;
    Q_FOREACH(Chat chat, chats ){
    chattitle = chat.title();
    part = chat.participantsCount();
    active = chat.left();
}
Q_EMIT
    loadGroupInfo(chattitle, part, grpid, active);
}

void ApplicationUI::saveGroupInfo(QString title)
{
    mApi->messagesEditChatTitle(popgroupid, title);
}

void ApplicationUI::onMessagesEditChatTitleStatedMessage(qint64, Message, QList<Chat>, QList<User>,
        qint32, qint32)
{
    Q_EMIT onMessagesEditChatTitleUpdated();
}

bool ApplicationUI::createBroadCast(QString title, QVariant info, qint32 random)
{
    bool ok = true;
    Q_FOREACH(QVariant ids, info.toList()){
    QVariantMap map;
    map.insert("broadcastid", random);
    map.insert("userid", ids.toInt(&ok));
    int usercheck = random + ids.toInt(&ok);
    map.insert("usercheck", usercheck);
    bool brod =
    db->insertQuery(
            "insert into broadCast( broadcastid, userid, usercheck)values( :broadcastid, :userid, :usercheck)",
            map);
}
    QVariantMap cmap;
    cmap["id"] = random;
    cmap["messagetime"] = QDateTime::currentDateTime().toTime_t();
    cmap["favourite"] = 0;
    cmap["firstname"] = title;
    cmap["wasonline"] = QDateTime::currentDateTime().toTime_t();
    cmap["type"] = "broadcast";
    bool contactsquery =
            db->insertQuery(
                    "insert into contacts(id,firstname,messagetime,favourite,wasonline,type)values(:id,:firstname,:messagetime,:favourite,:wasonline,:type)",
                    cmap);
    if (!contactsquery) {
        Q_EMIT accountcreated(getContactInfo(random));
        return true;
    } else
        return false;
}
//
bool ApplicationUI::addBroadCastMember(qint32 bid, qint32 id)
{
    QVariantMap map;
    map.insert("broadcastid", bid);
    map.insert("userid", id);
    int usercheck = bid + id;
    map.insert("usercheck", usercheck);
    bool brod =
            db->insertQuery(
                    "insert into broadCast(broadcastid,userid,usercheck)values(:broadcastid,:userid,:usercheck)",
                    map);
    Q_EMIT broadcastlistmembersupdate();
    return !brod;

}
bool ApplicationUI::removeBroadCastMember(qint32 bid, qint32 id)
{
    QVariantMap map;
    map["broadcastid"] = bid;
    map["userid"] = id;
    bool exe = db->insertQuery(
            "delete from broadCast where broadcastid=:broadcastid and userid = :userid", map);
    Q_EMIT broadcastlistmembersupdate();
    return !exe;

}
GroupDataModel* ApplicationUI::getBroadCastMembers(qint32 bid)
{
    QVariantMap map;
    map["broadcastid"] = bid;
    GroupDataModel* modelg = new GroupDataModel;

    QVariant data =
            db->executeSqlQuery(
                    "select firstname,id from contacts where contacts.id in(select userid from broadcast where broadCast.broadcastid = :broadcastid)",
                    map);

    QVariantList list = data.value<QVariantList>();
    modelg->insertList(list);
    qDebug() << "Broast Members List" << modelg->size();
    return modelg;
}
//
bool ApplicationUI::removeBroadCast(qint32 bid)
{
    QVariantMap map;
    map["broadcastid"] = bid;
    bool exe = db->insertQuery("delete from broadCast where broadcastid=:broadcastid", map);
    bool execontact = db->insertQuery("delete from contacts where id=:broadcastid", map);
    bool exemsg = db->insertQuery("delete from messagesTab where toId=:broadcastid", map);
    if (!exe && !execontact) {
        qDebug() << "Removing account " << " type : " << "broadcast " << bid;
        Q_EMIT deleteliveuser(bid);
        return true;
    } else
        return false;
}
int ApplicationUI::getPopBroadCastId()
{
    return popbroadcastid;
}
void ApplicationUI::setPopBroadCastId(int bid)
{
    popbroadcastid = bid;
}
void ApplicationUI::clearBroadCastHistory(qint32 bid)
{
    QVariantMap map;
    map["broadcastid"] = bid;
    bool exe = db->insertQuery("delete from messagesTab where toId=:broadcastid", map);
}

QVariant ApplicationUI::getContactInfo(qint32 id)

{
    QVariantMap map;
    map["id"] = id;
    return db->executeSqlQuery("select *from contacts where id = :id", map);

}

void ApplicationUI::sendMessageBroadCast(qint32 id, QString text, int random)
{
    QList<InputUser> contacts;
    QVariantMap map;
    map["id"] = id;
    QVariant data = db->executeSqlQuery("select userid from broadCast where broadcastid = :id",
            map);
    QVariantList list = data.value<QVariantList>();
    Q_FOREACH(QVariant var,list){
    QVariantMap mapp = var.value<QVariantMap>();
    contacts << askForInputUser("contact",mapp.value("userid").toInt());
}
    InputMedia inputMedia(InputMedia::typeInputMediaEmpty);
    mApi->messagesSendBroadcast(contacts, text, inputMedia);
    QVariantMap cmap;
    cmap.insert("latestmessage", text);
    cmap.insert("messagetime", QDateTime::currentDateTime().toTime_t());
    cmap.insert("id", id);
    db->executeSqlQuery(
            "update contacts set latestmessage = :latestmessage,messagetime=:messagetime where id =:id",
            cmap);
    QVariantMap msgmap;
    msgmap.insert("fomId", currentUser.userid);
    msgmap.insert("toId", id);
    msgmap.insert("message", text);
    msgmap.insert("read", 0);
    msgmap.insert("id", random);
    msgmap.insert("date", QDateTime::currentDateTime().toTime_t());
    msgmap.insert("out", 1);
    msgmap.insert("type", "broadcast");
    Q_EMIT liveupatemessage(id, text, read, QDateTime::currentDateTime().toTime_t(), 1);
    db->insertQuery(
            "insert into messagesTab(id,out,date,fromId,toId,message,read,type)values(:id,:out,:date,:fromId,:toId,:message,:read,:type)",
            msgmap);

}

QVariantList ApplicationUI::liveChat(qint32 uid, qint32 offset, QString type)
{
    qDebug() << uid << offset << type;
    QVariantMap map;
    map["uid"] = uid;
    map["msgid"] = offset;
    map["type"] = type;
    QVariant data = db->executeSqlQuery(
            "select * from messagesTab where fromId = :uid and id > :msgid and type = :type", map);

    QVariantList umap = data.value<QVariantList>();
    qDebug() << "the data" << umap.size();
    return umap;

}

void ApplicationUI::deleteContact(qint32 userid)
{
    InputUser user = askForInputUser("contact", userid);
    mApi->contactsDeleteContact(user);
}

void ApplicationUI::deletecontactlocal(qint32 popuserid)
{
    QVariantMap map;
    map["id"] = popuserid;
    db->executeSqlQuery("delete from contacts where id  = :id", map);
}

void ApplicationUI::contactsDeleteContactAnswer(qint64 msg, ContactsMyLink local,
        ContactsForeignLink foregin, User user)
{
    Q_EMIT contactDeleteoOnServer(true);
}
void ApplicationUI::boolcheck()
{
    Q_EMIT contactDeleteoOnServer(true);
}

bool ApplicationUI::contactSaveInfo(QString name)
{
    QVariantMap map;
    map["firstname"] = name;
    map["id"] = popuserid;
    bool update = db->insertQuery("update contacts set firstname=:firstname where id  = :id", map);
    return !update;
}

void ApplicationUI::AccountupdateProfile(QString first, QString last)
{
    mApi->accountUpdateProfile(first, last);

}
void ApplicationUI::accountUpdateProfileAnswer(qint64 id, User user)
{
    Q_EMIT accountUserInfo(user.firstName(), user.lastName(), user.phone(), user.username(),
            user.id());
    getCurrentUser();
}

void ApplicationUI::fetchUserDetails()
{
    Q_EMIT accountUserInfo(currentUser.firstName, currentUser.lastName, currentUser.phoneNumber,
            currentUser.username, currentUser.userid);

}

QVariantList ApplicationUI::searchFilterChatModel(QString type, QString search)
{
    QString q;
    QVariantMap map;
    ArrayDataModel *model = new ArrayDataModel();
    if (type.isEmpty()) {
        q =
                "select firstname,lastname,latestmessage,type,id,wasonline from contacts where firstname like \"%";
        q.append(search);
        q.append("%\" order by firstname desc");
    } else {
        map.insert("type", type);
        q =
                "select firstname,lastname,latestmessage,type,id,wasonline from contacts where type = :type and firstname like \"%";
        q.append(search);
        q.append("%\" order by firstname desc");
    }
    QVariant list = db->executeSqlQuery(q, map);
    model->clear();
    model->insert(0, list.value<QVariantList>());
    return list.value<QVariantList>();
}
void ApplicationUI::alert(QString message)
{
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->show();
}

void ApplicationUI::logout()
{
    mApi->authLogOut();
}

void ApplicationUI::onAuthLogOutAnswer(qint64 id, bool ok)
{
    mDcProvider.logOut();
    db->executeQuery("delete from contacts");
    db->executeQuery("delete from messagesTab");
    db->executeQuery("delete from attachments");
    db->executeQuery("delete from broadCast");
    //TODO Remove the files inside the data/cache folder
    Q_EMIT quitapp();
}

void ApplicationUI::markReadMessages(qint32 msgid, qint32 userid, QString type)
{
    InputPeer peer = askForInputPeer(QString::number(userid), type);
    mApi->messagesReadHistory(peer, msgid, 0);
}

void ApplicationUI::chatCatchUp(QString pop, qint32 msgid, QString type)
{
    InputPeer peer = askForInputPeer(pop, type);
    mApi->messagesGetHistory(peer, 0, msgid, 100);
}

void ApplicationUI::onMessagesCreateChatStatedMessage(qint64 id, Message message, QList<Chat> chats,
        QList<User> users, qint32 pts, qint32 seq)
{
    mApi->messagesGetDialogs(0, 0, 100);
}

void ApplicationUI::SaveSettings(QString name, QString base)
{
    QVariantMap map;
    map["type"] = name;
    map["value"] = base;
    qDebug() << "type" << name << "value" << base;
    QString query = "select count(*) from settings where type =\"";
    query.append(name);
    query.append("\"");
    int setsize = db->getTableSizeByQuery(query);
    if (setsize > 0)
        db->insertQuery("update settings set value= :value where type  = :type", map);
    else
        setSettings(name, base);
}
QVariant ApplicationUI::getSettings(QString type)
{
    QVariantMap map;
    map["type"] = type;
    QVariant var = db->executeSqlQuery("select * from settings where type = :type", map);
    return var;

}
void ApplicationUI::setSettings(QString name, QString base)
{
    QVariantMap map;
    map["type"] = name;
    map["value"] = base;
    bool test = db->insertQuery("insert into settings(type,value) values(:type,:value)", map);
}

qint64 ApplicationUI::uploadGetFile(const InputFileLocation &location, qint32 fileSize,
        qint32 dcNum)
{
    // change of dc if received a dcNum
    DC *dc;
    dcNum ? dc = mDcProvider.getDc(dcNum) : dc = mDcProvider.getWorkingDc();
    Session *session = mApi->fileSession(dc);
    DownloadFile *f = new DownloadFile(session, location, fileSize, this);
    mActiveDownloadsMap.insert(f->id(), true);

    switch (session->state()) {
        case QAbstractSocket::ConnectingState: {
            mInitialDownloadsMap[session->sessionId()].append(f);
            break;
        }
        case QAbstractSocket::ConnectedState: {
            qint64 msgId = mApi->uploadGetFile(session, f->fileLocation());
            mDownloadsMap.insert(msgId, f);
            break;
        }
        default: {
            QList<DownloadFile *> sessionInitialFiles;
            sessionInitialFiles << f;
            mInitialDownloadsMap.insert(session->sessionId(), sessionInitialFiles);
            connect(session, SIGNAL(sessionReady(DC*)), this,
                    SLOT(onUploadGetFileSessionCreated()));
            session->connectToServer();
            break;
        }
    }

    // return file id as request id for apps
    return f->id();
}

void ApplicationUI::onUploadGetFileSessionCreated()
{
    Session *session = qobject_cast<Session *>(sender());
    QList<DownloadFile *> sessionInitialFiles = mInitialDownloadsMap.take(session->sessionId());
    Q_FOREACH (DownloadFile *f, sessionInitialFiles){
    qint64 msgId = mApi->uploadGetFile(session, f->fileLocation());
    mDownloadsMap.insert(msgId, f);
}
}

void ApplicationUI::onUploadGetFileAnswer(qint64 msgId, StorageFileType type, qint32 mtime,
        QByteArray bytes)
{
    DownloadFile *f = mDownloadsMap.take(msgId);
    Q_ASSERT(f);

    if (mCancelDownloadsMap.take(f->id())) {
//        Q_EMIT uploadCancelFileAnswer(f->id(), true);
        finishDownload(f);
        return;
    }
    // if 'managedDownloads' flag is set, store all temporal received data and return a unique event
    // when file downloading is finished. Emit an event for every part otherwise.
    if (Settings::getInstance()->managedDownloads()) {
        qint32 expectedSize = f->expectedSize();
        if (expectedSize == 0 && (bytes.length() < BLOCK || bytes.length() == 0)) {
            expectedSize = f->offset();
        }
        // partial bytes appended to DownloadFile object
        f->appendBytes(bytes);

        if (expectedSize == 0 || f->bytes().length() < expectedSize) {
            qint64 newMsgId = mApi->uploadGetFile(f->session(), f->fileLocation(), f->offset(),
                    f->partLength());
            mDownloadsMap.insert(newMsgId, f);
        } else {
            qDebug() << f->length() << expectedSize;
            onUploadGetFileAnswer(f->id(), type, mtime, f->bytes(), 0, f->length(), expectedSize); //emit signal of finished
            finishDownload(f);
        }
    } else {
        qint32 downloaded = f->partId() * f->partLength() + bytes.length();
        qint32 thisPartId = f->partId();
        qint32 expectedSize = f->expectedSize();

        f->incrementPartId();
        f->setOffset(f->offset() + bytes.length());

        if (expectedSize == 0 && (bytes.length() < BLOCK || bytes.length() == 0)) {
            expectedSize = f->offset();
        }
        onUploadGetFileAnswer(f->id(), type, mtime, bytes, thisPartId, downloaded, expectedSize);
        if (expectedSize == 0 || f->offset() < expectedSize) {
            qint64 newMsgId = mApi->uploadGetFile(f->session(), f->fileLocation(), f->offset(),
                    f->partLength());
            mDownloadsMap.insert(newMsgId, f);
        } else {
            finishDownload(f);
        }
    }
}

qint64 ApplicationUI::uploadCancelFile(qint64 fileId)
{
    if (!mActiveDownloadsMap.value(fileId)) {
        // return signal of cancelled file, setting the 'cancelled' flag to false, only if the file is not into active
        // downloads. Otherwise, the signal will be emitted when received next part of the file
//        Q_EMIT uploadCancelFileAnswer(fileId, false);
    } else {
        mCancelDownloadsMap.insert(fileId, true);
    }
    return fileId;
}

void ApplicationUI::finishDownload(DownloadFile *f)
{
    mActiveDownloadsMap.remove(f->id());
    delete f;
    f = 0;
}

void ApplicationUI::onUploadGetFileAnswer(qint64 id, StorageFileType type, qint32 mtime,
        QByteArray bytes, qint32 partId, qint32 downloaded, qint32 total)
{
//    qDebug() << "uploadGetFileAnswer SIGNAL received, id:" << id;
//    qDebug() << "modificationTime:" << Dumper::unixTimeToDate(mtime);
//    qDebug() << "partId:" << partId;
//    qDebug() << "size:" << bytes.length();
//    qDebug() << "downloaded:" << downloaded;
//    qDebug() << "total:" << total;
//    qDebug() << "calculated percentage:" << (downloaded * 100 / total) << "%";
//    Dumper::dumpStorageFileType(type);

    if (!m_downloadFile.isOpen()) {
        QString in = "select id,type from attachments where messageid=\"" + QString::number(id)
                + "\"";
        qDebug() << "query string" << in;
        QSqlQuery query = db->executeQuery(in);
        qint64 m_id;
        qDebug() << "msg id after conversion" << QString::number(id) << "before" << id;
        qDebug() << "query results" << query.size();
        while (query.next()) {
            bool ok = true;
            m_id = query.value(0).value<qint64>();
            qDebug() << "check " << query.value(0);
        }

        QString ext;
        switch (type.classType()) {
            case StorageFileType::typeStorageFileUnknown:
                ext = "unknown";
                break;
            case StorageFileType::typeStorageFileJpeg:
                ext = "jpeg";
                break;
            case StorageFileType::typeStorageFileGif:
                ext = "gif";
                break;
            case StorageFileType::typeStorageFilePng:
                ext = "png";
                break;
            case StorageFileType::typeStorageFilePdf:
                ext = "pdf";
                break;
            case StorageFileType::typeStorageFileMp3:
                ext = "mp3";
                break;
            case StorageFileType::typeStorageFileMov:
                ext = "mov";
                break;
            case StorageFileType::typeStorageFileMp4:
                ext = "mp4";
                break;
            case StorageFileType::typeStorageFileWebp:
                ext = "webp";
                break;
        }
        QString absPath = QDir::homePath() + "/" + QString::number(m_id) + "." + ext;
        qDebug() << absPath << id << m_id;
        m_downloadFile.setFileName(absPath);
        m_downloadFile.open(QIODevice::WriteOnly);
    }

    m_downloadFile.write(bytes);

    if ((type.classType() != StorageFileType::typeStorageFilePartial)
            || (m_downloadFile.size() >= m_downloadFileExpectedLength)) {
        m_downloadFile.close();
        QSqlQuery query = db->executeQuery(
                QString(
                        "select type,id from attachments where messageid=\"" + QString::number(id)
                                + "\""));
        QString type;
        qint64 did;
        while (query.next()) {
            type = query.value(0).toString();
            did = query.value(1).value<qint64>();
        }
        qDebug() << type << m_downloadFile.fileName();
        if (type == "userphotomsgid") {
            qDebug() << type << m_downloadFile.fileName();
            Q_EMIT profileimage(did);

        } else if (type == "userprofile") {
            qDebug() << "file ind photos" << QFile(m_downloadFile.fileName()).exists()
                    << "filepath\n" << m_downloadFile.fileName();
            getIdByPhotoId(did);
        }
        else if(type == "messageimage"){
            getmessageofimage(did);
        }
    }
}

void ApplicationUI::searchMessage(QString q)
{
    InputPeer peer(InputPeer::typeInputPeerEmpty);
    MessagesFilter f(MessagesFilter::typeInputMessagesFilterEmpty);
    qDebug() << "Searching for Message";
    mApi->messagesSearch(peer, q, f, 0, 0, 0, 0, 50);
}

void ApplicationUI::onMessagesSearchAnswer(qint64 id, const QList<Message> &messages,
        const QList<Chat> &chats, const QList<User> &users)
{
    qDebug() << "messages:" << messages.size();
    qDebug() << "chats mentioned in messages:" << chats.size();
    qDebug() << "users mentioned in messages and chats:" << users.size();
    qDebug() << "-----";
    QMap<qint32, const User *> mentionedUsers;
    for (qint32 i = 0; i < users.size(); i++) {
        mentionedUsers.insert(users.at(i).id(), &(users.at(i)));
    }
    QMap<qint32, const Chat *> mentionedChats;
    for (qint32 i = 0; i < chats.size(); i++) {
        mentionedChats.insert(chats.at(i).id(), &(chats.at(i)));
    }

    Q_FOREACH (Message m, messages){
    const User *u = mentionedUsers.value(m.fromId());
    QString from = u->firstName() + " " + u->lastName();
    QString to;
    if (m.toId().classType() == Peer::typePeerUser) {
        const User *u = mentionedUsers.value(m.toId().userId());
        to = u->firstName() + " " + u->lastName();
    } else {
        const Chat *c = mentionedChats.value(m.toId().chatId());
        to = c->title();
    }
    qDebug() << "msgId:" << m.id() << "-" << from << " -> " << to << ":" << m.message();
}
}

void ApplicationUI::onMessagesForwardMsgStatedMessage(qint64 id, const Message &message,
        const QList<Chat> &chats, QList<User> users, qint32 pts, qint32 seq)
{
    qDebug() << id << message.message();
    insertMessage(message);
}

void ApplicationUI::setCheckInitial(qint32 id)
{
    QVariantMap map;
    map["init"] = false;
    map["id"] = id;
    db->executeSqlQuery("update contacts set init = :init where id = :id", map);
}
void ApplicationUI::setinitfalse(qint32 id)
{
    QVariantMap map;
    map.insert("id", id);
    db->insertQuery("update contacts set init = 0 where id = :id", map);
    db->insertQuery("delete from messagesTab where ((fromId = :id or toId=:id) and type='chat')",
            map);
}
QString ApplicationUI::getThemeSettings()
{
    QString themequery = "select * from settings where type = 'theme'";
    QSqlQuery themedata = db->executeQuery(themequery);
    QString theme;
    while (themedata.next()) {
        int fieldNo = themedata.record().indexOf("value");
        theme = themedata.value(fieldNo).toString();
        qDebug() << "this is the theme settings value" << theme;
    }
    return theme;
}
void ApplicationUI::setThemeColorVariable(QString str)
{

    themecolorvariable = str;

}
QString ApplicationUI::getThemeColorVariable()
{

    QString themequery = "select * from settings where type = 'themecolor'";
    QSqlQuery themedata = db->executeQuery(themequery);
    QString theme;
    while (themedata.next()) {
        int fieldNo = themedata.record().indexOf("value");
        theme = themedata.value(fieldNo).toString();
    }
    return theme;

}
QVariantMap ApplicationUI::getidtousers()
{
    return idtoname;
}
bool ApplicationUI::updateBroadCast(int id, QString name)
{
    QVariantMap map;
    map["firstname"] = name;
    map["id"] = id;
    bool upexecution = db->insertQuery("update contacts set firstname=:firstname where id = :id",
            map);
    return !upexecution;
}

bool ApplicationUI::updateGroupSettings(int id, QString name)
{
    QVariantMap map;
    map["firstname"] = name;
    map["id"] = id;
    bool upexecution = db->insertQuery("update contacts set firstname=:firstname where id = :id",
            map);
    return !upexecution;
}

bool ApplicationUI::updateContactSettings(int id, QString name)
{
    QVariantMap map;
    map["firstname"] = name;
    map["id"] = id;
    bool upexecution = db->insertQuery("update contacts set firstname=:firstname where id = :id",
            map);
    return !upexecution;
}

QVariant ApplicationUI::getMessageContacts(int id)
{
    qDebug() << "this is the id " << id;
    QVariantMap map;
    map["id"] = id;
    QString q =
            "select * from contacts cross join messagesTab where((messagesTab.fromId= :id or messagesTab.toId = :id) and contacts.id=:id)";
    QVariant data = db->executeSqlQuery(q, map);
    QVariantList list = data.value<QVariantList>();
    qDebug() << "thisi is the multiple query list data size" << list.size();
    return data;
}

void ApplicationUI::importContacts()
{
//    qDebug()<< "entered into the import contacts";
//    QList<InputContact> inputcontactlist;
//    LoadContacts lc;
//    QVariantList list = lc.loadContacts();
//    qDebug()<<"this is the list size"<<list.size();
//    foreach(QVariant var,list){
//        QVariantMap map = var.value<QVariantMap>();
//           InputContact ic;
//           ic.setClientId(map.value("id").toInt());
//           ic.setPhone(map.value("phone").toString());
//           ic.setFirstName(map.value("firstname").toString());
//           ic.setLastName(map.value("lastname").toString());
//           inputcontactlist << ic;
//       }
}
void ApplicationUI::getIdByPhotoId(qint64 id)
{
    QVariant v;
    QString q = "select id from contacts where photoid = \"" + QString::number(id) + "\"";
    int count = db->getTableSizeByQuery("select count(*) from contacts where photoid = \"" + QString::number(id) + "\"");

    QSqlQuery query = db->executeQuery(q);
    int fiel =  query.record().indexOf("id");
    while(query.next()){
        v = query.value(fiel);
    }

    Q_EMIT userprofile(v.value<qint32>(),id);
}
void  ApplicationUI::getmessageofimage(qint64 id){
    QVariant v;
    QString q = "select id from messagesTab where messageid = \"" + QString::number(id) + "\"";

    QSqlQuery query = db->executeQuery(q);
    int fiel =  query.record().indexOf("id");
    while(query.next()){
        v = query.value(fiel);
    }

    Q_EMIT userprofile(v.value<qint32>(),id);
}
