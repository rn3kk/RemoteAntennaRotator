#include <QTimerEvent>
#include "TcpUartModule.h"
#include "CommonConst.h"
#include "ProtocolUtil.h"
#include "Encoder.h"

Encoder::Encoder()
{

}


Encoder::Encoder(unsigned char address, int angleShift, TcpUartModule *tcpUartModule):
  m_address(address),
  m_angleShift(angleShift),
  m_tcpUartModule(tcpUartModule)
{
  unsigned char length = 0x81;
  unsigned char data1 = 0x02;
  unsigned char crc = ProtocolUtil::createCrc(length, address, data1, 0x0, 0x0);
  m_angleRequestBinaryData.append(START_BYTE);
  m_angleRequestBinaryData.append(address);
  m_angleRequestBinaryData.append(length);
  m_angleRequestBinaryData.append(data1);
  m_angleRequestBinaryData.append(crc);
  m_angleRequestBinaryData.append(STOP_BYTE);
}

int Encoder::getAddress() const
{
  return m_address;
}

int Encoder::getAngleShift() const
{
  return m_angleShift;
}

void Encoder::sendAngleRquest()
{
  if(m_tcpUartModule)
    m_tcpUartModule->writeData(m_angleRequestBinaryData);
}
