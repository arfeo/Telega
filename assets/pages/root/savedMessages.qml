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

import bb.cascades 1.4

NavigationPane {
    id: savedMessagesNav
    
    // -----------------------
    // Attached objects
    // -----------------------
    
    attachedObjects: [
        ComponentDefinition {
            id: savedMessagesChatView
            source: "asset:///pages/utils/chatView.qml"
        }
    ]
    
    // -----------------------
    // Page handlers
    // -----------------------
    
    onCreationCompleted: {
        var createdControl = savedMessagesChatView.createObject();
        savedMessagesNav.push(createdControl);
    }
    
    // -----------------------
    // Navigation handlers
    // -----------------------
    
    onPopTransitionEnded: {
        page.destroy();
    }
}