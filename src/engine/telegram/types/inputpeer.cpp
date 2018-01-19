// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "inputpeer.h"
#include "core/inboundpkt.h"
#include "core/outboundpkt.h"
#include "../coretypes.h"

#include <QDataStream>

InputPeer::InputPeer(InputPeerClassType classType, InboundPkt *in) :
    m_accessHash(0),
    m_channelId(0),
    m_chatId(0),
    m_userId(0),
    m_classType(classType)
{
    if(in) fetch(in);
}

InputPeer::InputPeer(InboundPkt *in) :
    m_accessHash(0),
    m_channelId(0),
    m_chatId(0),
    m_userId(0),
    m_classType(typeInputPeerEmpty)
{
    fetch(in);
}

InputPeer::InputPeer(const InputPeer &another) :
    TelegramTypeObject(),
    m_accessHash(0),
    m_channelId(0),
    m_chatId(0),
    m_userId(0),
    m_classType(typeInputPeerEmpty)
{
    operator=(another);
}

InputPeer::InputPeer(const Null &null) :
    TelegramTypeObject(null),
    m_accessHash(0),
    m_channelId(0),
    m_chatId(0),
    m_userId(0),
    m_classType(typeInputPeerEmpty)
{
}

InputPeer::~InputPeer() {

}

void InputPeer::setAccessHash(qint64 accessHash) {
    m_accessHash = accessHash;
}

qint64 InputPeer::accessHash() const {
    return m_accessHash;
}

void InputPeer::setChannelId(qint32 channelId) {
    m_channelId = channelId;
}

qint32 InputPeer::channelId() const {
    return m_channelId;
}

void InputPeer::setChatId(qint32 chatId) {
    m_chatId = chatId;
}

qint32 InputPeer::chatId() const {
    return m_chatId;
}

void InputPeer::setUserId(qint32 userId) {
    m_userId = userId;
}

qint32 InputPeer::userId() const {
    return m_userId;
}

bool InputPeer::operator ==(const InputPeer &b) const {
    return m_classType == b.m_classType &&
           m_accessHash == b.m_accessHash &&
           m_channelId == b.m_channelId &&
           m_chatId == b.m_chatId &&
           m_userId == b.m_userId;
}

InputPeer &InputPeer::operator =(const InputPeer &b) {
    m_classType = b.m_classType;
    m_accessHash = b.m_accessHash;
    m_channelId = b.m_channelId;
    m_chatId = b.m_chatId;
    m_userId = b.m_userId;
    setNull(b.isNull());
    return *this;
}

void InputPeer::setClassType(InputPeer::InputPeerClassType classType) {
    m_classType = classType;
}

InputPeer::InputPeerClassType InputPeer::classType() const {
    return m_classType;
}

bool InputPeer::fetch(InboundPkt *in) {
    LQTG_FETCH_LOG;
    int x = in->fetchInt();
    switch(x) {
    case typeInputPeerEmpty: {
        m_classType = static_cast<InputPeerClassType>(x);
        return true;
    }
        break;

    case typeInputPeerSelf: {
        m_classType = static_cast<InputPeerClassType>(x);
        return true;
    }
        break;

    case typeInputPeerChat: {
        m_chatId = in->fetchInt();
        m_classType = static_cast<InputPeerClassType>(x);
        return true;
    }
        break;

    case typeInputPeerUser: {
        m_userId = in->fetchInt();
        m_accessHash = in->fetchLong();
        m_classType = static_cast<InputPeerClassType>(x);
        return true;
    }
        break;

    case typeInputPeerChannel: {
        m_channelId = in->fetchInt();
        m_accessHash = in->fetchLong();
        m_classType = static_cast<InputPeerClassType>(x);
        return true;
    }
        break;

    default:
        LQTG_FETCH_ASSERT;
        return false;
    }
}

bool InputPeer::push(OutboundPkt *out) const {
    out->appendInt(m_classType);
    switch(m_classType) {
    case typeInputPeerEmpty: {
        return true;
    }
        break;

    case typeInputPeerSelf: {
        return true;
    }
        break;

    case typeInputPeerChat: {
        out->appendInt(m_chatId);
        return true;
    }
        break;

    case typeInputPeerUser: {
        out->appendInt(m_userId);
        out->appendLong(m_accessHash);
        return true;
    }
        break;

    case typeInputPeerChannel: {
        out->appendInt(m_channelId);
        out->appendLong(m_accessHash);
        return true;
    }
        break;

    default:
        return false;
    }
}

QMap<QString, QVariant> InputPeer::toMap() const {
    QMap<QString, QVariant> result;
    switch(static_cast<int>(m_classType)) {
    case typeInputPeerEmpty: {
        result["classType"] = "InputPeer::typeInputPeerEmpty";
        return result;
    }
        break;

    case typeInputPeerSelf: {
        result["classType"] = "InputPeer::typeInputPeerSelf";
        return result;
    }
        break;

    case typeInputPeerChat: {
        result["classType"] = "InputPeer::typeInputPeerChat";
        if( chatId() ) result["chatId"] = QString::number(chatId());
        return result;
    }
        break;

    case typeInputPeerUser: {
        result["classType"] = "InputPeer::typeInputPeerUser";
        if( userId() ) result["userId"] = QString::number(userId());
        if( accessHash() ) result["accessHash"] = QString::number(accessHash());
        return result;
    }
        break;

    case typeInputPeerChannel: {
        result["classType"] = "InputPeer::typeInputPeerChannel";
        if( channelId() ) result["channelId"] = QString::number(channelId());
        if( accessHash() ) result["accessHash"] = QString::number(accessHash());
        return result;
    }
        break;

    default:
        return result;
    }
}

InputPeer InputPeer::fromMap(const QMap<QString, QVariant> &map) {
    InputPeer result;
    if(map.value("classType").toString() == "InputPeer::typeInputPeerEmpty") {
        result.setClassType(typeInputPeerEmpty);
        return result;
    }
    if(map.value("classType").toString() == "InputPeer::typeInputPeerSelf") {
        result.setClassType(typeInputPeerSelf);
        return result;
    }
    if(map.value("classType").toString() == "InputPeer::typeInputPeerChat") {
        result.setClassType(typeInputPeerChat);
        QVariant _chatId_var = map.value("chatId");
        if( !_chatId_var.isNull() ) {
            _chatId_var.convert( QVariant::nameToType("qint32") );
            result.setChatId( _chatId_var.value<qint32>() );
        }

        return result;
    }
    if(map.value("classType").toString() == "InputPeer::typeInputPeerUser") {
        result.setClassType(typeInputPeerUser);
        QVariant _userId_var = map.value("userId");
        if( !_userId_var.isNull() ) {
            _userId_var.convert( QVariant::nameToType("qint32") );
            result.setUserId( _userId_var.value<qint32>() );
        }

        QVariant _accessHash_var = map.value("accessHash");
        if( !_accessHash_var.isNull() ) {
            _accessHash_var.convert( QVariant::nameToType("qint64") );
            result.setAccessHash( _accessHash_var.value<qint64>() );
        }

        return result;
    }
    if(map.value("classType").toString() == "InputPeer::typeInputPeerChannel") {
        result.setClassType(typeInputPeerChannel);
        QVariant _channelId_var = map.value("channelId");
        if( !_channelId_var.isNull() ) {
            _channelId_var.convert( QVariant::nameToType("qint32") );
            result.setChannelId( _channelId_var.value<qint32>() );
        }

        QVariant _accessHash_var = map.value("accessHash");
        if( !_accessHash_var.isNull() ) {
            _accessHash_var.convert( QVariant::nameToType("qint64") );
            result.setAccessHash( _accessHash_var.value<qint64>() );
        }

        return result;
    }
    return result;
}

InputPeer InputPeer::fromJson(const QString &json) {
    return InputPeer::fromMap(QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap());
}

QByteArray InputPeer::getHash(QCryptographicHash::Algorithm alg) const {
    QByteArray data;
    QDataStream str(&data, QIODevice::WriteOnly);
    str << *this;
    return QCryptographicHash::hash(data, alg);
}

QDataStream &operator<<(QDataStream &stream, const InputPeer &item) {
    stream << static_cast<uint>(item.classType());
    switch(item.classType()) {
    case InputPeer::typeInputPeerEmpty:

        break;
    case InputPeer::typeInputPeerSelf:

        break;
    case InputPeer::typeInputPeerChat:
        stream << item.chatId();
        break;
    case InputPeer::typeInputPeerUser:
        stream << item.userId();
        stream << item.accessHash();
        break;
    case InputPeer::typeInputPeerChannel:
        stream << item.channelId();
        stream << item.accessHash();
        break;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, InputPeer &item) {
    uint type = 0;
    stream >> type;
    item.setClassType(static_cast<InputPeer::InputPeerClassType>(type));
    switch(type) {
    case InputPeer::typeInputPeerEmpty: {

    }
        break;
    case InputPeer::typeInputPeerSelf: {

    }
        break;
    case InputPeer::typeInputPeerChat: {
        qint32 m_chat_id;
        stream >> m_chat_id;
        item.setChatId(m_chat_id);
    }
        break;
    case InputPeer::typeInputPeerUser: {
        qint32 m_user_id;
        stream >> m_user_id;
        item.setUserId(m_user_id);
        qint64 m_access_hash;
        stream >> m_access_hash;
        item.setAccessHash(m_access_hash);
    }
        break;
    case InputPeer::typeInputPeerChannel: {
        qint32 m_channel_id;
        stream >> m_channel_id;
        item.setChannelId(m_channel_id);
        qint64 m_access_hash;
        stream >> m_access_hash;
        item.setAccessHash(m_access_hash);
    }
        break;
    }
    return stream;
}

/*QDebug operator<<(QDebug debug,  const InputPeer &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)
    debug.nospace() << "Telegram.InputPeer(";
    switch(item.classType()) {
    case InputPeer::typeInputPeerEmpty:
        debug.nospace() << "classType: typeInputPeerEmpty";
        break;
    case InputPeer::typeInputPeerSelf:
        debug.nospace() << "classType: typeInputPeerSelf";
        break;
    case InputPeer::typeInputPeerChat:
        debug.nospace() << "classType: typeInputPeerChat";
        debug.nospace() << ", chatId: " << item.chatId();
        break;
    case InputPeer::typeInputPeerUser:
        debug.nospace() << "classType: typeInputPeerUser";
        debug.nospace() << ", userId: " << item.userId();
        debug.nospace() << ", accessHash: " << item.accessHash();
        break;
    case InputPeer::typeInputPeerChannel:
        debug.nospace() << "classType: typeInputPeerChannel";
        debug.nospace() << ", channelId: " << item.channelId();
        debug.nospace() << ", accessHash: " << item.accessHash();
        break;
    }
    debug.nospace() << ")";
    return debug;
}
*/