// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "chatparticipant.h"
#include "core/inboundpkt.h"
#include "core/outboundpkt.h"
#include "../coretypes.h"

#include <QDataStream>

ChatParticipant::ChatParticipant(ChatParticipantClassType classType, InboundPkt *in) :
    m_date(0),
    m_inviterId(0),
    m_userId(0),
    m_classType(classType)
{
    if(in) fetch(in);
}

ChatParticipant::ChatParticipant(InboundPkt *in) :
    m_date(0),
    m_inviterId(0),
    m_userId(0),
    m_classType(typeChatParticipant)
{
    fetch(in);
}

ChatParticipant::ChatParticipant(const ChatParticipant &another) :
    TelegramTypeObject(),
    m_date(0),
    m_inviterId(0),
    m_userId(0),
    m_classType(typeChatParticipant)
{
    operator=(another);
}

ChatParticipant::ChatParticipant(const Null &null) :
    TelegramTypeObject(null),
    m_date(0),
    m_inviterId(0),
    m_userId(0),
    m_classType(typeChatParticipant)
{
}

ChatParticipant::~ChatParticipant() {
    
}

void ChatParticipant::setDate(qint32 date) {
    m_date = date;
}

qint32 ChatParticipant::date() const {
    return m_date;
}

void ChatParticipant::setInviterId(qint32 inviterId) {
    m_inviterId = inviterId;
}

qint32 ChatParticipant::inviterId() const {
    return m_inviterId;
}

void ChatParticipant::setUserId(qint32 userId) {
    m_userId = userId;
}

qint32 ChatParticipant::userId() const {
    return m_userId;
}

bool ChatParticipant::operator ==(const ChatParticipant &b) const {
    return m_classType == b.m_classType &&
           m_date == b.m_date &&
           m_inviterId == b.m_inviterId &&
           m_userId == b.m_userId;
}

ChatParticipant &ChatParticipant::operator =(const ChatParticipant &b) {
    m_classType = b.m_classType;
    m_date = b.m_date;
    m_inviterId = b.m_inviterId;
    m_userId = b.m_userId;
    setNull(b.isNull());
    return *this;
}

void ChatParticipant::setClassType(ChatParticipant::ChatParticipantClassType classType) {
    m_classType = classType;
}

ChatParticipant::ChatParticipantClassType ChatParticipant::classType() const {
    return m_classType;
}

bool ChatParticipant::fetch(InboundPkt *in) {
    LQTG_FETCH_LOG;
    int x = in->fetchInt();
    switch(x) {
    case typeChatParticipant: {
        m_userId = in->fetchInt();
        m_inviterId = in->fetchInt();
        m_date = in->fetchInt();
        m_classType = static_cast<ChatParticipantClassType>(x);
        return true;
    }
        break;
    
    case typeChatParticipantCreator: {
        m_userId = in->fetchInt();
        m_classType = static_cast<ChatParticipantClassType>(x);
        return true;
    }
        break;
    
    case typeChatParticipantAdmin: {
        m_userId = in->fetchInt();
        m_inviterId = in->fetchInt();
        m_date = in->fetchInt();
        m_classType = static_cast<ChatParticipantClassType>(x);
        return true;
    }
        break;
    
    default:
        LQTG_FETCH_ASSERT;
        return false;
    }
}

bool ChatParticipant::push(OutboundPkt *out) const {
    out->appendInt(m_classType);
    switch(m_classType) {
    case typeChatParticipant: {
        out->appendInt(m_userId);
        out->appendInt(m_inviterId);
        out->appendInt(m_date);
        return true;
    }
        break;
    
    case typeChatParticipantCreator: {
        out->appendInt(m_userId);
        return true;
    }
        break;
    
    case typeChatParticipantAdmin: {
        out->appendInt(m_userId);
        out->appendInt(m_inviterId);
        out->appendInt(m_date);
        return true;
    }
        break;
    
    default:
        return false;
    }
}

QMap<QString, QVariant> ChatParticipant::toMap() const {
    QMap<QString, QVariant> result;
    switch(static_cast<int>(m_classType)) {
    case typeChatParticipant: {
        result["classType"] = "ChatParticipant::typeChatParticipant";
        if( userId() ) result["userId"] = QString::number(userId());
        if( inviterId() ) result["inviterId"] = QString::number(inviterId());
        if( date() ) result["date"] = QString::number(date());
        return result;
    }
        break;
    
    case typeChatParticipantCreator: {
        result["classType"] = "ChatParticipant::typeChatParticipantCreator";
        if( userId() ) result["userId"] = QString::number(userId());
        return result;
    }
        break;
    
    case typeChatParticipantAdmin: {
        result["classType"] = "ChatParticipant::typeChatParticipantAdmin";
        if( userId() ) result["userId"] = QString::number(userId());
        if( inviterId() ) result["inviterId"] = QString::number(inviterId());
        if( date() ) result["date"] = QString::number(date());
        return result;
    }
        break;
    
    default:
        return result;
    }
}

ChatParticipant ChatParticipant::fromMap(const QMap<QString, QVariant> &map) {
    ChatParticipant result;
    if(map.value("classType").toString() == "ChatParticipant::typeChatParticipant") {
        result.setClassType(typeChatParticipant);
        QVariant _userId_var = map.value("userId");
        if( !_userId_var.isNull() ) {
            _userId_var.convert( QVariant::nameToType("qint32") );
            result.setUserId( _userId_var.value<qint32>() );
        }
        
        QVariant _inviterId_var = map.value("inviterId");
        if( !_inviterId_var.isNull() ) {
            _inviterId_var.convert( QVariant::nameToType("qint32") );
            result.setInviterId( _inviterId_var.value<qint32>() );
        }
        
        QVariant _date_var = map.value("date");
        if( !_date_var.isNull() ) {
            _date_var.convert( QVariant::nameToType("qint32") );
            result.setDate( _date_var.value<qint32>() );
        }
        
        return result;
    }
    if(map.value("classType").toString() == "ChatParticipant::typeChatParticipantCreator") {
        result.setClassType(typeChatParticipantCreator);
        QVariant _userId_var = map.value("userId");
        if( !_userId_var.isNull() ) {
            _userId_var.convert( QVariant::nameToType("qint32") );
            result.setUserId( _userId_var.value<qint32>() );
        }
        
        return result;
    }
    if(map.value("classType").toString() == "ChatParticipant::typeChatParticipantAdmin") {
        result.setClassType(typeChatParticipantAdmin);
        QVariant _userId_var = map.value("userId");
        if( !_userId_var.isNull() ) {
            _userId_var.convert( QVariant::nameToType("qint32") );
            result.setUserId( _userId_var.value<qint32>() );
        }
        
        QVariant _inviterId_var = map.value("inviterId");
        if( !_inviterId_var.isNull() ) {
            _inviterId_var.convert( QVariant::nameToType("qint32") );
            result.setInviterId( _inviterId_var.value<qint32>() );
        }
        
        QVariant _date_var = map.value("date");
        if( !_date_var.isNull() ) {
            _date_var.convert( QVariant::nameToType("qint32") );
            result.setDate( _date_var.value<qint32>() );
        }
        
        return result;
    }
    return result;
}

ChatParticipant ChatParticipant::fromJson(const QString &json) {
    return ChatParticipant::fromMap(QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap());
}

QByteArray ChatParticipant::getHash(QCryptographicHash::Algorithm alg) const {
    QByteArray data;
    QDataStream str(&data, QIODevice::WriteOnly);
    str << *this;
    return QCryptographicHash::hash(data, alg);
}

QDataStream &operator<<(QDataStream &stream, const ChatParticipant &item) {
    stream << static_cast<uint>(item.classType());
    switch(item.classType()) {
    case ChatParticipant::typeChatParticipant:
        stream << item.userId();
        stream << item.inviterId();
        stream << item.date();
        break;
    case ChatParticipant::typeChatParticipantCreator:
        stream << item.userId();
        break;
    case ChatParticipant::typeChatParticipantAdmin:
        stream << item.userId();
        stream << item.inviterId();
        stream << item.date();
        break;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, ChatParticipant &item) {
    uint type = 0;
    stream >> type;
    item.setClassType(static_cast<ChatParticipant::ChatParticipantClassType>(type));
    switch(type) {
    case ChatParticipant::typeChatParticipant: {
        qint32 m_user_id;
        stream >> m_user_id;
        item.setUserId(m_user_id);
        qint32 m_inviter_id;
        stream >> m_inviter_id;
        item.setInviterId(m_inviter_id);
        qint32 m_date;
        stream >> m_date;
        item.setDate(m_date);
    }
        break;
    case ChatParticipant::typeChatParticipantCreator: {
        qint32 m_user_id;
        stream >> m_user_id;
        item.setUserId(m_user_id);
    }
        break;
    case ChatParticipant::typeChatParticipantAdmin: {
        qint32 m_user_id;
        stream >> m_user_id;
        item.setUserId(m_user_id);
        qint32 m_inviter_id;
        stream >> m_inviter_id;
        item.setInviterId(m_inviter_id);
        qint32 m_date;
        stream >> m_date;
        item.setDate(m_date);
    }
        break;
    }
    return stream;
}

QDebug operator<<(QDebug debug,  const ChatParticipant &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)
    debug.nospace() << "Telegram.ChatParticipant(";
    switch(item.classType()) {
    case ChatParticipant::typeChatParticipant:
        debug.nospace() << "classType: typeChatParticipant";
        debug.nospace() << ", userId: " << item.userId();
        debug.nospace() << ", inviterId: " << item.inviterId();
        debug.nospace() << ", date: " << item.date();
        break;
    case ChatParticipant::typeChatParticipantCreator:
        debug.nospace() << "classType: typeChatParticipantCreator";
        debug.nospace() << ", userId: " << item.userId();
        break;
    case ChatParticipant::typeChatParticipantAdmin:
        debug.nospace() << "classType: typeChatParticipantAdmin";
        debug.nospace() << ", userId: " << item.userId();
        debug.nospace() << ", inviterId: " << item.inviterId();
        debug.nospace() << ", date: " << item.date();
        break;
    }
    debug.nospace() << ")";
    return debug;
}
