#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QThread>

#include "iostream"
#include "Settings.h"
#include "Log/Log.h"
#include "DataExchange.h"
#include "ProtocolUtil.h"
#include "MVP/Presentor.h"
#include "MessageBus.h"
#include "EncoderMonitor.h"

#include <QLoggingCategory>

void toLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  qInstallMessageHandler(toLog);

  ProtocolUtil pu;

  Settings* settings = Settings::getInstance();
  if(settings == 0x0)
  {    
    qCritical("Can't load settings");
    return 0;
  }

  MessageBus* messageBus = MessageBus::getInstance();

  QWidget* mainWidget = new QWidget();
  QVBoxLayout* vbox = new QVBoxLayout();
  for(Servo* servo: settings->getServoList())
  {
    unsigned int address = servo->getEncoderAddress();
    Presentor* p = new Presentor(address);
    QWidget* w = p->getView();
    vbox->addWidget(w);
    QObject::connect(messageBus, SIGNAL(encoderData(void*)), p, SLOT(dataFromEncoder(void*)));
  }

  mainWidget->setLayout(vbox);
  mainWidget->show();


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
