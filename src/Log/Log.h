/*!\file Log.h
* \brief Log class declaration
*/

#ifndef LOG_H
#define LOG_H

#include <QString>
#include <log4cpp/Category.hh>

class Log
{
public:

  static log4cpp::Category& loggerRoot;
  static bool initialize(const QString& pathToLoggerProp);

private:
  static bool checkForFolderExistance(const QString& pathToLogIni);

};

#endif /* LOG_H */
