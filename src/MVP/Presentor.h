#ifndef PRESENTOR_H
#define PRESENTOR_H

#include <QObject>

#include "Model.h"
#include "AzElView.h"

class Presentor : public QObject
{
  Q_OBJECT
public:
  explicit Presentor(QObject *parent = 0);

  QWidget* getView();

signals:
  void viewIsClose();

public slots:
  void dataFromEncoder(void*);
  void encoderChangeSate(int state);
  void azIsChange(float az);
  void elIsChange(float el);

private:
  Model m_model;
  AzElView m_view;
};

#endif // PRESENTOR_H
