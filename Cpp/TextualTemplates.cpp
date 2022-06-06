// Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

#include "TextualTemplates.hpp"

LinkedListNodes *CreateLinkedListNodes(){
  LinkedListNodes *ll;

  ll = new LinkedListNodes();
  ll->first = new LinkedListNodeNodes();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddNode(LinkedListNodes *ll, Node *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new LinkedListNodeNodes();
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
vector<Node*> *LinkedListNodesToArray(LinkedListNodes *ll){
  vector<Node*> *array;
  double length, i;
  LinkedListNodeNodes *node;

  node = ll->first;

  length = LinkedListNodesLength(ll);

  array = new vector<Node*> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = node->value;
    node = node->next;
  }

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
    delete prev;
  }

  delete node;
}
bool IsValidTemplate(vector<wchar_t> *templatex){
  StringReference *errorMessage;
  bool success;
  LinkedListStrings *tokens;
  vector<StringReference*> *ts;
  Node *root;

  tokens = CreateLinkedListString();
  errorMessage = new StringReference();

  success = GenerateTokensFromTemplate(templatex, tokens, errorMessage);

  if(success){
    root = new Node();

    ts = LinkedListStringsToArray(tokens);

    success = ParseTemplate(ts, root, errorMessage);
  }

  if( !success ){
    FreeStringReference(errorMessage);
  }

  return success;
}
bool GenerateTokensFromTemplate(vector<wchar_t> *templatex, LinkedListStrings *tokens, StringReference *errorMessage){
  bool success, found;
  double i, j, l;
  wchar_t pc, c;
  vector<wchar_t> *cs, *a;
  DynamicArrayCharacters *da;

  success = true;
  cs = new vector<wchar_t> (4.0);

  da = CreateDynamicArrayCharacters();

  pc = 'x';
  for(i = 0.0; i < templatex->size() && success; ){
    c = templatex->at(i);

    if(c != '{'){
      DynamicArrayAddCharacter(da, c);
      i = i + 1.0;
    }else{
      FillString(cs, 'x');
      for(j = 0.0; i + j < templatex->size() && j < cs->size(); j = j + 1.0){
        cs->at(j) = templatex->at(i + j);
      }

      if(StringsEqual(cs, toVector(L"{use")) || StringsEqual(cs, toVector(L"{end")) || StringsEqual(cs, toVector(L"{pri")) || StringsEqual(cs, toVector(L"{for")) || StringsEqual(cs, toVector(L"{if ")) || StringsEqual(cs, toVector(L"{els"))){
        if(pc != '\\'){
          /* Find end. */
          found = false;
          for(l = 0.0; i + l < templatex->size() &&  !found ; l = l + 1.0){
            if(templatex->at(i + l) == '}'){
              found = true;
            }
          }

          if(found){
            if(da->length > 0.0){
              a = DynamicArrayCharactersToArray(da);
              LinkedListAddString(tokens, a);
              FreeDynamicArrayCharacters(da);
              da = CreateDynamicArrayCharacters();
            }

            for(j = 0.0; j < l; j = j + 1.0){
              DynamicArrayAddCharacter(da, templatex->at(i + j));
            }

            a = DynamicArrayCharactersToArray(da);
            LinkedListAddString(tokens, a);
            FreeDynamicArrayCharacters(da);
            da = CreateDynamicArrayCharacters();

            i = i + l;
          }else{
            success = false;
            errorMessage->string = toVector(L"Template command found, but not ended properly.");
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
    a = DynamicArrayCharactersToArray(da);
    LinkedListAddString(tokens, a);
  }

  FreeDynamicArrayCharacters(da);

  return success;
}
bool GenerateDocument(vector<wchar_t> *templatex, Element *data, StringReference *document, StringReference *errorMessage){
  LinkedListCharacters *ll;
  bool success;
  LinkedListStrings *tokens;
  vector<StringReference*> *ts;
  Node *root;

  tokens = CreateLinkedListString();

  success = GenerateTokensFromTemplate(templatex, tokens, errorMessage);

  if(success){
    root = new Node();

    ts = LinkedListStringsToArray(tokens);
    FreeLinkedListString(tokens);

    success = ParseTemplate(ts, root, errorMessage);

    if(success){
      ll = CreateLinkedListCharacter();

      success = GenerateDocumentFromBlock(root, data, ll, errorMessage);

      if(success){
        document->string = LinkedListCharactersToArray(ll);
        FreeLinkedListCharacter(ll);
      }
    }
  }

  return success;
}
bool GenerateDocumentFromBlock(Node *root, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  LinkedListNodeNodes *n;
  bool success;

  n = root->nodes->first;
  success = true;

  for(;  !n->end  && success; ){
    success = GenerateDocumentFromNode(n->value, data, ll, errorMessage);
    n = n->next;
  }

  return success;
}
bool GenerateDocumentFromNode(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  bool success;
  BooleanReference *found;
  vector<wchar_t> *valueString;
  Element *value;

  success = true;
  found = new BooleanReference();

  if(StringsEqual(n->type, toVector(L"block"))){
    success = GenerateDocumentFromBlock(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, toVector(L"use"))){
    success = GenerateDocumentFromUse(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, toVector(L"if"))){
    success = GenerateDocumentFromIf(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, toVector(L"foreach"))){
    success = GenerateDocumentFromForeach(n, data, ll, errorMessage);
  }else if(StringsEqual(n->type, toVector(L"text"))){
    success = true;
    LinkedListCharactersAddString(ll, n->p1);
  }else if(StringsEqual(n->type, toVector(L"print"))){
    if(StringsEqual(data->type, toVector(L"object"))){
      value = GetObjectValueWithCheck(data->object, n->p1, found);

      if(found->booleanValue){
        if(StringsEqual(value->type, toVector(L"string"))){
          valueString = value->string;
        }else{
          valueString = WriteJSON(value);
        }
        LinkedListCharactersAddString(ll, valueString);
      }else{
        success = false;
        errorMessage->string = toVector(L"Key for printing not found in JSON object: ");
        errorMessage->string = sConcatenateString(errorMessage->string, n->p1);
      }
    }else{
      success = false;
      errorMessage->string = toVector(L"Data structure for print command is not a JSON object.");
    }
  }

  return success;
}
bool GenerateDocumentFromUse(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  bool success;
  BooleanReference *found;
  Element *value;

  found = new BooleanReference();

  if(StringsEqual(data->type, toVector(L"object"))){
    value = GetObjectValueWithCheck(data->object, n->p1, found);

    if(found->booleanValue){
      success = GenerateDocumentFromNode(n->block1, value, ll, errorMessage);
    }else{
      success = false;
      errorMessage->string = toVector(L"Key for use not found in JSON object.");
    }
  }else{
    success = false;
    errorMessage->string = toVector(L"Data structure for use command is not a JSON object.");
  }

  return success;
}
bool GenerateDocumentFromIf(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  bool success;
  BooleanReference *found;
  Element *value;

  success = true;
  found = new BooleanReference();

  if(StringsEqual(data->type, toVector(L"object"))){
    value = GetObjectValueWithCheck(data->object, n->p1, found);

    if(found->booleanValue){
      if(StringsEqual(value->type, toVector(L"boolean"))){
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
        errorMessage->string = toVector(L"Value for if not boolean.");
      }
    }else{
      success = false;
      errorMessage->string = toVector(L"Key for if not found in JSON object: ");
      errorMessage->string = sConcatenateString(errorMessage->string, n->p1);
    }
  }else{
    success = false;
    errorMessage->string = toVector(L"Data structure for if command is not a JSON object.");
  }

  return success;
}
bool GenerateDocumentFromForeach(Node *n, Element *data, LinkedListCharacters *ll, StringReference *errorMessage){
  bool success;
  BooleanReference *found;
  Element *value, *loopVar, *arrayElement;
  double i;

  success = true;
  found = new BooleanReference();
  loopVar = CreateObjectElement(0.0);

  PutStringElementMap(loopVar->object, n->p1, new Element());

  if(StringsEqual(data->type, toVector(L"object"))){
    value = GetObjectValueWithCheck(data->object, n->p2, found);

    if(found->booleanValue){
      if(StringsEqual(value->type, toVector(L"array"))){

        for(i = 0.0; i < value->array->size(); i = i + 1.0){
          arrayElement = value->array->at(i);
          if(StringsEqual(arrayElement->type, toVector(L"object"))){
            success = GenerateDocumentFromBlock(n->block1, arrayElement, ll, errorMessage);
          }else{
            SetStringElementMap(loopVar->object, 0.0, n->p1, arrayElement);
            success = GenerateDocumentFromBlock(n->block1, loopVar, ll, errorMessage);
          }
        }
      }else{
        success = false;
        errorMessage->string = toVector(L"Value for foreach is not an array.");
      }
    }else{
      success = false;
      errorMessage->string = toVector(L"Key for foreach not found in JSON object: ");
      errorMessage->string = sConcatenateString(errorMessage->string, n->p2);
    }
  }else{
    success = false;
    errorMessage->string = toVector(L"Data structure for foreach command is not a JSON object.");
  }

  return success;
}
bool ParseTemplate(vector<StringReference*> *tokens, Node *node, StringReference *errorMessage){
  bool success;
  NumberReference *position;

  position = CreateNumberReference(0.0);
  success = ParseTemplateBlock(tokens, position, node, errorMessage);

  if(success){
    if(position->numberValue != tokens->size()){
      success = false;
      errorMessage->string = toVector(L"Unexpected token at the end of template.");
    }
  }

  delete position;

  return success;
}
bool ParseTemplateBlock(vector<StringReference*> *tokens, NumberReference *position, Node *node, StringReference *errorMessage){
  bool success, done;
  Node *tn, *nb;

  success = true;
  done = false;

  node->type = toVector(L"block");
  node->nodes = CreateLinkedListNodes();

  for(; position->numberValue < tokens->size() && success &&  !done ; ){
    tn = new Node();
    success = ParseNodeString(tokens->at(position->numberValue)->string, tn, errorMessage);

    if(success){
      if(StringsEqual(tn->type, toVector(L"text")) || StringsEqual(tn->type, toVector(L"print"))){
        LinkedListAddNode(node->nodes, tn);
        position->numberValue = position->numberValue + 1.0;
      }else if(StringsEqual(tn->type, toVector(L"use"))){
        nb = new Node();
        success = ParseUseBlock(tokens, position, nb, errorMessage);
        LinkedListAddNode(node->nodes, nb);
      }else if(StringsEqual(tn->type, toVector(L"if"))){
        nb = new Node();
        success = ParseIfBlock(tokens, position, nb, errorMessage);
        LinkedListAddNode(node->nodes, nb);
      }else if(StringsEqual(tn->type, toVector(L"foreach"))){
        nb = new Node();
        success = ParseForeachBlock(tokens, position, nb, errorMessage);
        LinkedListAddNode(node->nodes, nb);
      }else{
        done = true;
      }
    }
  }

  return success;
}
bool ParseUseBlock(vector<StringReference*> *tokens, NumberReference *position, Node *useBlock, StringReference *errorMessage){
  bool success;
  Node *n;

  n = new Node();
  ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

  useBlock->type = CopyString(n->type);
  useBlock->p1 = CopyString(n->p1);
  useBlock->block1 = new Node();

  position->numberValue = position->numberValue + 1.0;

  success = ParseTemplateBlock(tokens, position, useBlock->block1, errorMessage);

  if(success){
    if(position->numberValue < tokens->size()){
      n = new Node();
      ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

      if(StringsEqual(n->type, toVector(L"end"))){
        success = true;
        position->numberValue = position->numberValue + 1.0;
      }else{
        success = false;
        errorMessage->string = toVector(L"End block expected at the end of use block.");
      }
    }else{
      success = false;
      errorMessage->string = toVector(L"End block expected at the end of use block.");
    }
  }

  return success;
}
bool ParseIfBlock(vector<StringReference*> *tokens, NumberReference *position, Node *ifBlock, StringReference *errorMessage){
  bool success;
  Node *n;

  n = new Node();
  ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

  ifBlock->type = CopyString(n->type);
  ifBlock->p1 = CopyString(n->p1);
  ifBlock->block1 = new Node();
  ifBlock->hasElseBlock = false;

  position->numberValue = position->numberValue + 1.0;

  success = ParseTemplateBlock(tokens, position, ifBlock->block1, errorMessage);

  if(success){
    if(position->numberValue < tokens->size()){
      n = new Node();
      ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

      if(StringsEqual(n->type, toVector(L"end"))){
        success = true;
        position->numberValue = position->numberValue + 1.0;
      }else if(StringsEqual(n->type, toVector(L"else"))){
        position->numberValue = position->numberValue + 1.0;
        ifBlock->hasElseBlock = true;
        ifBlock->block2 = new Node();
        success = ParseTemplateBlock(tokens, position, ifBlock->block2, errorMessage);

        if(success){
          if(position->numberValue < tokens->size()){
            n = new Node();
            ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

            if(StringsEqual(n->type, toVector(L"end"))){
              success = true;
              position->numberValue = position->numberValue + 1.0;
            }else{
              success = false;
              errorMessage->string = toVector(L"End block expected at the end of else block.");
            }
          }else{
            success = false;
            errorMessage->string = toVector(L"End block expected at the end of else block.");
          }
        }
      }else{
        success = false;
        errorMessage->string = toVector(L"End or else block expected at the end of if block.");
      }
    }else{
      success = false;
      errorMessage->string = toVector(L"End or else block expected at the end of if block.");
    }
  }

  return success;
}
bool ParseForeachBlock(vector<StringReference*> *tokens, NumberReference *position, Node *foreachBlock, StringReference *errorMessage){
  bool success;
  Node *n;

  n = new Node();
  ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

  foreachBlock->type = CopyString(n->type);
  foreachBlock->p1 = CopyString(n->p1);
  foreachBlock->p2 = CopyString(n->p2);
  foreachBlock->block1 = new Node();

  position->numberValue = position->numberValue + 1.0;

  success = ParseTemplateBlock(tokens, position, foreachBlock->block1, errorMessage);

  if(success){
    if(position->numberValue < tokens->size()){
      n = new Node();
      ParseNodeString(tokens->at(position->numberValue)->string, n, errorMessage);

      if(StringsEqual(n->type, toVector(L"end"))){
        success = true;
        position->numberValue = position->numberValue + 1.0;
      }else{
        success = false;
        errorMessage->string = toVector(L"End block expected at the end of for each block.");
      }
    }else{
      success = false;
      errorMessage->string = toVector(L"End block expected at the end of foreach block.");
    }
  }

  return success;
}
bool ParseNodeString(vector<wchar_t> *token, Node *node, StringReference *errorMessage){
  vector<wchar_t> *command;
  bool success, isText;
  vector<StringReference*> *parts;

  success = true;
  isText = false;

  if(token->size() <= 2.0){
    isText = true;
  }else if(token->at(0) == '\\' && token->at(1) == '{'){
    isText = true;
  }else if(token->at(0) != '{'){
    isText = true;
  }else{
    command = strSubstring(token, 1.0, token->size() - 1.0);
    parts = sSplitByCharacter(command, ' ');

    if(command->size() > 0.0){
      if(StringsEqual(parts->at(0)->string, toVector(L"use"))){
        if(parts->size() == 2.0){
          node->type = CopyString(parts->at(0)->string);
          node->p1 = CopyString(parts->at(1)->string);
        }else{
          success = false;
          errorMessage->string = toVector(L"The use command takes one parameter.");
        }
      }else if(StringsEqual(parts->at(0)->string, toVector(L"end"))){
        if(parts->size() == 1.0){
          node->type = CopyString(parts->at(0)->string);
        }else{
          success = false;
          errorMessage->string = toVector(L"The end command takes no parameters.");
        }
      }else if(StringsEqual(parts->at(0)->string, toVector(L"print"))){
        if(parts->size() == 2.0){
          node->type = CopyString(parts->at(0)->string);
          node->p1 = CopyString(parts->at(1)->string);
        }else{
          success = false;
          errorMessage->string = toVector(L"The print command takes one parameter.");
        }
      }else if(StringsEqual(parts->at(0)->string, toVector(L"foreach"))){
        if(parts->size() == 4.0){
          if(StringsEqual(parts->at(2)->string, toVector(L"in"))){
            node->type = CopyString(parts->at(0)->string);
            node->p1 = CopyString(parts->at(1)->string);
            node->p2 = CopyString(parts->at(3)->string);
          }else{
            success = false;
            errorMessage->string = toVector(L"The foreach command must have \"in\" after the first parameter.");
          }
        }else{
          success = false;
          errorMessage->string = toVector(L"The foreach command takes three parameters.");
        }
      }else if(StringsEqual(parts->at(0)->string, toVector(L"if"))){
        if(parts->size() == 2.0){
          node->type = CopyString(parts->at(0)->string);
          node->p1 = CopyString(parts->at(1)->string);
        }else{
          success = false;
          errorMessage->string = toVector(L"The if command takes one parameter.");
        }
      }else if(StringsEqual(parts->at(0)->string, toVector(L"else"))){
        if(parts->size() == 1.0){
          node->type = CopyString(parts->at(0)->string);
        }else{
          success = false;
          errorMessage->string = toVector(L"The else command takes no parameters.");
        }
      }else{
        isText = true;
      }
    }else{
      isText = true;
    }
  }

  if(isText){
    node->type = toVector(L"text");
    node->p1 = sReplaceString(token, toVector(L"\\{print "), toVector(L"{print "));
    node->p1 = sReplaceString(node->p1, toVector(L"\\{use "), toVector(L"{use "));
    node->p1 = sReplaceString(node->p1, toVector(L"\\{if "), toVector(L"{if "));
    node->p1 = sReplaceString(node->p1, toVector(L"\\{end}"), toVector(L"{end}"));
    node->p1 = sReplaceString(node->p1, toVector(L"\\{foreach "), toVector(L"{foreach "));
    node->p1 = sReplaceString(node->p1, toVector(L"\\{else}"), toVector(L"{else}"));
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
  vector<wchar_t> *templatex;
  LinkedListStrings *tokens;
  bool success;
  StringReference *errorMessage;

  errorMessage = new StringReference();

  tokens = CreateLinkedListString();
  templatex = toVector(L"This is a template, this is a value: {print a}.");
  success = GenerateTokensFromTemplate(templatex, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(3.0, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  templatex = toVector(L"This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.");
  success = GenerateTokensFromTemplate(templatex, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(9.0, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  templatex = toVector(L"This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.");
  success = GenerateTokensFromTemplate(templatex, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(16.0, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  templatex = toVector(L"T: {foreach a in b}{print a}{end}.");
  success = GenerateTokensFromTemplate(templatex, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(5.0, LinkedListStringsLength(tokens), failures);
}
void testGenerateDocument1(NumberReference *failures){
  AssertTemplateResult(toVector(L"This is a template, this is a value: {print a}."), toVector(L"{\"c\": 5, \"a\": 6}"), toVector(L"This is a template, this is a value: 6."), failures);
}
void testGenerateDocument2(NumberReference *failures){
  AssertTemplateResult(toVector(L"This is a template, this is a value: {print a} {use b}{print a} {print b}{end}."), toVector(L"{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}"), toVector(L"This is a template, this is a value: 6 1 2."), failures);
}
void testGenerateDocument3(NumberReference *failures){
  AssertTemplateResult(toVector(L"This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}."), toVector(L"{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}"), toVector(L"This is a template, this is a value: 6 1 23 416."), failures);
}
void testGenerateDocument4(NumberReference *failures){
  AssertTemplateResult(toVector(L"T: {if a}a{end}."), toVector(L"{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}"), toVector(L"T: a."), failures);

  AssertTemplateResult(toVector(L"T: {if a}a{else}b{end}."), toVector(L"{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}"), toVector(L"T: b."), failures);
}
void testGenerateDocument5(NumberReference *failures){
  AssertTemplateResult(toVector(L"T: {foreach a in b}{print a}{end}."), toVector(L"{\"b\": [1, 2, 3, 4]}"), toVector(L"T: 1234."), failures);
}
void testGenerateDocument6(NumberReference *failures){
  AssertTemplateResult(toVector(L"{test}\\{print a}."), toVector(L"{\"c\": 5, \"a\": 6}"), toVector(L"{test}{print a}."), failures);
}
void testGenerateDocument7(NumberReference *failures){
  AssertTemplateResult(toVector(L"{"), toVector(L"{}"), toVector(L"{"), failures);

  AssertTemplateResult(toVector(L"{  "), toVector(L"{}"), toVector(L"{  "), failures);

  AssertTemplateResult(toVector(L"{use a}\\{print a}{end}"), toVector(L"{\"a\": {}}"), toVector(L"{print a}"), failures);

  AssertTemplateResult(toVector(L"\\{print a}{print a}}"), toVector(L"{\"a\": 3}"), toVector(L"{print a}3}"), failures);

  AssertTemplateResult(toVector(L"\\\\{print a}{print a}}"), toVector(L"{\"a\": 3}"), toVector(L"\\{print a}3}"), failures);

  AssertTemplateResult(toVector(L"\\\\{print a}{print a}\\\\{print a}}"), toVector(L"{\"a\": 3}"), toVector(L"\\{print a}3\\{print a}}"), failures);

  AssertTemplateResult(toVector(L"\\{print a}{print a}\\{"), toVector(L"{\"a\": 3}"), toVector(L"{print a}3\\{"), failures);

  AssertTemplateResult(toVector(L"    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>"), toVector(L"{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}"), toVector(L"    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>"), failures);

  AssertTemplateError(toVector(L"{print"), toVector(L"{}"), toVector(L"Template command found, but not ended properly."), failures);

  AssertTemplateError(toVector(L"{print a}"), toVector(L"{}"), toVector(L"Key for printing not found in JSON object: a"), failures);

  AssertTemplateError(toVector(L"This is a template, this is a value: {print a {print a}."), toVector(L"{\"a\": 5}"), toVector(L"The print command takes one parameter."), failures);

  AssertTemplateError(toVector(L"This is a {use a}\\{print a}template, this is a value: {print a}.{end}"), toVector(L"{\"a\": 5}"), toVector(L"Data structure for print command is not a JSON object."), failures);

  AssertTemplateError(toVector(L"{use a}"), toVector(L"{}"), toVector(L"End block expected at the end of use block."), failures);

  AssertTemplateError(toVector(L"{if a}"), toVector(L"{\"a\": true}"), toVector(L"End or else block expected at the end of if block."), failures);

  AssertTemplateError(toVector(L"{if a}{else}"), toVector(L"{\"a\": true}"), toVector(L"End block expected at the end of else block."), failures);

  AssertTemplateError(toVector(L"{foreach x in a}"), toVector(L"{\"a\": [1, 2, 3, 4]}"), toVector(L"End block expected at the end of foreach block."), failures);
}
void AssertTemplateResult(vector<wchar_t> *templatex, vector<wchar_t> *json, vector<wchar_t> *result, NumberReference *failures){
  ElementReference *data;
  StringArrayReference *errorMessages;
  StringReference *document;
  StringReference *errorMessage;
  bool success;

  data = new ElementReference();
  errorMessages = new StringArrayReference();
  document = new StringReference();
  errorMessage = new StringReference();

  success = ReadJSON(json, data, errorMessages);

  AssertTrue(success, failures);

  if(success){
    success = GenerateDocument(templatex, data->element, document, errorMessage);

    AssertTrue(success, failures);

    if(success){
      AssertStringEquals(document->string, result, failures);
    }
  }
}
void AssertTemplateError(vector<wchar_t> *templatex, vector<wchar_t> *json, vector<wchar_t> *errorMessage, NumberReference *failures){
  ElementReference *data;
  StringArrayReference *errorMessages;
  StringReference *document;
  StringReference *errorMessageRef;
  bool success;

  data = new ElementReference();
  errorMessages = new StringArrayReference();
  document = new StringReference();
  errorMessageRef = new StringReference();

  success = ReadJSON(json, data, errorMessages);

  AssertTrue(success, failures);

  if(success){
    success = GenerateDocument(templatex, data->element, document, errorMessageRef);

    AssertFalse(success, failures);

    if( !success ){
      AssertStringEquals(errorMessage, errorMessageRef->string, failures);
    }
  }
}
BooleanReference *CreateBooleanReference(bool value){
  BooleanReference *ref;

  ref = new BooleanReference();
  ref->booleanValue = value;

  return ref;
}
BooleanArrayReference *CreateBooleanArrayReference(vector<bool> *value){
  BooleanArrayReference *ref;

  ref = new BooleanArrayReference();
  ref->booleanArray = value;

  return ref;
}
BooleanArrayReference *CreateBooleanArrayReferenceLengthValue(double length, bool value){
  BooleanArrayReference *ref;
  double i;

  ref = new BooleanArrayReference();
  ref->booleanArray = new vector<bool> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    ref->booleanArray->at(i) = value;
  }

  return ref;
}
void FreeBooleanArrayReference(BooleanArrayReference *booleanArrayReference){
  delete booleanArrayReference->booleanArray;
  delete booleanArrayReference;
}
CharacterReference *CreateCharacterReference(wchar_t value){
  CharacterReference *ref;

  ref = new CharacterReference();
  ref->characterValue = value;

  return ref;
}
NumberReference *CreateNumberReference(double value){
  NumberReference *ref;

  ref = new NumberReference();
  ref->numberValue = value;

  return ref;
}
NumberArrayReference *CreateNumberArrayReference(vector<double> *value){
  NumberArrayReference *ref;

  ref = new NumberArrayReference();
  ref->numberArray = value;

  return ref;
}
NumberArrayReference *CreateNumberArrayReferenceLengthValue(double length, double value){
  NumberArrayReference *ref;
  double i;

  ref = new NumberArrayReference();
  ref->numberArray = new vector<double> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    ref->numberArray->at(i) = value;
  }

  return ref;
}
void FreeNumberArrayReference(NumberArrayReference *numberArrayReference){
  delete numberArrayReference->numberArray;
  delete numberArrayReference;
}
StringReference *CreateStringReference(vector<wchar_t> *value){
  StringReference *ref;

  ref = new StringReference();
  ref->string = value;

  return ref;
}
StringReference *CreateStringReferenceLengthValue(double length, wchar_t value){
  StringReference *ref;
  double i;

  ref = new StringReference();
  ref->string = new vector<wchar_t> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    ref->string->at(i) = value;
  }

  return ref;
}
void FreeStringReference(StringReference *stringReference){
  delete stringReference->string;
  delete stringReference;
}
StringArrayReference *CreateStringArrayReference(vector<StringReference*> *strings){
  StringArrayReference *ref;

  ref = new StringArrayReference();
  ref->stringArray = strings;

  return ref;
}
StringArrayReference *CreateStringArrayReferenceLengthValue(double length, vector<wchar_t> *value){
  StringArrayReference *ref;
  double i;

  ref = new StringArrayReference();
  ref->stringArray = new vector<StringReference*> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    ref->stringArray->at(i) = CreateStringReference(value);
  }

  return ref;
}
void FreeStringArrayReference(StringArrayReference *stringArrayReference){
  double i;

  for(i = 0.0; i < stringArrayReference->stringArray->size(); i = i + 1.0){
    delete stringArrayReference->stringArray->at(i);
  }
  delete stringArrayReference->stringArray;
  delete stringArrayReference;
}
bool IsValidJSON(vector<wchar_t> *json, StringArrayReference *errorMessage){
  bool success;
  ElementReference *elementReference;

  elementReference = new ElementReference();

  success = ReadJSON(json, elementReference, errorMessage);

  if(success){
    DeleteElement(elementReference->element);
  }

  return success;
}
bool JSONTokenize(vector<wchar_t> *json, StringArrayReference *tokensReference, StringArrayReference *errorMessages){
  double i;
  wchar_t c;
  vector<wchar_t> *str;
  StringReference *stringReference, *tokenReference;
  NumberReference *stringLength;
  bool success;
  lLinkedListStrings *ll;

  ll = lCreateLinkedListString();
  success = true;

  stringLength = new NumberReference();
  tokenReference = new StringReference();

  for(i = 0.0; i < json->size() && success; ){
    c = json->at(i);

    if(c == '{'){
      lLinkedListAddString(ll, toVector(L"{"));
      i = i + 1.0;
    }else if(c == '}'){
      lLinkedListAddString(ll, toVector(L"}"));
      i = i + 1.0;
    }else if(c == '['){
      lLinkedListAddString(ll, toVector(L"["));
      i = i + 1.0;
    }else if(c == ']'){
      lLinkedListAddString(ll, toVector(L"]"));
      i = i + 1.0;
    }else if(c == ':'){
      lLinkedListAddString(ll, toVector(L":"));
      i = i + 1.0;
    }else if(c == ','){
      lLinkedListAddString(ll, toVector(L","));
      i = i + 1.0;
    }else if(c == 'f'){
      success = GetJSONPrimitiveName(json, i, errorMessages, toVector(L"false"), tokenReference);
      if(success){
        lLinkedListAddString(ll, toVector(L"false"));
        i = i + toVector(L"false")->size();
      }
    }else if(c == 't'){
      success = GetJSONPrimitiveName(json, i, errorMessages, toVector(L"true"), tokenReference);
      if(success){
        lLinkedListAddString(ll, toVector(L"true"));
        i = i + toVector(L"true")->size();
      }
    }else if(c == 'n'){
      success = GetJSONPrimitiveName(json, i, errorMessages, toVector(L"null"), tokenReference);
      if(success){
        lLinkedListAddString(ll, toVector(L"null"));
        i = i + toVector(L"null")->size();
      }
    }else if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
      /* Skip. */
      i = i + 1.0;
    }else if(c == '\"'){
      success = GetJSONString(json, i, tokenReference, stringLength, errorMessages);
      if(success){
        lLinkedListAddString(ll, tokenReference->string);
        i = i + stringLength->numberValue;
      }
    }else if(IsJSONNumberCharacter(c)){
      success = GetJSONNumberToken(json, i, tokenReference, errorMessages);
      if(success){
        lLinkedListAddString(ll, tokenReference->string);
        i = i + tokenReference->string->size();
      }
    }else{
      str = strConcatenateCharacter(toVector(L"Invalid start of Token: "), c);
      stringReference = CreateStringReference(str);
      lAddStringRef(errorMessages, stringReference);
      i = i + 1.0;
      success = false;
    }
  }

  if(success){
    lLinkedListAddString(ll, toVector(L"<end>"));
    tokensReference->stringArray = lLinkedListStringsToArray(ll);
    lFreeLinkedListString(ll);
  }

  return success;
}
bool GetJSONNumberToken(vector<wchar_t> *json, double start, StringReference *tokenReference, StringArrayReference *errorMessages){
  wchar_t c;
  double end, i;
  bool done, success;
  vector<wchar_t> *numberString;

  end = json->size();
  done = false;

  for(i = start; i < json->size() &&  !done ; i = i + 1.0){
    c = json->at(i);
    if( !IsJSONNumberCharacter(c) ){
      done = true;
      end = i;
    }
  }

  numberString = strSubstring(json, start, end);

  success = IsValidJSONNumber(numberString, errorMessages);

  tokenReference->string = numberString;

  return success;
}
bool IsValidJSONNumber(vector<wchar_t> *n, StringArrayReference *errorMessages){
  bool success;
  double i;

  i = 0.0;

  /* JSON allows an optional negative sign. */
  if(n->at(i) == '-'){
    i = i + 1.0;
  }

  if(i < n->size()){
    success = IsValidJSONNumberAfterSign(n, i, errorMessages);
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"Number must contain at least one digit.")));
  }

  return success;
}
bool IsValidJSONNumberAfterSign(vector<wchar_t> *n, double i, StringArrayReference *errorMessages){
  bool success;

  if(charIsNumber(n->at(i))){
    /* 0 first means only 0. */
    if(n->at(i) == '0'){
      i = i + 1.0;
    }else{
      /* 1-9 first, read following digits. */
      i = IsValidJSONNumberAdvancePastDigits(n, i);
    }

    if(i < n->size()){
      success = IsValidJSONNumberFromDotOrExponent(n, i, errorMessages);
    }else{
      /* If integer, we are done now. */
      success = true;
    }
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"A number must start with 0-9 (after the optional sign).")));
  }

  return success;
}
double IsValidJSONNumberAdvancePastDigits(vector<wchar_t> *n, double i){
  bool done;

  i = i + 1.0;
  done = false;
  for(; i < n->size() &&  !done ; ){
    if(charIsNumber(n->at(i))){
      i = i + 1.0;
    }else{
      done = true;
    }
  }

  return i;
}
bool IsValidJSONNumberFromDotOrExponent(vector<wchar_t> *n, double i, StringArrayReference *errorMessages){
  bool wasDotAndOrE, success;

  wasDotAndOrE = false;
  success = true;

  if(n->at(i) == '.'){
    i = i + 1.0;
    wasDotAndOrE = true;

    if(i < n->size()){
      if(charIsNumber(n->at(i))){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, i);

        if(i == n->size()){
          /* If non-scientific decimal number, we are done. */
          success = true;
        }
      }else{
        success = false;
        lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be numbers after the decimal point.")));
      }
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be numbers after the decimal point.")));
    }
  }

  if(i < n->size() && success){
    if(n->at(i) == 'e' || n->at(i) == 'E'){
      wasDotAndOrE = true;
      success = IsValidJSONNumberFromExponent(n, i, errorMessages);
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(toVector(L"Expected e or E.")));
    }
  }else if(i == n->size() && success){
    /* If number with decimal point. */
    success = true;
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be numbers after the decimal point.")));
  }

  if(wasDotAndOrE){
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"Exprected decimal point or e or E.")));
  }

  return success;
}
bool IsValidJSONNumberFromExponent(vector<wchar_t> *n, double i, StringArrayReference *errorMessages){
  bool success;

  i = i + 1.0;

  if(i < n->size()){
    /* The exponent sign can either + or -, */
    if(n->at(i) == '+' || n->at(i) == '-'){
      i = i + 1.0;
    }

    if(i < n->size()){
      if(charIsNumber(n->at(i))){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, i);

        if(i == n->size()){
          /* We found scientific number. */
          success = true;
        }else{
          success = false;
          lAddStringRef(errorMessages, CreateStringReference(toVector(L"There was characters following the exponent.")));
        }
      }else{
        success = false;
        lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be a digit following the optional exponent sign.")));
      }
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be a digit following optional the exponent sign.")));
    }
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be a sign or a digit following e or E.")));
  }

  return success;
}
bool IsJSONNumberCharacter(wchar_t c){
  vector<wchar_t> *numericCharacters;
  bool found;
  double i;

  numericCharacters = toVector(L"0123456789.-+eE");

  found = false;

  for(i = 0.0; i < numericCharacters->size(); i = i + 1.0){
    if(numericCharacters->at(i) == c){
      found = true;
    }
  }

  return found;
}
bool GetJSONPrimitiveName(vector<wchar_t> *string, double start, StringArrayReference *errorMessages, vector<wchar_t> *primitive, StringReference *tokenReference){
  wchar_t c, p;
  bool done, success;
  double i;
  vector<wchar_t> *str, *token;

  done = false;
  success = true;

  token = toVector(L"");

  for(i = start; i < string->size() && ((i - start) < primitive->size()) &&  !done ; i = i + 1.0){
    c = string->at(i);
    p = primitive->at(i - start);
    if(c == p){
      /* OK */
      if((i + 1.0 - start) == primitive->size()){
        done = true;
      }
    }else{
      str = toVector(L"");
      str = strConcatenateString(str, toVector(L"Primitive invalid: "));
      str = strAppendCharacter(str, c);
      str = strAppendString(str, toVector(L" vs "));
      str = strAppendCharacter(str, p);

      lAddStringRef(errorMessages, CreateStringReference(str));
      done = true;
      success = false;
    }
  }

  if(done){
    if(StringsEqual(primitive, toVector(L"false"))){
      token = toVector(L"false");
    }
    if(StringsEqual(primitive, toVector(L"true"))){
      token = toVector(L"true");
    }
    if(StringsEqual(primitive, toVector(L"null"))){
      token = toVector(L"null");
    }
  }else{
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"Primitive invalid")));
    success = false;
  }

  tokenReference->string = token;

  return success;
}
bool GetJSONString(vector<wchar_t> *json, double start, StringReference *tokenReference, NumberReference *stringLengthReference, StringArrayReference *errorMessages){
  bool success, done;
  vector<wchar_t> *string, *hex;
  NumberReference *characterCount, *hexReference;
  double i, l, c;
  StringReference *errorMessage;

  characterCount = CreateNumberReference(0.0);
  hex = CreateString(4.0, '0');
  hexReference = new NumberReference();
  errorMessage = new StringReference();

  success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages);

  if(success){
    l = characterCount->numberValue;
    string = new vector<wchar_t> (l);

    c = 0.0;
    string->at(c) = '\"';
    c = c + 1.0;

    done = false;
    for(i = start + 1.0;  !done ; i = i + 1.0){
      if(json->at(i) == '\\'){
        i = i + 1.0;
        if(json->at(i) == '\"' || json->at(i) == '\\' || json->at(i) == '/'){
          string->at(c) = json->at(i);
          c = c + 1.0;
        }else if(json->at(i) == 'b'){
          string->at(c) = 8.0;
          c = c + 1.0;
        }else if(json->at(i) == 'f'){
          string->at(c) = 12.0;
          c = c + 1.0;
        }else if(json->at(i) == 'n'){
          string->at(c) = 10.0;
          c = c + 1.0;
        }else if(json->at(i) == 'r'){
          string->at(c) = 13.0;
          c = c + 1.0;
        }else if(json->at(i) == 't'){
          string->at(c) = 9.0;
          c = c + 1.0;
        }else if(json->at(i) == 'u'){
          i = i + 1.0;
          hex->at(0) = charToUpperCase(json->at(i + 0.0));
          hex->at(1) = charToUpperCase(json->at(i + 1.0));
          hex->at(2) = charToUpperCase(json->at(i + 2.0));
          hex->at(3) = charToUpperCase(json->at(i + 3.0));
          nCreateNumberFromStringWithCheck(hex, 16.0, hexReference, errorMessage);
          string->at(c) = hexReference->numberValue;
          i = i + 3.0;
          c = c + 1.0;
        }
      }else if(json->at(i) == '\"'){
        string->at(c) = json->at(i);
        c = c + 1.0;
        done = true;
      }else{
        string->at(c) = json->at(i);
        c = c + 1.0;
      }
    }

    tokenReference->string = string;
    success = true;
  }else{
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"End of string was not found.")));
    success = false;
  }

  return success;
}
bool IsValidJSONString(vector<wchar_t> *jsonString, StringArrayReference *errorMessages){
  bool valid;
  NumberReference *numberReference, *stringLength;

  numberReference = new NumberReference();
  stringLength = new NumberReference();

  valid = IsValidJSONStringInJSON(jsonString, 0.0, numberReference, stringLength, errorMessages);

  return valid;
}
bool IsValidJSONStringInJSON(vector<wchar_t> *json, double start, NumberReference *characterCount, NumberReference *stringLengthReference, StringArrayReference *errorMessages){
  bool success, done;
  double i, j;
  wchar_t c;

  success = true;
  done = false;

  characterCount->numberValue = 1.0;

  for(i = start + 1.0; i < json->size() &&  !done  && success; i = i + 1.0){
    if( !IsJSONIllegalControllCharacter(json->at(i)) ){
      if(json->at(i) == '\\'){
        i = i + 1.0;
        if(i < json->size()){
          if(json->at(i) == '\"' || json->at(i) == '\\' || json->at(i) == '/' || json->at(i) == 'b' || json->at(i) == 'f' || json->at(i) == 'n' || json->at(i) == 'r' || json->at(i) == 't'){
            characterCount->numberValue = characterCount->numberValue + 1.0;
          }else if(json->at(i) == 'u'){
            if(i + 4.0 < json->size()){
              for(j = 0.0; j < 4.0 && success; j = j + 1.0){
                c = json->at(i + j + 1.0);
                if(nCharacterIsNumberCharacterInBase(c, 16.0) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f'){
                }else{
                  success = false;
                  lAddStringRef(errorMessages, CreateStringReference(toVector(L"\\u must be followed by four hexadecimal digits.")));
                }
              }
              characterCount->numberValue = characterCount->numberValue + 1.0;
              i = i + 4.0;
            }else{
              success = false;
              lAddStringRef(errorMessages, CreateStringReference(toVector(L"\\u must be followed by four characters.")));
            }
          }else{
            success = false;
            lAddStringRef(errorMessages, CreateStringReference(toVector(L"Escaped character invalid.")));
          }
        }else{
          success = false;
          lAddStringRef(errorMessages, CreateStringReference(toVector(L"There must be at least two character after string escape.")));
        }
      }else if(json->at(i) == '\"'){
        characterCount->numberValue = characterCount->numberValue + 1.0;
        done = true;
      }else{
        characterCount->numberValue = characterCount->numberValue + 1.0;
      }
    }else{
      success = false;
      lAddStringRef(errorMessages, CreateStringReference(toVector(L"Unicode code points 0-31 not allowed in JSON string.")));
    }
  }

  if(done){
    stringLengthReference->numberValue = i - start;
  }else{
    success = false;
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"String must end with \".")));
  }

  return success;
}
bool IsJSONIllegalControllCharacter(wchar_t c){
  double cnr;
  bool isControll;

  cnr = c;

  if(cnr >= 0.0 && cnr < 32.0){
    isControll = true;
  }else{
    isControll = false;
  }

  return isControll;
}
vector<Element*> *AddElement(vector<Element*> *list, Element *a){
  vector<Element*> *newlist;
  double i;

  newlist = new vector<Element*> (list->size() + 1.0);

  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;

  delete list;

  return newlist;
}
void AddElementRef(ElementArrayReference *list, Element *i){
  list->array = AddElement(list->array, i);
}
vector<Element*> *RemoveElement(vector<Element*> *list, double n){
  vector<Element*> *newlist;
  double i;

  newlist = new vector<Element*> (list->size() - 1.0);

  for(i = 0.0; i < list->size(); i = i + 1.0){
    if(i < n){
      newlist->at(i) = list->at(i);
    }
    if(i > n){
      newlist->at(i - 1.0) = list->at(i);
    }
  }

  delete list;

  return newlist;
}
Element *GetElementRef(ElementArrayReference *list, double i){
  return list->array->at(i);
}
void RemoveElementRef(ElementArrayReference *list, double i){
  list->array = RemoveElement(list->array, i);
}
LinkedListElements *CreateLinkedListElements(){
  LinkedListElements *ll;

  ll = new LinkedListElements();
  ll->first = new LinkedListNodeElements();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddElement(LinkedListElements *ll, Element *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new LinkedListNodeElements();
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
vector<Element*> *LinkedListElementsToArray(LinkedListElements *ll){
  vector<Element*> *array;
  double length, i;
  LinkedListNodeElements *node;

  node = ll->first;

  length = LinkedListElementsLength(ll);

  array = new vector<Element*> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = node->value;
    node = node->next;
  }

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
    delete prev;
  }

  delete node;
}
double ComputeJSONStringLength(Element *element){
  double result;

  result = 0.0;

  if(StringsEqual(element->type, toVector(L"object"))){
    result = result + ComputeJSONObjectStringLength(element);
  }else if(StringsEqual(element->type, toVector(L"string"))){
    result = JSONEscapedStringLength(element->string) + 2.0;
  }else if(StringsEqual(element->type, toVector(L"array"))){
    result = result + ComputeJSONArrayStringLength(element);
  }else if(StringsEqual(element->type, toVector(L"number"))){
    result = result + ComputeJSONNumberStringLength(element);
  }else if(StringsEqual(element->type, toVector(L"null"))){
    result = result + toVector(L"null")->size();
  }else if(StringsEqual(element->type, toVector(L"boolean"))){
    result = result + ComputeJSONBooleanStringLength(element);
  }

  return result;
}
double ComputeJSONBooleanStringLength(Element *element){
  double result;

  if(element->booleanValue){
    result = toVector(L"true")->size();
  }else{
    result = toVector(L"false")->size();
  }

  return result;
}
double ComputeJSONNumberStringLength(Element *element){
  double length;
  vector<wchar_t> *a;

  if(abs(element->number) >= pow(10.0, 15.0) || abs(element->number) <= pow(10.0,  -15.0)){
    a = nCreateStringScientificNotationDecimalFromNumber(element->number);
    length = a->size();
  }else{
    a = nCreateStringDecimalFromNumber(element->number);
    length = a->size();
  }

  return length;
}
double ComputeJSONArrayStringLength(Element *element){
  Element *arrayElement;
  double i;
  double length;

  length = 1.0;

  for(i = 0.0; i < element->array->size(); i = i + 1.0){
    arrayElement = element->array->at(i);

    length = length + ComputeJSONStringLength(arrayElement);

    if(i + 1.0 != element->array->size()){
      length = length + 1.0;
    }
  }

  length = length + 1.0;

  return length;
}
double ComputeJSONObjectStringLength(Element *element){
  vector<wchar_t> *key;
  double i;
  StringArrayReference *keys;
  Element *objectElement;
  double length;

  length = 1.0;

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArray->size(); i = i + 1.0){
    key = keys->stringArray->at(i)->string;
    objectElement = GetObjectValue(element->object, key);

    length = length + 1.0;
    length = length + JSONEscapedStringLength(key);
    length = length + 1.0;
    length = length + 1.0;

    length = length + ComputeJSONStringLength(objectElement);

    if(i + 1.0 != keys->stringArray->size()){
      length = length + 1.0;
    }
  }

  length = length + 1.0;

  return length;
}
Element *CreateStringElement(vector<wchar_t> *string){
  Element *element;
  element = new Element();
  element->type = toVector(L"string");
  element->string = string;
  return element;
}
Element *CreateBooleanElement(bool booleanValue){
  Element *element;
  element = new Element();
  element->type = toVector(L"boolean");
  element->booleanValue = booleanValue;
  return element;
}
Element *CreateNullElement(){
  Element *element;
  element = new Element();
  element->type = toVector(L"null");
  return element;
}
Element *CreateNumberElement(double number){
  Element *element;
  element = new Element();
  element->type = toVector(L"number");
  element->number = number;
  return element;
}
Element *CreateArrayElement(double length){
  Element *element;
  element = new Element();
  element->type = toVector(L"array");
  element->array = new vector<Element*> (length);
  return element;
}
Element *CreateObjectElement(double length){
  Element *element;
  element = new Element();
  element->type = toVector(L"object");
  element->object = new StringElementMap();
  element->object->stringListRef = CreateStringArrayReferenceLengthValue(length, toVector(L""));
  element->object->elementListRef = new ElementArrayReference();
  element->object->elementListRef->array = new vector<Element*> (length);
  return element;
}
void DeleteElement(Element *element){
  if(StringsEqual(element->type, toVector(L"object"))){
    DeleteObject(element);
  }else if(StringsEqual(element->type, toVector(L"string"))){
    delete element;
  }else if(StringsEqual(element->type, toVector(L"array"))){
    DeleteArray(element);
  }else if(StringsEqual(element->type, toVector(L"number"))){
    delete element;
  }else if(StringsEqual(element->type, toVector(L"null"))){
    delete element;
  }else if(StringsEqual(element->type, toVector(L"boolean"))){
    delete element;
  }else{
  }
}
void DeleteObject(Element *element){
  StringArrayReference *keys;
  double i;
  vector<wchar_t> *key;
  Element *objectElement;

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArray->size(); i = i + 1.0){
    key = keys->stringArray->at(i)->string;
    objectElement = GetObjectValue(element->object, key);
    DeleteElement(objectElement);
  }
}
void DeleteArray(Element *element){
  double i;
  Element *arrayElement;

  for(i = 0.0; i < element->array->size(); i = i + 1.0){
    arrayElement = element->array->at(i);
    DeleteElement(arrayElement);
  }
}
vector<wchar_t> *WriteJSON(Element *element){
  vector<wchar_t> *result;
  double length;
  NumberReference *index;

  length = ComputeJSONStringLength(element);
  result = new vector<wchar_t> (length);
  index = CreateNumberReference(0.0);

  if(StringsEqual(element->type, toVector(L"object"))){
    WriteObject(element, result, index);
  }else if(StringsEqual(element->type, toVector(L"string"))){
    WriteString(element, result, index);
  }else if(StringsEqual(element->type, toVector(L"array"))){
    WriteArray(element, result, index);
  }else if(StringsEqual(element->type, toVector(L"number"))){
    WriteNumber(element, result, index);
  }else if(StringsEqual(element->type, toVector(L"null"))){
    strWriteStringToStingStream(result, index, toVector(L"null"));
  }else if(StringsEqual(element->type, toVector(L"boolean"))){
    WriteBooleanValue(element, result, index);
  }

  return result;
}
void WriteBooleanValue(Element *element, vector<wchar_t> *result, NumberReference *index){
  if(element->booleanValue){
    strWriteStringToStingStream(result, index, toVector(L"true"));
  }else{
    strWriteStringToStingStream(result, index, toVector(L"false"));
  }
}
void WriteNumber(Element *element, vector<wchar_t> *result, NumberReference *index){
  vector<wchar_t> *numberString;

  if(abs(element->number) >= pow(10.0, 15.0) || abs(element->number) <= pow(10.0,  -15.0)){
    numberString = nCreateStringScientificNotationDecimalFromNumber(element->number);
  }else{
    numberString = nCreateStringDecimalFromNumber(element->number);
  }

  strWriteStringToStingStream(result, index, numberString);
}
void WriteArray(Element *element, vector<wchar_t> *result, NumberReference *index){
  vector<wchar_t> *s;
  Element *arrayElement;
  double i;

  strWriteStringToStingStream(result, index, toVector(L"["));

  for(i = 0.0; i < element->array->size(); i = i + 1.0){
    arrayElement = element->array->at(i);

    s = WriteJSON(arrayElement);
    strWriteStringToStingStream(result, index, s);

    if(i + 1.0 != element->array->size()){
      strWriteStringToStingStream(result, index, toVector(L","));
    }
  }

  strWriteStringToStingStream(result, index, toVector(L"]"));
}
void WriteString(Element *element, vector<wchar_t> *result, NumberReference *index){
  strWriteStringToStingStream(result, index, toVector(L"\""));
  element->string = JSONEscapeString(element->string);
  strWriteStringToStingStream(result, index, element->string);
  strWriteStringToStingStream(result, index, toVector(L"\""));
}
vector<wchar_t> *JSONEscapeString(vector<wchar_t> *string){
  double i, length;
  NumberReference *index, *lettersReference;
  vector<wchar_t> *ns, *escaped;

  length = JSONEscapedStringLength(string);

  ns = new vector<wchar_t> (length);
  index = CreateNumberReference(0.0);
  lettersReference = CreateNumberReference(0.0);

  for(i = 0.0; i < string->size(); i = i + 1.0){
    if(JSONMustBeEscaped(string->at(i), lettersReference)){
      escaped = JSONEscapeCharacter(string->at(i));
      strWriteStringToStingStream(ns, index, escaped);
    }else{
      strWriteCharacterToStingStream(ns, index, string->at(i));
    }
  }

  return ns;
}
double JSONEscapedStringLength(vector<wchar_t> *string){
  NumberReference *lettersReference;
  double i, length;

  lettersReference = CreateNumberReference(0.0);
  length = 0.0;

  for(i = 0.0; i < string->size(); i = i + 1.0){
    if(JSONMustBeEscaped(string->at(i), lettersReference)){
      length = length + lettersReference->numberValue;
    }else{
      length = length + 1.0;
    }
  }
  return length;
}
vector<wchar_t> *JSONEscapeCharacter(wchar_t c){
  double code;
  vector<wchar_t> *escaped;
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

  hexNumber = new StringReference();

  if(code == q){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = '\"';
  }else if(code == rs){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = '\\';
  }else if(code == s){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = '/';
  }else if(code == b){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = 'b';
  }else if(code == f){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = 'f';
  }else if(code == n){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = 'n';
  }else if(code == r){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = 'r';
  }else if(code == t){
    escaped = new vector<wchar_t> (2.0);
    escaped->at(0) = '\\';
    escaped->at(1) = 't';
  }else if(code >= 0.0 && code <= 31.0){
    escaped = new vector<wchar_t> (6.0);
    escaped->at(0) = '\\';
    escaped->at(1) = 'u';
    escaped->at(2) = '0';
    escaped->at(3) = '0';

    nCreateStringFromNumberWithCheck(code, 16.0, hexNumber);

    if(hexNumber->string->size() == 1.0){
      escaped->at(4) = '0';
      escaped->at(5) = hexNumber->string->at(0);
    }else if(hexNumber->string->size() == 2.0){
      escaped->at(4) = hexNumber->string->at(0);
      escaped->at(5) = hexNumber->string->at(1);
    }
  }else{
    escaped = new vector<wchar_t> (1.0);
    escaped->at(0) = c;
  }

  return escaped;
}
bool JSONMustBeEscaped(wchar_t c, NumberReference *letters){
  double code;
  bool mustBeEscaped;
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
void WriteObject(Element *element, vector<wchar_t> *result, NumberReference *index){
  vector<wchar_t> *s, *key;
  double i;
  StringArrayReference *keys;
  Element *objectElement;

  strWriteStringToStingStream(result, index, toVector(L"{"));

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArray->size(); i = i + 1.0){
    key = keys->stringArray->at(i)->string;
    key = JSONEscapeString(key);
    objectElement = GetObjectValue(element->object, key);

    strWriteStringToStingStream(result, index, toVector(L"\""));
    strWriteStringToStingStream(result, index, key);
    strWriteStringToStingStream(result, index, toVector(L"\""));
    strWriteStringToStingStream(result, index, toVector(L":"));

    s = WriteJSON(objectElement);
    strWriteStringToStingStream(result, index, s);

    if(i + 1.0 != keys->stringArray->size()){
      strWriteStringToStingStream(result, index, toVector(L","));
    }
  }

  strWriteStringToStingStream(result, index, toVector(L"}"));
}
bool ReadJSON(vector<wchar_t> *string, ElementReference *elementReference, StringArrayReference *errorMessages){
  StringArrayReference *tokenArrayReference;
  bool success;

  /* Tokenize. */
  tokenArrayReference = new StringArrayReference();
  success = JSONTokenize(string, tokenArrayReference, errorMessages);

  if(success){
    /* Parse. */
    success = GetJSONValue(tokenArrayReference->stringArray, elementReference, errorMessages);
  }

  return success;
}
bool GetJSONValue(vector<StringReference*> *tokens, ElementReference *elementReference, StringArrayReference *errorMessages){
  NumberReference *i;
  bool success;

  i = CreateNumberReference(0.0);
  success = GetJSONValueRecursive(tokens, i, 0.0, elementReference, errorMessages);

  return success;
}
bool GetJSONValueRecursive(vector<StringReference*> *tokens, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  vector<wchar_t> *str, *substr, *token;
  double stringToDecimalResult;
  bool success;

  success = true;
  token = tokens->at(i->numberValue)->string;

  if(StringsEqual(token, toVector(L"{"))){
    success = GetJSONObject(tokens, i, depth + 1.0, elementReference, errorMessages);
  }else if(StringsEqual(token, toVector(L"["))){
    success = GetJSONArray(tokens, i, depth + 1.0, elementReference, errorMessages);
  }else if(StringsEqual(token, toVector(L"true"))){
    elementReference->element = CreateBooleanElement(true);
    i->numberValue = i->numberValue + 1.0;
  }else if(StringsEqual(token, toVector(L"false"))){
    elementReference->element = CreateBooleanElement(false);
    i->numberValue = i->numberValue + 1.0;
  }else if(StringsEqual(token, toVector(L"null"))){
    elementReference->element = CreateNullElement();
    i->numberValue = i->numberValue + 1.0;
  }else if(charIsNumber(token->at(0)) || token->at(0) == '-'){
    stringToDecimalResult = nCreateNumberFromDecimalString(token);
    elementReference->element = CreateNumberElement(stringToDecimalResult);
    i->numberValue = i->numberValue + 1.0;
  }else if(token->at(0) == '\"'){
    substr = strSubstring(token, 1.0, token->size() - 1.0);
    elementReference->element = CreateStringElement(substr);
    i->numberValue = i->numberValue + 1.0;
  }else{
    str = toVector(L"");
    str = strConcatenateString(str, toVector(L"Invalid token first in value: "));
    str = strAppendString(str, token);
    lAddStringRef(errorMessages, CreateStringReference(str));
    success = false;
  }

  if(success && depth == 0.0){
    if(StringsEqual(tokens->at(i->numberValue)->string, toVector(L"<end>"))){
    }else{
      lAddStringRef(errorMessages, CreateStringReference(toVector(L"The outer value cannot have any tokens following it.")));
      success = false;
    }
  }

  return success;
}
bool GetJSONObject(vector<StringReference*> *tokens, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  Element *element, *value;
  bool done, success;
  vector<wchar_t> *key, *colon, *comma, *closeCurly;
  vector<wchar_t> *keystring, *str;
  ElementReference *valueReference;
  LinkedListElements *values;
  lLinkedListStrings *keys;

  keys = lCreateLinkedListString();
  values = CreateLinkedListElements();
  element = CreateObjectElement(0.0);
  valueReference = new ElementReference();
  success = true;
  i->numberValue = i->numberValue + 1.0;

  if( !StringsEqual(tokens->at(i->numberValue)->string, toVector(L"}")) ){
    done = false;

    for(;  !done  && success; ){
      key = tokens->at(i->numberValue)->string;

      if(key->at(0) == '\"'){
        i->numberValue = i->numberValue + 1.0;
        colon = tokens->at(i->numberValue)->string;
        if(StringsEqual(colon, toVector(L":"))){
          i->numberValue = i->numberValue + 1.0;
          success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

          if(success){
            keystring = strSubstring(key, 1.0, key->size() - 1.0);
            value = valueReference->element;
            lLinkedListAddString(keys, keystring);
            LinkedListAddElement(values, value);

            comma = tokens->at(i->numberValue)->string;
            if(StringsEqual(comma, toVector(L","))){
              /* OK */
              i->numberValue = i->numberValue + 1.0;
            }else{
              done = true;
            }
          }
        }else{
          str = toVector(L"");
          str = strConcatenateString(str, toVector(L"Expected colon after key in object: "));
          str = strAppendString(str, colon);
          lAddStringRef(errorMessages, CreateStringReference(str));

          success = false;
          done = true;
        }
      }else{
        lAddStringRef(errorMessages, CreateStringReference(toVector(L"Expected string as key in object.")));

        done = true;
        success = false;
      }
    }
  }

  if(success){
    closeCurly = tokens->at(i->numberValue)->string;

    if(StringsEqual(closeCurly, toVector(L"}"))){
      /* OK */
      delete element->object->stringListRef->stringArray;
      delete element->object->elementListRef->array;
      element->object->stringListRef->stringArray = lLinkedListStringsToArray(keys);
      element->object->elementListRef->array = LinkedListElementsToArray(values);
      elementReference->element = element;
      i->numberValue = i->numberValue + 1.0;
    }else{
      lAddStringRef(errorMessages, CreateStringReference(toVector(L"Expected close curly brackets at end of object value.")));
      success = false;
    }
  }

  lFreeLinkedListString(keys);
  FreeLinkedListElements(values);
  delete valueReference;

  return success;
}
bool GetJSONArray(vector<StringReference*> *tokens, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  Element *element, *value;
  vector<wchar_t> *nextToken, *comma;
  bool done, success;
  ElementReference *valueReference;
  LinkedListElements *elements;

  elements = CreateLinkedListElements();
  i->numberValue = i->numberValue + 1.0;

  valueReference = new ElementReference();
  success = true;
  element = CreateArrayElement(0.0);

  nextToken = tokens->at(i->numberValue)->string;

  if( !StringsEqual(nextToken, toVector(L"]")) ){
    done = false;
    for(;  !done  && success; ){
      success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

      if(success){
        value = valueReference->element;
        LinkedListAddElement(elements, value);

        comma = tokens->at(i->numberValue)->string;

        if(StringsEqual(comma, toVector(L","))){
          /* OK */
          i->numberValue = i->numberValue + 1.0;
        }else{
          done = true;
        }
      }
    }
  }

  nextToken = tokens->at(i->numberValue)->string;
  if(StringsEqual(nextToken, toVector(L"]"))){
    /* OK */
    i->numberValue = i->numberValue + 1.0;
    delete element->array;
    element->array = LinkedListElementsToArray(elements);
  }else{
    lAddStringRef(errorMessages, CreateStringReference(toVector(L"Expected close square bracket at end of array.")));
    success = false;
  }

  elementReference->element = element;
  FreeLinkedListElements(elements);
  delete valueReference;

  return success;
}
StringArrayReference *GetStringElementMapKeySet(StringElementMap *stringElementMap){
  return stringElementMap->stringListRef;
}
Element *GetObjectValue(StringElementMap *stringElementMap, vector<wchar_t> *key){
  Element *result;
  double i;

  result = new Element();

  for(i = 0.0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1.0){
    if(StringsEqual(stringElementMap->stringListRef->stringArray->at(i)->string, key)){
      result = stringElementMap->elementListRef->array->at(i);
    }
  }

  return result;
}
Element *GetObjectValueWithCheck(StringElementMap *stringElementMap, vector<wchar_t> *key, BooleanReference *foundReference){
  Element *result;
  double i;

  result = new Element();

  foundReference->booleanValue = false;
  for(i = 0.0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1.0){
    if(StringsEqual(stringElementMap->stringListRef->stringArray->at(i)->string, key)){
      result = stringElementMap->elementListRef->array->at(i);
      foundReference->booleanValue = true;
    }
  }

  return result;
}
void PutStringElementMap(StringElementMap *stringElementMap, vector<wchar_t> *keystring, Element *value){
  lAddStringRef(stringElementMap->stringListRef, CreateStringReference(keystring));
  AddElementRef(stringElementMap->elementListRef, value);
}
void SetStringElementMap(StringElementMap *stringElementMap, double index, vector<wchar_t> *keystring, Element *value){
  stringElementMap->stringListRef->stringArray->at(index)->string = keystring;
  stringElementMap->elementListRef->array->at(index) = value;
}
double GetStringElementMapNumberOfKeys(StringElementMap *stringElementMap){
  return stringElementMap->stringListRef->stringArray->size();
}
bool JSONCompare(vector<wchar_t> *a, vector<wchar_t> *b, double epsilon, BooleanReference *equal, StringArrayReference *errorMessage){
  bool success;
  ElementReference *eaRef, *ebRef;
  Element *ea, *eb;

  eaRef = new ElementReference();
  ebRef = new ElementReference();

  success = ReadJSON(a, eaRef, errorMessage);

  if(success){
    ea = eaRef->element;

    success = ReadJSON(b, ebRef, errorMessage);

    if(success){
      eb = ebRef->element;

      equal->booleanValue = JSONCompareElements(ea, eb, epsilon);

      DeleteElement(eb);
    }

    DeleteElement(ea);
  }

  return success;
}
bool JSONCompareElements(Element *ea, Element *eb, double epsilon){
  bool equal;
  vector<wchar_t> *typeName;

  equal = StringsEqual(ea->type, eb->type);
        
  if(equal){
    typeName = ea->type;
    if(StringsEqual(typeName, toVector(L"object"))){
      equal = JSONCompareObjects(ea->object, eb->object, epsilon);
    }else if(StringsEqual(typeName, toVector(L"string"))){
      equal = StringsEqual(ea->string, eb->string);
    }else if(StringsEqual(typeName, toVector(L"array"))){
      equal = JSONCompareArrays(ea->array, eb->array, epsilon);
    }else if(StringsEqual(typeName, toVector(L"number"))){
      equal = EpsilonCompare(ea->number, eb->number, epsilon);
    }else if(StringsEqual(typeName, toVector(L"null"))){
      equal = true;
    }else if(StringsEqual(typeName, toVector(L"boolean"))){
      equal = ea->booleanValue == eb->booleanValue;
    }
  }
        
  return equal;
}
bool JSONCompareArrays(vector<Element*> *ea, vector<Element*> *eb, double epsilon){
  bool equals;
  double i, length;

  equals = ea->size() == eb->size();

  if(equals){
    length = ea->size();
    for(i = 0.0; i < length && equals; i = i + 1.0){
      equals = JSONCompareElements(ea->at(i), eb->at(i), epsilon);
    }
  }

  return equals;
}
bool JSONCompareObjects(StringElementMap *ea, StringElementMap *eb, double epsilon){
  bool equals;
  double akeys, bkeys, i;
  StringArrayReference *keys;
  vector<wchar_t> *key;
  BooleanReference *aFoundReference, *bFoundReference;
  Element *eaValue, *ebValue;

  aFoundReference = new BooleanReference();
  bFoundReference = new BooleanReference();

  akeys = GetStringElementMapNumberOfKeys(ea);
  bkeys = GetStringElementMapNumberOfKeys(eb);

  equals = akeys == bkeys;

  if(equals){
    keys = GetStringElementMapKeySet(ea);

    for(i = 0.0; i < keys->stringArray->size() && equals; i = i + 1.0){
      key = keys->stringArray->at(i)->string;

      eaValue = GetObjectValueWithCheck(ea, key, aFoundReference);
      ebValue = GetObjectValueWithCheck(eb, key, bFoundReference);

      if(aFoundReference->booleanValue && bFoundReference->booleanValue){
        equals = JSONCompareElements(eaValue, ebValue, epsilon);
      }else{
        equals = false;
      }
    }
  }

  return equals;
}
vector<double> *AddNumber(vector<double> *list, double a){
  vector<double> *newlist;
  double i;

  newlist = new vector<double> (list->size() + 1.0);
  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void AddNumberRef(NumberArrayReference *list, double i){
  list->numberArray = AddNumber(list->numberArray, i);
}
vector<double> *RemoveNumber(vector<double> *list, double n){
  vector<double> *newlist;
  double i;

  newlist = new vector<double> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }
		
  return newlist;
}
double GetNumberRef(NumberArrayReference *list, double i){
  return list->numberArray->at(i);
}
void RemoveNumberRef(NumberArrayReference *list, double i){
  list->numberArray = RemoveNumber(list->numberArray, i);
}
vector<StringReference*> *AddString(vector<StringReference*> *list, StringReference *a){
  vector<StringReference*> *newlist;
  double i;

  newlist = new vector<StringReference*> (list->size() + 1.0);

  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void AddStringRef(StringArrayReference *list, StringReference *i){
  list->stringArray = AddString(list->stringArray, i);
}
vector<StringReference*> *RemoveString(vector<StringReference*> *list, double n){
  vector<StringReference*> *newlist;
  double i;

  newlist = new vector<StringReference*> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }
		
  return newlist;
}
StringReference *GetStringRef(StringArrayReference *list, double i){
  return list->stringArray->at(i);
}
void RemoveStringRef(StringArrayReference *list, double i){
  list->stringArray = RemoveString(list->stringArray, i);
}
DynamicArrayCharacters *CreateDynamicArrayCharacters(){
  DynamicArrayCharacters *da;

  da = new DynamicArrayCharacters();
  da->array = new vector<wchar_t> (10.0);
  da->length = 0.0;

  return da;
}
DynamicArrayCharacters *CreateDynamicArrayCharactersWithInitialCapacity(double capacity){
  DynamicArrayCharacters *da;

  da = new DynamicArrayCharacters();
  da->array = new vector<wchar_t> (capacity);
  da->length = 0.0;

  return da;
}
void DynamicArrayAddCharacter(DynamicArrayCharacters *da, wchar_t value){
  if(da->length == da->array->size()){
    DynamicArrayCharactersIncreaseSize(da);
  }

  da->array->at(da->length) = value;
  da->length = da->length + 1.0;
}
void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters *da){
  double newLength, i;
  vector<wchar_t> *newArray;

  newLength = round(da->array->size()*3.0/2.0);
  newArray = new vector<wchar_t> (newLength);

  for(i = 0.0; i < da->array->size(); i = i + 1.0){
    newArray->at(i) = da->array->at(i);
  }

  delete da->array;

  da->array = newArray;
}
bool DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters *da){
  bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->array->size()*2.0/3.0);
  }

  return needsDecrease;
}
void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters *da){
  double newLength, i;
  vector<wchar_t> *newArray;

  newLength = round(da->array->size()*2.0/3.0);
  newArray = new vector<wchar_t> (newLength);

  for(i = 0.0; i < newLength; i = i + 1.0){
    newArray->at(i) = da->array->at(i);
  }

  delete da->array;

  da->array = newArray;
}
double DynamicArrayCharactersIndex(DynamicArrayCharacters *da, double index){
  return da->array->at(index);
}
double DynamicArrayCharactersLength(DynamicArrayCharacters *da){
  return da->length;
}
void DynamicArrayInsertCharacter(DynamicArrayCharacters *da, double index, wchar_t value){
  double i;

  if(da->length == da->array->size()){
    DynamicArrayCharactersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array->at(i) = da->array->at(i - 1.0);
  }

  da->array->at(index) = value;

  da->length = da->length + 1.0;
}
bool DynamicArrayCharacterSet(DynamicArrayCharacters *da, double index, wchar_t value){
  bool success;

  if(index < da->length){
    da->array->at(index) = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
void DynamicArrayRemoveCharacter(DynamicArrayCharacters *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array->at(i) = da->array->at(i + 1.0);
  }

  da->length = da->length - 1.0;

  if(DynamicArrayCharactersDecreaseSizeNecessary(da)){
    DynamicArrayCharactersDecreaseSize(da);
  }
}
void FreeDynamicArrayCharacters(DynamicArrayCharacters *da){
  delete da->array;
  delete da;
}
vector<wchar_t> *DynamicArrayCharactersToArray(DynamicArrayCharacters *da){
  vector<wchar_t> *array;
  double i;

  array = new vector<wchar_t> (da->length);

  for(i = 0.0; i < da->length; i = i + 1.0){
    array->at(i) = da->array->at(i);
  }

  return array;
}
DynamicArrayCharacters *ArrayToDynamicArrayCharactersWithOptimalSize(vector<wchar_t> *array){
  DynamicArrayCharacters *da;
  double i;
  double c, n, newCapacity;

  c = array->size();
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < array->size(); i = i + 1.0){
    da->array->at(i) = array->at(i);
  }

  return da;
}
DynamicArrayCharacters *ArrayToDynamicArrayCharacters(vector<wchar_t> *array){
  DynamicArrayCharacters *da;

  da = new DynamicArrayCharacters();
  da->array = CopyString(array);
  da->length = array->size();

  return da;
}
bool DynamicArrayCharactersEqual(DynamicArrayCharacters *a, DynamicArrayCharacters *b){
  bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array->at(i) != b->array->at(i)){
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
    LinkedListAddCharacter(ll, da->array->at(i));
  }

  return ll;
}
DynamicArrayCharacters *LinkedListToDynamicArrayCharacters(LinkedListCharacters *ll){
  DynamicArrayCharacters *da;
  double i;
  LinkedListNodeCharacters *node;

  node = ll->first;

  da = new DynamicArrayCharacters();
  da->length = LinkedListCharactersLength(ll);

  da->array = new vector<wchar_t> (da->length);

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array->at(i) = node->value;
    node = node->next;
  }

  return da;
}
vector<bool> *AddBoolean(vector<bool> *list, bool a){
  vector<bool> *newlist;
  double i;

  newlist = new vector<bool> (list->size() + 1.0);
  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void AddBooleanRef(BooleanArrayReference *list, bool i){
  list->booleanArray = AddBoolean(list->booleanArray, i);
}
vector<bool> *RemoveBoolean(vector<bool> *list, double n){
  vector<bool> *newlist;
  double i;

  newlist = new vector<bool> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }
		
  return newlist;
}
bool GetBooleanRef(BooleanArrayReference *list, double i){
  return list->booleanArray->at(i);
}
void RemoveDecimalRef(BooleanArrayReference *list, double i){
  list->booleanArray = RemoveBoolean(list->booleanArray, i);
}
LinkedListStrings *CreateLinkedListString(){
  LinkedListStrings *ll;

  ll = new LinkedListStrings();
  ll->first = new LinkedListNodeStrings();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddString(LinkedListStrings *ll, vector<wchar_t> *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new LinkedListNodeStrings();
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
vector<StringReference*> *LinkedListStringsToArray(LinkedListStrings *ll){
  vector<StringReference*> *array;
  double length, i;
  LinkedListNodeStrings *node;

  node = ll->first;

  length = LinkedListStringsLength(ll);

  array = new vector<StringReference*> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = new StringReference();
    array->at(i)->string = node->value;
    node = node->next;
  }

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
    delete prev;
  }

  delete node;
}
LinkedListNumbers *CreateLinkedListNumbers(){
  LinkedListNumbers *ll;

  ll = new LinkedListNumbers();
  ll->first = new LinkedListNodeNumbers();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
vector<LinkedListNumbers*> *CreateLinkedListNumbersArray(double length){
  vector<LinkedListNumbers*> *lls;
  double i;

  lls = new vector<LinkedListNumbers*> (length);
  for(i = 0.0; i < lls->size(); i = i + 1.0){
    lls->at(i) = CreateLinkedListNumbers();
  }

  return lls;
}
void LinkedListAddNumber(LinkedListNumbers *ll, double value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new LinkedListNodeNumbers();
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
    ll->first = new LinkedListNodeNumbers();
    ll->first->next = tmp;
    ll->first->value = value;
    ll->first->end = false;
  }else{
    node = ll->first;
    for(i = 0.0; i < index - 1.0; i = i + 1.0){
      node = node->next;
    }

    tmp = node->next;
    node->next = new LinkedListNodeNumbers();
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
    delete prev;
  }

  delete node;
}
void FreeLinkedListNumbersArray(vector<LinkedListNumbers*> *lls){
  double i;

  for(i = 0.0; i < lls->size(); i = i + 1.0){
    FreeLinkedListNumbers(lls->at(i));
  }
  delete lls;
}
vector<double> *LinkedListNumbersToArray(LinkedListNumbers *ll){
  vector<double> *array;
  double length, i;
  LinkedListNodeNumbers *node;

  node = ll->first;

  length = LinkedListNumbersLength(ll);

  array = new vector<double> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = node->value;
    node = node->next;
  }

  return array;
}
LinkedListNumbers *ArrayToLinkedListNumbers(vector<double> *array){
  LinkedListNumbers *ll;
  double i;

  ll = CreateLinkedListNumbers();

  for(i = 0.0; i < array->size(); i = i + 1.0){
    LinkedListAddNumber(ll, array->at(i));
  }

  return ll;
}
bool LinkedListNumbersEqual(LinkedListNumbers *a, LinkedListNumbers *b){
  bool equal, done;
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

  ll = new LinkedListCharacters();
  ll->first = new LinkedListNodeCharacters();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddCharacter(LinkedListCharacters *ll, wchar_t value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new LinkedListNodeCharacters();
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
vector<wchar_t> *LinkedListCharactersToArray(LinkedListCharacters *ll){
  vector<wchar_t> *array;
  double length, i;
  LinkedListNodeCharacters *node;

  node = ll->first;

  length = LinkedListCharactersLength(ll);

  array = new vector<wchar_t> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = node->value;
    node = node->next;
  }

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
    delete prev;
  }

  delete node;
}
void LinkedListCharactersAddString(LinkedListCharacters *ll, vector<wchar_t> *str){
  double i;

  for(i = 0.0; i < str->size(); i = i + 1.0){
    LinkedListAddCharacter(ll, str->at(i));
  }
}
DynamicArrayNumbers *CreateDynamicArrayNumbers(){
  DynamicArrayNumbers *da;

  da = new DynamicArrayNumbers();
  da->array = new vector<double> (10.0);
  da->length = 0.0;

  return da;
}
DynamicArrayNumbers *CreateDynamicArrayNumbersWithInitialCapacity(double capacity){
  DynamicArrayNumbers *da;

  da = new DynamicArrayNumbers();
  da->array = new vector<double> (capacity);
  da->length = 0.0;

  return da;
}
void DynamicArrayAddNumber(DynamicArrayNumbers *da, double value){
  if(da->length == da->array->size()){
    DynamicArrayNumbersIncreaseSize(da);
  }

  da->array->at(da->length) = value;
  da->length = da->length + 1.0;
}
void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers *da){
  double newLength, i;
  vector<double> *newArray;

  newLength = round(da->array->size()*3.0/2.0);
  newArray = new vector<double> (newLength);

  for(i = 0.0; i < da->array->size(); i = i + 1.0){
    newArray->at(i) = da->array->at(i);
  }

  delete da->array;

  da->array = newArray;
}
bool DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers *da){
  bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->array->size()*2.0/3.0);
  }

  return needsDecrease;
}
void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers *da){
  double newLength, i;
  vector<double> *newArray;

  newLength = round(da->array->size()*2.0/3.0);
  newArray = new vector<double> (newLength);

  for(i = 0.0; i < newLength; i = i + 1.0){
    newArray->at(i) = da->array->at(i);
  }

  delete da->array;

  da->array = newArray;
}
double DynamicArrayNumbersIndex(DynamicArrayNumbers *da, double index){
  return da->array->at(index);
}
double DynamicArrayNumbersLength(DynamicArrayNumbers *da){
  return da->length;
}
void DynamicArrayInsertNumber(DynamicArrayNumbers *da, double index, double value){
  double i;

  if(da->length == da->array->size()){
    DynamicArrayNumbersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array->at(i) = da->array->at(i - 1.0);
  }

  da->array->at(index) = value;

  da->length = da->length + 1.0;
}
bool DynamicArrayNumberSet(DynamicArrayNumbers *da, double index, double value){
  bool success;

  if(index < da->length){
    da->array->at(index) = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
void DynamicArrayRemoveNumber(DynamicArrayNumbers *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array->at(i) = da->array->at(i + 1.0);
  }

  da->length = da->length - 1.0;

  if(DynamicArrayNumbersDecreaseSizeNecessary(da)){
    DynamicArrayNumbersDecreaseSize(da);
  }
}
void FreeDynamicArrayNumbers(DynamicArrayNumbers *da){
  delete da->array;
  delete da;
}
vector<double> *DynamicArrayNumbersToArray(DynamicArrayNumbers *da){
  vector<double> *array;
  double i;

  array = new vector<double> (da->length);

  for(i = 0.0; i < da->length; i = i + 1.0){
    array->at(i) = da->array->at(i);
  }

  return array;
}
DynamicArrayNumbers *ArrayToDynamicArrayNumbersWithOptimalSize(vector<double> *array){
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
  c = array->size();
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < array->size(); i = i + 1.0){
    da->array->at(i) = array->at(i);
  }

  return da;
}
DynamicArrayNumbers *ArrayToDynamicArrayNumbers(vector<double> *array){
  DynamicArrayNumbers *da;

  da = new DynamicArrayNumbers();
  da->array = CopyNumberArray(array);
  da->length = array->size();

  return da;
}
bool DynamicArrayNumbersEqual(DynamicArrayNumbers *a, DynamicArrayNumbers *b){
  bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array->at(i) != b->array->at(i)){
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
    LinkedListAddNumber(ll, da->array->at(i));
  }

  return ll;
}
DynamicArrayNumbers *LinkedListToDynamicArrayNumbers(LinkedListNumbers *ll){
  DynamicArrayNumbers *da;
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;

  da = new DynamicArrayNumbers();
  da->length = LinkedListNumbersLength(ll);

  da->array = new vector<double> (da->length);

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array->at(i) = node->value;
    node = node->next;
  }

  return da;
}
vector<wchar_t> *AddCharacter(vector<wchar_t> *list, wchar_t a){
  vector<wchar_t> *newlist;
  double i;

  newlist = new vector<wchar_t> (list->size() + 1.0);
  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void AddCharacterRef(StringReference *list, wchar_t i){
  list->string = AddCharacter(list->string, i);
}
vector<wchar_t> *RemoveCharacter(vector<wchar_t> *list, double n){
  vector<wchar_t> *newlist;
  double i;

  newlist = new vector<wchar_t> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }

  return newlist;
}
wchar_t GetCharacterRef(StringReference *list, double i){
  return list->string->at(i);
}
void RemoveCharacterRef(StringReference *list, double i){
  list->string = RemoveCharacter(list->string, i);
}
void sWriteStringToStingStream(vector<wchar_t> *stream, NumberReference *index, vector<wchar_t> *src){
  double i;

  for(i = 0.0; i < src->size(); i = i + 1.0){
    stream->at(index->numberValue + i) = src->at(i);
  }
  index->numberValue = index->numberValue + src->size();
}
void sWriteCharacterToStingStream(vector<wchar_t> *stream, NumberReference *index, wchar_t src){
  stream->at(index->numberValue) = src;
  index->numberValue = index->numberValue + 1.0;
}
void sWriteBooleanToStingStream(vector<wchar_t> *stream, NumberReference *index, bool src){
  if(src){
    sWriteStringToStingStream(stream, index, toVector(L"true"));
  }else{
    sWriteStringToStingStream(stream, index, toVector(L"false"));
  }
}
bool sSubstringWithCheck(vector<wchar_t> *string, double from, double to, StringReference *stringReference){
  bool success;

  if(from >= 0.0 && from <= string->size() && to >= 0.0 && to <= string->size() && from <= to){
    stringReference->string = sSubstring(string, from, to);
    success = true;
  }else{
    success = false;
  }

  return success;
}
vector<wchar_t> *sSubstring(vector<wchar_t> *string, double from, double to){
  vector<wchar_t> *n;
  double i, length;

  length = to - from;

  n = new vector<wchar_t> (length);

  for(i = from; i < to; i = i + 1.0){
    n->at(i - from) = string->at(i);
  }

  return n;
}
vector<wchar_t> *sAppendString(vector<wchar_t> *s1, vector<wchar_t> *s2){
  vector<wchar_t> *newString;

  newString = sConcatenateString(s1, s2);

  delete s1;

  return newString;
}
vector<wchar_t> *sConcatenateString(vector<wchar_t> *s1, vector<wchar_t> *s2){
  vector<wchar_t> *newString;
  double i;

  newString = new vector<wchar_t> (s1->size() + s2->size());

  for(i = 0.0; i < s1->size(); i = i + 1.0){
    newString->at(i) = s1->at(i);
  }

  for(i = 0.0; i < s2->size(); i = i + 1.0){
    newString->at(s1->size() + i) = s2->at(i);
  }

  return newString;
}
vector<wchar_t> *sAppendCharacter(vector<wchar_t> *string, wchar_t c){
  vector<wchar_t> *newString;

  newString = sConcatenateCharacter(string, c);

  delete string;

  return newString;
}
vector<wchar_t> *sConcatenateCharacter(vector<wchar_t> *string, wchar_t c){
  vector<wchar_t> *newString;
  double i;
  newString = new vector<wchar_t> (string->size() + 1.0);

  for(i = 0.0; i < string->size(); i = i + 1.0){
    newString->at(i) = string->at(i);
  }

  newString->at(string->size()) = c;

  return newString;
}
vector<StringReference*> *sSplitByCharacter(vector<wchar_t> *toSplit, wchar_t splitBy){
  vector<StringReference*> *split;
  vector<wchar_t> *stringToSplitBy;

  stringToSplitBy = new vector<wchar_t> (1.0);
  stringToSplitBy->at(0) = splitBy;

  split = sSplitByString(toSplit, stringToSplitBy);

  delete stringToSplitBy;

  return split;
}
bool sIndexOfCharacter(vector<wchar_t> *string, wchar_t character, NumberReference *indexReference){
  double i;
  bool found;

  found = false;
  for(i = 0.0; i < string->size() &&  !found ; i = i + 1.0){
    if(string->at(i) == character){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
bool sSubstringEqualsWithCheck(vector<wchar_t> *string, double from, vector<wchar_t> *substring, BooleanReference *equalsReference){
  bool success;

  if(from < string->size()){
    success = true;
    equalsReference->booleanValue = sSubstringEquals(string, from, substring);
  }else{
    success = false;
  }

  return success;
}
bool sSubstringEquals(vector<wchar_t> *string, double from, vector<wchar_t> *substring){
  double i;
  bool equal;

  equal = true;
  if(string->size() - from >= substring->size()){
    for(i = 0.0; i < substring->size() && equal; i = i + 1.0){
      if(string->at(from + i) != substring->at(i)){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
bool sIndexOfString(vector<wchar_t> *string, vector<wchar_t> *substring, NumberReference *indexReference){
  double i;
  bool found;

  found = false;
  for(i = 0.0; i < string->size() - substring->size() + 1.0 &&  !found ; i = i + 1.0){
    if(sSubstringEquals(string, i, substring)){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
bool sContainsCharacter(vector<wchar_t> *string, wchar_t character){
  double i;
  bool found;

  found = false;
  for(i = 0.0; i < string->size() &&  !found ; i = i + 1.0){
    if(string->at(i) == character){
      found = true;
    }
  }

  return found;
}
bool sContainsString(vector<wchar_t> *string, vector<wchar_t> *substring){
  return sIndexOfString(string, substring, new NumberReference());
}
void sToUpperCase(vector<wchar_t> *string){
  double i;

  for(i = 0.0; i < string->size(); i = i + 1.0){
    string->at(i) = charToUpperCase(string->at(i));
  }
}
void sToLowerCase(vector<wchar_t> *string){
  double i;

  for(i = 0.0; i < string->size(); i = i + 1.0){
    string->at(i) = charToLowerCase(string->at(i));
  }
}
bool sEqualsIgnoreCase(vector<wchar_t> *a, vector<wchar_t> *b){
  bool equal;
  double i;

  if(a->size() == b->size()){
    equal = true;
    for(i = 0.0; i < a->size() && equal; i = i + 1.0){
      if(charToLowerCase(a->at(i)) != charToLowerCase(b->at(i))){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
vector<wchar_t> *sReplaceString(vector<wchar_t> *string, vector<wchar_t> *toReplace, vector<wchar_t> *replaceWith){
  vector<wchar_t> *result;
  double i, j;
  BooleanReference *equalsReference;
  bool success;
  DynamicArrayCharacters *da;

  da = CreateDynamicArrayCharacters();

  equalsReference = new BooleanReference();

  for(i = 0.0; i < string->size(); ){
    success = sSubstringEqualsWithCheck(string, i, toReplace, equalsReference);
    if(success){
      success = equalsReference->booleanValue;
    }

    if(success && toReplace->size() > 0.0){
      for(j = 0.0; j < replaceWith->size(); j = j + 1.0){
        DynamicArrayAddCharacter(da, replaceWith->at(j));
      }
      i = i + toReplace->size();
    }else{
      DynamicArrayAddCharacter(da, string->at(i));
      i = i + 1.0;
    }
  }

  result = DynamicArrayCharactersToArray(da);

  FreeDynamicArrayCharacters(da);

  return result;
}
vector<wchar_t> *sReplaceCharacterToNew(vector<wchar_t> *string, wchar_t toReplace, wchar_t replaceWith){
  vector<wchar_t> *result;
  double i;

  result = new vector<wchar_t> (string->size());

  for(i = 0.0; i < string->size(); i = i + 1.0){
    if(string->at(i) == toReplace){
      result->at(i) = replaceWith;
    }else{
      result->at(i) = string->at(i);
    }
  }

  return result;
}
void sReplaceCharacter(vector<wchar_t> *string, wchar_t toReplace, wchar_t replaceWith){
  double i;

  for(i = 0.0; i < string->size(); i = i + 1.0){
    if(string->at(i) == toReplace){
      string->at(i) = replaceWith;
    }
  }
}
vector<wchar_t> *sTrim(vector<wchar_t> *string){
  vector<wchar_t> *result;
  double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
  bool firstNonWhitespaceFound;

  /* Find whitepaces at the start. */
  lastWhitespaceLocationStart =  -1.0;
  firstNonWhitespaceFound = false;
  for(i = 0.0; i < string->size() &&  !firstNonWhitespaceFound ; i = i + 1.0){
    if(charIsWhiteSpace(string->at(i))){
      lastWhitespaceLocationStart = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  lastWhitespaceLocationEnd = string->size();
  firstNonWhitespaceFound = false;
  for(i = string->size() - 1.0; i >= 0.0 &&  !firstNonWhitespaceFound ; i = i - 1.0){
    if(charIsWhiteSpace(string->at(i))){
      lastWhitespaceLocationEnd = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
    result = sSubstring(string, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd);
  }else{
    result = new vector<wchar_t> (0.0);
  }

  return result;
}
bool sStartsWith(vector<wchar_t> *string, vector<wchar_t> *start){
  bool startsWithString;

  startsWithString = false;
  if(string->size() >= start->size()){
    startsWithString = sSubstringEquals(string, 0.0, start);
  }

  return startsWithString;
}
bool sEndsWith(vector<wchar_t> *string, vector<wchar_t> *end){
  bool endsWithString;

  endsWithString = false;
  if(string->size() >= end->size()){
    endsWithString = sSubstringEquals(string, string->size() - end->size(), end);
  }

  return endsWithString;
}
vector<StringReference*> *sSplitByString(vector<wchar_t> *toSplit, vector<wchar_t> *splitBy){
  vector<StringReference*> *split;
  vector<wchar_t> *next;
  double i;
  wchar_t c;
  StringReference *n;

  split = new vector<StringReference*> (0.0);

  next = new vector<wchar_t> (0.0);
  for(i = 0.0; i < toSplit->size(); ){
    c = toSplit->at(i);

    if(sSubstringEquals(toSplit, i, splitBy)){
      n = new StringReference();
      n->string = next;
      split = AddString(split, n);
      next = new vector<wchar_t> (0.0);
      i = i + splitBy->size();
    }else{
      next = sAppendCharacter(next, c);
      i = i + 1.0;
    }
  }

  n = new StringReference();
  n->string = next;
  split = AddString(split, n);

  return split;
}
bool sStringIsBefore(vector<wchar_t> *a, vector<wchar_t> *b){
  bool before, equal, done;
  double i;

  before = false;
  equal = true;
  done = false;

  if(a->size() == 0.0 && b->size() > 0.0){
    before = true;
  }else{
    for(i = 0.0; i < a->size() && i < b->size() &&  !done ; i = i + 1.0){
      if(a->at(i) != b->at(i)){
        equal = false;
      }
      if(charCharacterIsBefore(a->at(i), b->at(i))){
        before = true;
      }
      if(charCharacterIsBefore(b->at(i), a->at(i))){
        done = true;
      }
    }

    if(equal){
      if(a->size() < b->size()){
        before = true;
      }
    }
  }

  return before;
}
void strWriteStringToStingStream(vector<wchar_t> *stream, NumberReference *index, vector<wchar_t> *src){
  double i;

  for(i = 0.0; i < src->size(); i = i + 1.0){
    stream->at(index->numberValue + i) = src->at(i);
  }
  index->numberValue = index->numberValue + src->size();
}
void strWriteCharacterToStingStream(vector<wchar_t> *stream, NumberReference *index, wchar_t src){
  stream->at(index->numberValue) = src;
  index->numberValue = index->numberValue + 1.0;
}
void strWriteBooleanToStingStream(vector<wchar_t> *stream, NumberReference *index, bool src){
  if(src){
    strWriteStringToStingStream(stream, index, toVector(L"true"));
  }else{
    strWriteStringToStingStream(stream, index, toVector(L"false"));
  }
}
bool strSubstringWithCheck(vector<wchar_t> *string, double from, double to, StringReference *stringReference){
  bool success;

  if(from >= 0.0 && from <= string->size() && to >= 0.0 && to <= string->size() && from <= to){
    stringReference->string = strSubstring(string, from, to);
    success = true;
  }else{
    success = false;
  }

  return success;
}
vector<wchar_t> *strSubstring(vector<wchar_t> *string, double from, double to){
  vector<wchar_t> *n;
  double i, length;

  length = to - from;

  n = new vector<wchar_t> (length);

  for(i = from; i < to; i = i + 1.0){
    n->at(i - from) = string->at(i);
  }

  return n;
}
vector<wchar_t> *strAppendString(vector<wchar_t> *s1, vector<wchar_t> *s2){
  vector<wchar_t> *newString;

  newString = strConcatenateString(s1, s2);

  delete s1;

  return newString;
}
vector<wchar_t> *strConcatenateString(vector<wchar_t> *s1, vector<wchar_t> *s2){
  vector<wchar_t> *newString;
  double i;

  newString = new vector<wchar_t> (s1->size() + s2->size());

  for(i = 0.0; i < s1->size(); i = i + 1.0){
    newString->at(i) = s1->at(i);
  }

  for(i = 0.0; i < s2->size(); i = i + 1.0){
    newString->at(s1->size() + i) = s2->at(i);
  }

  return newString;
}
vector<wchar_t> *strAppendCharacter(vector<wchar_t> *string, wchar_t c){
  vector<wchar_t> *newString;

  newString = strConcatenateCharacter(string, c);

  delete string;

  return newString;
}
vector<wchar_t> *strConcatenateCharacter(vector<wchar_t> *string, wchar_t c){
  vector<wchar_t> *newString;
  double i;
  newString = new vector<wchar_t> (string->size() + 1.0);

  for(i = 0.0; i < string->size(); i = i + 1.0){
    newString->at(i) = string->at(i);
  }

  newString->at(string->size()) = c;

  return newString;
}
vector<StringReference*> *strSplitByCharacter(vector<wchar_t> *toSplit, wchar_t splitBy){
  vector<StringReference*> *split;
  vector<wchar_t> *stringToSplitBy;

  stringToSplitBy = new vector<wchar_t> (1.0);
  stringToSplitBy->at(0) = splitBy;

  split = strSplitByString(toSplit, stringToSplitBy);

  delete stringToSplitBy;

  return split;
}
bool strIndexOfCharacter(vector<wchar_t> *string, wchar_t character, NumberReference *indexReference){
  double i;
  bool found;

  found = false;
  for(i = 0.0; i < string->size() &&  !found ; i = i + 1.0){
    if(string->at(i) == character){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
bool strSubstringEqualsWithCheck(vector<wchar_t> *string, double from, vector<wchar_t> *substring, BooleanReference *equalsReference){
  bool success;

  if(from < string->size()){
    success = true;
    equalsReference->booleanValue = strSubstringEquals(string, from, substring);
  }else{
    success = false;
  }

  return success;
}
bool strSubstringEquals(vector<wchar_t> *string, double from, vector<wchar_t> *substring){
  double i;
  bool equal;

  equal = true;
  for(i = 0.0; i < substring->size() && equal; i = i + 1.0){
    if(string->at(from + i) != substring->at(i)){
      equal = false;
    }
  }

  return equal;
}
bool strIndexOfString(vector<wchar_t> *string, vector<wchar_t> *substring, NumberReference *indexReference){
  double i;
  bool found;

  found = false;
  for(i = 0.0; i < string->size() - substring->size() + 1.0 &&  !found ; i = i + 1.0){
    if(strSubstringEquals(string, i, substring)){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
bool strContainsCharacter(vector<wchar_t> *string, wchar_t character){
  double i;
  bool found;

  found = false;
  for(i = 0.0; i < string->size() &&  !found ; i = i + 1.0){
    if(string->at(i) == character){
      found = true;
    }
  }

  return found;
}
bool strContainsString(vector<wchar_t> *string, vector<wchar_t> *substring){
  return strIndexOfString(string, substring, new NumberReference());
}
void strToUpperCase(vector<wchar_t> *string){
  double i;

  for(i = 0.0; i < string->size(); i = i + 1.0){
    string->at(i) = charToUpperCase(string->at(i));
  }
}
void strToLowerCase(vector<wchar_t> *string){
  double i;

  for(i = 0.0; i < string->size(); i = i + 1.0){
    string->at(i) = charToLowerCase(string->at(i));
  }
}
bool strEqualsIgnoreCase(vector<wchar_t> *a, vector<wchar_t> *b){
  bool equal;
  double i;

  if(a->size() == b->size()){
    equal = true;
    for(i = 0.0; i < a->size() && equal; i = i + 1.0){
      if(charToLowerCase(a->at(i)) != charToLowerCase(b->at(i))){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
vector<wchar_t> *strReplaceString(vector<wchar_t> *string, vector<wchar_t> *toReplace, vector<wchar_t> *replaceWith){
  vector<wchar_t> *result;
  double i;
  BooleanReference *equalsReference;
  bool success;

  equalsReference = new BooleanReference();
  result = new vector<wchar_t> (0.0);

  for(i = 0.0; i < string->size(); ){
    success = strSubstringEqualsWithCheck(string, i, toReplace, equalsReference);
    if(success){
      success = equalsReference->booleanValue;
    }

    if(success && toReplace->size() > 0.0){
      result = strConcatenateString(result, replaceWith);
      i = i + toReplace->size();
    }else{
      result = strConcatenateCharacter(result, string->at(i));
      i = i + 1.0;
    }
  }

  return result;
}
vector<wchar_t> *strReplaceCharacter(vector<wchar_t> *string, wchar_t toReplace, wchar_t replaceWith){
  vector<wchar_t> *result;
  double i;

  result = new vector<wchar_t> (0.0);

  for(i = 0.0; i < string->size(); i = i + 1.0){
    if(string->at(i) == toReplace){
      result = strConcatenateCharacter(result, replaceWith);
    }else{
      result = strConcatenateCharacter(result, string->at(i));
    }
  }

  return result;
}
vector<wchar_t> *strTrim(vector<wchar_t> *string){
  vector<wchar_t> *result;
  double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
  bool firstNonWhitespaceFound;

  /* Find whitepaces at the start. */
  lastWhitespaceLocationStart =  -1.0;
  firstNonWhitespaceFound = false;
  for(i = 0.0; i < string->size() &&  !firstNonWhitespaceFound ; i = i + 1.0){
    if(charIsWhiteSpace(string->at(i))){
      lastWhitespaceLocationStart = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  lastWhitespaceLocationEnd = string->size();
  firstNonWhitespaceFound = false;
  for(i = string->size() - 1.0; i >= 0.0 &&  !firstNonWhitespaceFound ; i = i - 1.0){
    if(charIsWhiteSpace(string->at(i))){
      lastWhitespaceLocationEnd = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
    result = strSubstring(string, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd);
  }else{
    result = new vector<wchar_t> (0.0);
  }

  return result;
}
bool strStartsWith(vector<wchar_t> *string, vector<wchar_t> *start){
  bool startsWithString;

  startsWithString = false;
  if(string->size() >= start->size()){
    startsWithString = strSubstringEquals(string, 0.0, start);
  }

  return startsWithString;
}
bool strEndsWith(vector<wchar_t> *string, vector<wchar_t> *end){
  bool endsWithString;

  endsWithString = false;
  if(string->size() >= end->size()){
    endsWithString = strSubstringEquals(string, string->size() - end->size(), end);
  }

  return endsWithString;
}
vector<StringReference*> *strSplitByString(vector<wchar_t> *toSplit, vector<wchar_t> *splitBy){
  vector<StringReference*> *split;
  vector<wchar_t> *next;
  double i;
  wchar_t c;
  StringReference *n;

  split = new vector<StringReference*> (0.0);

  next = new vector<wchar_t> (0.0);
  for(i = 0.0; i < toSplit->size(); ){
    c = toSplit->at(i);

    if(strSubstringEquals(toSplit, i, splitBy)){
      if(split->size() != 0.0 || i != 0.0){
        n = new StringReference();
        n->string = next;
        split = lAddString(split, n);
        next = new vector<wchar_t> (0.0);
        i = i + splitBy->size();
      }
    }else{
      next = strAppendCharacter(next, c);
      i = i + 1.0;
    }
  }

  if(next->size() > 0.0){
    n = new StringReference();
    n->string = next;
    split = lAddString(split, n);
  }

  return split;
}
bool strStringIsBefore(vector<wchar_t> *a, vector<wchar_t> *b){
  bool before, equal, done;
  double i;

  before = false;
  equal = true;
  done = false;

  if(a->size() == 0.0 && b->size() > 0.0){
    before = true;
  }else{
    for(i = 0.0; i < a->size() && i < b->size() &&  !done ; i = i + 1.0){
      if(a->at(i) != b->at(i)){
        equal = false;
      }
      if(charCharacterIsBefore(a->at(i), b->at(i))){
        before = true;
      }
      if(charCharacterIsBefore(b->at(i), a->at(i))){
        done = true;
      }
    }

    if(equal){
      if(a->size() < b->size()){
        before = true;
      }
    }
  }

  return before;
}
vector<double> *StringToNumberArray(vector<wchar_t> *string){
  double i;
  vector<double> *array;

  array = new vector<double> (string->size());

  for(i = 0.0; i < string->size(); i = i + 1.0){
    array->at(i) = string->at(i);
  }
  return array;
}
vector<wchar_t> *NumberArrayToString(vector<double> *array){
  double i;
  vector<wchar_t> *string;

  string = new vector<wchar_t> (array->size());

  for(i = 0.0; i < array->size(); i = i + 1.0){
    string->at(i) = array->at(i);
  }
  return string;
}
bool NumberArraysEqual(vector<double> *a, vector<double> *b){
  bool equal;
  double i;

  equal = true;
  if(a->size() == b->size()){
    for(i = 0.0; i < a->size() && equal; i = i + 1.0){
      if(a->at(i) != b->at(i)){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
bool BooleanArraysEqual(vector<bool> *a, vector<bool> *b){
  bool equal;
  double i;

  equal = true;
  if(a->size() == b->size()){
    for(i = 0.0; i < a->size() && equal; i = i + 1.0){
      if(a->at(i) != b->at(i)){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
bool StringsEqual(vector<wchar_t> *a, vector<wchar_t> *b){
  bool equal;
  double i;

  equal = true;
  if(a->size() == b->size()){
    for(i = 0.0; i < a->size() && equal; i = i + 1.0){
      if(a->at(i) != b->at(i)){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
void FillNumberArray(vector<double> *a, double value){
  double i;

  for(i = 0.0; i < a->size(); i = i + 1.0){
    a->at(i) = value;
  }
}
void FillString(vector<wchar_t> *a, wchar_t value){
  double i;

  for(i = 0.0; i < a->size(); i = i + 1.0){
    a->at(i) = value;
  }
}
void FillBooleanArray(vector<bool> *a, bool value){
  double i;

  for(i = 0.0; i < a->size(); i = i + 1.0){
    a->at(i) = value;
  }
}
bool FillNumberArrayRange(vector<double> *a, double value, double from, double to){
  double i, length;
  bool success;

  if(from >= 0.0 && from <= a->size() && to >= 0.0 && to <= a->size() && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a->at(from + i) = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
bool FillBooleanArrayRange(vector<bool> *a, bool value, double from, double to){
  double i, length;
  bool success;

  if(from >= 0.0 && from <= a->size() && to >= 0.0 && to <= a->size() && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a->at(from + i) = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
bool FillStringRange(vector<wchar_t> *a, wchar_t value, double from, double to){
  double i, length;
  bool success;

  if(from >= 0.0 && from <= a->size() && to >= 0.0 && to <= a->size() && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a->at(from + i) = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
vector<double> *CopyNumberArray(vector<double> *a){
  double i;
  vector<double> *n;

  n = new vector<double> (a->size());

  for(i = 0.0; i < a->size(); i = i + 1.0){
    n->at(i) = a->at(i);
  }

  return n;
}
vector<bool> *CopyBooleanArray(vector<bool> *a){
  double i;
  vector<bool> *n;

  n = new vector<bool> (a->size());

  for(i = 0.0; i < a->size(); i = i + 1.0){
    n->at(i) = a->at(i);
  }

  return n;
}
vector<wchar_t> *CopyString(vector<wchar_t> *a){
  double i;
  vector<wchar_t> *n;

  n = new vector<wchar_t> (a->size());

  for(i = 0.0; i < a->size(); i = i + 1.0){
    n->at(i) = a->at(i);
  }

  return n;
}
bool CopyNumberArrayRange(vector<double> *a, double from, double to, NumberArrayReference *copyReference){
  double i, length;
  vector<double> *n;
  bool success;

  if(from >= 0.0 && from <= a->size() && to >= 0.0 && to <= a->size() && from <= to){
    length = to - from;
    n = new vector<double> (length);

    for(i = 0.0; i < length; i = i + 1.0){
      n->at(i) = a->at(from + i);
    }

    copyReference->numberArray = n;
    success = true;
  }else{
    success = false;
  }

  return success;
}
bool CopyBooleanArrayRange(vector<bool> *a, double from, double to, BooleanArrayReference *copyReference){
  double i, length;
  vector<bool> *n;
  bool success;

  if(from >= 0.0 && from <= a->size() && to >= 0.0 && to <= a->size() && from <= to){
    length = to - from;
    n = new vector<bool> (length);

    for(i = 0.0; i < length; i = i + 1.0){
      n->at(i) = a->at(from + i);
    }

    copyReference->booleanArray = n;
    success = true;
  }else{
    success = false;
  }

  return success;
}
bool CopyStringRange(vector<wchar_t> *a, double from, double to, StringReference *copyReference){
  double i, length;
  vector<wchar_t> *n;
  bool success;

  if(from >= 0.0 && from <= a->size() && to >= 0.0 && to <= a->size() && from <= to){
    length = to - from;
    n = new vector<wchar_t> (length);

    for(i = 0.0; i < length; i = i + 1.0){
      n->at(i) = a->at(from + i);
    }

    copyReference->string = n;
    success = true;
  }else{
    success = false;
  }

  return success;
}
bool IsLastElement(double length, double index){
  return index + 1.0 == length;
}
vector<double> *CreateNumberArray(double length, double value){
  vector<double> *array;

  array = new vector<double> (length);
  FillNumberArray(array, value);

  return array;
}
vector<bool> *CreateBooleanArray(double length, bool value){
  vector<bool> *array;

  array = new vector<bool> (length);
  FillBooleanArray(array, value);

  return array;
}
vector<wchar_t> *CreateString(double length, wchar_t value){
  vector<wchar_t> *array;

  array = new vector<wchar_t> (length);
  FillString(array, value);

  return array;
}
void SwapElementsOfNumberArray(vector<double> *A, double ai, double bi){
  double tmp;

  tmp = A->at(ai);
  A->at(ai) = A->at(bi);
  A->at(bi) = tmp;
}
void SwapElementsOfStringArray(StringArrayReference *A, double ai, double bi){
  StringReference *tmp;

  tmp = A->stringArray->at(ai);
  A->stringArray->at(ai) = A->stringArray->at(bi);
  A->stringArray->at(bi) = tmp;
}
void ReverseNumberArray(vector<double> *array){
  double i;

  for(i = 0.0; i < array->size()/2.0; i = i + 1.0){
    SwapElementsOfNumberArray(array, i, array->size() - i - 1.0);
  }
}
void AssertFalse(bool b, NumberReference *failures){
  if(b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertTrue(bool b, NumberReference *failures){
  if( !b ){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertEquals(double a, double b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertBooleansEqual(bool a, bool b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertCharactersEqual(wchar_t a, wchar_t b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertStringEquals(vector<wchar_t> *a, vector<wchar_t> *b, NumberReference *failures){
  if( !StringsEqual(a, b) ){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertNumberArraysEqual(vector<double> *a, vector<double> *b, NumberReference *failures){
  double i;

  if(a->size() == b->size()){
    for(i = 0.0; i < a->size(); i = i + 1.0){
      AssertEquals(a->at(i), b->at(i), failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertBooleanArraysEqual(vector<bool> *a, vector<bool> *b, NumberReference *failures){
  double i;

  if(a->size() == b->size()){
    for(i = 0.0; i < a->size(); i = i + 1.0){
      AssertBooleansEqual(a->at(i), b->at(i), failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertStringArraysEqual(vector<StringReference*> *a, vector<StringReference*> *b, NumberReference *failures){
  double i;

  if(a->size() == b->size()){
    for(i = 0.0; i < a->size(); i = i + 1.0){
      AssertStringEquals(a->at(i)->string, b->at(i)->string, failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
vector<wchar_t> *nCreateStringScientificNotationDecimalFromNumber(double decimal){
  StringReference *mantissaReference, *exponentReference;
  double multiplier, inc;
  double exponent;
  bool done, isPositive;
  vector<wchar_t> *result;

  mantissaReference = new StringReference();
  exponentReference = new StringReference();
  result = new vector<wchar_t> (0.0);
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
    result = strAppendString(result, toVector(L"-"));
  }

  result = strAppendString(result, mantissaReference->string);
  result = strAppendString(result, toVector(L"e"));
  result = strAppendString(result, exponentReference->string);

  return result;
}
vector<wchar_t> *nCreateStringDecimalFromNumber(double decimal){
  StringReference *stringReference;

  stringReference = new StringReference();

  /* This will succeed because base = 10. */
  nCreateStringFromNumberWithCheck(decimal, 10.0, stringReference);

  return stringReference->string;
}
bool nCreateStringFromNumberWithCheck(double decimal, double base, StringReference *stringReference){
  vector<wchar_t> *string;
  double maximumDigits;
  double digitPosition;
  bool hasPrintedPoint, isPositive;
  double i, d;
  bool success;
  CharacterReference *characterReference;
  wchar_t c;

  isPositive = true;

  if(decimal < 0.0){
    isPositive = false;
    decimal =  -decimal;
  }

  if(decimal == 0.0){
    stringReference->string = toVector(L"0");
    success = true;
  }else{
    characterReference = new CharacterReference();

    if(IsInteger(base)){
      success = true;

      string = new vector<wchar_t> (0.0);

      maximumDigits = nGetMaximumDigitsForBase(base);

      digitPosition = nGetFirstDigitPosition(decimal, base);

      decimal = round(decimal*pow(base, maximumDigits - digitPosition - 1.0));

      hasPrintedPoint = false;

      if( !isPositive ){
        string = strAppendCharacter(string, '-');
      }

      /* Print leading zeros. */
      if(digitPosition < 0.0){
        string = strAppendCharacter(string, '0');
        string = strAppendCharacter(string, '.');
        hasPrintedPoint = true;
        for(i = 0.0; i <  -digitPosition - 1.0; i = i + 1.0){
          string = strAppendCharacter(string, '0');
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
            string = strAppendCharacter(string, '.');
          }
          hasPrintedPoint = true;
        }

        if(decimal == 0.0 && hasPrintedPoint){
        }else{
          success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
          if(success){
            c = characterReference->characterValue;
            string = strAppendCharacter(string, c);
          }
        }

        if(success){
          decimal = decimal - d*pow(base, maximumDigits - i - 1.0);
        }
      }

      if(success){
        /* Print trailing zeros. */
        for(i = 0.0; i < digitPosition - maximumDigits + 1.0; i = i + 1.0){
          string = strAppendCharacter(string, '0');
        }

        stringReference->string = string;
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
bool nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference *characterReference){
  vector<wchar_t> *numberTable;
  bool success;

  numberTable = nGetDigitCharacterTable();

  if(c < base || c < numberTable->size()){
    success = true;
    characterReference->characterValue = numberTable->at(c);
  }else{
    success = false;
  }

  return success;
}
vector<wchar_t> *nGetDigitCharacterTable(){
  vector<wchar_t> *numberTable;

  numberTable = toVector(L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

  return numberTable;
}
bool nCreateNumberFromDecimalStringWithCheck(vector<wchar_t> *string, NumberReference *decimalReference, StringReference *errorMessage){
  return nCreateNumberFromStringWithCheck(string, 10.0, decimalReference, errorMessage);
}
double nCreateNumberFromDecimalString(vector<wchar_t> *string){
  NumberReference *doubleReference;
  StringReference *stringReference;
  double number;

  doubleReference = CreateNumberReference(0.0);
  stringReference = CreateStringReference(toVector(L""));
  nCreateNumberFromStringWithCheck(string, 10.0, doubleReference, stringReference);
  number = doubleReference->numberValue;

  delete doubleReference;
  delete stringReference;

  return number;
}
bool nCreateNumberFromStringWithCheck(vector<wchar_t> *string, double base, NumberReference *numberReference, StringReference *errorMessage){
  bool success;
  BooleanReference *numberIsPositive, *exponentIsPositive;
  NumberArrayReference *beforePoint, *afterPoint, *exponent;

  numberIsPositive = CreateBooleanReference(true);
  exponentIsPositive = CreateBooleanReference(true);
  beforePoint = new NumberArrayReference();
  afterPoint = new NumberArrayReference();
  exponent = new NumberArrayReference();

  if(base >= 2.0 && base <= 36.0){
    success = nExtractPartsFromNumberString(string, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage);

    if(success){
      numberReference->numberValue = nCreateNumberFromParts(base, numberIsPositive->booleanValue, beforePoint->numberArray, afterPoint->numberArray, exponentIsPositive->booleanValue, exponent->numberArray);
    }
  }else{
    success = false;
    errorMessage->string = toVector(L"Base must be from 2 to 36.");
  }

  return success;
}
double nCreateNumberFromParts(double base, bool numberIsPositive, vector<double> *beforePoint, vector<double> *afterPoint, bool exponentIsPositive, vector<double> *exponent){
  double n, i, p, e;

  n = 0.0;

  for(i = 0.0; i < beforePoint->size(); i = i + 1.0){
    p = beforePoint->at(beforePoint->size() - i - 1.0);

    n = n + p*pow(base, i);
  }

  for(i = 0.0; i < afterPoint->size(); i = i + 1.0){
    p = afterPoint->at(i);

    n = n + p*pow(base,  -(i + 1.0));
  }

  if(exponent->size() > 0.0){
    e = 0.0;
    for(i = 0.0; i < exponent->size(); i = i + 1.0){
      p = exponent->at(exponent->size() - i - 1.0);

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
bool nExtractPartsFromNumberString(vector<wchar_t> *n, double base, BooleanReference *numberIsPositive, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  double i;
  bool success;

  i = 0.0;

  if(i < n->size()){
    if(n->at(i) == '-'){
      numberIsPositive->booleanValue = false;
      i = i + 1.0;
    }else if(n->at(i) == '+'){
      numberIsPositive->booleanValue = true;
      i = i + 1.0;
    }

    success = nExtractPartsFromNumberStringFromSign(n, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages);
  }else{
    success = false;
    errorMessages->string = toVector(L"Number cannot have length zero.");
  }

  return success;
}
bool nExtractPartsFromNumberStringFromSign(vector<wchar_t> *n, double base, double i, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  bool success, done;
  double count, j;

  done = false;
  count = 0.0;
  for(; i + count < n->size() &&  !done ; ){
    if(nCharacterIsNumberCharacterInBase(n->at(i + count), base)){
      count = count + 1.0;
    }else{
      done = true;
    }
  }

  if(count >= 1.0){
    beforePoint->numberArray = new vector<double> (count);

    for(j = 0.0; j < count; j = j + 1.0){
      beforePoint->numberArray->at(j) = nGetNumberFromNumberCharacterForBase(n->at(i + j), base);
    }

    i = i + count;

    if(i < n->size()){
      success = nExtractPartsFromNumberStringFromPointOrExponent(n, base, i, afterPoint, exponentIsPositive, exponent, errorMessages);
    }else{
      afterPoint->numberArray = new vector<double> (0.0);
      exponent->numberArray = new vector<double> (0.0);
      success = true;
    }
  }else{
    success = false;
    errorMessages->string = toVector(L"Number must have at least one number after the optional sign.");
  }

  return success;
}
bool nExtractPartsFromNumberStringFromPointOrExponent(vector<wchar_t> *n, double base, double i, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  bool success, done;
  double count, j;

  if(n->at(i) == '.'){
    i = i + 1.0;

    if(i < n->size()){
      done = false;
      count = 0.0;
      for(; i + count < n->size() &&  !done ; ){
        if(nCharacterIsNumberCharacterInBase(n->at(i + count), base)){
          count = count + 1.0;
        }else{
          done = true;
        }
      }

      if(count >= 1.0){
        afterPoint->numberArray = new vector<double> (count);

        for(j = 0.0; j < count; j = j + 1.0){
          afterPoint->numberArray->at(j) = nGetNumberFromNumberCharacterForBase(n->at(i + j), base);
        }

        i = i + count;

        if(i < n->size()){
          success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages);
        }else{
          exponent->numberArray = new vector<double> (0.0);
          success = true;
        }
      }else{
        success = false;
        errorMessages->string = toVector(L"There must be at least one digit after the decimal point.");
      }
    }else{
      success = false;
      errorMessages->string = toVector(L"There must be at least one digit after the decimal point.");
    }
  }else if(base <= 14.0 && (n->at(i) == 'e' || n->at(i) == 'E')){
    if(i < n->size()){
      success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages);
      afterPoint->numberArray = new vector<double> (0.0);
    }else{
      success = false;
      errorMessages->string = toVector(L"There must be at least one digit after the exponent.");
    }
  }else{
    success = false;
    errorMessages->string = toVector(L"Expected decimal point or exponent symbol.");
  }

  return success;
}
bool nExtractPartsFromNumberStringFromExponent(vector<wchar_t> *n, double base, double i, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  bool success, done;
  double count, j;

  if(base <= 14.0 && (n->at(i) == 'e' || n->at(i) == 'E')){
    i = i + 1.0;

    if(i < n->size()){
      if(n->at(i) == '-'){
        exponentIsPositive->booleanValue = false;
        i = i + 1.0;
      }else if(n->at(i) == '+'){
        exponentIsPositive->booleanValue = true;
        i = i + 1.0;
      }

      if(i < n->size()){
        done = false;
        count = 0.0;
        for(; i + count < n->size() &&  !done ; ){
          if(nCharacterIsNumberCharacterInBase(n->at(i + count), base)){
            count = count + 1.0;
          }else{
            done = true;
          }
        }

        if(count >= 1.0){
          exponent->numberArray = new vector<double> (count);

          for(j = 0.0; j < count; j = j + 1.0){
            exponent->numberArray->at(j) = nGetNumberFromNumberCharacterForBase(n->at(i + j), base);
          }

          i = i + count;

          if(i == n->size()){
            success = true;
          }else{
            success = false;
            errorMessages->string = toVector(L"There cannot be any characters past the exponent of the number.");
          }
        }else{
          success = false;
          errorMessages->string = toVector(L"There must be at least one digit after the decimal point.");
        }
      }else{
        success = false;
        errorMessages->string = toVector(L"There must be at least one digit after the exponent symbol.");
      }
    }else{
      success = false;
      errorMessages->string = toVector(L"There must be at least one digit after the exponent symbol.");
    }
  }else{
    success = false;
    errorMessages->string = toVector(L"Expected exponent symbol.");
  }

  return success;
}
double nGetNumberFromNumberCharacterForBase(wchar_t c, double base){
  vector<wchar_t> *numberTable;
  double i;
  double position;

  numberTable = nGetDigitCharacterTable();
  position = 0.0;

  for(i = 0.0; i < base; i = i + 1.0){
    if(numberTable->at(i) == c){
      position = i;
    }
  }

  return position;
}
bool nCharacterIsNumberCharacterInBase(wchar_t c, double base){
  vector<wchar_t> *numberTable;
  double i;
  bool found;

  numberTable = nGetDigitCharacterTable();
  found = false;

  for(i = 0.0; i < base; i = i + 1.0){
    if(numberTable->at(i) == c){
      found = true;
    }
  }

  return found;
}
vector<double> *nStringToNumberArray(vector<wchar_t> *str){
  NumberArrayReference *numberArrayReference;
  StringReference *stringReference;
  vector<double> *numbers;

  numberArrayReference = new NumberArrayReference();
  stringReference = new StringReference();

  nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference);

  numbers = numberArrayReference->numberArray;

  delete numberArrayReference;
  delete stringReference;

  return numbers;
}
bool nStringToNumberArrayWithCheck(vector<wchar_t> *str, NumberArrayReference *numberArrayReference, StringReference *errorMessage){
  vector<StringReference*> *numberStrings;
  vector<double> *numbers;
  double i;
  vector<wchar_t> *numberString, *trimmedNumberString;
  bool success;
  NumberReference *numberReference;

  numberStrings = strSplitByString(str, toVector(L","));

  numbers = new vector<double> (numberStrings->size());
  success = true;
  numberReference = new NumberReference();

  for(i = 0.0; i < numberStrings->size(); i = i + 1.0){
    numberString = numberStrings->at(i)->string;
    trimmedNumberString = strTrim(numberString);
    success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage);
    numbers->at(i) = numberReference->numberValue;

    FreeStringReference(numberStrings->at(i));
    delete trimmedNumberString;
  }

  delete numberStrings;
  delete numberReference;

  numberArrayReference->numberArray = numbers;

  return success;
}
vector<double> *lAddNumber(vector<double> *list, double a){
  vector<double> *newlist;
  double i;

  newlist = new vector<double> (list->size() + 1.0);
  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void lAddNumberRef(NumberArrayReference *list, double i){
  list->numberArray = lAddNumber(list->numberArray, i);
}
vector<double> *lRemoveNumber(vector<double> *list, double n){
  vector<double> *newlist;
  double i;

  newlist = new vector<double> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }
		
  return newlist;
}
double lGetNumberRef(NumberArrayReference *list, double i){
  return list->numberArray->at(i);
}
void lRemoveNumberRef(NumberArrayReference *list, double i){
  list->numberArray = lRemoveNumber(list->numberArray, i);
}
vector<StringReference*> *lAddString(vector<StringReference*> *list, StringReference *a){
  vector<StringReference*> *newlist;
  double i;

  newlist = new vector<StringReference*> (list->size() + 1.0);

  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void lAddStringRef(StringArrayReference *list, StringReference *i){
  list->stringArray = lAddString(list->stringArray, i);
}
vector<StringReference*> *lRemoveString(vector<StringReference*> *list, double n){
  vector<StringReference*> *newlist;
  double i;

  newlist = new vector<StringReference*> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }
		
  return newlist;
}
StringReference *lGetStringRef(StringArrayReference *list, double i){
  return list->stringArray->at(i);
}
void lRemoveStringRef(StringArrayReference *list, double i){
  list->stringArray = lRemoveString(list->stringArray, i);
}
vector<bool> *lAddBoolean(vector<bool> *list, bool a){
  vector<bool> *newlist;
  double i;

  newlist = new vector<bool> (list->size() + 1.0);
  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void lAddBooleanRef(BooleanArrayReference *list, bool i){
  list->booleanArray = lAddBoolean(list->booleanArray, i);
}
vector<bool> *lRemoveBoolean(vector<bool> *list, double n){
  vector<bool> *newlist;
  double i;

  newlist = new vector<bool> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }
		
  return newlist;
}
bool lGetBooleanRef(BooleanArrayReference *list, double i){
  return list->booleanArray->at(i);
}
void lRemoveDecimalRef(BooleanArrayReference *list, double i){
  list->booleanArray = lRemoveBoolean(list->booleanArray, i);
}
lLinkedListStrings *lCreateLinkedListString(){
  lLinkedListStrings *ll;

  ll = new lLinkedListStrings();
  ll->first = new lLinkedListNodeStrings();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void lLinkedListAddString(lLinkedListStrings *ll, vector<wchar_t> *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new lLinkedListNodeStrings();
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
vector<StringReference*> *lLinkedListStringsToArray(lLinkedListStrings *ll){
  vector<StringReference*> *array;
  double length, i;
  lLinkedListNodeStrings *node;

  node = ll->first;

  length = lLinkedListStringsLength(ll);

  array = new vector<StringReference*> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = new StringReference();
    array->at(i)->string = node->value;
    node = node->next;
  }

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
    delete prev;
  }

  delete node;
}
lLinkedListNumbers *lCreateLinkedListNumbers(){
  lLinkedListNumbers *ll;

  ll = new lLinkedListNumbers();
  ll->first = new lLinkedListNodeNumbers();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
vector<lLinkedListNumbers*> *lCreateLinkedListNumbersArray(double length){
  vector<lLinkedListNumbers*> *lls;
  double i;

  lls = new vector<lLinkedListNumbers*> (length);
  for(i = 0.0; i < lls->size(); i = i + 1.0){
    lls->at(i) = lCreateLinkedListNumbers();
  }

  return lls;
}
void lLinkedListAddNumber(lLinkedListNumbers *ll, double value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new lLinkedListNodeNumbers();
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
    ll->first = new lLinkedListNodeNumbers();
    ll->first->next = tmp;
    ll->first->value = value;
    ll->first->end = false;
  }else{
    node = ll->first;
    for(i = 0.0; i < index - 1.0; i = i + 1.0){
      node = node->next;
    }

    tmp = node->next;
    node->next = new lLinkedListNodeNumbers();
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
    delete prev;
  }

  delete node;
}
void lFreeLinkedListNumbersArray(vector<lLinkedListNumbers*> *lls){
  double i;

  for(i = 0.0; i < lls->size(); i = i + 1.0){
    lFreeLinkedListNumbers(lls->at(i));
  }
  delete lls;
}
vector<double> *lLinkedListNumbersToArray(lLinkedListNumbers *ll){
  vector<double> *array;
  double length, i;
  lLinkedListNodeNumbers *node;

  node = ll->first;

  length = lLinkedListNumbersLength(ll);

  array = new vector<double> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = node->value;
    node = node->next;
  }

  return array;
}
lLinkedListNumbers *lArrayToLinkedListNumbers(vector<double> *array){
  lLinkedListNumbers *ll;
  double i;

  ll = lCreateLinkedListNumbers();

  for(i = 0.0; i < array->size(); i = i + 1.0){
    lLinkedListAddNumber(ll, array->at(i));
  }

  return ll;
}
bool lLinkedListNumbersEqual(lLinkedListNumbers *a, lLinkedListNumbers *b){
  bool equal, done;
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

  ll = new lLinkedListCharacters();
  ll->first = new lLinkedListNodeCharacters();
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void lLinkedListAddCharacter(lLinkedListCharacters *ll, wchar_t value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = new lLinkedListNodeCharacters();
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
vector<wchar_t> *lLinkedListCharactersToArray(lLinkedListCharacters *ll){
  vector<wchar_t> *array;
  double length, i;
  lLinkedListNodeCharacters *node;

  node = ll->first;

  length = lLinkedListCharactersLength(ll);

  array = new vector<wchar_t> (length);

  for(i = 0.0; i < length; i = i + 1.0){
    array->at(i) = node->value;
    node = node->next;
  }

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
    delete prev;
  }

  delete node;
}
lDynamicArrayNumbers *lCreateDynamicArrayNumbers(){
  lDynamicArrayNumbers *da;

  da = new lDynamicArrayNumbers();
  da->array = new vector<double> (10.0);
  da->length = 0.0;

  return da;
}
lDynamicArrayNumbers *lCreateDynamicArrayNumbersWithInitialCapacity(double capacity){
  lDynamicArrayNumbers *da;

  da = new lDynamicArrayNumbers();
  da->array = new vector<double> (capacity);
  da->length = 0.0;

  return da;
}
void lDynamicArrayAddNumber(lDynamicArrayNumbers *da, double value){
  if(da->length == da->array->size()){
    lDynamicArrayNumbersIncreaseSize(da);
  }

  da->array->at(da->length) = value;
  da->length = da->length + 1.0;
}
void lDynamicArrayNumbersIncreaseSize(lDynamicArrayNumbers *da){
  double newLength, i;
  vector<double> *newArray;

  newLength = round(da->array->size()*3.0/2.0);
  newArray = new vector<double> (newLength);

  for(i = 0.0; i < da->array->size(); i = i + 1.0){
    newArray->at(i) = da->array->at(i);
  }

  delete da->array;

  da->array = newArray;
}
bool lDynamicArrayNumbersDecreaseSizeNecessary(lDynamicArrayNumbers *da){
  bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->array->size()*2.0/3.0);
  }

  return needsDecrease;
}
void lDynamicArrayNumbersDecreaseSize(lDynamicArrayNumbers *da){
  double newLength, i;
  vector<double> *newArray;

  newLength = round(da->array->size()*2.0/3.0);
  newArray = new vector<double> (newLength);

  for(i = 0.0; i < da->array->size(); i = i + 1.0){
    newArray->at(i) = da->array->at(i);
  }

  delete da->array;

  da->array = newArray;
}
double lDynamicArrayNumbersIndex(lDynamicArrayNumbers *da, double index){
  return da->array->at(index);
}
double lDynamicArrayNumbersLength(lDynamicArrayNumbers *da){
  return da->length;
}
void lDynamicArrayInsertNumber(lDynamicArrayNumbers *da, double index, double value){
  double i;

  if(da->length == da->array->size()){
    lDynamicArrayNumbersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array->at(i) = da->array->at(i - 1.0);
  }

  da->array->at(index) = value;

  da->length = da->length + 1.0;
}
void lDynamicArraySet(lDynamicArrayNumbers *da, double index, double value){
  da->array->at(index) = value;
}
void lDynamicArrayRemoveNumber(lDynamicArrayNumbers *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array->at(i) = da->array->at(i + 1.0);
  }

  da->length = da->length - 1.0;

  if(lDynamicArrayNumbersDecreaseSizeNecessary(da)){
    lDynamicArrayNumbersDecreaseSize(da);
  }
}
void lFreeDynamicArrayNumbers(lDynamicArrayNumbers *da){
  delete da->array;
  delete da;
}
vector<double> *lDynamicArrayNumbersToArray(lDynamicArrayNumbers *da){
  vector<double> *array;
  double i;

  array = new vector<double> (da->length);

  for(i = 0.0; i < da->length; i = i + 1.0){
    array->at(i) = da->array->at(i);
  }

  return array;
}
lDynamicArrayNumbers *lArrayToDynamicArrayNumbersWithOptimalSize(vector<double> *array){
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
  c = array->size();
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = lCreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < array->size(); i = i + 1.0){
    da->array->at(i) = array->at(i);
  }

  return da;
}
lDynamicArrayNumbers *lArrayToDynamicArrayNumbers(vector<double> *array){
  lDynamicArrayNumbers *da;

  da = new lDynamicArrayNumbers();
  da->array = CopyNumberArray(array);
  da->length = array->size();

  return da;
}
bool lDynamicArrayNumbersEqual(lDynamicArrayNumbers *a, lDynamicArrayNumbers *b){
  bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array->at(i) != b->array->at(i)){
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
    lLinkedListAddNumber(ll, da->array->at(i));
  }

  return ll;
}
lDynamicArrayNumbers *lLinkedListToDynamicArrayNumbers(lLinkedListNumbers *ll){
  lDynamicArrayNumbers *da;
  double i;
  lLinkedListNodeNumbers *node;

  node = ll->first;

  da = new lDynamicArrayNumbers();
  da->length = lLinkedListNumbersLength(ll);

  da->array = new vector<double> (da->length);

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array->at(i) = node->value;
    node = node->next;
  }

  return da;
}
vector<wchar_t> *lAddCharacter(vector<wchar_t> *list, wchar_t a){
  vector<wchar_t> *newlist;
  double i;

  newlist = new vector<wchar_t> (list->size() + 1.0);
  for(i = 0.0; i < list->size(); i = i + 1.0){
    newlist->at(i) = list->at(i);
  }
  newlist->at(list->size()) = a;
		
  delete list;
		
  return newlist;
}
void lAddCharacterRef(StringReference *list, wchar_t i){
  list->string = lAddCharacter(list->string, i);
}
vector<wchar_t> *lRemoveCharacter(vector<wchar_t> *list, double n){
  vector<wchar_t> *newlist;
  double i;

  newlist = new vector<wchar_t> (list->size() - 1.0);

  if(n >= 0.0 && n < list->size()){
    for(i = 0.0; i < list->size(); i = i + 1.0){
      if(i < n){
        newlist->at(i) = list->at(i);
      }
      if(i > n){
        newlist->at(i - 1.0) = list->at(i);
      }
    }

    delete list;
  }else{
    delete newlist;
  }

  return newlist;
}
wchar_t lGetCharacterRef(StringReference *list, double i){
  return list->string->at(i);
}
void lRemoveCharacterRef(StringReference *list, double i){
  list->string = lRemoveCharacter(list->string, i);
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
  return abs(x);
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
bool DivisibleBy(double a, double b){
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
bool EpsilonCompare(double a, double b, double epsilon){
  return abs(a - b) < epsilon;
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
bool IsInteger(double a){
  return (a - floor(a)) == 0.0;
}
bool GreatestCommonDivisorWithCheck(double a, double b, NumberReference *gcdReference){
  bool success;
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
    lcm = abs(a*b)/GreatestCommonDivisor(a, b);
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
  vector<double> *p;
  double i, y, t, x;

  p = new vector<double> (8.0);
  p->at(0) = 676.5203681218851;
  p->at(1) =  -1259.1392167224028;
  p->at(2) = 771.32342877765313;
  p->at(3) =  -176.61502916214059;
  p->at(4) = 12.507343278686905;
  p->at(5) =  -0.13857109526572012;
  p->at(6) = 9.9843695780195716e-6;
  p->at(7) = 1.5056327351493116e-7;

  if(z < 0.5){
    y = M_PI/(sin(M_PI*z)*LanczosApproximation(1.0 - z));
  }else{
    z = z - 1.0;
    x = 0.99999999999980993;
    for(i = 0.0; i < p->size(); i = i + 1.0){
      x = x + p->at(i)/(z + i + 1.0);
    }
    t = z + p->size() - 0.5;
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

    t = 1.0/(1.0 + 0.5*abs(x));

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

  if(abs(z) >= 0.5){
    y = pow(1.0 - z,  -a)*HypergeometricDirect(a, c - b, c, z/(z - 1.0), maxIterations, precision);
  }else{
    y = HypergeometricDirect(a, b, c, z, maxIterations, precision);
  }

  return y;
}
double HypergeometricDirect(double a, double b, double c, double z, double maxIterations, double precision){
  double y, yp, n;
  bool done;

  y = 0.0;
  done = false;

  for(n = 0.0; n < maxIterations &&  !done ; n = n + 1.0){
    yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*pow(z, n)/Factorial(n);
    if(abs(yp) < precision){
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
  vector<double> *A;

  A = new vector<double> (n + 1.0);

  for(m = 0.0; m <= n; m = m + 1.0){
    A->at(m) = 1.0/(m + 1.0);
    for(j = m; j >= 1.0; j = j - 1.0){
      A->at(j - 1.0) = j*(A->at(j - 1.0) - A->at(j));
    }
  }

  B = A->at(0);

  delete A;

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
bool charIsUpperCase(wchar_t character){
  bool isUpper;

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
bool charIsLowerCase(wchar_t character){
  bool isLower;

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
bool charIsLetter(wchar_t character){
  return charIsUpperCase(character) || charIsLowerCase(character);
}
bool charIsNumber(wchar_t character){
  bool isNumberx;

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
bool charIsWhiteSpace(wchar_t character){
  bool isWhiteSpacex;

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
bool charIsSymbol(wchar_t character){
  bool isSymbolx;

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
bool charCharacterIsBefore(wchar_t a, wchar_t b){
  double ad, bd;

  ad = a;
  bd = b;

  return ad < bd;
}

