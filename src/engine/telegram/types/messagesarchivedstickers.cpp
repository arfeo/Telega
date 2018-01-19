// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "messagesarchivedstickers.h"
#include "core/inboundpkt.h"
#include "core/outboundpkt.h"
#include "../coretypes.h"

#include <QDataStream>

MessagesArchivedStickers::MessagesArchivedStickers(MessagesArchivedStickersClassType classType, InboundPkt *in) :
    m_count(0),
    m_classType(classType)
{
    if(in) fetch(in);
}

MessagesArchivedStickers::MessagesArchivedStickers(InboundPkt *in) :
    m_count(0),
    m_classType(typeMessagesArchivedStickers)
{
    fetch(in);
}

MessagesArchivedStickers::MessagesArchivedStickers(const MessagesArchivedStickers &another) :
    TelegramTypeObject(),
    m_count(0),
    m_classType(typeMessagesArchivedStickers)
{
    operator=(another);
}

MessagesArchivedStickers::MessagesArchivedStickers(const Null &null) :
    TelegramTypeObject(null),
    m_count(0),
    m_classType(typeMessagesArchivedStickers)
{
}

MessagesArchivedStickers::~MessagesArchivedStickers() {

}

void MessagesArchivedStickers::setCount(qint32 count) {
    m_count = count;
}

qint32 MessagesArchivedStickers::count() const {
    return m_count;
}

void MessagesArchivedStickers::setSets(const QList<StickerSetCovered> &sets) {
    m_sets = sets;
}

QList<StickerSetCovered> MessagesArchivedStickers::sets() const {
    return m_sets;
}

bool MessagesArchivedStickers::operator ==(const MessagesArchivedStickers &b) const {
    return m_classType == b.m_classType &&
           m_count == b.m_count &&
           m_sets == b.m_sets;
}

MessagesArchivedStickers &MessagesArchivedStickers::operator =(const MessagesArchivedStickers &b) {
    m_classType = b.m_classType;
    m_count = b.m_count;
    m_sets = b.m_sets;
    setNull(b.isNull());
    return *this;
}

void MessagesArchivedStickers::setClassType(MessagesArchivedStickers::MessagesArchivedStickersClassType classType) {
    m_classType = classType;
}

MessagesArchivedStickers::MessagesArchivedStickersClassType MessagesArchivedStickers::classType() const {
    return m_classType;
}

bool MessagesArchivedStickers::fetch(InboundPkt *in) {
    LQTG_FETCH_LOG;
    int x = in->fetchInt();
    switch(x) {
    case typeMessagesArchivedStickers: {
        m_count = in->fetchInt();
        if(in->fetchInt() != (qint32)CoreTypes::typeVector) return false;
        qint32 m_sets_length = in->fetchInt();
        m_sets.clear();
        for (qint32 i = 0; i < m_sets_length; i++) {
            StickerSetCovered type;
            type.fetch(in);
            m_sets.append(type);
        }
        m_classType = static_cast<MessagesArchivedStickersClassType>(x);
        return true;
    }
        break;

    default:
        LQTG_FETCH_ASSERT;
        return false;
    }
}

bool MessagesArchivedStickers::push(OutboundPkt *out) const {
    out->appendInt(m_classType);
    switch(m_classType) {
    case typeMessagesArchivedStickers: {
        out->appendInt(m_count);
        out->appendInt(CoreTypes::typeVector);
        out->appendInt(m_sets.count());
        for (qint32 i = 0; i < m_sets.count(); i++) {
            m_sets[i].push(out);
        }
        return true;
    }
        break;

    default:
        return false;
    }
}

QMap<QString, QVariant> MessagesArchivedStickers::toMap() const {
    QMap<QString, QVariant> result;
    switch(static_cast<int>(m_classType)) {
    case typeMessagesArchivedStickers: {
        result["classType"] = "MessagesArchivedStickers::typeMessagesArchivedStickers";
        if( count() ) result["count"] = QString::number(count());
        QList<QVariant> _sets;
        //for(const StickerSetCovered &m__type: m_sets)
        for(int i = 0; i < m_sets.size(); ++i) {
            const StickerSetCovered &m__type = m_sets[i];
            if( !m__type.isNull() ) _sets << m__type.toMap();
        }
        result["sets"] = _sets;
        return result;
    }
        break;

    default:
        return result;
    }
}

MessagesArchivedStickers MessagesArchivedStickers::fromMap(const QMap<QString, QVariant> &map) {
    MessagesArchivedStickers result;
    if(map.value("classType").toString() == "MessagesArchivedStickers::typeMessagesArchivedStickers") {
        result.setClassType(typeMessagesArchivedStickers);
        QVariant _count_var = map.value("count");
        if( !_count_var.isNull() ) {
            _count_var.convert( QVariant::nameToType("qint32") );
            result.setCount( _count_var.value<qint32>() );
        }

        QList<QVariant> map_sets = map["sets"].toList();
        QList<StickerSetCovered> _sets;
        //for(const QVariant &var: map_sets)
        for(int i = 0; i < map_sets.size(); ++i) {
            const QVariant &var = map_sets[i];
            _sets << StickerSetCovered::fromMap(var.toMap());
        }
        result.setSets(_sets);
        return result;
    }
    return result;
}

MessagesArchivedStickers MessagesArchivedStickers::fromJson(const QString &json) {
    return MessagesArchivedStickers::fromMap(QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap());
}

QByteArray MessagesArchivedStickers::getHash(QCryptographicHash::Algorithm alg) const {
    QByteArray data;
    QDataStream str(&data, QIODevice::WriteOnly);
    str << *this;
    return QCryptographicHash::hash(data, alg);
}

QDataStream &operator<<(QDataStream &stream, const MessagesArchivedStickers &item) {
    stream << static_cast<uint>(item.classType());
    switch(item.classType()) {
    case MessagesArchivedStickers::typeMessagesArchivedStickers:
        stream << item.count();
        stream << item.sets();
        break;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, MessagesArchivedStickers &item) {
    uint type = 0;
    stream >> type;
    item.setClassType(static_cast<MessagesArchivedStickers::MessagesArchivedStickersClassType>(type));
    switch(type) {
    case MessagesArchivedStickers::typeMessagesArchivedStickers: {
        qint32 m_count;
        stream >> m_count;
        item.setCount(m_count);
        QList<StickerSetCovered> m_sets;
        stream >> m_sets;
        item.setSets(m_sets);
    }
        break;
    }
    return stream;
}

/*QDebug operator<<(QDebug debug,  const MessagesArchivedStickers &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)
    debug.nospace() << "Telegram.MessagesArchivedStickers(";
    switch(item.classType()) {
    case MessagesArchivedStickers::typeMessagesArchivedStickers:
        debug.nospace() << "classType: typeMessagesArchivedStickers";
        debug.nospace() << ", count: " << item.count();
        debug.nospace() << ", sets: " << item.sets();
        break;
    }
    debug.nospace() << ")";
    return debug;
}
*/