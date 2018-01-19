// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "decryptedmessageactionobject.h"

DecryptedMessageActionObject::DecryptedMessageActionObject(const DecryptedMessageAction &core, QObject *parent) :
    TelegramTypeQObject(parent),
    m_action(0),
    m_core(core)
{
    m_action = new SendMessageActionObject(m_core.action(), this);
    connect(m_action.data(), SIGNAL(coreChanged()), this, SLOT(coreActionChanged()));
}

DecryptedMessageActionObject::DecryptedMessageActionObject(QObject *parent) :
    TelegramTypeQObject(parent),
    m_action(0),
    m_core()
{
    m_action = new SendMessageActionObject(m_core.action(), this);
    connect(m_action.data(), SIGNAL(coreChanged()), this, SLOT(coreActionChanged()));
}

DecryptedMessageActionObject::~DecryptedMessageActionObject() {
}

void DecryptedMessageActionObject::setAction(SendMessageActionObject* action) {
    if(m_action == action) return;
    if(m_action) delete m_action;
    m_action = action;
    if(m_action) {
        m_action->setParent(this);
        m_core.setAction(m_action->core());
        connect(m_action.data(), SIGNAL(coreChanged()), this, SLOT(coreActionChanged()));
    }
    Q_EMIT actionChanged();
    Q_EMIT coreChanged();
}

SendMessageActionObject*  DecryptedMessageActionObject::action() const {
    return m_action;
}

void DecryptedMessageActionObject::setEndSeqNo(qint32 endSeqNo) {
    if(m_core.endSeqNo() == endSeqNo) return;
    m_core.setEndSeqNo(endSeqNo);
    Q_EMIT endSeqNoChanged();
    Q_EMIT coreChanged();
}

qint32 DecryptedMessageActionObject::endSeqNo() const {
    return m_core.endSeqNo();
}

void DecryptedMessageActionObject::setExchangeId(qint64 exchangeId) {
    if(m_core.exchangeId() == exchangeId) return;
    m_core.setExchangeId(exchangeId);
    Q_EMIT exchangeIdChanged();
    Q_EMIT coreChanged();
}

qint64 DecryptedMessageActionObject::exchangeId() const {
    return m_core.exchangeId();
}

void DecryptedMessageActionObject::setGA(const QByteArray &gA) {
    if(m_core.gA() == gA) return;
    m_core.setGA(gA);
    Q_EMIT gAChanged();
    Q_EMIT coreChanged();
}

QByteArray DecryptedMessageActionObject::gA() const {
    return m_core.gA();
}

void DecryptedMessageActionObject::setGB(const QByteArray &gB) {
    if(m_core.gB() == gB) return;
    m_core.setGB(gB);
    Q_EMIT gBChanged();
    Q_EMIT coreChanged();
}

QByteArray DecryptedMessageActionObject::gB() const {
    return m_core.gB();
}

void DecryptedMessageActionObject::setKeyFingerprint(qint64 keyFingerprint) {
    if(m_core.keyFingerprint() == keyFingerprint) return;
    m_core.setKeyFingerprint(keyFingerprint);
    Q_EMIT keyFingerprintChanged();
    Q_EMIT coreChanged();
}

qint64 DecryptedMessageActionObject::keyFingerprint() const {
    return m_core.keyFingerprint();
}

void DecryptedMessageActionObject::setLayer(qint32 layer) {
    if(m_core.layer() == layer) return;
    m_core.setLayer(layer);
    Q_EMIT layerChanged();
    Q_EMIT coreChanged();
}

qint32 DecryptedMessageActionObject::layer() const {
    return m_core.layer();
}

void DecryptedMessageActionObject::setRandomIds(const QList<qint64> &randomIds) {
    if(m_core.randomIds() == randomIds) return;
    m_core.setRandomIds(randomIds);
    Q_EMIT randomIdsChanged();
    Q_EMIT coreChanged();
}

QList<qint64> DecryptedMessageActionObject::randomIds() const {
    return m_core.randomIds();
}

void DecryptedMessageActionObject::setStartSeqNo(qint32 startSeqNo) {
    if(m_core.startSeqNo() == startSeqNo) return;
    m_core.setStartSeqNo(startSeqNo);
    Q_EMIT startSeqNoChanged();
    Q_EMIT coreChanged();
}

qint32 DecryptedMessageActionObject::startSeqNo() const {
    return m_core.startSeqNo();
}

void DecryptedMessageActionObject::setTtlSeconds(qint32 ttlSeconds) {
    if(m_core.ttlSeconds() == ttlSeconds) return;
    m_core.setTtlSeconds(ttlSeconds);
    Q_EMIT ttlSecondsChanged();
    Q_EMIT coreChanged();
}

qint32 DecryptedMessageActionObject::ttlSeconds() const {
    return m_core.ttlSeconds();
}

DecryptedMessageActionObject &DecryptedMessageActionObject::operator =(const DecryptedMessageAction &b) {
    if(m_core == b) return *this;
    m_core = b;
    m_action->setCore(b.action());

    Q_EMIT actionChanged();
    Q_EMIT endSeqNoChanged();
    Q_EMIT exchangeIdChanged();
    Q_EMIT gAChanged();
    Q_EMIT gBChanged();
    Q_EMIT keyFingerprintChanged();
    Q_EMIT layerChanged();
    Q_EMIT randomIdsChanged();
    Q_EMIT startSeqNoChanged();
    Q_EMIT ttlSecondsChanged();
    Q_EMIT coreChanged();
    return *this;
}

bool DecryptedMessageActionObject::operator ==(const DecryptedMessageAction &b) const {
    return m_core == b;
}

void DecryptedMessageActionObject::setClassType(quint32 classType) {
    DecryptedMessageAction::DecryptedMessageActionClassType result;
    switch(classType) {
    case TypeDecryptedMessageActionSetMessageTTLSecret8:
        result = DecryptedMessageAction::typeDecryptedMessageActionSetMessageTTLSecret8;
        break;
    case TypeDecryptedMessageActionReadMessagesSecret8:
        result = DecryptedMessageAction::typeDecryptedMessageActionReadMessagesSecret8;
        break;
    case TypeDecryptedMessageActionDeleteMessagesSecret8:
        result = DecryptedMessageAction::typeDecryptedMessageActionDeleteMessagesSecret8;
        break;
    case TypeDecryptedMessageActionScreenshotMessagesSecret8:
        result = DecryptedMessageAction::typeDecryptedMessageActionScreenshotMessagesSecret8;
        break;
    case TypeDecryptedMessageActionFlushHistorySecret8:
        result = DecryptedMessageAction::typeDecryptedMessageActionFlushHistorySecret8;
        break;
    case TypeDecryptedMessageActionResendSecret17:
        result = DecryptedMessageAction::typeDecryptedMessageActionResendSecret17;
        break;
    case TypeDecryptedMessageActionNotifyLayerSecret17:
        result = DecryptedMessageAction::typeDecryptedMessageActionNotifyLayerSecret17;
        break;
    case TypeDecryptedMessageActionTypingSecret17:
        result = DecryptedMessageAction::typeDecryptedMessageActionTypingSecret17;
        break;
    case TypeDecryptedMessageActionRequestKeySecret20:
        result = DecryptedMessageAction::typeDecryptedMessageActionRequestKeySecret20;
        break;
    case TypeDecryptedMessageActionAcceptKeySecret20:
        result = DecryptedMessageAction::typeDecryptedMessageActionAcceptKeySecret20;
        break;
    case TypeDecryptedMessageActionAbortKeySecret20:
        result = DecryptedMessageAction::typeDecryptedMessageActionAbortKeySecret20;
        break;
    case TypeDecryptedMessageActionCommitKeySecret20:
        result = DecryptedMessageAction::typeDecryptedMessageActionCommitKeySecret20;
        break;
    case TypeDecryptedMessageActionNoopSecret20:
        result = DecryptedMessageAction::typeDecryptedMessageActionNoopSecret20;
        break;
    default:
        result = DecryptedMessageAction::typeDecryptedMessageActionSetMessageTTLSecret8;
        break;
    }

    if(m_core.classType() == result) return;
    m_core.setClassType(result);
    Q_EMIT classTypeChanged();
    Q_EMIT coreChanged();
}

quint32 DecryptedMessageActionObject::classType() const {
    int result;
    switch(static_cast<qint64>(m_core.classType())) {
    case DecryptedMessageAction::typeDecryptedMessageActionSetMessageTTLSecret8:
        result = TypeDecryptedMessageActionSetMessageTTLSecret8;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionReadMessagesSecret8:
        result = TypeDecryptedMessageActionReadMessagesSecret8;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionDeleteMessagesSecret8:
        result = TypeDecryptedMessageActionDeleteMessagesSecret8;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionScreenshotMessagesSecret8:
        result = TypeDecryptedMessageActionScreenshotMessagesSecret8;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionFlushHistorySecret8:
        result = TypeDecryptedMessageActionFlushHistorySecret8;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionResendSecret17:
        result = TypeDecryptedMessageActionResendSecret17;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionNotifyLayerSecret17:
        result = TypeDecryptedMessageActionNotifyLayerSecret17;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionTypingSecret17:
        result = TypeDecryptedMessageActionTypingSecret17;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionRequestKeySecret20:
        result = TypeDecryptedMessageActionRequestKeySecret20;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionAcceptKeySecret20:
        result = TypeDecryptedMessageActionAcceptKeySecret20;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionAbortKeySecret20:
        result = TypeDecryptedMessageActionAbortKeySecret20;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionCommitKeySecret20:
        result = TypeDecryptedMessageActionCommitKeySecret20;
        break;
    case DecryptedMessageAction::typeDecryptedMessageActionNoopSecret20:
        result = TypeDecryptedMessageActionNoopSecret20;
        break;
    default:
        result = TypeDecryptedMessageActionSetMessageTTLSecret8;
        break;
    }

    return result;
}

void DecryptedMessageActionObject::setCore(const DecryptedMessageAction &core) {
    operator =(core);
}

DecryptedMessageAction DecryptedMessageActionObject::core() const {
    return m_core;
}

void DecryptedMessageActionObject::coreActionChanged() {
    if(m_core.action() == m_action->core()) return;
    m_core.setAction(m_action->core());
    Q_EMIT actionChanged();
    Q_EMIT coreChanged();
}
