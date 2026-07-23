#include "cryptomanager.h"
#include <QDebug>

CryptoManager::CryptoManager(UartReceive *uart, QObject *parent) : QObject(parent)
{

  qDebug()<<"Function: "<<__FUNCTION__;
}
