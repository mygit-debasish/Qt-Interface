#include "parentwindow.h"
#include <QApplication>
#include <ntpserver.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParentWindow w;
    QFont font("Ubuntu Mono", 12);
    w.setFont(font);
    w.show();

    return a.exec();
}
