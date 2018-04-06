#include "../ProtocolUtil.h"
#include "Presentor.h"

Presentor::Presentor(QObject *parent) : QObject(parent)
{
  QObject::connect(&m_view, SIGNAL(viewIsClosed()), this, SIGNAL(viewIsClose()));
}

QWidget *Presentor::getView()
{
  return &m_view;
}

void Presentor::dataFromEncoder(void *p)
{
  if(p == 0x0) return;
  EncoderData* e = (EncoderData*)p;
  azIsChange(e->deg);
  delete e;
}

void Presentor::encoderChangeSate(int state)
{

}

void Presentor::azIsChange(float az)
{
  m_model.setAz(az);
  m_view.setAz(az);
}

void Presentor::elIsChange(float el)
{
  m_model.setEl(el);
  m_model.setEl(el);
}
