// ******************************************
// * Baah Box Arduino : Sensor BTLE gateway *
// ******************************************

// Copyright (C) 2017 – 2023 Orange SA

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <Arduino.h>
#include "display.hpp"
#include <Adafruit_MAX1704X.h>
#include <Fonts/FreeSans12pt7b.h>

Adafruit_MAX17048 maxlipo;
int decalage = 120;
int cptDisplayBatt = 0;

GFXcanvas16 canvas(240, 135);
extern configSDClass config3dHandz;

//*********************************************
//*
//*       Constructor
//*
//*********************************************
handzDisplay::handzDisplay()
{
}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
handzDisplay::~handzDisplay()
{
}

//*********************************************
//*
//*       Init
//*
//*********************************************
void handzDisplay::init(void)
{
    Serial.println("ESP32S3 TFT  test");

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  display.init(135, 240); // Init ST7789 240x135
  display.setRotation(3);

  

  Serial.println(F("Initialized"));

  //
   // oledcanvas.clearDisplay();
  //  canvas.display();
// 128 X 32
    for (int index = 0; index < 118; index++)
    {
        tblCapteur1[index] = 0;
        tblCapteur2[index] = 0;
    }
    idxTblCapteur = 0;

    displayMode = 1;

    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT);
    pinMode(2, INPUT_PULLDOWN);
    button_A_pressed = 0;
    button_B_pressed = 0;
    button_C_pressed = 0;

    scheduler = new Scheduler(millis(), DISPLAY_DELAY);
}

//*********************************************
//*
//*       Display banner
//*
//*********************************************
void handzDisplay::DisplayBanner(void)
{
    display.setFont(&FreeSans12pt7b);
    display.setTextWrap(false);
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(10, 40);
    display.setTextColor(ST77XX_WHITE);
    display.setTextSize(2);
    display.println(APPLICATION_NAME);
    display.setTextSize(1); 
    display.setTextColor(ST77XX_ORANGE);
    display.setCursor(10, 80);
    display.println(config3dHandz.copyright);
}

//*********************************************
//*
//*       isButtonPressed
//*
//*********************************************
int handzDisplay::isButtonPressed(void)
{
    // read displayer buttons
    int button_A = digitalRead(0);
    int button_B = digitalRead(1);
    int button_C = digitalRead(2);

    if (button_A == 0) // pressed
    {
        if (button_A_pressed == 0)
        {
            Serial.println("Button A pressed");
            button_A_pressed = 1;
        }
    }
    else
    {
        if (button_A_pressed == 1)
        {
            Serial.println("Button A released");
            button_A_pressed = 0;
        }
    }

    if (button_B == 1)
    {
        if (button_B_pressed == 0)
        {
            Serial.println("Button B pressed");
            button_B_pressed = 1;
        }
    }
    else
    {
        if (button_B_pressed == 1)
        {
            Serial.println("Button B released");
            button_B_pressed = 0;
        }
    }

    if (button_C == 1)
    {
        if (button_C_pressed == 0)
        {
            Serial.println("Button C pressed");
            button_C_pressed = 1;
        }
    }
    else
    {
        if (button_C_pressed == 1)
        {
            Serial.println("Button C released");
            button_C_pressed = 0;
        }
    }

    if (button_A_pressed == 1 || button_B_pressed == 1 || button_C_pressed == 1)
    {
        return 1;
    }

    return 0;
}

//*********************************************
//*
//*       displayAxes
//*
//*********************************************
void handzDisplay::displayAxes(int type)
{
    // type =>
    //  0 : 2 sensors
    //  1 : sensor 1 only
    //  2 : sensor 2 only

    GFXcanvas16 canvas(240, 135);
    canvas.setFont(&FreeSans12pt7b);
    canvas.setTextSize(2);
    canvas.setTextColor(ST77XX_WHITE);
    int PosLegende = 85;
    int x0 = 0;
    int y0 = 0;
    int x1 = 120;
    int y1 = 75;

    switch (type)
    {
    case 0:
        // display caption for sensor 1
        canvas.setCursor(5, PosLegende);
        canvas.print(getTranslatedString(KEY_SENSOR));
        canvas.print("1");
        // display caption for sensor 2
        canvas.setCursor(129 + 5, PosLegende);
        canvas.print(getTranslatedString(KEY_SENSOR));
        canvas.print("2");

        canvas.drawLine(x0, y0, x0, y1, ST77XX_WHITE);
        canvas.drawLine(x0, y1, x1 - 2, y1, ST77XX_WHITE);

        canvas.drawLine(x0 + decalage, y0, x0 + decalage, y1, ST77XX_WHITE);
        canvas.drawLine(x0 + decalage, y1, x1 + decalage - 2, y1, ST77XX_WHITE);
        break;
    case 1:
    case 2:
        x1 = 120 + decalage;
        // display caption for selected sensor
        canvas.setCursor(50, PosLegende);
        canvas.print(getTranslatedString(KEY_SENSOR));
        canvas.print(type);
        canvas.drawLine(x0, y0, x0, y1, ST77XX_WHITE);
        canvas.drawLine(x0, y1, x1 - 2, y1, ST77XX_WHITE);
        break;
    }
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
    
}

//*********************************************
//*
//*       capteurs
//*
//*********************************************
void handzDisplay::capteurs(int type)
{
    muscleSensor.getValue(&capteur1, &capteur2);
    displayAxes(type);

    switch (type)
    {
    case 0:
        tblCapteur1[idxTblCapteur] = capteur1;
        tblCapteur2[idxTblCapteur] = capteur2;
        if (idxTblCapteur >= 62)
        {
            idxTblCapteur = 0;
        }
        displayCapteur(0, type);
        displayCapteur(1, type);
        break;
    case 1:
        tblCapteur[idxTblCapteur] = capteur1;
        if (idxTblCapteur >= 126)
        {
            idxTblCapteur = 0;
        }
        displayCapteur(0, type);
        break;
    case 2:
        tblCapteur[idxTblCapteur] = capteur2;
        if (idxTblCapteur >= 126)
        {
            idxTblCapteur = 0;
        }
        displayCapteur(1, type);
        break;
    }
    idxTblCapteur++;
}

//*********************************************
//*
//*       displayCapteur
//*
//*********************************************
void handzDisplay::displayCapteur(int channel, int type)
{
    int posX, posY;
    GFXcanvas16 canvas(240, 135);
    canvas.setFont(&FreeSans12pt7b);
   
    switch (type)
    {
    case 0:
        for (int index = 0; index < 62; index++)
        {
            if (channel == 0)
            {
                posX = index + 1;
                posY = map(tblCapteur1[index], 0, 1023, 22, 0);
            }
            else
            {
                posX = index + 1 + decalage;
                posY = map(tblCapteur2[index], 0, 1023, 22, 0);
            }
            canvas.drawPixel(posX, posY, ST77XX_GREEN);
        }
        break;
    case 1:
    case 2:
        for (int index = 0; index < 126; index++)
        {
            posX = index + 1;
            posY = map(tblCapteur[index], 0, 1023, 22, 0);
            canvas.drawPixel(posX, posY, ST77XX_WHITE);
        }
        break;
    }
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
   
}

//*********************************************
//*
//*       refreshDisplay
//*
//*********************************************
void handzDisplay::refreshDisplay()
{
    if (cptRefresh++ > 5)
    {
        //canvas.display();
        cptRefresh = 0;
    }
}

//*********************************************
//*
//*       joystick
//*
//*********************************************
void handzDisplay::joystick()
{
    int btGauche = digitalRead(config3dHandz.joystickDigitalInputTab[1]);
    int btDroit = digitalRead(config3dHandz.joystickDigitalInputTab[0]);
    int btBas = digitalRead(config3dHandz.joystickDigitalInputTab[2]);
    int btHaut = digitalRead(config3dHandz.joystickDigitalInputTab[3]);
    //int btGauche = digitalRead(11);
    //int btDroit = digitalRead(A5);
    //int btBas = digitalRead(12);
    //int btHaut = digitalRead(13);

    char blancs[20] = "           ";
    char titre[20] = " Joystick  ";

    GFXcanvas16 canvas(240, 135);
    canvas.setFont(&FreeSans12pt7b);
    canvas.setTextColor(ST77XX_WHITE);
    canvas.setCursor(0, 0);

    canvas.print(blancs);
    canvas.print("   [");
    if (btHaut == 0) {
        canvas.setTextColor(ST77XX_CYAN);
        canvas.print("*");
         canvas.setTextColor(ST77XX_WHITE);
    }
    else  
       canvas.print(" ");
    canvas.println("]");

    canvas.print(titre);
    canvas.print("[");
    if (btGauche == 0) {
        canvas.setTextColor(ST77XX_CYAN);
        canvas.print("*");
        canvas.setTextColor(ST77XX_WHITE);
    }
    else {
        canvas.print(" ");
    }
    canvas.print("]");
    canvas.print("   [");
    if (btDroit == 0){
        canvas.setTextColor(ST77XX_CYAN);
        canvas.print("*");
        canvas.setTextColor(ST77XX_WHITE);
    }
    else {
        canvas.print(" ");
    }
     canvas.println("]");
    

    canvas.print(blancs);
    canvas.print("   [");
    if (btBas == 0){
        canvas.setTextColor(ST77XX_CYAN);
        canvas.print("*");
        canvas.setTextColor(ST77XX_WHITE);
    }
    else {
        canvas.print(" ");
    }
     canvas.print("]");

     display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
   
}

//*********************************************
//*
//*       displayConfig
//*
//*********************************************
void handzDisplay::displayConfig()
{
    display.setTextColor(ST77XX_WHITE);
    display.setCursor(0, 20);
    display.println(getTranslatedString(KEY_SETTINGS));
    display.print("BTLE : ");
    display.setTextColor(ST77XX_GREEN);
    display.println(config3dHandz.btleDeviceName);
    display.setTextColor(ST77XX_WHITE);
    display.print(getTranslatedString(KEY_ANALOG_INPUTS)); 
    display.setTextColor(ST77XX_GREEN);
    display.println(config3dHandz.pinAnalogInputs);
     display.setTextColor(ST77XX_WHITE);
    display.print(getTranslatedString(KEY_VERSION));
     display.setTextColor(ST77XX_GREEN);
    display.println(VERSION_3DHANDZ);
}

//*********************************************
//*
//*       displayConfig2
//*
//*********************************************
void handzDisplay::displayConfig2()
{
    display.setTextColor(ST77XX_WHITE);
    display.setCursor(0, 20);
    display.println(getTranslatedString(KEY_SETTINGS));
    float tmp = getVbat();
    // map batterie level between 3,7V and 4,2V to %
    int charge = map(tmp * 100, 370, 420, 0, 100);
    if (charge > 100)
    {
        charge = 100;
    }
    else if (charge < 0)
    {
        charge = 0;
    }
    display.print("Vbat = ");
    display.setTextColor(ST77XX_MAGENTA);
    display.print(tmp);
     display.print("V (");
    display.print(charge);
    display.println("%)");
     display.setTextColor(ST77XX_YELLOW);
     display.print(getTranslatedString(KEY_BATTERY));
     display.setTextColor(ST77XX_MAGENTA);
     display.print("[");
    for (int i = 0; i <= 95; i += 12)
    {
        if (i <= charge)
        {
            display.print("*");
        }
        else
        {
            display.print(" ");
        }
    }
    display.println("]");
}

//*********************************************
//*
//*       displayLicences
//*
//*********************************************
void handzDisplay::displayLicences()
{
    display.setTextColor(ST77XX_WHITE);
    display.setCursor(5, 20);
    display.println(getTranslatedString(KEY_LICENCE));
}

//*********************************************
//*
//*       update
//*
//*********************************************
void handzDisplay::update(void)
{
    switch (displayMode)
    {
    case 0:
    case 30:
    case 32:
    case 36:
        // nothing to display
        break;
    case 1:
       display.fillScreen(ST77XX_BLACK);
        DisplayBanner();
        displayMode = 0;
        break;
    case 20:
        displayMode = 21;
        break;
    case 21:
       display.fillScreen(ST77XX_BLACK);
        capteurs(0);
        break;
    case 22:
        displayMode = 23;
        break;
    case 23:
        display.fillScreen(ST77XX_BLACK);
        capteurs(1);
        break;
    case 24:
        displayMode = 25;
        break;
    case 25:
        display.fillScreen(ST77XX_BLACK);
        capteurs(2);
        break;
    case 26:
        displayMode = 27;
        break;
    case 27:
        display.fillScreen(ST77XX_BLACK);
        joystick();
        break;
    case 31:
      display.fillScreen(ST77XX_BLACK);
        displayConfig();
        displayMode =  32;
        break;
    case 33:
       display.fillScreen(ST77XX_BLACK);
        displayConfig2();
        cptDisplayBatt = 0;
        displayMode = 34;
        break;
    case 34:
        
        if (cptDisplayBatt++ > 50)
        {
             displayConfig2();
            cptDisplayBatt = 0;
        }
        break;
    case 35:
        display.fillScreen(ST77XX_BLACK);
        displayLicences();
        displayMode = 36;
        break;
    default:
        Serial.println("Unknown display mode");
        displayMode = 0;
    }
    refreshDisplay();

}

//*********************************************
//*
//*       chackButtons
//*
//*********************************************
void handzDisplay::checkButtons(void)
{
    int buttonPressed = isButtonPressed();

    if (buttonPressed != 0)
    {
        if (buttonNotReleased == 0)
        {
            if (button_A_pressed == 1)
            {
                displayMode = 1;
                Serial.println("Banner screen");

            } else 
            if (button_B_pressed == 1)
            {
                Serial.println("Sensor screen");
                switch (displayMode)
                {
                case 21: // Display capteur 1 curve
                    displayMode = 22;
                    break;
                case 23: // Display capteur 2 curve
                    displayMode = 24;
                    break;
                case 25: // Display joystick
                    displayMode = 26;
                    break;
                default: // Display 2 capteurs curves
                    displayMode = 20;
                }
               
            } else 
            if (button_C_pressed == 1)
            {
                Serial.println("Info screen");
                switch (displayMode)
                {
                case 31: // nothing to do
                case 33: // nothing to do
                case 35: // nothing to do
                    break;
                case 32: // display 1st screen (Parameters)
                    displayMode = 33;
                    break;
                case 34: // display 2nd screen (Battery)
                    displayMode = 35;
                    break;
                case 36: // display 3rd screen (Licences)
                    displayMode = 31;
                    break;
                default: // first press of C button
                    displayMode = 31;
                    break;
                }
            }
            buttonNotReleased = 1;
        }
    }
    else
    {
        buttonNotReleased = 0;
    }
}

// key are defined in canvas.hpp
String EN_Strings[] = {"Sensor ", "Used channels:", "Settings", "Version: ", "Battery: ", "Licence"};
String FR_Strings[] = {"Capteur ", "Canaux utilises : ", "Parametres", "Version : ", "Batterie : ", "Licence"};
String ES_Strings[] = {"Sensor ", "Canales usados : ", "Configuraciones", "Version: ", "Batería: ", "Licencia"};
String DE_Strings[] = {"Sensor ", "Benutzte Kanäle : ", "Parameter", "Version: ", "Batterie: ", "Lizenz"};
//*********************************************
//*
//*       getTranslatedString
//*
//*********************************************
String handzDisplay::getTranslatedString(int key)
{
    // get the translated value of string identified by the key
    String language = config3dHandz.language;
    if (language.equals("FR"))
    {
        return FR_Strings[key];
    }
    else if (language.equals("EN"))
    {
        return EN_Strings[key];
    }
    else if (language.equals("ES"))
    {
        return ES_Strings[key];
    }
    else if (language.equals("DE"))
    {
        return DE_Strings[key];
    }
    return EN_Strings[key];
}
