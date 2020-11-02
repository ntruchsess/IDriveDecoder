/*
 *   IDriveDecoder - Arduino library to decode CAN-Bus message of IDrive.
 *
 *   Copyright (C) 2020 Norbert Truchsess norbert.truchsess@t-online.de
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *                                            
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <mcp_can.h>
#include <SPI.h>
#include <IDriveDecoder.h>

// CAN TX Variables

// CAN RX Variables
long unsigned int rxId;
unsigned char len;
unsigned char rxBuf[8];

// CAN0 INT and CS
#define CAN0_INT 6                              // Set INT to pin 6
MCP_CAN CAN0(10);                               // Set CS to pin 10

//forward declaration
const void onSwitchEvent(const unsigned char&);  
const void onRotaryEvent(const short&);

IDriveDecoder IDrive(onSwitchEvent,onRotaryEvent);

// Serial Output String Buffer
char msgString[128];
unsigned char lastCounter = 0xff;
unsigned char diff = 0;

void setup()
{
  Serial.begin(115200);  // CAN is running at 500,000BPS; 115,200BPS is SLOW, not FAST, thus 9600 is crippling.
  
  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  // Since we do not set NORMAL mode, we are in loopback mode by default.
  CAN0.setMode(MCP_NORMAL);

  pinMode(CAN0_INT, INPUT);                           // Configuring pin for /INT input
  
  //Serial.println("MCP2515 Library Loopback Example...");
}

void loop()
{
  if(!digitalRead(CAN0_INT))                          // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);              // Read data: len = data length, buf = data byte(s)
    if(!(rxId & 0x80000000)) // Determine if ID is standard (11 bits) or extended (29 bits)
    {
//      std_message();
      switch (rxId)
      {
        case 0x25B:   //idrive rotary
        {
//          std_message();
          IDrive.decode(rxBuf);
        }
      }
    }
  }
}

const void onSwitchEvent(const unsigned char& eventId) {
//  Serial.print("EventId: ");
//  Serial.println(eventId);
  
  switch(eventId) {
    case IDRIVEDECODER_CENTER: {
      Serial.println("CENTER");
      break;
    }
    case IDRIVEDECODER_CENTER_EXT: {
      Serial.println("CENTER Long");
      break;
    }
    case IDRIVEDECODER_CENTER_REL: {
      Serial.println("CENTER Release");
      break;
    }
    case IDRIVEDECODER_LEFT: {
      Serial.println("LEFT");
      break;
    }
    case IDRIVEDECODER_LEFT_EXT: {
      Serial.println("LEFT Long");
      break;
    }
    case IDRIVEDECODER_LEFT_REL: {
      Serial.println("LEFT Release");
      break;
    }
    case IDRIVEDECODER_UP: {
      Serial.println("UP");
      break;
    }
    case IDRIVEDECODER_UP_EXT: {
      Serial.println("UP Long");
      break;
    }
    case IDRIVEDECODER_UP_REL: {
      Serial.println("UP Release");
      break;
    }
    case IDRIVEDECODER_RIGHT: {
      Serial.println("RIGHT");
      break;
    }
    case IDRIVEDECODER_RIGHT_EXT: {
      Serial.println("RIGHT Long");
      break;
    }
    case IDRIVEDECODER_RIGHT_REL: {
      Serial.println("RIGHT Release");
      break;
    }
    case IDRIVEDECODER_DOWN: {
      Serial.println("DOWN");
      break;
    }
    case IDRIVEDECODER_DOWN_EXT: {
      Serial.println("DOWN Long");
      break;
    }
    case IDRIVEDECODER_DOWN_REL: {
      Serial.println("DOWN Release");
      break;
    }
    case IDRIVEDECODER_MEDIA: {
      Serial.println("MEDIA");
      break;
    }
    case IDRIVEDECODER_MEDIA_EXT: {
      Serial.println("MEDIA Long");
      break;
    }
    case IDRIVEDECODER_MEDIA_REL: {
      Serial.println("MEDIA Release");
      break;
    }
    case IDRIVEDECODER_MENU: {
      Serial.println("MENU");
      break;
    }
    case IDRIVEDECODER_MENU_EXT: {
      Serial.println("MENU Long");
      break;
    }
    case IDRIVEDECODER_MENU_REL: {
      Serial.println("MENU Release");
      break;
    }
    case IDRIVEDECODER_MAP: {
      Serial.println("MAP");
      break;
    }
    case IDRIVEDECODER_MAP_EXT: {
      Serial.println("MAP Long");
      break;
    }
    case IDRIVEDECODER_MAP_REL: {
      Serial.println("MAP Release");
      break;
    }
    case IDRIVEDECODER_COM: {
      Serial.println("COM");
      break;
    }
    case IDRIVEDECODER_COM_EXT: {
      Serial.println("COM Long");
      break;
    }
    case IDRIVEDECODER_COM_REL: {
      Serial.println("COM Release");
      break;
    }
    case IDRIVEDECODER_NAV: {
      Serial.println("NAV");
      break;
    }
    case IDRIVEDECODER_NAV_EXT: {
      Serial.println("NAV Long");
      break;
    }
    case IDRIVEDECODER_NAV_REL: {
      Serial.println("NAV Release");
      break;
    }
    case IDRIVEDECODER_BACK: {
      Serial.println("BACK");
      break;
    }
    case IDRIVEDECODER_BACK_EXT: {
      Serial.println("BACK Long");
      break;
    }
    case IDRIVEDECODER_BACK_REL: {
      Serial.println("BACK Release");
      break;
    }
    case IDRIVEDECODER_OPTION: {
      Serial.println("OPTION");
      break;
    }
    case IDRIVEDECODER_OPTION_EXT: {
      Serial.println("OPTION Long");
      break;
    }
    case IDRIVEDECODER_OPTION_REL: {
      Serial.println("OPTION Release");
      break;
    }
  }
}

const void onRotaryEvent(const short& rotaryPos) {
  if (rotaryPos > 0) {
    Serial.print("CLOCKWISE: ");
  } else if (rotaryPos < 0) {
    Serial.print("COUNTERCLOCKWISE: ");
  }
  
  Serial.println(rotaryPos);
}

void std_message(void) {
  sprintf(msgString, "Standard ID: 0x%.3lX DLC: %1d  Data: 0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X 0x%.2X", rxId, len, rxBuf[0], rxBuf[1], rxBuf[2], rxBuf[3], rxBuf[4], rxBuf[5], rxBuf[6], rxBuf[7] );
  Serial.println(msgString);
}
