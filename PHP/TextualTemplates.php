<?php
// Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.


function unichr($unicode){
    return mb_convert_encoding("&#{$unicode};", 'UTF-8', 'HTML-ENTITIES');
}
function uniord($s) {
    return unpack('V', iconv('UTF-8', 'UCS-4LE', $s))[1];
}

function CreateLinkedListNodes(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function LinkedListAddNode($ll, $value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function &LinkedListNodesToArray($ll){

  $node = $ll->first;

  $length = LinkedListNodesLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = $node->value;
    $node = $node->next;
  }

  return $array;
}
function LinkedListNodesLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function FreeLinkedListNode($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function IsValidTemplate(&$template){

  $tokens = CreateLinkedListString();
  $errorMessage = new stdClass();

  $success = GenerateTokensFromTemplate($template, $tokens, $errorMessage);

  if($success){
    $root = new stdClass();

    $ts = LinkedListStringsToArray($tokens);

    $success = ParseTemplate($ts, $root, $errorMessage);
  }

  if( !$success ){
    FreeStringReference($errorMessage);
  }

  return $success;
}
function GenerateTokensFromTemplate(&$template, $tokens, $errorMessage){

  $success = true;
  $cs = array_fill(0, 4.0, 0);

  $da = CreateDynamicArrayCharacters();

  $pc = "x";
  for($i = 0.0; $i < count($template) && $success; ){
    $c = $template[$i];

    if($c != "{"){
      DynamicArrayAddCharacter($da, $c);
      $i = $i + 1.0;
    }else{
      FillString($cs, "x");
      for($j = 0.0; $i + $j < count($template) && $j < count($cs); $j = $j + 1.0){
        $cs[$j] = $template[$i + $j];
      }

      if(StringsEqual($cs, $literal = str_split("{use")) || StringsEqual($cs, $literal = str_split("{end")) || StringsEqual($cs, $literal = str_split("{pri")) || StringsEqual($cs, $literal = str_split("{for")) || StringsEqual($cs, $literal = str_split("{if ")) || StringsEqual($cs, $literal = str_split("{els"))){
        if($pc != "\\"){
          /* Find end. */
          $found = false;
          for($l = 0.0; $i + $l < count($template) &&  !$found ; $l = $l + 1.0){
            if($template[$i + $l] == "}"){
              $found = true;
            }
          }

          if($found){
            if($da->length > 0.0){
              $a = DynamicArrayCharactersToArray($da);
              LinkedListAddString($tokens, $a);
              FreeDynamicArrayCharacters($da);
              $da = CreateDynamicArrayCharacters();
            }

            for($j = 0.0; $j < $l; $j = $j + 1.0){
              DynamicArrayAddCharacter($da, $template[$i + $j]);
            }

            $a = DynamicArrayCharactersToArray($da);
            LinkedListAddString($tokens, $a);
            FreeDynamicArrayCharacters($da);
            $da = CreateDynamicArrayCharacters();

            $i = $i + $l;
          }else{
            $success = false;
            $errorMessage->string = str_split("Template command found, but not ended properly.");
          }
        }else{
          DynamicArrayAddCharacter($da, $c);
          $i = $i + 1.0;
        }
      }else{
        DynamicArrayAddCharacter($da, $c);
        $i = $i + 1.0;
      }
    }

    $pc = $c;
  }

  if($da->length > 0.0){
    $a = DynamicArrayCharactersToArray($da);
    LinkedListAddString($tokens, $a);
  }

  FreeDynamicArrayCharacters($da);

  return $success;
}
function GenerateDocument(&$template, $data, $document, $errorMessage){

  $tokens = CreateLinkedListString();

  $success = GenerateTokensFromTemplate($template, $tokens, $errorMessage);

  if($success){
    $root = new stdClass();

    $ts = LinkedListStringsToArray($tokens);
    FreeLinkedListString($tokens);

    $success = ParseTemplate($ts, $root, $errorMessage);

    if($success){
      $ll = CreateLinkedListCharacter();

      $success = GenerateDocumentFromBlock($root, $data, $ll, $errorMessage);

      if($success){
        $document->string = LinkedListCharactersToArray($ll);
        FreeLinkedListCharacter($ll);
      }
    }
  }

  return $success;
}
function GenerateDocumentFromBlock($root, $data, $ll, $errorMessage){

  $n = $root->nodes->first;
  $success = true;

  for(;  !$n->end  && $success; ){
    $success = GenerateDocumentFromNode($n->value, $data, $ll, $errorMessage);
    $n = $n->next;
  }

  return $success;
}
function GenerateDocumentFromNode($n, $data, $ll, $errorMessage){

  $success = true;
  $found = new stdClass();

  if(StringsEqual($n->type, $literal = str_split("block"))){
    $success = GenerateDocumentFromBlock($n, $data, $ll, $errorMessage);
  }else if(StringsEqual($n->type, $literal = str_split("use"))){
    $success = GenerateDocumentFromUse($n, $data, $ll, $errorMessage);
  }else if(StringsEqual($n->type, $literal = str_split("if"))){
    $success = GenerateDocumentFromIf($n, $data, $ll, $errorMessage);
  }else if(StringsEqual($n->type, $literal = str_split("foreach"))){
    $success = GenerateDocumentFromForeach($n, $data, $ll, $errorMessage);
  }else if(StringsEqual($n->type, $literal = str_split("text"))){
    $success = true;
    LinkedListCharactersAddString($ll, $n->p1);
  }else if(StringsEqual($n->type, $literal = str_split("print"))){
    if(StringsEqual($data->type, $literal = str_split("object"))){
      $value = GetObjectValueWithCheck($data->object, $n->p1, $found);

      if($found->booleanValue){
        if(StringsEqual($value->type, $literal = str_split("string"))){
          $valueString = $value->string;
        }else{
          $valueString = WriteJSON($value);
        }
        LinkedListCharactersAddString($ll, $valueString);
      }else{
        $success = false;
        $errorMessage->string = str_split("Key for printing not found in JSON object: ");
        $errorMessage->string = sConcatenateString($errorMessage->string, $n->p1);
      }
    }else{
      $success = false;
      $errorMessage->string = str_split("Data structure for print command is not a JSON object.");
    }
  }

  return $success;
}
function GenerateDocumentFromUse($n, $data, $ll, $errorMessage){

  $found = new stdClass();

  if(StringsEqual($data->type, $literal = str_split("object"))){
    $value = GetObjectValueWithCheck($data->object, $n->p1, $found);

    if($found->booleanValue){
      $success = GenerateDocumentFromNode($n->block1, $value, $ll, $errorMessage);
    }else{
      $success = false;
      $errorMessage->string = str_split("Key for use not found in JSON object.");
    }
  }else{
    $success = false;
    $errorMessage->string = str_split("Data structure for use command is not a JSON object.");
  }

  return $success;
}
function GenerateDocumentFromIf($n, $data, $ll, $errorMessage){

  $success = true;
  $found = new stdClass();

  if(StringsEqual($data->type, $literal = str_split("object"))){
    $value = GetObjectValueWithCheck($data->object, $n->p1, $found);

    if($found->booleanValue){
      if(StringsEqual($value->type, $literal = str_split("boolean"))){
        if($value->booleanValue){
          $success = GenerateDocumentFromBlock($n->block1, $data, $ll, $errorMessage);
        }

        if($n->hasElseBlock){
          if( !$value->booleanValue ){
            $success = GenerateDocumentFromBlock($n->block2, $data, $ll, $errorMessage);
          }
        }
      }else{
        $success = false;
        $errorMessage->string = str_split("Value for if not boolean.");
      }
    }else{
      $success = false;
      $errorMessage->string = str_split("Key for if not found in JSON object: ");
      $errorMessage->string = sConcatenateString($errorMessage->string, $n->p1);
    }
  }else{
    $success = false;
    $errorMessage->string = str_split("Data structure for if command is not a JSON object.");
  }

  return $success;
}
function GenerateDocumentFromForeach($n, $data, $ll, $errorMessage){

  $success = true;
  $found = new stdClass();
  $loopVar = CreateObjectElement(0.0);

  PutStringElementMap($loopVar->object, $n->p1, new stdClass());

  if(StringsEqual($data->type, $literal = str_split("object"))){
    $value = GetObjectValueWithCheck($data->object, $n->p2, $found);

    if($found->booleanValue){
      if(StringsEqual($value->type, $literal = str_split("array"))){

        for($i = 0.0; $i < count($value->array); $i = $i + 1.0){
          $arrayElement = $value->array[$i];
          if(StringsEqual($arrayElement->type, $literal = str_split("object"))){
            $success = GenerateDocumentFromBlock($n->block1, $arrayElement, $ll, $errorMessage);
          }else{
            SetStringElementMap($loopVar->object, 0.0, $n->p1, $arrayElement);
            $success = GenerateDocumentFromBlock($n->block1, $loopVar, $ll, $errorMessage);
          }
        }
      }else{
        $success = false;
        $errorMessage->string = str_split("Value for foreach is not an array.");
      }
    }else{
      $success = false;
      $errorMessage->string = str_split("Key for foreach not found in JSON object: ");
      $errorMessage->string = sConcatenateString($errorMessage->string, $n->p2);
    }
  }else{
    $success = false;
    $errorMessage->string = str_split("Data structure for foreach command is not a JSON object.");
  }

  return $success;
}
function ParseTemplate(&$tokens, $node, $errorMessage){

  $position = CreateNumberReference(0.0);
  $success = ParseTemplateBlock($tokens, $position, $node, $errorMessage);

  if($success){
    if($position->numberValue != count($tokens)){
      $success = false;
      $errorMessage->string = str_split("Unexpected token at the end of template.");
    }
  }

  unset($position);

  return $success;
}
function ParseTemplateBlock(&$tokens, $position, $node, $errorMessage){

  $success = true;
  $done = false;

  $node->type = str_split("block");
  $node->nodes = CreateLinkedListNodes();

  for(; $position->numberValue < count($tokens) && $success &&  !$done ; ){
    $tn = new stdClass();
    $success = ParseNodeString($tokens[$position->numberValue]->string, $tn, $errorMessage);

    if($success){
      if(StringsEqual($tn->type, $literal = str_split("text")) || StringsEqual($tn->type, $literal = str_split("print"))){
        LinkedListAddNode($node->nodes, $tn);
        $position->numberValue = $position->numberValue + 1.0;
      }else if(StringsEqual($tn->type, $literal = str_split("use"))){
        $nb = new stdClass();
        $success = ParseUseBlock($tokens, $position, $nb, $errorMessage);
        LinkedListAddNode($node->nodes, $nb);
      }else if(StringsEqual($tn->type, $literal = str_split("if"))){
        $nb = new stdClass();
        $success = ParseIfBlock($tokens, $position, $nb, $errorMessage);
        LinkedListAddNode($node->nodes, $nb);
      }else if(StringsEqual($tn->type, $literal = str_split("foreach"))){
        $nb = new stdClass();
        $success = ParseForeachBlock($tokens, $position, $nb, $errorMessage);
        LinkedListAddNode($node->nodes, $nb);
      }else{
        $done = true;
      }
    }
  }

  return $success;
}
function ParseUseBlock(&$tokens, $position, $useBlock, $errorMessage){

  $n = new stdClass();
  ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

  $useBlock->type = CopyString($n->type);
  $useBlock->p1 = CopyString($n->p1);
  $useBlock->block1 = new stdClass();

  $position->numberValue = $position->numberValue + 1.0;

  $success = ParseTemplateBlock($tokens, $position, $useBlock->block1, $errorMessage);

  if($success){
    if($position->numberValue < count($tokens)){
      $n = new stdClass();
      ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

      if(StringsEqual($n->type, $literal = str_split("end"))){
        $success = true;
        $position->numberValue = $position->numberValue + 1.0;
      }else{
        $success = false;
        $errorMessage->string = str_split("End block expected at the end of use block.");
      }
    }else{
      $success = false;
      $errorMessage->string = str_split("End block expected at the end of use block.");
    }
  }

  return $success;
}
function ParseIfBlock(&$tokens, $position, $ifBlock, $errorMessage){

  $n = new stdClass();
  ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

  $ifBlock->type = CopyString($n->type);
  $ifBlock->p1 = CopyString($n->p1);
  $ifBlock->block1 = new stdClass();
  $ifBlock->hasElseBlock = false;

  $position->numberValue = $position->numberValue + 1.0;

  $success = ParseTemplateBlock($tokens, $position, $ifBlock->block1, $errorMessage);

  if($success){
    if($position->numberValue < count($tokens)){
      $n = new stdClass();
      ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

      if(StringsEqual($n->type, $literal = str_split("end"))){
        $success = true;
        $position->numberValue = $position->numberValue + 1.0;
      }else if(StringsEqual($n->type, $literal = str_split("else"))){
        $position->numberValue = $position->numberValue + 1.0;
        $ifBlock->hasElseBlock = true;
        $ifBlock->block2 = new stdClass();
        $success = ParseTemplateBlock($tokens, $position, $ifBlock->block2, $errorMessage);

        if($success){
          if($position->numberValue < count($tokens)){
            $n = new stdClass();
            ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

            if(StringsEqual($n->type, $literal = str_split("end"))){
              $success = true;
              $position->numberValue = $position->numberValue + 1.0;
            }else{
              $success = false;
              $errorMessage->string = str_split("End block expected at the end of else block.");
            }
          }else{
            $success = false;
            $errorMessage->string = str_split("End block expected at the end of else block.");
          }
        }
      }else{
        $success = false;
        $errorMessage->string = str_split("End or else block expected at the end of if block.");
      }
    }else{
      $success = false;
      $errorMessage->string = str_split("End or else block expected at the end of if block.");
    }
  }

  return $success;
}
function ParseForeachBlock(&$tokens, $position, $foreachBlock, $errorMessage){

  $n = new stdClass();
  ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

  $foreachBlock->type = CopyString($n->type);
  $foreachBlock->p1 = CopyString($n->p1);
  $foreachBlock->p2 = CopyString($n->p2);
  $foreachBlock->block1 = new stdClass();

  $position->numberValue = $position->numberValue + 1.0;

  $success = ParseTemplateBlock($tokens, $position, $foreachBlock->block1, $errorMessage);

  if($success){
    if($position->numberValue < count($tokens)){
      $n = new stdClass();
      ParseNodeString($tokens[$position->numberValue]->string, $n, $errorMessage);

      if(StringsEqual($n->type, $literal = str_split("end"))){
        $success = true;
        $position->numberValue = $position->numberValue + 1.0;
      }else{
        $success = false;
        $errorMessage->string = str_split("End block expected at the end of for each block.");
      }
    }else{
      $success = false;
      $errorMessage->string = str_split("End block expected at the end of foreach block.");
    }
  }

  return $success;
}
function ParseNodeString(&$token, $node, $errorMessage){

  $success = true;
  $isText = false;

  if(count($token) <= 2.0){
    $isText = true;
  }else if($token[0.0] == "\\" && $token[1.0] == "{"){
    $isText = true;
  }else if($token[0.0] != "{"){
    $isText = true;
  }else{
    $command = strSubstring($token, 1.0, count($token) - 1.0);
    $parts = sSplitByCharacter($command, " ");

    if(count($command) > 0.0){
      if(StringsEqual($parts[0.0]->string, $literal = str_split("use"))){
        if(count($parts) == 2.0){
          $node->type = CopyString($parts[0.0]->string);
          $node->p1 = CopyString($parts[1.0]->string);
        }else{
          $success = false;
          $errorMessage->string = str_split("The use command takes one parameter.");
        }
      }else if(StringsEqual($parts[0.0]->string, $literal = str_split("end"))){
        if(count($parts) == 1.0){
          $node->type = CopyString($parts[0.0]->string);
        }else{
          $success = false;
          $errorMessage->string = str_split("The end command takes no parameters.");
        }
      }else if(StringsEqual($parts[0.0]->string, $literal = str_split("print"))){
        if(count($parts) == 2.0){
          $node->type = CopyString($parts[0.0]->string);
          $node->p1 = CopyString($parts[1.0]->string);
        }else{
          $success = false;
          $errorMessage->string = str_split("The print command takes one parameter.");
        }
      }else if(StringsEqual($parts[0.0]->string, $literal = str_split("foreach"))){
        if(count($parts) == 4.0){
          if(StringsEqual($parts[2.0]->string, $literal = str_split("in"))){
            $node->type = CopyString($parts[0.0]->string);
            $node->p1 = CopyString($parts[1.0]->string);
            $node->p2 = CopyString($parts[3.0]->string);
          }else{
            $success = false;
            $errorMessage->string = str_split("The foreach command must have \"in\" after the first parameter.");
          }
        }else{
          $success = false;
          $errorMessage->string = str_split("The foreach command takes three parameters.");
        }
      }else if(StringsEqual($parts[0.0]->string, $literal = str_split("if"))){
        if(count($parts) == 2.0){
          $node->type = CopyString($parts[0.0]->string);
          $node->p1 = CopyString($parts[1.0]->string);
        }else{
          $success = false;
          $errorMessage->string = str_split("The if command takes one parameter.");
        }
      }else if(StringsEqual($parts[0.0]->string, $literal = str_split("else"))){
        if(count($parts) == 1.0){
          $node->type = CopyString($parts[0.0]->string);
        }else{
          $success = false;
          $errorMessage->string = str_split("The else command takes no parameters.");
        }
      }else{
        $isText = true;
      }
    }else{
      $isText = true;
    }
  }

  if($isText){
    $node->type = str_split("text");
    $node->p1 = sReplaceString($token, $literal = str_split("\\{print "), $literal = str_split("{print "));
    $node->p1 = sReplaceString($node->p1, $literal = str_split("\\{use "), $literal = str_split("{use "));
    $node->p1 = sReplaceString($node->p1, $literal = str_split("\\{if "), $literal = str_split("{if "));
    $node->p1 = sReplaceString($node->p1, $literal = str_split("\\{end}"), $literal = str_split("{end}"));
    $node->p1 = sReplaceString($node->p1, $literal = str_split("\\{foreach "), $literal = str_split("{foreach "));
    $node->p1 = sReplaceString($node->p1, $literal = str_split("\\{else}"), $literal = str_split("{else}"));
  }

  return $success;
}
function test(){

  $failures = CreateNumberReference(0.0);

  testTokenGeneration($failures);

  testGenerateDocument1($failures);
  testGenerateDocument2($failures);
  testGenerateDocument3($failures);
  testGenerateDocument4($failures);
  testGenerateDocument5($failures);
  testGenerateDocument6($failures);
  testGenerateDocument7($failures);

  return $failures->numberValue;
}
function testTokenGeneration($failures){

  $errorMessage = new stdClass();

  $tokens = CreateLinkedListString();
  $template = str_split("This is a template, this is a value: {print a}.");
  $success = GenerateTokensFromTemplate($template, $tokens, $errorMessage);
  AssertTrue($success, $failures);
  AssertEquals(3.0, LinkedListStringsLength($tokens), $failures);

  $tokens = CreateLinkedListString();
  $template = str_split("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.");
  $success = GenerateTokensFromTemplate($template, $tokens, $errorMessage);
  AssertTrue($success, $failures);
  AssertEquals(9.0, LinkedListStringsLength($tokens), $failures);

  $tokens = CreateLinkedListString();
  $template = str_split("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.");
  $success = GenerateTokensFromTemplate($template, $tokens, $errorMessage);
  AssertTrue($success, $failures);
  AssertEquals(16.0, LinkedListStringsLength($tokens), $failures);

  $tokens = CreateLinkedListString();
  $template = str_split("T: {foreach a in b}{print a}{end}.");
  $success = GenerateTokensFromTemplate($template, $tokens, $errorMessage);
  AssertTrue($success, $failures);
  AssertEquals(5.0, LinkedListStringsLength($tokens), $failures);
}
function testGenerateDocument1($failures){
  AssertTemplateResult($literal = str_split("This is a template, this is a value: {print a}."), $literal = str_split("{\"c\": 5, \"a\": 6}"), $literal = str_split("This is a template, this is a value: 6."), $failures);
}
function testGenerateDocument2($failures){
  AssertTemplateResult($literal = str_split("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}."), $literal = str_split("{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}"), $literal = str_split("This is a template, this is a value: 6 1 2."), $failures);
}
function testGenerateDocument3($failures){
  AssertTemplateResult($literal = str_split("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}."), $literal = str_split("{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}"), $literal = str_split("This is a template, this is a value: 6 1 23 416."), $failures);
}
function testGenerateDocument4($failures){
  AssertTemplateResult($literal = str_split("T: {if a}a{end}."), $literal = str_split("{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}"), $literal = str_split("T: a."), $failures);

  AssertTemplateResult($literal = str_split("T: {if a}a{else}b{end}."), $literal = str_split("{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}"), $literal = str_split("T: b."), $failures);
}
function testGenerateDocument5($failures){
  AssertTemplateResult($literal = str_split("T: {foreach a in b}{print a}{end}."), $literal = str_split("{\"b\": [1, 2, 3, 4]}"), $literal = str_split("T: 1234."), $failures);
}
function testGenerateDocument6($failures){
  AssertTemplateResult($literal = str_split("{test}\\{print a}."), $literal = str_split("{\"c\": 5, \"a\": 6}"), $literal = str_split("{test}{print a}."), $failures);
}
function testGenerateDocument7($failures){
  AssertTemplateResult($literal = str_split("{"), $literal = str_split("{}"), $literal = str_split("{"), $failures);

  AssertTemplateResult($literal = str_split("{  "), $literal = str_split("{}"), $literal = str_split("{  "), $failures);

  AssertTemplateResult($literal = str_split("{use a}\\{print a}{end}"), $literal = str_split("{\"a\": {}}"), $literal = str_split("{print a}"), $failures);

  AssertTemplateResult($literal = str_split("\\{print a}{print a}}"), $literal = str_split("{\"a\": 3}"), $literal = str_split("{print a}3}"), $failures);

  AssertTemplateResult($literal = str_split("\\\\{print a}{print a}}"), $literal = str_split("{\"a\": 3}"), $literal = str_split("\\{print a}3}"), $failures);

  AssertTemplateResult($literal = str_split("\\\\{print a}{print a}\\\\{print a}}"), $literal = str_split("{\"a\": 3}"), $literal = str_split("\\{print a}3\\{print a}}"), $failures);

  AssertTemplateResult($literal = str_split("\\{print a}{print a}\\{"), $literal = str_split("{\"a\": 3}"), $literal = str_split("{print a}3\\{"), $failures);

  AssertTemplateResult($literal = str_split("    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>"), $literal = str_split("{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}"), $literal = str_split("    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>"), $failures);

  AssertTemplateError($literal = str_split("{print"), $literal = str_split("{}"), $literal = str_split("Template command found, but not ended properly."), $failures);

  AssertTemplateError($literal = str_split("{print a}"), $literal = str_split("{}"), $literal = str_split("Key for printing not found in JSON object: a"), $failures);

  AssertTemplateError($literal = str_split("This is a template, this is a value: {print a {print a}."), $literal = str_split("{\"a\": 5}"), $literal = str_split("The print command takes one parameter."), $failures);

  AssertTemplateError($literal = str_split("This is a {use a}\\{print a}template, this is a value: {print a}.{end}"), $literal = str_split("{\"a\": 5}"), $literal = str_split("Data structure for print command is not a JSON object."), $failures);

  AssertTemplateError($literal = str_split("{use a}"), $literal = str_split("{}"), $literal = str_split("End block expected at the end of use block."), $failures);

  AssertTemplateError($literal = str_split("{if a}"), $literal = str_split("{\"a\": true}"), $literal = str_split("End or else block expected at the end of if block."), $failures);

  AssertTemplateError($literal = str_split("{if a}{else}"), $literal = str_split("{\"a\": true}"), $literal = str_split("End block expected at the end of else block."), $failures);

  AssertTemplateError($literal = str_split("{foreach x in a}"), $literal = str_split("{\"a\": [1, 2, 3, 4]}"), $literal = str_split("End block expected at the end of foreach block."), $failures);
}
function AssertTemplateResult(&$template, &$json, &$result, $failures){

  $data = new stdClass();
  $errorMessages = new stdClass();
  $document = new stdClass();
  $errorMessage = new stdClass();

  $success = ReadJSON($json, $data, $errorMessages);

  AssertTrue($success, $failures);

  if($success){
    $success = GenerateDocument($template, $data->element, $document, $errorMessage);

    AssertTrue($success, $failures);

    if($success){
      AssertStringEquals($document->string, $result, $failures);
    }
  }
}
function AssertTemplateError(&$template, &$json, &$errorMessage, $failures){

  $data = new stdClass();
  $errorMessages = new stdClass();
  $document = new stdClass();
  $errorMessageRef = new stdClass();

  $success = ReadJSON($json, $data, $errorMessages);

  AssertTrue($success, $failures);

  if($success){
    $success = GenerateDocument($template, $data->element, $document, $errorMessageRef);

    AssertFalse($success, $failures);

    if( !$success ){
      AssertStringEquals($errorMessage, $errorMessageRef->string, $failures);
    }
  }
}
function CreateBooleanReference($value){

  $ref = new stdClass();
  $ref->booleanValue = $value;

  return $ref;
}
function CreateBooleanArrayReference(&$value){

  $ref = new stdClass();
  $ref->booleanArray = $value;

  return $ref;
}
function CreateBooleanArrayReferenceLengthValue($length, $value){

  $ref = new stdClass();
  $ref->booleanArray = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $ref->booleanArray[$i] = $value;
  }

  return $ref;
}
function FreeBooleanArrayReference($booleanArrayReference){
  unset($booleanArrayReference->booleanArray);
  unset($booleanArrayReference);
}
function CreateCharacterReference($value){

  $ref = new stdClass();
  $ref->characterValue = $value;

  return $ref;
}
function CreateNumberReference($value){

  $ref = new stdClass();
  $ref->numberValue = $value;

  return $ref;
}
function CreateNumberArrayReference(&$value){

  $ref = new stdClass();
  $ref->numberArray = $value;

  return $ref;
}
function CreateNumberArrayReferenceLengthValue($length, $value){

  $ref = new stdClass();
  $ref->numberArray = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $ref->numberArray[$i] = $value;
  }

  return $ref;
}
function FreeNumberArrayReference($numberArrayReference){
  unset($numberArrayReference->numberArray);
  unset($numberArrayReference);
}
function CreateStringReference(&$value){

  $ref = new stdClass();
  $ref->string = $value;

  return $ref;
}
function CreateStringReferenceLengthValue($length, $value){

  $ref = new stdClass();
  $ref->string = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $ref->string[$i] = $value;
  }

  return $ref;
}
function FreeStringReference($stringReference){
  unset($stringReference->string);
  unset($stringReference);
}
function CreateStringArrayReference(&$strings){

  $ref = new stdClass();
  $ref->stringArray = $strings;

  return $ref;
}
function CreateStringArrayReferenceLengthValue($length, &$value){

  $ref = new stdClass();
  $ref->stringArray = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $ref->stringArray[$i] = CreateStringReference($value);
  }

  return $ref;
}
function FreeStringArrayReference($stringArrayReference){

  for($i = 0.0; $i < count($stringArrayReference->stringArray); $i = $i + 1.0){
    unset($stringArrayReference->stringArray[$i]);
  }
  unset($stringArrayReference->stringArray);
  unset($stringArrayReference);
}
function IsValidJSON(&$json, $errorMessage){

  $elementReference = new stdClass();

  $success = ReadJSON($json, $elementReference, $errorMessage);

  if($success){
    DeleteElement($elementReference->element);
  }

  return $success;
}
function JSONTokenize(&$json, $tokensReference, $errorMessages){

  $ll = lCreateLinkedListString();
  $success = true;

  $stringLength = new stdClass();
  $tokenReference = new stdClass();

  for($i = 0.0; $i < count($json) && $success; ){
    $c = $json[$i];

    if($c == "{"){
      lLinkedListAddString($ll, $literal = str_split("{"));
      $i = $i + 1.0;
    }else if($c == "}"){
      lLinkedListAddString($ll, $literal = str_split("}"));
      $i = $i + 1.0;
    }else if($c == "["){
      lLinkedListAddString($ll, $literal = str_split("["));
      $i = $i + 1.0;
    }else if($c == "]"){
      lLinkedListAddString($ll, $literal = str_split("]"));
      $i = $i + 1.0;
    }else if($c == ":"){
      lLinkedListAddString($ll, $literal = str_split(":"));
      $i = $i + 1.0;
    }else if($c == ","){
      lLinkedListAddString($ll, $literal = str_split(","));
      $i = $i + 1.0;
    }else if($c == "f"){
      $success = GetJSONPrimitiveName($json, $i, $errorMessages, $literal = str_split("false"), $tokenReference);
      if($success){
        lLinkedListAddString($ll, $literal = str_split("false"));
        $i = $i + count(str_split("false"));
      }
    }else if($c == "t"){
      $success = GetJSONPrimitiveName($json, $i, $errorMessages, $literal = str_split("true"), $tokenReference);
      if($success){
        lLinkedListAddString($ll, $literal = str_split("true"));
        $i = $i + count(str_split("true"));
      }
    }else if($c == "n"){
      $success = GetJSONPrimitiveName($json, $i, $errorMessages, $literal = str_split("null"), $tokenReference);
      if($success){
        lLinkedListAddString($ll, $literal = str_split("null"));
        $i = $i + count(str_split("null"));
      }
    }else if($c == " " || $c == "\n" || $c == "\t" || $c == "\r"){
      /* Skip. */
      $i = $i + 1.0;
    }else if($c == "\""){
      $success = GetJSONString($json, $i, $tokenReference, $stringLength, $errorMessages);
      if($success){
        lLinkedListAddString($ll, $tokenReference->string);
        $i = $i + $stringLength->numberValue;
      }
    }else if(IsJSONNumberCharacter($c)){
      $success = GetJSONNumberToken($json, $i, $tokenReference, $errorMessages);
      if($success){
        lLinkedListAddString($ll, $tokenReference->string);
        $i = $i + count($tokenReference->string);
      }
    }else{
      $str = strConcatenateCharacter($literal = str_split("Invalid start of Token: "), $c);
      $stringReference = CreateStringReference($str);
      lAddStringRef($errorMessages, $stringReference);
      $i = $i + 1.0;
      $success = false;
    }
  }

  if($success){
    lLinkedListAddString($ll, $literal = str_split("<end>"));
    $tokensReference->stringArray = lLinkedListStringsToArray($ll);
    lFreeLinkedListString($ll);
  }

  return $success;
}
function GetJSONNumberToken(&$json, $start, $tokenReference, $errorMessages){

  $end = count($json);
  $done = false;

  for($i = $start; $i < count($json) &&  !$done ; $i = $i + 1.0){
    $c = $json[$i];
    if( !IsJSONNumberCharacter($c) ){
      $done = true;
      $end = $i;
    }
  }

  $numberString = strSubstring($json, $start, $end);

  $success = IsValidJSONNumber($numberString, $errorMessages);

  $tokenReference->string = $numberString;

  return $success;
}
function IsValidJSONNumber(&$n, $errorMessages){

  $i = 0.0;

  /* JSON allows an optional negative sign. */
  if($n[$i] == "-"){
    $i = $i + 1.0;
  }

  if($i < count($n)){
    $success = IsValidJSONNumberAfterSign($n, $i, $errorMessages);
  }else{
    $success = false;
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Number must contain at least one digit.")));
  }

  return $success;
}
function IsValidJSONNumberAfterSign(&$n, $i, $errorMessages){

  if(charIsNumber($n[$i])){
    /* 0 first means only 0. */
    if($n[$i] == "0"){
      $i = $i + 1.0;
    }else{
      /* 1-9 first, read following digits. */
      $i = IsValidJSONNumberAdvancePastDigits($n, $i);
    }

    if($i < count($n)){
      $success = IsValidJSONNumberFromDotOrExponent($n, $i, $errorMessages);
    }else{
      /* If integer, we are done now. */
      $success = true;
    }
  }else{
    $success = false;
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("A number must start with 0-9 (after the optional sign).")));
  }

  return $success;
}
function IsValidJSONNumberAdvancePastDigits(&$n, $i){

  $i = $i + 1.0;
  $done = false;
  for(; $i < count($n) &&  !$done ; ){
    if(charIsNumber($n[$i])){
      $i = $i + 1.0;
    }else{
      $done = true;
    }
  }

  return $i;
}
function IsValidJSONNumberFromDotOrExponent(&$n, $i, $errorMessages){

  $wasDotAndOrE = false;
  $success = true;

  if($n[$i] == "."){
    $i = $i + 1.0;
    $wasDotAndOrE = true;

    if($i < count($n)){
      if(charIsNumber($n[$i])){
        /* Read digits following decimal point. */
        $i = IsValidJSONNumberAdvancePastDigits($n, $i);

        if($i == count($n)){
          /* If non-scientific decimal number, we are done. */
          $success = true;
        }
      }else{
        $success = false;
        lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be numbers after the decimal point.")));
      }
    }else{
      $success = false;
      lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be numbers after the decimal point.")));
    }
  }

  if($i < count($n) && $success){
    if($n[$i] == "e" || $n[$i] == "E"){
      $wasDotAndOrE = true;
      $success = IsValidJSONNumberFromExponent($n, $i, $errorMessages);
    }else{
      $success = false;
      lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Expected e or E.")));
    }
  }else if($i == count($n) && $success){
    /* If number with decimal point. */
    $success = true;
  }else{
    $success = false;
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be numbers after the decimal point.")));
  }

  if($wasDotAndOrE){
  }else{
    $success = false;
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Exprected decimal point or e or E.")));
  }

  return $success;
}
function IsValidJSONNumberFromExponent(&$n, $i, $errorMessages){

  $i = $i + 1.0;

  if($i < count($n)){
    /* The exponent sign can either + or -, */
    if($n[$i] == "+" || $n[$i] == "-"){
      $i = $i + 1.0;
    }

    if($i < count($n)){
      if(charIsNumber($n[$i])){
        /* Read digits following decimal point. */
        $i = IsValidJSONNumberAdvancePastDigits($n, $i);

        if($i == count($n)){
          /* We found scientific number. */
          $success = true;
        }else{
          $success = false;
          lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There was characters following the exponent.")));
        }
      }else{
        $success = false;
        lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be a digit following the optional exponent sign.")));
      }
    }else{
      $success = false;
      lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be a digit following optional the exponent sign.")));
    }
  }else{
    $success = false;
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be a sign or a digit following e or E.")));
  }

  return $success;
}
function IsJSONNumberCharacter($c){

  $numericCharacters = str_split("0123456789.-+eE");

  $found = false;

  for($i = 0.0; $i < count($numericCharacters); $i = $i + 1.0){
    if($numericCharacters[$i] == $c){
      $found = true;
    }
  }

  return $found;
}
function GetJSONPrimitiveName(&$string, $start, $errorMessages, &$primitive, $tokenReference){

  $done = false;
  $success = true;

  $token = array();

  for($i = $start; $i < count($string) && (($i - $start) < count($primitive)) &&  !$done ; $i = $i + 1.0){
    $c = $string[$i];
    $p = $primitive[$i - $start];
    if($c == $p){
      /* OK */
      if(($i + 1.0 - $start) == count($primitive)){
        $done = true;
      }
    }else{
      $str = array();
      $str = strConcatenateString($str, $literal = str_split("Primitive invalid: "));
      $str = strAppendCharacter($str, $c);
      $str = strAppendString($str, $literal = str_split(" vs "));
      $str = strAppendCharacter($str, $p);

      lAddStringRef($errorMessages, CreateStringReference($str));
      $done = true;
      $success = false;
    }
  }

  if($done){
    if(StringsEqual($primitive, $literal = str_split("false"))){
      $token = str_split("false");
    }
    if(StringsEqual($primitive, $literal = str_split("true"))){
      $token = str_split("true");
    }
    if(StringsEqual($primitive, $literal = str_split("null"))){
      $token = str_split("null");
    }
  }else{
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Primitive invalid")));
    $success = false;
  }

  $tokenReference->string = $token;

  return $success;
}
function GetJSONString(&$json, $start, $tokenReference, $stringLengthReference, $errorMessages){

  $characterCount = CreateNumberReference(0.0);
  $hex = CreateString(4.0, "0");
  $hexReference = new stdClass();
  $errorMessage = new stdClass();

  $success = IsValidJSONStringInJSON($json, $start, $characterCount, $stringLengthReference, $errorMessages);

  if($success){
    $l = $characterCount->numberValue;
    $string = array_fill(0, $l, 0);

    $c = 0.0;
    $string[$c] = "\"";
    $c = $c + 1.0;

    $done = false;
    for($i = $start + 1.0;  !$done ; $i = $i + 1.0){
      if($json[$i] == "\\"){
        $i = $i + 1.0;
        if($json[$i] == "\"" || $json[$i] == "\\" || $json[$i] == "/"){
          $string[$c] = $json[$i];
          $c = $c + 1.0;
        }else if($json[$i] == "b"){
          $string[$c] = unichr(8.0);
          $c = $c + 1.0;
        }else if($json[$i] == "f"){
          $string[$c] = unichr(12.0);
          $c = $c + 1.0;
        }else if($json[$i] == "n"){
          $string[$c] = unichr(10.0);
          $c = $c + 1.0;
        }else if($json[$i] == "r"){
          $string[$c] = unichr(13.0);
          $c = $c + 1.0;
        }else if($json[$i] == "t"){
          $string[$c] = unichr(9.0);
          $c = $c + 1.0;
        }else if($json[$i] == "u"){
          $i = $i + 1.0;
          $hex[0.0] = charToUpperCase($json[$i + 0.0]);
          $hex[1.0] = charToUpperCase($json[$i + 1.0]);
          $hex[2.0] = charToUpperCase($json[$i + 2.0]);
          $hex[3.0] = charToUpperCase($json[$i + 3.0]);
          nCreateNumberFromStringWithCheck($hex, 16.0, $hexReference, $errorMessage);
          $string[$c] = unichr($hexReference->numberValue);
          $i = $i + 3.0;
          $c = $c + 1.0;
        }
      }else if($json[$i] == "\""){
        $string[$c] = $json[$i];
        $c = $c + 1.0;
        $done = true;
      }else{
        $string[$c] = $json[$i];
        $c = $c + 1.0;
      }
    }

    $tokenReference->string = $string;
    $success = true;
  }else{
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("End of string was not found.")));
    $success = false;
  }

  return $success;
}
function IsValidJSONString(&$jsonString, $errorMessages){

  $numberReference = new stdClass();
  $stringLength = new stdClass();

  $valid = IsValidJSONStringInJSON($jsonString, 0.0, $numberReference, $stringLength, $errorMessages);

  return $valid;
}
function IsValidJSONStringInJSON(&$json, $start, $characterCount, $stringLengthReference, $errorMessages){

  $success = true;
  $done = false;

  $characterCount->numberValue = 1.0;

  for($i = $start + 1.0; $i < count($json) &&  !$done  && $success; $i = $i + 1.0){
    if( !IsJSONIllegalControllCharacter($json[$i]) ){
      if($json[$i] == "\\"){
        $i = $i + 1.0;
        if($i < count($json)){
          if($json[$i] == "\"" || $json[$i] == "\\" || $json[$i] == "/" || $json[$i] == "b" || $json[$i] == "f" || $json[$i] == "n" || $json[$i] == "r" || $json[$i] == "t"){
            $characterCount->numberValue = $characterCount->numberValue + 1.0;
          }else if($json[$i] == "u"){
            if($i + 4.0 < count($json)){
              for($j = 0.0; $j < 4.0 && $success; $j = $j + 1.0){
                $c = $json[$i + $j + 1.0];
                if(nCharacterIsNumberCharacterInBase($c, 16.0) || $c == "a" || $c == "b" || $c == "c" || $c == "d" || $c == "e" || $c == "f"){
                }else{
                  $success = false;
                  lAddStringRef($errorMessages, CreateStringReference($literal = str_split("\\u must be followed by four hexadecimal digits.")));
                }
              }
              $characterCount->numberValue = $characterCount->numberValue + 1.0;
              $i = $i + 4.0;
            }else{
              $success = false;
              lAddStringRef($errorMessages, CreateStringReference($literal = str_split("\\u must be followed by four characters.")));
            }
          }else{
            $success = false;
            lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Escaped character invalid.")));
          }
        }else{
          $success = false;
          lAddStringRef($errorMessages, CreateStringReference($literal = str_split("There must be at least two character after string escape.")));
        }
      }else if($json[$i] == "\""){
        $characterCount->numberValue = $characterCount->numberValue + 1.0;
        $done = true;
      }else{
        $characterCount->numberValue = $characterCount->numberValue + 1.0;
      }
    }else{
      $success = false;
      lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Unicode code points 0-31 not allowed in JSON string.")));
    }
  }

  if($done){
    $stringLengthReference->numberValue = $i - $start;
  }else{
    $success = false;
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("String must end with \".")));
  }

  return $success;
}
function IsJSONIllegalControllCharacter($c){

  $cnr = uniord($c);

  if($cnr >= 0.0 && $cnr < 32.0){
    $isControll = true;
  }else{
    $isControll = false;
  }

  return $isControll;
}
function &AddElement(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);

  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;

  unset($list);

  return $newlist;
}
function AddElementRef($list, $i){
  $list->array = AddElement($list->array, $i);
}
function &RemoveElement(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    if($i < $n){
      $newlist[$i] = $list[$i];
    }
    if($i > $n){
      $newlist[$i - 1.0] = $list[$i];
    }
  }

  unset($list);

  return $newlist;
}
function GetElementRef($list, $i){
  return $list->array[$i];
}
function RemoveElementRef($list, $i){
  $list->array = RemoveElement($list->array, $i);
}
function CreateLinkedListElements(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function LinkedListAddElement($ll, $value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function &LinkedListElementsToArray($ll){

  $node = $ll->first;

  $length = LinkedListElementsLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = $node->value;
    $node = $node->next;
  }

  return $array;
}
function LinkedListElementsLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function FreeLinkedListElements($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function ComputeJSONStringLength($element){

  $result = 0.0;

  if(StringsEqual($element->type, $literal = str_split("object"))){
    $result = $result + ComputeJSONObjectStringLength($element);
  }else if(StringsEqual($element->type, $literal = str_split("string"))){
    $result = JSONEscapedStringLength($element->string) + 2.0;
  }else if(StringsEqual($element->type, $literal = str_split("array"))){
    $result = $result + ComputeJSONArrayStringLength($element);
  }else if(StringsEqual($element->type, $literal = str_split("number"))){
    $result = $result + ComputeJSONNumberStringLength($element);
  }else if(StringsEqual($element->type, $literal = str_split("null"))){
    $result = $result + count(str_split("null"));
  }else if(StringsEqual($element->type, $literal = str_split("boolean"))){
    $result = $result + ComputeJSONBooleanStringLength($element);
  }

  return $result;
}
function ComputeJSONBooleanStringLength($element){

  if($element->booleanValue){
    $result = count(str_split("true"));
  }else{
    $result = count(str_split("false"));
  }

  return $result;
}
function ComputeJSONNumberStringLength($element){

  if(abs($element->number) >= 10.0**15.0 || abs($element->number) <= 10.0**(-15.0)){
    $a = nCreateStringScientificNotationDecimalFromNumber($element->number);
    $length = count($a);
  }else{
    $a = nCreateStringDecimalFromNumber($element->number);
    $length = count($a);
  }

  return $length;
}
function ComputeJSONArrayStringLength($element){

  $length = 1.0;

  for($i = 0.0; $i < count($element->array); $i = $i + 1.0){
    $arrayElement = $element->array[$i];

    $length = $length + ComputeJSONStringLength($arrayElement);

    if($i + 1.0 != count($element->array)){
      $length = $length + 1.0;
    }
  }

  $length = $length + 1.0;

  return $length;
}
function ComputeJSONObjectStringLength($element){

  $length = 1.0;

  $keys = GetStringElementMapKeySet($element->object);
  for($i = 0.0; $i < count($keys->stringArray); $i = $i + 1.0){
    $key = $keys->stringArray[$i]->string;
    $objectElement = GetObjectValue($element->object, $key);

    $length = $length + 1.0;
    $length = $length + JSONEscapedStringLength($key);
    $length = $length + 1.0;
    $length = $length + 1.0;

    $length = $length + ComputeJSONStringLength($objectElement);

    if($i + 1.0 != count($keys->stringArray)){
      $length = $length + 1.0;
    }
  }

  $length = $length + 1.0;

  return $length;
}
function CreateStringElement(&$string){
  $element = new stdClass();
  $element->type = str_split("string");
  $element->string = $string;
  return $element;
}
function CreateBooleanElement($booleanValue){
  $element = new stdClass();
  $element->type = str_split("boolean");
  $element->booleanValue = $booleanValue;
  return $element;
}
function CreateNullElement(){
  $element = new stdClass();
  $element->type = str_split("null");
  return $element;
}
function CreateNumberElement($number){
  $element = new stdClass();
  $element->type = str_split("number");
  $element->number = $number;
  return $element;
}
function CreateArrayElement($length){
  $element = new stdClass();
  $element->type = str_split("array");
  $element->array = array_fill(0, $length, 0);
  return $element;
}
function CreateObjectElement($length){
  $element = new stdClass();
  $element->type = str_split("object");
  $element->object = new stdClass();
  $element->object->stringListRef = CreateStringArrayReferenceLengthValue($length, $literal = array());
  $element->object->elementListRef = new stdClass();
  $element->object->elementListRef->array = array_fill(0, $length, 0);
  return $element;
}
function DeleteElement($element){
  if(StringsEqual($element->type, $literal = str_split("object"))){
    DeleteObject($element);
  }else if(StringsEqual($element->type, $literal = str_split("string"))){
    unset($element);
  }else if(StringsEqual($element->type, $literal = str_split("array"))){
    DeleteArray($element);
  }else if(StringsEqual($element->type, $literal = str_split("number"))){
    unset($element);
  }else if(StringsEqual($element->type, $literal = str_split("null"))){
    unset($element);
  }else if(StringsEqual($element->type, $literal = str_split("boolean"))){
    unset($element);
  }else{
  }
}
function DeleteObject($element){

  $keys = GetStringElementMapKeySet($element->object);
  for($i = 0.0; $i < count($keys->stringArray); $i = $i + 1.0){
    $key = $keys->stringArray[$i]->string;
    $objectElement = GetObjectValue($element->object, $key);
    DeleteElement($objectElement);
  }
}
function DeleteArray($element){

  for($i = 0.0; $i < count($element->array); $i = $i + 1.0){
    $arrayElement = $element->array[$i];
    DeleteElement($arrayElement);
  }
}
function &WriteJSON($element){

  $length = ComputeJSONStringLength($element);
  $result = array_fill(0, $length, 0);
  $index = CreateNumberReference(0.0);

  if(StringsEqual($element->type, $literal = str_split("object"))){
    WriteObject($element, $result, $index);
  }else if(StringsEqual($element->type, $literal = str_split("string"))){
    WriteString($element, $result, $index);
  }else if(StringsEqual($element->type, $literal = str_split("array"))){
    WriteArray($element, $result, $index);
  }else if(StringsEqual($element->type, $literal = str_split("number"))){
    WriteNumber($element, $result, $index);
  }else if(StringsEqual($element->type, $literal = str_split("null"))){
    strWriteStringToStingStream($result, $index, $literal = str_split("null"));
  }else if(StringsEqual($element->type, $literal = str_split("boolean"))){
    WriteBooleanValue($element, $result, $index);
  }

  return $result;
}
function WriteBooleanValue($element, &$result, $index){
  if($element->booleanValue){
    strWriteStringToStingStream($result, $index, $literal = str_split("true"));
  }else{
    strWriteStringToStingStream($result, $index, $literal = str_split("false"));
  }
}
function WriteNumber($element, &$result, $index){

  if(abs($element->number) >= 10.0**15.0 || abs($element->number) <= 10.0**(-15.0)){
    $numberString = nCreateStringScientificNotationDecimalFromNumber($element->number);
  }else{
    $numberString = nCreateStringDecimalFromNumber($element->number);
  }

  strWriteStringToStingStream($result, $index, $numberString);
}
function WriteArray($element, &$result, $index){

  strWriteStringToStingStream($result, $index, $literal = str_split("["));

  for($i = 0.0; $i < count($element->array); $i = $i + 1.0){
    $arrayElement = $element->array[$i];

    $s = WriteJSON($arrayElement);
    strWriteStringToStingStream($result, $index, $s);

    if($i + 1.0 != count($element->array)){
      strWriteStringToStingStream($result, $index, $literal = str_split(","));
    }
  }

  strWriteStringToStingStream($result, $index, $literal = str_split("]"));
}
function WriteString($element, &$result, $index){
  strWriteStringToStingStream($result, $index, $literal = str_split("\""));
  $element->string = JSONEscapeString($element->string);
  strWriteStringToStingStream($result, $index, $element->string);
  strWriteStringToStingStream($result, $index, $literal = str_split("\""));
}
function &JSONEscapeString(&$string){

  $length = JSONEscapedStringLength($string);

  $ns = array_fill(0, $length, 0);
  $index = CreateNumberReference(0.0);
  $lettersReference = CreateNumberReference(0.0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    if(JSONMustBeEscaped($string[$i], $lettersReference)){
      $escaped = JSONEscapeCharacter($string[$i]);
      strWriteStringToStingStream($ns, $index, $escaped);
    }else{
      strWriteCharacterToStingStream($ns, $index, $string[$i]);
    }
  }

  return $ns;
}
function JSONEscapedStringLength(&$string){

  $lettersReference = CreateNumberReference(0.0);
  $length = 0.0;

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    if(JSONMustBeEscaped($string[$i], $lettersReference)){
      $length = $length + $lettersReference->numberValue;
    }else{
      $length = $length + 1.0;
    }
  }
  return $length;
}
function &JSONEscapeCharacter($c){

  $code = uniord($c);

  $q = 34.0;
  $rs = 92.0;
  $s = 47.0;
  $b = 8.0;
  $f = 12.0;
  $n = 10.0;
  $r = 13.0;
  $t = 9.0;

  $hexNumber = new stdClass();

  if($code == $q){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "\"";
  }else if($code == $rs){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "\\";
  }else if($code == $s){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "/";
  }else if($code == $b){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "b";
  }else if($code == $f){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "f";
  }else if($code == $n){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "n";
  }else if($code == $r){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "r";
  }else if($code == $t){
    $escaped = array_fill(0, 2.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "t";
  }else if($code >= 0.0 && $code <= 31.0){
    $escaped = array_fill(0, 6.0, 0);
    $escaped[0.0] = "\\";
    $escaped[1.0] = "u";
    $escaped[2.0] = "0";
    $escaped[3.0] = "0";

    nCreateStringFromNumberWithCheck($code, 16.0, $hexNumber);

    if(count($hexNumber->string) == 1.0){
      $escaped[4.0] = "0";
      $escaped[5.0] = $hexNumber->string[0.0];
    }else if(count($hexNumber->string) == 2.0){
      $escaped[4.0] = $hexNumber->string[0.0];
      $escaped[5.0] = $hexNumber->string[1.0];
    }
  }else{
    $escaped = array_fill(0, 1.0, 0);
    $escaped[0.0] = $c;
  }

  return $escaped;
}
function JSONMustBeEscaped($c, $letters){

  $code = uniord($c);
  $mustBeEscaped = false;

  $q = 34.0;
  $rs = 92.0;
  $s = 47.0;
  $b = 8.0;
  $f = 12.0;
  $n = 10.0;
  $r = 13.0;
  $t = 9.0;

  if($code == $q || $code == $rs || $code == $s || $code == $b || $code == $f || $code == $n || $code == $r || $code == $t){
    $mustBeEscaped = true;
    $letters->numberValue = 2.0;
  }else if($code >= 0.0 && $code <= 31.0){
    $mustBeEscaped = true;
    $letters->numberValue = 6.0;
  }else if($code >= 2.0**16.0){
    $mustBeEscaped = true;
    $letters->numberValue = 6.0;
  }

  return $mustBeEscaped;
}
function WriteObject($element, &$result, $index){

  strWriteStringToStingStream($result, $index, $literal = str_split("{"));

  $keys = GetStringElementMapKeySet($element->object);
  for($i = 0.0; $i < count($keys->stringArray); $i = $i + 1.0){
    $key = $keys->stringArray[$i]->string;
    $key = JSONEscapeString($key);
    $objectElement = GetObjectValue($element->object, $key);

    strWriteStringToStingStream($result, $index, $literal = str_split("\""));
    strWriteStringToStingStream($result, $index, $key);
    strWriteStringToStingStream($result, $index, $literal = str_split("\""));
    strWriteStringToStingStream($result, $index, $literal = str_split(":"));

    $s = WriteJSON($objectElement);
    strWriteStringToStingStream($result, $index, $s);

    if($i + 1.0 != count($keys->stringArray)){
      strWriteStringToStingStream($result, $index, $literal = str_split(","));
    }
  }

  strWriteStringToStingStream($result, $index, $literal = str_split("}"));
}
function ReadJSON(&$string, $elementReference, $errorMessages){

  /* Tokenize. */
  $tokenArrayReference = new stdClass();
  $success = JSONTokenize($string, $tokenArrayReference, $errorMessages);

  if($success){
    /* Parse. */
    $success = GetJSONValue($tokenArrayReference->stringArray, $elementReference, $errorMessages);
  }

  return $success;
}
function GetJSONValue(&$tokens, $elementReference, $errorMessages){

  $i = CreateNumberReference(0.0);
  $success = GetJSONValueRecursive($tokens, $i, 0.0, $elementReference, $errorMessages);

  return $success;
}
function GetJSONValueRecursive(&$tokens, $i, $depth, $elementReference, $errorMessages){

  $success = true;
  $token = $tokens[$i->numberValue]->string;

  if(StringsEqual($token, $literal = str_split("{"))){
    $success = GetJSONObject($tokens, $i, $depth + 1.0, $elementReference, $errorMessages);
  }else if(StringsEqual($token, $literal = str_split("["))){
    $success = GetJSONArray($tokens, $i, $depth + 1.0, $elementReference, $errorMessages);
  }else if(StringsEqual($token, $literal = str_split("true"))){
    $elementReference->element = CreateBooleanElement(true);
    $i->numberValue = $i->numberValue + 1.0;
  }else if(StringsEqual($token, $literal = str_split("false"))){
    $elementReference->element = CreateBooleanElement(false);
    $i->numberValue = $i->numberValue + 1.0;
  }else if(StringsEqual($token, $literal = str_split("null"))){
    $elementReference->element = CreateNullElement();
    $i->numberValue = $i->numberValue + 1.0;
  }else if(charIsNumber($token[0.0]) || $token[0.0] == "-"){
    $stringToDecimalResult = nCreateNumberFromDecimalString($token);
    $elementReference->element = CreateNumberElement($stringToDecimalResult);
    $i->numberValue = $i->numberValue + 1.0;
  }else if($token[0.0] == "\""){
    $substr = strSubstring($token, 1.0, count($token) - 1.0);
    $elementReference->element = CreateStringElement($substr);
    $i->numberValue = $i->numberValue + 1.0;
  }else{
    $str = array();
    $str = strConcatenateString($str, $literal = str_split("Invalid token first in value: "));
    $str = strAppendString($str, $token);
    lAddStringRef($errorMessages, CreateStringReference($str));
    $success = false;
  }

  if($success && $depth == 0.0){
    if(StringsEqual($tokens[$i->numberValue]->string, $literal = str_split("<end>"))){
    }else{
      lAddStringRef($errorMessages, CreateStringReference($literal = str_split("The outer value cannot have any tokens following it.")));
      $success = false;
    }
  }

  return $success;
}
function GetJSONObject(&$tokens, $i, $depth, $elementReference, $errorMessages){

  $keys = lCreateLinkedListString();
  $values = CreateLinkedListElements();
  $element = CreateObjectElement(0.0);
  $valueReference = new stdClass();
  $success = true;
  $i->numberValue = $i->numberValue + 1.0;

  if( !StringsEqual($tokens[$i->numberValue]->string, $literal = str_split("}")) ){
    $done = false;

    for(;  !$done  && $success; ){
      $key = $tokens[$i->numberValue]->string;

      if($key[0.0] == "\""){
        $i->numberValue = $i->numberValue + 1.0;
        $colon = $tokens[$i->numberValue]->string;
        if(StringsEqual($colon, $literal = str_split(":"))){
          $i->numberValue = $i->numberValue + 1.0;
          $success = GetJSONValueRecursive($tokens, $i, $depth, $valueReference, $errorMessages);

          if($success){
            $keystring = strSubstring($key, 1.0, count($key) - 1.0);
            $value = $valueReference->element;
            lLinkedListAddString($keys, $keystring);
            LinkedListAddElement($values, $value);

            $comma = $tokens[$i->numberValue]->string;
            if(StringsEqual($comma, $literal = str_split(","))){
              /* OK */
              $i->numberValue = $i->numberValue + 1.0;
            }else{
              $done = true;
            }
          }
        }else{
          $str = array();
          $str = strConcatenateString($str, $literal = str_split("Expected colon after key in object: "));
          $str = strAppendString($str, $colon);
          lAddStringRef($errorMessages, CreateStringReference($str));

          $success = false;
          $done = true;
        }
      }else{
        lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Expected string as key in object.")));

        $done = true;
        $success = false;
      }
    }
  }

  if($success){
    $closeCurly = $tokens[$i->numberValue]->string;

    if(StringsEqual($closeCurly, $literal = str_split("}"))){
      /* OK */
      unset($element->object->stringListRef->stringArray);
      unset($element->object->elementListRef->array);
      $element->object->stringListRef->stringArray = lLinkedListStringsToArray($keys);
      $element->object->elementListRef->array = LinkedListElementsToArray($values);
      $elementReference->element = $element;
      $i->numberValue = $i->numberValue + 1.0;
    }else{
      lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Expected close curly brackets at end of object value.")));
      $success = false;
    }
  }

  lFreeLinkedListString($keys);
  FreeLinkedListElements($values);
  unset($valueReference);

  return $success;
}
function GetJSONArray(&$tokens, $i, $depth, $elementReference, $errorMessages){

  $elements = CreateLinkedListElements();
  $i->numberValue = $i->numberValue + 1.0;

  $valueReference = new stdClass();
  $success = true;
  $element = CreateArrayElement(0.0);

  $nextToken = $tokens[$i->numberValue]->string;

  if( !StringsEqual($nextToken, $literal = str_split("]")) ){
    $done = false;
    for(;  !$done  && $success; ){
      $success = GetJSONValueRecursive($tokens, $i, $depth, $valueReference, $errorMessages);

      if($success){
        $value = $valueReference->element;
        LinkedListAddElement($elements, $value);

        $comma = $tokens[$i->numberValue]->string;

        if(StringsEqual($comma, $literal = str_split(","))){
          /* OK */
          $i->numberValue = $i->numberValue + 1.0;
        }else{
          $done = true;
        }
      }
    }
  }

  $nextToken = $tokens[$i->numberValue]->string;
  if(StringsEqual($nextToken, $literal = str_split("]"))){
    /* OK */
    $i->numberValue = $i->numberValue + 1.0;
    unset($element->array);
    $element->array = LinkedListElementsToArray($elements);
  }else{
    lAddStringRef($errorMessages, CreateStringReference($literal = str_split("Expected close square bracket at end of array.")));
    $success = false;
  }

  $elementReference->element = $element;
  FreeLinkedListElements($elements);
  unset($valueReference);

  return $success;
}
function GetStringElementMapKeySet($stringElementMap){
  return $stringElementMap->stringListRef;
}
function GetObjectValue($stringElementMap, &$key){

  $result = new stdClass();

  for($i = 0.0; $i < GetStringElementMapNumberOfKeys($stringElementMap); $i = $i + 1.0){
    if(StringsEqual($stringElementMap->stringListRef->stringArray[$i]->string, $key)){
      $result = $stringElementMap->elementListRef->array[$i];
    }
  }

  return $result;
}
function GetObjectValueWithCheck($stringElementMap, &$key, $foundReference){

  $result = new stdClass();

  $foundReference->booleanValue = false;
  for($i = 0.0; $i < GetStringElementMapNumberOfKeys($stringElementMap); $i = $i + 1.0){
    if(StringsEqual($stringElementMap->stringListRef->stringArray[$i]->string, $key)){
      $result = $stringElementMap->elementListRef->array[$i];
      $foundReference->booleanValue = true;
    }
  }

  return $result;
}
function PutStringElementMap($stringElementMap, &$keystring, $value){
  lAddStringRef($stringElementMap->stringListRef, CreateStringReference($keystring));
  AddElementRef($stringElementMap->elementListRef, $value);
}
function SetStringElementMap($stringElementMap, $index, &$keystring, $value){
  $stringElementMap->stringListRef->stringArray[$index]->string = $keystring;
  $stringElementMap->elementListRef->array[$index] = $value;
}
function GetStringElementMapNumberOfKeys($stringElementMap){
  return count($stringElementMap->stringListRef->stringArray);
}
function JSONCompare(&$a, &$b, $epsilon, $equal, $errorMessage){

  $eaRef = new stdClass();
  $ebRef = new stdClass();

  $success = ReadJSON($a, $eaRef, $errorMessage);

  if($success){
    $ea = $eaRef->element;

    $success = ReadJSON($b, $ebRef, $errorMessage);

    if($success){
      $eb = $ebRef->element;

      $equal->booleanValue = JSONCompareElements($ea, $eb, $epsilon);

      DeleteElement($eb);
    }

    DeleteElement($ea);
  }

  return $success;
}
function JSONCompareElements($ea, $eb, $epsilon){

  $equal = StringsEqual($ea->type, $eb->type);
        
  if($equal){
    $typeName = $ea->type;
    if(StringsEqual($typeName, $literal = str_split("object"))){
      $equal = JSONCompareObjects($ea->object, $eb->object, $epsilon);
    }else if(StringsEqual($typeName, $literal = str_split("string"))){
      $equal = StringsEqual($ea->string, $eb->string);
    }else if(StringsEqual($typeName, $literal = str_split("array"))){
      $equal = JSONCompareArrays($ea->array, $eb->array, $epsilon);
    }else if(StringsEqual($typeName, $literal = str_split("number"))){
      $equal = EpsilonCompare($ea->number, $eb->number, $epsilon);
    }else if(StringsEqual($typeName, $literal = str_split("null"))){
      $equal = true;
    }else if(StringsEqual($typeName, $literal = str_split("boolean"))){
      $equal = $ea->booleanValue == $eb->booleanValue;
    }
  }
        
  return $equal;
}
function JSONCompareArrays(&$ea, &$eb, $epsilon){

  $equals = count($ea) == count($eb);

  if($equals){
    $length = count($ea);
    for($i = 0.0; $i < $length && $equals; $i = $i + 1.0){
      $equals = JSONCompareElements($ea[$i], $eb[$i], $epsilon);
    }
  }

  return $equals;
}
function JSONCompareObjects($ea, $eb, $epsilon){

  $aFoundReference = new stdClass();
  $bFoundReference = new stdClass();

  $akeys = GetStringElementMapNumberOfKeys($ea);
  $bkeys = GetStringElementMapNumberOfKeys($eb);

  $equals = $akeys == $bkeys;

  if($equals){
    $keys = GetStringElementMapKeySet($ea);

    for($i = 0.0; $i < count($keys->stringArray) && $equals; $i = $i + 1.0){
      $key = $keys->stringArray[$i]->string;

      $eaValue = GetObjectValueWithCheck($ea, $key, $aFoundReference);
      $ebValue = GetObjectValueWithCheck($eb, $key, $bFoundReference);

      if($aFoundReference->booleanValue && $bFoundReference->booleanValue){
        $equals = JSONCompareElements($eaValue, $ebValue, $epsilon);
      }else{
        $equals = false;
      }
    }
  }

  return $equals;
}
function &AddNumber(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);
  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function AddNumberRef($list, $i){
  $list->numberArray = AddNumber($list->numberArray, $i);
}
function &RemoveNumber(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }
		
  return $newlist;
}
function GetNumberRef($list, $i){
  return $list->numberArray[$i];
}
function RemoveNumberRef($list, $i){
  $list->numberArray = RemoveNumber($list->numberArray, $i);
}
function &AddString(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);

  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function AddStringRef($list, $i){
  $list->stringArray = AddString($list->stringArray, $i);
}
function &RemoveString(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }
		
  return $newlist;
}
function GetStringRef($list, $i){
  return $list->stringArray[$i];
}
function RemoveStringRef($list, $i){
  $list->stringArray = RemoveString($list->stringArray, $i);
}
function CreateDynamicArrayCharacters(){

  $da = new stdClass();
  $da->array = array_fill(0, 10.0, 0);
  $da->length = 0.0;

  return $da;
}
function CreateDynamicArrayCharactersWithInitialCapacity($capacity){

  $da = new stdClass();
  $da->array = array_fill(0, $capacity, 0);
  $da->length = 0.0;

  return $da;
}
function DynamicArrayAddCharacter($da, $value){
  if($da->length == count($da->array)){
    DynamicArrayCharactersIncreaseSize($da);
  }

  $da->array[$da->length] = $value;
  $da->length = $da->length + 1.0;
}
function DynamicArrayCharactersIncreaseSize($da){

  $newLength = round(count($da->array)*3.0/2.0);
  $newArray = array_fill(0, $newLength, 0);

  for($i = 0.0; $i < count($da->array); $i = $i + 1.0){
    $newArray[$i] = $da->array[$i];
  }

  unset($da->array);

  $da->array = $newArray;
}
function DynamicArrayCharactersDecreaseSizeNecessary($da){

  $needsDecrease = false;

  if($da->length > 10.0){
    $needsDecrease = $da->length <= round(count($da->array)*2.0/3.0);
  }

  return $needsDecrease;
}
function DynamicArrayCharactersDecreaseSize($da){

  $newLength = round(count($da->array)*2.0/3.0);
  $newArray = array_fill(0, $newLength, 0);

  for($i = 0.0; $i < $newLength; $i = $i + 1.0){
    $newArray[$i] = $da->array[$i];
  }

  unset($da->array);

  $da->array = $newArray;
}
function DynamicArrayCharactersIndex($da, $index){
  return $da->array[$index];
}
function DynamicArrayCharactersLength($da){
  return $da->length;
}
function DynamicArrayInsertCharacter($da, $index, $value){

  if($da->length == count($da->array)){
    DynamicArrayCharactersIncreaseSize($da);
  }

  for($i = $da->length; $i > $index; $i = $i - 1.0){
    $da->array[$i] = $da->array[$i - 1.0];
  }

  $da->array[$index] = $value;

  $da->length = $da->length + 1.0;
}
function DynamicArrayCharacterSet($da, $index, $value){

  if($index < $da->length){
    $da->array[$index] = $value;
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function DynamicArrayRemoveCharacter($da, $index){

  for($i = $index; $i < $da->length - 1.0; $i = $i + 1.0){
    $da->array[$i] = $da->array[$i + 1.0];
  }

  $da->length = $da->length - 1.0;

  if(DynamicArrayCharactersDecreaseSizeNecessary($da)){
    DynamicArrayCharactersDecreaseSize($da);
  }
}
function FreeDynamicArrayCharacters($da){
  unset($da->array);
  unset($da);
}
function &DynamicArrayCharactersToArray($da){

  $array = array_fill(0, $da->length, 0);

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    $array[$i] = $da->array[$i];
  }

  return $array;
}
function ArrayToDynamicArrayCharactersWithOptimalSize(&$array){

  $c = count($array);
  $n = (log($c) - 1.0)/log(3.0/2.0);
  $newCapacity = floor($n) + 1.0;

  $da = CreateDynamicArrayCharactersWithInitialCapacity($newCapacity);

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
    $da->array[$i] = $array[$i];
  }

  return $da;
}
function ArrayToDynamicArrayCharacters(&$array){

  $da = new stdClass();
  $da->array = CopyString($array);
  $da->length = count($array);

  return $da;
}
function DynamicArrayCharactersEqual($a, $b){

  $equal = true;
  if($a->length == $b->length){
    for($i = 0.0; $i < $a->length && $equal; $i = $i + 1.0){
      if($a->array[$i] != $b->array[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function DynamicArrayCharactersToLinkedList($da){

  $ll = CreateLinkedListCharacter();

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    LinkedListAddCharacter($ll, $da->array[$i]);
  }

  return $ll;
}
function LinkedListToDynamicArrayCharacters($ll){

  $node = $ll->first;

  $da = new stdClass();
  $da->length = LinkedListCharactersLength($ll);

  $da->array = array_fill(0, $da->length, 0);

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    $da->array[$i] = $node->value;
    $node = $node->next;
  }

  return $da;
}
function &AddBoolean(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);
  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function AddBooleanRef($list, $i){
  $list->booleanArray = AddBoolean($list->booleanArray, $i);
}
function &RemoveBoolean(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }
		
  return $newlist;
}
function GetBooleanRef($list, $i){
  return $list->booleanArray[$i];
}
function RemoveDecimalRef($list, $i){
  $list->booleanArray = RemoveBoolean($list->booleanArray, $i);
}
function CreateLinkedListString(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function LinkedListAddString($ll, &$value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function &LinkedListStringsToArray($ll){

  $node = $ll->first;

  $length = LinkedListStringsLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = new stdClass();
    $array[$i]->string = $node->value;
    $node = $node->next;
  }

  return $array;
}
function LinkedListStringsLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function FreeLinkedListString($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function CreateLinkedListNumbers(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function &CreateLinkedListNumbersArray($length){

  $lls = array_fill(0, $length, 0);
  for($i = 0.0; $i < count($lls); $i = $i + 1.0){
    $lls[$i] = CreateLinkedListNumbers();
  }

  return $lls;
}
function LinkedListAddNumber($ll, $value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function LinkedListNumbersLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function LinkedListNumbersIndex($ll, $index){

  $node = $ll->first;
  for($i = 0.0; $i < $index; $i = $i + 1.0){
    $node = $node->next;
  }

  return $node->value;
}
function LinkedListInsertNumber($ll, $index, $value){

  if($index == 0.0){
    $tmp = $ll->first;
    $ll->first = new stdClass();
    $ll->first->next = $tmp;
    $ll->first->value = $value;
    $ll->first->end = false;
  }else{
    $node = $ll->first;
    for($i = 0.0; $i < $index - 1.0; $i = $i + 1.0){
      $node = $node->next;
    }

    $tmp = $node->next;
    $node->next = new stdClass();
    $node->next->next = $tmp;
    $node->next->value = $value;
    $node->next->end = false;
  }
}
function LinkedListSet($ll, $index, $value){

  $node = $ll->first;
  for($i = 0.0; $i < $index; $i = $i + 1.0){
    $node = $node->next;
  }

  $node->next->value = $value;
}
function LinkedListRemoveNumber($ll, $index){

  $node = $ll->first;
  $prev = $ll->first;

  for($i = 0.0; $i < $index; $i = $i + 1.0){
    $prev = $node;
    $node = $node->next;
  }

  if($index == 0.0){
    $ll->first = $prev->next;
  }
  if( !$prev->next->end ){
    $prev->next = $prev->next->next;
  }
}
function FreeLinkedListNumbers($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function FreeLinkedListNumbersArray(&$lls){

  for($i = 0.0; $i < count($lls); $i = $i + 1.0){
    FreeLinkedListNumbers($lls[$i]);
  }
  unset($lls);
}
function &LinkedListNumbersToArray($ll){

  $node = $ll->first;

  $length = LinkedListNumbersLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = $node->value;
    $node = $node->next;
  }

  return $array;
}
function ArrayToLinkedListNumbers(&$array){

  $ll = CreateLinkedListNumbers();

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
    LinkedListAddNumber($ll, $array[$i]);
  }

  return $ll;
}
function LinkedListNumbersEqual($a, $b){

  $an = $a->first;
  $bn = $b->first;

  $equal = true;
  $done = false;
  for(; $equal &&  !$done ; ){
    if($an->end == $bn->end){
      if($an->end){
        $done = true;
      }else if($an->value == $bn->value){
        $an = $an->next;
        $bn = $bn->next;
      }else{
        $equal = false;
      }
    }else{
      $equal = false;
    }
  }

  return $equal;
}
function CreateLinkedListCharacter(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function LinkedListAddCharacter($ll, $value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function &LinkedListCharactersToArray($ll){

  $node = $ll->first;

  $length = LinkedListCharactersLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = $node->value;
    $node = $node->next;
  }

  return $array;
}
function LinkedListCharactersLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function FreeLinkedListCharacter($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function LinkedListCharactersAddString($ll, &$str){

  for($i = 0.0; $i < count($str); $i = $i + 1.0){
    LinkedListAddCharacter($ll, $str[$i]);
  }
}
function CreateDynamicArrayNumbers(){

  $da = new stdClass();
  $da->array = array_fill(0, 10.0, 0);
  $da->length = 0.0;

  return $da;
}
function CreateDynamicArrayNumbersWithInitialCapacity($capacity){

  $da = new stdClass();
  $da->array = array_fill(0, $capacity, 0);
  $da->length = 0.0;

  return $da;
}
function DynamicArrayAddNumber($da, $value){
  if($da->length == count($da->array)){
    DynamicArrayNumbersIncreaseSize($da);
  }

  $da->array[$da->length] = $value;
  $da->length = $da->length + 1.0;
}
function DynamicArrayNumbersIncreaseSize($da){

  $newLength = round(count($da->array)*3.0/2.0);
  $newArray = array_fill(0, $newLength, 0);

  for($i = 0.0; $i < count($da->array); $i = $i + 1.0){
    $newArray[$i] = $da->array[$i];
  }

  unset($da->array);

  $da->array = $newArray;
}
function DynamicArrayNumbersDecreaseSizeNecessary($da){

  $needsDecrease = false;

  if($da->length > 10.0){
    $needsDecrease = $da->length <= round(count($da->array)*2.0/3.0);
  }

  return $needsDecrease;
}
function DynamicArrayNumbersDecreaseSize($da){

  $newLength = round(count($da->array)*2.0/3.0);
  $newArray = array_fill(0, $newLength, 0);

  for($i = 0.0; $i < $newLength; $i = $i + 1.0){
    $newArray[$i] = $da->array[$i];
  }

  unset($da->array);

  $da->array = $newArray;
}
function DynamicArrayNumbersIndex($da, $index){
  return $da->array[$index];
}
function DynamicArrayNumbersLength($da){
  return $da->length;
}
function DynamicArrayInsertNumber($da, $index, $value){

  if($da->length == count($da->array)){
    DynamicArrayNumbersIncreaseSize($da);
  }

  for($i = $da->length; $i > $index; $i = $i - 1.0){
    $da->array[$i] = $da->array[$i - 1.0];
  }

  $da->array[$index] = $value;

  $da->length = $da->length + 1.0;
}
function DynamicArrayNumberSet($da, $index, $value){

  if($index < $da->length){
    $da->array[$index] = $value;
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function DynamicArrayRemoveNumber($da, $index){

  for($i = $index; $i < $da->length - 1.0; $i = $i + 1.0){
    $da->array[$i] = $da->array[$i + 1.0];
  }

  $da->length = $da->length - 1.0;

  if(DynamicArrayNumbersDecreaseSizeNecessary($da)){
    DynamicArrayNumbersDecreaseSize($da);
  }
}
function FreeDynamicArrayNumbers($da){
  unset($da->array);
  unset($da);
}
function &DynamicArrayNumbersToArray($da){

  $array = array_fill(0, $da->length, 0);

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    $array[$i] = $da->array[$i];
  }

  return $array;
}
function ArrayToDynamicArrayNumbersWithOptimalSize(&$array){

  /*
         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
         */
  $c = count($array);
  $n = (log($c) - 1.0)/log(3.0/2.0);
  $newCapacity = floor($n) + 1.0;

  $da = CreateDynamicArrayNumbersWithInitialCapacity($newCapacity);

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
    $da->array[$i] = $array[$i];
  }

  return $da;
}
function ArrayToDynamicArrayNumbers(&$array){

  $da = new stdClass();
  $da->array = CopyNumberArray($array);
  $da->length = count($array);

  return $da;
}
function DynamicArrayNumbersEqual($a, $b){

  $equal = true;
  if($a->length == $b->length){
    for($i = 0.0; $i < $a->length && $equal; $i = $i + 1.0){
      if($a->array[$i] != $b->array[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function DynamicArrayNumbersToLinkedList($da){

  $ll = CreateLinkedListNumbers();

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    LinkedListAddNumber($ll, $da->array[$i]);
  }

  return $ll;
}
function LinkedListToDynamicArrayNumbers($ll){

  $node = $ll->first;

  $da = new stdClass();
  $da->length = LinkedListNumbersLength($ll);

  $da->array = array_fill(0, $da->length, 0);

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    $da->array[$i] = $node->value;
    $node = $node->next;
  }

  return $da;
}
function &AddCharacter(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);
  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function AddCharacterRef($list, $i){
  $list->string = AddCharacter($list->string, $i);
}
function &RemoveCharacter(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }

  return $newlist;
}
function GetCharacterRef($list, $i){
  return $list->string[$i];
}
function RemoveCharacterRef($list, $i){
  $list->string = RemoveCharacter($list->string, $i);
}
function sWriteStringToStingStream(&$stream, $index, &$src){

  for($i = 0.0; $i < count($src); $i = $i + 1.0){
    $stream[$index->numberValue + $i] = $src[$i];
  }
  $index->numberValue = $index->numberValue + count($src);
}
function sWriteCharacterToStingStream(&$stream, $index, $src){
  $stream[$index->numberValue] = $src;
  $index->numberValue = $index->numberValue + 1.0;
}
function sWriteBooleanToStingStream(&$stream, $index, $src){
  if($src){
    sWriteStringToStingStream($stream, $index, $literal = str_split("true"));
  }else{
    sWriteStringToStingStream($stream, $index, $literal = str_split("false"));
  }
}
function sSubstringWithCheck(&$string, $from, $to, $stringReference){

  if($from >= 0.0 && $from <= count($string) && $to >= 0.0 && $to <= count($string) && $from <= $to){
    $stringReference->string = sSubstring($string, $from, $to);
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function &sSubstring(&$string, $from, $to){

  $length = $to - $from;

  $n = array_fill(0, $length, 0);

  for($i = $from; $i < $to; $i = $i + 1.0){
    $n[$i - $from] = $string[$i];
  }

  return $n;
}
function &sAppendString(&$s1, &$s2){

  $newString = sConcatenateString($s1, $s2);

  unset($s1);

  return $newString;
}
function &sConcatenateString(&$s1, &$s2){

  $newString = array_fill(0, count($s1) + count($s2), 0);

  for($i = 0.0; $i < count($s1); $i = $i + 1.0){
    $newString[$i] = $s1[$i];
  }

  for($i = 0.0; $i < count($s2); $i = $i + 1.0){
    $newString[count($s1) + $i] = $s2[$i];
  }

  return $newString;
}
function &sAppendCharacter(&$string, $c){

  $newString = sConcatenateCharacter($string, $c);

  unset($string);

  return $newString;
}
function &sConcatenateCharacter(&$string, $c){
  $newString = array_fill(0, count($string) + 1.0, 0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $newString[$i] = $string[$i];
  }

  $newString[count($string)] = $c;

  return $newString;
}
function &sSplitByCharacter(&$toSplit, $splitBy){

  $stringToSplitBy = array_fill(0, 1.0, 0);
  $stringToSplitBy[0.0] = $splitBy;

  $split = sSplitByString($toSplit, $stringToSplitBy);

  unset($stringToSplitBy);

  return $split;
}
function sIndexOfCharacter(&$string, $character, $indexReference){

  $found = false;
  for($i = 0.0; $i < count($string) &&  !$found ; $i = $i + 1.0){
    if($string[$i] == $character){
      $found = true;
      $indexReference->numberValue = $i;
    }
  }

  return $found;
}
function sSubstringEqualsWithCheck(&$string, $from, &$substring, $equalsReference){

  if($from < count($string)){
    $success = true;
    $equalsReference->booleanValue = sSubstringEquals($string, $from, $substring);
  }else{
    $success = false;
  }

  return $success;
}
function sSubstringEquals(&$string, $from, &$substring){

  $equal = true;
  if(count($string) - $from >= count($substring)){
    for($i = 0.0; $i < count($substring) && $equal; $i = $i + 1.0){
      if($string[$from + $i] != $substring[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function sIndexOfString(&$string, &$substring, $indexReference){

  $found = false;
  for($i = 0.0; $i < count($string) - count($substring) + 1.0 &&  !$found ; $i = $i + 1.0){
    if(sSubstringEquals($string, $i, $substring)){
      $found = true;
      $indexReference->numberValue = $i;
    }
  }

  return $found;
}
function sContainsCharacter(&$string, $character){

  $found = false;
  for($i = 0.0; $i < count($string) &&  !$found ; $i = $i + 1.0){
    if($string[$i] == $character){
      $found = true;
    }
  }

  return $found;
}
function sContainsString(&$string, &$substring){
  return sIndexOfString($string, $substring, new stdClass());
}
function sToUpperCase(&$string){

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $string[$i] = charToUpperCase($string[$i]);
  }
}
function sToLowerCase(&$string){

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $string[$i] = charToLowerCase($string[$i]);
  }
}
function sEqualsIgnoreCase(&$a, &$b){

  if(count($a) == count($b)){
    $equal = true;
    for($i = 0.0; $i < count($a) && $equal; $i = $i + 1.0){
      if(charToLowerCase($a[$i]) != charToLowerCase($b[$i])){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function &sReplaceString(&$string, &$toReplace, &$replaceWith){

  $da = CreateDynamicArrayCharacters();

  $equalsReference = new stdClass();

  for($i = 0.0; $i < count($string); ){
    $success = sSubstringEqualsWithCheck($string, $i, $toReplace, $equalsReference);
    if($success){
      $success = $equalsReference->booleanValue;
    }

    if($success && count($toReplace) > 0.0){
      for($j = 0.0; $j < count($replaceWith); $j = $j + 1.0){
        DynamicArrayAddCharacter($da, $replaceWith[$j]);
      }
      $i = $i + count($toReplace);
    }else{
      DynamicArrayAddCharacter($da, $string[$i]);
      $i = $i + 1.0;
    }
  }

  $result = DynamicArrayCharactersToArray($da);

  FreeDynamicArrayCharacters($da);

  return $result;
}
function &sReplaceCharacterToNew(&$string, $toReplace, $replaceWith){

  $result = array_fill(0, count($string), 0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    if($string[$i] == $toReplace){
      $result[$i] = $replaceWith;
    }else{
      $result[$i] = $string[$i];
    }
  }

  return $result;
}
function sReplaceCharacter(&$string, $toReplace, $replaceWith){

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    if($string[$i] == $toReplace){
      $string[$i] = $replaceWith;
    }
  }
}
function &sTrim(&$string){

  /* Find whitepaces at the start. */
  $lastWhitespaceLocationStart = -1.0;
  $firstNonWhitespaceFound = false;
  for($i = 0.0; $i < count($string) &&  !$firstNonWhitespaceFound ; $i = $i + 1.0){
    if(charIsWhiteSpace($string[$i])){
      $lastWhitespaceLocationStart = $i;
    }else{
      $firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  $lastWhitespaceLocationEnd = count($string);
  $firstNonWhitespaceFound = false;
  for($i = count($string) - 1.0; $i >= 0.0 &&  !$firstNonWhitespaceFound ; $i = $i - 1.0){
    if(charIsWhiteSpace($string[$i])){
      $lastWhitespaceLocationEnd = $i;
    }else{
      $firstNonWhitespaceFound = true;
    }
  }

  if($lastWhitespaceLocationStart < $lastWhitespaceLocationEnd){
    $result = sSubstring($string, $lastWhitespaceLocationStart + 1.0, $lastWhitespaceLocationEnd);
  }else{
    $result = array_fill(0, 0.0, 0);
  }

  return $result;
}
function sStartsWith(&$string, &$start){

  $startsWithString = false;
  if(count($string) >= count($start)){
    $startsWithString = sSubstringEquals($string, 0.0, $start);
  }

  return $startsWithString;
}
function sEndsWith(&$string, &$end){

  $endsWithString = false;
  if(count($string) >= count($end)){
    $endsWithString = sSubstringEquals($string, count($string) - count($end), $end);
  }

  return $endsWithString;
}
function &sSplitByString(&$toSplit, &$splitBy){

  $split = array_fill(0, 0.0, 0);

  $next = array_fill(0, 0.0, 0);
  for($i = 0.0; $i < count($toSplit); ){
    $c = $toSplit[$i];

    if(sSubstringEquals($toSplit, $i, $splitBy)){
      $n = new stdClass();
      $n->string = $next;
      $split = AddString($split, $n);
      $next = array_fill(0, 0.0, 0);
      $i = $i + count($splitBy);
    }else{
      $next = sAppendCharacter($next, $c);
      $i = $i + 1.0;
    }
  }

  $n = new stdClass();
  $n->string = $next;
  $split = AddString($split, $n);

  return $split;
}
function sStringIsBefore(&$a, &$b){

  $before = false;
  $equal = true;
  $done = false;

  if(count($a) == 0.0 && count($b) > 0.0){
    $before = true;
  }else{
    for($i = 0.0; $i < count($a) && $i < count($b) &&  !$done ; $i = $i + 1.0){
      if($a[$i] != $b[$i]){
        $equal = false;
      }
      if(charCharacterIsBefore($a[$i], $b[$i])){
        $before = true;
      }
      if(charCharacterIsBefore($b[$i], $a[$i])){
        $done = true;
      }
    }

    if($equal){
      if(count($a) < count($b)){
        $before = true;
      }
    }
  }

  return $before;
}
function strWriteStringToStingStream(&$stream, $index, &$src){

  for($i = 0.0; $i < count($src); $i = $i + 1.0){
    $stream[$index->numberValue + $i] = $src[$i];
  }
  $index->numberValue = $index->numberValue + count($src);
}
function strWriteCharacterToStingStream(&$stream, $index, $src){
  $stream[$index->numberValue] = $src;
  $index->numberValue = $index->numberValue + 1.0;
}
function strWriteBooleanToStingStream(&$stream, $index, $src){
  if($src){
    strWriteStringToStingStream($stream, $index, $literal = str_split("true"));
  }else{
    strWriteStringToStingStream($stream, $index, $literal = str_split("false"));
  }
}
function strSubstringWithCheck(&$string, $from, $to, $stringReference){

  if($from >= 0.0 && $from <= count($string) && $to >= 0.0 && $to <= count($string) && $from <= $to){
    $stringReference->string = strSubstring($string, $from, $to);
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function &strSubstring(&$string, $from, $to){

  $length = $to - $from;

  $n = array_fill(0, $length, 0);

  for($i = $from; $i < $to; $i = $i + 1.0){
    $n[$i - $from] = $string[$i];
  }

  return $n;
}
function &strAppendString(&$s1, &$s2){

  $newString = strConcatenateString($s1, $s2);

  unset($s1);

  return $newString;
}
function &strConcatenateString(&$s1, &$s2){

  $newString = array_fill(0, count($s1) + count($s2), 0);

  for($i = 0.0; $i < count($s1); $i = $i + 1.0){
    $newString[$i] = $s1[$i];
  }

  for($i = 0.0; $i < count($s2); $i = $i + 1.0){
    $newString[count($s1) + $i] = $s2[$i];
  }

  return $newString;
}
function &strAppendCharacter(&$string, $c){

  $newString = strConcatenateCharacter($string, $c);

  unset($string);

  return $newString;
}
function &strConcatenateCharacter(&$string, $c){
  $newString = array_fill(0, count($string) + 1.0, 0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $newString[$i] = $string[$i];
  }

  $newString[count($string)] = $c;

  return $newString;
}
function &strSplitByCharacter(&$toSplit, $splitBy){

  $stringToSplitBy = array_fill(0, 1.0, 0);
  $stringToSplitBy[0.0] = $splitBy;

  $split = strSplitByString($toSplit, $stringToSplitBy);

  unset($stringToSplitBy);

  return $split;
}
function strIndexOfCharacter(&$string, $character, $indexReference){

  $found = false;
  for($i = 0.0; $i < count($string) &&  !$found ; $i = $i + 1.0){
    if($string[$i] == $character){
      $found = true;
      $indexReference->numberValue = $i;
    }
  }

  return $found;
}
function strSubstringEqualsWithCheck(&$string, $from, &$substring, $equalsReference){

  if($from < count($string)){
    $success = true;
    $equalsReference->booleanValue = strSubstringEquals($string, $from, $substring);
  }else{
    $success = false;
  }

  return $success;
}
function strSubstringEquals(&$string, $from, &$substring){

  $equal = true;
  for($i = 0.0; $i < count($substring) && $equal; $i = $i + 1.0){
    if($string[$from + $i] != $substring[$i]){
      $equal = false;
    }
  }

  return $equal;
}
function strIndexOfString(&$string, &$substring, $indexReference){

  $found = false;
  for($i = 0.0; $i < count($string) - count($substring) + 1.0 &&  !$found ; $i = $i + 1.0){
    if(strSubstringEquals($string, $i, $substring)){
      $found = true;
      $indexReference->numberValue = $i;
    }
  }

  return $found;
}
function strContainsCharacter(&$string, $character){

  $found = false;
  for($i = 0.0; $i < count($string) &&  !$found ; $i = $i + 1.0){
    if($string[$i] == $character){
      $found = true;
    }
  }

  return $found;
}
function strContainsString(&$string, &$substring){
  return strIndexOfString($string, $substring, new stdClass());
}
function strToUpperCase(&$string){

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $string[$i] = charToUpperCase($string[$i]);
  }
}
function strToLowerCase(&$string){

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $string[$i] = charToLowerCase($string[$i]);
  }
}
function strEqualsIgnoreCase(&$a, &$b){

  if(count($a) == count($b)){
    $equal = true;
    for($i = 0.0; $i < count($a) && $equal; $i = $i + 1.0){
      if(charToLowerCase($a[$i]) != charToLowerCase($b[$i])){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function &strReplaceString(&$string, &$toReplace, &$replaceWith){

  $equalsReference = new stdClass();
  $result = array_fill(0, 0.0, 0);

  for($i = 0.0; $i < count($string); ){
    $success = strSubstringEqualsWithCheck($string, $i, $toReplace, $equalsReference);
    if($success){
      $success = $equalsReference->booleanValue;
    }

    if($success && count($toReplace) > 0.0){
      $result = strConcatenateString($result, $replaceWith);
      $i = $i + count($toReplace);
    }else{
      $result = strConcatenateCharacter($result, $string[$i]);
      $i = $i + 1.0;
    }
  }

  return $result;
}
function &strReplaceCharacter(&$string, $toReplace, $replaceWith){

  $result = array_fill(0, 0.0, 0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    if($string[$i] == $toReplace){
      $result = strConcatenateCharacter($result, $replaceWith);
    }else{
      $result = strConcatenateCharacter($result, $string[$i]);
    }
  }

  return $result;
}
function &strTrim(&$string){

  /* Find whitepaces at the start. */
  $lastWhitespaceLocationStart = -1.0;
  $firstNonWhitespaceFound = false;
  for($i = 0.0; $i < count($string) &&  !$firstNonWhitespaceFound ; $i = $i + 1.0){
    if(charIsWhiteSpace($string[$i])){
      $lastWhitespaceLocationStart = $i;
    }else{
      $firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  $lastWhitespaceLocationEnd = count($string);
  $firstNonWhitespaceFound = false;
  for($i = count($string) - 1.0; $i >= 0.0 &&  !$firstNonWhitespaceFound ; $i = $i - 1.0){
    if(charIsWhiteSpace($string[$i])){
      $lastWhitespaceLocationEnd = $i;
    }else{
      $firstNonWhitespaceFound = true;
    }
  }

  if($lastWhitespaceLocationStart < $lastWhitespaceLocationEnd){
    $result = strSubstring($string, $lastWhitespaceLocationStart + 1.0, $lastWhitespaceLocationEnd);
  }else{
    $result = array_fill(0, 0.0, 0);
  }

  return $result;
}
function strStartsWith(&$string, &$start){

  $startsWithString = false;
  if(count($string) >= count($start)){
    $startsWithString = strSubstringEquals($string, 0.0, $start);
  }

  return $startsWithString;
}
function strEndsWith(&$string, &$end){

  $endsWithString = false;
  if(count($string) >= count($end)){
    $endsWithString = strSubstringEquals($string, count($string) - count($end), $end);
  }

  return $endsWithString;
}
function &strSplitByString(&$toSplit, &$splitBy){

  $split = array_fill(0, 0.0, 0);

  $next = array_fill(0, 0.0, 0);
  for($i = 0.0; $i < count($toSplit); ){
    $c = $toSplit[$i];

    if(strSubstringEquals($toSplit, $i, $splitBy)){
      if(count($split) != 0.0 || $i != 0.0){
        $n = new stdClass();
        $n->string = $next;
        $split = lAddString($split, $n);
        $next = array_fill(0, 0.0, 0);
        $i = $i + count($splitBy);
      }
    }else{
      $next = strAppendCharacter($next, $c);
      $i = $i + 1.0;
    }
  }

  if(count($next) > 0.0){
    $n = new stdClass();
    $n->string = $next;
    $split = lAddString($split, $n);
  }

  return $split;
}
function strStringIsBefore(&$a, &$b){

  $before = false;
  $equal = true;
  $done = false;

  if(count($a) == 0.0 && count($b) > 0.0){
    $before = true;
  }else{
    for($i = 0.0; $i < count($a) && $i < count($b) &&  !$done ; $i = $i + 1.0){
      if($a[$i] != $b[$i]){
        $equal = false;
      }
      if(charCharacterIsBefore($a[$i], $b[$i])){
        $before = true;
      }
      if(charCharacterIsBefore($b[$i], $a[$i])){
        $done = true;
      }
    }

    if($equal){
      if(count($a) < count($b)){
        $before = true;
      }
    }
  }

  return $before;
}
function &StringToNumberArray(&$string){

  $array = array_fill(0, count($string), 0);

  for($i = 0.0; $i < count($string); $i = $i + 1.0){
    $array[$i] = uniord($string[$i]);
  }
  return $array;
}
function &NumberArrayToString(&$array){

  $string = array_fill(0, count($array), 0);

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
    $string[$i] = unichr($array[$i]);
  }
  return $string;
}
function NumberArraysEqual(&$a, &$b){

  $equal = true;
  if(count($a) == count($b)){
    for($i = 0.0; $i < count($a) && $equal; $i = $i + 1.0){
      if($a[$i] != $b[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function BooleanArraysEqual(&$a, &$b){

  $equal = true;
  if(count($a) == count($b)){
    for($i = 0.0; $i < count($a) && $equal; $i = $i + 1.0){
      if($a[$i] != $b[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function StringsEqual(&$a, &$b){

  $equal = true;
  if(count($a) == count($b)){
    for($i = 0.0; $i < count($a) && $equal; $i = $i + 1.0){
      if($a[$i] != $b[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function FillNumberArray(&$a, $value){

  for($i = 0.0; $i < count($a); $i = $i + 1.0){
    $a[$i] = $value;
  }
}
function FillString(&$a, $value){

  for($i = 0.0; $i < count($a); $i = $i + 1.0){
    $a[$i] = $value;
  }
}
function FillBooleanArray(&$a, $value){

  for($i = 0.0; $i < count($a); $i = $i + 1.0){
    $a[$i] = $value;
  }
}
function FillNumberArrayRange(&$a, $value, $from, $to){

  if($from >= 0.0 && $from <= count($a) && $to >= 0.0 && $to <= count($a) && $from <= $to){
    $length = $to - $from;
    for($i = 0.0; $i < $length; $i = $i + 1.0){
      $a[$from + $i] = $value;
    }

    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function FillBooleanArrayRange(&$a, $value, $from, $to){

  if($from >= 0.0 && $from <= count($a) && $to >= 0.0 && $to <= count($a) && $from <= $to){
    $length = $to - $from;
    for($i = 0.0; $i < $length; $i = $i + 1.0){
      $a[$from + $i] = $value;
    }

    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function FillStringRange(&$a, $value, $from, $to){

  if($from >= 0.0 && $from <= count($a) && $to >= 0.0 && $to <= count($a) && $from <= $to){
    $length = $to - $from;
    for($i = 0.0; $i < $length; $i = $i + 1.0){
      $a[$from + $i] = $value;
    }

    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function &CopyNumberArray(&$a){

  $n = array_fill(0, count($a), 0);

  for($i = 0.0; $i < count($a); $i = $i + 1.0){
    $n[$i] = $a[$i];
  }

  return $n;
}
function &CopyBooleanArray(&$a){

  $n = array_fill(0, count($a), 0);

  for($i = 0.0; $i < count($a); $i = $i + 1.0){
    $n[$i] = $a[$i];
  }

  return $n;
}
function &CopyString(&$a){

  $n = array_fill(0, count($a), 0);

  for($i = 0.0; $i < count($a); $i = $i + 1.0){
    $n[$i] = $a[$i];
  }

  return $n;
}
function CopyNumberArrayRange(&$a, $from, $to, $copyReference){

  if($from >= 0.0 && $from <= count($a) && $to >= 0.0 && $to <= count($a) && $from <= $to){
    $length = $to - $from;
    $n = array_fill(0, $length, 0);

    for($i = 0.0; $i < $length; $i = $i + 1.0){
      $n[$i] = $a[$from + $i];
    }

    $copyReference->numberArray = $n;
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function CopyBooleanArrayRange(&$a, $from, $to, $copyReference){

  if($from >= 0.0 && $from <= count($a) && $to >= 0.0 && $to <= count($a) && $from <= $to){
    $length = $to - $from;
    $n = array_fill(0, $length, 0);

    for($i = 0.0; $i < $length; $i = $i + 1.0){
      $n[$i] = $a[$from + $i];
    }

    $copyReference->booleanArray = $n;
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function CopyStringRange(&$a, $from, $to, $copyReference){

  if($from >= 0.0 && $from <= count($a) && $to >= 0.0 && $to <= count($a) && $from <= $to){
    $length = $to - $from;
    $n = array_fill(0, $length, 0);

    for($i = 0.0; $i < $length; $i = $i + 1.0){
      $n[$i] = $a[$from + $i];
    }

    $copyReference->string = $n;
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function IsLastElement($length, $index){
  return $index + 1.0 == $length;
}
function &CreateNumberArray($length, $value){

  $array = array_fill(0, $length, 0);
  FillNumberArray($array, $value);

  return $array;
}
function &CreateBooleanArray($length, $value){

  $array = array_fill(0, $length, 0);
  FillBooleanArray($array, $value);

  return $array;
}
function &CreateString($length, $value){

  $array = array_fill(0, $length, 0);
  FillString($array, $value);

  return $array;
}
function SwapElementsOfNumberArray(&$A, $ai, $bi){

  $tmp = $A[$ai];
  $A[$ai] = $A[$bi];
  $A[$bi] = $tmp;
}
function SwapElementsOfStringArray($A, $ai, $bi){

  $tmp = $A->stringArray[$ai];
  $A->stringArray[$ai] = $A->stringArray[$bi];
  $A->stringArray[$bi] = $tmp;
}
function ReverseNumberArray(&$array){

  for($i = 0.0; $i < count($array)/2.0; $i = $i + 1.0){
    SwapElementsOfNumberArray($array, $i, count($array) - $i - 1.0);
  }
}
function AssertFalse($b, $failures){
  if($b){
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertTrue($b, $failures){
  if( !$b ){
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertEquals($a, $b, $failures){
  if($a != $b){
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertBooleansEqual($a, $b, $failures){
  if($a != $b){
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertCharactersEqual($a, $b, $failures){
  if($a != $b){
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertStringEquals(&$a, &$b, $failures){
  if( !StringsEqual($a, $b) ){
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertNumberArraysEqual(&$a, &$b, $failures){

  if(count($a) == count($b)){
    for($i = 0.0; $i < count($a); $i = $i + 1.0){
      AssertEquals($a[$i], $b[$i], $failures);
    }
  }else{
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertBooleanArraysEqual(&$a, &$b, $failures){

  if(count($a) == count($b)){
    for($i = 0.0; $i < count($a); $i = $i + 1.0){
      AssertBooleansEqual($a[$i], $b[$i], $failures);
    }
  }else{
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function AssertStringArraysEqual(&$a, &$b, $failures){

  if(count($a) == count($b)){
    for($i = 0.0; $i < count($a); $i = $i + 1.0){
      AssertStringEquals($a[$i]->string, $b[$i]->string, $failures);
    }
  }else{
    $failures->numberValue = $failures->numberValue + 1.0;
  }
}
function &nCreateStringScientificNotationDecimalFromNumber($decimal){

  $mantissaReference = new stdClass();
  $exponentReference = new stdClass();
  $result = array_fill(0, 0.0, 0);
  $done = false;
  $exponent = 0.0;

  if($decimal < 0.0){
    $isPositive = false;
    $decimal = -$decimal;
  }else{
    $isPositive = true;
  }

  if($decimal == 0.0){
    $done = true;
  }

  if( !$done ){
    $multiplier = 0.0;
    $inc = 0.0;

    if($decimal < 1.0){
      $multiplier = 10.0;
      $inc = -1.0;
    }else if($decimal >= 10.0){
      $multiplier = 0.1;
      $inc = 1.0;
    }else{
      $done = true;
    }

    if( !$done ){
      for(; $decimal >= 10.0 || $decimal < 1.0; ){
        $decimal = $decimal*$multiplier;
        $exponent = $exponent + $inc;
      }
    }
  }

  nCreateStringFromNumberWithCheck($decimal, 10.0, $mantissaReference);

  nCreateStringFromNumberWithCheck($exponent, 10.0, $exponentReference);

  if( !$isPositive ){
    $result = strAppendString($result, $literal = str_split("-"));
  }

  $result = strAppendString($result, $mantissaReference->string);
  $result = strAppendString($result, $literal = str_split("e"));
  $result = strAppendString($result, $exponentReference->string);

  return $result;
}
function &nCreateStringDecimalFromNumber($decimal){

  $stringReference = new stdClass();

  /* This will succeed because base = 10. */
  nCreateStringFromNumberWithCheck($decimal, 10.0, $stringReference);

  return $stringReference->string;
}
function nCreateStringFromNumberWithCheck($decimal, $base, $stringReference){

  $isPositive = true;

  if($decimal < 0.0){
    $isPositive = false;
    $decimal = -$decimal;
  }

  if($decimal == 0.0){
    $stringReference->string = str_split("0");
    $success = true;
  }else{
    $characterReference = new stdClass();

    if(IsInteger($base)){
      $success = true;

      $string = array_fill(0, 0.0, 0);

      $maximumDigits = nGetMaximumDigitsForBase($base);

      $digitPosition = nGetFirstDigitPosition($decimal, $base);

      $decimal = round($decimal*$base**($maximumDigits - $digitPosition - 1.0));

      $hasPrintedPoint = false;

      if( !$isPositive ){
        $string = strAppendCharacter($string, "-");
      }

      /* Print leading zeros. */
      if($digitPosition < 0.0){
        $string = strAppendCharacter($string, "0");
        $string = strAppendCharacter($string, ".");
        $hasPrintedPoint = true;
        for($i = 0.0; $i < -$digitPosition - 1.0; $i = $i + 1.0){
          $string = strAppendCharacter($string, "0");
        }
      }

      /* Print number. */
      for($i = 0.0; $i < $maximumDigits && $success; $i = $i + 1.0){
        $d = floor($decimal/$base**($maximumDigits - $i - 1.0));

        if($d >= $base){
          $d = $base - 1.0;
        }

        if( !$hasPrintedPoint  && $digitPosition - $i + 1.0 == 0.0){
          if($decimal != 0.0){
            $string = strAppendCharacter($string, ".");
          }
          $hasPrintedPoint = true;
        }

        if($decimal == 0.0 && $hasPrintedPoint){
        }else{
          $success = nGetSingleDigitCharacterFromNumberWithCheck($d, $base, $characterReference);
          if($success){
            $c = $characterReference->characterValue;
            $string = strAppendCharacter($string, $c);
          }
        }

        if($success){
          $decimal = $decimal - $d*$base**($maximumDigits - $i - 1.0);
        }
      }

      if($success){
        /* Print trailing zeros. */
        for($i = 0.0; $i < $digitPosition - $maximumDigits + 1.0; $i = $i + 1.0){
          $string = strAppendCharacter($string, "0");
        }

        $stringReference->string = $string;
      }
    }else{
      $success = false;
    }
  }

  /* Done */
  return $success;
}
function nGetMaximumDigitsForBase($base){

  $t = 10.0**15.0;
  return floor(log10($t)/log10($base));
}
function nGetFirstDigitPosition($decimal, $base){

  $power = ceil(log10($decimal)/log10($base));

  $t = $decimal*$base**(-$power);
  if($t < $base && $t >= 1.0){
  }else if($t >= $base){
    $power = $power + 1.0;
  }else if($t < 1.0){
    $power = $power - 1.0;
  }

  return $power;
}
function nGetSingleDigitCharacterFromNumberWithCheck($c, $base, $characterReference){

  $numberTable = nGetDigitCharacterTable();

  if($c < $base || $c < count($numberTable)){
    $success = true;
    $characterReference->characterValue = $numberTable[$c];
  }else{
    $success = false;
  }

  return $success;
}
function &nGetDigitCharacterTable(){

  $numberTable = str_split("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

  return $numberTable;
}
function nCreateNumberFromDecimalStringWithCheck(&$string, $decimalReference, $errorMessage){
  return nCreateNumberFromStringWithCheck($string, 10.0, $decimalReference, $errorMessage);
}
function nCreateNumberFromDecimalString(&$string){

  $doubleReference = CreateNumberReference(0.0);
  $stringReference = CreateStringReference($literal = array());
  nCreateNumberFromStringWithCheck($string, 10.0, $doubleReference, $stringReference);
  $number = $doubleReference->numberValue;

  unset($doubleReference);
  unset($stringReference);

  return $number;
}
function nCreateNumberFromStringWithCheck(&$string, $base, $numberReference, $errorMessage){

  $numberIsPositive = CreateBooleanReference(true);
  $exponentIsPositive = CreateBooleanReference(true);
  $beforePoint = new stdClass();
  $afterPoint = new stdClass();
  $exponent = new stdClass();

  if($base >= 2.0 && $base <= 36.0){
    $success = nExtractPartsFromNumberString($string, $base, $numberIsPositive, $beforePoint, $afterPoint, $exponentIsPositive, $exponent, $errorMessage);

    if($success){
      $numberReference->numberValue = nCreateNumberFromParts($base, $numberIsPositive->booleanValue, $beforePoint->numberArray, $afterPoint->numberArray, $exponentIsPositive->booleanValue, $exponent->numberArray);
    }
  }else{
    $success = false;
    $errorMessage->string = str_split("Base must be from 2 to 36.");
  }

  return $success;
}
function nCreateNumberFromParts($base, $numberIsPositive, &$beforePoint, &$afterPoint, $exponentIsPositive, &$exponent){

  $n = 0.0;

  for($i = 0.0; $i < count($beforePoint); $i = $i + 1.0){
    $p = $beforePoint[count($beforePoint) - $i - 1.0];

    $n = $n + $p*$base**$i;
  }

  for($i = 0.0; $i < count($afterPoint); $i = $i + 1.0){
    $p = $afterPoint[$i];

    $n = $n + $p*$base**(-($i + 1.0));
  }

  if(count($exponent) > 0.0){
    $e = 0.0;
    for($i = 0.0; $i < count($exponent); $i = $i + 1.0){
      $p = $exponent[count($exponent) - $i - 1.0];

      $e = $e + $p*$base**$i;
    }

    if( !$exponentIsPositive ){
      $e = -$e;
    }

    $n = $n*$base**$e;
  }

  if( !$numberIsPositive ){
    $n = -$n;
  }

  return $n;
}
function nExtractPartsFromNumberString(&$n, $base, $numberIsPositive, $beforePoint, $afterPoint, $exponentIsPositive, $exponent, $errorMessages){

  $i = 0.0;

  if($i < count($n)){
    if($n[$i] == "-"){
      $numberIsPositive->booleanValue = false;
      $i = $i + 1.0;
    }else if($n[$i] == "+"){
      $numberIsPositive->booleanValue = true;
      $i = $i + 1.0;
    }

    $success = nExtractPartsFromNumberStringFromSign($n, $base, $i, $beforePoint, $afterPoint, $exponentIsPositive, $exponent, $errorMessages);
  }else{
    $success = false;
    $errorMessages->string = str_split("Number cannot have length zero.");
  }

  return $success;
}
function nExtractPartsFromNumberStringFromSign(&$n, $base, $i, $beforePoint, $afterPoint, $exponentIsPositive, $exponent, $errorMessages){

  $done = false;
  $count = 0.0;
  for(; $i + $count < count($n) &&  !$done ; ){
    if(nCharacterIsNumberCharacterInBase($n[$i + $count], $base)){
      $count = $count + 1.0;
    }else{
      $done = true;
    }
  }

  if($count >= 1.0){
    $beforePoint->numberArray = array_fill(0, $count, 0);

    for($j = 0.0; $j < $count; $j = $j + 1.0){
      $beforePoint->numberArray[$j] = nGetNumberFromNumberCharacterForBase($n[$i + $j], $base);
    }

    $i = $i + $count;

    if($i < count($n)){
      $success = nExtractPartsFromNumberStringFromPointOrExponent($n, $base, $i, $afterPoint, $exponentIsPositive, $exponent, $errorMessages);
    }else{
      $afterPoint->numberArray = array_fill(0, 0.0, 0);
      $exponent->numberArray = array_fill(0, 0.0, 0);
      $success = true;
    }
  }else{
    $success = false;
    $errorMessages->string = str_split("Number must have at least one number after the optional sign.");
  }

  return $success;
}
function nExtractPartsFromNumberStringFromPointOrExponent(&$n, $base, $i, $afterPoint, $exponentIsPositive, $exponent, $errorMessages){

  if($n[$i] == "."){
    $i = $i + 1.0;

    if($i < count($n)){
      $done = false;
      $count = 0.0;
      for(; $i + $count < count($n) &&  !$done ; ){
        if(nCharacterIsNumberCharacterInBase($n[$i + $count], $base)){
          $count = $count + 1.0;
        }else{
          $done = true;
        }
      }

      if($count >= 1.0){
        $afterPoint->numberArray = array_fill(0, $count, 0);

        for($j = 0.0; $j < $count; $j = $j + 1.0){
          $afterPoint->numberArray[$j] = nGetNumberFromNumberCharacterForBase($n[$i + $j], $base);
        }

        $i = $i + $count;

        if($i < count($n)){
          $success = nExtractPartsFromNumberStringFromExponent($n, $base, $i, $exponentIsPositive, $exponent, $errorMessages);
        }else{
          $exponent->numberArray = array_fill(0, 0.0, 0);
          $success = true;
        }
      }else{
        $success = false;
        $errorMessages->string = str_split("There must be at least one digit after the decimal point.");
      }
    }else{
      $success = false;
      $errorMessages->string = str_split("There must be at least one digit after the decimal point.");
    }
  }else if($base <= 14.0 && ($n[$i] == "e" || $n[$i] == "E")){
    if($i < count($n)){
      $success = nExtractPartsFromNumberStringFromExponent($n, $base, $i, $exponentIsPositive, $exponent, $errorMessages);
      $afterPoint->numberArray = array_fill(0, 0.0, 0);
    }else{
      $success = false;
      $errorMessages->string = str_split("There must be at least one digit after the exponent.");
    }
  }else{
    $success = false;
    $errorMessages->string = str_split("Expected decimal point or exponent symbol.");
  }

  return $success;
}
function nExtractPartsFromNumberStringFromExponent(&$n, $base, $i, $exponentIsPositive, $exponent, $errorMessages){

  if($base <= 14.0 && ($n[$i] == "e" || $n[$i] == "E")){
    $i = $i + 1.0;

    if($i < count($n)){
      if($n[$i] == "-"){
        $exponentIsPositive->booleanValue = false;
        $i = $i + 1.0;
      }else if($n[$i] == "+"){
        $exponentIsPositive->booleanValue = true;
        $i = $i + 1.0;
      }

      if($i < count($n)){
        $done = false;
        $count = 0.0;
        for(; $i + $count < count($n) &&  !$done ; ){
          if(nCharacterIsNumberCharacterInBase($n[$i + $count], $base)){
            $count = $count + 1.0;
          }else{
            $done = true;
          }
        }

        if($count >= 1.0){
          $exponent->numberArray = array_fill(0, $count, 0);

          for($j = 0.0; $j < $count; $j = $j + 1.0){
            $exponent->numberArray[$j] = nGetNumberFromNumberCharacterForBase($n[$i + $j], $base);
          }

          $i = $i + $count;

          if($i == count($n)){
            $success = true;
          }else{
            $success = false;
            $errorMessages->string = str_split("There cannot be any characters past the exponent of the number.");
          }
        }else{
          $success = false;
          $errorMessages->string = str_split("There must be at least one digit after the decimal point.");
        }
      }else{
        $success = false;
        $errorMessages->string = str_split("There must be at least one digit after the exponent symbol.");
      }
    }else{
      $success = false;
      $errorMessages->string = str_split("There must be at least one digit after the exponent symbol.");
    }
  }else{
    $success = false;
    $errorMessages->string = str_split("Expected exponent symbol.");
  }

  return $success;
}
function nGetNumberFromNumberCharacterForBase($c, $base){

  $numberTable = nGetDigitCharacterTable();
  $position = 0.0;

  for($i = 0.0; $i < $base; $i = $i + 1.0){
    if($numberTable[$i] == $c){
      $position = $i;
    }
  }

  return $position;
}
function nCharacterIsNumberCharacterInBase($c, $base){

  $numberTable = nGetDigitCharacterTable();
  $found = false;

  for($i = 0.0; $i < $base; $i = $i + 1.0){
    if($numberTable[$i] == $c){
      $found = true;
    }
  }

  return $found;
}
function &nStringToNumberArray(&$str){

  $numberArrayReference = new stdClass();
  $stringReference = new stdClass();

  nStringToNumberArrayWithCheck($str, $numberArrayReference, $stringReference);

  $numbers = $numberArrayReference->numberArray;

  unset($numberArrayReference);
  unset($stringReference);

  return $numbers;
}
function nStringToNumberArrayWithCheck(&$str, $numberArrayReference, $errorMessage){

  $numberStrings = strSplitByString($str, $literal = str_split(","));

  $numbers = array_fill(0, count($numberStrings), 0);
  $success = true;
  $numberReference = new stdClass();

  for($i = 0.0; $i < count($numberStrings); $i = $i + 1.0){
    $numberString = $numberStrings[$i]->string;
    $trimmedNumberString = strTrim($numberString);
    $success = nCreateNumberFromDecimalStringWithCheck($trimmedNumberString, $numberReference, $errorMessage);
    $numbers[$i] = $numberReference->numberValue;

    FreeStringReference($numberStrings[$i]);
    unset($trimmedNumberString);
  }

  unset($numberStrings);
  unset($numberReference);

  $numberArrayReference->numberArray = $numbers;

  return $success;
}
function &lAddNumber(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);
  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function lAddNumberRef($list, $i){
  $list->numberArray = lAddNumber($list->numberArray, $i);
}
function &lRemoveNumber(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }
		
  return $newlist;
}
function lGetNumberRef($list, $i){
  return $list->numberArray[$i];
}
function lRemoveNumberRef($list, $i){
  $list->numberArray = lRemoveNumber($list->numberArray, $i);
}
function &lAddString(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);

  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function lAddStringRef($list, $i){
  $list->stringArray = lAddString($list->stringArray, $i);
}
function &lRemoveString(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }
		
  return $newlist;
}
function lGetStringRef($list, $i){
  return $list->stringArray[$i];
}
function lRemoveStringRef($list, $i){
  $list->stringArray = lRemoveString($list->stringArray, $i);
}
function &lAddBoolean(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);
  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function lAddBooleanRef($list, $i){
  $list->booleanArray = lAddBoolean($list->booleanArray, $i);
}
function &lRemoveBoolean(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }
		
  return $newlist;
}
function lGetBooleanRef($list, $i){
  return $list->booleanArray[$i];
}
function lRemoveDecimalRef($list, $i){
  $list->booleanArray = lRemoveBoolean($list->booleanArray, $i);
}
function lCreateLinkedListString(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function lLinkedListAddString($ll, &$value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function &lLinkedListStringsToArray($ll){

  $node = $ll->first;

  $length = lLinkedListStringsLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = new stdClass();
    $array[$i]->string = $node->value;
    $node = $node->next;
  }

  return $array;
}
function lLinkedListStringsLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function lFreeLinkedListString($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function lCreateLinkedListNumbers(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function &lCreateLinkedListNumbersArray($length){

  $lls = array_fill(0, $length, 0);
  for($i = 0.0; $i < count($lls); $i = $i + 1.0){
    $lls[$i] = lCreateLinkedListNumbers();
  }

  return $lls;
}
function lLinkedListAddNumber($ll, $value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function lLinkedListNumbersLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function lLinkedListNumbersIndex($ll, $index){

  $node = $ll->first;
  for($i = 0.0; $i < $index; $i = $i + 1.0){
    $node = $node->next;
  }

  return $node->value;
}
function lLinkedListInsertNumber($ll, $index, $value){

  if($index == 0.0){
    $tmp = $ll->first;
    $ll->first = new stdClass();
    $ll->first->next = $tmp;
    $ll->first->value = $value;
    $ll->first->end = false;
  }else{
    $node = $ll->first;
    for($i = 0.0; $i < $index - 1.0; $i = $i + 1.0){
      $node = $node->next;
    }

    $tmp = $node->next;
    $node->next = new stdClass();
    $node->next->next = $tmp;
    $node->next->value = $value;
    $node->next->end = false;
  }
}
function lLinkedListSet($ll, $index, $value){

  $node = $ll->first;
  for($i = 0.0; $i < $index; $i = $i + 1.0){
    $node = $node->next;
  }

  $node->next->value = $value;
}
function lLinkedListRemoveNumber($ll, $index){

  $node = $ll->first;
  $prev = $ll->first;

  for($i = 0.0; $i < $index; $i = $i + 1.0){
    $prev = $node;
    $node = $node->next;
  }

  if($index == 0.0){
    $ll->first = $prev->next;
  }
  if( !$prev->next->end ){
    $prev->next = $prev->next->next;
  }
}
function lFreeLinkedListNumbers($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function lFreeLinkedListNumbersArray(&$lls){

  for($i = 0.0; $i < count($lls); $i = $i + 1.0){
    lFreeLinkedListNumbers($lls[$i]);
  }
  unset($lls);
}
function &lLinkedListNumbersToArray($ll){

  $node = $ll->first;

  $length = lLinkedListNumbersLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = $node->value;
    $node = $node->next;
  }

  return $array;
}
function lArrayToLinkedListNumbers(&$array){

  $ll = lCreateLinkedListNumbers();

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
    lLinkedListAddNumber($ll, $array[$i]);
  }

  return $ll;
}
function lLinkedListNumbersEqual($a, $b){

  $an = $a->first;
  $bn = $b->first;

  $equal = true;
  $done = false;
  for(; $equal &&  !$done ; ){
    if($an->end == $bn->end){
      if($an->end){
        $done = true;
      }else if($an->value == $bn->value){
        $an = $an->next;
        $bn = $bn->next;
      }else{
        $equal = false;
      }
    }else{
      $equal = false;
    }
  }

  return $equal;
}
function lCreateLinkedListCharacter(){

  $ll = new stdClass();
  $ll->first = new stdClass();
  $ll->last = $ll->first;
  $ll->last->end = true;

  return $ll;
}
function lLinkedListAddCharacter($ll, $value){
  $ll->last->end = false;
  $ll->last->value = $value;
  $ll->last->next = new stdClass();
  $ll->last->next->end = true;
  $ll->last = $ll->last->next;
}
function &lLinkedListCharactersToArray($ll){

  $node = $ll->first;

  $length = lLinkedListCharactersLength($ll);

  $array = array_fill(0, $length, 0);

  for($i = 0.0; $i < $length; $i = $i + 1.0){
    $array[$i] = $node->value;
    $node = $node->next;
  }

  return $array;
}
function lLinkedListCharactersLength($ll){

  $l = 0.0;
  $node = $ll->first;
  for(;  !$node->end ; ){
    $node = $node->next;
    $l = $l + 1.0;
  }

  return $l;
}
function lFreeLinkedListCharacter($ll){

  $node = $ll->first;

  for(;  !$node->end ; ){
    $prev = $node;
    $node = $node->next;
    unset($prev);
  }

  unset($node);
}
function lCreateDynamicArrayNumbers(){

  $da = new stdClass();
  $da->array = array_fill(0, 10.0, 0);
  $da->length = 0.0;

  return $da;
}
function lCreateDynamicArrayNumbersWithInitialCapacity($capacity){

  $da = new stdClass();
  $da->array = array_fill(0, $capacity, 0);
  $da->length = 0.0;

  return $da;
}
function lDynamicArrayAddNumber($da, $value){
  if($da->length == count($da->array)){
    lDynamicArrayNumbersIncreaseSize($da);
  }

  $da->array[$da->length] = $value;
  $da->length = $da->length + 1.0;
}
function lDynamicArrayNumbersIncreaseSize($da){

  $newLength = round(count($da->array)*3.0/2.0);
  $newArray = array_fill(0, $newLength, 0);

  for($i = 0.0; $i < count($da->array); $i = $i + 1.0){
    $newArray[$i] = $da->array[$i];
  }

  unset($da->array);

  $da->array = $newArray;
}
function lDynamicArrayNumbersDecreaseSizeNecessary($da){

  $needsDecrease = false;

  if($da->length > 10.0){
    $needsDecrease = $da->length <= round(count($da->array)*2.0/3.0);
  }

  return $needsDecrease;
}
function lDynamicArrayNumbersDecreaseSize($da){

  $newLength = round(count($da->array)*2.0/3.0);
  $newArray = array_fill(0, $newLength, 0);

  for($i = 0.0; $i < count($da->array); $i = $i + 1.0){
    $newArray[$i] = $da->array[$i];
  }

  unset($da->array);

  $da->array = $newArray;
}
function lDynamicArrayNumbersIndex($da, $index){
  return $da->array[$index];
}
function lDynamicArrayNumbersLength($da){
  return $da->length;
}
function lDynamicArrayInsertNumber($da, $index, $value){

  if($da->length == count($da->array)){
    lDynamicArrayNumbersIncreaseSize($da);
  }

  for($i = $da->length; $i > $index; $i = $i - 1.0){
    $da->array[$i] = $da->array[$i - 1.0];
  }

  $da->array[$index] = $value;

  $da->length = $da->length + 1.0;
}
function lDynamicArraySet($da, $index, $value){
  $da->array[$index] = $value;
}
function lDynamicArrayRemoveNumber($da, $index){

  for($i = $index; $i < $da->length - 1.0; $i = $i + 1.0){
    $da->array[$i] = $da->array[$i + 1.0];
  }

  $da->length = $da->length - 1.0;

  if(lDynamicArrayNumbersDecreaseSizeNecessary($da)){
    lDynamicArrayNumbersDecreaseSize($da);
  }
}
function lFreeDynamicArrayNumbers($da){
  unset($da->array);
  unset($da);
}
function &lDynamicArrayNumbersToArray($da){

  $array = array_fill(0, $da->length, 0);

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    $array[$i] = $da->array[$i];
  }

  return $array;
}
function lArrayToDynamicArrayNumbersWithOptimalSize(&$array){

  /*
         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
         */
  $c = count($array);
  $n = (log($c) - 1.0)/log(3.0/2.0);
  $newCapacity = floor($n) + 1.0;

  $da = lCreateDynamicArrayNumbersWithInitialCapacity($newCapacity);

  for($i = 0.0; $i < count($array); $i = $i + 1.0){
    $da->array[$i] = $array[$i];
  }

  return $da;
}
function lArrayToDynamicArrayNumbers(&$array){

  $da = new stdClass();
  $da->array = CopyNumberArray($array);
  $da->length = count($array);

  return $da;
}
function lDynamicArrayNumbersEqual($a, $b){

  $equal = true;
  if($a->length == $b->length){
    for($i = 0.0; $i < $a->length && $equal; $i = $i + 1.0){
      if($a->array[$i] != $b->array[$i]){
        $equal = false;
      }
    }
  }else{
    $equal = false;
  }

  return $equal;
}
function lDynamicArrayNumbersToLinkedList($da){

  $ll = lCreateLinkedListNumbers();

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    lLinkedListAddNumber($ll, $da->array[$i]);
  }

  return $ll;
}
function lLinkedListToDynamicArrayNumbers($ll){

  $node = $ll->first;

  $da = new stdClass();
  $da->length = lLinkedListNumbersLength($ll);

  $da->array = array_fill(0, $da->length, 0);

  for($i = 0.0; $i < $da->length; $i = $i + 1.0){
    $da->array[$i] = $node->value;
    $node = $node->next;
  }

  return $da;
}
function &lAddCharacter(&$list, $a){

  $newlist = array_fill(0, count($list) + 1.0, 0);
  for($i = 0.0; $i < count($list); $i = $i + 1.0){
    $newlist[$i] = $list[$i];
  }
  $newlist[count($list)] = $a;
		
  unset($list);
		
  return $newlist;
}
function lAddCharacterRef($list, $i){
  $list->string = lAddCharacter($list->string, $i);
}
function &lRemoveCharacter(&$list, $n){

  $newlist = array_fill(0, count($list) - 1.0, 0);

  if($n >= 0.0 && $n < count($list)){
    for($i = 0.0; $i < count($list); $i = $i + 1.0){
      if($i < $n){
        $newlist[$i] = $list[$i];
      }
      if($i > $n){
        $newlist[$i - 1.0] = $list[$i];
      }
    }

    unset($list);
  }else{
    unset($newlist);
  }

  return $newlist;
}
function lGetCharacterRef($list, $i){
  return $list->string[$i];
}
function lRemoveCharacterRef($list, $i){
  $list->string = lRemoveCharacter($list->string, $i);
}
function Negate($x){
  return -$x;
}
function Positive($x){
  return +$x;
}
function Factorial($x){

  $f = 1.0;

  for($i = 2.0; $i <= $x; $i = $i + 1.0){
    $f = $f*$i;
  }

  return $f;
}
function Roundx($x){
  return floor($x + 0.5);
}
function BankersRound($x){

  if(Absolute($x - Truncate($x)) == 0.5){
    if( !DivisibleBy(Roundx($x), 2.0) ){
      $r = Roundx($x) - 1.0;
    }else{
      $r = Roundx($x);
    }
  }else{
    $r = Roundx($x);
  }

  return $r;
}
function Ceilx($x){
  return ceil($x);
}
function Floorx($x){
  return floor($x);
}
function Truncate($x){

  if($x >= 0.0){
    $t = floor($x);
  }else{
    $t = ceil($x);
  }

  return $t;
}
function Absolute($x){
  return abs($x);
}
function Logarithm($x){
  return log10($x);
}
function NaturalLogarithm($x){
  return log($x);
}
function Sinx($x){
  return sin($x);
}
function Cosx($x){
  return cos($x);
}
function Tanx($x){
  return tan($x);
}
function Asinx($x){
  return asin($x);
}
function Acosx($x){
  return acos($x);
}
function Atanx($x){
  return atan($x);
}
function Atan2x($y, $x){

  /* Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors. */
  $a = 0.0;

  if($x > 0.0){
    $a = Atanx($y/$x);
  }else if($x < 0.0 && $y >= 0.0){
    $a = Atanx($y/$x) + M_PI;
  }else if($x < 0.0 && $y < 0.0){
    $a = Atanx($y/$x) - M_PI;
  }else if($x == 0.0 && $y > 0.0){
    $a = M_PI/2.0;
  }else if($x == 0.0 && $y < 0.0){
    $a = -M_PI/2.0;
  }

  return $a;
}
function Squareroot($x){
  return sqrt($x);
}
function Expx($x){
  return exp($x);
}
function DivisibleBy($a, $b){
  return (($a%$b) == 0.0);
}
function Combinations($n, $k){

  $c = 1.0;
  $j = 1.0;
  $i = $n - $k + 1.0;

  for(; $i <= $n; ){
    $c = $c*$i;
    $c = $c/$j;

    $i = $i + 1.0;
    $j = $j + 1.0;
  }

  return $c;
}
function Permutations($n, $k){

  $c = 1.0;

  for($i = $n - $k + 1.0; $i <= $n; $i = $i + 1.0){
    $c = $c*$i;
  }

  return $c;
}
function EpsilonCompare($a, $b, $epsilon){
  return abs($a - $b) < $epsilon;
}
function GreatestCommonDivisor($a, $b){

  for(; $b != 0.0; ){
    $t = $b;
    $b = $a%$b;
    $a = $t;
  }

  return $a;
}
function GCDWithSubtraction($a, $b){

  if($a == 0.0){
    $g = $b;
  }else{
    for(; $b != 0.0; ){
      if($a > $b){
        $a = $a - $b;
      }else{
        $b = $b - $a;
      }
    }

    $g = $a;
  }

  return $g;
}
function IsInteger($a){
  return ($a - floor($a)) == 0.0;
}
function GreatestCommonDivisorWithCheck($a, $b, $gcdReference){

  if(IsInteger($a) && IsInteger($b)){
    $gcd = GreatestCommonDivisor($a, $b);
    $gcdReference->numberValue = $gcd;
    $success = true;
  }else{
    $success = false;
  }

  return $success;
}
function LeastCommonMultiple($a, $b){

  if($a > 0.0 && $b > 0.0){
    $lcm = abs($a*$b)/GreatestCommonDivisor($a, $b);
  }else{
    $lcm = 0.0;
  }

  return $lcm;
}
function Sign($a){

  if($a > 0.0){
    $s = 1.0;
  }else if($a < 0.0){
    $s = -1.0;
  }else{
    $s = 0.0;
  }

  return $s;
}
function Maxx($a, $b){
  return max($a, $b);
}
function Minx($a, $b){
  return min($a, $b);
}
function Power($a, $b){
  return $a**$b;
}
function Gamma($x){
  return LanczosApproximation($x);
}
function LogGamma($x){
  return log(Gamma($x));
}
function LanczosApproximation($z){

  $p = array_fill(0, 8.0, 0);
  $p[0.0] = 676.5203681218851;
  $p[1.0] = -1259.1392167224028;
  $p[2.0] = 771.32342877765313;
  $p[3.0] = -176.61502916214059;
  $p[4.0] = 12.507343278686905;
  $p[5.0] = -0.13857109526572012;
  $p[6.0] = 9.9843695780195716e-6;
  $p[7.0] = 1.5056327351493116e-7;

  if($z < 0.5){
    $y = M_PI/(sin(M_PI*$z)*LanczosApproximation(1.0 - $z));
  }else{
    $z = $z - 1.0;
    $x = 0.99999999999980993;
    for($i = 0.0; $i < count($p); $i = $i + 1.0){
      $x = $x + $p[$i]/($z + $i + 1.0);
    }
    $t = $z + count($p) - 0.5;
    $y = sqrt(2.0*M_PI)*$t**($z + 0.5)*exp(-$t)*$x;
  }

  return $y;
}
function Beta($x, $y){
  return Gamma($x)*Gamma($y)/Gamma($x + $y);
}
function Sinhx($x){
  return (exp($x) - exp(-$x))/2.0;
}
function Coshx($x){
  return (exp($x) + exp(-$x))/2.0;
}
function Tanhx($x){
  return Sinhx($x)/Coshx($x);
}
function Cot($x){
  return 1.0/tan($x);
}
function Sec($x){
  return 1.0/cos($x);
}
function Csc($x){
  return 1.0/sin($x);
}
function Coth($x){
  return Coshx($x)/Sinhx($x);
}
function Sech($x){
  return 1.0/Coshx($x);
}
function Csch($x){
  return 1.0/Sinhx($x);
}
function Error($x){

  if($x == 0.0){
    $y = 0.0;
  }else if($x < 0.0){
    $y = -Error(-$x);
  }else{
    $c1 = -1.26551223;
    $c2 = +1.00002368;
    $c3 = +0.37409196;
    $c4 = +0.09678418;
    $c5 = -0.18628806;
    $c6 = +0.27886807;
    $c7 = -1.13520398;
    $c8 = +1.48851587;
    $c9 = -0.82215223;
    $c10 = +0.17087277;

    $t = 1.0/(1.0 + 0.5*abs($x));

    $tau = $t*exp(-$x**2.0 + $c1 + $t*($c2 + $t*($c3 + $t*($c4 + $t*($c5 + $t*($c6 + $t*($c7 + $t*($c8 + $t*($c9 + $t*$c10)))))))));

    $y = 1.0 - $tau;
  }

  return $y;
}
function ErrorInverse($x){

  $a = (8.0*(M_PI - 3.0))/(3.0*M_PI*(4.0 - M_PI));

  $t = 2.0/(M_PI*$a) + log(1.0 - $x**2.0)/2.0;
  $y = Sign($x)*sqrt(sqrt($t**2.0 - log(1.0 - $x**2.0)/$a) - $t);

  return $y;
}
function FallingFactorial($x, $n){

  $y = 1.0;

  for($k = 0.0; $k <= $n - 1.0; $k = $k + 1.0){
    $y = $y*($x - $k);
  }

  return $y;
}
function RisingFactorial($x, $n){

  $y = 1.0;

  for($k = 0.0; $k <= $n - 1.0; $k = $k + 1.0){
    $y = $y*($x + $k);
  }

  return $y;
}
function Hypergeometric($a, $b, $c, $z, $maxIterations, $precision){

  if(abs($z) >= 0.5){
    $y = (1.0 - $z)**(-$a)*HypergeometricDirect($a, $c - $b, $c, $z/($z - 1.0), $maxIterations, $precision);
  }else{
    $y = HypergeometricDirect($a, $b, $c, $z, $maxIterations, $precision);
  }

  return $y;
}
function HypergeometricDirect($a, $b, $c, $z, $maxIterations, $precision){

  $y = 0.0;
  $done = false;

  for($n = 0.0; $n < $maxIterations &&  !$done ; $n = $n + 1.0){
    $yp = RisingFactorial($a, $n)*RisingFactorial($b, $n)/RisingFactorial($c, $n)*$z**$n/Factorial($n);
    if(abs($yp) < $precision){
      $done = true;
    }
    $y = $y + $yp;
  }

  return $y;
}
function BernouilliNumber($n){
  return AkiyamaTanigawaAlgorithm($n);
}
function AkiyamaTanigawaAlgorithm($n){

  $A = array_fill(0, $n + 1.0, 0);

  for($m = 0.0; $m <= $n; $m = $m + 1.0){
    $A[$m] = 1.0/($m + 1.0);
    for($j = $m; $j >= 1.0; $j = $j - 1.0){
      $A[$j - 1.0] = $j*($A[$j - 1.0] - $A[$j]);
    }
  }

  $B = $A[0.0];

  unset($A);

  return $B;
}
function charToLowerCase($character){

  $toReturn = $character;
  if($character == "A"){
    $toReturn = "a";
  }else if($character == "B"){
    $toReturn = "b";
  }else if($character == "C"){
    $toReturn = "c";
  }else if($character == "D"){
    $toReturn = "d";
  }else if($character == "E"){
    $toReturn = "e";
  }else if($character == "F"){
    $toReturn = "f";
  }else if($character == "G"){
    $toReturn = "g";
  }else if($character == "H"){
    $toReturn = "h";
  }else if($character == "I"){
    $toReturn = "i";
  }else if($character == "J"){
    $toReturn = "j";
  }else if($character == "K"){
    $toReturn = "k";
  }else if($character == "L"){
    $toReturn = "l";
  }else if($character == "M"){
    $toReturn = "m";
  }else if($character == "N"){
    $toReturn = "n";
  }else if($character == "O"){
    $toReturn = "o";
  }else if($character == "P"){
    $toReturn = "p";
  }else if($character == "Q"){
    $toReturn = "q";
  }else if($character == "R"){
    $toReturn = "r";
  }else if($character == "S"){
    $toReturn = "s";
  }else if($character == "T"){
    $toReturn = "t";
  }else if($character == "U"){
    $toReturn = "u";
  }else if($character == "V"){
    $toReturn = "v";
  }else if($character == "W"){
    $toReturn = "w";
  }else if($character == "X"){
    $toReturn = "x";
  }else if($character == "Y"){
    $toReturn = "y";
  }else if($character == "Z"){
    $toReturn = "z";
  }

  return $toReturn;
}
function charToUpperCase($character){

  $toReturn = $character;
  if($character == "a"){
    $toReturn = "A";
  }else if($character == "b"){
    $toReturn = "B";
  }else if($character == "c"){
    $toReturn = "C";
  }else if($character == "d"){
    $toReturn = "D";
  }else if($character == "e"){
    $toReturn = "E";
  }else if($character == "f"){
    $toReturn = "F";
  }else if($character == "g"){
    $toReturn = "G";
  }else if($character == "h"){
    $toReturn = "H";
  }else if($character == "i"){
    $toReturn = "I";
  }else if($character == "j"){
    $toReturn = "J";
  }else if($character == "k"){
    $toReturn = "K";
  }else if($character == "l"){
    $toReturn = "L";
  }else if($character == "m"){
    $toReturn = "M";
  }else if($character == "n"){
    $toReturn = "N";
  }else if($character == "o"){
    $toReturn = "O";
  }else if($character == "p"){
    $toReturn = "P";
  }else if($character == "q"){
    $toReturn = "Q";
  }else if($character == "r"){
    $toReturn = "R";
  }else if($character == "s"){
    $toReturn = "S";
  }else if($character == "t"){
    $toReturn = "T";
  }else if($character == "u"){
    $toReturn = "U";
  }else if($character == "v"){
    $toReturn = "V";
  }else if($character == "w"){
    $toReturn = "W";
  }else if($character == "x"){
    $toReturn = "X";
  }else if($character == "y"){
    $toReturn = "Y";
  }else if($character == "z"){
    $toReturn = "Z";
  }

  return $toReturn;
}
function charIsUpperCase($character){

  $isUpper = false;
  if($character == "A"){
    $isUpper = true;
  }else if($character == "B"){
    $isUpper = true;
  }else if($character == "C"){
    $isUpper = true;
  }else if($character == "D"){
    $isUpper = true;
  }else if($character == "E"){
    $isUpper = true;
  }else if($character == "F"){
    $isUpper = true;
  }else if($character == "G"){
    $isUpper = true;
  }else if($character == "H"){
    $isUpper = true;
  }else if($character == "I"){
    $isUpper = true;
  }else if($character == "J"){
    $isUpper = true;
  }else if($character == "K"){
    $isUpper = true;
  }else if($character == "L"){
    $isUpper = true;
  }else if($character == "M"){
    $isUpper = true;
  }else if($character == "N"){
    $isUpper = true;
  }else if($character == "O"){
    $isUpper = true;
  }else if($character == "P"){
    $isUpper = true;
  }else if($character == "Q"){
    $isUpper = true;
  }else if($character == "R"){
    $isUpper = true;
  }else if($character == "S"){
    $isUpper = true;
  }else if($character == "T"){
    $isUpper = true;
  }else if($character == "U"){
    $isUpper = true;
  }else if($character == "V"){
    $isUpper = true;
  }else if($character == "W"){
    $isUpper = true;
  }else if($character == "X"){
    $isUpper = true;
  }else if($character == "Y"){
    $isUpper = true;
  }else if($character == "Z"){
    $isUpper = true;
  }

  return $isUpper;
}
function charIsLowerCase($character){

  $isLower = false;
  if($character == "a"){
    $isLower = true;
  }else if($character == "b"){
    $isLower = true;
  }else if($character == "c"){
    $isLower = true;
  }else if($character == "d"){
    $isLower = true;
  }else if($character == "e"){
    $isLower = true;
  }else if($character == "f"){
    $isLower = true;
  }else if($character == "g"){
    $isLower = true;
  }else if($character == "h"){
    $isLower = true;
  }else if($character == "i"){
    $isLower = true;
  }else if($character == "j"){
    $isLower = true;
  }else if($character == "k"){
    $isLower = true;
  }else if($character == "l"){
    $isLower = true;
  }else if($character == "m"){
    $isLower = true;
  }else if($character == "n"){
    $isLower = true;
  }else if($character == "o"){
    $isLower = true;
  }else if($character == "p"){
    $isLower = true;
  }else if($character == "q"){
    $isLower = true;
  }else if($character == "r"){
    $isLower = true;
  }else if($character == "s"){
    $isLower = true;
  }else if($character == "t"){
    $isLower = true;
  }else if($character == "u"){
    $isLower = true;
  }else if($character == "v"){
    $isLower = true;
  }else if($character == "w"){
    $isLower = true;
  }else if($character == "x"){
    $isLower = true;
  }else if($character == "y"){
    $isLower = true;
  }else if($character == "z"){
    $isLower = true;
  }

  return $isLower;
}
function charIsLetter($character){
  return charIsUpperCase($character) || charIsLowerCase($character);
}
function charIsNumber($character){

  $isNumberx = false;
  if($character == "0"){
    $isNumberx = true;
  }else if($character == "1"){
    $isNumberx = true;
  }else if($character == "2"){
    $isNumberx = true;
  }else if($character == "3"){
    $isNumberx = true;
  }else if($character == "4"){
    $isNumberx = true;
  }else if($character == "5"){
    $isNumberx = true;
  }else if($character == "6"){
    $isNumberx = true;
  }else if($character == "7"){
    $isNumberx = true;
  }else if($character == "8"){
    $isNumberx = true;
  }else if($character == "9"){
    $isNumberx = true;
  }

  return $isNumberx;
}
function charIsWhiteSpace($character){

  $isWhiteSpacex = false;
  if($character == " "){
    $isWhiteSpacex = true;
  }else if($character == "\t"){
    $isWhiteSpacex = true;
  }else if($character == "\n"){
    $isWhiteSpacex = true;
  }else if($character == "\r"){
    $isWhiteSpacex = true;
  }

  return $isWhiteSpacex;
}
function charIsSymbol($character){

  $isSymbolx = false;
  if($character == "!"){
    $isSymbolx = true;
  }else if($character == "\""){
    $isSymbolx = true;
  }else if($character == "#"){
    $isSymbolx = true;
  }else if($character == "$"){
    $isSymbolx = true;
  }else if($character == "%"){
    $isSymbolx = true;
  }else if($character == "&"){
    $isSymbolx = true;
  }else if($character == "\'"){
    $isSymbolx = true;
  }else if($character == "("){
    $isSymbolx = true;
  }else if($character == ")"){
    $isSymbolx = true;
  }else if($character == "*"){
    $isSymbolx = true;
  }else if($character == "+"){
    $isSymbolx = true;
  }else if($character == ","){
    $isSymbolx = true;
  }else if($character == "-"){
    $isSymbolx = true;
  }else if($character == "."){
    $isSymbolx = true;
  }else if($character == "/"){
    $isSymbolx = true;
  }else if($character == ":"){
    $isSymbolx = true;
  }else if($character == ";"){
    $isSymbolx = true;
  }else if($character == "<"){
    $isSymbolx = true;
  }else if($character == "="){
    $isSymbolx = true;
  }else if($character == ">"){
    $isSymbolx = true;
  }else if($character == "?"){
    $isSymbolx = true;
  }else if($character == "@"){
    $isSymbolx = true;
  }else if($character == "["){
    $isSymbolx = true;
  }else if($character == "\\"){
    $isSymbolx = true;
  }else if($character == "]"){
    $isSymbolx = true;
  }else if($character == "^"){
    $isSymbolx = true;
  }else if($character == "_"){
    $isSymbolx = true;
  }else if($character == "`"){
    $isSymbolx = true;
  }else if($character == "{"){
    $isSymbolx = true;
  }else if($character == "|"){
    $isSymbolx = true;
  }else if($character == "}"){
    $isSymbolx = true;
  }else if($character == "~"){
    $isSymbolx = true;
  }

  return $isSymbolx;
}
function charCharacterIsBefore($a, $b){

  $ad = uniord($a);
  $bd = uniord($b);

  return $ad < $bd;
}

