/*
 * loadcontacts.hpp
 *
 *  Created on: Jan 23, 2015
 *      Author: HTML5
 */

#ifndef LOADCONTACTS_HPP_
#define LOADCONTACTS_HPP_

#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactService>
#include <bb/pim/contacts/ContactListFilters>
#include <bb/pim/contacts/ContactAttribute>

class LoadContacts :
        public QObject
{

    Q_OBJECT

public:
    LoadContacts();
    QVariantList loadContacts();

};




#endif /* LOADCONTACTS_HPP_ */
