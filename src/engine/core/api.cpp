/*
 * Copyright 2013 Vitaly Valtman
 * Copyright 2014 Canonical Ltd.
 * Authors:
 *      Roberto Mier
 *      Tiago Herrmann
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
 *
 */

#include "api.h"

#include "tlvalues.h"
#include "dcprovider.h"
#include <QtCore/QDataStream>
#include <QtCore/QByteArray>

Api::Api(Session *session, QObject *parent) :
    AbstractApi(session, parent)
{
    helpGetConfigMethods.onAnswer = &Api::onHelpGetConfigAnswer;
    helpGetConfigMethods.onError = &Api::onError; // this is not needed, because is the default one. Hereafter it won't be set explicitly

    helpGetInviteTextMethods.onAnswer = &Api::onHelpGetInviteTextAnswer;
    helpGetInviteTextMethods.onError = &Api::onErrorRetry;

    sendpingMethods.onAnswer = &Api::sendpingAnswer;

    authCheckPhoneMethods.onAnswer = &Api::onAuthCheckPhoneAnswer;
    authCheckPhoneMethods.onError = &Api::onErrorRetry;

    authSendCodeMethods.onAnswer = &Api::onAuthSendCodeAnswer;
    authSendCodeMethods.onError = &Api::onAuthSendCodeError;

    authSendCallMethods.onAnswer = &Api::onAuthSendCallAnswer;

    authSignInMethods.onAnswer = &Api::onAuthSignInAnswer;
    authSignInMethods.onError = &Api::onAuthSignInError;

    authSignUpMethods.onAnswer = &Api::onAuthSignUpAnswer;
    authSignUpMethods.onError = &Api::onAuthSignUpError;

    authLogOutMethods.onAnswer = &Api::onAuthLogOutAnswer;

    authSendInvitesMethods.onAnswer = &Api::onAuthSendInvitesAnswer;

    authResetAuthorizationsMethods.onAnswer = &Api::onAuthResetAuthorizationsAnswer;

    authImportAuthorizationMethods.onAnswer = &Api::onAuthImportAuthorizationAnswer;

    authExportAuthorizationMethods.onAnswer = &Api::onAuthExportAuthorizationAnswer;

    accountRegisterDeviceMethods.onAnswer = &Api::onAccountRegisterDeviceAnswer;

    accountUnregisterDeviceMethods.onAnswer = &Api::onAccountUnregisterDeviceAnswer;

    accountUpdateNotifySettingsMethods.onAnswer = &Api::onAccountUpdateNotifySettingsAnswer;

    accountGetNotifySettingsMethods.onAnswer = &Api::onAccountGetNotifySettingsAnswer;

    accountResetNotifySettingsMethods.onAnswer = &Api::onAccountResetNotifySettingsAnswer;

    accountUpdateProfileMethods.onAnswer = &Api::onAccountUpdateProfileAnswer;

    accountUpdateStatusMethods.onAnswer = &Api::onAccountUpdateStatusAnswer;

    photosUploadProfilePhotoMethods.onAnswer = &Api::onPhotosUploadProfilePhotoAnswer;

    photosUpdateProfilePhotoMethods.onAnswer = &Api::onPhotosUpdateProfilePhotoAnswer;

    usersGetUsersMethods.onAnswer = &Api::onUsersGetUsersAnswer;

    usersGetFullUserMethods.onAnswer = &Api::onUsersGetFullUserAnswer;

    photosGetUserPhotosMethods.onAnswer = &Api::onPhotosGetUserPhotos;

    contactsGetStatusesMethods.onAnswer = &Api::onContactsGetStatusesAnswer;

    contactsGetContactsMethods.onAnswer = &Api::onContactsGetContactsAnswer;

    contactsImportContactsMethods.onAnswer = &Api::onContactsImportContactsAnswer;
    contactsImportContactsMethods.onError = &Api::onErrorRetry;

    contactsDeleteContactMethods.onAnswer = &Api::onContactsDeleteContactAnswer;

    contactsDeleteContactsMethods.onAnswer = &Api::onContactsDeleteContactsAnswer;

    contactsBlockMethods.onAnswer = &Api::onContactsBlockAnswer;

    contactsUnblockMethods.onAnswer = &Api::onContactsUnblockAnswer;

    contactsGetBlockedMethods.onAnswer = &Api::onContactsGetBlockedAnswer;

    messagesGetLatestMessageMethods.onAnswer = &Api::onMessagesGetLatestMessageAnswer;
    messagesSendMessageMethods.onAnswer = &Api::onMessagesSendMessageAnswer;

    messagesSendMediaMethods.onAnswer = &Api::onMessagesSendMediaAnswer;

    messagesSetTypingMethods.onAnswer = &Api::onMessagesSetTypingAnswer;

    messagesGetMessagesMethods.onAnswer = &Api::onMessagesGetMessagesAnswer;

    messagesGetDialogsMethods.onAnswer = &Api::onMessagesGetDialogsAnswer;

    messagesGetHistoryMethods.onAnswer = &Api::onMessagesGetHistoryAnswer;

    messagesSearchMethods.onAnswer = &Api::onMessagesSearchAnswer;

    messagesReadHistoryMethods.onAnswer = &Api::onMessagesReadHistoryAnswer;

    messagesDeleteHistoryMethods.onAnswer = &Api::onMessagesDeleteHistoryAnswer;

    messagesDeleteMessagesMethods.onAnswer = &Api::onMessagesDeleteMessagesAnswer;

    messagesRestoreMessagesMethods.onAnswer = &Api::onMessagesRestoreMessagesAnswer;

    messagesReceivedMessagesMethods.onAnswer = &Api::onMessagesReceivedMessagesAnswer;

    messagesForwardMessageMethods.onAnswer = &Api::onMessagesForwardMessageAnswer;

    messagesForwardMessagesMethods.onAnswer = &Api::onMessagesForwardMessagesAnswer;

    messagesSendBroadcastMethods.onAnswer = &Api::onMessagesSendBroadcastAnswer;

    messagesGetChatsMethods.onAnswer = &Api::onMessagesGetChatsAnswer;

    messagesGetFullChatMethods.onAnswer = &Api::onMessagesGetFullChatAnswer;

    messagesEditChatTitleMethods.onAnswer = &Api::onMessagesEditChatTitleAnswer;

    messagesEditChatPhotoMethods.onAnswer = &Api::onMessagesEditChatPhotoAnswer;

    messagesAddChatUserMethods.onAnswer = &Api::onMessagesAddChatUserAnswer;

    messagesDeleteChatUserMethods.onAnswer = &Api::onMessagesDeleteChatUserAnswer;

    messagesCreateChatMethods.onAnswer = &Api::onMessagesCreateChatAnswer;

    messagesGetDhConfigMethods.onAnswer = &Api::onMessagesGetDhConfigAnswer;

    messagesRequestEncryptionMethods.onAnswer = &Api::onMessagesRequestEncryptionAnswer;

    messagesDiscardEncryptionMethods.onAnswer = &Api::onMessagesDiscardEncryptionAnswer;

    messagesAcceptEncryptionMethods.onAnswer = &Api::onMessagesAcceptEncryptionAnswer;

    messagesSetEncryptedTypingMethods.onAnswer = &Api::onMessagesSetEncryptedTypingAnswer;

    messagesReadEncryptedHistoryMethods.onAnswer = &Api::onMessagesReadEncryptedHistoryAnswer;

    messagesSendEncryptedMethods.onAnswer = &Api::onMessagesSendEncryptedAnswer;

    messagesSendEncryptedFileMethods.onAnswer = &Api::onMessagesSendEncryptedFileAnswer;

    messagesSendEncryptedServiceMethods.onAnswer = &Api::onMessagesSendEncryptedServiceAnswer;

    messagesReceivedQueueMethods.onAnswer = &Api::onMessagesReceivedQueueAnswer;

    updatesGetStateMethods.onAnswer = &Api::onUpdatesGetStateAnswer;

    updatesGetDifferenceMethods.onAnswer = &Api::onUpdatesGetDifferenceAnswer;

    uploadSaveFilePartMethods.onAnswer = &Api::onUploadSaveFilePartAnswer;

    uploadSaveBigFilePartMethods.onAnswer = &Api::onUploadSaveBigFilePartAnswer;

    uploadGetFileMethods.onAnswer = &Api::onUploadGetFileAnswer;
    uploadGetFileMethods.onError = &Api::onUploadGetFileError;

}

void Api::onError(Query *q, qint32 errorCode, const QString &errorText) {
    Q_EMIT error(q->msgId(), errorCode, errorText);
}

//
// REGISTRATION / AUTHORIZATION
//
// ### help-> getConfig()
void Api::onHelpGetConfigAnswer(Query *q, InboundPkt &inboundPkt) {
    qDebug() << "----------------------------";
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Config);
    qint32 date = inboundPkt.fetchInt();
    bool testMode = inboundPkt.fetchBool();
    qint32 thisDc = inboundPkt.fetchInt();
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    qDebug() << "Number of DCs found in response - " << n;
    QList<DcOption> dcOptions;
    for (qint32 i = 0; i < n; i++) {
        dcOptions.append(inboundPkt.fetchDcOption());
    }
    qint32 chatMaxSize = inboundPkt.fetchInt();
    qint32 broadcastMaxSize = inboundPkt.fetchInt();
    Q_EMIT config(q->msgId(), date, testMode, thisDc, dcOptions, chatMaxSize, broadcastMaxSize);
}

qint64 Api::createnewsession(){
    OutboundPkt *p = new OutboundPkt();

        if (mMainSession->initConnectionNeeded()) {
            p->initConnection();
            mMainSession->setInitConnectionNeeded(false);
        }
        p->appendInt(TL_getNearstDC);
        qint64 ret = mMainSession->sendQuery(*p, &helpGetConfigMethods);
        delete p;
        return ret;

}

qint64 Api::helpGetConfig() {
    qDebug() << "Calling Get Config Stuff";
    OutboundPkt *p = new OutboundPkt();

    if (mMainSession->initConnectionNeeded()) {
        p->initConnection();
        mMainSession->setInitConnectionNeeded(false);
    }
    p->appendInt(TL_HelpGetConfig);
//    qDebug()<< "tl help get config data "<<QString::number(TL_HelpGetConfig,16) <<  **p;
    qint64 msession =  mMainSession->sendQuery(*p, &helpGetConfigMethods);
    delete p;
    return msession;
}

void Api::onHelpGetInviteTextAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_HelpInviteText);
    QString textInvite = inboundPkt.fetchQString();
    Q_EMIT helpGetInviteTextAnswer(q->msgId(), textInvite);
}

qint64 Api::helpGetInviteText(const QString &langCode) {
    OutboundPkt *p = new OutboundPkt();

    if (mMainSession->initConnectionNeeded()) {
        p->initConnection();
        mMainSession->setInitConnectionNeeded(false);
    }
    p->appendInt(TL_HelpGetInviteText);
    p->appendQString(langCode);
    qint64 ret =  mMainSession->sendQuery(*p, &helpGetInviteTextMethods);
    delete p;
    return ret;
}

// ### auth.checkPhone()
void Api::onErrorRetry(Query *q, qint32 errorCode, const QString &errorText) {
    Q_EMIT errorRetry(q->msgId(), errorCode, errorText);

}

void Api::onAuthCheckPhoneAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_AuthCheckedPhone);
    bool phoneRegistered = inboundPkt.fetchBool();
    bool phoneInvited = inboundPkt.fetchBool();
//    Q_EMIT authCheckedPhone(q->msgId(), phoneRegistered, phoneInvited);

}

qint64 Api::authCheckPhone(const QString &phoneNumber) {

    OutboundPkt *p = new OutboundPkt;
    if (mMainSession->initConnectionNeeded()) {
        p->initConnection();
        mMainSession->setInitConnectionNeeded(false);
    }
    p->appendInt(TL_AuthCheckPhone);
    p->appendQString(phoneNumber);
    qint64 resultId = mMainSession->sendQuery(*p, &authCheckPhoneMethods);
//    Q_EMIT authCheckPhoneSent(resultId, phoneNumber);
    delete  p;
    return resultId;

}

// ### auth.sendCode()
void Api::onAuthSendCodeAnswer(Query *q, InboundPkt &inboundPkt) {
    qDebug() << QString::number(inboundPkt.fetchInt(),16) << QString::number((qint32)TL_AuthSentCode,16);
    bool phoneRegistered = inboundPkt.fetchInt();
    QString phoneCodeHash = inboundPkt.fetchQString();
//    qDebug() << phoneRegistered << phoneCodeHash;
//    qDebug() << phoneCodeHash;
//    qint32 sendCallTimeout = inboundPkt.fetchInt();
//    qDebug() << sendCallTimeout;
//    bool isPassword = inboundPkt.fetchBool();
    Q_EMIT authSentCode(q->msgId(), phoneRegistered, phoneCodeHash);

}
void Api::onAuthSendCodeError(Query *q, qint32 errorCode, const QString &errorText) {
    Q_EMIT authSendCodeError(q->msgId(), errorCode, errorText);

}

qint64 Api::authSendCode(const QString &phoneNumber, qint32 smsType, qint32 apiId, const QString &apiHash, const QString &langCode) {
    qDebug() << phoneNumber;
    OutboundPkt *p = new OutboundPkt;
    p->appendInt(TL_AuthSendCode);
    p->appendQString(phoneNumber);
    p->appendInt(smsType);
    p->appendInt(apiId);
    p->appendQString(apiHash);
    p->appendQString(langCode);

    qint64 ret = mMainSession->sendQuery( *p, &authSendCodeMethods);
    delete p;
    return ret;
}
qint64 Api::sendping(){
    OutboundPkt *p = new OutboundPkt;
    p->appendInt(TL_NearestDc);
    qint64 ret = mMainSession->sendQuery(*p, &sendpingMethods);
    delete p;
    return ret;
}
void Api::sendpingAnswer(Query *p , InboundPkt &inboundPkt){
    qDebug() << QString::number(inboundPkt.fetchInt(),16);
    qDebug() << inboundPkt.buffer();

//    qDebug() << inboundPkt.fetchInt();
//    qDebug() << inboundPkt.fetchInt();
//    Q_EMIT showToast( inboundPkt.fetchQString());

}
// ## auth.sendCall()
void Api::onAuthSendCallAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT authSendCallResult(q->msgId(), inboundPkt.fetchBool());
}

qint64 Api::authSendCall(const QString &phoneNumber, const QString &phoneCodeHash) {
    OutboundPkt *p = new OutboundPkt();
    p-> appendInt(TL_AuthSendCall);
    p-> appendQString(phoneNumber);
    p-> appendQString(phoneCodeHash);
    qint64 ret=  mMainSession->sendQuery(*p, &authSendCallMethods);
    delete p;
    return ret;
}

// ### auth.signIn()
void Api::onAuthSignInError(Query *q, qint32 errorCode, const QString &errorText) {
    Q_EMIT authSignInError(q->msgId(), errorCode, errorText);

}

void Api::onAuthSignInAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_AuthAuthorization);
    qint32 expires = inboundPkt.fetchInt();
//    User user = inboundPkt.fetchUser();
    qDebug() << "Experies" << expires ;
    Q_EMIT authSignInAuthorization(q->msgId(), expires);
}

qint64 Api::authSignIn(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode) {
    OutboundPkt *p = new OutboundPkt();
    p->appendInt(TL_AuthSignIn);
    p->appendQString(phoneNumber);
    p->appendQString(phoneCodeHash);
    p->appendQString(phoneCode);
    qint64 ret =  mMainSession->sendQuery(*p, &authSignInMethods);
    delete p;
    return ret;
}

// ### auth.signUp()
void Api::onAuthSignUpError(Query *q, qint32 errorCode, const QString &errorText) {
    Q_EMIT authSignUpError(q->msgId(), errorCode, errorText);
}

void Api::onAuthSignUpAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_AuthAuthorization);
    qint32 expires = inboundPkt.fetchInt();
    User user = inboundPkt.fetchUser();
    Q_EMIT authSignUpAuthorization(q->msgId(), expires, user);

}

qint64 Api::authSignUp(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode, const QString &firstName, const QString &lastName) {
    OutboundPkt *p = new OutboundPkt();
    p-> appendInt(TL_AuthSignUp);
    p-> appendQString(phoneNumber);
    p-> appendQString(phoneCodeHash);
    p-> appendQString(phoneCode);
    p-> appendQString(firstName);
    p-> appendQString(lastName);
    qint64 ret =  mMainSession->sendQuery(*p, &authSignUpMethods);
    delete p;
    return ret;
}

// ### auth.logOut
void Api::onAuthLogOutAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT authLogOutResult(q->msgId(), inboundPkt.fetchBool());

}

qint64 Api::authLogOut() {
    OutboundPkt *p = new OutboundPkt();
    p->appendInt(TL_AuthLogOut);
    qint64 ret = mMainSession->sendQuery(*p, &authLogOutMethods);
    delete p;
    return ret;
}

// ### auth.sendInvites
void Api::onAuthSendInvitesAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT authSendInvitesResult(q->msgId(), inboundPkt.fetchBool());

}

qint64 Api::authSendInvites(const QStringList &phoneNumbers, const QString &message) {
    OutboundPkt *p = new OutboundPkt();
    p-> appendInt(TL_AuthSendInvites);
    p-> appendInt(TL_Vector);
    p-> appendInt(phoneNumbers.size());
    Q_FOREACH (QString phoneNumber, phoneNumbers) {
        p-> appendQString(phoneNumber);
    }
    p-> appendQString(message);
    qint64 ret= mMainSession->sendQuery(*p, &authSendInvitesMethods);
    delete p;
    return ret;
}

// ### auth.resetAuthorizationel
void Api::onAuthResetAuthorizationsAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT authResetAuthorizationsResult(q->msgId(), inboundPkt.fetchBool());

}

qint64 Api::authResetAuthorizations() {
    OutboundPkt *p = new OutboundPkt();
    p-> appendInt(TL_AuthResetAuthorizations);
    qint64 ret =  mMainSession->sendQuery(*p, &authResetAuthorizationsMethods);
    delete p;
    return ret;
}

// ### auth.importAuthorization
void Api::onAuthImportAuthorizationAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_AuthAuthorization);
    qint32 expires = inboundPkt.fetchInt();
    User user = inboundPkt.fetchUser();
    Q_EMIT authImportedAuthorization(q->msgId(), expires, user);


}

qint64 Api::authImportAuthorization(qint32 id, const  QByteArray &bytes) {
    OutboundPkt *p= new OutboundPkt();
    if (mMainSession->initConnectionNeeded()) {
        p-> initConnection();
        mMainSession->setInitConnectionNeeded(false);
    }
    p-> appendInt(TL_AuthImportAuthorization);
    p-> appendInt(id);
    p-> appendBytes(bytes);
    qint64 ret=  mMainSession->sendQuery(*p, &authImportAuthorizationMethods);
    delete p;
    return ret;
}

// ### auth.exportAuthorization
void Api::onAuthExportAuthorizationAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_AuthExportedAuthorization);
    qint32 id = inboundPkt.fetchInt();
    qDebug() << "onAuthExportAuthorizationAnswer Dc : " << id ;
    QByteArray bytes = inboundPkt.fetchBytes();
    Q_EMIT authExportedAuthorization(q->msgId(), id, bytes);
}

qint64 Api::authExportAuthorization(qint32 dcId) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AuthExportAuthorization);
    p-> appendInt(dcId);
    qDebug() << "Auth Export " << dcId;
    qint64 ret =  mMainSession->sendQuery(*p, &authExportAuthorizationMethods);
    delete p;
    return ret;
}

//
// NOTIFICATIONS / SETTINGS
//
// ### account.registerDevice
void Api::onAccountRegisterDeviceAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountRegisterDeviceResult(q->msgId(), inboundPkt.fetchBool());
}

qint64 Api::accountRegisterDevice(qint32 tokenType, const QString &token, const QString &deviceModel, const QString &systemVersion, const QString &appVersion, bool appSandbox, const QString &langCode) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountRegisterDevice);
    p-> appendInt(tokenType);
    p-> appendQString(token);
    p-> appendQString(deviceModel);
    p-> appendQString(systemVersion);
    p-> appendQString(appVersion);
    p-> appendBool(appSandbox);
    p-> appendQString(langCode);
    qint64 ret = mMainSession->sendQuery(*p, &accountRegisterDeviceMethods);
    delete p;
    return ret;
}

// ### account.unregisterDevice
void Api::onAccountUnregisterDeviceAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountUnregisterDeviceResult(q->msgId(), inboundPkt.fetchBool());
}

qint64 Api::accountUnregisterDevice(qint32 tokenType, const QString &token) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountUnregisterDevice);
    p-> appendInt(tokenType);
    p-> appendQString(token);
    qint64 ret = mMainSession->sendQuery(*p, &accountUnregisterDeviceMethods);
    delete p;
    return ret;
}

// ### account.updateNotifySettings
void Api::onAccountUpdateNotifySettingsAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountUpdateNotifySettingsResult(q->msgId(), inboundPkt.fetchBool());

}

qint64 Api::accountUpdateNotifySettings(const InputNotifyPeer &peer, const InputPeerNotifySettings &settings) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountUpdateNotifySettings);
    p-> appendInputNotifyPeer(peer);
    p-> appendInputPeerNotifySettings(settings);
    qint64 ret = mMainSession->sendQuery(*p, &accountUpdateNotifySettingsMethods);
    delete p;
    return ret;
}

// ### account.getNotifySettings
void Api::onAccountGetNotifySettingsAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountPeerNotifySettings(q->msgId(), inboundPkt.fetchPeerNotifySetting());
}

qint64 Api::accountGetNotifySettings(const InputNotifyPeer &peer) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountGetNotifySettings);
    p-> appendInputNotifyPeer(peer);
    qint64 ret = mMainSession->sendQuery(*p, &accountGetNotifySettingsMethods);
    delete p;
    return ret;
}

// ### account.resetNotifySettings
void Api::onAccountResetNotifySettingsAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountResetNotifySettingsResult(q->msgId(), inboundPkt.fetchBool());
}

qint64 Api::accountResetNotifySettings() {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountResetNotifySettings);
    qint64 ret = mMainSession->sendQuery(*p, &accountResetNotifySettingsMethods);
    delete p;
    return ret;
}

// ### account.updateProfile
void Api::onAccountUpdateProfileAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountUser(q->msgId(), inboundPkt.fetchUser());
}

qint64 Api::accountUpdateProfile(const QString &firstName, const QString &lastName) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountUpdateProfile);
    p-> appendQString(firstName);
    p-> appendQString(lastName);
    qint64 ret = mMainSession->sendQuery(*p, &accountUpdateProfileMethods);
    delete p;
    return ret;

}

// ### account.updateStatus
void Api::onAccountUpdateStatusAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT accountUpdateStatusResult(q->msgId(), inboundPkt.fetchBool());
}

qint64 Api::accountUpdateStatus(bool offline) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_AccountUpdateStatus);
    p-> appendBool(offline);
    qint64 ret = mMainSession->sendQuery(*p, &accountUpdateStatusMethods);
    delete p;
    return ret;
}



// ### photos.uploadProfilePhoto
void Api::onPhotosUploadProfilePhotoAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_PhotosPhoto);
    Photo photo = inboundPkt.fetchPhoto();
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    Q_EMIT photosPhoto(q->msgId(), photo, users);

}

qint64 Api::photosUploadProfilePhoto(const InputFile &file, const QString &caption, const InputGeoPoint &geoPoint, const InputPhotoCrop &crop) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_PhotosUploadProfilePhoto);
    p-> appendInputFile(file);
    p-> appendQString(caption);
    p-> appendInputGeoPoint(geoPoint);
    p-> appendInputPhotoCrop(crop);
    qint64 ret = mMainSession->sendQuery(*p, &photosUploadProfilePhotoMethods);
    delete p;
    return ret;
}

// ### photos.updateProfilePhoto
void Api::onPhotosUpdateProfilePhotoAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT photosUserProfilePhoto(q->msgId(), inboundPkt.fetchUserProfilePhoto());
}

qint64 Api::photosUpdateProfilePhoto(const InputPhoto &id, const InputPhotoCrop &crop) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_PhotosUpdateProfilePhoto);
    p-> appendInputPhoto(id);
    p-> appendInputPhotoCrop(crop);
    qint64 ret = mMainSession->sendQuery(*p, &photosUpdateProfilePhotoMethods);
    delete p;
    return ret;
}

//
// USERS
//
// ### users.getUsers
void Api::onUsersGetUsersAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    Q_EMIT usersGetUsersResult(q->msgId(), users);
}

qint64 Api::usersGetUsers(const QList<InputUser> &users) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_UsersGetUsers);
    p-> appendInt(TL_Vector);
    p-> appendInt(users.size());
    Q_FOREACH (InputUser user, users) {
        p-> appendInputUser(user);
    }
    qint64 ret = mMainSession->sendQuery(*p, &usersGetUsersMethods);
    delete p;
    return ret;
}

// ### users.getFullUser
void Api::onUsersGetFullUserAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_UserFull);
    User user = inboundPkt.fetchUser();
    ContactsLink link = inboundPkt.fetchContactsLink();
    Photo photo = inboundPkt.fetchPhoto();
    PeerNotifySettings notifySettings = inboundPkt.fetchPeerNotifySetting();
    bool blocked = inboundPkt.fetchBool();
    QString realFirstName = inboundPkt.fetchQString();
    QString realLastName = inboundPkt.fetchQString();
    Q_EMIT userFull(q->msgId(), user, link, photo, notifySettings, blocked, realFirstName, realLastName);
}

qint64 Api::usersGetFullUser(const InputUser &user) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_UsersGetFullUser);
    p-> appendInputUser(user);
    qint64 ret = mMainSession->sendQuery(*p, &usersGetFullUserMethods);
    delete p;
    return ret;
}

// ### photos.getUserPhotos
void Api::onPhotosGetUserPhotos(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_PhotosPhotos || x == (qint32)TL_PhotosPhotosSlice);
    qint32 count = -1;
    if (x == (qint32)TL_PhotosPhotosSlice) {
        count = inboundPkt.fetchInt();
    }
    //photos
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Photo> photos;
    for (qint32 i = 0; i < n; i++) {
        photos.append(inboundPkt.fetchPhoto());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    if (x == (qint32)TL_PhotosPhotosSlice) {
        Q_EMIT photosPhotosSlice(q->msgId(), count, photos, users);
    } else {
        Q_EMIT photosPhotos(q->msgId(), photos, users);
    }
}

qint64 Api::photosGetUserPhotos(const InputUser &user, qint32 offset, qint32 maxId, qint32 limit) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_PhotosGetUserPhotos);
    p-> appendInputUser(user);
    p-> appendInt(offset);
    p-> appendInt(maxId);
    p-> appendInt(limit);
    return mMainSession->sendQuery(*p, &photosGetUserPhotosMethods);
}

//
// CONTACTS
//
// ### contacts.getStatuses
void Api::onContactsGetStatusesAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<ContactStatus> statuses;
    for (qint32 i = 0; i < n; i++) {

        statuses.append(inboundPkt.fetchContactStatus());
//        qDebug() << inboundPkt.fetchContactStatus();

    }
    Q_EMIT contactsGetStatusesResult(q->msgId(), statuses);
}

qint64 Api::contactsGetStatuses() {
    OutboundPkt *p = new OutboundPkt;
    p->appendInt(TL_ContactsGetStatuses);
    return mMainSession->sendQuery( *p, &contactsGetStatusesMethods);
}

// ### contacts.getContacts
void Api::onContactsGetContactsAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_ContactsContacts || x == (qint32)TL_ContactsContactsNotModified);
    if (x == (qint32)TL_ContactsContacts) {
        //contacts
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        qint32 n = inboundPkt.fetchInt();
        QList<Contact> contacts;
        for (qint32 i = 0; i < n; i++) {
            contacts.append(inboundPkt.fetchContact());
        }
        //users
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        QList<User> users;
        for (qint32 i = 0; i < n; i++) {
            users.append(inboundPkt.fetchUser());
        }
        Q_EMIT contactsContacts(q->msgId(), contacts, users);
    } else {
        Q_EMIT contactsContactsNotModified(q->msgId());
    }

}

qint64 Api::contactsGetContacts(const QString &hash) {
    OutboundPkt *p = new OutboundPkt();
    p->appendInt(TL_ContactsGetContacts);
    p->appendQString(hash);
    qint64 ret = mMainSession->sendQuery(*p, &contactsGetContactsMethods);
    delete p;
    return ret;
}

// ### contacts.importContacts
void Api::onContactsImportContactsAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_ContactsImportedContacts);
    // imported contacts
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<ImportedContact> imported;
    for (qint32 i = 0; i < n; i++) {
        imported.append(inboundPkt.fetchImportedContact());
    }
    // retry contacts
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<qint64> retryContacts;
    for (qint32 i = 0; i < n; i++) {
        retryContacts.append(inboundPkt.fetchLong());
    }
    // users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
//        users.
    }
    Q_EMIT contactsImportedContacts(q->msgId(), imported, retryContacts, users);

}

qint64 Api::contactsImportContacts(const QList<InputContact> contacts, bool replace) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_ContactsImportContacts);
    p-> appendInt(TL_Vector);
    p-> appendInt(contacts.length());
    Q_FOREACH (InputContact contact, contacts) {
        p-> appendInputContact(contact);
    }
    p-> appendBool(replace);
    qint64 ret =  mMainSession->sendQuery(*p, &contactsImportContactsMethods);
    delete p;
    return ret;
}

// ### contacts.deleteContact
void Api::onContactsDeleteContactAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_ContactsLink);
    ContactsMyLink myLink = inboundPkt.fetchContactsMyLink();
    ContactsForeignLink foreignLink = inboundPkt.fetchContactsForeignLink();
    User user = inboundPkt.fetchUser();
    Q_EMIT contactsDeleteContactLink(q->msgId(), myLink, foreignLink, user);
}

qint64 Api::contactsDeleteContact(const InputUser &id) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_ContactsDeleteContact);
    p-> appendInputUser(id);
    return mMainSession->sendQuery(*p, &contactsDeleteContactMethods);
}

// ### contacts.deleteContacts
void Api::onContactsDeleteContactsAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT contactsDeleteContactsResult(q->msgId(), inboundPkt.fetchBool());

}

qint64 Api::contactsDeleteContacts(const QList<InputUser> &ids) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_ContactsDeleteContacts);
    p-> appendInt(TL_Vector);
    p-> appendInt(ids.length());
    Q_FOREACH (InputUser id, ids) {
        p-> appendInputUser(id);
    }
    qint64 ret =  mMainSession->sendQuery(*p, &contactsDeleteContactsMethods);
    delete p;
    return ret;
}

//
// BLACKLIST
//
// ### contacts.block
void Api::onContactsBlockAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT contactsBlockResult(q->msgId(), inboundPkt.fetchBool());
}

qint64 Api::contactsBlock(const InputUser &id) {
    OutboundPkt *p = new OutboundPkt ();
    p-> appendInt(TL_ContactsBlock);
    p-> appendInputUser(id);
    qint64 ret =  mMainSession->sendQuery(*p, &contactsBlockMethods);
    delete p;
    return ret;
}

// ### contacts.unblock
void Api::onContactsUnblockAnswer(Query *q, InboundPkt &inboundPkt) {
    qDebug() << "onContactsUnblockAnswer" << q->msgId();
    Q_EMIT contactsUnblockResult(q->msgId(), inboundPkt.fetchBool());

}

qint64 Api::contactsUnblock(const InputUser &id) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_ContactsUnblock);
    p-> appendInputUser(id);
    qint64 ret = mMainSession->sendQuery(*p, &contactsUnblockMethods);
    delete p;
    return ret;
}

// ### contacts.getBlocked
void Api::onContactsGetBlockedAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_ContactsBlocked || x == (qint32)TL_ContactsBlockedSlice);
    qint32 count = -1;
    if (x == (qint32)TL_ContactsBlockedSlice) {
        count = inboundPkt.fetchInt();
    }
    // blocked
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<ContactBlocked> blocked;
    for (qint32 i = 0; i < n; i++) {
        blocked.append(inboundPkt.fetchContactBlocked());
    }
    // users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    if (x == (qint32)TL_ContactsBlockedSlice) {
        Q_EMIT contactsBlockedSlice(q->msgId(), count, blocked, users);
    } else {
        Q_EMIT contactsBlocked(q->msgId(), blocked, users);
    }
    delete q;
}

qint64 Api::contactsGetBlocked(qint32 offset, qint32 limit) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_ContactsGetBlocked);
    p-> appendInt(offset);
    p-> appendInt(limit);
    qint64 ret =  mMainSession->sendQuery(*p, &contactsGetBlockedMethods);
    delete p;
    return ret;
}

//
// MESSAGES
//
// ### messages.sendMessage
void Api::onMessagesSendMessageAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesSentMessage || x == (qint32)TL_MessagesSentMessageLink);
    qint32 id = inboundPkt.fetchInt();
    qint32 date = inboundPkt.fetchInt();
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesSentMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        qint32 n = inboundPkt.fetchInt();
        QList<ContactsLink> links;
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
        Q_EMIT messagesSentMessageLink(q->msgId(), id, date, pts, seq, links);
    } else {
        Q_EMIT messagesSentMessage(q->msgId(), id, date, pts, seq);
    }

}

qint64 Api::messagesSendMessage(const InputPeer &peer, const QString &message, qint64 randomId) {
    OutboundPkt *p = new OutboundPkt;
    p->appendInt(TL_MessagesSendMessage);
    p->appendInputPeer(peer);
    p->appendQString(message);
    p->appendLong(randomId);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSendMessageMethods);
    delete p;
    return ret;
}

// ### messages.sendMedia
void Api::onMessagesSendMediaAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        Q_EMIT messagesSentMediaStatedMessageLink(q->msgId(), message, chats, users, links, pts, seq);
    } else {
        Q_EMIT messagesSentMediaStatedMessage(q->msgId(), message, chats, users, pts, seq);
    }
}

qint64 Api::messagesSendMedia(const InputPeer &peer, const InputMedia &media, qint64 randomId) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSendMedia);
    p-> appendInputPeer(peer);
    p-> appendInputMedia(media);
    p-> appendLong(randomId);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSendMediaMethods);
    delete p;
    return ret;
}

// ### messages.setTyping
void Api::onMessagesSetTypingAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT messagesSetTypingResult(q->msgId(), inboundPkt.fetchBool());
    delete q;
}

qint64 Api::messagesSetTyping(const InputPeer &peer, bool typing) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSetTyping);
    p-> appendInputPeer(peer);
    p-> appendBool(typing);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSetTypingMethods);
    delete p;
    return ret;
}

// ### messages.getMessages
void Api::onMessagesGetMessagesAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesMessages || x == (qint32)TL_MessagesMessagesSlice);
    // slice count
    qint32 count = -1;
    if (x == (qint32)TL_MessagesMessagesSlice) {
        count = inboundPkt.fetchInt();
    }
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    if (x == (qint32)TL_MessagesMessagesSlice) {
        qDebug() << "in Slice";
        Q_EMIT messagesGetMessagesMessagesSlice(q->msgId(), count, messages, chats, users);
    } else {
        Q_EMIT messagesGetMessagesMessages(q->msgId(), messages, chats, users);
    }
}



qint64 Api::messagesGetMessages(const QList<qint32> &ids) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetMessages);
    p-> appendInt(TL_Vector);
    p-> appendInt(ids.length());
    Q_FOREACH (qint32 id, ids) {
        p-> appendInt(id);
    }
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetMessagesMethods);
    delete p;
    return ret;
}

// ### messages.getDialogs
void Api::onMessagesGetDialogsAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesDialogs || x == (qint32)TL_MessagesDialogsSlice);
    // slice count
    qint32 count = -1;
    if (x == (qint32)TL_MessagesDialogsSlice) {
        count = inboundPkt.fetchInt();
    }
    //dialogs
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Dialog> dialogs;
    for (qint32 i = 0; i < n; i++) {
        dialogs.append(inboundPkt.fetchDialog());
    }
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
//        qDebug() << "messages" << messages.at(i).message();
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
//        qDebug() << "chat ::" << chats.at(i).id();
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
//        qDebug() << " user name " << users.at(i).firstName();
    }

//qDebug() << "messages count" << messages.count() << "chat ids :" << chats.count() << "users count " << users.count();
    if (x == (qint32)TL_MessagesDialogsSlice) {
        qDebug() << "in slice";
        Q_EMIT messagesDialogsSlice(q->msgId(), count, dialogs, messages, chats, users);
    }
    else {
        qDebug() << "Out of Slice";
        Q_EMIT messagesDialogs(q->msgId(),dialogs, messages, chats, users);

    }

}

qint64 Api::messagesGetDialogs(qint32 offset, qint32 maxId, qint32 limit) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetDialogs);
    p-> appendInt(offset);
    p-> appendInt(maxId);
    p-> appendInt(limit);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetDialogsMethods);
    delete p;
    return maxId;
}

// ### messages.getHistory
void Api::onMessagesGetHistoryAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesMessages || x == (qint32)TL_MessagesMessagesSlice);
    // slice count
    qint32 count = -1;
    if (x == (qint32)TL_MessagesMessagesSlice) {
        count = inboundPkt.fetchInt();
    }
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }

    if (x == (qint32)TL_MessagesMessagesSlice) {
//            qDebug() << "In slice ";
            Q_EMIT messagesGetHistoryMessagesSlice(q->msgId(), count, messages, chats, users);
        } else {
//            qDebug() << "Bypassing slice ";
            Q_EMIT messagesGetHistoryMessages(q->msgId(), messages, chats, users);
        }
    delete q;
}






qint64 Api::messagesGetHistory(const InputPeer &peer, qint32 offset, qint32 maxId, qint32 limit) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetHistory);
    p-> appendInputPeer(peer);
    p-> appendInt(offset);
    p-> appendInt(maxId);
    p-> appendInt(limit);
//    qDebug() << "History " << peer.userId() << peer.classType();
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetHistoryMethods);
    delete p;
    return ret;
}

// ### messages.search
void Api::onMessagesSearchAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesMessages || x == (qint32)TL_MessagesMessagesSlice);
    // slice count
    qint32 count = -1;
    if (x == (qint32)TL_MessagesMessagesSlice) {
        count = inboundPkt.fetchInt();
    }
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }

        Q_EMIT messagesSearchMessages(q->msgId(), messages, chats, users);

}

qint64 Api::messagesSearch(const InputPeer &peer, const QString &q, const MessagesFilter &filter, qint32 minDate, qint32 maxDate, qint32 offset, qint32 maxId, qint32 limit) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSearch);
    p-> appendInputPeer(peer);
    p-> appendQString(q);
    p-> appendMessagesFilter(filter);
    p-> appendInt(minDate);
    p-> appendInt(maxDate);
    p-> appendInt(offset);
    p-> appendInt(maxId);
    p-> appendInt(limit);
    qDebug() << q << maxId;
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSearchMethods);
    delete p;
    return ret;
}
// ### messages.readHistory
void Api::onMessagesReadHistoryAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_MessagesAffectedHistory);
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    qint32 offset = inboundPkt.fetchInt();
    Q_EMIT messagesReadAffectedHistory(q->msgId(), pts, seq, offset);delete q;
}

qint64 Api::messagesReadHistory(const InputPeer &peer, qint32 maxId, qint32 offset) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesReadHistory);
    p-> appendInputPeer(peer);
    p-> appendInt(maxId);
    p-> appendInt(offset);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesReadHistoryMethods);
    delete p;
    return ret;
}

// ### messages.deleteHistory
void Api::onMessagesDeleteHistoryAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_MessagesAffectedHistory);
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    qint32 offset = inboundPkt.fetchInt();
    Q_EMIT messagesDeleteAffectedHistory(q->msgId(), pts, seq, offset);
}

qint64 Api::messagesDeleteHistory(const InputPeer &peer, qint32 offset) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesDeleteHistory);
    p-> appendInputPeer(peer);
    p-> appendInt(offset);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesDeleteHistoryMethods);
    delete p;
    return ret;
}

// ### messages.deleteMessages
void Api::onMessagesDeleteMessagesAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<qint32> deletedIds;
    for (qint32 i = 0; i < n; i++) {
        deletedIds.append(inboundPkt.fetchInt());
    }
    Q_EMIT messagesDeleteMessagesResult(q->msgId(), deletedIds);
}

qint64 Api::messagesDeleteMessages(const QList<qint32> &ids) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesDeleteMessages);
    p-> appendInt(TL_Vector);
    p-> appendInt(ids.length());
    Q_FOREACH (qint32 id, ids) {
        p-> appendInt(id);
    }
    qint64 ret =  mMainSession->sendQuery(*p, &messagesDeleteMessagesMethods);
    delete p;
    return ret;
}

// ### messages.restoreMessages
void Api::onMessagesRestoreMessagesAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<qint32> restoredIds;
    for (qint32 i = 0; i < n; i++) {
        restoredIds.append(inboundPkt.fetchInt());
    }
    Q_EMIT messagesRestoreMessagesResult(q->msgId(), restoredIds);
    delete q;

}

qint64 Api::messagesRestoreMessages(const QList<qint32> &ids) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesRestoreMessages);
    p-> appendInt(TL_Vector);
    p-> appendInt(ids.length());
    Q_FOREACH (qint32 id, ids) {
        p-> appendInt(id);
    }
    qint64 ret =  mMainSession->sendQuery(*p, &messagesRestoreMessagesMethods);
    delete p;
    return ret;
}

// ### messages.receivedMessages
void Api::onMessagesReceivedMessagesAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<qint32> receivedIds;
    for (qint32 i = 0; i < n; i++) {
        receivedIds.append(inboundPkt.fetchInt());
    }
    Q_EMIT messagesReceivedMessagesResult(q->msgId(), receivedIds);
    delete q;
}

qint64 Api::messagesReceivedMessages(qint32 maxId) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesReceivedMessages);
    p-> appendInt(maxId);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesReceivedMessagesMethods);
    delete p;
    return ret;
}

// ### messages.forwardMessage
void Api::onMessagesForwardMessageAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    qDebug() << seq << message.message();
        Q_EMIT messagesForwardMsgStatedMessage(q->msgId(), message, chats, users, pts, seq);


}

qint64 Api::messagesForwardMessage(const InputPeer &peer, qint32 id, qint64 randomId) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesForwardMessage);
    p-> appendInputPeer(peer);
    p-> appendInt(id);
    p-> appendLong(randomId);
    qint64 ret = mMainSession->sendQuery(*p, &messagesForwardMessageMethods);
    delete p;
    return ret;
}




// ### messages.forwardMessages
void Api::onMessagesForwardMessagesAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessages || x == (qint32)TL_MessagesStatedMessagesLinks);
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessagesLinks) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    qDebug() << messages.size() << messages.at(0).message();
    Q_EMIT messagesForwardMsgsStatedMessages(q->msgId(), messages, chats, users);
}

qint64 Api::messagesForwardMessages(const InputPeer &peer, const QList<qint32> &ids) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesForwardMessage);
    p-> appendInputPeer(peer);
    p-> appendInt(TL_Vector);
    p-> appendInt(ids.length());
    Q_FOREACH (qint32 id, ids) {
        p-> appendInt(id);
    }
    qint64 ret =  mMainSession->sendQuery(*p, &messagesForwardMessagesMethods);
    delete p;
    return ret;
}



// ### messages.L
qint64 Api::messagesGetLatestMessage(const InputPeer &peer, qint32 offset, qint32 maxId, qint32 limit) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetHistory);
    p-> appendInputPeer(peer);
    p-> appendInt(offset);
    p-> appendInt(maxId);
    p-> appendInt(limit);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetHistoryMethods);
    delete p;
    return ret;
}

void Api::onMessagesGetLatestMessageAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesMessages || x == (qint32)TL_MessagesMessagesSlice);
    // slice count
    qint32 count = -1;
    if (x == (qint32)TL_MessagesMessagesSlice) {
        count = inboundPkt.fetchInt();
    }
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
//        qDebug() << messages[i].id() <<messages[i].message();
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    Q_EMIT messagesGetLatestMessage(q->msgId(), messages, chats, users);
    delete q;
}
// ### messages.sendBroadcast
void Api::onMessagesSendBroadcastAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessages || x == (qint32)TL_MessagesStatedMessagesLinks);
    //messages
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Message> messages;
    for (qint32 i = 0; i < n; i++) {
        messages.append(inboundPkt.fetchMessage());
    }
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessagesLinks) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesStatedMessagesLinks) {
        Q_EMIT messagesSendBroadcastStatedMessagesLinks(q->msgId(), messages, chats, users, links);
    } else {
        Q_EMIT messagesSendBroadcastStatedMessages(q->msgId(), messages, chats, users);
    }
    delete q;
}

qint64 Api::messagesSendBroadcast(const QList<InputUser> &contacts, const QString &message, const InputMedia &media) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSendBroadcast);
    p-> appendInt(TL_Vector);
    p-> appendInt(contacts.length());
    Q_FOREACH (InputUser user, contacts) {
        p-> appendInputUser(user);
    }
    p-> appendQString(message);
    p-> appendInputMedia(media);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSendBroadcastMethods);
    delete p;
    return ret;
}

//
// CHATS
//
// ### messages.getChats

void Api::onMessagesGetChatsAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_MessagesChats);
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    Q_EMIT messagesChats(q->msgId(), chats, users);
    delete q;

}

qint64 Api::messagesGetChats(const QList<qint32> chatIds) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetChats);
    p-> appendInt(TL_Vector);
    p-> appendInt(chatIds.length());
    Q_FOREACH (qint32 id, chatIds) {
        p-> appendInt(id);
    }
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetChatsMethods);
    delete p;
    return ret;

}

// ### messages.getFullChat

void Api::onMessagesGetFullChatAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_MessagesChatFull);
    ChatFull fullChat = inboundPkt.fetchChatFull();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    Q_EMIT messagesChatFull(q->msgId(), fullChat, chats, users);
    delete q;
}

qint64 Api::messagesGetFullChat(qint32 chatId) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetFullChat);
    p-> appendInt(chatId);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetFullChatMethods);
    delete p;
    return ret;

}

// ### messages.editChatTitle
void Api::onMessagesEditChatTitleAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        Q_EMIT messagesEditChatTitleStatedMessageLink(q->msgId(), message, chats, users, links, pts, seq);
    } else {
        Q_EMIT messagesEditChatTitleStatedMessage(q->msgId(), message, chats, users, pts, seq);
    }
    delete q;
}

qint64 Api::messagesEditChatTitle(qint32 chatId, const QString &title) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesEditChatTitle);
    p-> appendInt(chatId);
    p-> appendQString(title);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesEditChatTitleMethods);
    delete p;
    return ret;
}

// ### messages.editChatPhoto
void Api::onMessagesEditChatPhotoAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        Q_EMIT messagesEditChatPhotoStatedMessageLink(q->msgId(), message, chats, users, links, pts, seq);
    } else {
        Q_EMIT messagesEditChatPhotoStatedMessage(q->msgId(), message, chats, users, pts, seq);
    }
    delete q;
}

qint64 Api::messagesEditChatPhoto(qint32 chatId, const InputChatPhoto &photo) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesEditChatPhoto);
    p-> appendInt(chatId);
    p-> appendInputChatPhoto(photo);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesEditChatPhotoMethods);
    delete p;
    return ret;
}

// ### messages.addChatUser
void Api::onMessagesAddChatUserAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();

        Q_EMIT messagesAddChatUserStatedMessage(q->msgId(), message, chats, users, pts, seq);

}

qint64 Api::messagesAddChatUser(qint32 chatId, const InputUser &user, qint32 fwdLimit) {
    OutboundPkt *p = new OutboundPkt ();
    p-> appendInt(TL_MessagesAddChatUser);
    p-> appendInt(chatId);
    p-> appendInputUser(user);
    p-> appendInt(fwdLimit);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesAddChatUserMethods);
    delete p;
    return ret;
}

// ### messages.deleteChatUser
void Api::onMessagesDeleteChatUserAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        Q_EMIT messagesDeleteChatUserStatedMessageLink(q->msgId(), message, chats, users, links, pts, seq);
    } else {
        Q_EMIT messagesDeleteChatUserStatedMessage(q->msgId(), message, chats, users, pts, seq);
    }
}

qint64 Api::messagesDeleteChatUser(qint32 chatId, const InputUser &user) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesDeleteChatUser);
    p-> appendInt(chatId);
    p-> appendInputUser(user);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesDeleteChatUserMethods);
    delete p;
    return ret;
}

// ### messages.createChat
void Api::onMessagesCreateChatAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesStatedMessage || x == (qint32)TL_MessagesStatedMessageLink);
    Message message = inboundPkt.fetchMessage();
    //chats
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    //users
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //links
    QList<ContactsLink> links;
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        for (qint32 i = 0; i < n; i++) {
            links.append(inboundPkt.fetchContactsLink());
        }
    }
    qint32 pts = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesStatedMessageLink) {
        Q_EMIT messagesCreateChatStatedMessageLink(q->msgId(), message, chats, users, links, pts, seq);
    } else {
        Q_EMIT messagesCreateChatStatedMessage(q->msgId(), message, chats, users, pts, seq);
    }

}

qint64 Api::messagesCreateChat(const QList<InputUser> &users, const QString &title) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesCreateChat);
    p-> appendInt(TL_Vector);
    p-> appendInt(users.length());
    Q_FOREACH (InputUser user, users) {
        p-> appendInputUser(user);
    }
    p-> appendQString(title);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesCreateChatMethods);
    delete p;
    return ret;
}

//
// SECRET CHATS
//
// ### messages.getDhConfig
void Api::onMessagesGetDhConfigAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesDhConfig || x == (qint32)TL_MessagesDhConfigNotModified);
    if (x == (qint32)TL_MessagesDhConfig) {
        qint32 g = inboundPkt.fetchInt();
        QByteArray p = inboundPkt.fetchBytes();
        qint32 version = inboundPkt.fetchInt();
        QByteArray random = inboundPkt.fetchBytes();
        Q_EMIT messagesDhConfig(q->msgId(), g, p, version, random);
    } else {
        QByteArray random = inboundPkt.fetchBytes();
        Q_EMIT messagesDhConfigNotModified(q->msgId(), random);
    }
    delete q;
}

qint64 Api::messagesGetDhConfig(qint32 version, qint32 randomLength) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesGetDhConfig);
    p-> appendInt(version);
    p-> appendInt(randomLength);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesGetDhConfigMethods);
    delete p;
    return ret;
}

// ### messages.requestEncryption
void Api::onMessagesRequestEncryptionAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT messagesRequestEncryptionEncryptedChat(q->msgId(), inboundPkt.fetchEncryptedChat());
    delete q;
}

qint64 Api::messagesRequestEncryption(const InputUser &user, qint32 randomId, QByteArray g_a) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesRequestEncryption);
    p-> appendInputUser(user);
    p-> appendInt(randomId);
    p-> appendBytes(g_a);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesRequestEncryptionMethods);
    delete p;
    return ret;
}

// ### message.acceptEncryption
void Api::onMessagesAcceptEncryptionAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT messagesAcceptEncryptionEncryptedChat(q->msgId(), inboundPkt.fetchEncryptedChat());
    delete q;
}

qint64 Api::messagesAcceptEncryption(const InputEncryptedChat &inputEncryptedChat, QByteArray g_b, qint64 keyFingerprint) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesAcceptEncryption);
    p-> appendInputEncryptedChat(inputEncryptedChat);
    p-> appendBytes(g_b);
    p-> appendLong(keyFingerprint);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesAcceptEncryptionMethods);
    delete p;
    return ret;
}

// ### messages.discardEncryption
void Api::onMessagesDiscardEncryptionAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT messagesDiscardEncryptionResult(q->msgId(), inboundPkt.fetchBool());
    delete q;
}

qint64 Api::messagesDiscardEncryption(qint32 chatId) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesDiscardEncryption);
    p-> appendInt(chatId);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesDiscardEncryptionMethods);
    delete p;
    return ret;
}

// ### messages.setEncryptedTyping
void Api::onMessagesSetEncryptedTypingAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT messagesSetEncryptedTypingResult(q->msgId(), inboundPkt.fetchBool());
    delete q;
}

qint64 Api::messagesSetEncryptedTyping(const InputEncryptedChat &inputEncryptedChat, bool typing) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSetEncryptedTyping);
    p-> appendInputEncryptedChat(inputEncryptedChat);
    p-> appendBool(typing);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSetEncryptedTypingMethods);
    delete p;
    return ret;
}

// ### messages.readEncryptedHistory
void Api::onMessagesReadEncryptedHistoryAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT messagesReadEncryptedHistoryResult(q->msgId(), inboundPkt.fetchBool());
    delete q;
}

qint64 Api::messagesReadEncryptedHistory(const InputEncryptedChat &inputEncryptedChat, qint32 maxDate){
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesReadEncryptedHistory);
    p-> appendInputEncryptedChat(inputEncryptedChat);
    p-> appendInt(maxDate);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesReadEncryptedHistoryMethods);
    delete p;
    return ret;

}

// ### messages.sendEncrypted
void Api::onMessagesSendEncryptedAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesSentEncryptedMessage || x == (qint32)TL_MessagesSentEncryptedFile);
    qint32 date = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesSentEncryptedFile) {
        EncryptedFile file = inboundPkt.fetchEncryptedFile();
        Q_EMIT messagesSendEncryptedSentEncryptedFile(q->msgId(), date, file);
    } else {
        Q_EMIT messagesSendEncryptedSentEncryptedMessage(q->msgId(), date);
    }
    delete q;
}

qint64 Api::messagesSendEncrypted(const InputEncryptedChat &inputEncryptedChat, qint64 randomId, QByteArray data) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSendEncrypted);
    p-> appendInputEncryptedChat(inputEncryptedChat);
    p-> appendLong(randomId);
    p-> appendBytes(data);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSendEncryptedMethods);
    delete p;
    return ret;
}

// ### messages.sendEncryptedFile
void Api::onMessagesSendEncryptedFileAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesSentEncryptedMessage || x == (qint32)TL_MessagesSentEncryptedFile);
    qint32 date = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesSentEncryptedFile) {
        EncryptedFile file = inboundPkt.fetchEncryptedFile();
        Q_EMIT messagesSendEncryptedFileSentEncryptedFile(q->msgId(), date, file);
    } else {
        Q_EMIT messagesSendEncryptedFileSentEncryptedMessage(q->msgId(), date);
    }
    delete q;
}

qint64 Api::messagesSendEncryptedFile(const InputEncryptedChat &inputEncryptedChat, qint64 randomId, QByteArray data,
                                      const InputEncryptedFile &inputEncryptedFile) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSendEncrypted);
    p-> appendInputEncryptedChat(inputEncryptedChat);
    p-> appendLong(randomId);
    p-> appendBytes(data);
    p-> appendInputEncryptedFile(inputEncryptedFile);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSendEncryptedFileMethods);
    delete p;
    return ret;

}

// ### messages.sendEncryptedService
void Api::onMessagesSendEncryptedServiceAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_MessagesSentEncryptedMessage || x == (qint32)TL_MessagesSentEncryptedFile);
    qint32 date = inboundPkt.fetchInt();
    if (x == (qint32)TL_MessagesSentEncryptedFile) {
        EncryptedFile file = inboundPkt.fetchEncryptedFile();
        Q_EMIT messagesSendEncryptedServiceSentEncryptedFile(q->msgId(), date, file);
    } else {
        Q_EMIT messagesSendEncryptedServiceSentEncryptedMessage(q->msgId(), date);
    }
    delete q;
}

qint64 Api::messagesSendEncryptedService(const InputEncryptedChat &inputEncryptedChat, qint64 randomId, QByteArray data) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesSendEncryptedService);
    p-> appendInputEncryptedChat(inputEncryptedChat);
    p-> appendLong(randomId);
    p-> appendBytes(data);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesSendEncryptedServiceMethods);
    delete p;
    return ret;
}

// ### messages.receivedQueue
void Api::onMessagesReceivedQueueAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<qint64> randomIds;
    for (qint32 i = 0; i < n; i++) {
        randomIds.append(inboundPkt.fetchLong());
    }
    Q_EMIT messagesReceivedQueueResult(q->msgId(), randomIds);
    delete q;
}

qint64 Api::messagesReceivedQueue(qint32 maxQts) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_MessagesReceivedQueue);
    p-> appendInt(maxQts);
    qint64 ret =  mMainSession->sendQuery(*p, &messagesReceivedQueueMethods);
    delete p;
    return ret;
}

//
// UPDATES
//
// ### updates.getState
void Api::onUpdatesGetStateAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_UpdatesState);
    qint32 pts = inboundPkt.fetchInt();
    qint32 qts = inboundPkt.fetchInt();
    qint32 date = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    qint32 unreadCount = inboundPkt.fetchInt();
    Q_EMIT updatesState(q->msgId(), pts, qts, date, seq, unreadCount);
    delete q;
}

qint64 Api::updatesGetState() {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_UpdatesGetState);
    qint64 ret =  mMainSession->sendQuery(*p, &updatesGetStateMethods);
    delete p;
    return ret;

}

// ### updates.getDifference
void Api::onUpdatesGetDifferenceAnswer(Query *q, InboundPkt &inboundPkt) {
    qint32 x = inboundPkt.fetchInt();
    ASSERT(x == (qint32)TL_UpdatesDifferenceEmpty || x == (qint32)TL_UpdatesDifferenceSlice || x == (qint32)TL_UpdatesDifference);
    if (x == (qint32)TL_UpdatesDifferenceEmpty) {
        qint32 date = inboundPkt.fetchInt();
        qint32 seq = inboundPkt.fetchInt();
        Q_EMIT updatesDifferenceEmpty(q->msgId(), date, seq);
    } else {
        //newMessages
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        qint32 n = inboundPkt.fetchInt();
        QList<Message> newMessages;
        for (qint32 i = 0; i < n; i++) {
            newMessages.append(inboundPkt.fetchMessage());
        }
        //newEncryptedMessages
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        QList<EncryptedMessage> encMessages;
        for (qint32 i = 0; i < n; i++) {
            encMessages.append(inboundPkt.fetchEncryptedMessage());
        }
        //otherUpdates
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        QList<Update> otherUpdates;
        for (qint32 i = 0; i < n; i++) {
            otherUpdates.append(inboundPkt.fetchUpdate());
        }
        //chats
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        QList<Chat> chats;
        for (qint32 i = 0; i < n; i++) {
            chats.append(inboundPkt.fetchChat());
        }
        //users
        ASSERT(inboundPkt.fetchInt() == (qint32)TL_Vector);
        n = inboundPkt.fetchInt();
        QList<User> users;
        for (qint32 i = 0; i < n; i++) {
            users.append(inboundPkt.fetchUser());
        }
        UpdatesState state = inboundPkt.fetchUpdatesState();
        if (x == (qint32)TL_UpdatesDifferenceSlice) {
            Q_EMIT updatesDifferenceSlice(q->msgId(), newMessages, encMessages, otherUpdates, chats, users, state);
        } else {
            Q_EMIT updatesDifference(q->msgId(), newMessages, encMessages, otherUpdates, chats, users, state);
        }
    }
    delete q;
}

qint64 Api::updatesGetDifference(qint32 pts, qint32 date, qint32 qts) {
    OutboundPkt *p = new OutboundPkt (); ;
    p-> appendInt(TL_UpdatesGetDifference);
    p-> appendInt(pts);
    p-> appendInt(date);
    p-> appendInt(qts);
    qint64 ret =  mMainSession->sendQuery(*p, &updatesGetDifferenceMethods);
    delete p;
  return ret;
}

//
// FILES
//
// ### upload.saveFilePart
void Api::onUploadSaveFilePartAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT uploadSaveFilePartResult(q->msgId(), q->extra().toLongLong(), inboundPkt.fetchBool());
    delete q;
}


qint64 Api::uploadSaveFilePart(Session *session, qint64 fileId, qint32 filePart, const QByteArray &bytes) {
    Q_ASSERT(session);
    OutboundPkt *p = new OutboundPkt (); ;
    if (session->initConnectionNeeded()) {
        p-> initConnection();
        session->setInitConnectionNeeded(false);
    }
    p-> appendInt(TL_UploadSaveFilePart);
    p-> appendLong(fileId);
    p-> appendInt(filePart);
    p-> appendBytes(bytes);
    QVariant extra = fileId;
    qint64 ret =  session->sendQuery(*p, &uploadSaveFilePartMethods, extra);
    delete p;
    return ret;
}

// ### upload.saveBigFilePart
void Api::onUploadSaveBigFilePartAnswer(Query *q, InboundPkt &inboundPkt) {
    Q_EMIT uploadSaveBigFilePartResult(q->msgId(), q->extra().toLongLong(), inboundPkt.fetchBool());
    delete q;
}

qint64 Api::uploadSaveBigFilePart(Session *session, qint64 fileId, qint32 filePart, qint32 fileTotalParts, const QByteArray &bytes) {
    Q_ASSERT(session);
    OutboundPkt *p = new OutboundPkt (); ;
    if (session->initConnectionNeeded()) {
        p-> initConnection();
        session->setInitConnectionNeeded(false);
    }
    p-> appendInt(TL_UploadSaveBigFilePart);
    p-> appendLong(fileId);
    p-> appendInt(filePart);
    p-> appendInt(fileTotalParts);
    p-> appendBytes(bytes);
    QVariant extra = fileId;
    qint64 ret =  session->sendQuery(*p, &uploadSaveBigFilePartMethods, extra);
    delete p;
    return ret;
}

// ### upload.getFile
void Api::onUploadGetFileError(Query *q, qint32 errorCode, const QString &errorText) {
    Q_EMIT uploadFileError(q->msgId(), errorCode, errorText);
    delete  q;
}

void Api::onUploadGetFileAnswer(Query *q, InboundPkt &inboundPkt) {
    ASSERT(inboundPkt.fetchInt() == (qint32)TL_UploadFile);
    StorageFileType storageType = inboundPkt.fetchStorageFileType();
    qint32 mtime = inboundPkt.fetchInt();
    QByteArray bytes = inboundPkt.fetchBytes();
    Q_EMIT uploadFile(q->msgId(), storageType, mtime, bytes);
    delete  q;
}

qint64 Api::uploadGetFile(Session *session, const InputFileLocation &location, qint32 offset, qint32 limit) {
    Q_ASSERT(session);
    OutboundPkt *p = new OutboundPkt (); ;
    if (session->initConnectionNeeded()) {
        p-> initConnection();
        session->setInitConnectionNeeded(false);
    }
    p-> appendInt(TL_UploadGetFile);
    p-> appendInputFileLocation(location);
    p-> appendInt(offset);
    p-> appendInt(limit);
    qint64 ret =  session->sendQuery(*p, &uploadGetFileMethods);
    delete p;
    return ret;
}

