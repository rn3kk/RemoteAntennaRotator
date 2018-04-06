#include <QApplication>
#include <QThread>

#include "Settings.h"
#include "Log/Log.h"
#include "DataExchange.h"
#include "ProtocolUtil.h"
#include "MVP/Presentor.h"
#include "MessageBus.h"
#include "EncoderMonitor.h"

int main(int argc, char *argv[])
{
  ProtocolUtil pu;
  if(!Log::initialize("Config/log4cpp.ini"))
  {
    std::cout << "Error load Config/log4cpp.ini";
    return 0;
  }
  Settings* settings = Settings::getInstance();
  if(settings == 0x0)
  {
    Log::loggerRoot.error("Can't load settings");
    return 0;
  }

  QApplication a(argc, argv);

  MessageBus* messageBus = MessageBus::getInstance();

  Presentor p;
  QWidget* w = p.getView();
  w->show();

  QObject::connect(messageBus, SIGNAL(encoderData(void*)), &p, SLOT(dataFromEncoder(void*)));


  QThread* encoderMonitorThread = new QThread();
  EncoderMonitor* em = new EncoderMonitor();
  em->moveToThread(encoderMonitorThread);

  QObject::connect(encoderMonitorThread, SIGNAL(started()), em, SLOT(process()));
  QObject::connect(em, SIGNAL(finished()), encoderMonitorThread, SLOT(quit()));
  QObject::connect(em, SIGNAL(finished()), em, SLOT(deleteLater()));
  QObject::connect(encoderMonitorThread, SIGNAL(finished()), encoderMonitorThread, SLOT(deleteLater()));
  QObject::connect(em, SIGNAL(encoderData(void*)), messageBus, SIGNAL(encoderData(void*)));  
  encoderMonitorThread->start();

  a.exec();
  em->setTerminate();

  while(!encoderMonitorThread->wait(10))
  {
    QApplication::processEvents(QEventLoop::AllEvents, 10);
  }

  return 0;
}
