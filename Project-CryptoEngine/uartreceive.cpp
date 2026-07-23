#include "uartreceive.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

/* Constructor definition */
UartReceive::UartReceive(QObject *parent) : QObject(parent)
{
<<<<<<< HEAD
    connect(&portSerial,
            &QSerialPort::readyRead,
            this,
            &UartReceive::onReadSerialData);
=======
  connect(&portSerial,
          &QSerialPort::readyRead,
          this,
          &UartReceive::onReadSerialData);
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}

bool UartReceive::connectSerialPort(const QString &portname)
{
<<<<<<< HEAD
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
=======
  bool status = false;

  if(portSerial.isOpen())
    {
      conString = "Conncetion is already Opened";
      qDebug()<<"[Debug]: Serial Port already conncted";
      return true;
    }
  portSerial.setPortName(portname);
  portSerial.setBaudRate(QSerialPort::Baud115200);

  status = portSerial.open(QIODevice::ReadWrite);
  if(status == true)
    {
      conString = "Port opened Successfully";
      qDebug()<<"[Debug]: Serial Port open Succeed";
      status = true;
    }
  else
    {
      conString = "Port opened Failed";
      qDebug()<<"[Debug]: Serial Port open failed";
      status = false;
    }
  return status;
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}

void UartReceive::onClickConnectButton()
{
<<<<<<< HEAD
    qDebug()<<"Connecting to serial port ..";
    connectSerialPort("/dev/ttyACM0");
=======
  qDebug()<<"Connecting to serial port ..";
  connectSerialPort("/dev/ttyACM0");
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}

void UartReceive::onClickDisconnectButton()
{
<<<<<<< HEAD
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
    if(portSerial.isOpen())
    {
        portSerial.write("LwIPSYNC");
    }

    ntpCmdStr.clear();

    ntpCmdStr.clear();
    ntpCmdStr.append(static_cast<char>(NTPCMD));
    ntpCmdStr.append(7, char(0x00));

    quint64 byteWritten = portSerial.write(ntpCmdStr);
    portSerial.flush();
    emit logLocalSync("Sending request for inbuilt SNTP Sync ..");
=======
  qDebug()<<"Closing serial port ..";
  if(!portSerial.isOpen())
    {
      qDebug()<<"Port "<<portSerial.portName()<<"is Disconnected";
    }
  portSerial.close();
  //emit logDiconnectMsg("Disconnected: " + portSerial.portName());
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}

/* Descructor */
UartReceive::~UartReceive()
{
<<<<<<< HEAD
    qDebug()<<"Closing serial port ..";
    if(portSerial.isOpen())
    {
        portSerial.close();
    }
}

=======
  qDebug()<<"Closing serial port .."<<__PRETTY_FUNCTION__;
  if(portSerial.isOpen())
    {
      portSerial.close();
    }
}



>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
/* Update this API to send data either to Status
or Data LineEdit */
void UartReceive::onReadSerialData()
{
<<<<<<< HEAD
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
=======
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
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
    }
}

void UartReceive::onWriteUartMsg()
{
<<<<<<< HEAD
    emit logUartMsg(conString +" : " + portSerial.portName());
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
    qDebug()<<"Byte written :"<<byteWritten;
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



=======
  //emit logUartMsg(conString +" : " + portSerial.portName() + " 🔗");
}


void UartReceive::SendEncryptMsg(const QByteArray &bArray)
{
  if (!portSerial.isOpen())
    {
      qWarning() << "Serial port is not open.";
      return;
    }

  quint64 byteWritten = portSerial.write(bArray);

  if(byteWritten == bArray.size())
    {
        qDebug()<<"All Bytes written";
    }
  portSerial.flush();
}


void UartReceive::sendByteArrayCrypto(const QString &Str)
{
  if (Str.isEmpty())
    {
      qWarning() << "Empty String !";
      return;
    }
  qPlainTextArr = QByteArray::fromHex(Str.toUtf8());
  qDebug()<<"Bytes: "<<qPlainTextArr.toHex(' ').toUpper();
  qDebug()<<"Length: "<<qPlainTextArr.length();
}
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
