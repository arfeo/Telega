// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "inputcontactobject.h"

InputContactObject::InputContactObject(const InputContact &core, QObject *parent) :
    TelegramTypeQObject(parent),
    m_core(core)
{
}

InputContactObject::InputContactObject(QObject *parent) :
    TelegramTypeQObject(parent),
    m_core()
{
}

InputContactObject::~InputContactObject() {
}

void InputContactObject::setClientId(qint64 clientId) {
    if(m_core.clientId() == clientId) return;
    m_core.setClientId(clientId);
    Q_EMIT clientIdChanged();
    Q_EMIT coreChanged();
}

qint64 InputContactObject::clientId() const {
    return m_core.clientId();
}

void InputContactObject::setFirstName(const QString &firstName) {
    if(m_core.firstName() == firstName) return;
    m_core.setFirstName(firstName);
    Q_EMIT firstNameChanged();
    Q_EMIT coreChanged();
}

QString InputContactObject::firstName() const {
    return m_core.firstName();
}

void InputContactObject::setLastName(const QString &lastName) {
    if(m_core.lastName() == lastName) return;
    m_core.setLastName(lastName);
    Q_EMIT lastNameChanged();
    Q_EMIT coreChanged();
}

QString InputContactObject::lastName() const {
    return m_core.lastName();
}

void InputContactObject::setPhone(const QString &phone) {
    if(m_core.phone() == phone) return;
    m_core.setPhone(phone);
    Q_EMIT phoneChanged();
    Q_EMIT coreChanged();
}

QString InputContactObject::phone() const {
    return m_core.phone();
}

InputContactObject &InputContactObject::operator =(const InputContact &b) {
    if(m_core == b) return *this;
    m_core = b;

    Q_EMIT clientIdChanged();
    Q_EMIT firstNameChanged();
    Q_EMIT lastNameChanged();
    Q_EMIT phoneChanged();
    Q_EMIT coreChanged();
    return *this;
}

bool InputContactObject::operator ==(const InputContact &b) const {
    return m_core == b;
}

void InputContactObject::setClassType(quint32 classType) {
    InputContact::InputContactClassType result;
    switch(classType) {
    case TypeInputPhoneContact:
        result = InputContact::typeInputPhoneContact;
        break;
    default:
        result = InputContact::typeInputPhoneContact;
        break;
    }

    if(m_core.classType() == result) return;
    m_core.setClassType(result);
    Q_EMIT classTypeChanged();
    Q_EMIT coreChanged();
}

quint32 InputContactObject::classType() const {
    int result;
    switch(static_cast<qint64>(m_core.classType())) {
    case InputContact::typeInputPhoneContact:
        result = TypeInputPhoneContact;
        break;
    default:
        result = TypeInputPhoneContact;
        break;
    }

    return result;
}

void InputContactObject::setCore(const InputContact &core) {
    operator =(core);
}

InputContact InputContactObject::core() const {
    return m_core;
}
