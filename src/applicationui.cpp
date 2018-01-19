/*
 * Copyright (c) 2017-2018 Arfeo.net.
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
 */

#include "applicationui.hpp"
#include "telegram.h"
#include "config.h"

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
        QObject()
{
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_telega", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);
}

ApplicationUI::~ApplicationUI()
{
    // ..
}

/**
 *
 * ~~~ ENGINE ~~~
 *
 */

// ..
