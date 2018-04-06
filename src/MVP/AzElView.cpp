#include <QVBoxLayout>
#include "../Settings.h"
#include "../Log/Log.h"

#include "AzElView.h"

AzElView::AzElView(QWidget *parent) : QWidget(parent)
{
  m_rotrator = Settings::getInstance()->getRotatorList().at(0);
  createView();
}

void AzElView::setAz(float az)
{  
  m_az->setText(QString::number(az));
}

void AzElView::setEl(float el)
{
  m_el->setText(QString::number(el));
}

void AzElView::hideAz()
{
  m_az->setText("");
}

void AzElView::hideEl()
{
  m_el->setText("");
}

void AzElView::closeEvent(QCloseEvent *event)
{
  emit viewIsClosed();
}

void AzElView::toLeftPressed()
{
  m_rotrator->toLeft();
}

void AzElView::toRightPressed()
{
  m_rotrator->toRight();
}

void AzElView::toEndPressed()
{
  if(dynamic_cast<QPushButton*>(sender())->isDown()) return;
  m_rotrator->toStop();
}

void AzElView::attChanged(const QString &attValue)
{
  Log::loggerRoot.debug("ATT is changed to " + attValue.toStdString());
  unsigned char value = attValue.toUInt();
  m_rotrator->attChange(value);
}

void AzElView::createView()
{
  QSize size(50,50);

  QHBoxLayout* hBox = new QHBoxLayout(this);

  m_attBox = new QSpinBox(dynamic_cast<QWidget*>(hBox));
  m_attBox->setRange(0, 30);
  m_attBox->stepBy(1);
  m_attBox->setValue(0);
  connect(m_attBox, SIGNAL(valueChanged(QString)), this, SLOT(attChanged(QString)));

  m_toLeft = new QPushButton(dynamic_cast<QWidget*>(hBox));
  m_toLeft->setAutoRepeat(true);
  m_toLeft->setAutoRepeatInterval(300);
  m_toLeft->setMinimumSize(size);
  m_toLeft->setText("<");
  connect(m_toLeft, SIGNAL(pressed()), this, SLOT(toLeftPressed()));
  connect(m_toLeft, SIGNAL(released()), this, SLOT(toEndPressed()));

  hBox->addWidget(m_attBox);
  hBox->addWidget(m_toLeft);

  m_toRight = new QPushButton(dynamic_cast<QWidget*>(hBox));
  m_toRight->setAutoRepeat(true);
  m_toRight->setAutoRepeatInterval(300);
  m_toRight->setMinimumSize(size);
  m_toRight->setText(">");
  connect(m_toRight, SIGNAL(pressed()), this, SLOT(toRightPressed()));
  connect(m_toRight, SIGNAL(released()), this, SLOT(toEndPressed()));


  QVBoxLayout* vbox = new QVBoxLayout(dynamic_cast<QWidget*>(hBox));

  m_az = new QLabel(dynamic_cast<QWidget*>(vbox));
  m_az->setText("-1");

  m_el = new QLabel(dynamic_cast<QWidget*>(vbox));
  m_el->setText("El:-1");

  vbox->addWidget(m_az);
  vbox->addWidget(m_el);
  QFont font("Arial", 20, QFont::Bold);
  m_az->setFont(font);
  m_el->setFont(font);

  hBox->addLayout(vbox);
  hBox->addWidget(m_toRight);
  setLayout(hBox);

}
