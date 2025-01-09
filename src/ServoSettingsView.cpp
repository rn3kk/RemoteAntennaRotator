#include "ServoSettingsView.h"
#include <QVBoxLayout>

ServoSettingsView::ServoSettingsView(const int number,
                                     const QString& name,
                                     const QString& azAdr,
                                     const QString& elAdr,
                                     const QString& rotatorAdr,
                                     const double azError,
                                     const double elError)
{
  mId = number;
  setTitle("Antenna rotator module");
  QVBoxLayout *vb = new QVBoxLayout(this);

  mName = new QTextEdit(name, this);
  connect(mName, &QTextEdit::textChanged, this, &ServoSettingsView::nameTextChanged);
  QFontMetrics metrics(mName->font());
  int h = metrics.lineSpacing() * 1.2;
  int w = metrics.maxWidth() * 20;
  mName->setFixedHeight(h);
  mName->setFixedWidth(w);

  QHBoxLayout *hb = new QHBoxLayout(this);
  QLabel *l = new QLabel(tr("Name"));
  l->setParent(this);
  hb->addWidget(l);
  hb->addWidget(mName);
  vb->addLayout(hb);

  mAzAdr = new QTextEdit(azAdr, this);
  connect(mAzAdr, &QTextEdit::textChanged, this, &ServoSettingsView::addressTextChanged);
  QFontMetrics metrics2(mName->font());
  h = metrics2.lineSpacing() * 1.2;
  w = metrics2.maxWidth() * 3;
  mAzAdr->setFixedHeight(h);
  mAzAdr->setFixedWidth(w);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Azimut detector RS-485 address"));
  l->setParent(this);
  hb->addWidget(l);
  hb->addWidget(mAzAdr);
  vb->addLayout(hb);

  mElAdr = new QTextEdit(elAdr, this);
  connect(mElAdr, &QTextEdit::textChanged, this, &ServoSettingsView::addressTextChanged);
  mElAdr->setFixedHeight(h);
  mElAdr->setFixedWidth(w);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Elevation detector RS-485 address"));
  l->setParent(this);
  hb->addWidget(l);
  hb->addWidget(mElAdr);
  vb->addLayout(hb);

  mRotAdr = new QTextEdit(rotatorAdr, this);
  connect(mRotAdr, &QTextEdit::textChanged, this, &ServoSettingsView::addressTextChanged);
  mRotAdr->setFixedHeight(h);
  mRotAdr->setFixedWidth(w);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Rotator RS-485 address"));
  l->setParent(this);
  hb->addWidget(l);
  hb->addWidget(mRotAdr);
  vb->addLayout(hb);

  mAzAngleCorrection = new QDoubleSpinBox(this);
  mAzAngleCorrection->setSingleStep(0.01);
  mAzAngleCorrection->setRange(-360.0, 360.0);
  mAzAngleCorrection->setSuffix("°");
  mAzAngleCorrection->setValue(azError);
  connect(mAzAngleCorrection,
          &QDoubleSpinBox::textChanged,
          this,
          &ServoSettingsView::angleCorrectionTextChanged);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Azimuth correction"));
  l->setParent(this);
  hb->addWidget(l);
  hb->addWidget(mAzAngleCorrection);
  vb->addLayout(hb);

  mElAngleCorrection = new QDoubleSpinBox(this);
  mElAngleCorrection->setSingleStep(0.01);
  mElAngleCorrection->setRange(-360.0, 360.0);
  mElAngleCorrection->setSuffix("°");
  mElAngleCorrection->setValue(elError);
  connect(mElAngleCorrection,
          &QDoubleSpinBox::textChanged,
          this,
          &ServoSettingsView::angleCorrectionTextChanged);

  hb = new QHBoxLayout(this);
  l = new QLabel(tr("Elevation correction"));
  l->setParent(this);
  hb->addWidget(l);
  hb->addWidget(mElAngleCorrection);
  vb->addLayout(hb);

  mDelButton = new QPushButton(tr("Delete"), this);
  vb->addWidget(mDelButton);

  setLayout(vb);
}

ServoSettingsView::ServoSettingsView()
  : ServoSettingsView(1, "2m EME Antenna", "A0", "A1", "A2", 0.0, 0.0)
{}

void ServoSettingsView::addressTextChanged()
{
  QTextEdit *te = reinterpret_cast<QTextEdit *>(sender());
  if (te->toPlainText().length() > 2) {
    te->setText(te->toPlainText().left(2));
    te->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
  if (te == mAzAdr)
    emit azAdrChanged(mId, te->toPlainText());
  else if (te == mElAdr)
    emit elAdrChanged(mId, te->toPlainText());
  else if (te == mRotAdr)
    emit rotatorAdrChanged(mId, te->toPlainText());
}

void ServoSettingsView::angleCorrectionTextChanged()
{
  QDoubleSpinBox *te = reinterpret_cast<QDoubleSpinBox *>(sender());
  if (te == mAzAngleCorrection)
    emit azCorrectionChanged(mId, te->value());
  else if (te == mElAngleCorrection)
    emit elCorrectionChanged(mId, te->value());
}

void ServoSettingsView::nameTextChanged()
{
  QTextEdit *te = reinterpret_cast<QTextEdit *>(sender());
  if (te->toPlainText().length() > 20) {
    te->setText(te->toPlainText().left(20));
    te->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  }
  if (te == mName)
    emit nameChanged(mId, te->toPlainText());
}

void ServoSettingsView::deleteClic()
{
  emit removeServo(mId);
}
