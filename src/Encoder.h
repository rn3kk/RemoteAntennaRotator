#ifndef REMOTEENCODER_H
#define REMOTEENCODER_H

#include <QString>

class TcpUartModule;

class Encoder
{
public:
  Encoder();
  Encoder(const QString& name, const QByteArray& address, int angleShift, TcpUartModule* tcpModule);

  QString name() const;
  void sendAzInfoRequest() const;
  unsigned int getAddress() const;
  int getAngleShift() const;

private:
  QString m_name;
  QByteArray m_address;  
  int m_angleShift;

  TcpUartModule* m_tcpUartModule;
};

#endif // REMOTEENCODER_H
