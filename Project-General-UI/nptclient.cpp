#include "nptclient.h"
#include <QDateTime>

NTPClient::NTPClient(QObject *parent):QObject(parent)
{
  qDebug()<<"[DEBUG] "<<__PRETTY_FUNCTION__;
  NtpArray.resize(48);
  NtpArray.fill(0x00);

  m_NtpUdpSocket = new QUdpSocket(this);
  NtpArray[0] = 0x1B;

  connect(m_NtpUdpSocket,
          &QUdpSocket::readyRead,
          this,
          &NTPClient::onReadPendingDatagrams);
}

void NTPClient::onSendingQueryNTPServer()
{
    /* Send Query Bytes and Wait for the response in connect below */
  qDebug()<<"[DEBUG] Connecting to NTP server";

  NtpArray.resize(48);
  NtpArray.fill(0x00);
  NtpArray[0] = 0x1B;

  m_NtpUdpSocket->writeDatagram(
                NtpArray,
                QHostAddress("129.6.15.28"),
                123);
}

void NTPClient::onReadPendingDatagrams()
{
    qDebug()<<"[DEBUG] Reading UDP datagram ..";
    while(m_NtpUdpSocket->hasPendingDatagrams())
    {
        QByteArray udpData = QByteArray(8, 0x00); /* Here Size is arbitrary */
        udpData.resize(m_NtpUdpSocket->pendingDatagramSize());

        /* Reading NTP time epoch */
        m_NtpUdpSocket->readDatagram(udpData.data(),
                                  udpData.size());
        //qDebug()<<"[DEBUG] UDP Bytes: "<<udpData.toHex(' ').toUpper();
        //qDebug()<<"[DEBUG] UDP Size : "<<udpData.size();

#if 0
        quint32 ntpEpoch =
                ((quint8)udpData[40] << 24) |
                ((quint8)udpData[41] << 16) |
                ((quint8)udpData[42] << 8)  |
                (quint8)udpData[43];
        quint32 unixEpoch = ntpEpoch - 2208988800;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unixEpoch);

        qDebug() <<"Current Time:"<<dt.toString("dd-MM-yyyy hh:mm:ss");
#endif

        /* Create transmit frame */
        NtpArray.clear();

        NtpArray.append(udpData[0]);    /*MODE_VER_LI*/
        NtpArray.append(udpData[1]);    /*Stratum*/
        NtpArray.append(udpData[2]);    /*Poll*/
        NtpArray.append(udpData[3]);    /* Precision or custom control byte*/
        NtpArray.append(udpData[40]);   /* Seconds */
        NtpArray.append(udpData[41]);   /* Seconds */
        NtpArray.append(udpData[42]);   /* Seconds */
        NtpArray.append(udpData[43]);   /* Seconds */
        NtpArray.append(static_cast<char>(0xFF));   /* End Bit */

        qDebug()<<"[DEBUG] Transmit Bytes: "<<NtpArray.toHex(' ').toUpper();
        /* Signal */
        emit PendingDatagramsReady(NtpArray);
    }
}
