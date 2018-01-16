// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_DRAFTMESSAGE
#define LQTG_TYPE_DRAFTMESSAGE

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include <QtGlobal>
#include <QList>
#include "messageentity.h"
#include <QString>

class LIBQTELEGRAMSHARED_EXPORT DraftMessage : public TelegramTypeObject
{
public:
    enum DraftMessageClassType {
        typeDraftMessageEmpty = 0xba4baec5,
        typeDraftMessage = 0xfd8e711f
    };

    DraftMessage(DraftMessageClassType classType = typeDraftMessageEmpty, InboundPkt *in = 0);
    DraftMessage(InboundPkt *in);
    DraftMessage(const Null&);
    DraftMessage(const DraftMessage &another);
    virtual ~DraftMessage();

    void setDate(qint32 date);
    qint32 date() const;

    void setEntities(const QList<MessageEntity> &entities);
    QList<MessageEntity> entities() const;

    void setFlags(qint32 flags);
    qint32 flags() const;

    void setMessage(const QString &message);
    QString message() const;

    void setNoWebpage(bool noWebpage);
    bool noWebpage() const;

    void setReplyToMsgId(qint32 replyToMsgId);
    qint32 replyToMsgId() const;

    void setClassType(DraftMessageClassType classType);
    DraftMessageClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static DraftMessage fromMap(const QMap<QString, QVariant> &map);
    static DraftMessage fromJson(const QString &json);

    bool operator ==(const DraftMessage &b) const;
    DraftMessage &operator =(const DraftMessage &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    qint32 m_date;
    QList<MessageEntity> m_entities;
    qint32 m_flags;
    QString m_message;
    qint32 m_replyToMsgId;
    DraftMessageClassType m_classType;
};

Q_DECLARE_METATYPE(DraftMessage)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const DraftMessage &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, DraftMessage &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const DraftMessage &item);

#endif // LQTG_TYPE_DRAFTMESSAGE