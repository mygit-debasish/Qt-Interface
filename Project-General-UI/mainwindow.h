#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serialport.h>
#include <nptclient.h>


class SerialPort;
class NPTClient;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  SerialPort *portSerial;
  NTPClient *m_NtpClient;

private slots:
  void onSendStatusDebugWidget(const QString &Msg);



};
#endif // MAINWINDOW_H
