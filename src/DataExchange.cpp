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
  m_socket(0x0),
  m_state(IDLE)
{
  qDebug() << "DataExchange::DataExchange()";
  m_queueToWrite.clear();
  QObject::connect(this, SIGNAL(queueContainsData()), this, SLOT(writeDataToSocket()));
  MessageBus* bus = MessageBus::getInstance();
  QObject::connect(this,SIGNAL(dataFromEncoder(void*)), bus, SIGNAL(encoderData(void*)));
}

DataExchange::~DataExchange()
{
  qDebug() << "DataExchange::~DataExchange()";
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
    qDebug() << "emit queueContainsData()";
    emit queueContainsData();
  }
}

void DataExchange::connectedToHost()
{
  qDebug() << "Connected to host " <<  m_ip << ":" << QString::number(m_port) << " OK";
}

void DataExchange::disconnectedFromHost()
{
  qDebug() << "Disconected from host";
  connectToHost();
}

void DataExchange::socketChangeState(QAbstractSocket::SocketState state)
{
  qDebug() << "Socket change state to " << QString::number(state);
}

void DataExchange::socketError(QAbstractSocket::SocketError error)
{
  qCritical() << "Socket error code " << QString::number(error);
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
      qDebug() << "WaitForReadyRead timeout or read data is emty";
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
  qDebug() << QString("Start connect to host %1 port %2").arg(m_ip).arg(m_port);
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
      qCritical() << "Can't connect to device by ip:" <<  m_ip << ":" << QString::number(m_port) << " Socket error is: " << QString::number(error);
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
        qDebug() << "WaitForReadyRead timeout or read data is emty";
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
    qDebug() << "Socket is NULL";
    return;
  }

  m_state = IN_PROGRESS;
  while(m_queueToWrite.count() > 0)
  {
    if(m_socket->state() != QAbstractSocket::ConnectedState)
    {
      qDebug() << "Socket in no connected sate";
      goto EndWhile;
    }

    qDebug() << "Lock QUEUE mutex";
    {
      QMutexLocker q(&m_queue);
      qDebug() << "Lock QUEUE mutex OK";
      QByteArray ar = m_queueToWrite.dequeue();
      m_socket->write(ar, ar.size());
    }
    if(!m_socket->waitForBytesWritten(m_waiteTime))
    {
      qCritical() << "Can't write to socket: timeout";
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
          qDebug() << "WaitForReadyRead timeout or read data is emty";
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
  qDebug() << "UNLock SOCKET mutex";
  m_state = IDLE;
}

