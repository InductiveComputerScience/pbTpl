// Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

#include <cmath>
#include <cstring>
#include <vector>
#include <cwchar>

using namespace std;

#define toVector(s) (new vector<wchar_t> ((s), (s) + wcslen(s)))

struct LinkedListNodeNodes;

struct LinkedListNodes;

struct Node;

struct BooleanArrayReference;

struct BooleanReference;

struct CharacterReference;

struct NumberArrayReference;

struct NumberReference;

struct StringArrayReference;

struct StringReference;

struct ElementArrayReference;

struct LinkedListElements;

struct LinkedListNodeElements;

struct Element;

struct ElementReference;

struct ElementType;

struct StringElementMap;

struct DynamicArrayCharacters;

struct LinkedListNodeStrings;

struct LinkedListStrings;

struct LinkedListNodeNumbers;

struct LinkedListNumbers;

struct LinkedListCharacters;

struct LinkedListNodeCharacters;

struct DynamicArrayNumbers;

struct lLinkedListNodeStrings;

struct lLinkedListStrings;

struct lLinkedListNodeNumbers;

struct lLinkedListNumbers;

struct lLinkedListCharacters;

struct lLinkedListNodeCharacters;

struct lDynamicArrayNumbers;

struct LinkedListNodeNodes{
  bool end;
  Node *value;
  LinkedListNodeNodes *next;
};

struct LinkedListNodes{
  LinkedListNodeNodes *first;
  LinkedListNodeNodes *last;
};

struct Node{
  vector<wchar_t> *type;
  vector<wchar_t> *p1;
  vector<wchar_t> *p2;
  Node *block1;
  bool hasElseBlock;
  Node *block2;
  LinkedListNodes *nodes;
};

struct BooleanArrayReference{
  vector<bool> *booleanArray;
};

struct BooleanReference{
  bool booleanValue;
};

struct CharacterReference{
  wchar_t characterValue;
};

struct NumberArrayReference{
  vector<double> *numberArray;
};

struct NumberReference{
  double numberValue;
};

struct StringArrayReference{
  vector<StringReference*> *stringArray;
};

struct StringReference{
  vector<wchar_t> *string;
};

struct ElementArrayReference{
  vector<Element*> *array;
};

struct LinkedListElements{
  LinkedListNodeElements *first;
  LinkedListNodeElements *last;
};

struct LinkedListNodeElements{
  bool end;
  Element *value;
  LinkedListNodeElements *next;
};

struct Element{
  vector<wchar_t> *type;
  StringElementMap *object;
  vector<Element*> *array;
  vector<wchar_t> *string;
  double number;
  bool booleanValue;
};

struct ElementReference{
  Element *element;
};

struct ElementType{
  vector<wchar_t> *name;
};

struct StringElementMap{
  StringArrayReference *stringListRef;
  ElementArrayReference *elementListRef;
};

struct DynamicArrayCharacters{
  vector<wchar_t> *array;
  double length;
};

struct LinkedListNodeStrings{
  bool end;
  vector<wchar_t> *value;
  LinkedListNodeStrings *next;
};

struct LinkedListStrings{
  LinkedListNodeStrings *first;
  LinkedListNodeStrings *last;
};

struct LinkedListNodeNumbers{
  LinkedListNodeNumbers *next;
  bool end;
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
  bool end;
  wchar_t value;
  LinkedListNodeCharacters *next;
};

struct DynamicArrayNumbers{
  vector<double> *array;
  double length;
};

struct lLinkedListNodeStrings{
  bool end;
  vector<wchar_t> *value;
  lLinkedListNodeStrings *next;
};

struct lLinkedListStrings{
  lLinkedListNodeStrings *first;
  lLinkedListNodeStrings *last;
};

struct lLinkedListNodeNumbers{
  lLinkedListNodeNumbers *next;
  bool end;
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
  bool end;
  wchar_t value;
  lLinkedListNodeCharacters *next;
};

struct lDynamicArrayNumbers{
  vector<double> *array;
  double length;
};

LinkedListNodes *CreateLinkedListNodes();
void LinkedListAddNode(LinkedListNodes *ll, Node *value);
vector<Node*> *LinkedListNodesToArray(LinkedListNodes *ll);
double LinkedListNodesLength(LinkedListNodes *ll);
void FreeLinkedListNode(LinkedListNodes *ll);

bool IsValidTemplate(vector<wchar_t> *templatex);
bool GenerateTokensFromTemplate(vector<wchar_t> *templatex, LinkedListStrings *tokens, StringReference *errorMessage);
bool GenerateDocument(vector<wchar_t> *templatex, Element *data, StringReference *document, StringReference *errorMessage);
bool GenerateDocumentFromBlock(Node *root, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
bool GenerateDocumentFromNode(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
bool GenerateDocumentFromUse(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
bool GenerateDocumentFromIf(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
bool GenerateDocumentFromForeach(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage);
bool ParseTemplate(vector<StringReference*> *tokens, Node *node, StringReference *errorMessage);
bool ParseTemplateBlock(vector<StringReference*> *tokens, NumberReference *position, Node *node, StringReference *errorMessage);
bool ParseUseBlock(vector<StringReference*> *tokens, NumberReference *position, Node *useBlock, StringReference *errorMessage);
bool ParseIfBlock(vector<StringReference*> *tokens, NumberReference *position, Node *ifBlock, StringReference *errorMessage);
bool ParseForeachBlock(vector<StringReference*> *tokens, NumberReference *position, Node *foreachBlock, StringReference *errorMessage);
bool ParseNodeString(vector<wchar_t> *token, Node *node, StringReference *errorMessage);

double test();
void testTokenGeneration(NumberReference *failures);
void testGenerateDocument1(NumberReference *failures);
void testGenerateDocument2(NumberReference *failures);
void testGenerateDocument3(NumberReference *failures);
void testGenerateDocument4(NumberReference *failures);
void testGenerateDocument5(NumberReference *failures);
void testGenerateDocument6(NumberReference *failures);
void testGenerateDocument7(NumberReference *failures);
void AssertTemplateResult(vector<wchar_t> *templatex, vector<wchar_t> *json, vector<wchar_t> *result, NumberReference *failures);
void AssertTemplateError(vector<wchar_t> *templatex, vector<wchar_t> *json, vector<wchar_t> *errorMessage, NumberReference *failures);

BooleanReference *CreateBooleanReference(bool value);
BooleanArrayReference *CreateBooleanArrayReference(vector<bool> *value);
BooleanArrayReference *CreateBooleanArrayReferenceLengthValue(double length, bool value);
void FreeBooleanArrayReference(BooleanArrayReference *booleanArrayReference);
CharacterReference *CreateCharacterReference(wchar_t value);
NumberReference *CreateNumberReference(double value);
NumberArrayReference *CreateNumberArrayReference(vector<double> *value);
NumberArrayReference *CreateNumberArrayReferenceLengthValue(double length, double value);
void FreeNumberArrayReference(NumberArrayReference *numberArrayReference);
StringReference *CreateStringReference(vector<wchar_t> *value);
StringReference *CreateStringReferenceLengthValue(double length, wchar_t value);
void FreeStringReference(StringReference *stringReference);
StringArrayReference *CreateStringArrayReference(vector<StringReference*> *strings);
StringArrayReference *CreateStringArrayReferenceLengthValue(double length, vector<wchar_t> *value);
void FreeStringArrayReference(StringArrayReference *stringArrayReference);

bool IsValidJSON(vector<wchar_t> *json, StringArrayReference *errorMessage);

bool JSONTokenize(vector<wchar_t> *json, StringArrayReference *tokensReference, StringArrayReference *errorMessages);
bool GetJSONNumberToken(vector<wchar_t> *json, double start, StringReference *tokenReference, StringArrayReference *errorMessages);
bool IsValidJSONNumber(vector<wchar_t> *n, StringArrayReference *errorMessages);
bool IsValidJSONNumberAfterSign(vector<wchar_t> *n, double i, StringArrayReference *errorMessages);
double IsValidJSONNumberAdvancePastDigits(vector<wchar_t> *n, double i);
bool IsValidJSONNumberFromDotOrExponent(vector<wchar_t> *n, double i, StringArrayReference *errorMessages);
bool IsValidJSONNumberFromExponent(vector<wchar_t> *n, double i, StringArrayReference *errorMessages);
bool IsJSONNumberCharacter(wchar_t c);
bool GetJSONPrimitiveName(vector<wchar_t> *string, double start, StringArrayReference *errorMessages, vector<wchar_t> *primitive, StringReference *tokenReference);
bool GetJSONString(vector<wchar_t> *json, double start, StringReference *tokenReference, NumberReference *stringLengthReference, StringArrayReference *errorMessages);
bool IsValidJSONString(vector<wchar_t> *jsonString, StringArrayReference *errorMessages);
bool IsValidJSONStringInJSON(vector<wchar_t> *json, double start, NumberReference *characterCount, NumberReference *stringLengthReference, StringArrayReference *errorMessages);
bool IsJSONIllegalControllCharacter(wchar_t c);

vector<Element*> *AddElement(vector<Element*> *list, Element *a);
void AddElementRef(ElementArrayReference *list, Element *i);
vector<Element*> *RemoveElement(vector<Element*> *list, double n);
Element *GetElementRef(ElementArrayReference *list, double i);
void RemoveElementRef(ElementArrayReference *list, double i);


LinkedListElements *CreateLinkedListElements();
void LinkedListAddElement(LinkedListElements *ll, Element *value);
vector<Element*> *LinkedListElementsToArray(LinkedListElements *ll);
double LinkedListElementsLength(LinkedListElements *ll);
void FreeLinkedListElements(LinkedListElements *ll);


double ComputeJSONStringLength(Element *element);
double ComputeJSONBooleanStringLength(Element *element);
double ComputeJSONNumberStringLength(Element *element);
double ComputeJSONArrayStringLength(Element *element);
double ComputeJSONObjectStringLength(Element *element);

Element *CreateStringElement(vector<wchar_t> *string);
Element *CreateBooleanElement(bool booleanValue);
Element *CreateNullElement();
Element *CreateNumberElement(double number);
Element *CreateArrayElement(double length);
Element *CreateObjectElement(double length);
void DeleteElement(Element *element);
void DeleteObject(Element *element);
void DeleteArray(Element *element);

vector<wchar_t> *WriteJSON(Element *element);
void WriteBooleanValue(Element *element, vector<wchar_t> *result, NumberReference *index);
void WriteNumber(Element *element, vector<wchar_t> *result, NumberReference *index);
void WriteArray(Element *element, vector<wchar_t> *result, NumberReference *index);
void WriteString(Element *element, vector<wchar_t> *result, NumberReference *index);
vector<wchar_t> *JSONEscapeString(vector<wchar_t> *string);
double JSONEscapedStringLength(vector<wchar_t> *string);
vector<wchar_t> *JSONEscapeCharacter(wchar_t c);
bool JSONMustBeEscaped(wchar_t c, NumberReference *letters);
void WriteObject(Element *element, vector<wchar_t> *result, NumberReference *index);

bool ReadJSON(vector<wchar_t> *string, ElementReference *elementReference, StringArrayReference *errorMessages);
bool GetJSONValue(vector<StringReference*> *tokens, ElementReference *elementReference, StringArrayReference *errorMessages);
bool GetJSONValueRecursive(vector<StringReference*> *tokens, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);
bool GetJSONObject(vector<StringReference*> *tokens, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);
bool GetJSONArray(vector<StringReference*> *tokens, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);

StringArrayReference *GetStringElementMapKeySet(StringElementMap *stringElementMap);
Element *GetObjectValue(StringElementMap *stringElementMap, vector<wchar_t> *key);
Element *GetObjectValueWithCheck(StringElementMap *stringElementMap, vector<wchar_t> *key, BooleanReference *foundReference);
void PutStringElementMap(StringElementMap *stringElementMap, vector<wchar_t> *keystring, Element *value);
void SetStringElementMap(StringElementMap *stringElementMap, double index, vector<wchar_t> *keystring, Element *value);
double GetStringElementMapNumberOfKeys(StringElementMap *stringElementMap);

bool JSONCompare(vector<wchar_t> *a, vector<wchar_t> *b, double epsilon, BooleanReference *equal, StringArrayReference *errorMessage);
bool JSONCompareElements(Element *ea, Element *eb, double epsilon);
bool JSONCompareArrays(vector<Element*> *ea, vector<Element*> *eb, double epsilon);
bool JSONCompareObjects(StringElementMap *ea, StringElementMap *eb, double epsilon);

vector<double> *AddNumber(vector<double> *list, double a);
void AddNumberRef(NumberArrayReference *list, double i);
vector<double> *RemoveNumber(vector<double> *list, double n);
double GetNumberRef(NumberArrayReference *list, double i);
void RemoveNumberRef(NumberArrayReference *list, double i);

vector<StringReference*> *AddString(vector<StringReference*> *list, StringReference *a);
void AddStringRef(StringArrayReference *list, StringReference *i);
vector<StringReference*> *RemoveString(vector<StringReference*> *list, double n);
StringReference *GetStringRef(StringArrayReference *list, double i);
void RemoveStringRef(StringArrayReference *list, double i);


DynamicArrayCharacters *CreateDynamicArrayCharacters();
DynamicArrayCharacters *CreateDynamicArrayCharactersWithInitialCapacity(double capacity);
void DynamicArrayAddCharacter(DynamicArrayCharacters *da, wchar_t value);
void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters *da);
bool DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters *da);
void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters *da);
double DynamicArrayCharactersIndex(DynamicArrayCharacters *da, double index);
double DynamicArrayCharactersLength(DynamicArrayCharacters *da);
void DynamicArrayInsertCharacter(DynamicArrayCharacters *da, double index, wchar_t value);
bool DynamicArrayCharacterSet(DynamicArrayCharacters *da, double index, wchar_t value);
void DynamicArrayRemoveCharacter(DynamicArrayCharacters *da, double index);
void FreeDynamicArrayCharacters(DynamicArrayCharacters *da);
vector<wchar_t> *DynamicArrayCharactersToArray(DynamicArrayCharacters *da);
DynamicArrayCharacters *ArrayToDynamicArrayCharactersWithOptimalSize(vector<wchar_t> *array);
DynamicArrayCharacters *ArrayToDynamicArrayCharacters(vector<wchar_t> *array);
bool DynamicArrayCharactersEqual(DynamicArrayCharacters *a, DynamicArrayCharacters *b);
LinkedListCharacters *DynamicArrayCharactersToLinkedList(DynamicArrayCharacters *da);
DynamicArrayCharacters *LinkedListToDynamicArrayCharacters(LinkedListCharacters *ll);

vector<bool> *AddBoolean(vector<bool> *list, bool a);
void AddBooleanRef(BooleanArrayReference *list, bool i);
vector<bool> *RemoveBoolean(vector<bool> *list, double n);
bool GetBooleanRef(BooleanArrayReference *list, double i);
void RemoveDecimalRef(BooleanArrayReference *list, double i);


LinkedListStrings *CreateLinkedListString();
void LinkedListAddString(LinkedListStrings *ll, vector<wchar_t> *value);
vector<StringReference*> *LinkedListStringsToArray(LinkedListStrings *ll);
double LinkedListStringsLength(LinkedListStrings *ll);
void FreeLinkedListString(LinkedListStrings *ll);


LinkedListNumbers *CreateLinkedListNumbers();
vector<LinkedListNumbers*> *CreateLinkedListNumbersArray(double length);
void LinkedListAddNumber(LinkedListNumbers *ll, double value);
double LinkedListNumbersLength(LinkedListNumbers *ll);
double LinkedListNumbersIndex(LinkedListNumbers *ll, double index);
void LinkedListInsertNumber(LinkedListNumbers *ll, double index, double value);
void LinkedListSet(LinkedListNumbers *ll, double index, double value);
void LinkedListRemoveNumber(LinkedListNumbers *ll, double index);
void FreeLinkedListNumbers(LinkedListNumbers *ll);
void FreeLinkedListNumbersArray(vector<LinkedListNumbers*> *lls);
vector<double> *LinkedListNumbersToArray(LinkedListNumbers *ll);
LinkedListNumbers *ArrayToLinkedListNumbers(vector<double> *array);
bool LinkedListNumbersEqual(LinkedListNumbers *a, LinkedListNumbers *b);

LinkedListCharacters *CreateLinkedListCharacter();
void LinkedListAddCharacter(LinkedListCharacters *ll, wchar_t value);
vector<wchar_t> *LinkedListCharactersToArray(LinkedListCharacters *ll);
double LinkedListCharactersLength(LinkedListCharacters *ll);
void FreeLinkedListCharacter(LinkedListCharacters *ll);
void LinkedListCharactersAddString(LinkedListCharacters *ll, vector<wchar_t> *str);



DynamicArrayNumbers *CreateDynamicArrayNumbers();
DynamicArrayNumbers *CreateDynamicArrayNumbersWithInitialCapacity(double capacity);
void DynamicArrayAddNumber(DynamicArrayNumbers *da, double value);
void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers *da);
bool DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers *da);
void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers *da);
double DynamicArrayNumbersIndex(DynamicArrayNumbers *da, double index);
double DynamicArrayNumbersLength(DynamicArrayNumbers *da);
void DynamicArrayInsertNumber(DynamicArrayNumbers *da, double index, double value);
bool DynamicArrayNumberSet(DynamicArrayNumbers *da, double index, double value);
void DynamicArrayRemoveNumber(DynamicArrayNumbers *da, double index);
void FreeDynamicArrayNumbers(DynamicArrayNumbers *da);
vector<double> *DynamicArrayNumbersToArray(DynamicArrayNumbers *da);
DynamicArrayNumbers *ArrayToDynamicArrayNumbersWithOptimalSize(vector<double> *array);
DynamicArrayNumbers *ArrayToDynamicArrayNumbers(vector<double> *array);
bool DynamicArrayNumbersEqual(DynamicArrayNumbers *a, DynamicArrayNumbers *b);
LinkedListNumbers *DynamicArrayNumbersToLinkedList(DynamicArrayNumbers *da);
DynamicArrayNumbers *LinkedListToDynamicArrayNumbers(LinkedListNumbers *ll);

vector<wchar_t> *AddCharacter(vector<wchar_t> *list, wchar_t a);
void AddCharacterRef(StringReference *list, wchar_t i);
vector<wchar_t> *RemoveCharacter(vector<wchar_t> *list, double n);
wchar_t GetCharacterRef(StringReference *list, double i);
void RemoveCharacterRef(StringReference *list, double i);

void sWriteStringToStingStream(vector<wchar_t> *stream, NumberReference *index, vector<wchar_t> *src);
void sWriteCharacterToStingStream(vector<wchar_t> *stream, NumberReference *index, wchar_t src);
void sWriteBooleanToStingStream(vector<wchar_t> *stream, NumberReference *index, bool src);

bool sSubstringWithCheck(vector<wchar_t> *string, double from, double to, StringReference *stringReference);
vector<wchar_t> *sSubstring(vector<wchar_t> *string, double from, double to);
vector<wchar_t> *sAppendString(vector<wchar_t> *s1, vector<wchar_t> *s2);
vector<wchar_t> *sConcatenateString(vector<wchar_t> *s1, vector<wchar_t> *s2);
vector<wchar_t> *sAppendCharacter(vector<wchar_t> *string, wchar_t c);
vector<wchar_t> *sConcatenateCharacter(vector<wchar_t> *string, wchar_t c);
vector<StringReference*> *sSplitByCharacter(vector<wchar_t> *toSplit, wchar_t splitBy);
bool sIndexOfCharacter(vector<wchar_t> *string, wchar_t character, NumberReference *indexReference);
bool sSubstringEqualsWithCheck(vector<wchar_t> *string, double from, vector<wchar_t> *substring, BooleanReference *equalsReference);
bool sSubstringEquals(vector<wchar_t> *string, double from, vector<wchar_t> *substring);
bool sIndexOfString(vector<wchar_t> *string, vector<wchar_t> *substring, NumberReference *indexReference);
bool sContainsCharacter(vector<wchar_t> *string, wchar_t character);
bool sContainsString(vector<wchar_t> *string, vector<wchar_t> *substring);
void sToUpperCase(vector<wchar_t> *string);
void sToLowerCase(vector<wchar_t> *string);
bool sEqualsIgnoreCase(vector<wchar_t> *a, vector<wchar_t> *b);
vector<wchar_t> *sReplaceString(vector<wchar_t> *string, vector<wchar_t> *toReplace, vector<wchar_t> *replaceWith);
vector<wchar_t> *sReplaceCharacterToNew(vector<wchar_t> *string, wchar_t toReplace, wchar_t replaceWith);
void sReplaceCharacter(vector<wchar_t> *string, wchar_t toReplace, wchar_t replaceWith);
vector<wchar_t> *sTrim(vector<wchar_t> *string);
bool sStartsWith(vector<wchar_t> *string, vector<wchar_t> *start);
bool sEndsWith(vector<wchar_t> *string, vector<wchar_t> *end);
vector<StringReference*> *sSplitByString(vector<wchar_t> *toSplit, vector<wchar_t> *splitBy);
bool sStringIsBefore(vector<wchar_t> *a, vector<wchar_t> *b);

void strWriteStringToStingStream(vector<wchar_t> *stream, NumberReference *index, vector<wchar_t> *src);
void strWriteCharacterToStingStream(vector<wchar_t> *stream, NumberReference *index, wchar_t src);
void strWriteBooleanToStingStream(vector<wchar_t> *stream, NumberReference *index, bool src);

bool strSubstringWithCheck(vector<wchar_t> *string, double from, double to, StringReference *stringReference);
vector<wchar_t> *strSubstring(vector<wchar_t> *string, double from, double to);
vector<wchar_t> *strAppendString(vector<wchar_t> *s1, vector<wchar_t> *s2);
vector<wchar_t> *strConcatenateString(vector<wchar_t> *s1, vector<wchar_t> *s2);
vector<wchar_t> *strAppendCharacter(vector<wchar_t> *string, wchar_t c);
vector<wchar_t> *strConcatenateCharacter(vector<wchar_t> *string, wchar_t c);
vector<StringReference*> *strSplitByCharacter(vector<wchar_t> *toSplit, wchar_t splitBy);
bool strIndexOfCharacter(vector<wchar_t> *string, wchar_t character, NumberReference *indexReference);
bool strSubstringEqualsWithCheck(vector<wchar_t> *string, double from, vector<wchar_t> *substring, BooleanReference *equalsReference);
bool strSubstringEquals(vector<wchar_t> *string, double from, vector<wchar_t> *substring);
bool strIndexOfString(vector<wchar_t> *string, vector<wchar_t> *substring, NumberReference *indexReference);
bool strContainsCharacter(vector<wchar_t> *string, wchar_t character);
bool strContainsString(vector<wchar_t> *string, vector<wchar_t> *substring);
void strToUpperCase(vector<wchar_t> *string);
void strToLowerCase(vector<wchar_t> *string);
bool strEqualsIgnoreCase(vector<wchar_t> *a, vector<wchar_t> *b);
vector<wchar_t> *strReplaceString(vector<wchar_t> *string, vector<wchar_t> *toReplace, vector<wchar_t> *replaceWith);
vector<wchar_t> *strReplaceCharacter(vector<wchar_t> *string, wchar_t toReplace, wchar_t replaceWith);
vector<wchar_t> *strTrim(vector<wchar_t> *string);
bool strStartsWith(vector<wchar_t> *string, vector<wchar_t> *start);
bool strEndsWith(vector<wchar_t> *string, vector<wchar_t> *end);
vector<StringReference*> *strSplitByString(vector<wchar_t> *toSplit, vector<wchar_t> *splitBy);
bool strStringIsBefore(vector<wchar_t> *a, vector<wchar_t> *b);

vector<double> *StringToNumberArray(vector<wchar_t> *string);
vector<wchar_t> *NumberArrayToString(vector<double> *array);
bool NumberArraysEqual(vector<double> *a, vector<double> *b);
bool BooleanArraysEqual(vector<bool> *a, vector<bool> *b);
bool StringsEqual(vector<wchar_t> *a, vector<wchar_t> *b);
void FillNumberArray(vector<double> *a, double value);
void FillString(vector<wchar_t> *a, wchar_t value);
void FillBooleanArray(vector<bool> *a, bool value);
bool FillNumberArrayRange(vector<double> *a, double value, double from, double to);
bool FillBooleanArrayRange(vector<bool> *a, bool value, double from, double to);
bool FillStringRange(vector<wchar_t> *a, wchar_t value, double from, double to);
vector<double> *CopyNumberArray(vector<double> *a);
vector<bool> *CopyBooleanArray(vector<bool> *a);
vector<wchar_t> *CopyString(vector<wchar_t> *a);
bool CopyNumberArrayRange(vector<double> *a, double from, double to, NumberArrayReference *copyReference);
bool CopyBooleanArrayRange(vector<bool> *a, double from, double to, BooleanArrayReference *copyReference);
bool CopyStringRange(vector<wchar_t> *a, double from, double to, StringReference *copyReference);
bool IsLastElement(double length, double index);
vector<double> *CreateNumberArray(double length, double value);
vector<bool> *CreateBooleanArray(double length, bool value);
vector<wchar_t> *CreateString(double length, wchar_t value);
void SwapElementsOfNumberArray(vector<double> *A, double ai, double bi);
void SwapElementsOfStringArray(StringArrayReference *A, double ai, double bi);
void ReverseNumberArray(vector<double> *array);

void AssertFalse(bool b, NumberReference *failures);
void AssertTrue(bool b, NumberReference *failures);
void AssertEquals(double a, double b, NumberReference *failures);
void AssertBooleansEqual(bool a, bool b, NumberReference *failures);
void AssertCharactersEqual(wchar_t a, wchar_t b, NumberReference *failures);
void AssertStringEquals(vector<wchar_t> *a, vector<wchar_t> *b, NumberReference *failures);
void AssertNumberArraysEqual(vector<double> *a, vector<double> *b, NumberReference *failures);
void AssertBooleanArraysEqual(vector<bool> *a, vector<bool> *b, NumberReference *failures);
void AssertStringArraysEqual(vector<StringReference*> *a, vector<StringReference*> *b, NumberReference *failures);

vector<wchar_t> *nCreateStringScientificNotationDecimalFromNumber(double decimal);
vector<wchar_t> *nCreateStringDecimalFromNumber(double decimal);
bool nCreateStringFromNumberWithCheck(double decimal, double base, StringReference *stringReference);
double nGetMaximumDigitsForBase(double base);
double nGetFirstDigitPosition(double decimal, double base);
bool nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference *characterReference);
vector<wchar_t> *nGetDigitCharacterTable();

bool nCreateNumberFromDecimalStringWithCheck(vector<wchar_t> *string, NumberReference *decimalReference, StringReference *errorMessage);
double nCreateNumberFromDecimalString(vector<wchar_t> *string);
bool nCreateNumberFromStringWithCheck(vector<wchar_t> *string, double base, NumberReference *numberReference, StringReference *errorMessage);
double nCreateNumberFromParts(double base, bool numberIsPositive, vector<double> *beforePoint, vector<double> *afterPoint, bool exponentIsPositive, vector<double> *exponent);
bool nExtractPartsFromNumberString(vector<wchar_t> *n, double base, BooleanReference *numberIsPositive, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
bool nExtractPartsFromNumberStringFromSign(vector<wchar_t> *n, double base, double i, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
bool nExtractPartsFromNumberStringFromPointOrExponent(vector<wchar_t> *n, double base, double i, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
bool nExtractPartsFromNumberStringFromExponent(vector<wchar_t> *n, double base, double i, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
double nGetNumberFromNumberCharacterForBase(wchar_t c, double base);
bool nCharacterIsNumberCharacterInBase(wchar_t c, double base);
vector<double> *nStringToNumberArray(vector<wchar_t> *str);
bool nStringToNumberArrayWithCheck(vector<wchar_t> *str, NumberArrayReference *numberArrayReference, StringReference *errorMessage);

vector<double> *lAddNumber(vector<double> *list, double a);
void lAddNumberRef(NumberArrayReference *list, double i);
vector<double> *lRemoveNumber(vector<double> *list, double n);
double lGetNumberRef(NumberArrayReference *list, double i);
void lRemoveNumberRef(NumberArrayReference *list, double i);

vector<StringReference*> *lAddString(vector<StringReference*> *list, StringReference *a);
void lAddStringRef(StringArrayReference *list, StringReference *i);
vector<StringReference*> *lRemoveString(vector<StringReference*> *list, double n);
StringReference *lGetStringRef(StringArrayReference *list, double i);
void lRemoveStringRef(StringArrayReference *list, double i);

vector<bool> *lAddBoolean(vector<bool> *list, bool a);
void lAddBooleanRef(BooleanArrayReference *list, bool i);
vector<bool> *lRemoveBoolean(vector<bool> *list, double n);
bool lGetBooleanRef(BooleanArrayReference *list, double i);
void lRemoveDecimalRef(BooleanArrayReference *list, double i);


lLinkedListStrings *lCreateLinkedListString();
void lLinkedListAddString(lLinkedListStrings *ll, vector<wchar_t> *value);
vector<StringReference*> *lLinkedListStringsToArray(lLinkedListStrings *ll);
double lLinkedListStringsLength(lLinkedListStrings *ll);
void lFreeLinkedListString(lLinkedListStrings *ll);


lLinkedListNumbers *lCreateLinkedListNumbers();
vector<lLinkedListNumbers*> *lCreateLinkedListNumbersArray(double length);
void lLinkedListAddNumber(lLinkedListNumbers *ll, double value);
double lLinkedListNumbersLength(lLinkedListNumbers *ll);
double lLinkedListNumbersIndex(lLinkedListNumbers *ll, double index);
void lLinkedListInsertNumber(lLinkedListNumbers *ll, double index, double value);
void lLinkedListSet(lLinkedListNumbers *ll, double index, double value);
void lLinkedListRemoveNumber(lLinkedListNumbers *ll, double index);
void lFreeLinkedListNumbers(lLinkedListNumbers *ll);
void lFreeLinkedListNumbersArray(vector<lLinkedListNumbers*> *lls);
vector<double> *lLinkedListNumbersToArray(lLinkedListNumbers *ll);
lLinkedListNumbers *lArrayToLinkedListNumbers(vector<double> *array);
bool lLinkedListNumbersEqual(lLinkedListNumbers *a, lLinkedListNumbers *b);

lLinkedListCharacters *lCreateLinkedListCharacter();
void lLinkedListAddCharacter(lLinkedListCharacters *ll, wchar_t value);
vector<wchar_t> *lLinkedListCharactersToArray(lLinkedListCharacters *ll);
double lLinkedListCharactersLength(lLinkedListCharacters *ll);
void lFreeLinkedListCharacter(lLinkedListCharacters *ll);



lDynamicArrayNumbers *lCreateDynamicArrayNumbers();
lDynamicArrayNumbers *lCreateDynamicArrayNumbersWithInitialCapacity(double capacity);
void lDynamicArrayAddNumber(lDynamicArrayNumbers *da, double value);
void lDynamicArrayNumbersIncreaseSize(lDynamicArrayNumbers *da);
bool lDynamicArrayNumbersDecreaseSizeNecessary(lDynamicArrayNumbers *da);
void lDynamicArrayNumbersDecreaseSize(lDynamicArrayNumbers *da);
double lDynamicArrayNumbersIndex(lDynamicArrayNumbers *da, double index);
double lDynamicArrayNumbersLength(lDynamicArrayNumbers *da);
void lDynamicArrayInsertNumber(lDynamicArrayNumbers *da, double index, double value);
void lDynamicArraySet(lDynamicArrayNumbers *da, double index, double value);
void lDynamicArrayRemoveNumber(lDynamicArrayNumbers *da, double index);
void lFreeDynamicArrayNumbers(lDynamicArrayNumbers *da);
vector<double> *lDynamicArrayNumbersToArray(lDynamicArrayNumbers *da);
lDynamicArrayNumbers *lArrayToDynamicArrayNumbersWithOptimalSize(vector<double> *array);
lDynamicArrayNumbers *lArrayToDynamicArrayNumbers(vector<double> *array);
bool lDynamicArrayNumbersEqual(lDynamicArrayNumbers *a, lDynamicArrayNumbers *b);
lLinkedListNumbers *lDynamicArrayNumbersToLinkedList(lDynamicArrayNumbers *da);
lDynamicArrayNumbers *lLinkedListToDynamicArrayNumbers(lLinkedListNumbers *ll);

vector<wchar_t> *lAddCharacter(vector<wchar_t> *list, wchar_t a);
void lAddCharacterRef(StringReference *list, wchar_t i);
vector<wchar_t> *lRemoveCharacter(vector<wchar_t> *list, double n);
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
bool DivisibleBy(double a, double b);
double Combinations(double n, double k);
double Permutations(double n, double k);
bool EpsilonCompare(double a, double b, double epsilon);
double GreatestCommonDivisor(double a, double b);
double GCDWithSubtraction(double a, double b);
bool IsInteger(double a);
bool GreatestCommonDivisorWithCheck(double a, double b, NumberReference *gcdReference);
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
bool charIsUpperCase(wchar_t character);
bool charIsLowerCase(wchar_t character);
bool charIsLetter(wchar_t character);
bool charIsNumber(wchar_t character);
bool charIsWhiteSpace(wchar_t character);
bool charIsSymbol(wchar_t character);
bool charCharacterIsBefore(wchar_t a, wchar_t b);

