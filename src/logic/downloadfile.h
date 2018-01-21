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

#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QString>
#include <QByteArray>
#include "../core/constants.h"
#include "../types/inputfilelocation.h"
#include "file.h"
#include "../core/session.h"

class DownloadFile :
        public File
{
    Q_OBJECT

public:
    DownloadFile(Session *session, InputFileLocation fileLocation, qint32 expectedSize,QObject *parent = 0);
    ~DownloadFile();
    void appendBytes(const QByteArray &bytes);
    QByteArray bytes() const;
    qint32 offset() const;
    void setOffset(qint32 offset);
    void setMtime(qint32 mtime);
    qint32 mtime() const;
    InputFileLocation fileLocation() const;
    qint32 expectedSize() const ;
    qint32 partId() const;
    void incrementPartId();
    QString filename();
    QString type();

private:
    InputFileLocation m_fileLocation;
    QString m_name;
    QString m_filename;
    QString m_type;
    QByteArray m_bytes;
    qint32 m_offset;
    qint32 m_mtime;
    qint32 m_expectedSize;
    qint32 m_partId;
};

#endif // DOWNLOADFILE_H
