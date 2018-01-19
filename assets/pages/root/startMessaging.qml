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
    id: mainNav
    
    Page {
        id: startMessagingPage
        
        // -----------------------
        // Page design
        // -----------------------
        
        Container {
            
            Container {
                id: appHeader
                background: Color.create("#ff5b95c2")
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
                topPadding: 20.0
                leftPadding: 30.0
                rightPadding: 30.0
                bottomPadding: 20.0
                
                Label {
                    text: qsTr("Telega")
                    textStyle.fontStyle: FontStyle.Normal
                    textStyle.fontWeight: FontWeight.Normal
                    textStyle.fontSizeValue: 0.0
                    textStyle.color: Color.White
                    textFit.minFontSizeValue: 10.0
                    textFit.maxFontSizeValue: 10.0
                    textFit.mode: LabelTextFitMode.Default
                }
            }
            
            ScrollView {
                horizontalAlignment: HorizontalAlignment.Fill
                
                Container {
                    id: appContent
                    topPadding: 30
                    rightPadding: 30
                    bottomPadding: 30
                    leftPadding: 30
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    Label {
                        text: qsTr("Telega is a native Telegram client for BlackBerry® 10") + Retranslate.onLocaleOrLanguageChanged
                        multiline: true
                        textStyle.fontWeight: FontWeight.W500
                    }
                    
                    /*Label {
                     text: qsTr("Telegram is a messaging app with a focus on speed and security, it’s super-fast, simple and free. You can use Telegram on all your devices at the same time — your messages sync seamlessly across any number of your phones, tablets or computers.\n\nWith Telegram, you can send messages, photos, videos and files of any type (doc, zip, mp3, etc), as well as create groups for up to 30,000 people or channels for broadcasting to unlimited audiences. You can write to your phone contacts and find people by their usernames. As a result, Telegram is like SMS and email combined — and can take care of all your personal or business messaging needs. In addition to this, we support end-to-end encrypted voice calls.\n\nTelegram is for everyone who wants fast and reliable messaging and calls. Business users and small teams may like the large groups, usernames, desktop apps and powerful file sharing options.") + Retranslate.onLocaleOrLanguageChanged
                     textStyle.base: SystemDefaults.TextStyles.MediumText
                     multiline: true
                     }*/
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        
                        Label {
                            text: qsTr("Sign in with:") + Retranslate.onLocaleOrLanguageChanged
                            multiline: true
                        }
                        
                        Button {
                            text: qsTr("New account") + Retranslate.onLocaleOrLanguageChanged
                            horizontalAlignment: HorizontalAlignment.Center
                            topMargin: 50.0
                            onClicked: {
                                mainNav.push(signUp.createObject())
                            }
                            enabled: false
                            visible: false
                        }
                        
                        Button {
                            text: qsTr("Existing account") + Retranslate.onLocaleOrLanguageChanged
                            horizontalAlignment: HorizontalAlignment.Center
                            topMargin: 50.0
                            onClicked: {
                                mainNav.push(signIn.createObject())
                            }
                        }
                    
                    }
                
                }
            
            }
        
        }
    
    }
    
    // -----------------------
    // Navigation handlers
    // -----------------------
    
    onPopTransitionEnded: {
        page.destroy();
    }

}