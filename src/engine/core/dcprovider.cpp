/*
 * Copyright 2014 Canonical Ltd.
 * Authors:
 *      Roberto Mier
 *      Tiago Herrmann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "dcprovider.h"
#include "constants.h"
#include "session.h"
#include "tlvalues.h"

DcProvider::DcProvider() :
        mApi(0),
        mPendingDcs(0),
        mPendingTransferSessions(0),
        mWorkingDcSession(0)
{
    // ..
}

DcProvider::~DcProvider() {
   clean();
}

void DcProvider::clean() {
    Q_FOREACH (DC *dc, mDcs) {
        delete dc;
        dc = 0;
    }
    Q_FOREACH (DCAuth *dcAuth, mDcAuths) {
        if (dcAuth) {
            if (dcAuth->state() != QAbstractSocket::UnconnectedState) {
                connect(dcAuth, SIGNAL(disconnected()), dcAuth, SLOT(deleteLater()));
                dcAuth->disconnectFromHost();
            } else {
                dcAuth->deleteLater();
            }
            dcAuth = 0;
        }
    }
    Q_FOREACH (Session *session, mTransferSessions)  {
        if (session) {
            session->close();
        }
    }
    if (mApi) {
        mApi->deleteLater();
        mApi = 0;
    }
//    qDebug() << mDcs.count();
    mDcs.clear();
    mDcAuths.clear();
    mTransferSessions.clear();
    mPendingDcs = 0;
    mPendingTransferSessions = 0;
    mWorkingDcSession = 0;
}

DC *DcProvider::getDc(qint32 dcNum) const {
    return mDcs.value(dcNum, NULL);
}

DC *DcProvider::getWorkingDc() const {
    return mDcs.value(Settings::getInstance()->workingDcNum(), 0);
}


void DcProvider::initialize() {

    clean();

    /**
      1.- Read settings (dcsList and working dc num)
      2.- Fullfill m_dcs map with dcsList
      3.- Connect to working dc
      4.- Ask for config when working dc is connected
      5.- Connect to the rest of the dcs received in config
      */

    // 1.- Get the settings dcs, fullfill m_dcs map and current dc num
    Settings *s = Settings::getInstance();
    qDebug() << "dc list getting warning up";
    QList<DC *> dcsList = s->dcsList();
    qDebug() << "dc list is getting poluted" << s->workingDcNum();
    Q_FOREACH (DC *dc, dcsList) {
        qDebug()<< "DC list status of servers" << dc->state();
        mDcs.insert(dc->id(), dc);
    }
    qDebug() << "Active DC" << s->workingDcNum();
//     2.- connect to working DC
// If this is the initial configuration, if will be executed
//    s->setWorkingDcNum(1);
    if ( dcsList.count() == 0 ) {
         qDebug() << "If dcId == 1, it's the default one, so call default host and port";
        if (!mDcs.value(1, 0)) {
            mDcs[1] = new DC(1);
        }
        qDebug() << "If Dc are NUll, Populate DC with default server settings";
        if (mDcs[1]->state() < DC::authKeyCreated) {
            qDebug() << "Creating a new DC with IP" << TELEGRAM_SERVER_HOST << "@ port" << TELEGRAM_SERVER_PORT;
            mDcs[1]->setHost(TELEGRAM_SERVER_HOST);
            mDcs[1]->setPort(TELEGRAM_SERVER_PORT);
            DCAuth *dcAuth = new DCAuth(mDcs[1], this);
            mDcAuths.insert(1, dcAuth);
            connect(dcAuth, SIGNAL(fatalError()), this, SLOT(logOut()));
            connect(dcAuth, SIGNAL(dcReady(DC*)), this, SLOT(onDcReady(DC*)));
            dcAuth->createAuthKey();
        } else {
            onDcReady(mDcs[1]);
        }
    }
    else {
        qDebug() << "In any other case, the host and port have been retrieved from auth file settings and the DC object is already created";
        if (mDcs[s->workingDcNum()]->state() < DC::authKeyCreated) {
            DCAuth *dcAuth = new DCAuth(mDcs[s->workingDcNum()]);
            mDcAuths.insert(s->workingDcNum(), dcAuth);
            connect(dcAuth, SIGNAL(fatalError()), this, SLOT(logOut()));
            connect(dcAuth, SIGNAL(fatalError()), this, SIGNAL(fatalError()));
            connect(dcAuth, SIGNAL(dcReady(DC*)), this, SLOT(onDcReady(DC*)));
            dcAuth->createAuthKey();
        } else {
            onDcReady(mDcs[s->workingDcNum()]);
        }
    }
}

void DcProvider::onDcReady(DC *dc) {
    qDebug() << "DC" << dc->id() << "with auth key state"<< dc->state();
    DCAuth *dcAuth = mDcAuths.value(dc->id());
    if (dcAuth) {
        if (dcAuth->state() != QAbstractSocket::UnconnectedState) {
            connect(dcAuth, SIGNAL(disconnected()), this, SLOT(onDcAuthDisconnected()));
            dcAuth->disconnectFromHost();
            qDebug() << "DcAuth disconnect";
        } else {
            processDcReady(dc);
        }
    } else {
        processDcReady(dc);
    }
}

void DcProvider::onDcAuthDisconnected() {
    DCAuth *dcAuth = qobject_cast<DCAuth *>(sender());
    processDcReady(dcAuth->dc());
    mDcAuths.remove(dcAuth->dc()->id());
    dcAuth->deleteLater();
//    delete dcAuth;
}

void DcProvider::processDcReady(DC *dc) {
    if ((!mApi) && (dc->id() == Settings::getInstance()->workingDcNum())) {
        Session *session = new Session(dc, this);
        mApi = new Api(session, this);
        connect(session, SIGNAL(sessionReady(DC*)), this, SLOT(onApiReady(DC*)));
        connect(session, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onApiError()));
        session->connectToServer();
//        delete session;
    }
    else{
        if (mPendingDcs-- == 0) {
        qDebug() << "Authenticated All DC's";
//        int check = 0;
//        Q_FOREACH (DC *dc, mDcs) {
//                       if(dc->authKey() && dc->authKeyId() ){
//                           check++;
//                           }
//                       }
//        qDebug()<< "check " << check ;
        // if all dcs are authorized, emit provider ready signal
        // save the settings here, after all dcs are ready

        Settings::getInstance()->setDcsList(mDcs.values());
        Settings::getInstance()->writeAuthFile();
//        delete dc;
//        // qCDebug(TG_INIT_DCPROVIDER) << "DcProvider ready";
        Q_EMIT dcProviderReady();
        // if current dc is in "authKeyCreated" state, authNeeded signal must be emitted for user to sign in.
        // if current dc is in "userSignedIn" state, check that all dcs are in that state or export/import auth.
        // If they aren't, transfer auth from workingDc to workingDc+1, from workingDc+1 to workingDc+2...etc until completed all.
        switch (mDcs.value(Settings::getInstance()->workingDcNum())->state()) {
        case DC::authKeyCreated:
            emit authNeeded();
            break;
        case DC::userSignedIn:
            emit chatview();
            break;
        default:
            Q_ASSERT(0); // it's impossible getting here. This is done to avoid warning when compilation
            break;
        }
        }
    }
}

void DcProvider::onApiError() {

    Session *session = qobject_cast<Session *>(sender());
//    qCWarning(TG_INIT_DCPROVIDER) << "Api init error when connecting session to server:" << session->errorString();

    // after emitting these startup offline signals, we don't want to do it again when session gets connected, so disconnect signal-slot
    disconnect(session, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onApiError()));
    // also disconnect the signal-slot to query for current server config when connected (once logged in that info is irrelevant)
    disconnect(session, SIGNAL(sessionReady(DC*)), this, SLOT(onApiReady(DC*)));

    bool userSignedInAllDCs = true;
    // because we haven't connection, we assume the available DCs have not changed since last startup.
    // Check if there is a shared key created for all DCs readed from config and if the user has
    // been authenticated in all them
    Q_FOREACH (DC *dc, mDcs) {
        if (dc->state() < DC::authKeyCreated) {
            // emit a fatal error that should be shown in app as the reason to abort starting up the app
//            Q_EMIT error(-1,
//                         500,
//                         "Impossible to start application. There is no connection to server and stored credentials are not valid");
            return;
        } else if (dc->state() < DC::userSignedIn) {
            userSignedInAllDCs = false;
        }
    }

    Q_EMIT dcProviderReady();

    if (userSignedInAllDCs) {
        qDebug() << "User signed all DC's";
        Q_EMIT authTransferCompleted();
    } else {
        qDebug() << "User need auth needed";
        Q_EMIT authNeeded();
    }
}

void DcProvider::onApiReady(DC*) {
    qDebug() << "Getting DC config";
;    Session *session = qobject_cast<Session *>(sender());
//    // qCDebug(TG_INIT_DCPROVIDER) << "Api connected to server and ready";

    // after emitting the api startup signals, we don't want to do it again when session gets connected, so disconnect signal-slot
    disconnect(session, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onApiError()));
    // disconnect the signal-slot to query for current server config when connected (once logged in that info is irrelevant)
    disconnect(session, SIGNAL(sessionReady(DC*)), this, SLOT(onApiReady(DC*)));

    // get the config
    connect(mApi, SIGNAL(config(qint64,qint32,bool,qint32,QList<DcOption>,qint32,qint32)), this, SLOT(onConfigReceived(qint64,qint32,bool,qint32,QList<DcOption>,qint32,qint32)));
    mApi->helpGetConfig();
}

void DcProvider::onConfigReceived(qint64 msgId, qint32 date, bool testMode, qint32 thisDc, const QList<DcOption> &dcOptions, qint32 chatMaxSize, qint32 broadcastMaxSize) {

//     qDebug() << "onConfigReceived(), msgId =" << QString::number(msgId, 16);
//     qDebug() << "date =" << date;
//     qDebug() << "testMode =" << testMode;
//     qDebug() << "thisDc =" << thisDc;
    bool keyauth = true;
    qint32 dcslist = 0;
    mPendingDcs = dcOptions.length() -1; //all the received options but the default one, yet used

    Q_FOREACH (DcOption dcOption, dcOptions) {
//         qDebug() << "dcOption | id =" << dcOption.id() << ", ipAddress =" << dcOption.ipAddress() << ", port =" << dcOption.port() << ", hostname =" << dcOption.hostname();
        // for every new DC or not authenticated DC, insert into m_dcs and authenticate
        DC *dc = mDcs.value(dcOption.id());
        // check if dc is not null or if received host and port are not equals than settings ones
        if ((!dc) || ((dc->host() != dcOption.ipAddress()) || (dc->port() != dcOption.port()))) {
            // if not exists dc or host and port different, create a new dc object for this dcId and add it to m_dcs map
            dc = new DC(dcOption.id());
            dc->setHost(dcOption.ipAddress());
            dc->setPort(dcOption.port());
            mDcs.insert(dcOption.id(), dc);
        }
        if (dc->state() < DC::authKeyCreated) {
            qDebug() << "Verify DC" << dc->state();
            DCAuth *dcAuth = new DCAuth(dc, this);
            mDcAuths.insert(dcOption.id(), dcAuth);
            connect(dcAuth, SIGNAL(fatalError()), this, SLOT(logOut()));
            connect(dcAuth, SIGNAL(fatalError()), this, SIGNAL(fatalError()));
            connect(dcAuth, SIGNAL(dcReady(DC*)), this, SLOT(onDcReady(DC*)));
            dcAuth->createAuthKey();
        } else if (dcOption.id() != thisDc) {
            // if authorized and not working dc emit dcReady signal directly
            onDcReady(dc);
        }
        if(dc->state() == DC::authKeyCreated){
            dcslist = dcslist + 1;
        }
        qDebug() << dc->host() << dc->port() ;
    }
    if(dcslist == 5){
        Q_EMIT dcProviderReady();
    }
}

Api *DcProvider::getApi() const {
    return mApi;
}

QList<DC *> DcProvider::getDcs() const {
    return mDcs.values();
}

void DcProvider::transferAuth() {
    // add in a list all the dcs in DcState::authKeyCreated but not in DCState::userSignedIn
    // and receiving the authorization data (all but working dc one)
    Q_ASSERT(mApi);
    bool hasTransferSessions = false;
    mWorkingDcSession = mApi->mainSession();
    Q_ASSERT(mWorkingDcSession);
    QList<DC *> m_dcsList = mDcs.values();
    for (qint32 i=0 ; i < m_dcsList.size(); i++) {
        DC *dc = m_dcsList.value(i);
        if (dc->state() == DC::authKeyCreated &&
                dc->id() != Settings::getInstance()->workingDcNum()) {
            hasTransferSessions = true;
            // create a new session for this dc
            Session *session = mApi->fileSession(dc);
            connect(session, SIGNAL(sessionReady(DC*)), this, SLOT(onTransferSessionReady(DC*)));
            session->connectToServer();
            mPendingTransferSessions++;
        }
    }
    if (!hasTransferSessions) {
        qDebug() << "authtransfer completed";
        Q_EMIT authTransferCompleted();
    }
}

void DcProvider::onTransferSessionReady(DC *) {
    qDebug()<< "on transfer session ready begin";
    qDebug()<<"mPendingTransferSessions"<<mPendingTransferSessions;
    Session *session = qobject_cast<Session *>(sender());
    mTransferSessions.append(session);
    if(--mPendingTransferSessions == 0){
        connect(mApi, SIGNAL(authExportedAuthorization(qint64,qint32,QByteArray)), this, SLOT(onAuthExportedAuthorization(qint64,qint32,QByteArray)));
        connect(mApi, SIGNAL(authImportedAuthorization(qint64,qint32,User)), this, SLOT(onAuthImportedAuthorization(qint64,qint32,User)));
        mApi->authExportAuthorization(mTransferSessions.first()->dc()->id());
        qDebug()<< "on transfer session ready begin if loop end" << mTransferSessions.first()->dc()->id();
    }
    qDebug()<< "on transfer session ready end";
//    delete session;
}

void DcProvider::onAuthExportedAuthorization(qint64, qint32 ourId, const QByteArray &bytes) {
    qDebug()<< "onAuthExportedAuthorization begin";
    // Set ourId into settings (It doesn't matter if set before)
    Settings::getInstance()->setOurId(ourId);
    // Change api dc to first in the transfer dcs list
    mApi->setMainSession(mTransferSessions.first());
    // Execute import in this dc
    mApi->authImportAuthorization(ourId, bytes);
    qDebug()<< "onAuthExportedAuthorization ebd";

}

void DcProvider::onAuthImportedAuthorization(qint64, qint32 expires, const User &) {
    Session *session = mTransferSessions.takeFirst();
    DC *authorizedDc = session->dc();
    authorizedDc->setExpires(expires);
    authorizedDc->setState(DC::userSignedIn);
    session->release();
    mApi->setMainSession(mWorkingDcSession);
    if (mTransferSessions.isEmpty()) {
        Settings::getInstance()->setDcsList(mDcs.values());
        Settings::getInstance()->writeAuthFile();
        Q_EMIT authTransferCompleted();
//        delete session;
    } else {
        mApi->authExportAuthorization(mTransferSessions.first()->dc()->id());
//        delete session;
    }
}

void DcProvider::logOut() {
    Settings *settings = Settings::getInstance();
    settings->setDcsList(mDcs.values());
    settings->removeAuthFile();
}
