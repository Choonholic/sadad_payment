// ICCardLoader.h

#ifndef ICCARDLOADER_H
#define ICCARDLOADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BBICKEYINJECTOR_NOERROR
#define BBICKEYINJECTOR_NOERROR							(0)
#endif

#ifndef BBICKEYINJECTOR_ERROR_INVALID_CARD
#define BBICKEYINJECTOR_ERROR_INVALID_CARD				(-1)
#endif

#ifndef BBICKEYINJECTOR_ERROR_ICREADER_FAIL
#define BBICKEYINJECTOR_ERROR_ICREADER_FAIL				(-2)
#endif

#ifndef BBICKEYINJECTOR_ERROR_INVALID_ARGUMENT
#define BBICKEYINJECTOR_ERROR_INVALID_ARGUMENT			(-3)
#endif

#ifndef BBICKEYINJECTOR_ERROR_MALLOC_FAIL
#define BBICKEYINJECTOR_ERROR_MALLOC_FAIL				(-4)
#endif

#ifndef BBICKEYINJECTOR_ERROR_SECULIB_FAIL
#define BBICKEYINJECTOR_ERROR_SECULIB_FAIL				(-5)
#endif

#ifndef BBICKEYINJECTOR_ERROR_CARD_NOT_INSERTED
#define BBICKEYINJECTOR_ERROR_CARD_NOT_INSERTED			(-6)
#endif

#ifndef BBICKEYINJECTOR_ERROR_INITIAL_UPDATE
#define BBICKEYINJECTOR_ERROR_INITIAL_UPDATE			(-101)
#endif

#ifndef BBICKEYINJECTOR_ERROR_EXTERNAL_AUTHENTICATE
#define BBICKEYINJECTOR_ERROR_EXTERNAL_AUTHENTICATE		(-102)
#endif

#ifndef BBICKEYINJECTOR_ERROR_SELECT_APPLET
#define BBICKEYINJECTOR_ERROR_SELECT_APPLET				(-103)
#endif

#ifndef BBICKEYINJECTOR_ERROR_VERIFY_PIN
#define BBICKEYINJECTOR_ERROR_VERIFY_PIN				(-104)
#endif

#ifndef BBICKEYINJECTOR_ERROR_GET_MASTER_KEY
#define BBICKEYINJECTOR_ERROR_GET_MASTER_KEY			(-105)
#endif

#ifndef BBICKEYINJECTOR_ERROR_INVALID_KEY
#define BBICKEYINJECTOR_ERROR_INVALID_KEY				(-106)
#endif

#ifndef BBICKEYINJECTOR_ERROR_PIN_WRONG
#define BBICKEYINJECTOR_ERROR_PIN_WRONG					(-107)
#endif

int BBICKEYINJECTOR_LoadKeyFromSecureICCard(unsigned char* PIN, int PINLength, unsigned char* privExponent, int privLength, unsigned char* pubModulus, int pubLength, unsigned char keyIndex, unsigned char* key, int* keyLength, int keyBuffersize, unsigned char* statusword);

#ifdef __cplusplus
}
#endif

#endif
