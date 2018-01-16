// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_DOCUMENTATTRIBUTE_OBJECT
#define LQTG_TYPE_DOCUMENTATTRIBUTE_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/documentattribute.h"

#include <QPointer>
#include "maskcoordsobject.h"
#include "inputstickersetobject.h"

class LIBQTELEGRAMSHARED_EXPORT DocumentAttributeObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(DocumentAttributeClassType)
    Q_PROPERTY(QString alt READ alt WRITE setAlt NOTIFY altChanged)
    Q_PROPERTY(qint32 duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(qint32 flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(qint32 h READ h WRITE setH NOTIFY hChanged)
    Q_PROPERTY(bool mask READ mask WRITE setMask NOTIFY maskChanged)
    Q_PROPERTY(MaskCoordsObject* maskCoords READ maskCoords WRITE setMaskCoords NOTIFY maskCoordsChanged)
    Q_PROPERTY(QString performer READ performer WRITE setPerformer NOTIFY performerChanged)
    Q_PROPERTY(bool roundMessage READ roundMessage WRITE setRoundMessage NOTIFY roundMessageChanged)
    Q_PROPERTY(InputStickerSetObject* stickerset READ stickerset WRITE setStickerset NOTIFY stickersetChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool voice READ voice WRITE setVoice NOTIFY voiceChanged)
    Q_PROPERTY(qint32 w READ w WRITE setW NOTIFY wChanged)
    Q_PROPERTY(QByteArray waveform READ waveform WRITE setWaveform NOTIFY waveformChanged)
    Q_PROPERTY(DocumentAttribute core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum DocumentAttributeClassType {
        TypeDocumentAttributeImageSize,
        TypeDocumentAttributeAnimated,
        TypeDocumentAttributeSticker,
        TypeDocumentAttributeVideo,
        TypeDocumentAttributeAudio,
        TypeDocumentAttributeFilename,
        TypeDocumentAttributeHasStickers,
        TypeDocumentAttributeImageSizeSecret23,
        TypeDocumentAttributeAnimatedSecret23,
        TypeDocumentAttributeStickerSecret23,
        TypeDocumentAttributeVideoSecret23,
        TypeDocumentAttributeAudioSecret23,
        TypeDocumentAttributeFilenameSecret23
    };

    DocumentAttributeObject(const DocumentAttribute &core, QObject *parent = 0);
    DocumentAttributeObject(QObject *parent = 0);
    virtual ~DocumentAttributeObject();

    void setAlt(const QString &alt);
    QString alt() const;

    void setDuration(qint32 duration);
    qint32 duration() const;

    void setFileName(const QString &fileName);
    QString fileName() const;

    void setFlags(qint32 flags);
    qint32 flags() const;

    void setH(qint32 h);
    qint32 h() const;

    void setMask(bool mask);
    bool mask() const;

    void setMaskCoords(MaskCoordsObject* maskCoords);
    MaskCoordsObject* maskCoords() const;

    void setPerformer(const QString &performer);
    QString performer() const;

    void setRoundMessage(bool roundMessage);
    bool roundMessage() const;

    void setStickerset(InputStickerSetObject* stickerset);
    InputStickerSetObject* stickerset() const;

    void setTitle(const QString &title);
    QString title() const;

    void setVoice(bool voice);
    bool voice() const;

    void setW(qint32 w);
    qint32 w() const;

    void setWaveform(const QByteArray &waveform);
    QByteArray waveform() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const DocumentAttribute &core);
    DocumentAttribute core() const;

    DocumentAttributeObject &operator =(const DocumentAttribute &b);
    bool operator ==(const DocumentAttribute &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void altChanged();
    void durationChanged();
    void fileNameChanged();
    void flagsChanged();
    void hChanged();
    void maskChanged();
    void maskCoordsChanged();
    void performerChanged();
    void roundMessageChanged();
    void stickersetChanged();
    void titleChanged();
    void voiceChanged();
    void wChanged();
    void waveformChanged();

private Q_SLOTS:
    void coreMaskCoordsChanged();
    void coreStickersetChanged();

private:
    QPointer<MaskCoordsObject> m_maskCoords;
    QPointer<InputStickerSetObject> m_stickerset;
    DocumentAttribute m_core;
};

#endif // LQTG_TYPE_DOCUMENTATTRIBUTE_OBJECT