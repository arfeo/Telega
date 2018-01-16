/*
 * loadcontacts.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: HTML5
 */

#include "loadcontacts.hpp"

using namespace bb::pim::contacts;

LoadContacts::LoadContacts()
{
    // ..
}

QVariantList LoadContacts::loadContacts()
{
    QVariantMap map;
    QVariantList list;
    ContactListFilters filters;
    filters.setLimit(100);
    QList < bb::pim::contacts::Contact > contactList;
    contactList = ContactService().contacts(filters);
    Q_FOREACH(bb::pim::contacts::Contact con,contactList){
        bb::pim::contacts::Contact contact = ContactService().contactDetails(con.id());
        Q_FOREACH(ContactAttribute ph, contact.phoneNumbers()) {
            qDebug()<<"name"<<contact.firstName()<<"lastname"<<contact.lastName();
            map.insert("id",ph.id());
            map.insert("phone",ph.value());
            map.insert("firstname",contact.firstName());
            map.insert("lastname",contact.lastName());
            list<<map;
        }
    }
    return list;
}

