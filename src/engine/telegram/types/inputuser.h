// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_INPUTUSER
#define LQTG_TYPE_INPUTUSER

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include <QtGlobal>

class LIBQTELEGRAMSHARED_EXPORT InputUser : public TelegramTypeObject
{
public:
    enum InputUserClassType {
        typeInputUserEmpty = 0xb98886cf,
        typeInputUserSelf = 0xf7c1b13f,
        typeInputUser = 0xd8292816
    };

    InputUser(InputUserClassType classType = typeInputUserEmpty, InboundPkt *in = 0);
    InputUser(InboundPkt *in);
    InputUser(const Null&);
    InputUser(const InputUser &another);
    virtual ~InputUser();

    void setAccessHash(qint64 accessHash);
    qint64 accessHash() const;

    void setUserId(qint32 userId);
    qint32 userId() const;

    void setClassType(InputUserClassType classType);
    InputUserClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static InputUser fromMap(const QMap<QString, QVariant> &map);
    static InputUser fromJson(const QString &json);

    bool operator ==(const InputUser &b) const;
    InputUser &operator =(const InputUser &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    qint64 m_accessHash;
    qint32 m_userId;
    InputUserClassType m_classType;
};

Q_DECLARE_METATYPE(InputUser)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const InputUser &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, InputUser &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const InputUser &item);

#endif // LQTG_TYPE_INPUTUSER