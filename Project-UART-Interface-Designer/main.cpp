#include "parentwindow.h"
#include <QApplication>
#include <QDebug>
#include <ntpserver.h>
#include <mbedtls/aes.h>
#include <mbedtls/md.h>
#include <mbedtls/version.h>
#include "custom-crypto.h"
#include "cryptomanager.h"
#include "uartreceive.h"

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

const uint8_t plainMsg[] =
        {
          0xDE, 0xBA, 0xF0, 0xBA,
          0x04, 0x05, 0x06, 0x07,
          0x08, 0x09, 0x0A, 0x0B,
          0x0C, 0x0D, 0x0E, 0x0F,
          0x10, 0x11, 0x12, 0x13,
          0x14, 0x15, 0x16, 0x17,
          0x18, 0x19, 0x1A, 0x1B,
          0x1C, 0x1D, 0x1E, 0x1F
        };
uint8_t ciperMsg[32];
uint8_t decpMsg[32];

uint8_t iv_enc[16];
uint8_t iv_dec[16];

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParentWindow w;
    UartReceive *uart = new UartReceive(&w);
    uart->connectSerialPort("/dev/ttyACM0");


    QFont font("Ubuntu Mono", 12);


    QByteArray ivByteArray = generateRndNum(16);
    qDebug()<<"Genarated IV: "<<ivByteArray.toHex(' ').toUpper();

    memcpy(iv, ivByteArray.constData(), 16);
    memcpy(iv_enc, iv, sizeof (iv));
    memcpy(iv_dec, iv, sizeof (iv));

    /* Hash the Key */
    uint8_t inHash[HASH_LEN_256];

    /* Generating Hash of IV */
    hashParameter(iv_enc, sizeof(iv_enc), inHash);
    QByteArray HashVal(reinterpret_cast<const char*>(inHash), 32);
    qDebug()<<"HASH Value: "<<HashVal.toHex(' ').toUpper();

    encryptMsgCBC(plainMsg, sizeof(plainMsg), ciperMsg, key, iv_enc );
    QByteArray txEnc = FormatEncryptData(ciperMsg, sizeof (ciperMsg), iv,
                                         sizeof (iv), inHash, HASH_LEN_256);

    qDebug() <<"Transmit Frame: "<<txEnc.toHex(' ').toUpper();
    qDebug() <<"Transmit Frame Len: "<<txEnc.length();

    /* Sendind the data to STM32 */
    uart->SendEncryptMsg(txEnc);

    QByteArray plainData(reinterpret_cast<const char *>(plainMsg), 32);
    QByteArray ciperData(reinterpret_cast<const char *>(ciperMsg), 32);
    qDebug().noquote() << "Input :" << plainData.toHex(' ').toUpper();
    qDebug().noquote() << "Encrypted:" << ciperData.toHex(' ').toUpper();

#if 0
    decryptMsgCBC(ciperMsg, sizeof(ciperMsg), decpMsg, key, iv_dec );

    QByteArray inMsg(reinterpret_cast<const char *>(ciperMsg), 32);
    QByteArray outMsg(reinterpret_cast<const char *>(decpMsg), 32);
    qDebug().noquote() << "Decrypted:" << outMsg.toHex(' ').toUpper();
#endif

    w.setFont(font);
    w.show();

    return a.exec();
}
