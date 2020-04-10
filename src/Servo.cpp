#include "TcpUartModule.h"
#include "Rotator.h"
#include "CommonConst.h"
#include "ProtocolUtil.h"
#include "Encoder.h"
#include "Servo.h"


Servo::Servo()
{

}

Servo::Servo(const QString &name, Encoder *azEncoder, Encoder *elEncoder, Rotator *rotator):
  m_name(name),
  m_azEncoder(azEncoder),
  m_elEncoder(elEncoder),
  m_rotator(rotator)
{

}

int Servo::azEncoderAddress()
{
  return m_azEncoder->getAddress();
}

int Servo::elEncoderAddress()
{
  return m_elEncoder->getAddress();
}

QString Servo::name() const
{
  return m_name;
}

void Servo::updateAngle()
{

}
