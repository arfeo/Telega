// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_UPDATESSTATE_OBJECT
#define LQTG_TYPE_UPDATESSTATE_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/updatesstate.h"

#include <QPointer>

class LIBQTELEGRAMSHARED_EXPORT UpdatesStateObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(UpdatesStateClassType)
    Q_PROPERTY(qint32 date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(qint32 pts READ pts WRITE setPts NOTIFY ptsChanged)
    Q_PROPERTY(qint32 qts READ qts WRITE setQts NOTIFY qtsChanged)
    Q_PROPERTY(qint32 seq READ seq WRITE setSeq NOTIFY seqChanged)
    Q_PROPERTY(qint32 unreadCount READ unreadCount WRITE setUnreadCount NOTIFY unreadCountChanged)
    Q_PROPERTY(UpdatesState core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum UpdatesStateClassType {
        TypeUpdatesState
    };

    UpdatesStateObject(const UpdatesState &core, QObject *parent = 0);
    UpdatesStateObject(QObject *parent = 0);
    virtual ~UpdatesStateObject();

    void setDate(qint32 date);
    qint32 date() const;

    void setPts(qint32 pts);
    qint32 pts() const;

    void setQts(qint32 qts);
    qint32 qts() const;

    void setSeq(qint32 seq);
    qint32 seq() const;

    void setUnreadCount(qint32 unreadCount);
    qint32 unreadCount() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const UpdatesState &core);
    UpdatesState core() const;

    UpdatesStateObject &operator =(const UpdatesState &b);
    bool operator ==(const UpdatesState &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void dateChanged();
    void ptsChanged();
    void qtsChanged();
    void seqChanged();
    void unreadCountChanged();

private Q_SLOTS:

private:
    UpdatesState m_core;
};

#endif // LQTG_TYPE_UPDATESSTATE_OBJECT