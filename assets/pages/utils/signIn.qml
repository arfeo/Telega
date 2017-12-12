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
    id: signInPage
    
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
                text: qsTr("Sign in")
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
                topPadding: 30
                rightPadding: 30
                bottomPadding: 30
                leftPadding: 30
                horizontalAlignment: HorizontalAlignment.Fill
                 
                Label {
                    text: qsTr("Please choose your country and enter your full phone number.") + Retranslate.onLocaleOrLanguageChanged
                    multiline: true
                }
                
                DropDown {
                    id: countrySelector
                    title: qsTr("Country")

                    Option {
                        text: "Russia (+7)"
                        value: "7"
                    }
                    Option {
                        text: "United Kingdom (+44)"
                        value: "44"
                    }
                    Option {
                        text: "United States (+1)"
                        value: "1"
                    }
                    
                    onSelectedIndexChanged: {
                        countryCode.text = selectedValue
                    }
                }
                
                Container {

                    layout: GridLayout {
                        columnCount: 3
                    }
                    
                    Label {
                        text: "+"
                        verticalAlignment: VerticalAlignment.Center
                        margin.rightOffset: 10.0
                    }
                    
                    TextField {
                        id: countryCode
                        textFormat: TextFormat.Auto
                        inputMode: TextFieldInputMode.Pin
                        hintText: qsTr("Code") + Retranslate.onLocaleOrLanguageChanged
                        minWidth: 120.0
                        maxWidth: 120.0
                        margin.rightOffset: 20.0
                    }
                    
                    TextField {
                        id: phoneNumber
                        textFormat: TextFormat.Auto
                        inputMode: TextFieldInputMode.PhoneNumber
                        hintText: qsTr("Phone number") + Retranslate.onLocaleOrLanguageChanged
                        text: qsTr("")
                    }

                }
                 
                Button {
                    text: qsTr("Next")
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 50.0
                    onClicked: {
                        mainTab.currentPhoneNumber = countryCode.text + phoneNumber.text
                        signInPage.parent.push(enterCode.createObject())
                    }
                }

            }
             
        }
    
    }

}