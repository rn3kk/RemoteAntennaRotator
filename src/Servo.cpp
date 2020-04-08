#include "TcpUartModule.h"
#include "CommonConst.h"
#include "ProtocolUtil.h"
#include "Servo.h"

Servo::Servo()
{

}

Servo::Servo(const QString &name, const QByteArray &encoderAddress, const QByteArray &rotatorAddress, int angleShift, TcpUartModule *tcpModule):
  m_name(name),
  m_rotatorAddress(rotatorAddress),
  m_angleShift(angleShift),
  m_tcpUartModule(tcpModule)
{
  unsigned char address = encoderAddress.at(0);
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

QString Servo::name() const
{
  return m_name;
}

void Servo::getAngle() const
{
  m_tcpUartModule->writeData(m_angleRequestBinaryData);
}

unsigned int Servo::getEncoderAddress() const
{
  if(m_encoderAddress.isNull() || m_encoderAddress.isEmpty()) return 0;
  return m_encoderAddress.at(0);
}

unsigned int Servo::getRotatorAddress() const
{
  if(m_rotatorAddress.isNull() || m_rotatorAddress.isEmpty()) return 0;
  return m_rotatorAddress.at(0);
}

int Servo::getAngleShift() const
{
  return m_angleShift;
}
