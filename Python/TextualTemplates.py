# Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

from math import *
class LinkedListNodeNodes:
  end = None
  value = None
  next = None

class LinkedListNodes:
  first = None
  last = None

class Node:
  type = None
  p1 = None
  p2 = None
  block1 = None
  hasElseBlock = None
  block2 = None
  nodes = None

def CreateLinkedListNodes():

  ll = LinkedListNodes()
  ll.first = LinkedListNodeNodes()
  ll.last = ll.first
  ll.last.end = True

  return ll

def LinkedListAddNode(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = LinkedListNodeNodes()
  ll.last.next.end = True
  ll.last = ll.last.next

def LinkedListNodesToArray(ll):

  node = ll.first

  length = LinkedListNodesLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return array

def LinkedListNodesLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def FreeLinkedListNode(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def IsValidTemplate(template):

  tokens = CreateLinkedListString()
  errorMessage = StringReference()

  success = GenerateTokensFromTemplate(template, tokens, errorMessage)

  if success:
    root = Node()

    ts = LinkedListStringsToArray(tokens)

    success = ParseTemplate(ts, root, errorMessage)

  if  not success :
    FreeStringReference(errorMessage)

  return success

def GenerateTokensFromTemplate(template, tokens, errorMessage):

  success = True
  cs =  [None]*int(4.0)

  da = CreateDynamicArrayCharacters()

  pc = 'x'
  i = 0.0
  while i < len(template) and success:
    c = template[int(i)]

    if c != '{':
      DynamicArrayAddCharacter(da, c)
      i = i + 1.0
    else:
      FillString(cs, 'x')
      j = 0.0
      while i + j < len(template) and j < len(cs):
        cs[int(j)] = template[int(i + j)]
        j = j + 1.0
      

      if StringsEqual(cs, "{use") or StringsEqual(cs, "{end") or StringsEqual(cs, "{pri") or StringsEqual(cs, "{for") or StringsEqual(cs, "{if ") or StringsEqual(cs, "{els"):
        if pc != '\\':
          # Find end.
          found = False
          l = 0.0
          while i + l < len(template) and  not found :
            if template[int(i + l)] == '}':
              found = True
            l = l + 1.0
          

          if found:
            if da.length > 0.0:
              a = DynamicArrayCharactersToArray(da)
              LinkedListAddString(tokens, a)
              FreeDynamicArrayCharacters(da)
              da = CreateDynamicArrayCharacters()

            j = 0.0
            while j < l:
              DynamicArrayAddCharacter(da, template[int(i + j)])
              j = j + 1.0
            

            a = DynamicArrayCharactersToArray(da)
            LinkedListAddString(tokens, a)
            FreeDynamicArrayCharacters(da)
            da = CreateDynamicArrayCharacters()

            i = i + l
          else:
            success = False
            errorMessage.string = "Template command found, but not ended properly."
        else:
          DynamicArrayAddCharacter(da, c)
          i = i + 1.0
      else:
        DynamicArrayAddCharacter(da, c)
        i = i + 1.0

    pc = c
  

  if da.length > 0.0:
    a = DynamicArrayCharactersToArray(da)
    LinkedListAddString(tokens, a)

  FreeDynamicArrayCharacters(da)

  return success

def GenerateDocument(template, data, document, errorMessage):

  tokens = CreateLinkedListString()

  success = GenerateTokensFromTemplate(template, tokens, errorMessage)

  if success:
    root = Node()

    ts = LinkedListStringsToArray(tokens)
    FreeLinkedListString(tokens)

    success = ParseTemplate(ts, root, errorMessage)

    if success:
      ll = CreateLinkedListCharacter()

      success = GenerateDocumentFromBlock(root, data, ll, errorMessage)

      if success:
        document.string = LinkedListCharactersToArray(ll)
        FreeLinkedListCharacter(ll)

  return success

def GenerateDocumentFromBlock(root, data, ll, errorMessage):

  n = root.nodes.first
  success = True

  while  not n.end  and success:
    success = GenerateDocumentFromNode(n.value, data, ll, errorMessage)
    n = n.next
  

  return success

def GenerateDocumentFromNode(n, data, ll, errorMessage):

  success = True
  found = BooleanReference()

  if StringsEqual(n.type, "block"):
    success = GenerateDocumentFromBlock(n, data, ll, errorMessage)
  elif StringsEqual(n.type, "use"):
    success = GenerateDocumentFromUse(n, data, ll, errorMessage)
  elif StringsEqual(n.type, "if"):
    success = GenerateDocumentFromIf(n, data, ll, errorMessage)
  elif StringsEqual(n.type, "foreach"):
    success = GenerateDocumentFromForeach(n, data, ll, errorMessage)
  elif StringsEqual(n.type, "text"):
    success = True
    LinkedListCharactersAddString(ll, n.p1)
  elif StringsEqual(n.type, "print"):
    if StringsEqual(data.type, "object"):
      value = GetObjectValueWithCheck(data.object, n.p1, found)

      if found.booleanValue:
        if StringsEqual(value.type, "string"):
          valueString = value.string
        else:
          valueString = WriteJSON(value)
        LinkedListCharactersAddString(ll, valueString)
      else:
        success = False
        errorMessage.string = "Key for printing not found in JSON object: "
        errorMessage.string = sConcatenateString(errorMessage.string, n.p1)
    else:
      success = False
      errorMessage.string = "Data structure for print command is not a JSON object."

  return success

def GenerateDocumentFromUse(n, data, ll, errorMessage):

  found = BooleanReference()

  if StringsEqual(data.type, "object"):
    value = GetObjectValueWithCheck(data.object, n.p1, found)

    if found.booleanValue:
      success = GenerateDocumentFromNode(n.block1, value, ll, errorMessage)
    else:
      success = False
      errorMessage.string = "Key for use not found in JSON object."
  else:
    success = False
    errorMessage.string = "Data structure for use command is not a JSON object."

  return success

def GenerateDocumentFromIf(n, data, ll, errorMessage):

  success = True
  found = BooleanReference()

  if StringsEqual(data.type, "object"):
    value = GetObjectValueWithCheck(data.object, n.p1, found)

    if found.booleanValue:
      if StringsEqual(value.type, "boolean"):
        if value.booleanValue:
          success = GenerateDocumentFromBlock(n.block1, data, ll, errorMessage)

        if n.hasElseBlock:
          if  not value.booleanValue :
            success = GenerateDocumentFromBlock(n.block2, data, ll, errorMessage)
      else:
        success = False
        errorMessage.string = "Value for if not boolean."
    else:
      success = False
      errorMessage.string = "Key for if not found in JSON object: "
      errorMessage.string = sConcatenateString(errorMessage.string, n.p1)
  else:
    success = False
    errorMessage.string = "Data structure for if command is not a JSON object."

  return success

def GenerateDocumentFromForeach(n, data, ll, errorMessage):

  success = True
  found = BooleanReference()
  loopVar = CreateObjectElement(0.0)

  PutStringElementMap(loopVar.object, n.p1, Element())

  if StringsEqual(data.type, "object"):
    value = GetObjectValueWithCheck(data.object, n.p2, found)

    if found.booleanValue:
      if StringsEqual(value.type, "array"):

        i = 0.0
        while i < len(value.array):
          arrayElement = value.array[int(i)]
          if StringsEqual(arrayElement.type, "object"):
            success = GenerateDocumentFromBlock(n.block1, arrayElement, ll, errorMessage)
          else:
            SetStringElementMap(loopVar.object, 0.0, n.p1, arrayElement)
            success = GenerateDocumentFromBlock(n.block1, loopVar, ll, errorMessage)
          i = i + 1.0
        
      else:
        success = False
        errorMessage.string = "Value for foreach is not an array."
    else:
      success = False
      errorMessage.string = "Key for foreach not found in JSON object: "
      errorMessage.string = sConcatenateString(errorMessage.string, n.p2)
  else:
    success = False
    errorMessage.string = "Data structure for foreach command is not a JSON object."

  return success

def ParseTemplate(tokens, node, errorMessage):

  position = CreateNumberReference(0.0)
  success = ParseTemplateBlock(tokens, position, node, errorMessage)

  if success:
    if position.numberValue != len(tokens):
      success = False
      errorMessage.string = "Unexpected token at the end of template."

  position = None

  return success

def ParseTemplateBlock(tokens, position, node, errorMessage):

  success = True
  done = False

  node.type = "block"
  node.nodes = CreateLinkedListNodes()

  while position.numberValue < len(tokens) and success and  not done :
    tn = Node()
    success = ParseNodeString(tokens[int(position.numberValue)].string, tn, errorMessage)

    if success:
      if StringsEqual(tn.type, "text") or StringsEqual(tn.type, "print"):
        LinkedListAddNode(node.nodes, tn)
        position.numberValue = position.numberValue + 1.0
      elif StringsEqual(tn.type, "use"):
        nb = Node()
        success = ParseUseBlock(tokens, position, nb, errorMessage)
        LinkedListAddNode(node.nodes, nb)
      elif StringsEqual(tn.type, "if"):
        nb = Node()
        success = ParseIfBlock(tokens, position, nb, errorMessage)
        LinkedListAddNode(node.nodes, nb)
      elif StringsEqual(tn.type, "foreach"):
        nb = Node()
        success = ParseForeachBlock(tokens, position, nb, errorMessage)
        LinkedListAddNode(node.nodes, nb)
      else:
        done = True
  

  return success

def ParseUseBlock(tokens, position, useBlock, errorMessage):

  n = Node()
  ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

  useBlock.type = CopyString(n.type)
  useBlock.p1 = CopyString(n.p1)
  useBlock.block1 = Node()

  position.numberValue = position.numberValue + 1.0

  success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage)

  if success:
    if position.numberValue < len(tokens):
      n = Node()
      ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

      if StringsEqual(n.type, "end"):
        success = True
        position.numberValue = position.numberValue + 1.0
      else:
        success = False
        errorMessage.string = "End block expected at the end of use block."
    else:
      success = False
      errorMessage.string = "End block expected at the end of use block."

  return success

def ParseIfBlock(tokens, position, ifBlock, errorMessage):

  n = Node()
  ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

  ifBlock.type = CopyString(n.type)
  ifBlock.p1 = CopyString(n.p1)
  ifBlock.block1 = Node()
  ifBlock.hasElseBlock = False

  position.numberValue = position.numberValue + 1.0

  success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage)

  if success:
    if position.numberValue < len(tokens):
      n = Node()
      ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

      if StringsEqual(n.type, "end"):
        success = True
        position.numberValue = position.numberValue + 1.0
      elif StringsEqual(n.type, "else"):
        position.numberValue = position.numberValue + 1.0
        ifBlock.hasElseBlock = True
        ifBlock.block2 = Node()
        success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage)

        if success:
          if position.numberValue < len(tokens):
            n = Node()
            ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

            if StringsEqual(n.type, "end"):
              success = True
              position.numberValue = position.numberValue + 1.0
            else:
              success = False
              errorMessage.string = "End block expected at the end of else block."
          else:
            success = False
            errorMessage.string = "End block expected at the end of else block."
      else:
        success = False
        errorMessage.string = "End or else block expected at the end of if block."
    else:
      success = False
      errorMessage.string = "End or else block expected at the end of if block."

  return success

def ParseForeachBlock(tokens, position, foreachBlock, errorMessage):

  n = Node()
  ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

  foreachBlock.type = CopyString(n.type)
  foreachBlock.p1 = CopyString(n.p1)
  foreachBlock.p2 = CopyString(n.p2)
  foreachBlock.block1 = Node()

  position.numberValue = position.numberValue + 1.0

  success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage)

  if success:
    if position.numberValue < len(tokens):
      n = Node()
      ParseNodeString(tokens[int(position.numberValue)].string, n, errorMessage)

      if StringsEqual(n.type, "end"):
        success = True
        position.numberValue = position.numberValue + 1.0
      else:
        success = False
        errorMessage.string = "End block expected at the end of for each block."
    else:
      success = False
      errorMessage.string = "End block expected at the end of foreach block."

  return success

def ParseNodeString(token, node, errorMessage):

  success = True
  isText = False

  if len(token) <= 2.0:
    isText = True
  elif token[int(0.0)] == '\\' and token[int(1.0)] == '{':
    isText = True
  elif token[int(0.0)] != '{':
    isText = True
  else:
    command = strSubstring(token, 1.0, len(token) - 1.0)
    parts = sSplitByCharacter(command, ' ')

    if len(command) > 0.0:
      if StringsEqual(parts[int(0.0)].string, "use"):
        if len(parts) == 2.0:
          node.type = CopyString(parts[int(0.0)].string)
          node.p1 = CopyString(parts[int(1.0)].string)
        else:
          success = False
          errorMessage.string = "The use command takes one parameter."
      elif StringsEqual(parts[int(0.0)].string, "end"):
        if len(parts) == 1.0:
          node.type = CopyString(parts[int(0.0)].string)
        else:
          success = False
          errorMessage.string = "The end command takes no parameters."
      elif StringsEqual(parts[int(0.0)].string, "print"):
        if len(parts) == 2.0:
          node.type = CopyString(parts[int(0.0)].string)
          node.p1 = CopyString(parts[int(1.0)].string)
        else:
          success = False
          errorMessage.string = "The print command takes one parameter."
      elif StringsEqual(parts[int(0.0)].string, "foreach"):
        if len(parts) == 4.0:
          if StringsEqual(parts[int(2.0)].string, "in"):
            node.type = CopyString(parts[int(0.0)].string)
            node.p1 = CopyString(parts[int(1.0)].string)
            node.p2 = CopyString(parts[int(3.0)].string)
          else:
            success = False
            errorMessage.string = "The foreach command must have \"in\" after the first parameter."
        else:
          success = False
          errorMessage.string = "The foreach command takes three parameters."
      elif StringsEqual(parts[int(0.0)].string, "if"):
        if len(parts) == 2.0:
          node.type = CopyString(parts[int(0.0)].string)
          node.p1 = CopyString(parts[int(1.0)].string)
        else:
          success = False
          errorMessage.string = "The if command takes one parameter."
      elif StringsEqual(parts[int(0.0)].string, "else"):
        if len(parts) == 1.0:
          node.type = CopyString(parts[int(0.0)].string)
        else:
          success = False
          errorMessage.string = "The else command takes no parameters."
      else:
        isText = True
    else:
      isText = True

  if isText:
    node.type = "text"
    node.p1 = sReplaceString(token, "\\{print ", "{print ")
    node.p1 = sReplaceString(node.p1, "\\{use ", "{use ")
    node.p1 = sReplaceString(node.p1, "\\{if ", "{if ")
    node.p1 = sReplaceString(node.p1, "\\{end}", "{end}")
    node.p1 = sReplaceString(node.p1, "\\{foreach ", "{foreach ")
    node.p1 = sReplaceString(node.p1, "\\{else}", "{else}")

  return success

def test():

  failures = CreateNumberReference(0.0)

  testTokenGeneration(failures)

  testGenerateDocument1(failures)
  testGenerateDocument2(failures)
  testGenerateDocument3(failures)
  testGenerateDocument4(failures)
  testGenerateDocument5(failures)
  testGenerateDocument6(failures)
  testGenerateDocument7(failures)

  return failures.numberValue

def testTokenGeneration(failures):

  errorMessage = StringReference()

  tokens = CreateLinkedListString()
  template = "This is a template, this is a value: {print a}."
  success = GenerateTokensFromTemplate(template, tokens, errorMessage)
  AssertTrue(success, failures)
  AssertEquals(3.0, LinkedListStringsLength(tokens), failures)

  tokens = CreateLinkedListString()
  template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{end}."
  success = GenerateTokensFromTemplate(template, tokens, errorMessage)
  AssertTrue(success, failures)
  AssertEquals(9.0, LinkedListStringsLength(tokens), failures)

  tokens = CreateLinkedListString()
  template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}."
  success = GenerateTokensFromTemplate(template, tokens, errorMessage)
  AssertTrue(success, failures)
  AssertEquals(16.0, LinkedListStringsLength(tokens), failures)

  tokens = CreateLinkedListString()
  template = "T: {foreach a in b}{print a}{end}."
  success = GenerateTokensFromTemplate(template, tokens, errorMessage)
  AssertTrue(success, failures)
  AssertEquals(5.0, LinkedListStringsLength(tokens), failures)

def testGenerateDocument1(failures):
  AssertTemplateResult("This is a template, this is a value: {print a}.", "{\"c\": 5, \"a\": 6}", "This is a template, this is a value: 6.", failures)

def testGenerateDocument2(failures):
  AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.", "{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}", "This is a template, this is a value: 6 1 2.", failures)

def testGenerateDocument3(failures):
  AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.", "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}", "This is a template, this is a value: 6 1 23 416.", failures)

def testGenerateDocument4(failures):
  AssertTemplateResult("T: {if a}a{end}.", "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}", "T: a.", failures)

  AssertTemplateResult("T: {if a}a{else}b{end}.", "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}", "T: b.", failures)

def testGenerateDocument5(failures):
  AssertTemplateResult("T: {foreach a in b}{print a}{end}.", "{\"b\": [1, 2, 3, 4]}", "T: 1234.", failures)

def testGenerateDocument6(failures):
  AssertTemplateResult("{test}\\{print a}.", "{\"c\": 5, \"a\": 6}", "{test}{print a}.", failures)

def testGenerateDocument7(failures):
  AssertTemplateResult("{", "{}", "{", failures)

  AssertTemplateResult("{  ", "{}", "{  ", failures)

  AssertTemplateResult("{use a}\\{print a}{end}", "{\"a\": {}}", "{print a}", failures)

  AssertTemplateResult("\\{print a}{print a}}", "{\"a\": 3}", "{print a}3}", failures)

  AssertTemplateResult("\\\\{print a}{print a}}", "{\"a\": 3}", "\\{print a}3}", failures)

  AssertTemplateResult("\\\\{print a}{print a}\\\\{print a}}", "{\"a\": 3}", "\\{print a}3\\{print a}}", failures)

  AssertTemplateResult("\\{print a}{print a}\\{", "{\"a\": 3}", "{print a}3\\{", failures)

  AssertTemplateResult("    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>", "{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}", "    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>", failures)

  AssertTemplateError("{print", "{}", "Template command found, but not ended properly.", failures)

  AssertTemplateError("{print a}", "{}", "Key for printing not found in JSON object: a", failures)

  AssertTemplateError("This is a template, this is a value: {print a {print a}.", "{\"a\": 5}", "The print command takes one parameter.", failures)

  AssertTemplateError("This is a {use a}\\{print a}template, this is a value: {print a}.{end}", "{\"a\": 5}", "Data structure for print command is not a JSON object.", failures)

  AssertTemplateError("{use a}", "{}", "End block expected at the end of use block.", failures)

  AssertTemplateError("{if a}", "{\"a\": true}", "End or else block expected at the end of if block.", failures)

  AssertTemplateError("{if a}{else}", "{\"a\": true}", "End block expected at the end of else block.", failures)

  AssertTemplateError("{foreach x in a}", "{\"a\": [1, 2, 3, 4]}", "End block expected at the end of foreach block.", failures)

def AssertTemplateResult(template, json, result, failures):

  data = ElementReference()
  errorMessages = StringArrayReference()
  document = StringReference()
  errorMessage = StringReference()

  success = ReadJSON(json, data, errorMessages)

  AssertTrue(success, failures)

  if success:
    success = GenerateDocument(template, data.element, document, errorMessage)

    AssertTrue(success, failures)

    if success:
      AssertStringEquals(document.string, result, failures)

def AssertTemplateError(template, json, errorMessage, failures):

  data = ElementReference()
  errorMessages = StringArrayReference()
  document = StringReference()
  errorMessageRef = StringReference()

  success = ReadJSON(json, data, errorMessages)

  AssertTrue(success, failures)

  if success:
    success = GenerateDocument(template, data.element, document, errorMessageRef)

    AssertFalse(success, failures)

    if  not success :
      AssertStringEquals(errorMessage, errorMessageRef.string, failures)

class BooleanArrayReference:
  booleanArray = None

class BooleanReference:
  booleanValue = None

class CharacterReference:
  characterValue = None

class NumberArrayReference:
  numberArray = None

class NumberReference:
  numberValue = None

class StringArrayReference:
  stringArray = None

class StringReference:
  string = None

def CreateBooleanReference(value):

  ref = BooleanReference()
  ref.booleanValue = value

  return ref

def CreateBooleanArrayReference(value):

  ref = BooleanArrayReference()
  ref.booleanArray = value

  return ref

def CreateBooleanArrayReferenceLengthValue(length, value):

  ref = BooleanArrayReference()
  ref.booleanArray =  [None]*int(length)

  i = 0.0
  while i < length:
    ref.booleanArray[int(i)] = value
    i = i + 1.0
  

  return ref

def FreeBooleanArrayReference(booleanArrayReference):
  booleanArrayReference.booleanArray = None
  booleanArrayReference = None

def CreateCharacterReference(value):

  ref = CharacterReference()
  ref.characterValue = value

  return ref

def CreateNumberReference(value):

  ref = NumberReference()
  ref.numberValue = value

  return ref

def CreateNumberArrayReference(value):

  ref = NumberArrayReference()
  ref.numberArray = value

  return ref

def CreateNumberArrayReferenceLengthValue(length, value):

  ref = NumberArrayReference()
  ref.numberArray =  [None]*int(length)

  i = 0.0
  while i < length:
    ref.numberArray[int(i)] = value
    i = i + 1.0
  

  return ref

def FreeNumberArrayReference(numberArrayReference):
  numberArrayReference.numberArray = None
  numberArrayReference = None

def CreateStringReference(value):

  ref = StringReference()
  ref.string = value

  return ref

def CreateStringReferenceLengthValue(length, value):

  ref = StringReference()
  ref.string =  [None]*int(length)

  i = 0.0
  while i < length:
    ref.string[int(i)] = value
    i = i + 1.0
  

  return ref

def FreeStringReference(stringReference):
  stringReference.string = None
  stringReference = None

def CreateStringArrayReference(strings):

  ref = StringArrayReference()
  ref.stringArray = strings

  return ref

def CreateStringArrayReferenceLengthValue(length, value):

  ref = StringArrayReference()
  ref.stringArray =  [None]*int(length)

  i = 0.0
  while i < length:
    ref.stringArray[int(i)] = CreateStringReference(value)
    i = i + 1.0
  

  return ref

def FreeStringArrayReference(stringArrayReference):

  i = 0.0
  while i < len(stringArrayReference.stringArray):
    stringArrayReference.stringArray[int(i)] = None
    i = i + 1.0
  
  stringArrayReference.stringArray = None
  stringArrayReference = None

class ElementArrayReference:
  array = None

class LinkedListElements:
  first = None
  last = None

class LinkedListNodeElements:
  end = None
  value = None
  next = None

class Element:
  type = None
  object = None
  array = None
  string = None
  number = None
  booleanValue = None

class ElementReference:
  element = None

class ElementType:
  name = None

class StringElementMap:
  stringListRef = None
  elementListRef = None

def IsValidJSON(json, errorMessage):

  elementReference = ElementReference()

  success = ReadJSON(json, elementReference, errorMessage)

  if success:
    DeleteElement(elementReference.element)

  return success

def JSONTokenize(json, tokensReference, errorMessages):

  ll = lCreateLinkedListString()
  success = True

  stringLength = NumberReference()
  tokenReference = StringReference()

  i = 0.0
  while i < len(json) and success:
    c = json[int(i)]

    if c == '{':
      lLinkedListAddString(ll, "{")
      i = i + 1.0
    elif c == '}':
      lLinkedListAddString(ll, "}")
      i = i + 1.0
    elif c == '[':
      lLinkedListAddString(ll, "[")
      i = i + 1.0
    elif c == ']':
      lLinkedListAddString(ll, "]")
      i = i + 1.0
    elif c == ':':
      lLinkedListAddString(ll, ":")
      i = i + 1.0
    elif c == ',':
      lLinkedListAddString(ll, ",")
      i = i + 1.0
    elif c == 'f':
      success = GetJSONPrimitiveName(json, i, errorMessages, "false", tokenReference)
      if success:
        lLinkedListAddString(ll, "false")
        i = i + len("false")
    elif c == 't':
      success = GetJSONPrimitiveName(json, i, errorMessages, "true", tokenReference)
      if success:
        lLinkedListAddString(ll, "true")
        i = i + len("true")
    elif c == 'n':
      success = GetJSONPrimitiveName(json, i, errorMessages, "null", tokenReference)
      if success:
        lLinkedListAddString(ll, "null")
        i = i + len("null")
    elif c == ' ' or c == '\n' or c == '\t' or c == '\r':
      # Skip.
      i = i + 1.0
    elif c == '\"':
      success = GetJSONString(json, i, tokenReference, stringLength, errorMessages)
      if success:
        lLinkedListAddString(ll, tokenReference.string)
        i = i + stringLength.numberValue
    elif IsJSONNumberCharacter(c):
      success = GetJSONNumberToken(json, i, tokenReference, errorMessages)
      if success:
        lLinkedListAddString(ll, tokenReference.string)
        i = i + len(tokenReference.string)
    else:
      str = strConcatenateCharacter("Invalid start of Token: ", c)
      stringReference = CreateStringReference(str)
      lAddStringRef(errorMessages, stringReference)
      i = i + 1.0
      success = False
  

  if success:
    lLinkedListAddString(ll, "<end>")
    tokensReference.stringArray = lLinkedListStringsToArray(ll)
    lFreeLinkedListString(ll)

  return success

def GetJSONNumberToken(json, start, tokenReference, errorMessages):

  end = len(json)
  done = False

  i = start
  while i < len(json) and  not done :
    c = json[int(i)]
    if  not IsJSONNumberCharacter(c) :
      done = True
      end = i
    i = i + 1.0
  

  numberString = strSubstring(json, start, end)

  success = IsValidJSONNumber(numberString, errorMessages)

  tokenReference.string = numberString

  return success

def IsValidJSONNumber(n, errorMessages):

  i = 0.0

  # JSON allows an optional negative sign.
  if n[int(i)] == '-':
    i = i + 1.0

  if i < len(n):
    success = IsValidJSONNumberAfterSign(n, i, errorMessages)
  else:
    success = False
    lAddStringRef(errorMessages, CreateStringReference("Number must contain at least one digit."))

  return success

def IsValidJSONNumberAfterSign(n, i, errorMessages):

  if charIsNumber(n[int(i)]):
    # 0 first means only 0.
    if n[int(i)] == '0':
      i = i + 1.0
    else:
      # 1-9 first, read following digits.
      i = IsValidJSONNumberAdvancePastDigits(n, i)

    if i < len(n):
      success = IsValidJSONNumberFromDotOrExponent(n, i, errorMessages)
    else:
      # If integer, we are done now.
      success = True
  else:
    success = False
    lAddStringRef(errorMessages, CreateStringReference("A number must start with 0-9 (after the optional sign)."))

  return success

def IsValidJSONNumberAdvancePastDigits(n, i):

  i = i + 1.0
  done = False
  while i < len(n) and  not done :
    if charIsNumber(n[int(i)]):
      i = i + 1.0
    else:
      done = True
  

  return i

def IsValidJSONNumberFromDotOrExponent(n, i, errorMessages):

  wasDotAndOrE = False
  success = True

  if n[int(i)] == '.':
    i = i + 1.0
    wasDotAndOrE = True

    if i < len(n):
      if charIsNumber(n[int(i)]):
        # Read digits following decimal point.
        i = IsValidJSONNumberAdvancePastDigits(n, i)

        if i == len(n):
          # If non-scientific decimal number, we are done.
          success = True
      else:
        success = False
        lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point."))
    else:
      success = False
      lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point."))

  if i < len(n) and success:
    if n[int(i)] == 'e' or n[int(i)] == 'E':
      wasDotAndOrE = True
      success = IsValidJSONNumberFromExponent(n, i, errorMessages)
    else:
      success = False
      lAddStringRef(errorMessages, CreateStringReference("Expected e or E."))
  elif i == len(n) and success:
    # If number with decimal point.
    success = True
  else:
    success = False
    lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point."))

  if wasDotAndOrE:
    pass
  else:
    success = False
    lAddStringRef(errorMessages, CreateStringReference("Exprected decimal point or e or E."))

  return success

def IsValidJSONNumberFromExponent(n, i, errorMessages):

  i = i + 1.0

  if i < len(n):
    # The exponent sign can either + or -,
    if n[int(i)] == '+' or n[int(i)] == '-':
      i = i + 1.0

    if i < len(n):
      if charIsNumber(n[int(i)]):
        # Read digits following decimal point.
        i = IsValidJSONNumberAdvancePastDigits(n, i)

        if i == len(n):
          # We found scientific number.
          success = True
        else:
          success = False
          lAddStringRef(errorMessages, CreateStringReference("There was characters following the exponent."))
      else:
        success = False
        lAddStringRef(errorMessages, CreateStringReference("There must be a digit following the optional exponent sign."))
    else:
      success = False
      lAddStringRef(errorMessages, CreateStringReference("There must be a digit following optional the exponent sign."))
  else:
    success = False
    lAddStringRef(errorMessages, CreateStringReference("There must be a sign or a digit following e or E."))

  return success

def IsJSONNumberCharacter(c):

  numericCharacters = "0123456789.-+eE"

  found = False

  i = 0.0
  while i < len(numericCharacters):
    if numericCharacters[int(i)] == c:
      found = True
    i = i + 1.0
  

  return found

def GetJSONPrimitiveName(string, start, errorMessages, primitive, tokenReference):

  done = False
  success = True

  token = ""

  i = start
  while i < len(string) and ((i - start) < len(primitive)) and  not done :
    c = string[int(i)]
    p = primitive[int(i - start)]
    if c == p:
      # OK
      if (i + 1.0 - start) == len(primitive):
        done = True
    else:
      str = ""
      str = strConcatenateString(str, "Primitive invalid: ")
      str = strAppendCharacter(str, c)
      str = strAppendString(str, " vs ")
      str = strAppendCharacter(str, p)

      lAddStringRef(errorMessages, CreateStringReference(str))
      done = True
      success = False
    i = i + 1.0
  

  if done:
    if StringsEqual(primitive, "false"):
      token = "false"
    if StringsEqual(primitive, "true"):
      token = "true"
    if StringsEqual(primitive, "null"):
      token = "null"
  else:
    lAddStringRef(errorMessages, CreateStringReference("Primitive invalid"))
    success = False

  tokenReference.string = token

  return success

def GetJSONString(json, start, tokenReference, stringLengthReference, errorMessages):

  characterCount = CreateNumberReference(0.0)
  hex = CreateString(4.0, '0')
  hexReference = NumberReference()
  errorMessage = StringReference()

  success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages)

  if success:
    l = characterCount.numberValue
    string =  [None]*int(l)

    c = 0.0
    string[int(c)] = '\"'
    c = c + 1.0

    done = False
    i = start + 1.0
    while  not done :
      if json[int(i)] == '\\':
        i = i + 1.0
        if json[int(i)] == '\"' or json[int(i)] == '\\' or json[int(i)] == '/':
          string[int(c)] = json[int(i)]
          c = c + 1.0
        elif json[int(i)] == 'b':
          string[int(c)] = unichr(int(8.0))
          c = c + 1.0
        elif json[int(i)] == 'f':
          string[int(c)] = unichr(int(12.0))
          c = c + 1.0
        elif json[int(i)] == 'n':
          string[int(c)] = unichr(int(10.0))
          c = c + 1.0
        elif json[int(i)] == 'r':
          string[int(c)] = unichr(int(13.0))
          c = c + 1.0
        elif json[int(i)] == 't':
          string[int(c)] = unichr(int(9.0))
          c = c + 1.0
        elif json[int(i)] == 'u':
          i = i + 1.0
          hex[int(0.0)] = charToUpperCase(json[int(i + 0.0)])
          hex[int(1.0)] = charToUpperCase(json[int(i + 1.0)])
          hex[int(2.0)] = charToUpperCase(json[int(i + 2.0)])
          hex[int(3.0)] = charToUpperCase(json[int(i + 3.0)])
          nCreateNumberFromStringWithCheck(hex, 16.0, hexReference, errorMessage)
          string[int(c)] = unichr(int(hexReference.numberValue))
          i = i + 3.0
          c = c + 1.0
      elif json[int(i)] == '\"':
        string[int(c)] = json[int(i)]
        c = c + 1.0
        done = True
      else:
        string[int(c)] = json[int(i)]
        c = c + 1.0
      i = i + 1.0
    

    tokenReference.string = string
    success = True
  else:
    lAddStringRef(errorMessages, CreateStringReference("End of string was not found."))
    success = False

  return success

def IsValidJSONString(jsonString, errorMessages):

  numberReference = NumberReference()
  stringLength = NumberReference()

  valid = IsValidJSONStringInJSON(jsonString, 0.0, numberReference, stringLength, errorMessages)

  return valid

def IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages):

  success = True
  done = False

  characterCount.numberValue = 1.0

  i = start + 1.0
  while i < len(json) and  not done  and success:
    if  not IsJSONIllegalControllCharacter(json[int(i)]) :
      if json[int(i)] == '\\':
        i = i + 1.0
        if i < len(json):
          if json[int(i)] == '\"' or json[int(i)] == '\\' or json[int(i)] == '/' or json[int(i)] == 'b' or json[int(i)] == 'f' or json[int(i)] == 'n' or json[int(i)] == 'r' or json[int(i)] == 't':
            characterCount.numberValue = characterCount.numberValue + 1.0
          elif json[int(i)] == 'u':
            if i + 4.0 < len(json):
              j = 0.0
              while j < 4.0 and success:
                c = json[int(i + j + 1.0)]
                if nCharacterIsNumberCharacterInBase(c, 16.0) or c == 'a' or c == 'b' or c == 'c' or c == 'd' or c == 'e' or c == 'f':
                  pass
                else:
                  success = False
                  lAddStringRef(errorMessages, CreateStringReference("\\u must be followed by four hexadecimal digits."))
                j = j + 1.0
              
              characterCount.numberValue = characterCount.numberValue + 1.0
              i = i + 4.0
            else:
              success = False
              lAddStringRef(errorMessages, CreateStringReference("\\u must be followed by four characters."))
          else:
            success = False
            lAddStringRef(errorMessages, CreateStringReference("Escaped character invalid."))
        else:
          success = False
          lAddStringRef(errorMessages, CreateStringReference("There must be at least two character after string escape."))
      elif json[int(i)] == '\"':
        characterCount.numberValue = characterCount.numberValue + 1.0
        done = True
      else:
        characterCount.numberValue = characterCount.numberValue + 1.0
    else:
      success = False
      lAddStringRef(errorMessages, CreateStringReference("Unicode code points 0-31 not allowed in JSON string."))
    i = i + 1.0
  

  if done:
    stringLengthReference.numberValue = i - start
  else:
    success = False
    lAddStringRef(errorMessages, CreateStringReference("String must end with \"."))

  return success

def IsJSONIllegalControllCharacter(c):

  cnr = ord(c)

  if cnr >= 0.0 and cnr < 32.0:
    isControll = True
  else:
    isControll = False

  return isControll

def AddElement(list, a):

  newlist =  [None]*int(len(list) + 1.0)

  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a

  list = None

  return newlist

def AddElementRef(list, i):
  list.array = AddElement(list.array, i)

def RemoveElement(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  i = 0.0
  while i < len(list):
    if i < n:
      newlist[int(i)] = list[int(i)]
    if i > n:
      newlist[int(i - 1.0)] = list[int(i)]
    i = i + 1.0
  

  list = None

  return newlist

def GetElementRef(list, i):
  return list.array[int(i)]

def RemoveElementRef(list, i):
  list.array = RemoveElement(list.array, i)

def CreateLinkedListElements():

  ll = LinkedListElements()
  ll.first = LinkedListNodeElements()
  ll.last = ll.first
  ll.last.end = True

  return ll

def LinkedListAddElement(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = LinkedListNodeElements()
  ll.last.next.end = True
  ll.last = ll.last.next

def LinkedListElementsToArray(ll):

  node = ll.first

  length = LinkedListElementsLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return array

def LinkedListElementsLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def FreeLinkedListElements(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def ComputeJSONStringLength(element):

  result = 0.0

  if StringsEqual(element.type, "object"):
    result = result + ComputeJSONObjectStringLength(element)
  elif StringsEqual(element.type, "string"):
    result = JSONEscapedStringLength(element.string) + 2.0
  elif StringsEqual(element.type, "array"):
    result = result + ComputeJSONArrayStringLength(element)
  elif StringsEqual(element.type, "number"):
    result = result + ComputeJSONNumberStringLength(element)
  elif StringsEqual(element.type, "null"):
    result = result + len("null")
  elif StringsEqual(element.type, "boolean"):
    result = result + ComputeJSONBooleanStringLength(element)

  return result

def ComputeJSONBooleanStringLength(element):

  if element.booleanValue:
    result = len("true")
  else:
    result = len("false")

  return result

def ComputeJSONNumberStringLength(element):

  if fabs(element.number) >= 10.0**15.0 or fabs(element.number) <= 10.0**( -15.0):
    a = nCreateStringScientificNotationDecimalFromNumber(element.number)
    length = len(a)
  else:
    a = nCreateStringDecimalFromNumber(element.number)
    length = len(a)

  return length

def ComputeJSONArrayStringLength(element):

  length = 1.0

  i = 0.0
  while i < len(element.array):
    arrayElement = element.array[int(i)]

    length = length + ComputeJSONStringLength(arrayElement)

    if i + 1.0 != len(element.array):
      length = length + 1.0
    i = i + 1.0
  

  length = length + 1.0

  return length

def ComputeJSONObjectStringLength(element):

  length = 1.0

  keys = GetStringElementMapKeySet(element.object)
  i = 0.0
  while i < len(keys.stringArray):
    key = keys.stringArray[int(i)].string
    objectElement = GetObjectValue(element.object, key)

    length = length + 1.0
    length = length + JSONEscapedStringLength(key)
    length = length + 1.0
    length = length + 1.0

    length = length + ComputeJSONStringLength(objectElement)

    if i + 1.0 != len(keys.stringArray):
      length = length + 1.0
    i = i + 1.0
  

  length = length + 1.0

  return length

def CreateStringElement(string):
  element = Element()
  element.type = "string"
  element.string = string
  return element

def CreateBooleanElement(booleanValue):
  element = Element()
  element.type = "boolean"
  element.booleanValue = booleanValue
  return element

def CreateNullElement():
  element = Element()
  element.type = "null"
  return element

def CreateNumberElement(number):
  element = Element()
  element.type = "number"
  element.number = number
  return element

def CreateArrayElement(length):
  element = Element()
  element.type = "array"
  element.array =  [None]*int(length)
  return element

def CreateObjectElement(length):
  element = Element()
  element.type = "object"
  element.object = StringElementMap()
  element.object.stringListRef = CreateStringArrayReferenceLengthValue(length, "")
  element.object.elementListRef = ElementArrayReference()
  element.object.elementListRef.array =  [None]*int(length)
  return element

def DeleteElement(element):
  if StringsEqual(element.type, "object"):
    DeleteObject(element)
  elif StringsEqual(element.type, "string"):
    element = None
  elif StringsEqual(element.type, "array"):
    DeleteArray(element)
  elif StringsEqual(element.type, "number"):
    element = None
  elif StringsEqual(element.type, "null"):
    element = None
  elif StringsEqual(element.type, "boolean"):
    element = None
  else:
    pass

def DeleteObject(element):

  keys = GetStringElementMapKeySet(element.object)
  i = 0.0
  while i < len(keys.stringArray):
    key = keys.stringArray[int(i)].string
    objectElement = GetObjectValue(element.object, key)
    DeleteElement(objectElement)
    i = i + 1.0
  

def DeleteArray(element):

  i = 0.0
  while i < len(element.array):
    arrayElement = element.array[int(i)]
    DeleteElement(arrayElement)
    i = i + 1.0
  

def WriteJSON(element):

  length = ComputeJSONStringLength(element)
  result =  [None]*int(length)
  index = CreateNumberReference(0.0)

  if StringsEqual(element.type, "object"):
    WriteObject(element, result, index)
  elif StringsEqual(element.type, "string"):
    WriteString(element, result, index)
  elif StringsEqual(element.type, "array"):
    WriteArray(element, result, index)
  elif StringsEqual(element.type, "number"):
    WriteNumber(element, result, index)
  elif StringsEqual(element.type, "null"):
    strWriteStringToStingStream(result, index, "null")
  elif StringsEqual(element.type, "boolean"):
    WriteBooleanValue(element, result, index)

  return result

def WriteBooleanValue(element, result, index):
  if element.booleanValue:
    strWriteStringToStingStream(result, index, "true")
  else:
    strWriteStringToStingStream(result, index, "false")

def WriteNumber(element, result, index):

  if fabs(element.number) >= 10.0**15.0 or fabs(element.number) <= 10.0**( -15.0):
    numberString = nCreateStringScientificNotationDecimalFromNumber(element.number)
  else:
    numberString = nCreateStringDecimalFromNumber(element.number)

  strWriteStringToStingStream(result, index, numberString)

def WriteArray(element, result, index):

  strWriteStringToStingStream(result, index, "[")

  i = 0.0
  while i < len(element.array):
    arrayElement = element.array[int(i)]

    s = WriteJSON(arrayElement)
    strWriteStringToStingStream(result, index, s)

    if i + 1.0 != len(element.array):
      strWriteStringToStingStream(result, index, ",")
    i = i + 1.0
  

  strWriteStringToStingStream(result, index, "]")

def WriteString(element, result, index):
  strWriteStringToStingStream(result, index, "\"")
  element.string = JSONEscapeString(element.string)
  strWriteStringToStingStream(result, index, element.string)
  strWriteStringToStingStream(result, index, "\"")

def JSONEscapeString(string):

  length = JSONEscapedStringLength(string)

  ns =  [None]*int(length)
  index = CreateNumberReference(0.0)
  lettersReference = CreateNumberReference(0.0)

  i = 0.0
  while i < len(string):
    if JSONMustBeEscaped(string[int(i)], lettersReference):
      escaped = JSONEscapeCharacter(string[int(i)])
      strWriteStringToStingStream(ns, index, escaped)
    else:
      strWriteCharacterToStingStream(ns, index, string[int(i)])
    i = i + 1.0
  

  return ns

def JSONEscapedStringLength(string):

  lettersReference = CreateNumberReference(0.0)
  length = 0.0

  i = 0.0
  while i < len(string):
    if JSONMustBeEscaped(string[int(i)], lettersReference):
      length = length + lettersReference.numberValue
    else:
      length = length + 1.0
    i = i + 1.0
  
  return length

def JSONEscapeCharacter(c):

  code = ord(c)

  q = 34.0
  rs = 92.0
  s = 47.0
  b = 8.0
  f = 12.0
  n = 10.0
  r = 13.0
  t = 9.0

  hexNumber = StringReference()

  if code == q:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = '\"'
  elif code == rs:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = '\\'
  elif code == s:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = '/'
  elif code == b:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = 'b'
  elif code == f:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = 'f'
  elif code == n:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = 'n'
  elif code == r:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = 'r'
  elif code == t:
    escaped =  [None]*int(2.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = 't'
  elif code >= 0.0 and code <= 31.0:
    escaped =  [None]*int(6.0)
    escaped[int(0.0)] = '\\'
    escaped[int(1.0)] = 'u'
    escaped[int(2.0)] = '0'
    escaped[int(3.0)] = '0'

    nCreateStringFromNumberWithCheck(code, 16.0, hexNumber)

    if len(hexNumber.string) == 1.0:
      escaped[int(4.0)] = '0'
      escaped[int(5.0)] = hexNumber.string[int(0.0)]
    elif len(hexNumber.string) == 2.0:
      escaped[int(4.0)] = hexNumber.string[int(0.0)]
      escaped[int(5.0)] = hexNumber.string[int(1.0)]
  else:
    escaped =  [None]*int(1.0)
    escaped[int(0.0)] = c

  return escaped

def JSONMustBeEscaped(c, letters):

  code = ord(c)
  mustBeEscaped = False

  q = 34.0
  rs = 92.0
  s = 47.0
  b = 8.0
  f = 12.0
  n = 10.0
  r = 13.0
  t = 9.0

  if code == q or code == rs or code == s or code == b or code == f or code == n or code == r or code == t:
    mustBeEscaped = True
    letters.numberValue = 2.0
  elif code >= 0.0 and code <= 31.0:
    mustBeEscaped = True
    letters.numberValue = 6.0
  elif code >= 2.0**16.0:
    mustBeEscaped = True
    letters.numberValue = 6.0

  return mustBeEscaped

def WriteObject(element, result, index):

  strWriteStringToStingStream(result, index, "{")

  keys = GetStringElementMapKeySet(element.object)
  i = 0.0
  while i < len(keys.stringArray):
    key = keys.stringArray[int(i)].string
    key = JSONEscapeString(key)
    objectElement = GetObjectValue(element.object, key)

    strWriteStringToStingStream(result, index, "\"")
    strWriteStringToStingStream(result, index, key)
    strWriteStringToStingStream(result, index, "\"")
    strWriteStringToStingStream(result, index, ":")

    s = WriteJSON(objectElement)
    strWriteStringToStingStream(result, index, s)

    if i + 1.0 != len(keys.stringArray):
      strWriteStringToStingStream(result, index, ",")
    i = i + 1.0
  

  strWriteStringToStingStream(result, index, "}")

def ReadJSON(string, elementReference, errorMessages):

  # Tokenize.
  tokenArrayReference = StringArrayReference()
  success = JSONTokenize(string, tokenArrayReference, errorMessages)

  if success:
    # Parse.
    success = GetJSONValue(tokenArrayReference.stringArray, elementReference, errorMessages)

  return success

def GetJSONValue(tokens, elementReference, errorMessages):

  i = CreateNumberReference(0.0)
  success = GetJSONValueRecursive(tokens, i, 0.0, elementReference, errorMessages)

  return success

def GetJSONValueRecursive(tokens, i, depth, elementReference, errorMessages):

  success = True
  token = tokens[int(i.numberValue)].string

  if StringsEqual(token, "{"):
    success = GetJSONObject(tokens, i, depth + 1.0, elementReference, errorMessages)
  elif StringsEqual(token, "["):
    success = GetJSONArray(tokens, i, depth + 1.0, elementReference, errorMessages)
  elif StringsEqual(token, "true"):
    elementReference.element = CreateBooleanElement(True)
    i.numberValue = i.numberValue + 1.0
  elif StringsEqual(token, "false"):
    elementReference.element = CreateBooleanElement(False)
    i.numberValue = i.numberValue + 1.0
  elif StringsEqual(token, "null"):
    elementReference.element = CreateNullElement()
    i.numberValue = i.numberValue + 1.0
  elif charIsNumber(token[int(0.0)]) or token[int(0.0)] == '-':
    stringToDecimalResult = nCreateNumberFromDecimalString(token)
    elementReference.element = CreateNumberElement(stringToDecimalResult)
    i.numberValue = i.numberValue + 1.0
  elif token[int(0.0)] == '\"':
    substr = strSubstring(token, 1.0, len(token) - 1.0)
    elementReference.element = CreateStringElement(substr)
    i.numberValue = i.numberValue + 1.0
  else:
    str = ""
    str = strConcatenateString(str, "Invalid token first in value: ")
    str = strAppendString(str, token)
    lAddStringRef(errorMessages, CreateStringReference(str))
    success = False

  if success and depth == 0.0:
    if StringsEqual(tokens[int(i.numberValue)].string, "<end>"):
      pass
    else:
      lAddStringRef(errorMessages, CreateStringReference("The outer value cannot have any tokens following it."))
      success = False

  return success

def GetJSONObject(tokens, i, depth, elementReference, errorMessages):

  keys = lCreateLinkedListString()
  values = CreateLinkedListElements()
  element = CreateObjectElement(0.0)
  valueReference = ElementReference()
  success = True
  i.numberValue = i.numberValue + 1.0

  if  not StringsEqual(tokens[int(i.numberValue)].string, "}") :
    done = False

    while  not done  and success:
      key = tokens[int(i.numberValue)].string

      if key[int(0.0)] == '\"':
        i.numberValue = i.numberValue + 1.0
        colon = tokens[int(i.numberValue)].string
        if StringsEqual(colon, ":"):
          i.numberValue = i.numberValue + 1.0
          success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages)

          if success:
            keystring = strSubstring(key, 1.0, len(key) - 1.0)
            value = valueReference.element
            lLinkedListAddString(keys, keystring)
            LinkedListAddElement(values, value)

            comma = tokens[int(i.numberValue)].string
            if StringsEqual(comma, ","):
              # OK
              i.numberValue = i.numberValue + 1.0
            else:
              done = True
        else:
          str = ""
          str = strConcatenateString(str, "Expected colon after key in object: ")
          str = strAppendString(str, colon)
          lAddStringRef(errorMessages, CreateStringReference(str))

          success = False
          done = True
      else:
        lAddStringRef(errorMessages, CreateStringReference("Expected string as key in object."))

        done = True
        success = False
    

  if success:
    closeCurly = tokens[int(i.numberValue)].string

    if StringsEqual(closeCurly, "}"):
      # OK
      element.object.stringListRef.stringArray = None
      element.object.elementListRef.array = None
      element.object.stringListRef.stringArray = lLinkedListStringsToArray(keys)
      element.object.elementListRef.array = LinkedListElementsToArray(values)
      elementReference.element = element
      i.numberValue = i.numberValue + 1.0
    else:
      lAddStringRef(errorMessages, CreateStringReference("Expected close curly brackets at end of object value."))
      success = False

  lFreeLinkedListString(keys)
  FreeLinkedListElements(values)
  valueReference = None

  return success

def GetJSONArray(tokens, i, depth, elementReference, errorMessages):

  elements = CreateLinkedListElements()
  i.numberValue = i.numberValue + 1.0

  valueReference = ElementReference()
  success = True
  element = CreateArrayElement(0.0)

  nextToken = tokens[int(i.numberValue)].string

  if  not StringsEqual(nextToken, "]") :
    done = False
    while  not done  and success:
      success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages)

      if success:
        value = valueReference.element
        LinkedListAddElement(elements, value)

        comma = tokens[int(i.numberValue)].string

        if StringsEqual(comma, ","):
          # OK
          i.numberValue = i.numberValue + 1.0
        else:
          done = True
    

  nextToken = tokens[int(i.numberValue)].string
  if StringsEqual(nextToken, "]"):
    # OK
    i.numberValue = i.numberValue + 1.0
    element.array = None
    element.array = LinkedListElementsToArray(elements)
  else:
    lAddStringRef(errorMessages, CreateStringReference("Expected close square bracket at end of array."))
    success = False

  elementReference.element = element
  FreeLinkedListElements(elements)
  valueReference = None

  return success

def GetStringElementMapKeySet(stringElementMap):
  return stringElementMap.stringListRef

def GetObjectValue(stringElementMap, key):

  result = Element()

  i = 0.0
  while i < GetStringElementMapNumberOfKeys(stringElementMap):
    if StringsEqual(stringElementMap.stringListRef.stringArray[int(i)].string, key):
      result = stringElementMap.elementListRef.array[int(i)]
    i = i + 1.0
  

  return result

def GetObjectValueWithCheck(stringElementMap, key, foundReference):

  result = Element()

  foundReference.booleanValue = False
  i = 0.0
  while i < GetStringElementMapNumberOfKeys(stringElementMap):
    if StringsEqual(stringElementMap.stringListRef.stringArray[int(i)].string, key):
      result = stringElementMap.elementListRef.array[int(i)]
      foundReference.booleanValue = True
    i = i + 1.0
  

  return result

def PutStringElementMap(stringElementMap, keystring, value):
  lAddStringRef(stringElementMap.stringListRef, CreateStringReference(keystring))
  AddElementRef(stringElementMap.elementListRef, value)

def SetStringElementMap(stringElementMap, index, keystring, value):
  stringElementMap.stringListRef.stringArray[int(index)].string = keystring
  stringElementMap.elementListRef.array[int(index)] = value

def GetStringElementMapNumberOfKeys(stringElementMap):
  return len(stringElementMap.stringListRef.stringArray)

def JSONCompare(a, b, epsilon, equal, errorMessage):

  eaRef = ElementReference()
  ebRef = ElementReference()

  success = ReadJSON(a, eaRef, errorMessage)

  if success:
    ea = eaRef.element

    success = ReadJSON(b, ebRef, errorMessage)

    if success:
      eb = ebRef.element

      equal.booleanValue = JSONCompareElements(ea, eb, epsilon)

      DeleteElement(eb)

    DeleteElement(ea)

  return success

def JSONCompareElements(ea, eb, epsilon):

  equal = StringsEqual(ea.type, eb.type)
        
  if equal:
    typeName = ea.type
    if StringsEqual(typeName, "object"):
      equal = JSONCompareObjects(ea.object, eb.object, epsilon)
    elif StringsEqual(typeName, "string"):
      equal = StringsEqual(ea.string, eb.string)
    elif StringsEqual(typeName, "array"):
      equal = JSONCompareArrays(ea.array, eb.array, epsilon)
    elif StringsEqual(typeName, "number"):
      equal = EpsilonCompare(ea.number, eb.number, epsilon)
    elif StringsEqual(typeName, "null"):
      equal = True
    elif StringsEqual(typeName, "boolean"):
      equal = ea.booleanValue == eb.booleanValue
        
  return equal

def JSONCompareArrays(ea, eb, epsilon):

  equals = len(ea) == len(eb)

  if equals:
    length = len(ea)
    i = 0.0
    while i < length and equals:
      equals = JSONCompareElements(ea[int(i)], eb[int(i)], epsilon)
      i = i + 1.0
    

  return equals

def JSONCompareObjects(ea, eb, epsilon):

  aFoundReference = BooleanReference()
  bFoundReference = BooleanReference()

  akeys = GetStringElementMapNumberOfKeys(ea)
  bkeys = GetStringElementMapNumberOfKeys(eb)

  equals = akeys == bkeys

  if equals:
    keys = GetStringElementMapKeySet(ea)

    i = 0.0
    while i < len(keys.stringArray) and equals:
      key = keys.stringArray[int(i)].string

      eaValue = GetObjectValueWithCheck(ea, key, aFoundReference)
      ebValue = GetObjectValueWithCheck(eb, key, bFoundReference)

      if aFoundReference.booleanValue and bFoundReference.booleanValue:
        equals = JSONCompareElements(eaValue, ebValue, epsilon)
      else:
        equals = False
      i = i + 1.0
    

  return equals

class DynamicArrayCharacters:
  array = None
  length = None

class LinkedListNodeStrings:
  end = None
  value = None
  next = None

class LinkedListStrings:
  first = None
  last = None

class LinkedListNodeNumbers:
  next = None
  end = None
  value = None

class LinkedListNumbers:
  first = None
  last = None

class LinkedListCharacters:
  first = None
  last = None

class LinkedListNodeCharacters:
  end = None
  value = None
  next = None

class DynamicArrayNumbers:
  array = None
  length = None

def AddNumber(list, a):

  newlist =  [None]*int(len(list) + 1.0)
  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def AddNumberRef(list, i):
  list.numberArray = AddNumber(list.numberArray, i)

def RemoveNumber(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None
		
  return newlist

def GetNumberRef(list, i):
  return list.numberArray[int(i)]

def RemoveNumberRef(list, i):
  list.numberArray = RemoveNumber(list.numberArray, i)

def AddString(list, a):

  newlist =  [None]*int(len(list) + 1.0)

  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def AddStringRef(list, i):
  list.stringArray = AddString(list.stringArray, i)

def RemoveString(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None
		
  return newlist

def GetStringRef(list, i):
  return list.stringArray[int(i)]

def RemoveStringRef(list, i):
  list.stringArray = RemoveString(list.stringArray, i)

def CreateDynamicArrayCharacters():

  da = DynamicArrayCharacters()
  da.array =  [None]*int(10.0)
  da.length = 0.0

  return da

def CreateDynamicArrayCharactersWithInitialCapacity(capacity):

  da = DynamicArrayCharacters()
  da.array =  [None]*int(capacity)
  da.length = 0.0

  return da

def DynamicArrayAddCharacter(da, value):
  if da.length == len(da.array):
    DynamicArrayCharactersIncreaseSize(da)

  da.array[int(da.length)] = value
  da.length = da.length + 1.0

def DynamicArrayCharactersIncreaseSize(da):

  newLength = round(len(da.array)*3.0/2.0)
  newArray =  [None]*int(newLength)

  i = 0.0
  while i < len(da.array):
    newArray[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  da.array = None

  da.array = newArray

def DynamicArrayCharactersDecreaseSizeNecessary(da):

  needsDecrease = False

  if da.length > 10.0:
    needsDecrease = da.length <= round(len(da.array)*2.0/3.0)

  return needsDecrease

def DynamicArrayCharactersDecreaseSize(da):

  newLength = round(len(da.array)*2.0/3.0)
  newArray =  [None]*int(newLength)

  i = 0.0
  while i < newLength:
    newArray[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  da.array = None

  da.array = newArray

def DynamicArrayCharactersIndex(da, index):
  return da.array[int(index)]

def DynamicArrayCharactersLength(da):
  return da.length

def DynamicArrayInsertCharacter(da, index, value):

  if da.length == len(da.array):
    DynamicArrayCharactersIncreaseSize(da)

  i = da.length
  while i > index:
    da.array[int(i)] = da.array[int(i - 1.0)]
    i = i - 1.0
  

  da.array[int(index)] = value

  da.length = da.length + 1.0

def DynamicArrayCharacterSet(da, index, value):

  if index < da.length:
    da.array[int(index)] = value
    success = True
  else:
    success = False

  return success

def DynamicArrayRemoveCharacter(da, index):

  i = index
  while i < da.length - 1.0:
    da.array[int(i)] = da.array[int(i + 1.0)]
    i = i + 1.0
  

  da.length = da.length - 1.0

  if DynamicArrayCharactersDecreaseSizeNecessary(da):
    DynamicArrayCharactersDecreaseSize(da)

def FreeDynamicArrayCharacters(da):
  da.array = None
  da = None

def DynamicArrayCharactersToArray(da):

  array =  [None]*int(da.length)

  i = 0.0
  while i < da.length:
    array[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  return array

def ArrayToDynamicArrayCharactersWithOptimalSize(array):

  c = len(array)
  n = (log(c) - 1.0)/log(3.0/2.0)
  newCapacity = floor(n) + 1.0

  da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity)

  i = 0.0
  while i < len(array):
    da.array[int(i)] = array[int(i)]
    i = i + 1.0
  

  return da

def ArrayToDynamicArrayCharacters(array):

  da = DynamicArrayCharacters()
  da.array = CopyString(array)
  da.length = len(array)

  return da

def DynamicArrayCharactersEqual(a, b):

  equal = True
  if a.length == b.length:
    i = 0.0
    while i < a.length and equal:
      if a.array[int(i)] != b.array[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def DynamicArrayCharactersToLinkedList(da):

  ll = CreateLinkedListCharacter()

  i = 0.0
  while i < da.length:
    LinkedListAddCharacter(ll, da.array[int(i)])
    i = i + 1.0
  

  return ll

def LinkedListToDynamicArrayCharacters(ll):

  node = ll.first

  da = DynamicArrayCharacters()
  da.length = LinkedListCharactersLength(ll)

  da.array =  [None]*int(da.length)

  i = 0.0
  while i < da.length:
    da.array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return da

def AddBoolean(list, a):

  newlist =  [None]*int(len(list) + 1.0)
  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def AddBooleanRef(list, i):
  list.booleanArray = AddBoolean(list.booleanArray, i)

def RemoveBoolean(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None
		
  return newlist

def GetBooleanRef(list, i):
  return list.booleanArray[int(i)]

def RemoveDecimalRef(list, i):
  list.booleanArray = RemoveBoolean(list.booleanArray, i)

def CreateLinkedListString():

  ll = LinkedListStrings()
  ll.first = LinkedListNodeStrings()
  ll.last = ll.first
  ll.last.end = True

  return ll

def LinkedListAddString(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = LinkedListNodeStrings()
  ll.last.next.end = True
  ll.last = ll.last.next

def LinkedListStringsToArray(ll):

  node = ll.first

  length = LinkedListStringsLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = StringReference()
    array[int(i)].string = node.value
    node = node.next
    i = i + 1.0
  

  return array

def LinkedListStringsLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def FreeLinkedListString(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def CreateLinkedListNumbers():

  ll = LinkedListNumbers()
  ll.first = LinkedListNodeNumbers()
  ll.last = ll.first
  ll.last.end = True

  return ll

def CreateLinkedListNumbersArray(length):

  lls =  [None]*int(length)
  i = 0.0
  while i < len(lls):
    lls[int(i)] = CreateLinkedListNumbers()
    i = i + 1.0
  

  return lls

def LinkedListAddNumber(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = LinkedListNodeNumbers()
  ll.last.next.end = True
  ll.last = ll.last.next

def LinkedListNumbersLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def LinkedListNumbersIndex(ll, index):

  node = ll.first
  i = 0.0
  while i < index:
    node = node.next
    i = i + 1.0
  

  return node.value

def LinkedListInsertNumber(ll, index, value):

  if index == 0.0:
    tmp = ll.first
    ll.first = LinkedListNodeNumbers()
    ll.first.next = tmp
    ll.first.value = value
    ll.first.end = False
  else:
    node = ll.first
    i = 0.0
    while i < index - 1.0:
      node = node.next
      i = i + 1.0
    

    tmp = node.next
    node.next = LinkedListNodeNumbers()
    node.next.next = tmp
    node.next.value = value
    node.next.end = False

def LinkedListSet(ll, index, value):

  node = ll.first
  i = 0.0
  while i < index:
    node = node.next
    i = i + 1.0
  

  node.next.value = value

def LinkedListRemoveNumber(ll, index):

  node = ll.first
  prev = ll.first

  i = 0.0
  while i < index:
    prev = node
    node = node.next
    i = i + 1.0
  

  if index == 0.0:
    ll.first = prev.next
  if  not prev.next.end :
    prev.next = prev.next.next

def FreeLinkedListNumbers(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def FreeLinkedListNumbersArray(lls):

  i = 0.0
  while i < len(lls):
    FreeLinkedListNumbers(lls[int(i)])
    i = i + 1.0
  
  lls = None

def LinkedListNumbersToArray(ll):

  node = ll.first

  length = LinkedListNumbersLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return array

def ArrayToLinkedListNumbers(array):

  ll = CreateLinkedListNumbers()

  i = 0.0
  while i < len(array):
    LinkedListAddNumber(ll, array[int(i)])
    i = i + 1.0
  

  return ll

def LinkedListNumbersEqual(a, b):

  an = a.first
  bn = b.first

  equal = True
  done = False
  while equal and  not done :
    if an.end == bn.end:
      if an.end:
        done = True
      elif an.value == bn.value:
        an = an.next
        bn = bn.next
      else:
        equal = False
    else:
      equal = False
  

  return equal

def CreateLinkedListCharacter():

  ll = LinkedListCharacters()
  ll.first = LinkedListNodeCharacters()
  ll.last = ll.first
  ll.last.end = True

  return ll

def LinkedListAddCharacter(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = LinkedListNodeCharacters()
  ll.last.next.end = True
  ll.last = ll.last.next

def LinkedListCharactersToArray(ll):

  node = ll.first

  length = LinkedListCharactersLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return array

def LinkedListCharactersLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def FreeLinkedListCharacter(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def LinkedListCharactersAddString(ll, str):

  i = 0.0
  while i < len(str):
    LinkedListAddCharacter(ll, str[int(i)])
    i = i + 1.0
  

def CreateDynamicArrayNumbers():

  da = DynamicArrayNumbers()
  da.array =  [None]*int(10.0)
  da.length = 0.0

  return da

def CreateDynamicArrayNumbersWithInitialCapacity(capacity):

  da = DynamicArrayNumbers()
  da.array =  [None]*int(capacity)
  da.length = 0.0

  return da

def DynamicArrayAddNumber(da, value):
  if da.length == len(da.array):
    DynamicArrayNumbersIncreaseSize(da)

  da.array[int(da.length)] = value
  da.length = da.length + 1.0

def DynamicArrayNumbersIncreaseSize(da):

  newLength = round(len(da.array)*3.0/2.0)
  newArray =  [None]*int(newLength)

  i = 0.0
  while i < len(da.array):
    newArray[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  da.array = None

  da.array = newArray

def DynamicArrayNumbersDecreaseSizeNecessary(da):

  needsDecrease = False

  if da.length > 10.0:
    needsDecrease = da.length <= round(len(da.array)*2.0/3.0)

  return needsDecrease

def DynamicArrayNumbersDecreaseSize(da):

  newLength = round(len(da.array)*2.0/3.0)
  newArray =  [None]*int(newLength)

  i = 0.0
  while i < newLength:
    newArray[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  da.array = None

  da.array = newArray

def DynamicArrayNumbersIndex(da, index):
  return da.array[int(index)]

def DynamicArrayNumbersLength(da):
  return da.length

def DynamicArrayInsertNumber(da, index, value):

  if da.length == len(da.array):
    DynamicArrayNumbersIncreaseSize(da)

  i = da.length
  while i > index:
    da.array[int(i)] = da.array[int(i - 1.0)]
    i = i - 1.0
  

  da.array[int(index)] = value

  da.length = da.length + 1.0

def DynamicArrayNumberSet(da, index, value):

  if index < da.length:
    da.array[int(index)] = value
    success = True
  else:
    success = False

  return success

def DynamicArrayRemoveNumber(da, index):

  i = index
  while i < da.length - 1.0:
    da.array[int(i)] = da.array[int(i + 1.0)]
    i = i + 1.0
  

  da.length = da.length - 1.0

  if DynamicArrayNumbersDecreaseSizeNecessary(da):
    DynamicArrayNumbersDecreaseSize(da)

def FreeDynamicArrayNumbers(da):
  da.array = None
  da = None

def DynamicArrayNumbersToArray(da):

  array =  [None]*int(da.length)

  i = 0.0
  while i < da.length:
    array[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  return array

def ArrayToDynamicArrayNumbersWithOptimalSize(array):

  #
  #         c = 10*(3/2)^n
  #         log(c) = log(10*(3/2)^n)
  #         log(c) = log(10) + log((3/2)^n)
  #         log(c) = 1 + log((3/2)^n)
  #         log(c) - 1 = log((3/2)^n)
  #         log(c) - 1 = n*log(3/2)
  #         n = (log(c) - 1)/log(3/2)
  #        
  c = len(array)
  n = (log(c) - 1.0)/log(3.0/2.0)
  newCapacity = floor(n) + 1.0

  da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity)

  i = 0.0
  while i < len(array):
    da.array[int(i)] = array[int(i)]
    i = i + 1.0
  

  return da

def ArrayToDynamicArrayNumbers(array):

  da = DynamicArrayNumbers()
  da.array = CopyNumberArray(array)
  da.length = len(array)

  return da

def DynamicArrayNumbersEqual(a, b):

  equal = True
  if a.length == b.length:
    i = 0.0
    while i < a.length and equal:
      if a.array[int(i)] != b.array[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def DynamicArrayNumbersToLinkedList(da):

  ll = CreateLinkedListNumbers()

  i = 0.0
  while i < da.length:
    LinkedListAddNumber(ll, da.array[int(i)])
    i = i + 1.0
  

  return ll

def LinkedListToDynamicArrayNumbers(ll):

  node = ll.first

  da = DynamicArrayNumbers()
  da.length = LinkedListNumbersLength(ll)

  da.array =  [None]*int(da.length)

  i = 0.0
  while i < da.length:
    da.array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return da

def AddCharacter(list, a):

  newlist =  [None]*int(len(list) + 1.0)
  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def AddCharacterRef(list, i):
  list.string = AddCharacter(list.string, i)

def RemoveCharacter(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None

  return newlist

def GetCharacterRef(list, i):
  return list.string[int(i)]

def RemoveCharacterRef(list, i):
  list.string = RemoveCharacter(list.string, i)

def sWriteStringToStingStream(stream, index, src):

  i = 0.0
  while i < len(src):
    stream[int(index.numberValue + i)] = src[int(i)]
    i = i + 1.0
  
  index.numberValue = index.numberValue + len(src)

def sWriteCharacterToStingStream(stream, index, src):
  stream[int(index.numberValue)] = src
  index.numberValue = index.numberValue + 1.0

def sWriteBooleanToStingStream(stream, index, src):
  if src:
    sWriteStringToStingStream(stream, index, "true")
  else:
    sWriteStringToStingStream(stream, index, "false")

def sSubstringWithCheck(string, fromx, to, stringReference):

  if fromx >= 0.0 and fromx <= len(string) and to >= 0.0 and to <= len(string) and fromx <= to:
    stringReference.string = sSubstring(string, fromx, to)
    success = True
  else:
    success = False

  return success

def sSubstring(string, fromx, to):

  length = to - fromx

  n =  [None]*int(length)

  i = fromx
  while i < to:
    n[int(i - fromx)] = string[int(i)]
    i = i + 1.0
  

  return n

def sAppendString(s1, s2):

  newString = sConcatenateString(s1, s2)

  s1 = None

  return newString

def sConcatenateString(s1, s2):

  newString =  [None]*int(len(s1) + len(s2))

  i = 0.0
  while i < len(s1):
    newString[int(i)] = s1[int(i)]
    i = i + 1.0
  

  i = 0.0
  while i < len(s2):
    newString[int(len(s1) + i)] = s2[int(i)]
    i = i + 1.0
  

  return newString

def sAppendCharacter(string, c):

  newString = sConcatenateCharacter(string, c)

  string = None

  return newString

def sConcatenateCharacter(string, c):
  newString =  [None]*int(len(string) + 1.0)

  i = 0.0
  while i < len(string):
    newString[int(i)] = string[int(i)]
    i = i + 1.0
  

  newString[int(len(string))] = c

  return newString

def sSplitByCharacter(toSplit, splitBy):

  stringToSplitBy =  [None]*int(1.0)
  stringToSplitBy[int(0.0)] = splitBy

  split = sSplitByString(toSplit, stringToSplitBy)

  stringToSplitBy = None

  return split

def sIndexOfCharacter(string, character, indexReference):

  found = False
  i = 0.0
  while i < len(string) and  not found :
    if string[int(i)] == character:
      found = True
      indexReference.numberValue = i
    i = i + 1.0
  

  return found

def sSubstringEqualsWithCheck(string, fromx, substring, equalsReference):

  if fromx < len(string):
    success = True
    equalsReference.booleanValue = sSubstringEquals(string, fromx, substring)
  else:
    success = False

  return success

def sSubstringEquals(string, fromx, substring):

  equal = True
  if len(string) - fromx >= len(substring):
    i = 0.0
    while i < len(substring) and equal:
      if string[int(fromx + i)] != substring[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def sIndexOfString(string, substring, indexReference):

  found = False
  i = 0.0
  while i < len(string) - len(substring) + 1.0 and  not found :
    if sSubstringEquals(string, i, substring):
      found = True
      indexReference.numberValue = i
    i = i + 1.0
  

  return found

def sContainsCharacter(string, character):

  found = False
  i = 0.0
  while i < len(string) and  not found :
    if string[int(i)] == character:
      found = True
    i = i + 1.0
  

  return found

def sContainsString(string, substring):
  return sIndexOfString(string, substring, NumberReference())

def sToUpperCase(string):

  i = 0.0
  while i < len(string):
    string[int(i)] = charToUpperCase(string[int(i)])
    i = i + 1.0
  

def sToLowerCase(string):

  i = 0.0
  while i < len(string):
    string[int(i)] = charToLowerCase(string[int(i)])
    i = i + 1.0
  

def sEqualsIgnoreCase(a, b):

  if len(a) == len(b):
    equal = True
    i = 0.0
    while i < len(a) and equal:
      if charToLowerCase(a[int(i)]) != charToLowerCase(b[int(i)]):
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def sReplaceString(string, toReplace, replaceWith):

  da = CreateDynamicArrayCharacters()

  equalsReference = BooleanReference()

  i = 0.0
  while i < len(string):
    success = sSubstringEqualsWithCheck(string, i, toReplace, equalsReference)
    if success:
      success = equalsReference.booleanValue

    if success and len(toReplace) > 0.0:
      j = 0.0
      while j < len(replaceWith):
        DynamicArrayAddCharacter(da, replaceWith[int(j)])
        j = j + 1.0
      
      i = i + len(toReplace)
    else:
      DynamicArrayAddCharacter(da, string[int(i)])
      i = i + 1.0
  

  result = DynamicArrayCharactersToArray(da)

  FreeDynamicArrayCharacters(da)

  return result

def sReplaceCharacterToNew(string, toReplace, replaceWith):

  result =  [None]*int(len(string))

  i = 0.0
  while i < len(string):
    if string[int(i)] == toReplace:
      result[int(i)] = replaceWith
    else:
      result[int(i)] = string[int(i)]
    i = i + 1.0
  

  return result

def sReplaceCharacter(string, toReplace, replaceWith):

  i = 0.0
  while i < len(string):
    if string[int(i)] == toReplace:
      string[int(i)] = replaceWith
    i = i + 1.0
  

def sTrim(string):

  # Find whitepaces at the start.
  lastWhitespaceLocationStart =  -1.0
  firstNonWhitespaceFound = False
  i = 0.0
  while i < len(string) and  not firstNonWhitespaceFound :
    if charIsWhiteSpace(string[int(i)]):
      lastWhitespaceLocationStart = i
    else:
      firstNonWhitespaceFound = True
    i = i + 1.0
  

  # Find whitepaces at the end.
  lastWhitespaceLocationEnd = len(string)
  firstNonWhitespaceFound = False
  i = len(string) - 1.0
  while i >= 0.0 and  not firstNonWhitespaceFound :
    if charIsWhiteSpace(string[int(i)]):
      lastWhitespaceLocationEnd = i
    else:
      firstNonWhitespaceFound = True
    i = i - 1.0
  

  if lastWhitespaceLocationStart < lastWhitespaceLocationEnd:
    result = sSubstring(string, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd)
  else:
    result =  [None]*int(0.0)

  return result

def sStartsWith(string, start):

  startsWithString = False
  if len(string) >= len(start):
    startsWithString = sSubstringEquals(string, 0.0, start)

  return startsWithString

def sEndsWith(string, end):

  endsWithString = False
  if len(string) >= len(end):
    endsWithString = sSubstringEquals(string, len(string) - len(end), end)

  return endsWithString

def sSplitByString(toSplit, splitBy):

  split =  [None]*int(0.0)

  next =  [None]*int(0.0)
  i = 0.0
  while i < len(toSplit):
    c = toSplit[int(i)]

    if sSubstringEquals(toSplit, i, splitBy):
      n = StringReference()
      n.string = next
      split = AddString(split, n)
      next =  [None]*int(0.0)
      i = i + len(splitBy)
    else:
      next = sAppendCharacter(next, c)
      i = i + 1.0
  

  n = StringReference()
  n.string = next
  split = AddString(split, n)

  return split

def sStringIsBefore(a, b):

  before = False
  equal = True
  done = False

  if len(a) == 0.0 and len(b) > 0.0:
    before = True
  else:
    i = 0.0
    while i < len(a) and i < len(b) and  not done :
      if a[int(i)] != b[int(i)]:
        equal = False
      if charCharacterIsBefore(a[int(i)], b[int(i)]):
        before = True
      if charCharacterIsBefore(b[int(i)], a[int(i)]):
        done = True
      i = i + 1.0
    

    if equal:
      if len(a) < len(b):
        before = True

  return before

def strWriteStringToStingStream(stream, index, src):

  i = 0.0
  while i < len(src):
    stream[int(index.numberValue + i)] = src[int(i)]
    i = i + 1.0
  
  index.numberValue = index.numberValue + len(src)

def strWriteCharacterToStingStream(stream, index, src):
  stream[int(index.numberValue)] = src
  index.numberValue = index.numberValue + 1.0

def strWriteBooleanToStingStream(stream, index, src):
  if src:
    strWriteStringToStingStream(stream, index, "true")
  else:
    strWriteStringToStingStream(stream, index, "false")

def strSubstringWithCheck(string, fromx, to, stringReference):

  if fromx >= 0.0 and fromx <= len(string) and to >= 0.0 and to <= len(string) and fromx <= to:
    stringReference.string = strSubstring(string, fromx, to)
    success = True
  else:
    success = False

  return success

def strSubstring(string, fromx, to):

  length = to - fromx

  n =  [None]*int(length)

  i = fromx
  while i < to:
    n[int(i - fromx)] = string[int(i)]
    i = i + 1.0
  

  return n

def strAppendString(s1, s2):

  newString = strConcatenateString(s1, s2)

  s1 = None

  return newString

def strConcatenateString(s1, s2):

  newString =  [None]*int(len(s1) + len(s2))

  i = 0.0
  while i < len(s1):
    newString[int(i)] = s1[int(i)]
    i = i + 1.0
  

  i = 0.0
  while i < len(s2):
    newString[int(len(s1) + i)] = s2[int(i)]
    i = i + 1.0
  

  return newString

def strAppendCharacter(string, c):

  newString = strConcatenateCharacter(string, c)

  string = None

  return newString

def strConcatenateCharacter(string, c):
  newString =  [None]*int(len(string) + 1.0)

  i = 0.0
  while i < len(string):
    newString[int(i)] = string[int(i)]
    i = i + 1.0
  

  newString[int(len(string))] = c

  return newString

def strSplitByCharacter(toSplit, splitBy):

  stringToSplitBy =  [None]*int(1.0)
  stringToSplitBy[int(0.0)] = splitBy

  split = strSplitByString(toSplit, stringToSplitBy)

  stringToSplitBy = None

  return split

def strIndexOfCharacter(string, character, indexReference):

  found = False
  i = 0.0
  while i < len(string) and  not found :
    if string[int(i)] == character:
      found = True
      indexReference.numberValue = i
    i = i + 1.0
  

  return found

def strSubstringEqualsWithCheck(string, fromx, substring, equalsReference):

  if fromx < len(string):
    success = True
    equalsReference.booleanValue = strSubstringEquals(string, fromx, substring)
  else:
    success = False

  return success

def strSubstringEquals(string, fromx, substring):

  equal = True
  i = 0.0
  while i < len(substring) and equal:
    if string[int(fromx + i)] != substring[int(i)]:
      equal = False
    i = i + 1.0
  

  return equal

def strIndexOfString(string, substring, indexReference):

  found = False
  i = 0.0
  while i < len(string) - len(substring) + 1.0 and  not found :
    if strSubstringEquals(string, i, substring):
      found = True
      indexReference.numberValue = i
    i = i + 1.0
  

  return found

def strContainsCharacter(string, character):

  found = False
  i = 0.0
  while i < len(string) and  not found :
    if string[int(i)] == character:
      found = True
    i = i + 1.0
  

  return found

def strContainsString(string, substring):
  return strIndexOfString(string, substring, NumberReference())

def strToUpperCase(string):

  i = 0.0
  while i < len(string):
    string[int(i)] = charToUpperCase(string[int(i)])
    i = i + 1.0
  

def strToLowerCase(string):

  i = 0.0
  while i < len(string):
    string[int(i)] = charToLowerCase(string[int(i)])
    i = i + 1.0
  

def strEqualsIgnoreCase(a, b):

  if len(a) == len(b):
    equal = True
    i = 0.0
    while i < len(a) and equal:
      if charToLowerCase(a[int(i)]) != charToLowerCase(b[int(i)]):
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def strReplaceString(string, toReplace, replaceWith):

  equalsReference = BooleanReference()
  result =  [None]*int(0.0)

  i = 0.0
  while i < len(string):
    success = strSubstringEqualsWithCheck(string, i, toReplace, equalsReference)
    if success:
      success = equalsReference.booleanValue

    if success and len(toReplace) > 0.0:
      result = strConcatenateString(result, replaceWith)
      i = i + len(toReplace)
    else:
      result = strConcatenateCharacter(result, string[int(i)])
      i = i + 1.0
  

  return result

def strReplaceCharacter(string, toReplace, replaceWith):

  result =  [None]*int(0.0)

  i = 0.0
  while i < len(string):
    if string[int(i)] == toReplace:
      result = strConcatenateCharacter(result, replaceWith)
    else:
      result = strConcatenateCharacter(result, string[int(i)])
    i = i + 1.0
  

  return result

def strTrim(string):

  # Find whitepaces at the start.
  lastWhitespaceLocationStart =  -1.0
  firstNonWhitespaceFound = False
  i = 0.0
  while i < len(string) and  not firstNonWhitespaceFound :
    if charIsWhiteSpace(string[int(i)]):
      lastWhitespaceLocationStart = i
    else:
      firstNonWhitespaceFound = True
    i = i + 1.0
  

  # Find whitepaces at the end.
  lastWhitespaceLocationEnd = len(string)
  firstNonWhitespaceFound = False
  i = len(string) - 1.0
  while i >= 0.0 and  not firstNonWhitespaceFound :
    if charIsWhiteSpace(string[int(i)]):
      lastWhitespaceLocationEnd = i
    else:
      firstNonWhitespaceFound = True
    i = i - 1.0
  

  if lastWhitespaceLocationStart < lastWhitespaceLocationEnd:
    result = strSubstring(string, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd)
  else:
    result =  [None]*int(0.0)

  return result

def strStartsWith(string, start):

  startsWithString = False
  if len(string) >= len(start):
    startsWithString = strSubstringEquals(string, 0.0, start)

  return startsWithString

def strEndsWith(string, end):

  endsWithString = False
  if len(string) >= len(end):
    endsWithString = strSubstringEquals(string, len(string) - len(end), end)

  return endsWithString

def strSplitByString(toSplit, splitBy):

  split =  [None]*int(0.0)

  next =  [None]*int(0.0)
  i = 0.0
  while i < len(toSplit):
    c = toSplit[int(i)]

    if strSubstringEquals(toSplit, i, splitBy):
      if len(split) != 0.0 or i != 0.0:
        n = StringReference()
        n.string = next
        split = lAddString(split, n)
        next =  [None]*int(0.0)
        i = i + len(splitBy)
    else:
      next = strAppendCharacter(next, c)
      i = i + 1.0
  

  if len(next) > 0.0:
    n = StringReference()
    n.string = next
    split = lAddString(split, n)

  return split

def strStringIsBefore(a, b):

  before = False
  equal = True
  done = False

  if len(a) == 0.0 and len(b) > 0.0:
    before = True
  else:
    i = 0.0
    while i < len(a) and i < len(b) and  not done :
      if a[int(i)] != b[int(i)]:
        equal = False
      if charCharacterIsBefore(a[int(i)], b[int(i)]):
        before = True
      if charCharacterIsBefore(b[int(i)], a[int(i)]):
        done = True
      i = i + 1.0
    

    if equal:
      if len(a) < len(b):
        before = True

  return before

def StringToNumberArray(string):

  array =  [None]*int(len(string))

  i = 0.0
  while i < len(string):
    array[int(i)] = ord(string[int(i)])
    i = i + 1.0
  
  return array

def NumberArrayToString(array):

  string =  [None]*int(len(array))

  i = 0.0
  while i < len(array):
    string[int(i)] = unichr(int(array[int(i)]))
    i = i + 1.0
  
  return string

def NumberArraysEqual(a, b):

  equal = True
  if len(a) == len(b):
    i = 0.0
    while i < len(a) and equal:
      if a[int(i)] != b[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def BooleanArraysEqual(a, b):

  equal = True
  if len(a) == len(b):
    i = 0.0
    while i < len(a) and equal:
      if a[int(i)] != b[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def StringsEqual(a, b):

  equal = True
  if len(a) == len(b):
    i = 0.0
    while i < len(a) and equal:
      if a[int(i)] != b[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def FillNumberArray(a, value):

  i = 0.0
  while i < len(a):
    a[int(i)] = value
    i = i + 1.0
  

def FillString(a, value):

  i = 0.0
  while i < len(a):
    a[int(i)] = value
    i = i + 1.0
  

def FillBooleanArray(a, value):

  i = 0.0
  while i < len(a):
    a[int(i)] = value
    i = i + 1.0
  

def FillNumberArrayRange(a, value, fromx, to):

  if fromx >= 0.0 and fromx <= len(a) and to >= 0.0 and to <= len(a) and fromx <= to:
    length = to - fromx
    i = 0.0
    while i < length:
      a[int(fromx + i)] = value
      i = i + 1.0
    

    success = True
  else:
    success = False

  return success

def FillBooleanArrayRange(a, value, fromx, to):

  if fromx >= 0.0 and fromx <= len(a) and to >= 0.0 and to <= len(a) and fromx <= to:
    length = to - fromx
    i = 0.0
    while i < length:
      a[int(fromx + i)] = value
      i = i + 1.0
    

    success = True
  else:
    success = False

  return success

def FillStringRange(a, value, fromx, to):

  if fromx >= 0.0 and fromx <= len(a) and to >= 0.0 and to <= len(a) and fromx <= to:
    length = to - fromx
    i = 0.0
    while i < length:
      a[int(fromx + i)] = value
      i = i + 1.0
    

    success = True
  else:
    success = False

  return success

def CopyNumberArray(a):

  n =  [None]*int(len(a))

  i = 0.0
  while i < len(a):
    n[int(i)] = a[int(i)]
    i = i + 1.0
  

  return n

def CopyBooleanArray(a):

  n =  [None]*int(len(a))

  i = 0.0
  while i < len(a):
    n[int(i)] = a[int(i)]
    i = i + 1.0
  

  return n

def CopyString(a):

  n =  [None]*int(len(a))

  i = 0.0
  while i < len(a):
    n[int(i)] = a[int(i)]
    i = i + 1.0
  

  return n

def CopyNumberArrayRange(a, fromx, to, copyReference):

  if fromx >= 0.0 and fromx <= len(a) and to >= 0.0 and to <= len(a) and fromx <= to:
    length = to - fromx
    n =  [None]*int(length)

    i = 0.0
    while i < length:
      n[int(i)] = a[int(fromx + i)]
      i = i + 1.0
    

    copyReference.numberArray = n
    success = True
  else:
    success = False

  return success

def CopyBooleanArrayRange(a, fromx, to, copyReference):

  if fromx >= 0.0 and fromx <= len(a) and to >= 0.0 and to <= len(a) and fromx <= to:
    length = to - fromx
    n =  [None]*int(length)

    i = 0.0
    while i < length:
      n[int(i)] = a[int(fromx + i)]
      i = i + 1.0
    

    copyReference.booleanArray = n
    success = True
  else:
    success = False

  return success

def CopyStringRange(a, fromx, to, copyReference):

  if fromx >= 0.0 and fromx <= len(a) and to >= 0.0 and to <= len(a) and fromx <= to:
    length = to - fromx
    n =  [None]*int(length)

    i = 0.0
    while i < length:
      n[int(i)] = a[int(fromx + i)]
      i = i + 1.0
    

    copyReference.string = n
    success = True
  else:
    success = False

  return success

def IsLastElement(length, index):
  return index + 1.0 == length

def CreateNumberArray(length, value):

  array =  [None]*int(length)
  FillNumberArray(array, value)

  return array

def CreateBooleanArray(length, value):

  array =  [None]*int(length)
  FillBooleanArray(array, value)

  return array

def CreateString(length, value):

  array =  [None]*int(length)
  FillString(array, value)

  return array

def SwapElementsOfNumberArray(A, ai, bi):

  tmp = A[int(ai)]
  A[int(ai)] = A[int(bi)]
  A[int(bi)] = tmp

def SwapElementsOfStringArray(A, ai, bi):

  tmp = A.stringArray[int(ai)]
  A.stringArray[int(ai)] = A.stringArray[int(bi)]
  A.stringArray[int(bi)] = tmp

def ReverseNumberArray(array):

  i = 0.0
  while i < len(array)/2.0:
    SwapElementsOfNumberArray(array, i, len(array) - i - 1.0)
    i = i + 1.0
  

def AssertFalse(b, failures):
  if b:
    failures.numberValue = failures.numberValue + 1.0

def AssertTrue(b, failures):
  if  not b :
    failures.numberValue = failures.numberValue + 1.0

def AssertEquals(a, b, failures):
  if a != b:
    failures.numberValue = failures.numberValue + 1.0

def AssertBooleansEqual(a, b, failures):
  if a != b:
    failures.numberValue = failures.numberValue + 1.0

def AssertCharactersEqual(a, b, failures):
  if a != b:
    failures.numberValue = failures.numberValue + 1.0

def AssertStringEquals(a, b, failures):
  if  not StringsEqual(a, b) :
    failures.numberValue = failures.numberValue + 1.0

def AssertNumberArraysEqual(a, b, failures):

  if len(a) == len(b):
    i = 0.0
    while i < len(a):
      AssertEquals(a[int(i)], b[int(i)], failures)
      i = i + 1.0
    
  else:
    failures.numberValue = failures.numberValue + 1.0

def AssertBooleanArraysEqual(a, b, failures):

  if len(a) == len(b):
    i = 0.0
    while i < len(a):
      AssertBooleansEqual(a[int(i)], b[int(i)], failures)
      i = i + 1.0
    
  else:
    failures.numberValue = failures.numberValue + 1.0

def AssertStringArraysEqual(a, b, failures):

  if len(a) == len(b):
    i = 0.0
    while i < len(a):
      AssertStringEquals(a[int(i)].string, b[int(i)].string, failures)
      i = i + 1.0
    
  else:
    failures.numberValue = failures.numberValue + 1.0

def nCreateStringScientificNotationDecimalFromNumber(decimal):

  mantissaReference = StringReference()
  exponentReference = StringReference()
  result =  [None]*int(0.0)
  done = False
  exponent = 0.0

  if decimal < 0.0:
    isPositive = False
    decimal =  -decimal
  else:
    isPositive = True

  if decimal == 0.0:
    done = True

  if  not done :
    multiplier = 0.0
    inc = 0.0

    if decimal < 1.0:
      multiplier = 10.0
      inc =  -1.0
    elif decimal >= 10.0:
      multiplier = 0.1
      inc = 1.0
    else:
      done = True

    if  not done :
      while decimal >= 10.0 or decimal < 1.0:
        decimal = decimal*multiplier
        exponent = exponent + inc
      

  nCreateStringFromNumberWithCheck(decimal, 10.0, mantissaReference)

  nCreateStringFromNumberWithCheck(exponent, 10.0, exponentReference)

  if  not isPositive :
    result = strAppendString(result, "-")

  result = strAppendString(result, mantissaReference.string)
  result = strAppendString(result, "e")
  result = strAppendString(result, exponentReference.string)

  return result

def nCreateStringDecimalFromNumber(decimal):

  stringReference = StringReference()

  # This will succeed because base = 10.
  nCreateStringFromNumberWithCheck(decimal, 10.0, stringReference)

  return stringReference.string

def nCreateStringFromNumberWithCheck(decimal, base, stringReference):

  isPositive = True

  if decimal < 0.0:
    isPositive = False
    decimal =  -decimal

  if decimal == 0.0:
    stringReference.string = "0"
    success = True
  else:
    characterReference = CharacterReference()

    if IsInteger(base):
      success = True

      string =  [None]*int(0.0)

      maximumDigits = nGetMaximumDigitsForBase(base)

      digitPosition = nGetFirstDigitPosition(decimal, base)

      decimal = round(decimal*base**(maximumDigits - digitPosition - 1.0))

      hasPrintedPoint = False

      if  not isPositive :
        string = strAppendCharacter(string, '-')

      # Print leading zeros.
      if digitPosition < 0.0:
        string = strAppendCharacter(string, '0')
        string = strAppendCharacter(string, '.')
        hasPrintedPoint = True
        i = 0.0
        while i <  -digitPosition - 1.0:
          string = strAppendCharacter(string, '0')
          i = i + 1.0
        

      # Print number.
      i = 0.0
      while i < maximumDigits and success:
        d = floor(decimal/base**(maximumDigits - i - 1.0))

        if d >= base:
          d = base - 1.0

        if  not hasPrintedPoint  and digitPosition - i + 1.0 == 0.0:
          if decimal != 0.0:
            string = strAppendCharacter(string, '.')
          hasPrintedPoint = True

        if decimal == 0.0 and hasPrintedPoint:
          pass
        else:
          success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference)
          if success:
            c = characterReference.characterValue
            string = strAppendCharacter(string, c)

        if success:
          decimal = decimal - d*base**(maximumDigits - i - 1.0)
        i = i + 1.0
      

      if success:
        # Print trailing zeros.
        i = 0.0
        while i < digitPosition - maximumDigits + 1.0:
          string = strAppendCharacter(string, '0')
          i = i + 1.0
        

        stringReference.string = string
    else:
      success = False

  # Done
  return success

def nGetMaximumDigitsForBase(base):

  t = 10.0**15.0
  return floor(log10(t)/log10(base))

def nGetFirstDigitPosition(decimal, base):

  power = ceil(log10(decimal)/log10(base))

  t = decimal*base**( -power)
  if t < base and t >= 1.0:
    pass
  elif t >= base:
    power = power + 1.0
  elif t < 1.0:
    power = power - 1.0

  return power

def nGetSingleDigitCharacterFromNumberWithCheck(c, base, characterReference):

  numberTable = nGetDigitCharacterTable()

  if c < base or c < len(numberTable):
    success = True
    characterReference.characterValue = numberTable[int(c)]
  else:
    success = False

  return success

def nGetDigitCharacterTable():

  numberTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

  return numberTable

def nCreateNumberFromDecimalStringWithCheck(string, decimalReference, errorMessage):
  return nCreateNumberFromStringWithCheck(string, 10.0, decimalReference, errorMessage)

def nCreateNumberFromDecimalString(string):

  doubleReference = CreateNumberReference(0.0)
  stringReference = CreateStringReference("")
  nCreateNumberFromStringWithCheck(string, 10.0, doubleReference, stringReference)
  number = doubleReference.numberValue

  doubleReference = None
  stringReference = None

  return number

def nCreateNumberFromStringWithCheck(string, base, numberReference, errorMessage):

  numberIsPositive = CreateBooleanReference(True)
  exponentIsPositive = CreateBooleanReference(True)
  beforePoint = NumberArrayReference()
  afterPoint = NumberArrayReference()
  exponent = NumberArrayReference()

  if base >= 2.0 and base <= 36.0:
    success = nExtractPartsFromNumberString(string, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage)

    if success:
      numberReference.numberValue = nCreateNumberFromParts(base, numberIsPositive.booleanValue, beforePoint.numberArray, afterPoint.numberArray, exponentIsPositive.booleanValue, exponent.numberArray)
  else:
    success = False
    errorMessage.string = "Base must be from 2 to 36."

  return success

def nCreateNumberFromParts(base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent):

  n = 0.0

  i = 0.0
  while i < len(beforePoint):
    p = beforePoint[int(len(beforePoint) - i - 1.0)]

    n = n + p*base**i
    i = i + 1.0
  

  i = 0.0
  while i < len(afterPoint):
    p = afterPoint[int(i)]

    n = n + p*base**( -(i + 1.0))
    i = i + 1.0
  

  if len(exponent) > 0.0:
    e = 0.0
    i = 0.0
    while i < len(exponent):
      p = exponent[int(len(exponent) - i - 1.0)]

      e = e + p*base**i
      i = i + 1.0
    

    if  not exponentIsPositive :
      e =  -e

    n = n*base**e

  if  not numberIsPositive :
    n =  -n

  return n

def nExtractPartsFromNumberString(n, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages):

  i = 0.0

  if i < len(n):
    if n[int(i)] == '-':
      numberIsPositive.booleanValue = False
      i = i + 1.0
    elif n[int(i)] == '+':
      numberIsPositive.booleanValue = True
      i = i + 1.0

    success = nExtractPartsFromNumberStringFromSign(n, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages)
  else:
    success = False
    errorMessages.string = "Number cannot have length zero."

  return success

def nExtractPartsFromNumberStringFromSign(n, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages):

  done = False
  count = 0.0
  while i + count < len(n) and  not done :
    if nCharacterIsNumberCharacterInBase(n[int(i + count)], base):
      count = count + 1.0
    else:
      done = True
  

  if count >= 1.0:
    beforePoint.numberArray =  [None]*int(count)

    j = 0.0
    while j < count:
      beforePoint.numberArray[int(j)] = nGetNumberFromNumberCharacterForBase(n[int(i + j)], base)
      j = j + 1.0
    

    i = i + count

    if i < len(n):
      success = nExtractPartsFromNumberStringFromPointOrExponent(n, base, i, afterPoint, exponentIsPositive, exponent, errorMessages)
    else:
      afterPoint.numberArray =  [None]*int(0.0)
      exponent.numberArray =  [None]*int(0.0)
      success = True
  else:
    success = False
    errorMessages.string = "Number must have at least one number after the optional sign."

  return success

def nExtractPartsFromNumberStringFromPointOrExponent(n, base, i, afterPoint, exponentIsPositive, exponent, errorMessages):

  if n[int(i)] == '.':
    i = i + 1.0

    if i < len(n):
      done = False
      count = 0.0
      while i + count < len(n) and  not done :
        if nCharacterIsNumberCharacterInBase(n[int(i + count)], base):
          count = count + 1.0
        else:
          done = True
      

      if count >= 1.0:
        afterPoint.numberArray =  [None]*int(count)

        j = 0.0
        while j < count:
          afterPoint.numberArray[int(j)] = nGetNumberFromNumberCharacterForBase(n[int(i + j)], base)
          j = j + 1.0
        

        i = i + count

        if i < len(n):
          success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages)
        else:
          exponent.numberArray =  [None]*int(0.0)
          success = True
      else:
        success = False
        errorMessages.string = "There must be at least one digit after the decimal point."
    else:
      success = False
      errorMessages.string = "There must be at least one digit after the decimal point."
  elif base <= 14.0 and (n[int(i)] == 'e' or n[int(i)] == 'E'):
    if i < len(n):
      success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages)
      afterPoint.numberArray =  [None]*int(0.0)
    else:
      success = False
      errorMessages.string = "There must be at least one digit after the exponent."
  else:
    success = False
    errorMessages.string = "Expected decimal point or exponent symbol."

  return success

def nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages):

  if base <= 14.0 and (n[int(i)] == 'e' or n[int(i)] == 'E'):
    i = i + 1.0

    if i < len(n):
      if n[int(i)] == '-':
        exponentIsPositive.booleanValue = False
        i = i + 1.0
      elif n[int(i)] == '+':
        exponentIsPositive.booleanValue = True
        i = i + 1.0

      if i < len(n):
        done = False
        count = 0.0
        while i + count < len(n) and  not done :
          if nCharacterIsNumberCharacterInBase(n[int(i + count)], base):
            count = count + 1.0
          else:
            done = True
        

        if count >= 1.0:
          exponent.numberArray =  [None]*int(count)

          j = 0.0
          while j < count:
            exponent.numberArray[int(j)] = nGetNumberFromNumberCharacterForBase(n[int(i + j)], base)
            j = j + 1.0
          

          i = i + count

          if i == len(n):
            success = True
          else:
            success = False
            errorMessages.string = "There cannot be any characters past the exponent of the number."
        else:
          success = False
          errorMessages.string = "There must be at least one digit after the decimal point."
      else:
        success = False
        errorMessages.string = "There must be at least one digit after the exponent symbol."
    else:
      success = False
      errorMessages.string = "There must be at least one digit after the exponent symbol."
  else:
    success = False
    errorMessages.string = "Expected exponent symbol."

  return success

def nGetNumberFromNumberCharacterForBase(c, base):

  numberTable = nGetDigitCharacterTable()
  position = 0.0

  i = 0.0
  while i < base:
    if numberTable[int(i)] == c:
      position = i
    i = i + 1.0
  

  return position

def nCharacterIsNumberCharacterInBase(c, base):

  numberTable = nGetDigitCharacterTable()
  found = False

  i = 0.0
  while i < base:
    if numberTable[int(i)] == c:
      found = True
    i = i + 1.0
  

  return found

def nStringToNumberArray(str):

  numberArrayReference = NumberArrayReference()
  stringReference = StringReference()

  nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference)

  numbers = numberArrayReference.numberArray

  numberArrayReference = None
  stringReference = None

  return numbers

def nStringToNumberArrayWithCheck(str, numberArrayReference, errorMessage):

  numberStrings = strSplitByString(str, ",")

  numbers =  [None]*int(len(numberStrings))
  success = True
  numberReference = NumberReference()

  i = 0.0
  while i < len(numberStrings):
    numberString = numberStrings[int(i)].string
    trimmedNumberString = strTrim(numberString)
    success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage)
    numbers[int(i)] = numberReference.numberValue

    FreeStringReference(numberStrings[int(i)])
    trimmedNumberString = None
    i = i + 1.0
  

  numberStrings = None
  numberReference = None

  numberArrayReference.numberArray = numbers

  return success

class lLinkedListNodeStrings:
  end = None
  value = None
  next = None

class lLinkedListStrings:
  first = None
  last = None

class lLinkedListNodeNumbers:
  next = None
  end = None
  value = None

class lLinkedListNumbers:
  first = None
  last = None

class lLinkedListCharacters:
  first = None
  last = None

class lLinkedListNodeCharacters:
  end = None
  value = None
  next = None

class lDynamicArrayNumbers:
  array = None
  length = None

def lAddNumber(list, a):

  newlist =  [None]*int(len(list) + 1.0)
  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def lAddNumberRef(list, i):
  list.numberArray = lAddNumber(list.numberArray, i)

def lRemoveNumber(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None
		
  return newlist

def lGetNumberRef(list, i):
  return list.numberArray[int(i)]

def lRemoveNumberRef(list, i):
  list.numberArray = lRemoveNumber(list.numberArray, i)

def lAddString(list, a):

  newlist =  [None]*int(len(list) + 1.0)

  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def lAddStringRef(list, i):
  list.stringArray = lAddString(list.stringArray, i)

def lRemoveString(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None
		
  return newlist

def lGetStringRef(list, i):
  return list.stringArray[int(i)]

def lRemoveStringRef(list, i):
  list.stringArray = lRemoveString(list.stringArray, i)

def lAddBoolean(list, a):

  newlist =  [None]*int(len(list) + 1.0)
  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def lAddBooleanRef(list, i):
  list.booleanArray = lAddBoolean(list.booleanArray, i)

def lRemoveBoolean(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None
		
  return newlist

def lGetBooleanRef(list, i):
  return list.booleanArray[int(i)]

def lRemoveDecimalRef(list, i):
  list.booleanArray = lRemoveBoolean(list.booleanArray, i)

def lCreateLinkedListString():

  ll = lLinkedListStrings()
  ll.first = lLinkedListNodeStrings()
  ll.last = ll.first
  ll.last.end = True

  return ll

def lLinkedListAddString(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = lLinkedListNodeStrings()
  ll.last.next.end = True
  ll.last = ll.last.next

def lLinkedListStringsToArray(ll):

  node = ll.first

  length = lLinkedListStringsLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = StringReference()
    array[int(i)].string = node.value
    node = node.next
    i = i + 1.0
  

  return array

def lLinkedListStringsLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def lFreeLinkedListString(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def lCreateLinkedListNumbers():

  ll = lLinkedListNumbers()
  ll.first = lLinkedListNodeNumbers()
  ll.last = ll.first
  ll.last.end = True

  return ll

def lCreateLinkedListNumbersArray(length):

  lls =  [None]*int(length)
  i = 0.0
  while i < len(lls):
    lls[int(i)] = lCreateLinkedListNumbers()
    i = i + 1.0
  

  return lls

def lLinkedListAddNumber(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = lLinkedListNodeNumbers()
  ll.last.next.end = True
  ll.last = ll.last.next

def lLinkedListNumbersLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def lLinkedListNumbersIndex(ll, index):

  node = ll.first
  i = 0.0
  while i < index:
    node = node.next
    i = i + 1.0
  

  return node.value

def lLinkedListInsertNumber(ll, index, value):

  if index == 0.0:
    tmp = ll.first
    ll.first = lLinkedListNodeNumbers()
    ll.first.next = tmp
    ll.first.value = value
    ll.first.end = False
  else:
    node = ll.first
    i = 0.0
    while i < index - 1.0:
      node = node.next
      i = i + 1.0
    

    tmp = node.next
    node.next = lLinkedListNodeNumbers()
    node.next.next = tmp
    node.next.value = value
    node.next.end = False

def lLinkedListSet(ll, index, value):

  node = ll.first
  i = 0.0
  while i < index:
    node = node.next
    i = i + 1.0
  

  node.next.value = value

def lLinkedListRemoveNumber(ll, index):

  node = ll.first
  prev = ll.first

  i = 0.0
  while i < index:
    prev = node
    node = node.next
    i = i + 1.0
  

  if index == 0.0:
    ll.first = prev.next
  if  not prev.next.end :
    prev.next = prev.next.next

def lFreeLinkedListNumbers(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def lFreeLinkedListNumbersArray(lls):

  i = 0.0
  while i < len(lls):
    lFreeLinkedListNumbers(lls[int(i)])
    i = i + 1.0
  
  lls = None

def lLinkedListNumbersToArray(ll):

  node = ll.first

  length = lLinkedListNumbersLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return array

def lArrayToLinkedListNumbers(array):

  ll = lCreateLinkedListNumbers()

  i = 0.0
  while i < len(array):
    lLinkedListAddNumber(ll, array[int(i)])
    i = i + 1.0
  

  return ll

def lLinkedListNumbersEqual(a, b):

  an = a.first
  bn = b.first

  equal = True
  done = False
  while equal and  not done :
    if an.end == bn.end:
      if an.end:
        done = True
      elif an.value == bn.value:
        an = an.next
        bn = bn.next
      else:
        equal = False
    else:
      equal = False
  

  return equal

def lCreateLinkedListCharacter():

  ll = lLinkedListCharacters()
  ll.first = lLinkedListNodeCharacters()
  ll.last = ll.first
  ll.last.end = True

  return ll

def lLinkedListAddCharacter(ll, value):
  ll.last.end = False
  ll.last.value = value
  ll.last.next = lLinkedListNodeCharacters()
  ll.last.next.end = True
  ll.last = ll.last.next

def lLinkedListCharactersToArray(ll):

  node = ll.first

  length = lLinkedListCharactersLength(ll)

  array =  [None]*int(length)

  i = 0.0
  while i < length:
    array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return array

def lLinkedListCharactersLength(ll):

  l = 0.0
  node = ll.first
  while  not node.end :
    node = node.next
    l = l + 1.0
  

  return l

def lFreeLinkedListCharacter(ll):

  node = ll.first

  while  not node.end :
    prev = node
    node = node.next
    prev = None
  

  node = None

def lCreateDynamicArrayNumbers():

  da = lDynamicArrayNumbers()
  da.array =  [None]*int(10.0)
  da.length = 0.0

  return da

def lCreateDynamicArrayNumbersWithInitialCapacity(capacity):

  da = lDynamicArrayNumbers()
  da.array =  [None]*int(capacity)
  da.length = 0.0

  return da

def lDynamicArrayAddNumber(da, value):
  if da.length == len(da.array):
    lDynamicArrayNumbersIncreaseSize(da)

  da.array[int(da.length)] = value
  da.length = da.length + 1.0

def lDynamicArrayNumbersIncreaseSize(da):

  newLength = round(len(da.array)*3.0/2.0)
  newArray =  [None]*int(newLength)

  i = 0.0
  while i < len(da.array):
    newArray[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  da.array = None

  da.array = newArray

def lDynamicArrayNumbersDecreaseSizeNecessary(da):

  needsDecrease = False

  if da.length > 10.0:
    needsDecrease = da.length <= round(len(da.array)*2.0/3.0)

  return needsDecrease

def lDynamicArrayNumbersDecreaseSize(da):

  newLength = round(len(da.array)*2.0/3.0)
  newArray =  [None]*int(newLength)

  i = 0.0
  while i < len(da.array):
    newArray[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  da.array = None

  da.array = newArray

def lDynamicArrayNumbersIndex(da, index):
  return da.array[int(index)]

def lDynamicArrayNumbersLength(da):
  return da.length

def lDynamicArrayInsertNumber(da, index, value):

  if da.length == len(da.array):
    lDynamicArrayNumbersIncreaseSize(da)

  i = da.length
  while i > index:
    da.array[int(i)] = da.array[int(i - 1.0)]
    i = i - 1.0
  

  da.array[int(index)] = value

  da.length = da.length + 1.0

def lDynamicArraySet(da, index, value):
  da.array[int(index)] = value

def lDynamicArrayRemoveNumber(da, index):

  i = index
  while i < da.length - 1.0:
    da.array[int(i)] = da.array[int(i + 1.0)]
    i = i + 1.0
  

  da.length = da.length - 1.0

  if lDynamicArrayNumbersDecreaseSizeNecessary(da):
    lDynamicArrayNumbersDecreaseSize(da)

def lFreeDynamicArrayNumbers(da):
  da.array = None
  da = None

def lDynamicArrayNumbersToArray(da):

  array =  [None]*int(da.length)

  i = 0.0
  while i < da.length:
    array[int(i)] = da.array[int(i)]
    i = i + 1.0
  

  return array

def lArrayToDynamicArrayNumbersWithOptimalSize(array):

  #
  #         c = 10*(3/2)^n
  #         log(c) = log(10*(3/2)^n)
  #         log(c) = log(10) + log((3/2)^n)
  #         log(c) = 1 + log((3/2)^n)
  #         log(c) - 1 = log((3/2)^n)
  #         log(c) - 1 = n*log(3/2)
  #         n = (log(c) - 1)/log(3/2)
  #        
  c = len(array)
  n = (log(c) - 1.0)/log(3.0/2.0)
  newCapacity = floor(n) + 1.0

  da = lCreateDynamicArrayNumbersWithInitialCapacity(newCapacity)

  i = 0.0
  while i < len(array):
    da.array[int(i)] = array[int(i)]
    i = i + 1.0
  

  return da

def lArrayToDynamicArrayNumbers(array):

  da = lDynamicArrayNumbers()
  da.array = CopyNumberArray(array)
  da.length = len(array)

  return da

def lDynamicArrayNumbersEqual(a, b):

  equal = True
  if a.length == b.length:
    i = 0.0
    while i < a.length and equal:
      if a.array[int(i)] != b.array[int(i)]:
        equal = False
      i = i + 1.0
    
  else:
    equal = False

  return equal

def lDynamicArrayNumbersToLinkedList(da):

  ll = lCreateLinkedListNumbers()

  i = 0.0
  while i < da.length:
    lLinkedListAddNumber(ll, da.array[int(i)])
    i = i + 1.0
  

  return ll

def lLinkedListToDynamicArrayNumbers(ll):

  node = ll.first

  da = lDynamicArrayNumbers()
  da.length = lLinkedListNumbersLength(ll)

  da.array =  [None]*int(da.length)

  i = 0.0
  while i < da.length:
    da.array[int(i)] = node.value
    node = node.next
    i = i + 1.0
  

  return da

def lAddCharacter(list, a):

  newlist =  [None]*int(len(list) + 1.0)
  i = 0.0
  while i < len(list):
    newlist[int(i)] = list[int(i)]
    i = i + 1.0
  
  newlist[int(len(list))] = a
		
  list = None
		
  return newlist

def lAddCharacterRef(list, i):
  list.string = lAddCharacter(list.string, i)

def lRemoveCharacter(list, n):

  newlist =  [None]*int(len(list) - 1.0)

  if n >= 0.0 and n < len(list):
    i = 0.0
    while i < len(list):
      if i < n:
        newlist[int(i)] = list[int(i)]
      if i > n:
        newlist[int(i - 1.0)] = list[int(i)]
      i = i + 1.0
    

    list = None
  else:
    newlist = None

  return newlist

def lGetCharacterRef(list, i):
  return list.string[int(i)]

def lRemoveCharacterRef(list, i):
  list.string = lRemoveCharacter(list.string, i)

def Negate(x):
  return  -x

def Positive(x):
  return  +x

def Factorial(x):

  f = 1.0

  i = 2.0
  while i <= x:
    f = f*i
    i = i + 1.0
  

  return f

def Round(x):
  return floor(x + 0.5)

def BankersRound(x):

  if Absolute(x - Truncate(x)) == 0.5:
    if  not DivisibleBy(Round(x), 2.0) :
      r = Round(x) - 1.0
    else:
      r = Round(x)
  else:
    r = Round(x)

  return r

def Ceil(x):
  return ceil(x)

def Floor(x):
  return floor(x)

def Truncate(x):

  if x >= 0.0:
    t = floor(x)
  else:
    t = ceil(x)

  return t

def Absolute(x):
  return fabs(x)

def Logarithm(x):
  return log10(x)

def NaturalLogarithm(x):
  return log(x)

def Sin(x):
  return sin(x)

def Cos(x):
  return cos(x)

def Tan(x):
  return tan(x)

def Asin(x):
  return asin(x)

def Acos(x):
  return acos(x)

def Atan(x):
  return atan(x)

def Atan2(y, x):

  # Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors.
  a = 0.0

  if x > 0.0:
    a = Atan(y/x)
  elif x < 0.0 and y >= 0.0:
    a = Atan(y/x) + pi
  elif x < 0.0 and y < 0.0:
    a = Atan(y/x) - pi
  elif x == 0.0 and y > 0.0:
    a = pi/2.0
  elif x == 0.0 and y < 0.0:
    a =  -pi/2.0

  return a

def Squareroot(x):
  return sqrt(x)

def Exp(x):
  return exp(x)

def DivisibleBy(a, b):
  return ((a % b) == 0.0)

def Combinations(n, k):

  c = 1.0
  j = 1.0
  i = n - k + 1.0

  while i <= n:
    c = c*i
    c = c/j

    i = i + 1.0
    j = j + 1.0
  

  return c

def Permutations(n, k):

  c = 1.0

  i = n - k + 1.0
  while i <= n:
    c = c*i
    i = i + 1.0
  

  return c

def EpsilonCompare(a, b, epsilon):
  return fabs(a - b) < epsilon

def GreatestCommonDivisor(a, b):

  while b != 0.0:
    t = b
    b = a % b
    a = t
  

  return a

def GCDWithSubtraction(a, b):

  if a == 0.0:
    g = b
  else:
    while b != 0.0:
      if a > b:
        a = a - b
      else:
        b = b - a
    

    g = a

  return g

def IsInteger(a):
  return (a - floor(a)) == 0.0

def GreatestCommonDivisorWithCheck(a, b, gcdReference):

  if IsInteger(a) and IsInteger(b):
    gcd = GreatestCommonDivisor(a, b)
    gcdReference.numberValue = gcd
    success = True
  else:
    success = False

  return success

def LeastCommonMultiple(a, b):

  if a > 0.0 and b > 0.0:
    lcm = fabs(a*b)/GreatestCommonDivisor(a, b)
  else:
    lcm = 0.0

  return lcm

def Sign(a):

  if a > 0.0:
    s = 1.0
  elif a < 0.0:
    s =  -1.0
  else:
    s = 0.0

  return s

def Max(a, b):
  return max(a,b)

def Min(a, b):
  return min(a,b)

def Power(a, b):
  return a**b

def Gamma(x):
  return LanczosApproximation(x)

def LogGamma(x):
  return log(Gamma(x))

def LanczosApproximation(z):

  p =  [None]*int(8.0)
  p[int(0.0)] = 676.5203681218851
  p[int(1.0)] =  -1259.1392167224028
  p[int(2.0)] = 771.32342877765313
  p[int(3.0)] =  -176.61502916214059
  p[int(4.0)] = 12.507343278686905
  p[int(5.0)] =  -0.13857109526572012
  p[int(6.0)] = 9.9843695780195716e-6
  p[int(7.0)] = 1.5056327351493116e-7

  if z < 0.5:
    y = pi/(sin(pi*z)*LanczosApproximation(1.0 - z))
  else:
    z = z - 1.0
    x = 0.99999999999980993
    i = 0.0
    while i < len(p):
      x = x + p[int(i)]/(z + i + 1.0)
      i = i + 1.0
    
    t = z + len(p) - 0.5
    y = sqrt(2.0*pi)*t**(z + 0.5)*exp( -t)*x

  return y

def Beta(x, y):
  return Gamma(x)*Gamma(y)/Gamma(x + y)

def Sinh(x):
  return (exp(x) - exp( -x))/2.0

def Cosh(x):
  return (exp(x) + exp( -x))/2.0

def Tanh(x):
  return Sinh(x)/Cosh(x)

def Cot(x):
  return 1.0/tan(x)

def Sec(x):
  return 1.0/cos(x)

def Csc(x):
  return 1.0/sin(x)

def Coth(x):
  return Cosh(x)/Sinh(x)

def Sech(x):
  return 1.0/Cosh(x)

def Csch(x):
  return 1.0/Sinh(x)

def Error(x):

  if x == 0.0:
    y = 0.0
  elif x < 0.0:
    y =  -Error( -x)
  else:
    c1 =  -1.26551223
    c2 =  +1.00002368
    c3 =  +0.37409196
    c4 =  +0.09678418
    c5 =  -0.18628806
    c6 =  +0.27886807
    c7 =  -1.13520398
    c8 =  +1.48851587
    c9 =  -0.82215223
    c10 =  +0.17087277

    t = 1.0/(1.0 + 0.5*fabs(x))

    tau = t*exp( -x**2.0 + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))))

    y = 1.0 - tau

  return y

def ErrorInverse(x):

  a = (8.0*(pi - 3.0))/(3.0*pi*(4.0 - pi))

  t = 2.0/(pi*a) + log(1.0 - x**2.0)/2.0
  y = Sign(x)*sqrt(sqrt(t**2.0 - log(1.0 - x**2.0)/a) - t)

  return y

def FallingFactorial(x, n):

  y = 1.0

  k = 0.0
  while k <= n - 1.0:
    y = y*(x - k)
    k = k + 1.0
  

  return y

def RisingFactorial(x, n):

  y = 1.0

  k = 0.0
  while k <= n - 1.0:
    y = y*(x + k)
    k = k + 1.0
  

  return y

def Hypergeometric(a, b, c, z, maxIterations, precision):

  if fabs(z) >= 0.5:
    y = (1.0 - z)**( -a)*HypergeometricDirect(a, c - b, c, z/(z - 1.0), maxIterations, precision)
  else:
    y = HypergeometricDirect(a, b, c, z, maxIterations, precision)

  return y

def HypergeometricDirect(a, b, c, z, maxIterations, precision):

  y = 0.0
  done = False

  n = 0.0
  while n < maxIterations and  not done :
    yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*z**n/Factorial(n)
    if fabs(yp) < precision:
      done = True
    y = y + yp
    n = n + 1.0
  

  return y

def BernouilliNumber(n):
  return AkiyamaTanigawaAlgorithm(n)

def AkiyamaTanigawaAlgorithm(n):

  A =  [None]*int(n + 1.0)

  m = 0.0
  while m <= n:
    A[int(m)] = 1.0/(m + 1.0)
    j = m
    while j >= 1.0:
      A[int(j - 1.0)] = j*(A[int(j - 1.0)] - A[int(j)])
      j = j - 1.0
    
    m = m + 1.0
  

  B = A[int(0.0)]

  A = None

  return B

def charToLowerCase(character):

  toReturn = character
  if character == 'A':
    toReturn = 'a'
  elif character == 'B':
    toReturn = 'b'
  elif character == 'C':
    toReturn = 'c'
  elif character == 'D':
    toReturn = 'd'
  elif character == 'E':
    toReturn = 'e'
  elif character == 'F':
    toReturn = 'f'
  elif character == 'G':
    toReturn = 'g'
  elif character == 'H':
    toReturn = 'h'
  elif character == 'I':
    toReturn = 'i'
  elif character == 'J':
    toReturn = 'j'
  elif character == 'K':
    toReturn = 'k'
  elif character == 'L':
    toReturn = 'l'
  elif character == 'M':
    toReturn = 'm'
  elif character == 'N':
    toReturn = 'n'
  elif character == 'O':
    toReturn = 'o'
  elif character == 'P':
    toReturn = 'p'
  elif character == 'Q':
    toReturn = 'q'
  elif character == 'R':
    toReturn = 'r'
  elif character == 'S':
    toReturn = 's'
  elif character == 'T':
    toReturn = 't'
  elif character == 'U':
    toReturn = 'u'
  elif character == 'V':
    toReturn = 'v'
  elif character == 'W':
    toReturn = 'w'
  elif character == 'X':
    toReturn = 'x'
  elif character == 'Y':
    toReturn = 'y'
  elif character == 'Z':
    toReturn = 'z'

  return toReturn

def charToUpperCase(character):

  toReturn = character
  if character == 'a':
    toReturn = 'A'
  elif character == 'b':
    toReturn = 'B'
  elif character == 'c':
    toReturn = 'C'
  elif character == 'd':
    toReturn = 'D'
  elif character == 'e':
    toReturn = 'E'
  elif character == 'f':
    toReturn = 'F'
  elif character == 'g':
    toReturn = 'G'
  elif character == 'h':
    toReturn = 'H'
  elif character == 'i':
    toReturn = 'I'
  elif character == 'j':
    toReturn = 'J'
  elif character == 'k':
    toReturn = 'K'
  elif character == 'l':
    toReturn = 'L'
  elif character == 'm':
    toReturn = 'M'
  elif character == 'n':
    toReturn = 'N'
  elif character == 'o':
    toReturn = 'O'
  elif character == 'p':
    toReturn = 'P'
  elif character == 'q':
    toReturn = 'Q'
  elif character == 'r':
    toReturn = 'R'
  elif character == 's':
    toReturn = 'S'
  elif character == 't':
    toReturn = 'T'
  elif character == 'u':
    toReturn = 'U'
  elif character == 'v':
    toReturn = 'V'
  elif character == 'w':
    toReturn = 'W'
  elif character == 'x':
    toReturn = 'X'
  elif character == 'y':
    toReturn = 'Y'
  elif character == 'z':
    toReturn = 'Z'

  return toReturn

def charIsUpperCase(character):

  isUpper = False
  if character == 'A':
    isUpper = True
  elif character == 'B':
    isUpper = True
  elif character == 'C':
    isUpper = True
  elif character == 'D':
    isUpper = True
  elif character == 'E':
    isUpper = True
  elif character == 'F':
    isUpper = True
  elif character == 'G':
    isUpper = True
  elif character == 'H':
    isUpper = True
  elif character == 'I':
    isUpper = True
  elif character == 'J':
    isUpper = True
  elif character == 'K':
    isUpper = True
  elif character == 'L':
    isUpper = True
  elif character == 'M':
    isUpper = True
  elif character == 'N':
    isUpper = True
  elif character == 'O':
    isUpper = True
  elif character == 'P':
    isUpper = True
  elif character == 'Q':
    isUpper = True
  elif character == 'R':
    isUpper = True
  elif character == 'S':
    isUpper = True
  elif character == 'T':
    isUpper = True
  elif character == 'U':
    isUpper = True
  elif character == 'V':
    isUpper = True
  elif character == 'W':
    isUpper = True
  elif character == 'X':
    isUpper = True
  elif character == 'Y':
    isUpper = True
  elif character == 'Z':
    isUpper = True

  return isUpper

def charIsLowerCase(character):

  isLower = False
  if character == 'a':
    isLower = True
  elif character == 'b':
    isLower = True
  elif character == 'c':
    isLower = True
  elif character == 'd':
    isLower = True
  elif character == 'e':
    isLower = True
  elif character == 'f':
    isLower = True
  elif character == 'g':
    isLower = True
  elif character == 'h':
    isLower = True
  elif character == 'i':
    isLower = True
  elif character == 'j':
    isLower = True
  elif character == 'k':
    isLower = True
  elif character == 'l':
    isLower = True
  elif character == 'm':
    isLower = True
  elif character == 'n':
    isLower = True
  elif character == 'o':
    isLower = True
  elif character == 'p':
    isLower = True
  elif character == 'q':
    isLower = True
  elif character == 'r':
    isLower = True
  elif character == 's':
    isLower = True
  elif character == 't':
    isLower = True
  elif character == 'u':
    isLower = True
  elif character == 'v':
    isLower = True
  elif character == 'w':
    isLower = True
  elif character == 'x':
    isLower = True
  elif character == 'y':
    isLower = True
  elif character == 'z':
    isLower = True

  return isLower

def charIsLetter(character):
  return charIsUpperCase(character) or charIsLowerCase(character)

def charIsNumber(character):

  isNumberx = False
  if character == '0':
    isNumberx = True
  elif character == '1':
    isNumberx = True
  elif character == '2':
    isNumberx = True
  elif character == '3':
    isNumberx = True
  elif character == '4':
    isNumberx = True
  elif character == '5':
    isNumberx = True
  elif character == '6':
    isNumberx = True
  elif character == '7':
    isNumberx = True
  elif character == '8':
    isNumberx = True
  elif character == '9':
    isNumberx = True

  return isNumberx

def charIsWhiteSpace(character):

  isWhiteSpacex = False
  if character == ' ':
    isWhiteSpacex = True
  elif character == '\t':
    isWhiteSpacex = True
  elif character == '\n':
    isWhiteSpacex = True
  elif character == '\r':
    isWhiteSpacex = True

  return isWhiteSpacex

def charIsSymbol(character):

  isSymbolx = False
  if character == '!':
    isSymbolx = True
  elif character == '\"':
    isSymbolx = True
  elif character == '#':
    isSymbolx = True
  elif character == '$':
    isSymbolx = True
  elif character == '%':
    isSymbolx = True
  elif character == '&':
    isSymbolx = True
  elif character == '\'':
    isSymbolx = True
  elif character == '(':
    isSymbolx = True
  elif character == ')':
    isSymbolx = True
  elif character == '*':
    isSymbolx = True
  elif character == '+':
    isSymbolx = True
  elif character == ',':
    isSymbolx = True
  elif character == '-':
    isSymbolx = True
  elif character == '.':
    isSymbolx = True
  elif character == '/':
    isSymbolx = True
  elif character == ':':
    isSymbolx = True
  elif character == ';':
    isSymbolx = True
  elif character == '<':
    isSymbolx = True
  elif character == '=':
    isSymbolx = True
  elif character == '>':
    isSymbolx = True
  elif character == '?':
    isSymbolx = True
  elif character == '@':
    isSymbolx = True
  elif character == '[':
    isSymbolx = True
  elif character == '\\':
    isSymbolx = True
  elif character == ']':
    isSymbolx = True
  elif character == '^':
    isSymbolx = True
  elif character == '_':
    isSymbolx = True
  elif character == '`':
    isSymbolx = True
  elif character == '{':
    isSymbolx = True
  elif character == '|':
    isSymbolx = True
  elif character == '}':
    isSymbolx = True
  elif character == '~':
    isSymbolx = True

  return isSymbolx

def charCharacterIsBefore(a, b):

  ad = ord(a)
  bd = ord(b)

  return ad < bd


