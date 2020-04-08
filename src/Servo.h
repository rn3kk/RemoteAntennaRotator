#ifndef REMOTEENCODER_H
#define REMOTEENCODER_H

#include <QString>

class TcpUartModule;

class Servo
{
public:
  Servo();
  Servo(const QString& name, const QByteArray& encoderAddress, const QByteArray& rotatorAddress, int angleShift, TcpUartModule* tcpModule);

  QString name() const;
  void getAngle() const;
  unsigned int getEncoderAddress() const;
  unsigned int getRotatorAddress() const;
  int getAngleShift() const;

private:
  QString m_name;
  QByteArray m_encoderAddress;
  QByteArray m_rotatorAddress;
  int m_angleShift;
  TcpUartModule* m_tcpUartModule;

  QByteArray m_angleRequestBinaryData;
};

#endif // REMOTEENCODER_H
