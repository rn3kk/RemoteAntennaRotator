#include <QApplication>
#include <QMutexLocker>
#include "ProtocolUtil.h"
#include "sleepthread.h"
#include "Log/Log.h"
#include "MessageBus.h"
#include "DataExchange.h"


static const QString XML_BEGIN_STRING = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";

DataExchange::DataExchange(const QString &ip, int port, int waiteTime):
  m_ip(ip),
  m_port(port),
  m_waiteTime(waiteTime),
  m_state(IDLE),
  m_socket(0x0)
{
  m_queueToWrite.clear();
  QObject::connect(this, SIGNAL(queueContainsData()), this, SLOT(writeDataToSocket()));
  MessageBus* bus = MessageBus::getInstance();
  QObject::connect(this,SIGNAL(dataFromEncoder(void*)), bus, SIGNAL(encoderData(void*)));
}

DataExchange::~DataExchange()
{
  Log::loggerRoot.debug("DataExchange::~DataExchange()");
  if(m_socket)
  {
    m_socket->abort();
    delete m_socket;
  }
}

void DataExchange::addToWrite(const QByteArray &data)
{  
  QMutexLocker l(&m_queue);
  if(m_queueToWrite.length() > 10)
  {
    m_queueToWrite.clear();
  }
  m_queueToWrite.enqueue(data);
  if(m_state == IDLE)
  {
    Log::loggerRoot.debug("emit queueContainsData()");
    emit queueContainsData();
  }
}

void DataExchange::connectedToHost()
{
  Log::loggerRoot.info("Connected to host " + m_ip.toStdString() + ":" + QString::number(m_port).toStdString() + " OK");
}

void DataExchange::disconnectedFromHost()
{
  Log::loggerRoot.debug("Disconected from host");
  connectToHost();
}

void DataExchange::socketChangeState(QAbstractSocket::SocketState state)
{
  Log::loggerRoot.debug("Socket change state to " + QString::number(state).toStdString());
}

void DataExchange::socketError(QAbstractSocket::SocketError error)
{
  Log::loggerRoot.error("Socket error code " + QString::number(error).toStdString());
  m_socket->disconnectFromHost();

}

void DataExchange::readyRead()
{
  QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

  QByteArray result;
  int numRead = 0;
  char buffer[2];
  memset(&buffer, 0, sizeof(char));
  forever //read answer from socket
  {
    numRead = socket->read(buffer, 2);

    if(numRead > 0)
    {
      result.append(buffer, numRead);
    }
    if(result.length() > 0 && result.at(result.length()-1) == (char)0xA9)
    {
      break;
    }
    if(numRead == 0 && !socket->waitForReadyRead(m_waiteTime))
    {
      Log::loggerRoot.debug("WaitForReadyRead timeout or read data is emty");
      break;
    }
  }
  if(result.isNull()|| result.isEmpty())
    return;
  ProtocolUtil util;
  EncoderData* d = new EncoderData(util.getEncoderData(result));
  emit dataFromEncoder(d);
  //Log::loggerRoot.debug("encoder data:" + QString::number(d.deg).toStdString());
}

bool DataExchange::connectToHost()
{
  Log::loggerRoot.debug("Start connect to host");
  if(m_socket != 0x0)
  {
    m_socket->disconnect();
    QTcpSocket* buf = m_socket;
    m_socket = 0x0;
    buf->close();
    buf->abort();
    buf->deleteLater();
  }
  m_socket = new QTcpSocket();
  m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
  QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(connectedToHost()));
  QObject::connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnectedFromHost()));
  QObject::connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketChangeState(QAbstractSocket::SocketState)));
  QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
  //QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

  int count = 0;
  m_socket->connectToHost(m_ip, m_port);
  forever
  {
    if(count = 10) return false;
    if(!m_socket->waitForConnected(m_waiteTime))
    {
      QAbstractSocket::SocketError error = m_socket->error();
      Log::loggerRoot.error("Can't connect to device by ip:" + m_ip.toStdString() + ":" + QString::number(m_port).toStdString() + " Socket error is: " + QString::number(error).toStdString() );
      QApplication::processEvents(QEventLoop::AllEvents, 100);
      ++count;
      continue;
    }
    break;
  }
  //read
  {
    QByteArray result;
    int numRead = 0;
    char buffer[2];
    memset(&buffer, 0, sizeof(char));
    forever //read answer from socket
    {
      numRead = m_socket->read(buffer, 2);

      if(numRead > 0)
      {
        result.append(buffer, numRead);
      }
      if(result.length() > 0 && result.at(result.length()-1) == (char)0xA9)
      {
        break;
      }
      if(numRead == 0 && !m_socket->waitForReadyRead(m_waiteTime))
      {
        Log::loggerRoot.debug("WaitForReadyRead timeout or read data is emty");
        break;
      }
    }
    ProtocolUtil util;
    EncoderData* d = new EncoderData(util.getEncoderData(result));
    emit dataFromEncoder(d);
  }

  return true;
}

void DataExchange::writeDataToSocket()
{
  if(m_socket == 0x0)
  {
    Log::loggerRoot.debug("Socket is NULL");
    return;
  }

  m_state = IN_PROGRESS;
  while(m_queueToWrite.count() > 0)
  {
    if(m_socket->state() != QAbstractSocket::ConnectedState)
    {
      Log::loggerRoot.debug("Socket in no connected sate");
      goto EndWhile;
    }

    Log::loggerRoot.debug("Lock QUEUE mutex");
    {
      QMutexLocker q(&m_queue);
      Log::loggerRoot.debug("Lock QUEUE mutex OK");
      QByteArray ar = m_queueToWrite.dequeue();
      m_socket->write(ar, ar.size());
    }
    if(!m_socket->waitForBytesWritten(m_waiteTime))
    {
      Log::loggerRoot.error("Can't write to socket: timeout");
    }

    //read
    {
      QByteArray result;
      int numRead = 0;
      char buffer[2];
      memset(&buffer, 0, sizeof(char));
      forever //read answer from socket
      {
        numRead = m_socket->read(buffer, 2);

        if(numRead > 0)
        {
          result.append(buffer, numRead);
        }
        if(result.length() > 0 && result.at(result.length()-1) == (char)0xA9)
        {
          break;
        }
        if(numRead == 0 && !m_socket->waitForReadyRead(m_waiteTime))
        {
          Log::loggerRoot.debug("WaitForReadyRead timeout or read data is emty");
          break;
        }
      }
      ProtocolUtil util;
      EncoderData* d = new EncoderData(util.getEncoderData(result));
      emit dataFromEncoder(d);
    }
EndWhile:
    QApplication::processEvents(QEventLoop::AllEvents, 10);
    //((SleepThread*)thread())->msleep(1000);
  }
  Log::loggerRoot.debug("UNLock SOCKET mutex");
  m_state = IDLE;
}

