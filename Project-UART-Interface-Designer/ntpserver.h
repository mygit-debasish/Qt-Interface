#ifndef NTPSERVER_H
#define NTPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>

class NTPClient : public QObject
{
    Q_OBJECT

public:
    explicit NTPClient(QObject *parent = nullptr);

    /* Destructor*/
    ~NTPClient();

private:
    QUdpSocket *m_udpSocket;
    QByteArray txFrame;
    QByteArray packet;

signals:
    void onReadPendingDatagrams(const QByteArray txFrame);
    void onSendingRemoteSyncLog(const QString &msg );

public slots:
    void ReadPendingDatagrams();
    void onConnectNTPServer();

};

#endif // NTPSERVER_H
