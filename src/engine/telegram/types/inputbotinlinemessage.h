// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_INPUTBOTINLINEMESSAGE
#define LQTG_TYPE_INPUTBOTINLINEMESSAGE

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include <QString>
#include <QList>
#include "messageentity.h"
#include <QtGlobal>
#include "inputgeopoint.h"
#include "replymarkup.h"

class LIBQTELEGRAMSHARED_EXPORT InputBotInlineMessage : public TelegramTypeObject
{
public:
    enum InputBotInlineMessageClassType {
        typeInputBotInlineMessageMediaAuto = 0x292fed13,
        typeInputBotInlineMessageText = 0x3dcd7a87,
        typeInputBotInlineMessageMediaGeo = 0xf4a59de1,
        typeInputBotInlineMessageMediaVenue = 0xaaafadc8,
        typeInputBotInlineMessageMediaContact = 0x2daf01a7,
        typeInputBotInlineMessageGame = 0x4b425864
    };

    InputBotInlineMessage(InputBotInlineMessageClassType classType = typeInputBotInlineMessageMediaAuto, InboundPkt *in = 0);
    InputBotInlineMessage(InboundPkt *in);
    InputBotInlineMessage(const Null&);
    InputBotInlineMessage(const InputBotInlineMessage &another);
    virtual ~InputBotInlineMessage();

    void setAddress(const QString &address);
    QString address() const;

    void setCaption(const QString &caption);
    QString caption() const;

    void setEntities(const QList<MessageEntity> &entities);
    QList<MessageEntity> entities() const;

    void setFirstName(const QString &firstName);
    QString firstName() const;

    void setFlags(qint32 flags);
    qint32 flags() const;

    void setGeoPoint(const InputGeoPoint &geoPoint);
    InputGeoPoint geoPoint() const;

    void setLastName(const QString &lastName);
    QString lastName() const;

    void setMessage(const QString &message);
    QString message() const;

    void setNoWebpage(bool noWebpage);
    bool noWebpage() const;

    void setPhoneNumber(const QString &phoneNumber);
    QString phoneNumber() const;

    void setProvider(const QString &provider);
    QString provider() const;

    void setReplyMarkup(const ReplyMarkup &replyMarkup);
    ReplyMarkup replyMarkup() const;

    void setTitle(const QString &title);
    QString title() const;

    void setVenueId(const QString &venueId);
    QString venueId() const;

    void setClassType(InputBotInlineMessageClassType classType);
    InputBotInlineMessageClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static InputBotInlineMessage fromMap(const QMap<QString, QVariant> &map);
    static InputBotInlineMessage fromJson(const QString &json);

    bool operator ==(const InputBotInlineMessage &b) const;
    InputBotInlineMessage &operator =(const InputBotInlineMessage &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    QString m_address;
    QString m_caption;
    QList<MessageEntity> m_entities;
    QString m_firstName;
    qint32 m_flags;
    InputGeoPoint m_geoPoint;
    QString m_lastName;
    QString m_message;
    QString m_phoneNumber;
    QString m_provider;
    ReplyMarkup m_replyMarkup;
    QString m_title;
    QString m_venueId;
    InputBotInlineMessageClassType m_classType;
};

Q_DECLARE_METATYPE(InputBotInlineMessage)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const InputBotInlineMessage &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, InputBotInlineMessage &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const InputBotInlineMessage &item);

#endif // LQTG_TYPE_INPUTBOTINLINEMESSAGE