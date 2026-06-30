#include "uartreceive.h"
#include <QDebug>

/* Constructor definition */
UartReceive::UartReceive(QObject *parent) : QObject(parent)
{
    connect(&portSerial,
            &QSerialPort::readyRead,
            this,
            &UartReceive::onReadSerialData);
}

bool UartReceive::connectSerialPort(const QString &portname)
{
    bool status = false;

    if(portSerial.isOpen())
    {
        conString = "Conncetion is already Opened";
        return true;
    }
    portSerial.setPortName(portname);
    portSerial.setBaudRate(QSerialPort::Baud115200);

    status = portSerial.open(QIODevice::ReadWrite);
    if(status == true)
    {
        conString = "Port opened Successfully";
        status = true;
    }
    else
    {
        conString = "Port opened Failed";
        status = false;
    }
    return status;
}

void UartReceive::onClickConnectButton()
{
    qDebug()<<"Connecting to serial port ..";
    connectSerialPort("/dev/ttyACM0");
}

/* Descructor */
UartReceive::~UartReceive()
{
    qDebug()<<"Closing serial port ..";
    if(portSerial.isOpen())
    {
        portSerial.close();
    }
}

void UartReceive::onReadSerialData()
{
    static QByteArray uartData;
    QByteArray line;
    uartData.append(portSerial.readAll());

    while(uartData.contains('\n'))
    {
        int pos = uartData.indexOf('\n');

        /* Reading character up '\n' */
        line = uartData.left(pos);

        /* Clearing Uart Data buffer for next line */
        uartData.remove(0, pos+1);
        line = line.trimmed();

        qDebug()<<"Read Line :"<<line;

        /* Saving Time string for sending to LCD */
        timeStr = line;
        emit logMessage(QString::fromUtf8(line));   /* Send a line to be appended in LineEdit */
    }
}

void UartReceive::onWriteUartMsg()
{
    emit logUartMsg(conString +" : " + portSerial.portName());
}

void UartReceive::onSendTimeFrame(const QByteArray &bArray)
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    qDebug()<<"Sending Byte array : "<<bArray.toHex(' ').toUpper();

    qDebug() << "Port:" << portSerial.portName();
    qDebug() << "Open:" << portSerial.isOpen();
    qDebug() << "Writable:" << portSerial.isWritable();

    quint64 byteWritten = portSerial.write(bArray);
    portSerial.flush();

    qDebug()<<"Byte written :"<<byteWritten;
}





