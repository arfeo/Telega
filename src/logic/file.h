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

#ifndef FILE_H
#define FILE_H

#include <QObject>
#include "../core/constants.h"
#include "../core/utils.h"
#include "../core/session.h"

class File :
        public QObject
{
    Q_OBJECT

public:
    explicit File(Session *session, qint64 length, QObject *parent = 0) : QObject(parent), m_session(session), m_length(length) {
        // generate random file id
        Utils::randomBytes(&m_id, 8);
    }
    ~File() {
        if (m_session) {
            m_session->release();
        }
    }
    void setSession(Session *session) { m_session = session; }
    Session *session() { return m_session; }
    qint64 id() const { return m_id; }
    qint64 length() const { return m_length; }
    qint64 partLength() const { return BLOCK; }

protected:
    Session *m_session;
    qint64 m_id;
    qint64 m_length;
};

#endif // FILE_H
