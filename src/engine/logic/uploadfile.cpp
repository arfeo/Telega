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

#include "uploadfile.h"

#include <QFileInfo>

UploadFile::UploadFile(Session *session, UploadFile::FileType fileType, const QByteArray &buffer, QObject *parent) :
    File(session, buffer.size(), parent),
    m_fileType(fileType),
    m_relatedFileId(0),
    m_name(""),
    m_bytes(buffer),
    m_filePath(""),
    m_nParts(0),
    m_currentPart(0),
    m_uploadedParts(0),
    m_krypt(QCryptographicHash::Md5)
{
    // calculate number of parts to be uploaded
    m_nParts = m_length / BLOCK;
    if (m_length % BLOCK > 0) m_nParts++;
}

UploadFile::UploadFile(Session *session, UploadFile::FileType fileType, const QString &filePath, QObject *parent) :
    File(session, QFileInfo(filePath).size(), parent),
    m_fileType(fileType),
    m_relatedFileId(0),
    m_name(QFileInfo(filePath).fileName()),
    m_filePath(filePath),
    m_nParts(0),
    m_currentPart(0),
    m_uploadedParts(0),
    m_ioDevice(filePath),
    m_krypt(QCryptographicHash::Md5)
{
    // open file
    openIODevice();
    // calculate number of parts to be uploaded
    m_nParts = m_length / BLOCK;
    if (m_length % BLOCK > 0) m_nParts++;
}

UploadFile::~UploadFile() {
}

void UploadFile::openIODevice() {
    if (m_ioDevice.open(QIODevice::ReadOnly)) {
        // get file size with file open
        m_length = m_ioDevice.size();
    } else {
        Q_EMIT fileNotFound();
    }
}

bool UploadFile::hasMoreParts() const {
    return m_currentPart < m_nParts;
}

QByteArray UploadFile::nextPart() {
    QByteArray partBytes(BLOCK, Qt::Uninitialized); //XXX take a look if this initialization results in some failure with final block

    //let's see if the file content is loaded or a filepath is set
    if (!m_bytes.isEmpty()) {
        qint32 k = 0;
        if (m_currentPart < m_nParts -1) {
            // if not the last part
            for (qint32 j = (m_currentPart * BLOCK); j < (m_currentPart + 1) * BLOCK; j++) {
                partBytes[k++] = m_bytes[j];
            }
        } else {
            // if last part
            qint32 lastPartSize = m_length % BLOCK;
            //XXX take a look at this, if there is some problem with last part
            //partBytes = block(lastPartSize, Qt::Uninitialized);
            // if not the last part
            for (qint32 j = (m_currentPart * BLOCK); j < (m_currentPart * BLOCK) + lastPartSize; j++) {
                partBytes[k++] = m_bytes[j];
            }
        }
    } else {
        // in this case we're reading an open file from a local path
        if (m_ioDevice.isOpen()) {
            m_ioDevice.seek(m_currentPart * BLOCK); //set offset
            partBytes = m_ioDevice.read(BLOCK);
            if (!hasMoreParts()) {
                m_ioDevice.close();
            }
        } else {
            Q_EMIT fileNotOpen();
        }
    }
    m_currentPart++;
    // add the new data to cryptographic hash before return it
    m_krypt.addData(partBytes, partBytes.length());
    return partBytes;
}

void UploadFile::increaseUploadedParts() {
    m_uploadedParts++;
}

QByteArray UploadFile::md5() {
    return m_krypt.result();
}
