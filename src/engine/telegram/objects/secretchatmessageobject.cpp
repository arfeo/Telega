// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "secretchatmessageobject.h"

SecretChatMessageObject::SecretChatMessageObject(const SecretChatMessage &core, QObject *parent) :
    TelegramTypeQObject(parent),
    m_attachment(0),
    m_decryptedMessage(0),
    m_core(core)
{
    m_attachment = new EncryptedFileObject(m_core.attachment(), this);
    connect(m_attachment.data(), SIGNAL(coreChanged()), this, SLOT(coreAttachmentChanged()));
    m_decryptedMessage = new DecryptedMessageObject(m_core.decryptedMessage(), this);
    connect(m_decryptedMessage.data(), SIGNAL(coreChanged()), this, SLOT(coreDecryptedMessageChanged()));
}

SecretChatMessageObject::SecretChatMessageObject(QObject *parent) :
    TelegramTypeQObject(parent),
    m_attachment(0),
    m_decryptedMessage(0),
    m_core()
{
    m_attachment = new EncryptedFileObject(m_core.attachment(), this);
    connect(m_attachment.data(), SIGNAL(coreChanged()), this, SLOT(coreAttachmentChanged()));
    m_decryptedMessage = new DecryptedMessageObject(m_core.decryptedMessage(), this);
    connect(m_decryptedMessage.data(), SIGNAL(coreChanged()), this, SLOT(coreDecryptedMessageChanged()));
}

SecretChatMessageObject::~SecretChatMessageObject() {
}

void SecretChatMessageObject::setAttachment(EncryptedFileObject* attachment) {
    if(m_attachment == attachment) return;
    if(m_attachment) delete m_attachment;
    m_attachment = attachment;
    if(m_attachment) {
        m_attachment->setParent(this);
        m_core.setAttachment(m_attachment->core());
        connect(m_attachment.data(), SIGNAL(coreChanged()), this, SLOT(coreAttachmentChanged()));
    }
    Q_EMIT attachmentChanged();
    Q_EMIT coreChanged();
}

EncryptedFileObject*  SecretChatMessageObject::attachment() const {
    return m_attachment;
}

void SecretChatMessageObject::setChatId(qint32 chatId) {
    if(m_core.chatId() == chatId) return;
    m_core.setChatId(chatId);
    Q_EMIT chatIdChanged();
    Q_EMIT coreChanged();
}

qint32 SecretChatMessageObject::chatId() const {
    return m_core.chatId();
}

void SecretChatMessageObject::setDate(qint32 date) {
    if(m_core.date() == date) return;
    m_core.setDate(date);
    Q_EMIT dateChanged();
    Q_EMIT coreChanged();
}

qint32 SecretChatMessageObject::date() const {
    return m_core.date();
}

void SecretChatMessageObject::setDecryptedMessage(DecryptedMessageObject* decryptedMessage) {
    if(m_decryptedMessage == decryptedMessage) return;
    if(m_decryptedMessage) delete m_decryptedMessage;
    m_decryptedMessage = decryptedMessage;
    if(m_decryptedMessage) {
        m_decryptedMessage->setParent(this);
        m_core.setDecryptedMessage(m_decryptedMessage->core());
        connect(m_decryptedMessage.data(), SIGNAL(coreChanged()), this, SLOT(coreDecryptedMessageChanged()));
    }
    Q_EMIT decryptedMessageChanged();
    Q_EMIT coreChanged();
}

DecryptedMessageObject*  SecretChatMessageObject::decryptedMessage() const {
    return m_decryptedMessage;
}

void SecretChatMessageObject::setTtl(qint32 ttl) {
    if(m_core.ttl() == ttl) return;
    m_core.setTtl(ttl);
    Q_EMIT ttlChanged();
    Q_EMIT coreChanged();
}

qint32 SecretChatMessageObject::ttl() const {
    return m_core.ttl();
}

SecretChatMessageObject &SecretChatMessageObject::operator =(const SecretChatMessage &b) {
    if(m_core == b) return *this;
    m_core = b;
    m_attachment->setCore(b.attachment());
    m_decryptedMessage->setCore(b.decryptedMessage());

    Q_EMIT attachmentChanged();
    Q_EMIT chatIdChanged();
    Q_EMIT dateChanged();
    Q_EMIT decryptedMessageChanged();
    Q_EMIT ttlChanged();
    Q_EMIT coreChanged();
    return *this;
}

bool SecretChatMessageObject::operator ==(const SecretChatMessage &b) const {
    return m_core == b;
}

void SecretChatMessageObject::setClassType(quint32 classType) {
    SecretChatMessage::SecretChatMessageClassType result;
    switch(classType) {
    case TypeSecretChatMessage:
        result = SecretChatMessage::typeSecretChatMessage;
        break;
    default:
        result = SecretChatMessage::typeSecretChatMessage;
        break;
    }

    if(m_core.classType() == result) return;
    m_core.setClassType(result);
    Q_EMIT classTypeChanged();
    Q_EMIT coreChanged();
}

quint32 SecretChatMessageObject::classType() const {
    int result;
    switch(static_cast<qint64>(m_core.classType())) {
    case SecretChatMessage::typeSecretChatMessage:
        result = TypeSecretChatMessage;
        break;
    default:
        result = TypeSecretChatMessage;
        break;
    }

    return result;
}

void SecretChatMessageObject::setCore(const SecretChatMessage &core) {
    operator =(core);
}

SecretChatMessage SecretChatMessageObject::core() const {
    return m_core;
}

void SecretChatMessageObject::coreAttachmentChanged() {
    if(m_core.attachment() == m_attachment->core()) return;
    m_core.setAttachment(m_attachment->core());
    Q_EMIT attachmentChanged();
    Q_EMIT coreChanged();
}

void SecretChatMessageObject::coreDecryptedMessageChanged() {
    if(m_core.decryptedMessage() == m_decryptedMessage->core()) return;
    m_core.setDecryptedMessage(m_decryptedMessage->core());
    Q_EMIT decryptedMessageChanged();
    Q_EMIT coreChanged();
}
