#ifndef DUMPER_H
#define DUMPER_H

#include <QObject>
#include "types/types.h"

class Dumper : public QObject
{
    Q_OBJECT
public:
    static void dumpUser(const User &user, bool full = true);
    static void dumpContactsLink(const ContactsLink &link);
    static void dumpPhoto(const Photo &photo);
    static void dumpPeerNotifySettings(const PeerNotifySettings &notifySettings);
    static void dumpContact(const Contact &contact);
    static void dumpDialog(const Dialog &dialog);
    static void dumpMessage(const Message &message);
    static void dumpContactStatus(const ContactStatus &status);
    static void dumpImportedContact(const ImportedContact &imported);
    static void dumpContactsMyLink(const ContactsMyLink &myLink);
    static void dumpContactsForeignLink(const ContactsForeignLink &foreignLink);
    static void dumpContactBlocked(const ContactBlocked &blocked);
    static void dumpUpdate(const Update &update);
    static void dumpChat(const Chat &chat);
    static void dumpChatFull(const ChatFull &chat);
    static void dumpStatedMessage(const Message &message, const QList<Chat> &chats, const QList<User> &users, const QList<ContactsLink> &links, qint32 pts, qint32 seq);
    static void dumpStatedMessages(const QList<Message> &messages, const QList<Chat> &chats, const QList<User> &users, const QList<ContactsLink> &links, qint32 pts, qint32 seq);
    static void dumpEncryptedMessage(const EncryptedMessage &msg);
    static void dumpUpdatesState(const UpdatesState &update);
    static void dumpWallPaper(const WallPaper &wallPaper);
    static void dumpUserProfilePhoto(const UserProfilePhoto &photo);
    static void dumpStorageFileType(const StorageFileType &type);
    static void dumpMessageMedia(const MessageMedia &mm);

    static QString unixTimeToDate(uint unixTime);

private:
    static void dumpFileLocation(const FileLocation &location);
    static void dumpUserStatus(const UserStatus &status);
    static void dumpGeoPoint(const GeoPoint &geo);
    static void dumpPhotoSize(const PhotoSize &photoSize);
    static void dumpPeer(const Peer &peer);
    static void dumpMessageAction(const MessageAction &ma);
    static void dumpVideo(const Video &video);
    static void dumpAudio(const Audio &audio);
    static void dumpDocument(const Document &doc);
    static void dumpChatParticipant(const ChatParticipant &p);
    static void dumpGeoChatMessage(const GeoChatMessage &msg);
    static void dumpDcOption(const DcOption &dcOption);
    static void dumpNotifyPeer(const NotifyPeer &notifyPeer);
    static void dumpChatPhoto(const ChatPhoto &ChatPhoto);
    static void dumpChatParticipants(const ChatParticipants &cps);
    static void dumpEncryptedFile(const EncryptedFile &file);
};

#endif // DUMPER_H
