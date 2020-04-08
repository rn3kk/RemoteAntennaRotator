#ifndef REMOTEENCODER_H
#define REMOTEENCODER_H

#include <QString>

class TcpUartModule;

class Servo
{
public:
  Servo();
  Servo(const QString& name, unsigned char encoderAddress, unsigned char rotatorAddress, int angleShift, TcpUartModule* tcpModule);

  QString name() const;
  void getAngle() const;
  unsigned char getEncoderAddress() const;
  unsigned char getRotatorAddress() const;
  int getAngleShift() const;

private:
  QString m_name;
  unsigned char m_encoderAddress;
  unsigned char m_rotatorAddress;
  int m_angleShift;
  TcpUartModule* m_tcpUartModule;

  QByteArray m_angleRequestBinaryData;
};

#endif // REMOTEENCODER_H
