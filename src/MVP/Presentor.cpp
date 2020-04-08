#include "../ProtocolUtil.h"
#include "Presentor.h"

Presentor::Presentor(unsigned int id, QObject *parent) :
  m_id(id),
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
    if(e->encoderAddress == m_id)
    {
      angleIsChanged(e->deg);
      delete e;
    }
  }
}

void Presentor::encoderChangeSate(int state)
{

}

void Presentor::angleIsChanged(float az)
{
  m_model.setAz(az);
  m_view.setAz(az);
}
