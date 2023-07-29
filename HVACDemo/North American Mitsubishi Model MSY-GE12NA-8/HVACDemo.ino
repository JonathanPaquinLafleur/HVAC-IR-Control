/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * Version update Nov, 2014
 * Mitsubishi HVAC protocol added by Vincent Cruvellier.
 *  delaymicrosecond() modified to avoid limit.
 * Version update Dec, 2014
 * Panasonic HVAC protocol added by Mathieu Thomas. 
 * Version update Dec, 2015
 * Mitsubishi HVAC protocol enhancement by Vincent Cruvellier.
 *   added    void sendHvacMitsubishiFD(
      HvacMode                  HVAC_Mode,           // Example HVAC_HOT  HvacMitsubishiMode
      int                       HVAC_Temp,           // Example 21  (°c)
      HvacFanMode               HVAC_FanMode,        // Example FAN_SPEED_AUTO  HvacMitsubishiFanMode
      HvacVanneMode             HVAC_VanneMode,      // Example VANNE_AUTO_MOVE  HvacMitsubishiVanneMode
      HvacAreaMode              HVAC_AreaMode,       // Example AREA_AUTO
      HvacWideVanneMode         HVAC_WideMode,       // Example WIDE_MIDDLE
      int                       HVAC_PLASMA,          // Example true to Turn ON PLASMA Function
      int                       HVAC_CLEAN_MODE,      // Example false 
      int                       HVAC_ISEE,            // Example False
      int                       OFF                   // Example false to Turn ON HVAC / true to request to turn off
    );
 * sendHvacMitsubishiFD Not tested, i don't have this material -- if someone can confirm then please push a note.
 */

#include "IRremote2.h"
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

BridgeServer server;
IRsend irsend;

void setup()
{
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();  
}

void loop() {

  BridgeClient client = server.accept(); //check new clients
  if(client) {
    int power;
    int temp;
    HvacMode mode;
    HvacFanMode fanSpeed;
    HvacVanneMode vanne;

    client.readStringUntil('=');
    power = client.readStringUntil(',') != "on";
    client.readStringUntil('=');
    temp = client.readStringUntil(',').toInt();
    client.readStringUntil('=');
    switch(client.readStringUntil(',').toInt()){
       case 0:
            mode = HVAC_HOT;
            break;
       case 1:
            mode = HVAC_COLD;
            break;
       case 2:
            mode = HVAC_DRY;
            break;
       case 3:
            mode = HVAC_AUTO;
            break;
    }
    client.readStringUntil('=');
    switch(client.readStringUntil(',').toInt()){
       case 0:
            fanSpeed = FAN_SPEED_1;
            break;
       case 1:
            fanSpeed = FAN_SPEED_2;
            break;
       case 2:
            fanSpeed = FAN_SPEED_3;
            break;
       case 3:
            fanSpeed = FAN_SPEED_4;
            break;
       case 4:
            fanSpeed = FAN_SPEED_AUTO;
            break;
       case 5:
            fanSpeed = FAN_SPEED_SILENT;
            break;
    }
    client.readStringUntil('=');
    switch(client.readStringUntil(' ').toInt()){
       case 0:
            vanne = VANNE_AUTO;
            break;
       case 1:
            vanne = VANNE_H1;
            break;
       case 2:
            vanne = VANNE_H2;
            break;
       case 3:
            vanne = VANNE_H3;
            break;
       case 4:
            vanne = VANNE_H4;
            break;
       case 5:
            vanne = VANNE_H5;
            break;
       case 6:
            vanne = VANNE_AUTO_MOVE;
            break;
    }
    irsend.sendHvacMitsubishi(mode, temp, fanSpeed, vanne, power);
   
    client.stop();   
  }  
}
