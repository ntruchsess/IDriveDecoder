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

#include "IDriveDecoder.h"

IDriveDecoder::IDriveDecoder(const void (&switchEvent)(const unsigned char&),const void (&rotaryEvent)(const short&)):switchEvent(switchEvent),rotaryEvent(rotaryEvent) {
}

void IDriveDecoder::decode(const unsigned char* data) {

  const unsigned char &counter = data[0];

  if (counter == 0) {
    reset();
  }

  const unsigned char diff = counter - lastCounter;

  if (diff > 0x7f) {
    return;
  }

  lastCounter = counter;

  const unsigned short pos = data[2] << 8 | data[1];

  if (pos != lastPos) {
    rotaryEvent(pos-lastPos);
    lastPos = pos;
  }

  const unsigned char &data3 = data[3];

  if (isCenter3(data3)) {
    if (!wasCenter()) {
      setLastCenter();
      switchEvent(IDRIVEDECODER_CENTER);
    }
  } else {
    if (isCenterExt3(data3)) {
      if (!wasCenterExt()) {
        setLastCenterExt();
        switchEvent(IDRIVEDECODER_CENTER_EXT);
      }
    } else {
      if (wasCenterOrExt()) {
        clearLastCenter();
        switchEvent(IDRIVEDECODER_CENTER_REL);
      }
    }
  }

  if (isLeft3(data3)) {
    if (!wasLeft()) {
      setLastLeft();
      switchEvent(IDRIVEDECODER_LEFT);
    }
  } else {
    if (isLeftExt3(data3)) {
      if (!wasLeftExt()) {
        setLastLeftExt();
        switchEvent(IDRIVEDECODER_LEFT_EXT);
      }
    } else {
      if (wasLeftOrExt()) {
        clearLastLeft();
        switchEvent(IDRIVEDECODER_LEFT_REL);
      }
    }
  }

  if (isUp3(data3)) {
    if (!wasUp()) {
      setLastUp();
      switchEvent(IDRIVEDECODER_UP);
    }
  } else {
    if (isUpExt3(data3)) {
      if (!wasUpExt()) {
        setLastUpExt();
        switchEvent(IDRIVEDECODER_UP_EXT);
      }
    } else {
      if (wasUpOrExt()) {
        clearLastUp();
        switchEvent(IDRIVEDECODER_UP_REL);
      }
    }
  }

  if (isRight3(data3)) {
    if (!wasRight()) {
      setLastRight();
      switchEvent(IDRIVEDECODER_RIGHT);
    }
  } else {
    if (isRightExt3(data3)) {
      if (!wasRightExt()) {
        setLastRightExt();
        switchEvent(IDRIVEDECODER_RIGHT_EXT);
      }
    } else {
      if (wasRightOrExt()) {
        clearLastRight();
        switchEvent(IDRIVEDECODER_RIGHT_REL);
      }
    }
  }

  if (isDown3(data3)) {
    if (!wasDown()) {
      setLastDown();
      switchEvent(IDRIVEDECODER_DOWN);
    }
  } else {
    if (isDownExt3(data3)) {
      if (!wasDownExt()) {
        setLastDownExt();
        switchEvent(IDRIVEDECODER_DOWN_EXT);
      }
    } else {
      if (wasDownOrExt()) {
        clearLastDown();
        switchEvent(IDRIVEDECODER_DOWN_REL);
      }
    }
  }

  const unsigned char &data4 = data[4];

  if (isMenu4(data4)) {
    if (!wasMenu()) {
      setLastMenu();
      switchEvent(IDRIVEDECODER_MENU);
    }
  } else {
    if (isMenuExt4(data4)) {
      if (!wasMenuExt()) {
        setLastMenuExt();
        switchEvent(IDRIVEDECODER_MENU_EXT);
      }
    } else {
      if (wasMenuOrExt()) {
        clearLastMenu();
        switchEvent(IDRIVEDECODER_MENU_REL);
      }
    }
  }

  if (isBack4(data4)) {
    if (!wasBack()) {
      setLastBack();
      switchEvent(IDRIVEDECODER_BACK);
    }
  } else {
    if (isBackExt4(data4)) {
      if (!wasBackExt()) {
        setLastBackExt();
        switchEvent(IDRIVEDECODER_BACK_EXT);
      }
    } else {
      if (wasBackOrExt()) {
        clearLastBack();
        switchEvent(IDRIVEDECODER_BACK_REL);
      }
    }
  }

  const unsigned char &data5 = data[5];

  if (isCom5(data5)) {
    if (!wasCom()) {
      setLastCom();
      switchEvent(IDRIVEDECODER_COM);
    }
  } else {
    if (isComExt5(data5)) {
      if (!wasComExt()) {
        setLastComExt();
        switchEvent(IDRIVEDECODER_COM_EXT);
      }
    } else {
      if (wasComOrExt()) {
        clearLastCom();
        switchEvent(IDRIVEDECODER_COM_REL);
      }
    }
  }

  if (isOption5(data5)) {
    if (!wasOption()) {
      setLastOption();
      switchEvent(IDRIVEDECODER_OPTION);
    }
  } else {
    if (isOptionExt5(data5)) {
      if (!wasOptionExt()) {
        setLastOptionExt();
        switchEvent(IDRIVEDECODER_OPTION_EXT);
      }
    } else {
      if (wasOptionOrExt()) {
        clearLastOption();
        switchEvent(IDRIVEDECODER_OPTION_REL);
      }
    }
  }

  const unsigned char &data6 = data[6];

  if (isMedia6(data6)) {
    if (!wasMedia()) {
      setLastMedia();
      switchEvent(IDRIVEDECODER_MEDIA);
    }
  } else {
    if (isMediaExt6(data6)) {
      if (!wasMediaExt()) {
        setLastMediaExt();
        switchEvent(IDRIVEDECODER_MEDIA_EXT);
      }
    } else {
      if (wasMediaOrExt()) {
        clearLastMedia();
        switchEvent(IDRIVEDECODER_MEDIA_REL);
      }
    }
  }

  if (isNav6(data6)) {
    if (!wasNav()) {
      setLastNav();
      switchEvent(IDRIVEDECODER_NAV);
    }
  } else {
    if (isNavExt6(data6)) {
      if (!wasNavExt()) {
        setLastNavExt();
        switchEvent(IDRIVEDECODER_NAV_EXT);
      }
    } else {
      if (wasNavOrExt()) {
        clearLastNav();
        switchEvent(IDRIVEDECODER_NAV_REL);
      }
    }
  }

  const unsigned char &data7 = data[7];

  if (isMap7(data7)) {
    if (!wasMap()) {
      setLastMap();
      switchEvent(IDRIVEDECODER_MAP);
    }
  } else {
    if (isMapExt7(data7)) {
      if (!wasMapExt()) {
        setLastMapExt();
        switchEvent(IDRIVEDECODER_MAP_EXT);
      }
    } else {
      if (wasMapOrExt()) {
        clearLastMap();
        switchEvent(IDRIVEDECODER_MAP_REL);
      }
    }
  }
}


IDriveDecoder::~IDriveDecoder() {
  // TODO Auto-generated destructor stub
}
