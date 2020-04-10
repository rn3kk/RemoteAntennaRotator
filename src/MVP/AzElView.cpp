#include <QVBoxLayout>
#include <QLoggingCategory>
#include "../Settings.h"
#include "../Log/Log.h"

#include "AzElView.h"

AngleView::AngleView(QWidget *parent) : QWidget(parent)
{
  createView();
}

void AngleView::setAz(float az)
{  
  m_az->setText(QString::number(az));
}

void AngleView::setEl(float el)
{
  m_el->setText(QString::number(el));
}

void AngleView::hideAz()
{
  m_az->setText("");
}

void AngleView::hideEl()
{
  m_el->setText("");
}

void AngleView::closeEvent(QCloseEvent *event)
{
  emit viewIsClosed();
}

void AngleView::toLeftPressed()
{

}

void AngleView::toRightPressed()
{
}

void AngleView::toEndPressed()
{

}

void AngleView::attChanged(const QString &attValue)
{

}

void AngleView::createView()
{
  QSize size(50,50);

  QHBoxLayout* hBox = new QHBoxLayout(this);

  QVBoxLayout* first = new QVBoxLayout(this);
  QVBoxLayout* second = new QVBoxLayout(this);
  QVBoxLayout* third = new QVBoxLayout(this);

  m_toLeft = new QPushButton(dynamic_cast<QWidget*>(first));
  m_toLeft->setAutoRepeat(true);
  m_toLeft->setAutoRepeatInterval(300);
  m_toLeft->setMinimumSize(size);
  m_toLeft->setText("<");
  connect(m_toLeft, SIGNAL(pressed()), this, SLOT(toLeftPressed()));
  connect(m_toLeft, SIGNAL(released()), this, SLOT(toEndPressed()));

  m_toUp = new QPushButton(dynamic_cast<QWidget*>(first));
  m_toUp->setAutoRepeat(true);
  m_toUp->setAutoRepeatInterval(300);
  m_toUp->setMinimumSize(size);
  m_toUp->setText("^");
  connect(m_toUp, SIGNAL(pressed()), this, SLOT(toLeftPressed()));
  connect(m_toUp, SIGNAL(released()), this, SLOT(toEndPressed()));
  first->addWidget(m_toLeft);
  first->addWidget(m_toUp);

  QFont font("Arial", 20, QFont::Bold);
  m_az = new QLabel(dynamic_cast<QWidget*>(second));
  m_az->setText("-1");
  m_az->setFont(font);
  m_el = new QLabel(dynamic_cast<QWidget*>(second));
  m_el->setText("-1");
  m_el->setFont(font);

  second->addWidget(m_az);
  second->addWidget(m_el);

  m_toRight = new QPushButton(dynamic_cast<QWidget*>(third));
  m_toRight->setAutoRepeat(true);
  m_toRight->setAutoRepeatInterval(300);
  m_toRight->setMinimumSize(size);
  m_toRight->setText(">");
  connect(m_toRight, SIGNAL(pressed()), this, SLOT(toRightPressed()));
  connect(m_toRight, SIGNAL(released()), this, SLOT(toEndPressed()));

  m_toDown = new QPushButton(dynamic_cast<QWidget*>(third));
  m_toDown->setAutoRepeat(true);
  m_toDown->setAutoRepeatInterval(300);
  m_toDown->setMinimumSize(size);
  m_toDown->setText("Y");
  connect(m_toDown, SIGNAL(pressed()), this, SLOT(toRightPressed()));
  connect(m_toDown, SIGNAL(released()), this, SLOT(toEndPressed()));
  third->addWidget(m_toRight);
  third->addWidget(m_toDown);

  hBox->addLayout(first);
  hBox->addLayout(second);
  hBox->addLayout(third);

  setLayout(hBox);
}
