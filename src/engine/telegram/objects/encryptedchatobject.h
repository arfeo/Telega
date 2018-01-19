// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_ENCRYPTEDCHAT_OBJECT
#define LQTG_TYPE_ENCRYPTEDCHAT_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/encryptedchat.h"

#include <QPointer>

class LIBQTELEGRAMSHARED_EXPORT EncryptedChatObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(EncryptedChatClassType)
    Q_PROPERTY(qint64 accessHash READ accessHash WRITE setAccessHash NOTIFY accessHashChanged)
    Q_PROPERTY(qint32 adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged)
    Q_PROPERTY(qint32 date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QByteArray gA READ gA WRITE setGA NOTIFY gAChanged)
    Q_PROPERTY(QByteArray gAOrB READ gAOrB WRITE setGAOrB NOTIFY gAOrBChanged)
    Q_PROPERTY(qint32 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(qint64 keyFingerprint READ keyFingerprint WRITE setKeyFingerprint NOTIFY keyFingerprintChanged)
    Q_PROPERTY(qint32 participantId READ participantId WRITE setParticipantId NOTIFY participantIdChanged)
    Q_PROPERTY(EncryptedChat core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum EncryptedChatClassType {
        TypeEncryptedChatEmpty,
        TypeEncryptedChatWaiting,
        TypeEncryptedChatRequested,
        TypeEncryptedChat,
        TypeEncryptedChatDiscarded
    };

    EncryptedChatObject(const EncryptedChat &core, QObject *parent = 0);
    EncryptedChatObject(QObject *parent = 0);
    virtual ~EncryptedChatObject();

    void setAccessHash(qint64 accessHash);
    qint64 accessHash() const;

    void setAdminId(qint32 adminId);
    qint32 adminId() const;

    void setDate(qint32 date);
    qint32 date() const;

    void setGA(const QByteArray &gA);
    QByteArray gA() const;

    void setGAOrB(const QByteArray &gAOrB);
    QByteArray gAOrB() const;

    void setId(qint32 id);
    qint32 id() const;

    void setKeyFingerprint(qint64 keyFingerprint);
    qint64 keyFingerprint() const;

    void setParticipantId(qint32 participantId);
    qint32 participantId() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const EncryptedChat &core);
    EncryptedChat core() const;

    EncryptedChatObject &operator =(const EncryptedChat &b);
    bool operator ==(const EncryptedChat &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void accessHashChanged();
    void adminIdChanged();
    void dateChanged();
    void gAChanged();
    void gAOrBChanged();
    void idChanged();
    void keyFingerprintChanged();
    void participantIdChanged();

private Q_SLOTS:

private:
    EncryptedChat m_core;
};

#endif // LQTG_TYPE_ENCRYPTEDCHAT_OBJECT