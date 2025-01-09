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
  ServoSettingsView(const int number, const QString& name, const QString& azAdr, const QString& elAdr, const QString& rotatorAdr,
const double azError, const double elError);
  ServoSettingsView();

signals:
  void nameChanged(int id, const QString& name);
  void azAdrChanged(int id, const QString& azAdr);
  void elAdrChanged(int id, const QString& elAdr);
  void rotatorAdrChanged(int id, const QString& rotatorAdr);
  void azCorrectionChanged(int id, double azCorr);
  void elCorrectionChanged(int id, double elCorr);
  void removeServo(int id);

private slots:
  void addressTextChanged();
  void angleCorrectionTextChanged();
  void nameTextChanged();
  void deleteClic();

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
