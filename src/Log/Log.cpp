#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>

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
  QFile outFile("log.log");
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream ts(&outFile);
  ts << txt << "\r\n";
  if(outFile.isOpen())outFile.close();
}
