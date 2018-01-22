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
import bb.system 1.2
import bb.cascades.pickers 1.0

TabbedPane {
    id: mainTab
    objectName: "mainTab"
    
    property bool loggedIn: _telega.checkLogin()
    property bool loggedOut: false
    property string currentPage: ""
    property string currentChat: ""
    property string currentChatUserpic: ""
    property string currentPhoneNumber: ""
    property string currentUsername: "Leonid Belikov"
    property string currentUserpic: "asset:///images/user.jpg"
    
    // -----------------------
    // Attached objects
    // -----------------------
    attachedObjects: [
        
        /**
         *  ~~~ Application tabs ~~~
         */
        
        Tab {
            id: startMessagingTab
            title: "Start Messaging"
            enabled: false
            
            delegate: Delegate {
                id: signInDelegate
                source: "asset:///pages/root/startMessaging.qml"
            }
            
            onTriggered: {
                mainTab.currentPage = "startMessaging"
            }
        },
        
        Tab {
            id: chatsListTab
            title: "Chats"
            imageSource: "asset:///images/icons/ic_view_post.png"
            
            delegate: Delegate {
                id: chatsListDelegate
                source: "asset:///pages/root/chatsList.qml"
            }
            
            onTriggered: {
                mainTab.currentPage = "chatsList"
            }
        },
        
        Tab {
            id: contactsListTab
            title: "Contacts"
            imageSource: "asset:///images/icons/ic_contact.png"
            
            delegate: Delegate {
                id: contactsListDelegate
                source: "asset:///pages/root/contactsList.qml"
            }
            
            onTriggered: {
                mainTab.currentPage = "contactsList"
            }
        },
        
        Tab {
            id: savedMessagesTab
            title: "Saved Messages"
            imageSource: "asset:///images/icons/ic_favorite.png"
            
            delegate: Delegate {
                id: savedMessagesDelegate
                source: "asset:///pages/root/savedMessages.qml"
            }

            onTriggered: {
                mainTab.currentPage = "savedMessages"
                mainTab.currentChatUserpic = mainTab.currentUserpic
                mainTab.currentChat = mainTab.currentUsername
            }
        },
        
        Tab {
            id: settingsListTab
            title: "Settings"
            imageSource: "asset:///images/icons/ic_settings.png"
            
            delegate: Delegate {
                id: settingsListDelegate
                source: "asset:///pages/root/settingsList.qml"
            }
            
            onTriggered: {
                mainTab.currentPage = "settingsList"
            }
        },
        
        /**
         *  ~~~ Toasts & dialogs ~~~
         */
        
        SystemToast {
            id: errorMessageToast
        },
        
        SystemToast {
            id: successSignInToast
            body: "Logged in as " + mainTab.currentUsername
        },
        
        SystemToast {
            id: successLogoutToast
            body: "You've just successfully logged out"
        },
        
        SystemDialog {
            id: confirmLogoutDialog
            title: "Are you sure you want to log out?"
            onFinished: {
                if(confirmLogoutDialog.result != SystemUiResult.CancelButtonSelection) {
                    successLogoutToast.show()
                    setAppTabs("start")
                }  
            }
        },
        
        /**
         *  ~~~ Pages ~~~
         */
        
        ComponentDefinition {
            id: signIn
            source: "asset:///pages/utils/signIn.qml"
        },
        
        ComponentDefinition {
            id: enterCode
            source: "asset:///pages/utils/enterCode.qml"
        },
        
        ComponentDefinition {
            id: chatView
            source: "asset:///pages/utils/chatView.qml"
        },
        
        ComponentDefinition {
            id: userpicView
            source: "asset:///pages/utils/userpicView.qml"
        },
        
        /**
         *  ~~~ Active frame ~~~
         */
        
        MultiCover {
            id: multiCover
            
            SceneCover {
                id: bigCover
                
                MultiCover.level: CoverDetailLevel.High
                content: Container {
                    
                    Label {
                        text: "Large Cover"
                        textStyle.color: Color.create("#ff5b95c2")
                        textStyle.fontSizeValue: 10.0
                    }
                }
                function update() {
                    // ..
                }
            }
            
            SceneCover {
                id: smallCover
                
                MultiCover.level: CoverDetailLevel.Medium
                content: Container {
                    Label {
                        text: "Small Cover"
                        textStyle.color: Color.create("#ff5b95c2")
                        textStyle.fontSizeValue: 5.0
                    }
                }
                function update() {
                    // ..
                }
            }
            
            function update() {
                bigCover.update()
                smallCover.update()
            }
        
        },
        
        FilePicker {
            id: filePicker
            type: FileType.Other
            title: "Select File"
            directories: ["/accounts/1000/shared/misc"]
            onFileSelected: {
               console.log("FileSelected signal received : " + selectedFiles)
            }
        }
    
    ]
    
    // -----------------------
    // Application handlers
    // -----------------------
    
    onCreationCompleted: {
        Application.setCover(multiCover)
        setAppTabs((!mainTab.loggedIn) ? "start" : "main")
    }
    
    // -----------------------
    // Application functions
    // -----------------------
    
    function setAppTabs(n) {
        
        switch(n) {
            
            case "start":
                mainTab.loggedIn = false
                mainTab.add(startMessagingTab)
                mainTab.remove(chatsListTab)
                mainTab.remove(contactsListTab)
                mainTab.remove(savedMessagesTab)
                mainTab.remove(settingsListTab)
                mainTab.activeTab = startMessagingTab
                break
            
            case "main":
                mainTab.loggedIn = true
                mainTab.remove(startMessagingTab)
                mainTab.add(chatsListTab)
                mainTab.add(contactsListTab)
                mainTab.add(savedMessagesTab)
                mainTab.add(settingsListTab)
                mainTab.activeTab = chatsListTab
                break
            
        }
        
    }

}