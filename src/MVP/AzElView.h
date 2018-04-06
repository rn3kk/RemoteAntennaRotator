#ifndef AZELVIEW_H
#define AZELVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>

class Rotator;

class AzElView : public QWidget
{
  Q_OBJECT
public:
  explicit AzElView(QWidget *parent = 0);

  void setAz(float az);
  void setEl(float el);

  void hideAz();
  void hideEl();

signals:
  void viewIsClosed();  

public slots:
  void closeEvent(QCloseEvent *event);  

  void toLeftPressed();
  void toRightPressed();
  void toEndPressed();
  void attChanged(const QString& attValue);

private:
  void createView();
private:
  QLabel* m_az;
  QLabel* m_el;
  QPushButton* m_toLeft;
  QPushButton* m_toRight;
  QPushButton* m_toUp;
  QPushButton* m_toDown;  

  QSpinBox* m_attBox;
  Rotator* m_rotrator;
};

#endif // AZELVIEW_H
