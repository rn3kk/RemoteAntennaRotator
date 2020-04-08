#ifndef SPIROMETRDATAEXCHANGE_H
#define SPIROMETRDATAEXCHANGE_H

#include <QObject>
#include <QByteArray>
#include <QSemaphore>
#include <QQueue>
#include <QTcpSocket>
#include <QMutex>

class DataExchange : public QObject
{
  Q_OBJECT
public:
  DataExchange(const QString& ip, int port, int waiteTime);
  ~DataExchange();

signals:
  void dataFromEncoder(void*);
  void queueContainsData();
  void finished();

public slots:
  bool connectToHost();
  void addToWrite(const QByteArray& data);

private slots:
  void connectedToHost();
  void disconnectedFromHost();
  void socketChangeState(QAbstractSocket::SocketState state);
  void socketError(QAbstractSocket::SocketError error);
  void readyRead();
  void writeDataToSocket();

private:  
  enum ModuleSate
  {
    IN_PROGRESS,
    IDLE
  };

  QString m_ip; // device ip
  int m_port;
  int m_waiteTime;

  QTcpSocket* m_socket;
  QMutex m_mutex;
  QMutex m_queue;

  QQueue<QByteArray> m_queueToWrite;
  QQueue<QByteArray> m_queueRightData;

  ModuleSate m_state;

};

#endif // SPIROMETRDATAEXCHANGE_H
