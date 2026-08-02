#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  portSerial = new SerialPort();
  m_NtpClient = new NTPClient();

  /**************** Signals-Slots connection *******************/
  connect(ui->pQuit,
          &QPushButton::clicked,
          this,
          &QWidget::close);

  /* Serial port connect */
  connect(ui->pConnect,
          &QPushButton::clicked,
          portSerial,
          &SerialPort::onConnectButtClicked);

  /* Serial port discconnect */
  connect(ui->pDisconnect,
          &QPushButton::clicked,
          portSerial,
          &SerialPort::onDisConnectButtClicked);

  /* Soft Reset STM32 */
  connect(ui->pResetSTM32,
          &QPushButton::clicked,
          portSerial,
          &SerialPort::onSendRstButtClicked);

  /* Verifying Firmware Signature */
  connect(ui->pSignature,
          &QPushButton::clicked,
          portSerial,
          &SerialPort::onSignVerifyButtClicked);


  /* Connecting to NTP Server, send Query and recive time stamped bytes */
  connect(ui->pConNTP,
          &QPushButton::clicked,
          m_NtpClient,
          &NTPClient::onSendingQueryNTPServer);

  /* Send recived NTP Bytes to UART (STM32F769I-DISCO) */
  connect(m_NtpClient,
          &NTPClient::PendingDatagramsReady,
          portSerial,
          &SerialPort::onSendNTPByteSTM32);

  /* Sending Debug message to Log window */
  connect(portSerial,
          &SerialPort::sendStatusDebugWidget,
          this,
          &MainWindow::onSendStatusDebugWidget);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::onSendStatusDebugWidget(const QString &Msg)
{
  ui->pDebugText->appendPlainText(Msg);
}

