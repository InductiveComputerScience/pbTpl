// Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

import static java.lang.Math.*;

public class TextualTemplates{
static public class LinkedListNodeNodes{
	public boolean end;
	public Node value;
	public LinkedListNodeNodes next;
}
static public class LinkedListNodes{
	public LinkedListNodeNodes first;
	public LinkedListNodeNodes last;
}
static public class Node{
	public char [] type;
	public char [] p1;
	public char [] p2;
	public Node block1;
	public boolean hasElseBlock;
	public Node block2;
	public LinkedListNodes nodes;
}
	public static LinkedListNodes CreateLinkedListNodes(){
		LinkedListNodes ll;

		ll = new LinkedListNodes();
		ll.first = new LinkedListNodeNodes();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void LinkedListAddNode(LinkedListNodes ll, Node value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeNodes();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static Node [] LinkedListNodesToArray(LinkedListNodes ll){
		Node [] array;
		double length, i;
		LinkedListNodeNodes node;

		node = ll.first;

		length = LinkedListNodesLength(ll);

		array = new Node [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static double LinkedListNodesLength(LinkedListNodes ll){
		double l;
		LinkedListNodeNodes node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void FreeLinkedListNode(LinkedListNodes ll){
		LinkedListNodeNodes node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static boolean IsValidTemplate(char [] template){
		StringReference errorMessage;
		boolean success;
		LinkedListStrings tokens;
		StringReference [] ts;
		Node root;

		tokens = CreateLinkedListString();
		errorMessage = new StringReference();

		success = GenerateTokensFromTemplate(template, tokens, errorMessage);

		if(success){
			root = new Node();

			ts = LinkedListStringsToArray(tokens);

			success = ParseTemplate(ts, root, errorMessage);
		}

		if(!success){
			FreeStringReference(errorMessage);
		}

		return success;
	}

	public static boolean GenerateTokensFromTemplate(char [] template, LinkedListStrings tokens, StringReference errorMessage){
		boolean success, found;
		double i, j, l;
		char pc, c;
		char [] cs, a;
		DynamicArrayCharacters da;

		success = true;
		cs = new char [4];

		da = CreateDynamicArrayCharacters();

		pc = 'x';
		for(i = 0d; i < template.length && success; ){
			c = template[(int)(i)];

			if(c != '{'){
				DynamicArrayAddCharacter(da, c);
				i = i + 1d;
			}else{
				FillString(cs, 'x');
				for(j = 0d; i + j < template.length && j < cs.length; j = j + 1d){
					cs[(int)(j)] = template[(int)(i + j)];
				}

				if(StringsEqual(cs, "{use".toCharArray()) || StringsEqual(cs, "{end".toCharArray()) || StringsEqual(cs, "{pri".toCharArray()) || StringsEqual(cs, "{for".toCharArray()) || StringsEqual(cs, "{if ".toCharArray()) || StringsEqual(cs, "{els".toCharArray())){
					if(pc != '\\'){
						/* Find end.*/
						found = false;
						for(l = 0d; i + l < template.length && !found; l = l + 1d){
							if(template[(int)(i + l)] == '}'){
								found = true;
							}
						}

						if(found){
							if(da.length > 0d){
								a = DynamicArrayCharactersToArray(da);
								LinkedListAddString(tokens, a);
								FreeDynamicArrayCharacters(da);
								da = CreateDynamicArrayCharacters();
							}

							for(j = 0d; j < l; j = j + 1d){
								DynamicArrayAddCharacter(da, template[(int)(i + j)]);
							}

							a = DynamicArrayCharactersToArray(da);
							LinkedListAddString(tokens, a);
							FreeDynamicArrayCharacters(da);
							da = CreateDynamicArrayCharacters();

							i = i + l;
						}else{
							success = false;
							errorMessage.string = "Template command found, but not ended properly.".toCharArray();
						}
					}else{
						DynamicArrayAddCharacter(da, c);
						i = i + 1d;
					}
				}else{
					DynamicArrayAddCharacter(da, c);
					i = i + 1d;
				}
			}

			pc = c;
		}

		if(da.length > 0d){
			a = DynamicArrayCharactersToArray(da);
			LinkedListAddString(tokens, a);
		}

		FreeDynamicArrayCharacters(da);

		return success;
	}

	public static boolean GenerateDocument(char [] template, Element data, StringReference document, StringReference errorMessage){
		LinkedListCharacters ll;
		boolean success;
		LinkedListStrings tokens;
		StringReference [] ts;
		Node root;

		tokens = CreateLinkedListString();

		success = GenerateTokensFromTemplate(template, tokens, errorMessage);

		if(success){
			root = new Node();

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

	public static boolean GenerateDocumentFromBlock(Node root, Element data, LinkedListCharacters ll, StringReference errorMessage){
		LinkedListNodeNodes n;
		boolean success;

		n = root.nodes.first;
		success = true;

		for(; !n.end && success; ){
			success = GenerateDocumentFromNode(n.value, data, ll, errorMessage);
			n = n.next;
		}

		return success;
	}

	public static boolean GenerateDocumentFromNode(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage){
		boolean success;
		BooleanReference found;
		char [] valueString;
		Element value;

		success = true;
		found = new BooleanReference();

		if(StringsEqual(n.type, "block".toCharArray())){
			success = GenerateDocumentFromBlock(n, data, ll, errorMessage);
		}else if(StringsEqual(n.type, "use".toCharArray())){
			success = GenerateDocumentFromUse(n, data, ll, errorMessage);
		}else if(StringsEqual(n.type, "if".toCharArray())){
			success = GenerateDocumentFromIf(n, data, ll, errorMessage);
		}else if(StringsEqual(n.type, "foreach".toCharArray())){
			success = GenerateDocumentFromForeach(n, data, ll, errorMessage);
		}else if(StringsEqual(n.type, "text".toCharArray())){
			success = true;
			LinkedListCharactersAddString(ll, n.p1);
		}else if(StringsEqual(n.type, "print".toCharArray())){
			if(StringsEqual(data.type, "object".toCharArray())){
				value = GetObjectValueWithCheck(data.object, n.p1, found);

				if(found.booleanValue){
					if(StringsEqual(value.type, "string".toCharArray())){
						valueString = value.string;
					}else{
						valueString = WriteJSON(value);
					}
					LinkedListCharactersAddString(ll, valueString);
				}else{
					success = false;
					errorMessage.string = "Key for printing not found in JSON object: ".toCharArray();
					errorMessage.string = sConcatenateString(errorMessage.string, n.p1);
				}
			}else{
				success = false;
				errorMessage.string = "Data structure for print command is not a JSON object.".toCharArray();
			}
		}

		return success;
	}

	public static boolean GenerateDocumentFromUse(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage){
		boolean success;
		BooleanReference found;
		Element value;

		found = new BooleanReference();

		if(StringsEqual(data.type, "object".toCharArray())){
			value = GetObjectValueWithCheck(data.object, n.p1, found);

			if(found.booleanValue){
				success = GenerateDocumentFromNode(n.block1, value, ll, errorMessage);
			}else{
				success = false;
				errorMessage.string = "Key for use not found in JSON object.".toCharArray();
			}
		}else{
			success = false;
			errorMessage.string = "Data structure for use command is not a JSON object.".toCharArray();
		}

		return success;
	}

	public static boolean GenerateDocumentFromIf(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage){
		boolean success;
		BooleanReference found;
		Element value;

		success = true;
		found = new BooleanReference();

		if(StringsEqual(data.type, "object".toCharArray())){
			value = GetObjectValueWithCheck(data.object, n.p1, found);

			if(found.booleanValue){
				if(StringsEqual(value.type, "boolean".toCharArray())){
					if(value.booleanValue){
						success = GenerateDocumentFromBlock(n.block1, data, ll, errorMessage);
					}

					if(n.hasElseBlock){
						if(!value.booleanValue){
							success = GenerateDocumentFromBlock(n.block2, data, ll, errorMessage);
						}
					}
				}else{
					success = false;
					errorMessage.string = "Value for if not boolean.".toCharArray();
				}
			}else{
				success = false;
				errorMessage.string = "Key for if not found in JSON object: ".toCharArray();
				errorMessage.string = sConcatenateString(errorMessage.string, n.p1);
			}
		}else{
			success = false;
			errorMessage.string = "Data structure for if command is not a JSON object.".toCharArray();
		}

		return success;
	}

	public static boolean GenerateDocumentFromForeach(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage){
		boolean success;
		BooleanReference found;
		Element value, loopVar, arrayElement;
		double i;

		success = true;
		found = new BooleanReference();
		loopVar = CreateObjectElement(0d);

		PutStringElementMap(loopVar.object, n.p1, new Element());

		if(StringsEqual(data.type, "object".toCharArray())){
			value = GetObjectValueWithCheck(data.object, n.p2, found);

			if(found.booleanValue){
				if(StringsEqual(value.type, "array".toCharArray())){

					for(i = 0d; i < value.array.length; i = i + 1d){
						arrayElement = value.array[(int)(i)];
						if(StringsEqual(arrayElement.type, "object".toCharArray())){
							success = GenerateDocumentFromBlock(n.block1, arrayElement, ll, errorMessage);
						}else{
							SetStringElementMap(loopVar.object, 0d, n.p1, arrayElement);
							success = GenerateDocumentFromBlock(n.block1, loopVar, ll, errorMessage);
						}
					}
				}else{
					success = false;
					errorMessage.string = "Value for foreach is not an array.".toCharArray();
				}
			}else{
				success = false;
				errorMessage.string = "Key for foreach not found in JSON object: ".toCharArray();
				errorMessage.string = sConcatenateString(errorMessage.string, n.p2);
			}
		}else{
			success = false;
			errorMessage.string = "Data structure for foreach command is not a JSON object.".toCharArray();
		}

		return success;
	}

	public static boolean ParseTemplate(StringReference [] tokens, Node node, StringReference errorMessage){
		boolean success;
		NumberReference position;

		position = CreateNumberReference(0d);
		success = ParseTemplateBlock(tokens, position, node, errorMessage);

		if(success){
			if(position.numberValue != tokens.length){
				success = false;
				errorMessage.string = "Unexpected token at the end of template.".toCharArray();
			}
		}

		delete(position);

		return success;
	}

	public static boolean ParseTemplateBlock(StringReference [] tokens, NumberReference position, Node node, StringReference errorMessage){
		boolean success, done;
		Node tn, nb;

		success = true;
		done = false;

		node.type = "block".toCharArray();
		node.nodes = CreateLinkedListNodes();

		for(; position.numberValue < tokens.length && success && !done; ){
			tn = new Node();
			success = ParseNodeString(tokens[(int)(position.numberValue)].string, tn, errorMessage);

			if(success){
				if(StringsEqual(tn.type, "text".toCharArray()) || StringsEqual(tn.type, "print".toCharArray())){
					LinkedListAddNode(node.nodes, tn);
					position.numberValue = position.numberValue + 1d;
				}else if(StringsEqual(tn.type, "use".toCharArray())){
					nb = new Node();
					success = ParseUseBlock(tokens, position, nb, errorMessage);
					LinkedListAddNode(node.nodes, nb);
				}else if(StringsEqual(tn.type, "if".toCharArray())){
					nb = new Node();
					success = ParseIfBlock(tokens, position, nb, errorMessage);
					LinkedListAddNode(node.nodes, nb);
				}else if(StringsEqual(tn.type, "foreach".toCharArray())){
					nb = new Node();
					success = ParseForeachBlock(tokens, position, nb, errorMessage);
					LinkedListAddNode(node.nodes, nb);
				}else{
					done = true;
				}
			}
		}

		return success;
	}

	public static boolean ParseUseBlock(StringReference [] tokens, NumberReference position, Node useBlock, StringReference errorMessage){
		boolean success;
		Node n;

		n = new Node();
		ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

		useBlock.type = CopyString(n.type);
		useBlock.p1 = CopyString(n.p1);
		useBlock.block1 = new Node();

		position.numberValue = position.numberValue + 1d;

		success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage);

		if(success){
			if(position.numberValue < tokens.length){
				n = new Node();
				ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

				if(StringsEqual(n.type, "end".toCharArray())){
					success = true;
					position.numberValue = position.numberValue + 1d;
				}else{
					success = false;
					errorMessage.string = "End block expected at the end of use block.".toCharArray();
				}
			}else{
				success = false;
				errorMessage.string = "End block expected at the end of use block.".toCharArray();
			}
		}

		return success;
	}

	public static boolean ParseIfBlock(StringReference [] tokens, NumberReference position, Node ifBlock, StringReference errorMessage){
		boolean success;
		Node n;

		n = new Node();
		ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

		ifBlock.type = CopyString(n.type);
		ifBlock.p1 = CopyString(n.p1);
		ifBlock.block1 = new Node();
		ifBlock.hasElseBlock = false;

		position.numberValue = position.numberValue + 1d;

		success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage);

		if(success){
			if(position.numberValue < tokens.length){
				n = new Node();
				ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

				if(StringsEqual(n.type, "end".toCharArray())){
					success = true;
					position.numberValue = position.numberValue + 1d;
				}else if(StringsEqual(n.type, "else".toCharArray())){
					position.numberValue = position.numberValue + 1d;
					ifBlock.hasElseBlock = true;
					ifBlock.block2 = new Node();
					success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage);

					if(success){
						if(position.numberValue < tokens.length){
							n = new Node();
							ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

							if(StringsEqual(n.type, "end".toCharArray())){
								success = true;
								position.numberValue = position.numberValue + 1d;
							}else{
								success = false;
								errorMessage.string = "End block expected at the end of else block.".toCharArray();
							}
						}else{
							success = false;
							errorMessage.string = "End block expected at the end of else block.".toCharArray();
						}
					}
				}else{
					success = false;
					errorMessage.string = "End or else block expected at the end of if block.".toCharArray();
				}
			}else{
				success = false;
				errorMessage.string = "End or else block expected at the end of if block.".toCharArray();
			}
		}

		return success;
	}

	public static boolean ParseForeachBlock(StringReference [] tokens, NumberReference position, Node foreachBlock, StringReference errorMessage){
		boolean success;
		Node n;

		n = new Node();
		ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

		foreachBlock.type = CopyString(n.type);
		foreachBlock.p1 = CopyString(n.p1);
		foreachBlock.p2 = CopyString(n.p2);
		foreachBlock.block1 = new Node();

		position.numberValue = position.numberValue + 1d;

		success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage);

		if(success){
			if(position.numberValue < tokens.length){
				n = new Node();
				ParseNodeString(tokens[(int)(position.numberValue)].string, n, errorMessage);

				if(StringsEqual(n.type, "end".toCharArray())){
					success = true;
					position.numberValue = position.numberValue + 1d;
				}else{
					success = false;
					errorMessage.string = "End block expected at the end of for each block.".toCharArray();
				}
			}else{
				success = false;
				errorMessage.string = "End block expected at the end of foreach block.".toCharArray();
			}
		}

		return success;
	}

	public static boolean ParseNodeString(char [] token, Node node, StringReference errorMessage){
		char [] command;
		boolean success, isText;
		StringReference [] parts;

		success = true;
		isText = false;

		if(token.length <= 2d){
			isText = true;
		}else if(token[0] == '\\' && token[1] == '{'){
			isText = true;
		}else if(token[0] != '{'){
			isText = true;
		}else{
			command = strSubstring(token, 1d, token.length - 1d);
			parts = sSplitByCharacter(command, ' ');

			if(command.length > 0d){
				if(StringsEqual(parts[0].string, "use".toCharArray())){
					if(parts.length == 2d){
						node.type = CopyString(parts[0].string);
						node.p1 = CopyString(parts[1].string);
					}else{
						success = false;
						errorMessage.string = "The use command takes one parameter.".toCharArray();
					}
				}else if(StringsEqual(parts[0].string, "end".toCharArray())){
					if(parts.length == 1d){
						node.type = CopyString(parts[0].string);
					}else{
						success = false;
						errorMessage.string = "The end command takes no parameters.".toCharArray();
					}
				}else if(StringsEqual(parts[0].string, "print".toCharArray())){
					if(parts.length == 2d){
						node.type = CopyString(parts[0].string);
						node.p1 = CopyString(parts[1].string);
					}else{
						success = false;
						errorMessage.string = "The print command takes one parameter.".toCharArray();
					}
				}else if(StringsEqual(parts[0].string, "foreach".toCharArray())){
					if(parts.length == 4d){
						if(StringsEqual(parts[2].string, "in".toCharArray())){
							node.type = CopyString(parts[0].string);
							node.p1 = CopyString(parts[1].string);
							node.p2 = CopyString(parts[3].string);
						}else{
							success = false;
							errorMessage.string = "The foreach command must have \"in\" after the first parameter.".toCharArray();
						}
					}else{
						success = false;
						errorMessage.string = "The foreach command takes three parameters.".toCharArray();
					}
				}else if(StringsEqual(parts[0].string, "if".toCharArray())){
					if(parts.length == 2d){
						node.type = CopyString(parts[0].string);
						node.p1 = CopyString(parts[1].string);
					}else{
						success = false;
						errorMessage.string = "The if command takes one parameter.".toCharArray();
					}
				}else if(StringsEqual(parts[0].string, "else".toCharArray())){
					if(parts.length == 1d){
						node.type = CopyString(parts[0].string);
					}else{
						success = false;
						errorMessage.string = "The else command takes no parameters.".toCharArray();
					}
				}else{
					isText = true;
				}
			}else{
				isText = true;
			}
		}

		if(isText){
			node.type = "text".toCharArray();
			node.p1 = sReplaceString(token, "\\{print ".toCharArray(), "{print ".toCharArray());
			node.p1 = sReplaceString(node.p1, "\\{use ".toCharArray(), "{use ".toCharArray());
			node.p1 = sReplaceString(node.p1, "\\{if ".toCharArray(), "{if ".toCharArray());
			node.p1 = sReplaceString(node.p1, "\\{end}".toCharArray(), "{end}".toCharArray());
			node.p1 = sReplaceString(node.p1, "\\{foreach ".toCharArray(), "{foreach ".toCharArray());
			node.p1 = sReplaceString(node.p1, "\\{else}".toCharArray(), "{else}".toCharArray());
		}

		return success;
	}

	public static double test(){
		NumberReference failures;

		failures = CreateNumberReference(0d);

		testTokenGeneration(failures);

		testGenerateDocument1(failures);
		testGenerateDocument2(failures);
		testGenerateDocument3(failures);
		testGenerateDocument4(failures);
		testGenerateDocument5(failures);
		testGenerateDocument6(failures);
		testGenerateDocument7(failures);

		return failures.numberValue;
	}

	public static void testTokenGeneration(NumberReference failures){
		char [] template;
		LinkedListStrings tokens;
		boolean success;
		StringReference errorMessage;

		errorMessage = new StringReference();

		tokens = CreateLinkedListString();
		template = "This is a template, this is a value: {print a}.".toCharArray();
		success = GenerateTokensFromTemplate(template, tokens, errorMessage);
		AssertTrue(success, failures);
		AssertEquals(3d, LinkedListStringsLength(tokens), failures);

		tokens = CreateLinkedListString();
		template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".toCharArray();
		success = GenerateTokensFromTemplate(template, tokens, errorMessage);
		AssertTrue(success, failures);
		AssertEquals(9d, LinkedListStringsLength(tokens), failures);

		tokens = CreateLinkedListString();
		template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".toCharArray();
		success = GenerateTokensFromTemplate(template, tokens, errorMessage);
		AssertTrue(success, failures);
		AssertEquals(16d, LinkedListStringsLength(tokens), failures);

		tokens = CreateLinkedListString();
		template = "T: {foreach a in b}{print a}{end}.".toCharArray();
		success = GenerateTokensFromTemplate(template, tokens, errorMessage);
		AssertTrue(success, failures);
		AssertEquals(5d, LinkedListStringsLength(tokens), failures);
	}

	public static void testGenerateDocument1(NumberReference failures){
		AssertTemplateResult("This is a template, this is a value: {print a}.".toCharArray(), "{\"c\": 5, \"a\": 6}".toCharArray(), "This is a template, this is a value: 6.".toCharArray(), failures);
	}

	public static void testGenerateDocument2(NumberReference failures){
		AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".toCharArray(), "{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}".toCharArray(), "This is a template, this is a value: 6 1 2.".toCharArray(), failures);
	}

	public static void testGenerateDocument3(NumberReference failures){
		AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".toCharArray(), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}".toCharArray(), "This is a template, this is a value: 6 1 23 416.".toCharArray(), failures);
	}

	public static void testGenerateDocument4(NumberReference failures){
		AssertTemplateResult("T: {if a}a{end}.".toCharArray(), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}".toCharArray(), "T: a.".toCharArray(), failures);

		AssertTemplateResult("T: {if a}a{else}b{end}.".toCharArray(), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}".toCharArray(), "T: b.".toCharArray(), failures);
	}

	public static void testGenerateDocument5(NumberReference failures){
		AssertTemplateResult("T: {foreach a in b}{print a}{end}.".toCharArray(), "{\"b\": [1, 2, 3, 4]}".toCharArray(), "T: 1234.".toCharArray(), failures);
	}

	public static void testGenerateDocument6(NumberReference failures){
		AssertTemplateResult("{test}\\{print a}.".toCharArray(), "{\"c\": 5, \"a\": 6}".toCharArray(), "{test}{print a}.".toCharArray(), failures);
	}

	public static void testGenerateDocument7(NumberReference failures){
		AssertTemplateResult("{".toCharArray(), "{}".toCharArray(), "{".toCharArray(), failures);

		AssertTemplateResult("{  ".toCharArray(), "{}".toCharArray(), "{  ".toCharArray(), failures);

		AssertTemplateResult("{use a}\\{print a}{end}".toCharArray(), "{\"a\": {}}".toCharArray(), "{print a}".toCharArray(), failures);

		AssertTemplateResult("\\{print a}{print a}}".toCharArray(), "{\"a\": 3}".toCharArray(), "{print a}3}".toCharArray(), failures);

		AssertTemplateResult("\\\\{print a}{print a}}".toCharArray(), "{\"a\": 3}".toCharArray(), "\\{print a}3}".toCharArray(), failures);

		AssertTemplateResult("\\\\{print a}{print a}\\\\{print a}}".toCharArray(), "{\"a\": 3}".toCharArray(), "\\{print a}3\\{print a}}".toCharArray(), failures);

		AssertTemplateResult("\\{print a}{print a}\\{".toCharArray(), "{\"a\": 3}".toCharArray(), "{print a}3\\{".toCharArray(), failures);

		AssertTemplateResult("    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>".toCharArray(), "{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}".toCharArray(), "    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>".toCharArray(), failures);

		AssertTemplateError("{print".toCharArray(), "{}".toCharArray(), "Template command found, but not ended properly.".toCharArray(), failures);

		AssertTemplateError("{print a}".toCharArray(), "{}".toCharArray(), "Key for printing not found in JSON object: a".toCharArray(), failures);

		AssertTemplateError("This is a template, this is a value: {print a {print a}.".toCharArray(), "{\"a\": 5}".toCharArray(), "The print command takes one parameter.".toCharArray(), failures);

		AssertTemplateError("This is a {use a}\\{print a}template, this is a value: {print a}.{end}".toCharArray(), "{\"a\": 5}".toCharArray(), "Data structure for print command is not a JSON object.".toCharArray(), failures);

		AssertTemplateError("{use a}".toCharArray(), "{}".toCharArray(), "End block expected at the end of use block.".toCharArray(), failures);

		AssertTemplateError("{if a}".toCharArray(), "{\"a\": true}".toCharArray(), "End or else block expected at the end of if block.".toCharArray(), failures);

		AssertTemplateError("{if a}{else}".toCharArray(), "{\"a\": true}".toCharArray(), "End block expected at the end of else block.".toCharArray(), failures);

		AssertTemplateError("{foreach x in a}".toCharArray(), "{\"a\": [1, 2, 3, 4]}".toCharArray(), "End block expected at the end of foreach block.".toCharArray(), failures);
	}

	public static void AssertTemplateResult(char [] template, char [] json, char [] result, NumberReference failures){
		ElementReference data;
		StringArrayReference errorMessages;
		StringReference document;
		StringReference errorMessage;
		boolean success;

		data = new ElementReference();
		errorMessages = new StringArrayReference();
		document = new StringReference();
		errorMessage = new StringReference();

		success = ReadJSON(json, data, errorMessages);

		AssertTrue(success, failures);

		if(success){
			success = GenerateDocument(template, data.element, document, errorMessage);

			AssertTrue(success, failures);

			if(success){
				AssertStringEquals(document.string, result, failures);
			}
		}
	}

	public static void AssertTemplateError(char [] template, char [] json, char [] errorMessage, NumberReference failures){
		ElementReference data;
		StringArrayReference errorMessages;
		StringReference document;
		StringReference errorMessageRef;
		boolean success;

		data = new ElementReference();
		errorMessages = new StringArrayReference();
		document = new StringReference();
		errorMessageRef = new StringReference();

		success = ReadJSON(json, data, errorMessages);

		AssertTrue(success, failures);

		if(success){
			success = GenerateDocument(template, data.element, document, errorMessageRef);

			AssertFalse(success, failures);

			if(!success){
				AssertStringEquals(errorMessage, errorMessageRef.string, failures);
			}
		}
	}

static public class BooleanArrayReference{
	public boolean [] booleanArray;
}
static public class BooleanReference{
	public boolean booleanValue;
}
static public class CharacterReference{
	public char characterValue;
}
static public class NumberArrayReference{
	public double [] numberArray;
}
static public class NumberReference{
	public double numberValue;
}
static public class StringArrayReference{
	public StringReference [] stringArray;
}
static public class StringReference{
	public char [] string;
}
	public static BooleanReference CreateBooleanReference(boolean value){
		BooleanReference ref;

		ref = new BooleanReference();
		ref.booleanValue = value;

		return ref;
	}

	public static BooleanArrayReference CreateBooleanArrayReference(boolean [] value){
		BooleanArrayReference ref;

		ref = new BooleanArrayReference();
		ref.booleanArray = value;

		return ref;
	}

	public static BooleanArrayReference CreateBooleanArrayReferenceLengthValue(double length, boolean value){
		BooleanArrayReference ref;
		double i;

		ref = new BooleanArrayReference();
		ref.booleanArray = new boolean [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			ref.booleanArray[(int)(i)] = value;
		}

		return ref;
	}

	public static void FreeBooleanArrayReference(BooleanArrayReference booleanArrayReference){
		delete(booleanArrayReference.booleanArray);
		delete(booleanArrayReference);
	}

	public static CharacterReference CreateCharacterReference(char value){
		CharacterReference ref;

		ref = new CharacterReference();
		ref.characterValue = value;

		return ref;
	}

	public static NumberReference CreateNumberReference(double value){
		NumberReference ref;

		ref = new NumberReference();
		ref.numberValue = value;

		return ref;
	}

	public static NumberArrayReference CreateNumberArrayReference(double [] value){
		NumberArrayReference ref;

		ref = new NumberArrayReference();
		ref.numberArray = value;

		return ref;
	}

	public static NumberArrayReference CreateNumberArrayReferenceLengthValue(double length, double value){
		NumberArrayReference ref;
		double i;

		ref = new NumberArrayReference();
		ref.numberArray = new double [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			ref.numberArray[(int)(i)] = value;
		}

		return ref;
	}

	public static void FreeNumberArrayReference(NumberArrayReference numberArrayReference){
		delete(numberArrayReference.numberArray);
		delete(numberArrayReference);
	}

	public static StringReference CreateStringReference(char [] value){
		StringReference ref;

		ref = new StringReference();
		ref.string = value;

		return ref;
	}

	public static StringReference CreateStringReferenceLengthValue(double length, char value){
		StringReference ref;
		double i;

		ref = new StringReference();
		ref.string = new char [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			ref.string[(int)(i)] = value;
		}

		return ref;
	}

	public static void FreeStringReference(StringReference stringReference){
		delete(stringReference.string);
		delete(stringReference);
	}

	public static StringArrayReference CreateStringArrayReference(StringReference [] strings){
		StringArrayReference ref;

		ref = new StringArrayReference();
		ref.stringArray = strings;

		return ref;
	}

	public static StringArrayReference CreateStringArrayReferenceLengthValue(double length, char [] value){
		StringArrayReference ref;
		double i;

		ref = new StringArrayReference();
		ref.stringArray = new StringReference [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			ref.stringArray[(int)(i)] = CreateStringReference(value);
		}

		return ref;
	}

	public static void FreeStringArrayReference(StringArrayReference stringArrayReference){
		double i;

		for(i = 0d; i < stringArrayReference.stringArray.length; i = i + 1d){
			delete(stringArrayReference.stringArray[(int)(i)]);
		}
		delete(stringArrayReference.stringArray);
		delete(stringArrayReference);
	}

static public class ElementArrayReference{
	public Element [] array;
}
static public class LinkedListElements{
	public LinkedListNodeElements first;
	public LinkedListNodeElements last;
}
static public class LinkedListNodeElements{
	public boolean end;
	public Element value;
	public LinkedListNodeElements next;
}
static public class Element{
	public char [] type;
	public StringElementMap object;
	public Element [] array;
	public char [] string;
	public double number;
	public boolean booleanValue;
}
static public class ElementReference{
	public Element element;
}
static public class ElementType{
	public char [] name;
}
static public class StringElementMap{
	public StringArrayReference stringListRef;
	public ElementArrayReference elementListRef;
}
	public static boolean IsValidJSON(char [] json, StringArrayReference errorMessage){
		boolean success;
		ElementReference elementReference;

		elementReference = new ElementReference();

		success = ReadJSON(json, elementReference, errorMessage);

		if(success){
			DeleteElement(elementReference.element);
		}

		return success;
	}

	public static boolean JSONTokenize(char [] json, StringArrayReference tokensReference, StringArrayReference errorMessages){
		double i;
		char c;
		char [] str;
		StringReference stringReference, tokenReference;
		NumberReference stringLength;
		boolean success;
		lLinkedListStrings ll;

		ll = lCreateLinkedListString();
		success = true;

		stringLength = new NumberReference();
		tokenReference = new StringReference();

		for(i = 0d; i < json.length && success; ){
			c = json[(int)(i)];

			if(c == '{'){
				lLinkedListAddString(ll, "{".toCharArray());
				i = i + 1d;
			}else if(c == '}'){
				lLinkedListAddString(ll, "}".toCharArray());
				i = i + 1d;
			}else if(c == '['){
				lLinkedListAddString(ll, "[".toCharArray());
				i = i + 1d;
			}else if(c == ']'){
				lLinkedListAddString(ll, "]".toCharArray());
				i = i + 1d;
			}else if(c == ':'){
				lLinkedListAddString(ll, ":".toCharArray());
				i = i + 1d;
			}else if(c == ','){
				lLinkedListAddString(ll, ",".toCharArray());
				i = i + 1d;
			}else if(c == 'f'){
				success = GetJSONPrimitiveName(json, i, errorMessages, "false".toCharArray(), tokenReference);
				if(success){
					lLinkedListAddString(ll, "false".toCharArray());
					i = i + "false".toCharArray().length;
				}
			}else if(c == 't'){
				success = GetJSONPrimitiveName(json, i, errorMessages, "true".toCharArray(), tokenReference);
				if(success){
					lLinkedListAddString(ll, "true".toCharArray());
					i = i + "true".toCharArray().length;
				}
			}else if(c == 'n'){
				success = GetJSONPrimitiveName(json, i, errorMessages, "null".toCharArray(), tokenReference);
				if(success){
					lLinkedListAddString(ll, "null".toCharArray());
					i = i + "null".toCharArray().length;
				}
			}else if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
				/* Skip.*/
				i = i + 1d;
			}else if(c == '\"'){
				success = GetJSONString(json, i, tokenReference, stringLength, errorMessages);
				if(success){
					lLinkedListAddString(ll, tokenReference.string);
					i = i + stringLength.numberValue;
				}
			}else if(IsJSONNumberCharacter(c)){
				success = GetJSONNumberToken(json, i, tokenReference, errorMessages);
				if(success){
					lLinkedListAddString(ll, tokenReference.string);
					i = i + tokenReference.string.length;
				}
			}else{
				str = strConcatenateCharacter("Invalid start of Token: ".toCharArray(), c);
				stringReference = CreateStringReference(str);
				lAddStringRef(errorMessages, stringReference);
				i = i + 1d;
				success = false;
			}
		}

		if(success){
			lLinkedListAddString(ll, "<end>".toCharArray());
			tokensReference.stringArray = lLinkedListStringsToArray(ll);
			lFreeLinkedListString(ll);
		}

		return success;
	}

	public static boolean GetJSONNumberToken(char [] json, double start, StringReference tokenReference, StringArrayReference errorMessages){
		char c;
		double end, i;
		boolean done, success;
		char [] numberString;

		end = json.length;
		done = false;

		for(i = start; i < json.length && !done; i = i + 1d){
			c = json[(int)(i)];
			if(!IsJSONNumberCharacter(c)){
				done = true;
				end = i;
			}
		}

		numberString = strSubstring(json, start, end);

		success = IsValidJSONNumber(numberString, errorMessages);

		tokenReference.string = numberString;

		return success;
	}

	public static boolean IsValidJSONNumber(char [] n, StringArrayReference errorMessages){
		boolean success;
		double i;

		i = 0d;

		/* JSON allows an optional negative sign.*/
		if(n[(int)(i)] == '-'){
			i = i + 1d;
		}

		if(i < n.length){
			success = IsValidJSONNumberAfterSign(n, i, errorMessages);
		}else{
			success = false;
			lAddStringRef(errorMessages, CreateStringReference("Number must contain at least one digit.".toCharArray()));
		}

		return success;
	}

	public static boolean IsValidJSONNumberAfterSign(char [] n, double i, StringArrayReference errorMessages){
		boolean success;

		if(charIsNumber(n[(int)(i)])){
			/* 0 first means only 0.*/
			if(n[(int)(i)] == '0'){
				i = i + 1d;
			}else{
				/* 1-9 first, read following digits.*/
				i = IsValidJSONNumberAdvancePastDigits(n, i);
			}

			if(i < n.length){
				success = IsValidJSONNumberFromDotOrExponent(n, i, errorMessages);
			}else{
				/* If integer, we are done now.*/
				success = true;
			}
		}else{
			success = false;
			lAddStringRef(errorMessages, CreateStringReference("A number must start with 0-9 (after the optional sign).".toCharArray()));
		}

		return success;
	}

	public static double IsValidJSONNumberAdvancePastDigits(char [] n, double i){
		boolean done;

		i = i + 1d;
		done = false;
		for(; i < n.length && !done; ){
			if(charIsNumber(n[(int)(i)])){
				i = i + 1d;
			}else{
				done = true;
			}
		}

		return i;
	}

	public static boolean IsValidJSONNumberFromDotOrExponent(char [] n, double i, StringArrayReference errorMessages){
		boolean wasDotAndOrE, success;

		wasDotAndOrE = false;
		success = true;

		if(n[(int)(i)] == '.'){
			i = i + 1d;
			wasDotAndOrE = true;

			if(i < n.length){
				if(charIsNumber(n[(int)(i)])){
					/* Read digits following decimal point.*/
					i = IsValidJSONNumberAdvancePastDigits(n, i);

					if(i == n.length){
						/* If non-scientific decimal number, we are done.*/
						success = true;
					}
				}else{
					success = false;
					lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".toCharArray()));
				}
			}else{
				success = false;
				lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".toCharArray()));
			}
		}

		if(i < n.length && success){
			if(n[(int)(i)] == 'e' || n[(int)(i)] == 'E'){
				wasDotAndOrE = true;
				success = IsValidJSONNumberFromExponent(n, i, errorMessages);
			}else{
				success = false;
				lAddStringRef(errorMessages, CreateStringReference("Expected e or E.".toCharArray()));
			}
		}else if(i == n.length && success){
			/* If number with decimal point.*/
			success = true;
		}else{
			success = false;
			lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".toCharArray()));
		}

		if(wasDotAndOrE){
		}else{
			success = false;
			lAddStringRef(errorMessages, CreateStringReference("Exprected decimal point or e or E.".toCharArray()));
		}

		return success;
	}

	public static boolean IsValidJSONNumberFromExponent(char [] n, double i, StringArrayReference errorMessages){
		boolean success;

		i = i + 1d;

		if(i < n.length){
			/* The exponent sign can either + or -,*/
			if(n[(int)(i)] == '+' || n[(int)(i)] == '-'){
				i = i + 1d;
			}

			if(i < n.length){
				if(charIsNumber(n[(int)(i)])){
					/* Read digits following decimal point.*/
					i = IsValidJSONNumberAdvancePastDigits(n, i);

					if(i == n.length){
						/* We found scientific number.*/
						success = true;
					}else{
						success = false;
						lAddStringRef(errorMessages, CreateStringReference("There was characters following the exponent.".toCharArray()));
					}
				}else{
					success = false;
					lAddStringRef(errorMessages, CreateStringReference("There must be a digit following the optional exponent sign.".toCharArray()));
				}
			}else{
				success = false;
				lAddStringRef(errorMessages, CreateStringReference("There must be a digit following optional the exponent sign.".toCharArray()));
			}
		}else{
			success = false;
			lAddStringRef(errorMessages, CreateStringReference("There must be a sign or a digit following e or E.".toCharArray()));
		}

		return success;
	}

	public static boolean IsJSONNumberCharacter(char c){
		char [] numericCharacters;
		boolean found;
		double i;

		numericCharacters = "0123456789.-+eE".toCharArray();

		found = false;

		for(i = 0d; i < numericCharacters.length; i = i + 1d){
			if(numericCharacters[(int)(i)] == c){
				found = true;
			}
		}

		return found;
	}

	public static boolean GetJSONPrimitiveName(char [] string, double start, StringArrayReference errorMessages, char [] primitive, StringReference tokenReference){
		char c, p;
		boolean done, success;
		double i;
		char [] str, token;

		done = false;
		success = true;

		token = "".toCharArray();

		for(i = start; i < string.length && ((i - start) < primitive.length) && !done; i = i + 1d){
			c = string[(int)(i)];
			p = primitive[(int)(i - start)];
			if(c == p){
				/* OK*/
				if((i + 1d - start) == primitive.length){
					done = true;
				}
			}else{
				str = "".toCharArray();
				str = strConcatenateString(str, "Primitive invalid: ".toCharArray());
				str = strAppendCharacter(str, c);
				str = strAppendString(str, " vs ".toCharArray());
				str = strAppendCharacter(str, p);

				lAddStringRef(errorMessages, CreateStringReference(str));
				done = true;
				success = false;
			}
		}

		if(done){
			if(StringsEqual(primitive, "false".toCharArray())){
				token = "false".toCharArray();
			}
			if(StringsEqual(primitive, "true".toCharArray())){
				token = "true".toCharArray();
			}
			if(StringsEqual(primitive, "null".toCharArray())){
				token = "null".toCharArray();
			}
		}else{
			lAddStringRef(errorMessages, CreateStringReference("Primitive invalid".toCharArray()));
			success = false;
		}

		tokenReference.string = token;

		return success;
	}

	public static boolean GetJSONString(char [] json, double start, StringReference tokenReference, NumberReference stringLengthReference, StringArrayReference errorMessages){
		boolean success, done;
		char [] string, hex;
		NumberReference characterCount, hexReference;
		double i, l, c;
		StringReference errorMessage;

		characterCount = CreateNumberReference(0d);
		hex = CreateString(4d, '0');
		hexReference = new NumberReference();
		errorMessage = new StringReference();

		success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages);

		if(success){
			l = characterCount.numberValue;
			string = new char [(int)(l)];

			c = 0d;
			string[(int)(c)] = '\"';
			c = c + 1d;

			done = false;
			for(i = start + 1d; !done; i = i + 1d){
				if(json[(int)(i)] == '\\'){
					i = i + 1d;
					if(json[(int)(i)] == '\"' || json[(int)(i)] == '\\' || json[(int)(i)] == '/'){
						string[(int)(c)] = json[(int)(i)];
						c = c + 1d;
					}else if(json[(int)(i)] == 'b'){
						string[(int)(c)] = (char)(8d);
						c = c + 1d;
					}else if(json[(int)(i)] == 'f'){
						string[(int)(c)] = (char)(12d);
						c = c + 1d;
					}else if(json[(int)(i)] == 'n'){
						string[(int)(c)] = (char)(10d);
						c = c + 1d;
					}else if(json[(int)(i)] == 'r'){
						string[(int)(c)] = (char)(13d);
						c = c + 1d;
					}else if(json[(int)(i)] == 't'){
						string[(int)(c)] = (char)(9d);
						c = c + 1d;
					}else if(json[(int)(i)] == 'u'){
						i = i + 1d;
						hex[0] = charToUpperCase(json[(int)(i + 0d)]);
						hex[1] = charToUpperCase(json[(int)(i + 1d)]);
						hex[2] = charToUpperCase(json[(int)(i + 2d)]);
						hex[3] = charToUpperCase(json[(int)(i + 3d)]);
						nCreateNumberFromStringWithCheck(hex, 16d, hexReference, errorMessage);
						string[(int)(c)] = (char)(hexReference.numberValue);
						i = i + 3d;
						c = c + 1d;
					}
				}else if(json[(int)(i)] == '\"'){
					string[(int)(c)] = json[(int)(i)];
					c = c + 1d;
					done = true;
				}else{
					string[(int)(c)] = json[(int)(i)];
					c = c + 1d;
				}
			}

			tokenReference.string = string;
			success = true;
		}else{
			lAddStringRef(errorMessages, CreateStringReference("End of string was not found.".toCharArray()));
			success = false;
		}

		return success;
	}

	public static boolean IsValidJSONString(char [] jsonString, StringArrayReference errorMessages){
		boolean valid;
		NumberReference numberReference, stringLength;

		numberReference = new NumberReference();
		stringLength = new NumberReference();

		valid = IsValidJSONStringInJSON(jsonString, 0d, numberReference, stringLength, errorMessages);

		return valid;
	}

	public static boolean IsValidJSONStringInJSON(char [] json, double start, NumberReference characterCount, NumberReference stringLengthReference, StringArrayReference errorMessages){
		boolean success, done;
		double i, j;
		char c;

		success = true;
		done = false;

		characterCount.numberValue = 1d;

		for(i = start + 1d; i < json.length && !done && success; i = i + 1d){
			if(!IsJSONIllegalControllCharacter(json[(int)(i)])){
				if(json[(int)(i)] == '\\'){
					i = i + 1d;
					if(i < json.length){
						if(json[(int)(i)] == '\"' || json[(int)(i)] == '\\' || json[(int)(i)] == '/' || json[(int)(i)] == 'b' || json[(int)(i)] == 'f' || json[(int)(i)] == 'n' || json[(int)(i)] == 'r' || json[(int)(i)] == 't'){
							characterCount.numberValue = characterCount.numberValue + 1d;
						}else if(json[(int)(i)] == 'u'){
							if(i + 4d < json.length){
								for(j = 0d; j < 4d && success; j = j + 1d){
									c = json[(int)(i + j + 1d)];
									if(nCharacterIsNumberCharacterInBase(c, 16d) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f'){
									}else{
										success = false;
										lAddStringRef(errorMessages, CreateStringReference("\\u must be followed by four hexadecimal digits.".toCharArray()));
									}
								}
								characterCount.numberValue = characterCount.numberValue + 1d;
								i = i + 4d;
							}else{
								success = false;
								lAddStringRef(errorMessages, CreateStringReference("\\u must be followed by four characters.".toCharArray()));
							}
						}else{
							success = false;
							lAddStringRef(errorMessages, CreateStringReference("Escaped character invalid.".toCharArray()));
						}
					}else{
						success = false;
						lAddStringRef(errorMessages, CreateStringReference("There must be at least two character after string escape.".toCharArray()));
					}
				}else if(json[(int)(i)] == '\"'){
					characterCount.numberValue = characterCount.numberValue + 1d;
					done = true;
				}else{
					characterCount.numberValue = characterCount.numberValue + 1d;
				}
			}else{
				success = false;
				lAddStringRef(errorMessages, CreateStringReference("Unicode code points 0-31 not allowed in JSON string.".toCharArray()));
			}
		}

		if(done){
			stringLengthReference.numberValue = i - start;
		}else{
			success = false;
			lAddStringRef(errorMessages, CreateStringReference("String must end with \".".toCharArray()));
		}

		return success;
	}

	public static boolean IsJSONIllegalControllCharacter(char c){
		double cnr;
		boolean isControll;

		cnr = c;

		if(cnr >= 0d && cnr < 32d){
			isControll = true;
		}else{
			isControll = false;
		}

		return isControll;
	}

	public static Element [] AddElement(Element [] list, Element a){
		Element [] newlist;
		double i;

		newlist = new Element [(int)(list.length + 1d)];

		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;

		delete(list);

		return newlist;
	}

	public static void AddElementRef(ElementArrayReference list, Element i){
		list.array = AddElement(list.array, i);
	}

	public static Element [] RemoveElement(Element [] list, double n){
		Element [] newlist;
		double i;

		newlist = new Element [(int)(list.length - 1d)];

		for(i = 0d; i < list.length; i = i + 1d){
			if(i < n){
				newlist[(int)(i)] = list[(int)(i)];
			}
			if(i > n){
				newlist[(int)(i - 1d)] = list[(int)(i)];
			}
		}

		delete(list);

		return newlist;
	}

	public static Element GetElementRef(ElementArrayReference list, double i){
		return list.array[(int)(i)];
	}

	public static void RemoveElementRef(ElementArrayReference list, double i){
		list.array = RemoveElement(list.array, i);
	}

	public static LinkedListElements CreateLinkedListElements(){
		LinkedListElements ll;

		ll = new LinkedListElements();
		ll.first = new LinkedListNodeElements();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void LinkedListAddElement(LinkedListElements ll, Element value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeElements();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static Element [] LinkedListElementsToArray(LinkedListElements ll){
		Element [] array;
		double length, i;
		LinkedListNodeElements node;

		node = ll.first;

		length = LinkedListElementsLength(ll);

		array = new Element [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static double LinkedListElementsLength(LinkedListElements ll){
		double l;
		LinkedListNodeElements node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void FreeLinkedListElements(LinkedListElements ll){
		LinkedListNodeElements node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static double ComputeJSONStringLength(Element element){
		double result;

		result = 0d;

		if(StringsEqual(element.type, "object".toCharArray())){
			result = result + ComputeJSONObjectStringLength(element);
		}else if(StringsEqual(element.type, "string".toCharArray())){
			result = JSONEscapedStringLength(element.string) + 2d;
		}else if(StringsEqual(element.type, "array".toCharArray())){
			result = result + ComputeJSONArrayStringLength(element);
		}else if(StringsEqual(element.type, "number".toCharArray())){
			result = result + ComputeJSONNumberStringLength(element);
		}else if(StringsEqual(element.type, "null".toCharArray())){
			result = result + "null".toCharArray().length;
		}else if(StringsEqual(element.type, "boolean".toCharArray())){
			result = result + ComputeJSONBooleanStringLength(element);
		}

		return result;
	}

	public static double ComputeJSONBooleanStringLength(Element element){
		double result;

		if(element.booleanValue){
			result = "true".toCharArray().length;
		}else{
			result = "false".toCharArray().length;
		}

		return result;
	}

	public static double ComputeJSONNumberStringLength(Element element){
		double length;
		char [] a;

		if(abs(element.number) >= pow(10d, 15d) || abs(element.number) <= pow(10d, -15d)){
			a = nCreateStringScientificNotationDecimalFromNumber(element.number);
			length = a.length;
		}else{
			a = nCreateStringDecimalFromNumber(element.number);
			length = a.length;
		}

		return length;
	}

	public static double ComputeJSONArrayStringLength(Element element){
		Element arrayElement;
		double i;
		double length;

		length = 1d;

		for(i = 0d; i < element.array.length; i = i + 1d){
			arrayElement = element.array[(int)(i)];

			length = length + ComputeJSONStringLength(arrayElement);

			if(i + 1d != element.array.length){
				length = length + 1d;
			}
		}

		length = length + 1d;

		return length;
	}

	public static double ComputeJSONObjectStringLength(Element element){
		char [] key;
		double i;
		StringArrayReference keys;
		Element objectElement;
		double length;

		length = 1d;

		keys = GetStringElementMapKeySet(element.object);
		for(i = 0d; i < keys.stringArray.length; i = i + 1d){
			key = keys.stringArray[(int)(i)].string;
			objectElement = GetObjectValue(element.object, key);

			length = length + 1d;
			length = length + JSONEscapedStringLength(key);
			length = length + 1d;
			length = length + 1d;

			length = length + ComputeJSONStringLength(objectElement);

			if(i + 1d != keys.stringArray.length){
				length = length + 1d;
			}
		}

		length = length + 1d;

		return length;
	}

	public static Element CreateStringElement(char [] string){
		Element element;
		element = new Element();
		element.type = "string".toCharArray();
		element.string = string;
		return element;
	}

	public static Element CreateBooleanElement(boolean booleanValue){
		Element element;
		element = new Element();
		element.type = "boolean".toCharArray();
		element.booleanValue = booleanValue;
		return element;
	}

	public static Element CreateNullElement(){
		Element element;
		element = new Element();
		element.type = "null".toCharArray();
		return element;
	}

	public static Element CreateNumberElement(double number){
		Element element;
		element = new Element();
		element.type = "number".toCharArray();
		element.number = number;
		return element;
	}

	public static Element CreateArrayElement(double length){
		Element element;
		element = new Element();
		element.type = "array".toCharArray();
		element.array = new Element [(int)(length)];
		return element;
	}

	public static Element CreateObjectElement(double length){
		Element element;
		element = new Element();
		element.type = "object".toCharArray();
		element.object = new StringElementMap();
		element.object.stringListRef = CreateStringArrayReferenceLengthValue(length, "".toCharArray());
		element.object.elementListRef = new ElementArrayReference();
		element.object.elementListRef.array = new Element [(int)(length)];
		return element;
	}

	public static void DeleteElement(Element element){
		if(StringsEqual(element.type, "object".toCharArray())){
			DeleteObject(element);
		}else if(StringsEqual(element.type, "string".toCharArray())){
			delete(element);
		}else if(StringsEqual(element.type, "array".toCharArray())){
			DeleteArray(element);
		}else if(StringsEqual(element.type, "number".toCharArray())){
			delete(element);
		}else if(StringsEqual(element.type, "null".toCharArray())){
			delete(element);
		}else if(StringsEqual(element.type, "boolean".toCharArray())){
			delete(element);
		}else{
		}
	}

	public static void DeleteObject(Element element){
		StringArrayReference keys;
		double i;
		char [] key;
		Element objectElement;

		keys = GetStringElementMapKeySet(element.object);
		for(i = 0d; i < keys.stringArray.length; i = i + 1d){
			key = keys.stringArray[(int)(i)].string;
			objectElement = GetObjectValue(element.object, key);
			DeleteElement(objectElement);
		}
	}

	public static void DeleteArray(Element element){
		double i;
		Element arrayElement;

		for(i = 0d; i < element.array.length; i = i + 1d){
			arrayElement = element.array[(int)(i)];
			DeleteElement(arrayElement);
		}
	}

	public static char [] WriteJSON(Element element){
		char [] result;
		double length;
		NumberReference index;

		length = ComputeJSONStringLength(element);
		result = new char [(int)(length)];
		index = CreateNumberReference(0d);

		if(StringsEqual(element.type, "object".toCharArray())){
			WriteObject(element, result, index);
		}else if(StringsEqual(element.type, "string".toCharArray())){
			WriteString(element, result, index);
		}else if(StringsEqual(element.type, "array".toCharArray())){
			WriteArray(element, result, index);
		}else if(StringsEqual(element.type, "number".toCharArray())){
			WriteNumber(element, result, index);
		}else if(StringsEqual(element.type, "null".toCharArray())){
			strWriteStringToStingStream(result, index, "null".toCharArray());
		}else if(StringsEqual(element.type, "boolean".toCharArray())){
			WriteBooleanValue(element, result, index);
		}

		return result;
	}

	public static void WriteBooleanValue(Element element, char [] result, NumberReference index){
		if(element.booleanValue){
			strWriteStringToStingStream(result, index, "true".toCharArray());
		}else{
			strWriteStringToStingStream(result, index, "false".toCharArray());
		}
	}

	public static void WriteNumber(Element element, char [] result, NumberReference index){
		char [] numberString;

		if(abs(element.number) >= pow(10d, 15d) || abs(element.number) <= pow(10d, -15d)){
			numberString = nCreateStringScientificNotationDecimalFromNumber(element.number);
		}else{
			numberString = nCreateStringDecimalFromNumber(element.number);
		}

		strWriteStringToStingStream(result, index, numberString);
	}

	public static void WriteArray(Element element, char [] result, NumberReference index){
		char [] s;
		Element arrayElement;
		double i;

		strWriteStringToStingStream(result, index, "[".toCharArray());

		for(i = 0d; i < element.array.length; i = i + 1d){
			arrayElement = element.array[(int)(i)];

			s = WriteJSON(arrayElement);
			strWriteStringToStingStream(result, index, s);

			if(i + 1d != element.array.length){
				strWriteStringToStingStream(result, index, ",".toCharArray());
			}
		}

		strWriteStringToStingStream(result, index, "]".toCharArray());
	}

	public static void WriteString(Element element, char [] result, NumberReference index){
		strWriteStringToStingStream(result, index, "\"".toCharArray());
		element.string = JSONEscapeString(element.string);
		strWriteStringToStingStream(result, index, element.string);
		strWriteStringToStingStream(result, index, "\"".toCharArray());
	}

	public static char [] JSONEscapeString(char [] string){
		double i, length;
		NumberReference index, lettersReference;
		char [] ns, escaped;

		length = JSONEscapedStringLength(string);

		ns = new char [(int)(length)];
		index = CreateNumberReference(0d);
		lettersReference = CreateNumberReference(0d);

		for(i = 0d; i < string.length; i = i + 1d){
			if(JSONMustBeEscaped(string[(int)(i)], lettersReference)){
				escaped = JSONEscapeCharacter(string[(int)(i)]);
				strWriteStringToStingStream(ns, index, escaped);
			}else{
				strWriteCharacterToStingStream(ns, index, string[(int)(i)]);
			}
		}

		return ns;
	}

	public static double JSONEscapedStringLength(char [] string){
		NumberReference lettersReference;
		double i, length;

		lettersReference = CreateNumberReference(0d);
		length = 0d;

		for(i = 0d; i < string.length; i = i + 1d){
			if(JSONMustBeEscaped(string[(int)(i)], lettersReference)){
				length = length + lettersReference.numberValue;
			}else{
				length = length + 1d;
			}
		}
		return length;
	}

	public static char [] JSONEscapeCharacter(char c){
		double code;
		char [] escaped;
		StringReference hexNumber;
		double q, rs, s, b, f, n, r, t;

		code = c;

		q = 34d;
		rs = 92d;
		s = 47d;
		b = 8d;
		f = 12d;
		n = 10d;
		r = 13d;
		t = 9d;

		hexNumber = new StringReference();

		if(code == q){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = '\"';
		}else if(code == rs){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = '\\';
		}else if(code == s){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = '/';
		}else if(code == b){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = 'b';
		}else if(code == f){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = 'f';
		}else if(code == n){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = 'n';
		}else if(code == r){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = 'r';
		}else if(code == t){
			escaped = new char [2];
			escaped[0] = '\\';
			escaped[1] = 't';
		}else if(code >= 0d && code <= 31d){
			escaped = new char [6];
			escaped[0] = '\\';
			escaped[1] = 'u';
			escaped[2] = '0';
			escaped[3] = '0';

			nCreateStringFromNumberWithCheck(code, 16d, hexNumber);

			if(hexNumber.string.length == 1d){
				escaped[4] = '0';
				escaped[5] = hexNumber.string[0];
			}else if(hexNumber.string.length == 2d){
				escaped[4] = hexNumber.string[0];
				escaped[5] = hexNumber.string[1];
			}
		}else{
			escaped = new char [1];
			escaped[0] = c;
		}

		return escaped;
	}

	public static boolean JSONMustBeEscaped(char c, NumberReference letters){
		double code;
		boolean mustBeEscaped;
		double q, rs, s, b, f, n, r, t;

		code = c;
		mustBeEscaped = false;

		q = 34d;
		rs = 92d;
		s = 47d;
		b = 8d;
		f = 12d;
		n = 10d;
		r = 13d;
		t = 9d;

		if(code == q || code == rs || code == s || code == b || code == f || code == n || code == r || code == t){
			mustBeEscaped = true;
			letters.numberValue = 2d;
		}else if(code >= 0d && code <= 31d){
			mustBeEscaped = true;
			letters.numberValue = 6d;
		}else if(code >= pow(2d, 16d)){
			mustBeEscaped = true;
			letters.numberValue = 6d;
		}

		return mustBeEscaped;
	}

	public static void WriteObject(Element element, char [] result, NumberReference index){
		char [] s, key;
		double i;
		StringArrayReference keys;
		Element objectElement;

		strWriteStringToStingStream(result, index, "{".toCharArray());

		keys = GetStringElementMapKeySet(element.object);
		for(i = 0d; i < keys.stringArray.length; i = i + 1d){
			key = keys.stringArray[(int)(i)].string;
			key = JSONEscapeString(key);
			objectElement = GetObjectValue(element.object, key);

			strWriteStringToStingStream(result, index, "\"".toCharArray());
			strWriteStringToStingStream(result, index, key);
			strWriteStringToStingStream(result, index, "\"".toCharArray());
			strWriteStringToStingStream(result, index, ":".toCharArray());

			s = WriteJSON(objectElement);
			strWriteStringToStingStream(result, index, s);

			if(i + 1d != keys.stringArray.length){
				strWriteStringToStingStream(result, index, ",".toCharArray());
			}
		}

		strWriteStringToStingStream(result, index, "}".toCharArray());
	}

	public static boolean ReadJSON(char [] string, ElementReference elementReference, StringArrayReference errorMessages){
		StringArrayReference tokenArrayReference;
		boolean success;

		/* Tokenize.*/
		tokenArrayReference = new StringArrayReference();
		success = JSONTokenize(string, tokenArrayReference, errorMessages);

		if(success){
			/* Parse.*/
			success = GetJSONValue(tokenArrayReference.stringArray, elementReference, errorMessages);
		}

		return success;
	}

	public static boolean GetJSONValue(StringReference [] tokens, ElementReference elementReference, StringArrayReference errorMessages){
		NumberReference i;
		boolean success;

		i = CreateNumberReference(0d);
		success = GetJSONValueRecursive(tokens, i, 0d, elementReference, errorMessages);

		return success;
	}

	public static boolean GetJSONValueRecursive(StringReference [] tokens, NumberReference i, double depth, ElementReference elementReference, StringArrayReference errorMessages){
		char [] str, substr, token;
		double stringToDecimalResult;
		boolean success;

		success = true;
		token = tokens[(int)(i.numberValue)].string;

		if(StringsEqual(token, "{".toCharArray())){
			success = GetJSONObject(tokens, i, depth + 1d, elementReference, errorMessages);
		}else if(StringsEqual(token, "[".toCharArray())){
			success = GetJSONArray(tokens, i, depth + 1d, elementReference, errorMessages);
		}else if(StringsEqual(token, "true".toCharArray())){
			elementReference.element = CreateBooleanElement(true);
			i.numberValue = i.numberValue + 1d;
		}else if(StringsEqual(token, "false".toCharArray())){
			elementReference.element = CreateBooleanElement(false);
			i.numberValue = i.numberValue + 1d;
		}else if(StringsEqual(token, "null".toCharArray())){
			elementReference.element = CreateNullElement();
			i.numberValue = i.numberValue + 1d;
		}else if(charIsNumber(token[0]) || token[0] == '-'){
			stringToDecimalResult = nCreateNumberFromDecimalString(token);
			elementReference.element = CreateNumberElement(stringToDecimalResult);
			i.numberValue = i.numberValue + 1d;
		}else if(token[0] == '\"'){
			substr = strSubstring(token, 1d, token.length - 1d);
			elementReference.element = CreateStringElement(substr);
			i.numberValue = i.numberValue + 1d;
		}else{
			str = "".toCharArray();
			str = strConcatenateString(str, "Invalid token first in value: ".toCharArray());
			str = strAppendString(str, token);
			lAddStringRef(errorMessages, CreateStringReference(str));
			success = false;
		}

		if(success && depth == 0d){
			if(StringsEqual(tokens[(int)(i.numberValue)].string, "<end>".toCharArray())){
			}else{
				lAddStringRef(errorMessages, CreateStringReference("The outer value cannot have any tokens following it.".toCharArray()));
				success = false;
			}
		}

		return success;
	}

	public static boolean GetJSONObject(StringReference [] tokens, NumberReference i, double depth, ElementReference elementReference, StringArrayReference errorMessages){
		Element element, value;
		boolean done, success;
		char [] key, colon, comma, closeCurly;
		char [] keystring, str;
		ElementReference valueReference;
		LinkedListElements values;
		lLinkedListStrings keys;

		keys = lCreateLinkedListString();
		values = CreateLinkedListElements();
		element = CreateObjectElement(0d);
		valueReference = new ElementReference();
		success = true;
		i.numberValue = i.numberValue + 1d;

		if(!StringsEqual(tokens[(int)(i.numberValue)].string, "}".toCharArray())){
			done = false;

			for(; !done && success; ){
				key = tokens[(int)(i.numberValue)].string;

				if(key[0] == '\"'){
					i.numberValue = i.numberValue + 1d;
					colon = tokens[(int)(i.numberValue)].string;
					if(StringsEqual(colon, ":".toCharArray())){
						i.numberValue = i.numberValue + 1d;
						success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

						if(success){
							keystring = strSubstring(key, 1d, key.length - 1d);
							value = valueReference.element;
							lLinkedListAddString(keys, keystring);
							LinkedListAddElement(values, value);

							comma = tokens[(int)(i.numberValue)].string;
							if(StringsEqual(comma, ",".toCharArray())){
								/* OK*/
								i.numberValue = i.numberValue + 1d;
							}else{
								done = true;
							}
						}
					}else{
						str = "".toCharArray();
						str = strConcatenateString(str, "Expected colon after key in object: ".toCharArray());
						str = strAppendString(str, colon);
						lAddStringRef(errorMessages, CreateStringReference(str));

						success = false;
						done = true;
					}
				}else{
					lAddStringRef(errorMessages, CreateStringReference("Expected string as key in object.".toCharArray()));

					done = true;
					success = false;
				}
			}
		}

		if(success){
			closeCurly = tokens[(int)(i.numberValue)].string;

			if(StringsEqual(closeCurly, "}".toCharArray())){
				/* OK*/
				delete(element.object.stringListRef.stringArray);
				delete(element.object.elementListRef.array);
				element.object.stringListRef.stringArray = lLinkedListStringsToArray(keys);
				element.object.elementListRef.array = LinkedListElementsToArray(values);
				elementReference.element = element;
				i.numberValue = i.numberValue + 1d;
			}else{
				lAddStringRef(errorMessages, CreateStringReference("Expected close curly brackets at end of object value.".toCharArray()));
				success = false;
			}
		}

		lFreeLinkedListString(keys);
		FreeLinkedListElements(values);
		delete(valueReference);

		return success;
	}

	public static boolean GetJSONArray(StringReference [] tokens, NumberReference i, double depth, ElementReference elementReference, StringArrayReference errorMessages){
		Element element, value;
		char [] nextToken, comma;
		boolean done, success;
		ElementReference valueReference;
		LinkedListElements elements;

		elements = CreateLinkedListElements();
		i.numberValue = i.numberValue + 1d;

		valueReference = new ElementReference();
		success = true;
		element = CreateArrayElement(0d);

		nextToken = tokens[(int)(i.numberValue)].string;

		if(!StringsEqual(nextToken, "]".toCharArray())){
			done = false;
			for(; !done && success; ){
				success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

				if(success){
					value = valueReference.element;
					LinkedListAddElement(elements, value);

					comma = tokens[(int)(i.numberValue)].string;

					if(StringsEqual(comma, ",".toCharArray())){
						/* OK*/
						i.numberValue = i.numberValue + 1d;
					}else{
						done = true;
					}
				}
			}
		}

		nextToken = tokens[(int)(i.numberValue)].string;
		if(StringsEqual(nextToken, "]".toCharArray())){
			/* OK*/
			i.numberValue = i.numberValue + 1d;
			delete(element.array);
			element.array = LinkedListElementsToArray(elements);
		}else{
			lAddStringRef(errorMessages, CreateStringReference("Expected close square bracket at end of array.".toCharArray()));
			success = false;
		}

		elementReference.element = element;
		FreeLinkedListElements(elements);
		delete(valueReference);

		return success;
	}

	public static StringArrayReference GetStringElementMapKeySet(StringElementMap stringElementMap){
		return stringElementMap.stringListRef;
	}

	public static Element GetObjectValue(StringElementMap stringElementMap, char [] key){
		Element result;
		double i;

		result = new Element();

		for(i = 0d; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1d){
			if(StringsEqual(stringElementMap.stringListRef.stringArray[(int)(i)].string, key)){
				result = stringElementMap.elementListRef.array[(int)(i)];
			}
		}

		return result;
	}

	public static Element GetObjectValueWithCheck(StringElementMap stringElementMap, char [] key, BooleanReference foundReference){
		Element result;
		double i;

		result = new Element();

		foundReference.booleanValue = false;
		for(i = 0d; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1d){
			if(StringsEqual(stringElementMap.stringListRef.stringArray[(int)(i)].string, key)){
				result = stringElementMap.elementListRef.array[(int)(i)];
				foundReference.booleanValue = true;
			}
		}

		return result;
	}

	public static void PutStringElementMap(StringElementMap stringElementMap, char [] keystring, Element value){
		lAddStringRef(stringElementMap.stringListRef, CreateStringReference(keystring));
		AddElementRef(stringElementMap.elementListRef, value);
	}

	public static void SetStringElementMap(StringElementMap stringElementMap, double index, char [] keystring, Element value){
		stringElementMap.stringListRef.stringArray[(int)(index)].string = keystring;
		stringElementMap.elementListRef.array[(int)(index)] = value;
	}

	public static double GetStringElementMapNumberOfKeys(StringElementMap stringElementMap){
		return stringElementMap.stringListRef.stringArray.length;
	}

	public static boolean JSONCompare(char [] a, char [] b, double epsilon, BooleanReference equal, StringArrayReference errorMessage){
		boolean success;
		ElementReference eaRef, ebRef;
		Element ea, eb;

		eaRef = new ElementReference();
		ebRef = new ElementReference();

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

	public static boolean JSONCompareElements(Element ea, Element eb, double epsilon){
		boolean equal;
		char [] typeName;

		equal = StringsEqual(ea.type, eb.type);
        
		if(equal){
			typeName = ea.type;
			if(StringsEqual(typeName, "object".toCharArray())){
				equal = JSONCompareObjects(ea.object, eb.object, epsilon);
			}else if(StringsEqual(typeName, "string".toCharArray())){
				equal = StringsEqual(ea.string, eb.string);
			}else if(StringsEqual(typeName, "array".toCharArray())){
				equal = JSONCompareArrays(ea.array, eb.array, epsilon);
			}else if(StringsEqual(typeName, "number".toCharArray())){
				equal = EpsilonCompare(ea.number, eb.number, epsilon);
			}else if(StringsEqual(typeName, "null".toCharArray())){
				equal = true;
			}else if(StringsEqual(typeName, "boolean".toCharArray())){
				equal = ea.booleanValue == eb.booleanValue;
			}
		}
        
		return equal;
	}

	public static boolean JSONCompareArrays(Element [] ea, Element [] eb, double epsilon){
		boolean equals;
		double i, length;

		equals = ea.length == eb.length;

		if(equals){
			length = ea.length;
			for(i = 0d; i < length && equals; i = i + 1d){
				equals = JSONCompareElements(ea[(int)(i)], eb[(int)(i)], epsilon);
			}
		}

		return equals;
	}

	public static boolean JSONCompareObjects(StringElementMap ea, StringElementMap eb, double epsilon){
		boolean equals;
		double akeys, bkeys, i;
		StringArrayReference keys;
		char [] key;
		BooleanReference aFoundReference, bFoundReference;
		Element eaValue, ebValue;

		aFoundReference = new BooleanReference();
		bFoundReference = new BooleanReference();

		akeys = GetStringElementMapNumberOfKeys(ea);
		bkeys = GetStringElementMapNumberOfKeys(eb);

		equals = akeys == bkeys;

		if(equals){
			keys = GetStringElementMapKeySet(ea);

			for(i = 0d; i < keys.stringArray.length && equals; i = i + 1d){
				key = keys.stringArray[(int)(i)].string;

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

static public class DynamicArrayCharacters{
	public char [] array;
	public double length;
}
static public class LinkedListNodeStrings{
	public boolean end;
	public char [] value;
	public LinkedListNodeStrings next;
}
static public class LinkedListStrings{
	public LinkedListNodeStrings first;
	public LinkedListNodeStrings last;
}
static public class LinkedListNodeNumbers{
	public LinkedListNodeNumbers next;
	public boolean end;
	public double value;
}
static public class LinkedListNumbers{
	public LinkedListNodeNumbers first;
	public LinkedListNodeNumbers last;
}
static public class LinkedListCharacters{
	public LinkedListNodeCharacters first;
	public LinkedListNodeCharacters last;
}
static public class LinkedListNodeCharacters{
	public boolean end;
	public char value;
	public LinkedListNodeCharacters next;
}
static public class DynamicArrayNumbers{
	public double [] array;
	public double length;
}
	public static double [] AddNumber(double [] list, double a){
		double [] newlist;
		double i;

		newlist = new double [(int)(list.length + 1d)];
		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void AddNumberRef(NumberArrayReference list, double i){
		list.numberArray = AddNumber(list.numberArray, i);
	}

	public static double [] RemoveNumber(double [] list, double n){
		double [] newlist;
		double i;

		newlist = new double [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}
		
		return newlist;
	}

	public static double GetNumberRef(NumberArrayReference list, double i){
		return list.numberArray[(int)(i)];
	}

	public static void RemoveNumberRef(NumberArrayReference list, double i){
		list.numberArray = RemoveNumber(list.numberArray, i);
	}

	public static StringReference [] AddString(StringReference [] list, StringReference a){
		StringReference [] newlist;
		double i;

		newlist = new StringReference [(int)(list.length + 1d)];

		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void AddStringRef(StringArrayReference list, StringReference i){
		list.stringArray = AddString(list.stringArray, i);
	}

	public static StringReference [] RemoveString(StringReference [] list, double n){
		StringReference [] newlist;
		double i;

		newlist = new StringReference [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}
		
		return newlist;
	}

	public static StringReference GetStringRef(StringArrayReference list, double i){
		return list.stringArray[(int)(i)];
	}

	public static void RemoveStringRef(StringArrayReference list, double i){
		list.stringArray = RemoveString(list.stringArray, i);
	}

	public static DynamicArrayCharacters CreateDynamicArrayCharacters(){
		DynamicArrayCharacters da;

		da = new DynamicArrayCharacters();
		da.array = new char [10];
		da.length = 0d;

		return da;
	}

	public static DynamicArrayCharacters CreateDynamicArrayCharactersWithInitialCapacity(double capacity){
		DynamicArrayCharacters da;

		da = new DynamicArrayCharacters();
		da.array = new char [(int)(capacity)];
		da.length = 0d;

		return da;
	}

	public static void DynamicArrayAddCharacter(DynamicArrayCharacters da, char value){
		if(da.length == da.array.length){
			DynamicArrayCharactersIncreaseSize(da);
		}

		da.array[(int)(da.length)] = value;
		da.length = da.length + 1d;
	}

	public static void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters da){
		double newLength, i;
		char [] newArray;

		newLength = (double)round(da.array.length*3d/2d);
		newArray = new char [(int)(newLength)];

		for(i = 0d; i < da.array.length; i = i + 1d){
			newArray[(int)(i)] = da.array[(int)(i)];
		}

		delete(da.array);

		da.array = newArray;
	}

	public static boolean DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters da){
		boolean needsDecrease;

		needsDecrease = false;

		if(da.length > 10d){
			needsDecrease = da.length <= (double)round(da.array.length*2d/3d);
		}

		return needsDecrease;
	}

	public static void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters da){
		double newLength, i;
		char [] newArray;

		newLength = (double)round(da.array.length*2d/3d);
		newArray = new char [(int)(newLength)];

		for(i = 0d; i < newLength; i = i + 1d){
			newArray[(int)(i)] = da.array[(int)(i)];
		}

		delete(da.array);

		da.array = newArray;
	}

	public static double DynamicArrayCharactersIndex(DynamicArrayCharacters da, double index){
		return da.array[(int)(index)];
	}

	public static double DynamicArrayCharactersLength(DynamicArrayCharacters da){
		return da.length;
	}

	public static void DynamicArrayInsertCharacter(DynamicArrayCharacters da, double index, char value){
		double i;

		if(da.length == da.array.length){
			DynamicArrayCharactersIncreaseSize(da);
		}

		for(i = da.length; i > index; i = i - 1d){
			da.array[(int)(i)] = da.array[(int)(i - 1d)];
		}

		da.array[(int)(index)] = value;

		da.length = da.length + 1d;
	}

	public static boolean DynamicArrayCharacterSet(DynamicArrayCharacters da, double index, char value){
		boolean success;

		if(index < da.length){
			da.array[(int)(index)] = value;
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static void DynamicArrayRemoveCharacter(DynamicArrayCharacters da, double index){
		double i;

		for(i = index; i < da.length - 1d; i = i + 1d){
			da.array[(int)(i)] = da.array[(int)(i + 1d)];
		}

		da.length = da.length - 1d;

		if(DynamicArrayCharactersDecreaseSizeNecessary(da)){
			DynamicArrayCharactersDecreaseSize(da);
		}
	}

	public static void FreeDynamicArrayCharacters(DynamicArrayCharacters da){
		delete(da.array);
		delete(da);
	}

	public static char [] DynamicArrayCharactersToArray(DynamicArrayCharacters da){
		char [] array;
		double i;

		array = new char [(int)(da.length)];

		for(i = 0d; i < da.length; i = i + 1d){
			array[(int)(i)] = da.array[(int)(i)];
		}

		return array;
	}

	public static DynamicArrayCharacters ArrayToDynamicArrayCharactersWithOptimalSize(char [] array){
		DynamicArrayCharacters da;
		double i;
		double c, n, newCapacity;

		c = array.length;
		n = (log(c) - 1d)/log(3d/2d);
		newCapacity = floor(n) + 1d;

		da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity);

		for(i = 0d; i < array.length; i = i + 1d){
			da.array[(int)(i)] = array[(int)(i)];
		}

		return da;
	}

	public static DynamicArrayCharacters ArrayToDynamicArrayCharacters(char [] array){
		DynamicArrayCharacters da;

		da = new DynamicArrayCharacters();
		da.array = CopyString(array);
		da.length = array.length;

		return da;
	}

	public static boolean DynamicArrayCharactersEqual(DynamicArrayCharacters a, DynamicArrayCharacters b){
		boolean equal;
		double i;

		equal = true;
		if(a.length == b.length){
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(a.array[(int)(i)] != b.array[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static LinkedListCharacters DynamicArrayCharactersToLinkedList(DynamicArrayCharacters da){
		LinkedListCharacters ll;
		double i;

		ll = CreateLinkedListCharacter();

		for(i = 0d; i < da.length; i = i + 1d){
			LinkedListAddCharacter(ll, da.array[(int)(i)]);
		}

		return ll;
	}

	public static DynamicArrayCharacters LinkedListToDynamicArrayCharacters(LinkedListCharacters ll){
		DynamicArrayCharacters da;
		double i;
		LinkedListNodeCharacters node;

		node = ll.first;

		da = new DynamicArrayCharacters();
		da.length = LinkedListCharactersLength(ll);

		da.array = new char [(int)(da.length)];

		for(i = 0d; i < da.length; i = i + 1d){
			da.array[(int)(i)] = node.value;
			node = node.next;
		}

		return da;
	}

	public static boolean [] AddBoolean(boolean [] list, boolean a){
		boolean [] newlist;
		double i;

		newlist = new boolean [(int)(list.length + 1d)];
		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void AddBooleanRef(BooleanArrayReference list, boolean i){
		list.booleanArray = AddBoolean(list.booleanArray, i);
	}

	public static boolean [] RemoveBoolean(boolean [] list, double n){
		boolean [] newlist;
		double i;

		newlist = new boolean [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}
		
		return newlist;
	}

	public static boolean GetBooleanRef(BooleanArrayReference list, double i){
		return list.booleanArray[(int)(i)];
	}

	public static void RemoveDecimalRef(BooleanArrayReference list, double i){
		list.booleanArray = RemoveBoolean(list.booleanArray, i);
	}

	public static LinkedListStrings CreateLinkedListString(){
		LinkedListStrings ll;

		ll = new LinkedListStrings();
		ll.first = new LinkedListNodeStrings();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void LinkedListAddString(LinkedListStrings ll, char [] value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeStrings();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static StringReference [] LinkedListStringsToArray(LinkedListStrings ll){
		StringReference [] array;
		double length, i;
		LinkedListNodeStrings node;

		node = ll.first;

		length = LinkedListStringsLength(ll);

		array = new StringReference [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = new StringReference();
			array[(int)(i)].string = node.value;
			node = node.next;
		}

		return array;
	}

	public static double LinkedListStringsLength(LinkedListStrings ll){
		double l;
		LinkedListNodeStrings node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void FreeLinkedListString(LinkedListStrings ll){
		LinkedListNodeStrings node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static LinkedListNumbers CreateLinkedListNumbers(){
		LinkedListNumbers ll;

		ll = new LinkedListNumbers();
		ll.first = new LinkedListNodeNumbers();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static LinkedListNumbers [] CreateLinkedListNumbersArray(double length){
		LinkedListNumbers [] lls;
		double i;

		lls = new LinkedListNumbers [(int)(length)];
		for(i = 0d; i < lls.length; i = i + 1d){
			lls[(int)(i)] = CreateLinkedListNumbers();
		}

		return lls;
	}

	public static void LinkedListAddNumber(LinkedListNumbers ll, double value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeNumbers();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static double LinkedListNumbersLength(LinkedListNumbers ll){
		double l;
		LinkedListNodeNumbers node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static double LinkedListNumbersIndex(LinkedListNumbers ll, double index){
		double i;
		LinkedListNodeNumbers node;

		node = ll.first;
		for(i = 0d; i < index; i = i + 1d){
			node = node.next;
		}

		return node.value;
	}

	public static void LinkedListInsertNumber(LinkedListNumbers ll, double index, double value){
		double i;
		LinkedListNodeNumbers node, tmp;

		if(index == 0d){
			tmp = ll.first;
			ll.first = new LinkedListNodeNumbers();
			ll.first.next = tmp;
			ll.first.value = value;
			ll.first.end = false;
		}else{
			node = ll.first;
			for(i = 0d; i < index - 1d; i = i + 1d){
				node = node.next;
			}

			tmp = node.next;
			node.next = new LinkedListNodeNumbers();
			node.next.next = tmp;
			node.next.value = value;
			node.next.end = false;
		}
	}

	public static void LinkedListSet(LinkedListNumbers ll, double index, double value){
		double i;
		LinkedListNodeNumbers node;

		node = ll.first;
		for(i = 0d; i < index; i = i + 1d){
			node = node.next;
		}

		node.next.value = value;
	}

	public static void LinkedListRemoveNumber(LinkedListNumbers ll, double index){
		double i;
		LinkedListNodeNumbers node, prev;

		node = ll.first;
		prev = ll.first;

		for(i = 0d; i < index; i = i + 1d){
			prev = node;
			node = node.next;
		}

		if(index == 0d){
			ll.first = prev.next;
		}
		if(!prev.next.end){
			prev.next = prev.next.next;
		}
	}

	public static void FreeLinkedListNumbers(LinkedListNumbers ll){
		LinkedListNodeNumbers node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static void FreeLinkedListNumbersArray(LinkedListNumbers [] lls){
		double i;

		for(i = 0d; i < lls.length; i = i + 1d){
			FreeLinkedListNumbers(lls[(int)(i)]);
		}
		delete(lls);
	}

	public static double [] LinkedListNumbersToArray(LinkedListNumbers ll){
		double [] array;
		double length, i;
		LinkedListNodeNumbers node;

		node = ll.first;

		length = LinkedListNumbersLength(ll);

		array = new double [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static LinkedListNumbers ArrayToLinkedListNumbers(double [] array){
		LinkedListNumbers ll;
		double i;

		ll = CreateLinkedListNumbers();

		for(i = 0d; i < array.length; i = i + 1d){
			LinkedListAddNumber(ll, array[(int)(i)]);
		}

		return ll;
	}

	public static boolean LinkedListNumbersEqual(LinkedListNumbers a, LinkedListNumbers b){
		boolean equal, done;
		LinkedListNodeNumbers an, bn;

		an = a.first;
		bn = b.first;

		equal = true;
		done = false;
		for(; equal && !done; ){
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

	public static LinkedListCharacters CreateLinkedListCharacter(){
		LinkedListCharacters ll;

		ll = new LinkedListCharacters();
		ll.first = new LinkedListNodeCharacters();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void LinkedListAddCharacter(LinkedListCharacters ll, char value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeCharacters();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static char [] LinkedListCharactersToArray(LinkedListCharacters ll){
		char [] array;
		double length, i;
		LinkedListNodeCharacters node;

		node = ll.first;

		length = LinkedListCharactersLength(ll);

		array = new char [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static double LinkedListCharactersLength(LinkedListCharacters ll){
		double l;
		LinkedListNodeCharacters node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void FreeLinkedListCharacter(LinkedListCharacters ll){
		LinkedListNodeCharacters node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static void LinkedListCharactersAddString(LinkedListCharacters ll, char [] str){
		double i;

		for(i = 0d; i < str.length; i = i + 1d){
			LinkedListAddCharacter(ll, str[(int)(i)]);
		}
	}

	public static DynamicArrayNumbers CreateDynamicArrayNumbers(){
		DynamicArrayNumbers da;

		da = new DynamicArrayNumbers();
		da.array = new double [10];
		da.length = 0d;

		return da;
	}

	public static DynamicArrayNumbers CreateDynamicArrayNumbersWithInitialCapacity(double capacity){
		DynamicArrayNumbers da;

		da = new DynamicArrayNumbers();
		da.array = new double [(int)(capacity)];
		da.length = 0d;

		return da;
	}

	public static void DynamicArrayAddNumber(DynamicArrayNumbers da, double value){
		if(da.length == da.array.length){
			DynamicArrayNumbersIncreaseSize(da);
		}

		da.array[(int)(da.length)] = value;
		da.length = da.length + 1d;
	}

	public static void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers da){
		double newLength, i;
		double [] newArray;

		newLength = (double)round(da.array.length*3d/2d);
		newArray = new double [(int)(newLength)];

		for(i = 0d; i < da.array.length; i = i + 1d){
			newArray[(int)(i)] = da.array[(int)(i)];
		}

		delete(da.array);

		da.array = newArray;
	}

	public static boolean DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers da){
		boolean needsDecrease;

		needsDecrease = false;

		if(da.length > 10d){
			needsDecrease = da.length <= (double)round(da.array.length*2d/3d);
		}

		return needsDecrease;
	}

	public static void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers da){
		double newLength, i;
		double [] newArray;

		newLength = (double)round(da.array.length*2d/3d);
		newArray = new double [(int)(newLength)];

		for(i = 0d; i < newLength; i = i + 1d){
			newArray[(int)(i)] = da.array[(int)(i)];
		}

		delete(da.array);

		da.array = newArray;
	}

	public static double DynamicArrayNumbersIndex(DynamicArrayNumbers da, double index){
		return da.array[(int)(index)];
	}

	public static double DynamicArrayNumbersLength(DynamicArrayNumbers da){
		return da.length;
	}

	public static void DynamicArrayInsertNumber(DynamicArrayNumbers da, double index, double value){
		double i;

		if(da.length == da.array.length){
			DynamicArrayNumbersIncreaseSize(da);
		}

		for(i = da.length; i > index; i = i - 1d){
			da.array[(int)(i)] = da.array[(int)(i - 1d)];
		}

		da.array[(int)(index)] = value;

		da.length = da.length + 1d;
	}

	public static boolean DynamicArrayNumberSet(DynamicArrayNumbers da, double index, double value){
		boolean success;

		if(index < da.length){
			da.array[(int)(index)] = value;
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static void DynamicArrayRemoveNumber(DynamicArrayNumbers da, double index){
		double i;

		for(i = index; i < da.length - 1d; i = i + 1d){
			da.array[(int)(i)] = da.array[(int)(i + 1d)];
		}

		da.length = da.length - 1d;

		if(DynamicArrayNumbersDecreaseSizeNecessary(da)){
			DynamicArrayNumbersDecreaseSize(da);
		}
	}

	public static void FreeDynamicArrayNumbers(DynamicArrayNumbers da){
		delete(da.array);
		delete(da);
	}

	public static double [] DynamicArrayNumbersToArray(DynamicArrayNumbers da){
		double [] array;
		double i;

		array = new double [(int)(da.length)];

		for(i = 0d; i < da.length; i = i + 1d){
			array[(int)(i)] = da.array[(int)(i)];
		}

		return array;
	}

	public static DynamicArrayNumbers ArrayToDynamicArrayNumbersWithOptimalSize(double [] array){
		DynamicArrayNumbers da;
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
		c = array.length;
		n = (log(c) - 1d)/log(3d/2d);
		newCapacity = floor(n) + 1d;

		da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

		for(i = 0d; i < array.length; i = i + 1d){
			da.array[(int)(i)] = array[(int)(i)];
		}

		return da;
	}

	public static DynamicArrayNumbers ArrayToDynamicArrayNumbers(double [] array){
		DynamicArrayNumbers da;

		da = new DynamicArrayNumbers();
		da.array = CopyNumberArray(array);
		da.length = array.length;

		return da;
	}

	public static boolean DynamicArrayNumbersEqual(DynamicArrayNumbers a, DynamicArrayNumbers b){
		boolean equal;
		double i;

		equal = true;
		if(a.length == b.length){
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(a.array[(int)(i)] != b.array[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static LinkedListNumbers DynamicArrayNumbersToLinkedList(DynamicArrayNumbers da){
		LinkedListNumbers ll;
		double i;

		ll = CreateLinkedListNumbers();

		for(i = 0d; i < da.length; i = i + 1d){
			LinkedListAddNumber(ll, da.array[(int)(i)]);
		}

		return ll;
	}

	public static DynamicArrayNumbers LinkedListToDynamicArrayNumbers(LinkedListNumbers ll){
		DynamicArrayNumbers da;
		double i;
		LinkedListNodeNumbers node;

		node = ll.first;

		da = new DynamicArrayNumbers();
		da.length = LinkedListNumbersLength(ll);

		da.array = new double [(int)(da.length)];

		for(i = 0d; i < da.length; i = i + 1d){
			da.array[(int)(i)] = node.value;
			node = node.next;
		}

		return da;
	}

	public static char [] AddCharacter(char [] list, char a){
		char [] newlist;
		double i;

		newlist = new char [(int)(list.length + 1d)];
		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void AddCharacterRef(StringReference list, char i){
		list.string = AddCharacter(list.string, i);
	}

	public static char [] RemoveCharacter(char [] list, double n){
		char [] newlist;
		double i;

		newlist = new char [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}

		return newlist;
	}

	public static char GetCharacterRef(StringReference list, double i){
		return list.string[(int)(i)];
	}

	public static void RemoveCharacterRef(StringReference list, double i){
		list.string = RemoveCharacter(list.string, i);
	}

	public static void sWriteStringToStingStream(char [] stream, NumberReference index, char [] src){
		double i;

		for(i = 0d; i < src.length; i = i + 1d){
			stream[(int)(index.numberValue + i)] = src[(int)(i)];
		}
		index.numberValue = index.numberValue + src.length;
	}

	public static void sWriteCharacterToStingStream(char [] stream, NumberReference index, char src){
		stream[(int)(index.numberValue)] = src;
		index.numberValue = index.numberValue + 1d;
	}

	public static void sWriteBooleanToStingStream(char [] stream, NumberReference index, boolean src){
		if(src){
			sWriteStringToStingStream(stream, index, "true".toCharArray());
		}else{
			sWriteStringToStingStream(stream, index, "false".toCharArray());
		}
	}

	public static boolean sSubstringWithCheck(char [] string, double from, double to, StringReference stringReference){
		boolean success;

		if(from >= 0d && from <= string.length && to >= 0d && to <= string.length && from <= to){
			stringReference.string = sSubstring(string, from, to);
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static char [] sSubstring(char [] string, double from, double to){
		char [] n;
		double i, length;

		length = to - from;

		n = new char [(int)(length)];

		for(i = from; i < to; i = i + 1d){
			n[(int)(i - from)] = string[(int)(i)];
		}

		return n;
	}

	public static char [] sAppendString(char [] s1, char [] s2){
		char [] newString;

		newString = sConcatenateString(s1, s2);

		delete(s1);

		return newString;
	}

	public static char [] sConcatenateString(char [] s1, char [] s2){
		char [] newString;
		double i;

		newString = new char [(int)(s1.length + s2.length)];

		for(i = 0d; i < s1.length; i = i + 1d){
			newString[(int)(i)] = s1[(int)(i)];
		}

		for(i = 0d; i < s2.length; i = i + 1d){
			newString[(int)(s1.length + i)] = s2[(int)(i)];
		}

		return newString;
	}

	public static char [] sAppendCharacter(char [] string, char c){
		char [] newString;

		newString = sConcatenateCharacter(string, c);

		delete(string);

		return newString;
	}

	public static char [] sConcatenateCharacter(char [] string, char c){
		char [] newString;
		double i;
		newString = new char [(int)(string.length + 1d)];

		for(i = 0d; i < string.length; i = i + 1d){
			newString[(int)(i)] = string[(int)(i)];
		}

		newString[(int)(string.length)] = c;

		return newString;
	}

	public static StringReference [] sSplitByCharacter(char [] toSplit, char splitBy){
		StringReference [] split;
		char [] stringToSplitBy;

		stringToSplitBy = new char [1];
		stringToSplitBy[0] = splitBy;

		split = sSplitByString(toSplit, stringToSplitBy);

		delete(stringToSplitBy);

		return split;
	}

	public static boolean sIndexOfCharacter(char [] string, char character, NumberReference indexReference){
		double i;
		boolean found;

		found = false;
		for(i = 0d; i < string.length && !found; i = i + 1d){
			if(string[(int)(i)] == character){
				found = true;
				indexReference.numberValue = i;
			}
		}

		return found;
	}

	public static boolean sSubstringEqualsWithCheck(char [] string, double from, char [] substring, BooleanReference equalsReference){
		boolean success;

		if(from < string.length){
			success = true;
			equalsReference.booleanValue = sSubstringEquals(string, from, substring);
		}else{
			success = false;
		}

		return success;
	}

	public static boolean sSubstringEquals(char [] string, double from, char [] substring){
		double i;
		boolean equal;

		equal = true;
		if(string.length - from >= substring.length){
			for(i = 0d; i < substring.length && equal; i = i + 1d){
				if(string[(int)(from + i)] != substring[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static boolean sIndexOfString(char [] string, char [] substring, NumberReference indexReference){
		double i;
		boolean found;

		found = false;
		for(i = 0d; i < string.length - substring.length + 1d && !found; i = i + 1d){
			if(sSubstringEquals(string, i, substring)){
				found = true;
				indexReference.numberValue = i;
			}
		}

		return found;
	}

	public static boolean sContainsCharacter(char [] string, char character){
		double i;
		boolean found;

		found = false;
		for(i = 0d; i < string.length && !found; i = i + 1d){
			if(string[(int)(i)] == character){
				found = true;
			}
		}

		return found;
	}

	public static boolean sContainsString(char [] string, char [] substring){
		return sIndexOfString(string, substring, new NumberReference());
	}

	public static void sToUpperCase(char [] string){
		double i;

		for(i = 0d; i < string.length; i = i + 1d){
			string[(int)(i)] = charToUpperCase(string[(int)(i)]);
		}
	}

	public static void sToLowerCase(char [] string){
		double i;

		for(i = 0d; i < string.length; i = i + 1d){
			string[(int)(i)] = charToLowerCase(string[(int)(i)]);
		}
	}

	public static boolean sEqualsIgnoreCase(char [] a, char [] b){
		boolean equal;
		double i;

		if(a.length == b.length){
			equal = true;
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(charToLowerCase(a[(int)(i)]) != charToLowerCase(b[(int)(i)])){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static char [] sReplaceString(char [] string, char [] toReplace, char [] replaceWith){
		char [] result;
		double i, j;
		BooleanReference equalsReference;
		boolean success;
		DynamicArrayCharacters da;

		da = CreateDynamicArrayCharacters();

		equalsReference = new BooleanReference();

		for(i = 0d; i < string.length; ){
			success = sSubstringEqualsWithCheck(string, i, toReplace, equalsReference);
			if(success){
				success = equalsReference.booleanValue;
			}

			if(success && toReplace.length > 0d){
				for(j = 0d; j < replaceWith.length; j = j + 1d){
					DynamicArrayAddCharacter(da, replaceWith[(int)(j)]);
				}
				i = i + toReplace.length;
			}else{
				DynamicArrayAddCharacter(da, string[(int)(i)]);
				i = i + 1d;
			}
		}

		result = DynamicArrayCharactersToArray(da);

		FreeDynamicArrayCharacters(da);

		return result;
	}

	public static char [] sReplaceCharacterToNew(char [] string, char toReplace, char replaceWith){
		char [] result;
		double i;

		result = new char [(int)(string.length)];

		for(i = 0d; i < string.length; i = i + 1d){
			if(string[(int)(i)] == toReplace){
				result[(int)(i)] = replaceWith;
			}else{
				result[(int)(i)] = string[(int)(i)];
			}
		}

		return result;
	}

	public static void sReplaceCharacter(char [] string, char toReplace, char replaceWith){
		double i;

		for(i = 0d; i < string.length; i = i + 1d){
			if(string[(int)(i)] == toReplace){
				string[(int)(i)] = replaceWith;
			}
		}
	}

	public static char [] sTrim(char [] string){
		char [] result;
		double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
		boolean firstNonWhitespaceFound;

		/* Find whitepaces at the start.*/
		lastWhitespaceLocationStart = -1d;
		firstNonWhitespaceFound = false;
		for(i = 0d; i < string.length && !firstNonWhitespaceFound; i = i + 1d){
			if(charIsWhiteSpace(string[(int)(i)])){
				lastWhitespaceLocationStart = i;
			}else{
				firstNonWhitespaceFound = true;
			}
		}

		/* Find whitepaces at the end.*/
		lastWhitespaceLocationEnd = string.length;
		firstNonWhitespaceFound = false;
		for(i = string.length - 1d; i >= 0d && !firstNonWhitespaceFound; i = i - 1d){
			if(charIsWhiteSpace(string[(int)(i)])){
				lastWhitespaceLocationEnd = i;
			}else{
				firstNonWhitespaceFound = true;
			}
		}

		if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
			result = sSubstring(string, lastWhitespaceLocationStart + 1d, lastWhitespaceLocationEnd);
		}else{
			result = new char [0];
		}

		return result;
	}

	public static boolean sStartsWith(char [] string, char [] start){
		boolean startsWithString;

		startsWithString = false;
		if(string.length >= start.length){
			startsWithString = sSubstringEquals(string, 0d, start);
		}

		return startsWithString;
	}

	public static boolean sEndsWith(char [] string, char [] end){
		boolean endsWithString;

		endsWithString = false;
		if(string.length >= end.length){
			endsWithString = sSubstringEquals(string, string.length - end.length, end);
		}

		return endsWithString;
	}

	public static StringReference [] sSplitByString(char [] toSplit, char [] splitBy){
		StringReference [] split;
		char [] next;
		double i;
		char c;
		StringReference n;

		split = new StringReference [0];

		next = new char [0];
		for(i = 0d; i < toSplit.length; ){
			c = toSplit[(int)(i)];

			if(sSubstringEquals(toSplit, i, splitBy)){
				n = new StringReference();
				n.string = next;
				split = AddString(split, n);
				next = new char [0];
				i = i + splitBy.length;
			}else{
				next = sAppendCharacter(next, c);
				i = i + 1d;
			}
		}

		n = new StringReference();
		n.string = next;
		split = AddString(split, n);

		return split;
	}

	public static boolean sStringIsBefore(char [] a, char [] b){
		boolean before, equal, done;
		double i;

		before = false;
		equal = true;
		done = false;

		if(a.length == 0d && b.length > 0d){
			before = true;
		}else{
			for(i = 0d; i < a.length && i < b.length && !done; i = i + 1d){
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
				if(a.length < b.length){
					before = true;
				}
			}
		}

		return before;
	}

	public static void strWriteStringToStingStream(char [] stream, NumberReference index, char [] src){
		double i;

		for(i = 0d; i < src.length; i = i + 1d){
			stream[(int)(index.numberValue + i)] = src[(int)(i)];
		}
		index.numberValue = index.numberValue + src.length;
	}

	public static void strWriteCharacterToStingStream(char [] stream, NumberReference index, char src){
		stream[(int)(index.numberValue)] = src;
		index.numberValue = index.numberValue + 1d;
	}

	public static void strWriteBooleanToStingStream(char [] stream, NumberReference index, boolean src){
		if(src){
			strWriteStringToStingStream(stream, index, "true".toCharArray());
		}else{
			strWriteStringToStingStream(stream, index, "false".toCharArray());
		}
	}

	public static boolean strSubstringWithCheck(char [] string, double from, double to, StringReference stringReference){
		boolean success;

		if(from >= 0d && from <= string.length && to >= 0d && to <= string.length && from <= to){
			stringReference.string = strSubstring(string, from, to);
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static char [] strSubstring(char [] string, double from, double to){
		char [] n;
		double i, length;

		length = to - from;

		n = new char [(int)(length)];

		for(i = from; i < to; i = i + 1d){
			n[(int)(i - from)] = string[(int)(i)];
		}

		return n;
	}

	public static char [] strAppendString(char [] s1, char [] s2){
		char [] newString;

		newString = strConcatenateString(s1, s2);

		delete(s1);

		return newString;
	}

	public static char [] strConcatenateString(char [] s1, char [] s2){
		char [] newString;
		double i;

		newString = new char [(int)(s1.length + s2.length)];

		for(i = 0d; i < s1.length; i = i + 1d){
			newString[(int)(i)] = s1[(int)(i)];
		}

		for(i = 0d; i < s2.length; i = i + 1d){
			newString[(int)(s1.length + i)] = s2[(int)(i)];
		}

		return newString;
	}

	public static char [] strAppendCharacter(char [] string, char c){
		char [] newString;

		newString = strConcatenateCharacter(string, c);

		delete(string);

		return newString;
	}

	public static char [] strConcatenateCharacter(char [] string, char c){
		char [] newString;
		double i;
		newString = new char [(int)(string.length + 1d)];

		for(i = 0d; i < string.length; i = i + 1d){
			newString[(int)(i)] = string[(int)(i)];
		}

		newString[(int)(string.length)] = c;

		return newString;
	}

	public static StringReference [] strSplitByCharacter(char [] toSplit, char splitBy){
		StringReference [] split;
		char [] stringToSplitBy;

		stringToSplitBy = new char [1];
		stringToSplitBy[0] = splitBy;

		split = strSplitByString(toSplit, stringToSplitBy);

		delete(stringToSplitBy);

		return split;
	}

	public static boolean strIndexOfCharacter(char [] string, char character, NumberReference indexReference){
		double i;
		boolean found;

		found = false;
		for(i = 0d; i < string.length && !found; i = i + 1d){
			if(string[(int)(i)] == character){
				found = true;
				indexReference.numberValue = i;
			}
		}

		return found;
	}

	public static boolean strSubstringEqualsWithCheck(char [] string, double from, char [] substring, BooleanReference equalsReference){
		boolean success;

		if(from < string.length){
			success = true;
			equalsReference.booleanValue = strSubstringEquals(string, from, substring);
		}else{
			success = false;
		}

		return success;
	}

	public static boolean strSubstringEquals(char [] string, double from, char [] substring){
		double i;
		boolean equal;

		equal = true;
		for(i = 0d; i < substring.length && equal; i = i + 1d){
			if(string[(int)(from + i)] != substring[(int)(i)]){
				equal = false;
			}
		}

		return equal;
	}

	public static boolean strIndexOfString(char [] string, char [] substring, NumberReference indexReference){
		double i;
		boolean found;

		found = false;
		for(i = 0d; i < string.length - substring.length + 1d && !found; i = i + 1d){
			if(strSubstringEquals(string, i, substring)){
				found = true;
				indexReference.numberValue = i;
			}
		}

		return found;
	}

	public static boolean strContainsCharacter(char [] string, char character){
		double i;
		boolean found;

		found = false;
		for(i = 0d; i < string.length && !found; i = i + 1d){
			if(string[(int)(i)] == character){
				found = true;
			}
		}

		return found;
	}

	public static boolean strContainsString(char [] string, char [] substring){
		return strIndexOfString(string, substring, new NumberReference());
	}

	public static void strToUpperCase(char [] string){
		double i;

		for(i = 0d; i < string.length; i = i + 1d){
			string[(int)(i)] = charToUpperCase(string[(int)(i)]);
		}
	}

	public static void strToLowerCase(char [] string){
		double i;

		for(i = 0d; i < string.length; i = i + 1d){
			string[(int)(i)] = charToLowerCase(string[(int)(i)]);
		}
	}

	public static boolean strEqualsIgnoreCase(char [] a, char [] b){
		boolean equal;
		double i;

		if(a.length == b.length){
			equal = true;
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(charToLowerCase(a[(int)(i)]) != charToLowerCase(b[(int)(i)])){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static char [] strReplaceString(char [] string, char [] toReplace, char [] replaceWith){
		char [] result;
		double i;
		BooleanReference equalsReference;
		boolean success;

		equalsReference = new BooleanReference();
		result = new char [0];

		for(i = 0d; i < string.length; ){
			success = strSubstringEqualsWithCheck(string, i, toReplace, equalsReference);
			if(success){
				success = equalsReference.booleanValue;
			}

			if(success && toReplace.length > 0d){
				result = strConcatenateString(result, replaceWith);
				i = i + toReplace.length;
			}else{
				result = strConcatenateCharacter(result, string[(int)(i)]);
				i = i + 1d;
			}
		}

		return result;
	}

	public static char [] strReplaceCharacter(char [] string, char toReplace, char replaceWith){
		char [] result;
		double i;

		result = new char [0];

		for(i = 0d; i < string.length; i = i + 1d){
			if(string[(int)(i)] == toReplace){
				result = strConcatenateCharacter(result, replaceWith);
			}else{
				result = strConcatenateCharacter(result, string[(int)(i)]);
			}
		}

		return result;
	}

	public static char [] strTrim(char [] string){
		char [] result;
		double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
		boolean firstNonWhitespaceFound;

		/* Find whitepaces at the start.*/
		lastWhitespaceLocationStart = -1d;
		firstNonWhitespaceFound = false;
		for(i = 0d; i < string.length && !firstNonWhitespaceFound; i = i + 1d){
			if(charIsWhiteSpace(string[(int)(i)])){
				lastWhitespaceLocationStart = i;
			}else{
				firstNonWhitespaceFound = true;
			}
		}

		/* Find whitepaces at the end.*/
		lastWhitespaceLocationEnd = string.length;
		firstNonWhitespaceFound = false;
		for(i = string.length - 1d; i >= 0d && !firstNonWhitespaceFound; i = i - 1d){
			if(charIsWhiteSpace(string[(int)(i)])){
				lastWhitespaceLocationEnd = i;
			}else{
				firstNonWhitespaceFound = true;
			}
		}

		if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
			result = strSubstring(string, lastWhitespaceLocationStart + 1d, lastWhitespaceLocationEnd);
		}else{
			result = new char [0];
		}

		return result;
	}

	public static boolean strStartsWith(char [] string, char [] start){
		boolean startsWithString;

		startsWithString = false;
		if(string.length >= start.length){
			startsWithString = strSubstringEquals(string, 0d, start);
		}

		return startsWithString;
	}

	public static boolean strEndsWith(char [] string, char [] end){
		boolean endsWithString;

		endsWithString = false;
		if(string.length >= end.length){
			endsWithString = strSubstringEquals(string, string.length - end.length, end);
		}

		return endsWithString;
	}

	public static StringReference [] strSplitByString(char [] toSplit, char [] splitBy){
		StringReference [] split;
		char [] next;
		double i;
		char c;
		StringReference n;

		split = new StringReference [0];

		next = new char [0];
		for(i = 0d; i < toSplit.length; ){
			c = toSplit[(int)(i)];

			if(strSubstringEquals(toSplit, i, splitBy)){
				if(split.length != 0d || i != 0d){
					n = new StringReference();
					n.string = next;
					split = lAddString(split, n);
					next = new char [0];
					i = i + splitBy.length;
				}
			}else{
				next = strAppendCharacter(next, c);
				i = i + 1d;
			}
		}

		if(next.length > 0d){
			n = new StringReference();
			n.string = next;
			split = lAddString(split, n);
		}

		return split;
	}

	public static boolean strStringIsBefore(char [] a, char [] b){
		boolean before, equal, done;
		double i;

		before = false;
		equal = true;
		done = false;

		if(a.length == 0d && b.length > 0d){
			before = true;
		}else{
			for(i = 0d; i < a.length && i < b.length && !done; i = i + 1d){
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
				if(a.length < b.length){
					before = true;
				}
			}
		}

		return before;
	}

	public static double [] StringToNumberArray(char [] string){
		double i;
		double [] array;

		array = new double [(int)(string.length)];

		for(i = 0d; i < string.length; i = i + 1d){
			array[(int)(i)] = string[(int)(i)];
		}
		return array;
	}

	public static char [] NumberArrayToString(double [] array){
		double i;
		char [] string;

		string = new char [(int)(array.length)];

		for(i = 0d; i < array.length; i = i + 1d){
			string[(int)(i)] = (char)(array[(int)(i)]);
		}
		return string;
	}

	public static boolean NumberArraysEqual(double [] a, double [] b){
		boolean equal;
		double i;

		equal = true;
		if(a.length == b.length){
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(a[(int)(i)] != b[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static boolean BooleanArraysEqual(boolean [] a, boolean [] b){
		boolean equal;
		double i;

		equal = true;
		if(a.length == b.length){
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(a[(int)(i)] != b[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static boolean StringsEqual(char [] a, char [] b){
		boolean equal;
		double i;

		equal = true;
		if(a.length == b.length){
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(a[(int)(i)] != b[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static void FillNumberArray(double [] a, double value){
		double i;

		for(i = 0d; i < a.length; i = i + 1d){
			a[(int)(i)] = value;
		}
	}

	public static void FillString(char [] a, char value){
		double i;

		for(i = 0d; i < a.length; i = i + 1d){
			a[(int)(i)] = value;
		}
	}

	public static void FillBooleanArray(boolean [] a, boolean value){
		double i;

		for(i = 0d; i < a.length; i = i + 1d){
			a[(int)(i)] = value;
		}
	}

	public static boolean FillNumberArrayRange(double [] a, double value, double from, double to){
		double i, length;
		boolean success;

		if(from >= 0d && from <= a.length && to >= 0d && to <= a.length && from <= to){
			length = to - from;
			for(i = 0d; i < length; i = i + 1d){
				a[(int)(from + i)] = value;
			}

			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static boolean FillBooleanArrayRange(boolean [] a, boolean value, double from, double to){
		double i, length;
		boolean success;

		if(from >= 0d && from <= a.length && to >= 0d && to <= a.length && from <= to){
			length = to - from;
			for(i = 0d; i < length; i = i + 1d){
				a[(int)(from + i)] = value;
			}

			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static boolean FillStringRange(char [] a, char value, double from, double to){
		double i, length;
		boolean success;

		if(from >= 0d && from <= a.length && to >= 0d && to <= a.length && from <= to){
			length = to - from;
			for(i = 0d; i < length; i = i + 1d){
				a[(int)(from + i)] = value;
			}

			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static double [] CopyNumberArray(double [] a){
		double i;
		double [] n;

		n = new double [(int)(a.length)];

		for(i = 0d; i < a.length; i = i + 1d){
			n[(int)(i)] = a[(int)(i)];
		}

		return n;
	}

	public static boolean [] CopyBooleanArray(boolean [] a){
		double i;
		boolean [] n;

		n = new boolean [(int)(a.length)];

		for(i = 0d; i < a.length; i = i + 1d){
			n[(int)(i)] = a[(int)(i)];
		}

		return n;
	}

	public static char [] CopyString(char [] a){
		double i;
		char [] n;

		n = new char [(int)(a.length)];

		for(i = 0d; i < a.length; i = i + 1d){
			n[(int)(i)] = a[(int)(i)];
		}

		return n;
	}

	public static boolean CopyNumberArrayRange(double [] a, double from, double to, NumberArrayReference copyReference){
		double i, length;
		double [] n;
		boolean success;

		if(from >= 0d && from <= a.length && to >= 0d && to <= a.length && from <= to){
			length = to - from;
			n = new double [(int)(length)];

			for(i = 0d; i < length; i = i + 1d){
				n[(int)(i)] = a[(int)(from + i)];
			}

			copyReference.numberArray = n;
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static boolean CopyBooleanArrayRange(boolean [] a, double from, double to, BooleanArrayReference copyReference){
		double i, length;
		boolean [] n;
		boolean success;

		if(from >= 0d && from <= a.length && to >= 0d && to <= a.length && from <= to){
			length = to - from;
			n = new boolean [(int)(length)];

			for(i = 0d; i < length; i = i + 1d){
				n[(int)(i)] = a[(int)(from + i)];
			}

			copyReference.booleanArray = n;
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static boolean CopyStringRange(char [] a, double from, double to, StringReference copyReference){
		double i, length;
		char [] n;
		boolean success;

		if(from >= 0d && from <= a.length && to >= 0d && to <= a.length && from <= to){
			length = to - from;
			n = new char [(int)(length)];

			for(i = 0d; i < length; i = i + 1d){
				n[(int)(i)] = a[(int)(from + i)];
			}

			copyReference.string = n;
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static boolean IsLastElement(double length, double index){
		return index + 1d == length;
	}

	public static double [] CreateNumberArray(double length, double value){
		double [] array;

		array = new double [(int)(length)];
		FillNumberArray(array, value);

		return array;
	}

	public static boolean [] CreateBooleanArray(double length, boolean value){
		boolean [] array;

		array = new boolean [(int)(length)];
		FillBooleanArray(array, value);

		return array;
	}

	public static char [] CreateString(double length, char value){
		char [] array;

		array = new char [(int)(length)];
		FillString(array, value);

		return array;
	}

	public static void SwapElementsOfNumberArray(double [] A, double ai, double bi){
		double tmp;

		tmp = A[(int)(ai)];
		A[(int)(ai)] = A[(int)(bi)];
		A[(int)(bi)] = tmp;
	}

	public static void SwapElementsOfStringArray(StringArrayReference A, double ai, double bi){
		StringReference tmp;

		tmp = A.stringArray[(int)(ai)];
		A.stringArray[(int)(ai)] = A.stringArray[(int)(bi)];
		A.stringArray[(int)(bi)] = tmp;
	}

	public static void ReverseNumberArray(double [] array){
		double i;

		for(i = 0d; i < array.length/2d; i = i + 1d){
			SwapElementsOfNumberArray(array, i, array.length - i - 1d);
		}
	}

	public static void AssertFalse(boolean b, NumberReference failures){
		if(b){
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertTrue(boolean b, NumberReference failures){
		if(!b){
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertEquals(double a, double b, NumberReference failures){
		if(a != b){
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertBooleansEqual(boolean a, boolean b, NumberReference failures){
		if(a != b){
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertCharactersEqual(char a, char b, NumberReference failures){
		if(a != b){
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertStringEquals(char [] a, char [] b, NumberReference failures){
		if(!StringsEqual(a, b)){
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertNumberArraysEqual(double [] a, double [] b, NumberReference failures){
		double i;

		if(a.length == b.length){
			for(i = 0d; i < a.length; i = i + 1d){
				AssertEquals(a[(int)(i)], b[(int)(i)], failures);
			}
		}else{
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertBooleanArraysEqual(boolean [] a, boolean [] b, NumberReference failures){
		double i;

		if(a.length == b.length){
			for(i = 0d; i < a.length; i = i + 1d){
				AssertBooleansEqual(a[(int)(i)], b[(int)(i)], failures);
			}
		}else{
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static void AssertStringArraysEqual(StringReference [] a, StringReference [] b, NumberReference failures){
		double i;

		if(a.length == b.length){
			for(i = 0d; i < a.length; i = i + 1d){
				AssertStringEquals(a[(int)(i)].string, b[(int)(i)].string, failures);
			}
		}else{
			failures.numberValue = failures.numberValue + 1d;
		}
	}

	public static char [] nCreateStringScientificNotationDecimalFromNumber(double decimal){
		StringReference mantissaReference, exponentReference;
		double multiplier, inc;
		double exponent;
		boolean done, isPositive;
		char [] result;

		mantissaReference = new StringReference();
		exponentReference = new StringReference();
		result = new char [0];
		done = false;
		exponent = 0d;

		if(decimal < 0d){
			isPositive = false;
			decimal = -decimal;
		}else{
			isPositive = true;
		}

		if(decimal == 0d){
			done = true;
		}

		if(!done){
			multiplier = 0d;
			inc = 0d;

			if(decimal < 1d){
				multiplier = 10d;
				inc = -1d;
			}else if(decimal >= 10d){
				multiplier = 0.1;
				inc = 1d;
			}else{
				done = true;
			}

			if(!done){
				for(; decimal >= 10d || decimal < 1d; ){
					decimal = decimal*multiplier;
					exponent = exponent + inc;
				}
			}
		}

		nCreateStringFromNumberWithCheck(decimal, 10d, mantissaReference);

		nCreateStringFromNumberWithCheck(exponent, 10d, exponentReference);

		if(!isPositive){
			result = strAppendString(result, "-".toCharArray());
		}

		result = strAppendString(result, mantissaReference.string);
		result = strAppendString(result, "e".toCharArray());
		result = strAppendString(result, exponentReference.string);

		return result;
	}

	public static char [] nCreateStringDecimalFromNumber(double decimal){
		StringReference stringReference;

		stringReference = new StringReference();

		/* This will succeed because base = 10.*/
		nCreateStringFromNumberWithCheck(decimal, 10d, stringReference);

		return stringReference.string;
	}

	public static boolean nCreateStringFromNumberWithCheck(double decimal, double base, StringReference stringReference){
		char [] string;
		double maximumDigits;
		double digitPosition;
		boolean hasPrintedPoint, isPositive;
		double i, d;
		boolean success;
		CharacterReference characterReference;
		char c;

		isPositive = true;

		if(decimal < 0d){
			isPositive = false;
			decimal = -decimal;
		}

		if(decimal == 0d){
			stringReference.string = "0".toCharArray();
			success = true;
		}else{
			characterReference = new CharacterReference();

			if(IsInteger(base)){
				success = true;

				string = new char [0];

				maximumDigits = nGetMaximumDigitsForBase(base);

				digitPosition = nGetFirstDigitPosition(decimal, base);

				decimal = (double)round(decimal*pow(base, maximumDigits - digitPosition - 1d));

				hasPrintedPoint = false;

				if(!isPositive){
					string = strAppendCharacter(string, '-');
				}

				/* Print leading zeros.*/
				if(digitPosition < 0d){
					string = strAppendCharacter(string, '0');
					string = strAppendCharacter(string, '.');
					hasPrintedPoint = true;
					for(i = 0d; i < -digitPosition - 1d; i = i + 1d){
						string = strAppendCharacter(string, '0');
					}
				}

				/* Print number.*/
				for(i = 0d; i < maximumDigits && success; i = i + 1d){
					d = floor(decimal/pow(base, maximumDigits - i - 1d));

					if(d >= base){
						d = base - 1d;
					}

					if(!hasPrintedPoint && digitPosition - i + 1d == 0d){
						if(decimal != 0d){
							string = strAppendCharacter(string, '.');
						}
						hasPrintedPoint = true;
					}

					if(decimal == 0d && hasPrintedPoint){
					}else{
						success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
						if(success){
							c = characterReference.characterValue;
							string = strAppendCharacter(string, c);
						}
					}

					if(success){
						decimal = decimal - d*pow(base, maximumDigits - i - 1d);
					}
				}

				if(success){
					/* Print trailing zeros.*/
					for(i = 0d; i < digitPosition - maximumDigits + 1d; i = i + 1d){
						string = strAppendCharacter(string, '0');
					}

					stringReference.string = string;
				}
			}else{
				success = false;
			}
		}

		/* Done*/
		return success;
	}

	public static double nGetMaximumDigitsForBase(double base){
		double t;

		t = pow(10d, 15d);
		return floor(log10(t)/log10(base));
	}

	public static double nGetFirstDigitPosition(double decimal, double base){
		double power;
		double t;

		power = ceil(log10(decimal)/log10(base));

		t = decimal*pow(base, -power);
		if(t < base && t >= 1d){
		}else if(t >= base){
			power = power + 1d;
		}else if(t < 1d){
			power = power - 1d;
		}

		return power;
	}

	public static boolean nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference characterReference){
		char [] numberTable;
		boolean success;

		numberTable = nGetDigitCharacterTable();

		if(c < base || c < numberTable.length){
			success = true;
			characterReference.characterValue = numberTable[(int)(c)];
		}else{
			success = false;
		}

		return success;
	}

	public static char [] nGetDigitCharacterTable(){
		char [] numberTable;

		numberTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".toCharArray();

		return numberTable;
	}

	public static boolean nCreateNumberFromDecimalStringWithCheck(char [] string, NumberReference decimalReference, StringReference errorMessage){
		return nCreateNumberFromStringWithCheck(string, 10d, decimalReference, errorMessage);
	}

	public static double nCreateNumberFromDecimalString(char [] string){
		NumberReference doubleReference;
		StringReference stringReference;
		double number;

		doubleReference = CreateNumberReference(0d);
		stringReference = CreateStringReference("".toCharArray());
		nCreateNumberFromStringWithCheck(string, 10d, doubleReference, stringReference);
		number = doubleReference.numberValue;

		delete(doubleReference);
		delete(stringReference);

		return number;
	}

	public static boolean nCreateNumberFromStringWithCheck(char [] string, double base, NumberReference numberReference, StringReference errorMessage){
		boolean success;
		BooleanReference numberIsPositive, exponentIsPositive;
		NumberArrayReference beforePoint, afterPoint, exponent;

		numberIsPositive = CreateBooleanReference(true);
		exponentIsPositive = CreateBooleanReference(true);
		beforePoint = new NumberArrayReference();
		afterPoint = new NumberArrayReference();
		exponent = new NumberArrayReference();

		if(base >= 2d && base <= 36d){
			success = nExtractPartsFromNumberString(string, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage);

			if(success){
				numberReference.numberValue = nCreateNumberFromParts(base, numberIsPositive.booleanValue, beforePoint.numberArray, afterPoint.numberArray, exponentIsPositive.booleanValue, exponent.numberArray);
			}
		}else{
			success = false;
			errorMessage.string = "Base must be from 2 to 36.".toCharArray();
		}

		return success;
	}

	public static double nCreateNumberFromParts(double base, boolean numberIsPositive, double [] beforePoint, double [] afterPoint, boolean exponentIsPositive, double [] exponent){
		double n, i, p, e;

		n = 0d;

		for(i = 0d; i < beforePoint.length; i = i + 1d){
			p = beforePoint[(int)(beforePoint.length - i - 1d)];

			n = n + p*pow(base, i);
		}

		for(i = 0d; i < afterPoint.length; i = i + 1d){
			p = afterPoint[(int)(i)];

			n = n + p*pow(base, -(i + 1d));
		}

		if(exponent.length > 0d){
			e = 0d;
			for(i = 0d; i < exponent.length; i = i + 1d){
				p = exponent[(int)(exponent.length - i - 1d)];

				e = e + p*pow(base, i);
			}

			if(!exponentIsPositive){
				e = -e;
			}

			n = n*pow(base, e);
		}

		if(!numberIsPositive){
			n = -n;
		}

		return n;
	}

	public static boolean nExtractPartsFromNumberString(char [] n, double base, BooleanReference numberIsPositive, NumberArrayReference beforePoint, NumberArrayReference afterPoint, BooleanReference exponentIsPositive, NumberArrayReference exponent, StringReference errorMessages){
		double i;
		boolean success;

		i = 0d;

		if(i < n.length){
			if(n[(int)(i)] == '-'){
				numberIsPositive.booleanValue = false;
				i = i + 1d;
			}else if(n[(int)(i)] == '+'){
				numberIsPositive.booleanValue = true;
				i = i + 1d;
			}

			success = nExtractPartsFromNumberStringFromSign(n, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages);
		}else{
			success = false;
			errorMessages.string = "Number cannot have length zero.".toCharArray();
		}

		return success;
	}

	public static boolean nExtractPartsFromNumberStringFromSign(char [] n, double base, double i, NumberArrayReference beforePoint, NumberArrayReference afterPoint, BooleanReference exponentIsPositive, NumberArrayReference exponent, StringReference errorMessages){
		boolean success, done;
		double count, j;

		done = false;
		count = 0d;
		for(; i + count < n.length && !done; ){
			if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
				count = count + 1d;
			}else{
				done = true;
			}
		}

		if(count >= 1d){
			beforePoint.numberArray = new double [(int)(count)];

			for(j = 0d; j < count; j = j + 1d){
				beforePoint.numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
			}

			i = i + count;

			if(i < n.length){
				success = nExtractPartsFromNumberStringFromPointOrExponent(n, base, i, afterPoint, exponentIsPositive, exponent, errorMessages);
			}else{
				afterPoint.numberArray = new double [0];
				exponent.numberArray = new double [0];
				success = true;
			}
		}else{
			success = false;
			errorMessages.string = "Number must have at least one number after the optional sign.".toCharArray();
		}

		return success;
	}

	public static boolean nExtractPartsFromNumberStringFromPointOrExponent(char [] n, double base, double i, NumberArrayReference afterPoint, BooleanReference exponentIsPositive, NumberArrayReference exponent, StringReference errorMessages){
		boolean success, done;
		double count, j;

		if(n[(int)(i)] == '.'){
			i = i + 1d;

			if(i < n.length){
				done = false;
				count = 0d;
				for(; i + count < n.length && !done; ){
					if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
						count = count + 1d;
					}else{
						done = true;
					}
				}

				if(count >= 1d){
					afterPoint.numberArray = new double [(int)(count)];

					for(j = 0d; j < count; j = j + 1d){
						afterPoint.numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
					}

					i = i + count;

					if(i < n.length){
						success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages);
					}else{
						exponent.numberArray = new double [0];
						success = true;
					}
				}else{
					success = false;
					errorMessages.string = "There must be at least one digit after the decimal point.".toCharArray();
				}
			}else{
				success = false;
				errorMessages.string = "There must be at least one digit after the decimal point.".toCharArray();
			}
		}else if(base <= 14d && (n[(int)(i)] == 'e' || n[(int)(i)] == 'E')){
			if(i < n.length){
				success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages);
				afterPoint.numberArray = new double [0];
			}else{
				success = false;
				errorMessages.string = "There must be at least one digit after the exponent.".toCharArray();
			}
		}else{
			success = false;
			errorMessages.string = "Expected decimal point or exponent symbol.".toCharArray();
		}

		return success;
	}

	public static boolean nExtractPartsFromNumberStringFromExponent(char [] n, double base, double i, BooleanReference exponentIsPositive, NumberArrayReference exponent, StringReference errorMessages){
		boolean success, done;
		double count, j;

		if(base <= 14d && (n[(int)(i)] == 'e' || n[(int)(i)] == 'E')){
			i = i + 1d;

			if(i < n.length){
				if(n[(int)(i)] == '-'){
					exponentIsPositive.booleanValue = false;
					i = i + 1d;
				}else if(n[(int)(i)] == '+'){
					exponentIsPositive.booleanValue = true;
					i = i + 1d;
				}

				if(i < n.length){
					done = false;
					count = 0d;
					for(; i + count < n.length && !done; ){
						if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
							count = count + 1d;
						}else{
							done = true;
						}
					}

					if(count >= 1d){
						exponent.numberArray = new double [(int)(count)];

						for(j = 0d; j < count; j = j + 1d){
							exponent.numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
						}

						i = i + count;

						if(i == n.length){
							success = true;
						}else{
							success = false;
							errorMessages.string = "There cannot be any characters past the exponent of the number.".toCharArray();
						}
					}else{
						success = false;
						errorMessages.string = "There must be at least one digit after the decimal point.".toCharArray();
					}
				}else{
					success = false;
					errorMessages.string = "There must be at least one digit after the exponent symbol.".toCharArray();
				}
			}else{
				success = false;
				errorMessages.string = "There must be at least one digit after the exponent symbol.".toCharArray();
			}
		}else{
			success = false;
			errorMessages.string = "Expected exponent symbol.".toCharArray();
		}

		return success;
	}

	public static double nGetNumberFromNumberCharacterForBase(char c, double base){
		char [] numberTable;
		double i;
		double position;

		numberTable = nGetDigitCharacterTable();
		position = 0d;

		for(i = 0d; i < base; i = i + 1d){
			if(numberTable[(int)(i)] == c){
				position = i;
			}
		}

		return position;
	}

	public static boolean nCharacterIsNumberCharacterInBase(char c, double base){
		char [] numberTable;
		double i;
		boolean found;

		numberTable = nGetDigitCharacterTable();
		found = false;

		for(i = 0d; i < base; i = i + 1d){
			if(numberTable[(int)(i)] == c){
				found = true;
			}
		}

		return found;
	}

	public static double [] nStringToNumberArray(char [] str){
		NumberArrayReference numberArrayReference;
		StringReference stringReference;
		double [] numbers;

		numberArrayReference = new NumberArrayReference();
		stringReference = new StringReference();

		nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference);

		numbers = numberArrayReference.numberArray;

		delete(numberArrayReference);
		delete(stringReference);

		return numbers;
	}

	public static boolean nStringToNumberArrayWithCheck(char [] str, NumberArrayReference numberArrayReference, StringReference errorMessage){
		StringReference [] numberStrings;
		double [] numbers;
		double i;
		char [] numberString, trimmedNumberString;
		boolean success;
		NumberReference numberReference;

		numberStrings = strSplitByString(str, ",".toCharArray());

		numbers = new double [(int)(numberStrings.length)];
		success = true;
		numberReference = new NumberReference();

		for(i = 0d; i < numberStrings.length; i = i + 1d){
			numberString = numberStrings[(int)(i)].string;
			trimmedNumberString = strTrim(numberString);
			success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage);
			numbers[(int)(i)] = numberReference.numberValue;

			FreeStringReference(numberStrings[(int)(i)]);
			delete(trimmedNumberString);
		}

		delete(numberStrings);
		delete(numberReference);

		numberArrayReference.numberArray = numbers;

		return success;
	}

static public class lLinkedListNodeStrings{
	public boolean end;
	public char [] value;
	public lLinkedListNodeStrings next;
}
static public class lLinkedListStrings{
	public lLinkedListNodeStrings first;
	public lLinkedListNodeStrings last;
}
static public class lLinkedListNodeNumbers{
	public lLinkedListNodeNumbers next;
	public boolean end;
	public double value;
}
static public class lLinkedListNumbers{
	public lLinkedListNodeNumbers first;
	public lLinkedListNodeNumbers last;
}
static public class lLinkedListCharacters{
	public lLinkedListNodeCharacters first;
	public lLinkedListNodeCharacters last;
}
static public class lLinkedListNodeCharacters{
	public boolean end;
	public char value;
	public lLinkedListNodeCharacters next;
}
static public class lDynamicArrayNumbers{
	public double [] array;
	public double length;
}
	public static double [] lAddNumber(double [] list, double a){
		double [] newlist;
		double i;

		newlist = new double [(int)(list.length + 1d)];
		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void lAddNumberRef(NumberArrayReference list, double i){
		list.numberArray = lAddNumber(list.numberArray, i);
	}

	public static double [] lRemoveNumber(double [] list, double n){
		double [] newlist;
		double i;

		newlist = new double [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}
		
		return newlist;
	}

	public static double lGetNumberRef(NumberArrayReference list, double i){
		return list.numberArray[(int)(i)];
	}

	public static void lRemoveNumberRef(NumberArrayReference list, double i){
		list.numberArray = lRemoveNumber(list.numberArray, i);
	}

	public static StringReference [] lAddString(StringReference [] list, StringReference a){
		StringReference [] newlist;
		double i;

		newlist = new StringReference [(int)(list.length + 1d)];

		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void lAddStringRef(StringArrayReference list, StringReference i){
		list.stringArray = lAddString(list.stringArray, i);
	}

	public static StringReference [] lRemoveString(StringReference [] list, double n){
		StringReference [] newlist;
		double i;

		newlist = new StringReference [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}
		
		return newlist;
	}

	public static StringReference lGetStringRef(StringArrayReference list, double i){
		return list.stringArray[(int)(i)];
	}

	public static void lRemoveStringRef(StringArrayReference list, double i){
		list.stringArray = lRemoveString(list.stringArray, i);
	}

	public static boolean [] lAddBoolean(boolean [] list, boolean a){
		boolean [] newlist;
		double i;

		newlist = new boolean [(int)(list.length + 1d)];
		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void lAddBooleanRef(BooleanArrayReference list, boolean i){
		list.booleanArray = lAddBoolean(list.booleanArray, i);
	}

	public static boolean [] lRemoveBoolean(boolean [] list, double n){
		boolean [] newlist;
		double i;

		newlist = new boolean [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}
		
		return newlist;
	}

	public static boolean lGetBooleanRef(BooleanArrayReference list, double i){
		return list.booleanArray[(int)(i)];
	}

	public static void lRemoveDecimalRef(BooleanArrayReference list, double i){
		list.booleanArray = lRemoveBoolean(list.booleanArray, i);
	}

	public static lLinkedListStrings lCreateLinkedListString(){
		lLinkedListStrings ll;

		ll = new lLinkedListStrings();
		ll.first = new lLinkedListNodeStrings();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void lLinkedListAddString(lLinkedListStrings ll, char [] value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new lLinkedListNodeStrings();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static StringReference [] lLinkedListStringsToArray(lLinkedListStrings ll){
		StringReference [] array;
		double length, i;
		lLinkedListNodeStrings node;

		node = ll.first;

		length = lLinkedListStringsLength(ll);

		array = new StringReference [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = new StringReference();
			array[(int)(i)].string = node.value;
			node = node.next;
		}

		return array;
	}

	public static double lLinkedListStringsLength(lLinkedListStrings ll){
		double l;
		lLinkedListNodeStrings node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void lFreeLinkedListString(lLinkedListStrings ll){
		lLinkedListNodeStrings node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static lLinkedListNumbers lCreateLinkedListNumbers(){
		lLinkedListNumbers ll;

		ll = new lLinkedListNumbers();
		ll.first = new lLinkedListNodeNumbers();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static lLinkedListNumbers [] lCreateLinkedListNumbersArray(double length){
		lLinkedListNumbers [] lls;
		double i;

		lls = new lLinkedListNumbers [(int)(length)];
		for(i = 0d; i < lls.length; i = i + 1d){
			lls[(int)(i)] = lCreateLinkedListNumbers();
		}

		return lls;
	}

	public static void lLinkedListAddNumber(lLinkedListNumbers ll, double value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new lLinkedListNodeNumbers();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static double lLinkedListNumbersLength(lLinkedListNumbers ll){
		double l;
		lLinkedListNodeNumbers node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static double lLinkedListNumbersIndex(lLinkedListNumbers ll, double index){
		double i;
		lLinkedListNodeNumbers node;

		node = ll.first;
		for(i = 0d; i < index; i = i + 1d){
			node = node.next;
		}

		return node.value;
	}

	public static void lLinkedListInsertNumber(lLinkedListNumbers ll, double index, double value){
		double i;
		lLinkedListNodeNumbers node, tmp;

		if(index == 0d){
			tmp = ll.first;
			ll.first = new lLinkedListNodeNumbers();
			ll.first.next = tmp;
			ll.first.value = value;
			ll.first.end = false;
		}else{
			node = ll.first;
			for(i = 0d; i < index - 1d; i = i + 1d){
				node = node.next;
			}

			tmp = node.next;
			node.next = new lLinkedListNodeNumbers();
			node.next.next = tmp;
			node.next.value = value;
			node.next.end = false;
		}
	}

	public static void lLinkedListSet(lLinkedListNumbers ll, double index, double value){
		double i;
		lLinkedListNodeNumbers node;

		node = ll.first;
		for(i = 0d; i < index; i = i + 1d){
			node = node.next;
		}

		node.next.value = value;
	}

	public static void lLinkedListRemoveNumber(lLinkedListNumbers ll, double index){
		double i;
		lLinkedListNodeNumbers node, prev;

		node = ll.first;
		prev = ll.first;

		for(i = 0d; i < index; i = i + 1d){
			prev = node;
			node = node.next;
		}

		if(index == 0d){
			ll.first = prev.next;
		}
		if(!prev.next.end){
			prev.next = prev.next.next;
		}
	}

	public static void lFreeLinkedListNumbers(lLinkedListNumbers ll){
		lLinkedListNodeNumbers node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static void lFreeLinkedListNumbersArray(lLinkedListNumbers [] lls){
		double i;

		for(i = 0d; i < lls.length; i = i + 1d){
			lFreeLinkedListNumbers(lls[(int)(i)]);
		}
		delete(lls);
	}

	public static double [] lLinkedListNumbersToArray(lLinkedListNumbers ll){
		double [] array;
		double length, i;
		lLinkedListNodeNumbers node;

		node = ll.first;

		length = lLinkedListNumbersLength(ll);

		array = new double [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static lLinkedListNumbers lArrayToLinkedListNumbers(double [] array){
		lLinkedListNumbers ll;
		double i;

		ll = lCreateLinkedListNumbers();

		for(i = 0d; i < array.length; i = i + 1d){
			lLinkedListAddNumber(ll, array[(int)(i)]);
		}

		return ll;
	}

	public static boolean lLinkedListNumbersEqual(lLinkedListNumbers a, lLinkedListNumbers b){
		boolean equal, done;
		lLinkedListNodeNumbers an, bn;

		an = a.first;
		bn = b.first;

		equal = true;
		done = false;
		for(; equal && !done; ){
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

	public static lLinkedListCharacters lCreateLinkedListCharacter(){
		lLinkedListCharacters ll;

		ll = new lLinkedListCharacters();
		ll.first = new lLinkedListNodeCharacters();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void lLinkedListAddCharacter(lLinkedListCharacters ll, char value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new lLinkedListNodeCharacters();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static char [] lLinkedListCharactersToArray(lLinkedListCharacters ll){
		char [] array;
		double length, i;
		lLinkedListNodeCharacters node;

		node = ll.first;

		length = lLinkedListCharactersLength(ll);

		array = new char [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static double lLinkedListCharactersLength(lLinkedListCharacters ll){
		double l;
		lLinkedListNodeCharacters node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void lFreeLinkedListCharacter(lLinkedListCharacters ll){
		lLinkedListNodeCharacters node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

	public static lDynamicArrayNumbers lCreateDynamicArrayNumbers(){
		lDynamicArrayNumbers da;

		da = new lDynamicArrayNumbers();
		da.array = new double [10];
		da.length = 0d;

		return da;
	}

	public static lDynamicArrayNumbers lCreateDynamicArrayNumbersWithInitialCapacity(double capacity){
		lDynamicArrayNumbers da;

		da = new lDynamicArrayNumbers();
		da.array = new double [(int)(capacity)];
		da.length = 0d;

		return da;
	}

	public static void lDynamicArrayAddNumber(lDynamicArrayNumbers da, double value){
		if(da.length == da.array.length){
			lDynamicArrayNumbersIncreaseSize(da);
		}

		da.array[(int)(da.length)] = value;
		da.length = da.length + 1d;
	}

	public static void lDynamicArrayNumbersIncreaseSize(lDynamicArrayNumbers da){
		double newLength, i;
		double [] newArray;

		newLength = (double)round(da.array.length*3d/2d);
		newArray = new double [(int)(newLength)];

		for(i = 0d; i < da.array.length; i = i + 1d){
			newArray[(int)(i)] = da.array[(int)(i)];
		}

		delete(da.array);

		da.array = newArray;
	}

	public static boolean lDynamicArrayNumbersDecreaseSizeNecessary(lDynamicArrayNumbers da){
		boolean needsDecrease;

		needsDecrease = false;

		if(da.length > 10d){
			needsDecrease = da.length <= (double)round(da.array.length*2d/3d);
		}

		return needsDecrease;
	}

	public static void lDynamicArrayNumbersDecreaseSize(lDynamicArrayNumbers da){
		double newLength, i;
		double [] newArray;

		newLength = (double)round(da.array.length*2d/3d);
		newArray = new double [(int)(newLength)];

		for(i = 0d; i < da.array.length; i = i + 1d){
			newArray[(int)(i)] = da.array[(int)(i)];
		}

		delete(da.array);

		da.array = newArray;
	}

	public static double lDynamicArrayNumbersIndex(lDynamicArrayNumbers da, double index){
		return da.array[(int)(index)];
	}

	public static double lDynamicArrayNumbersLength(lDynamicArrayNumbers da){
		return da.length;
	}

	public static void lDynamicArrayInsertNumber(lDynamicArrayNumbers da, double index, double value){
		double i;

		if(da.length == da.array.length){
			lDynamicArrayNumbersIncreaseSize(da);
		}

		for(i = da.length; i > index; i = i - 1d){
			da.array[(int)(i)] = da.array[(int)(i - 1d)];
		}

		da.array[(int)(index)] = value;

		da.length = da.length + 1d;
	}

	public static void lDynamicArraySet(lDynamicArrayNumbers da, double index, double value){
		da.array[(int)(index)] = value;
	}

	public static void lDynamicArrayRemoveNumber(lDynamicArrayNumbers da, double index){
		double i;

		for(i = index; i < da.length - 1d; i = i + 1d){
			da.array[(int)(i)] = da.array[(int)(i + 1d)];
		}

		da.length = da.length - 1d;

		if(lDynamicArrayNumbersDecreaseSizeNecessary(da)){
			lDynamicArrayNumbersDecreaseSize(da);
		}
	}

	public static void lFreeDynamicArrayNumbers(lDynamicArrayNumbers da){
		delete(da.array);
		delete(da);
	}

	public static double [] lDynamicArrayNumbersToArray(lDynamicArrayNumbers da){
		double [] array;
		double i;

		array = new double [(int)(da.length)];

		for(i = 0d; i < da.length; i = i + 1d){
			array[(int)(i)] = da.array[(int)(i)];
		}

		return array;
	}

	public static lDynamicArrayNumbers lArrayToDynamicArrayNumbersWithOptimalSize(double [] array){
		lDynamicArrayNumbers da;
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
		c = array.length;
		n = (log(c) - 1d)/log(3d/2d);
		newCapacity = floor(n) + 1d;

		da = lCreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

		for(i = 0d; i < array.length; i = i + 1d){
			da.array[(int)(i)] = array[(int)(i)];
		}

		return da;
	}

	public static lDynamicArrayNumbers lArrayToDynamicArrayNumbers(double [] array){
		lDynamicArrayNumbers da;

		da = new lDynamicArrayNumbers();
		da.array = CopyNumberArray(array);
		da.length = array.length;

		return da;
	}

	public static boolean lDynamicArrayNumbersEqual(lDynamicArrayNumbers a, lDynamicArrayNumbers b){
		boolean equal;
		double i;

		equal = true;
		if(a.length == b.length){
			for(i = 0d; i < a.length && equal; i = i + 1d){
				if(a.array[(int)(i)] != b.array[(int)(i)]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}

	public static lLinkedListNumbers lDynamicArrayNumbersToLinkedList(lDynamicArrayNumbers da){
		lLinkedListNumbers ll;
		double i;

		ll = lCreateLinkedListNumbers();

		for(i = 0d; i < da.length; i = i + 1d){
			lLinkedListAddNumber(ll, da.array[(int)(i)]);
		}

		return ll;
	}

	public static lDynamicArrayNumbers lLinkedListToDynamicArrayNumbers(lLinkedListNumbers ll){
		lDynamicArrayNumbers da;
		double i;
		lLinkedListNodeNumbers node;

		node = ll.first;

		da = new lDynamicArrayNumbers();
		da.length = lLinkedListNumbersLength(ll);

		da.array = new double [(int)(da.length)];

		for(i = 0d; i < da.length; i = i + 1d){
			da.array[(int)(i)] = node.value;
			node = node.next;
		}

		return da;
	}

	public static char [] lAddCharacter(char [] list, char a){
		char [] newlist;
		double i;

		newlist = new char [(int)(list.length + 1d)];
		for(i = 0d; i < list.length; i = i + 1d){
			newlist[(int)(i)] = list[(int)(i)];
		}
		newlist[(int)(list.length)] = a;
		
		delete(list);
		
		return newlist;
	}

	public static void lAddCharacterRef(StringReference list, char i){
		list.string = lAddCharacter(list.string, i);
	}

	public static char [] lRemoveCharacter(char [] list, double n){
		char [] newlist;
		double i;

		newlist = new char [(int)(list.length - 1d)];

		if(n >= 0d && n < list.length){
			for(i = 0d; i < list.length; i = i + 1d){
				if(i < n){
					newlist[(int)(i)] = list[(int)(i)];
				}
				if(i > n){
					newlist[(int)(i - 1d)] = list[(int)(i)];
				}
			}

			delete(list);
		}else{
			delete(newlist);
		}

		return newlist;
	}

	public static char lGetCharacterRef(StringReference list, double i){
		return list.string[(int)(i)];
	}

	public static void lRemoveCharacterRef(StringReference list, double i){
		list.string = lRemoveCharacter(list.string, i);
	}

	public static double Negate(double x){
		return -x;
	}

	public static double Positive(double x){
		return +x;
	}

	public static double Factorial(double x){
		double i, f;

		f = 1d;

		for(i = 2d; i <= x; i = i + 1d){
			f = f*i;
		}

		return f;
	}

	public static double Round(double x){
		return floor(x + 0.5);
	}

	public static double BankersRound(double x){
		double r;

		if(Absolute(x - Truncate(x)) == 0.5){
			if(!DivisibleBy(Round(x), 2d)){
				r = Round(x) - 1d;
			}else{
				r = Round(x);
			}
		}else{
			r = Round(x);
		}

		return r;
	}

	public static double Ceil(double x){
		return ceil(x);
	}

	public static double Floor(double x){
		return floor(x);
	}

	public static double Truncate(double x){
		double t;

		if(x >= 0d){
			t = floor(x);
		}else{
			t = ceil(x);
		}

		return t;
	}

	public static double Absolute(double x){
		return abs(x);
	}

	public static double Logarithm(double x){
		return log10(x);
	}

	public static double NaturalLogarithm(double x){
		return log(x);
	}

	public static double Sin(double x){
		return sin(x);
	}

	public static double Cos(double x){
		return cos(x);
	}

	public static double Tan(double x){
		return tan(x);
	}

	public static double Asin(double x){
		return asin(x);
	}

	public static double Acos(double x){
		return acos(x);
	}

	public static double Atan(double x){
		return atan(x);
	}

	public static double Atan2(double y, double x){
		double a;

		/* Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors.*/
		a = 0d;

		if(x > 0d){
			a = Atan(y/x);
		}else if(x < 0d && y >= 0d){
			a = Atan(y/x) + PI;
		}else if(x < 0d && y < 0d){
			a = Atan(y/x) - PI;
		}else if(x == 0d && y > 0d){
			a = PI/2d;
		}else if(x == 0d && y < 0d){
			a = -PI/2d;
		}

		return a;
	}

	public static double Squareroot(double x){
		return sqrt(x);
	}

	public static double Exp(double x){
		return exp(x);
	}

	public static boolean DivisibleBy(double a, double b){
		return ((a%b) == 0d);
	}

	public static double Combinations(double n, double k){
		double i, j, c;

		c = 1d;
		j = 1d;
		i = n - k + 1d;

		for(; i <= n; ){
			c = c*i;
			c = c/j;

			i = i + 1d;
			j = j + 1d;
		}

		return c;
	}

	public static double Permutations(double n, double k){
		double i, c;

		c = 1d;

		for(i = n - k + 1d; i <= n; i = i + 1d){
			c = c*i;
		}

		return c;
	}

	public static boolean EpsilonCompare(double a, double b, double epsilon){
		return abs(a - b) < epsilon;
	}

	public static double GreatestCommonDivisor(double a, double b){
		double t;

		for(; b != 0d; ){
			t = b;
			b = a%b;
			a = t;
		}

		return a;
	}

	public static double GCDWithSubtraction(double a, double b){
		double g;

		if(a == 0d){
			g = b;
		}else{
			for(; b != 0d; ){
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

	public static boolean IsInteger(double a){
		return (a - floor(a)) == 0d;
	}

	public static boolean GreatestCommonDivisorWithCheck(double a, double b, NumberReference gcdReference){
		boolean success;
		double gcd;

		if(IsInteger(a) && IsInteger(b)){
			gcd = GreatestCommonDivisor(a, b);
			gcdReference.numberValue = gcd;
			success = true;
		}else{
			success = false;
		}

		return success;
	}

	public static double LeastCommonMultiple(double a, double b){
		double lcm;

		if(a > 0d && b > 0d){
			lcm = abs(a*b)/GreatestCommonDivisor(a, b);
		}else{
			lcm = 0d;
		}

		return lcm;
	}

	public static double Sign(double a){
		double s;

		if(a > 0d){
			s = 1d;
		}else if(a < 0d){
			s = -1d;
		}else{
			s = 0d;
		}

		return s;
	}

	public static double Max(double a, double b){
		return max(a, b);
	}

	public static double Min(double a, double b){
		return min(a, b);
	}

	public static double Power(double a, double b){
		return pow(a, b);
	}

	public static double Gamma(double x){
		return LanczosApproximation(x);
	}

	public static double LogGamma(double x){
		return log(Gamma(x));
	}

	public static double LanczosApproximation(double z){
		double [] p;
		double i, y, t, x;

		p = new double [8];
		p[0] = 676.5203681218851;
		p[1] = -1259.1392167224028;
		p[2] = 771.32342877765313;
		p[3] = -176.61502916214059;
		p[4] = 12.507343278686905;
		p[5] = -0.13857109526572012;
		p[6] = 9.9843695780195716e-6;
		p[7] = 1.5056327351493116e-7;

		if(z < 0.5){
			y = PI/(sin(PI*z)*LanczosApproximation(1d - z));
		}else{
			z = z - 1d;
			x = 0.99999999999980993;
			for(i = 0d; i < p.length; i = i + 1d){
				x = x + p[(int)(i)]/(z + i + 1d);
			}
			t = z + p.length - 0.5;
			y = sqrt(2d*PI)*pow(t, z + 0.5)*exp(-t)*x;
		}

		return y;
	}

	public static double Beta(double x, double y){
		return Gamma(x)*Gamma(y)/Gamma(x + y);
	}

	public static double Sinh(double x){
		return (exp(x) - exp(-x))/2d;
	}

	public static double Cosh(double x){
		return (exp(x) + exp(-x))/2d;
	}

	public static double Tanh(double x){
		return Sinh(x)/Cosh(x);
	}

	public static double Cot(double x){
		return 1d/tan(x);
	}

	public static double Sec(double x){
		return 1d/cos(x);
	}

	public static double Csc(double x){
		return 1d/sin(x);
	}

	public static double Coth(double x){
		return Cosh(x)/Sinh(x);
	}

	public static double Sech(double x){
		return 1d/Cosh(x);
	}

	public static double Csch(double x){
		return 1d/Sinh(x);
	}

	public static double Error(double x){
		double y, t, tau, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;

		if(x == 0d){
			y = 0d;
		}else if(x < 0d){
			y = -Error(-x);
		}else{
			c1 = -1.26551223;
			c2 = +1.00002368;
			c3 = +0.37409196;
			c4 = +0.09678418;
			c5 = -0.18628806;
			c6 = +0.27886807;
			c7 = -1.13520398;
			c8 = +1.48851587;
			c9 = -0.82215223;
			c10 = +0.17087277;

			t = 1d/(1d + 0.5*abs(x));

			tau = t*exp(-pow(x, 2d) + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))));

			y = 1d - tau;
		}

		return y;
	}

	public static double ErrorInverse(double x){
		double y, a, t;

		a = (8d*(PI - 3d))/(3d*PI*(4d - PI));

		t = 2d/(PI*a) + log(1d - pow(x, 2d))/2d;
		y = Sign(x)*sqrt(sqrt(pow(t, 2d) - log(1d - pow(x, 2d))/a) - t);

		return y;
	}

	public static double FallingFactorial(double x, double n){
		double k, y;

		y = 1d;

		for(k = 0d; k <= n - 1d; k = k + 1d){
			y = y*(x - k);
		}

		return y;
	}

	public static double RisingFactorial(double x, double n){
		double k, y;

		y = 1d;

		for(k = 0d; k <= n - 1d; k = k + 1d){
			y = y*(x + k);
		}

		return y;
	}

	public static double Hypergeometric(double a, double b, double c, double z, double maxIterations, double precision){
		double y;

		if(abs(z) >= 0.5){
			y = pow(1d - z, -a)*HypergeometricDirect(a, c - b, c, z/(z - 1d), maxIterations, precision);
		}else{
			y = HypergeometricDirect(a, b, c, z, maxIterations, precision);
		}

		return y;
	}

	public static double HypergeometricDirect(double a, double b, double c, double z, double maxIterations, double precision){
		double y, yp, n;
		boolean done;

		y = 0d;
		done = false;

		for(n = 0d; n < maxIterations && !done; n = n + 1d){
			yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*pow(z, n)/Factorial(n);
			if(abs(yp) < precision){
				done = true;
			}
			y = y + yp;
		}

		return y;
	}

	public static double BernouilliNumber(double n){
		return AkiyamaTanigawaAlgorithm(n);
	}

	public static double AkiyamaTanigawaAlgorithm(double n){
		double m, j, B;
		double [] A;

		A = new double [(int)(n + 1d)];

		for(m = 0d; m <= n; m = m + 1d){
			A[(int)(m)] = 1d/(m + 1d);
			for(j = m; j >= 1d; j = j - 1d){
				A[(int)(j - 1d)] = j*(A[(int)(j - 1d)] - A[(int)(j)]);
			}
		}

		B = A[0];

		delete(A);

		return B;
	}

	public static char charToLowerCase(char character){
		char toReturn;

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

	public static char charToUpperCase(char character){
		char toReturn;

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

	public static boolean charIsUpperCase(char character){
		boolean isUpper;

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

	public static boolean charIsLowerCase(char character){
		boolean isLower;

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

	public static boolean charIsLetter(char character){
		return charIsUpperCase(character) || charIsLowerCase(character);
	}

	public static boolean charIsNumber(char character){
		boolean isNumberx;

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

	public static boolean charIsWhiteSpace(char character){
		boolean isWhiteSpacex;

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

	public static boolean charIsSymbol(char character){
		boolean isSymbolx;

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

	public static boolean charCharacterIsBefore(char a, char b){
		double ad, bd;

		ad = a;
		bd = b;

		return ad < bd;
	}

  public static void delete(Object object){
    // Java has garbage collection.
  }
}

