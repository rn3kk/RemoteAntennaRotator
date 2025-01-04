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
  unsigned char m_address = 0xA0;
  int m_angleShift = 0;
  QByteArray m_angleRequestBinaryData;
  TcpUartModule* m_tcpUartModule = nullptr;
};

#endif // ENCODER_H
