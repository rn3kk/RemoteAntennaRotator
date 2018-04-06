#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <QObject>

class MessageBus: public QObject
{
  Q_OBJECT
public:
  static MessageBus* getInstance();
  static void resetInstance();

signals:
  void encoderData(void*);

private:
  MessageBus();
  ~MessageBus();

private:
  static MessageBus* m_instance;

};

#endif // MESSAGEBUS_H
