#ifndef CUSTOMCRYPTO_H
#define CUSTOMCRYPTO_H

#include <stdint.h>
#include <stdio.h>
#include <QByteArray>

#define HASH_LEN_256 32U

/* Function Prototypes */
void encryptMsgECB(const uint8_t inMsg[16], uint8_t outMsg[16], const uint8_t key[16]);
void decryptMsgECB(const uint8_t inMsg[16], uint8_t outMsg[16], const uint8_t key[16]);

void encryptMsgCBC(const uint8_t plainText[16], size_t plainTextLen,
                  uint8_t ciperText[16], const uint8_t key[16], uint8_t iv[16]);

void decryptMsgCBC(const uint8_t ciperText[16],size_t ciperTextLen,
                    uint8_t decryptText[16], const uint8_t key[16], uint8_t iv[16]);

QByteArray generateRndNum(uint8_t len);
QByteArray FormatEncryptData(const uint8_t *ciperText, size_t ciperTextLen,
                             const uint8_t *iv, size_t ivLen,
                             const uint8_t *inHash, size_t hashLen);

void hashParameter(const uint8_t *pKey, uint8_t bKeyLen, uint8_t *hashVal);


#endif // CUSTOMCRYPTO_H
