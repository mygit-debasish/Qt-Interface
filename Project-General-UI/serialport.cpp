#include "serialport.h"

SerialPort::SerialPort(QObject *parent):QObject(parent)
{
  qDebug()<<"[DEBUG] Serial port constructor";

  /**************** Signals-Slots connection *******************/

  connect(this,
          &SerialPort::connectButtClicked,
          this,
          &SerialPort::connectSerialPort);
}

/* Slots definition */
void SerialPort::onConnectButtClicked()
{
  qDebug()<<"[DEBUG] Connect Button clicked. Emit connectButtClicked()";
  emit connectButtClicked();
}

void SerialPort::onDisConnectButtClicked()
{
  qDebug()<<"[DEBUG] Dicconnect Button clicked";

  if(m_serialPort.isOpen())
    {

      m_serialPort.close();
      if(!m_serialPort.isOpen())
        {
          qDebug()<<"[DEBUG] Serial port "<<m_serialPort.portName()<<"Closed successfully";
          emit sendStatusDebugWidget("[DEBUG] UART port closed");
        }
      else
        {
          qDebug()<<"[DEBUG] Serial port error: "<<m_serialPort.errorString();
          emit sendStatusDebugWidget("[DEBUG] UART port Error");
        }
    }
  else
    {
      qDebug()<<"[DEBUG] Serial port already closed";
      emit sendStatusDebugWidget("[DEBUG] UART port already closed");
    }
}

/* Soft Reset STM32 */
void SerialPort::onSendRstButtClicked()
{
  qDebug()<<"[DEBUG] STM32 RST Button clicked";
  GenCtrlBuff.clear();
  GenCtrlBuff.append(static_cast<char>(0x1C));
  GenCtrlBuff.append(static_cast<char>(0x01));
  GenCtrlBuff.append(static_cast<char>(0x0D));
  GenCtrlBuff.append(static_cast<char>(0xEE));
  GenCtrlBuff.append(static_cast<char>(0xFF));

  if(m_serialPort.isOpen())
    {
      size_t dataWritten = m_serialPort.write(GenCtrlBuff);
      qDebug()<<"[DEBUG] Byte written: "<<dataWritten;
       qDebug()<<"[DEBUG] Bytes: "<<GenCtrlBuff.toHex(' ').toUpper();
    }
  else
    {
      qDebug()<<"[DEBUG] Serial not open";
    }
  emit sendStatusDebugWidget("[DEBUG] Soft Reset STM32");
}

/* Verifying Signature */
void SerialPort::onSignVerifyButtClicked()
{
  qDebug()<<"[DEBUG] STM32 RST Button clicked";
  GenCtrlBuff.clear();
  GenCtrlBuff.append(static_cast<char>(0x1C));
  GenCtrlBuff.append(static_cast<char>(0x01));
  GenCtrlBuff.append(static_cast<char>(0x0D));
  GenCtrlBuff.append(static_cast<char>(0xDE));
  GenCtrlBuff.append(static_cast<char>(0xFF));

  if(m_serialPort.isOpen())
    {
      size_t dataWritten = m_serialPort.write(GenCtrlBuff);
      qDebug()<<"[DEBUG] Byte written: "<<dataWritten;
       qDebug()<<"[DEBUG] Bytes: "<<GenCtrlBuff.toHex(' ').toUpper();
    }
  else
    {
      qDebug()<<"[DEBUG] Serial not open";
    }
  emit sendStatusDebugWidget("[DEBUG] Verifying Firmware Signature");
}



void SerialPort::connectSerialPort()
{
  qDebug()<<"[DEBUG] Connecting Serial port ..";
  if(openSerialPort("/dev/ttyACM0", QSerialPort::Baud115200))
    {
      qDebug()<<"[DEBUG] Serial port opened successfully "<<m_serialPort.portName();
    }
  else
    {
      qDebug()<<"[DEBUG] Serial port Error "<<m_serialPort.errorString();
    }
}

/* Public/private function */
bool SerialPort::openSerialPort(const QString &portName, QSerialPort::BaudRate baudRate)
{
  bool status;
  m_serialPort.setPortName(portName);
  m_serialPort.setBaudRate(baudRate);
  m_serialPort.setDataBits(QSerialPort::Data8);
  m_serialPort.setParity(QSerialPort::NoParity);
  m_serialPort.setStopBits(QSerialPort::OneStop);
  m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

  if(m_serialPort.isOpen())
    {
      qDebug()<<"[DEBUG] Serial port is already opened";
      status = true;
      emit sendStatusDebugWidget("[DEBUG] UART port already opened");
    }
  else
    {
      if(m_serialPort.open(QIODevice::ReadWrite))
        {
          status = true;
          emit sendStatusDebugWidget("[DEBUG] UART port opened");
        }
      else
        status = false;
        emit sendStatusDebugWidget("[DEBUG] UART could not be  opened");
    }
  emit connectionStatusChanged(status);
  return status;
}

void SerialPort::onSendNTPByteSTM32(const QByteArray &NtpArr)
{
  if(!m_serialPort.isOpen())
    {
    qDebug()<<"[DEBUG] Serial port is not opened";
    return;
    }

  uint32_t nBytes = m_serialPort.write(NtpArr);
  emit sendStatusDebugWidget("[DEBUG] NTP Bytes Sent to UART");

  qDebug()<<"[DEBUG] Number Bytes "<<nBytes;
  qDebug()<<"[DEBUG] NTP Bytes "<<NtpArr.toHex(' ').toUpper();
}
