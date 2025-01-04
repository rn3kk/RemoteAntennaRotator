#ifndef SETTINGS_H
#define SETTINGS_H
#include <QObject>
#include<QVector>
#include"Servo.h"
#include"Rotator.h"
#include"TcpUartModule.h"

class QSettings;

struct ServoSettings
{
  QString name = "AziEl";
  unsigned char azEncoderAddress = 0xA0;
  unsigned char elEncoderAddress = 0xA1;
  unsigned char rotatorAddress = 0xA2;
  int azZeroAngle = 0;
  int elZeroAngle = 0;
};

struct TcpUartModuleSettings
{
  QString ip = "192.168.1.126";
  int port = 3000;
  int maxWaiteDataMs = 200;
};

class Settings: public QObject
{
  Q_OBJECT
public:
  static Settings* getInstance();
  static void resetInstance();
  const QVector<Servo *> &getServoList() const;
  const QVector<ServoSettings> getServoSettings();


private:
  Settings();
  ~Settings();
  bool loadServo(const QSettings& settings, const QString& path);
  bool loadTcpModule(const QSettings& settings, const QString& path);
  void testValues() const;

private:
  static Settings* m_instance;
  QVector<ServoSettings> m_servoSettingsList;
  TcpUartModuleSettings m_tcpUartModuleSettings;

  QVector<Servo*> m_servoList;
  TcpUartModule* m_tcpModule;

  QSettings* m_settings;
};

#endif // SETTINGS_H
