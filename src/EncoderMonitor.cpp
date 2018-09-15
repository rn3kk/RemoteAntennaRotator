#include <QApplication>
#include "Settings.h"
#include "DataExchange.h"
#include "ProtocolUtil.h"
#include "sleepthread.h"
#include "Log/Log.h"
#include "EncoderMonitor.h"

EncoderMonitor::EncoderMonitor():
  m_terminate(false)
{

}

void EncoderMonitor::process()
{
  const QVector<Encoder*>& encoderList = Settings::getInstance()->getEncoderList();  
  if(encoderList.count() == 0)
    emit finished();

  while(!m_terminate)
  {
    foreach (const Encoder* e, encoderList)
    {
      e->sendAzInfoRequest();
      ((SleepThread*)thread())->msleep(300);
      QApplication::processEvents(QEventLoop::AllEvents);
    }
  }
  emit finished();
}

void EncoderMonitor::setTerminate()
{
  qDebug() << "EncoderMonitor receive set terminate signal";
  m_terminate = true;
}

