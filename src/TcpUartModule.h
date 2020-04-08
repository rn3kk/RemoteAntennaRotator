#ifndef TCPUARTMODULE_H
#define TCPUARTMODULE_H

#include <QString>

class QThread;
class DataExchange;

class TcpUartModule
{
public:
  TcpUartModule();
  TcpUartModule(const QString&  ip, int port, int maxWaitetime);
  ~TcpUartModule();

  void writeData(const QByteArray& data);

private:
  QThread* m_dataExchangeThread;
  DataExchange* m_dataExchange;
};

#endif // TCPUARTMODULE_H
