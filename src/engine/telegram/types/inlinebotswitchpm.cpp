// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "inlinebotswitchpm.h"
#include "core/inboundpkt.h"
#include "core/outboundpkt.h"
#include "../coretypes.h"

#include <QDataStream>

InlineBotSwitchPM::InlineBotSwitchPM(InlineBotSwitchPMClassType classType, InboundPkt *in) :
    m_classType(classType)
{
    if(in) fetch(in);
}

InlineBotSwitchPM::InlineBotSwitchPM(InboundPkt *in) :
    m_classType(typeInlineBotSwitchPM)
{
    fetch(in);
}

InlineBotSwitchPM::InlineBotSwitchPM(const InlineBotSwitchPM &another) :
    TelegramTypeObject(),
    m_classType(typeInlineBotSwitchPM)
{
    operator=(another);
}

InlineBotSwitchPM::InlineBotSwitchPM(const Null &null) :
    TelegramTypeObject(null),
    m_classType(typeInlineBotSwitchPM)
{
}

InlineBotSwitchPM::~InlineBotSwitchPM() {
    
}

void InlineBotSwitchPM::setStartParam(const QString &startParam) {
    m_startParam = startParam;
}

QString InlineBotSwitchPM::startParam() const {
    return m_startParam;
}

void InlineBotSwitchPM::setText(const QString &text) {
    m_text = text;
}

QString InlineBotSwitchPM::text() const {
    return m_text;
}

bool InlineBotSwitchPM::operator ==(const InlineBotSwitchPM &b) const {
    return m_classType == b.m_classType &&
           m_startParam == b.m_startParam &&
           m_text == b.m_text;
}

InlineBotSwitchPM &InlineBotSwitchPM::operator =(const InlineBotSwitchPM &b) {
    m_classType = b.m_classType;
    m_startParam = b.m_startParam;
    m_text = b.m_text;
    setNull(b.isNull());
    return *this;
}

void InlineBotSwitchPM::setClassType(InlineBotSwitchPM::InlineBotSwitchPMClassType classType) {
    m_classType = classType;
}

InlineBotSwitchPM::InlineBotSwitchPMClassType InlineBotSwitchPM::classType() const {
    return m_classType;
}

bool InlineBotSwitchPM::fetch(InboundPkt *in) {
    LQTG_FETCH_LOG;
    int x = in->fetchInt();
    switch(x) {
    case typeInlineBotSwitchPM: {
        m_text = in->fetchQString();
        m_startParam = in->fetchQString();
        m_classType = static_cast<InlineBotSwitchPMClassType>(x);
        return true;
    }
        break;
    
    default:
        LQTG_FETCH_ASSERT;
        return false;
    }
}

bool InlineBotSwitchPM::push(OutboundPkt *out) const {
    out->appendInt(m_classType);
    switch(m_classType) {
    case typeInlineBotSwitchPM: {
        out->appendQString(m_text);
        out->appendQString(m_startParam);
        return true;
    }
        break;
    
    default:
        return false;
    }
}

QMap<QString, QVariant> InlineBotSwitchPM::toMap() const {
    QMap<QString, QVariant> result;
    switch(static_cast<int>(m_classType)) {
    case typeInlineBotSwitchPM: {
        result["classType"] = "InlineBotSwitchPM::typeInlineBotSwitchPM";
        if( !m_text.isEmpty() ) result["text"] = QVariant::fromValue<QString>(m_text);
        if( !m_startParam.isEmpty() ) result["startParam"] = QVariant::fromValue<QString>(m_startParam);
        return result;
    }
        break;
    
    default:
        return result;
    }
}

InlineBotSwitchPM InlineBotSwitchPM::fromMap(const QMap<QString, QVariant> &map) {
    InlineBotSwitchPM result;
    if(map.value("classType").toString() == "InlineBotSwitchPM::typeInlineBotSwitchPM") {
        result.setClassType(typeInlineBotSwitchPM);
        QVariant _text_var = map.value("text");
        if( !_text_var.isNull() ) {
            _text_var.convert( QVariant::nameToType("QString") );
            result.setText( _text_var.value<QString>() );
        }
        
        QVariant _startParam_var = map.value("startParam");
        if( !_startParam_var.isNull() ) {
            _startParam_var.convert( QVariant::nameToType("QString") );
            result.setStartParam( _startParam_var.value<QString>() );
        }
        
        return result;
    }
    return result;
}

InlineBotSwitchPM InlineBotSwitchPM::fromJson(const QString &json) {
    return InlineBotSwitchPM::fromMap(QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap());
}

QByteArray InlineBotSwitchPM::getHash(QCryptographicHash::Algorithm alg) const {
    QByteArray data;
    QDataStream str(&data, QIODevice::WriteOnly);
    str << *this;
    return QCryptographicHash::hash(data, alg);
}

QDataStream &operator<<(QDataStream &stream, const InlineBotSwitchPM &item) {
    stream << static_cast<uint>(item.classType());
    switch(item.classType()) {
    case InlineBotSwitchPM::typeInlineBotSwitchPM:
        stream << item.text();
        stream << item.startParam();
        break;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, InlineBotSwitchPM &item) {
    uint type = 0;
    stream >> type;
    item.setClassType(static_cast<InlineBotSwitchPM::InlineBotSwitchPMClassType>(type));
    switch(type) {
    case InlineBotSwitchPM::typeInlineBotSwitchPM: {
        QString m_text;
        stream >> m_text;
        item.setText(m_text);
        QString m_start_param;
        stream >> m_start_param;
        item.setStartParam(m_start_param);
    }
        break;
    }
    return stream;
}

QDebug operator<<(QDebug debug,  const InlineBotSwitchPM &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)
    debug.nospace() << "Telegram.InlineBotSwitchPM(";
    switch(item.classType()) {
    case InlineBotSwitchPM::typeInlineBotSwitchPM:
        debug.nospace() << "classType: typeInlineBotSwitchPM";
        debug.nospace() << ", text: " << item.text();
        debug.nospace() << ", startParam: " << item.startParam();
        break;
    }
    debug.nospace() << ")";
    return debug;
}
