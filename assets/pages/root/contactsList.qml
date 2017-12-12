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

NavigationPane {
    id: mainNav

    Page {
        id: contactsListPage
        
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
    
                Label {
                    text: qsTr("Contacts")
                    textStyle.fontStyle: FontStyle.Normal
                    textStyle.fontWeight: FontWeight.Normal
                    textStyle.fontSizeValue: 0.0
                    textStyle.color: Color.White
                    textFit.minFontSizeValue: 10.0
                    textFit.maxFontSizeValue: 10.0
                    textFit.mode: LabelTextFitMode.Default
                }
            }
            
            Container {
            
                ListView {
                    dataModel: XmlDataModel { source: "asset:///xml/contacts.xml" }
                    
                    listItemComponents:[
                        
                        ListItemComponent {
                            type: "letter"
                            
                            Container{
                                enabled: false
                                topPadding: 10.0
                                bottomPadding: 15.0
    
                                Label {
                                    text: ListItemData.name
                                    textStyle.base: SystemDefaults.TextStyles.BigText
                                    textStyle.fontWeight: FontWeight.W100
                                    margin.leftOffset: 30.0
                                    textStyle.color: Color.LightGray
                                }
                            }
                        },
                        
                        ListItemComponent {
                            type: "contact"
                            
                            StandardListItem {
                                imageSource: ListItemData.pic
                                title: ListItemData.contactName
                            }
                        }
                    
                    ]
                    
                    onTriggered: {
                        var selectedItem = dataModel.data(indexPath)
                        mainTab.currentChat = selectedItem.contactName
                        mainTab.currentChatUserpic = selectedItem.pic
                        mainNav.push(chatView.createObject())
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
                title: qsTr("Add contact")
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/icons/ic_add.png"
                
                onTriggered: {
                    // ..
                }
            }
        ]
    
    }
    
    // -----------------------
    // Navigation handlers
    // -----------------------
    
    onPopTransitionEnded: {
        page.destroy();
    }
    
}