// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_MESSAGESSTICKERSETINSTALLRESULT
#define LQTG_TYPE_MESSAGESSTICKERSETINSTALLRESULT

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include <QList>
#include "stickersetcovered.h"

class LIBQTELEGRAMSHARED_EXPORT MessagesStickerSetInstallResult : public TelegramTypeObject
{
public:
    enum MessagesStickerSetInstallResultClassType {
        typeMessagesStickerSetInstallResultSuccess = 0x38641628,
        typeMessagesStickerSetInstallResultArchive = 0x35e410a8
    };

    MessagesStickerSetInstallResult(MessagesStickerSetInstallResultClassType classType = typeMessagesStickerSetInstallResultSuccess, InboundPkt *in = 0);
    MessagesStickerSetInstallResult(InboundPkt *in);
    MessagesStickerSetInstallResult(const Null&);
    MessagesStickerSetInstallResult(const MessagesStickerSetInstallResult &another);
    virtual ~MessagesStickerSetInstallResult();

    void setSets(const QList<StickerSetCovered> &sets);
    QList<StickerSetCovered> sets() const;

    void setClassType(MessagesStickerSetInstallResultClassType classType);
    MessagesStickerSetInstallResultClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static MessagesStickerSetInstallResult fromMap(const QMap<QString, QVariant> &map);
    static MessagesStickerSetInstallResult fromJson(const QString &json);

    bool operator ==(const MessagesStickerSetInstallResult &b) const;
    MessagesStickerSetInstallResult &operator =(const MessagesStickerSetInstallResult &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    QList<StickerSetCovered> m_sets;
    MessagesStickerSetInstallResultClassType m_classType;
};

Q_DECLARE_METATYPE(MessagesStickerSetInstallResult)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const MessagesStickerSetInstallResult &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, MessagesStickerSetInstallResult &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const MessagesStickerSetInstallResult &item);

#endif // LQTG_TYPE_MESSAGESSTICKERSETINSTALLRESULT