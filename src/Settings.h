#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include<QVector>
#include"Servo.h"
#include"Rotator.h"
#include"TcpUartModule.h"

class Settings
{
public:
  static Settings* getInstance();
  static void resetInstance();

  bool loadSettings();

  const QVector<Servo *> &getServoList() const;

  int getAngleShiftForEncoder(unsigned short address) const;

private:
  Settings();
  ~Settings();
  bool loadServo(const QSettings& settings, const QString& path);
  bool loadTcpModule(const QSettings& settings, const QString& path);

private:
  static Settings* m_instance;
  QVector<Servo*> m_servoList;
  TcpUartModule* m_tcpModule;
};

#endif // SETTINGS_H
