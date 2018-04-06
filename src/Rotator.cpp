#include "TcpUartModule.h"
#include "commonconst.h"
#include "ProtocolUtil.h"
#include "Rotator.h"

Rotator::Rotator()
{

}

Rotator::Rotator(const QByteArray &address, TcpUartModule *tcpModule):
  m_address(address),
  m_tcpUartModule(tcpModule)
{

}

void Rotator::toLeft()
{
  unsigned char address = m_address.at(0);
  unsigned char length = 0x81;
  unsigned char data1 = 0x03;
  unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

  QByteArray arr;
  arr.append(START_BYTE);
  arr.append(address);
  arr.append(length);
  arr.append(data1);
  arr.append(crc);
  arr.append(STOP_BYTE);
  m_tcpUartModule->writeData(arr);
}

void Rotator::toRight()
{
  unsigned char address = m_address.at(0);
  unsigned char length = 0x81;
  unsigned char data1 = 0x04;
  unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

  QByteArray arr;
  arr.append(START_BYTE);
  arr.append(address);
  arr.append(length);
  arr.append(data1);
  arr.append(crc);
  arr.append(STOP_BYTE);
  m_tcpUartModule->writeData(arr);
}

void Rotator::toStop()
{
  unsigned char address = m_address.at(0);
  unsigned char length = 0x81;
  unsigned char data1 = 0x05;
  unsigned char crc =ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);

  QByteArray arr;
  arr.append(START_BYTE);
  arr.append(address);
  arr.append(length);
  arr.append(data1);
  arr.append(crc);
  arr.append(STOP_BYTE);
  m_tcpUartModule->writeData(arr);
}

void Rotator::attChange(unsigned char att)
{
  unsigned char address = m_address.at(0);
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
