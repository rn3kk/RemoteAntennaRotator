#ifndef REMOTEENCODER_H
#define REMOTEENCODER_H

#include <QString>

class TcpUartModule;
class Encoder;
class Rotator;

class Servo
{
public:
  Servo();
  Servo(const QString& name, Encoder* azEncoder, Encoder* elEncoder, Rotator* m_rotator);

  int azEncoderAddress();
  int elEncoderAddress();

  QString name() const;

  void updateAngle();

private:
  QString m_name;
  Encoder* m_azEncoder;
  Encoder* m_elEncoder;
  Rotator* m_rotator;
};

#endif // REMOTEENCODER_H
