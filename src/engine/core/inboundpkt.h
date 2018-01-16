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

#ifndef INBOUNDPKT_H
#define INBOUNDPKT_H

#include <QObject>
#include <openssl/bn.h>
#include "../types/dcoption.h"
#include "../types/user.h"
#include "../types/userprofilephoto.h"
#include "../types/filelocation.h"
#include "../types/chat.h"
#include "../types/dialog.h"
#include "../types/peer.h"
#include "../types/peernotifysettings.h"
#include "../types/audio.h"
#include "../types/document.h"
#include "../types/message.h"
#include "../types/messageaction.h"
#include "../types/messagemedia.h"
#include "../types/photo.h"
#include "../types/photosize.h"
#include "../types/video.h"
#include "../types/chatparticipant.h"
#include "../types/chatparticipants.h"
#include "../types/contactsforeignlink.h"
#include "../types/contactsmylink.h"
#include "../types/geochatmessage.h"
#include "../types/contact.h"
#include "../types/update.h"
#include "../types/contactslink.h"
#include "../types/contactstatus.h"
#include "../types/importedcontact.h"
#include "../types/contactblocked.h"
#include "../types/storagefiletype.h"
#include "../types/chatfull.h"
#include "../types/encryptedmessage.h"
#include "../types/updatesstate.h"
#include "../types/encryptedchat.h"

class InboundPkt
{

public:
    explicit InboundPkt(char* buffer, qint32 len);
    const char *buffer() const;
    qint32 length() const;
    void setInPtr(qint32 *inPtr);
    void setInEnd(qint32 *inEnd);
    qint32 *inPtr();
    qint32 *inEnd();
    void forwardInPtr(qint32 positions);
    qint32 prefetchInt();
    qint32 fetchInt();
    bool fetchBool();
    qint32 *fetchInts(qint32 length);
    double fetchDouble();
    qint64 fetchLong();
    qint32 prefetchStrlen();
    char *fetchStr(qint32 length);
    QByteArray fetchBytes();
    QString fetchQString();
    qint32 fetchBignum (BIGNUM *x);
    qint32 fetchDate();
    DcOption fetchDcOption();
    User fetchUser();
    Chat fetchChat();
    Dialog fetchDialog();
    Peer fetchPeer();
    Message fetchMessage();
    Contact fetchContact();
    Update fetchUpdate();
    ContactsLink fetchContactsLink();
    Photo fetchPhoto();
    PeerNotifySettings fetchPeerNotifySetting();
    ContactStatus fetchContactStatus();
    ImportedContact fetchImportedContact();
    ContactsMyLink fetchContactsMyLink();
    ContactsForeignLink fetchContactsForeignLink();
    ContactBlocked fetchContactBlocked();
    StorageFileType fetchStorageFileType();
    ChatFull fetchChatFull();
    EncryptedMessage fetchEncryptedMessage();
    UpdatesState fetchUpdatesState();
    UserProfilePhoto fetchUserProfilePhoto();
    EncryptedChat fetchEncryptedChat();
    EncryptedFile fetchEncryptedFile();

protected:
    char *m_buffer;
    qint32 m_length;
    qint32 *m_inPtr;
    qint32 *m_inEnd;
    FileLocation fetchFileLocation();
    UserStatus fetchUserStatus();
    GeoPoint fetchGeoPoint();
    ChatPhoto fetchChatPhoto();
    MessageAction fetchMessageAction();
    PhotoSize fetchPhotoSize();
    MessageMedia fetchMessageMedia();
    Video fetchVideo();
    Document fetchDocument();
    Audio fetchAudio();
    ChatParticipant fetchChatParticipant();
    ChatParticipants fetchChatParticipants();
    GeoChatMessage fetchGeoChatMessage();
    NotifyPeer fetchNotifyPeer();

};

#endif // INBOUNDPKT_H
