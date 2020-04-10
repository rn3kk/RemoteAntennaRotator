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
    Presentor* p = new Presentor(servo);
    QWidget* w = p->getView();
    vbox->addWidget(w);
    QObject::connect(messageBus, SIGNAL(encoderData(void*)), p, SLOT(dataFromEncoder(void*)));
  }

  mainWidget->setLayout(vbox);
  mainWidget->show();

  a.exec();

  return 0;
}
