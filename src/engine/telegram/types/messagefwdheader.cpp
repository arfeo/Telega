// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "messagefwdheader.h"
#include "core/inboundpkt.h"
#include "core/outboundpkt.h"
#include "../coretypes.h"

#include <QDataStream>

MessageFwdHeader::MessageFwdHeader(MessageFwdHeaderClassType classType, InboundPkt *in) :
    m_channelId(0),
    m_channelPost(0),
    m_date(0),
    m_flags(0),
    m_fromId(0),
    m_classType(classType)
{
    if(in) fetch(in);
}

MessageFwdHeader::MessageFwdHeader(InboundPkt *in) :
    m_channelId(0),
    m_channelPost(0),
    m_date(0),
    m_flags(0),
    m_fromId(0),
    m_classType(typeMessageFwdHeader)
{
    fetch(in);
}

MessageFwdHeader::MessageFwdHeader(const MessageFwdHeader &another) :
    TelegramTypeObject(),
    m_channelId(0),
    m_channelPost(0),
    m_date(0),
    m_flags(0),
    m_fromId(0),
    m_classType(typeMessageFwdHeader)
{
    operator=(another);
}

MessageFwdHeader::MessageFwdHeader(const Null &null) :
    TelegramTypeObject(null),
    m_channelId(0),
    m_channelPost(0),
    m_date(0),
    m_flags(0),
    m_fromId(0),
    m_classType(typeMessageFwdHeader)
{
}

MessageFwdHeader::~MessageFwdHeader() {

}

void MessageFwdHeader::setChannelId(qint32 channelId) {
    if(channelId) m_flags = (m_flags | (1<<1));
    else m_flags = (m_flags & ~(1<<1));
    m_channelId = channelId;
}

qint32 MessageFwdHeader::channelId() const {
    return m_channelId;
}

void MessageFwdHeader::setChannelPost(qint32 channelPost) {
    if(channelPost) m_flags = (m_flags | (1<<2));
    else m_flags = (m_flags & ~(1<<2));
    m_channelPost = channelPost;
}

qint32 MessageFwdHeader::channelPost() const {
    return m_channelPost;
}

void MessageFwdHeader::setDate(qint32 date) {
    m_date = date;
}

qint32 MessageFwdHeader::date() const {
    return m_date;
}

void MessageFwdHeader::setFlags(qint32 flags) {
    m_flags = flags;
}

qint32 MessageFwdHeader::flags() const {
    return m_flags;
}

void MessageFwdHeader::setFromId(qint32 fromId) {
    if(fromId) m_flags = (m_flags | (1<<0));
    else m_flags = (m_flags & ~(1<<0));
    m_fromId = fromId;
}

qint32 MessageFwdHeader::fromId() const {
    return m_fromId;
}

bool MessageFwdHeader::operator ==(const MessageFwdHeader &b) const {
    return m_classType == b.m_classType &&
           m_channelId == b.m_channelId &&
           m_channelPost == b.m_channelPost &&
           m_date == b.m_date &&
           m_flags == b.m_flags &&
           m_fromId == b.m_fromId;
}

MessageFwdHeader &MessageFwdHeader::operator =(const MessageFwdHeader &b) {
    m_classType = b.m_classType;
    m_channelId = b.m_channelId;
    m_channelPost = b.m_channelPost;
    m_date = b.m_date;
    m_flags = b.m_flags;
    m_fromId = b.m_fromId;
    setNull(b.isNull());
    return *this;
}

void MessageFwdHeader::setClassType(MessageFwdHeader::MessageFwdHeaderClassType classType) {
    m_classType = classType;
}

MessageFwdHeader::MessageFwdHeaderClassType MessageFwdHeader::classType() const {
    return m_classType;
}

bool MessageFwdHeader::fetch(InboundPkt *in) {
    LQTG_FETCH_LOG;
    int x = in->fetchInt();
    switch(x) {
    case typeMessageFwdHeader: {
        m_flags = in->fetchInt();
        if(m_flags & 1<<0) {
            m_fromId = in->fetchInt();
        }
        m_date = in->fetchInt();
        if(m_flags & 1<<1) {
            m_channelId = in->fetchInt();
        }
        if(m_flags & 1<<2) {
            m_channelPost = in->fetchInt();
        }
        m_classType = static_cast<MessageFwdHeaderClassType>(x);
        return true;
    }
        break;

    default:
        LQTG_FETCH_ASSERT;
        return false;
    }
}

bool MessageFwdHeader::push(OutboundPkt *out) const {
    out->appendInt(m_classType);
    switch(m_classType) {
    case typeMessageFwdHeader: {
        out->appendInt(m_flags);
        if(m_flags & 1<<0) {
            out->appendInt(m_fromId);
        }
        out->appendInt(m_date);
        if(m_flags & 1<<1) {
            out->appendInt(m_channelId);
        }
        if(m_flags & 1<<2) {
            out->appendInt(m_channelPost);
        }
        return true;
    }
        break;

    default:
        return false;
    }
}

QMap<QString, QVariant> MessageFwdHeader::toMap() const {
    QMap<QString, QVariant> result;
    switch(static_cast<int>(m_classType)) {
    case typeMessageFwdHeader: {
        result["classType"] = "MessageFwdHeader::typeMessageFwdHeader";
        if( fromId() ) result["fromId"] = QString::number(fromId());
        if( date() ) result["date"] = QString::number(date());
        if( channelId() ) result["channelId"] = QString::number(channelId());
        if( channelPost() ) result["channelPost"] = QString::number(channelPost());
        return result;
    }
        break;

    default:
        return result;
    }
}

MessageFwdHeader MessageFwdHeader::fromMap(const QMap<QString, QVariant> &map) {
    MessageFwdHeader result;
    if(map.value("classType").toString() == "MessageFwdHeader::typeMessageFwdHeader") {
        result.setClassType(typeMessageFwdHeader);
        QVariant _fromId_var = map.value("fromId");
        if( !_fromId_var.isNull() ) {
            _fromId_var.convert( QVariant::nameToType("qint32") );
            result.setFromId( _fromId_var.value<qint32>() );
        }

        QVariant _date_var = map.value("date");
        if( !_date_var.isNull() ) {
            _date_var.convert( QVariant::nameToType("qint32") );
            result.setDate( _date_var.value<qint32>() );
        }

        QVariant _channelId_var = map.value("channelId");
        if( !_channelId_var.isNull() ) {
            _channelId_var.convert( QVariant::nameToType("qint32") );
            result.setChannelId( _channelId_var.value<qint32>() );
        }

        QVariant _channelPost_var = map.value("channelPost");
        if( !_channelPost_var.isNull() ) {
            _channelPost_var.convert( QVariant::nameToType("qint32") );
            result.setChannelPost( _channelPost_var.value<qint32>() );
        }

        return result;
    }
    return result;
}

MessageFwdHeader MessageFwdHeader::fromJson(const QString &json) {
    return MessageFwdHeader::fromMap(QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap());
}

QByteArray MessageFwdHeader::getHash(QCryptographicHash::Algorithm alg) const {
    QByteArray data;
    QDataStream str(&data, QIODevice::WriteOnly);
    str << *this;
    return QCryptographicHash::hash(data, alg);
}

QDataStream &operator<<(QDataStream &stream, const MessageFwdHeader &item) {
    stream << static_cast<uint>(item.classType());
    switch(item.classType()) {
    case MessageFwdHeader::typeMessageFwdHeader:
        stream << item.flags();
        stream << item.fromId();
        stream << item.date();
        stream << item.channelId();
        stream << item.channelPost();
        break;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MessageFwdHeader &item) {
    uint type = 0;
    stream >> type;
    item.setClassType(static_cast<MessageFwdHeader::MessageFwdHeaderClassType>(type));
    switch(type) {
    case MessageFwdHeader::typeMessageFwdHeader: {
        qint32 m_flags;
        stream >> m_flags;
        item.setFlags(m_flags);
        qint32 m_from_id;
        stream >> m_from_id;
        item.setFromId(m_from_id);
        qint32 m_date;
        stream >> m_date;
        item.setDate(m_date);
        qint32 m_channel_id;
        stream >> m_channel_id;
        item.setChannelId(m_channel_id);
        qint32 m_channel_post;
        stream >> m_channel_post;
        item.setChannelPost(m_channel_post);
    }
        break;
    }
    return stream;
}

/*QDebug operator<<(QDebug debug,  const MessageFwdHeader &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)
    debug.nospace() << "Telegram.MessageFwdHeader(";
    switch(item.classType()) {
    case MessageFwdHeader::typeMessageFwdHeader:
        debug.nospace() << "classType: typeMessageFwdHeader";
        debug.nospace() << ", flags: " << item.flags();
        debug.nospace() << ", fromId: " << item.fromId();
        debug.nospace() << ", date: " << item.date();
        debug.nospace() << ", channelId: " << item.channelId();
        debug.nospace() << ", channelPost: " << item.channelPost();
        break;
    }
    debug.nospace() << ")";
    return debug;
}
*/