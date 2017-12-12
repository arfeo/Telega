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

#include "dcauth.h"

#include "openssl/sha.h"
#include "utils.h"
#include "outboundpkt.h"
#include "tlvalues.h"
#include "asserter.h"
#include <QDateTime>
#include "inboundpkt.h"
#include <cmath>


DCAuth::DCAuth(DC *dc, QObject *parent) : Connection(dc->host(), dc->port(), parent) , m_dc(dc) {
}

DCAuth::~DCAuth() {
}

DC *DCAuth::dc() {
    return m_dc;
}

void DCAuth::createAuthKey() {
    if (m_dc) {
        if (0 < DC::authKeyCreated) {
//            qDebug () << "calling to host";
            connectToServer();
        } else {
            // in this case, the key is already created, so dc is ready
            Q_EMIT dcReady(m_dc);
        }
    } else {
//        qCWarning(TG_AUTH_DCAUTH) << "Error! provided DC for creating auth key is null";
    }
}

void DCAuth::processConnected() {
    // depending on the state init shared key creatug()ion or set the DC as ready
//    qDebug()<<"connection state"<<m_dc->state();
//    qDebug() << DC::authKeyCreated<<DC::init << DC::userSignedIn;
    switch (m_dc->state()) {
    case DC::init:
        sendReqPQPacket();
        break;
    case DC::authKeyCreated:
    case DC::userSignedIn:
        Q_EMIT dcReady(m_dc);
        break;
    default:
        qCritical() << "Invalid state:" << m_dc->state();
        Q_EMIT fatalError();
    }
}



void DCAuth::processResPQAnswer(const InboundPkt &inboundPkt) {
    qDebug() << "Responce for PQ request";
    qDebug() << "Processing PQ";
    qint32 len = inboundPkt.length();
    const char* buffer = inboundPkt.buffer();
    qint32 i;

//    // qCDebug(TG_AUTH_DCAUTH) << "processResPQAnswer(), len=" << len;

    mAsserter.check(len >= 76);
    mAsserter.check(!*(qint64 *) buffer);
    mAsserter.check(*(qint32 *) (buffer + 16) == len - 20);
    mAsserter.check(!(len & 3));
    mAsserter.check(*(qint32 *) (buffer + 20) == TL_ResPQ);
    mAsserter.check(!memcmp(buffer +24, m_nonce, 16));
    memcpy(m_serverNonce, buffer + 40, 16);
    const char *from = buffer + 56;
    qint32 clen = *from++;
    mAsserter.check(clen <= 8);
    quint64 what = 0;
    for (i = 0; i < clen; i++) {
        what = (what << 8) + (uchar)*from++;
    }

    while (((quint64)from) & 3) ++from;

    uint p1 = 0, p2 = 0;
    // qCDebug(TG_AUTH_DCAUTH) << what << "received";

    qint32 it = 0;
    quint64 g = 0;
    for (i = 0; i < 3 || it < 10; i++) {
        qint32 q = ((lrand48() & 16) + 17) % what;
        quint64 x = (qint64)lrand48 () % (what - 1) + 1, y = x;
        qint32 lim = 1 << (i + 18);
        qint32 j;
        for (j = 1; j < lim; j++) {
            ++it;
            quint64 a = x, b = x, c = q;
            while (b) {
                if (b & 1) {
                    c += a;
                    if (c >= what) {
                        c -= what;
                    }
                }
                a += a;
                if (a >= what) {
                    a -= what;
                }
                b >>= 1;
            }
            x = c;
            quint64 z = x < y ? what + x - y : x - y;
            g = Utils::gcd (z, what);
//            qDebug() << z << what;
            if (g != 1) {
                break;
            }
            if (!(j & (j - 1))) {
                y = x;
            }
        }
        if (g > 1 && g < what) break;
    }

    mAsserter.check(g > 1 && g < what);
    p1 = g;
    p2 = what / g;
    if (p1 > p2) {
        uint t = p1; p1 = p2; p2 = t;
    }

    qDebug() << " Found p=" << p1 << ",q=" << p2 << ", " << it << "iterations";

    mAsserter.check(*(qint32 *) (from) == TL_Vector);
    qint32 fingerprints_num = *(qint32 *)(from + 4);
    mAsserter.check(fingerprints_num >= 1 && fingerprints_num <= 64 && len == fingerprints_num * 8 + 8 + (from - buffer));
    qint64 *fingerprints = (qint64 *) (from + 8);
//    for (i = 0; i < fingerprints_num; i++) {
//        if (fingerprints[i] == Settings::getInstance()->pkFingerprint()) {
//            // qCDebug(TG_AUTH_DCAUTH) << "found our public key at position" << i;
//            break;
//        }
//    }
////    qWarning()  << QString::number(Settings::getInstance()->pkFingerprint(), 16) << "expected)";
//    if (i == fingerprints_num) {
//        exit (2);
//    }

    // create inner part (P_Q_inner_data)
    OutboundPkt *p = new OutboundPkt();
    p->forwardPtr(5);
    p->appendInt(TL_PQInnerData);
    p->appendCString(buffer + 57, clen);

    if (p1 < 256) {
        clen = 1;
    } else if (p1 < 65536) {
        clen = 2;
    } else if (p1 < 16777216) {
        clen = 3;
    } else {
        clen = 4;
    }
    p1 = __builtin_bswap32 (p1);
    p->appendCString((char *)&p1 + 4 - clen, clen);
    p1 = __builtin_bswap32 (p1);

    if (p2 < 256) {
        clen = 1;
    } else if (p2 < 65536) {
        clen = 2;
    } else if (p2 < 16777216) {
        clen = 3;
    } else {
        clen = 4;
    }
    p2 = __builtin_bswap32 (p2);
    p->appendCString((char *)&p2 + 4 - clen, clen);
    p2 = __builtin_bswap32 (p2);

    p->appendInts((qint32 *) m_nonce, 4);
    p->appendInts((qint32 *) m_serverNonce, 4);
    Utils::randomBytes(m_newNonce, 32);
    p->appendInts((qint32 *) m_newNonce, 8);
    SHA1 ((uchar *) (p->buffer() + 5), (p->length() - 5) * 4, (uchar *) p->buffer());

    qint32 encryptBuffer[ENCRYPT_BUFFER_INTS];
    qint32 l = CryptoUtils::getInstance()->encryptPacketBuffer(*p, encryptBuffer);

    // Req DH params
    p->clearPacket();
    p->appendInt(TL_ReqDHParams);
    p->appendInts((qint32 *) m_nonce, 4);
    p->appendInts((qint32 *) m_serverNonce, 4);

    if (p1 < 256) {
        clen = 1;
    } else if (p1 < 65536) {
        clen = 2;
    } else if (p1 < 16777216) {
        clen = 3;
    } else {
        clen = 4;
    }
    p1 = __builtin_bswap32 (p1);
    p->appendCString((char *)&p1 + 4 - clen, clen);
    p1 = __builtin_bswap32 (p1);
    if (p2 < 256) {
        clen = 1;
    } else if (p2 < 65536) {
        clen = 2;
    } else if (p2 < 16777216) {
        clen = 3;
    } else {
        clen = 4;
    }
    p2 = __builtin_bswap32 (p2);
    p->appendCString((char *)&p2 + 4 - clen, clen);
    p2 = __builtin_bswap32 (p2);

    p->appendLong(Settings::getInstance()->pkFingerprint());
    p->appendCString((char *) encryptBuffer, l);

    m_dc->setState(DC::reqDHSent);
    qDebug() << "--------------------------- ";
    qDebug()<< "Reqesting for DH";
    rpcSendPacket(*p);
    delete p;
}

void DCAuth::processDHAnswer(InboundPkt &inboundPkt) {
    qDebug() << "Repsonce for DH ";
    qDebug()<< "Processing DH packet ";
    const char *buffer = inboundPkt.buffer();
    qint32 len = inboundPkt.length();
//    // qCDebug(TG_AUTH_DCAUTH) << "processDHAnswer(), len=" <<  len;

    mAsserter.check(len >= 116);
    mAsserter.check(!*(qint64 *) buffer);
    mAsserter.check(*(qint32 *) (buffer + 16) == len - 20);
    mAsserter.check(!(len & 3));
    mAsserter.check(*(qint32 *) (buffer + 20) == (qint32)TL_ServerDHParamsOK);

    mAsserter.check(!memcmp (buffer + 24, m_nonce, 16));
    mAsserter.check(!memcmp (buffer + 40, m_serverNonce, 16));
    CryptoUtils::getInstance()->initAESUnAuth (m_serverNonce, m_newNonce, AES_DECRYPT);

    inboundPkt.setInPtr((qint32 *)(buffer + 56));
    inboundPkt.setInEnd((qint32 *)(buffer + len));
    qint32 l = inboundPkt.prefetchStrlen();


    mAsserter.check(l > 0);
    qint32 decryptBuffer[DECRYPT_BUFFER_INTS];
    l = CryptoUtils::getInstance()->padAESDecrypt(inboundPkt.fetchStr(l), l, (char *)decryptBuffer, DECRYPT_BUFFER_INTS * 4 -16);
    mAsserter.check(inboundPkt.inPtr() == inboundPkt.inEnd());
    mAsserter.check(l >= 60);
    mAsserter.check(decryptBuffer[5] == (qint32)TL_ServerDHInnerData);

    mAsserter.check(!memcmp (decryptBuffer + 6, m_nonce, 16));
    mAsserter.check(!memcmp (decryptBuffer + 10, m_serverNonce, 16));
    qint32 g = decryptBuffer[14];
    inboundPkt.setInPtr(decryptBuffer + 15);
    inboundPkt.setInEnd(decryptBuffer + (l >> 2));
    BIGNUM dh_prime, dh_g, g_a, auth_key_num;
    BN_init(&dh_prime);
    BN_init (&g_a);
    mAsserter.check(inboundPkt.fetchBignum(&dh_prime) > 0);
    mAsserter.check(inboundPkt.fetchBignum(&g_a) > 0);
    mAsserter.check(Utils::check_g_bn(&dh_prime, &g_a) >= 0);
    qint32 serverTime = inboundPkt.fetchInt();
    mAsserter.check(inboundPkt.inPtr() <= inboundPkt.inEnd());
    mAsserter.check(CryptoUtils::getInstance()->checkDHParams (&dh_prime, g) >= 0);
    static char sha1Buffer[20];
    SHA1 ((uchar *) decryptBuffer + 20, (inboundPkt.inPtr() - decryptBuffer - 5) * 4, (uchar *) sha1Buffer);

//    !memcmp (decryptBuffer, sha1Buffer, 20);
    mAsserter.check((char *) inboundPkt.inEnd() - (char *) inboundPkt.inPtr() < 16);
    // Calculate time sync factor
    qint32 clientTime = QDateTime::currentDateTime().toTime_t();
    m_dc->setTimeSyncFactor(clientTime - serverTime);

    // Build set_client_DH_params answer
    OutboundPkt *outboundPkt = new OutboundPkt();
    outboundPkt->forwardPtr(5);
    outboundPkt->appendInt(TL_ClientDHInnerData);
    outboundPkt->appendInts((qint32 *)m_nonce, 4);
    outboundPkt->appendInts((qint32 *)m_serverNonce, 4);
    outboundPkt->appendLong(0LL);
    BN_init (&dh_g);
    Utils::ensure (BN_set_word (&dh_g, g));
    char s_power [256];
    Utils::randomBytes(s_power, 256);
    BIGNUM *dh_power = BN_bin2bn ((uchar *)s_power, 256, 0);
    Utils::ensurePtr(dh_power);
    BIGNUM *y = BN_new ();
    Utils::ensurePtr(y);
    Utils::ensure(CryptoUtils::getInstance()->BNModExp(y, &dh_g, dh_power, &dh_prime));
    outboundPkt->appendBignum(y);
    BN_free (y);
    BN_init (&auth_key_num);
    Utils::ensure(CryptoUtils::getInstance()->BNModExp(&auth_key_num, &g_a, dh_power, &dh_prime));
    l = BN_num_bytes (&auth_key_num);

    mAsserter.check(l >= 250 && l <= 256);
    mAsserter.check(BN_bn2bin (&auth_key_num, (uchar *)m_dc->authKey()));
    memset (m_dc->authKey() + l, 0, 256 - l);
    BN_free (dh_power);
    BN_free (&auth_key_num);
    BN_free (&dh_g);
    BN_free (&g_a);
    BN_free (&dh_prime);
    SHA1 ((uchar *) (outboundPkt->buffer() + 5), (outboundPkt->length() - 5) * 4, (uchar *) outboundPkt->buffer());
    qint32 encryptBuffer[DECRYPT_BUFFER_INTS];
    l = CryptoUtils::getInstance()->encryptPacketBufferAESUnAuth (m_serverNonce, m_newNonce, *outboundPkt, encryptBuffer);
    outboundPkt->clearPacket();
    outboundPkt->appendInt(TL_SetClientDHParams);
    outboundPkt->appendInts((qint32 *)m_nonce, 4);
    outboundPkt->appendInts((qint32 *)m_serverNonce, 4);
    outboundPkt->appendCString((char *)encryptBuffer, l);

    m_dc->setState(DC::clientDHSent);
    qDebug() << "--------------------------- ";
    qDebug() << "Requesting for CLient DH";
    rpcSendPacket(*outboundPkt);
    delete outboundPkt;
}

void DCAuth::processAuthComplete(InboundPkt &inboundPkt) {
    qDebug() << "Response from Client DH";
    const char *buffer = inboundPkt.buffer();
    qint32 len = inboundPkt.length();
//    // qCDebug(TG_AUTH_DCAUTH) << "processAuthComplete(), len=" <<  len;

    mAsserter.check(len == 72);
    mAsserter.check(!*(qint64 *) buffer);
    mAsserter.check(*(qint32 *) (buffer + 16) == len - 20);
    mAsserter.check(!(len & 3));
    mAsserter.check(*(qint32 *) (buffer + 20) == TL_DHGenOK);
    mAsserter.check(!memcmp (buffer + 24, m_nonce, 16));
    mAsserter.check(!memcmp (buffer + 40, m_serverNonce, 16));
    static uchar tmp[44], sha1Buffer[20];
    //uchar tmp[44], sha1Buffer[20];
    memcpy (tmp, m_newNonce, 32);
    tmp[32] = 1;

    SHA1 ((uchar*)m_dc->authKey(), 256, sha1Buffer);
    m_dc->setAuthKeyId(*(qint64 *)(sha1Buffer + 12));

    memcpy (tmp + 33, sha1Buffer, 8);
    SHA1 (tmp, 41, sha1Buffer);
    mAsserter.check(!memcmp (buffer + 56, sha1Buffer + 4, 16)); // XXX failed once!
    m_dc->setServerSalt(*(qint64 *)m_serverNonce ^ *(qint64 *)m_newNonce);
    m_dc->setState(DC::authKeyCreated);

  qDebug() << "Created Auth shared key for DC" << m_dc->id() << "Host" <<  m_dc->host() <<  "auth_key_id =" << m_dc->authKeyId();
  qDebug() << "*****************************************";
    Q_EMIT dcReady(m_dc);

}

void DCAuth::processRpcAnswer(QByteArray response) {

    qint32 op;
    peekIn(&op, 4);
    qint32 len = response.length();

//    qDebug() << "connection #" << socketDescriptor() << "received rpc answer" << op << "with" << len << "content bytes";

    const InboundPkt *p = new InboundPkt(response.data(), len);
    InboundPkt p1= *p;

    switch (m_dc->state()) {
    case DC::reqPQSent:
        processResPQAnswer(*p);
        break;
    case DC::reqDHSent:
        processDHAnswer(p1);
        break;
    case DC::clientDHSent:
        processAuthComplete(p1);
        break;
    case DC::authKeyCreated:
    case DC::userSignedIn:
        break;
    default:
      qWarning() << "fatal: cannot receive answer in state" << m_dc->state();
//        exit (2);
    }
}



void DCAuth::sendReqPQPacket() {
    qDebug() << " --------------------------- ";
    qDebug() << "Sending reuest for PQ Packet";
    //    Creates random Number for server of 16 bytes
    Utils::randomBytes(m_nonce, 16);
    OutboundPkt *obp = new OutboundPkt();
    obp->appendInt(TL_ReqPQ);
    obp->appendInts((qint32 *)m_nonce, 4);

    rpcSendPacket(*obp);
//
    delete obp;
//    // Settings state of DC to reqPQsent stage, this is needed for authorisation
    m_dc->setState(DC::reqPQSent);
}

void DCAuth::rpcSendPacket(OutboundPkt &packet) {

    qint32 written;
    Q_UNUSED(written);
    UnencryptedMsgHeader unencMsgHeader;
    unencMsgHeader.authKeyId = 0;
    unencMsgHeader.outMsgId = 0;
    qint32 len = packet.length() * 4;
    qint64 nextMsgId = (qint64) ((1LL << 32) * Utils::getUTime(CLOCK_REALTIME)) & -4;
    if (nextMsgId <= unencMsgHeader.outMsgId) {
        unencMsgHeader.outMsgId += 4;
    } else {
        unencMsgHeader.outMsgId = nextMsgId;
    }
    unencMsgHeader.msgLength = len;
    qint32 totalLen = len + 20;
    mAsserter.check(totalLen > 0 && !(totalLen & 0xfc000003));
    totalLen >>= 2;
    if (totalLen < 0x7f) {
        written = writeOut(&totalLen, 1);
        mAsserter.check(written == 1);
    } else {
        totalLen = (totalLen << 8) | 0x7f;
        written = writeOut(&totalLen, 4);
        mAsserter.check(written == 4);
    }
    writeOut(&unencMsgHeader, 20);
    writeOut(packet.buffer(), len);
//    qDebug()<<"rpcSendPacket sent successfully";
//    qCDebug(TG_AUTH_DCAUTH) << "packet sent";
}


