// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "channeladminrights.h"
#include "core/inboundpkt.h"
#include "core/outboundpkt.h"
#include "../coretypes.h"

#include <QDataStream>

ChannelAdminRights::ChannelAdminRights(ChannelAdminRightsClassType classType, InboundPkt *in) :
    m_flags(0),
    m_classType(classType)
{
    if(in) fetch(in);
}

ChannelAdminRights::ChannelAdminRights(InboundPkt *in) :
    m_flags(0),
    m_classType(typeChannelAdminRights)
{
    fetch(in);
}

ChannelAdminRights::ChannelAdminRights(const ChannelAdminRights &another) :
    TelegramTypeObject(),
    m_flags(0),
    m_classType(typeChannelAdminRights)
{
    operator=(another);
}

ChannelAdminRights::ChannelAdminRights(const Null &null) :
    TelegramTypeObject(null),
    m_flags(0),
    m_classType(typeChannelAdminRights)
{
}

ChannelAdminRights::~ChannelAdminRights() {

}

void ChannelAdminRights::setAddAdmins(bool addAdmins) {
    if(addAdmins) m_flags = (m_flags | (1<<9));
    else m_flags = (m_flags & ~(1<<9));
}

bool ChannelAdminRights::addAdmins() const {
    return (m_flags & 1<<9);
}

void ChannelAdminRights::setBanUsers(bool banUsers) {
    if(banUsers) m_flags = (m_flags | (1<<4));
    else m_flags = (m_flags & ~(1<<4));
}

bool ChannelAdminRights::banUsers() const {
    return (m_flags & 1<<4);
}

void ChannelAdminRights::setChangeInfo(bool changeInfo) {
    if(changeInfo) m_flags = (m_flags | (1<<0));
    else m_flags = (m_flags & ~(1<<0));
}

bool ChannelAdminRights::changeInfo() const {
    return (m_flags & 1<<0);
}

void ChannelAdminRights::setDeleteMessages(bool deleteMessages) {
    if(deleteMessages) m_flags = (m_flags | (1<<3));
    else m_flags = (m_flags & ~(1<<3));
}

bool ChannelAdminRights::deleteMessages() const {
    return (m_flags & 1<<3);
}

void ChannelAdminRights::setEditMessages(bool editMessages) {
    if(editMessages) m_flags = (m_flags | (1<<2));
    else m_flags = (m_flags & ~(1<<2));
}

bool ChannelAdminRights::editMessages() const {
    return (m_flags & 1<<2);
}

void ChannelAdminRights::setFlags(qint32 flags) {
    m_flags = flags;
}

qint32 ChannelAdminRights::flags() const {
    return m_flags;
}

void ChannelAdminRights::setInviteLink(bool inviteLink) {
    if(inviteLink) m_flags = (m_flags | (1<<6));
    else m_flags = (m_flags & ~(1<<6));
}

bool ChannelAdminRights::inviteLink() const {
    return (m_flags & 1<<6);
}

void ChannelAdminRights::setInviteUsers(bool inviteUsers) {
    if(inviteUsers) m_flags = (m_flags | (1<<5));
    else m_flags = (m_flags & ~(1<<5));
}

bool ChannelAdminRights::inviteUsers() const {
    return (m_flags & 1<<5);
}

void ChannelAdminRights::setPinMessages(bool pinMessages) {
    if(pinMessages) m_flags = (m_flags | (1<<7));
    else m_flags = (m_flags & ~(1<<7));
}

bool ChannelAdminRights::pinMessages() const {
    return (m_flags & 1<<7);
}

void ChannelAdminRights::setPostMessages(bool postMessages) {
    if(postMessages) m_flags = (m_flags | (1<<1));
    else m_flags = (m_flags & ~(1<<1));
}

bool ChannelAdminRights::postMessages() const {
    return (m_flags & 1<<1);
}

bool ChannelAdminRights::operator ==(const ChannelAdminRights &b) const {
    return m_classType == b.m_classType &&
           m_flags == b.m_flags;
}

ChannelAdminRights &ChannelAdminRights::operator =(const ChannelAdminRights &b) {
    m_classType = b.m_classType;
    m_flags = b.m_flags;
    setNull(b.isNull());
    return *this;
}

void ChannelAdminRights::setClassType(ChannelAdminRights::ChannelAdminRightsClassType classType) {
    m_classType = classType;
}

ChannelAdminRights::ChannelAdminRightsClassType ChannelAdminRights::classType() const {
    return m_classType;
}

bool ChannelAdminRights::fetch(InboundPkt *in) {
    LQTG_FETCH_LOG;
    int x = in->fetchInt();
    switch(x) {
    case typeChannelAdminRights: {
        m_flags = in->fetchInt();
        m_classType = static_cast<ChannelAdminRightsClassType>(x);
        return true;
    }
        break;

    default:
        LQTG_FETCH_ASSERT;
        return false;
    }
}

bool ChannelAdminRights::push(OutboundPkt *out) const {
    out->appendInt(m_classType);
    switch(m_classType) {
    case typeChannelAdminRights: {
        out->appendInt(m_flags);
        return true;
    }
        break;

    default:
        return false;
    }
}

QMap<QString, QVariant> ChannelAdminRights::toMap() const {
    QMap<QString, QVariant> result;
    switch(static_cast<int>(m_classType)) {
    case typeChannelAdminRights: {
        result["classType"] = "ChannelAdminRights::typeChannelAdminRights";
        if( changeInfo() ) result["changeInfo"] = QString::number(changeInfo());
        if( postMessages() ) result["postMessages"] = QString::number(postMessages());
        if( editMessages() ) result["editMessages"] = QString::number(editMessages());
        if( deleteMessages() ) result["deleteMessages"] = QString::number(deleteMessages());
        if( banUsers() ) result["banUsers"] = QString::number(banUsers());
        if( inviteUsers() ) result["inviteUsers"] = QString::number(inviteUsers());
        if( inviteLink() ) result["inviteLink"] = QString::number(inviteLink());
        if( pinMessages() ) result["pinMessages"] = QString::number(pinMessages());
        if( addAdmins() ) result["addAdmins"] = QString::number(addAdmins());
        return result;
    }
        break;

    default:
        return result;
    }
}

ChannelAdminRights ChannelAdminRights::fromMap(const QMap<QString, QVariant> &map) {
    ChannelAdminRights result;
    if(map.value("classType").toString() == "ChannelAdminRights::typeChannelAdminRights") {
        result.setClassType(typeChannelAdminRights);
        QVariant _changeInfo_var = map.value("changeInfo");
        if( !_changeInfo_var.isNull() ) {
            _changeInfo_var.convert( QVariant::nameToType("bool") );
            result.setChangeInfo( _changeInfo_var.value<bool>() );
        }

        QVariant _postMessages_var = map.value("postMessages");
        if( !_postMessages_var.isNull() ) {
            _postMessages_var.convert( QVariant::nameToType("bool") );
            result.setPostMessages( _postMessages_var.value<bool>() );
        }

        QVariant _editMessages_var = map.value("editMessages");
        if( !_editMessages_var.isNull() ) {
            _editMessages_var.convert( QVariant::nameToType("bool") );
            result.setEditMessages( _editMessages_var.value<bool>() );
        }

        QVariant _deleteMessages_var = map.value("deleteMessages");
        if( !_deleteMessages_var.isNull() ) {
            _deleteMessages_var.convert( QVariant::nameToType("bool") );
            result.setDeleteMessages( _deleteMessages_var.value<bool>() );
        }

        QVariant _banUsers_var = map.value("banUsers");
        if( !_banUsers_var.isNull() ) {
            _banUsers_var.convert( QVariant::nameToType("bool") );
            result.setBanUsers( _banUsers_var.value<bool>() );
        }

        QVariant _inviteUsers_var = map.value("inviteUsers");
        if( !_inviteUsers_var.isNull() ) {
            _inviteUsers_var.convert( QVariant::nameToType("bool") );
            result.setInviteUsers( _inviteUsers_var.value<bool>() );
        }

        QVariant _inviteLink_var = map.value("inviteLink");
        if( !_inviteLink_var.isNull() ) {
            _inviteLink_var.convert( QVariant::nameToType("bool") );
            result.setInviteLink( _inviteLink_var.value<bool>() );
        }

        QVariant _pinMessages_var = map.value("pinMessages");
        if( !_pinMessages_var.isNull() ) {
            _pinMessages_var.convert( QVariant::nameToType("bool") );
            result.setPinMessages( _pinMessages_var.value<bool>() );
        }

        QVariant _addAdmins_var = map.value("addAdmins");
        if( !_addAdmins_var.isNull() ) {
            _addAdmins_var.convert( QVariant::nameToType("bool") );
            result.setAddAdmins( _addAdmins_var.value<bool>() );
        }

        return result;
    }
    return result;
}

ChannelAdminRights ChannelAdminRights::fromJson(const QString &json) {
    return ChannelAdminRights::fromMap(QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap());
}

QByteArray ChannelAdminRights::getHash(QCryptographicHash::Algorithm alg) const {
    QByteArray data;
    QDataStream str(&data, QIODevice::WriteOnly);
    str << *this;
    return QCryptographicHash::hash(data, alg);
}

QDataStream &operator<<(QDataStream &stream, const ChannelAdminRights &item) {
    stream << static_cast<uint>(item.classType());
    switch(item.classType()) {
    case ChannelAdminRights::typeChannelAdminRights:
        stream << item.flags();
        break;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, ChannelAdminRights &item) {
    uint type = 0;
    stream >> type;
    item.setClassType(static_cast<ChannelAdminRights::ChannelAdminRightsClassType>(type));
    switch(type) {
    case ChannelAdminRights::typeChannelAdminRights: {
        qint32 m_flags;
        stream >> m_flags;
        item.setFlags(m_flags);
    }
        break;
    }
    return stream;
}

/*QDebug operator<<(QDebug debug,  const ChannelAdminRights &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)
    debug.nospace() << "Telegram.ChannelAdminRights(";
    switch(item.classType()) {
    case ChannelAdminRights::typeChannelAdminRights:
        debug.nospace() << "classType: typeChannelAdminRights";
        debug.nospace() << ", flags: " << item.flags();
        break;
    }
    debug.nospace() << ")";
    return debug;
}
*/