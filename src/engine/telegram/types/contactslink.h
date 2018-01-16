// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_CONTACTSLINK
#define LQTG_TYPE_CONTACTSLINK

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include "contactlink.h"
#include "user.h"

class LIBQTELEGRAMSHARED_EXPORT ContactsLink : public TelegramTypeObject
{
public:
    enum ContactsLinkClassType {
        typeContactsLink = 0x3ace484c
    };

    ContactsLink(ContactsLinkClassType classType = typeContactsLink, InboundPkt *in = 0);
    ContactsLink(InboundPkt *in);
    ContactsLink(const Null&);
    ContactsLink(const ContactsLink &another);
    virtual ~ContactsLink();

    void setForeignLink(const ContactLink &foreignLink);
    ContactLink foreignLink() const;

    void setMyLink(const ContactLink &myLink);
    ContactLink myLink() const;

    void setUser(const User &user);
    User user() const;

    void setClassType(ContactsLinkClassType classType);
    ContactsLinkClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static ContactsLink fromMap(const QMap<QString, QVariant> &map);
    static ContactsLink fromJson(const QString &json);

    bool operator ==(const ContactsLink &b) const;
    ContactsLink &operator =(const ContactsLink &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    ContactLink m_foreignLink;
    ContactLink m_myLink;
    User m_user;
    ContactsLinkClassType m_classType;
};

Q_DECLARE_METATYPE(ContactsLink)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const ContactsLink &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, ContactsLink &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const ContactsLink &item);

#endif // LQTG_TYPE_CONTACTSLINK