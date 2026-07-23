#ifndef PARENTWINDOW_H
#define PARENTWINDOW_H

#include <QMainWindow>

<<<<<<< HEAD
class UartReceive;  /* Forward Declaration */
class NTPClient;

=======
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)
QT_BEGIN_NAMESPACE
namespace Ui { class ParentWindow; }
QT_END_NAMESPACE

<<<<<<< HEAD
class ParentWindow : public QMainWindow
{
    Q_OBJECT

public:
    ParentWindow(QWidget *parent = nullptr);
    uint32_t countNtpUnixSeconds();

    ~ParentWindow();

private:
    Ui::ParentWindow *ui;
    UartReceive *uart;
    NTPClient *m_ntpClient;
    QString timeString;

public slots:
        void onClickClearButton();
        void onDisplayLCD(const QString &str);
        void showPendingTimeEpoch(const quint32 ntpEpoch);
        void onSendingRemoteSync();

signals:
=======
class UartReceive;
class CryptoEngine;

class ParentWindow : public QMainWindow
{
  Q_OBJECT

public:
  ParentWindow(QWidget *parent = nullptr);
  void getPlainTextArr();
  ~ParentWindow();

  UartReceive* getUART() const
  {
    return uart;
  }


private:
  Ui::ParentWindow *ui;
  UartReceive *uart;
  CryptoEngine *m_crypto;


public slots:
  void onEnterPresssedTextEdit();
  void onPressingSendButton();
  void updateLineEditWidget(const QString &Str);


signals:
  void PlainTextFromWidgetReady(const QString &Str);
  void ByteTextEditReady(const QString &Str);
>>>>>>> c7bc156 (AES-256-CBC Cryptography. UI support for STM32F769I-DISCO_CryptoManager)

};
#endif // PARENTWINDOW_H
