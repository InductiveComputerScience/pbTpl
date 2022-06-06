#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <wchar.h>

#define strparam(str) (str), wcslen(str)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct LinkedListNodeNodes;
typedef struct LinkedListNodeNodes LinkedListNodeNodes;

struct LinkedListNodes;
typedef struct LinkedListNodes LinkedListNodes;

struct Node;
typedef struct Node Node;

struct BooleanArrayReference;
typedef struct BooleanArrayReference BooleanArrayReference;

struct BooleanReference;
typedef struct BooleanReference BooleanReference;

struct CharacterReference;
typedef struct CharacterReference CharacterReference;

struct NumberArrayReference;
typedef struct NumberArrayReference NumberArrayReference;

struct NumberReference;
typedef struct NumberReference NumberReference;

struct StringArrayReference;
typedef struct StringArrayReference StringArrayReference;

struct StringReference;
typedef struct StringReference StringReference;

struct ElementArrayReference;
typedef struct ElementArrayReference ElementArrayReference;

struct LinkedListElements;
typedef struct LinkedListElements LinkedListElements;

struct LinkedListNodeElements;
typedef struct LinkedListNodeElements LinkedListNodeElements;

struct Element;
typedef struct Element Element;

struct ElementReference;
typedef struct ElementReference ElementReference;

struct ElementType;
typedef struct ElementType ElementType;

struct StringElementMap;
typedef struct StringElementMap StringElementMap;

struct DynamicArrayCharacters;
typedef struct DynamicArrayCharacters DynamicArrayCharacters;

struct LinkedListNodeStrings;
typedef struct LinkedListNodeStrings LinkedListNodeStrings;

struct LinkedListStrings;
typedef struct LinkedListStrings LinkedListStrings;

struct LinkedListNodeNumbers;
typedef struct LinkedListNodeNumbers LinkedListNodeNumbers;

struct LinkedListNumbers;
typedef struct LinkedListNumbers LinkedListNumbers;

struct LinkedListCharacters;
typedef struct LinkedListCharacters LinkedListCharacters;

struct LinkedListNodeCharacters;
typedef struct LinkedListNodeCharacters LinkedListNodeCharacters;

struct DynamicArrayNumbers;
typedef struct DynamicArrayNumbers DynamicArrayNumbers;

struct lLinkedListNodeStrings;
typedef struct lLinkedListNodeStrings lLinkedListNodeStrings;

struct lLinkedListStrings;
typedef struct lLinkedListStrings lLinkedListStrings;

struct lLinkedListNodeNumbers;
typedef struct lLinkedListNodeNumbers lLinkedListNodeNumbers;

struct lLinkedListNumbers;
typedef struct lLinkedListNumbers lLinkedListNumbers;

struct lLinkedListCharacters;
typedef struct lLinkedListCharacters lLinkedListCharacters;

struct lLinkedListNodeCharacters;
typedef struct lLinkedListNodeCharacters lLinkedListNodeCharacters;

struct lDynamicArrayNumbers;
typedef struct lDynamicArrayNumbers lDynamicArrayNumbers;

struct LinkedListNodeNodes{
  _Bool end;
  Node *value;
  LinkedListNodeNodes *next;
};

struct LinkedListNodes{
  LinkedListNodeNodes *first;
  LinkedListNodeNodes *last;
};

struct Node{
  wchar_t *type;
  size_t typeLength;
  wchar_t *p1;
  size_t p1Length;
  wchar_t *p2;
  size_t p2Length;
  Node *block1;
  _Bool hasElseBlock;
  Node *block2;
  LinkedListNodes *nodes;
};

struct BooleanArrayReference{
  _Bool *booleanArray;
  size_t booleanArrayLength;
};

struct BooleanReference{
  _Bool booleanValue;
};

struct CharacterReference{
  wchar_t characterValue;
};

struct NumberArrayReference{
  double *numberArray;
  size_t numberArrayLength;
};

struct NumberReference{
  double numberValue;
};

struct StringArrayReference{
  StringReference **stringArray;
  size_t stringArrayLength;
};

struct StringReference{
  wchar_t *string;
  size_t stringLength;
};

struct ElementArrayReference{
  Element **array;
  size_t arrayLength;
};

struct LinkedListElements{
  LinkedListNodeElements *first;
  LinkedListNodeElements *last;
};

struct LinkedListNodeElements{
  _Bool end;
  Element *value;
  LinkedListNodeElements *next;
};

struct Element{
  wchar_t *type;
  size_t typeLength;
  StringElementMap *object;
  Element **array;
  size_t arrayLength;
  wchar_t *string;
  size_t stringLength;
  double number;
  _Bool booleanValue;
};

struct ElementReference{
  Element *element;
};

struct ElementType{
  wchar_t *name;
  size_t nameLength;
};

struct StringElementMap{
  StringArrayReference *stringListRef;
  ElementArrayReference *elementListRef;
};

struct DynamicArrayCharacters{
  wchar_t *array;
  size_t arrayLength;
  double length;
};

struct LinkedListNodeStrings{
  _Bool end;
  wchar_t *value;
  size_t valueLength;
  LinkedListNodeStrings *next;
};

struct LinkedListStrings{
  LinkedListNodeStrings *first;
  LinkedListNodeStrings *last;
};

struct LinkedListNodeNumbers{
  LinkedListNodeNumbers *next;
  _Bool end;
  double value;
};

struct LinkedListNumbers{
  LinkedListNodeNumbers *first;
  LinkedListNodeNumbers *last;
};

struct LinkedListCharacters{
  LinkedListNodeCharacters *first;
  LinkedListNodeCharacters *last;
};

struct LinkedListNodeCharacters{
  _Bool end;
  wchar_t value;
  LinkedListNodeCharacters *next;
};

struct DynamicArrayNumbers{
  double *array;
  size_t arrayLength;
  double length;
};

struct lLinkedListNodeStrings{
  _Bool end;
  wchar_t *value;
  size_t valueLength;
  lLinkedListNodeStrings *next;
};

struct lLinkedListStrings{
  lLinkedListNodeStrings *first;
  lLinkedListNodeStrings *last;
};

struct lLinkedListNodeNumbers{
  lLinkedListNodeNumbers *next;
  _Bool end;
  double value;
};

struct lLinkedListNumbers{
  lLinkedListNodeNumbers *first;
  lLinkedListNodeNumbers *last;
};

struct lLinkedListCharacters{
  lLinkedListNodeCharacters *first;
  lLinkedListNodeCharacters *last;
};

struct lLinkedListNodeCharacters{
  _Bool end;
  wchar_t value;
  lLinkedListNodeCharacters *next;
};

struct lDynamicArrayNumbers{
  double *array;
  size_t arrayLength;
  double length;
};

LinkedListNodes *CreateLinkedListNodes();
void LinkedListAddNode(LinkedListNodes *ll, Node *value);
Node **LinkedListNodesToArray(size_t *returnArrayLength, LinkedListNodes *ll);
double LinkedListNodesLength(LinkedListNodes *ll);
void FreeLinkedListNode(LinkedListNodes *ll);

_Bool IsValidTemplate(wchar_t *template, size_t templateLength);
_Bool GenerateTokensFromTemplate(wchar_t *template, size_t templateLength, LinkedListStrings *tokens, StringReference *errorMessage);
_Bool GenerateDocument(wchar_t *template, size_t templateLength, Element *data, StringReference *document, StringReference *errorMessage);
_Bool GenerateDocumentFromBlock(Node *root, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
_Bool GenerateDocumentFromNode(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
_Bool GenerateDocumentFromUse(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
_Bool GenerateDocumentFromIf(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
_Bool GenerateDocumentFromForeach(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
_Bool ParseTemplate(StringReference **tokens, size_t tokensLength, Node *node, StringReference *errorMessage);
_Bool ParseTemplateBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *node, StringReference *errorMessage);
_Bool ParseUseBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *useBlock, StringReference *errorMessage);
_Bool ParseIfBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *ifBlock, StringReference *errorMessage);
_Bool ParseForeachBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *foreachBlock, StringReference *errorMessage);
_Bool ParseNodeString(wchar_t *token, size_t tokenLength, Node *node, StringReference *errorMessage);

double test();
void testTokenGeneration(NumberReference *failures);
void testGenerateDocument1(NumberReference *failures);
void testGenerateDocument2(NumberReference *failures);
void testGenerateDocument3(NumberReference *failures);
void testGenerateDocument4(NumberReference *failures);
void testGenerateDocument5(NumberReference *failures);
void testGenerateDocument6(NumberReference *failures);
void testGenerateDocument7(NumberReference *failures);
void AssertTemplateResult(wchar_t *template, size_t templateLength, wchar_t *json, size_t jsonLength, wchar_t *result, size_t resultLength, NumberReference *failures);
void AssertTemplateError(wchar_t *template, size_t templateLength, wchar_t *json, size_t jsonLength, wchar_t *errorMessage, size_t errorMessageLength, NumberReference *failures);

BooleanReference *CreateBooleanReference(_Bool value);
BooleanArrayReference *CreateBooleanArrayReference(_Bool *value, size_t valueLength);
BooleanArrayReference *CreateBooleanArrayReferenceLengthValue(double length, _Bool value);
void FreeBooleanArrayReference(BooleanArrayReference *booleanArrayReference);
CharacterReference *CreateCharacterReference(wchar_t value);
NumberReference *CreateNumberReference(double value);
NumberArrayReference *CreateNumberArrayReference(double *value, size_t valueLength);
NumberArrayReference *CreateNumberArrayReferenceLengthValue(double length, double value);
void FreeNumberArrayReference(NumberArrayReference *numberArrayReference);
StringReference *CreateStringReference(wchar_t *value, size_t valueLength);
StringReference *CreateStringReferenceLengthValue(double length, wchar_t value);
void FreeStringReference(StringReference *stringReference);
StringArrayReference *CreateStringArrayReference(StringReference **strings, size_t stringsLength);
StringArrayReference *CreateStringArrayReferenceLengthValue(double length, wchar_t *value, size_t valueLength);
void FreeStringArrayReference(StringArrayReference *stringArrayReference);

_Bool IsValidJSON(wchar_t *json, size_t jsonLength, StringArrayReference *errorMessage);

_Bool JSONTokenize(wchar_t *json, size_t jsonLength, StringArrayReference *tokensReference, StringArrayReference *errorMessages);
_Bool GetJSONNumberToken(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, StringArrayReference *errorMessages);
_Bool IsValidJSONNumber(wchar_t *n, size_t nLength, StringArrayReference *errorMessages);
_Bool IsValidJSONNumberAfterSign(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages);
double IsValidJSONNumberAdvancePastDigits(wchar_t *n, size_t nLength, double i);
_Bool IsValidJSONNumberFromDotOrExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages);
_Bool IsValidJSONNumberFromExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages);
_Bool IsJSONNumberCharacter(wchar_t c);
_Bool GetJSONPrimitiveName(wchar_t *string, size_t stringLength, double start, StringArrayReference *errorMessages, wchar_t *primitive, size_t primitiveLength, StringReference *tokenReference);
_Bool GetJSONString(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, NumberReference *stringLengthReference, StringArrayReference *errorMessages);
_Bool IsValidJSONString(wchar_t *jsonString, size_t jsonStringLength, StringArrayReference *errorMessages);
_Bool IsValidJSONStringInJSON(wchar_t *json, size_t jsonLength, double start, NumberReference *characterCount, NumberReference *stringLengthReference, StringArrayReference *errorMessages);
_Bool IsJSONIllegalControllCharacter(wchar_t c);

Element **AddElement(size_t *returnArrayLength, Element **list, size_t listLength, Element *a);
void AddElementRef(ElementArrayReference *list, Element *i);
Element **RemoveElement(size_t *returnArrayLength, Element **list, size_t listLength, double n);
Element *GetElementRef(ElementArrayReference *list, double i);
void RemoveElementRef(ElementArrayReference *list, double i);


LinkedListElements *CreateLinkedListElements();
void LinkedListAddElement(LinkedListElements *ll, Element *value);
Element **LinkedListElementsToArray(size_t *returnArrayLength, LinkedListElements *ll);
double LinkedListElementsLength(LinkedListElements *ll);
void FreeLinkedListElements(LinkedListElements *ll);


double ComputeJSONStringLength(Element *element);
double ComputeJSONBooleanStringLength(Element *element);
double ComputeJSONNumberStringLength(Element *element);
double ComputeJSONArrayStringLength(Element *element);
double ComputeJSONObjectStringLength(Element *element);

Element *CreateStringElement(wchar_t *string, size_t stringLength);
Element *CreateBooleanElement(_Bool booleanValue);
Element *CreateNullElement();
Element *CreateNumberElement(double number);
Element *CreateArrayElement(double length);
Element *CreateObjectElement(double length);
void DeleteElement(Element *element);
void DeleteObject(Element *element);
void DeleteArray(Element *element);

wchar_t *WriteJSON(size_t *returnArrayLength, Element *element);
void WriteBooleanValue(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
void WriteNumber(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
void WriteArray(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
void WriteString(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
wchar_t *JSONEscapeString(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
double JSONEscapedStringLength(wchar_t *string, size_t stringLength);
wchar_t *JSONEscapeCharacter(size_t *returnArrayLength, wchar_t c);
_Bool JSONMustBeEscaped(wchar_t c, NumberReference *letters);
void WriteObject(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);

_Bool ReadJSON(wchar_t *string, size_t stringLength, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONValue(StringReference **tokens, size_t tokensLength, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONValueRecursive(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONObject(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONArray(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);

StringArrayReference *GetStringElementMapKeySet(StringElementMap *stringElementMap);
Element *GetObjectValue(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength);
Element *GetObjectValueWithCheck(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength, BooleanReference *foundReference);
void PutStringElementMap(StringElementMap *stringElementMap, wchar_t *keystring, size_t keystringLength, Element *value);
void SetStringElementMap(StringElementMap *stringElementMap, double index, wchar_t *keystring, size_t keystringLength, Element *value);
double GetStringElementMapNumberOfKeys(StringElementMap *stringElementMap);

_Bool JSONCompare(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, double epsilon, BooleanReference *equal, StringArrayReference *errorMessage);
_Bool JSONCompareElements(Element *ea, Element *eb, double epsilon);
_Bool JSONCompareArrays(Element **ea, size_t eaLength, Element **eb, size_t ebLength, double epsilon);
_Bool JSONCompareObjects(StringElementMap *ea, StringElementMap *eb, double epsilon);

double *AddNumber(size_t *returnArrayLength, double *list, size_t listLength, double a);
void AddNumberRef(NumberArrayReference *list, double i);
double *RemoveNumber(size_t *returnArrayLength, double *list, size_t listLength, double n);
double GetNumberRef(NumberArrayReference *list, double i);
void RemoveNumberRef(NumberArrayReference *list, double i);

StringReference **AddString(size_t *returnArrayLength, StringReference **list, size_t listLength, StringReference *a);
void AddStringRef(StringArrayReference *list, StringReference *i);
StringReference **RemoveString(size_t *returnArrayLength, StringReference **list, size_t listLength, double n);
StringReference *GetStringRef(StringArrayReference *list, double i);
void RemoveStringRef(StringArrayReference *list, double i);


DynamicArrayCharacters *CreateDynamicArrayCharacters();
DynamicArrayCharacters *CreateDynamicArrayCharactersWithInitialCapacity(double capacity);
void DynamicArrayAddCharacter(DynamicArrayCharacters *da, wchar_t value);
void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters *da);
_Bool DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters *da);
void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters *da);
double DynamicArrayCharactersIndex(DynamicArrayCharacters *da, double index);
double DynamicArrayCharactersLength(DynamicArrayCharacters *da);
void DynamicArrayInsertCharacter(DynamicArrayCharacters *da, double index, wchar_t value);
_Bool DynamicArrayCharacterSet(DynamicArrayCharacters *da, double index, wchar_t value);
void DynamicArrayRemoveCharacter(DynamicArrayCharacters *da, double index);
void FreeDynamicArrayCharacters(DynamicArrayCharacters *da);
wchar_t *DynamicArrayCharactersToArray(size_t *returnArrayLength, DynamicArrayCharacters *da);
DynamicArrayCharacters *ArrayToDynamicArrayCharactersWithOptimalSize(wchar_t *array, size_t arrayLength);
DynamicArrayCharacters *ArrayToDynamicArrayCharacters(wchar_t *array, size_t arrayLength);
_Bool DynamicArrayCharactersEqual(DynamicArrayCharacters *a, DynamicArrayCharacters *b);
LinkedListCharacters *DynamicArrayCharactersToLinkedList(DynamicArrayCharacters *da);
DynamicArrayCharacters *LinkedListToDynamicArrayCharacters(LinkedListCharacters *ll);

_Bool *AddBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, _Bool a);
void AddBooleanRef(BooleanArrayReference *list, _Bool i);
_Bool *RemoveBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, double n);
_Bool GetBooleanRef(BooleanArrayReference *list, double i);
void RemoveDecimalRef(BooleanArrayReference *list, double i);


LinkedListStrings *CreateLinkedListString();
void LinkedListAddString(LinkedListStrings *ll, wchar_t *value, size_t valueLength);
StringReference **LinkedListStringsToArray(size_t *returnArrayLength, LinkedListStrings *ll);
double LinkedListStringsLength(LinkedListStrings *ll);
void FreeLinkedListString(LinkedListStrings *ll);


LinkedListNumbers *CreateLinkedListNumbers();
LinkedListNumbers **CreateLinkedListNumbersArray(size_t *returnArrayLength, double length);
void LinkedListAddNumber(LinkedListNumbers *ll, double value);
double LinkedListNumbersLength(LinkedListNumbers *ll);
double LinkedListNumbersIndex(LinkedListNumbers *ll, double index);
void LinkedListInsertNumber(LinkedListNumbers *ll, double index, double value);
void LinkedListSet(LinkedListNumbers *ll, double index, double value);
void LinkedListRemoveNumber(LinkedListNumbers *ll, double index);
void FreeLinkedListNumbers(LinkedListNumbers *ll);
void FreeLinkedListNumbersArray(LinkedListNumbers **lls, size_t llsLength);
double *LinkedListNumbersToArray(size_t *returnArrayLength, LinkedListNumbers *ll);
LinkedListNumbers *ArrayToLinkedListNumbers(double *array, size_t arrayLength);
_Bool LinkedListNumbersEqual(LinkedListNumbers *a, LinkedListNumbers *b);

LinkedListCharacters *CreateLinkedListCharacter();
void LinkedListAddCharacter(LinkedListCharacters *ll, wchar_t value);
wchar_t *LinkedListCharactersToArray(size_t *returnArrayLength, LinkedListCharacters *ll);
double LinkedListCharactersLength(LinkedListCharacters *ll);
void FreeLinkedListCharacter(LinkedListCharacters *ll);
void LinkedListCharactersAddString(LinkedListCharacters *ll, wchar_t *str, size_t strLength);



DynamicArrayNumbers *CreateDynamicArrayNumbers();
DynamicArrayNumbers *CreateDynamicArrayNumbersWithInitialCapacity(double capacity);
void DynamicArrayAddNumber(DynamicArrayNumbers *da, double value);
void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers *da);
_Bool DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers *da);
void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers *da);
double DynamicArrayNumbersIndex(DynamicArrayNumbers *da, double index);
double DynamicArrayNumbersLength(DynamicArrayNumbers *da);
void DynamicArrayInsertNumber(DynamicArrayNumbers *da, double index, double value);
_Bool DynamicArrayNumberSet(DynamicArrayNumbers *da, double index, double value);
void DynamicArrayRemoveNumber(DynamicArrayNumbers *da, double index);
void FreeDynamicArrayNumbers(DynamicArrayNumbers *da);
double *DynamicArrayNumbersToArray(size_t *returnArrayLength, DynamicArrayNumbers *da);
DynamicArrayNumbers *ArrayToDynamicArrayNumbersWithOptimalSize(double *array, size_t arrayLength);
DynamicArrayNumbers *ArrayToDynamicArrayNumbers(double *array, size_t arrayLength);
_Bool DynamicArrayNumbersEqual(DynamicArrayNumbers *a, DynamicArrayNumbers *b);
LinkedListNumbers *DynamicArrayNumbersToLinkedList(DynamicArrayNumbers *da);
DynamicArrayNumbers *LinkedListToDynamicArrayNumbers(LinkedListNumbers *ll);

wchar_t *AddCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, wchar_t a);
void AddCharacterRef(StringReference *list, wchar_t i);
wchar_t *RemoveCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, double n);
wchar_t GetCharacterRef(StringReference *list, double i);
void RemoveCharacterRef(StringReference *list, double i);

void sWriteStringToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t *src, size_t srcLength);
void sWriteCharacterToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t src);
void sWriteBooleanToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, _Bool src);

_Bool sSubstringWithCheck(wchar_t *string, size_t stringLength, double from, double to, StringReference *stringReference);
wchar_t *sSubstring(size_t *returnArrayLength, wchar_t *string, size_t stringLength, double from, double to);
wchar_t *sAppendString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length);
wchar_t *sConcatenateString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length);
wchar_t *sAppendCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c);
wchar_t *sConcatenateCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c);
StringReference **sSplitByCharacter(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t splitBy);
_Bool sIndexOfCharacter(wchar_t *string, size_t stringLength, wchar_t character, NumberReference *indexReference);
_Bool sSubstringEqualsWithCheck(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength, BooleanReference *equalsReference);
_Bool sSubstringEquals(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength);
_Bool sIndexOfString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength, NumberReference *indexReference);
_Bool sContainsCharacter(wchar_t *string, size_t stringLength, wchar_t character);
_Bool sContainsString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength);
void sToUpperCase(wchar_t *string, size_t stringLength);
void sToLowerCase(wchar_t *string, size_t stringLength);
_Bool sEqualsIgnoreCase(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);
wchar_t *sReplaceString(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t *toReplace, size_t toReplaceLength, wchar_t *replaceWith, size_t replaceWithLength);
wchar_t *sReplaceCharacterToNew(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith);
void sReplaceCharacter(wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith);
wchar_t *sTrim(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
_Bool sStartsWith(wchar_t *string, size_t stringLength, wchar_t *start, size_t startLength);
_Bool sEndsWith(wchar_t *string, size_t stringLength, wchar_t *end, size_t endLength);
StringReference **sSplitByString(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t *splitBy, size_t splitByLength);
_Bool sStringIsBefore(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);

void strWriteStringToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t *src, size_t srcLength);
void strWriteCharacterToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t src);
void strWriteBooleanToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, _Bool src);

_Bool strSubstringWithCheck(wchar_t *string, size_t stringLength, double from, double to, StringReference *stringReference);
wchar_t *strSubstring(size_t *returnArrayLength, wchar_t *string, size_t stringLength, double from, double to);
wchar_t *strAppendString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length);
wchar_t *strConcatenateString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length);
wchar_t *strAppendCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c);
wchar_t *strConcatenateCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c);
StringReference **strSplitByCharacter(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t splitBy);
_Bool strIndexOfCharacter(wchar_t *string, size_t stringLength, wchar_t character, NumberReference *indexReference);
_Bool strSubstringEqualsWithCheck(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength, BooleanReference *equalsReference);
_Bool strSubstringEquals(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength);
_Bool strIndexOfString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength, NumberReference *indexReference);
_Bool strContainsCharacter(wchar_t *string, size_t stringLength, wchar_t character);
_Bool strContainsString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength);
void strToUpperCase(wchar_t *string, size_t stringLength);
void strToLowerCase(wchar_t *string, size_t stringLength);
_Bool strEqualsIgnoreCase(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);
wchar_t *strReplaceString(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t *toReplace, size_t toReplaceLength, wchar_t *replaceWith, size_t replaceWithLength);
wchar_t *strReplaceCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith);
wchar_t *strTrim(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
_Bool strStartsWith(wchar_t *string, size_t stringLength, wchar_t *start, size_t startLength);
_Bool strEndsWith(wchar_t *string, size_t stringLength, wchar_t *end, size_t endLength);
StringReference **strSplitByString(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t *splitBy, size_t splitByLength);
_Bool strStringIsBefore(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);

double *StringToNumberArray(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
wchar_t *NumberArrayToString(size_t *returnArrayLength, double *array, size_t arrayLength);
_Bool NumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength);
_Bool BooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength);
_Bool StringsEqual(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);
void FillNumberArray(double *a, size_t aLength, double value);
void FillString(wchar_t *a, size_t aLength, wchar_t value);
void FillBooleanArray(_Bool *a, size_t aLength, _Bool value);
_Bool FillNumberArrayRange(double *a, size_t aLength, double value, double from, double to);
_Bool FillBooleanArrayRange(_Bool *a, size_t aLength, _Bool value, double from, double to);
_Bool FillStringRange(wchar_t *a, size_t aLength, wchar_t value, double from, double to);
double *CopyNumberArray(size_t *returnArrayLength, double *a, size_t aLength);
_Bool *CopyBooleanArray(size_t *returnArrayLength, _Bool *a, size_t aLength);
wchar_t *CopyString(size_t *returnArrayLength, wchar_t *a, size_t aLength);
_Bool CopyNumberArrayRange(double *a, size_t aLength, double from, double to, NumberArrayReference *copyReference);
_Bool CopyBooleanArrayRange(_Bool *a, size_t aLength, double from, double to, BooleanArrayReference *copyReference);
_Bool CopyStringRange(wchar_t *a, size_t aLength, double from, double to, StringReference *copyReference);
_Bool IsLastElement(double length, double index);
double *CreateNumberArray(size_t *returnArrayLength, double length, double value);
_Bool *CreateBooleanArray(size_t *returnArrayLength, double length, _Bool value);
wchar_t *CreateString(size_t *returnArrayLength, double length, wchar_t value);
void SwapElementsOfNumberArray(double *A, size_t ALength, double ai, double bi);
void SwapElementsOfStringArray(StringArrayReference *A, double ai, double bi);
void ReverseNumberArray(double *array, size_t arrayLength);

void AssertFalse(_Bool b, NumberReference *failures);
void AssertTrue(_Bool b, NumberReference *failures);
void AssertEquals(double a, double b, NumberReference *failures);
void AssertBooleansEqual(_Bool a, _Bool b, NumberReference *failures);
void AssertCharactersEqual(wchar_t a, wchar_t b, NumberReference *failures);
void AssertStringEquals(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, NumberReference *failures);
void AssertNumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength, NumberReference *failures);
void AssertBooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength, NumberReference *failures);
void AssertStringArraysEqual(StringReference **a, size_t aLength, StringReference **b, size_t bLength, NumberReference *failures);

wchar_t *nCreateStringScientificNotationDecimalFromNumber(size_t *returnArrayLength, double decimal);
wchar_t *nCreateStringDecimalFromNumber(size_t *returnArrayLength, double decimal);
_Bool nCreateStringFromNumberWithCheck(double decimal, double base, StringReference *stringReference);
double nGetMaximumDigitsForBase(double base);
double nGetFirstDigitPosition(double decimal, double base);
_Bool nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference *characterReference);
wchar_t *nGetDigitCharacterTable(size_t *returnArrayLength);

_Bool nCreateNumberFromDecimalStringWithCheck(wchar_t *string, size_t stringLength, NumberReference *decimalReference, StringReference *errorMessage);
double nCreateNumberFromDecimalString(wchar_t *string, size_t stringLength);
_Bool nCreateNumberFromStringWithCheck(wchar_t *string, size_t stringLength, double base, NumberReference *numberReference, StringReference *errorMessage);
double nCreateNumberFromParts(double base, _Bool numberIsPositive, double *beforePoint, size_t beforePointLength, double *afterPoint, size_t afterPointLength, _Bool exponentIsPositive, double *exponent, size_t exponentLength);
_Bool nExtractPartsFromNumberString(wchar_t *n, size_t nLength, double base, BooleanReference *numberIsPositive, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
_Bool nExtractPartsFromNumberStringFromSign(wchar_t *n, size_t nLength, double base, double i, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
_Bool nExtractPartsFromNumberStringFromPointOrExponent(wchar_t *n, size_t nLength, double base, double i, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
_Bool nExtractPartsFromNumberStringFromExponent(wchar_t *n, size_t nLength, double base, double i, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
double nGetNumberFromNumberCharacterForBase(wchar_t c, double base);
_Bool nCharacterIsNumberCharacterInBase(wchar_t c, double base);
double *nStringToNumberArray(size_t *returnArrayLength, wchar_t *str, size_t strLength);
_Bool nStringToNumberArrayWithCheck(wchar_t *str, size_t strLength, NumberArrayReference *numberArrayReference, StringReference *errorMessage);

double *lAddNumber(size_t *returnArrayLength, double *list, size_t listLength, double a);
void lAddNumberRef(NumberArrayReference *list, double i);
double *lRemoveNumber(size_t *returnArrayLength, double *list, size_t listLength, double n);
double lGetNumberRef(NumberArrayReference *list, double i);
void lRemoveNumberRef(NumberArrayReference *list, double i);

StringReference **lAddString(size_t *returnArrayLength, StringReference **list, size_t listLength, StringReference *a);
void lAddStringRef(StringArrayReference *list, StringReference *i);
StringReference **lRemoveString(size_t *returnArrayLength, StringReference **list, size_t listLength, double n);
StringReference *lGetStringRef(StringArrayReference *list, double i);
void lRemoveStringRef(StringArrayReference *list, double i);

_Bool *lAddBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, _Bool a);
void lAddBooleanRef(BooleanArrayReference *list, _Bool i);
_Bool *lRemoveBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, double n);
_Bool lGetBooleanRef(BooleanArrayReference *list, double i);
void lRemoveDecimalRef(BooleanArrayReference *list, double i);


lLinkedListStrings *lCreateLinkedListString();
void lLinkedListAddString(lLinkedListStrings *ll, wchar_t *value, size_t valueLength);
StringReference **lLinkedListStringsToArray(size_t *returnArrayLength, lLinkedListStrings *ll);
double lLinkedListStringsLength(lLinkedListStrings *ll);
void lFreeLinkedListString(lLinkedListStrings *ll);


lLinkedListNumbers *lCreateLinkedListNumbers();
lLinkedListNumbers **lCreateLinkedListNumbersArray(size_t *returnArrayLength, double length);
void lLinkedListAddNumber(lLinkedListNumbers *ll, double value);
double lLinkedListNumbersLength(lLinkedListNumbers *ll);
double lLinkedListNumbersIndex(lLinkedListNumbers *ll, double index);
void lLinkedListInsertNumber(lLinkedListNumbers *ll, double index, double value);
void lLinkedListSet(lLinkedListNumbers *ll, double index, double value);
void lLinkedListRemoveNumber(lLinkedListNumbers *ll, double index);
void lFreeLinkedListNumbers(lLinkedListNumbers *ll);
void lFreeLinkedListNumbersArray(lLinkedListNumbers **lls, size_t llsLength);
double *lLinkedListNumbersToArray(size_t *returnArrayLength, lLinkedListNumbers *ll);
lLinkedListNumbers *lArrayToLinkedListNumbers(double *array, size_t arrayLength);
_Bool lLinkedListNumbersEqual(lLinkedListNumbers *a, lLinkedListNumbers *b);

lLinkedListCharacters *lCreateLinkedListCharacter();
void lLinkedListAddCharacter(lLinkedListCharacters *ll, wchar_t value);
wchar_t *lLinkedListCharactersToArray(size_t *returnArrayLength, lLinkedListCharacters *ll);
double lLinkedListCharactersLength(lLinkedListCharacters *ll);
void lFreeLinkedListCharacter(lLinkedListCharacters *ll);



lDynamicArrayNumbers *lCreateDynamicArrayNumbers();
lDynamicArrayNumbers *lCreateDynamicArrayNumbersWithInitialCapacity(double capacity);
void lDynamicArrayAddNumber(lDynamicArrayNumbers *da, double value);
void lDynamicArrayNumbersIncreaseSize(lDynamicArrayNumbers *da);
_Bool lDynamicArrayNumbersDecreaseSizeNecessary(lDynamicArrayNumbers *da);
void lDynamicArrayNumbersDecreaseSize(lDynamicArrayNumbers *da);
double lDynamicArrayNumbersIndex(lDynamicArrayNumbers *da, double index);
double lDynamicArrayNumbersLength(lDynamicArrayNumbers *da);
void lDynamicArrayInsertNumber(lDynamicArrayNumbers *da, double index, double value);
void lDynamicArraySet(lDynamicArrayNumbers *da, double index, double value);
void lDynamicArrayRemoveNumber(lDynamicArrayNumbers *da, double index);
void lFreeDynamicArrayNumbers(lDynamicArrayNumbers *da);
double *lDynamicArrayNumbersToArray(size_t *returnArrayLength, lDynamicArrayNumbers *da);
lDynamicArrayNumbers *lArrayToDynamicArrayNumbersWithOptimalSize(double *array, size_t arrayLength);
lDynamicArrayNumbers *lArrayToDynamicArrayNumbers(double *array, size_t arrayLength);
_Bool lDynamicArrayNumbersEqual(lDynamicArrayNumbers *a, lDynamicArrayNumbers *b);
lLinkedListNumbers *lDynamicArrayNumbersToLinkedList(lDynamicArrayNumbers *da);
lDynamicArrayNumbers *lLinkedListToDynamicArrayNumbers(lLinkedListNumbers *ll);

wchar_t *lAddCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, wchar_t a);
void lAddCharacterRef(StringReference *list, wchar_t i);
wchar_t *lRemoveCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, double n);
wchar_t lGetCharacterRef(StringReference *list, double i);
void lRemoveCharacterRef(StringReference *list, double i);

double Negate(double x);
double Positive(double x);
double Factorial(double x);
double Round(double x);
double BankersRound(double x);
double Ceil(double x);
double Floor(double x);
double Truncate(double x);
double Absolute(double x);
double Logarithm(double x);
double NaturalLogarithm(double x);
double Sin(double x);
double Cos(double x);
double Tan(double x);
double Asin(double x);
double Acos(double x);
double Atan(double x);
double Atan2(double y, double x);
double Squareroot(double x);
double Exp(double x);
_Bool DivisibleBy(double a, double b);
double Combinations(double n, double k);
double Permutations(double n, double k);
_Bool EpsilonCompare(double a, double b, double epsilon);
double GreatestCommonDivisor(double a, double b);
double GCDWithSubtraction(double a, double b);
_Bool IsInteger(double a);
_Bool GreatestCommonDivisorWithCheck(double a, double b, NumberReference *gcdReference);
double LeastCommonMultiple(double a, double b);
double Sign(double a);
double Max(double a, double b);
double Min(double a, double b);
double Power(double a, double b);
double Gamma(double x);
double LogGamma(double x);
double LanczosApproximation(double z);
double Beta(double x, double y);
double Sinh(double x);
double Cosh(double x);
double Tanh(double x);
double Cot(double x);
double Sec(double x);
double Csc(double x);
double Coth(double x);
double Sech(double x);
double Csch(double x);
double Error(double x);
double ErrorInverse(double x);
double FallingFactorial(double x, double n);
double RisingFactorial(double x, double n);
double Hypergeometric(double a, double b, double c, double z, double maxIterations, double precision);
double HypergeometricDirect(double a, double b, double c, double z, double maxIterations, double precision);
double BernouilliNumber(double n);
double AkiyamaTanigawaAlgorithm(double n);

wchar_t charToLowerCase(wchar_t character);
wchar_t charToUpperCase(wchar_t character);
_Bool charIsUpperCase(wchar_t character);
_Bool charIsLowerCase(wchar_t character);
_Bool charIsLetter(wchar_t character);
_Bool charIsNumber(wchar_t character);
_Bool charIsWhiteSpace(wchar_t character);
_Bool charIsSymbol(wchar_t character);
_Bool charCharacterIsBefore(wchar_t a, wchar_t b);
