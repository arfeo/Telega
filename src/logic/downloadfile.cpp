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

#include "downloadfile.h"

DownloadFile::DownloadFile(Session *session, InputFileLocation fileLocation, qint32 expectedSize, QObject *parent) :
    File(session, 0, parent),
    m_fileLocation(fileLocation),
    m_offset(0),
    m_mtime(0),
    m_expectedSize(expectedSize),
    m_partId(0){
}

DownloadFile::~DownloadFile() {
}

QByteArray DownloadFile::bytes() const {
    return m_bytes;
}

void DownloadFile::appendBytes(const QByteArray &bytes) {
    m_bytes.append(bytes);
    m_offset += bytes.length();
}

qint32 DownloadFile::offset() const {
    return m_offset;
}

void DownloadFile::setOffset(qint32 offset) {
    m_offset = offset;
}

qint32 DownloadFile::mtime() const  {
    return m_mtime;
}
void DownloadFile::setMtime(qint32 mtime) {
    m_mtime = mtime;
}

InputFileLocation DownloadFile::fileLocation() const {
    return m_fileLocation;
}

qint32 DownloadFile::expectedSize() const {
    return m_expectedSize;
}

qint32 DownloadFile::partId() const {
    return m_partId;
}

void DownloadFile::incrementPartId() {
    m_partId++;
}
QString DownloadFile::filename() {
   return m_filename;
}
QString DownloadFile::type() {
    return m_type;
}
