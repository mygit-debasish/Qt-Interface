#include "uartreceive.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

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

void UartReceive::onClickDisconnectButton()
{
    qDebug()<<"Closing serial port ..";
    if(portSerial.isOpen())
    {
        portSerial.close();
    }
    emit logDiconnectMsg("Disconnected: " + portSerial.portName());
}

void UartReceive::onClickLwipSync()
{
    qDebug()<<"Send command for LwIP Sync ..";
    if(!portSerial.isOpen())
    {
        qDebug()<<"Serial port not opened"<<portSerial.errorString();
        return;
    }

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(NTPCMD));
    ntpCmdStr.append(7, char(0x00));

    qDebug()<<"Remote NTP Sync command :"<<ntpCmdStr.toHex();
    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();

    emit logLocalSync("Sending request for inbuilt SNTP Sync ..");
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

/* Update this API to send data either to Status
or Data LineEdit */
void UartReceive::onReadSerialData()
{
    static QByteArray uartData;
    QByteArray line;
    static const QRegularExpression re("^\\[(\\w+)\\]\\s*:\\s*(.*)$");

    uartData.append(portSerial.readAll());

    while(uartData.contains('\n'))
    {
        int pos = uartData.indexOf('\n');

        /* Reading character up '\n' */
        line = uartData.left(pos);

        /* Clearing Uart Data buffer for next line */
        uartData.remove(0, pos+1);
        line = line.trimmed();

        QRegularExpressionMatch match = re.match(line);
        if(match.hasMatch())
        {
            QString type = match.captured(1);
            QString text = match.captured(2);

            if(type == "STATUS")
            {
                emit logMsgStatusLineEdit(text);   /* Send a line to be appended in status LineEdit */
            }

            else if(type == "TIME")
            {
                emit logMsgDataLcd(text);
            }
            else
            {
                qDebug()<<"Unknow Messsage Type";
            }
        }
        else
        {
            emit logMsgDataLineEdit(QString::fromUtf8(line));
        }
    }
}

void UartReceive::onWriteUartMsg()
{
    emit logUartMsg(conString +" : " + portSerial.portName() + " 🔗");
}

void UartReceive::onSendTimeFrame(const QByteArray &bArray)
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    quint64 byteWritten = portSerial.write(bArray);
    portSerial.flush();
    qDebug()<<"Byte written :"<<byteWritten;
}

void UartReceive::onSendNtpSyncCmd()
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }
    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(NTPCMD));
    ntpCmdStr.append(7, char(0x00));

    qDebug()<<"Sending Byte array : "<<ntpCmdStr.toHex(' ').toUpper();
    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();
    qDebug()<<"Remote SNTP Sync :"<<ntpCmdStr;
}

void UartReceive::onMountSDCard()
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(MOUNT));
    ntpCmdStr.append(7, char(0x00));
    portSerial.flush();
    quint64 byteWritten = portSerial.write(ntpCmdStr);

    qDebug()<<"Sending Byte array : "<<ntpCmdStr.toHex(' ').toUpper();
}

void UartReceive::onUnMountSDCard()
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(UMOUNT));
    ntpCmdStr.append(7, char(0x00));
    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();

    qDebug()<<"Sending Byte array : "<<ntpCmdStr.toHex(' ').toUpper();
}

void UartReceive::onWriteSDCard()
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(WRITE));
    ntpCmdStr.append(7, char(0x00));
    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();

    qDebug()<<"Sending Byte array : "<<ntpCmdStr.toHex(' ').toUpper();
}

void UartReceive::onReadSDCard()
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(READ));
    ntpCmdStr.append(7, char(0x00));
    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();

    qDebug()<<"Sending Byte array : "<<ntpCmdStr.toHex(' ').toUpper();
}

void UartReceive::onDelSDCard()
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(DEL));
    ntpCmdStr.append(7, char(0x00));
    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();

    qDebug()<<"Sending Byte array : "<<ntpCmdStr.toHex(' ').toUpper();
}

void UartReceive::SendEncryptMsg(const QByteArray &bArray)
{
    if (!portSerial.isOpen())
    {
        qWarning() << "Serial port is not open.";
        return;
    }

    quint64 byteWritten = portSerial.write(bArray);
    portSerial.flush();

    qDebug()<<"Byte written :"<<byteWritten;
}

