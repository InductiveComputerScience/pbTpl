/* Downloaded from https://repo.progsbase.com - Code Developed Using progsbase. */

#include "TextualTemplates.h"

LinkedListNodes *CreateLinkedListNodes(){
  LinkedListNodes *ll;

  ll = (LinkedListNodes *)malloc(sizeof(LinkedListNodes));
  ll->first = (LinkedListNodeNodes *)malloc(sizeof(LinkedListNodeNodes));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddNode(LinkedListNodes *ll, Node *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeNodes *)malloc(sizeof(LinkedListNodeNodes));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
Node **LinkedListNodesToArray(size_t *returnArrayLength, LinkedListNodes *ll){
  Node **array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeNodes *node;

  node = ll->first;

  length = LinkedListNodesLength(ll);

  array = (Node**)malloc(sizeof(Node) * length);
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListNodesLength(LinkedListNodes *ll){
  double l;
  LinkedListNodeNodes *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListNode(LinkedListNodes *ll){
  LinkedListNodeNodes *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
_Bool IsValidTemplate(wchar_t *template, size_t templateLength){
  StringReference *errorMessage;
  _Bool success;
  LinkedListStrings *tokens;
  StringReference **ts;
  size_t tsLength;
  Node *root;

  tokens = CreateLinkedListString();
  errorMessage = (StringReference *)malloc(sizeof(StringReference));

  success = GenerateTokensFromTemplate(template, templateLength, tokens, errorMessage);

  if(success){
    root = (Node *)malloc(sizeof(Node));

    ts = LinkedListStringsToArray(&tsLength, tokens);

    success = ParseTemplate(ts, tsLength, root, errorMessage);
  }

  if( !success ){
    FreeStringReference(errorMessage);
  }

  return success;
}
_Bool GenerateTokensFromTemplate(wchar_t *template, size_t templateLength, LinkedListStrings *tokens, StringReference *errorMessage){
  _Bool success, found;
  double i, j, l;
  wchar_t pc, c;
  wchar_t *cs, *a;
  size_t csLength, aLength;
  DynamicArrayCharacters *da;

  success = true;
  cs = (wchar_t*)malloc(sizeof(wchar_t) * (4.0));
  csLength = 4.0;

  da = CreateDynamicArrayCharacters();

  pc = 'x';
  for(i = 0.0; i < templateLength && success; ){
    c = template[(int)(i)];

    if(c != '{'){
      DynamicArrayAddCharacter(da, c);
      i = i + 1.0;
    }else{
      FillString(cs, csLength, 'x');
      for(j = 0.0; i + j < templateLength && j < csLength; j = j + 1.0){
        cs[(int)(j)] = template[(int)(i + j)];
      }

      if(StringsEqual(cs, csLength, strparam(L"{use")) || StringsEqual(cs, csLength, strparam(L"{end")) || StringsEqual(cs, csLength, strparam(L"{pri")) || StringsEqual(cs, csLength, strparam(L"{for")) || StringsEqual(cs, csLength, strparam(L"{if ")) || StringsEqual(cs, csLength, strparam(L"{els"))){
        if(pc != '\\'){
          /* Find end. */
          found = false;
          for(l = 0.0; i + l < templateLength &&  !found ; l = l + 1.0){
            if(template[(int)(i + l)] == '}'){
              found = true;
            }
          }

          if(found){
            if(da->length > 0.0){
              a = DynamicArrayCharactersToArray(&aLength, da);
              LinkedListAddString(tokens, a, aLength);
              FreeDynamicArrayCharacters(da);
              da = CreateDynamicArrayCharacters();
            }

            for(j = 0.0; j < l; j = j + 1.0){
              DynamicArrayAddCharacter(da, template[(int)(i + j)]);
            }

            a = DynamicArrayCharactersToArray(&aLength, da);
            LinkedListAddString(tokens, a, aLength);
            FreeDynamicArrayCharacters(da);
            da = CreateDynamicArrayCharacters();

            i = i + l;
          }else{
            success = false;
            errorMessage->string = L"Template command found, but not ended properly.";
            errorMessage->stringLength = wcslen(errorMessage->string);
          }
        }else{
          DynamicArrayAddCharacter(da, c);
          i = i + 1.0;
        }
      }else{
        DynamicArrayAddCharacter(da, c);
        i = i + 1.0;
      }
    }

    pc = c;
  }

  if(da->length > 0.0){
    a = DynamicArrayCharactersToArray(&aLength, da);
    LinkedListAddString(tokens, a, aLength);
  }

  FreeDynamicArrayCharacters(da);

  return success;
}
_Bool GenerateDocument(wchar_t *template, size_t templateLength, Element *data, StringReference *document, StringReference *errorMessage){
  LinkedListCharacters *ll;
  _Bool success;
  LinkedListStrings *tokens;
  StringReference **ts;
  size_t tsLength;
  Node *root;

  tokens = CreateLinkedListString();

  success = GenerateTokensFromTemplate(template, templateLength, tokens, errorMessage);

  if(success){
    root = (Node *)malloc(sizeof(Node));

    ts = LinkedListStringsToArray(&tsLength, tokens);
    FreeLinkedListString(tokens);

    success = ParseTemplate(ts, tsLength, root, errorMessage);

    if(success){
      ll = CreateLinkedListCharacter();

      success = GenerateDocumentFromBlock(root, data, ll, errorMessage);

      if(success){
        document->string = LinkedListCharactersToArray(&document->stringLength, ll);
        FreeLinkedListCharacter(ll);
      }
    }
  }

  return success;
}
_Bool GenerateDocumentFromBlock(Node *root, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  LinkedListNodeNodes *n;
  _Bool success;

  n = root->nodes->first;
  success = true;

  for(;  !n->end  && success; ){
    success = GenerateDocumentFromNode(n->value, data, ll, errorMessage);
    n = n->next;
  }

  return success;
}
_Bool GenerateDocumentFromNode(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  _Bool success;
  BooleanReference *found;
  wchar_t *valueString;
  size_t valueStringLength;
  Element *value;

  success = true;
  found = (BooleanReference *)malloc(sizeof(BooleanReference));

  if(StringsEqual(n->type, n->typeLength, strparam(L"block"))){
    success = GenerateDocumentFromBlock(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, n->typeLength, strparam(L"use"))){
    success = GenerateDocumentFromUse(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, n->typeLength, strparam(L"if"))){
    success = GenerateDocumentFromIf(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, n->typeLength, strparam(L"foreach"))){
    success = GenerateDocumentFromForeach(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, n->typeLength, strparam(L"text"))){
    success = true;
    LinkedListCharactersAddString(ll, n->p1, n->p1Length);
  }else if(StringsEqual(n->type, n->typeLength, strparam(L"print"))){
    if(StringsEqual(data->type, data->typeLength, strparam(L"object"))){
      value = GetObjectValueWithCheck(data->object, n->p1, n->p1Length, found);

      if(found->booleanValue){
        if(StringsEqual(value->type, value->typeLength, strparam(L"string"))){
          valueString = value->string;
          valueStringLength = value->stringLength;
        }else{
          valueString = WriteJSON(&valueStringLength, value);
        }
        LinkedListCharactersAddString(ll, valueString, valueStringLength);
      }else{
        success = false;
        errorMessage->string = L"Key for printing not found in JSON object: ";
        errorMessage->stringLength = wcslen(errorMessage->string);
        errorMessage->string = sConcatenateString(&errorMessage->stringLength, errorMessage->string, errorMessage->stringLength, n->p1, n->p1Length);
      }
    }else{
      success = false;
      errorMessage->string = L"Data structure for print command is not a JSON object.";
      errorMessage->stringLength = wcslen(errorMessage->string);
    }
  }

  return success;
}
_Bool GenerateDocumentFromUse(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  _Bool success;
  BooleanReference *found;
  Element *value;

  found = (BooleanReference *)malloc(sizeof(BooleanReference));

  if(StringsEqual(data->type, data->typeLength, strparam(L"object"))){
    value = GetObjectValueWithCheck(data->object, n->p1, n->p1Length, found);

    if(found->booleanValue){
      success = GenerateDocumentFromNode(n->block1, value, ll, errorMessage);
    }else{
      success = false;
      errorMessage->string = L"Key for use not found in JSON object.";
      errorMessage->stringLength = wcslen(errorMessage->string);
    }
  }else{
    success = false;
    errorMessage->string = L"Data structure for use command is not a JSON object.";
    errorMessage->stringLength = wcslen(errorMessage->string);
  }

  return success;
}
_Bool GenerateDocumentFromIf(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  _Bool success;
  BooleanReference *found;
  Element *value;

  success = true;
  found = (BooleanReference *)malloc(sizeof(BooleanReference));

  if(StringsEqual(data->type, data->typeLength, strparam(L"object"))){
    value = GetObjectValueWithCheck(data->object, n->p1, n->p1Length, found);

    if(found->booleanValue){
      if(StringsEqual(value->type, value->typeLength, strparam(L"boolean"))){
        if(value->booleanValue){
          success = GenerateDocumentFromBlock(n->block1, data, ll, errorMessage);
        }

        if(n->hasElseBlock){
          if( !value->booleanValue ){
            success = GenerateDocumentFromBlock(n->block2, data, ll, errorMessage);
          }
        }
      }else{
        success = false;
        errorMessage->string = L"Value for if not boolean.";
        errorMessage->stringLength = wcslen(errorMessage->string);
      }
    }else{
      success = false;
      errorMessage->string = L"Key for if not found in JSON object: ";
      errorMessage->stringLength = wcslen(errorMessage->string);
      errorMessage->string = sConcatenateString(&errorMessage->stringLength, errorMessage->string, errorMessage->stringLength, n->p1, n->p1Length);
    }
  }else{
    success = false;
    errorMessage->string = L"Data structure for if command is not a JSON object.";
    errorMessage->stringLength = wcslen(errorMessage->string);
  }

  return success;
}
_Bool GenerateDocumentFromForeach(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  _Bool success;
  BooleanReference *found;
  Element *value, *loopVar, *arrayElement;
  double i;

  success = true;
  found = (BooleanReference *)malloc(sizeof(BooleanReference));
  loopVar = CreateObjectElement(0.0);

  PutStringElementMap(loopVar->object, n->p1, n->p1Length, (Element *)malloc(sizeof(Element)));

  if(StringsEqual(data->type, data->typeLength, strparam(L"object"))){
    value = GetObjectValueWithCheck(data->object, n->p2, n->p2Length, found);

    if(found->booleanValue){
      if(StringsEqual(value->type, value->typeLength, strparam(L"array"))){

        for(i = 0.0; i < value->arrayLength; i = i + 1.0){
          arrayElement = value->array[(int)(i)];
          if(StringsEqual(arrayElement->type, arrayElement->typeLength, strparam(L"object"))){
            success = GenerateDocumentFromBlock(n->block1, arrayElement, ll, errorMessage);
          }else{
            SetStringElementMap(loopVar->object, 0.0, n->p1, n->p1Length, arrayElement);
            success = GenerateDocumentFromBlock(n->block1, loopVar, ll, errorMessage);
          }
        }
      }else{
        success = false;
        errorMessage->string = L"Value for foreach is not an array.";
        errorMessage->stringLength = wcslen(errorMessage->string);
      }
    }else{
      success = false;
      errorMessage->string = L"Key for foreach not found in JSON object: ";
      errorMessage->stringLength = wcslen(errorMessage->string);
      errorMessage->string = sConcatenateString(&errorMessage->stringLength, errorMessage->string, errorMessage->stringLength, n->p2, n->p2Length);
    }
  }else{
    success = false;
    errorMessage->string = L"Data structure for foreach command is not a JSON object.";
    errorMessage->stringLength = wcslen(errorMessage->string);
  }

  return success;
}
_Bool ParseTemplate(StringReference **tokens, size_t tokensLength, Node *node, StringReference *errorMessage){
  _Bool success;
  NumberReference *position;

  position = CreateNumberReference(0.0);
  success = ParseTemplateBlock(tokens, tokensLength, position, node, errorMessage);

  if(success){
    if(position->numberValue != tokensLength){
      success = false;
      errorMessage->string = L"Unexpected token at the end of template.";
      errorMessage->stringLength = wcslen(errorMessage->string);
    }
  }

  free(position);

  return success;
}
_Bool ParseTemplateBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *node, StringReference *errorMessage){
  _Bool success, done;
  Node *tn, *nb;

  success = true;
  done = false;

  node->type = L"block";
  node->typeLength = wcslen(node->type);
  node->nodes = CreateLinkedListNodes();

  for(; position->numberValue < tokensLength && success &&  !done ; ){
    tn = (Node *)malloc(sizeof(Node));
    success = ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, tn, errorMessage);

    if(success){
      if(StringsEqual(tn->type, tn->typeLength, strparam(L"text")) || StringsEqual(tn->type, tn->typeLength, strparam(L"print"))){
        LinkedListAddNode(node->nodes, tn);
        position->numberValue = position->numberValue + 1.0;
      }else if(StringsEqual(tn->type, tn->typeLength, strparam(L"use"))){
        nb = (Node *)malloc(sizeof(Node));
        success = ParseUseBlock(tokens, tokensLength, position, nb, errorMessage);
        LinkedListAddNode(node->nodes, nb);
      }else if(StringsEqual(tn->type, tn->typeLength, strparam(L"if"))){
        nb = (Node *)malloc(sizeof(Node));
        success = ParseIfBlock(tokens, tokensLength, position, nb, errorMessage);
        LinkedListAddNode(node->nodes, nb);
      }else if(StringsEqual(tn->type, tn->typeLength, strparam(L"foreach"))){
        nb = (Node *)malloc(sizeof(Node));
        success = ParseForeachBlock(tokens, tokensLength, position, nb, errorMessage);
        LinkedListAddNode(node->nodes, nb);
      }else{
        done = true;
      }
    }
  }

  return success;
}
_Bool ParseUseBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *useBlock, StringReference *errorMessage){
  _Bool success;
  Node *n;

  n = (Node *)malloc(sizeof(Node));
  ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

  useBlock->type = CopyString(&useBlock->typeLength, n->type, n->typeLength);
  useBlock->p1 = CopyString(&useBlock->p1Length, n->p1, n->p1Length);
  useBlock->block1 = (Node *)malloc(sizeof(Node));

  position->numberValue = position->numberValue + 1.0;

  success = ParseTemplateBlock(tokens, tokensLength, position, useBlock->block1, errorMessage);

  if(success){
    if(position->numberValue < tokensLength){
      n = (Node *)malloc(sizeof(Node));
      ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

      if(StringsEqual(n->type, n->typeLength, strparam(L"end"))){
        success = true;
        position->numberValue = position->numberValue + 1.0;
      }else{
        success = false;
        errorMessage->string = L"End block expected at the end of use block.";
        errorMessage->stringLength = wcslen(errorMessage->string);
      }
    }else{
      success = false;
      errorMessage->string = L"End block expected at the end of use block.";
      errorMessage->stringLength = wcslen(errorMessage->string);
    }
  }

  return success;
}
_Bool ParseIfBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *ifBlock, StringReference *errorMessage){
  _Bool success;
  Node *n;

  n = (Node *)malloc(sizeof(Node));
  ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

  ifBlock->type = CopyString(&ifBlock->typeLength, n->type, n->typeLength);
  ifBlock->p1 = CopyString(&ifBlock->p1Length, n->p1, n->p1Length);
  ifBlock->block1 = (Node *)malloc(sizeof(Node));
  ifBlock->hasElseBlock = false;

  position->numberValue = position->numberValue + 1.0;

  success = ParseTemplateBlock(tokens, tokensLength, position, ifBlock->block1, errorMessage);

  if(success){
    if(position->numberValue < tokensLength){
      n = (Node *)malloc(sizeof(Node));
      ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

      if(StringsEqual(n->type, n->typeLength, strparam(L"end"))){
        success = true;
        position->numberValue = position->numberValue + 1.0;
      }else if(StringsEqual(n->type, n->typeLength, strparam(L"else"))){
        position->numberValue = position->numberValue + 1.0;
        ifBlock->hasElseBlock = true;
        ifBlock->block2 = (Node *)malloc(sizeof(Node));
        success = ParseTemplateBlock(tokens, tokensLength, position, ifBlock->block2, errorMessage);

        if(success){
          if(position->numberValue < tokensLength){
            n = (Node *)malloc(sizeof(Node));
            ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

            if(StringsEqual(n->type, n->typeLength, strparam(L"end"))){
              success = true;
              position->numberValue = position->numberValue + 1.0;
            }else{
              success = false;
              errorMessage->string = L"End block expected at the end of else block.";
              errorMessage->stringLength = wcslen(errorMessage->string);
            }
          }else{
            success = false;
            errorMessage->string = L"End block expected at the end of else block.";
            errorMessage->stringLength = wcslen(errorMessage->string);
          }
        }
      }else{
        success = false;
        errorMessage->string = L"End or else block expected at the end of if block.";
        errorMessage->stringLength = wcslen(errorMessage->string);
      }
    }else{
      success = false;
      errorMessage->string = L"End or else block expected at the end of if block.";
      errorMessage->stringLength = wcslen(errorMessage->string);
    }
  }

  return success;
}
_Bool ParseForeachBlock(StringReference **tokens, size_t tokensLength, NumberReference *position, Node *foreachBlock, StringReference *errorMessage){
  _Bool success;
  Node *n;

  n = (Node *)malloc(sizeof(Node));
  ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

  foreachBlock->type = CopyString(&foreachBlock->typeLength, n->type, n->typeLength);
  foreachBlock->p1 = CopyString(&foreachBlock->p1Length, n->p1, n->p1Length);
  foreachBlock->p2 = CopyString(&foreachBlock->p2Length, n->p2, n->p2Length);
  foreachBlock->block1 = (Node *)malloc(sizeof(Node));

  position->numberValue = position->numberValue + 1.0;

  success = ParseTemplateBlock(tokens, tokensLength, position, foreachBlock->block1, errorMessage);

  if(success){
    if(position->numberValue < tokensLength){
      n = (Node *)malloc(sizeof(Node));
      ParseNodeString(tokens[(int)(position->numberValue)]->string, tokens[(int)(position->numberValue)]->stringLength, n, errorMessage);

      if(StringsEqual(n->type, n->typeLength, strparam(L"end"))){
        success = true;
        position->numberValue = position->numberValue + 1.0;
      }else{
        success = false;
        errorMessage->string = L"End block expected at the end of for each block.";
        errorMessage->stringLength = wcslen(errorMessage->string);
      }
    }else{
      success = false;
      errorMessage->string = L"End block expected at the end of foreach block.";
      errorMessage->stringLength = wcslen(errorMessage->string);
    }
  }

  return success;
}
_Bool ParseNodeString(wchar_t *token, size_t tokenLength, Node *node, StringReference *errorMessage){
  wchar_t *command;
  size_t commandLength;
  _Bool success, isText;
  StringReference **parts;
  size_t partsLength;

  success = true;
  isText = false;

  if(tokenLength <= 2.0){
    isText = true;
  }else if(token[0] == '\\' && token[1] == '{'){
    isText = true;
  }else if(token[0] != '{'){
    isText = true;
  }else{
    command = strSubstring(&commandLength, token, tokenLength, 1.0, tokenLength - 1.0);
    parts = sSplitByCharacter(&partsLength, command, commandLength, ' ');

    if(commandLength > 0.0){
      if(StringsEqual(parts[0]->string, parts[0]->stringLength, strparam(L"use"))){
        if(partsLength == 2.0){
          node->type = CopyString(&node->typeLength, parts[0]->string, parts[0]->stringLength);
          node->p1 = CopyString(&node->p1Length, parts[1]->string, parts[1]->stringLength);
        }else{
          success = false;
          errorMessage->string = L"The use command takes one parameter.";
          errorMessage->stringLength = wcslen(errorMessage->string);
        }
      }else if(StringsEqual(parts[0]->string, parts[0]->stringLength, strparam(L"end"))){
        if(partsLength == 1.0){
          node->type = CopyString(&node->typeLength, parts[0]->string, parts[0]->stringLength);
        }else{
          success = false;
          errorMessage->string = L"The end command takes no parameters.";
          errorMessage->stringLength = wcslen(errorMessage->string);
        }
      }else if(StringsEqual(parts[0]->string, parts[0]->stringLength, strparam(L"print"))){
        if(partsLength == 2.0){
          node->type = CopyString(&node->typeLength, parts[0]->string, parts[0]->stringLength);
          node->p1 = CopyString(&node->p1Length, parts[1]->string, parts[1]->stringLength);
        }else{
          success = false;
          errorMessage->string = L"The print command takes one parameter.";
          errorMessage->stringLength = wcslen(errorMessage->string);
        }
      }else if(StringsEqual(parts[0]->string, parts[0]->stringLength, strparam(L"foreach"))){
        if(partsLength == 4.0){
          if(StringsEqual(parts[2]->string, parts[2]->stringLength, strparam(L"in"))){
            node->type = CopyString(&node->typeLength, parts[0]->string, parts[0]->stringLength);
            node->p1 = CopyString(&node->p1Length, parts[1]->string, parts[1]->stringLength);
            node->p2 = CopyString(&node->p2Length, parts[3]->string, parts[3]->stringLength);
          }else{
            success = false;
            errorMessage->string = L"The foreach command must have \"in\" after the first parameter.";
            errorMessage->stringLength = wcslen(errorMessage->string);
          }
        }else{
          success = false;
          errorMessage->string = L"The foreach command takes three parameters.";
          errorMessage->stringLength = wcslen(errorMessage->string);
        }
      }else if(StringsEqual(parts[0]->string, parts[0]->stringLength, strparam(L"if"))){
        if(partsLength == 2.0){
          node->type = CopyString(&node->typeLength, parts[0]->string, parts[0]->stringLength);
          node->p1 = CopyString(&node->p1Length, parts[1]->string, parts[1]->stringLength);
        }else{
          success = false;
          errorMessage->string = L"The if command takes one parameter.";
          errorMessage->stringLength = wcslen(errorMessage->string);
        }
      }else if(StringsEqual(parts[0]->string, parts[0]->stringLength, strparam(L"else"))){
        if(partsLength == 1.0){
          node->type = CopyString(&node->typeLength, parts[0]->string, parts[0]->stringLength);
        }else{
          success = false;
          errorMessage->string = L"The else command takes no parameters.";
          errorMessage->stringLength = wcslen(errorMessage->string);
        }
      }else{
        isText = true;
      }
    }else{
      isText = true;
    }
  }

  if(isText){
    node->type = L"text";
    node->typeLength = wcslen(node->type);
    node->p1 = sReplaceString(&node->p1Length, token, tokenLength, strparam(L"\\{print "), strparam(L"{print "));
    node->p1 = sReplaceString(&node->p1Length, node->p1, node->p1Length, strparam(L"\\{use "), strparam(L"{use "));
    node->p1 = sReplaceString(&node->p1Length, node->p1, node->p1Length, strparam(L"\\{if "), strparam(L"{if "));
    node->p1 = sReplaceString(&node->p1Length, node->p1, node->p1Length, strparam(L"\\{end}"), strparam(L"{end}"));
    node->p1 = sReplaceString(&node->p1Length, node->p1, node->p1Length, strparam(L"\\{foreach "), strparam(L"{foreach "));
    node->p1 = sReplaceString(&node->p1Length, node->p1, node->p1Length, strparam(L"\\{else}"), strparam(L"{else}"));
  }

  return success;
}
double test(){
  NumberReference *failures;

  failures = CreateNumberReference(0.0);

  testTokenGeneration(failures);

  testGenerateDocument1(failures);
  testGenerateDocument2(failures);
  testGenerateDocument3(failures);
  testGenerateDocument4(failures);
  testGenerateDocument5(failures);
  testGenerateDocument6(failures);
  testGenerateDocument7(failures);

  return failures->numberValue;
}
void testTokenGeneration(NumberReference *failures){
  wchar_t *template;
  size_t templateLength;
  LinkedListStrings *tokens;
  _Bool success;
  StringReference *errorMessage;

  errorMessage = (StringReference *)malloc(sizeof(StringReference));

  tokens = CreateLinkedListString();
  template = L"This is a template, this is a value: {print a}.";
  templateLength = wcslen(template);
  success = GenerateTokensFromTemplate(template, templateLength, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(3.0, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  template = L"This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.";
  templateLength = wcslen(template);
  success = GenerateTokensFromTemplate(template, templateLength, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(9.0, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  template = L"This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.";
  templateLength = wcslen(template);
  success = GenerateTokensFromTemplate(template, templateLength, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(16.0, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  template = L"T: {foreach a in b}{print a}{end}.";
  templateLength = wcslen(template);
  success = GenerateTokensFromTemplate(template, templateLength, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(5.0, LinkedListStringsLength(tokens), failures);
}
void testGenerateDocument1(NumberReference *failures){
  AssertTemplateResult(strparam(L"This is a template, this is a value: {print a}."), strparam(L"{\"c\": 5, \"a\": 6}"), strparam(L"This is a template, this is a value: 6."), failures);
}
void testGenerateDocument2(NumberReference *failures){
  AssertTemplateResult(strparam(L"This is a template, this is a value: {print a} {use b}{print a} {print b}{end}."), strparam(L"{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}"), strparam(L"This is a template, this is a value: 6 1 2."), failures);
}
void testGenerateDocument3(NumberReference *failures){
  AssertTemplateResult(strparam(L"This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}."), strparam(L"{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}"), strparam(L"This is a template, this is a value: 6 1 23 416."), failures);
}
void testGenerateDocument4(NumberReference *failures){
  AssertTemplateResult(strparam(L"T: {if a}a{end}."), strparam(L"{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}"), strparam(L"T: a."), failures);

  AssertTemplateResult(strparam(L"T: {if a}a{else}b{end}."), strparam(L"{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}"), strparam(L"T: b."), failures);
}
void testGenerateDocument5(NumberReference *failures){
  AssertTemplateResult(strparam(L"T: {foreach a in b}{print a}{end}."), strparam(L"{\"b\": [1, 2, 3, 4]}"), strparam(L"T: 1234."), failures);
}
void testGenerateDocument6(NumberReference *failures){
  AssertTemplateResult(strparam(L"{test}\\{print a}."), strparam(L"{\"c\": 5, \"a\": 6}"), strparam(L"{test}{print a}."), failures);
}
void testGenerateDocument7(NumberReference *failures){
  AssertTemplateResult(strparam(L"{"), strparam(L"{}"), strparam(L"{"), failures);

  AssertTemplateResult(strparam(L"{  "), strparam(L"{}"), strparam(L"{  "), failures);

  AssertTemplateResult(strparam(L"{use a}\\{print a}{end}"), strparam(L"{\"a\": {}}"), strparam(L"{print a}"), failures);

  AssertTemplateResult(strparam(L"\\{print a}{print a}}"), strparam(L"{\"a\": 3}"), strparam(L"{print a}3}"), failures);

  AssertTemplateResult(strparam(L"\\\\{print a}{print a}}"), strparam(L"{\"a\": 3}"), strparam(L"\\{print a}3}"), failures);

  AssertTemplateResult(strparam(L"\\\\{print a}{print a}\\\\{print a}}"), strparam(L"{\"a\": 3}"), strparam(L"\\{print a}3\\{print a}}"), failures);

  AssertTemplateResult(strparam(L"\\{print a}{print a}\\{"), strparam(L"{\"a\": 3}"), strparam(L"{print a}3\\{"), failures);

  AssertTemplateResult(strparam(L"    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>"), strparam(L"{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}"), strparam(L"    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>"), failures);

  AssertTemplateError(strparam(L"{print"), strparam(L"{}"), strparam(L"Template command found, but not ended properly."), failures);

  AssertTemplateError(strparam(L"{print a}"), strparam(L"{}"), strparam(L"Key for printing not found in JSON object: a"), failures);

  AssertTemplateError(strparam(L"This is a template, this is a value: {print a {print a}."), strparam(L"{\"a\": 5}"), strparam(L"The print command takes one parameter."), failures);

  AssertTemplateError(strparam(L"This is a {use a}\\{print a}template, this is a value: {print a}.{end}"), strparam(L"{\"a\": 5}"), strparam(L"Data structure for print command is not a JSON object."), failures);

  AssertTemplateError(strparam(L"{use a}"), strparam(L"{}"), strparam(L"End block expected at the end of use block."), failures);

  AssertTemplateError(strparam(L"{if a}"), strparam(L"{\"a\": true}"), strparam(L"End or else block expected at the end of if block."), failures);

  AssertTemplateError(strparam(L"{if a}{else}"), strparam(L"{\"a\": true}"), strparam(L"End block expected at the end of else block."), failures);

  AssertTemplateError(strparam(L"{foreach x in a}"), strparam(L"{\"a\": [1, 2, 3, 4]}"), strparam(L"End block expected at the end of foreach block."), failures);
}
void AssertTemplateResult(wchar_t *template, size_t templateLength, wchar_t *json, size_t jsonLength, wchar_t *result, size_t resultLength, NumberReference *failures){
  ElementReference *data;
  StringArrayReference *errorMessages;
  StringReference *document;
  StringReference *errorMessage;
  _Bool success;

  data = (ElementReference *)malloc(sizeof(ElementReference));
  errorMessages = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  document = (StringReference *)malloc(sizeof(StringReference));
  errorMessage = (StringReference *)malloc(sizeof(StringReference));

  success = ReadJSON(json, jsonLength, data, errorMessages);

  AssertTrue(success, failures);

  if(success){
    success = GenerateDocument(template, templateLength, data->element, document, errorMessage);

    AssertTrue(success, failures);

    if(success){
      AssertStringEquals(document->string, document->stringLength, result, resultLength, failures);
    }
  }
}
void AssertTemplateError(wchar_t *template, size_t templateLength, wchar_t *json, size_t jsonLength, wchar_t *errorMessage, size_t errorMessageLength, NumberReference *failures){
  ElementReference *data;
  StringArrayReference *errorMessages;
  StringReference *document;
  StringReference *errorMessageRef;
  _Bool success;

  data = (ElementReference *)malloc(sizeof(ElementReference));
  errorMessages = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  document = (StringReference *)malloc(sizeof(StringReference));
  errorMessageRef = (StringReference *)malloc(sizeof(StringReference));

  success = ReadJSON(json, jsonLength, data, errorMessages);

  AssertTrue(success, failures);

  if(success){
    success = GenerateDocument(template, templateLength, data->element, document, errorMessageRef);

    AssertFalse(success, failures);

    if( !success ){
      AssertStringEquals(errorMessage, errorMessageLength, errorMessageRef->string, errorMessageRef->stringLength, failures);
    }
  }
}
BooleanReference *CreateBooleanReference(_Bool value){
  BooleanReference *ref;

  ref = (BooleanReference *)malloc(sizeof(BooleanReference));
  ref->booleanValue = value;

  return ref;
}
BooleanArrayReference *CreateBooleanArrayReference(_Bool *value, size_t valueLength){
  BooleanArrayReference *ref;

  ref = (BooleanArrayReference *)malloc(sizeof(BooleanArrayReference));
  ref->booleanArray = value;
  ref->booleanArrayLength = valueLength;

  return ref;
}
BooleanArrayReference *CreateBooleanArrayReferenceLengthValue(double length, _Bool value){
  BooleanArrayReference *ref;
  double i;

  ref = (BooleanArrayReference *)malloc(sizeof(BooleanArrayReference));
  ref->booleanArray = (_Bool*)malloc(sizeof(_Bool) * (length));
  ref->booleanArrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->booleanArray[(int)(i)] = value;
  }

  return ref;
}
void FreeBooleanArrayReference(BooleanArrayReference *booleanArrayReference){
  free(booleanArrayReference->booleanArray);
  free(booleanArrayReference);
}
CharacterReference *CreateCharacterReference(wchar_t value){
  CharacterReference *ref;

  ref = (CharacterReference *)malloc(sizeof(CharacterReference));
  ref->characterValue = value;

  return ref;
}
NumberReference *CreateNumberReference(double value){
  NumberReference *ref;

  ref = (NumberReference *)malloc(sizeof(NumberReference));
  ref->numberValue = value;

  return ref;
}
NumberArrayReference *CreateNumberArrayReference(double *value, size_t valueLength){
  NumberArrayReference *ref;

  ref = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  ref->numberArray = value;
  ref->numberArrayLength = valueLength;

  return ref;
}
NumberArrayReference *CreateNumberArrayReferenceLengthValue(double length, double value){
  NumberArrayReference *ref;
  double i;

  ref = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  ref->numberArray = (double*)malloc(sizeof(double) * (length));
  ref->numberArrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->numberArray[(int)(i)] = value;
  }

  return ref;
}
void FreeNumberArrayReference(NumberArrayReference *numberArrayReference){
  free(numberArrayReference->numberArray);
  free(numberArrayReference);
}
StringReference *CreateStringReference(wchar_t *value, size_t valueLength){
  StringReference *ref;

  ref = (StringReference *)malloc(sizeof(StringReference));
  ref->string = value;
  ref->stringLength = valueLength;

  return ref;
}
StringReference *CreateStringReferenceLengthValue(double length, wchar_t value){
  StringReference *ref;
  double i;

  ref = (StringReference *)malloc(sizeof(StringReference));
  ref->string = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  ref->stringLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->string[(int)(i)] = value;
  }

  return ref;
}
void FreeStringReference(StringReference *stringReference){
  free(stringReference->string);
  free(stringReference);
}
StringArrayReference *CreateStringArrayReference(StringReference **strings, size_t stringsLength){
  StringArrayReference *ref;

  ref = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  ref->stringArray = strings;
  ref->stringArrayLength = stringsLength;

  return ref;
}
StringArrayReference *CreateStringArrayReferenceLengthValue(double length, wchar_t *value, size_t valueLength){
  StringArrayReference *ref;
  double i;

  ref = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  ref->stringArray = (StringReference**)malloc(sizeof(StringReference) * length);
  ref->stringArrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->stringArray[(int)(i)] = CreateStringReference(value, valueLength);
  }

  return ref;
}
void FreeStringArrayReference(StringArrayReference *stringArrayReference){
  double i;

  for(i = 0.0; i < stringArrayReference->stringArrayLength; i = i + 1.0){
    free(stringArrayReference->stringArray[(int)(i)]);
  }
  free(stringArrayReference->stringArray);
  free(stringArrayReference);
}
_Bool IsValidJSON(wchar_t *json, size_t jsonLength, StringArrayReference *errorMessage){
  _Bool success;
  ElementReference *elementReference;

  elementReference = (ElementReference *)malloc(sizeof(ElementReference));

  success = ReadJSON(json, jsonLength, elementReference, errorMessage);

  if(success){
    DeleteElement(elementReference->element);
  }

  return success;
}
_Bool JSONTokenize(wchar_t *json, size_t jsonLength, StringArrayReference *tokensReference, StringArrayReference *errorMessages){
  double i;
  wchar_t c;
  wchar_t *str;
  size_t strLength;
  StringReference *stringReference, *tokenReference;
  NumberReference *stringLength;
  _Bool success;
  lLinkedListStrings *ll;

  ll = lCreateLinkedListString();
  success = true;

  stringLength = (NumberReference *)malloc(sizeof(NumberReference));
  tokenReference = (StringReference *)malloc(sizeof(StringReference));

  for(i = 0.0; i < jsonLength && success; ){
    c = json[(int)(i)];

    if(c == '{'){
      lLinkedListAddString(ll, strparam(L"{"));
      i = i + 1.0;
    }else if(c == '}'){
      lLinkedListAddString(ll, strparam(L"}"));
      i = i + 1.0;
    }else if(c == '['){
      lLinkedListAddString(ll, strparam(L"["));
      i = i + 1.0;
    }else if(c == ']'){
      lLinkedListAddString(ll, strparam(L"]"));
      i = i + 1.0;
    }else if(c == ':'){
      lLinkedListAddString(ll, strparam(L":"));
      i = i + 1.0;
    }else if(c == ','){
      lLinkedListAddString(ll, strparam(L","));
      i = i + 1.0;
    }else if(c == 'f'){
      success = GetJSONPrimitiveName(json, jsonLength, i, errorMessages, strparam(L"false"), tokenReference);
      if(success){
        lLinkedListAddString(ll, strparam(L"false"));
        i = i + wcslen(L"false");
      }
    }else if(c == 't'){
      success = GetJSONPrimitiveName(json, jsonLength, i, errorMessages, strparam(L"true"), tokenReference);
      if(success){
        lLinkedListAddString(ll, strparam(L"true"));
        i = i + wcslen(L"true");
      }
    }else if(c == 'n'){
      success = GetJSONPrimitiveName(json, jsonLength, i, errorMessages, strparam(L"null"), tokenReference);
      if(success){
        lLinkedListAddString(ll, strparam(L"null"));
        i = i + wcslen(L"null");
      }
    }else if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
      /* Skip. */
      i = i + 1.0;
    }else if(c == '\"'){
      success = GetJSONString(json, jsonLength, i, tokenReference, stringLength, errorMessages);
      if(success){
        lLinkedListAddString(ll, tokenReference->string, tokenReference->stringLength);
        i = i + stringLength->numberValue;
      }
    }else if(IsJSONNumberCharacter(c)){
      success = GetJSONNumberToken(json, jsonLength, i, tokenReference, errorMessages);
      if(success){
        lLinkedListAddString(ll, tokenReference->string, tokenReference->stringLength);
        i = i + tokenReference->stringLength;
      }
    }else{
      str = strConcatenateCharacter(&strLength, strparam(L"Invalid start of Token: "), c);
      stringReference = CreateStringReference(str, strLength);
      lAddStringRef(errorMessages, stringReference);
      i = i + 1.0;
      success = false;
    }
  }

  if(success){
    lLinkedListAddString(ll, strparam(L"<end>"));
    tokensReference->stringArray = lLinkedListStringsToArray(&tokensReference->stringArrayLength, ll);
    lFreeLinkedListString(ll);
  }

  return success;
}
_Bool GetJSONNumberToken(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, StringArrayReference *errorMessages){
  wchar_t c;
  double end, i;
  _Bool done, success;
  wchar_t *numberString;
  size_t numberStringLength;

  end = jsonLength;
  done = false;

  for(i = start; i < jsonLength &&  !done ; i = i + 1.0){
    c = json[(int)(i)];
    if( !IsJSONNumberCharacter(c) ){
      done = true;
      end = i;
    }
  }

  numberString = strSubstring(&numberStringLength, json, jsonLength, start, end);

  success = IsValidJSONNumber(numberString, numberStringLength, errorMessages);

  tokenReference->string = numberString;
  tokenReference->stringLength = numberStringLength;

  return success;
}
_Bool IsValidJSONNumber(wchar_t *n, size_t nLength, StringArrayReference *errorMessages){
  _Bool success;
  double i;

  i = 0.0;

  /* JSON allows an optional negative sign. */
  if(n[(int)(i)] == '-'){
    i = i + 1.0;
  }

  if(i < nLength){
    success = IsValidJSONNumberAfterSign(n, nLength, i, errorMessages);
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"Number must contain at least one digit.")));
  }

  return success;
}
_Bool IsValidJSONNumberAfterSign(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages){
  _Bool success;

  if(charIsNumber(n[(int)(i)])){
    /* 0 first means only 0. */
    if(n[(int)(i)] == '0'){
      i = i + 1.0;
    }else{
      /* 1-9 first, read following digits. */
      i = IsValidJSONNumberAdvancePastDigits(n, nLength, i);
    }

    if(i < nLength){
      success = IsValidJSONNumberFromDotOrExponent(n, nLength, i, errorMessages);
    }else{
      /* If integer, we are done now. */
      success = true;
    }
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"A number must start with 0-9 (after the optional sign).")));
  }

  return success;
}
double IsValidJSONNumberAdvancePastDigits(wchar_t *n, size_t nLength, double i){
  _Bool done;

  i = i + 1.0;
  done = false;
  for(; i < nLength &&  !done ; ){
    if(charIsNumber(n[(int)(i)])){
      i = i + 1.0;
    }else{
      done = true;
    }
  }

  return i;
}
_Bool IsValidJSONNumberFromDotOrExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages){
  _Bool wasDotAndOrE, success;

  wasDotAndOrE = false;
  success = true;

  if(n[(int)(i)] == '.'){
    i = i + 1.0;
    wasDotAndOrE = true;

    if(i < nLength){
      if(charIsNumber(n[(int)(i)])){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, nLength, i);

        if(i == nLength){
          /* If non-scientific decimal number, we are done. */
          success = true;
        }
      }else{
        success = false;
        lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be numbers after the decimal point.")));
      }
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be numbers after the decimal point.")));
    }
  }

  if(i < nLength && success){
    if(n[(int)(i)] == 'e' || n[(int)(i)] == 'E'){
      wasDotAndOrE = true;
      success = IsValidJSONNumberFromExponent(n, nLength, i, errorMessages);
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(strparam(L"Expected e or E.")));
    }
  }else if(i == nLength && success){
    /* If number with decimal point. */
    success = true;
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be numbers after the decimal point.")));
  }

  if(wasDotAndOrE){
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"Exprected decimal point or e or E.")));
  }

  return success;
}
_Bool IsValidJSONNumberFromExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages){
  _Bool success;

  i = i + 1.0;

  if(i < nLength){
    /* The exponent sign can either + or -, */
    if(n[(int)(i)] == '+' || n[(int)(i)] == '-'){
      i = i + 1.0;
    }

    if(i < nLength){
      if(charIsNumber(n[(int)(i)])){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, nLength, i);

        if(i == nLength){
          /* We found scientific number. */
          success = true;
        }else{
          success = false;
          lAddStringRef(errorMessages, CreateStringReference(strparam(L"There was characters following the exponent.")));
        }
      }else{
        success = false;
        lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be a digit following the optional exponent sign.")));
      }
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be a digit following optional the exponent sign.")));
    }
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be a sign or a digit following e or E.")));
  }

  return success;
}
_Bool IsJSONNumberCharacter(wchar_t c){
  wchar_t *numericCharacters;
  size_t numericCharactersLength;
  _Bool found;
  double i;

  numericCharacters = L"0123456789.-+eE";
  numericCharactersLength = wcslen(numericCharacters);

  found = false;

  for(i = 0.0; i < numericCharactersLength; i = i + 1.0){
    if(numericCharacters[(int)(i)] == c){
      found = true;
    }
  }

  return found;
}
_Bool GetJSONPrimitiveName(wchar_t *string, size_t stringLength, double start, StringArrayReference *errorMessages, wchar_t *primitive, size_t primitiveLength, StringReference *tokenReference){
  wchar_t c, p;
  _Bool done, success;
  double i;
  wchar_t *str, *token;
  size_t strLength, tokenLength;

  done = false;
  success = true;

  token = L"";
  tokenLength = wcslen(token);

  for(i = start; i < stringLength && ((i - start) < primitiveLength) &&  !done ; i = i + 1.0){
    c = string[(int)(i)];
    p = primitive[(int)(i - start)];
    if(c == p){
      /* OK */
      if((i + 1.0 - start) == primitiveLength){
        done = true;
      }
    }else{
      str = L"";
      strLength = wcslen(str);
      str = strConcatenateString(&strLength, str, strLength, strparam(L"Primitive invalid: "));
      str = strAppendCharacter(&strLength, str, strLength, c);
      str = strAppendString(&strLength, str, strLength, strparam(L" vs "));
      str = strAppendCharacter(&strLength, str, strLength, p);

      lAddStringRef(errorMessages, CreateStringReference(str, strLength));
      done = true;
      success = false;
    }
  }

  if(done){
    if(StringsEqual(primitive, primitiveLength, strparam(L"false"))){
      token = L"false";
      tokenLength = wcslen(token);
    }
    if(StringsEqual(primitive, primitiveLength, strparam(L"true"))){
      token = L"true";
      tokenLength = wcslen(token);
    }
    if(StringsEqual(primitive, primitiveLength, strparam(L"null"))){
      token = L"null";
      tokenLength = wcslen(token);
    }
  }else{
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"Primitive invalid")));
    success = false;
  }

  tokenReference->string = token;
  tokenReference->stringLength = tokenLength;

  return success;
}
_Bool GetJSONString(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, NumberReference *stringLengthReference, StringArrayReference *errorMessages){
  _Bool success, done;
  wchar_t *string, *hex;
  size_t stringLength, hexLength;
  NumberReference *characterCount, *hexReference;
  double i, l, c;
  StringReference *errorMessage;

  characterCount = CreateNumberReference(0.0);
  hex = CreateString(&hexLength, 4.0, '0');
  hexReference = (NumberReference *)malloc(sizeof(NumberReference));
  errorMessage = (StringReference *)malloc(sizeof(StringReference));

  success = IsValidJSONStringInJSON(json, jsonLength, start, characterCount, stringLengthReference, errorMessages);

  if(success){
    l = characterCount->numberValue;
    string = (wchar_t*)malloc(sizeof(wchar_t) * (l));
    stringLength = l;

    c = 0.0;
    string[(int)(c)] = '\"';
    c = c + 1.0;

    done = false;
    for(i = start + 1.0;  !done ; i = i + 1.0){
      if(json[(int)(i)] == '\\'){
        i = i + 1.0;
        if(json[(int)(i)] == '\"' || json[(int)(i)] == '\\' || json[(int)(i)] == '/'){
          string[(int)(c)] = json[(int)(i)];
          c = c + 1.0;
        }else if(json[(int)(i)] == 'b'){
          string[(int)(c)] = 8.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'f'){
          string[(int)(c)] = 12.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'n'){
          string[(int)(c)] = 10.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'r'){
          string[(int)(c)] = 13.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 't'){
          string[(int)(c)] = 9.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'u'){
          i = i + 1.0;
          hex[0] = charToUpperCase(json[(int)(i + 0.0)]);
          hex[1] = charToUpperCase(json[(int)(i + 1.0)]);
          hex[2] = charToUpperCase(json[(int)(i + 2.0)]);
          hex[3] = charToUpperCase(json[(int)(i + 3.0)]);
          nCreateNumberFromStringWithCheck(hex, hexLength, 16.0, hexReference, errorMessage);
          string[(int)(c)] = hexReference->numberValue;
          i = i + 3.0;
          c = c + 1.0;
        }
      }else if(json[(int)(i)] == '\"'){
        string[(int)(c)] = json[(int)(i)];
        c = c + 1.0;
        done = true;
      }else{
        string[(int)(c)] = json[(int)(i)];
        c = c + 1.0;
      }
    }

    tokenReference->string = string;
    tokenReference->stringLength = stringLength;
    success = true;
  }else{
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"End of string was not found.")));
    success = false;
  }

  return success;
}
_Bool IsValidJSONString(wchar_t *jsonString, size_t jsonStringLength, StringArrayReference *errorMessages){
  _Bool valid;
  NumberReference *numberReference, *stringLength;

  numberReference = (NumberReference *)malloc(sizeof(NumberReference));
  stringLength = (NumberReference *)malloc(sizeof(NumberReference));

  valid = IsValidJSONStringInJSON(jsonString, jsonStringLength, 0.0, numberReference, stringLength, errorMessages);

  return valid;
}
_Bool IsValidJSONStringInJSON(wchar_t *json, size_t jsonLength, double start, NumberReference *characterCount, NumberReference *stringLengthReference, StringArrayReference *errorMessages){
  _Bool success, done;
  double i, j;
  wchar_t c;

  success = true;
  done = false;

  characterCount->numberValue = 1.0;

  for(i = start + 1.0; i < jsonLength &&  !done  && success; i = i + 1.0){
    if( !IsJSONIllegalControllCharacter(json[(int)(i)]) ){
      if(json[(int)(i)] == '\\'){
        i = i + 1.0;
        if(i < jsonLength){
          if(json[(int)(i)] == '\"' || json[(int)(i)] == '\\' || json[(int)(i)] == '/' || json[(int)(i)] == 'b' || json[(int)(i)] == 'f' || json[(int)(i)] == 'n' || json[(int)(i)] == 'r' || json[(int)(i)] == 't'){
            characterCount->numberValue = characterCount->numberValue + 1.0;
          }else if(json[(int)(i)] == 'u'){
            if(i + 4.0 < jsonLength){
              for(j = 0.0; j < 4.0 && success; j = j + 1.0){
                c = json[(int)(i + j + 1.0)];
                if(nCharacterIsNumberCharacterInBase(c, 16.0) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f'){
                }else{
                  success = false;
                  lAddStringRef(errorMessages, CreateStringReference(strparam(L"\\u must be followed by four hexadecimal digits.")));
                }
              }
              characterCount->numberValue = characterCount->numberValue + 1.0;
              i = i + 4.0;
            }else{
              success = false;
              lAddStringRef(errorMessages, CreateStringReference(strparam(L"\\u must be followed by four characters.")));
            }
          }else{
            success = false;
            lAddStringRef(errorMessages, CreateStringReference(strparam(L"Escaped character invalid.")));
          }
        }else{
          success = false;
          lAddStringRef(errorMessages, CreateStringReference(strparam(L"There must be at least two character after string escape.")));
        }
      }else if(json[(int)(i)] == '\"'){
        characterCount->numberValue = characterCount->numberValue + 1.0;
        done = true;
      }else{
        characterCount->numberValue = characterCount->numberValue + 1.0;
      }
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(strparam(L"Unicode code points 0-31 not allowed in JSON string.")));
    }
  }

  if(done){
    stringLengthReference->numberValue = i - start;
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"String must end with \".")));
  }

  return success;
}
_Bool IsJSONIllegalControllCharacter(wchar_t c){
  double cnr;
  _Bool isControll;

  cnr = c;

  if(cnr >= 0.0 && cnr < 32.0){
    isControll = true;
  }else{
    isControll = false;
  }

  return isControll;
}
Element **AddElement(size_t *returnArrayLength, Element **list, size_t listLength, Element *a){
  Element **newlist;
  size_t newlistLength;
  double i;

  newlist = (Element**)malloc(sizeof(Element) * listLength + 1.0);
  newlistLength = listLength + 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;

  free(list);

  *returnArrayLength = newlistLength;
  return newlist;
}
void AddElementRef(ElementArrayReference *list, Element *i){
  list->array = AddElement(&list->arrayLength, list->array, list->arrayLength, i);
}
Element **RemoveElement(size_t *returnArrayLength, Element **list, size_t listLength, double n){
  Element **newlist;
  size_t newlistLength;
  double i;

  newlist = (Element**)malloc(sizeof(Element) * listLength - 1.0);
  newlistLength = listLength - 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    if(i < n){
      newlist[(int)(i)] = list[(int)(i)];
    }
    if(i > n){
      newlist[(int)(i - 1.0)] = list[(int)(i)];
    }
  }

  free(list);

  *returnArrayLength = newlistLength;
  return newlist;
}
Element *GetElementRef(ElementArrayReference *list, double i){
  return list->array[(int)(i)];
}
void RemoveElementRef(ElementArrayReference *list, double i){
  list->array = RemoveElement(&list->arrayLength, list->array, list->arrayLength, i);
}
LinkedListElements *CreateLinkedListElements(){
  LinkedListElements *ll;

  ll = (LinkedListElements *)malloc(sizeof(LinkedListElements));
  ll->first = (LinkedListNodeElements *)malloc(sizeof(LinkedListNodeElements));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddElement(LinkedListElements *ll, Element *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeElements *)malloc(sizeof(LinkedListNodeElements));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
Element **LinkedListElementsToArray(size_t *returnArrayLength, LinkedListElements *ll){
  Element **array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeElements *node;

  node = ll->first;

  length = LinkedListElementsLength(ll);

  array = (Element**)malloc(sizeof(Element) * length);
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListElementsLength(LinkedListElements *ll){
  double l;
  LinkedListNodeElements *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListElements(LinkedListElements *ll){
  LinkedListNodeElements *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
double ComputeJSONStringLength(Element *element){
  double result;

  result = 0.0;

  if(StringsEqual(element->type, element->typeLength, strparam(L"object"))){
    result = result + ComputeJSONObjectStringLength(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"string"))){
    result = JSONEscapedStringLength(element->string, element->stringLength) + 2.0;
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"array"))){
    result = result + ComputeJSONArrayStringLength(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"number"))){
    result = result + ComputeJSONNumberStringLength(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"null"))){
    result = result + wcslen(L"null");
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"boolean"))){
    result = result + ComputeJSONBooleanStringLength(element);
  }

  return result;
}
double ComputeJSONBooleanStringLength(Element *element){
  double result;

  if(element->booleanValue){
    result = wcslen(L"true");
  }else{
    result = wcslen(L"false");
  }

  return result;
}
double ComputeJSONNumberStringLength(Element *element){
  double length;
  wchar_t *a;
  size_t aLength;

  if(fabs(element->number) >= pow(10.0, 15.0) || fabs(element->number) <= pow(10.0,  -15.0)){
    a = nCreateStringScientificNotationDecimalFromNumber(&aLength, element->number);
    length = aLength;
  }else{
    a = nCreateStringDecimalFromNumber(&aLength, element->number);
    length = aLength;
  }

  return length;
}
double ComputeJSONArrayStringLength(Element *element){
  Element *arrayElement;
  double i;
  double length;

  length = 1.0;

  for(i = 0.0; i < element->arrayLength; i = i + 1.0){
    arrayElement = element->array[(int)(i)];

    length = length + ComputeJSONStringLength(arrayElement);

    if(i + 1.0 != element->arrayLength){
      length = length + 1.0;
    }
  }

  length = length + 1.0;

  return length;
}
double ComputeJSONObjectStringLength(Element *element){
  wchar_t *key;
  size_t keyLength;
  double i;
  StringArrayReference *keys;
  Element *objectElement;
  double length;

  length = 1.0;

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArrayLength; i = i + 1.0){
    key = keys->stringArray[(int)(i)]->string;
    keyLength = keys->stringArray[(int)(i)]->stringLength;
    objectElement = GetObjectValue(element->object, key, keyLength);

    length = length + 1.0;
    length = length + JSONEscapedStringLength(key, keyLength);
    length = length + 1.0;
    length = length + 1.0;

    length = length + ComputeJSONStringLength(objectElement);

    if(i + 1.0 != keys->stringArrayLength){
      length = length + 1.0;
    }
  }

  length = length + 1.0;

  return length;
}
Element *CreateStringElement(wchar_t *string, size_t stringLength){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = L"string";
  element->typeLength = wcslen(element->type);
  element->string = string;
  element->stringLength = stringLength;
  return element;
}
Element *CreateBooleanElement(_Bool booleanValue){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = L"boolean";
  element->typeLength = wcslen(element->type);
  element->booleanValue = booleanValue;
  return element;
}
Element *CreateNullElement(){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = L"null";
  element->typeLength = wcslen(element->type);
  return element;
}
Element *CreateNumberElement(double number){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = L"number";
  element->typeLength = wcslen(element->type);
  element->number = number;
  return element;
}
Element *CreateArrayElement(double length){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = L"array";
  element->typeLength = wcslen(element->type);
  element->array = (Element**)malloc(sizeof(Element) * length);
  element->arrayLength = length;
  return element;
}
Element *CreateObjectElement(double length){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = L"object";
  element->typeLength = wcslen(element->type);
  element->object = (StringElementMap *)malloc(sizeof(StringElementMap));
  element->object->stringListRef = CreateStringArrayReferenceLengthValue(length, strparam(L""));
  element->object->elementListRef = (ElementArrayReference *)malloc(sizeof(ElementArrayReference));
  element->object->elementListRef->array = (Element**)malloc(sizeof(Element) * length);
  element->object->elementListRef->arrayLength = length;
  return element;
}
void DeleteElement(Element *element){
  if(StringsEqual(element->type, element->typeLength, strparam(L"object"))){
    DeleteObject(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"string"))){
    free(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"array"))){
    DeleteArray(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"number"))){
    free(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"null"))){
    free(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"boolean"))){
    free(element);
  }else{
  }
}
void DeleteObject(Element *element){
  StringArrayReference *keys;
  double i;
  wchar_t *key;
  size_t keyLength;
  Element *objectElement;

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArrayLength; i = i + 1.0){
    key = keys->stringArray[(int)(i)]->string;
    keyLength = keys->stringArray[(int)(i)]->stringLength;
    objectElement = GetObjectValue(element->object, key, keyLength);
    DeleteElement(objectElement);
  }
}
void DeleteArray(Element *element){
  double i;
  Element *arrayElement;

  for(i = 0.0; i < element->arrayLength; i = i + 1.0){
    arrayElement = element->array[(int)(i)];
    DeleteElement(arrayElement);
  }
}
wchar_t *WriteJSON(size_t *returnArrayLength, Element *element){
  wchar_t *result;
  size_t resultLength;
  double length;
  NumberReference *index;

  length = ComputeJSONStringLength(element);
  result = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  resultLength = length;
  index = CreateNumberReference(0.0);

  if(StringsEqual(element->type, element->typeLength, strparam(L"object"))){
    WriteObject(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"string"))){
    WriteString(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"array"))){
    WriteArray(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"number"))){
    WriteNumber(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"null"))){
    strWriteStringToStingStream(result, resultLength, index, strparam(L"null"));
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"boolean"))){
    WriteBooleanValue(element, result, resultLength, index);
  }

  *returnArrayLength = resultLength;
  return result;
}
void WriteBooleanValue(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  if(element->booleanValue){
    strWriteStringToStingStream(result, resultLength, index, strparam(L"true"));
  }else{
    strWriteStringToStingStream(result, resultLength, index, strparam(L"false"));
  }
}
void WriteNumber(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  wchar_t *numberString;
  size_t numberStringLength;

  if(fabs(element->number) >= pow(10.0, 15.0) || fabs(element->number) <= pow(10.0,  -15.0)){
    numberString = nCreateStringScientificNotationDecimalFromNumber(&numberStringLength, element->number);
  }else{
    numberString = nCreateStringDecimalFromNumber(&numberStringLength, element->number);
  }

  strWriteStringToStingStream(result, resultLength, index, numberString, numberStringLength);
}
void WriteArray(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  wchar_t *s;
  size_t sLength;
  Element *arrayElement;
  double i;

  strWriteStringToStingStream(result, resultLength, index, strparam(L"["));

  for(i = 0.0; i < element->arrayLength; i = i + 1.0){
    arrayElement = element->array[(int)(i)];

    s = WriteJSON(&sLength, arrayElement);
    strWriteStringToStingStream(result, resultLength, index, s, sLength);

    if(i + 1.0 != element->arrayLength){
      strWriteStringToStingStream(result, resultLength, index, strparam(L","));
    }
  }

  strWriteStringToStingStream(result, resultLength, index, strparam(L"]"));
}
void WriteString(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
  element->string = JSONEscapeString(&element->stringLength, element->string, element->stringLength);
  strWriteStringToStingStream(result, resultLength, index, element->string, element->stringLength);
  strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
}
wchar_t *JSONEscapeString(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  double i, length;
  NumberReference *index, *lettersReference;
  wchar_t *ns, *escaped;
  size_t nsLength, escapedLength;

  length = JSONEscapedStringLength(string, stringLength);

  ns = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  nsLength = length;
  index = CreateNumberReference(0.0);
  lettersReference = CreateNumberReference(0.0);

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(JSONMustBeEscaped(string[(int)(i)], lettersReference)){
      escaped = JSONEscapeCharacter(&escapedLength, string[(int)(i)]);
      strWriteStringToStingStream(ns, nsLength, index, escaped, escapedLength);
    }else{
      strWriteCharacterToStingStream(ns, nsLength, index, string[(int)(i)]);
    }
  }

  *returnArrayLength = nsLength;
  return ns;
}
double JSONEscapedStringLength(wchar_t *string, size_t stringLength){
  NumberReference *lettersReference;
  double i, length;

  lettersReference = CreateNumberReference(0.0);
  length = 0.0;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(JSONMustBeEscaped(string[(int)(i)], lettersReference)){
      length = length + lettersReference->numberValue;
    }else{
      length = length + 1.0;
    }
  }
  return length;
}
wchar_t *JSONEscapeCharacter(size_t *returnArrayLength, wchar_t c){
  double code;
  wchar_t *escaped;
  size_t escapedLength;
  StringReference *hexNumber;
  double q, rs, s, b, f, n, r, t;

  code = c;

  q = 34.0;
  rs = 92.0;
  s = 47.0;
  b = 8.0;
  f = 12.0;
  n = 10.0;
  r = 13.0;
  t = 9.0;

  hexNumber = (StringReference *)malloc(sizeof(StringReference));

  if(code == q){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = '\"';
  }else if(code == rs){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = '\\';
  }else if(code == s){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = '/';
  }else if(code == b){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'b';
  }else if(code == f){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'f';
  }else if(code == n){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'n';
  }else if(code == r){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'r';
  }else if(code == t){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 't';
  }else if(code >= 0.0 && code <= 31.0){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (6.0));
    escapedLength = 6.0;
    escaped[0] = '\\';
    escaped[1] = 'u';
    escaped[2] = '0';
    escaped[3] = '0';

    nCreateStringFromNumberWithCheck(code, 16.0, hexNumber);

    if(hexNumber->stringLength == 1.0){
      escaped[4] = '0';
      escaped[5] = hexNumber->string[0];
    }else if(hexNumber->stringLength == 2.0){
      escaped[4] = hexNumber->string[0];
      escaped[5] = hexNumber->string[1];
    }
  }else{
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (1.0));
    escapedLength = 1.0;
    escaped[0] = c;
  }

  *returnArrayLength = escapedLength;
  return escaped;
}
_Bool JSONMustBeEscaped(wchar_t c, NumberReference *letters){
  double code;
  _Bool mustBeEscaped;
  double q, rs, s, b, f, n, r, t;

  code = c;
  mustBeEscaped = false;

  q = 34.0;
  rs = 92.0;
  s = 47.0;
  b = 8.0;
  f = 12.0;
  n = 10.0;
  r = 13.0;
  t = 9.0;

  if(code == q || code == rs || code == s || code == b || code == f || code == n || code == r || code == t){
    mustBeEscaped = true;
    letters->numberValue = 2.0;
  }else if(code >= 0.0 && code <= 31.0){
    mustBeEscaped = true;
    letters->numberValue = 6.0;
  }else if(code >= pow(2.0, 16.0)){
    mustBeEscaped = true;
    letters->numberValue = 6.0;
  }

  return mustBeEscaped;
}
void WriteObject(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  wchar_t *s, *key;
  size_t sLength, keyLength;
  double i;
  StringArrayReference *keys;
  Element *objectElement;

  strWriteStringToStingStream(result, resultLength, index, strparam(L"{"));

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArrayLength; i = i + 1.0){
    key = keys->stringArray[(int)(i)]->string;
    keyLength = keys->stringArray[(int)(i)]->stringLength;
    key = JSONEscapeString(&keyLength, key, keyLength);
    objectElement = GetObjectValue(element->object, key, keyLength);

    strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
    strWriteStringToStingStream(result, resultLength, index, key, keyLength);
    strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
    strWriteStringToStingStream(result, resultLength, index, strparam(L":"));

    s = WriteJSON(&sLength, objectElement);
    strWriteStringToStingStream(result, resultLength, index, s, sLength);

    if(i + 1.0 != keys->stringArrayLength){
      strWriteStringToStingStream(result, resultLength, index, strparam(L","));
    }
  }

  strWriteStringToStingStream(result, resultLength, index, strparam(L"}"));
}
_Bool ReadJSON(wchar_t *string, size_t stringLength, ElementReference *elementReference, StringArrayReference *errorMessages){
  StringArrayReference *tokenArrayReference;
  _Bool success;

  /* Tokenize. */
  tokenArrayReference = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  success = JSONTokenize(string, stringLength, tokenArrayReference, errorMessages);

  if(success){
    /* Parse. */
    success = GetJSONValue(tokenArrayReference->stringArray, tokenArrayReference->stringArrayLength, elementReference, errorMessages);
  }

  return success;
}
_Bool GetJSONValue(StringReference **tokens, size_t tokensLength, ElementReference *elementReference, StringArrayReference *errorMessages){
  NumberReference *i;
  _Bool success;

  i = CreateNumberReference(0.0);
  success = GetJSONValueRecursive(tokens, tokensLength, i, 0.0, elementReference, errorMessages);

  return success;
}
_Bool GetJSONValueRecursive(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  wchar_t *str, *substr, *token;
  size_t strLength, substrLength, tokenLength;
  double stringToDecimalResult;
  _Bool success;

  success = true;
  token = tokens[(int)(i->numberValue)]->string;
  tokenLength = tokens[(int)(i->numberValue)]->stringLength;

  if(StringsEqual(token, tokenLength, strparam(L"{"))){
    success = GetJSONObject(tokens, tokensLength, i, depth + 1.0, elementReference, errorMessages);
  }else if(StringsEqual(token, tokenLength, strparam(L"["))){
    success = GetJSONArray(tokens, tokensLength, i, depth + 1.0, elementReference, errorMessages);
  }else if(StringsEqual(token, tokenLength, strparam(L"true"))){
    elementReference->element = CreateBooleanElement(true);
    i->numberValue = i->numberValue + 1.0;
  }else if(StringsEqual(token, tokenLength, strparam(L"false"))){
    elementReference->element = CreateBooleanElement(false);
    i->numberValue = i->numberValue + 1.0;
  }else if(StringsEqual(token, tokenLength, strparam(L"null"))){
    elementReference->element = CreateNullElement();
    i->numberValue = i->numberValue + 1.0;
  }else if(charIsNumber(token[0]) || token[0] == '-'){
    stringToDecimalResult = nCreateNumberFromDecimalString(token, tokenLength);
    elementReference->element = CreateNumberElement(stringToDecimalResult);
    i->numberValue = i->numberValue + 1.0;
  }else if(token[0] == '\"'){
    substr = strSubstring(&substrLength, token, tokenLength, 1.0, tokenLength - 1.0);
    elementReference->element = CreateStringElement(substr, substrLength);
    i->numberValue = i->numberValue + 1.0;
  }else{
    str = L"";
    strLength = wcslen(str);
    str = strConcatenateString(&strLength, str, strLength, strparam(L"Invalid token first in value: "));
    str = strAppendString(&strLength, str, strLength, token, tokenLength);
    lAddStringRef(errorMessages, CreateStringReference(str, strLength));
    success = false;
  }

  if(success && depth == 0.0){
    if(StringsEqual(tokens[(int)(i->numberValue)]->string, tokens[(int)(i->numberValue)]->stringLength, strparam(L"<end>"))){
    }else{
      lAddStringRef(errorMessages, CreateStringReference(strparam(L"The outer value cannot have any tokens following it.")));
      success = false;
    }
  }

  return success;
}
_Bool GetJSONObject(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  Element *element, *value;
  _Bool done, success;
  wchar_t *key, *colon, *comma, *closeCurly;
  size_t keyLength, colonLength, commaLength, closeCurlyLength;
  wchar_t *keystring, *str;
  size_t keystringLength, strLength;
  ElementReference *valueReference;
  LinkedListElements *values;
  lLinkedListStrings *keys;

  keys = lCreateLinkedListString();
  values = CreateLinkedListElements();
  element = CreateObjectElement(0.0);
  valueReference = (ElementReference *)malloc(sizeof(ElementReference));
  success = true;
  i->numberValue = i->numberValue + 1.0;

  if( !StringsEqual(tokens[(int)(i->numberValue)]->string, tokens[(int)(i->numberValue)]->stringLength, strparam(L"}")) ){
    done = false;

    for(;  !done  && success; ){
      key = tokens[(int)(i->numberValue)]->string;
      keyLength = tokens[(int)(i->numberValue)]->stringLength;

      if(key[0] == '\"'){
        i->numberValue = i->numberValue + 1.0;
        colon = tokens[(int)(i->numberValue)]->string;
        colonLength = tokens[(int)(i->numberValue)]->stringLength;
        if(StringsEqual(colon, colonLength, strparam(L":"))){
          i->numberValue = i->numberValue + 1.0;
          success = GetJSONValueRecursive(tokens, tokensLength, i, depth, valueReference, errorMessages);

          if(success){
            keystring = strSubstring(&keystringLength, key, keyLength, 1.0, keyLength - 1.0);
            value = valueReference->element;
            lLinkedListAddString(keys, keystring, keystringLength);
            LinkedListAddElement(values, value);

            comma = tokens[(int)(i->numberValue)]->string;
            commaLength = tokens[(int)(i->numberValue)]->stringLength;
            if(StringsEqual(comma, commaLength, strparam(L","))){
              /* OK */
              i->numberValue = i->numberValue + 1.0;
            }else{
              done = true;
            }
          }
        }else{
          str = L"";
          strLength = wcslen(str);
          str = strConcatenateString(&strLength, str, strLength, strparam(L"Expected colon after key in object: "));
          str = strAppendString(&strLength, str, strLength, colon, colonLength);
          lAddStringRef(errorMessages, CreateStringReference(str, strLength));

          success = false;
          done = true;
        }
      }else{
        lAddStringRef(errorMessages, CreateStringReference(strparam(L"Expected string as key in object.")));

        done = true;
        success = false;
      }
    }
  }

  if(success){
    closeCurly = tokens[(int)(i->numberValue)]->string;
    closeCurlyLength = tokens[(int)(i->numberValue)]->stringLength;

    if(StringsEqual(closeCurly, closeCurlyLength, strparam(L"}"))){
      /* OK */
      free(element->object->stringListRef->stringArray);
      free(element->object->elementListRef->array);
      element->object->stringListRef->stringArray = lLinkedListStringsToArray(&element->object->stringListRef->stringArrayLength, keys);
      element->object->elementListRef->array = LinkedListElementsToArray(&element->object->elementListRef->arrayLength, values);
      elementReference->element = element;
      i->numberValue = i->numberValue + 1.0;
    }else{
      lAddStringRef(errorMessages, CreateStringReference(strparam(L"Expected close curly brackets at end of object value.")));
      success = false;
    }
  }

  lFreeLinkedListString(keys);
  FreeLinkedListElements(values);
  free(valueReference);

  return success;
}
_Bool GetJSONArray(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  Element *element, *value;
  wchar_t *nextToken, *comma;
  size_t nextTokenLength, commaLength;
  _Bool done, success;
  ElementReference *valueReference;
  LinkedListElements *elements;

  elements = CreateLinkedListElements();
  i->numberValue = i->numberValue + 1.0;

  valueReference = (ElementReference *)malloc(sizeof(ElementReference));
  success = true;
  element = CreateArrayElement(0.0);

  nextToken = tokens[(int)(i->numberValue)]->string;
  nextTokenLength = tokens[(int)(i->numberValue)]->stringLength;

  if( !StringsEqual(nextToken, nextTokenLength, strparam(L"]")) ){
    done = false;
    for(;  !done  && success; ){
      success = GetJSONValueRecursive(tokens, tokensLength, i, depth, valueReference, errorMessages);

      if(success){
        value = valueReference->element;
        LinkedListAddElement(elements, value);

        comma = tokens[(int)(i->numberValue)]->string;
        commaLength = tokens[(int)(i->numberValue)]->stringLength;

        if(StringsEqual(comma, commaLength, strparam(L","))){
          /* OK */
          i->numberValue = i->numberValue + 1.0;
        }else{
          done = true;
        }
      }
    }
  }

  nextToken = tokens[(int)(i->numberValue)]->string;
  nextTokenLength = tokens[(int)(i->numberValue)]->stringLength;
  if(StringsEqual(nextToken, nextTokenLength, strparam(L"]"))){
    /* OK */
    i->numberValue = i->numberValue + 1.0;
    free(element->array);
    element->array = LinkedListElementsToArray(&element->arrayLength, elements);
  }else{
    lAddStringRef(errorMessages, CreateStringReference(strparam(L"Expected close square bracket at end of array.")));
    success = false;
  }

  elementReference->element = element;
  FreeLinkedListElements(elements);
  free(valueReference);

  return success;
}
StringArrayReference *GetStringElementMapKeySet(StringElementMap *stringElementMap){
  return stringElementMap->stringListRef;
}
Element *GetObjectValue(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength){
  Element *result;
  double i;

  result = (Element *)malloc(sizeof(Element));

  for(i = 0.0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1.0){
    if(StringsEqual(stringElementMap->stringListRef->stringArray[(int)(i)]->string, stringElementMap->stringListRef->stringArray[(int)(i)]->stringLength, key, keyLength)){
      result = stringElementMap->elementListRef->array[(int)(i)];
    }
  }

  return result;
}
Element *GetObjectValueWithCheck(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength, BooleanReference *foundReference){
  Element *result;
  double i;

  result = (Element *)malloc(sizeof(Element));

  foundReference->booleanValue = false;
  for(i = 0.0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1.0){
    if(StringsEqual(stringElementMap->stringListRef->stringArray[(int)(i)]->string, stringElementMap->stringListRef->stringArray[(int)(i)]->stringLength, key, keyLength)){
      result = stringElementMap->elementListRef->array[(int)(i)];
      foundReference->booleanValue = true;
    }
  }

  return result;
}
void PutStringElementMap(StringElementMap *stringElementMap, wchar_t *keystring, size_t keystringLength, Element *value){
  lAddStringRef(stringElementMap->stringListRef, CreateStringReference(keystring, keystringLength));
  AddElementRef(stringElementMap->elementListRef, value);
}
void SetStringElementMap(StringElementMap *stringElementMap, double index, wchar_t *keystring, size_t keystringLength, Element *value){
  stringElementMap->stringListRef->stringArray[(int)(index)]->string = keystring;
  stringElementMap->stringListRef->stringArray[(int)(index)]->stringLength = keystringLength;
  stringElementMap->elementListRef->array[(int)(index)] = value;
}
double GetStringElementMapNumberOfKeys(StringElementMap *stringElementMap){
  return stringElementMap->stringListRef->stringArrayLength;
}
_Bool JSONCompare(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, double epsilon, BooleanReference *equal, StringArrayReference *errorMessage){
  _Bool success;
  ElementReference *eaRef, *ebRef;
  Element *ea, *eb;

  eaRef = (ElementReference *)malloc(sizeof(ElementReference));
  ebRef = (ElementReference *)malloc(sizeof(ElementReference));

  success = ReadJSON(a, aLength, eaRef, errorMessage);

  if(success){
    ea = eaRef->element;

    success = ReadJSON(b, bLength, ebRef, errorMessage);

    if(success){
      eb = ebRef->element;

      equal->booleanValue = JSONCompareElements(ea, eb, epsilon);

      DeleteElement(eb);
    }

    DeleteElement(ea);
  }

  return success;
}
_Bool JSONCompareElements(Element *ea, Element *eb, double epsilon){
  _Bool equal;
  wchar_t *typeName;
  size_t typeNameLength;

  equal = StringsEqual(ea->type, ea->typeLength, eb->type, eb->typeLength);
        
  if(equal){
    typeName = ea->type;
    typeNameLength = ea->typeLength;
    if(StringsEqual(typeName, typeNameLength, strparam(L"object"))){
      equal = JSONCompareObjects(ea->object, eb->object, epsilon);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"string"))){
      equal = StringsEqual(ea->string, ea->stringLength, eb->string, eb->stringLength);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"array"))){
      equal = JSONCompareArrays(ea->array, ea->arrayLength, eb->array, eb->arrayLength, epsilon);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"number"))){
      equal = EpsilonCompare(ea->number, eb->number, epsilon);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"null"))){
      equal = true;
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"boolean"))){
      equal = ea->booleanValue == eb->booleanValue;
    }
  }
        
  return equal;
}
_Bool JSONCompareArrays(Element **ea, size_t eaLength, Element **eb, size_t ebLength, double epsilon){
  _Bool equals;
  double i, length;

  equals = eaLength == ebLength;

  if(equals){
    length = eaLength;
    for(i = 0.0; i < length && equals; i = i + 1.0){
      equals = JSONCompareElements(ea[(int)(i)], eb[(int)(i)], epsilon);
    }
  }

  return equals;
}
_Bool JSONCompareObjects(StringElementMap *ea, StringElementMap *eb, double epsilon){
  _Bool equals;
  double akeys, bkeys, i;
  StringArrayReference *keys;
  wchar_t *key;
  size_t keyLength;
  BooleanReference *aFoundReference, *bFoundReference;
  Element *eaValue, *ebValue;

  aFoundReference = (BooleanReference *)malloc(sizeof(BooleanReference));
  bFoundReference = (BooleanReference *)malloc(sizeof(BooleanReference));

  akeys = GetStringElementMapNumberOfKeys(ea);
  bkeys = GetStringElementMapNumberOfKeys(eb);

  equals = akeys == bkeys;

  if(equals){
    keys = GetStringElementMapKeySet(ea);

    for(i = 0.0; i < keys->stringArrayLength && equals; i = i + 1.0){
      key = keys->stringArray[(int)(i)]->string;
      keyLength = keys->stringArray[(int)(i)]->stringLength;

      eaValue = GetObjectValueWithCheck(ea, key, keyLength, aFoundReference);
      ebValue = GetObjectValueWithCheck(eb, key, keyLength, bFoundReference);

      if(aFoundReference->booleanValue && bFoundReference->booleanValue){
        equals = JSONCompareElements(eaValue, ebValue, epsilon);
      }else{
        equals = false;
      }
    }
  }

  return equals;
}
double *AddNumber(size_t *returnArrayLength, double *list, size_t listLength, double a){
  double *newlist;
  size_t newlistLength;
  double i;

  newlist = (double*)malloc(sizeof(double) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddNumberRef(NumberArrayReference *list, double i){
  list->numberArray = AddNumber(&list->numberArrayLength, list->numberArray, list->numberArrayLength, i);
}
double *RemoveNumber(size_t *returnArrayLength, double *list, size_t listLength, double n){
  double *newlist;
  size_t newlistLength;
  double i;

  newlist = (double*)malloc(sizeof(double) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
double GetNumberRef(NumberArrayReference *list, double i){
  return list->numberArray[(int)(i)];
}
void RemoveNumberRef(NumberArrayReference *list, double i){
  list->numberArray = RemoveNumber(&list->numberArrayLength, list->numberArray, list->numberArrayLength, i);
}
StringReference **AddString(size_t *returnArrayLength, StringReference **list, size_t listLength, StringReference *a){
  StringReference **newlist;
  size_t newlistLength;
  double i;

  newlist = (StringReference**)malloc(sizeof(StringReference) * listLength + 1.0);
  newlistLength = listLength + 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddStringRef(StringArrayReference *list, StringReference *i){
  list->stringArray = AddString(&list->stringArrayLength, list->stringArray, list->stringArrayLength, i);
}
StringReference **RemoveString(size_t *returnArrayLength, StringReference **list, size_t listLength, double n){
  StringReference **newlist;
  size_t newlistLength;
  double i;

  newlist = (StringReference**)malloc(sizeof(StringReference) * listLength - 1.0);
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
StringReference *GetStringRef(StringArrayReference *list, double i){
  return list->stringArray[(int)(i)];
}
void RemoveStringRef(StringArrayReference *list, double i){
  list->stringArray = RemoveString(&list->stringArrayLength, list->stringArray, list->stringArrayLength, i);
}
DynamicArrayCharacters *CreateDynamicArrayCharacters(){
  DynamicArrayCharacters *da;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->array = (wchar_t*)malloc(sizeof(wchar_t) * (10.0));
  da->arrayLength = 10.0;
  da->length = 0.0;

  return da;
}
DynamicArrayCharacters *CreateDynamicArrayCharactersWithInitialCapacity(double capacity){
  DynamicArrayCharacters *da;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->array = (wchar_t*)malloc(sizeof(wchar_t) * (capacity));
  da->arrayLength = capacity;
  da->length = 0.0;

  return da;
}
void DynamicArrayAddCharacter(DynamicArrayCharacters *da, wchar_t value){
  if(da->length == da->arrayLength){
    DynamicArrayCharactersIncreaseSize(da);
  }

  da->array[(int)(da->length)] = value;
  da->length = da->length + 1.0;
}
void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters *da){
  double newLength, i;
  wchar_t *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*3.0/2.0);
  newArray = (wchar_t*)malloc(sizeof(wchar_t) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < da->arrayLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
_Bool DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters *da){
  _Bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->arrayLength*2.0/3.0);
  }

  return needsDecrease;
}
void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters *da){
  double newLength, i;
  wchar_t *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*2.0/3.0);
  newArray = (wchar_t*)malloc(sizeof(wchar_t) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < newLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
double DynamicArrayCharactersIndex(DynamicArrayCharacters *da, double index){
  return da->array[(int)(index)];
}
double DynamicArrayCharactersLength(DynamicArrayCharacters *da){
  return da->length;
}
void DynamicArrayInsertCharacter(DynamicArrayCharacters *da, double index, wchar_t value){
  double i;

  if(da->length == da->arrayLength){
    DynamicArrayCharactersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array[(int)(i)] = da->array[(int)(i - 1.0)];
  }

  da->array[(int)(index)] = value;

  da->length = da->length + 1.0;
}
_Bool DynamicArrayCharacterSet(DynamicArrayCharacters *da, double index, wchar_t value){
  _Bool success;

  if(index < da->length){
    da->array[(int)(index)] = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
void DynamicArrayRemoveCharacter(DynamicArrayCharacters *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array[(int)(i)] = da->array[(int)(i + 1.0)];
  }

  da->length = da->length - 1.0;

  if(DynamicArrayCharactersDecreaseSizeNecessary(da)){
    DynamicArrayCharactersDecreaseSize(da);
  }
}
void FreeDynamicArrayCharacters(DynamicArrayCharacters *da){
  free(da->array);
  free(da);
}
wchar_t *DynamicArrayCharactersToArray(size_t *returnArrayLength, DynamicArrayCharacters *da){
  wchar_t *array;
  size_t arrayLength;
  double i;

  array = (wchar_t*)malloc(sizeof(wchar_t) * (da->length));
  arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    array[(int)(i)] = da->array[(int)(i)];
  }

  *returnArrayLength = arrayLength;
  return array;
}
DynamicArrayCharacters *ArrayToDynamicArrayCharactersWithOptimalSize(wchar_t *array, size_t arrayLength){
  DynamicArrayCharacters *da;
  double i;
  double c, n, newCapacity;

  c = arrayLength;
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    da->array[(int)(i)] = array[(int)(i)];
  }

  return da;
}
DynamicArrayCharacters *ArrayToDynamicArrayCharacters(wchar_t *array, size_t arrayLength){
  DynamicArrayCharacters *da;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->array = CopyString(&da->arrayLength, array, arrayLength);
  da->length = arrayLength;

  return da;
}
_Bool DynamicArrayCharactersEqual(DynamicArrayCharacters *a, DynamicArrayCharacters *b){
  _Bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array[(int)(i)] != b->array[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
LinkedListCharacters *DynamicArrayCharactersToLinkedList(DynamicArrayCharacters *da){
  LinkedListCharacters *ll;
  double i;

  ll = CreateLinkedListCharacter();

  for(i = 0.0; i < da->length; i = i + 1.0){
    LinkedListAddCharacter(ll, da->array[(int)(i)]);
  }

  return ll;
}
DynamicArrayCharacters *LinkedListToDynamicArrayCharacters(LinkedListCharacters *ll){
  DynamicArrayCharacters *da;
  double i;
  LinkedListNodeCharacters *node;

  node = ll->first;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->length = LinkedListCharactersLength(ll);

  da->array = (wchar_t*)malloc(sizeof(wchar_t) * (da->length));
  da->arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array[(int)(i)] = node->value;
    node = node->next;
  }

  return da;
}
_Bool *AddBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, _Bool a){
  _Bool *newlist;
  size_t newlistLength;
  double i;

  newlist = (_Bool*)malloc(sizeof(_Bool) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddBooleanRef(BooleanArrayReference *list, _Bool i){
  list->booleanArray = AddBoolean(&list->booleanArrayLength, list->booleanArray, list->booleanArrayLength, i);
}
_Bool *RemoveBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, double n){
  _Bool *newlist;
  size_t newlistLength;
  double i;

  newlist = (_Bool*)malloc(sizeof(_Bool) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
_Bool GetBooleanRef(BooleanArrayReference *list, double i){
  return list->booleanArray[(int)(i)];
}
void RemoveDecimalRef(BooleanArrayReference *list, double i){
  list->booleanArray = RemoveBoolean(&list->booleanArrayLength, list->booleanArray, list->booleanArrayLength, i);
}
LinkedListStrings *CreateLinkedListString(){
  LinkedListStrings *ll;

  ll = (LinkedListStrings *)malloc(sizeof(LinkedListStrings));
  ll->first = (LinkedListNodeStrings *)malloc(sizeof(LinkedListNodeStrings));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddString(LinkedListStrings *ll, wchar_t *value, size_t valueLength){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->valueLength = valueLength;
  ll->last->next = (LinkedListNodeStrings *)malloc(sizeof(LinkedListNodeStrings));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
StringReference **LinkedListStringsToArray(size_t *returnArrayLength, LinkedListStrings *ll){
  StringReference **array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeStrings *node;

  node = ll->first;

  length = LinkedListStringsLength(ll);

  array = (StringReference**)malloc(sizeof(StringReference) * length);
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = (StringReference *)malloc(sizeof(StringReference));
    array[(int)(i)]->string = node->value;
    array[(int)(i)]->stringLength = node->valueLength;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListStringsLength(LinkedListStrings *ll){
  double l;
  LinkedListNodeStrings *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListString(LinkedListStrings *ll){
  LinkedListNodeStrings *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
LinkedListNumbers *CreateLinkedListNumbers(){
  LinkedListNumbers *ll;

  ll = (LinkedListNumbers *)malloc(sizeof(LinkedListNumbers));
  ll->first = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
LinkedListNumbers **CreateLinkedListNumbersArray(size_t *returnArrayLength, double length){
  LinkedListNumbers **lls;
  size_t llsLength;
  double i;

  lls = (LinkedListNumbers**)malloc(sizeof(LinkedListNumbers) * length);
  llsLength = length;
  for(i = 0.0; i < llsLength; i = i + 1.0){
    lls[(int)(i)] = CreateLinkedListNumbers();
  }

  *returnArrayLength = llsLength;
  return lls;
}
void LinkedListAddNumber(LinkedListNumbers *ll, double value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
double LinkedListNumbersLength(LinkedListNumbers *ll){
  double l;
  LinkedListNodeNumbers *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
double LinkedListNumbersIndex(LinkedListNumbers *ll, double index){
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;
  for(i = 0.0; i < index; i = i + 1.0){
    node = node->next;
  }

  return node->value;
}
void LinkedListInsertNumber(LinkedListNumbers *ll, double index, double value){
  double i;
  LinkedListNodeNumbers *node, *tmp;

  if(index == 0.0){
    tmp = ll->first;
    ll->first = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
    ll->first->next = tmp;
    ll->first->value = value;
    ll->first->end = false;
  }else{
    node = ll->first;
    for(i = 0.0; i < index - 1.0; i = i + 1.0){
      node = node->next;
    }

    tmp = node->next;
    node->next = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
    node->next->next = tmp;
    node->next->value = value;
    node->next->end = false;
  }
}
void LinkedListSet(LinkedListNumbers *ll, double index, double value){
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;
  for(i = 0.0; i < index; i = i + 1.0){
    node = node->next;
  }

  node->next->value = value;
}
void LinkedListRemoveNumber(LinkedListNumbers *ll, double index){
  double i;
  LinkedListNodeNumbers *node, *prev;

  node = ll->first;
  prev = ll->first;

  for(i = 0.0; i < index; i = i + 1.0){
    prev = node;
    node = node->next;
  }

  if(index == 0.0){
    ll->first = prev->next;
  }
  if( !prev->next->end ){
    prev->next = prev->next->next;
  }
}
void FreeLinkedListNumbers(LinkedListNumbers *ll){
  LinkedListNodeNumbers *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
void FreeLinkedListNumbersArray(LinkedListNumbers **lls, size_t llsLength){
  double i;

  for(i = 0.0; i < llsLength; i = i + 1.0){
    FreeLinkedListNumbers(lls[(int)(i)]);
  }
  free(lls);
}
double *LinkedListNumbersToArray(size_t *returnArrayLength, LinkedListNumbers *ll){
  double *array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeNumbers *node;

  node = ll->first;

  length = LinkedListNumbersLength(ll);

  array = (double*)malloc(sizeof(double) * (length));
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
LinkedListNumbers *ArrayToLinkedListNumbers(double *array, size_t arrayLength){
  LinkedListNumbers *ll;
  double i;

  ll = CreateLinkedListNumbers();

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    LinkedListAddNumber(ll, array[(int)(i)]);
  }

  return ll;
}
_Bool LinkedListNumbersEqual(LinkedListNumbers *a, LinkedListNumbers *b){
  _Bool equal, done;
  LinkedListNodeNumbers *an, *bn;

  an = a->first;
  bn = b->first;

  equal = true;
  done = false;
  for(; equal &&  !done ; ){
    if(an->end == bn->end){
      if(an->end){
        done = true;
      }else if(an->value == bn->value){
        an = an->next;
        bn = bn->next;
      }else{
        equal = false;
      }
    }else{
      equal = false;
    }
  }

  return equal;
}
LinkedListCharacters *CreateLinkedListCharacter(){
  LinkedListCharacters *ll;

  ll = (LinkedListCharacters *)malloc(sizeof(LinkedListCharacters));
  ll->first = (LinkedListNodeCharacters *)malloc(sizeof(LinkedListNodeCharacters));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddCharacter(LinkedListCharacters *ll, wchar_t value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeCharacters *)malloc(sizeof(LinkedListNodeCharacters));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
wchar_t *LinkedListCharactersToArray(size_t *returnArrayLength, LinkedListCharacters *ll){
  wchar_t *array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeCharacters *node;

  node = ll->first;

  length = LinkedListCharactersLength(ll);

  array = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListCharactersLength(LinkedListCharacters *ll){
  double l;
  LinkedListNodeCharacters *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListCharacter(LinkedListCharacters *ll){
  LinkedListNodeCharacters *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
void LinkedListCharactersAddString(LinkedListCharacters *ll, wchar_t *str, size_t strLength){
  double i;

  for(i = 0.0; i < strLength; i = i + 1.0){
    LinkedListAddCharacter(ll, str[(int)(i)]);
  }
}
DynamicArrayNumbers *CreateDynamicArrayNumbers(){
  DynamicArrayNumbers *da;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->array = (double*)malloc(sizeof(double) * (10.0));
  da->arrayLength = 10.0;
  da->length = 0.0;

  return da;
}
DynamicArrayNumbers *CreateDynamicArrayNumbersWithInitialCapacity(double capacity){
  DynamicArrayNumbers *da;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->array = (double*)malloc(sizeof(double) * (capacity));
  da->arrayLength = capacity;
  da->length = 0.0;

  return da;
}
void DynamicArrayAddNumber(DynamicArrayNumbers *da, double value){
  if(da->length == da->arrayLength){
    DynamicArrayNumbersIncreaseSize(da);
  }

  da->array[(int)(da->length)] = value;
  da->length = da->length + 1.0;
}
void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers *da){
  double newLength, i;
  double *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*3.0/2.0);
  newArray = (double*)malloc(sizeof(double) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < da->arrayLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
_Bool DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers *da){
  _Bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->arrayLength*2.0/3.0);
  }

  return needsDecrease;
}
void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers *da){
  double newLength, i;
  double *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*2.0/3.0);
  newArray = (double*)malloc(sizeof(double) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < newLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
double DynamicArrayNumbersIndex(DynamicArrayNumbers *da, double index){
  return da->array[(int)(index)];
}
double DynamicArrayNumbersLength(DynamicArrayNumbers *da){
  return da->length;
}
void DynamicArrayInsertNumber(DynamicArrayNumbers *da, double index, double value){
  double i;

  if(da->length == da->arrayLength){
    DynamicArrayNumbersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array[(int)(i)] = da->array[(int)(i - 1.0)];
  }

  da->array[(int)(index)] = value;

  da->length = da->length + 1.0;
}
_Bool DynamicArrayNumberSet(DynamicArrayNumbers *da, double index, double value){
  _Bool success;

  if(index < da->length){
    da->array[(int)(index)] = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
void DynamicArrayRemoveNumber(DynamicArrayNumbers *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array[(int)(i)] = da->array[(int)(i + 1.0)];
  }

  da->length = da->length - 1.0;

  if(DynamicArrayNumbersDecreaseSizeNecessary(da)){
    DynamicArrayNumbersDecreaseSize(da);
  }
}
void FreeDynamicArrayNumbers(DynamicArrayNumbers *da){
  free(da->array);
  free(da);
}
double *DynamicArrayNumbersToArray(size_t *returnArrayLength, DynamicArrayNumbers *da){
  double *array;
  size_t arrayLength;
  double i;

  array = (double*)malloc(sizeof(double) * (da->length));
  arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    array[(int)(i)] = da->array[(int)(i)];
  }

  *returnArrayLength = arrayLength;
  return array;
}
DynamicArrayNumbers *ArrayToDynamicArrayNumbersWithOptimalSize(double *array, size_t arrayLength){
  DynamicArrayNumbers *da;
  double i;
  double c, n, newCapacity;

  /*
         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
         */
  c = arrayLength;
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    da->array[(int)(i)] = array[(int)(i)];
  }

  return da;
}
DynamicArrayNumbers *ArrayToDynamicArrayNumbers(double *array, size_t arrayLength){
  DynamicArrayNumbers *da;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->array = CopyNumberArray(&da->arrayLength, array, arrayLength);
  da->length = arrayLength;

  return da;
}
_Bool DynamicArrayNumbersEqual(DynamicArrayNumbers *a, DynamicArrayNumbers *b){
  _Bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array[(int)(i)] != b->array[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
LinkedListNumbers *DynamicArrayNumbersToLinkedList(DynamicArrayNumbers *da){
  LinkedListNumbers *ll;
  double i;

  ll = CreateLinkedListNumbers();

  for(i = 0.0; i < da->length; i = i + 1.0){
    LinkedListAddNumber(ll, da->array[(int)(i)]);
  }

  return ll;
}
DynamicArrayNumbers *LinkedListToDynamicArrayNumbers(LinkedListNumbers *ll){
  DynamicArrayNumbers *da;
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->length = LinkedListNumbersLength(ll);

  da->array = (double*)malloc(sizeof(double) * (da->length));
  da->arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array[(int)(i)] = node->value;
    node = node->next;
  }

  return da;
}
wchar_t *AddCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, wchar_t a){
  wchar_t *newlist;
  size_t newlistLength;
  double i;

  newlist = (wchar_t*)malloc(sizeof(wchar_t) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddCharacterRef(StringReference *list, wchar_t i){
  list->string = AddCharacter(&list->stringLength, list->string, list->stringLength, i);
}
wchar_t *RemoveCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, double n){
  wchar_t *newlist;
  size_t newlistLength;
  double i;

  newlist = (wchar_t*)malloc(sizeof(wchar_t) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }

  *returnArrayLength = newlistLength;
  return newlist;
}
wchar_t GetCharacterRef(StringReference *list, double i){
  return list->string[(int)(i)];
}
void RemoveCharacterRef(StringReference *list, double i){
  list->string = RemoveCharacter(&list->stringLength, list->string, list->stringLength, i);
}
void sWriteStringToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t *src, size_t srcLength){
  double i;

  for(i = 0.0; i < srcLength; i = i + 1.0){
    stream[(int)(index->numberValue + i)] = src[(int)(i)];
  }
  index->numberValue = index->numberValue + srcLength;
}
void sWriteCharacterToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t src){
  stream[(int)(index->numberValue)] = src;
  index->numberValue = index->numberValue + 1.0;
}
void sWriteBooleanToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, _Bool src){
  if(src){
    sWriteStringToStingStream(stream, streamLength, index, strparam(L"true"));
  }else{
    sWriteStringToStingStream(stream, streamLength, index, strparam(L"false"));
  }
}
_Bool sSubstringWithCheck(wchar_t *string, size_t stringLength, double from, double to, StringReference *stringReference){
  _Bool success;

  if(from >= 0.0 && from <= stringLength && to >= 0.0 && to <= stringLength && from <= to){
    stringReference->string = sSubstring(&stringReference->stringLength, string, stringLength, from, to);
    success = true;
  }else{
    success = false;
  }

  return success;
}
wchar_t *sSubstring(size_t *returnArrayLength, wchar_t *string, size_t stringLength, double from, double to){
  wchar_t *n;
  size_t nLength;
  double i, length;

  length = to - from;

  n = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  nLength = length;

  for(i = from; i < to; i = i + 1.0){
    n[(int)(i - from)] = string[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
wchar_t *sAppendString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length){
  wchar_t *newString;
  size_t newStringLength;

  newString = sConcatenateString(&newStringLength, s1, s1Length, s2, s2Length);

  free(s1);

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *sConcatenateString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length){
  wchar_t *newString;
  size_t newStringLength;
  double i;

  newString = (wchar_t*)malloc(sizeof(wchar_t) * (s1Length + s2Length));
  newStringLength = s1Length + s2Length;

  for(i = 0.0; i < s1Length; i = i + 1.0){
    newString[(int)(i)] = s1[(int)(i)];
  }

  for(i = 0.0; i < s2Length; i = i + 1.0){
    newString[(int)(s1Length + i)] = s2[(int)(i)];
  }

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *sAppendCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c){
  wchar_t *newString;
  size_t newStringLength;

  newString = sConcatenateCharacter(&newStringLength, string, stringLength, c);

  free(string);

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *sConcatenateCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c){
  wchar_t *newString;
  size_t newStringLength;
  double i;
  newString = (wchar_t*)malloc(sizeof(wchar_t) * (stringLength + 1.0));
  newStringLength = stringLength + 1.0;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    newString[(int)(i)] = string[(int)(i)];
  }

  newString[(int)(stringLength)] = c;

  *returnArrayLength = newStringLength;
  return newString;
}
StringReference **sSplitByCharacter(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t splitBy){
  StringReference **split;
  size_t splitLength;
  wchar_t *stringToSplitBy;
  size_t stringToSplitByLength;

  stringToSplitBy = (wchar_t*)malloc(sizeof(wchar_t) * (1.0));
  stringToSplitByLength = 1.0;
  stringToSplitBy[0] = splitBy;

  split = sSplitByString(&splitLength, toSplit, toSplitLength, stringToSplitBy, stringToSplitByLength);

  free(stringToSplitBy);

  *returnArrayLength = splitLength;
  return split;
}
_Bool sIndexOfCharacter(wchar_t *string, size_t stringLength, wchar_t character, NumberReference *indexReference){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength &&  !found ; i = i + 1.0){
    if(string[(int)(i)] == character){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
_Bool sSubstringEqualsWithCheck(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength, BooleanReference *equalsReference){
  _Bool success;

  if(from < stringLength){
    success = true;
    equalsReference->booleanValue = sSubstringEquals(string, stringLength, from, substring, substringLength);
  }else{
    success = false;
  }

  return success;
}
_Bool sSubstringEquals(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength){
  double i;
  _Bool equal;

  equal = true;
  if(stringLength - from >= substringLength){
    for(i = 0.0; i < substringLength && equal; i = i + 1.0){
      if(string[(int)(from + i)] != substring[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool sIndexOfString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength, NumberReference *indexReference){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength - substringLength + 1.0 &&  !found ; i = i + 1.0){
    if(sSubstringEquals(string, stringLength, i, substring, substringLength)){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
_Bool sContainsCharacter(wchar_t *string, size_t stringLength, wchar_t character){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength &&  !found ; i = i + 1.0){
    if(string[(int)(i)] == character){
      found = true;
    }
  }

  return found;
}
_Bool sContainsString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength){
  return sIndexOfString(string, stringLength, substring, substringLength, (NumberReference *)malloc(sizeof(NumberReference)));
}
void sToUpperCase(wchar_t *string, size_t stringLength){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    string[(int)(i)] = charToUpperCase(string[(int)(i)]);
  }
}
void sToLowerCase(wchar_t *string, size_t stringLength){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    string[(int)(i)] = charToLowerCase(string[(int)(i)]);
  }
}
_Bool sEqualsIgnoreCase(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool equal;
  double i;

  if(aLength == bLength){
    equal = true;
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(charToLowerCase(a[(int)(i)]) != charToLowerCase(b[(int)(i)])){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
wchar_t *sReplaceString(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t *toReplace, size_t toReplaceLength, wchar_t *replaceWith, size_t replaceWithLength){
  wchar_t *result;
  size_t resultLength;
  double i, j;
  BooleanReference *equalsReference;
  _Bool success;
  DynamicArrayCharacters *da;

  da = CreateDynamicArrayCharacters();

  equalsReference = (BooleanReference *)malloc(sizeof(BooleanReference));

  for(i = 0.0; i < stringLength; ){
    success = sSubstringEqualsWithCheck(string, stringLength, i, toReplace, toReplaceLength, equalsReference);
    if(success){
      success = equalsReference->booleanValue;
    }

    if(success && toReplaceLength > 0.0){
      for(j = 0.0; j < replaceWithLength; j = j + 1.0){
        DynamicArrayAddCharacter(da, replaceWith[(int)(j)]);
      }
      i = i + toReplaceLength;
    }else{
      DynamicArrayAddCharacter(da, string[(int)(i)]);
      i = i + 1.0;
    }
  }

  result = DynamicArrayCharactersToArray(&resultLength, da);

  FreeDynamicArrayCharacters(da);

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *sReplaceCharacterToNew(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith){
  wchar_t *result;
  size_t resultLength;
  double i;

  result = (wchar_t*)malloc(sizeof(wchar_t) * (stringLength));
  resultLength = stringLength;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(string[(int)(i)] == toReplace){
      result[(int)(i)] = replaceWith;
    }else{
      result[(int)(i)] = string[(int)(i)];
    }
  }

  *returnArrayLength = resultLength;
  return result;
}
void sReplaceCharacter(wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(string[(int)(i)] == toReplace){
      string[(int)(i)] = replaceWith;
    }
  }
}
wchar_t *sTrim(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  wchar_t *result;
  size_t resultLength;
  double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
  _Bool firstNonWhitespaceFound;

  /* Find whitepaces at the start. */
  lastWhitespaceLocationStart =  -1.0;
  firstNonWhitespaceFound = false;
  for(i = 0.0; i < stringLength &&  !firstNonWhitespaceFound ; i = i + 1.0){
    if(charIsWhiteSpace(string[(int)(i)])){
      lastWhitespaceLocationStart = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  lastWhitespaceLocationEnd = stringLength;
  firstNonWhitespaceFound = false;
  for(i = stringLength - 1.0; i >= 0.0 &&  !firstNonWhitespaceFound ; i = i - 1.0){
    if(charIsWhiteSpace(string[(int)(i)])){
      lastWhitespaceLocationEnd = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
    result = sSubstring(&resultLength, string, stringLength, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd);
  }else{
    result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
    resultLength = 0.0;
  }

  *returnArrayLength = resultLength;
  return result;
}
_Bool sStartsWith(wchar_t *string, size_t stringLength, wchar_t *start, size_t startLength){
  _Bool startsWithString;

  startsWithString = false;
  if(stringLength >= startLength){
    startsWithString = sSubstringEquals(string, stringLength, 0.0, start, startLength);
  }

  return startsWithString;
}
_Bool sEndsWith(wchar_t *string, size_t stringLength, wchar_t *end, size_t endLength){
  _Bool endsWithString;

  endsWithString = false;
  if(stringLength >= endLength){
    endsWithString = sSubstringEquals(string, stringLength, stringLength - endLength, end, endLength);
  }

  return endsWithString;
}
StringReference **sSplitByString(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t *splitBy, size_t splitByLength){
  StringReference **split;
  size_t splitLength;
  wchar_t *next;
  size_t nextLength;
  double i;
  wchar_t c;
  StringReference *n;

  split = (StringReference**)malloc(sizeof(StringReference) * 0.0);
  splitLength = 0.0;

  next = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  nextLength = 0.0;
  for(i = 0.0; i < toSplitLength; ){
    c = toSplit[(int)(i)];

    if(sSubstringEquals(toSplit, toSplitLength, i, splitBy, splitByLength)){
      n = (StringReference *)malloc(sizeof(StringReference));
      n->string = next;
      n->stringLength = nextLength;
      split = AddString(&splitLength, split, splitLength, n);
      next = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
      nextLength = 0.0;
      i = i + splitByLength;
    }else{
      next = sAppendCharacter(&nextLength, next, nextLength, c);
      i = i + 1.0;
    }
  }

  n = (StringReference *)malloc(sizeof(StringReference));
  n->string = next;
  n->stringLength = nextLength;
  split = AddString(&splitLength, split, splitLength, n);

  *returnArrayLength = splitLength;
  return split;
}
_Bool sStringIsBefore(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool before, equal, done;
  double i;

  before = false;
  equal = true;
  done = false;

  if(aLength == 0.0 && bLength > 0.0){
    before = true;
  }else{
    for(i = 0.0; i < aLength && i < bLength &&  !done ; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
      if(charCharacterIsBefore(a[(int)(i)], b[(int)(i)])){
        before = true;
      }
      if(charCharacterIsBefore(b[(int)(i)], a[(int)(i)])){
        done = true;
      }
    }

    if(equal){
      if(aLength < bLength){
        before = true;
      }
    }
  }

  return before;
}
void strWriteStringToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t *src, size_t srcLength){
  double i;

  for(i = 0.0; i < srcLength; i = i + 1.0){
    stream[(int)(index->numberValue + i)] = src[(int)(i)];
  }
  index->numberValue = index->numberValue + srcLength;
}
void strWriteCharacterToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t src){
  stream[(int)(index->numberValue)] = src;
  index->numberValue = index->numberValue + 1.0;
}
void strWriteBooleanToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, _Bool src){
  if(src){
    strWriteStringToStingStream(stream, streamLength, index, strparam(L"true"));
  }else{
    strWriteStringToStingStream(stream, streamLength, index, strparam(L"false"));
  }
}
_Bool strSubstringWithCheck(wchar_t *string, size_t stringLength, double from, double to, StringReference *stringReference){
  _Bool success;

  if(from >= 0.0 && from <= stringLength && to >= 0.0 && to <= stringLength && from <= to){
    stringReference->string = strSubstring(&stringReference->stringLength, string, stringLength, from, to);
    success = true;
  }else{
    success = false;
  }

  return success;
}
wchar_t *strSubstring(size_t *returnArrayLength, wchar_t *string, size_t stringLength, double from, double to){
  wchar_t *n;
  size_t nLength;
  double i, length;

  length = to - from;

  n = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  nLength = length;

  for(i = from; i < to; i = i + 1.0){
    n[(int)(i - from)] = string[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
wchar_t *strAppendString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length){
  wchar_t *newString;
  size_t newStringLength;

  newString = strConcatenateString(&newStringLength, s1, s1Length, s2, s2Length);

  free(s1);

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *strConcatenateString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length){
  wchar_t *newString;
  size_t newStringLength;
  double i;

  newString = (wchar_t*)malloc(sizeof(wchar_t) * (s1Length + s2Length));
  newStringLength = s1Length + s2Length;

  for(i = 0.0; i < s1Length; i = i + 1.0){
    newString[(int)(i)] = s1[(int)(i)];
  }

  for(i = 0.0; i < s2Length; i = i + 1.0){
    newString[(int)(s1Length + i)] = s2[(int)(i)];
  }

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *strAppendCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c){
  wchar_t *newString;
  size_t newStringLength;

  newString = strConcatenateCharacter(&newStringLength, string, stringLength, c);

  free(string);

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *strConcatenateCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c){
  wchar_t *newString;
  size_t newStringLength;
  double i;
  newString = (wchar_t*)malloc(sizeof(wchar_t) * (stringLength + 1.0));
  newStringLength = stringLength + 1.0;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    newString[(int)(i)] = string[(int)(i)];
  }

  newString[(int)(stringLength)] = c;

  *returnArrayLength = newStringLength;
  return newString;
}
StringReference **strSplitByCharacter(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t splitBy){
  StringReference **split;
  size_t splitLength;
  wchar_t *stringToSplitBy;
  size_t stringToSplitByLength;

  stringToSplitBy = (wchar_t*)malloc(sizeof(wchar_t) * (1.0));
  stringToSplitByLength = 1.0;
  stringToSplitBy[0] = splitBy;

  split = strSplitByString(&splitLength, toSplit, toSplitLength, stringToSplitBy, stringToSplitByLength);

  free(stringToSplitBy);

  *returnArrayLength = splitLength;
  return split;
}
_Bool strIndexOfCharacter(wchar_t *string, size_t stringLength, wchar_t character, NumberReference *indexReference){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength &&  !found ; i = i + 1.0){
    if(string[(int)(i)] == character){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
_Bool strSubstringEqualsWithCheck(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength, BooleanReference *equalsReference){
  _Bool success;

  if(from < stringLength){
    success = true;
    equalsReference->booleanValue = strSubstringEquals(string, stringLength, from, substring, substringLength);
  }else{
    success = false;
  }

  return success;
}
_Bool strSubstringEquals(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength){
  double i;
  _Bool equal;

  equal = true;
  for(i = 0.0; i < substringLength && equal; i = i + 1.0){
    if(string[(int)(from + i)] != substring[(int)(i)]){
      equal = false;
    }
  }

  return equal;
}
_Bool strIndexOfString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength, NumberReference *indexReference){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength - substringLength + 1.0 &&  !found ; i = i + 1.0){
    if(strSubstringEquals(string, stringLength, i, substring, substringLength)){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
_Bool strContainsCharacter(wchar_t *string, size_t stringLength, wchar_t character){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength &&  !found ; i = i + 1.0){
    if(string[(int)(i)] == character){
      found = true;
    }
  }

  return found;
}
_Bool strContainsString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength){
  return strIndexOfString(string, stringLength, substring, substringLength, (NumberReference *)malloc(sizeof(NumberReference)));
}
void strToUpperCase(wchar_t *string, size_t stringLength){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    string[(int)(i)] = charToUpperCase(string[(int)(i)]);
  }
}
void strToLowerCase(wchar_t *string, size_t stringLength){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    string[(int)(i)] = charToLowerCase(string[(int)(i)]);
  }
}
_Bool strEqualsIgnoreCase(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool equal;
  double i;

  if(aLength == bLength){
    equal = true;
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(charToLowerCase(a[(int)(i)]) != charToLowerCase(b[(int)(i)])){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
wchar_t *strReplaceString(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t *toReplace, size_t toReplaceLength, wchar_t *replaceWith, size_t replaceWithLength){
  wchar_t *result;
  size_t resultLength;
  double i;
  BooleanReference *equalsReference;
  _Bool success;

  equalsReference = (BooleanReference *)malloc(sizeof(BooleanReference));
  result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  resultLength = 0.0;

  for(i = 0.0; i < stringLength; ){
    success = strSubstringEqualsWithCheck(string, stringLength, i, toReplace, toReplaceLength, equalsReference);
    if(success){
      success = equalsReference->booleanValue;
    }

    if(success && toReplaceLength > 0.0){
      result = strConcatenateString(&resultLength, result, resultLength, replaceWith, replaceWithLength);
      i = i + toReplaceLength;
    }else{
      result = strConcatenateCharacter(&resultLength, result, resultLength, string[(int)(i)]);
      i = i + 1.0;
    }
  }

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *strReplaceCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith){
  wchar_t *result;
  size_t resultLength;
  double i;

  result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  resultLength = 0.0;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(string[(int)(i)] == toReplace){
      result = strConcatenateCharacter(&resultLength, result, resultLength, replaceWith);
    }else{
      result = strConcatenateCharacter(&resultLength, result, resultLength, string[(int)(i)]);
    }
  }

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *strTrim(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  wchar_t *result;
  size_t resultLength;
  double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
  _Bool firstNonWhitespaceFound;

  /* Find whitepaces at the start. */
  lastWhitespaceLocationStart =  -1.0;
  firstNonWhitespaceFound = false;
  for(i = 0.0; i < stringLength &&  !firstNonWhitespaceFound ; i = i + 1.0){
    if(charIsWhiteSpace(string[(int)(i)])){
      lastWhitespaceLocationStart = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  lastWhitespaceLocationEnd = stringLength;
  firstNonWhitespaceFound = false;
  for(i = stringLength - 1.0; i >= 0.0 &&  !firstNonWhitespaceFound ; i = i - 1.0){
    if(charIsWhiteSpace(string[(int)(i)])){
      lastWhitespaceLocationEnd = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
    result = strSubstring(&resultLength, string, stringLength, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd);
  }else{
    result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
    resultLength = 0.0;
  }

  *returnArrayLength = resultLength;
  return result;
}
_Bool strStartsWith(wchar_t *string, size_t stringLength, wchar_t *start, size_t startLength){
  _Bool startsWithString;

  startsWithString = false;
  if(stringLength >= startLength){
    startsWithString = strSubstringEquals(string, stringLength, 0.0, start, startLength);
  }

  return startsWithString;
}
_Bool strEndsWith(wchar_t *string, size_t stringLength, wchar_t *end, size_t endLength){
  _Bool endsWithString;

  endsWithString = false;
  if(stringLength >= endLength){
    endsWithString = strSubstringEquals(string, stringLength, stringLength - endLength, end, endLength);
  }

  return endsWithString;
}
StringReference **strSplitByString(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t *splitBy, size_t splitByLength){
  StringReference **split;
  size_t splitLength;
  wchar_t *next;
  size_t nextLength;
  double i;
  wchar_t c;
  StringReference *n;

  split = (StringReference**)malloc(sizeof(StringReference) * 0.0);
  splitLength = 0.0;

  next = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  nextLength = 0.0;
  for(i = 0.0; i < toSplitLength; ){
    c = toSplit[(int)(i)];

    if(strSubstringEquals(toSplit, toSplitLength, i, splitBy, splitByLength)){
      if(splitLength != 0.0 || i != 0.0){
        n = (StringReference *)malloc(sizeof(StringReference));
        n->string = next;
        n->stringLength = nextLength;
        split = lAddString(&splitLength, split, splitLength, n);
        next = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
        nextLength = 0.0;
        i = i + splitByLength;
      }
    }else{
      next = strAppendCharacter(&nextLength, next, nextLength, c);
      i = i + 1.0;
    }
  }

  if(nextLength > 0.0){
    n = (StringReference *)malloc(sizeof(StringReference));
    n->string = next;
    n->stringLength = nextLength;
    split = lAddString(&splitLength, split, splitLength, n);
  }

  *returnArrayLength = splitLength;
  return split;
}
_Bool strStringIsBefore(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool before, equal, done;
  double i;

  before = false;
  equal = true;
  done = false;

  if(aLength == 0.0 && bLength > 0.0){
    before = true;
  }else{
    for(i = 0.0; i < aLength && i < bLength &&  !done ; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
      if(charCharacterIsBefore(a[(int)(i)], b[(int)(i)])){
        before = true;
      }
      if(charCharacterIsBefore(b[(int)(i)], a[(int)(i)])){
        done = true;
      }
    }

    if(equal){
      if(aLength < bLength){
        before = true;
      }
    }
  }

  return before;
}
double *StringToNumberArray(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  double i;
  double *array;
  size_t arrayLength;

  array = (double*)malloc(sizeof(double) * (stringLength));
  arrayLength = stringLength;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    array[(int)(i)] = string[(int)(i)];
  }
  *returnArrayLength = arrayLength;
  return array;
}
wchar_t *NumberArrayToString(size_t *returnArrayLength, double *array, size_t arrayLength){
  double i;
  wchar_t *string;
  size_t stringLength;

  string = (wchar_t*)malloc(sizeof(wchar_t) * (arrayLength));
  stringLength = arrayLength;

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    string[(int)(i)] = array[(int)(i)];
  }
  *returnArrayLength = stringLength;
  return string;
}
_Bool NumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength){
  _Bool equal;
  double i;

  equal = true;
  if(aLength == bLength){
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool BooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength){
  _Bool equal;
  double i;

  equal = true;
  if(aLength == bLength){
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool StringsEqual(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool equal;
  double i;

  equal = true;
  if(aLength == bLength){
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
void FillNumberArray(double *a, size_t aLength, double value){
  double i;

  for(i = 0.0; i < aLength; i = i + 1.0){
    a[(int)(i)] = value;
  }
}
void FillString(wchar_t *a, size_t aLength, wchar_t value){
  double i;

  for(i = 0.0; i < aLength; i = i + 1.0){
    a[(int)(i)] = value;
  }
}
void FillBooleanArray(_Bool *a, size_t aLength, _Bool value){
  double i;

  for(i = 0.0; i < aLength; i = i + 1.0){
    a[(int)(i)] = value;
  }
}
_Bool FillNumberArrayRange(double *a, size_t aLength, double value, double from, double to){
  double i, length;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a[(int)(from + i)] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool FillBooleanArrayRange(_Bool *a, size_t aLength, _Bool value, double from, double to){
  double i, length;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a[(int)(from + i)] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool FillStringRange(wchar_t *a, size_t aLength, wchar_t value, double from, double to){
  double i, length;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a[(int)(from + i)] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
double *CopyNumberArray(size_t *returnArrayLength, double *a, size_t aLength){
  double i;
  double *n;
  size_t nLength;

  n = (double*)malloc(sizeof(double) * (aLength));
  nLength = aLength;

  for(i = 0.0; i < aLength; i = i + 1.0){
    n[(int)(i)] = a[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
_Bool *CopyBooleanArray(size_t *returnArrayLength, _Bool *a, size_t aLength){
  double i;
  _Bool *n;
  size_t nLength;

  n = (_Bool*)malloc(sizeof(_Bool) * (aLength));
  nLength = aLength;

  for(i = 0.0; i < aLength; i = i + 1.0){
    n[(int)(i)] = a[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
wchar_t *CopyString(size_t *returnArrayLength, wchar_t *a, size_t aLength){
  double i;
  wchar_t *n;
  size_t nLength;

  n = (wchar_t*)malloc(sizeof(wchar_t) * (aLength));
  nLength = aLength;

  for(i = 0.0; i < aLength; i = i + 1.0){
    n[(int)(i)] = a[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
_Bool CopyNumberArrayRange(double *a, size_t aLength, double from, double to, NumberArrayReference *copyReference){
  double i, length;
  double *n;
  size_t nLength;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    n = (double*)malloc(sizeof(double) * (length));
    nLength = length;

    for(i = 0.0; i < length; i = i + 1.0){
      n[(int)(i)] = a[(int)(from + i)];
    }

    copyReference->numberArray = n;
    copyReference->numberArrayLength = nLength;
    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool CopyBooleanArrayRange(_Bool *a, size_t aLength, double from, double to, BooleanArrayReference *copyReference){
  double i, length;
  _Bool *n;
  size_t nLength;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    n = (_Bool*)malloc(sizeof(_Bool) * (length));
    nLength = length;

    for(i = 0.0; i < length; i = i + 1.0){
      n[(int)(i)] = a[(int)(from + i)];
    }

    copyReference->booleanArray = n;
    copyReference->booleanArrayLength = nLength;
    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool CopyStringRange(wchar_t *a, size_t aLength, double from, double to, StringReference *copyReference){
  double i, length;
  wchar_t *n;
  size_t nLength;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    n = (wchar_t*)malloc(sizeof(wchar_t) * (length));
    nLength = length;

    for(i = 0.0; i < length; i = i + 1.0){
      n[(int)(i)] = a[(int)(from + i)];
    }

    copyReference->string = n;
    copyReference->stringLength = nLength;
    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool IsLastElement(double length, double index){
  return index + 1.0 == length;
}
double *CreateNumberArray(size_t *returnArrayLength, double length, double value){
  double *array;
  size_t arrayLength;

  array = (double*)malloc(sizeof(double) * (length));
  arrayLength = length;
  FillNumberArray(array, arrayLength, value);

  *returnArrayLength = arrayLength;
  return array;
}
_Bool *CreateBooleanArray(size_t *returnArrayLength, double length, _Bool value){
  _Bool *array;
  size_t arrayLength;

  array = (_Bool*)malloc(sizeof(_Bool) * (length));
  arrayLength = length;
  FillBooleanArray(array, arrayLength, value);

  *returnArrayLength = arrayLength;
  return array;
}
wchar_t *CreateString(size_t *returnArrayLength, double length, wchar_t value){
  wchar_t *array;
  size_t arrayLength;

  array = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  arrayLength = length;
  FillString(array, arrayLength, value);

  *returnArrayLength = arrayLength;
  return array;
}
void SwapElementsOfNumberArray(double *A, size_t ALength, double ai, double bi){
  double tmp;

  tmp = A[(int)(ai)];
  A[(int)(ai)] = A[(int)(bi)];
  A[(int)(bi)] = tmp;
}
void SwapElementsOfStringArray(StringArrayReference *A, double ai, double bi){
  StringReference *tmp;

  tmp = A->stringArray[(int)(ai)];
  A->stringArray[(int)(ai)] = A->stringArray[(int)(bi)];
  A->stringArray[(int)(bi)] = tmp;
}
void ReverseNumberArray(double *array, size_t arrayLength){
  double i;

  for(i = 0.0; i < arrayLength/2.0; i = i + 1.0){
    SwapElementsOfNumberArray(array, arrayLength, i, arrayLength - i - 1.0);
  }
}
void AssertFalse(_Bool b, NumberReference *failures){
  if(b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertTrue(_Bool b, NumberReference *failures){
  if( !b ){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertEquals(double a, double b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertBooleansEqual(_Bool a, _Bool b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertCharactersEqual(wchar_t a, wchar_t b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertStringEquals(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, NumberReference *failures){
  if( !StringsEqual(a, aLength, b, bLength) ){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertNumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength, NumberReference *failures){
  double i;

  if(aLength == bLength){
    for(i = 0.0; i < aLength; i = i + 1.0){
      AssertEquals(a[(int)(i)], b[(int)(i)], failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertBooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength, NumberReference *failures){
  double i;

  if(aLength == bLength){
    for(i = 0.0; i < aLength; i = i + 1.0){
      AssertBooleansEqual(a[(int)(i)], b[(int)(i)], failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertStringArraysEqual(StringReference **a, size_t aLength, StringReference **b, size_t bLength, NumberReference *failures){
  double i;

  if(aLength == bLength){
    for(i = 0.0; i < aLength; i = i + 1.0){
      AssertStringEquals(a[(int)(i)]->string, a[(int)(i)]->stringLength, b[(int)(i)]->string, b[(int)(i)]->stringLength, failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
wchar_t *nCreateStringScientificNotationDecimalFromNumber(size_t *returnArrayLength, double decimal){
  StringReference *mantissaReference, *exponentReference;
  double multiplier, inc;
  double exponent;
  _Bool done, isPositive;
  wchar_t *result;
  size_t resultLength;

  mantissaReference = (StringReference *)malloc(sizeof(StringReference));
  exponentReference = (StringReference *)malloc(sizeof(StringReference));
  result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  resultLength = 0.0;
  done = false;
  exponent = 0.0;

  if(decimal < 0.0){
    isPositive = false;
    decimal =  -decimal;
  }else{
    isPositive = true;
  }

  if(decimal == 0.0){
    done = true;
  }

  if( !done ){
    multiplier = 0.0;
    inc = 0.0;

    if(decimal < 1.0){
      multiplier = 10.0;
      inc =  -1.0;
    }else if(decimal >= 10.0){
      multiplier = 0.1;
      inc = 1.0;
    }else{
      done = true;
    }

    if( !done ){
      for(; decimal >= 10.0 || decimal < 1.0; ){
        decimal = decimal*multiplier;
        exponent = exponent + inc;
      }
    }
  }

  nCreateStringFromNumberWithCheck(decimal, 10.0, mantissaReference);

  nCreateStringFromNumberWithCheck(exponent, 10.0, exponentReference);

  if( !isPositive ){
    result = strAppendString(&resultLength, result, resultLength, strparam(L"-"));
  }

  result = strAppendString(&resultLength, result, resultLength, mantissaReference->string, mantissaReference->stringLength);
  result = strAppendString(&resultLength, result, resultLength, strparam(L"e"));
  result = strAppendString(&resultLength, result, resultLength, exponentReference->string, exponentReference->stringLength);

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *nCreateStringDecimalFromNumber(size_t *returnArrayLength, double decimal){
  StringReference *stringReference;

  stringReference = (StringReference *)malloc(sizeof(StringReference));

  /* This will succeed because base = 10. */
  nCreateStringFromNumberWithCheck(decimal, 10.0, stringReference);

  *returnArrayLength = stringReference->stringLength;
  return stringReference->string;
}
_Bool nCreateStringFromNumberWithCheck(double decimal, double base, StringReference *stringReference){
  wchar_t *string;
  size_t stringLength;
  double maximumDigits;
  double digitPosition;
  _Bool hasPrintedPoint, isPositive;
  double i, d;
  _Bool success;
  CharacterReference *characterReference;
  wchar_t c;

  isPositive = true;

  if(decimal < 0.0){
    isPositive = false;
    decimal =  -decimal;
  }

  if(decimal == 0.0){
    stringReference->string = L"0";
    stringReference->stringLength = wcslen(stringReference->string);
    success = true;
  }else{
    characterReference = (CharacterReference *)malloc(sizeof(CharacterReference));

    if(IsInteger(base)){
      success = true;

      string = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
      stringLength = 0.0;

      maximumDigits = nGetMaximumDigitsForBase(base);

      digitPosition = nGetFirstDigitPosition(decimal, base);

      decimal = round(decimal*pow(base, maximumDigits - digitPosition - 1.0));

      hasPrintedPoint = false;

      if( !isPositive ){
        string = strAppendCharacter(&stringLength, string, stringLength, '-');
      }

      /* Print leading zeros. */
      if(digitPosition < 0.0){
        string = strAppendCharacter(&stringLength, string, stringLength, '0');
        string = strAppendCharacter(&stringLength, string, stringLength, '.');
        hasPrintedPoint = true;
        for(i = 0.0; i <  -digitPosition - 1.0; i = i + 1.0){
          string = strAppendCharacter(&stringLength, string, stringLength, '0');
        }
      }

      /* Print number. */
      for(i = 0.0; i < maximumDigits && success; i = i + 1.0){
        d = floor(decimal/pow(base, maximumDigits - i - 1.0));

        if(d >= base){
          d = base - 1.0;
        }

        if( !hasPrintedPoint  && digitPosition - i + 1.0 == 0.0){
          if(decimal != 0.0){
            string = strAppendCharacter(&stringLength, string, stringLength, '.');
          }
          hasPrintedPoint = true;
        }

        if(decimal == 0.0 && hasPrintedPoint){
        }else{
          success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
          if(success){
            c = characterReference->characterValue;
            string = strAppendCharacter(&stringLength, string, stringLength, c);
          }
        }

        if(success){
          decimal = decimal - d*pow(base, maximumDigits - i - 1.0);
        }
      }

      if(success){
        /* Print trailing zeros. */
        for(i = 0.0; i < digitPosition - maximumDigits + 1.0; i = i + 1.0){
          string = strAppendCharacter(&stringLength, string, stringLength, '0');
        }

        stringReference->string = string;
        stringReference->stringLength = stringLength;
      }
    }else{
      success = false;
    }
  }

  /* Done */
  return success;
}
double nGetMaximumDigitsForBase(double base){
  double t;

  t = pow(10.0, 15.0);
  return floor(log10(t)/log10(base));
}
double nGetFirstDigitPosition(double decimal, double base){
  double power;
  double t;

  power = ceil(log10(decimal)/log10(base));

  t = decimal*pow(base,  -power);
  if(t < base && t >= 1.0){
  }else if(t >= base){
    power = power + 1.0;
  }else if(t < 1.0){
    power = power - 1.0;
  }

  return power;
}
_Bool nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference *characterReference){
  wchar_t *numberTable;
  size_t numberTableLength;
  _Bool success;

  numberTable = nGetDigitCharacterTable(&numberTableLength);

  if(c < base || c < numberTableLength){
    success = true;
    characterReference->characterValue = numberTable[(int)(c)];
  }else{
    success = false;
  }

  return success;
}
wchar_t *nGetDigitCharacterTable(size_t *returnArrayLength){
  wchar_t *numberTable;
  size_t numberTableLength;

  numberTable = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  numberTableLength = wcslen(numberTable);

  *returnArrayLength = numberTableLength;
  return numberTable;
}
_Bool nCreateNumberFromDecimalStringWithCheck(wchar_t *string, size_t stringLength, NumberReference *decimalReference, StringReference *errorMessage){
  return nCreateNumberFromStringWithCheck(string, stringLength, 10.0, decimalReference, errorMessage);
}
double nCreateNumberFromDecimalString(wchar_t *string, size_t stringLength){
  NumberReference *doubleReference;
  StringReference *stringReference;
  double number;

  doubleReference = CreateNumberReference(0.0);
  stringReference = CreateStringReference(strparam(L""));
  nCreateNumberFromStringWithCheck(string, stringLength, 10.0, doubleReference, stringReference);
  number = doubleReference->numberValue;

  free(doubleReference);
  free(stringReference);

  return number;
}
_Bool nCreateNumberFromStringWithCheck(wchar_t *string, size_t stringLength, double base, NumberReference *numberReference, StringReference *errorMessage){
  _Bool success;
  BooleanReference *numberIsPositive, *exponentIsPositive;
  NumberArrayReference *beforePoint, *afterPoint, *exponent;

  numberIsPositive = CreateBooleanReference(true);
  exponentIsPositive = CreateBooleanReference(true);
  beforePoint = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  afterPoint = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  exponent = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));

  if(base >= 2.0 && base <= 36.0){
    success = nExtractPartsFromNumberString(string, stringLength, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage);

    if(success){
      numberReference->numberValue = nCreateNumberFromParts(base, numberIsPositive->booleanValue, beforePoint->numberArray, beforePoint->numberArrayLength, afterPoint->numberArray, afterPoint->numberArrayLength, exponentIsPositive->booleanValue, exponent->numberArray, exponent->numberArrayLength);
    }
  }else{
    success = false;
    errorMessage->string = L"Base must be from 2 to 36.";
    errorMessage->stringLength = wcslen(errorMessage->string);
  }

  return success;
}
double nCreateNumberFromParts(double base, _Bool numberIsPositive, double *beforePoint, size_t beforePointLength, double *afterPoint, size_t afterPointLength, _Bool exponentIsPositive, double *exponent, size_t exponentLength){
  double n, i, p, e;

  n = 0.0;

  for(i = 0.0; i < beforePointLength; i = i + 1.0){
    p = beforePoint[(int)(beforePointLength - i - 1.0)];

    n = n + p*pow(base, i);
  }

  for(i = 0.0; i < afterPointLength; i = i + 1.0){
    p = afterPoint[(int)(i)];

    n = n + p*pow(base,  -(i + 1.0));
  }

  if(exponentLength > 0.0){
    e = 0.0;
    for(i = 0.0; i < exponentLength; i = i + 1.0){
      p = exponent[(int)(exponentLength - i - 1.0)];

      e = e + p*pow(base, i);
    }

    if( !exponentIsPositive ){
      e =  -e;
    }

    n = n*pow(base, e);
  }

  if( !numberIsPositive ){
    n =  -n;
  }

  return n;
}
_Bool nExtractPartsFromNumberString(wchar_t *n, size_t nLength, double base, BooleanReference *numberIsPositive, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  double i;
  _Bool success;

  i = 0.0;

  if(i < nLength){
    if(n[(int)(i)] == '-'){
      numberIsPositive->booleanValue = false;
      i = i + 1.0;
    }else if(n[(int)(i)] == '+'){
      numberIsPositive->booleanValue = true;
      i = i + 1.0;
    }

    success = nExtractPartsFromNumberStringFromSign(n, nLength, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages);
  }else{
    success = false;
    errorMessages->string = L"Number cannot have length zero.";
    errorMessages->stringLength = wcslen(errorMessages->string);
  }

  return success;
}
_Bool nExtractPartsFromNumberStringFromSign(wchar_t *n, size_t nLength, double base, double i, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  _Bool success, done;
  double count, j;

  done = false;
  count = 0.0;
  for(; i + count < nLength &&  !done ; ){
    if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
      count = count + 1.0;
    }else{
      done = true;
    }
  }

  if(count >= 1.0){
    beforePoint->numberArray = (double*)malloc(sizeof(double) * (count));
    beforePoint->numberArrayLength = count;

    for(j = 0.0; j < count; j = j + 1.0){
      beforePoint->numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
    }

    i = i + count;

    if(i < nLength){
      success = nExtractPartsFromNumberStringFromPointOrExponent(n, nLength, base, i, afterPoint, exponentIsPositive, exponent, errorMessages);
    }else{
      afterPoint->numberArray = (double*)malloc(sizeof(double) * (0.0));
      afterPoint->numberArrayLength = 0.0;
      exponent->numberArray = (double*)malloc(sizeof(double) * (0.0));
      exponent->numberArrayLength = 0.0;
      success = true;
    }
  }else{
    success = false;
    errorMessages->string = L"Number must have at least one number after the optional sign.";
    errorMessages->stringLength = wcslen(errorMessages->string);
  }

  return success;
}
_Bool nExtractPartsFromNumberStringFromPointOrExponent(wchar_t *n, size_t nLength, double base, double i, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  _Bool success, done;
  double count, j;

  if(n[(int)(i)] == '.'){
    i = i + 1.0;

    if(i < nLength){
      done = false;
      count = 0.0;
      for(; i + count < nLength &&  !done ; ){
        if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
          count = count + 1.0;
        }else{
          done = true;
        }
      }

      if(count >= 1.0){
        afterPoint->numberArray = (double*)malloc(sizeof(double) * (count));
        afterPoint->numberArrayLength = count;

        for(j = 0.0; j < count; j = j + 1.0){
          afterPoint->numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
        }

        i = i + count;

        if(i < nLength){
          success = nExtractPartsFromNumberStringFromExponent(n, nLength, base, i, exponentIsPositive, exponent, errorMessages);
        }else{
          exponent->numberArray = (double*)malloc(sizeof(double) * (0.0));
          exponent->numberArrayLength = 0.0;
          success = true;
        }
      }else{
        success = false;
        errorMessages->string = L"There must be at least one digit after the decimal point.";
        errorMessages->stringLength = wcslen(errorMessages->string);
      }
    }else{
      success = false;
      errorMessages->string = L"There must be at least one digit after the decimal point.";
      errorMessages->stringLength = wcslen(errorMessages->string);
    }
  }else if(base <= 14.0 && (n[(int)(i)] == 'e' || n[(int)(i)] == 'E')){
    if(i < nLength){
      success = nExtractPartsFromNumberStringFromExponent(n, nLength, base, i, exponentIsPositive, exponent, errorMessages);
      afterPoint->numberArray = (double*)malloc(sizeof(double) * (0.0));
      afterPoint->numberArrayLength = 0.0;
    }else{
      success = false;
      errorMessages->string = L"There must be at least one digit after the exponent.";
      errorMessages->stringLength = wcslen(errorMessages->string);
    }
  }else{
    success = false;
    errorMessages->string = L"Expected decimal point or exponent symbol.";
    errorMessages->stringLength = wcslen(errorMessages->string);
  }

  return success;
}
_Bool nExtractPartsFromNumberStringFromExponent(wchar_t *n, size_t nLength, double base, double i, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  _Bool success, done;
  double count, j;

  if(base <= 14.0 && (n[(int)(i)] == 'e' || n[(int)(i)] == 'E')){
    i = i + 1.0;

    if(i < nLength){
      if(n[(int)(i)] == '-'){
        exponentIsPositive->booleanValue = false;
        i = i + 1.0;
      }else if(n[(int)(i)] == '+'){
        exponentIsPositive->booleanValue = true;
        i = i + 1.0;
      }

      if(i < nLength){
        done = false;
        count = 0.0;
        for(; i + count < nLength &&  !done ; ){
          if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
            count = count + 1.0;
          }else{
            done = true;
          }
        }

        if(count >= 1.0){
          exponent->numberArray = (double*)malloc(sizeof(double) * (count));
          exponent->numberArrayLength = count;

          for(j = 0.0; j < count; j = j + 1.0){
            exponent->numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
          }

          i = i + count;

          if(i == nLength){
            success = true;
          }else{
            success = false;
            errorMessages->string = L"There cannot be any characters past the exponent of the number.";
            errorMessages->stringLength = wcslen(errorMessages->string);
          }
        }else{
          success = false;
          errorMessages->string = L"There must be at least one digit after the decimal point.";
          errorMessages->stringLength = wcslen(errorMessages->string);
        }
      }else{
        success = false;
        errorMessages->string = L"There must be at least one digit after the exponent symbol.";
        errorMessages->stringLength = wcslen(errorMessages->string);
      }
    }else{
      success = false;
      errorMessages->string = L"There must be at least one digit after the exponent symbol.";
      errorMessages->stringLength = wcslen(errorMessages->string);
    }
  }else{
    success = false;
    errorMessages->string = L"Expected exponent symbol.";
    errorMessages->stringLength = wcslen(errorMessages->string);
  }

  return success;
}
double nGetNumberFromNumberCharacterForBase(wchar_t c, double base){
  wchar_t *numberTable;
  size_t numberTableLength;
  double i;
  double position;

  numberTable = nGetDigitCharacterTable(&numberTableLength);
  position = 0.0;

  for(i = 0.0; i < base; i = i + 1.0){
    if(numberTable[(int)(i)] == c){
      position = i;
    }
  }

  return position;
}
_Bool nCharacterIsNumberCharacterInBase(wchar_t c, double base){
  wchar_t *numberTable;
  size_t numberTableLength;
  double i;
  _Bool found;

  numberTable = nGetDigitCharacterTable(&numberTableLength);
  found = false;

  for(i = 0.0; i < base; i = i + 1.0){
    if(numberTable[(int)(i)] == c){
      found = true;
    }
  }

  return found;
}
double *nStringToNumberArray(size_t *returnArrayLength, wchar_t *str, size_t strLength){
  NumberArrayReference *numberArrayReference;
  StringReference *stringReference;
  double *numbers;
  size_t numbersLength;

  numberArrayReference = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  stringReference = (StringReference *)malloc(sizeof(StringReference));

  nStringToNumberArrayWithCheck(str, strLength, numberArrayReference, stringReference);

  numbers = numberArrayReference->numberArray;
  numbersLength = numberArrayReference->numberArrayLength;

  free(numberArrayReference);
  free(stringReference);

  *returnArrayLength = numbersLength;
  return numbers;
}
_Bool nStringToNumberArrayWithCheck(wchar_t *str, size_t strLength, NumberArrayReference *numberArrayReference, StringReference *errorMessage){
  StringReference **numberStrings;
  size_t numberStringsLength;
  double *numbers;
  size_t numbersLength;
  double i;
  wchar_t *numberString, *trimmedNumberString;
  size_t numberStringLength, trimmedNumberStringLength;
  _Bool success;
  NumberReference *numberReference;

  numberStrings = strSplitByString(&numberStringsLength, str, strLength, strparam(L","));

  numbers = (double*)malloc(sizeof(double) * (numberStringsLength));
  numbersLength = numberStringsLength;
  success = true;
  numberReference = (NumberReference *)malloc(sizeof(NumberReference));

  for(i = 0.0; i < numberStringsLength; i = i + 1.0){
    numberString = numberStrings[(int)(i)]->string;
    numberStringLength = numberStrings[(int)(i)]->stringLength;
    trimmedNumberString = strTrim(&trimmedNumberStringLength, numberString, numberStringLength);
    success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, trimmedNumberStringLength, numberReference, errorMessage);
    numbers[(int)(i)] = numberReference->numberValue;

    FreeStringReference(numberStrings[(int)(i)]);
    free(trimmedNumberString);
  }

  free(numberStrings);
  free(numberReference);

  numberArrayReference->numberArray = numbers;
  numberArrayReference->numberArrayLength = numbersLength;

  return success;
}
double *lAddNumber(size_t *returnArrayLength, double *list, size_t listLength, double a){
  double *newlist;
  size_t newlistLength;
  double i;

  newlist = (double*)malloc(sizeof(double) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void lAddNumberRef(NumberArrayReference *list, double i){
  list->numberArray = lAddNumber(&list->numberArrayLength, list->numberArray, list->numberArrayLength, i);
}
double *lRemoveNumber(size_t *returnArrayLength, double *list, size_t listLength, double n){
  double *newlist;
  size_t newlistLength;
  double i;

  newlist = (double*)malloc(sizeof(double) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
double lGetNumberRef(NumberArrayReference *list, double i){
  return list->numberArray[(int)(i)];
}
void lRemoveNumberRef(NumberArrayReference *list, double i){
  list->numberArray = lRemoveNumber(&list->numberArrayLength, list->numberArray, list->numberArrayLength, i);
}
StringReference **lAddString(size_t *returnArrayLength, StringReference **list, size_t listLength, StringReference *a){
  StringReference **newlist;
  size_t newlistLength;
  double i;

  newlist = (StringReference**)malloc(sizeof(StringReference) * listLength + 1.0);
  newlistLength = listLength + 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void lAddStringRef(StringArrayReference *list, StringReference *i){
  list->stringArray = lAddString(&list->stringArrayLength, list->stringArray, list->stringArrayLength, i);
}
StringReference **lRemoveString(size_t *returnArrayLength, StringReference **list, size_t listLength, double n){
  StringReference **newlist;
  size_t newlistLength;
  double i;

  newlist = (StringReference**)malloc(sizeof(StringReference) * listLength - 1.0);
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
StringReference *lGetStringRef(StringArrayReference *list, double i){
  return list->stringArray[(int)(i)];
}
void lRemoveStringRef(StringArrayReference *list, double i){
  list->stringArray = lRemoveString(&list->stringArrayLength, list->stringArray, list->stringArrayLength, i);
}
_Bool *lAddBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, _Bool a){
  _Bool *newlist;
  size_t newlistLength;
  double i;

  newlist = (_Bool*)malloc(sizeof(_Bool) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void lAddBooleanRef(BooleanArrayReference *list, _Bool i){
  list->booleanArray = lAddBoolean(&list->booleanArrayLength, list->booleanArray, list->booleanArrayLength, i);
}
_Bool *lRemoveBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, double n){
  _Bool *newlist;
  size_t newlistLength;
  double i;

  newlist = (_Bool*)malloc(sizeof(_Bool) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
_Bool lGetBooleanRef(BooleanArrayReference *list, double i){
  return list->booleanArray[(int)(i)];
}
void lRemoveDecimalRef(BooleanArrayReference *list, double i){
  list->booleanArray = lRemoveBoolean(&list->booleanArrayLength, list->booleanArray, list->booleanArrayLength, i);
}
lLinkedListStrings *lCreateLinkedListString(){
  lLinkedListStrings *ll;

  ll = (lLinkedListStrings *)malloc(sizeof(lLinkedListStrings));
  ll->first = (lLinkedListNodeStrings *)malloc(sizeof(lLinkedListNodeStrings));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void lLinkedListAddString(lLinkedListStrings *ll, wchar_t *value, size_t valueLength){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->valueLength = valueLength;
  ll->last->next = (lLinkedListNodeStrings *)malloc(sizeof(lLinkedListNodeStrings));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
StringReference **lLinkedListStringsToArray(size_t *returnArrayLength, lLinkedListStrings *ll){
  StringReference **array;
  size_t arrayLength;
  double length, i;
  lLinkedListNodeStrings *node;

  node = ll->first;

  length = lLinkedListStringsLength(ll);

  array = (StringReference**)malloc(sizeof(StringReference) * length);
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = (StringReference *)malloc(sizeof(StringReference));
    array[(int)(i)]->string = node->value;
    array[(int)(i)]->stringLength = node->valueLength;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double lLinkedListStringsLength(lLinkedListStrings *ll){
  double l;
  lLinkedListNodeStrings *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void lFreeLinkedListString(lLinkedListStrings *ll){
  lLinkedListNodeStrings *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
lLinkedListNumbers *lCreateLinkedListNumbers(){
  lLinkedListNumbers *ll;

  ll = (lLinkedListNumbers *)malloc(sizeof(lLinkedListNumbers));
  ll->first = (lLinkedListNodeNumbers *)malloc(sizeof(lLinkedListNodeNumbers));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
lLinkedListNumbers **lCreateLinkedListNumbersArray(size_t *returnArrayLength, double length){
  lLinkedListNumbers **lls;
  size_t llsLength;
  double i;

  lls = (lLinkedListNumbers**)malloc(sizeof(lLinkedListNumbers) * length);
  llsLength = length;
  for(i = 0.0; i < llsLength; i = i + 1.0){
    lls[(int)(i)] = lCreateLinkedListNumbers();
  }

  *returnArrayLength = llsLength;
  return lls;
}
void lLinkedListAddNumber(lLinkedListNumbers *ll, double value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (lLinkedListNodeNumbers *)malloc(sizeof(lLinkedListNodeNumbers));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
double lLinkedListNumbersLength(lLinkedListNumbers *ll){
  double l;
  lLinkedListNodeNumbers *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
double lLinkedListNumbersIndex(lLinkedListNumbers *ll, double index){
  double i;
  lLinkedListNodeNumbers *node;

  node = ll->first;
  for(i = 0.0; i < index; i = i + 1.0){
    node = node->next;
  }

  return node->value;
}
void lLinkedListInsertNumber(lLinkedListNumbers *ll, double index, double value){
  double i;
  lLinkedListNodeNumbers *node, *tmp;

  if(index == 0.0){
    tmp = ll->first;
    ll->first = (lLinkedListNodeNumbers *)malloc(sizeof(lLinkedListNodeNumbers));
    ll->first->next = tmp;
    ll->first->value = value;
    ll->first->end = false;
  }else{
    node = ll->first;
    for(i = 0.0; i < index - 1.0; i = i + 1.0){
      node = node->next;
    }

    tmp = node->next;
    node->next = (lLinkedListNodeNumbers *)malloc(sizeof(lLinkedListNodeNumbers));
    node->next->next = tmp;
    node->next->value = value;
    node->next->end = false;
  }
}
void lLinkedListSet(lLinkedListNumbers *ll, double index, double value){
  double i;
  lLinkedListNodeNumbers *node;

  node = ll->first;
  for(i = 0.0; i < index; i = i + 1.0){
    node = node->next;
  }

  node->next->value = value;
}
void lLinkedListRemoveNumber(lLinkedListNumbers *ll, double index){
  double i;
  lLinkedListNodeNumbers *node, *prev;

  node = ll->first;
  prev = ll->first;

  for(i = 0.0; i < index; i = i + 1.0){
    prev = node;
    node = node->next;
  }

  if(index == 0.0){
    ll->first = prev->next;
  }
  if( !prev->next->end ){
    prev->next = prev->next->next;
  }
}
void lFreeLinkedListNumbers(lLinkedListNumbers *ll){
  lLinkedListNodeNumbers *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
void lFreeLinkedListNumbersArray(lLinkedListNumbers **lls, size_t llsLength){
  double i;

  for(i = 0.0; i < llsLength; i = i + 1.0){
    lFreeLinkedListNumbers(lls[(int)(i)]);
  }
  free(lls);
}
double *lLinkedListNumbersToArray(size_t *returnArrayLength, lLinkedListNumbers *ll){
  double *array;
  size_t arrayLength;
  double length, i;
  lLinkedListNodeNumbers *node;

  node = ll->first;

  length = lLinkedListNumbersLength(ll);

  array = (double*)malloc(sizeof(double) * (length));
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
lLinkedListNumbers *lArrayToLinkedListNumbers(double *array, size_t arrayLength){
  lLinkedListNumbers *ll;
  double i;

  ll = lCreateLinkedListNumbers();

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    lLinkedListAddNumber(ll, array[(int)(i)]);
  }

  return ll;
}
_Bool lLinkedListNumbersEqual(lLinkedListNumbers *a, lLinkedListNumbers *b){
  _Bool equal, done;
  lLinkedListNodeNumbers *an, *bn;

  an = a->first;
  bn = b->first;

  equal = true;
  done = false;
  for(; equal &&  !done ; ){
    if(an->end == bn->end){
      if(an->end){
        done = true;
      }else if(an->value == bn->value){
        an = an->next;
        bn = bn->next;
      }else{
        equal = false;
      }
    }else{
      equal = false;
    }
  }

  return equal;
}
lLinkedListCharacters *lCreateLinkedListCharacter(){
  lLinkedListCharacters *ll;

  ll = (lLinkedListCharacters *)malloc(sizeof(lLinkedListCharacters));
  ll->first = (lLinkedListNodeCharacters *)malloc(sizeof(lLinkedListNodeCharacters));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void lLinkedListAddCharacter(lLinkedListCharacters *ll, wchar_t value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (lLinkedListNodeCharacters *)malloc(sizeof(lLinkedListNodeCharacters));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
wchar_t *lLinkedListCharactersToArray(size_t *returnArrayLength, lLinkedListCharacters *ll){
  wchar_t *array;
  size_t arrayLength;
  double length, i;
  lLinkedListNodeCharacters *node;

  node = ll->first;

  length = lLinkedListCharactersLength(ll);

  array = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double lLinkedListCharactersLength(lLinkedListCharacters *ll){
  double l;
  lLinkedListNodeCharacters *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void lFreeLinkedListCharacter(lLinkedListCharacters *ll){
  lLinkedListNodeCharacters *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
lDynamicArrayNumbers *lCreateDynamicArrayNumbers(){
  lDynamicArrayNumbers *da;

  da = (lDynamicArrayNumbers *)malloc(sizeof(lDynamicArrayNumbers));
  da->array = (double*)malloc(sizeof(double) * (10.0));
  da->arrayLength = 10.0;
  da->length = 0.0;

  return da;
}
lDynamicArrayNumbers *lCreateDynamicArrayNumbersWithInitialCapacity(double capacity){
  lDynamicArrayNumbers *da;

  da = (lDynamicArrayNumbers *)malloc(sizeof(lDynamicArrayNumbers));
  da->array = (double*)malloc(sizeof(double) * (capacity));
  da->arrayLength = capacity;
  da->length = 0.0;

  return da;
}
void lDynamicArrayAddNumber(lDynamicArrayNumbers *da, double value){
  if(da->length == da->arrayLength){
    lDynamicArrayNumbersIncreaseSize(da);
  }

  da->array[(int)(da->length)] = value;
  da->length = da->length + 1.0;
}
void lDynamicArrayNumbersIncreaseSize(lDynamicArrayNumbers *da){
  double newLength, i;
  double *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*3.0/2.0);
  newArray = (double*)malloc(sizeof(double) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < da->arrayLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
_Bool lDynamicArrayNumbersDecreaseSizeNecessary(lDynamicArrayNumbers *da){
  _Bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->arrayLength*2.0/3.0);
  }

  return needsDecrease;
}
void lDynamicArrayNumbersDecreaseSize(lDynamicArrayNumbers *da){
  double newLength, i;
  double *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*2.0/3.0);
  newArray = (double*)malloc(sizeof(double) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < da->arrayLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
double lDynamicArrayNumbersIndex(lDynamicArrayNumbers *da, double index){
  return da->array[(int)(index)];
}
double lDynamicArrayNumbersLength(lDynamicArrayNumbers *da){
  return da->length;
}
void lDynamicArrayInsertNumber(lDynamicArrayNumbers *da, double index, double value){
  double i;

  if(da->length == da->arrayLength){
    lDynamicArrayNumbersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array[(int)(i)] = da->array[(int)(i - 1.0)];
  }

  da->array[(int)(index)] = value;

  da->length = da->length + 1.0;
}
void lDynamicArraySet(lDynamicArrayNumbers *da, double index, double value){
  da->array[(int)(index)] = value;
}
void lDynamicArrayRemoveNumber(lDynamicArrayNumbers *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array[(int)(i)] = da->array[(int)(i + 1.0)];
  }

  da->length = da->length - 1.0;

  if(lDynamicArrayNumbersDecreaseSizeNecessary(da)){
    lDynamicArrayNumbersDecreaseSize(da);
  }
}
void lFreeDynamicArrayNumbers(lDynamicArrayNumbers *da){
  free(da->array);
  free(da);
}
double *lDynamicArrayNumbersToArray(size_t *returnArrayLength, lDynamicArrayNumbers *da){
  double *array;
  size_t arrayLength;
  double i;

  array = (double*)malloc(sizeof(double) * (da->length));
  arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    array[(int)(i)] = da->array[(int)(i)];
  }

  *returnArrayLength = arrayLength;
  return array;
}
lDynamicArrayNumbers *lArrayToDynamicArrayNumbersWithOptimalSize(double *array, size_t arrayLength){
  lDynamicArrayNumbers *da;
  double i;
  double c, n, newCapacity;

  /*
         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
         */
  c = arrayLength;
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = lCreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    da->array[(int)(i)] = array[(int)(i)];
  }

  return da;
}
lDynamicArrayNumbers *lArrayToDynamicArrayNumbers(double *array, size_t arrayLength){
  lDynamicArrayNumbers *da;

  da = (lDynamicArrayNumbers *)malloc(sizeof(lDynamicArrayNumbers));
  da->array = CopyNumberArray(&da->arrayLength, array, arrayLength);
  da->length = arrayLength;

  return da;
}
_Bool lDynamicArrayNumbersEqual(lDynamicArrayNumbers *a, lDynamicArrayNumbers *b){
  _Bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array[(int)(i)] != b->array[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
lLinkedListNumbers *lDynamicArrayNumbersToLinkedList(lDynamicArrayNumbers *da){
  lLinkedListNumbers *ll;
  double i;

  ll = lCreateLinkedListNumbers();

  for(i = 0.0; i < da->length; i = i + 1.0){
    lLinkedListAddNumber(ll, da->array[(int)(i)]);
  }

  return ll;
}
lDynamicArrayNumbers *lLinkedListToDynamicArrayNumbers(lLinkedListNumbers *ll){
  lDynamicArrayNumbers *da;
  double i;
  lLinkedListNodeNumbers *node;

  node = ll->first;

  da = (lDynamicArrayNumbers *)malloc(sizeof(lDynamicArrayNumbers));
  da->length = lLinkedListNumbersLength(ll);

  da->array = (double*)malloc(sizeof(double) * (da->length));
  da->arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array[(int)(i)] = node->value;
    node = node->next;
  }

  return da;
}
wchar_t *lAddCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, wchar_t a){
  wchar_t *newlist;
  size_t newlistLength;
  double i;

  newlist = (wchar_t*)malloc(sizeof(wchar_t) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void lAddCharacterRef(StringReference *list, wchar_t i){
  list->string = lAddCharacter(&list->stringLength, list->string, list->stringLength, i);
}
wchar_t *lRemoveCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, double n){
  wchar_t *newlist;
  size_t newlistLength;
  double i;

  newlist = (wchar_t*)malloc(sizeof(wchar_t) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }

  *returnArrayLength = newlistLength;
  return newlist;
}
wchar_t lGetCharacterRef(StringReference *list, double i){
  return list->string[(int)(i)];
}
void lRemoveCharacterRef(StringReference *list, double i){
  list->string = lRemoveCharacter(&list->stringLength, list->string, list->stringLength, i);
}
double Negate(double x){
  return  -x;
}
double Positive(double x){
  return  +x;
}
double Factorial(double x){
  double i, f;

  f = 1.0;

  for(i = 2.0; i <= x; i = i + 1.0){
    f = f*i;
  }

  return f;
}
double Round(double x){
  return floor(x + 0.5);
}
double BankersRound(double x){
  double r;

  if(Absolute(x - Truncate(x)) == 0.5){
    if( !DivisibleBy(Round(x), 2.0) ){
      r = Round(x) - 1.0;
    }else{
      r = Round(x);
    }
  }else{
    r = Round(x);
  }

  return r;
}
double Ceil(double x){
  return ceil(x);
}
double Floor(double x){
  return floor(x);
}
double Truncate(double x){
  double t;

  if(x >= 0.0){
    t = floor(x);
  }else{
    t = ceil(x);
  }

  return t;
}
double Absolute(double x){
  return fabs(x);
}
double Logarithm(double x){
  return log10(x);
}
double NaturalLogarithm(double x){
  return log(x);
}
double Sin(double x){
  return sin(x);
}
double Cos(double x){
  return cos(x);
}
double Tan(double x){
  return tan(x);
}
double Asin(double x){
  return asin(x);
}
double Acos(double x){
  return acos(x);
}
double Atan(double x){
  return atan(x);
}
double Atan2(double y, double x){
  double a;

  /* Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors. */
  a = 0.0;

  if(x > 0.0){
    a = Atan(y/x);
  }else if(x < 0.0 && y >= 0.0){
    a = Atan(y/x) + M_PI;
  }else if(x < 0.0 && y < 0.0){
    a = Atan(y/x) - M_PI;
  }else if(x == 0.0 && y > 0.0){
    a = M_PI/2.0;
  }else if(x == 0.0 && y < 0.0){
    a =  -M_PI/2.0;
  }

  return a;
}
double Squareroot(double x){
  return sqrt(x);
}
double Exp(double x){
  return exp(x);
}
_Bool DivisibleBy(double a, double b){
  return ((fmod(a, b)) == 0.0);
}
double Combinations(double n, double k){
  double i, j, c;

  c = 1.0;
  j = 1.0;
  i = n - k + 1.0;

  for(; i <= n; ){
    c = c*i;
    c = c/j;

    i = i + 1.0;
    j = j + 1.0;
  }

  return c;
}
double Permutations(double n, double k){
  double i, c;

  c = 1.0;

  for(i = n - k + 1.0; i <= n; i = i + 1.0){
    c = c*i;
  }

  return c;
}
_Bool EpsilonCompare(double a, double b, double epsilon){
  return fabs(a - b) < epsilon;
}
double GreatestCommonDivisor(double a, double b){
  double t;

  for(; b != 0.0; ){
    t = b;
    b = fmod(a, b);
    a = t;
  }

  return a;
}
double GCDWithSubtraction(double a, double b){
  double g;

  if(a == 0.0){
    g = b;
  }else{
    for(; b != 0.0; ){
      if(a > b){
        a = a - b;
      }else{
        b = b - a;
      }
    }

    g = a;
  }

  return g;
}
_Bool IsInteger(double a){
  return (a - floor(a)) == 0.0;
}
_Bool GreatestCommonDivisorWithCheck(double a, double b, NumberReference *gcdReference){
  _Bool success;
  double gcd;

  if(IsInteger(a) && IsInteger(b)){
    gcd = GreatestCommonDivisor(a, b);
    gcdReference->numberValue = gcd;
    success = true;
  }else{
    success = false;
  }

  return success;
}
double LeastCommonMultiple(double a, double b){
  double lcm;

  if(a > 0.0 && b > 0.0){
    lcm = fabs(a*b)/GreatestCommonDivisor(a, b);
  }else{
    lcm = 0.0;
  }

  return lcm;
}
double Sign(double a){
  double s;

  if(a > 0.0){
    s = 1.0;
  }else if(a < 0.0){
    s =  -1.0;
  }else{
    s = 0.0;
  }

  return s;
}
double Max(double a, double b){
  return fmax(a, b);
}
double Min(double a, double b){
  return fmin(a, b);
}
double Power(double a, double b){
  return pow(a, b);
}
double Gamma(double x){
  return LanczosApproximation(x);
}
double LogGamma(double x){
  return log(Gamma(x));
}
double LanczosApproximation(double z){
  double *p;
  size_t pLength;
  double i, y, t, x;

  p = (double*)malloc(sizeof(double) * (8.0));
  pLength = 8.0;
  p[0] = 676.5203681218851;
  p[1] =  -1259.1392167224028;
  p[2] = 771.32342877765313;
  p[3] =  -176.61502916214059;
  p[4] = 12.507343278686905;
  p[5] =  -0.13857109526572012;
  p[6] = 9.9843695780195716e-6;
  p[7] = 1.5056327351493116e-7;

  if(z < 0.5){
    y = M_PI/(sin(M_PI*z)*LanczosApproximation(1.0 - z));
  }else{
    z = z - 1.0;
    x = 0.99999999999980993;
    for(i = 0.0; i < pLength; i = i + 1.0){
      x = x + p[(int)(i)]/(z + i + 1.0);
    }
    t = z + pLength - 0.5;
    y = sqrt(2.0*M_PI)*pow(t, z + 0.5)*exp( -t)*x;
  }

  return y;
}
double Beta(double x, double y){
  return Gamma(x)*Gamma(y)/Gamma(x + y);
}
double Sinh(double x){
  return (exp(x) - exp( -x))/2.0;
}
double Cosh(double x){
  return (exp(x) + exp( -x))/2.0;
}
double Tanh(double x){
  return Sinh(x)/Cosh(x);
}
double Cot(double x){
  return 1.0/tan(x);
}
double Sec(double x){
  return 1.0/cos(x);
}
double Csc(double x){
  return 1.0/sin(x);
}
double Coth(double x){
  return Cosh(x)/Sinh(x);
}
double Sech(double x){
  return 1.0/Cosh(x);
}
double Csch(double x){
  return 1.0/Sinh(x);
}
double Error(double x){
  double y, t, tau, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;

  if(x == 0.0){
    y = 0.0;
  }else if(x < 0.0){
    y =  -Error( -x);
  }else{
    c1 =  -1.26551223;
    c2 =  +1.00002368;
    c3 =  +0.37409196;
    c4 =  +0.09678418;
    c5 =  -0.18628806;
    c6 =  +0.27886807;
    c7 =  -1.13520398;
    c8 =  +1.48851587;
    c9 =  -0.82215223;
    c10 =  +0.17087277;

    t = 1.0/(1.0 + 0.5*fabs(x));

    tau = t*exp( -pow(x, 2.0) + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))));

    y = 1.0 - tau;
  }

  return y;
}
double ErrorInverse(double x){
  double y, a, t;

  a = (8.0*(M_PI - 3.0))/(3.0*M_PI*(4.0 - M_PI));

  t = 2.0/(M_PI*a) + log(1.0 - pow(x, 2.0))/2.0;
  y = Sign(x)*sqrt(sqrt(pow(t, 2.0) - log(1.0 - pow(x, 2.0))/a) - t);

  return y;
}
double FallingFactorial(double x, double n){
  double k, y;

  y = 1.0;

  for(k = 0.0; k <= n - 1.0; k = k + 1.0){
    y = y*(x - k);
  }

  return y;
}
double RisingFactorial(double x, double n){
  double k, y;

  y = 1.0;

  for(k = 0.0; k <= n - 1.0; k = k + 1.0){
    y = y*(x + k);
  }

  return y;
}
double Hypergeometric(double a, double b, double c, double z, double maxIterations, double precision){
  double y;

  if(fabs(z) >= 0.5){
    y = pow(1.0 - z,  -a)*HypergeometricDirect(a, c - b, c, z/(z - 1.0), maxIterations, precision);
  }else{
    y = HypergeometricDirect(a, b, c, z, maxIterations, precision);
  }

  return y;
}
double HypergeometricDirect(double a, double b, double c, double z, double maxIterations, double precision){
  double y, yp, n;
  _Bool done;

  y = 0.0;
  done = false;

  for(n = 0.0; n < maxIterations &&  !done ; n = n + 1.0){
    yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*pow(z, n)/Factorial(n);
    if(fabs(yp) < precision){
      done = true;
    }
    y = y + yp;
  }

  return y;
}
double BernouilliNumber(double n){
  return AkiyamaTanigawaAlgorithm(n);
}
double AkiyamaTanigawaAlgorithm(double n){
  double m, j, B;
  double *A;
  size_t ALength;

  A = (double*)malloc(sizeof(double) * (n + 1.0));
  ALength = n + 1.0;

  for(m = 0.0; m <= n; m = m + 1.0){
    A[(int)(m)] = 1.0/(m + 1.0);
    for(j = m; j >= 1.0; j = j - 1.0){
      A[(int)(j - 1.0)] = j*(A[(int)(j - 1.0)] - A[(int)(j)]);
    }
  }

  B = A[0];

  free(A);

  return B;
}
wchar_t charToLowerCase(wchar_t character){
  wchar_t toReturn;

  toReturn = character;
  if(character == 'A'){
    toReturn = 'a';
  }else if(character == 'B'){
    toReturn = 'b';
  }else if(character == 'C'){
    toReturn = 'c';
  }else if(character == 'D'){
    toReturn = 'd';
  }else if(character == 'E'){
    toReturn = 'e';
  }else if(character == 'F'){
    toReturn = 'f';
  }else if(character == 'G'){
    toReturn = 'g';
  }else if(character == 'H'){
    toReturn = 'h';
  }else if(character == 'I'){
    toReturn = 'i';
  }else if(character == 'J'){
    toReturn = 'j';
  }else if(character == 'K'){
    toReturn = 'k';
  }else if(character == 'L'){
    toReturn = 'l';
  }else if(character == 'M'){
    toReturn = 'm';
  }else if(character == 'N'){
    toReturn = 'n';
  }else if(character == 'O'){
    toReturn = 'o';
  }else if(character == 'P'){
    toReturn = 'p';
  }else if(character == 'Q'){
    toReturn = 'q';
  }else if(character == 'R'){
    toReturn = 'r';
  }else if(character == 'S'){
    toReturn = 's';
  }else if(character == 'T'){
    toReturn = 't';
  }else if(character == 'U'){
    toReturn = 'u';
  }else if(character == 'V'){
    toReturn = 'v';
  }else if(character == 'W'){
    toReturn = 'w';
  }else if(character == 'X'){
    toReturn = 'x';
  }else if(character == 'Y'){
    toReturn = 'y';
  }else if(character == 'Z'){
    toReturn = 'z';
  }

  return toReturn;
}
wchar_t charToUpperCase(wchar_t character){
  wchar_t toReturn;

  toReturn = character;
  if(character == 'a'){
    toReturn = 'A';
  }else if(character == 'b'){
    toReturn = 'B';
  }else if(character == 'c'){
    toReturn = 'C';
  }else if(character == 'd'){
    toReturn = 'D';
  }else if(character == 'e'){
    toReturn = 'E';
  }else if(character == 'f'){
    toReturn = 'F';
  }else if(character == 'g'){
    toReturn = 'G';
  }else if(character == 'h'){
    toReturn = 'H';
  }else if(character == 'i'){
    toReturn = 'I';
  }else if(character == 'j'){
    toReturn = 'J';
  }else if(character == 'k'){
    toReturn = 'K';
  }else if(character == 'l'){
    toReturn = 'L';
  }else if(character == 'm'){
    toReturn = 'M';
  }else if(character == 'n'){
    toReturn = 'N';
  }else if(character == 'o'){
    toReturn = 'O';
  }else if(character == 'p'){
    toReturn = 'P';
  }else if(character == 'q'){
    toReturn = 'Q';
  }else if(character == 'r'){
    toReturn = 'R';
  }else if(character == 's'){
    toReturn = 'S';
  }else if(character == 't'){
    toReturn = 'T';
  }else if(character == 'u'){
    toReturn = 'U';
  }else if(character == 'v'){
    toReturn = 'V';
  }else if(character == 'w'){
    toReturn = 'W';
  }else if(character == 'x'){
    toReturn = 'X';
  }else if(character == 'y'){
    toReturn = 'Y';
  }else if(character == 'z'){
    toReturn = 'Z';
  }

  return toReturn;
}
_Bool charIsUpperCase(wchar_t character){
  _Bool isUpper;

  isUpper = false;
  if(character == 'A'){
    isUpper = true;
  }else if(character == 'B'){
    isUpper = true;
  }else if(character == 'C'){
    isUpper = true;
  }else if(character == 'D'){
    isUpper = true;
  }else if(character == 'E'){
    isUpper = true;
  }else if(character == 'F'){
    isUpper = true;
  }else if(character == 'G'){
    isUpper = true;
  }else if(character == 'H'){
    isUpper = true;
  }else if(character == 'I'){
    isUpper = true;
  }else if(character == 'J'){
    isUpper = true;
  }else if(character == 'K'){
    isUpper = true;
  }else if(character == 'L'){
    isUpper = true;
  }else if(character == 'M'){
    isUpper = true;
  }else if(character == 'N'){
    isUpper = true;
  }else if(character == 'O'){
    isUpper = true;
  }else if(character == 'P'){
    isUpper = true;
  }else if(character == 'Q'){
    isUpper = true;
  }else if(character == 'R'){
    isUpper = true;
  }else if(character == 'S'){
    isUpper = true;
  }else if(character == 'T'){
    isUpper = true;
  }else if(character == 'U'){
    isUpper = true;
  }else if(character == 'V'){
    isUpper = true;
  }else if(character == 'W'){
    isUpper = true;
  }else if(character == 'X'){
    isUpper = true;
  }else if(character == 'Y'){
    isUpper = true;
  }else if(character == 'Z'){
    isUpper = true;
  }

  return isUpper;
}
_Bool charIsLowerCase(wchar_t character){
  _Bool isLower;

  isLower = false;
  if(character == 'a'){
    isLower = true;
  }else if(character == 'b'){
    isLower = true;
  }else if(character == 'c'){
    isLower = true;
  }else if(character == 'd'){
    isLower = true;
  }else if(character == 'e'){
    isLower = true;
  }else if(character == 'f'){
    isLower = true;
  }else if(character == 'g'){
    isLower = true;
  }else if(character == 'h'){
    isLower = true;
  }else if(character == 'i'){
    isLower = true;
  }else if(character == 'j'){
    isLower = true;
  }else if(character == 'k'){
    isLower = true;
  }else if(character == 'l'){
    isLower = true;
  }else if(character == 'm'){
    isLower = true;
  }else if(character == 'n'){
    isLower = true;
  }else if(character == 'o'){
    isLower = true;
  }else if(character == 'p'){
    isLower = true;
  }else if(character == 'q'){
    isLower = true;
  }else if(character == 'r'){
    isLower = true;
  }else if(character == 's'){
    isLower = true;
  }else if(character == 't'){
    isLower = true;
  }else if(character == 'u'){
    isLower = true;
  }else if(character == 'v'){
    isLower = true;
  }else if(character == 'w'){
    isLower = true;
  }else if(character == 'x'){
    isLower = true;
  }else if(character == 'y'){
    isLower = true;
  }else if(character == 'z'){
    isLower = true;
  }

  return isLower;
}
_Bool charIsLetter(wchar_t character){
  return charIsUpperCase(character) || charIsLowerCase(character);
}
_Bool charIsNumber(wchar_t character){
  _Bool isNumberx;

  isNumberx = false;
  if(character == '0'){
    isNumberx = true;
  }else if(character == '1'){
    isNumberx = true;
  }else if(character == '2'){
    isNumberx = true;
  }else if(character == '3'){
    isNumberx = true;
  }else if(character == '4'){
    isNumberx = true;
  }else if(character == '5'){
    isNumberx = true;
  }else if(character == '6'){
    isNumberx = true;
  }else if(character == '7'){
    isNumberx = true;
  }else if(character == '8'){
    isNumberx = true;
  }else if(character == '9'){
    isNumberx = true;
  }

  return isNumberx;
}
_Bool charIsWhiteSpace(wchar_t character){
  _Bool isWhiteSpacex;

  isWhiteSpacex = false;
  if(character == ' '){
    isWhiteSpacex = true;
  }else if(character == '\t'){
    isWhiteSpacex = true;
  }else if(character == '\n'){
    isWhiteSpacex = true;
  }else if(character == '\r'){
    isWhiteSpacex = true;
  }

  return isWhiteSpacex;
}
_Bool charIsSymbol(wchar_t character){
  _Bool isSymbolx;

  isSymbolx = false;
  if(character == '!'){
    isSymbolx = true;
  }else if(character == '\"'){
    isSymbolx = true;
  }else if(character == '#'){
    isSymbolx = true;
  }else if(character == '$'){
    isSymbolx = true;
  }else if(character == '%'){
    isSymbolx = true;
  }else if(character == '&'){
    isSymbolx = true;
  }else if(character == '\''){
    isSymbolx = true;
  }else if(character == '('){
    isSymbolx = true;
  }else if(character == ')'){
    isSymbolx = true;
  }else if(character == '*'){
    isSymbolx = true;
  }else if(character == '+'){
    isSymbolx = true;
  }else if(character == ','){
    isSymbolx = true;
  }else if(character == '-'){
    isSymbolx = true;
  }else if(character == '.'){
    isSymbolx = true;
  }else if(character == '/'){
    isSymbolx = true;
  }else if(character == ':'){
    isSymbolx = true;
  }else if(character == ';'){
    isSymbolx = true;
  }else if(character == '<'){
    isSymbolx = true;
  }else if(character == '='){
    isSymbolx = true;
  }else if(character == '>'){
    isSymbolx = true;
  }else if(character == '?'){
    isSymbolx = true;
  }else if(character == '@'){
    isSymbolx = true;
  }else if(character == '['){
    isSymbolx = true;
  }else if(character == '\\'){
    isSymbolx = true;
  }else if(character == ']'){
    isSymbolx = true;
  }else if(character == '^'){
    isSymbolx = true;
  }else if(character == '_'){
    isSymbolx = true;
  }else if(character == '`'){
    isSymbolx = true;
  }else if(character == '{'){
    isSymbolx = true;
  }else if(character == '|'){
    isSymbolx = true;
  }else if(character == '}'){
    isSymbolx = true;
  }else if(character == '~'){
    isSymbolx = true;
  }

  return isSymbolx;
}
_Bool charCharacterIsBefore(wchar_t a, wchar_t b){
  double ad, bd;

  ad = a;
  bd = b;

  return ad < bd;
}

