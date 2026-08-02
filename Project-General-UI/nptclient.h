#ifndef NPTCLIENT_H
#define NPTCLIENT_H

#include <QObject>
#include <QDebug>
#include <QUdpSocket>


class NTPClient: public QObject
{
  Q_OBJECT

public:
  explicit NTPClient(QObject *parent = nullptr);

public slots:
  void onReadPendingDatagrams();
  void onSendingQueryNTPServer();

signals:
  void PendingDatagramsReady(const QByteArray &NtpBytes);

private:
  QUdpSocket *m_NtpUdpSocket;
  QByteArray NtpArray;
};

#endif // NPTCLIENT_H
