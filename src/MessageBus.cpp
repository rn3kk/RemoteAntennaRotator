#include "MessageBus.h"

MessageBus* MessageBus::m_instance=0x0;

MessageBus *MessageBus::getInstance()
{
  if(m_instance == 0x0)
  {
    m_instance = new MessageBus();
  }
  return m_instance;
}

void MessageBus::resetInstance()
{
  if(m_instance)
  {
    delete m_instance;
    m_instance = 0x0;
  }
}

MessageBus::MessageBus()
{

}

MessageBus::~MessageBus()
{

}

