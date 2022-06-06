#include "supportLib.h"

unsigned char *DoubleArrayToByteArray(double *data, size_t length){
	unsigned char *out;
	size_t i;

	out = (unsigned char*)malloc(sizeof(unsigned char)*length);

	for(i = 0; i < length; i++){
		out[i] = data[i];
	}

	return out;
}

void WriteToFile(double *data, size_t dataLength, char *filename){
	unsigned char *bytes;

	bytes = DoubleArrayToByteArray(data, dataLength);

	FILE* file = fopen(filename, "wb");
	fwrite(bytes, 1, dataLength, file);

	free(bytes);
}

double *ByteArrayToDoubleArray(unsigned char *data, size_t length){
	double *out;
	size_t i;

	out = (double*)malloc(sizeof(double)*length);

	for(i = 0; i < length; i++){
		out[i] = data[i];
	}

	return out;
}

_Bool ReadTextFromFile(char *filename, wchar_t **text, size_t *textLength){
	FILE *file;
	unsigned char *bytes;
	size_t bytesLength;
	const char *tmp;
	_Bool success;

	file = fopen(filename, "rb");
	if(file != NULL){
		fseek(file, 0, SEEK_END);
		bytesLength = ftell(file);
		rewind(file);

		// +4 extra bytes for 0-termination
		bytes = (unsigned char *)malloc((bytesLength + 4) * sizeof(unsigned char));
		bytesLength = fread(bytes, 1, bytesLength, file);
		fclose(file);

		bytes[bytesLength + 0] = '\0';
		bytes[bytesLength + 1] = '\0';
		bytes[bytesLength + 2] = '\0';
		bytes[bytesLength + 3] = '\0';

		//fprintf(stderr, "bytes: %ld\n", bytesLength);

		setlocale(LC_CTYPE, "en_US.utf8");
		*text = (wchar_t *)malloc(bytesLength * sizeof(wchar_t));
		tmp = bytes;
		*textLength = mbsrtowcs(*text, &tmp, bytesLength, NULL);

		if(*textLength != -1){
			//fprintf(stderr, "chars: %ld", *textLength);

			free(bytes);
			success = true;
		}else{
					fprintf(stderr, "errno: %d, %ld\n", errno, ((char*)tmp - (char*)bytes));
			success = false;
		}
	}else{
		success = false;
	}

	return success;
}

void PrintString(wchar_t *text, size_t textLength){
	int i;

	for(i = 0; i < textLength; i++){
		putwchar(text[i]);
	}
}

void PrintStringToStderr(wchar_t *text, size_t textLength){
	int i;

	for(i = 0; i < textLength; i++){
		fprintf(stderr, "%lc", text[i]);
		//fputwc(text[i], stderr);
	}
}







