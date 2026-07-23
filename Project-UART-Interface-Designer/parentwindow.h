#ifndef PARENTWINDOW_H
#define PARENTWINDOW_H

#include <QMainWindow>

class UartReceive;  /* Forward Declaration */
class NTPClient;

QT_BEGIN_NAMESPACE
namespace Ui { class ParentWindow; }
QT_END_NAMESPACE

class ParentWindow : public QMainWindow
{
    Q_OBJECT

public:
    ParentWindow(QWidget *parent = nullptr);
    uint32_t countNtpUnixSeconds();
    UartReceive *uart;

    ~ParentWindow();

private:
    Ui::ParentWindow *ui;

    NTPClient *m_ntpClient;
    QString timeString;

public slots:
        void onClickClearButton();
        void onDisplayLCD(const QString &str);
        void showPendingTimeEpoch(const quint32 ntpEpoch);
        void onSendingRemoteSync();

signals:

};
#endif // PARENTWINDOW_H
