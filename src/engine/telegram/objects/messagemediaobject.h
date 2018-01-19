// This file generated by libqtelegram-code-generator.
// You can download it from: https://github.com/Aseman-Land/libqtelegram-code-generator
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

#ifndef LQTG_TYPE_MESSAGEMEDIA_OBJECT
#define LQTG_TYPE_MESSAGEMEDIA_OBJECT

#include "telegramtypeqobject.h"
#include "telegram/types/messagemedia.h"

#include <QPointer>
#include "documentobject.h"
#include "gameobject.h"
#include "geopointobject.h"
#include "photoobject.h"
#include "webdocumentobject.h"
#include "webpageobject.h"

class LIBQTELEGRAMSHARED_EXPORT MessageMediaObject : public TelegramTypeQObject
{
    Q_OBJECT
    Q_ENUMS(MessageMediaClassType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString caption READ caption WRITE setCaption NOTIFY captionChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(DocumentObject* document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(qint32 flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(GameObject* game READ game WRITE setGame NOTIFY gameChanged)
    Q_PROPERTY(GeoPointObject* geo READ geo WRITE setGeo NOTIFY geoChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(PhotoObject* photo READ photo WRITE setPhoto NOTIFY photoChanged)
    Q_PROPERTY(WebDocumentObject* photoWebDocument READ photoWebDocument WRITE setPhotoWebDocument NOTIFY photoWebDocumentChanged)
    Q_PROPERTY(QString provider READ provider WRITE setProvider NOTIFY providerChanged)
    Q_PROPERTY(qint32 receiptMsgId READ receiptMsgId WRITE setReceiptMsgId NOTIFY receiptMsgIdChanged)
    Q_PROPERTY(bool shippingAddressRequested READ shippingAddressRequested WRITE setShippingAddressRequested NOTIFY shippingAddressRequestedChanged)
    Q_PROPERTY(QString startParam READ startParam WRITE setStartParam NOTIFY startParamChanged)
    Q_PROPERTY(bool test READ test WRITE setTest NOTIFY testChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(qint64 totalAmount READ totalAmount WRITE setTotalAmount NOTIFY totalAmountChanged)
    Q_PROPERTY(qint32 userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString venueId READ venueId WRITE setVenueId NOTIFY venueIdChanged)
    Q_PROPERTY(WebPageObject* webpage READ webpage WRITE setWebpage NOTIFY webpageChanged)
    Q_PROPERTY(MessageMedia core READ core WRITE setCore NOTIFY coreChanged)
    Q_PROPERTY(quint32 classType READ classType WRITE setClassType NOTIFY classTypeChanged)

public:
    enum MessageMediaClassType {
        TypeMessageMediaEmpty,
        TypeMessageMediaPhoto,
        TypeMessageMediaGeo,
        TypeMessageMediaContact,
        TypeMessageMediaUnsupported,
        TypeMessageMediaDocument,
        TypeMessageMediaWebPage,
        TypeMessageMediaVenue,
        TypeMessageMediaGame,
        TypeMessageMediaInvoice
    };

    MessageMediaObject(const MessageMedia &core, QObject *parent = 0);
    MessageMediaObject(QObject *parent = 0);
    virtual ~MessageMediaObject();

    void setAddress(const QString &address);
    QString address() const;

    void setCaption(const QString &caption);
    QString caption() const;

    void setCurrency(const QString &currency);
    QString currency() const;

    void setDescription(const QString &description);
    QString description() const;

    void setDocument(DocumentObject* document);
    DocumentObject* document() const;

    void setFirstName(const QString &firstName);
    QString firstName() const;

    void setFlags(qint32 flags);
    qint32 flags() const;

    void setGame(GameObject* game);
    GameObject* game() const;

    void setGeo(GeoPointObject* geo);
    GeoPointObject* geo() const;

    void setLastName(const QString &lastName);
    QString lastName() const;

    void setPhoneNumber(const QString &phoneNumber);
    QString phoneNumber() const;

    void setPhoto(PhotoObject* photo);
    PhotoObject* photo() const;

    void setPhotoWebDocument(WebDocumentObject* photoWebDocument);
    WebDocumentObject* photoWebDocument() const;

    void setProvider(const QString &provider);
    QString provider() const;

    void setReceiptMsgId(qint32 receiptMsgId);
    qint32 receiptMsgId() const;

    void setShippingAddressRequested(bool shippingAddressRequested);
    bool shippingAddressRequested() const;

    void setStartParam(const QString &startParam);
    QString startParam() const;

    void setTest(bool test);
    bool test() const;

    void setTitle(const QString &title);
    QString title() const;

    void setTotalAmount(qint64 totalAmount);
    qint64 totalAmount() const;

    void setUserId(qint32 userId);
    qint32 userId() const;

    void setVenueId(const QString &venueId);
    QString venueId() const;

    void setWebpage(WebPageObject* webpage);
    WebPageObject* webpage() const;

    void setClassType(quint32 classType);
    quint32 classType() const;

    void setCore(const MessageMedia &core);
    MessageMedia core() const;

    MessageMediaObject &operator =(const MessageMedia &b);
    bool operator ==(const MessageMedia &b) const;

Q_SIGNALS:
    void coreChanged();
    void classTypeChanged();
    void addressChanged();
    void captionChanged();
    void currencyChanged();
    void descriptionChanged();
    void documentChanged();
    void firstNameChanged();
    void flagsChanged();
    void gameChanged();
    void geoChanged();
    void lastNameChanged();
    void phoneNumberChanged();
    void photoChanged();
    void photoWebDocumentChanged();
    void providerChanged();
    void receiptMsgIdChanged();
    void shippingAddressRequestedChanged();
    void startParamChanged();
    void testChanged();
    void titleChanged();
    void totalAmountChanged();
    void userIdChanged();
    void venueIdChanged();
    void webpageChanged();

private Q_SLOTS:
    void coreDocumentChanged();
    void coreGameChanged();
    void coreGeoChanged();
    void corePhotoChanged();
    void corePhotoWebDocumentChanged();
    void coreWebpageChanged();

private:
    QPointer<DocumentObject> m_document;
    QPointer<GameObject> m_game;
    QPointer<GeoPointObject> m_geo;
    QPointer<PhotoObject> m_photo;
    QPointer<WebDocumentObject> m_photoWebDocument;
    QPointer<WebPageObject> m_webpage;
    MessageMedia m_core;
};

#endif // LQTG_TYPE_MESSAGEMEDIA_OBJECT