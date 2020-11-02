IDriveDecoder
=====

This library decodes the CAN-Bus messages sent by a 7-Button i-Drive-controller with Touch as it is being used in 2017 BMW 5 Series.

## Usage

Basic usage of the library is as shown.

```
#include <mcp_can.h>
#include <SPI.h>
#include "IDriveDecoder.h"

// CAN RX Variables
long unsigned int rxId;
unsigned char len;
unsigned char rxBuf[8];

// CAN0 INT and CS
#define CAN0_INT 6
MCP_CAN CAN0(10);

//forward declaration
const void onSwitchEvent(const unsigned char&);  
const void onRotaryEvent(const short&);

IDriveDecoder IDrive(onSwitchEvent,onRotaryEvent);

void setup()
{
  Serial.begin(115200);
  CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
  CAN0.setMode(MCP_NORMAL);
  pinMode(CAN0_INT, INPUT);
}

void loop()
{
  if(!digitalRead(CAN0_INT))
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);
    if (rxId == 0x25B)
    {
      IDrive.decode(rxBuf);
    }
  }
}

const void onSwitchEvent(const unsigned char& eventId)
{
  switch(eventId)
  {
    case IDRIVEDECODER_CENTER:
    {
      Serial.println("CENTER");
      break;
    }
    case IDRIVEDECODER_CENTER_EXT:
    {
      Serial.println("CENTER Long");
      break;
    }
    case IDRIVEDECODER_CENTER_REL:
    {
      Serial.println("CENTER Release");
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
```
....

## Examples

- [IDriveController](https://github.com/ntruchsess/IDriveDecoder/blob/master/examples/IDriveController/IDriveController.ino)

## Details

Check out the [header file of the library](https://github.com/ntruchsess/IDriveDecoder/blob/master/src/IDriveDecoder.h) for a full list of functions and parameters available. If you have suggests on how to present this better please feel free to submit a PR!
