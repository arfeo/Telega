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
    id: enterCodePage
    
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
                    text: "+" + mainTab.currentPhoneNumber
                    textStyle.fontWeight: FontWeight.W500
                }
                
                Label {
                    text: qsTr("We've sent the code to the Telegram app on your other device.\nPlease enter the code below.") + Retranslate.onLocaleOrLanguageChanged
                    multiline: true
                }
                
                TextField {
                    hintText: qsTr("Enter your code")
                    textFormat: TextFormat.Plain
                    inputMode: TextFieldInputMode.Pin
                    input.submitKey: SubmitKey.Next
                }
                
                Button {
                    text: qsTr("Next")
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 50.0
                    onClicked: {
                        successSignInToast.show()
                        setAppTabs("main")
                    }
                }
                
            }
            
        }
        
    }

}