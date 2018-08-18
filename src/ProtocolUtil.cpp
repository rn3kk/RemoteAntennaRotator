#include <QString>
#include <QDebug>
#include <iostream>
#include "CommonConst.h"
#include "Settings.h"
#include "ProtocolUtil.h"


ProtocolUtil::ProtocolUtil()
{

  //char art[6] = {0x96, 0xA0, 0x81, 0x02, 0xDC, 0xA9};
  unsigned char c = createCrc(0x82, 0xA9, 0x09, 0x55, 0x0);
  c = createCrc(0x81, 0xA0, 0x02,0x0,0x0);
  c = createCrc(0x81, 0xA0, 0x0,0x0,0x0);
  int d = getAngle(300, 30);
  d = getAngle(100, 250);
  d = getAngle(200, 190);
  d = getAngle(300, 30);
  d = getAngle(300, 30);

}

EncoderData ProtocolUtil::getEncoderData(const QByteArray &data)
{

  if(data.isNull() || data.isNull() || data.length() != 8)
  {
    return EncoderData();
  }
  unsigned int address = data.at(1);

  int angleShift = Settings::getInstance()->getAngleShiftForEncoder(address);

  unsigned int val_16;
  unsigned char b1, b2, b3;
  b1 = (unsigned char)data.at(3);
  b2 = (unsigned char)data.at(4);
  b3 = (unsigned char)data.at(5);

  val_16 = (unsigned int) b1;
  val_16 <<= 8;
  val_16 |= (unsigned int)b2;
  val_16 = val_16 >> 4;


  // 0хB4 0х98 0х40 - информация из AS5045
  unsigned char status, tmp, i;
  unsigned int val_32(0);

  b1 = 0xb4;
  b2 = 0x98;
  b3 = 0x40;

  status = b3 >> 4;
  status &= 0x0F;
  tmp = b2 << 4;
  tmp &= 0xF0;
  status |= tmp;

  int tmp_val32(0);
  val_32 = b1;
  val_32 <<= 16;

  tmp_val32 = b2;
  tmp_val32 <<= 8;
  val_32 |= tmp_val32;
  tmp_val32 = b3;
  val_32 |= tmp_val32;


  if ((status == 0x84) || (status == 0x80))
  {  // магниты установлены правильно
    // проверка на четность
    tmp = 0;
    for(i=0;i<17;i++)
    {
      if(val_32 & 0x00800000) tmp++;
    }
    tmp &=0x01;
    if((tmp == 1) && ((b3 & 0x40) == 0x40))
    {
      // проверка на четность пройдена, данные верные
      return EncoderData(getAngle(val_16*0.087890625, angleShift), true, true);
    }
    if((tmp == 0) && ((b3 & 0x40) == 0))
    {
      // проверка на четность пройдена, данные верные
      return EncoderData(getAngle(val_16*0.087890625, angleShift), true, true);
    }
  }
  else
  {  //магнит установлен не правильно, данным верить нельзя
    return EncoderData(getAngle(val_16*0.087890625, angleShift), false, false);
  }
}

unsigned char ProtocolUtil::createCrc( const unsigned char datalen, const unsigned char address, const unsigned char data1, const unsigned char data2, const unsigned char data3)
{
  unsigned char crc_send;
  crc_send = datalen ^ address;

  if(datalen == 0x81 || datalen == 0x82 || datalen == 0x83)
  {
    crc_send ^= data1; // if exists
  }

  if(datalen == 0x82 || datalen == 0x83)
  {
    crc_send ^= data2; // if exists
  }
  if(datalen == 0x83)
  {
    crc_send ^= data3; // if exists
  }

  crc_send = ~crc_send;

  if ((crc_send == START_BYTE) || (crc_send == STOP_BYTE))

    crc_send++;
  return crc_send;
}

int ProtocolUtil::getAngle(int currentAngle, int angleShift)
{
  if (angleShift == 0)
    return currentAngle;
  int deg = currentAngle + angleShift;
  if(deg >= 360)
  {
    deg = deg - 360;
  }
  return deg;
}
