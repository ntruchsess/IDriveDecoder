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

#ifndef IDRIVEDECODER_H_
#define IDRIVEDECODER_H_

/* CAN-message format:
 *
 * 0   Counter (1 byte)
 *
 * 1,2 Rotary Pos (2 byte) low, high
 *
 * 3   Knob (1 byte)
 *
 * press: 0x01 - 0000 0001
 * long:  0x02 - 0000 0010
 *
 * left:  0xA0 - 1010 0000
 * long:  0xB0 - 1011 0000
 *
 * up:    0x10 - 0001 0000
 * long:  0x20 - 0010 0000
 *
 * right: 0x40 - 0100 0000
 * long:  0x50 - 0101 0000
 *
 * down:  0x70 - 0111 0000
 * long:  0x80 - 1000 0000
 *
 * 4-7 Buttons (4 byte)
 *
 * Media:   0x00 0x00 0xC1 0xF8 - 0000 0000 0000 0000 1100 0001 1111 1000
 * long:    0x00 0x00 0xC2 0xF8 - 0000 0000 0000 0000 1100 0010 1111 1000
 *
 * Menu:    0x04 0x00 0xC0 0xF8 - 0000 0100 0000 0000 1100 0000 1111 1000
 * long:    0x08 0x00 0xC0 0xF8 - 0000 1000 0000 0000 1100 0000 1111 1000
 *
 * Map:     0x00 0x00 0xC0 0xF9 - 0000 0000 0000 0000 1100 0000 1111 1001
 * long:    0x00 0x00 0xC0 0xFA - 0000 0000 0000 0000 1100 0000 1111 1010
 *
 * Com:     0x00 0x08 0xC0 0xF8 - 0000 0000 0000 1000 1100 0000 1111 1000
 * long:    0x00 0x10 0xC0 0xF8 - 0000 0000 0001 0000 1100 0000 1111 1000
 *
 * Nav:     0x00 0x00 0xC8 0xF8 - 0000 0000 0000 0000 1100 1000 1111 1000
 * long:    0x00 0x00 0xD0 0xF8 - 0000 0000 0000 0000 1101 0000 1111 1000
 *
 * Back:    0x20 0x00 0xC0 0xF8 - 0010 0000 0000 0000 1100 0000 1111 1000
 * long:    0x40 0x00 0xC0 0xF8 - 0100 0000 0000 0000 1100 0000 1111 1000
 *
 * Option:  0x00 0x01 0xC0 0xF8 - 0000 0000 0000 0001 1100 0000 1111 1000
 * long:    0x00 0x02 0xC0 0xF8 - 0000 0000 0000 0010 1100 0000 1111 1000
 *
 * Release: 0x00 0x00 0xC0 0xF8 - 0000 0000 0000 0000 1100 0000 1111 1000
 *
 * Mask:    0x6c 0x1b 0x1b 0x03 - 0110 1100 0001 1011 0001 1011 0000 0011
 */

#define IDRIVEDECODER_CENTER      1
#define IDRIVEDECODER_CENTER_EXT  2
#define IDRIVEDECODER_CENTER_REL  3
#define IDRIVEDECODER_LEFT        4
#define IDRIVEDECODER_LEFT_EXT    5
#define IDRIVEDECODER_LEFT_REL    6
#define IDRIVEDECODER_UP          7
#define IDRIVEDECODER_UP_EXT      8
#define IDRIVEDECODER_UP_REL      9
#define IDRIVEDECODER_RIGHT      10
#define IDRIVEDECODER_RIGHT_EXT  11
#define IDRIVEDECODER_RIGHT_REL  12
#define IDRIVEDECODER_DOWN       13
#define IDRIVEDECODER_DOWN_EXT   14
#define IDRIVEDECODER_DOWN_REL   15
#define IDRIVEDECODER_MEDIA      16
#define IDRIVEDECODER_MEDIA_EXT  17
#define IDRIVEDECODER_MEDIA_REL  18
#define IDRIVEDECODER_MENU       19
#define IDRIVEDECODER_MENU_EXT   20
#define IDRIVEDECODER_MENU_REL   21
#define IDRIVEDECODER_MAP        22
#define IDRIVEDECODER_MAP_EXT    23
#define IDRIVEDECODER_MAP_REL    24
#define IDRIVEDECODER_COM        25
#define IDRIVEDECODER_COM_EXT    26
#define IDRIVEDECODER_COM_REL    27
#define IDRIVEDECODER_NAV        28
#define IDRIVEDECODER_NAV_EXT    29
#define IDRIVEDECODER_NAV_REL    30
#define IDRIVEDECODER_BACK       31
#define IDRIVEDECODER_BACK_EXT   32
#define IDRIVEDECODER_BACK_REL   33
#define IDRIVEDECODER_OPTION     34
#define IDRIVEDECODER_OPTION_EXT 35
#define IDRIVEDECODER_OPTION_REL 36

class IDriveDecoder {
public:

  const void (&switchEvent)(const unsigned char&);
  const void (&rotaryEvent)(const short&);
  IDriveDecoder(const void (&switchEvent)(const unsigned char&), const void (&rotaryEvent)(const short&));
  void decode(const unsigned char* data);
  virtual ~IDriveDecoder();

private:
  unsigned char  lastCounter = 0xff;
  unsigned short lastPos     = 0x7fff;
  unsigned char  lastSwitch0 = 0;
  unsigned char  lastSwitch1 = 0;
  unsigned char  lastSwitch2 = 0;

  static const unsigned char centerBit3    = 0x01;
  static const unsigned char centerExtBit3 = 0x02;

  static const unsigned char dirMask3      = 0xf0;

  static const unsigned char leftBit3      = 0xa0;
  static const unsigned char leftExtBit3   = 0xb0;

  static const unsigned char upBit3        = 0x10;
  static const unsigned char upExtBit3     = 0x20;

  static const unsigned char rightBit3     = 0x40;
  static const unsigned char rightExtBit3  = 0x50;

  static const unsigned char downBit3      = 0x70;
  static const unsigned char downExtBit3   = 0x80;

  static const unsigned char menuBit4      = 0x04;
  static const unsigned char menuExtBit4   = 0x08;

  static const unsigned char backBit4      = 0x20;
  static const unsigned char backExtBit4   = 0x40;

  static const unsigned char comBit5       = 0x08;
  static const unsigned char comExtBit5    = 0x10;

  static const unsigned char optionBit5    = 0x01;
  static const unsigned char optionExtBit5 = 0x02;

  static const unsigned char mediaBit6     = 0x01;
  static const unsigned char mediaExtBit6  = 0x02;

  static const unsigned char navBit6       = 0x08;
  static const unsigned char navExtBit6    = 0x10;

  static const unsigned char mapBit7       = 0x01;
  static const unsigned char mapExtBit7    = 0x02;

  inline void reset(void) {
    lastCounter = 0xff;
    lastPos     = 0x7fff;
    lastSwitch0 = 0;
    lastSwitch1 = 0;
    lastSwitch2 = 0;
  }

  inline bool isCenter3( const unsigned char &data3 ) const {
    return data3 & centerBit3;
  }

  inline bool isCenterExt3( const unsigned char &data3 ) const {
    return data3 & centerExtBit3;
  }

  inline bool wasCenter(void) const {
    return lastSwitch0 & 0x80;
  }

  inline bool wasCenterExt(void) const {
    return lastSwitch0 & 0x40;
  }

  inline bool wasCenterOrExt(void) const {
    return lastSwitch0 & 0xc0;
  }

  inline void setLastCenter(void) {
    clearLastCenter();
    lastSwitch0 |= 0x80;
  }

  inline void setLastCenterExt(void) {
    clearLastCenter();
    lastSwitch0 |= 0x40;
  }

  inline void clearLastCenter(void) {
    lastSwitch0 &= 0x3f;
  }

  inline bool isLeft3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == leftBit3;
  }

  inline bool isLeftExt3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == leftExtBit3;
  }

  inline bool wasLeft(void) const {
    return lastSwitch0 & 0x20;
  }

  inline bool wasLeftExt(void) const {
    return lastSwitch0 & 0x10;
  }

  inline bool wasLeftOrExt(void) const {
    return lastSwitch0 & 0x30;
  }

  inline void setLastLeft(void) {
    clearLastLeft();
    lastSwitch0 |= 0x20;
  }

  inline void setLastLeftExt(void) {
    clearLastLeft();
    lastSwitch0 |= 0x10;
  }

  inline void clearLastLeft(void) {
    lastSwitch0 &= 0xcf;
  }

  inline bool isUp3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == upBit3;
  }

  inline bool isUpExt3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == upExtBit3;
  }

  inline bool wasUp(void) const {
    return lastSwitch0 & 0x08;
  }

  inline bool wasUpExt(void) const {
    return lastSwitch0 & 0x04;
  }

  inline bool wasUpOrExt(void) const {
    return lastSwitch0 & 0x0c;
  }

  inline void setLastUp(void) {
    clearLastUp();
    lastSwitch0 |= 0x08;
  }

  inline void setLastUpExt(void) {
    clearLastUp();
    lastSwitch0 |= 0x04;
  }

  inline void clearLastUp(void) {
    lastSwitch0 &= 0xf3;
  }

  inline bool isRight3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == rightBit3;
  }

  inline bool isRightExt3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == rightExtBit3;
  }

  inline bool wasRight(void) const {
    return lastSwitch0 & 0x02;
  }

  inline bool wasRightExt(void) const {
    return lastSwitch0 & 0x01;
  }

  inline bool wasRightOrExt(void) const {
    return lastSwitch0 & 0x03;
  }

  inline void setLastRight(void) {
    clearLastRight();
    lastSwitch0 |= 0x02;
  }

  inline void setLastRightExt(void) {
    clearLastRight();
    lastSwitch0 |= 0x01;
  }

  inline void clearLastRight(void) {
    lastSwitch0 &= 0xfc;
  }

  inline bool isDown3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == downBit3;
  }

  inline bool isDownExt3( const unsigned char &data3 ) const {
    return (data3 & dirMask3) == downExtBit3;
  }

  inline bool wasDown(void) const {
    return lastSwitch1 & 0x80;
  }

  inline bool wasDownExt(void) const {
    return lastSwitch1 & 0x40;
  }

  inline bool wasDownOrExt(void) const {
    return lastSwitch1 & 0xc0;
  }

  inline void setLastDown(void) {
    clearLastDown();
    lastSwitch1 |= 0x80;
  }

  inline void setLastDownExt(void) {
    clearLastDown();
    lastSwitch1 |= 0x40;
  }

  inline void clearLastDown(void) {
    lastSwitch1 &= 0x3f;
  }

  inline bool isMenu4( const unsigned char &data4 ) const {
    return data4 & menuBit4;
  }

  inline bool isMenuExt4( const unsigned char &data4 ) const {
    return data4 & menuExtBit4;
  }

  inline bool wasMenu(void) const {
    return lastSwitch1 & 0x20;
  }

  inline bool wasMenuExt(void) const {
    return lastSwitch1 & 0x10;
  }

  inline bool wasMenuOrExt(void) const {
    return lastSwitch1 & 0x30;
  }

  inline void setLastMenu(void) {
    clearLastMenu();
    lastSwitch1 |= 0x20;
  }

  inline void setLastMenuExt(void) {
    clearLastMenu();
    lastSwitch1 |= 0x10;
  }

  inline void clearLastMenu(void) {
    lastSwitch1 &= 0xcf;
  }

  inline bool isBack4( const unsigned char &data4 ) const {
    return data4 & backBit4;
  }

  inline bool isBackExt4( const unsigned char &data4 ) const {
    return data4 & backExtBit4;
  }

  inline bool wasBack(void) const {
    return lastSwitch1 & 0x08;
  }

  inline bool wasBackExt(void) const {
    return lastSwitch1 & 0x04;
  }

  inline bool wasBackOrExt(void) const {
    return lastSwitch1 & 0x0c;
  }

  inline void setLastBack(void) {
    clearLastBack();
    lastSwitch1 |= 0x08;
  }

  inline void setLastBackExt(void) {
    clearLastBack();
    lastSwitch1 |= 0x04;
  }

  inline void clearLastBack(void) {
    lastSwitch1 &= 0xf3;
  }

  inline bool isCom5( const unsigned char &data5 ) const {
    return data5 & comBit5;
  }

  inline bool isComExt5( const unsigned char &data5 ) const {
    return data5 & comExtBit5;
  }

  inline bool wasCom(void) const {
    return lastSwitch1 & 0x02;
  }

  inline bool wasComExt(void) const {
    return lastSwitch1 & 0x01;
  }

  inline bool wasComOrExt(void) const {
    return lastSwitch1 & 0x03;
  }

  inline void setLastCom(void) {
    clearLastCom();
    lastSwitch1 |= 0x02;
  }

  inline void setLastComExt(void) {
    clearLastCom();
    lastSwitch1 |= 0x01;
  }

  inline void clearLastCom(void) {
    lastSwitch1 &= 0xfc;
  }

  inline bool isOption5( const unsigned char &data5 ) const {
    return data5 & optionBit5;
  }

  inline bool isOptionExt5( const unsigned char &data5 ) const {
    return data5 & optionExtBit5;
  }

  inline bool wasOption(void) const {
    return lastSwitch2 & 0x80;
  }

  inline bool wasOptionExt(void) const {
    return lastSwitch2 & 0x40;
  }

  inline bool wasOptionOrExt(void) const {
    return lastSwitch2 & 0xc0;
  }

  inline void setLastOption(void) {
    clearLastOption();
    lastSwitch2 |= 0x80;
  }

  inline void setLastOptionExt(void) {
    clearLastOption();
    lastSwitch2 |= 0x40;
  }

  inline void clearLastOption(void) {
    lastSwitch2 &= 0x3f;
  }

  inline bool isMedia6( const unsigned char &data6 ) const {
    return data6 & mediaBit6;
  }

  inline bool isMediaExt6( const unsigned char &data6 ) const {
    return data6 & mediaExtBit6;
  }

  inline bool wasMedia(void) const {
    return lastSwitch2 & 0x20;
  }

  inline bool wasMediaExt(void) const {
    return lastSwitch2 & 0x10;
  }

  inline bool wasMediaOrExt(void) const {
    return lastSwitch2 & 0x30;
  }

  inline void setLastMedia(void) {
    clearLastMedia();
    lastSwitch2 |= 0x20;
  }

  inline void setLastMediaExt(void) {
    clearLastMedia();
    lastSwitch2 |= 0x10;
  }

  inline void clearLastMedia(void) {
    lastSwitch2 &= 0xcf;
  }

  inline bool isNav6( const unsigned char &data6 ) const {
    return data6 & navBit6;
  }

  inline bool isNavExt6( const unsigned char &data6 ) const {
    return data6 & navExtBit6;
  }

  inline bool wasNav(void) const {
    return lastSwitch2 & 0x08;
  }

  inline bool wasNavExt(void) const {
    return lastSwitch2 & 0x04;
  }

  inline bool wasNavOrExt(void) const {
    return lastSwitch2 & 0x0c;
  }

  inline void setLastNav(void) {
    clearLastNav();
    lastSwitch2 |= 0x08;
  }

  inline void setLastNavExt(void) {
    clearLastNav();
    lastSwitch2 |= 0x04;
  }

  inline void clearLastNav(void) {
    lastSwitch2 &= 0xf3;
  }

  inline bool isMap7( const unsigned char &data7 ) const {
    return data7 & mapBit7;
  }

  inline bool isMapExt7( const unsigned char &data7 ) const {
    return data7 & mapExtBit7;
  }

  inline bool wasMap(void) const {
    return lastSwitch2 & 0x02;
  }

  inline bool wasMapExt(void) const {
    return lastSwitch2 & 0x01;
  }

  inline bool wasMapOrExt(void) const {
    return lastSwitch2 & 0x03;
  }

  inline void setLastMap(void) {
    clearLastMap();
    lastSwitch2 |= 0x02;
  }

  inline void setLastMapExt(void) {
    clearLastMap();
    lastSwitch2 |= 0x01;
  }

  inline void clearLastMap(void) {
    lastSwitch2 &= 0xfc;
  }
};

#endif /* IDRIVEDECODER_H_ */
