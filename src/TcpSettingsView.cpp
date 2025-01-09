#include "TcpSettingsView.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

TcpSettingsView::TcpSettingsView():
  TcpSettingsView("192.168.0.123", 3000, 200)
{}

TcpSettingsView::TcpSettingsView(const QString &ip, int port, int timeoute)
{
  setTitle("LAN settings");
  QVBoxLayout* vb = new QVBoxLayout(this);

  QHBoxLayout *hb = new QHBoxLayout(this);
  QLabel *l = new QLabel(tr("IP"));
  l->setParent(this);
  QTextEdit * t = new QTextEdit(ip, this);
  connect(t, &QTextEdit::textChanged, this, &TcpSettingsView::ipChanged);
  QFontMetrics metrics2(t->font());
  int h = metrics2.lineSpacing() * 1.2;
  t->setFixedHeight(h);
  hb->addWidget(l);
  hb->addWidget(t);
  vb->addLayout(hb);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Port"));
  l->setParent(this);
  t = new QTextEdit(QString::number(port), this);
  t->setFixedHeight(h);
  connect(t, &QTextEdit::textChanged, this, &TcpSettingsView::portChanged);
  hb->addWidget(l);
  hb->addWidget(t);
  vb->addLayout(hb);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Timeoute"));
  l->setParent(this);
  t = new QTextEdit(QString::number(timeoute), this);
  t->setFixedHeight(h);
  connect(t, &QTextEdit::textChanged, this, &TcpSettingsView::timeoutChanged);
  hb->addWidget(l);
  hb->addWidget(t);
  vb->addLayout(hb);

  setLayout(vb);
}
