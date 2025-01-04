#ifndef SERVOSETTINGSVIEW_H
#define SERVOSETTINGSVIEW_H

#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

class ServoSettingsView : public QGroupBox
{
  Q_OBJECT

public:
  ServoSettingsView();

private slots:
  void addressTextChanged();
  void angleCorrectionTextChanged();
  void nameTextChanged();

private:
  int mId = 0;
  QTextEdit* mName;
  QTextEdit* mAzAdr;
  QTextEdit* mElAdr;
  QTextEdit* mRotAdr;
  QDoubleSpinBox* mAzAngleCorrection;
  QDoubleSpinBox* mElAngleCorrection;
  QPushButton* mDelButton;
};

#endif // SERVOSETTINGSVIEW_H
