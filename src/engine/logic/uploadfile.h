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

#ifndef UPLOADFILE_H
#define UPLOADFILE_H

#include <QObject>
#include <QFile>
#include <QCryptographicHash>
#include "file.h"
#include "../types/inputmedia.h"

class UploadFile :
        public File
{

    Q_OBJECT

public:
    enum FileType {
        file,
        thumbnail
    };
    explicit UploadFile(Session *session, FileType type, const QByteArray &buffer, QObject *parent = 0);
    explicit UploadFile(Session *session, FileType type, const QString &filePath, QObject *parent = 0);
    ~UploadFile();
    qint64 relatedFileId() const { return m_relatedFileId; }
    void setRelatedFileId(qint64 relatedFileId) { m_relatedFileId = relatedFileId; }
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    FileType fileType() const { return m_fileType; }
    qint32 nParts() const { return m_nParts; }
    qint32 uploadedParts() const { return m_uploadedParts; }
    bool hasMoreParts() const;
    QByteArray nextPart();
    void increaseUploadedParts();
    QByteArray md5();

Q_SIGNALS:
    void fileNotFound();
    void fileNotOpen();

private:
    FileType m_fileType;
    qint64 m_relatedFileId; // if this is the main file, related file is thumbnail and viceversa
    QString m_name;
    QByteArray m_bytes;
    QString m_filePath;
    qint32 m_nParts;
    qint32 m_currentPart;
    qint32 m_uploadedParts;
    QFile m_ioDevice;
    QCryptographicHash m_krypt;
    void openIODevice();

};

#endif // UPLOADFILE_H
