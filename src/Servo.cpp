#include "TcpUartModule.h"
#include "CommonConst.h"
#include "ProtocolUtil.h"
#include "Servo.h"

Servo::Servo()
{

}

Servo::Servo(const QString &name, unsigned char encoderAddress, unsigned char rotatorAddress, int angleShift, TcpUartModule *tcpModule):
  m_name(name),
  m_encoderAddress(encoderAddress),
  m_rotatorAddress(rotatorAddress),
  m_angleShift(angleShift),
  m_tcpUartModule(tcpModule)
{
  unsigned char address = encoderAddress;
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

unsigned char Servo::getEncoderAddress() const
{
  return m_encoderAddress;
}

unsigned char Servo::getRotatorAddress() const
{
  return m_rotatorAddress;
}

int Servo::getAngleShift() const
{
  return m_angleShift;
}
