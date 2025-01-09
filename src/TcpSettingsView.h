#ifndef TCPSETTINGSVIEW_H
#define TCPSETTINGSVIEW_H

#include <QGroupBox>

class TcpSettingsView : public QGroupBox
{
  Q_OBJECT
public:
  TcpSettingsView();
  TcpSettingsView(const QString &ip, int port, int timeoute);

  QString ip() const;
  int port() const;
  int timeoute() const;

signals:
  void ipChanged();
  void portChanged();
  void timeoutChanged();

};

#endif // TCPSETTINGSVIEW_H
