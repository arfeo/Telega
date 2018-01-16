// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_AUTHAUTHORIZATION_OBJECT
#define LQTG_TYPE_AUTHAUTHORIZATION_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/authauthorization.h"

#include <QPointer>
#include "userobject.h"

class LIBQTELEGRAMSHARED_EXPORT AuthAuthorizationObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(AuthAuthorizationClassType)
    Q_PROPERTY(qint32 flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(qint32 tmpSessions READ tmpSessions WRITE setTmpSessions NOTIFY tmpSessionsChanged)
    Q_PROPERTY(UserObject* user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(AuthAuthorization core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum AuthAuthorizationClassType {
        TypeAuthAuthorization
    };

    AuthAuthorizationObject(const AuthAuthorization &core, QObject *parent = 0);
    AuthAuthorizationObject(QObject *parent = 0);
    virtual ~AuthAuthorizationObject();

    void setFlags(qint32 flags);
    qint32 flags() const;

    void setTmpSessions(qint32 tmpSessions);
    qint32 tmpSessions() const;

    void setUser(UserObject* user);
    UserObject* user() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const AuthAuthorization &core);
    AuthAuthorization core() const;

    AuthAuthorizationObject &operator =(const AuthAuthorization &b);
    bool operator ==(const AuthAuthorization &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void flagsChanged();
    void tmpSessionsChanged();
    void userChanged();

private Q_SLOTS:
    void coreUserChanged();

private:
    QPointer<UserObject> m_user;
    AuthAuthorization m_core;
};

#endif // LQTG_TYPE_AUTHAUTHORIZATION_OBJECT