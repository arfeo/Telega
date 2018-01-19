// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_LANGPACKLANGUAGE
#define LQTG_TYPE_LANGPACKLANGUAGE

#include "telegramtypeobject.h"

#include <QMetaType>
#include <QVariant>
#include "../coretypes.h"

#include <QDataStream>

#include <QString>

class LIBQTELEGRAMSHARED_EXPORT LangPackLanguage : public TelegramTypeObject
{
public:
    enum LangPackLanguageClassType {
        typeLangPackLanguage = 0x117698f1
    };

    LangPackLanguage(LangPackLanguageClassType classType = typeLangPackLanguage, InboundPkt *in = 0);
    LangPackLanguage(InboundPkt *in);
    LangPackLanguage(const Null&);
    LangPackLanguage(const LangPackLanguage &another);
    virtual ~LangPackLanguage();

    void setLangCode(const QString &langCode);
    QString langCode() const;

    void setName(const QString &name);
    QString name() const;

    void setNativeName(const QString &nativeName);
    QString nativeName() const;

    void setClassType(LangPackLanguageClassType classType);
    LangPackLanguageClassType classType() const;

    bool fetch(InboundPkt *in);
    bool push(OutboundPkt *out) const;

    QMap<QString, QVariant> toMap() const;
    static LangPackLanguage fromMap(const QMap<QString, QVariant> &map);
    static LangPackLanguage fromJson(const QString &json);

    bool operator ==(const LangPackLanguage &b) const;
    LangPackLanguage &operator =(const LangPackLanguage &b);

    bool operator==(bool stt) const { return isNull() != stt; }
    bool operator!=(bool stt) const { return !operator ==(stt); }

    QByteArray getHash(QCryptographicHash::Algorithm alg = QCryptographicHash::Md5) const;

private:
    QString m_langCode;
    QString m_name;
    QString m_nativeName;
    LangPackLanguageClassType m_classType;
};

Q_DECLARE_METATYPE(LangPackLanguage)

QDataStream LIBQTELEGRAMSHARED_EXPORT &operator<<(QDataStream &stream, const LangPackLanguage &item);
QDataStream LIBQTELEGRAMSHARED_EXPORT &operator>>(QDataStream &stream, LangPackLanguage &item);

QDebug LIBQTELEGRAMSHARED_EXPORT operator<<(QDebug debug,  const LangPackLanguage &item);

#endif // LQTG_TYPE_LANGPACKLANGUAGE