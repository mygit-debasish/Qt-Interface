#include "cryptoengine.h"
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/sha256.h"
#include <QRandomGenerator>
#include <uartreceive.h>

#define AES_ECB_MODE 0
#define AES_CBC_MODE 1

#define IV_LEN        16U
#define HASH_LEN_256  32U
#define KEY_LEN       16U

/* Encryption Specific Data */
uint8_t iv[16] =
{
    0x7A, 0x2F, 0x19, 0xC8,
    0xB3, 0x44, 0xD1, 0x0E,
    0x65, 0x92, 0xA7, 0x3C,
    0xF8, 0x11, 0x6D, 0x9B
};

uint8_t key[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                    0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0xAA, 0xBB
                  };
#if 0
uint8_t plainMsg[] =
        {
          0xDE, 0xBA, 0xDE, 0xBA,
          0xEE, 0xEE, 0xEF, 0xEF,
          0x08, 0x09, 0x0A, 0x0B,
          0x0C, 0x0D, 0x0E, 0x0F,
          0x10, 0x11, 0x12, 0x13,
          0x14, 0x15, 0x16, 0x17,
          0x18, 0x19, 0x1A, 0x1B,
          0x1C, 0x1D, 0x1E, 0x1F
        };
#endif

uint8_t plainMsg[100] = { 0x00 };

uint8_t ciperMsg[100];
uint8_t decpMsg[100];

uint8_t iv_enc[16];
uint8_t iv_dec[16];

CryptoEngine::CryptoEngine(QObject *parent, UartReceive *uart)
              :m_uart(uart)
{
  qDebug()<<"[CONSTRUCTOR]: "<<__PRETTY_FUNCTION__;
}

#if AES_ECB_MODE

void encryptMsgECB(const uint8_t inMsg[16], uint8_t outMsg[16], const uint8_t key[16])
{
  mbedtls_aes_context ctx;
  mbedtls_aes_init(&ctx);

  if(mbedtls_aes_setkey_enc(&ctx, key, 128) != 0)
    {
      qDebug()<<"Invalid Key ..";
      mbedtls_aes_free(&ctx);
      return;
    }

  mbedtls_aes_crypt_ecb(&ctx,
                        MBEDTLS_AES_ENCRYPT,
                        inMsg,
                        outMsg);
  mbedtls_aes_free(&ctx);
}

void decryptMsgECB(const uint8_t inMsg[16], uint8_t outMsg[16], const uint8_t key[16])
{
  mbedtls_aes_context ctx;
  mbedtls_aes_init(&ctx);

  if(mbedtls_aes_setkey_dec(&ctx, key, 128) != 0)
    {
      qDebug()<<"Invalid Key ..";
      mbedtls_aes_free(&ctx);
      return;
    }

  mbedtls_aes_crypt_ecb(&ctx,
                        MBEDTLS_AES_DECRYPT,
                        inMsg,
                        outMsg);

  mbedtls_aes_free(&ctx);
}

#elif AES_CBC_MODE

void CryptoEngine::encryptMsgCBC(const uint8_t plainText[16], size_t plainTextLen,
                  uint8_t ciperText[16], const uint8_t key[16], uint8_t iv[16])
{
  mbedtls_aes_context ctx;
  mbedtls_aes_init(&ctx);

  if(mbedtls_aes_setkey_enc(&ctx, key, 128) != 0)
    {
      qDebug()<<"Invalid Key ..";
      mbedtls_aes_free(&ctx);
      return;
    }

  mbedtls_aes_crypt_cbc(&ctx,
                        MBEDTLS_AES_ENCRYPT,
                        plainTextLen,
                        iv,
                        plainText,
                        ciperText);
  mbedtls_aes_free(&ctx);
}

void CryptoEngine::decryptMsgCBC(const uint8_t ciperText[16],size_t ciperTextLen,
                    uint8_t decryptText[16], const uint8_t key[16], uint8_t iv[16])
{
  mbedtls_aes_context ctx;
  mbedtls_aes_init(&ctx);

  if(mbedtls_aes_setkey_dec(&ctx, key, 128) != 0)
    {
      qDebug()<<"Invalid Key ..";
      mbedtls_aes_free(&ctx);
      return;
    }

  mbedtls_aes_crypt_cbc(&ctx,
                        MBEDTLS_AES_DECRYPT,
                        ciperTextLen,
                        iv,
                        ciperText,
                        decryptText);
  mbedtls_aes_free(&ctx);
}
#endif

QByteArray CryptoEngine::generateRndNum(uint8_t len)
{
  qDebug()<<"Generaaing Random numer of "<<len<<"bytes..";
  QByteArray RndArr(len, 0);

  for (int i = 0; i < len; i++)
    {
      RndArr[i] = static_cast<char>(QRandomGenerator::global()->generate() & 0xFF);
    }

  return RndArr;
}

#define SYNC1_BYTE  0x55U
#define SYNC2_BYTE  0xAAU
#define ALGO_AES256 0xBBU
#define MODE_CBC    0xDDU
#define SYNC_LEN    0x02U
#define ALGO_MODE_LEN 0x02U

QByteArray CryptoEngine::FormatEncryptData(const uint8_t *ciperText, size_t ciperTextLen,
                             const uint8_t *iv, size_t ivLen,
                             const uint8_t *inHash, size_t hashLen)
{
  QByteArray TransmitCipher;

  TransmitCipher.append(static_cast<char>(SYNC1_BYTE));
  TransmitCipher.append(static_cast<char>(SYNC2_BYTE));
  TransmitCipher.append(static_cast<char>(ALGO_AES256));
  TransmitCipher.append(static_cast<char>(MODE_CBC));
  TransmitCipher.append(static_cast<char>((ciperTextLen >> 8) & 0xFF));
  TransmitCipher.append(static_cast<char>(ciperTextLen & 0xFF));
  TransmitCipher.append(reinterpret_cast<const char*>(iv), ivLen);
  TransmitCipher.append(reinterpret_cast<const char*>(ciperText), ciperTextLen);
  TransmitCipher.append(reinterpret_cast<const char*>(inHash), hashLen);

  return TransmitCipher;
}

void CryptoEngine::hashParameter(const uint8_t *pParam, uint8_t bParamLen, uint8_t *hashVal)
{
  mbedtls_sha256(pParam, bParamLen, hashVal, 0);
}

#define CBC_MSG_BLOCK  16U
int CryptoEngine::PadCBCPlainText(const uint8_t *pPlainMsg, size_t MsgLen, uint8_t *pPadPlainMsg)
{
  size_t PadLen = ((MsgLen / (CBC_MSG_BLOCK) ) + 1) * (CBC_MSG_BLOCK) - MsgLen;

  if(MsgLen % CBC_MSG_BLOCK == 0)
    {
      PadLen = CBC_MSG_BLOCK;
    }

  memcpy(pPadPlainMsg, pPlainMsg, MsgLen);
  memset(pPadPlainMsg + MsgLen, PadLen, PadLen);

  return MsgLen + PadLen;
}

void CryptoEngine::onEncryptMsgCBCtoSTM()
{

  QByteArray ivByteArray = generateRndNum(16);

  memcpy(iv, ivByteArray.constData(), 16);
  memcpy(iv_enc, iv, sizeof (iv));
  memcpy(iv_dec, iv, sizeof (iv));

  /* Hash the Key */
  uint8_t inHash[HASH_LEN_256];

  /* Generating Hash of IV */
  hashParameter(iv_enc, sizeof(iv_enc), inHash);
  QByteArray HashVal(reinterpret_cast<const char*>(inHash), 32);

  /* Updating member variables */
  memcpy(m_iv, iv, IV_LEN);
  memcpy(m_hash, inHash, HASH_LEN_256);
  memcpy(m_key, key, KEY_LEN);

  qDebug()<<"IV    : "<<ivByteArray.toHex(' ').toUpper();
  qDebug()<<"HASH  : "<<HashVal.toHex(' ').toUpper();

  /* Process Plaintext from UI */
  int paddedLen = PadCBCPlainText(
                reinterpret_cast<const uint8_t*>(m_uart->getqPlainText().constData()),
                static_cast<size_t>(m_uart->getqPlainText().size()),
                plainMsg);

  QByteArray plainData(reinterpret_cast<const char*>(plainMsg), paddedLen);
  qDebug()<<"Padded Message: "<<plainData.toHex(' ').toUpper();
  qDebug()<<"Padded Length: "<<paddedLen;

  encryptMsgCBC(plainMsg,
                paddedLen,
                ciperMsg,
                key,
                iv_enc );

  m_EncMsgTransmit = FormatEncryptData(ciperMsg,
                                       paddedLen,
                                       iv,
                                       sizeof (iv),
                                       inHash,
                                       HASH_LEN_256);
  /* Transmit Encrypt data to STM32 */

  qDebug()<<"Total Bytes :"<<m_EncMsgTransmit.size();
  m_uart->SendEncryptMsg(m_EncMsgTransmit);
  emit updateLineEditWidget("You Win");
}
