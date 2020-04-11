#include <QTimerEvent>
#include "Servo.h"
#include "Settings.h"
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

void MessageBus::timerEvent(QTimerEvent *event)
{
  killTimer(event->timerId());
  QVector<Servo*>servoList = Settings::getInstance()->getServoList();
  for(Servo* s:servoList)
  {
    s->updateAngleRequest();
  }
  startTimer(300);
}

MessageBus::MessageBus()
{

}

MessageBus::~MessageBus()
{

}

