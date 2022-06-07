#include "TextualTemplates.h"
#include "supportLib.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
	_Bool success;
	int code;
	wchar_t *template, *json;
	size_t templateLength, jsonLength;
	Element *data;
	ElementReference *elRef;
	StringReference *document, *errorMessage;
	StringArrayReference *errorMessages;
	int i;
	char *templateFilename, *jsonFilename;

	if(argc == 3){
		templateFilename = argv[1];
		jsonFilename = argv[2];

		if(access(templateFilename, R_OK) == 0){
			if(access(jsonFilename, R_OK) == 0){
				elRef = (ElementReference *)malloc(sizeof(ElementReference));
				document = CreateStringReference(L"", 0.0);
				errorMessages = CreateStringArrayReferenceLengthValue(0, L"", 0);

				success = ReadTextFromFile(templateFilename, &template, &templateLength);

				if(success){

					success = ReadTextFromFile(jsonFilename, &json, &jsonLength);

					if(success){

						// ----------------
						success = ReadJSON(json, jsonLength, elRef, errorMessages);

						if(success){
							data = elRef->element;

							errorMessage = CreateStringReference(L"", 0);
							success = GenerateDocumentBasedOnElement(template, templateLength, data, document, errorMessage);
							// ----------------

							if(success){
								PrintString(document->string, document->stringLength);

								code = 0;
							}else{
								fprintf(stderr, "Generating result failed: ");
								PrintStringToStderr(errorMessage->string, errorMessage->stringLength);
								fprintf(stderr, "\n");
								code = -1;
							}
						}else{
							fprintf(stderr, "Loading JSON failed.\n");
							for(i = 0; i < errorMessages->stringArrayLength; i++){
								fprintf(stderr, " - ");
								PrintStringToStderr(errorMessages->stringArray[i]->string, errorMessages->stringArray[i]->stringLength);
								fprintf(stderr, "\n");
							}
							code = -1;
						}
					}else{
						fprintf(stderr, "Cannot read json-file.\n");
						code = -1;
					}
				}else{
					fprintf(stderr, "Cannot read template-file.\n");
					code = -1;
				}
			}else{
				fprintf(stderr, "Cannot open json-file: %s\n", jsonFilename);
				code = -1;
			}
		}else{
			fprintf(stderr, "Cannot open template-file: %s\n", templateFilename);
			code = -1;
		}
	}else{
		fprintf(stderr, "Usage: <template-filename> <json-filename>\n");
		code = -1;
	}

	return code;
}
