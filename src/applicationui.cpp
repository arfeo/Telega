/*
 * Copyright (c) 2017-2018 Arfeo.net.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
        QObject()
{
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_telega", this);

    // Enable application settings
    QCoreApplication::setOrganizationName("Arfeo.net");
    QCoreApplication::setApplicationName("Telega");

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
