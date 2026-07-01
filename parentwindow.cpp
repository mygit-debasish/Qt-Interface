#include "parentwindow.h"
#include "ui_parentwindow.h"
#include "uartreceive.h"
#include "ntpserver.h"
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>

ParentWindow::ParentWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParentWindow)
{
    ui->setupUi(this);

    m_ntpClient = new NTPClient(this);
    uart = new UartReceive(this);
    QString name = "नमस्कार, आपका हार्दिक स्वागत है।";
    qDebug()<<name;

    /* Connect signals and slots */
    connect(ui->pExit,
            &QPushButton::clicked,
            this,
            &ParentWindow::close);

    connect(ui->pClearLog,
            &QPushButton::clicked,
            this,
            &ParentWindow::onClickClearButton);

    connect(ui->pConnectUART,
            &QPushButton::clicked,
            uart,
            &UartReceive::onClickConnectButton);

    /* ✍ Connect Message from Push button to Slot->Signal*/
    connect(ui->pConnectUART,
            &QPushButton::clicked,
            uart,
            &UartReceive::onWriteUartMsg);

    connect(uart,
            &UartReceive::logUartMsg,
            ui->pConStatus,
            &QTextEdit::append);

    /* Logging current time in Main Log Window */
    connect(uart,
            &UartReceive::logMessage,
            ui->pLogWindow,
            &QTextEdit::append);

    /* LCD Display */
    connect(uart,
            &UartReceive::logMessage,
            this,
            &ParentWindow::onDisplayLCD);

    /* Sending NTP time-frame to STM32 */
    connect(m_ntpClient,
            &NTPClient::onReadPendingDatagrams,
            uart,
            &UartReceive::onSendTimeFrame);

    /* NTP Client connect and display TxTimeFrame */
    connect(ui->pSendNtpEpoch,
            &QPushButton::clicked,
            m_ntpClient,
            &NTPClient::onConnectNTPServer);
}

ParentWindow::~ParentWindow()
{
    delete ui;
}

void ParentWindow::onClickClearButton()
{
    qDebug()<<"Clearing Log Window ...";
    ui->pLogWindow->clear();
}

void ParentWindow::onDisplayLCD()
{
    QString time =  uart->timeStr.section('[', 1).section(']', 0, 0);
    ui->pLcdTime->display(time);
}

void ParentWindow::showPendingTimeEpoch(const quint32 ntpEpoch)
{
    qDebug()<<"NTP time epoch :"<<ntpEpoch;;
    const quint32 NTP_TO_UNIX = 2208988800UL;

    quint32 unixEpoch = ntpEpoch - NTP_TO_UNIX;
    QDateTime dt =
            QDateTime::fromSecsSinceEpoch(unixEpoch,
                                          Qt::UTC);
    QTimeZone indiaTimeZone("Asia/Kolkata");
    QDateTime istTime = dt.toTimeZone(indiaTimeZone);

    qDebug() << istTime.toString();
}

uint32_t ParentWindow::countNtpUnixSeconds()
{
    long seconds;
    int count = 0;
    int year = 1900;
    for(year = 1900; year < 1970; year++)
    {
        if(year % 4 == 0)   /* 1900 is not Leap year */
        {
            count++;
        }
    }
    seconds = (70*365 + count -1 ) * 24 * 60 * 60;

    return seconds;
}
