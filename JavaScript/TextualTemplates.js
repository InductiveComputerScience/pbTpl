// Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

function CreateLinkedListNodes(){
  var ll;

  ll = {};
  ll.first = {};
  ll.last = ll.first;
  ll.last.end = true;

  return ll;
}
function LinkedListAddNode(ll, value){
  ll.last.end = false;
  ll.last.value = value;
  ll.last.next = {};
  ll.last.next.end = true;
  ll.last = ll.last.next;
}
function LinkedListNodesToArray(ll){
  var array;
  var lengthx, i;
  var node;

  node = ll.first;

  lengthx = LinkedListNodesLength(ll);

  array = [];
  array.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    array[i] = node.value;
    node = node.next;
  }

  return array;
}
function LinkedListNodesLength(ll){
  var l;
  var node;

  l = 0;
  node = ll.first;
  for(;  !node.end ; ){
    node = node.next;
    l = l + 1;
  }

  return l;
}
function FreeLinkedListNode(ll){
  var node, prev;

  node = ll.first;

  for(;  !node.end ; ){
    prev = node;
    node = node.next;
    delete(prev);
  }

  delete(node);
}
function IsValidTemplate(template){
  var errorMessage;
  var success;
  var tokens;
  var ts;
  var root;

  tokens = CreateLinkedListString();
  errorMessage = {};

  success = GenerateTokensFromTemplate(template, tokens, errorMessage);

  if(success){
    root = {};

    ts = LinkedListStringsToArray(tokens);

    success = ParseTemplate(ts, root, errorMessage);
  }

  if( !success ){
    FreeStringReference(errorMessage);
  }

  return success;
}
function GenerateTokensFromTemplate(template, tokens, errorMessage){
  var success, found;
  var i, j, l;
  var pc, c;
  var cs, a;
  var da;

  success = true;
  cs = [];
  cs.length = 4;

  da = CreateDynamicArrayCharacters();

  pc = 'x';
  for(i = 0; i < template.length && success; ){
    c = template[i];

    if(c != '{'){
      DynamicArrayAddCharacter(da, c);
      i = i + 1;
    }else{
      FillString(cs, 'x');
      for(j = 0; i + j < template.length && j < cs.length; j = j + 1){
        cs[j] = template[i + j];
      }

      if(StringsEqual(cs, "{use".split('')) || StringsEqual(cs, "{end".split('')) || StringsEqual(cs, "{pri".split('')) || StringsEqual(cs, "{for".split('')) || StringsEqual(cs, "{if ".split('')) || StringsEqual(cs, "{els".split(''))){
        if(pc != '\\'){
          /* Find end. */
          found = false;
          for(l = 0; i + l < template.length &&  !found ; l = l + 1){
            if(template[i + l] == '}'){
              found = true;
            }
          }

          if(found){
            if(da.lengthx > 0){
              a = DynamicArrayCharactersToArray(da);
              LinkedListAddString(tokens, a);
              FreeDynamicArrayCharacters(da);
              da = CreateDynamicArrayCharacters();
            }

            for(j = 0; j < l; j = j + 1){
              DynamicArrayAddCharacter(da, template[i + j]);
            }

            a = DynamicArrayCharactersToArray(da);
            LinkedListAddString(tokens, a);
            FreeDynamicArrayCharacters(da);
            da = CreateDynamicArrayCharacters();

            i = i + l;
          }else{
            success = false;
            errorMessage.string = "Template command found, but not ended properly.".split('');
          }
        }else{
          DynamicArrayAddCharacter(da, c);
          i = i + 1;
        }
      }else{
        DynamicArrayAddCharacter(da, c);
        i = i + 1;
      }
    }

    pc = c;
  }

  if(da.lengthx > 0){
    a = DynamicArrayCharactersToArray(da);
    LinkedListAddString(tokens, a);
  }

  FreeDynamicArrayCharacters(da);

  return success;
}
function GenerateDocument(template, json, document, errorMessage){
  var data;
  var errorMessages;
  var success;

  data = {};
  errorMessages = {};

  success = ReadJSON(json, data, errorMessages);

  if(success){
    success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage);
  }else{
    errorMessage.string = JoinStringsWithSeparator(errorMessages.stringArray, ", ".split(''));
    FreeStringArrayReference(errorMessages);
  }

  return success;
}
function GenerateDocumentBasedOnElement(template, data, document, errorMessage){
  var ll;
  var success;
  var tokens;
  var ts;
  var root;

  tokens = CreateLinkedListString();

  success = GenerateTokensFromTemplate(template, tokens, errorMessage);

  if(success){
    root = {};

    ts = LinkedListStringsToArray(tokens);
    FreeLinkedListString(tokens);

    success = ParseTemplate(ts, root, errorMessage);

    if(success){
      ll = CreateLinkedListCharacter();

      success = GenerateDocumentFromBlock(root, data, ll, errorMessage);

      if(success){
        document.string = LinkedListCharactersToArray(ll);
        FreeLinkedListCharacter(ll);
      }
    }
  }

  return success;
}
function GenerateDocumentFromBlock(root, data, ll, errorMessage){
  var n;
  var success;

  n = root.nodes.first;
  success = true;

  for(;  !n.end  && success; ){
    success = GenerateDocumentFromNode(n.value, data, ll, errorMessage);
    n = n.next;
  }

  return success;
}
function GenerateDocumentFromNode(n, data, ll, errorMessage){
  var success;
  var found;
  var valueString;
  var value;

  success = true;
  found = {};

  if(StringsEqual(n.type, "block".split(''))){
    success = GenerateDocumentFromBlock(n, data, ll, errorMessage);
  }else if(StringsEqual(n.type, "use".split(''))){
    success = GenerateDocumentFromUse(n, data, ll, errorMessage);
  }else if(StringsEqual(n.type, "if".split(''))){
    success = GenerateDocumentFromIf(n, data, ll, errorMessage);
  }else if(StringsEqual(n.type, "foreach".split(''))){
    success = GenerateDocumentFromForeach(n, data, ll, errorMessage);
  }else if(StringsEqual(n.type, "text".split(''))){
    success = true;
    LinkedListCharactersAddString(ll, n.p1);
  }else if(StringsEqual(n.type, "print".split(''))){
    if(StringsEqual(data.type, "object".split(''))){
      value = GetObjectValueWithCheck(data.object, n.p1, found);

      if(found.booleanValue){
        if(StringsEqual(value.type, "string".split(''))){
          valueString = value.string;
        }else{
          valueString = WriteJSON(value);
        }
        LinkedListCharactersAddString(ll, valueString);
      }else{
        success = false;
        errorMessage.string = "Key for printing not found in JSON object: ".split('');
        errorMessage.string = ConcatenateString(errorMessage.string, n.p1);
      }
    }else{
      success = false;
      errorMessage.string = "Data structure for print command is not a JSON object.".split('');
    }
  }

  return success;
}
function GenerateDocumentFromUse(n, data, ll, errorMessage){
  var success;
  var found;
  var value;

  found = {};

  if(StringsEqual(data.type, "object".split(''))){
    value = GetObjectValueWithCheck(data.object, n.p1, found);

    if(found.booleanValue){
      success = GenerateDocumentFromNode(n.block1, value, ll, errorMessage);
    }else{
      success = false;
      errorMessage.string = "Key for use not found in JSON object.".split('');
    }
  }else{
    success = false;
    errorMessage.string = "Data structure for use command is not a JSON object.".split('');
  }

  return success;
}
function GenerateDocumentFromIf(n, data, ll, errorMessage){
  var success;
  var found;
  var value;

  success = true;
  found = {};

  if(StringsEqual(data.type, "object".split(''))){
    value = GetObjectValueWithCheck(data.object, n.p1, found);

    if(found.booleanValue){
      if(StringsEqual(value.type, "boolean".split(''))){
        if(value.booleanValue){
          success = GenerateDocumentFromBlock(n.block1, data, ll, errorMessage);
        }

        if(n.hasElseBlock){
          if( !value.booleanValue ){
            success = GenerateDocumentFromBlock(n.block2, data, ll, errorMessage);
          }
        }
      }else{
        success = false;
        errorMessage.string = "Value for if not boolean.".split('');
      }
    }else{
      success = false;
      errorMessage.string = "Key for if not found in JSON object: ".split('');
      errorMessage.string = ConcatenateString(errorMessage.string, n.p1);
    }
  }else{
    success = false;
    errorMessage.string = "Data structure for if command is not a JSON object.".split('');
  }

  return success;
}
function GenerateDocumentFromForeach(n, data, ll, errorMessage){
  var success;
  var found;
  var value, loopVar, arrayElement;
  var i;

  success = true;
  found = {};
  loopVar = CreateObjectElement(0);

  PutStringElementMap(loopVar.object, n.p1, {});

  if(StringsEqual(data.type, "object".split(''))){
    value = GetObjectValueWithCheck(data.object, n.p2, found);

    if(found.booleanValue){
      if(StringsEqual(value.type, "array".split(''))){

        for(i = 0; i < value.array.length; i = i + 1){
          arrayElement = value.array[i];
          if(StringsEqual(arrayElement.type, "object".split(''))){
            success = GenerateDocumentFromBlock(n.block1, arrayElement, ll, errorMessage);
          }else{
            SetStringElementMap(loopVar.object, 0, n.p1, arrayElement);
            success = GenerateDocumentFromBlock(n.block1, loopVar, ll, errorMessage);
          }
        }
      }else{
        success = false;
        errorMessage.string = "Value for foreach is not an array.".split('');
      }
    }else{
      success = false;
      errorMessage.string = "Key for foreach not found in JSON object: ".split('');
      errorMessage.string = ConcatenateString(errorMessage.string, n.p2);
    }
  }else{
    success = false;
    errorMessage.string = "Data structure for foreach command is not a JSON object.".split('');
  }

  return success;
}
function ParseTemplate(tokens, node, errorMessage){
  var success;
  var position;

  position = CreateNumberReference(0);
  success = ParseTemplateBlock(tokens, position, node, errorMessage);

  if(success){
    if(position.numberValue != tokens.length){
      success = false;
      errorMessage.string = "Unexpected token at the end of template.".split('');
    }
  }

  delete(position);

  return success;
}
function ParseTemplateBlock(tokens, position, node, errorMessage){
  var success, done;
  var tn, nb;

  success = true;
  done = false;

  node.type = "block".split('');
  node.nodes = CreateLinkedListNodes();

  for(; position.numberValue < tokens.length && success &&  !done ; ){
    tn = {};
    success = ParseNodeString(tokens[position.numberValue].string, tn, errorMessage);

    if(success){
      if(StringsEqual(tn.type, "text".split('')) || StringsEqual(tn.type, "print".split(''))){
        LinkedListAddNode(node.nodes, tn);
        position.numberValue = position.numberValue + 1;
      }else if(StringsEqual(tn.type, "use".split(''))){
        nb = {};
        success = ParseUseBlock(tokens, position, nb, errorMessage);
        LinkedListAddNode(node.nodes, nb);
      }else if(StringsEqual(tn.type, "if".split(''))){
        nb = {};
        success = ParseIfBlock(tokens, position, nb, errorMessage);
        LinkedListAddNode(node.nodes, nb);
      }else if(StringsEqual(tn.type, "foreach".split(''))){
        nb = {};
        success = ParseForeachBlock(tokens, position, nb, errorMessage);
        LinkedListAddNode(node.nodes, nb);
      }else{
        done = true;
      }
    }
  }

  return success;
}
function ParseUseBlock(tokens, position, useBlock, errorMessage){
  var success;
  var n;

  n = {};
  ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

  useBlock.type = CopyString(n.type);
  useBlock.p1 = CopyString(n.p1);
  useBlock.block1 = {};

  position.numberValue = position.numberValue + 1;

  success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage);

  if(success){
    if(position.numberValue < tokens.length){
      n = {};
      ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

      if(StringsEqual(n.type, "end".split(''))){
        success = true;
        position.numberValue = position.numberValue + 1;
      }else{
        success = false;
        errorMessage.string = "End block expected at the end of use block.".split('');
      }
    }else{
      success = false;
      errorMessage.string = "End block expected at the end of use block.".split('');
    }
  }

  return success;
}
function ParseIfBlock(tokens, position, ifBlock, errorMessage){
  var success;
  var n;

  n = {};
  ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

  ifBlock.type = CopyString(n.type);
  ifBlock.p1 = CopyString(n.p1);
  ifBlock.block1 = {};
  ifBlock.hasElseBlock = false;

  position.numberValue = position.numberValue + 1;

  success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage);

  if(success){
    if(position.numberValue < tokens.length){
      n = {};
      ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

      if(StringsEqual(n.type, "end".split(''))){
        success = true;
        position.numberValue = position.numberValue + 1;
      }else if(StringsEqual(n.type, "else".split(''))){
        position.numberValue = position.numberValue + 1;
        ifBlock.hasElseBlock = true;
        ifBlock.block2 = {};
        success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage);

        if(success){
          if(position.numberValue < tokens.length){
            n = {};
            ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

            if(StringsEqual(n.type, "end".split(''))){
              success = true;
              position.numberValue = position.numberValue + 1;
            }else{
              success = false;
              errorMessage.string = "End block expected at the end of else block.".split('');
            }
          }else{
            success = false;
            errorMessage.string = "End block expected at the end of else block.".split('');
          }
        }
      }else{
        success = false;
        errorMessage.string = "End or else block expected at the end of if block.".split('');
      }
    }else{
      success = false;
      errorMessage.string = "End or else block expected at the end of if block.".split('');
    }
  }

  return success;
}
function ParseForeachBlock(tokens, position, foreachBlock, errorMessage){
  var success;
  var n;

  n = {};
  ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

  foreachBlock.type = CopyString(n.type);
  foreachBlock.p1 = CopyString(n.p1);
  foreachBlock.p2 = CopyString(n.p2);
  foreachBlock.block1 = {};

  position.numberValue = position.numberValue + 1;

  success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage);

  if(success){
    if(position.numberValue < tokens.length){
      n = {};
      ParseNodeString(tokens[position.numberValue].string, n, errorMessage);

      if(StringsEqual(n.type, "end".split(''))){
        success = true;
        position.numberValue = position.numberValue + 1;
      }else{
        success = false;
        errorMessage.string = "End block expected at the end of for each block.".split('');
      }
    }else{
      success = false;
      errorMessage.string = "End block expected at the end of foreach block.".split('');
    }
  }

  return success;
}
function ParseNodeString(token, node, errorMessage){
  var command;
  var success, isText;
  var parts;

  success = true;
  isText = false;

  if(token.length <= 2){
    isText = true;
  }else if(token[0] == '\\' && token[1] == '{'){
    isText = true;
  }else if(token[0] != '{'){
    isText = true;
  }else{
    command = Substring(token, 1, token.length - 1);
    parts = SplitByCharacter(command, ' ');

    if(command.length > 0){
      if(StringsEqual(parts[0].string, "use".split(''))){
        if(parts.length == 2){
          node.type = CopyString(parts[0].string);
          node.p1 = CopyString(parts[1].string);
        }else{
          success = false;
          errorMessage.string = "The use command takes one parameter.".split('');
        }
      }else if(StringsEqual(parts[0].string, "end".split(''))){
        if(parts.length == 1){
          node.type = CopyString(parts[0].string);
        }else{
          success = false;
          errorMessage.string = "The end command takes no parameters.".split('');
        }
      }else if(StringsEqual(parts[0].string, "print".split(''))){
        if(parts.length == 2){
          node.type = CopyString(parts[0].string);
          node.p1 = CopyString(parts[1].string);
        }else{
          success = false;
          errorMessage.string = "The print command takes one parameter.".split('');
        }
      }else if(StringsEqual(parts[0].string, "foreach".split(''))){
        if(parts.length == 4){
          if(StringsEqual(parts[2].string, "in".split(''))){
            node.type = CopyString(parts[0].string);
            node.p1 = CopyString(parts[1].string);
            node.p2 = CopyString(parts[3].string);
          }else{
            success = false;
            errorMessage.string = "The foreach command must have \"in\" after the first parameter.".split('');
          }
        }else{
          success = false;
          errorMessage.string = "The foreach command takes three parameters.".split('');
        }
      }else if(StringsEqual(parts[0].string, "if".split(''))){
        if(parts.length == 2){
          node.type = CopyString(parts[0].string);
          node.p1 = CopyString(parts[1].string);
        }else{
          success = false;
          errorMessage.string = "The if command takes one parameter.".split('');
        }
      }else if(StringsEqual(parts[0].string, "else".split(''))){
        if(parts.length == 1){
          node.type = CopyString(parts[0].string);
        }else{
          success = false;
          errorMessage.string = "The else command takes no parameters.".split('');
        }
      }else{
        isText = true;
      }
    }else{
      isText = true;
    }
  }

  if(isText){
    node.type = "text".split('');
    node.p1 = ReplaceString(token, "\\{print ".split(''), "{print ".split(''));
    node.p1 = ReplaceString(node.p1, "\\{use ".split(''), "{use ".split(''));
    node.p1 = ReplaceString(node.p1, "\\{if ".split(''), "{if ".split(''));
    node.p1 = ReplaceString(node.p1, "\\{end}".split(''), "{end}".split(''));
    node.p1 = ReplaceString(node.p1, "\\{foreach ".split(''), "{foreach ".split(''));
    node.p1 = ReplaceString(node.p1, "\\{else}".split(''), "{else}".split(''));
  }

  return success;
}
function test(){
  var failures;

  failures = CreateNumberReference(0);

  testTokenGeneration(failures);

  testGenerateDocument1(failures);
  testGenerateDocument2(failures);
  testGenerateDocument3(failures);
  testGenerateDocument4(failures);
  testGenerateDocument5(failures);
  testGenerateDocument6(failures);
  testGenerateDocument7(failures);
  testGenerateDocument8(failures);

  return failures.numberValue;
}
function testGenerateDocument8(failures){
  var document, errorMessage;
  var success;
  var template, json;

  document = {};
  errorMessage = {};

  template = "This is a test: {print b} {foreach x in a}{print x}{end}.".split('');
  json = "{\"a\": [1, 2, 3], \"b\": 4}".split('');
  success = GenerateDocument(template, json, document, errorMessage);

  if(success){
    AssertStringEquals("This is a test: 4 123.".split(''), document.string, failures);
  }

  AssertTrue(success, failures);
}
function testTokenGeneration(failures){
  var template;
  var tokens;
  var success;
  var errorMessage;

  errorMessage = {};

  tokens = CreateLinkedListString();
  template = "This is a template, this is a value: {print a}.".split('');
  success = GenerateTokensFromTemplate(template, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(3, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".split('');
  success = GenerateTokensFromTemplate(template, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(9, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".split('');
  success = GenerateTokensFromTemplate(template, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(16, LinkedListStringsLength(tokens), failures);

  tokens = CreateLinkedListString();
  template = "T: {foreach a in b}{print a}{end}.".split('');
  success = GenerateTokensFromTemplate(template, tokens, errorMessage);
  AssertTrue(success, failures);
  AssertEquals(5, LinkedListStringsLength(tokens), failures);
}
function testGenerateDocument1(failures){
  AssertTemplateResult("This is a template, this is a value: {print a}.".split(''), "{\"c\": 5, \"a\": 6}".split(''), "This is a template, this is a value: 6.".split(''), failures);
}
function testGenerateDocument2(failures){
  AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}".split(''), "This is a template, this is a value: 6 1 2.".split(''), failures);
}
function testGenerateDocument3(failures){
  AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}".split(''), "This is a template, this is a value: 6 1 23 416.".split(''), failures);
}
function testGenerateDocument4(failures){
  AssertTemplateResult("T: {if a}a{end}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}".split(''), "T: a.".split(''), failures);

  AssertTemplateResult("T: {if a}a{else}b{end}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}".split(''), "T: b.".split(''), failures);
}
function testGenerateDocument5(failures){
  AssertTemplateResult("T: {foreach a in b}{print a}{end}.".split(''), "{\"b\": [1, 2, 3, 4]}".split(''), "T: 1234.".split(''), failures);
}
function testGenerateDocument6(failures){
  AssertTemplateResult("{test}\\{print a}.".split(''), "{\"c\": 5, \"a\": 6}".split(''), "{test}{print a}.".split(''), failures);
}
function testGenerateDocument7(failures){
  AssertTemplateResult("{".split(''), "{}".split(''), "{".split(''), failures);

  AssertTemplateResult("{  ".split(''), "{}".split(''), "{  ".split(''), failures);

  AssertTemplateResult("{use a}\\{print a}{end}".split(''), "{\"a\": {}}".split(''), "{print a}".split(''), failures);

  AssertTemplateResult("\\{print a}{print a}}".split(''), "{\"a\": 3}".split(''), "{print a}3}".split(''), failures);

  AssertTemplateResult("\\\\{print a}{print a}}".split(''), "{\"a\": 3}".split(''), "\\{print a}3}".split(''), failures);

  AssertTemplateResult("\\\\{print a}{print a}\\\\{print a}}".split(''), "{\"a\": 3}".split(''), "\\{print a}3\\{print a}}".split(''), failures);

  AssertTemplateResult("\\{print a}{print a}\\{".split(''), "{\"a\": 3}".split(''), "{print a}3\\{".split(''), failures);

  AssertTemplateResult("    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>".split(''), "{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}".split(''), "    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>".split(''), failures);

  AssertTemplateError("{print".split(''), "{}".split(''), "Template command found, but not ended properly.".split(''), failures);

  AssertTemplateError("{print a}".split(''), "{}".split(''), "Key for printing not found in JSON object: a".split(''), failures);

  AssertTemplateError("This is a template, this is a value: {print a {print a}.".split(''), "{\"a\": 5}".split(''), "The print command takes one parameter.".split(''), failures);

  AssertTemplateError("This is a {use a}\\{print a}template, this is a value: {print a}.{end}".split(''), "{\"a\": 5}".split(''), "Data structure for print command is not a JSON object.".split(''), failures);

  AssertTemplateError("{use a}".split(''), "{}".split(''), "End block expected at the end of use block.".split(''), failures);

  AssertTemplateError("{if a}".split(''), "{\"a\": true}".split(''), "End or else block expected at the end of if block.".split(''), failures);

  AssertTemplateError("{if a}{else}".split(''), "{\"a\": true}".split(''), "End block expected at the end of else block.".split(''), failures);

  AssertTemplateError("{foreach x in a}".split(''), "{\"a\": [1, 2, 3, 4]}".split(''), "End block expected at the end of foreach block.".split(''), failures);
}
function AssertTemplateResult(template, json, result, failures){
  var data;
  var errorMessages;
  var document;
  var errorMessage;
  var success;

  data = {};
  errorMessages = {};
  document = {};
  errorMessage = {};

  success = ReadJSON(json, data, errorMessages);

  AssertTrue(success, failures);

  if(success){
    success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage);

    AssertTrue(success, failures);

    if(success){
      AssertStringEquals(document.string, result, failures);
    }
  }
}
function AssertTemplateError(template, json, errorMessage, failures){
  var data;
  var errorMessages;
  var document;
  var errorMessageRef;
  var success;

  data = {};
  errorMessages = {};
  document = {};
  errorMessageRef = {};

  success = ReadJSON(json, data, errorMessages);

  AssertTrue(success, failures);

  if(success){
    success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessageRef);

    AssertFalse(success, failures);

    if( !success ){
      AssertStringEquals(errorMessage, errorMessageRef.string, failures);
    }
  }
}
function CreateBooleanReference(value){
  var ref;

  ref = {};
  ref.booleanValue = value;

  return ref;
}
function CreateBooleanArrayReference(value){
  var ref;

  ref = {};
  ref.booleanArray = value;

  return ref;
}
function CreateBooleanArrayReferenceLengthValue(lengthx, value){
  var ref;
  var i;

  ref = {};
  ref.booleanArray = [];
  ref.booleanArray.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    ref.booleanArray[i] = value;
  }

  return ref;
}
function FreeBooleanArrayReference(booleanArrayReference){
  delete(booleanArrayReference.booleanArray);
  delete(booleanArrayReference);
}
function CreateCharacterReference(value){
  var ref;

  ref = {};
  ref.characterValue = value;

  return ref;
}
function CreateNumberReference(value){
  var ref;

  ref = {};
  ref.numberValue = value;

  return ref;
}
function CreateNumberArrayReference(value){
  var ref;

  ref = {};
  ref.numberArray = value;

  return ref;
}
function CreateNumberArrayReferenceLengthValue(lengthx, value){
  var ref;
  var i;

  ref = {};
  ref.numberArray = [];
  ref.numberArray.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    ref.numberArray[i] = value;
  }

  return ref;
}
function FreeNumberArrayReference(numberArrayReference){
  delete(numberArrayReference.numberArray);
  delete(numberArrayReference);
}
function CreateStringReference(value){
  var ref;

  ref = {};
  ref.string = value;

  return ref;
}
function CreateStringReferenceLengthValue(lengthx, value){
  var ref;
  var i;

  ref = {};
  ref.string = [];
  ref.string.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    ref.string[i] = value;
  }

  return ref;
}
function FreeStringReference(stringReference){
  delete(stringReference.string);
  delete(stringReference);
}
function CreateStringArrayReference(strings){
  var ref;

  ref = {};
  ref.stringArray = strings;

  return ref;
}
function CreateStringArrayReferenceLengthValue(lengthx, value){
  var ref;
  var i;

  ref = {};
  ref.stringArray = [];
  ref.stringArray.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    ref.stringArray[i] = CreateStringReference(value);
  }

  return ref;
}
function FreeStringArrayReference(stringArrayReference){
  var i;

  for(i = 0; i < stringArrayReference.stringArray.length; i = i + 1){
    delete(stringArrayReference.stringArray[i]);
  }
  delete(stringArrayReference.stringArray);
  delete(stringArrayReference);
}
function IsValidJSON(json, errorMessage){
  var success;
  var elementReference;

  elementReference = {};

  success = ReadJSON(json, elementReference, errorMessage);

  if(success){
    DeleteElement(elementReference.element);
  }

  return success;
}
function JSONTokenize(json, tokensReference, errorMessages){
  var i;
  var c;
  var str;
  var stringReference, tokenReference;
  var stringLength;
  var success;
  var ll;

  ll = CreateLinkedListString();
  success = true;

  stringLength = {};
  tokenReference = {};

  for(i = 0; i < json.length && success; ){
    c = json[i];

    if(c == '{'){
      LinkedListAddString(ll, "{".split(''));
      i = i + 1;
    }else if(c == '}'){
      LinkedListAddString(ll, "}".split(''));
      i = i + 1;
    }else if(c == '['){
      LinkedListAddString(ll, "[".split(''));
      i = i + 1;
    }else if(c == ']'){
      LinkedListAddString(ll, "]".split(''));
      i = i + 1;
    }else if(c == ':'){
      LinkedListAddString(ll, ":".split(''));
      i = i + 1;
    }else if(c == ','){
      LinkedListAddString(ll, ",".split(''));
      i = i + 1;
    }else if(c == 'f'){
      success = GetJSONPrimitiveName(json, i, errorMessages, "false".split(''), tokenReference);
      if(success){
        LinkedListAddString(ll, "false".split(''));
        i = i + "false".split('').length;
      }
    }else if(c == 't'){
      success = GetJSONPrimitiveName(json, i, errorMessages, "true".split(''), tokenReference);
      if(success){
        LinkedListAddString(ll, "true".split(''));
        i = i + "true".split('').length;
      }
    }else if(c == 'n'){
      success = GetJSONPrimitiveName(json, i, errorMessages, "null".split(''), tokenReference);
      if(success){
        LinkedListAddString(ll, "null".split(''));
        i = i + "null".split('').length;
      }
    }else if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
      /* Skip. */
      i = i + 1;
    }else if(c == '\"'){
      success = GetJSONString(json, i, tokenReference, stringLength, errorMessages);
      if(success){
        LinkedListAddString(ll, tokenReference.string);
        i = i + stringLength.numberValue;
      }
    }else if(IsJSONNumberCharacter(c)){
      success = GetJSONNumberToken(json, i, tokenReference, errorMessages);
      if(success){
        LinkedListAddString(ll, tokenReference.string);
        i = i + tokenReference.string.length;
      }
    }else{
      str = ConcatenateCharacter("Invalid start of Token: ".split(''), c);
      stringReference = CreateStringReference(str);
      AddStringRef(errorMessages, stringReference);
      i = i + 1;
      success = false;
    }
  }

  if(success){
    LinkedListAddString(ll, "<end>".split(''));
    tokensReference.stringArray = LinkedListStringsToArray(ll);
    FreeLinkedListString(ll);
  }

  return success;
}
function GetJSONNumberToken(json, start, tokenReference, errorMessages){
  var c;
  var end, i;
  var done, success;
  var numberString;

  end = json.length;
  done = false;

  for(i = start; i < json.length &&  !done ; i = i + 1){
    c = json[i];
    if( !IsJSONNumberCharacter(c) ){
      done = true;
      end = i;
    }
  }

  numberString = Substring(json, start, end);

  success = IsValidJSONNumber(numberString, errorMessages);

  tokenReference.string = numberString;

  return success;
}
function IsValidJSONNumber(n, errorMessages){
  var success;
  var i;

  i = 0;

  /* JSON allows an optional negative sign. */
  if(n[i] == '-'){
    i = i + 1;
  }

  if(i < n.length){
    success = IsValidJSONNumberAfterSign(n, i, errorMessages);
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference("Number must contain at least one digit.".split('')));
  }

  return success;
}
function IsValidJSONNumberAfterSign(n, i, errorMessages){
  var success;

  if(charIsNumber(n[i])){
    /* 0 first means only 0. */
    if(n[i] == '0'){
      i = i + 1;
    }else{
      /* 1-9 first, read following digits. */
      i = IsValidJSONNumberAdvancePastDigits(n, i);
    }

    if(i < n.length){
      success = IsValidJSONNumberFromDotOrExponent(n, i, errorMessages);
    }else{
      /* If integer, we are done now. */
      success = true;
    }
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference("A number must start with 0-9 (after the optional sign).".split('')));
  }

  return success;
}
function IsValidJSONNumberAdvancePastDigits(n, i){
  var done;

  i = i + 1;
  done = false;
  for(; i < n.length &&  !done ; ){
    if(charIsNumber(n[i])){
      i = i + 1;
    }else{
      done = true;
    }
  }

  return i;
}
function IsValidJSONNumberFromDotOrExponent(n, i, errorMessages){
  var wasDotAndOrE, success;

  wasDotAndOrE = false;
  success = true;

  if(n[i] == '.'){
    i = i + 1;
    wasDotAndOrE = true;

    if(i < n.length){
      if(charIsNumber(n[i])){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, i);

        if(i == n.length){
          /* If non-scientific decimal number, we are done. */
          success = true;
        }
      }else{
        success = false;
        AddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".split('')));
      }
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".split('')));
    }
  }

  if(i < n.length && success){
    if(n[i] == 'e' || n[i] == 'E'){
      wasDotAndOrE = true;
      success = IsValidJSONNumberFromExponent(n, i, errorMessages);
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference("Expected e or E.".split('')));
    }
  }else if(i == n.length && success){
    /* If number with decimal point. */
    success = true;
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".split('')));
  }

  if(wasDotAndOrE){
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference("Exprected decimal point or e or E.".split('')));
  }

  return success;
}
function IsValidJSONNumberFromExponent(n, i, errorMessages){
  var success;

  i = i + 1;

  if(i < n.length){
    /* The exponent sign can either + or -, */
    if(n[i] == '+' || n[i] == '-'){
      i = i + 1;
    }

    if(i < n.length){
      if(charIsNumber(n[i])){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, i);

        if(i == n.length){
          /* We found scientific number. */
          success = true;
        }else{
          success = false;
          AddStringRef(errorMessages, CreateStringReference("There was characters following the exponent.".split('')));
        }
      }else{
        success = false;
        AddStringRef(errorMessages, CreateStringReference("There must be a digit following the optional exponent sign.".split('')));
      }
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference("There must be a digit following optional the exponent sign.".split('')));
    }
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference("There must be a sign or a digit following e or E.".split('')));
  }

  return success;
}
function IsJSONNumberCharacter(c){
  var numericCharacters;
  var found;
  var i;

  numericCharacters = "0123456789.-+eE".split('');

  found = false;

  for(i = 0; i < numericCharacters.length; i = i + 1){
    if(numericCharacters[i] == c){
      found = true;
    }
  }

  return found;
}
function GetJSONPrimitiveName(string, start, errorMessages, primitive, tokenReference){
  var c, p;
  var done, success;
  var i;
  var str, token;

  done = false;
  success = true;

  token = "".split('');

  for(i = start; i < string.length && ((i - start) < primitive.length) &&  !done ; i = i + 1){
    c = string[i];
    p = primitive[i - start];
    if(c == p){
      /* OK */
      if((i + 1 - start) == primitive.length){
        done = true;
      }
    }else{
      str = "".split('');
      str = ConcatenateString(str, "Primitive invalid: ".split(''));
      str = AppendCharacter(str, c);
      str = AppendString(str, " vs ".split(''));
      str = AppendCharacter(str, p);

      AddStringRef(errorMessages, CreateStringReference(str));
      done = true;
      success = false;
    }
  }

  if(done){
    if(StringsEqual(primitive, "false".split(''))){
      token = "false".split('');
    }
    if(StringsEqual(primitive, "true".split(''))){
      token = "true".split('');
    }
    if(StringsEqual(primitive, "null".split(''))){
      token = "null".split('');
    }
  }else{
    AddStringRef(errorMessages, CreateStringReference("Primitive invalid".split('')));
    success = false;
  }

  tokenReference.string = token;

  return success;
}
function GetJSONString(json, start, tokenReference, stringLengthReference, errorMessages){
  var success, done;
  var string, hex;
  var characterCount, hexReference;
  var i, l, c;
  var errorMessage;

  characterCount = CreateNumberReference(0);
  hex = CreateString(4, '0');
  hexReference = {};
  errorMessage = {};

  success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages);

  if(success){
    l = characterCount.numberValue;
    string = [];
    string.length = l;

    c = 0;
    string[c] = '\"';
    c = c + 1;

    done = false;
    for(i = start + 1;  !done ; i = i + 1){
      if(json[i] == '\\'){
        i = i + 1;
        if(json[i] == '\"' || json[i] == '\\' || json[i] == '/'){
          string[c] = json[i];
          c = c + 1;
        }else if(json[i] == 'b'){
          string[c] = String.fromCharCode(8);
          c = c + 1;
        }else if(json[i] == 'f'){
          string[c] = String.fromCharCode(12);
          c = c + 1;
        }else if(json[i] == 'n'){
          string[c] = String.fromCharCode(10);
          c = c + 1;
        }else if(json[i] == 'r'){
          string[c] = String.fromCharCode(13);
          c = c + 1;
        }else if(json[i] == 't'){
          string[c] = String.fromCharCode(9);
          c = c + 1;
        }else if(json[i] == 'u'){
          i = i + 1;
          hex[0] = charToUpperCase(json[i + 0]);
          hex[1] = charToUpperCase(json[i + 1]);
          hex[2] = charToUpperCase(json[i + 2]);
          hex[3] = charToUpperCase(json[i + 3]);
          nCreateNumberFromStringWithCheck(hex, 16, hexReference, errorMessage);
          string[c] = String.fromCharCode(hexReference.numberValue);
          i = i + 3;
          c = c + 1;
        }
      }else if(json[i] == '\"'){
        string[c] = json[i];
        c = c + 1;
        done = true;
      }else{
        string[c] = json[i];
        c = c + 1;
      }
    }

    tokenReference.string = string;
    success = true;
  }else{
    AddStringRef(errorMessages, CreateStringReference("End of string was not found.".split('')));
    success = false;
  }

  return success;
}
function IsValidJSONString(jsonString, errorMessages){
  var valid;
  var numberReference, stringLength;

  numberReference = {};
  stringLength = {};

  valid = IsValidJSONStringInJSON(jsonString, 0, numberReference, stringLength, errorMessages);

  return valid;
}
function IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages){
  var success, done;
  var i, j;
  var c;

  success = true;
  done = false;

  characterCount.numberValue = 1;

  for(i = start + 1; i < json.length &&  !done  && success; i = i + 1){
    if( !IsJSONIllegalControllCharacter(json[i]) ){
      if(json[i] == '\\'){
        i = i + 1;
        if(i < json.length){
          if(json[i] == '\"' || json[i] == '\\' || json[i] == '/' || json[i] == 'b' || json[i] == 'f' || json[i] == 'n' || json[i] == 'r' || json[i] == 't'){
            characterCount.numberValue = characterCount.numberValue + 1;
          }else if(json[i] == 'u'){
            if(i + 4 < json.length){
              for(j = 0; j < 4 && success; j = j + 1){
                c = json[i + j + 1];
                if(nCharacterIsNumberCharacterInBase(c, 16) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f'){
                }else{
                  success = false;
                  AddStringRef(errorMessages, CreateStringReference("\\u must be followed by four hexadecimal digits.".split('')));
                }
              }
              characterCount.numberValue = characterCount.numberValue + 1;
              i = i + 4;
            }else{
              success = false;
              AddStringRef(errorMessages, CreateStringReference("\\u must be followed by four characters.".split('')));
            }
          }else{
            success = false;
            AddStringRef(errorMessages, CreateStringReference("Escaped character invalid.".split('')));
          }
        }else{
          success = false;
          AddStringRef(errorMessages, CreateStringReference("There must be at least two character after string escape.".split('')));
        }
      }else if(json[i] == '\"'){
        characterCount.numberValue = characterCount.numberValue + 1;
        done = true;
      }else{
        characterCount.numberValue = characterCount.numberValue + 1;
      }
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference("Unicode code points 0-31 not allowed in JSON string.".split('')));
    }
  }

  if(done){
    stringLengthReference.numberValue = i - start;
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference("String must end with \".".split('')));
  }

  return success;
}
function IsJSONIllegalControllCharacter(c){
  var cnr;
  var isControll;

  cnr = c.charCodeAt(0);

  if(cnr >= 0 && cnr < 32){
    isControll = true;
  }else{
    isControll = false;
  }

  return isControll;
}
function AddElement(list, a){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length + 1;

  for(i = 0; i < list.length; i = i + 1){
    newlist[i] = list[i];
  }
  newlist[list.length] = a;

  delete(list);

  return newlist;
}
function AddElementRef(list, i){
  list.array = AddElement(list.array, i);
}
function RemoveElement(list, n){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length - 1;

  for(i = 0; i < list.length; i = i + 1){
    if(i < n){
      newlist[i] = list[i];
    }
    if(i > n){
      newlist[i - 1] = list[i];
    }
  }

  delete(list);

  return newlist;
}
function GetElementRef(list, i){
  return list.array[i];
}
function RemoveElementRef(list, i){
  list.array = RemoveElement(list.array, i);
}
function CreateLinkedListElements(){
  var ll;

  ll = {};
  ll.first = {};
  ll.last = ll.first;
  ll.last.end = true;

  return ll;
}
function LinkedListAddElement(ll, value){
  ll.last.end = false;
  ll.last.value = value;
  ll.last.next = {};
  ll.last.next.end = true;
  ll.last = ll.last.next;
}
function LinkedListElementsToArray(ll){
  var array;
  var lengthx, i;
  var node;

  node = ll.first;

  lengthx = LinkedListElementsLength(ll);

  array = [];
  array.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    array[i] = node.value;
    node = node.next;
  }

  return array;
}
function LinkedListElementsLength(ll){
  var l;
  var node;

  l = 0;
  node = ll.first;
  for(;  !node.end ; ){
    node = node.next;
    l = l + 1;
  }

  return l;
}
function FreeLinkedListElements(ll){
  var node, prev;

  node = ll.first;

  for(;  !node.end ; ){
    prev = node;
    node = node.next;
    delete(prev);
  }

  delete(node);
}
function ComputeJSONStringLength(element){
  var result;

  result = 0;

  if(StringsEqual(element.type, "object".split(''))){
    result = result + ComputeJSONObjectStringLength(element);
  }else if(StringsEqual(element.type, "string".split(''))){
    result = JSONEscapedStringLength(element.string) + 2;
  }else if(StringsEqual(element.type, "array".split(''))){
    result = result + ComputeJSONArrayStringLength(element);
  }else if(StringsEqual(element.type, "number".split(''))){
    result = result + ComputeJSONNumberStringLength(element);
  }else if(StringsEqual(element.type, "null".split(''))){
    result = result + "null".split('').length;
  }else if(StringsEqual(element.type, "boolean".split(''))){
    result = result + ComputeJSONBooleanStringLength(element);
  }

  return result;
}
function ComputeJSONBooleanStringLength(element){
  var result;

  if(element.booleanValue){
    result = "true".split('').length;
  }else{
    result = "false".split('').length;
  }

  return result;
}
function ComputeJSONNumberStringLength(element){
  var lengthx;
  var a;

  if(element.number != 0){
    if(Math.abs(element.number) >= Math.pow(10, 15) || Math.abs(element.number) <= Math.pow(10,  -15)){
      a = nCreateStringScientificNotationDecimalFromNumber(element.number);
      lengthx = a.length;
    }else{
      a = nCreateStringDecimalFromNumber(element.number);
      lengthx = a.length;
    }
  }else{
    lengthx = 1;
  }

  return lengthx;
}
function ComputeJSONArrayStringLength(element){
  var arrayElement;
  var i;
  var lengthx;

  lengthx = 1;

  for(i = 0; i < element.array.length; i = i + 1){
    arrayElement = element.array[i];

    lengthx = lengthx + ComputeJSONStringLength(arrayElement);

    if(i + 1 != element.array.length){
      lengthx = lengthx + 1;
    }
  }

  lengthx = lengthx + 1;

  return lengthx;
}
function ComputeJSONObjectStringLength(element){
  var key;
  var i;
  var keys;
  var objectElement;
  var lengthx;

  lengthx = 1;

  keys = GetStringElementMapKeySet(element.object);
  for(i = 0; i < keys.stringArray.length; i = i + 1){
    key = keys.stringArray[i].string;
    objectElement = GetObjectValue(element.object, key);

    lengthx = lengthx + 1;
    lengthx = lengthx + JSONEscapedStringLength(key);
    lengthx = lengthx + 1;
    lengthx = lengthx + 1;

    lengthx = lengthx + ComputeJSONStringLength(objectElement);

    if(i + 1 != keys.stringArray.length){
      lengthx = lengthx + 1;
    }
  }

  lengthx = lengthx + 1;

  return lengthx;
}
function CreateStringElement(string){
  var element;
  element = {};
  element.type = "string".split('');
  element.string = string;
  return element;
}
function CreateBooleanElement(booleanValue){
  var element;
  element = {};
  element.type = "boolean".split('');
  element.booleanValue = booleanValue;
  return element;
}
function CreateNullElement(){
  var element;
  element = {};
  element.type = "null".split('');
  return element;
}
function CreateNumberElement(number){
  var element;
  element = {};
  element.type = "number".split('');
  element.number = number;
  return element;
}
function CreateArrayElement(lengthx){
  var element;
  element = {};
  element.type = "array".split('');
  element.array = [];
  element.array.length = lengthx;
  return element;
}
function CreateObjectElement(lengthx){
  var element;
  element = {};
  element.type = "object".split('');
  element.object = {};
  element.object.stringListRef = CreateStringArrayReferenceLengthValue(lengthx, "".split(''));
  element.object.elementListRef = {};
  element.object.elementListRef.array = [];
  element.object.elementListRef.array.length = lengthx;
  return element;
}
function DeleteElement(element){
  if(StringsEqual(element.type, "object".split(''))){
    DeleteObject(element);
  }else if(StringsEqual(element.type, "string".split(''))){
    delete(element);
  }else if(StringsEqual(element.type, "array".split(''))){
    DeleteArray(element);
  }else if(StringsEqual(element.type, "number".split(''))){
    delete(element);
  }else if(StringsEqual(element.type, "null".split(''))){
    delete(element);
  }else if(StringsEqual(element.type, "boolean".split(''))){
    delete(element);
  }else{
  }
}
function DeleteObject(element){
  var keys;
  var i;
  var key;
  var objectElement;

  keys = GetStringElementMapKeySet(element.object);
  for(i = 0; i < keys.stringArray.length; i = i + 1){
    key = keys.stringArray[i].string;
    objectElement = GetObjectValue(element.object, key);
    DeleteElement(objectElement);
  }
}
function DeleteArray(element){
  var i;
  var arrayElement;

  for(i = 0; i < element.array.length; i = i + 1){
    arrayElement = element.array[i];
    DeleteElement(arrayElement);
  }
}
function WriteJSON(element){
  var result;
  var lengthx;
  var index;

  lengthx = ComputeJSONStringLength(element);
  result = [];
  result.length = lengthx;
  index = CreateNumberReference(0);

  if(StringsEqual(element.type, "object".split(''))){
    WriteObject(element, result, index);
  }else if(StringsEqual(element.type, "string".split(''))){
    WriteString(element, result, index);
  }else if(StringsEqual(element.type, "array".split(''))){
    WriteArray(element, result, index);
  }else if(StringsEqual(element.type, "number".split(''))){
    WriteNumber(element, result, index);
  }else if(StringsEqual(element.type, "null".split(''))){
    WriteStringToStingStream(result, index, "null".split(''));
  }else if(StringsEqual(element.type, "boolean".split(''))){
    WriteBooleanValue(element, result, index);
  }

  return result;
}
function WriteBooleanValue(element, result, index){
  if(element.booleanValue){
    WriteStringToStingStream(result, index, "true".split(''));
  }else{
    WriteStringToStingStream(result, index, "false".split(''));
  }
}
function WriteNumber(element, result, index){
  var numberString;

  if(element.number != 0){
    if(Math.abs(element.number) >= Math.pow(10, 15) || Math.abs(element.number) <= Math.pow(10,  -15)){
      numberString = nCreateStringScientificNotationDecimalFromNumber(element.number);
    }else{
      numberString = nCreateStringDecimalFromNumber(element.number);
    }
  }else{
    numberString = nCreateStringDecimalFromNumber(element.number);
  }

  WriteStringToStingStream(result, index, numberString);
}
function WriteArray(element, result, index){
  var s;
  var arrayElement;
  var i;

  WriteStringToStingStream(result, index, "[".split(''));

  for(i = 0; i < element.array.length; i = i + 1){
    arrayElement = element.array[i];

    s = WriteJSON(arrayElement);
    WriteStringToStingStream(result, index, s);

    if(i + 1 != element.array.length){
      WriteStringToStingStream(result, index, ",".split(''));
    }
  }

  WriteStringToStingStream(result, index, "]".split(''));
}
function WriteString(element, result, index){
  WriteStringToStingStream(result, index, "\"".split(''));
  element.string = JSONEscapeString(element.string);
  WriteStringToStingStream(result, index, element.string);
  WriteStringToStingStream(result, index, "\"".split(''));
}
function JSONEscapeString(string){
  var i, lengthx;
  var index, lettersReference;
  var ns, escaped;

  lengthx = JSONEscapedStringLength(string);

  ns = [];
  ns.length = lengthx;
  index = CreateNumberReference(0);
  lettersReference = CreateNumberReference(0);

  for(i = 0; i < string.length; i = i + 1){
    if(JSONMustBeEscaped(string[i], lettersReference)){
      escaped = JSONEscapeCharacter(string[i]);
      WriteStringToStingStream(ns, index, escaped);
    }else{
      WriteCharacterToStingStream(ns, index, string[i]);
    }
  }

  return ns;
}
function JSONEscapedStringLength(string){
  var lettersReference;
  var i, lengthx;

  lettersReference = CreateNumberReference(0);
  lengthx = 0;

  for(i = 0; i < string.length; i = i + 1){
    if(JSONMustBeEscaped(string[i], lettersReference)){
      lengthx = lengthx + lettersReference.numberValue;
    }else{
      lengthx = lengthx + 1;
    }
  }
  return lengthx;
}
function JSONEscapeCharacter(c){
  var code;
  var escaped;
  var hexNumber;
  var q, rs, s, b, f, n, r, t;

  code = c.charCodeAt(0);

  q = 34;
  rs = 92;
  s = 47;
  b = 8;
  f = 12;
  n = 10;
  r = 13;
  t = 9;

  hexNumber = {};

  if(code == q){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = '\"';
  }else if(code == rs){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = '\\';
  }else if(code == s){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = '/';
  }else if(code == b){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = 'b';
  }else if(code == f){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = 'f';
  }else if(code == n){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = 'n';
  }else if(code == r){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = 'r';
  }else if(code == t){
    escaped = [];
    escaped.length = 2;
    escaped[0] = '\\';
    escaped[1] = 't';
  }else if(code >= 0 && code <= 31){
    escaped = [];
    escaped.length = 6;
    escaped[0] = '\\';
    escaped[1] = 'u';
    escaped[2] = '0';
    escaped[3] = '0';

    nCreateStringFromNumberWithCheck(code, 16, hexNumber);

    if(hexNumber.string.length == 1){
      escaped[4] = '0';
      escaped[5] = hexNumber.string[0];
    }else if(hexNumber.string.length == 2){
      escaped[4] = hexNumber.string[0];
      escaped[5] = hexNumber.string[1];
    }
  }else{
    escaped = [];
    escaped.length = 1;
    escaped[0] = c;
  }

  return escaped;
}
function JSONMustBeEscaped(c, letters){
  var code;
  var mustBeEscaped;
  var q, rs, s, b, f, n, r, t;

  code = c.charCodeAt(0);
  mustBeEscaped = false;

  q = 34;
  rs = 92;
  s = 47;
  b = 8;
  f = 12;
  n = 10;
  r = 13;
  t = 9;

  if(code == q || code == rs || code == s || code == b || code == f || code == n || code == r || code == t){
    mustBeEscaped = true;
    letters.numberValue = 2;
  }else if(code >= 0 && code <= 31){
    mustBeEscaped = true;
    letters.numberValue = 6;
  }else if(code >= Math.pow(2, 16)){
    mustBeEscaped = true;
    letters.numberValue = 6;
  }

  return mustBeEscaped;
}
function WriteObject(element, result, index){
  var s, key;
  var i;
  var keys;
  var objectElement;

  WriteStringToStingStream(result, index, "{".split(''));

  keys = GetStringElementMapKeySet(element.object);
  for(i = 0; i < keys.stringArray.length; i = i + 1){
    key = keys.stringArray[i].string;
    key = JSONEscapeString(key);
    objectElement = GetObjectValue(element.object, key);

    WriteStringToStingStream(result, index, "\"".split(''));
    WriteStringToStingStream(result, index, key);
    WriteStringToStingStream(result, index, "\"".split(''));
    WriteStringToStingStream(result, index, ":".split(''));

    s = WriteJSON(objectElement);
    WriteStringToStingStream(result, index, s);

    if(i + 1 != keys.stringArray.length){
      WriteStringToStingStream(result, index, ",".split(''));
    }
  }

  WriteStringToStingStream(result, index, "}".split(''));
}
function ReadJSON(string, elementReference, errorMessages){
  var tokenArrayReference;
  var success;

  /* Tokenize. */
  tokenArrayReference = {};
  success = JSONTokenize(string, tokenArrayReference, errorMessages);

  if(success){
    /* Parse. */
    success = GetJSONValue(tokenArrayReference.stringArray, elementReference, errorMessages);
  }

  return success;
}
function GetJSONValue(tokens, elementReference, errorMessages){
  var i;
  var success;

  i = CreateNumberReference(0);
  success = GetJSONValueRecursive(tokens, i, 0, elementReference, errorMessages);

  return success;
}
function GetJSONValueRecursive(tokens, i, depth, elementReference, errorMessages){
  var str, substr, token;
  var stringToDecimalResult;
  var success;

  success = true;
  token = tokens[i.numberValue].string;

  if(StringsEqual(token, "{".split(''))){
    success = GetJSONObject(tokens, i, depth + 1, elementReference, errorMessages);
  }else if(StringsEqual(token, "[".split(''))){
    success = GetJSONArray(tokens, i, depth + 1, elementReference, errorMessages);
  }else if(StringsEqual(token, "true".split(''))){
    elementReference.element = CreateBooleanElement(true);
    i.numberValue = i.numberValue + 1;
  }else if(StringsEqual(token, "false".split(''))){
    elementReference.element = CreateBooleanElement(false);
    i.numberValue = i.numberValue + 1;
  }else if(StringsEqual(token, "null".split(''))){
    elementReference.element = CreateNullElement();
    i.numberValue = i.numberValue + 1;
  }else if(charIsNumber(token[0]) || token[0] == '-'){
    stringToDecimalResult = nCreateNumberFromDecimalString(token);
    elementReference.element = CreateNumberElement(stringToDecimalResult);
    i.numberValue = i.numberValue + 1;
  }else if(token[0] == '\"'){
    substr = Substring(token, 1, token.length - 1);
    elementReference.element = CreateStringElement(substr);
    i.numberValue = i.numberValue + 1;
  }else{
    str = "".split('');
    str = ConcatenateString(str, "Invalid token first in value: ".split(''));
    str = AppendString(str, token);
    AddStringRef(errorMessages, CreateStringReference(str));
    success = false;
  }

  if(success && depth == 0){
    if(StringsEqual(tokens[i.numberValue].string, "<end>".split(''))){
    }else{
      AddStringRef(errorMessages, CreateStringReference("The outer value cannot have any tokens following it.".split('')));
      success = false;
    }
  }

  return success;
}
function GetJSONObject(tokens, i, depth, elementReference, errorMessages){
  var element, value;
  var done, success;
  var key, colon, comma, closeCurly;
  var keystring, str;
  var valueReference;
  var values;
  var keys;

  keys = CreateLinkedListString();
  values = CreateLinkedListElements();
  element = CreateObjectElement(0);
  valueReference = {};
  success = true;
  i.numberValue = i.numberValue + 1;

  if( !StringsEqual(tokens[i.numberValue].string, "}".split('')) ){
    done = false;

    for(;  !done  && success; ){
      key = tokens[i.numberValue].string;

      if(key[0] == '\"'){
        i.numberValue = i.numberValue + 1;
        colon = tokens[i.numberValue].string;
        if(StringsEqual(colon, ":".split(''))){
          i.numberValue = i.numberValue + 1;
          success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

          if(success){
            keystring = Substring(key, 1, key.length - 1);
            value = valueReference.element;
            LinkedListAddString(keys, keystring);
            LinkedListAddElement(values, value);

            comma = tokens[i.numberValue].string;
            if(StringsEqual(comma, ",".split(''))){
              /* OK */
              i.numberValue = i.numberValue + 1;
            }else{
              done = true;
            }
          }
        }else{
          str = "".split('');
          str = ConcatenateString(str, "Expected colon after key in object: ".split(''));
          str = AppendString(str, colon);
          AddStringRef(errorMessages, CreateStringReference(str));

          success = false;
          done = true;
        }
      }else{
        AddStringRef(errorMessages, CreateStringReference("Expected string as key in object.".split('')));

        done = true;
        success = false;
      }
    }
  }

  if(success){
    closeCurly = tokens[i.numberValue].string;

    if(StringsEqual(closeCurly, "}".split(''))){
      /* OK */
      delete(element.object.stringListRef.stringArray);
      delete(element.object.elementListRef.array);
      element.object.stringListRef.stringArray = LinkedListStringsToArray(keys);
      element.object.elementListRef.array = LinkedListElementsToArray(values);
      elementReference.element = element;
      i.numberValue = i.numberValue + 1;
    }else{
      AddStringRef(errorMessages, CreateStringReference("Expected close curly brackets at end of object value.".split('')));
      success = false;
    }
  }

  FreeLinkedListString(keys);
  FreeLinkedListElements(values);
  delete(valueReference);

  return success;
}
function GetJSONArray(tokens, i, depth, elementReference, errorMessages){
  var element, value;
  var nextToken, comma;
  var done, success;
  var valueReference;
  var elements;

  elements = CreateLinkedListElements();
  i.numberValue = i.numberValue + 1;

  valueReference = {};
  success = true;
  element = CreateArrayElement(0);

  nextToken = tokens[i.numberValue].string;

  if( !StringsEqual(nextToken, "]".split('')) ){
    done = false;
    for(;  !done  && success; ){
      success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

      if(success){
        value = valueReference.element;
        LinkedListAddElement(elements, value);

        comma = tokens[i.numberValue].string;

        if(StringsEqual(comma, ",".split(''))){
          /* OK */
          i.numberValue = i.numberValue + 1;
        }else{
          done = true;
        }
      }
    }
  }

  nextToken = tokens[i.numberValue].string;
  if(StringsEqual(nextToken, "]".split(''))){
    /* OK */
    i.numberValue = i.numberValue + 1;
    delete(element.array);
    element.array = LinkedListElementsToArray(elements);
  }else{
    AddStringRef(errorMessages, CreateStringReference("Expected close square bracket at end of array.".split('')));
    success = false;
  }

  elementReference.element = element;
  FreeLinkedListElements(elements);
  delete(valueReference);

  return success;
}
function GetStringElementMapKeySet(stringElementMap){
  return stringElementMap.stringListRef;
}
function GetObjectValue(stringElementMap, key){
  var result;
  var i;

  result = {};

  for(i = 0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1){
    if(StringsEqual(stringElementMap.stringListRef.stringArray[i].string, key)){
      result = stringElementMap.elementListRef.array[i];
    }
  }

  return result;
}
function GetObjectValueWithCheck(stringElementMap, key, foundReference){
  var result;
  var i;

  result = {};

  foundReference.booleanValue = false;
  for(i = 0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1){
    if(StringsEqual(stringElementMap.stringListRef.stringArray[i].string, key)){
      result = stringElementMap.elementListRef.array[i];
      foundReference.booleanValue = true;
    }
  }

  return result;
}
function PutStringElementMap(stringElementMap, keystring, value){
  AddStringRef(stringElementMap.stringListRef, CreateStringReference(keystring));
  AddElementRef(stringElementMap.elementListRef, value);
}
function SetStringElementMap(stringElementMap, index, keystring, value){
  stringElementMap.stringListRef.stringArray[index].string = keystring;
  stringElementMap.elementListRef.array[index] = value;
}
function GetStringElementMapNumberOfKeys(stringElementMap){
  return stringElementMap.stringListRef.stringArray.length;
}
function JSONCompare(a, b, epsilon, equal, errorMessage){
  var success;
  var eaRef, ebRef;
  var ea, eb;

  eaRef = {};
  ebRef = {};

  success = ReadJSON(a, eaRef, errorMessage);

  if(success){
    ea = eaRef.element;

    success = ReadJSON(b, ebRef, errorMessage);

    if(success){
      eb = ebRef.element;

      equal.booleanValue = JSONCompareElements(ea, eb, epsilon);

      DeleteElement(eb);
    }

    DeleteElement(ea);
  }

  return success;
}
function JSONCompareElements(ea, eb, epsilon){
  var equal;
  var typeName;

  equal = StringsEqual(ea.type, eb.type);
        
  if(equal){
    typeName = ea.type;
    if(StringsEqual(typeName, "object".split(''))){
      equal = JSONCompareObjects(ea.object, eb.object, epsilon);
    }else if(StringsEqual(typeName, "string".split(''))){
      equal = StringsEqual(ea.string, eb.string);
    }else if(StringsEqual(typeName, "array".split(''))){
      equal = JSONCompareArrays(ea.array, eb.array, epsilon);
    }else if(StringsEqual(typeName, "number".split(''))){
      equal = EpsilonCompare(ea.number, eb.number, epsilon);
    }else if(StringsEqual(typeName, "null".split(''))){
      equal = true;
    }else if(StringsEqual(typeName, "boolean".split(''))){
      equal = ea.booleanValue == eb.booleanValue;
    }
  }
        
  return equal;
}
function JSONCompareArrays(ea, eb, epsilon){
  var equals;
  var i, lengthx;

  equals = ea.length == eb.length;

  if(equals){
    lengthx = ea.length;
    for(i = 0; i < lengthx && equals; i = i + 1){
      equals = JSONCompareElements(ea[i], eb[i], epsilon);
    }
  }

  return equals;
}
function JSONCompareObjects(ea, eb, epsilon){
  var equals;
  var akeys, bkeys, i;
  var keys;
  var key;
  var aFoundReference, bFoundReference;
  var eaValue, ebValue;

  aFoundReference = {};
  bFoundReference = {};

  akeys = GetStringElementMapNumberOfKeys(ea);
  bkeys = GetStringElementMapNumberOfKeys(eb);

  equals = akeys == bkeys;

  if(equals){
    keys = GetStringElementMapKeySet(ea);

    for(i = 0; i < keys.stringArray.length && equals; i = i + 1){
      key = keys.stringArray[i].string;

      eaValue = GetObjectValueWithCheck(ea, key, aFoundReference);
      ebValue = GetObjectValueWithCheck(eb, key, bFoundReference);

      if(aFoundReference.booleanValue && bFoundReference.booleanValue){
        equals = JSONCompareElements(eaValue, ebValue, epsilon);
      }else{
        equals = false;
      }
    }
  }

  return equals;
}
function AddNumber(list, a){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length + 1;
  for(i = 0; i < list.length; i = i + 1){
    newlist[i] = list[i];
  }
  newlist[list.length] = a;
		
  delete(list);
		
  return newlist;
}
function AddNumberRef(list, i){
  list.numberArray = AddNumber(list.numberArray, i);
}
function RemoveNumber(list, n){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length - 1;

  if(n >= 0 && n < list.length){
    for(i = 0; i < list.length; i = i + 1){
      if(i < n){
        newlist[i] = list[i];
      }
      if(i > n){
        newlist[i - 1] = list[i];
      }
    }

    delete(list);
  }else{
    delete(newlist);
  }
		
  return newlist;
}
function GetNumberRef(list, i){
  return list.numberArray[i];
}
function RemoveNumberRef(list, i){
  list.numberArray = RemoveNumber(list.numberArray, i);
}
function AddString(list, a){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length + 1;

  for(i = 0; i < list.length; i = i + 1){
    newlist[i] = list[i];
  }
  newlist[list.length] = a;
		
  delete(list);
		
  return newlist;
}
function AddStringRef(list, i){
  list.stringArray = AddString(list.stringArray, i);
}
function RemoveString(list, n){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length - 1;

  if(n >= 0 && n < list.length){
    for(i = 0; i < list.length; i = i + 1){
      if(i < n){
        newlist[i] = list[i];
      }
      if(i > n){
        newlist[i - 1] = list[i];
      }
    }

    delete(list);
  }else{
    delete(newlist);
  }
		
  return newlist;
}
function GetStringRef(list, i){
  return list.stringArray[i];
}
function RemoveStringRef(list, i){
  list.stringArray = RemoveString(list.stringArray, i);
}
function CreateDynamicArrayCharacters(){
  var da;

  da = {};
  da.array = [];
  da.array.length = 10;
  da.lengthx = 0;

  return da;
}
function CreateDynamicArrayCharactersWithInitialCapacity(capacity){
  var da;

  da = {};
  da.array = [];
  da.array.length = capacity;
  da.lengthx = 0;

  return da;
}
function DynamicArrayAddCharacter(da, value){
  if(da.lengthx == da.array.length){
    DynamicArrayCharactersIncreaseSize(da);
  }

  da.array[da.lengthx] = value;
  da.lengthx = da.lengthx + 1;
}
function DynamicArrayCharactersIncreaseSize(da){
  var newLength, i;
  var newArray;

  newLength = Math.round(da.array.length*3/2);
  newArray = [];
  newArray.length = newLength;

  for(i = 0; i < da.array.length; i = i + 1){
    newArray[i] = da.array[i];
  }

  delete(da.array);

  da.array = newArray;
}
function DynamicArrayCharactersDecreaseSizeNecessary(da){
  var needsDecrease;

  needsDecrease = false;

  if(da.lengthx > 10){
    needsDecrease = da.lengthx <= Math.round(da.array.length*2/3);
  }

  return needsDecrease;
}
function DynamicArrayCharactersDecreaseSize(da){
  var newLength, i;
  var newArray;

  newLength = Math.round(da.array.length*2/3);
  newArray = [];
  newArray.length = newLength;

  for(i = 0; i < newLength; i = i + 1){
    newArray[i] = da.array[i];
  }

  delete(da.array);

  da.array = newArray;
}
function DynamicArrayCharactersIndex(da, index){
  return da.array[index];
}
function DynamicArrayCharactersLength(da){
  return da.lengthx;
}
function DynamicArrayInsertCharacter(da, index, value){
  var i;

  if(da.lengthx == da.array.length){
    DynamicArrayCharactersIncreaseSize(da);
  }

  for(i = da.lengthx; i > index; i = i - 1){
    da.array[i] = da.array[i - 1];
  }

  da.array[index] = value;

  da.lengthx = da.lengthx + 1;
}
function DynamicArrayCharacterSet(da, index, value){
  var success;

  if(index < da.lengthx){
    da.array[index] = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
function DynamicArrayRemoveCharacter(da, index){
  var i;

  for(i = index; i < da.lengthx - 1; i = i + 1){
    da.array[i] = da.array[i + 1];
  }

  da.lengthx = da.lengthx - 1;

  if(DynamicArrayCharactersDecreaseSizeNecessary(da)){
    DynamicArrayCharactersDecreaseSize(da);
  }
}
function FreeDynamicArrayCharacters(da){
  delete(da.array);
  delete(da);
}
function DynamicArrayCharactersToArray(da){
  var array;
  var i;

  array = [];
  array.length = da.lengthx;

  for(i = 0; i < da.lengthx; i = i + 1){
    array[i] = da.array[i];
  }

  return array;
}
function ArrayToDynamicArrayCharactersWithOptimalSize(array){
  var da;
  var i;
  var c, n, newCapacity;

  c = array.length;
  n = (Math.log(c) - 1)/Math.log(3/2);
  newCapacity = Math.floor(n) + 1;

  da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity);

  for(i = 0; i < array.length; i = i + 1){
    da.array[i] = array[i];
  }

  return da;
}
function ArrayToDynamicArrayCharacters(array){
  var da;

  da = {};
  da.array = CopyString(array);
  da.lengthx = array.length;

  return da;
}
function DynamicArrayCharactersEqual(a, b){
  var equal;
  var i;

  equal = true;
  if(a.lengthx == b.lengthx){
    for(i = 0; i < a.lengthx && equal; i = i + 1){
      if(a.array[i] != b.array[i]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function DynamicArrayCharactersToLinkedList(da){
  var ll;
  var i;

  ll = CreateLinkedListCharacter();

  for(i = 0; i < da.lengthx; i = i + 1){
    LinkedListAddCharacter(ll, da.array[i]);
  }

  return ll;
}
function LinkedListToDynamicArrayCharacters(ll){
  var da;
  var i;
  var node;

  node = ll.first;

  da = {};
  da.lengthx = LinkedListCharactersLength(ll);

  da.array = [];
  da.array.length = da.lengthx;

  for(i = 0; i < da.lengthx; i = i + 1){
    da.array[i] = node.value;
    node = node.next;
  }

  return da;
}
function AddBoolean(list, a){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length + 1;
  for(i = 0; i < list.length; i = i + 1){
    newlist[i] = list[i];
  }
  newlist[list.length] = a;
		
  delete(list);
		
  return newlist;
}
function AddBooleanRef(list, i){
  list.booleanArray = AddBoolean(list.booleanArray, i);
}
function RemoveBoolean(list, n){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length - 1;

  if(n >= 0 && n < list.length){
    for(i = 0; i < list.length; i = i + 1){
      if(i < n){
        newlist[i] = list[i];
      }
      if(i > n){
        newlist[i - 1] = list[i];
      }
    }

    delete(list);
  }else{
    delete(newlist);
  }
		
  return newlist;
}
function GetBooleanRef(list, i){
  return list.booleanArray[i];
}
function RemoveDecimalRef(list, i){
  list.booleanArray = RemoveBoolean(list.booleanArray, i);
}
function CreateLinkedListString(){
  var ll;

  ll = {};
  ll.first = {};
  ll.last = ll.first;
  ll.last.end = true;

  return ll;
}
function LinkedListAddString(ll, value){
  ll.last.end = false;
  ll.last.value = value;
  ll.last.next = {};
  ll.last.next.end = true;
  ll.last = ll.last.next;
}
function LinkedListStringsToArray(ll){
  var array;
  var lengthx, i;
  var node;

  node = ll.first;

  lengthx = LinkedListStringsLength(ll);

  array = [];
  array.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    array[i] = {};
    array[i].string = node.value;
    node = node.next;
  }

  return array;
}
function LinkedListStringsLength(ll){
  var l;
  var node;

  l = 0;
  node = ll.first;
  for(;  !node.end ; ){
    node = node.next;
    l = l + 1;
  }

  return l;
}
function FreeLinkedListString(ll){
  var node, prev;

  node = ll.first;

  for(;  !node.end ; ){
    prev = node;
    node = node.next;
    delete(prev);
  }

  delete(node);
}
function CreateLinkedListNumbers(){
  var ll;

  ll = {};
  ll.first = {};
  ll.last = ll.first;
  ll.last.end = true;

  return ll;
}
function CreateLinkedListNumbersArray(lengthx){
  var lls;
  var i;

  lls = [];
  lls.length = lengthx;
  for(i = 0; i < lls.length; i = i + 1){
    lls[i] = CreateLinkedListNumbers();
  }

  return lls;
}
function LinkedListAddNumber(ll, value){
  ll.last.end = false;
  ll.last.value = value;
  ll.last.next = {};
  ll.last.next.end = true;
  ll.last = ll.last.next;
}
function LinkedListNumbersLength(ll){
  var l;
  var node;

  l = 0;
  node = ll.first;
  for(;  !node.end ; ){
    node = node.next;
    l = l + 1;
  }

  return l;
}
function LinkedListNumbersIndex(ll, index){
  var i;
  var node;

  node = ll.first;
  for(i = 0; i < index; i = i + 1){
    node = node.next;
  }

  return node.value;
}
function LinkedListInsertNumber(ll, index, value){
  var i;
  var node, tmp;

  if(index == 0){
    tmp = ll.first;
    ll.first = {};
    ll.first.next = tmp;
    ll.first.value = value;
    ll.first.end = false;
  }else{
    node = ll.first;
    for(i = 0; i < index - 1; i = i + 1){
      node = node.next;
    }

    tmp = node.next;
    node.next = {};
    node.next.next = tmp;
    node.next.value = value;
    node.next.end = false;
  }
}
function LinkedListSet(ll, index, value){
  var i;
  var node;

  node = ll.first;
  for(i = 0; i < index; i = i + 1){
    node = node.next;
  }

  node.next.value = value;
}
function LinkedListRemoveNumber(ll, index){
  var i;
  var node, prev;

  node = ll.first;
  prev = ll.first;

  for(i = 0; i < index; i = i + 1){
    prev = node;
    node = node.next;
  }

  if(index == 0){
    ll.first = prev.next;
  }
  if( !prev.next.end ){
    prev.next = prev.next.next;
  }
}
function FreeLinkedListNumbers(ll){
  var node, prev;

  node = ll.first;

  for(;  !node.end ; ){
    prev = node;
    node = node.next;
    delete(prev);
  }

  delete(node);
}
function FreeLinkedListNumbersArray(lls){
  var i;

  for(i = 0; i < lls.length; i = i + 1){
    FreeLinkedListNumbers(lls[i]);
  }
  delete(lls);
}
function LinkedListNumbersToArray(ll){
  var array;
  var lengthx, i;
  var node;

  node = ll.first;

  lengthx = LinkedListNumbersLength(ll);

  array = [];
  array.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    array[i] = node.value;
    node = node.next;
  }

  return array;
}
function ArrayToLinkedListNumbers(array){
  var ll;
  var i;

  ll = CreateLinkedListNumbers();

  for(i = 0; i < array.length; i = i + 1){
    LinkedListAddNumber(ll, array[i]);
  }

  return ll;
}
function LinkedListNumbersEqual(a, b){
  var equal, done;
  var an, bn;

  an = a.first;
  bn = b.first;

  equal = true;
  done = false;
  for(; equal &&  !done ; ){
    if(an.end == bn.end){
      if(an.end){
        done = true;
      }else if(an.value == bn.value){
        an = an.next;
        bn = bn.next;
      }else{
        equal = false;
      }
    }else{
      equal = false;
    }
  }

  return equal;
}
function CreateLinkedListCharacter(){
  var ll;

  ll = {};
  ll.first = {};
  ll.last = ll.first;
  ll.last.end = true;

  return ll;
}
function LinkedListAddCharacter(ll, value){
  ll.last.end = false;
  ll.last.value = value;
  ll.last.next = {};
  ll.last.next.end = true;
  ll.last = ll.last.next;
}
function LinkedListCharactersToArray(ll){
  var array;
  var lengthx, i;
  var node;

  node = ll.first;

  lengthx = LinkedListCharactersLength(ll);

  array = [];
  array.length = lengthx;

  for(i = 0; i < lengthx; i = i + 1){
    array[i] = node.value;
    node = node.next;
  }

  return array;
}
function LinkedListCharactersLength(ll){
  var l;
  var node;

  l = 0;
  node = ll.first;
  for(;  !node.end ; ){
    node = node.next;
    l = l + 1;
  }

  return l;
}
function FreeLinkedListCharacter(ll){
  var node, prev;

  node = ll.first;

  for(;  !node.end ; ){
    prev = node;
    node = node.next;
    delete(prev);
  }

  delete(node);
}
function LinkedListCharactersAddString(ll, str){
  var i;

  for(i = 0; i < str.length; i = i + 1){
    LinkedListAddCharacter(ll, str[i]);
  }
}
function CreateDynamicArrayNumbers(){
  var da;

  da = {};
  da.array = [];
  da.array.length = 10;
  da.lengthx = 0;

  return da;
}
function CreateDynamicArrayNumbersWithInitialCapacity(capacity){
  var da;

  da = {};
  da.array = [];
  da.array.length = capacity;
  da.lengthx = 0;

  return da;
}
function DynamicArrayAddNumber(da, value){
  if(da.lengthx == da.array.length){
    DynamicArrayNumbersIncreaseSize(da);
  }

  da.array[da.lengthx] = value;
  da.lengthx = da.lengthx + 1;
}
function DynamicArrayNumbersIncreaseSize(da){
  var newLength, i;
  var newArray;

  newLength = Math.round(da.array.length*3/2);
  newArray = [];
  newArray.length = newLength;

  for(i = 0; i < da.array.length; i = i + 1){
    newArray[i] = da.array[i];
  }

  delete(da.array);

  da.array = newArray;
}
function DynamicArrayNumbersDecreaseSizeNecessary(da){
  var needsDecrease;

  needsDecrease = false;

  if(da.lengthx > 10){
    needsDecrease = da.lengthx <= Math.round(da.array.length*2/3);
  }

  return needsDecrease;
}
function DynamicArrayNumbersDecreaseSize(da){
  var newLength, i;
  var newArray;

  newLength = Math.round(da.array.length*2/3);
  newArray = [];
  newArray.length = newLength;

  for(i = 0; i < newLength; i = i + 1){
    newArray[i] = da.array[i];
  }

  delete(da.array);

  da.array = newArray;
}
function DynamicArrayNumbersIndex(da, index){
  return da.array[index];
}
function DynamicArrayNumbersLength(da){
  return da.lengthx;
}
function DynamicArrayInsertNumber(da, index, value){
  var i;

  if(da.lengthx == da.array.length){
    DynamicArrayNumbersIncreaseSize(da);
  }

  for(i = da.lengthx; i > index; i = i - 1){
    da.array[i] = da.array[i - 1];
  }

  da.array[index] = value;

  da.lengthx = da.lengthx + 1;
}
function DynamicArrayNumberSet(da, index, value){
  var success;

  if(index < da.lengthx){
    da.array[index] = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
function DynamicArrayRemoveNumber(da, index){
  var i;

  for(i = index; i < da.lengthx - 1; i = i + 1){
    da.array[i] = da.array[i + 1];
  }

  da.lengthx = da.lengthx - 1;

  if(DynamicArrayNumbersDecreaseSizeNecessary(da)){
    DynamicArrayNumbersDecreaseSize(da);
  }
}
function FreeDynamicArrayNumbers(da){
  delete(da.array);
  delete(da);
}
function DynamicArrayNumbersToArray(da){
  var array;
  var i;

  array = [];
  array.length = da.lengthx;

  for(i = 0; i < da.lengthx; i = i + 1){
    array[i] = da.array[i];
  }

  return array;
}
function ArrayToDynamicArrayNumbersWithOptimalSize(array){
  var da;
  var i;
  var c, n, newCapacity;

  /*
         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
         */
  c = array.length;
  n = (Math.log(c) - 1)/Math.log(3/2);
  newCapacity = Math.floor(n) + 1;

  da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

  for(i = 0; i < array.length; i = i + 1){
    da.array[i] = array[i];
  }

  return da;
}
function ArrayToDynamicArrayNumbers(array){
  var da;

  da = {};
  da.array = CopyNumberArray(array);
  da.lengthx = array.length;

  return da;
}
function DynamicArrayNumbersEqual(a, b){
  var equal;
  var i;

  equal = true;
  if(a.lengthx == b.lengthx){
    for(i = 0; i < a.lengthx && equal; i = i + 1){
      if(a.array[i] != b.array[i]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function DynamicArrayNumbersToLinkedList(da){
  var ll;
  var i;

  ll = CreateLinkedListNumbers();

  for(i = 0; i < da.lengthx; i = i + 1){
    LinkedListAddNumber(ll, da.array[i]);
  }

  return ll;
}
function LinkedListToDynamicArrayNumbers(ll){
  var da;
  var i;
  var node;

  node = ll.first;

  da = {};
  da.lengthx = LinkedListNumbersLength(ll);

  da.array = [];
  da.array.length = da.lengthx;

  for(i = 0; i < da.lengthx; i = i + 1){
    da.array[i] = node.value;
    node = node.next;
  }

  return da;
}
function DynamicArrayNumbersIndexOf(arr, n, foundReference){
  var found;
  var i;

  found = false;
  for(i = 0; i < arr.lengthx &&  !found ; i = i + 1){
    if(arr.array[i] == n){
      found = true;
    }
  }
  if( !found ){
    i =  -1;
  }else{
    i = i - 1;
  }

  foundReference.booleanValue = found;

  return i;
}
function DynamicArrayNumbersIsInArray(arr, n){
  var found;
  var i;

  found = false;
  for(i = 0; i < arr.lengthx &&  !found ; i = i + 1){
    if(arr.array[i] == n){
      found = true;
    }
  }

  return found;
}
function AddCharacter(list, a){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length + 1;
  for(i = 0; i < list.length; i = i + 1){
    newlist[i] = list[i];
  }
  newlist[list.length] = a;
		
  delete(list);
		
  return newlist;
}
function AddCharacterRef(list, i){
  list.string = AddCharacter(list.string, i);
}
function RemoveCharacter(list, n){
  var newlist;
  var i;

  newlist = [];
  newlist.length = list.length - 1;

  if(n >= 0 && n < list.length){
    for(i = 0; i < list.length; i = i + 1){
      if(i < n){
        newlist[i] = list[i];
      }
      if(i > n){
        newlist[i - 1] = list[i];
      }
    }

    delete(list);
  }else{
    delete(newlist);
  }

  return newlist;
}
function GetCharacterRef(list, i){
  return list.string[i];
}
function RemoveCharacterRef(list, i){
  list.string = RemoveCharacter(list.string, i);
}
function WriteStringToStingStream(stream, index, src){
  var i;

  for(i = 0; i < src.length; i = i + 1){
    stream[index.numberValue + i] = src[i];
  }
  index.numberValue = index.numberValue + src.length;
}
function WriteCharacterToStingStream(stream, index, src){
  stream[index.numberValue] = src;
  index.numberValue = index.numberValue + 1;
}
function WriteBooleanToStingStream(stream, index, src){
  if(src){
    WriteStringToStingStream(stream, index, "true".split(''));
  }else{
    WriteStringToStingStream(stream, index, "false".split(''));
  }
}
function SubstringWithCheck(string, from, to, stringReference){
  var success;

  if(from >= 0 && from <= string.length && to >= 0 && to <= string.length && from <= to){
    stringReference.string = Substring(string, from, to);
    success = true;
  }else{
    success = false;
  }

  return success;
}
function Substring(string, from, to){
  var n;
  var i, lengthx;

  lengthx = to - from;

  n = [];
  n.length = lengthx;

  for(i = from; i < to; i = i + 1){
    n[i - from] = string[i];
  }

  return n;
}
function AppendString(s1, s2){
  var newString;

  newString = ConcatenateString(s1, s2);

  delete(s1);

  return newString;
}
function ConcatenateString(s1, s2){
  var newString;
  var i;

  newString = [];
  newString.length = s1.length + s2.length;

  for(i = 0; i < s1.length; i = i + 1){
    newString[i] = s1[i];
  }

  for(i = 0; i < s2.length; i = i + 1){
    newString[s1.length + i] = s2[i];
  }

  return newString;
}
function AppendCharacter(string, c){
  var newString;

  newString = ConcatenateCharacter(string, c);

  delete(string);

  return newString;
}
function ConcatenateCharacter(string, c){
  var newString;
  var i;
  newString = [];
  newString.length = string.length + 1;

  for(i = 0; i < string.length; i = i + 1){
    newString[i] = string[i];
  }

  newString[string.length] = c;

  return newString;
}
function SplitByCharacter(toSplit, splitBy){
  var split;
  var stringToSplitBy;

  stringToSplitBy = [];
  stringToSplitBy.length = 1;
  stringToSplitBy[0] = splitBy;

  split = SplitByString(toSplit, stringToSplitBy);

  delete(stringToSplitBy);

  return split;
}
function IndexOfCharacter(string, character, indexReference){
  var i;
  var found;

  found = false;
  for(i = 0; i < string.length &&  !found ; i = i + 1){
    if(string[i] == character){
      found = true;
      indexReference.numberValue = i;
    }
  }

  return found;
}
function SubstringEqualsWithCheck(string, from, substring, equalsReference){
  var success;

  if(from < string.length){
    success = true;
    equalsReference.booleanValue = SubstringEquals(string, from, substring);
  }else{
    success = false;
  }

  return success;
}
function SubstringEquals(string, from, substring){
  var i;
  var equal;

  equal = true;
  if(string.length - from >= substring.length){
    for(i = 0; i < substring.length && equal; i = i + 1){
      if(string[from + i] != substring[i]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function IndexOfString(string, substring, indexReference){
  var i;
  var found;

  found = false;
  for(i = 0; i < string.length - substring.length + 1 &&  !found ; i = i + 1){
    if(SubstringEquals(string, i, substring)){
      found = true;
      indexReference.numberValue = i;
    }
  }

  return found;
}
function ContainsCharacter(string, character){
  var i;
  var found;

  found = false;
  for(i = 0; i < string.length &&  !found ; i = i + 1){
    if(string[i] == character){
      found = true;
    }
  }

  return found;
}
function ContainsString(string, substring){
  return IndexOfString(string, substring, {});
}
function ToUpperCase(string){
  var i;

  for(i = 0; i < string.length; i = i + 1){
    string[i] = charToUpperCase(string[i]);
  }
}
function ToLowerCase(string){
  var i;

  for(i = 0; i < string.length; i = i + 1){
    string[i] = charToLowerCase(string[i]);
  }
}
function EqualsIgnoreCase(a, b){
  var equal;
  var i;

  if(a.length == b.length){
    equal = true;
    for(i = 0; i < a.length && equal; i = i + 1){
      if(charToLowerCase(a[i]) != charToLowerCase(b[i])){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function ReplaceString(string, toReplace, replaceWith){
  var result;
  var i, j;
  var equalsReference;
  var success;
  var da;

  da = CreateDynamicArrayCharacters();

  equalsReference = {};

  for(i = 0; i < string.length; ){
    success = SubstringEqualsWithCheck(string, i, toReplace, equalsReference);
    if(success){
      success = equalsReference.booleanValue;
    }

    if(success && toReplace.length > 0){
      for(j = 0; j < replaceWith.length; j = j + 1){
        DynamicArrayAddCharacter(da, replaceWith[j]);
      }
      i = i + toReplace.length;
    }else{
      DynamicArrayAddCharacter(da, string[i]);
      i = i + 1;
    }
  }

  result = DynamicArrayCharactersToArray(da);

  FreeDynamicArrayCharacters(da);

  return result;
}
function ReplaceCharacterToNew(string, toReplace, replaceWith){
  var result;
  var i;

  result = [];
  result.length = string.length;

  for(i = 0; i < string.length; i = i + 1){
    if(string[i] == toReplace){
      result[i] = replaceWith;
    }else{
      result[i] = string[i];
    }
  }

  return result;
}
function ReplaceCharacter(string, toReplace, replaceWith){
  var i;

  for(i = 0; i < string.length; i = i + 1){
    if(string[i] == toReplace){
      string[i] = replaceWith;
    }
  }
}
function Trim(string){
  var result;
  var i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
  var firstNonWhitespaceFound;

  /* Find whitepaces at the start. */
  lastWhitespaceLocationStart =  -1;
  firstNonWhitespaceFound = false;
  for(i = 0; i < string.length &&  !firstNonWhitespaceFound ; i = i + 1){
    if(charIsWhiteSpace(string[i])){
      lastWhitespaceLocationStart = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  lastWhitespaceLocationEnd = string.length;
  firstNonWhitespaceFound = false;
  for(i = string.length - 1; i >= 0 &&  !firstNonWhitespaceFound ; i = i - 1){
    if(charIsWhiteSpace(string[i])){
      lastWhitespaceLocationEnd = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
    result = Substring(string, lastWhitespaceLocationStart + 1, lastWhitespaceLocationEnd);
  }else{
    result = [];
    result.length = 0;
  }

  return result;
}
function StartsWith(string, start){
  var startsWithString;

  startsWithString = false;
  if(string.length >= start.length){
    startsWithString = SubstringEquals(string, 0, start);
  }

  return startsWithString;
}
function EndsWith(string, end){
  var endsWithString;

  endsWithString = false;
  if(string.length >= end.length){
    endsWithString = SubstringEquals(string, string.length - end.length, end);
  }

  return endsWithString;
}
function SplitByString(toSplit, splitBy){
  var split;
  var next;
  var i;
  var c;
  var n;

  split = [];
  split.length = 0;

  next = [];
  next.length = 0;
  for(i = 0; i < toSplit.length; ){
    c = toSplit[i];

    if(SubstringEquals(toSplit, i, splitBy)){
      n = {};
      n.string = next;
      split = AddString(split, n);
      next = [];
      next.length = 0;
      i = i + splitBy.length;
    }else{
      next = AppendCharacter(next, c);
      i = i + 1;
    }
  }

  n = {};
  n.string = next;
  split = AddString(split, n);

  return split;
}
function StringIsBefore(a, b){
  var before, equal, done;
  var i;

  before = false;
  equal = true;
  done = false;

  if(a.length == 0 && b.length > 0){
    before = true;
  }else{
    for(i = 0; i < a.length && i < b.length &&  !done ; i = i + 1){
      if(a[i] != b[i]){
        equal = false;
      }
      if(charCharacterIsBefore(a[i], b[i])){
        before = true;
      }
      if(charCharacterIsBefore(b[i], a[i])){
        done = true;
      }
    }

    if(equal){
      if(a.length < b.length){
        before = true;
      }
    }
  }

  return before;
}
function JoinStringsWithSeparator(strings, separator){
  var result, string;
  var lengthx, i;
  var index;

  index = CreateNumberReference(0);

  lengthx = 0;
  for(i = 0; i < strings.length; i = i + 1){
    lengthx = lengthx + strings[i].string.length;
  }
  lengthx = lengthx + (strings.length - 1)*separator.length;

  result = [];
  result.length = lengthx;

  for(i = 0; i < strings.length; i = i + 1){
    string = strings[i].string;
    WriteStringToStingStream(result, index, string);
    if(i + 1 < strings.length){
      WriteStringToStingStream(result, index, separator);
    }
  }

  delete(index);

  return result;
}
function JoinStrings(strings){
  var result, string;
  var lengthx, i;
  var index;

  index = CreateNumberReference(0);

  lengthx = 0;
  for(i = 0; i < strings.length; i = i + 1){
    lengthx = lengthx + strings[i].string.length;
  }

  result = [];
  result.length = lengthx;

  for(i = 0; i < strings.length; i = i + 1){
    string = strings[i].string;
    WriteStringToStingStream(result, index, string);
  }

  delete(index);

  return result;
}
function StringToNumberArray(string){
  var i;
  var array;

  array = [];
  array.length = string.length;

  for(i = 0; i < string.length; i = i + 1){
    array[i] = string[i].charCodeAt(0);
  }
  return array;
}
function NumberArrayToString(array){
  var i;
  var string;

  string = [];
  string.length = array.length;

  for(i = 0; i < array.length; i = i + 1){
    string[i] = String.fromCharCode(array[i]);
  }
  return string;
}
function NumberArraysEqual(a, b){
  var equal;
  var i;

  equal = true;
  if(a.length == b.length){
    for(i = 0; i < a.length && equal; i = i + 1){
      if(a[i] != b[i]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function BooleanArraysEqual(a, b){
  var equal;
  var i;

  equal = true;
  if(a.length == b.length){
    for(i = 0; i < a.length && equal; i = i + 1){
      if(a[i] != b[i]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function StringsEqual(a, b){
  var equal;
  var i;

  equal = true;
  if(a.length == b.length){
    for(i = 0; i < a.length && equal; i = i + 1){
      if(a[i] != b[i]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
function FillNumberArray(a, value){
  var i;

  for(i = 0; i < a.length; i = i + 1){
    a[i] = value;
  }
}
function FillString(a, value){
  var i;

  for(i = 0; i < a.length; i = i + 1){
    a[i] = value;
  }
}
function FillBooleanArray(a, value){
  var i;

  for(i = 0; i < a.length; i = i + 1){
    a[i] = value;
  }
}
function FillNumberArrayRange(a, value, from, to){
  var i, lengthx;
  var success;

  if(from >= 0 && from <= a.length && to >= 0 && to <= a.length && from <= to){
    lengthx = to - from;
    for(i = 0; i < lengthx; i = i + 1){
      a[from + i] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
function FillBooleanArrayRange(a, value, from, to){
  var i, lengthx;
  var success;

  if(from >= 0 && from <= a.length && to >= 0 && to <= a.length && from <= to){
    lengthx = to - from;
    for(i = 0; i < lengthx; i = i + 1){
      a[from + i] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
function FillStringRange(a, value, from, to){
  var i, lengthx;
  var success;

  if(from >= 0 && from <= a.length && to >= 0 && to <= a.length && from <= to){
    lengthx = to - from;
    for(i = 0; i < lengthx; i = i + 1){
      a[from + i] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
function CopyNumberArray(a){
  var i;
  var n;

  n = [];
  n.length = a.length;

  for(i = 0; i < a.length; i = i + 1){
    n[i] = a[i];
  }

  return n;
}
function CopyBooleanArray(a){
  var i;
  var n;

  n = [];
  n.length = a.length;

  for(i = 0; i < a.length; i = i + 1){
    n[i] = a[i];
  }

  return n;
}
function CopyString(a){
  var i;
  var n;

  n = [];
  n.length = a.length;

  for(i = 0; i < a.length; i = i + 1){
    n[i] = a[i];
  }

  return n;
}
function CopyNumberArrayRange(a, from, to, copyReference){
  var i, lengthx;
  var n;
  var success;

  if(from >= 0 && from <= a.length && to >= 0 && to <= a.length && from <= to){
    lengthx = to - from;
    n = [];
    n.length = lengthx;

    for(i = 0; i < lengthx; i = i + 1){
      n[i] = a[from + i];
    }

    copyReference.numberArray = n;
    success = true;
  }else{
    success = false;
  }

  return success;
}
function CopyBooleanArrayRange(a, from, to, copyReference){
  var i, lengthx;
  var n;
  var success;

  if(from >= 0 && from <= a.length && to >= 0 && to <= a.length && from <= to){
    lengthx = to - from;
    n = [];
    n.length = lengthx;

    for(i = 0; i < lengthx; i = i + 1){
      n[i] = a[from + i];
    }

    copyReference.booleanArray = n;
    success = true;
  }else{
    success = false;
  }

  return success;
}
function CopyStringRange(a, from, to, copyReference){
  var i, lengthx;
  var n;
  var success;

  if(from >= 0 && from <= a.length && to >= 0 && to <= a.length && from <= to){
    lengthx = to - from;
    n = [];
    n.length = lengthx;

    for(i = 0; i < lengthx; i = i + 1){
      n[i] = a[from + i];
    }

    copyReference.string = n;
    success = true;
  }else{
    success = false;
  }

  return success;
}
function IsLastElement(lengthx, index){
  return index + 1 == lengthx;
}
function CreateNumberArray(lengthx, value){
  var array;

  array = [];
  array.length = lengthx;
  FillNumberArray(array, value);

  return array;
}
function CreateBooleanArray(lengthx, value){
  var array;

  array = [];
  array.length = lengthx;
  FillBooleanArray(array, value);

  return array;
}
function CreateString(lengthx, value){
  var array;

  array = [];
  array.length = lengthx;
  FillString(array, value);

  return array;
}
function SwapElementsOfNumberArray(A, ai, bi){
  var tmp;

  tmp = A[ai];
  A[ai] = A[bi];
  A[bi] = tmp;
}
function SwapElementsOfStringArray(A, ai, bi){
  var tmp;

  tmp = A.stringArray[ai];
  A.stringArray[ai] = A.stringArray[bi];
  A.stringArray[bi] = tmp;
}
function ReverseNumberArray(array){
  var i;

  for(i = 0; i < array.length/2; i = i + 1){
    SwapElementsOfNumberArray(array, i, array.length - i - 1);
  }
}
function AssertFalse(b, failures){
  if(b){
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertTrue(b, failures){
  if( !b ){
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertEquals(a, b, failures){
  if(a != b){
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertBooleansEqual(a, b, failures){
  if(a != b){
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertCharactersEqual(a, b, failures){
  if(a != b){
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertStringEquals(a, b, failures){
  if( !StringsEqual(a, b) ){
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertNumberArraysEqual(a, b, failures){
  var i;

  if(a.length == b.length){
    for(i = 0; i < a.length; i = i + 1){
      AssertEquals(a[i], b[i], failures);
    }
  }else{
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertBooleanArraysEqual(a, b, failures){
  var i;

  if(a.length == b.length){
    for(i = 0; i < a.length; i = i + 1){
      AssertBooleansEqual(a[i], b[i], failures);
    }
  }else{
    failures.numberValue = failures.numberValue + 1;
  }
}
function AssertStringArraysEqual(a, b, failures){
  var i;

  if(a.length == b.length){
    for(i = 0; i < a.length; i = i + 1){
      AssertStringEquals(a[i].string, b[i].string, failures);
    }
  }else{
    failures.numberValue = failures.numberValue + 1;
  }
}
function nCreateStringScientificNotationDecimalFromNumber(decimal){
  var mantissaReference, exponentReference;
  var multiplier, inc;
  var exponent;
  var done, isPositive;
  var result;

  mantissaReference = {};
  exponentReference = {};
  result = [];
  result.length = 0;
  done = false;
  exponent = 0;

  if(decimal < 0){
    isPositive = false;
    decimal =  -decimal;
  }else{
    isPositive = true;
  }

  if(decimal == 0){
    done = true;
  }

  if( !done ){
    multiplier = 0;
    inc = 0;

    if(decimal < 1){
      multiplier = 10;
      inc =  -1;
    }else if(decimal >= 10){
      multiplier = 0.1;
      inc = 1;
    }else{
      done = true;
    }

    if( !done ){
      exponent = Math.round(Math.log10(decimal));
      exponent = Math.min(99, exponent);
      exponent = Math.max( -99, exponent);

      decimal = decimal/Math.pow(10, exponent);

      /* Adjust */
      for(; (decimal >= 10 || decimal < 1) && Math.abs(exponent) < 99; ){
        decimal = decimal*multiplier;
        exponent = exponent + inc;
      }
    }
  }

  nCreateStringFromNumberWithCheck(decimal, 10, mantissaReference);

  nCreateStringFromNumberWithCheck(exponent, 10, exponentReference);

  if( !isPositive ){
    result = AppendString(result, "-".split(''));
  }

  result = AppendString(result, mantissaReference.string);
  result = AppendString(result, "e".split(''));
  result = AppendString(result, exponentReference.string);

  return result;
}
function nCreateStringDecimalFromNumber(decimal){
  var stringReference;

  stringReference = {};

  /* This will succeed because base = 10. */
  nCreateStringFromNumberWithCheck(decimal, 10, stringReference);

  return stringReference.string;
}
function nCreateStringFromNumberWithCheck(decimal, base, stringReference){
  var string;
  var maximumDigits;
  var digitPosition;
  var hasPrintedPoint, isPositive;
  var i, d;
  var success;
  var characterReference;
  var c;

  string = CreateDynamicArrayCharacters();
  isPositive = true;

  if(decimal < 0){
    isPositive = false;
    decimal =  -decimal;
  }

  if(decimal == 0){
    DynamicArrayAddCharacter(string, '0');
    success = true;
  }else{
    characterReference = {};

    if(IsInteger(base)){
      success = true;

      maximumDigits = nGetMaximumDigitsForBase(base);

      digitPosition = nGetFirstDigitPosition(decimal, base);

      decimal = Math.round(decimal*Math.pow(base, maximumDigits - digitPosition - 1));

      hasPrintedPoint = false;

      if( !isPositive ){
        DynamicArrayAddCharacter(string, '-');
      }

      /* Print leading zeros. */
      if(digitPosition < 0){
        DynamicArrayAddCharacter(string, '0');
        DynamicArrayAddCharacter(string, '.');
        hasPrintedPoint = true;
        for(i = 0; i <  -digitPosition - 1; i = i + 1){
          DynamicArrayAddCharacter(string, '0');
        }
      }

      /* Print number. */
      for(i = 0; i < maximumDigits && success; i = i + 1){
        d = Math.floor(decimal/Math.pow(base, maximumDigits - i - 1));

        if(d >= base){
          d = base - 1;
        }

        if( !hasPrintedPoint  && digitPosition - i + 1 == 0){
          if(decimal != 0){
            DynamicArrayAddCharacter(string, '.');
          }
          hasPrintedPoint = true;
        }

        if(decimal == 0 && hasPrintedPoint){
        }else{
          success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
          if(success){
            c = characterReference.characterValue;
            DynamicArrayAddCharacter(string, c);
          }
        }

        if(success){
          decimal = decimal - d*Math.pow(base, maximumDigits - i - 1);
          decimal = Math.max(decimal, 0);
          decimal = Math.round(decimal);
        }
      }

      if(success){
        /* Print trailing zeros. */
        for(i = 0; i < digitPosition - maximumDigits + 1; i = i + 1){
          DynamicArrayAddCharacter(string, '0');
        }
      }
    }else{
      success = false;
    }
  }

  if(success){
    stringReference.string = DynamicArrayCharactersToArray(string);
    FreeDynamicArrayCharacters(string);
  }

  /* Done */
  return success;
}
function nGetMaximumDigitsForBase(base){
  var t;

  t = Math.pow(10, 15);
  return Math.floor(Math.log10(t)/Math.log10(base));
}
function nGetFirstDigitPosition(decimal, base){
  var power;
  var t;

  power = Math.ceil(Math.log10(decimal)/Math.log10(base));

  t = decimal*Math.pow(base,  -power);
  if(t < base && t >= 1){
  }else if(t >= base){
    power = power + 1;
  }else if(t < 1){
    power = power - 1;
  }

  return power;
}
function nGetSingleDigitCharacterFromNumberWithCheck(c, base, characterReference){
  var numberTable;
  var success;

  numberTable = nGetDigitCharacterTable();

  if(c < base || c < numberTable.length){
    success = true;
    characterReference.characterValue = numberTable[c];
  }else{
    success = false;
  }

  return success;
}
function nGetDigitCharacterTable(){
  var numberTable;

  numberTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".split('');

  return numberTable;
}
function nCreateNumberFromDecimalStringWithCheck(string, decimalReference, errorMessage){
  return nCreateNumberFromStringWithCheck(string, 10, decimalReference, errorMessage);
}
function nCreateNumberFromDecimalString(string){
  var doubleReference;
  var stringReference;
  var number;

  doubleReference = CreateNumberReference(0);
  stringReference = CreateStringReference("".split(''));
  nCreateNumberFromStringWithCheck(string, 10, doubleReference, stringReference);
  number = doubleReference.numberValue;

  delete(doubleReference);
  delete(stringReference);

  return number;
}
function nCreateNumberFromStringWithCheck(string, base, numberReference, errorMessage){
  var success;
  var numberIsPositive, exponentIsPositive;
  var beforePoint, afterPoint, exponent;

  numberIsPositive = CreateBooleanReference(true);
  exponentIsPositive = CreateBooleanReference(true);
  beforePoint = {};
  afterPoint = {};
  exponent = {};

  if(base >= 2 && base <= 36){
    success = nExtractPartsFromNumberString(string, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage);

    if(success){
      numberReference.numberValue = nCreateNumberFromParts(base, numberIsPositive.booleanValue, beforePoint.numberArray, afterPoint.numberArray, exponentIsPositive.booleanValue, exponent.numberArray);
    }
  }else{
    success = false;
    errorMessage.string = "Base must be from 2 to 36.".split('');
  }

  return success;
}
function nCreateNumberFromParts(base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent){
  var n, i, p, e;

  n = 0;

  for(i = 0; i < beforePoint.length; i = i + 1){
    p = beforePoint[beforePoint.length - i - 1];

    n = n + p*Math.pow(base, i);
  }

  for(i = 0; i < afterPoint.length; i = i + 1){
    p = afterPoint[i];

    n = n + p*Math.pow(base,  -(i + 1));
  }

  if(exponent.length > 0){
    e = 0;
    for(i = 0; i < exponent.length; i = i + 1){
      p = exponent[exponent.length - i - 1];

      e = e + p*Math.pow(base, i);
    }

    if( !exponentIsPositive ){
      e =  -e;
    }

    n = n*Math.pow(base, e);
  }

  if( !numberIsPositive ){
    n =  -n;
  }

  return n;
}
function nExtractPartsFromNumberString(n, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages){
  var i, j, count;
  var success, done, complete;

  i = 0;
  complete = false;

  if(i < n.length){
    if(n[i] == '-'){
      numberIsPositive.booleanValue = false;
      i = i + 1;
    }else if(n[i] == '+'){
      numberIsPositive.booleanValue = true;
      i = i + 1;
    }

    success = true;
  }else{
    success = false;
    errorMessages.string = "Number cannot have length zero.".split('');
  }

  if(success){
    done = false;
    count = 0;
    for(; i + count < n.length &&  !done ; ){
      if(nCharacterIsNumberCharacterInBase(n[i + count], base)){
        count = count + 1;
      }else{
        done = true;
      }
    }

    if(count >= 1){
      beforePoint.numberArray = [];
      beforePoint.numberArray.length = count;

      for(j = 0; j < count; j = j + 1){
        beforePoint.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base);
      }

      i = i + count;

      if(i < n.length){
        success = true;
      }else{
        afterPoint.numberArray = [];
        afterPoint.numberArray.length = 0;
        exponent.numberArray = [];
        exponent.numberArray.length = 0;
        success = true;
        complete = true;
      }
    }else{
      success = false;
      errorMessages.string = "Number must have at least one number after the optional sign.".split('');
    }
  }

  if(success &&  !complete ){
    if(n[i] == '.'){
      i = i + 1;

      if(i < n.length){
        done = false;
        count = 0;
        for(; i + count < n.length &&  !done ; ){
          if(nCharacterIsNumberCharacterInBase(n[i + count], base)){
            count = count + 1;
          }else{
            done = true;
          }
        }

        if(count >= 1){
          afterPoint.numberArray = [];
          afterPoint.numberArray.length = count;

          for(j = 0; j < count; j = j + 1){
            afterPoint.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base);
          }

          i = i + count;

          if(i < n.length){
            success = true;
          }else{
            exponent.numberArray = [];
            exponent.numberArray.length = 0;
            success = true;
            complete = true;
          }
        }else{
          success = false;
          errorMessages.string = "There must be at least one digit after the decimal point.".split('');
        }
      }else{
        success = false;
        errorMessages.string = "There must be at least one digit after the decimal point.".split('');
      }
    }else if(base <= 14 && (n[i] == 'e' || n[i] == 'E')){
      if(i < n.length){
        success = true;
        afterPoint.numberArray = [];
        afterPoint.numberArray.length = 0;
      }else{
        success = false;
        errorMessages.string = "There must be at least one digit after the exponent.".split('');
      }
    }else{
      success = false;
      errorMessages.string = "Expected decimal point or exponent symbol.".split('');
    }
  }

  if(success &&  !complete ){
    if(base <= 14 && (n[i] == 'e' || n[i] == 'E')){
      i = i + 1;

      if(i < n.length){
        if(n[i] == '-'){
          exponentIsPositive.booleanValue = false;
          i = i + 1;
        }else if(n[i] == '+'){
          exponentIsPositive.booleanValue = true;
          i = i + 1;
        }

        if(i < n.length){
          done = false;
          count = 0;
          for(; i + count < n.length &&  !done ; ){
            if(nCharacterIsNumberCharacterInBase(n[i + count], base)){
              count = count + 1;
            }else{
              done = true;
            }
          }

          if(count >= 1){
            exponent.numberArray = [];
            exponent.numberArray.length = count;

            for(j = 0; j < count; j = j + 1){
              exponent.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base);
            }

            i = i + count;

            if(i == n.length){
              success = true;
            }else{
              success = false;
              errorMessages.string = "There cannot be any characters past the exponent of the number.".split('');
            }
          }else{
            success = false;
            errorMessages.string = "There must be at least one digit after the decimal point.".split('');
          }
        }else{
          success = false;
          errorMessages.string = "There must be at least one digit after the exponent symbol.".split('');
        }
      }else{
        success = false;
        errorMessages.string = "There must be at least one digit after the exponent symbol.".split('');
      }
    }else{
      success = false;
      errorMessages.string = "Expected exponent symbol.".split('');
    }
  }

  return success;
}
function nGetNumberFromNumberCharacterForBase(c, base){
  var numberTable;
  var i;
  var position;

  numberTable = nGetDigitCharacterTable();
  position = 0;

  for(i = 0; i < base; i = i + 1){
    if(numberTable[i] == c){
      position = i;
    }
  }

  return position;
}
function nCharacterIsNumberCharacterInBase(c, base){
  var numberTable;
  var i;
  var found;

  numberTable = nGetDigitCharacterTable();
  found = false;

  for(i = 0; i < base; i = i + 1){
    if(numberTable[i] == c){
      found = true;
    }
  }

  return found;
}
function nStringToNumberArray(str){
  var numberArrayReference;
  var stringReference;
  var numbers;

  numberArrayReference = {};
  stringReference = {};

  nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference);

  numbers = numberArrayReference.numberArray;

  delete(numberArrayReference);
  delete(stringReference);

  return numbers;
}
function nStringToNumberArrayWithCheck(str, numberArrayReference, errorMessage){
  var numberStrings;
  var numbers;
  var i;
  var numberString, trimmedNumberString;
  var success;
  var numberReference;

  numberStrings = SplitByString(str, ",".split(''));

  numbers = [];
  numbers.length = numberStrings.length;
  success = true;
  numberReference = {};

  for(i = 0; i < numberStrings.length; i = i + 1){
    numberString = numberStrings[i].string;
    trimmedNumberString = Trim(numberString);
    success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage);
    numbers[i] = numberReference.numberValue;

    FreeStringReference(numberStrings[i]);
    delete(trimmedNumberString);
  }

  delete(numberStrings);
  delete(numberReference);

  numberArrayReference.numberArray = numbers;

  return success;
}
function Negate(x){
  return  -x;
}
function Positive(x){
  return  +x;
}
function Factorial(x){
  var i, f;

  f = 1;

  for(i = 2; i <= x; i = i + 1){
    f = f*i;
  }

  return f;
}
function Round(x){
  return Math.floor(x + 0.5);
}
function BankersRound(x){
  var r;

  if(Absolute(x - Truncate(x)) == 0.5){
    if( !DivisibleBy(Round(x), 2) ){
      r = Round(x) - 1;
    }else{
      r = Round(x);
    }
  }else{
    r = Round(x);
  }

  return r;
}
function Ceil(x){
  return Math.ceil(x);
}
function Floor(x){
  return Math.floor(x);
}
function Truncate(x){
  var t;

  if(x >= 0){
    t = Math.floor(x);
  }else{
    t = Math.ceil(x);
  }

  return t;
}
function Absolute(x){
  return Math.abs(x);
}
function Logarithm(x){
  return Math.log10(x);
}
function NaturalLogarithm(x){
  return Math.log(x);
}
function Sin(x){
  return Math.sin(x);
}
function Cos(x){
  return Math.cos(x);
}
function Tan(x){
  return Math.tan(x);
}
function Asin(x){
  return Math.asin(x);
}
function Acos(x){
  return Math.acos(x);
}
function Atan(x){
  return Math.atan(x);
}
function Atan2(y, x){
  var a;

  /* Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors. */
  a = 0;

  if(x > 0){
    a = Atan(y/x);
  }else if(x < 0 && y >= 0){
    a = Atan(y/x) + Math.PI;
  }else if(x < 0 && y < 0){
    a = Atan(y/x) - Math.PI;
  }else if(x == 0 && y > 0){
    a = Math.PI/2;
  }else if(x == 0 && y < 0){
    a =  -Math.PI/2;
  }

  return a;
}
function Squareroot(x){
  return Math.sqrt(x);
}
function Exp(x){
  return Math.exp(x);
}
function DivisibleBy(a, b){
  return ((a%b) == 0);
}
function Combinations(n, k){
  var i, j, c;

  c = 1;
  j = 1;
  i = n - k + 1;

  for(; i <= n; ){
    c = c*i;
    c = c/j;

    i = i + 1;
    j = j + 1;
  }

  return c;
}
function Permutations(n, k){
  var i, c;

  c = 1;

  for(i = n - k + 1; i <= n; i = i + 1){
    c = c*i;
  }

  return c;
}
function EpsilonCompare(a, b, epsilon){
  return Math.abs(a - b) < epsilon;
}
function GreatestCommonDivisor(a, b){
  var t;

  for(; b != 0; ){
    t = b;
    b = a%b;
    a = t;
  }

  return a;
}
function GCDWithSubtraction(a, b){
  var g;

  if(a == 0){
    g = b;
  }else{
    for(; b != 0; ){
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
function IsInteger(a){
  return (a - Math.floor(a)) == 0;
}
function GreatestCommonDivisorWithCheck(a, b, gcdReference){
  var success;
  var gcd;

  if(IsInteger(a) && IsInteger(b)){
    gcd = GreatestCommonDivisor(a, b);
    gcdReference.numberValue = gcd;
    success = true;
  }else{
    success = false;
  }

  return success;
}
function LeastCommonMultiple(a, b){
  var lcm;

  if(a > 0 && b > 0){
    lcm = Math.abs(a*b)/GreatestCommonDivisor(a, b);
  }else{
    lcm = 0;
  }

  return lcm;
}
function Sign(a){
  var s;

  if(a > 0){
    s = 1;
  }else if(a < 0){
    s =  -1;
  }else{
    s = 0;
  }

  return s;
}
function Max(a, b){
  return Math.max(a, b);
}
function Min(a, b){
  return Math.min(a, b);
}
function Power(a, b){
  return Math.pow(a, b);
}
function Gamma(x){
  return LanczosApproximation(x);
}
function LogGamma(x){
  return Math.log(Gamma(x));
}
function LanczosApproximation(z){
  var p;
  var i, y, t, x;

  p = [];
  p.length = 8;
  p[0] = 676.5203681218851;
  p[1] =  -1259.1392167224028;
  p[2] = 771.32342877765313;
  p[3] =  -176.61502916214059;
  p[4] = 12.507343278686905;
  p[5] =  -0.13857109526572012;
  p[6] = 9.9843695780195716e-6;
  p[7] = 1.5056327351493116e-7;

  if(z < 0.5){
    y = Math.PI/(Math.sin(Math.PI*z)*LanczosApproximation(1 - z));
  }else{
    z = z - 1;
    x = 0.99999999999980993;
    for(i = 0; i < p.length; i = i + 1){
      x = x + p[i]/(z + i + 1);
    }
    t = z + p.length - 0.5;
    y = Math.sqrt(2*Math.PI)*Math.pow(t, z + 0.5)*Math.exp( -t)*x;
  }

  return y;
}
function Beta(x, y){
  return Gamma(x)*Gamma(y)/Gamma(x + y);
}
function Sinh(x){
  return (Math.exp(x) - Math.exp( -x))/2;
}
function Cosh(x){
  return (Math.exp(x) + Math.exp( -x))/2;
}
function Tanh(x){
  return Sinh(x)/Cosh(x);
}
function Cot(x){
  return 1/Math.tan(x);
}
function Sec(x){
  return 1/Math.cos(x);
}
function Csc(x){
  return 1/Math.sin(x);
}
function Coth(x){
  return Cosh(x)/Sinh(x);
}
function Sech(x){
  return 1/Cosh(x);
}
function Csch(x){
  return 1/Sinh(x);
}
function Error(x){
  var y, t, tau, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;

  if(x == 0){
    y = 0;
  }else if(x < 0){
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

    t = 1/(1 + 0.5*Math.abs(x));

    tau = t*Math.exp( -(Math.pow(x, 2)) + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))));

    y = 1 - tau;
  }

  return y;
}
function ErrorInverse(x){
  var y, a, t;

  a = (8*(Math.PI - 3))/(3*Math.PI*(4 - Math.PI));

  t = 2/(Math.PI*a) + Math.log(1 - Math.pow(x, 2))/2;
  y = Sign(x)*Math.sqrt(Math.sqrt(Math.pow(t, 2) - Math.log(1 - Math.pow(x, 2))/a) - t);

  return y;
}
function FallingFactorial(x, n){
  var k, y;

  y = 1;

  for(k = 0; k <= n - 1; k = k + 1){
    y = y*(x - k);
  }

  return y;
}
function RisingFactorial(x, n){
  var k, y;

  y = 1;

  for(k = 0; k <= n - 1; k = k + 1){
    y = y*(x + k);
  }

  return y;
}
function Hypergeometric(a, b, c, z, maxIterations, precision){
  var y;

  if(Math.abs(z) >= 0.5){
    y = Math.pow(1 - z,  -a)*HypergeometricDirect(a, c - b, c, z/(z - 1), maxIterations, precision);
  }else{
    y = HypergeometricDirect(a, b, c, z, maxIterations, precision);
  }

  return y;
}
function HypergeometricDirect(a, b, c, z, maxIterations, precision){
  var y, yp, n;
  var done;

  y = 0;
  done = false;

  for(n = 0; n < maxIterations &&  !done ; n = n + 1){
    yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*Math.pow(z, n)/Factorial(n);
    if(Math.abs(yp) < precision){
      done = true;
    }
    y = y + yp;
  }

  return y;
}
function BernouilliNumber(n){
  return AkiyamaTanigawaAlgorithm(n);
}
function AkiyamaTanigawaAlgorithm(n){
  var m, j, B;
  var A;

  A = [];
  A.length = n + 1;

  for(m = 0; m <= n; m = m + 1){
    A[m] = 1/(m + 1);
    for(j = m; j >= 1; j = j - 1){
      A[j - 1] = j*(A[j - 1] - A[j]);
    }
  }

  B = A[0];

  delete(A);

  return B;
}
function charToLowerCase(character){
  var toReturn;

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
function charToUpperCase(character){
  var toReturn;

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
function charIsUpperCase(character){
  var isUpper;

  isUpper = true;
  if(character == 'A'){
  }else if(character == 'B'){
  }else if(character == 'C'){
  }else if(character == 'D'){
  }else if(character == 'E'){
  }else if(character == 'F'){
  }else if(character == 'G'){
  }else if(character == 'H'){
  }else if(character == 'I'){
  }else if(character == 'J'){
  }else if(character == 'K'){
  }else if(character == 'L'){
  }else if(character == 'M'){
  }else if(character == 'N'){
  }else if(character == 'O'){
  }else if(character == 'P'){
  }else if(character == 'Q'){
  }else if(character == 'R'){
  }else if(character == 'S'){
  }else if(character == 'T'){
  }else if(character == 'U'){
  }else if(character == 'V'){
  }else if(character == 'W'){
  }else if(character == 'X'){
  }else if(character == 'Y'){
  }else if(character == 'Z'){
  }else{
    isUpper = false;
  }

  return isUpper;
}
function charIsLowerCase(character){
  var isLower;

  isLower = true;
  if(character == 'a'){
  }else if(character == 'b'){
  }else if(character == 'c'){
  }else if(character == 'd'){
  }else if(character == 'e'){
  }else if(character == 'f'){
  }else if(character == 'g'){
  }else if(character == 'h'){
  }else if(character == 'i'){
  }else if(character == 'j'){
  }else if(character == 'k'){
  }else if(character == 'l'){
  }else if(character == 'm'){
  }else if(character == 'n'){
  }else if(character == 'o'){
  }else if(character == 'p'){
  }else if(character == 'q'){
  }else if(character == 'r'){
  }else if(character == 's'){
  }else if(character == 't'){
  }else if(character == 'u'){
  }else if(character == 'v'){
  }else if(character == 'w'){
  }else if(character == 'x'){
  }else if(character == 'y'){
  }else if(character == 'z'){
  }else{
    isLower = false;
  }

  return isLower;
}
function charIsLetter(character){
  return charIsUpperCase(character) || charIsLowerCase(character);
}
function charIsNumber(character){
  var isNumberx;

  isNumberx = true;
  if(character == '0'){
  }else if(character == '1'){
  }else if(character == '2'){
  }else if(character == '3'){
  }else if(character == '4'){
  }else if(character == '5'){
  }else if(character == '6'){
  }else if(character == '7'){
  }else if(character == '8'){
  }else if(character == '9'){
  }else{
    isNumberx = false;
  }

  return isNumberx;
}
function charIsWhiteSpace(character){
  var isWhiteSpacex;

  isWhiteSpacex = true;
  if(character == ' '){
  }else if(character == '\t'){
  }else if(character == '\n'){
  }else if(character == '\r'){
  }else{
    isWhiteSpacex = false;
  }

  return isWhiteSpacex;
}
function charIsSymbol(character){
  var isSymbolx;

  isSymbolx = true;
  if(character == '!'){
  }else if(character == '\"'){
  }else if(character == '#'){
  }else if(character == '$'){
  }else if(character == '%'){
  }else if(character == '&'){
  }else if(character == '\''){
  }else if(character == '('){
  }else if(character == ')'){
  }else if(character == '*'){
  }else if(character == '+'){
  }else if(character == ','){
  }else if(character == '-'){
  }else if(character == '.'){
  }else if(character == '/'){
  }else if(character == ':'){
  }else if(character == ';'){
  }else if(character == '<'){
  }else if(character == '='){
  }else if(character == '>'){
  }else if(character == '?'){
  }else if(character == '@'){
  }else if(character == '['){
  }else if(character == '\\'){
  }else if(character == ']'){
  }else if(character == '^'){
  }else if(character == '_'){
  }else if(character == '`'){
  }else if(character == '{'){
  }else if(character == '|'){
  }else if(character == '}'){
  }else if(character == '~'){
  }else{
    isSymbolx = false;
  }

  return isSymbolx;
}
function charCharacterIsBefore(a, b){
  var ad, bd;

  ad = a.charCodeAt(0);
  bd = b.charCodeAt(0);

  return ad < bd;
}
function charDecimalDigitToCharacter(digit){
  var c;
  if(digit == 1){
    c = '1';
  }else if(digit == 2){
    c = '2';
  }else if(digit == 3){
    c = '3';
  }else if(digit == 4){
    c = '4';
  }else if(digit == 5){
    c = '5';
  }else if(digit == 6){
    c = '6';
  }else if(digit == 7){
    c = '7';
  }else if(digit == 8){
    c = '8';
  }else if(digit == 9){
    c = '9';
  }else{
    c = '0';
  }
  return c;
}
function charCharacterToDecimalDigit(c){
  var digit;

  if(c == '1'){
    digit = 1;
  }else if(c == '2'){
    digit = 2;
  }else if(c == '3'){
    digit = 3;
  }else if(c == '4'){
    digit = 4;
  }else if(c == '5'){
    digit = 5;
  }else if(c == '6'){
    digit = 6;
  }else if(c == '7'){
    digit = 7;
  }else if(c == '8'){
    digit = 8;
  }else if(c == '9'){
    digit = 9;
  }else{
    digit = 0;
  }

  return digit;
}

