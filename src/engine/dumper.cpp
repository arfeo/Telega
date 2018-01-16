#include "dumper.h"
#include <QDebug>
#include <QDateTime>

void Dumper::dumpUser(const User &user, bool full)
{
    qDebug() << "#User";
    switch(user.classType()) {
        case User::typeUserEmpty: qDebug() << "type: empty"; break;
        case User::typeUserSelf: qDebug() << "type: self"; break;
        case User::typeUserContact: qDebug() << "type: contact"; break;
        case User::typeUserRequest: qDebug() << "type: request"; break;
        case User::typeUserForeign: qDebug() << "type: foreign"; break;
        case User::typeUserDeleted: qDebug() << "type: deleted"; break;
    }
    Q_ASSERT(user.id());
    qDebug() << "id:" << user.id();
    if (user.classType() != User::typeUserEmpty) {
        qDebug() << "firstName:" << user.firstName();
        qDebug() << "lastName:" << user.lastName();
        if (user.classType() != User::typeUserDeleted) {
            if (user.classType() != User::typeUserForeign) {
                Q_ASSERT(user.phone().length() > 0);
                qDebug() << "msisdn:" << user.phone();
            }
            if (full) {
                qDebug() << "accessHash:" << user.accessHash();
                dumpUserProfilePhoto(user.photo());
                dumpUserStatus(user.status());
                if (user.classType() == User::typeUserSelf) {
                    qDebug() << "inactive:" << user.inactive();
                }
            }
        }
    }
}

void Dumper::dumpUserProfilePhoto(const UserProfilePhoto &photo)
{
    qDebug() << "#UserProfilePhoto";
    if (photo.classType() == UserProfilePhoto::typeUserProfilePhoto) {
        Q_ASSERT(photo.photoId());
        qDebug() << "photoId:" << photo.photoId();
        qDebug() << "photoSmall:";
        dumpFileLocation(photo.photoSmall());
        qDebug() << "photoBig:";
        dumpFileLocation(photo.photoBig());
    }
}

void Dumper::dumpFileLocation(const FileLocation &location)
{
    QString dumpStr = "#FileLocation:";
    if (location.classType() == FileLocation::typeFileLocation) {
        Q_ASSERT(location.dcId());
        dumpStr.append(" dcId:"); dumpStr.append(QString::number(location.dcId()));
    }
    Q_ASSERT(location.volumeId());
    Q_ASSERT(location.localId());
    Q_ASSERT(location.secret());
    dumpStr.append(" volumenId:"); dumpStr.append(QString::number(location.volumeId()));
    dumpStr.append(" localId:"); dumpStr.append(QString::number(location.localId()));
    dumpStr.append(" secret:"); dumpStr.append(QString::number(location.secret()));
    qDebug() << dumpStr;
}

void Dumper::dumpUserStatus(const UserStatus &status)
{
    qDebug() << "#UserStatus";
    switch (status.classType()) {
    case UserStatus::typeUserStatusEmpty:
        qDebug() << "Status: empty";
        break;
    case UserStatus::typeUserStatusOffline:
        qDebug() << "status: offline";
        qDebug() << "wasOnline:" << Dumper::unixTimeToDate(status.wasOnline());
        break;
    case UserStatus::typeUserStatusOnline:
        qDebug() << "status: online";
        qDebug() << "expires:" << Dumper::unixTimeToDate(status.expires());
    }
}

void Dumper::dumpContactsLink(const ContactsLink &link)
{
    qDebug() << "#ContactsLink";
    dumpContactsMyLink(link.myLink());
    dumpContactsForeignLink(link.foreignLink());
    dumpUser(link.user());
}

void Dumper::dumpContactsMyLink(const ContactsMyLink &myLink)
{
    qDebug() << "#ContactsMyLink";
    switch(myLink.classType()) {
        case ContactsMyLink::typeContactsMyLinkEmpty: qDebug() << "type: empty"; break;
        case ContactsMyLink::typeContactsMyLinkRequested: qDebug() << "type: requested; contact:" << myLink.contact(); break;
        case ContactsMyLink::typeContactsMyLinkContact: qDebug() << "type: contact"; break;
    }
}

void Dumper::dumpContactsForeignLink(const ContactsForeignLink &foreignLink)
{
    qDebug() << "#ContactsForeignLink";
    switch(foreignLink.classType()) {
        case ContactsForeignLink::typeContactsForeignLinkUnknown: qDebug() << "type: unknown"; break;
        case ContactsForeignLink::typeContactsForeignLinkRequested: qDebug() << "type: requested; hasPhone:" << foreignLink.hasPhone(); break;
        case ContactsForeignLink::typeContactsForeignLinkMutual: qDebug() << "type: mutual"; break;
    }
}

QString Dumper::unixTimeToDate(uint unixTime)
{
    QDateTime timestamp = QDateTime::fromTime_t(unixTime);
    return "(" + QString::number(unixTime) + ") " + timestamp.toString(Qt::SystemLocaleDate);
}

void Dumper::dumpPhoto(const Photo &photo)
{
    qDebug() << "#Photo";
    qDebug() << "id:" << photo.id();
    if (photo.classType() == Photo::typePhoto) {
        qDebug() << "accessHash:" << photo.accessHash();
        qDebug() << "userId:" << photo.userId();
        qDebug() << "date:" << Dumper::unixTimeToDate(photo.date());
        qDebug() << "caption:" << photo.caption();
        qDebug() << "geo:";
        dumpGeoPoint(photo.geo());
        QList<PhotoSize> sizes = photo.sizes();
        Q_FOREACH (PhotoSize size, sizes) {
            qDebug() << "PhotoSize:";
            dumpPhotoSize(size);
        }
    }
}

void Dumper::dumpGeoPoint(const GeoPoint &geo)
{
    if (geo.classType() == GeoPoint::typeGeoPoint) {
        qDebug() << "longitude =" << geo.longitude() << " ,latitude =" << geo.lat();
    }
}

void Dumper::dumpPhotoSize(const PhotoSize &photoSize)
{
    qDebug() << "#PhotoSize";
    qDebug() << "type:" << photoSize.type();
    if (photoSize.classType() != PhotoSize::typePhotoSizeEmpty) {
        qDebug() << "Location:"; dumpFileLocation(photoSize.location());
        qDebug() << "width:" << photoSize.w();
        qDebug() << "height:" << photoSize.h();
        if (photoSize.classType() == PhotoSize::typePhotoSize) {
            qDebug() << "size:" << photoSize.size();
        } else {
            qDebug() << "bytes (as base64):" << photoSize.bytes().toHex();
        }
    }
}

void Dumper::dumpPeerNotifySettings(const PeerNotifySettings &notifySettings)
{
    qDebug() << "#PeerNotifySettings";
    if (notifySettings.classType() == PeerNotifySettings::typePeerNotifySettings) {
        qDebug() << "muteUntil:" << notifySettings.muteUntil();
        qDebug() << "sound:" << notifySettings.sound();
        qDebug() << "showPreviews:" << notifySettings.showPreviews();
        qDebug() << "eventsMask:" << notifySettings.eventsMask();
    }
}

void Dumper::dumpContact(const Contact &contact)
{
    qDebug() << "#Contact";
    qDebug() << "userId:" << contact.userId();
    qDebug() << "mutual:" << contact.mutual();
}

void Dumper::dumpDialog(const Dialog &dialog)
{
    qDebug() << "#Dialog";
    dumpPeer(dialog.peer());
    qDebug() << "topMessage:" << dialog.topMessage();
    qDebug() << "unreadCount:" << dialog.unreadCount();
    dumpPeerNotifySettings(dialog.notifySettings());
}

void Dumper::dumpPeer(const Peer &peer)
{
    if (peer.classType() == Peer::typePeerUser) {
        qDebug() << "userId:" << peer.userId();
    } else {
        qDebug() << "chatId:" << peer.chatId();
    }
}

void Dumper::dumpMessage(const Message &message)
{
    qDebug() << "#Message:";
    if (message.classType() != Message::typeMessageEmpty) {
        if (message.classType() == Message::typeMessageForwarded) {
            qDebug() << "fwdFromId:" << message.fwdFromId();
            qDebug() << "fwdDate:" << Dumper::unixTimeToDate(message.fwdDate());
        }
        qDebug() << "fromId:" << message.fromId();
        qDebug() << "toId:";
        dumpPeer(message.toId());
        qDebug() << "out:" << message.out();
        qDebug() << "unread:" << message.unread();
        qDebug() << "date:" << Dumper::unixTimeToDate(message.date());
        if (message.classType() == Message::typeMessageService) {
            dumpMessageAction(message.action());
        } else {
            qDebug() << "message:" << message.message();
            qDebug() << "message:" << message.id();
            dumpMessageMedia(message.media());
        }
    }
}

void Dumper::dumpMessageAction(const MessageAction &ma)
{
    qDebug() << "#MessageAction:";
    switch (ma.classType()) {
        case MessageAction::typeMessageActionEmpty: qDebug() << "action: empty"; break;
        case MessageAction::typeMessageActionChatCreate: qDebug() << "action: create chat"; break;
        case MessageAction::typeMessageActionChatEditTitle: qDebug() << "action: edit chat title"; break;
        case MessageAction::typeMessageActionChatEditPhoto: qDebug() << "action: edit chat photo"; break;
        case MessageAction::typeMessageActionChatDeletePhoto: qDebug() << "action: delete chat photo"; break;
        case MessageAction::typeMessageActionChatAddUser: qDebug() << "action: add user to chat"; break;
        case MessageAction::typeMessageActionChatDeleteUser: qDebug() << "action: delete user from chat"; break;
        case MessageAction::typeMessageActionGeoChatCreate: qDebug() << "action: create geochat"; break;
        case MessageAction::typeMessageActionGeoChatCheckin: qDebug() << "action: registrar geochat"; break;
    }
    if(ma.classType() == MessageAction::typeMessageActionChatCreate ||
            ma.classType() == MessageAction::typeMessageActionChatEditTitle ||
            ma.classType() == MessageAction::typeMessageActionGeoChatCreate) {
        qDebug() << "title:" << ma.title();
    } else if(ma.classType() == MessageAction::typeMessageActionChatAddUser ||
               ma.classType() == MessageAction::typeMessageActionChatDeleteUser) {
        qDebug() << "userId:" << ma.userId();
    } else if(ma.classType() == MessageAction::typeMessageActionChatEditPhoto) {
        dumpPhoto(ma.photo());
    }
    if(ma.classType() == MessageAction::typeMessageActionChatCreate) {
        Q_FOREACH(qint32 userId, ma.users()) {
            qDebug() << "userId:" << userId;
        }
    } else if(ma.classType() == MessageAction::typeMessageActionGeoChatCreate) {
        qDebug() << "address:" << ma.address();
    }
}

void Dumper::dumpMessageMedia(const MessageMedia &mm)
{
    qDebug() << "#MessageMedia:";
    switch (mm.classType()) {
        case MessageMedia::typeMessageMediaEmpty:
            break;
        case MessageMedia::typeMessageMediaPhoto:
            dumpPhoto(mm.photo());
            break;
        case MessageMedia::typeMessageMediaVideo:
            dumpVideo(mm.video());
            break;
        case MessageMedia::typeMessageMediaGeo:
            dumpGeoPoint(mm.geo());
            break;
        case MessageMedia::typeMessageMediaContact:
            qDebug() << "phoneNumber:" << mm.phoneNumber();
            qDebug() << "firstName:" << mm.firstName();
            qDebug() << "lastName:" << mm.lastName();
            qDebug() << "userId:" << mm.userId();
            break;
        case MessageMedia::typeMessageMediaUnsupported:
            qDebug() << "bytes:" << mm.bytes().toHex();
            break;
        case MessageMedia::typeMessageMediaDocument:
            dumpDocument(mm.document());
            break;
        case MessageMedia::typeMessageMediaAudio:
            dumpAudio(mm.audio());
            break;
    }
}

void Dumper::dumpVideo(const Video &video)
{
    qDebug() << "#Video:";
    qDebug() << "id:" << video.id();
    if (video.classType() == Video::typeVideo) {
        qDebug() << "accessHash:" << video.accessHash();
        qDebug() << "userId:" << video.userId();
        qDebug() << "date:" << Dumper::unixTimeToDate(video.date());
        qDebug() << "caption:" << video.caption();
        qDebug() << "duration:" << video.duration();
        qDebug() << "mimeType:" << video.mimeType();
        qDebug() << "size:" << video.size();
        qDebug() << "thumb:";
        dumpPhotoSize(video.thumb());
        qDebug() << "dcId:" << video.dcId();
        qDebug() << "width:" << video.w();
        qDebug() << "height:" << video.h();
    }
}

void Dumper::dumpAudio(const Audio &audio)
{
    qDebug() << "#Audio:";
    qDebug() << "id:" << audio.id();
    if (audio.classType() == Audio::typeAudio) {
        qDebug() << "accessHash:" << audio.accessHash();
        qDebug() << "userId:" << audio.userId();
        qDebug() << "date:" << Dumper::unixTimeToDate(audio.date());
        qDebug() << "duration:" << audio.duration();
        qDebug() << "mimeType:" << audio.mimeType();
        qDebug() << "size:" << audio.size();
        qDebug() << "dcId:" << audio.dcId();
    }
}

void Dumper::dumpDocument(const Document &doc)
{
    qDebug() << "#Document:";
    qDebug() << "id:" << doc.id();
    if (doc.classType() == Document::typeDocument) {
        qDebug() << "accessHash:" << doc.accessHash();
        qDebug() << "userId:" << doc.userId();
        qDebug() << "date:" << Dumper::unixTimeToDate(doc.date());
        qDebug() << "fileName:" << doc.fileName();
        qDebug() << "mimeType:" << doc.mimeType();
        qDebug() << "size:" << doc.size();
        qDebug() << "thumb:";
        dumpPhotoSize(doc.thumb());
        qDebug() << "dcId:" << doc.dcId();
    }
}

void Dumper::dumpContactStatus(const ContactStatus &status)
{
    qDebug() << "#ContactStatus";
    qDebug() << "userId:" << status.userId();
    qDebug() << "expires:" << Dumper::unixTimeToDate(status.expires());
}

void Dumper::dumpImportedContact(const ImportedContact &imported)
{
    qDebug() << "#ImportedContact";
    qDebug() << "userId:" << imported.userId();
    qDebug() << "clientId:" << imported.clientId();
}

void Dumper::dumpContactBlocked(const ContactBlocked &blocked)
{
    qDebug() << "#ContactBlocked";
    qDebug() << "userId:" << blocked.userId();
    qDebug() << "date:" << Dumper::unixTimeToDate(blocked.date());
}

void Dumper::dumpUpdate(const Update &update)
{
    qDebug() << "#Update";
    Update::UpdateType x = update.classType();
    Q_ASSERT(x == Update::typeUpdateNewMessage ||
             x == Update::typeUpdateMessageID ||
             x == Update::typeUpdateReadMessages ||
             x == Update::typeUpdateDeleteMessages ||
             x == Update::typeUpdateRestoreMessages ||
             x == Update::typeUpdateUserTyping ||
             x == Update::typeUpdateChatUserTyping ||
             x == Update::typeUpdateChatParticipants ||
             x == Update::typeUpdateUserStatus ||
             x == Update::typeUpdateUserName ||
             x == Update::typeUpdateUserPhoto ||
             x == Update::typeUpdateContactRegistered ||
             x == Update::typeUpdateContactLink ||
             x == Update::typeUpdateActivation ||
             x == Update::typeUpdateNewAuthorization ||
             x == Update::typeUpdateNewGeoChatMessage ||
             x == Update::typeUpdateNewEncryptedMessage ||
             x == Update::typeUpdateEncryptedChatTyping ||
             x == Update::typeUpdateEncryption ||
             x == Update::typeUpdateEncryptedMessagesRead ||
             x == Update::typeUpdateChatParticipantAdd ||
             x == Update::typeUpdateChatParticipantDelete ||
             x == Update::typeUpdateDcOptions ||
             x == Update::typeUpdateUserBlocked ||
             x == Update::typeUpdateNotifySettings);

    switch (x) {
        case Update::typeUpdateNewMessage:
            qDebug() << "type: new message";
            dumpMessage(update.message());
            qDebug() << "pts: " << update.pts();
            break;
        case Update::typeUpdateMessageID:
            qDebug() << "type: messageId";
            qDebug() << "id:" << update.id();
            qDebug() << "randomId:" << update.randomId();
            break;
        case Update::typeUpdateReadMessages: {
            qDebug() << "type: read messages";
            QList<qint32> messages = update.messages();
            Q_FOREACH (qint32 msgId, messages) {
                qDebug() << "msgId:" << msgId;
            }
            qDebug() << "pts: " << update.pts();
            break;
        }
        case Update::typeUpdateDeleteMessages: {
            qDebug() << "type: delete messages";
            QList<qint32> messages = update.messages();
            Q_FOREACH (qint32 msgId, messages) {
                qDebug() << "msgId:" << msgId;
            }
            qDebug() << "pts: " << update.pts();
            break;
        }
        case Update::typeUpdateRestoreMessages: {
            qDebug() << "type: restore messages";
            QList<qint32> messages = update.messages();
            Q_FOREACH (qint32 msgId, messages) {
                qDebug() << "msgId:" << msgId;
            }
            qDebug() << "pts: " << update.pts();
            break;
        }
        case Update::typeUpdateUserTyping:
            qDebug() << "type: user typing";
            qDebug() << "userId:" << update.userId();
            break;
        case Update::typeUpdateChatUserTyping:
            qDebug() << "type: chat user typing";
            qDebug() << "chatId:" << update.chatId();
            qDebug() << "userId:" << update.userId();
            break;
        case Update::typeUpdateChatParticipants:
            qDebug() << "type: chat participants";
            Q_FOREACH (ChatParticipant cp, update.participants().participants()) {
                dumpChatParticipant(cp);
            }
            break;
        case Update::typeUpdateUserStatus:
            qDebug() << "type: user status";
            qDebug() << "userId:" << update.userId();
            dumpUserStatus(update.status());
            break;
        case Update::typeUpdateUserName:
            qDebug() << "type: user name";
            qDebug() << "userId:" << update.userId();
            qDebug() << "firstName:" << update.firstName();
            qDebug() << "lastName:" << update.lastName();
            break;
        case Update::typeUpdateUserPhoto:
            qDebug() << "type: user photo";
            qDebug() << "userId:" << update.userId();
            qDebug() << "date:" << unixTimeToDate(update.date());
            dumpUserProfilePhoto(update.photo());
            qDebug() << "previous:" << update.previous();
            break;
        case Update::typeUpdateContactRegistered:
            qDebug() << "type: contact registered";
            qDebug() << "userId:" << update.userId();
            qDebug() << "date:" << unixTimeToDate(update.date());
            break;
        case Update::typeUpdateContactLink:
            qDebug() << "type: contact link";
            qDebug() << "userId:" << update.userId();
            dumpContactsMyLink(update.myLink());
            dumpContactsForeignLink(update.foreignLink());
            break;
        case Update::typeUpdateActivation:
            qDebug() << "type: activation";
            qDebug() << "userId:" << update.userId();
            break;
        case Update::typeUpdateNewAuthorization:
            qDebug() << "type: new authorization";
            qDebug() << "authKeyId:" << update.authKeyId();
            qDebug() << "date:" << unixTimeToDate(update.date());
            qDebug() << "device:" << update.device();
            qDebug() << "location:" << update.location();
            break;
        case Update::typeUpdateNewGeoChatMessage:
            qDebug() << "type: new geo chat message";
            dumpGeoChatMessage(update.geoChatMessage());
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
                dumpDcOption(dcOption);
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
            dumpNotifyPeer(update.peer());
            dumpPeerNotifySettings(update.notifySettings());
            break;
        default:
            qDebug() << "Update received in a not contemplated option";
            break;
    }

}

void Dumper::dumpNotifyPeer(const NotifyPeer &notifyPeer)
{
    qDebug() << "#NotifyPeer";
    switch (notifyPeer.classType()) {
    case NotifyPeer::typeNotifyPeer: qDebug() << "type: notifyPeer"; dumpPeer(notifyPeer.peer()); break;
    case NotifyPeer::typeNotifyUsers: qDebug() << "type: notifyUsers"; break;
    case NotifyPeer::typeNotifyChats: qDebug() << "type: notifyChats"; break;
    case NotifyPeer::typeNotifyAll: qDebug() << "type: notifyAll"; break;
    }
}

void Dumper::dumpChatParticipant(const ChatParticipant &p)
{
    qDebug() << "#ChatParticipant";
    qDebug() << "userId:" << p.userId();
    qDebug() << "inviterId:" << p.inviterId();
    qDebug() << "date:" << unixTimeToDate(p.date());
}

void Dumper::dumpGeoChatMessage(const GeoChatMessage &msg)
{
    qDebug() << "#GeoChatMessage";
    qDebug() << "chatId:" << msg.chatId();
    qDebug() << "id:" << msg.id();
    GeoChatMessage::GeoChatMessageType x = msg.classType();
    if (x != GeoChatMessage::typeGeoChatMessageEmpty) {
        qDebug() << "fromId:" << msg.fromId();
        qDebug() << "date:" << unixTimeToDate(msg.date());
        if (x == GeoChatMessage::typeGeoChatMessage) {
            qDebug() << "message:" << msg.message();
            qDebug() << "messageMedia:";
            dumpMessageMedia(msg.media());
        } else if (x == GeoChatMessage::typeGeoChatMessageService) {
            qDebug() << "messageAction:";
            dumpMessageAction(msg.action());
        }
    }
}

void Dumper::dumpDcOption(const DcOption &dcOption)
{
    qDebug() << "#DcOption:";
    qDebug() << "id:" << dcOption.id();
    qDebug() << "hostname:" << dcOption.hostname();
    qDebug() << "ipAddress:" << dcOption.ipAddress();
    qDebug() << "port:" << dcOption.port();
}

void Dumper::dumpChat(const Chat &chat)
{
    qDebug() << "#Chat";
    qDebug() << "chatId:" << chat.id();
    Chat::ChatType x = chat.classType();
    if (x != Chat::typeChatEmpty) {
        if (x == Chat::typeGeoChat) {
            qDebug() << "accessHash:" << chat.accessHash();
        }
        qDebug() << "title:" << chat.title();
        if (x == Chat::typeGeoChat) {
            qDebug() << "address:" << chat.address();
            qDebug() << "venue:" << chat.venue();
            Dumper::dumpGeoPoint(chat.geo());
        }
        if (x != Chat::typeChatForbidden) {
            Dumper::dumpChatPhoto(chat.photo());
            qDebug() << "participantsCount:" << chat.participantsCount();
        }
        qDebug() << "date:" << Dumper::unixTimeToDate(chat.date());
        if (x == Chat::typeGeoChat) {
            qDebug() << "checkedIn:" << chat.checkedIn();
        } else if (x == Chat::typeChat) {
            qDebug() << "left:" << chat.left();
        }
        if (x != Chat::typeChatForbidden) {
            qDebug() << "version:" << chat.version();
        }
    }
}

void Dumper::dumpChatPhoto(const ChatPhoto &chatPhoto)
{
    qDebug() << "#ChatPhoto";
    if(chatPhoto.classType() == ChatPhoto::typeChatPhoto) {
        dumpFileLocation(chatPhoto.photoSmall());
        dumpFileLocation(chatPhoto.photoBig());
    }
}

void Dumper::dumpStatedMessage(const Message &message, const QList<Chat> &chats, const QList<User> &users, const QList<ContactsLink> &links, qint32 pts, qint32 seq)
{
    Dumper::dumpMessage(message);
    qDebug() << "Chats:";
    Q_FOREACH (Chat chat, chats) {
        Dumper::dumpChat(chat);
        qDebug() << "";
    }
    qDebug() << "Users:";
    Q_FOREACH (User user, users) {
        Dumper::dumpUser(user);
        qDebug() << "";
    }
    qDebug() << "ContactsLink:";
    Q_FOREACH (ContactsLink link, links) {
        Dumper::dumpContactsLink(link);
        qDebug() << "";
    }
    qDebug() << "pts:" << pts;
    qDebug() << "seq:" << seq;
}

void Dumper::dumpStatedMessages(const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users, const QList<ContactsLink> &links, qint32 pts, qint32 seq)
{
    qDebug() << "Messages:";
    Q_FOREACH (Message message, messages) {
        Dumper::dumpMessage(message);
        qDebug() << "";
    }
    qDebug() << "Chats:";
    Q_FOREACH (Chat chat, chats) {
        Dumper::dumpChat(chat);
        qDebug() << "";
    }
    qDebug() << "Users:";
    Q_FOREACH (User user, users) {
        Dumper::dumpUser(user);
        qDebug() << "";
    }
    qDebug() << "ContactsLink:";
    Q_FOREACH (ContactsLink link, links) {
        Dumper::dumpContactsLink(link);
        qDebug() << "";
    }
    qDebug() << "pts:" << pts;
    qDebug() << "seq:" << seq;
}

void Dumper::dumpChatParticipants(const ChatParticipants &cps)
{
    qDebug() << "#ChatParticipants";
    qDebug() << "chatId:" << cps.chatId();
    if (cps.classType() == ChatParticipants::typeChatParticipants) {
        qDebug() << "adminId:" << cps.adminId();
        Q_FOREACH (ChatParticipant cp, cps.participants()) {
            dumpChatParticipant(cp);
        }
        qDebug() << "version:" << cps.version();
    } else {
        qDebug() << "chatParticipantsForbidden: info on members unavailable";
    }
}

void Dumper::dumpChatFull(const ChatFull &chat)
{
    qDebug() << "#ChatFull";
    qDebug() << "chatFullId:" << chat.id();
    dumpChatParticipants(chat.participants());
    dumpPhoto(chat.chatPhoto());
    dumpPeerNotifySettings(chat.notifySettings());
}

void Dumper::dumpEncryptedMessage(const EncryptedMessage &msg)
{
    qDebug() << "#EncryptedMessage";
    qDebug() << "randomId:" << msg.randomId();
    qDebug() << "chatId:" << msg.chatId();
    qDebug() << "date:" << unixTimeToDate(msg.date());
    qDebug() << "bytes:" << msg.bytes();
    if (msg.classType() == EncryptedMessage::typeEncryptedMessage) {
        dumpEncryptedFile(msg.file());
    }
}

void Dumper::dumpEncryptedFile(const EncryptedFile &file)
{
    qDebug() << "#EncryptedFile";
    if (file.classType() == EncryptedFile::typeEncryptedFile) {
        qDebug() << "id:" << file.id();
        qDebug() << "accessHash:" << file.accessHash();
        qDebug() << "size:" << file.size();
        qDebug() << "dcId:" << file.dcId();
        qDebug() << "keyFingerprint:" << file.keyFingerprint();
    }
}

void Dumper::dumpUpdatesState(const UpdatesState &update)
{
    qDebug() << "#UpdatesState";
    qDebug() << "pts:" << update.pts();
    qDebug() << "qts:" << update.qts();
    qDebug() << "date:" << update.date();
    qDebug() << "seq:" << update.seq();
    qDebug() << "unreadCount:" << update.unreadCount();
}

void Dumper::dumpWallPaper(const WallPaper &wallPaper)
{
    qDebug() << "#WallPaper";
    qDebug() << "id:" << wallPaper.id();
    qDebug() << "title:" << wallPaper.title();
    if (wallPaper.classType() == WallPaper::typeWallPaper) {
        Q_FOREACH (PhotoSize size, wallPaper.sizes()) {
            dumpPhotoSize(size);
        }
    } else {
        qDebug() << "bgColor:" << wallPaper.bgColor();
    }
    qDebug() << "color:" << wallPaper.color();
}

void Dumper::dumpStorageFileType(const StorageFileType &type)
{
    switch (type.classType()) {
        case StorageFileType::typeStorageFileUnknown: qDebug() << "file type: unknown"; break;
        case StorageFileType::typeStorageFileJpeg: qDebug() << "file type: jpeg"; break;
        case StorageFileType::typeStorageFileGif: qDebug() << "file type: gif"; break;
        case StorageFileType::typeStorageFilePng: qDebug() << "file type: png"; break;
        case StorageFileType::typeStorageFilePdf: qDebug() << "file type: pdf"; break;
        case StorageFileType::typeStorageFileMp3: qDebug() << "file type: mp3"; break;
        case StorageFileType::typeStorageFileMov: qDebug() << "file type: mov"; break;
        case StorageFileType::typeStorageFilePartial: qDebug() << "file type: partial"; break;
        case StorageFileType::typeStorageFileMp4: qDebug() << "file type: mp4"; break;
        case StorageFileType::typeStorageFileWebp: qDebug() << "file type: webp"; break;
    }
}
