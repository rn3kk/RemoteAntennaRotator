#ifndef SLEEPTHREAD_H
#define SLEEPTHREAD_H

#include <QThread>

class SleepThread : public QThread
{
public:
  void msleep(int time)
  {
    QThread::msleep(time);
  }
};

#endif // SLEEPTHREAD_H
