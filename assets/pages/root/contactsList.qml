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