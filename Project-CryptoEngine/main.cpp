#include "parentwindow.h"
#include <QApplication>
<<<<<<< HEAD
#include <ntpserver.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParentWindow w;
    QFont font("Ubuntu Mono", 12);
    w.setFont(font);
    w.show();

    return a.exec();
=======
#include "customwidgets.h"
#include "custom-crypto.h"
#include "uartreceive.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ParentWindow win;
  win.show();
  return a.exec();
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}
