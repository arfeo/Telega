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

#include "settings.h"
#include "utils.h"
#include "constants.h"
#include "dc.h"

#include <QDir>
#include <QSettings>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>

Settings::Settings() :
        m_pubKey(0)
{
    // ..
}

Settings::~Settings()
{
    if (m_pubKey) {
        delete m_pubKey;
    }
}

Settings* Settings::getInstance()
{
    static Settings *instance = new Settings;
    return instance;
}

bool Settings::loadSettings()
{
    qDebug() << "Entered Load Settings";
    m_pubKey = Utils::rsaLoadPublicKey();
    m_pkFingerprint = Utils::computeRSAFingerprint(m_pubKey);
    readAuthFile();
//    readSecretFile();
    return true;
}

void Settings::writeAuthFile()
{
    qDebug() << "---------------------";
    qDebug() << "Write Mode : Started" ;
    QSettings settings("data/auth", QSettings::IniFormat);
    settings.beginGroup(ST_PRODUCTION);
    settings.setValue(ST_WORKING_DC_NUM, m_workingDcNum);
    settings.setValue(ST_OUR_ID, m_ourId);
    settings.beginWriteArray(ST_DCS_ARRAY);
    for (qint32 i = 0; i < m_dcsList.length(); i++) {
        //write dc
        settings.setArrayIndex(i);
        settings.setValue(ST_DC_NUM, m_dcsList[i]->id());
        settings.setValue(ST_HOST, m_dcsList[i]->host());
        settings.setValue(ST_PORT, m_dcsList[i]->port());
        settings.setValue(ST_DC_STATE, m_dcsList[i]->state());
//        qDebug() <<  "Auth Key is been Created for host : " << m_dcsList[i]->host() << m_dcsList[i]->authKeyId() << m_dcsList[i]->authKey();
        if (m_dcsList[i]->authKeyId()) {
            settings.setValue(ST_AUTH_KEY_ID, m_dcsList[i]->authKeyId());
            QByteArray baToSave(m_dcsList[i]->authKey(), SHARED_KEY_LENGTH);
            settings.setValue(ST_AUTH_KEY, baToSave.toBase64());
            qDebug() <<  "Auth Key is been Created for host : " << m_dcsList[i]->host() << m_dcsList[i]->state() ;
        }
        settings.setValue(ST_SERVER_SALT, m_dcsList[i]->serverSalt());
        settings.setValue(ST_EXPIRES, m_dcsList[i]->expires());
    }
    settings.endArray();
    settings.endGroup();
    qDebug() << "Write Mode : exited";
    qDebug() << "---------------------";
}

void Settings::readAuthFile()
{
    qDebug() << " ---------------- " ;
    qDebug() << "Read Mode : Started" ;
    QSettings settings("data/auth", QSettings::IniFormat);
    settings.beginGroup(ST_PRODUCTION);
    m_workingDcNum = settings.value(ST_WORKING_DC_NUM, 1).toInt();
    m_ourId = settings.value(ST_OUR_ID).toInt();
     qDebug() << "workingDcNum:" << m_workingDcNum;
    // read all dcs
    m_dcsList.clear();
    qint32 n = settings.beginReadArray(ST_DCS_ARRAY);
    for (qint32 i = 0; i < n; i++) {
        settings.setArrayIndex(i);
        qint32 dcNum = settings.value(ST_DC_NUM).toInt();
        DC* dc = new DC(dcNum);
        dc->setHost(settings.value(ST_HOST).toString());
        dc->setPort(settings.value(ST_PORT, 0).toInt());
        dc->setState((DC::DcState)settings.value(ST_DC_STATE, DC::init).toInt());
        dc->setAuthKeyId(settings.value(ST_AUTH_KEY_ID, 0).toLongLong());
        if (dc->state() >= DC::authKeyCreated) {
            QByteArray readedBa = QByteArray::fromBase64(settings.value(ST_AUTH_KEY).toByteArray());
            memcpy(dc->authKey(), readedBa.data(), SHARED_KEY_LENGTH);
            qDebug() << "DC | id:" << dc->id() << ", state:" << dc->state() << ", host:" << dc->host() << ", port:" << dc->port() << ", expires:" << dc->expires() << ", authKeyId:" << dc->authKeyId();
        }
        else{
            qDebug() << "DC | id:" << dc->id() << ", state:" << dc->state() << ", host:" << dc->host() << "Auth key is not created" ;
        }
        dc->setServerSalt(settings.value(ST_SERVER_SALT, 0).toLongLong());
        dc->setExpires(settings.value(ST_EXPIRES).toInt());
        m_dcsList.insert(dcNum, dc);
    }
    settings.endArray();
    settings.endGroup();
    qDebug() << "Read Mode : ended " ;
    qDebug() << " ---------------- " ;
}

bool Settings::removeAuthFile()
{
    QFile authFile("data/auth");
    if (authFile.exists()) {
        if (authFile.remove()) {
            // qCDebug(TG_INIT_SETTINGS) << "Auth file has been reset";
            return true;
        } else {
            // qCDebug(TG_INIT_SETTINGS) << "An error has happened when trying to remove auth file";
        }
    }
    return false;
}
//
//void Settings::readSecretFile()
//{
//    QSettings settings(m_secretChatFilename, QSettings::IniFormat);
//    mVersion = settings.value(ST_VERSION, 0).toInt();
//    mG = settings.value(ST_G, 0).toInt();
//    mP = QByteArray::fromBase64(settings.value(ST_P).toByteArray());
//
//    // qCDebug(TG_INIT_SETTINGS) << "version:" << mVersion;
//    // qCDebug(TG_INIT_SETTINGS) << "g:" << mG;
//    // qCDebug(TG_INIT_SETTINGS) << "p:" << mP.toBase64();
//
//    mSecretChats.clear();
//    qint32 n = settings.beginReadArray(ST_SECRET_CHATS_ARRAY);
//    for (qint32 i = 0; i < n; i++) {
//        settings.setArrayIndex(i);
//        SecretChat *secretChat = new SecretChat();
//        secretChat->setState(SecretChat::Accepted); // only accepted chats are saved.
//        secretChat->setChatId(settings.value(ST_CHAT_ID, 0).toInt());
//        secretChat->setAccessHash(settings.value(ST_ACCESS_HASH, 0).toLongLong());
//        secretChat->setAdminId(settings.value(ST_ADMIN_ID, 0).toInt());
//        secretChat->setParticipantId(settings.value(ST_PARTICIPANT_ID, 0).toInt());
//        secretChat->setDate(settings.value(ST_DATE, 0).toInt());
//        QByteArray base64Sk = settings.value(ST_SHARED_KEY).toByteArray();
//        QByteArray parsedSk = QByteArray::fromBase64(base64Sk);
//        memcpy(secretChat->sharedKey(), parsedSk.data(), SHARED_KEY_LENGTH);
//        secretChat->setLayer(settings.value(ST_LAYER, 0).toInt());
//        secretChat->setInSeqNo(settings.value(ST_IN_SEQ_NO, 0).toInt());
//        secretChat->setOutSeqNo(settings.value(ST_OUT_SEQ_NO, 0).toInt());
//        secretChat->setLastInSeqNo(settings.value(ST_LAST_IN_SEQ_NO, 0).toInt());
//        mSecretChats.append(secretChat);
//
//        // qCDebug(TG_INIT_SETTINGS) << "SecretChat:\nid:" << secretChat->chatId()
//                                  << "\nadminId:" << secretChat->adminId()
//                                  << "\nparticipantId:" << secretChat->participantId()
//                                  << "\ndate:" << secretChat->date()
//                                  << "\nsharedKey (keyFingerprint):" << secretChat->keyFingerprint()
//                                  << "\nlayer:" << secretChat->layer()
//                                  << "\ninSeqNo:" << secretChat->inSeqNo()
//                                  << "\noutSeqNo:" << secretChat->outSeqNo()
//                                  << "\nlastInSeqNo:" << secretChat->lastInSeqNo();
//    }
//    settings.endArray();
//}
//
//
//void Settings::writeSecretFile()
//{
//    QSettings settings(m_secretChatFilename, QSettings::IniFormat);
//    settings.clear();
//    settings.setValue(ST_VERSION, mVersion);
//    settings.setValue(ST_G, mG);
//    settings.setValue(ST_P, mP.toBase64());
//    settings.beginWriteArray(ST_SECRET_CHATS_ARRAY);
//    for (qint32 i = 0; i < mSecretChats.length(); i++) {
//        SecretChat *secretChat = mSecretChats[i];
//        settings.setArrayIndex(i);
//        settings.setValue(ST_CHAT_ID, secretChat->chatId());
//        settings.setValue(ST_ACCESS_HASH, secretChat->accessHash());
//        settings.setValue(ST_ADMIN_ID, secretChat->adminId());
//        settings.setValue(ST_PARTICIPANT_ID, secretChat->participantId());
//        settings.setValue(ST_DATE, secretChat->date());
//        QByteArray sharedKeyToSave((char *)secretChat->sharedKey(), SHARED_KEY_LENGTH);
//        settings.setValue(ST_SHARED_KEY, sharedKeyToSave.toBase64());
//        settings.setValue(ST_LAYER, secretChat->layer());
//        settings.setValue(ST_IN_SEQ_NO, secretChat->inSeqNo());
//        settings.setValue(ST_OUT_SEQ_NO, secretChat->outSeqNo());
//        settings.setValue(ST_LAST_IN_SEQ_NO, secretChat->lastInSeqNo());
//    }
//    settings.endArray();
//}
//
