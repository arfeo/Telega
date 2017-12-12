/*
 * Copyright (c) 2017 Arfeo.net.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QMap>
#include <QtCore/QByteArray>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "engine/types/types.h"
#include "engine/core/dcprovider.h"
#include "engine/core/api.h"
#include "engine/database.hpp"
#include "engine/types/contact.h"
#include "engine/types/user.h"
#include "engine/logic/uploadfile.h"
#include "engine/logic/fileoperation.h"
#include "engine/logic/downloadfile.h"
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
    namespace system
    {
        class InvokeManager;
    }
}

class Message;
class QTranslator;
class Api;
class DcProvider;
class InvokeManger;
class QByteArray;
class QSqlQuery;
class Update;
class bb::cascades::ArrayDataModel;
class bb::cascades::GroupDataModel;
class UdpModule;
class Database;

class ApplicationUI : public QObject
{

    Q_OBJECT

public:

    ApplicationUI();
    virtual ~ApplicationUI() {}

    typedef
        struct{
            int userid;
            QString firstName;
            QString lastName;
            QString photoLocation;
            QString phoneNumber;
            QString username;
        } currentuser;
    QVariantMap idtoname;
    QString phone;
    QString hash;
    qint32 stashfwdmessage;
    QString themecolorvariable;
    bool reg;
    int popuserid;
    int popgroupid;
    int popbroadcastid;
    currentuser currentUser;
    Database *db;
    bool loggedIn;
    QString m_videoFilePath;
    QString m_videoThumbPath;
    QString m_audioFilePath;
    QString m_photoFilePath;
    QString m_docFilePath;
    QString m_docThumbPath;
    QFile m_downloadFile;
    qint64 m_downloadFileExpectedLength;
    DcProvider mDcProvider;
    Settings *s;
    Api *mApi;
    QList<Contact> m_cachedContacts;
    QList<User> m_cachedUsers;
    QVariantList mem;

    Q_INVOKABLE QString getAppSettings(const QString&, const QString&);
    Q_INVOKABLE void setAppSettings(const QString&, const QString&);
    Q_INVOKABLE bool init();
    Q_INVOKABLE QString checkLogin();
    Q_INVOKABLE void changeServer( qint16 number );
    Q_INVOKABLE qint32 workingdc();

    //Auths
    Q_INVOKABLE void authSendCode( QString ph);
    Q_INVOKABLE void authUser(QString code);
    Q_INVOKABLE bool isUserReg();
    Q_INVOKABLE void authUserSignIn(QString first, QString last, QString code);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void getcontacts();
    Q_INVOKABLE void sendping();

    // Models
    Q_INVOKABLE bool addToFavourites(qint32 id);
    Q_INVOKABLE bool deleteFavContacts(qint32 id);
    Q_INVOKABLE bb::cascades::GroupDataModel* getFavContactsModel();
    Q_INVOKABLE bb::cascades::GroupDataModel* getContactsNamesModel(QString);
    Q_INVOKABLE QVariantList searchFilterChatModel(QString type,QString search);

    // User
    Q_INVOKABLE void getCurrentUser();
    Q_INVOKABLE void fetchUserDetails();

    // Contacts
    Q_INVOKABLE QVariantList getContactsmodal(QString);
    Q_INVOKABLE void getmessages(qint32,QString,qint32);
    Q_INVOKABLE void clearContactHistory(qint32,QString);
    Q_INVOKABLE void listofblockedUsers();
    Q_INVOKABLE void blockUserContact(qint64 id);
    Q_INVOKABLE void unBlockContact(qint64 id);
    Q_INVOKABLE void importContacts();

    // Broadcast
    Q_INVOKABLE bool createBroadCast( QString title, QVariant id,qint32 random);
    Q_INVOKABLE bool addBroadCastMember(qint32 bid, qint32 id);
    Q_INVOKABLE bool removeBroadCastMember(qint32 bid, qint32 id);
    Q_INVOKABLE bool removeBroadCast(qint32 bid);
    Q_INVOKABLE void setPopBroadCastId(int);
    Q_INVOKABLE int getPopBroadCastId();
    Q_INVOKABLE void clearBroadCastHistory(qint32);
    Q_INVOKABLE QVariant getContactInfo(qint32 id);
    Q_INVOKABLE bb::cascades::GroupDataModel* getBroadCastMembers(qint32 bid);
    Q_INVOKABLE QVariantList liveChat(qint32, qint32, QString);
    Q_INVOKABLE bool contactSaveInfo(QString );
    Q_INVOKABLE bool updateBroadCast(qint32 id,QString name);
    Q_INVOKABLE void userdetails(QString userid);
    Q_INVOKABLE void settyping(QString userid, bool check);
    Q_INVOKABLE QVariantList getMessagesModel( qint32,int,QString);
    Q_INVOKABLE void getLatestMessages();
    Q_INVOKABLE void flushTable(QString);
    Q_INVOKABLE void sendTyping(QString,bool);
    Q_INVOKABLE void sendMessage(QString,qint64);
    Q_INVOKABLE void sendMessageChat(QString,qint64);
    Q_INVOKABLE void forwardMessage(qint32, qint32 ,qint32,QString);
    Q_INVOKABLE void copyText(QByteArray text);
    Q_INVOKABLE void setPopUserId(int);
    Q_INVOKABLE int getPopUserId();
    Q_INVOKABLE bool deleteMessage(int);
    Q_INVOKABLE void setPopGroupId(int);
    Q_INVOKABLE int getPopGroupId();
    Q_INVOKABLE bool updateGroupSettings(int id,QString name);
    Q_INVOKABLE bool updateContactSettings(int id,QString name);
    Q_INVOKABLE void stashFwdMessage(qint32);
    Q_INVOKABLE qint32 getStashFwdMessage();
    Q_INVOKABLE void flushStashFwdMessage();
    Q_INVOKABLE void leaveGroup(qint32 id);
    Q_INVOKABLE void getGroupMembers(qint32 id);
    Q_INVOKABLE void removeGroupMember(qint32 groupid, qint32 id);
    Q_INVOKABLE void createGroup(QString, QVariant);
    Q_INVOKABLE void addGroupMember(qint32,qint32);
    Q_INVOKABLE void getGroupInfo(qint32 id);
    Q_INVOKABLE void saveGroupInfo(QString);
    Q_INVOKABLE void dumpTable(QString tablename);
    Q_INVOKABLE void sendMessageBroadCast(qint32 id, QString text,int random);
    Q_INVOKABLE void deleteContact(qint32 userid);
    Q_INVOKABLE void deletecontactlocal(qint32 popuserid);
    Q_INVOKABLE void boolcheck();
    Q_INVOKABLE void AccountupdateProfile(QString first, QString last);
    Q_INVOKABLE void alert(QString);
    Q_INVOKABLE void markReadMessages(qint32, qint32 , QString);
    Q_INVOKABLE void chatCatchUp(QString, qint32,QString);
    Q_INVOKABLE void SaveSettings(QString, QString);
    Q_INVOKABLE QVariant getSettings(QString);
    Q_INVOKABLE void setSettings(QString, QString);
    Q_INVOKABLE void searchMessage(QString);
    Q_INVOKABLE void setCheckInitial(qint32);
    Q_INVOKABLE void setinitfalse(qint32 id);
    Q_INVOKABLE QString getThemeSettings();
    Q_INVOKABLE void setThemeColorVariable(QString str);
    Q_INVOKABLE QString getThemeColorVariable();
    Q_INVOKABLE QVariantMap getidtousers();
    Q_INVOKABLE void deleteGroup(qint32 id);
    Q_INVOKABLE QVariant getMessageContacts(int id);
    void getIdByPhotoId(qint64 id);
    void getmessageofimage(qint64 id);
    void closeconnection();
    void switchDC(QString number);
    void dcchange(QString codetext);
    void onMessagesGetHistoryAnswer(qint64 id, qint32 count, const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users);
    void insertMessage(const Message &message);
    qint64 uploadGetFile(const InputFileLocation &file, qint32 fileSize, qint32 dc);
    qint64 uploadCancelFile(qint64 fileId);
    void DownloadImageThumb(QString,qint64,FileLocation, qint32, qint64);

private
    slots:
        void onSystemLanguageChanged();
        void keyloaded();
    signals:
        void invalidphone(QString);
        void movingdcinit();
        void qmlgethashpane( QString ,bool);
        void authcreatiomcomplete();
        bb::cascades::GroupDataModel* getmodal( bb::cascades::GroupDataModel* model );
        void getuserdetails(QString userid);
        bool onkeyloaded(bool);
        void reloadContacts();
        void reloadChat();
        void usertyping(qint32);
        void stoptypinggroup(qint32);
        void stoptyping(qint32);
        void updatemessage(QVariantMap);
        void showerror(QString);
        void loadBlockedUsers(bb::cascades::GroupDataModel* model);
        void loadGroupMembers(bb::cascades::GroupDataModel* model);
        void loadGroupInfo( QString, qint32,qint32,bool);
        void contactDeleteoOnServer(bool);
        void contactsDeleteContactsAnswer(qint64,bool);
        void accountUserInfo(QString,QString,QString,QString,int);
        void livechatdeletemessage(QVariantMap);
        void usertypinggroup(QVariant);
        void stoptypinggroup(QVariant);
        void chatgroupcreated();
        void onMessagesEditChatTitleUpdated();
        void chatuserdeleted();
        void newmessage(QVariant,QString);
        void accountremove(QString, qint32);
        void accountcreated(QVariant);
        void messageread(qint32);
        void historyloaded();
        void quitapp();
        void liveupatemessage(qint32,QString,bool,qint32,bool);
        void deleteliveuser(qint32);
        void broadcastlistmembersupdate();
        void profileimage(qint64);
        void userprofile(qint32,qint64);
        void checkserver();
        void onDcProviderReady();

private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bb::system::InvokeManager* m_pInvokeManager;
    QMap<qint64, DownloadFile *> mDownloadsMap;
    // sessionId -> list of files waiting for session connection to be ready for being downloaded
    QMap<qint64, QList<DownloadFile *> > mInitialDownloadsMap;
    // fileId -> true if the file download is in progress. An entry does not exists for that fileId if not downloading
    QMap<qint64, bool> mActiveDownloadsMap;
    // fileId -> true if requested a download cancel for that fileId
    QMap<qint64, bool> mCancelDownloadsMap;
    bb::cascades::GroupDataModel* modal;
    InputPeer askForInputPeer(QString userid, QString sType);
    InputUser askForInputUser(QString, qint64);
    void liveUpdate(const Update &update);
    void finishDownload(DownloadFile *f);
    void onStatusUpdate(const UserStatus &status, qint32 userid);

private
    Q_SLOTS:
        void onserverready();
        void onauthTransferCompleted();
        void onauthSentCode(qint64,bool,QString);
        void onUserAuthorized(qint64 id, qint32 expires);
        void onContactsContacts(qint64 msgId, const QList<Contact> &contacts, const QList<User> &users);
        void onMessagesGetHistoryMessages(qint64 id, const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users);
        void onMessagesGetHistoryMessageSlice(qint64 id, qint32 count, const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users);
        void onMessagesGetDialogsAnswer(qint64,QList<Dialog>,QList<Message>,QList<Chat>,QList<User>);
        void onUsersGetUsersAnswer(qint64,QList<User>);
        void onContactsBlockAnswer(bool);
        void onUpdateShortMessage(qint32 id, qint32 fromId, const QString &message, qint32 pts, qint32 date, qint32 seq);
        void onUpdateShortChatMessage(qint32 id, qint32 fromId, qint32 chatId, const QString &message, qint32 pts, qint32 date, qint32 seq);
        void onUpdateShort(const Update &update, qint32 date);
        void onUpdatesCombined(const QList<Update> &updates, const QList<User> &users, const QList<Chat> &chats, qint32 date, qint32 seqStart, qint32 seq);
        void onUpdates(const QList<Update> &udts, const QList<User> &users, const QList<Chat> &chats, qint32 date, qint32 seq);
        void onMessagesSentMessage(qint64 id, qint32 msgId, qint32 date, qint32 pts, qint32 seq);
        void onMessagesSendMessageAnswer(qint64 id, qint32 messageId, qint32 date, qint32 pts, qint32 seq, const QList<ContactsLink> &links);
        void onAuthSignInError(qint64 msgid ,qint32 errcode ,QString errtext);
        void contactsBlockAnswer(qint64 userid ,bool tr);
        void onContactsBlocked(qint64 id, const QList<ContactBlocked> &blocked, const QList<User> &users);
        void contactsUnBlockAnswer(qint64 id, bool ok);
        void onMessagesGetFullChatAnswer(qint64 id, const ChatFull &fullChat, const QList<Chat> &chats, const QList<User> &users);
        void onMessagesDeleteChatUserStatedMessage(qint64 msgId, Message message, QList<Chat> chats, QList<User> users, qint32 pts, qint32 seq);
        void messagesGetChatsAnswer(qint64 id, const QList<Chat> &chats, const QList<User> &users);
        void contactsDeleteContactAnswer(qint64,ContactsMyLink,ContactsForeignLink,User);
        void accountUpdateProfileAnswer(qint64,User);
        void onAuthSendCodeError(qint64 msgid,qint32 code ,QString codetext);
        void onAuthLogOutAnswer(qint64 id, bool ok);
        void onMessagesCreateChatStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32);
        void onMessagesEditChatTitleStatedMessage(qint64,Message,QList<Chat>,QList<User>,qint32,qint32);
        void onUploadGetFileAnswer(qint64 msgId, StorageFileType type, qint32 mtime, QByteArray bytes);
        void onUploadGetFileAnswer(qint64 id, StorageFileType type, qint32 mtime, QByteArray bytes, qint32 partId, qint32 downloaded, qint32 total);
        void onUploadGetFileSessionCreated();
        void onMessagesAddChatUserStatedMessage(qint64 id, Message message, QList<Chat> chats, QList<User> users, qint32 pts, qint32 seq);
        void onMessagesSearchAnswer(qint64 id, const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users) ;
        void onMessagesForwardMsgStatedMessage(qint64 id, const Message &message, const QList<Chat> &chats,QList<User> users , qint32 pts, qint32 seq);

};

#endif
