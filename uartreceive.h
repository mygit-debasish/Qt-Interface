#ifndef UARTRECEIVE_H
#define UARTRECEIVE_H

#include <QMainWindow>
#include <QSerialPort>


class UartReceive : public QObject
{
    Q_OBJECT
public:
    explicit UartReceive(QObject *parent = nullptr);    /* Constructor */
    bool connectSerialPort(const QString &portname);
    QString timeStr;

    ~UartReceive();

public slots:
    void onClickConnectButton();
    void onReadSerialData();
    void onWriteUartMsg();
    void onSendTimeFrame(const QByteArray &bArr);


signals:
    void logMessage(const QString &str);
    void logUartMsg(const QString &str);
    void sendTimeFrametoSTM(const QString &str);

private:
    QSerialPort portSerial;
    QString  conString;
};

#endif // UARTRECEIVE_H
