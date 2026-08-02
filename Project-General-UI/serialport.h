#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QSerialPort>
#include <QObject>
#include <QDebug>


class SerialPort : public QObject
{
  Q_OBJECT


public:
  explicit SerialPort(QObject *parent = nullptr);
  bool openSerialPort(const QString &portName,
                      QSerialPort::BaudRate baudRate);

signals:
  void connectButtClicked();
  void connectionStatusChanged(bool &status);
  void sendStatusDebugWidget(const QString &Msg);

public slots:
  void onConnectButtClicked();
  void connectSerialPort();
  void onDisConnectButtClicked();
  void onSendRstButtClicked();
  void onSignVerifyButtClicked();
  void onSendNTPByteSTM32(const QByteArray &NtpArr);

private:
  QSerialPort m_serialPort;
  QByteArray NtpBuff;
  QByteArray GenCtrlBuff;

};

#endif // SERIALPORT_H
