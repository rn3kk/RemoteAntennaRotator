#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>

#include "Log.h"

void toLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{  
  QString txt;
  switch (type) {
  case QtDebugMsg:
    txt = QString("[Debug] %1").arg(msg);
    break;
  case QtWarningMsg:
    txt = QString("[Warning] %1").arg(msg);
    break;
  case QtCriticalMsg:
    txt = QString("[Critical] %1").arg(msg);
    break;
  case QtFatalMsg:
    txt = QString("[Fatal] %1").arg(msg);
    break;
  case QtInfoMsg:
    txt = QString("[Info] %1").arg(msg);
    break;
  }
  QDateTime now =QDateTime::currentDateTime();
  txt = QString("%1 " + txt).arg(now.toString("dd.MM.yyyy HH.mm.ss"));

  auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (path.isEmpty())
  {
    qFatal("Cannot determine settings storage location");
    exit(1);
  }

  QDir d{path};
  if(!d.mkpath(d.absolutePath()))
  {
    qFatal() << "Cant create dir for save logs";
    exit(1);
  }

  QFile outFile(path + "/AntennaRotator.log");
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream ts(&outFile);
  ts << txt << "\r\n";
  if(outFile.isOpen())outFile.close();
}
