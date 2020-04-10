#ifndef ROTATOR_H
#define ROTATOR_H

#include <QString>

class TcpUartModule;

class Rotator
{
public:
  Rotator();
  Rotator(const unsigned char address, TcpUartModule* tcpModule);

  void toLeft();
  void toRight();
  void toUp();
  void toDown();
  void toStopAz();
  void toStopEl();

  void attChange(unsigned char att);

private:
  const unsigned char m_address;
  QByteArray m_toLeftBinaryData;
  QByteArray m_toRightBinaryData;
  QByteArray m_toUpBinaryData;
  QByteArray m_toDownBinaryData;
  QByteArray m_toStopAzBinaryData;
  QByteArray m_toStopElBinaryData;
  TcpUartModule* m_tcpUartModule;
};

#endif // ROTATOR_H
