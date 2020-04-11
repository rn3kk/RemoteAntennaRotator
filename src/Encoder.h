#ifndef ENCODER_H
#define ENCODER_H

#include <QByteArray>

class TcpUartModule;
class Encoder
{
public:
  Encoder();
  Encoder(unsigned char address, int angleShift, TcpUartModule* tcpUartModule);

  int getAddress() const;
  int getAngleShift() const;
  void sendAngleRquest();

private:
  unsigned char m_address;
  int m_angleShift;  
  QByteArray m_angleRequestBinaryData;
  TcpUartModule* m_tcpUartModule;
};

#endif // ENCODER_H
