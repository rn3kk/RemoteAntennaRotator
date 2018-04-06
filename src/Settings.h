#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include<QVector>
#include"Encoder.h"
#include"Rotator.h"
#include"TcpUartModule.h"

class Settings
{
public:
  static Settings* getInstance();
  static void resetInstance();

  bool loadSettings();

  const QVector<Encoder *> &getEncoderList() const;
  const QVector<TcpUartModule *> &getTcpModuleList() const;

  const QVector<Rotator *> getRotatorList() const;
  int getAngleShiftForEncoder(unsigned short address) const;

private:
  Settings();
  ~Settings();
  bool loadEncoder(const QSettings& settings, const QString& path);
  bool loadRotator(const QSettings& settings, const QString& path);
  bool loadTcpModule(const QSettings& settings, const QString& path);

private:
  static Settings* m_instance;
  QVector<Encoder*> m_encoderList;
  QVector<TcpUartModule*> m_tcpModuleList;
  QVector<Rotator*> m_rotatorList;

};

#endif // SETTINGS_H
