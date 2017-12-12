/*
 * Copyright (c) 2017 Arfeo.net.
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

import bb.cascades 1.4

Page {
    id: chatViewPage
    
    // -----------------------
    // Page design
    // -----------------------
    
    Container {
    
        Container {
            background: Color.create("#ff5b95c2")
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            
            topPadding: 20.0
            leftPadding: 30.0
            rightPadding: 30.0
            bottomPadding: 20.0
            
            Container {
    
                layout: GridLayout {
                    columnCount: 2
                }
    
                ImageView {
                    imageSource: (mainTab.currentPage != "savedMessages") ? mainTab.currentChatUserpic : "asset:///images/icons/ic_favorite.png"
                    minHeight: 50.0
                    preferredHeight: 50.0
                    maxHeight: 50.0
                    minWidth: 50.0
                    preferredWidth: 50.0
                    maxWidth: 50.0
                    margin.rightOffset: 20.0
                    
                    onTouch: {
                        if(event.touchType == TouchType.Down) {
                            if(mainTab.currentPage == "savedMessages")
                                savedMessagesNav.push(userpicView.createObject())
                            else
                                chatViewPage.parent.push(userpicView.createObject())
                        }   
                    }
                }

                Label {
                    text: qsTr((mainTab.currentPage != "savedMessages") ? mainTab.currentChat : "Saved Messages")
                    textStyle.fontStyle: FontStyle.Normal
                    textStyle.fontWeight: FontWeight.Normal
                    textStyle.fontSizeValue: 0.0
                    textStyle.color: Color.White
                    textFit.minFontSizeValue: 10.0
                    textFit.mode: LabelTextFitMode.Default
                }

            }
            
        }
        
        Container {

            Container {
                background: Color.White
                preferredHeight: maxHeight
                
                TextArea {
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        fontWeight: FontWeight.Bold
                        color: Color.Yellow
                    }
                    
                    backgroundVisible: true
                    editable: false
                    text: "Test"
                }
                
            }
            
            Container {
                
                layout: GridLayout {
                    columnCount: 3
                }
                
                ImageView {
                    imageSource: "asset:///images/telegram/drawable-xhdpi/ic_smiles.png"
                    verticalAlignment: VerticalAlignment.Center
                    margin.leftOffset: 20.0
                    
                    onTouch: {
                        if(event.touchType == TouchType.Down) {
                            // ..
                        }
                    }
                }
            
                Container {
                    leftPadding: ui.du(2.2)
                    rightPadding: ui.du(2.2)
                    bottomPadding: ui.du(2.2)
                    topPadding: ui.du(2.2)
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    TextArea {
                        id: messageInput
                        hintText: qsTr("Message")

                        onTextChanging: {
                            if(messageInput.text.length > 0) {
                                attachButton.visible = false
                                sendButton.visible = true
                            } else {
                                attachButton.visible = true
                                sendButton.visible = false
                            }
                        }
                    }
                }
                
                ImageView {
                    id: attachButton
                    imageSource: "asset:///images/telegram/drawable-xhdpi/ic_attach.png"
                    verticalAlignment: VerticalAlignment.Center
                    margin.rightOffset: 20.0
                    
                    onTouch: {
                        if(event.touchType == TouchType.Down) {
                            filePicker.open()
                        }
                    }
                }
                
                ImageView {
                    id: sendButton
                    imageSource: "asset:///images/telegram/drawable-xhdpi/ic_send.png"
                    verticalAlignment: VerticalAlignment.Center
                    margin.rightOffset: 20.0
                    visible: false
                    
                    onTouch: {
                        if(event.touchType == TouchType.Down) {
                            // ..
                            messageInput.text = ""
                        }
                    }
                }
            }
        }
    }
    
    // -----------------------
    // Page actions
    // -----------------------
    
    actions: [        
        ActionItem {
            title: qsTr("Search")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/icons/ic_search.png"
            
            onTriggered: {
                // ..
            }
        },
        
        ActionItem {
            title: qsTr("Clear history")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/icons/ic_clear_list.png"
            
            onTriggered: {
                // ..
            }
        },
        
        ActionItem {
            title: qsTr("Delete chat")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/icons/ic_delete.png"
            
            onTriggered: {
                // ..
            }
        },
        
        ActionItem {
            title: qsTr("Mute notifications")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/icons/ic_speaker_mute.png"
            
            onTriggered: {
                // ..
            }
        }
    ]

}