#ifndef ROTATOR_H
#define ROTATOR_H

#include <QString>

class TcpUartModule;

class Rotator
{
public:
  Rotator();
  Rotator(const QByteArray& address, TcpUartModule* tcpModule);

  void toLeft();
  void toRight();
  void toStop();

  void attChange(unsigned char att);

private:

private:
  QByteArray m_address;

  TcpUartModule* m_tcpUartModule;
};

#endif // ROTATOR_H
