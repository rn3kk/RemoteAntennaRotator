#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QByteArray>

class TcpUartModule;

class Encoder : public QObject
{
  Q_OBJECT
public:
  Encoder();
  Encoder(unsigned char address, int angleShift, TcpUartModule* tcpModule, QObject* parent = nullptr);

  int getAddress() const;
  int getAngleShift() const;

protected:
  void timerEvent(QTimerEvent *event) override;

private:
  unsigned char m_address;
  int m_angleShift;
  TcpUartModule* m_tcpUartModule;
  QByteArray m_angleRequestBinaryData;
};

#endif // ENCODER_H
