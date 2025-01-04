#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
  Q_OBJECT
  public:
  explicit MainWindow(QWidget *parent = nullptr);

  signals:

  private slots:
      void settings();

  private:
      void createMenus();
      void createActions();
};

#endif // MAINWINDOW_H
