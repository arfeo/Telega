// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_ENCRYPTEDMESSAGE
#define LQTG_TYPE_ENCRYPTEDMESSAGE

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include <QByteArray>
#include <QtGlobal>
#include "encryptedfile.h"

class LIBQTELEGRAMSHARED_EXPORT EncryptedMessage : public TelegramTypeObject
{
public:
    enum EncryptedMessageClassType {
        typeEncryptedMessage = 0xed18c118,
        typeEncryptedMessageService = 0x23734b06
    };

    EncryptedMessage(EncryptedMessageClassType classType = typeEncryptedMessage, InboundPkt *in = 0);
    EncryptedMessage(InboundPkt *in);
    EncryptedMessage(const Null&);
    EncryptedMessage(const EncryptedMessage &another);
    virtual ~EncryptedMessage();

    void setBytes(const QByteArray &bytes);
    QByteArray bytes() const;

    void setChatId(qint32 chatId);
    qint32 chatId() const;

    void setDate(qint32 date);
    qint32 date() const;

    void setFile(const EncryptedFile &file);
    EncryptedFile file() const;

    void setRandomId(qint64 randomId);
    qint64 randomId() const;

    void setClassType(EncryptedMessageClassType classType);
    EncryptedMessageClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static EncryptedMessage fromMap(const QMap<QString, QVariant> &map);
    static EncryptedMessage fromJson(const QString &json);

    bool operator ==(const EncryptedMessage &b) const;
    EncryptedMessage &operator =(const EncryptedMessage &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    QByteArray m_bytes;
    qint32 m_chatId;
    qint32 m_date;
    EncryptedFile m_file;
    qint64 m_randomId;
    EncryptedMessageClassType m_classType;
};

Q_DECLARE_METATYPE(EncryptedMessage)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const EncryptedMessage &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, EncryptedMessage &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const EncryptedMessage &item);

#endif // LQTG_TYPE_ENCRYPTEDMESSAGE