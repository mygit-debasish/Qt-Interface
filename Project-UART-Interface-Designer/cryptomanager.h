#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QObject>

/* Forward declaration */
class UartReceive;

class CryptoManager : public QObject
{
  Q_OBJECT
public:
  explicit CryptoManager(UartReceive *uart, QObject *parent = nullptr);

signals:

private:
  UartReceive *m_uart;

};

#endif // CRYPTOMANAGER_H
