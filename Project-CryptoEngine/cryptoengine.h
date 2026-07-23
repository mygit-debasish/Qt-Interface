#ifndef CRYPTOENGINE_H
#define CRYPTOENGINE_H

#include <QObject>
#include <QDebug>
#include "uartreceive.h"

/* Forward declaration */
class UartReceive;

#define IV_LEN        16U
#define HASH_LEN_256  32U
#define KEY_LEN       16U

class CryptoEngine : public QObject
{
  Q_OBJECT

public:
  void encryptMsgECB(const uint8_t inMsg[16], uint8_t outMsg[16], const uint8_t key[16]);
  void decryptMsgECB(const uint8_t inMsg[16], uint8_t outMsg[16], const uint8_t key[16]);

  void encryptMsgCBC(const uint8_t plainText[16], size_t plainTextLen,
                    uint8_t ciperText[16], const uint8_t key[16], uint8_t iv[16]);

  void decryptMsgCBC(const uint8_t ciperText[16],size_t ciperTextLen,
                      uint8_t decryptText[16], const uint8_t key[16], uint8_t iv[16]);

  QByteArray FormatEncryptData(const uint8_t *ciperText, size_t ciperTextLen,
                               const uint8_t *iv, size_t ivLen,
                               const uint8_t *inHash, size_t hashLen);

  void hashParameter(const uint8_t *pKey, uint8_t bKeyLen, uint8_t *hashVal);
  QByteArray generateRndNum(uint8_t len);

  int PadCBCPlainText(const uint8_t *pPlainMsg, size_t MsgLen, uint8_t *pPadPlainMsg);

private:

public slots:
  void onEncryptMsgCBCtoSTM();

public:
  explicit CryptoEngine(QObject *parent, UartReceive *uart);
  UartReceive *m_uart;
  uint8_t m_iv[IV_LEN];
  uint8_t m_key[KEY_LEN];
  uint8_t m_hash[HASH_LEN_256];
  QByteArray m_EncMsgTransmit;

signals:
  void updateLineEditWidget(const QString &str);

};

#endif // CRYPTOENGINE_H
