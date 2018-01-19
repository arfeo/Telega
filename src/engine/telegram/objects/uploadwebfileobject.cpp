// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#include "uploadwebfileobject.h"

UploadWebFileObject::UploadWebFileObject(const UploadWebFile &core, QObject *parent) :
    TelegramTypeQObject(parent),
    m_fileType(0),
    m_core(core)
{
    m_fileType = new StorageFileTypeObject(m_core.fileType(), this);
    connect(m_fileType.data(), SIGNAL(coreChanged()), this, SLOT(coreFileTypeChanged()));
}

UploadWebFileObject::UploadWebFileObject(QObject *parent) :
    TelegramTypeQObject(parent),
    m_fileType(0),
    m_core()
{
    m_fileType = new StorageFileTypeObject(m_core.fileType(), this);
    connect(m_fileType.data(), SIGNAL(coreChanged()), this, SLOT(coreFileTypeChanged()));
}

UploadWebFileObject::~UploadWebFileObject() {
}

void UploadWebFileObject::setBytes(const QByteArray &bytes) {
    if(m_core.bytes() == bytes) return;
    m_core.setBytes(bytes);
    Q_EMIT bytesChanged();
    Q_EMIT coreChanged();
}

QByteArray UploadWebFileObject::bytes() const {
    return m_core.bytes();
}

void UploadWebFileObject::setFileType(StorageFileTypeObject* fileType) {
    if(m_fileType == fileType) return;
    if(m_fileType) delete m_fileType;
    m_fileType = fileType;
    if(m_fileType) {
        m_fileType->setParent(this);
        m_core.setFileType(m_fileType->core());
        connect(m_fileType.data(), SIGNAL(coreChanged()), this, SLOT(coreFileTypeChanged()));
    }
    Q_EMIT fileTypeChanged();
    Q_EMIT coreChanged();
}

StorageFileTypeObject*  UploadWebFileObject::fileType() const {
    return m_fileType;
}

void UploadWebFileObject::setMimeType(const QString &mimeType) {
    if(m_core.mimeType() == mimeType) return;
    m_core.setMimeType(mimeType);
    Q_EMIT mimeTypeChanged();
    Q_EMIT coreChanged();
}

QString UploadWebFileObject::mimeType() const {
    return m_core.mimeType();
}

void UploadWebFileObject::setMtime(qint32 mtime) {
    if(m_core.mtime() == mtime) return;
    m_core.setMtime(mtime);
    Q_EMIT mtimeChanged();
    Q_EMIT coreChanged();
}

qint32 UploadWebFileObject::mtime() const {
    return m_core.mtime();
}

void UploadWebFileObject::setSize(qint32 size) {
    if(m_core.size() == size) return;
    m_core.setSize(size);
    Q_EMIT sizeChanged();
    Q_EMIT coreChanged();
}

qint32 UploadWebFileObject::size() const {
    return m_core.size();
}

UploadWebFileObject &UploadWebFileObject::operator =(const UploadWebFile &b) {
    if(m_core == b) return *this;
    m_core = b;
    m_fileType->setCore(b.fileType());

    Q_EMIT bytesChanged();
    Q_EMIT fileTypeChanged();
    Q_EMIT mimeTypeChanged();
    Q_EMIT mtimeChanged();
    Q_EMIT sizeChanged();
    Q_EMIT coreChanged();
    return *this;
}

bool UploadWebFileObject::operator ==(const UploadWebFile &b) const {
    return m_core == b;
}

void UploadWebFileObject::setClassType(quint32 classType) {
    UploadWebFile::UploadWebFileClassType result;
    switch(classType) {
    case TypeUploadWebFile:
        result = UploadWebFile::typeUploadWebFile;
        break;
    default:
        result = UploadWebFile::typeUploadWebFile;
        break;
    }

    if(m_core.classType() == result) return;
    m_core.setClassType(result);
    Q_EMIT classTypeChanged();
    Q_EMIT coreChanged();
}

quint32 UploadWebFileObject::classType() const {
    int result;
    switch(static_cast<qint64>(m_core.classType())) {
    case UploadWebFile::typeUploadWebFile:
        result = TypeUploadWebFile;
        break;
    default:
        result = TypeUploadWebFile;
        break;
    }

    return result;
}

void UploadWebFileObject::setCore(const UploadWebFile &core) {
    operator =(core);
}

UploadWebFile UploadWebFileObject::core() const {
    return m_core;
}

void UploadWebFileObject::coreFileTypeChanged() {
    if(m_core.fileType() == m_fileType->core()) return;
    m_core.setFileType(m_fileType->core());
    Q_EMIT fileTypeChanged();
    Q_EMIT coreChanged();
}
