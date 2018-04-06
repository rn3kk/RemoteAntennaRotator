/*!\file Log.cpp
* \brief Log class implementation
*/

#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <QSettings>
#include <QFileInfo>
#include <QDir>

#include "Log.h"

log4cpp::Category& Log::loggerRoot = log4cpp::Category::getInstance(std::string("root"));

/*log4cpp::Category& Log::loggerDocumentScannerPlugin = log4cpp::Category::getInstance(std::string("DocumentScanner Plugin"));
log4cpp::Category& Log::loggerDPFingerprintPlugin = log4cpp::Category::getInstance(std::string("DPFingerprint Plugin"));
log4cpp::Category& Log::loggerBarcodeScannerPlugin = log4cpp::Category::getInstance(std::string("BarcodeScanner Plugin"));
log4cpp::Category& Log::loggerAmpulaLabelPrinterPlugin = log4cpp::Category::getInstance(std::string("AmpulaPrinter Plugin"));
*/

bool Log::initialize(const QString& pathToLoggerProp)
{
  if (checkForFolderExistance(pathToLoggerProp))
    log4cpp::PropertyConfigurator::configure(pathToLoggerProp.toStdString());
  else
  {
    std::cout << "FATAL: cannot find folder to save logs to";
    return false;
  }
  return true;  
}

bool Log::checkForFolderExistance(const QString& pathToLogIni)
{
  QSettings settings(pathToLogIni, QSettings::IniFormat);
  QFile logIniFile(pathToLogIni);
  if(!logIniFile.exists())
    return false;

  QString logPath = settings.value("log4cpp.appender.A1.fileName").toString();
  QFileInfo fi (logPath);
  QDir dir (fi.absolutePath());
  return dir.exists();
}

