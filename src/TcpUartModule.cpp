#include <QApplication>
#include <QThread>
#include "DataExchange.h"
#include "TcpUartModule.h"

TcpUartModule::TcpUartModule():
  m_dataExchangeThread(0x0),
  m_dataExchange(0x0)
{

}

TcpUartModule::TcpUartModule(const QString &ip, int port, int maxWaitetime)
{
  m_dataExchange = new DataExchange(ip, port, maxWaitetime);
  m_dataExchangeThread = new QThread();
  m_dataExchange->moveToThread(m_dataExchangeThread);
  QObject::connect(m_dataExchangeThread, SIGNAL(started()), m_dataExchange, SLOT(connectToHost()));
  m_dataExchangeThread->start();
}

TcpUartModule::~TcpUartModule()
{
  if(m_dataExchange)
  {
    m_dataExchange->disconnect();
    delete m_dataExchange;
  }
  if(m_dataExchangeThread)
  {
    m_dataExchangeThread->disconnect();
    m_dataExchangeThread->quit();
    while(!m_dataExchangeThread->wait(10))
    {
      QApplication::processEvents(QEventLoop::AllEvents, 10);
    }
    delete m_dataExchangeThread;
  }
}

void TcpUartModule::writeData(const QByteArray &data)
{
  m_dataExchange->addToWrite(data);
}
