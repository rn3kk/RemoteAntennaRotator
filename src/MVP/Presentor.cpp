#include "../ProtocolUtil.h"
#include "../Servo.h"
#include "Presentor.h"

Presentor::Presentor(Servo* servo, QObject *parent) :
  m_servo(servo),
  QObject(parent)
{
  QObject::connect(&m_view, SIGNAL(viewIsClosed()), this, SIGNAL(viewIsClose()));
}

QWidget *Presentor::getView()
{
  return &m_view;
}

void Presentor::dataFromEncoder(void *p)
{
  if(p != 0x0)
  {
    EncoderData* e = (EncoderData*)p;
    if(e->encoderAddress == m_servo->azEncoderAddress())
    {
      azIsChanged(e->deg);
      delete e;
    }
    else if(e->encoderAddress == m_servo->elEncoderAddress())
    {
      elIsChanged(e->deg);
      delete e;
    }
  }
}

void Presentor::encoderChangeSate(int state)
{

}

void Presentor::azIsChanged(float az)
{
  m_model.setAz(az);
  m_view.setAz(az);
}

void Presentor::elIsChanged(float el)
{
  m_model.setEl(el);
  m_view.setEl(el);
}
