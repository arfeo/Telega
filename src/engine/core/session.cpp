/*
 * Copyright 2013 Vitaly Valtman
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

#include "session.h"
#include "openssl/rand.h"
#include "openssl/sha.h"
#include <QDateTime>
#include "tlvalues.h"
#include "../../applicationui.hpp"


qint64 Session::m_clientLastMsgId = 0;

Session::Session(DC *dc, QObject *parent) :
    Connection(dc->host(), dc->port(), parent),
    m_sessionId(0),
    m_serverSalt(0),
    m_timeSyncFactor(0),
    //m_clientLastMsgId(0),
    m_seqNo(0),
    m_dc(dc),
    m_initConnectionNeeded(true) {
    // ensure dc has, at least, the shared key created
//    qDebug() << dc->state() <<  DC::authKeyCreated;
//    Q_ASSERT(dc->state() >= DC::authKeyCreated);
    // copy calculated values for timeSyncFactor and serverSalt when created shared key.
    // This copy is needed because we could have several sessions against same dc with different values
    m_timeSyncFactor = m_dc->timeSyncFactor();
    m_serverSalt = m_dc->serverSalt();
    // create session id
    RAND_pseudo_bytes((uchar *) &m_sessionId, 8);
    // qCDebug(TG_NET_SESSION) << "created session with id" << QString::number(m_sessionId, 16);
}

Session::~Session() {
    // qCDebug(TG_NET_SESSION) << "destroyed session with id" << QString::number(m_sessionId, 16);
}

void Session::close() {
    if (this->state() != QAbstractSocket::UnconnectedState) {
        connect(this, SIGNAL(disconnected()), SLOT(onDisconnected()));
        this->disconnectFromHost();
    } else {
        Q_EMIT sessionClosed(m_sessionId);
    }
}

void Session::onDisconnected() {
//    isonline = false;
    Q_EMIT sessionClosed(m_sessionId);
}

void Session::release() {
    Q_EMIT sessionReleased(m_sessionId);
}

DC *Session::dc() {
    return m_dc;
}

qint64 Session::generateNextMsgId() {
    qint64 nextId = (qint64) ((QDateTime::currentDateTime().toTime_t() - m_timeSyncFactor) * (1LL << 32)) & -4;
    if (nextId <= m_clientLastMsgId) {
        nextId = m_clientLastMsgId += 4;
    } else {
        m_clientLastMsgId = nextId;
    }
    return nextId;
}

void Session::processConnected() {
//    isonline = false;
    Q_EMIT sessionReady(m_dc);
}

void Session::processRpcAnswer(QByteArray response) {

    qint32 op;
    peekIn(&op, 4);
    qint32 len = response.length();
//    qDebug() << "*-----------------------------------------*";
//     qDebug() << "connection #" << socketDescriptor() << "received rpc answer" << op << "with" << len << "content bytes by session" << QString::number(m_sessionId, 16);

    InboundPkt p(response.data(), len);

    if (op < 0 && op >= -999) {
        // qCDebug(TG_NET_SESSION) << "server error" << op;
    } else {
        processRpcMessage(p);
    }
}

void Session::processRpcMessage(InboundPkt &inboundPkt) {

    EncryptedMsg *enc = (EncryptedMsg *)inboundPkt.buffer();
    qint32 len = inboundPkt.length();

    const qint32 MINSZ = offsetof (EncryptedMsg, message);
    const qint32 UNENCSZ = offsetof (EncryptedMsg, serverSalt);
//    qDebug() << "processRpcMessage(), len=" << len;

    Q_ASSERT(len >= MINSZ && (len & 15) == (UNENCSZ & 15));
    Q_ASSERT(m_dc->authKeyId());
    mAsserter.check(enc->authKeyId == m_dc->authKeyId());
    //msg_key is used to compute AES key and to decrypt the received message
    CryptoUtils::getInstance()->initAESAuth(m_dc->authKey() + 8, enc->msgKey, AES_DECRYPT);
    qint32 l = CryptoUtils::getInstance()->padAESDecrypt((char *)&enc->serverSalt, len - UNENCSZ, (char *)&enc->serverSalt, len - UNENCSZ);
    Q_UNUSED(l);
    Q_ASSERT(l == len - UNENCSZ);
    Q_ASSERT(!(enc->msgLen & 3) && enc->msgLen > 0 && enc->msgLen <= len - MINSZ && len - MINSZ - enc->msgLen <= 12);
//    qDebug() << "Encrypted Message" << QString::number(*enc->message, 16) ;

    //check msg_key is indeed equal to SHA1 of the plaintext obtained after decription (without final padding bytes).
    static uchar sha1Buffer[20];
    SHA1((uchar *)&enc->serverSalt, enc->msgLen + (MINSZ - UNENCSZ), sha1Buffer);
    Q_ASSERT(!memcmp (&enc->msgKey, sha1Buffer + 4, 16));

    if (m_dc->serverSalt() != enc->serverSalt) {
        m_dc->setServerSalt(enc->serverSalt);
    }

    // check time synchronization
    qint32 serverTime = enc->msgId >> 32LL;
    qint32 clientTime = QDateTime::currentDateTime().toTime_t() - m_dc->timeSyncFactor();
    if (clientTime <= serverTime - 30 || clientTime >= serverTime + 300) {
        // qCDebug(TG_NET_SESSION) << "salt =" << enc->serverSalt << ", sessionId =" << QString::number(enc->sessionId, 16) << ", msgId =" << QString::number(enc->msgId, 16) << ", seqNo =" << enc->seqNo << ", serverTime =" << serverTime << ", clientTime =" << clientTime;
        QString alert("discarding received message due to client<->server dates too large difference - ");
        if (clientTime <= serverTime -30) {
            alert.append("the message has a date 30 seconds late in time than current date! :O");
        } else {
            alert.append("the message was sent at least 300 seconds ago");
        }
        qWarning() << alert;
        inboundPkt.setInPtr(inboundPkt.inEnd());
        return;
    }
    Q_ASSERT(clientTime > serverTime - 30 && clientTime < serverTime + 300);


    inboundPkt.setInPtr(enc->message);
    inboundPkt.setInEnd(inboundPkt.inPtr() + (enc->msgLen / 4));

//    qDebug() << "received message id" << QString::number(enc->msgId, 16);

    Q_ASSERT(l >= (MINSZ - UNENCSZ) + 8);

    mAsserter.check(m_sessionId == enc->sessionId);
//    qDebug() << inboundPkt.buffer();
    rpcExecuteAnswer(inboundPkt, enc->msgId);
    mAsserter.check(inboundPkt.inPtr() == inboundPkt.inEnd());
}

void Session::rpcExecuteAnswer(InboundPkt &inboundPkt, qint64 msgId) {
    qint32 op = inboundPkt.prefetchInt();
//     qDebug() << "rpcExecuteAnswer(), op =" << QString::number(op, 16);
    switch (op) {
    case TL_MsgContainer:
//        qDebug() << "Msg Container";
        workContainer(inboundPkt, msgId);
        return;
    case TL_NewSessionCreated:
        qDebug() << "Msg session created";
        workNewSessionCreated(inboundPkt, msgId);
        return;
    case TL_MsgsAck:
//        qDebug() << "Msg ack";
        workMsgsAck(inboundPkt, msgId);
        return;
    case TL_RpcResult:
//        qDebug() << "Msg RPC result";
        workRpcResult(inboundPkt, msgId);
        return;
    case TL_UpdateShort:
//        qDebug() << "Msg update short";
        workUpdateShort(inboundPkt, msgId);
        return;
    case TL_UpdatesCombined:
//        qDebug() << "Msg updates combined";
        workUpdatesCombined(inboundPkt, msgId);
    case TL_Updates:
        qDebug() << "Msg updates";
        workUpdates(inboundPkt, msgId);
        return;
    case TL_UpdateShortMessage:
//        qDebug() << "Msg updateshortmessage";
        workUpdateShortMessage(inboundPkt, msgId);
        return;
    case TL_UpdateShortChatMessage:
        qDebug() << "Msg udpate short chat msg";
        workUpdateShortChatMessage(inboundPkt, msgId);
        return;
    case TL_GZipPacked:
//        qDebug() << "Msg GZip packed";
        workPacked(inboundPkt, msgId);
        return;
    case TL_BadServerSalt:
//        qDebug() << "Msg bad server salt";
        workBadServerSalt(inboundPkt, msgId);
        return;
    case TL_Pong:
        qDebug() << "Msg pong";
        workPong(inboundPkt, msgId);
        return;
    case TL_MsgDetailedInfo:
        qDebug() << "Msg deatialed info";
        workDetailedInfo(inboundPkt, msgId);
        return;
    case TL_MsgNewDetailedInfo:
        qDebug() << "Msg new detailed info";
        workNewDetailedInfo(inboundPkt, msgId);
        return;
    case TL_UpdatesTooLong:
//        qDebug() << "Msg updates too long";
        workUpdatesTooLong(inboundPkt, msgId);
        return;
    case TL_BadMsgNotification:
//        qDebug() << "Msg bad msg notification";
        workBadMsgNotification(inboundPkt, msgId);
        return;
    }
    qWarning() << "Unknown rpc response message";
    inboundPkt.setInPtr(inboundPkt.inEnd());
}

void Session::workContainer (InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workContainer: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == TL_MsgContainer);
    qint32 n = inboundPkt.fetchInt();
    for (qint32 i = 0; i < n; i++) { // message
        qint64 id = inboundPkt.fetchLong (); // msg_id
        inboundPkt.fetchInt (); // seq_no
        qint32 bytes = inboundPkt.fetchInt ();
        qint32 *t = inboundPkt.inEnd();
        inboundPkt.setInEnd(inboundPkt.inPtr() + (bytes / 4));
        rpcExecuteAnswer(inboundPkt, id);
        Q_ASSERT (inboundPkt.inPtr() == inboundPkt.inEnd());
        inboundPkt.setInEnd(t);
    }
}

void Session::workNewSessionCreated(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workNewSessionCreated: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_NewSessionCreated);
    inboundPkt.fetchLong(); // first_msg_id; //XXX set is as m_clientLastMsgId??
    inboundPkt.fetchLong (); // unique_id
    m_dc->setServerSalt(inboundPkt.fetchLong()); // server_salt
//    qDebug() << m_dc->serverSalt();
}

void Session::workMsgsAck(InboundPkt &inboundPkt, qint64 msgId) {
//     qDebug() << "workMsgsAck: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_MsgsAck);
    mAsserter.check(inboundPkt.fetchInt () == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    for (qint32 i = 0; i < n; i++) {
        qint64 id = inboundPkt.fetchLong ();
        Query *q = m_pendingQueries.value(id);
        Q_ASSERT(q);
        q->setAcked(true);
    }
}

void Session::workRpcResult(InboundPkt &inboundPkt, qint64 msgId) {

    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_RpcResult);
    qint64 id = inboundPkt.fetchLong();
    qint32 op = inboundPkt.prefetchInt();
    if (op == (qint32)TL_RpcError) {
        queryOnError(inboundPkt, id);
    } else {
        queryOnResult(inboundPkt, id);
    }
}

void Session::workUpdateShort(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workUpdateShort: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_UpdateShort);
    Update update = inboundPkt.fetchUpdate();
    qint32 date = inboundPkt.fetchInt();
    Q_EMIT updateShort(update, date);
}

void Session::workUpdatesCombined(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workUpdatesCombined: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_UpdatesCombined);
    //updates
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Update> updates;
    for (qint32 i = 0; i < n; i++) {
        updates.append(inboundPkt.fetchUpdate());
    }
    //users
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //chats
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    qint32 date = inboundPkt.fetchInt();
    qint32 seqStart = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    Q_EMIT updatesCombined(updates, users, chats, date, seqStart, seq);
}

void Session::workUpdates(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workUpdates: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Updates);
    //updates
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Vector);
    qint32 n = inboundPkt.fetchInt();
    QList<Update> updatesList;
    for (qint32 i = 0; i < n; i++) {
        updatesList.append(inboundPkt.fetchUpdate());
    }
    //users
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<User> users;
    for (qint32 i = 0; i < n; i++) {
        users.append(inboundPkt.fetchUser());
    }
    //chats
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_Vector);
    n = inboundPkt.fetchInt();
    QList<Chat> chats;
    for (qint32 i = 0; i < n; i++) {
        chats.append(inboundPkt.fetchChat());
    }
    qint32 date = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    Q_EMIT updates(updatesList, users, chats, date, seq);
}

void Session::workUpdateShortMessage(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workUpdateShortMessage: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_UpdateShortMessage);
    qint32 id = inboundPkt.fetchInt();
    qint32 fromId = inboundPkt.fetchInt();
    QString message = inboundPkt.fetchQString();
    qint32 pts = inboundPkt.fetchInt();
    qint32 date = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    Q_EMIT updateShortMessage(id, fromId, message, pts, date, seq);
}

void Session::workUpdateShortChatMessage(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workUpdateShortChatMessage: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_UpdateShortChatMessage);
    qint32 id = inboundPkt.fetchInt();
    qint32 fromId = inboundPkt.fetchInt();
    qint32 chatId = inboundPkt.fetchInt();
    QString message = inboundPkt.fetchQString();
    qint32 pts = inboundPkt.fetchInt();
    qint32 date = inboundPkt.fetchInt();
    qint32 seq = inboundPkt.fetchInt();
    Q_EMIT updateShortChatMessage(id, fromId, chatId, message, pts, date, seq);
}

void Session::workPacked(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workPacked: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_GZipPacked);
    static qint32 buf[MAX_PACKED_SIZE >> 2];
    qint32 l = inboundPkt.prefetchStrlen();
    char *s = inboundPkt.fetchStr(l);

    qint32 totalOut = Utils::tinflate(s, l, buf, MAX_PACKED_SIZE);
    qint32 *inPtr = inboundPkt.inPtr();
    qint32 *inEnd = inboundPkt.inEnd();
    inboundPkt.setInPtr(buf);
    inboundPkt.setInEnd(inboundPkt.inPtr() + totalOut / 4);
    // qCDebug(TG_NET_SESSION) << "Unzipped data";
    rpcExecuteAnswer(inboundPkt, msgId);
    inboundPkt.setInPtr(inPtr); //TODO Not sure about this operations of setting inPtr and inEnd after execute answer completion
    inboundPkt.setInEnd(inEnd);
}

void Session::workBadServerSalt(InboundPkt &inboundPkt, qint64 msgId) {
    qDebug() << "workBadServerSalt: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_BadServerSalt);
    qint64 badMsgId = inboundPkt.fetchLong();
    inboundPkt.fetchInt(); // badMsgSeqNo
    inboundPkt.fetchInt(); // errorCode
    m_dc->setServerSalt(inboundPkt.fetchLong()); // new server_salt
    // resend the last query
    Query *q = m_pendingQueries.value(badMsgId);
    qDebug() << "Creating Re-Query";
    resendQuery( *q );
    qDebug() << "Sent Re-Query";
}

void Session::workPong(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workPong: msgId =" << QString::number(msgId, 16);
    mAsserter.check (inboundPkt.fetchInt() == (qint32)TL_Pong);
    inboundPkt.fetchLong(); // msg_id
    inboundPkt.fetchLong(); // ping_id
}

void Session::workDetailedInfo(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workDetailedInfo: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_MsgDetailedInfo);
    inboundPkt.fetchLong(); // msg_id
    inboundPkt.fetchLong(); // answer_msg_id
    inboundPkt.fetchInt(); // bytes
    inboundPkt.fetchInt(); // status
}

void Session::workNewDetailedInfo(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workNewDetailedInfo: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_MsgNewDetailedInfo);
    inboundPkt.fetchLong(); // answer_msg_id
    inboundPkt.fetchInt(); // bytes
    inboundPkt.fetchInt(); // status
}

void Session::workUpdatesTooLong(InboundPkt &inboundPkt, qint64 msgId) {
    // qCDebug(TG_NET_SESSION) << "workUpdatesTooLong: msgId =" << QString::number(msgId, 16);
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_UpdatesTooLong);
    Q_EMIT updatesTooLong();
}

void Session::workBadMsgNotification(InboundPkt &inboundPkt, qint64 msgId) {
    mAsserter.check(inboundPkt.fetchInt() == (qint32)TL_BadMsgNotification);
    qint64 badMsgId = inboundPkt.fetchLong();
    qint32 badMsgSeqNo = inboundPkt.fetchInt();
    qint32 errorCode = inboundPkt.fetchInt();
    // qCWarning(TG_NET_SESSION) << "workBadMsgNotification: msgId =" << badMsgId <<
//                  ", seqNo =" << badMsgSeqNo << ", errorCode =" << errorCode;
    switch (errorCode) {
    case 16:
    case 17:
        // update time sync factor
        qint32 serverTime = msgId >> 32LL;
        m_dc->setTimeSyncFactor(QDateTime::currentDateTime().toTime_t() - serverTime);
        // read removing from pending queries, recompose and send the last query
        Query *q = m_pendingQueries.take(badMsgId);
        recomposeAndSendQuery(q);
        break;
    }
}

Session::EncryptedMsg *Session::initEncryptedMessage(qint32 useful) {
    EncryptedMsg *encMsg = new EncryptedMsg;
    Q_ASSERT(m_dc->authKeyId());
    encMsg->authKeyId = m_dc->authKeyId();
    Q_ASSERT(m_dc->serverSalt());
    encMsg->serverSalt = m_dc->serverSalt();
    Q_ASSERT(m_sessionId);
    encMsg->sessionId = m_sessionId;
    encMsg->msgId = generateNextMsgId();
    encMsg->seqNo = m_seqNo;
    if (useful) {
        encMsg->seqNo |= 1;
    }
    m_seqNo += 2;
    return encMsg;
}

qint32 Session::aesEncryptMessage (EncryptedMsg *encMsg) {
    uchar sha1Buffer[20];
    const qint32 MINSZ = offsetof (EncryptedMsg, message);
    const qint32 UNENCSZ = offsetof (EncryptedMsg, serverSalt);
    qint32 encLen = (MINSZ - UNENCSZ) + encMsg->msgLen;
    Q_ASSERT (encMsg->msgLen >= 0 && encMsg->msgLen <= MAX_MESSAGE_INTS * 4 - 16 && !(encMsg->msgLen & 3));
    SHA1 ((uchar *) &encMsg->serverSalt, encLen, sha1Buffer);

    memcpy (encMsg->msgKey, sha1Buffer + 4, 16);
    CryptoUtils::getInstance()->initAESAuth(m_dc->authKey(), encMsg->msgKey, AES_ENCRYPT);
    return CryptoUtils::getInstance()->padAESEncrypt((char *) &encMsg->serverSalt, encLen, (char *) &encMsg->serverSalt, MAX_MESSAGE_INTS * 4 + (MINSZ - UNENCSZ));
}

qint64 Session::encryptSendMessage(qint32 *msg, qint32 msgInts, qint32 useful) {
    const qint32 UNENCSZ = offsetof (EncryptedMsg, serverSalt);
    if (msgInts <= 0 || msgInts > MAX_MESSAGE_INTS - 4) {
      return -1;
    }
    EncryptedMsg *encMsg = initEncryptedMessage(useful);
    if (msg) {
      memcpy (encMsg->message, msg, msgInts * 4);
      encMsg->msgLen = msgInts * 4;
    } else if ((encMsg->msgLen & 0x80000003) || encMsg->msgLen > MAX_MESSAGE_INTS * 4 - 16) {
      delete encMsg;
      return -1;
    }

    qint32 l = aesEncryptMessage(encMsg);
    Q_ASSERT(l > 0);

    rpcSendMessage(encMsg, l + UNENCSZ);
    delete encMsg;
    return m_clientLastMsgId;
}

void Session::rpcSendMessage(void *data, qint32 len) {
    qint32 written;
    Q_UNUSED(written);

    mAsserter.check(len > 0 && !(len & 0xfc000003));
    qint32 totalLen = len >> 2;

    if (totalLen < 0x7f) {
        written = writeOut(&totalLen, 1);
        Q_ASSERT(written == 1);
    } else {
        totalLen = (totalLen << 8) | 0x7f;
        written = writeOut(&totalLen, 4);
        Q_ASSERT(written == 4);
    }

    written = writeOut(data, len);
    Q_ASSERT(written == len);
}


//### query management
qint64 Session::sendQuery(OutboundPkt &outboundPkt, QueryMethods *methods, QVariant extra) {
//    if(isonline == true){
    Q_ASSERT (m_sessionId);
    Q_ASSERT (m_dc->authKeyId());
    qint32 *data = outboundPkt.buffer();
    qint32 ints = outboundPkt.length();
    qDebug() << "Sending query of size" << 4 * ints << "to DC" << m_dc->id() << "at" << peerName() << ":" << peerPort() << "by session" << QString::number(m_sessionId, 16);

    Query *q = new Query(this);
    q->setData(data, ints);
    q->setMsgId(encryptSendMessage(data, ints, 1));
    q->setSeqNo(m_seqNo - 1);
    q->setMethods(methods);
    q->setAcked(false);
    q->setExtra(extra);
    if (Settings::getInstance()->resendQueries()) {
//        connect(q, SIGNAL(timeout(Query*)), this, SLOT(resendQuery(Query*)), Qt::UniqueConnection);
        q->startTimer(QUERY_TIMEOUT);
    }
    m_pendingQueries.insert(q->msgId(), q);
    return q->msgId();
//    }
//    else{
//        return 0;
//    }
}

void Session::recomposeAndSendQuery(Query *q) {
    Q_ASSERT(q);
    // qCDebug(TG_NET_SESSION) << "Resending query with previous msgId" << QString::number(q->msgId(), 16);
    q->setMsgId(encryptSendMessage((qint32 *)q->data(), q->dataLength(), 1));
    q->setSeqNo(m_seqNo - 1);
    // qCDebug(TG_NET_SESSION) << "new msgId is" << QString::number(q->msgId(), 16);
    q->setAcked(false);
    m_pendingQueries.insert(q->msgId(), q);
}

void Session::resendQuery(Query &q) {
    qDebug() << "Resend query";
//    Q_ASSERT(q);
    //avoid resending if resend numbers is less than zero
//    if (q->decreaseResends() < 0) {
//        // qCDebug(TG_NET_SESSION) << "Max resend numbers reached for query with msgId" << QString::number(q->msgId(), 16) << ",query discarded";
//        m_pendingQueries.remove(q->msgId());
//        delete q;
//    } else {
        qDebug() << "Resending query with msgId" << QString::number(q.msgId(), 16);
        OutboundPkt *p = new OutboundPkt() ;
        p->appendInt(TL_MsgContainer);
        p->appendInt(1);
        p->appendLong(q.msgId());
        p->appendInt(q.seqNo());
        p->appendInt(4 * q.dataLength());
        p->appendInts((qint32 *)q.data(), q.dataLength());
//        qDebug() << p->buffer();
        encryptSendMessage(p->buffer(), p->length(), 0);
        delete p;
//    }
}

void Session::queryOnResult(InboundPkt &inboundPkt, qint64 msgId) {
//    qDebug() << "result for query" << QString::number(msgId, 16) << inboundPkt.prefetchInt() ;
    qint32 op = inboundPkt.prefetchInt();
    qint32 *inPtr = 0;
    qint32 *inEnd = 0;
    if (op == (qint32)TL_GZipPacked) {
        inboundPkt.fetchInt();
        qint32 l = inboundPkt.prefetchStrlen();
        char *s = inboundPkt.fetchStr(l);
        static qint32 packedBuffer[MAX_PACKED_SIZE / 4];
        qint32 totalOut = Utils::tinflate (s, l, packedBuffer, MAX_PACKED_SIZE);
        inPtr = inboundPkt.inPtr();
        inEnd = inboundPkt.inEnd();
        inboundPkt.setInPtr(packedBuffer);
        inboundPkt.setInEnd(inboundPkt.inPtr() + totalOut / 4);
        // qCDebug(TG_NET_SESSION) << "unzipped data";
    }

    Query *q = m_pendingQueries.take(msgId);
    if (!q) {
        inboundPkt.setInPtr(inboundPkt.inEnd());
    } else {
        q->setAcked(true);
        Q_EMIT resultReceived(q, inboundPkt);
    }
    if (inPtr) {
        inboundPkt.setInPtr(inPtr);
        inboundPkt.setInEnd(inEnd);
    }
}

void Session::queryOnError(InboundPkt &inboundPkt, qint64 msgId) {
    mAsserter.check(inboundPkt.fetchInt() == TL_RpcError);
    qint32 errorCode = inboundPkt.fetchInt();
    QString errorText = inboundPkt.fetchQString();
    Query *q = m_pendingQueries.take(msgId);

    if (!q) {
         qWarning() << "No such query";
    } else {
        q->setAcked(true);
        Q_EMIT errorReceived(q, errorCode, errorText);
        qDebug() << errorCode << errorText;
        switch (errorCode) {
            case 401:{
                        if(errorText == "SESSION_REVOKED"){
//                            ApplicationUI().logout();
                        }
                        break;
                    }
            default:
                break;
        }
    }
}
