#include "parentwindow.h"
#include "ui_parentwindow.h"
<<<<<<< HEAD
#include "uartreceive.h"
#include "ntpserver.h"
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

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
            &UartReceive::logMsgDataLineEdit,
            ui->pLogWindow,
            &QTextEdit::append);

    /* LCD Display */
    connect(uart,
            &UartReceive::logMsgDataLcd,
            this,
            &ParentWindow::onDisplayLCD);

    /* To staus Window */
    connect(uart,
            &UartReceive::logMsgStatusLineEdit,
            ui->pConStatus,
            &QTextEdit::append);


    /* Sending NTP time-frame to STM32 */
    connect(m_ntpClient,
            &NTPClient::onReadPendingDatagrams,
            uart,
            &UartReceive::onSendTimeFrame);

    /* NTP Client connect and display TxTimeFrame */
    connect(ui->pRemoteSync,
            &QPushButton::clicked,
            m_ntpClient,
            &NTPClient::onConnectNTPServer);

    connect(m_ntpClient,
            &NTPClient::onSendingRemoteSyncLog,
            ui->pConStatus,
            &QTextEdit::append);

    /* Disconnect UART */
    connect(ui->pDisconnectUART,
            &QPushButton::clicked,
            uart,
            &UartReceive::onClickDisconnectButton);
    connect(uart,
            &UartReceive::logDiconnectMsg,
            ui->pConStatus,
            &QTextEdit::append);

    /* Command for Local Sync */
    connect(ui->pLocalSync,
            &QPushButton::clicked,
            uart,
            &UartReceive::onClickLwipSync);

    connect(uart,
            &UartReceive::logLocalSync,
            ui->pConStatus,
            &QTextEdit::append);

    /* Data logging into MicroSD card */

    /* Mounting */
    connect(ui->pMtSDcard,
            &QPushButton::clicked,
            uart,
            &UartReceive::onMountSDCard);

    /* Un-mounting */
    connect(ui->pUmtSDcard,
            &QPushButton::clicked,
            uart,
            &UartReceive::onUnMountSDCard);

    /* Writing */
    connect(ui->pWriteSDcard,
            &QPushButton::clicked,
            uart,
            &UartReceive::onWriteSDCard);

    /* Reading */
    connect(ui->pReadSDcard,
            &QPushButton::clicked,
            uart,
            &UartReceive::onReadSDCard);

    /* Deleting Log file */
    connect(ui->pDelFile,
            &QPushButton::clicked,
            uart,
            &UartReceive::onDelSDCard);
=======
#include <QDebug>
#include <QRegularExpression>
#include <mbedtls/aes.h>
#include <mbedtls/cipher.h>
#include "customwidgets.h"
#include "customplaintextedit.h"
#include "uartreceive.h"
#include "cryptoengine.h"


ParentWindow::ParentWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::ParentWindow)
{
  ui->setupUi(this);
  uart = new UartReceive(this);
  m_crypto = new CryptoEngine(this, uart);

  connect(ui->pExit,
          &QPushButton::clicked,
          this,
          &QMainWindow::close);

  connect(ui->pConnectUART,
          &QPushButton::clicked,
          uart,
          &UartReceive::onClickConnectButton);

  connect(ui->pDisconnectUART,
          &QPushButton::clicked,
          uart,
          &UartReceive::onClickDisconnectButton);

  /* Sending Data to UART using ENTER in PlainTextEdit */
  connect(ui->pPlainText,
          &CustomPlainTextEdit::EnterPresssedTextEdit,  /* Signal from Key_ENTER event */
          this,
          &ParentWindow::onEnterPresssedTextEdit);

  /* Sending Data to UART using SEND button */
  connect(ui->pSendData,
          &QPushButton::clicked,
          this,
          &ParentWindow::onPressingSendButton);

  connect(this,
          &ParentWindow::PlainTextFromWidgetReady,  /* Signal*/
          uart,
          &UartReceive::sendByteArrayCrypto);

  /* Invoking Crypto engine and send to STM32 through UART : onEncryptMsgCBCtoSTM */
  connect(ui->pPlainText,
          &CustomPlainTextEdit::EnterPresssedTextEdit,  /* Generated from Key_ENTER event */
          m_crypto,
          &CryptoEngine::onEncryptMsgCBCtoSTM );

  /* Update PlainLineEdit IV, KEY & Hash */
  connect(m_crypto,
          &CryptoEngine::updateLineEditWidget,  /* Signal*/
          this,
          &ParentWindow::updateLineEditWidget);
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}

ParentWindow::~ParentWindow()
{
<<<<<<< HEAD
    delete ui;
}

void ParentWindow::onClickClearButton()
{
    qDebug()<<"Clearing Log Window ...";
    ui->pLogWindow->clear();
}

void ParentWindow::onDisplayLCD(const QString &str)
{
    QRegularExpression re("\\[(\\d{2}:\\d{2}:\\d{2})\\]\\s*(.*)$");
    QRegularExpressionMatch match = re.match(str);

    if(match.hasMatch())
    {
        QString time =  match.captured(1);
        ui->pLcdTime->display(time);
    }
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
void ParentWindow::onSendingRemoteSync()
{
    const QString msg = "Remote Sync command sent ..";
    ui->pConStatus->append("Hellow world");
    qDebug()<<"Sent status: "<<msg;
=======
  delete ui;
}

void ParentWindow::onEnterPresssedTextEdit()
{
  /* Include Cryptographic functionalities here  */
  /* TODO */

  QString str = ui->pPlainText->toPlainText();

  str.remove(QRegularExpression("\\s|,|0x"));     /* ORing remove characters in the string. No space between Regex */
  emit PlainTextFromWidgetReady(str);

  /* Clear the Text Window */
  ui->pPlainText->clear();
  ui->pPlainText->moveCursor(QTextCursor::Start);
  ui->pPlainText->ensureCursorVisible();
}

void ParentWindow::onPressingSendButton()
{
  QString str = ui->pPlainText->toPlainText();
  str.remove(QRegularExpression("\\s|,|0x"));

  if(str.isEmpty())
    {
      return;
    }

  emit PlainTextFromWidgetReady(str);

  /* Clear the text Window */
  ui->pPlainText->clear();
  ui->pPlainText->moveCursor(QTextCursor::Start);
  ui->pPlainText->ensureCursorVisible();
}

void ParentWindow::getPlainTextArr()
{
  qDebug()<<"Plain Text for Encryption: "<<uart->qPlainTextArr.toHex(' ').toUpper();
}

/* Update LineEdit widgets */
void ParentWindow::updateLineEditWidget(const QString &str)
{
  QByteArray iv_arr(reinterpret_cast<const char *>(m_crypto->m_iv), IV_LEN);
  QByteArray key_arr(reinterpret_cast<const char *>(m_crypto->m_key), KEY_LEN);
  QByteArray hash_arr(reinterpret_cast<const char *>(m_crypto->m_hash), HASH_LEN_256);

  ui->pInitVect->setPlaceholderText("Initialization Vector (IV)");
  ui->pInitVect->setPlainText(iv_arr.toHex(' ').toUpper());
  ui->pKey->setPlainText(key_arr.toHex(' ').toUpper());
  ui->pHash->setPlainText(hash_arr.toHex(' ').toUpper());
  ui->pCipherText->setPlainText(m_crypto->m_EncMsgTransmit.toHex(' ').toUpper());
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
}
