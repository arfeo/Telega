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
        id: settingsListPage
        
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
                    text: qsTr("Settings")
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
            
                Container {
                    topPadding: 30
                    rightPadding: 30
                    bottomPadding: 30
                    leftPadding: 30
                }
                    
            }
        
        }
        
        // -----------------------
        // Page actions
        // -----------------------
        
        actions: [
            ActionItem {
                title: qsTr("Edit Profile")
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/icons/ic_edit_profile.png"
                
                onTriggered: {
                    // ..
                }
            },
            
            ActionItem {
                title: qsTr("Logout")
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/icons/ic_cancel.png"
                
                onTriggered: {
                    confirmLogoutDialog.show()
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