#include "TcpUartModule.h"
#include "commonconst.h"
#include "ProtocolUtil.h"
#include "Rotator.h"

Rotator::Rotator():
  m_address(0)
{

}

Rotator::Rotator(const unsigned char address, TcpUartModule *tcpModule):
  m_address(address),
  m_tcpUartModule(tcpModule)
{
  if(address > 0)
  {
    { //TO LEFT
      unsigned char address = m_address;
      unsigned char length = 0x81;
      unsigned char data1 = 0x03;
      unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

      m_toLeftBinaryData.append(START_BYTE);
      m_toLeftBinaryData.append(address);
      m_toLeftBinaryData.append(length);
      m_toLeftBinaryData.append(data1);
      m_toLeftBinaryData.append(crc);
      m_toLeftBinaryData.append(STOP_BYTE);
    }
    {  //TO RIGHT
      unsigned char address = m_address;
      unsigned char length = 0x81;
      unsigned char data1 = 0x04;
      unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

      m_toRightBinaryData.append(START_BYTE);
      m_toRightBinaryData.append(address);
      m_toRightBinaryData.append(length);
      m_toRightBinaryData.append(data1);
      m_toRightBinaryData.append(crc);
      m_toRightBinaryData.append(STOP_BYTE);
    }
    {  //TO UP
      unsigned char address = m_address;
      unsigned char length = 0x81;
      unsigned char data1 = 0x06;
      unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

      m_toUpBinaryData.append(START_BYTE);
      m_toUpBinaryData.append(address);
      m_toUpBinaryData.append(length);
      m_toUpBinaryData.append(data1);
      m_toUpBinaryData.append(crc);
      m_toUpBinaryData.append(STOP_BYTE);
    }
    {  //TO DOWN
      unsigned char address = m_address;
      unsigned char length = 0x81;
      unsigned char data1 = 0x07;
      unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

      m_toUpBinaryData.append(START_BYTE);
      m_toUpBinaryData.append(address);
      m_toUpBinaryData.append(length);
      m_toUpBinaryData.append(data1);
      m_toUpBinaryData.append(crc);
      m_toUpBinaryData.append(STOP_BYTE);
    }
    { //STOP AZ
      unsigned char address = m_address;
      unsigned char length = 0x81;
      unsigned char data1 = 0x05;
      unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

      m_toStopAzBinaryData.append(START_BYTE);
      m_toStopAzBinaryData.append(address);
      m_toStopAzBinaryData.append(length);
      m_toStopAzBinaryData.append(data1);
      m_toStopAzBinaryData.append(crc);
      m_toStopAzBinaryData.append(STOP_BYTE);
    }
    { //STOP EL
      unsigned char address = m_address;
      unsigned char length = 0x81;
      unsigned char data1 = 0x08;
      unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

      m_toStopElBinaryData.append(START_BYTE);
      m_toStopElBinaryData.append(address);
      m_toStopElBinaryData.append(length);
      m_toStopElBinaryData.append(data1);
      m_toStopElBinaryData.append(crc);
      m_toStopElBinaryData.append(STOP_BYTE);
    }
  }
}

void Rotator::toLeft()
{
  m_tcpUartModule->writeData(m_toLeftBinaryData);
}

void Rotator::toRight()
{
  m_tcpUartModule->writeData(m_toRightBinaryData);
}

void Rotator::toUp()
{

}

void Rotator::toStopAz()
{
  m_tcpUartModule->writeData(m_toStopAzBinaryData);
}

void Rotator::toStopEl()
{
  m_tcpUartModule->writeData(m_toStopElBinaryData);
}

void Rotator::attChange(unsigned char att)
{
  unsigned char address = m_address;
  unsigned char length = 0x82;
  unsigned char data1 = 0x09;
  unsigned char data2 = att;

  unsigned char crc =ProtocolUtil::createCrc(length, address, data1, data2, 0x0);

  QByteArray arr;
  arr.append(START_BYTE);
  arr.append(address);
  arr.append(length);
  arr.append(data1);
  arr.append(data2);
  arr.append(crc);
  arr.append(STOP_BYTE);
  m_tcpUartModule->writeData(arr);
}
