#ifndef TCPUARTMODULE_H
#define TCPUARTMODULE_H

#include <QString>

class QThread;
class DataExchange;

class TcpUartModule
{
public:
  TcpUartModule();
  TcpUartModule(const QString& id, const QString&  ip, int port, int maxWaitetime);
  ~TcpUartModule();

  void writeData(const QByteArray& data);

  QString id() const;

private:
  QString m_id;

  QThread* m_dataExchangeThread;
  DataExchange* m_dataExchange;
};

#endif // TCPUARTMODULE_H
