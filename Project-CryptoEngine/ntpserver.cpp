#include "ntpserver.h"
#include <QDebug>
#include <QDateTime>


NTPClient::NTPClient(QObject *parent) : QObject(parent)
{
    packet.resize(48);
    packet.fill(0x00);

    m_udpSocket = new QUdpSocket(this);
    packet[0] = 0x1B;

    connect(m_udpSocket,
            &QUdpSocket::readyRead,
            this,
            &NTPClient::ReadPendingDatagrams);
}

NTPClient::~NTPClient()
{
}

void NTPClient::onConnectNTPServer()
{
    /* Send Query Bytes and Wait for the response in connect below */
    m_udpSocket->writeDatagram(
                packet,
                QHostAddress("129.6.15.28"),
                123);
}

void NTPClient::ReadPendingDatagrams()
{
    qDebug()<<"Reading UDP datagram ..";
    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray udpData = QByteArray(8, 0x00);
        udpData.resize(m_udpSocket->pendingDatagramSize());

        /* Reading NTP time epoch */
        m_udpSocket->readDatagram(udpData.data(),
                                  udpData.size());

        quint32 ntpEpoch =
                ((quint8)udpData[40] << 24) |
                ((quint8)udpData[41] << 16) |
                ((quint8)udpData[42] << 8)  |
                (quint8)udpData[43];
        quint32 unixEpoch = ntpEpoch - 2208988800;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unixEpoch);

        qDebug() <<"Current Time:"<<dt.toString("dd-MM-yyyy hh:mm:ss");

        /* Create transmit frame */
        txFrame.clear();

        txFrame.append(0xAA);
        txFrame.append(0x01);
        txFrame.append(0x04);
        txFrame.append(udpData[40]);
        txFrame.append(udpData[41]);
        txFrame.append(udpData[42]);
        txFrame.append(udpData[43]);

        /* Calculating checksum */
        uint8_t checkSum = 0;
        for (char Bytes: txFrame)
        {
            checkSum ^= static_cast <uint8_t>(Bytes);
        }

        txFrame.append(checkSum);
        /* Signal */
        emit onReadPendingDatagrams(txFrame);
        emit onSendingRemoteSyncLog("Sending request for remote UART Sync ..");
    }
}
