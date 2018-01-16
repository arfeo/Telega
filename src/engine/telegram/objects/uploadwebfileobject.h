// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_UPLOADWEBFILE_OBJECT
#define LQTG_TYPE_UPLOADWEBFILE_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/uploadwebfile.h"

#include <QPointer>
#include "storagefiletypeobject.h"

class LIBQTELEGRAMSHARED_EXPORT UploadWebFileObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(UploadWebFileClassType)
    Q_PROPERTY(QByteArray bytes READ bytes WRITE setBytes NOTIFY bytesChanged)
    Q_PROPERTY(StorageFileTypeObject* fileType READ fileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(qint32 mtime READ mtime WRITE setMtime NOTIFY mtimeChanged)
    Q_PROPERTY(qint32 size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(UploadWebFile core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum UploadWebFileClassType {
        TypeUploadWebFile
    };

    UploadWebFileObject(const UploadWebFile &core, QObject *parent = 0);
    UploadWebFileObject(QObject *parent = 0);
    virtual ~UploadWebFileObject();

    void setBytes(const QByteArray &bytes);
    QByteArray bytes() const;

    void setFileType(StorageFileTypeObject* fileType);
    StorageFileTypeObject* fileType() const;

    void setMimeType(const QString &mimeType);
    QString mimeType() const;

    void setMtime(qint32 mtime);
    qint32 mtime() const;

    void setSize(qint32 size);
    qint32 size() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const UploadWebFile &core);
    UploadWebFile core() const;

    UploadWebFileObject &operator =(const UploadWebFile &b);
    bool operator ==(const UploadWebFile &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void bytesChanged();
    void fileTypeChanged();
    void mimeTypeChanged();
    void mtimeChanged();
    void sizeChanged();

private Q_SLOTS:
    void coreFileTypeChanged();

private:
    QPointer<StorageFileTypeObject> m_fileType;
    UploadWebFile m_core;
};

#endif // LQTG_TYPE_UPLOADWEBFILE_OBJECT