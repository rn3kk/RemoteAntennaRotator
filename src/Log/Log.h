/*!\file Log.h
* \brief Log class declaration
*/

#ifndef LOG_H
#define LOG_H
// #include <QtMsgHandler>
#include <QtLogging>

void toLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif /* LOG_H */
