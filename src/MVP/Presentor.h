#ifndef PRESENTOR_H
#define PRESENTOR_H

#include <QObject>

#include "Model.h"
#include "AzElView.h"

class Presentor : public QObject
{
  Q_OBJECT
public:
  explicit Presentor(unsigned int id, QObject *parent = 0);

  QWidget* getView();

signals:
  void viewIsClose();

public slots:
  void dataFromEncoder(void*);
  void encoderChangeSate(int state);
  void angleIsChanged(float az);

private:
  Model m_model;
  AngleView m_view;
  unsigned int m_id;
};

#endif // PRESENTOR_H
