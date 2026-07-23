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

    enum SD_Operation
    {
        MOUNT =  0x01,
        UMOUNT = 0x02,
        READ =   0x03,
        WRITE =  0x04,
        DEL=    0x05,
        NTPCMD = 0x0C
    };

public slots:
    void onClickConnectButton();
    void onClickDisconnectButton();
    void onClickLwipSync();
    void onReadSerialData();
    void onWriteUartMsg();
    void onSendTimeFrame(const QByteArray &bArr);
    void onSendNtpSyncCmd();

    /* SD card Slots */
    void onMountSDCard();
    void onUnMountSDCard();
    void onWriteSDCard();
    void onReadSDCard();
    void onDelSDCard();


signals:
    void logMsgDataLineEdit(const QString &str);
    void logMsgStatusLineEdit(const QString &str);
    void logMsgDataLcd(const QString &str);
    void logUartMsg(const QString &str);
    void logDiconnectMsg(const QString &msg);
    void logLocalSync(const QString &msg);
    void sendTimeFrametoSTM(const QString &str);

private:
    QSerialPort portSerial;
    QString  conString;
    QByteArray ntpCmdStr;

public:
    void SendEncryptMsg(const QByteArray &bArray);
};

#endif // UARTRECEIVE_H
