#include "custom-crypto.h"
#include<mbedtls/aes.h>
#include <mbedtls/entropy.h>
#include <mbedtls/sha256.h>
#include <QDebug>
#include <QRandomGenerator>

#define AES_ECB_MODE 0
#define AES_CBC_MODE 1

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

void encryptMsgCBC(const uint8_t plainText[16], size_t plainTextLen,
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

void decryptMsgCBC(const uint8_t ciperText[16],size_t ciperTextLen,
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

QByteArray generateRndNum(uint8_t len)
{
  qDebug()<<"Generaaing Random numer of "<<len<<"bytes..";
  QByteArray RndArr(len, 0);

  for (int i = 0; i < len; i++)
    {
      RndArr[i] = static_cast<char>(QRandomGenerator::global()->generate() & 0xFF);
    }

  return RndArr;
}

QByteArray FormatEncryptData(const uint8_t *ciperText, size_t ciperTextLen,
                             const uint8_t *iv, size_t ivLen,
                             const uint8_t *inHash, size_t hashLen)
{
  QByteArray TransmitCiper(0, ciperTextLen+ivLen+hashLen);

  TransmitCiper.append(reinterpret_cast<const char*>(iv), ivLen);
  TransmitCiper.append(reinterpret_cast<const char*>(ciperText), ciperTextLen);
  TransmitCiper.append(reinterpret_cast<const char*>(inHash), hashLen);

  return TransmitCiper;
}

#define HASH_LEN_256 32U

void hashParameter(const uint8_t *pParam, uint8_t bParamLen, uint8_t *hashVal)
{
  mbedtls_sha256(pParam, bParamLen, hashVal, 0);
}

