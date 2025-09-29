// // ******************************************
// // * Baah Box Arduino : Sensor BTLE gateway *
// // ******************************************

// // Copyright (C) 2017 – 2023 Orange SA

// // This program is free software: you can redistribute it and/or modify
// // it under the terms of the GNU General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.

// // This program is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// // GNU General Public License for more details.

// // You should have received a copy of the GNU General Public License
// // along with this program. If not, see <http://www.gnu.org/licenses/>.
// /*
// #include <SPI.h>
// #include "btle.hpp"
// #include <bluefruit.h>

 
// // BLE Service
// BLEDfu  bledfu;  // OTA DFU service
// BLEDis  bledis;  // device information
// BLEUart bleuart; // uart over ble
// BLEBas  blebas;  // battery



// /* create the ble object */




// //*********************************************
// //*
// //*       Constructor
// //*
// //*********************************************
// /* btleClass::btleClass()
// {
//   btleBufferInputsIndex = 0;
//   for (int i = 0; i < BTLE_MAX_BUFFER_SIZE; i++)
//   {
//     btleBufferInputs[i] = 0;
//   }
// }
// */
// //*********************************************
// //*
// //*       init
// //*
// //*********************************************
//  void btleClass::init(char *inputDeviceName)
// {
//   /* Initialise the module */
//   memset(deviceName, 0, BTLE_MAX_DEVICE_NAME);
//   strncpy(deviceName, inputDeviceName, BTLE_MAX_DEVICE_NAME - 1);

//   Serial.println("begin of btle Init");
//   if (!ble.begin(VERBOSE_MODE))
//   {
//     Serial.println(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
//   }

//   if (FACTORYRESET_ENABLE)
//   {
//     /* Perform a factory reset to make sure everything is in a known state */
//     if (!ble.factoryReset())
//     {
//       Serial.println(F("Couldn't factory reset"));
//     }
//   }

//   /* Disable command echo from Bluefruit */
//   ble.echo(true);

//   /* Print Bluefruit information */
//   ble.info();
//   ble.verbose(false); // debug info is a little annoying after this point!

//   char tmpChar[BTLE_MAX_DEVICE_NAME + 30];

//   sprintf(tmpChar, "AT+GAPDEVNAME=%s", deviceName);

//   ble.sendCommandCheckOK(tmpChar);

//   Serial.println("wait for connection");
//   /* Wait for connection */

//   configured = false;
// }
// */

// //*********************************************
// //*
// //*       write
// //*
// //*********************************************
// void btleClass::write(char *data, int dataLength)
// {
//   if (ble.isConnected())
//   {
//     if (configured == false)
//     {
//       configured = true;
//       // LED Activity command is only supported from 0.6.6
//       if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
//       {
//         // Change Mode LED Activity
//         ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
//       }
//       // Set module to DATA mode
//       ble.setMode(BLUEFRUIT_MODE_DATA);
//     }
//     ble.write(data, dataLength);
//   }
// }

// /**************************************************************************/
// /*!
//   @brief  Constantly poll for new command or response data
// */
// /**************************************************************************/
// int btleClass::read(char *command)
// {
//   if (ble.isConnected())
//   {
//     if (configured == false)
//     {
//       configured = true;
//       Serial.println("Btle read");
//       // LED Activity command is only supported from 0.6.6
//       if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
//       {
//         // Change Mode LED Activity
//         ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
//       }

//       // Set module to DATA mode
//       ble.setMode(BLUEFRUIT_MODE_DATA);
//     }

//     // Echo received data
//     while (ble.available())
//     {
//       char c = (char)ble.read();
//       if (btleBufferInputsIndex < BTLE_MAX_BUFFER_SIZE)
//       {
//         btleBufferInputs[btleBufferInputsIndex] = c;
//         btleBufferInputsIndex++;
//       }
//       if (c == BTLE_END_OF_CHAR)
//       {
//         memcpy(command, btleBufferInputs, btleBufferInputsIndex + 1);
//         int output = btleBufferInputsIndex;
//         btleBufferInputsIndex = 0;
//         if (output < BTLE_MAX_BUFFER_SIZE)
//         {
//           return output;
//         }
//         else
//         {
//           return 0;
//         }
//       }
//     }
//     return 0;
//   }
//   else
//   {
//     return 0;
//   }
// }
// */