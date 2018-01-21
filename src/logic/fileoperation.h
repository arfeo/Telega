/*
 * Copyright 2014 Canonical Ltd.
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
 * Authors:
 *      Roberto Mier
 *      Tiago Herrmann
 */

#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include "uploadfile.h"
#include "../types/inputpeer.h"
#include "../types/inputmedia.h"
#include "../types/inputchatphoto.h"

class FileOperation
{
public:
    enum OpType {
        sendMedia,
        editChatPhoto,
        uploadProfilePhoto
    };
    FileOperation(OpType opType) :
        m_peer(InputPeer::typeInputPeerEmpty),
        m_inputMedia(InputMedia::typeInputMediaEmpty),
        mRandomId(0),
        m_inputChatPhoto(InputChatPhoto::typeInputChatPhotoEmpty),
        m_geoPoint(InputGeoPoint::typeInputGeoPointEmpty),
        m_crop(InputPhotoCrop::typeInputPhotoCropAuto),
        m_type(opType) {}
    InputPeer peer() const { return m_peer; }
    void setInputPeer(const InputPeer &peer) { m_peer = peer; }
    InputMedia inputMedia() const { return m_inputMedia; }
    void setInputMedia(const InputMedia &inputMedia) { m_inputMedia = inputMedia; }
    qint64 randomId() const { return mRandomId; }
    void setRandomId(qint64 randomId) { mRandomId = randomId; }
    qint32 chatId() const { return m_chatId; }
    void setChatId(qint32 chatId) { m_chatId = chatId; }
    InputChatPhoto inputChatPhoto() const { return m_inputChatPhoto; }
    void setInputChatPhoto(InputChatPhoto inputChatPhoto) { m_inputChatPhoto = inputChatPhoto; }
    QString caption() const { return m_caption; }
    void setCaption(QString caption) { m_caption = caption; }
    InputGeoPoint geoPoint() const { return m_geoPoint; }
    void setGeoPoint(InputGeoPoint geoPoint) { m_geoPoint = geoPoint; }
    InputPhotoCrop crop() const { return m_crop; }
    void setCrop(InputPhotoCrop crop) { m_crop = crop; }
    OpType opType() const { return m_type; }

private:
    // `sendMedia` operation attributes
    InputPeer m_peer;
    InputMedia m_inputMedia;
    qint64 mRandomId;

    // `editChatPhoto` operation attributes
    qint32 m_chatId;
    InputChatPhoto m_inputChatPhoto;

    // `uploadProfilePhoto` operation attributes
    QString m_caption;
    InputGeoPoint m_geoPoint;
    InputPhotoCrop m_crop;
    OpType m_type;
};

#endif // FILEOPERATION_H
