// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "chatinviteobject.h"

ChatInviteObject::ChatInviteObject(const ChatInvite &core, QObject *parent) :
    TelegramTypeQObject(parent),
    m_chat(0),
    m_photo(0),
    m_core(core)
{
    m_chat = new ChatObject(m_core.chat(), this);
    connect(m_chat.data(), &ChatObject::coreChanged, this, &ChatInviteObject::coreChatChanged);
    m_photo = new ChatPhotoObject(m_core.photo(), this);
    connect(m_photo.data(), &ChatPhotoObject::coreChanged, this, &ChatInviteObject::corePhotoChanged);
}

ChatInviteObject::ChatInviteObject(QObject *parent) :
    TelegramTypeQObject(parent),
    m_chat(0),
    m_photo(0),
    m_core()
{
    m_chat = new ChatObject(m_core.chat(), this);
    connect(m_chat.data(), &ChatObject::coreChanged, this, &ChatInviteObject::coreChatChanged);
    m_photo = new ChatPhotoObject(m_core.photo(), this);
    connect(m_photo.data(), &ChatPhotoObject::coreChanged, this, &ChatInviteObject::corePhotoChanged);
}

ChatInviteObject::~ChatInviteObject() {
}

void ChatInviteObject::setBroadcast(bool broadcast) {
    if(m_core.broadcast() == broadcast) return;
    m_core.setBroadcast(broadcast);
    Q_EMIT broadcastChanged();
    Q_EMIT coreChanged();
}

bool ChatInviteObject::broadcast() const {
    return m_core.broadcast();
}

void ChatInviteObject::setChannel(bool channel) {
    if(m_core.channel() == channel) return;
    m_core.setChannel(channel);
    Q_EMIT channelChanged();
    Q_EMIT coreChanged();
}

bool ChatInviteObject::channel() const {
    return m_core.channel();
}

void ChatInviteObject::setChat(ChatObject* chat) {
    if(m_chat == chat) return;
    if(m_chat) delete m_chat;
    m_chat = chat;
    if(m_chat) {
        m_chat->setParent(this);
        m_core.setChat(m_chat->core());
        connect(m_chat.data(), &ChatObject::coreChanged, this, &ChatInviteObject::coreChatChanged);
    }
    Q_EMIT chatChanged();
    Q_EMIT coreChanged();
}

ChatObject*  ChatInviteObject::chat() const {
    return m_chat;
}

void ChatInviteObject::setFlags(qint32 flags) {
    if(m_core.flags() == flags) return;
    m_core.setFlags(flags);
    Q_EMIT flagsChanged();
    Q_EMIT coreChanged();
}

qint32 ChatInviteObject::flags() const {
    return m_core.flags();
}

void ChatInviteObject::setMegagroup(bool megagroup) {
    if(m_core.megagroup() == megagroup) return;
    m_core.setMegagroup(megagroup);
    Q_EMIT megagroupChanged();
    Q_EMIT coreChanged();
}

bool ChatInviteObject::megagroup() const {
    return m_core.megagroup();
}

void ChatInviteObject::setParticipants(const QList<User> &participants) {
    if(m_core.participants() == participants) return;
    m_core.setParticipants(participants);
    Q_EMIT participantsChanged();
    Q_EMIT coreChanged();
}

QList<User> ChatInviteObject::participants() const {
    return m_core.participants();
}

void ChatInviteObject::setParticipantsCount(qint32 participantsCount) {
    if(m_core.participantsCount() == participantsCount) return;
    m_core.setParticipantsCount(participantsCount);
    Q_EMIT participantsCountChanged();
    Q_EMIT coreChanged();
}

qint32 ChatInviteObject::participantsCount() const {
    return m_core.participantsCount();
}

void ChatInviteObject::setPhoto(ChatPhotoObject* photo) {
    if(m_photo == photo) return;
    if(m_photo) delete m_photo;
    m_photo = photo;
    if(m_photo) {
        m_photo->setParent(this);
        m_core.setPhoto(m_photo->core());
        connect(m_photo.data(), &ChatPhotoObject::coreChanged, this, &ChatInviteObject::corePhotoChanged);
    }
    Q_EMIT photoChanged();
    Q_EMIT coreChanged();
}

ChatPhotoObject*  ChatInviteObject::photo() const {
    return m_photo;
}

void ChatInviteObject::setPublicValue(bool publicValue) {
    if(m_core.publicValue() == publicValue) return;
    m_core.setPublicValue(publicValue);
    Q_EMIT publicValueChanged();
    Q_EMIT coreChanged();
}

bool ChatInviteObject::publicValue() const {
    return m_core.publicValue();
}

void ChatInviteObject::setTitle(const QString &title) {
    if(m_core.title() == title) return;
    m_core.setTitle(title);
    Q_EMIT titleChanged();
    Q_EMIT coreChanged();
}

QString ChatInviteObject::title() const {
    return m_core.title();
}

ChatInviteObject &ChatInviteObject::operator =(const ChatInvite &b) {
    if(m_core == b) return *this;
    m_core = b;
    m_chat->setCore(b.chat());
    m_photo->setCore(b.photo());

    Q_EMIT broadcastChanged();
    Q_EMIT channelChanged();
    Q_EMIT chatChanged();
    Q_EMIT flagsChanged();
    Q_EMIT megagroupChanged();
    Q_EMIT participantsChanged();
    Q_EMIT participantsCountChanged();
    Q_EMIT photoChanged();
    Q_EMIT publicValueChanged();
    Q_EMIT titleChanged();
    Q_EMIT coreChanged();
    return *this;
}

bool ChatInviteObject::operator ==(const ChatInvite &b) const {
    return m_core == b;
}

void ChatInviteObject::setClassType(quint32 classType) {
    ChatInvite::ChatInviteClassType result;
    switch(classType) {
    case TypeChatInviteAlready:
        result = ChatInvite::typeChatInviteAlready;
        break;
    case TypeChatInvite:
        result = ChatInvite::typeChatInvite;
        break;
    default:
        result = ChatInvite::typeChatInviteAlready;
        break;
    }

    if(m_core.classType() == result) return;
    m_core.setClassType(result);
    Q_EMIT classTypeChanged();
    Q_EMIT coreChanged();
}

quint32 ChatInviteObject::classType() const {
    int result;
    switch(static_cast<qint64>(m_core.classType())) {
    case ChatInvite::typeChatInviteAlready:
        result = TypeChatInviteAlready;
        break;
    case ChatInvite::typeChatInvite:
        result = TypeChatInvite;
        break;
    default:
        result = TypeChatInviteAlready;
        break;
    }

    return result;
}

void ChatInviteObject::setCore(const ChatInvite &core) {
    operator =(core);
}

ChatInvite ChatInviteObject::core() const {
    return m_core;
}

void ChatInviteObject::coreChatChanged() {
    if(m_core.chat() == m_chat->core()) return;
    m_core.setChat(m_chat->core());
    Q_EMIT chatChanged();
    Q_EMIT coreChanged();
}

void ChatInviteObject::corePhotoChanged() {
    if(m_core.photo() == m_photo->core()) return;
    m_core.setPhoto(m_photo->core());
    Q_EMIT photoChanged();
    Q_EMIT coreChanged();
}
