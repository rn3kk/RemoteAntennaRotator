#include <QApplication>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

#include "Log/Log.h"
#include "MVP/Presentor.h"
#include "MessageBus.h"
#include "ProtocolUtil.h"
#include "ServoSettingsView.h"
#include "Settings.h"
#include "TcpSettingsView.h"

#include <QLoggingCategory>

void toLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  qInstallMessageHandler(toLog);

  QCoreApplication::setOrganizationName("Bi4Remote");
  QCoreApplication::setOrganizationDomain("bi4remote.ru");
  QCoreApplication::setApplicationName("Bi4AntennaRotator");

  ServoSettingsView ssw;
  ssw.show();

  TcpSettingsView tsv;
  tsv.show();

  // ProtocolUtil pu;

  // Settings* settings = Settings::getInstance();
  // if(settings == 0x0)
  // {
  //   qCritical("Can't load settings");
  //   return 0;
  // }
  // auto vs = settings->getServoSettings();

  // MessageBus* messageBus = MessageBus::getInstance();




  // QWidget* mainWidget = new QWidget();
  // QVBoxLayout* vbox = new QVBoxLayout();
  // for(Servo* servo: settings->getServoList())
  // {
  //   Presentor* p = new Presentor(servo);
  //   QWidget* w = p->getView();
  //   vbox->addWidget(w);
  //   QObject::connect(messageBus, SIGNAL(encoderData(void*)), p, SLOT(dataFromEncoder(void*)));
  // }

  // mainWidget->setLayout(vbox);
  // mainWidget->show();

  a.exec();

  return 0;
}
