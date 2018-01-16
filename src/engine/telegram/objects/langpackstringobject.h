// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_LANGPACKSTRING_OBJECT
#define LQTG_TYPE_LANGPACKSTRING_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/langpackstring.h"

#include <QPointer>

class LIBQTELEGRAMSHARED_EXPORT LangPackStringObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(LangPackStringClassType)
    Q_PROPERTY(QString fewValue READ fewValue WRITE setFewValue NOTIFY fewValueChanged)
    Q_PROPERTY(qint32 flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QString manyValue READ manyValue WRITE setManyValue NOTIFY manyValueChanged)
    Q_PROPERTY(QString oneValue READ oneValue WRITE setOneValue NOTIFY oneValueChanged)
    Q_PROPERTY(QString otherValue READ otherValue WRITE setOtherValue NOTIFY otherValueChanged)
    Q_PROPERTY(QString twoValue READ twoValue WRITE setTwoValue NOTIFY twoValueChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString zeroValue READ zeroValue WRITE setZeroValue NOTIFY zeroValueChanged)
    Q_PROPERTY(LangPackString core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum LangPackStringClassType {
        TypeLangPackString,
        TypeLangPackStringPluralized,
        TypeLangPackStringDeleted
    };

    LangPackStringObject(const LangPackString &core, QObject *parent = 0);
    LangPackStringObject(QObject *parent = 0);
    virtual ~LangPackStringObject();

    void setFewValue(const QString &fewValue);
    QString fewValue() const;

    void setFlags(qint32 flags);
    qint32 flags() const;

    void setKey(const QString &key);
    QString key() const;

    void setManyValue(const QString &manyValue);
    QString manyValue() const;

    void setOneValue(const QString &oneValue);
    QString oneValue() const;

    void setOtherValue(const QString &otherValue);
    QString otherValue() const;

    void setTwoValue(const QString &twoValue);
    QString twoValue() const;

    void setValue(const QString &value);
    QString value() const;

    void setZeroValue(const QString &zeroValue);
    QString zeroValue() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const LangPackString &core);
    LangPackString core() const;

    LangPackStringObject &operator =(const LangPackString &b);
    bool operator ==(const LangPackString &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void fewValueChanged();
    void flagsChanged();
    void keyChanged();
    void manyValueChanged();
    void oneValueChanged();
    void otherValueChanged();
    void twoValueChanged();
    void valueChanged();
    void zeroValueChanged();

private Q_SLOTS:

private:
    LangPackString m_core;
};

#endif // LQTG_TYPE_LANGPACKSTRING_OBJECT