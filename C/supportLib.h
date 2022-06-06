#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <errno.h>

unsigned char *DoubleArrayToByteArray(double *data, size_t length);
void WriteToFile(double *data, size_t dataLength, char *filename);
double *ByteArrayToDoubleArray(unsigned char *data, size_t length);
_Bool ReadTextFromFile(char *filename, wchar_t **text, size_t *textLength);
void PrintString(wchar_t *text, size_t textLength);
void PrintStringToStderr(wchar_t *text, size_t textLength);

