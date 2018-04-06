#ifndef ENCODERMONITOR_H
#define ENCODERMONITOR_H

#include <QObject>

class EncoderMonitor : public QObject
{
  Q_OBJECT
public:
  EncoderMonitor();

public slots:
  void process();
  void setTerminate();

signals:
  void encoderData(void*);
  void encoderStatus(void*);
  void finished();

private:
  bool m_terminate;
};

#endif // ENCODERMONITOR_H
