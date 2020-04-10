#ifndef PRESENTOR_H
#define PRESENTOR_H

#include <QObject>

#include "Model.h"
#include "AzElView.h"

class Servo;

class Presentor : public QObject
{
  Q_OBJECT
public:
  explicit Presentor(Servo* servo, QObject *parent = 0);

  QWidget* getView();

signals:
  void viewIsClose();

public slots:
  void dataFromEncoder(void*);
  void encoderChangeSate(int state);
  void azIsChanged(float az);
  void elIsChanged(float el);

private:
  Model m_model;
  AngleView m_view;
  Servo* m_servo;
};

#endif // PRESENTOR_H
