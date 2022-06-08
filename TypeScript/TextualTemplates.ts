
// Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

export class LinkedListNodeNodes{
	end : boolean;
	value : Nodex;
	next : LinkedListNodeNodes;
}
export class LinkedListNodes{
	first : LinkedListNodeNodes;
	last : LinkedListNodeNodes;
}
export class Nodex{
	typex : string [];
	p1 : string [];
	p2 : string [];
	block1 : Nodex;
	hasElseBlock : boolean;
	block2 : Nodex;
	nodes : LinkedListNodes;
}
export class BooleanArrayReference{
	booleanArray : boolean [];
}
export class BooleanReference{
	booleanValue : boolean;
}
export class CharacterReference{
	characterValue : string;
}
export class NumberArrayReference{
	numberArray : number [];
}
export class NumberReference{
	numberValue : number;
}
export class StringArrayReference{
	stringArray : StringReference [];
}
export class StringReference{
	stringx : string [];
}
export class ElementArrayReference{
	array : Elementx [];
}
export class LinkedListElements{
	first : LinkedListNodeElements;
	last : LinkedListNodeElements;
}
export class LinkedListNodeElements{
	end : boolean;
	value : Elementx;
	next : LinkedListNodeElements;
}
export class Elementx{
	typex : string [];
	object : StringElementMap;
	array : Elementx [];
	stringx : string [];
	numberx : number;
	booleanValue : boolean;
}
export class ElementReference{
	element : Elementx;
}
export class ElementType{
	name : string [];
}
export class StringElementMap{
	stringListRef : StringArrayReference;
	elementListRef : ElementArrayReference;
}
export class DynamicArrayCharacters{
	array : string [];
	length : number;
}
export class LinkedListNodeStrings{
	end : boolean;
	value : string [];
	next : LinkedListNodeStrings;
}
export class LinkedListStrings{
	first : LinkedListNodeStrings;
	last : LinkedListNodeStrings;
}
export class LinkedListNodeNumbers{
	next : LinkedListNodeNumbers;
	end : boolean;
	value : number;
}
export class LinkedListNumbers{
	first : LinkedListNodeNumbers;
	last : LinkedListNodeNumbers;
}
export class LinkedListCharacters{
	first : LinkedListNodeCharacters;
	last : LinkedListNodeCharacters;
}
export class LinkedListNodeCharacters{
	end : boolean;
	value : string;
	next : LinkedListNodeCharacters;
}
export class DynamicArrayNumbers{
	array : number [];
	length : number;
}
	export function CreateLinkedListNodes() : LinkedListNodes{
		var ll : LinkedListNodes;

		ll = new LinkedListNodes();
		ll.first = new LinkedListNodeNodes();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}


	export function LinkedListAddNode(ll : LinkedListNodes, value : Nodex) : void{
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeNodes();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}


	export function LinkedListNodesToArray(ll : LinkedListNodes) : Nodex []{
		var array : Nodex [];
		var length : number, i : number;
		var node : LinkedListNodeNodes;

		node = ll.first;

		length = LinkedListNodesLength(ll);

		array = new Array<Nodex>(length);

		for(i = 0; i < length; i = i + 1){
			array[i] = node.value;
			node = node.next;
		}

		return array;
	}


	export function LinkedListNodesLength(ll : LinkedListNodes) : number{
		var l : number;
		var node : LinkedListNodeNodes;

		l = 0;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1;
		}

		return l;
	}


	export function FreeLinkedListNode(ll : LinkedListNodes) : void{
		var node : LinkedListNodeNodes, prev : LinkedListNodeNodes;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			prev = undefined;
		}

		node = undefined;
	}


	export function IsValidTemplate(template : string []) : boolean{
		var errorMessage : StringReference;
		var success : boolean;
		var tokens : LinkedListStrings;
		var ts : StringReference [];
		var root : Nodex;

		tokens = CreateLinkedListString();
		errorMessage = new StringReference();

		success = GenerateTokensFromTemplate(template, tokens, errorMessage);

		if(success){
			root = new Nodex();

			ts = LinkedListStringsToArray(tokens);

			success = ParseTemplate(ts, root, errorMessage);
		}

		if(!success){
			FreeStringReference(errorMessage);
		}

		return success;
	}


	export function GenerateTokensFromTemplate(template : string [], tokens : LinkedListStrings, errorMessage : StringReference) : boolean{
		var success : boolean, found : boolean;
		var i : number, j : number, l : number;
		var pc : string, c : string;
		var cs : string [], a : string [];
		var da : DynamicArrayCharacters;

		success = true;
		cs = new Array<string>(4);

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
						/* Find end.*/
						found = false;
						for(l = 0; i + l < template.length && !found; l = l + 1){
							if(template[i + l] == '}'){
								found = true;
							}
						}

						if(found){
							if(da.length > 0){
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
							errorMessage.stringx = "Template command found, but not ended properly.".split('');
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

		if(da.length > 0){
			a = DynamicArrayCharactersToArray(da);
			LinkedListAddString(tokens, a);
		}

		FreeDynamicArrayCharacters(da);

		return success;
	}


	export function GenerateDocument(template : string [], json : string [], document : StringReference, errorMessage : StringReference) : boolean{
		var data : ElementReference;
		var errorMessages : StringArrayReference;
		var success : boolean;

		data = new ElementReference();
		errorMessages = new StringArrayReference();

		success = ReadJSON(json, data, errorMessages);

		if(success){
			success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage);
		}else{
			errorMessage.stringx = JoinStringsWithSeparator(errorMessages.stringArray, ", ".split(''));
			FreeStringArrayReference(errorMessages);
		}

		return success;
	}


	export function GenerateDocumentBasedOnElement(template : string [], data : Elementx, document : StringReference, errorMessage : StringReference) : boolean{
		var ll : LinkedListCharacters;
		var success : boolean;
		var tokens : LinkedListStrings;
		var ts : StringReference [];
		var root : Nodex;

		tokens = CreateLinkedListString();

		success = GenerateTokensFromTemplate(template, tokens, errorMessage);

		if(success){
			root = new Nodex();

			ts = LinkedListStringsToArray(tokens);
			FreeLinkedListString(tokens);

			success = ParseTemplate(ts, root, errorMessage);

			if(success){
				ll = CreateLinkedListCharacter();

				success = GenerateDocumentFromBlock(root, data, ll, errorMessage);

				if(success){
					document.stringx = LinkedListCharactersToArray(ll);
					FreeLinkedListCharacter(ll);
				}
			}
		}

		return success;
	}


	export function GenerateDocumentFromBlock(root : Nodex, data : Elementx, ll : LinkedListCharacters, errorMessage : StringReference) : boolean{
		var n : LinkedListNodeNodes;
		var success : boolean;

		n = root.nodes.first;
		success = true;

		for(; !n.end && success; ){
			success = GenerateDocumentFromNode(n.value, data, ll, errorMessage);
			n = n.next;
		}

		return success;
	}


	export function GenerateDocumentFromNode(n : Nodex, data : Elementx, ll : LinkedListCharacters, errorMessage : StringReference) : boolean{
		var success : boolean;
		var found : BooleanReference;
		var valueString : string [];
		var value : Elementx;

		success = true;
		found = new BooleanReference();

		if(StringsEqual(n.typex, "block".split(''))){
			success = GenerateDocumentFromBlock(n, data, ll, errorMessage);
		}else if(StringsEqual(n.typex, "use".split(''))){
			success = GenerateDocumentFromUse(n, data, ll, errorMessage);
		}else if(StringsEqual(n.typex, "if".split(''))){
			success = GenerateDocumentFromIf(n, data, ll, errorMessage);
		}else if(StringsEqual(n.typex, "foreach".split(''))){
			success = GenerateDocumentFromForeach(n, data, ll, errorMessage);
		}else if(StringsEqual(n.typex, "text".split(''))){
			success = true;
			LinkedListCharactersAddString(ll, n.p1);
		}else if(StringsEqual(n.typex, "print".split(''))){
			if(StringsEqual(data.typex, "object".split(''))){
				value = GetObjectValueWithCheck(data.object, n.p1, found);

				if(found.booleanValue){
					if(StringsEqual(value.typex, "string".split(''))){
						valueString = value.stringx;
					}else{
						valueString = WriteJSON(value);
					}
					LinkedListCharactersAddString(ll, valueString);
				}else{
					success = false;
					errorMessage.stringx = "Key for printing not found in JSON object: ".split('');
					errorMessage.stringx = ConcatenateString(errorMessage.stringx, n.p1);
				}
			}else{
				success = false;
				errorMessage.stringx = "Data structure for print command is not a JSON object.".split('');
			}
		}

		return success;
	}


	export function GenerateDocumentFromUse(n : Nodex, data : Elementx, ll : LinkedListCharacters, errorMessage : StringReference) : boolean{
		var success : boolean;
		var found : BooleanReference;
		var value : Elementx;

		found = new BooleanReference();

		if(StringsEqual(data.typex, "object".split(''))){
			value = GetObjectValueWithCheck(data.object, n.p1, found);

			if(found.booleanValue){
				success = GenerateDocumentFromNode(n.block1, value, ll, errorMessage);
			}else{
				success = false;
				errorMessage.stringx = "Key for use not found in JSON object.".split('');
			}
		}else{
			success = false;
			errorMessage.stringx = "Data structure for use command is not a JSON object.".split('');
		}

		return success;
	}


	export function GenerateDocumentFromIf(n : Nodex, data : Elementx, ll : LinkedListCharacters, errorMessage : StringReference) : boolean{
		var success : boolean;
		var found : BooleanReference;
		var value : Elementx;

		success = true;
		found = new BooleanReference();

		if(StringsEqual(data.typex, "object".split(''))){
			value = GetObjectValueWithCheck(data.object, n.p1, found);

			if(found.booleanValue){
				if(StringsEqual(value.typex, "boolean".split(''))){
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
					errorMessage.stringx = "Value for if not boolean.".split('');
				}
			}else{
				success = false;
				errorMessage.stringx = "Key for if not found in JSON object: ".split('');
				errorMessage.stringx = ConcatenateString(errorMessage.stringx, n.p1);
			}
		}else{
			success = false;
			errorMessage.stringx = "Data structure for if command is not a JSON object.".split('');
		}

		return success;
	}


	export function GenerateDocumentFromForeach(n : Nodex, data : Elementx, ll : LinkedListCharacters, errorMessage : StringReference) : boolean{
		var success : boolean;
		var found : BooleanReference;
		var value : Elementx, loopVar : Elementx, arrayElement : Elementx;
		var i : number;

		success = true;
		found = new BooleanReference();
		loopVar = CreateObjectElement(0);

		PutStringElementMap(loopVar.object, n.p1, new Elementx());

		if(StringsEqual(data.typex, "object".split(''))){
			value = GetObjectValueWithCheck(data.object, n.p2, found);

			if(found.booleanValue){
				if(StringsEqual(value.typex, "array".split(''))){

					for(i = 0; i < value.array.length; i = i + 1){
						arrayElement = value.array[i];
						if(StringsEqual(arrayElement.typex, "object".split(''))){
							success = GenerateDocumentFromBlock(n.block1, arrayElement, ll, errorMessage);
						}else{
							SetStringElementMap(loopVar.object, 0, n.p1, arrayElement);
							success = GenerateDocumentFromBlock(n.block1, loopVar, ll, errorMessage);
						}
					}
				}else{
					success = false;
					errorMessage.stringx = "Value for foreach is not an array.".split('');
				}
			}else{
				success = false;
				errorMessage.stringx = "Key for foreach not found in JSON object: ".split('');
				errorMessage.stringx = ConcatenateString(errorMessage.stringx, n.p2);
			}
		}else{
			success = false;
			errorMessage.stringx = "Data structure for foreach command is not a JSON object.".split('');
		}

		return success;
	}


	export function ParseTemplate(tokens : StringReference [], node : Nodex, errorMessage : StringReference) : boolean{
		var success : boolean;
		var position : NumberReference;

		position = CreateNumberReference(0);
		success = ParseTemplateBlock(tokens, position, node, errorMessage);

		if(success){
			if(position.numberValue != tokens.length){
				success = false;
				errorMessage.stringx = "Unexpected token at the end of template.".split('');
			}
		}

		position = undefined;

		return success;
	}


	export function ParseTemplateBlock(tokens : StringReference [], position : NumberReference, node : Nodex, errorMessage : StringReference) : boolean{
		var success : boolean, done : boolean;
		var tn : Nodex, nb : Nodex;

		success = true;
		done = false;

		node.typex = "block".split('');
		node.nodes = CreateLinkedListNodes();

		for(; position.numberValue < tokens.length && success && !done; ){
			tn = new Nodex();
			success = ParseNodeString(tokens[position.numberValue].stringx, tn, errorMessage);

			if(success){
				if(StringsEqual(tn.typex, "text".split('')) || StringsEqual(tn.typex, "print".split(''))){
					LinkedListAddNode(node.nodes, tn);
					position.numberValue = position.numberValue + 1;
				}else if(StringsEqual(tn.typex, "use".split(''))){
					nb = new Nodex();
					success = ParseUseBlock(tokens, position, nb, errorMessage);
					LinkedListAddNode(node.nodes, nb);
				}else if(StringsEqual(tn.typex, "if".split(''))){
					nb = new Nodex();
					success = ParseIfBlock(tokens, position, nb, errorMessage);
					LinkedListAddNode(node.nodes, nb);
				}else if(StringsEqual(tn.typex, "foreach".split(''))){
					nb = new Nodex();
					success = ParseForeachBlock(tokens, position, nb, errorMessage);
					LinkedListAddNode(node.nodes, nb);
				}else{
					done = true;
				}
			}
		}

		return success;
	}


	export function ParseUseBlock(tokens : StringReference [], position : NumberReference, useBlock : Nodex, errorMessage : StringReference) : boolean{
		var success : boolean;
		var n : Nodex;

		n = new Nodex();
		ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

		useBlock.typex = CopyString(n.typex);
		useBlock.p1 = CopyString(n.p1);
		useBlock.block1 = new Nodex();

		position.numberValue = position.numberValue + 1;

		success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage);

		if(success){
			if(position.numberValue < tokens.length){
				n = new Nodex();
				ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

				if(StringsEqual(n.typex, "end".split(''))){
					success = true;
					position.numberValue = position.numberValue + 1;
				}else{
					success = false;
					errorMessage.stringx = "End block expected at the end of use block.".split('');
				}
			}else{
				success = false;
				errorMessage.stringx = "End block expected at the end of use block.".split('');
			}
		}

		return success;
	}


	export function ParseIfBlock(tokens : StringReference [], position : NumberReference, ifBlock : Nodex, errorMessage : StringReference) : boolean{
		var success : boolean;
		var n : Nodex;

		n = new Nodex();
		ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

		ifBlock.typex = CopyString(n.typex);
		ifBlock.p1 = CopyString(n.p1);
		ifBlock.block1 = new Nodex();
		ifBlock.hasElseBlock = false;

		position.numberValue = position.numberValue + 1;

		success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage);

		if(success){
			if(position.numberValue < tokens.length){
				n = new Nodex();
				ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

				if(StringsEqual(n.typex, "end".split(''))){
					success = true;
					position.numberValue = position.numberValue + 1;
				}else if(StringsEqual(n.typex, "else".split(''))){
					position.numberValue = position.numberValue + 1;
					ifBlock.hasElseBlock = true;
					ifBlock.block2 = new Nodex();
					success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage);

					if(success){
						if(position.numberValue < tokens.length){
							n = new Nodex();
							ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

							if(StringsEqual(n.typex, "end".split(''))){
								success = true;
								position.numberValue = position.numberValue + 1;
							}else{
								success = false;
								errorMessage.stringx = "End block expected at the end of else block.".split('');
							}
						}else{
							success = false;
							errorMessage.stringx = "End block expected at the end of else block.".split('');
						}
					}
				}else{
					success = false;
					errorMessage.stringx = "End or else block expected at the end of if block.".split('');
				}
			}else{
				success = false;
				errorMessage.stringx = "End or else block expected at the end of if block.".split('');
			}
		}

		return success;
	}


	export function ParseForeachBlock(tokens : StringReference [], position : NumberReference, foreachBlock : Nodex, errorMessage : StringReference) : boolean{
		var success : boolean;
		var n : Nodex;

		n = new Nodex();
		ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

		foreachBlock.typex = CopyString(n.typex);
		foreachBlock.p1 = CopyString(n.p1);
		foreachBlock.p2 = CopyString(n.p2);
		foreachBlock.block1 = new Nodex();

		position.numberValue = position.numberValue + 1;

		success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage);

		if(success){
			if(position.numberValue < tokens.length){
				n = new Nodex();
				ParseNodeString(tokens[position.numberValue].stringx, n, errorMessage);

				if(StringsEqual(n.typex, "end".split(''))){
					success = true;
					position.numberValue = position.numberValue + 1;
				}else{
					success = false;
					errorMessage.stringx = "End block expected at the end of for each block.".split('');
				}
			}else{
				success = false;
				errorMessage.stringx = "End block expected at the end of foreach block.".split('');
			}
		}

		return success;
	}


	export function ParseNodeString(token : string [], node : Nodex, errorMessage : StringReference) : boolean{
		var command : string [];
		var success : boolean, isText : boolean;
		var parts : StringReference [];

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
				if(StringsEqual(parts[0].stringx, "use".split(''))){
					if(parts.length == 2){
						node.typex = CopyString(parts[0].stringx);
						node.p1 = CopyString(parts[1].stringx);
					}else{
						success = false;
						errorMessage.stringx = "The use command takes one parameter.".split('');
					}
				}else if(StringsEqual(parts[0].stringx, "end".split(''))){
					if(parts.length == 1){
						node.typex = CopyString(parts[0].stringx);
					}else{
						success = false;
						errorMessage.stringx = "The end command takes no parameters.".split('');
					}
				}else if(StringsEqual(parts[0].stringx, "print".split(''))){
					if(parts.length == 2){
						node.typex = CopyString(parts[0].stringx);
						node.p1 = CopyString(parts[1].stringx);
					}else{
						success = false;
						errorMessage.stringx = "The print command takes one parameter.".split('');
					}
				}else if(StringsEqual(parts[0].stringx, "foreach".split(''))){
					if(parts.length == 4){
						if(StringsEqual(parts[2].stringx, "in".split(''))){
							node.typex = CopyString(parts[0].stringx);
							node.p1 = CopyString(parts[1].stringx);
							node.p2 = CopyString(parts[3].stringx);
						}else{
							success = false;
							errorMessage.stringx = "The foreach command must have \"in\" after the first parameter.".split('');
						}
					}else{
						success = false;
						errorMessage.stringx = "The foreach command takes three parameters.".split('');
					}
				}else if(StringsEqual(parts[0].stringx, "if".split(''))){
					if(parts.length == 2){
						node.typex = CopyString(parts[0].stringx);
						node.p1 = CopyString(parts[1].stringx);
					}else{
						success = false;
						errorMessage.stringx = "The if command takes one parameter.".split('');
					}
				}else if(StringsEqual(parts[0].stringx, "else".split(''))){
					if(parts.length == 1){
						node.typex = CopyString(parts[0].stringx);
					}else{
						success = false;
						errorMessage.stringx = "The else command takes no parameters.".split('');
					}
				}else{
					isText = true;
				}
			}else{
				isText = true;
			}
		}

		if(isText){
			node.typex = "text".split('');
			node.p1 = ReplaceString(token, "\\{print ".split(''), "{print ".split(''));
			node.p1 = ReplaceString(node.p1, "\\{use ".split(''), "{use ".split(''));
			node.p1 = ReplaceString(node.p1, "\\{if ".split(''), "{if ".split(''));
			node.p1 = ReplaceString(node.p1, "\\{end}".split(''), "{end}".split(''));
			node.p1 = ReplaceString(node.p1, "\\{foreach ".split(''), "{foreach ".split(''));
			node.p1 = ReplaceString(node.p1, "\\{else}".split(''), "{else}".split(''));
		}

		return success;
	}


	export function test() : number{
		var failures : NumberReference;

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


	export function testGenerateDocument8(failures : NumberReference) : void{
		var document : StringReference, errorMessage : StringReference;
		var success : boolean;
		var template : string [], json : string [];

		document = new StringReference();
		errorMessage = new StringReference();

		template = "This is a test: {print b} {foreach x in a}{print x}{end}.".split('');
		json = "{\"a\": [1, 2, 3], \"b\": 4}".split('');
		success = GenerateDocument(template, json, document, errorMessage);

		if(success){
			AssertStringEquals("This is a test: 4 123.".split(''), document.stringx, failures);
		}

		AssertTrue(success, failures);
	}


	export function testTokenGeneration(failures : NumberReference) : void{
		var template : string [];
		var tokens : LinkedListStrings;
		var success : boolean;
		var errorMessage : StringReference;

		errorMessage = new StringReference();

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


	export function testGenerateDocument1(failures : NumberReference) : void{
		AssertTemplateResult("This is a template, this is a value: {print a}.".split(''), "{\"c\": 5, \"a\": 6}".split(''), "This is a template, this is a value: 6.".split(''), failures);
	}


	export function testGenerateDocument2(failures : NumberReference) : void{
		AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}".split(''), "This is a template, this is a value: 6 1 2.".split(''), failures);
	}


	export function testGenerateDocument3(failures : NumberReference) : void{
		AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}".split(''), "This is a template, this is a value: 6 1 23 416.".split(''), failures);
	}


	export function testGenerateDocument4(failures : NumberReference) : void{
		AssertTemplateResult("T: {if a}a{end}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}".split(''), "T: a.".split(''), failures);

		AssertTemplateResult("T: {if a}a{else}b{end}.".split(''), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}".split(''), "T: b.".split(''), failures);
	}


	export function testGenerateDocument5(failures : NumberReference) : void{
		AssertTemplateResult("T: {foreach a in b}{print a}{end}.".split(''), "{\"b\": [1, 2, 3, 4]}".split(''), "T: 1234.".split(''), failures);
	}


	export function testGenerateDocument6(failures : NumberReference) : void{
		AssertTemplateResult("{test}\\{print a}.".split(''), "{\"c\": 5, \"a\": 6}".split(''), "{test}{print a}.".split(''), failures);
	}


	export function testGenerateDocument7(failures : NumberReference) : void{
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


	export function AssertTemplateResult(template : string [], json : string [], result : string [], failures : NumberReference) : void{
		var data : ElementReference;
		var errorMessages : StringArrayReference;
		var document : StringReference;
		var errorMessage : StringReference;
		var success : boolean;

		data = new ElementReference();
		errorMessages = new StringArrayReference();
		document = new StringReference();
		errorMessage = new StringReference();

		success = ReadJSON(json, data, errorMessages);

		AssertTrue(success, failures);

		if(success){
			success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage);

			AssertTrue(success, failures);

			if(success){
				AssertStringEquals(document.stringx, result, failures);
			}
		}
	}


	export function AssertTemplateError(template : string [], json : string [], errorMessage : string [], failures : NumberReference) : void{
		var data : ElementReference;
		var errorMessages : StringArrayReference;
		var document : StringReference;
		var errorMessageRef : StringReference;
		var success : boolean;

		data = new ElementReference();
		errorMessages = new StringArrayReference();
		document = new StringReference();
		errorMessageRef = new StringReference();

		success = ReadJSON(json, data, errorMessages);

		AssertTrue(success, failures);

		if(success){
			success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessageRef);

			AssertFalse(success, failures);

			if(!success){
				AssertStringEquals(errorMessage, errorMessageRef.stringx, failures);
			}
		}
	}


	export function CreateBooleanReference(value : boolean) : BooleanReference{
		var ref : BooleanReference;

		ref = new BooleanReference();
		ref.booleanValue = value;

		return ref;
	}


	export function CreateBooleanArrayReference(value : boolean []) : BooleanArrayReference{
		var ref : BooleanArrayReference;

		ref = new BooleanArrayReference();
		ref.booleanArray = value;

		return ref;
	}


	export function CreateBooleanArrayReferenceLengthValue(length : number, value : boolean) : BooleanArrayReference{
		var ref : BooleanArrayReference;
		var i : number;

		ref = new BooleanArrayReference();
		ref.booleanArray = new Array<boolean>(length);

		for(i = 0; i < length; i = i + 1){
			ref.booleanArray[i] = value;
		}

		return ref;
	}


	export function FreeBooleanArrayReference(booleanArrayReference : BooleanArrayReference) : void{
		delete booleanArrayReference.booleanArray;
		booleanArrayReference = undefined;
	}


	export function CreateCharacterReference(value : string) : CharacterReference{
		var ref : CharacterReference;

		ref = new CharacterReference();
		ref.characterValue = value;

		return ref;
	}


	export function CreateNumberReference(value : number) : NumberReference{
		var ref : NumberReference;

		ref = new NumberReference();
		ref.numberValue = value;

		return ref;
	}


	export function CreateNumberArrayReference(value : number []) : NumberArrayReference{
		var ref : NumberArrayReference;

		ref = new NumberArrayReference();
		ref.numberArray = value;

		return ref;
	}


	export function CreateNumberArrayReferenceLengthValue(length : number, value : number) : NumberArrayReference{
		var ref : NumberArrayReference;
		var i : number;

		ref = new NumberArrayReference();
		ref.numberArray = new Array<number>(length);

		for(i = 0; i < length; i = i + 1){
			ref.numberArray[i] = value;
		}

		return ref;
	}


	export function FreeNumberArrayReference(numberArrayReference : NumberArrayReference) : void{
		delete numberArrayReference.numberArray;
		numberArrayReference = undefined;
	}


	export function CreateStringReference(value : string []) : StringReference{
		var ref : StringReference;

		ref = new StringReference();
		ref.stringx = value;

		return ref;
	}


	export function CreateStringReferenceLengthValue(length : number, value : string) : StringReference{
		var ref : StringReference;
		var i : number;

		ref = new StringReference();
		ref.stringx = new Array<string>(length);

		for(i = 0; i < length; i = i + 1){
			ref.stringx[i] = value;
		}

		return ref;
	}


	export function FreeStringReference(stringReference : StringReference) : void{
		delete stringReference.stringx;
		stringReference = undefined;
	}


	export function CreateStringArrayReference(strings : StringReference []) : StringArrayReference{
		var ref : StringArrayReference;

		ref = new StringArrayReference();
		ref.stringArray = strings;

		return ref;
	}


	export function CreateStringArrayReferenceLengthValue(length : number, value : string []) : StringArrayReference{
		var ref : StringArrayReference;
		var i : number;

		ref = new StringArrayReference();
		ref.stringArray = new Array<StringReference>(length);

		for(i = 0; i < length; i = i + 1){
			ref.stringArray[i] = CreateStringReference(value);
		}

		return ref;
	}


	export function FreeStringArrayReference(stringArrayReference : StringArrayReference) : void{
		var i : number;

		for(i = 0; i < stringArrayReference.stringArray.length; i = i + 1){
			delete stringArrayReference.stringArray[i];
		}
		delete stringArrayReference.stringArray;
		stringArrayReference = undefined;
	}


	export function IsValidJSON(json : string [], errorMessage : StringArrayReference) : boolean{
		var success : boolean;
		var elementReference : ElementReference;

		elementReference = new ElementReference();

		success = ReadJSON(json, elementReference, errorMessage);

		if(success){
			DeleteElement(elementReference.element);
		}

		return success;
	}


	export function JSONTokenize(json : string [], tokensReference : StringArrayReference, errorMessages : StringArrayReference) : boolean{
		var i : number;
		var c : string;
		var str : string [];
		var stringReference : StringReference, tokenReference : StringReference;
		var stringLength : NumberReference;
		var success : boolean;
		var ll : LinkedListStrings;

		ll = CreateLinkedListString();
		success = true;

		stringLength = new NumberReference();
		tokenReference = new StringReference();

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
				/* Skip.*/
				i = i + 1;
			}else if(c == '\"'){
				success = GetJSONString(json, i, tokenReference, stringLength, errorMessages);
				if(success){
					LinkedListAddString(ll, tokenReference.stringx);
					i = i + stringLength.numberValue;
				}
			}else if(IsJSONNumberCharacter(c)){
				success = GetJSONNumberToken(json, i, tokenReference, errorMessages);
				if(success){
					LinkedListAddString(ll, tokenReference.stringx);
					i = i + tokenReference.stringx.length;
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


	export function GetJSONNumberToken(json : string [], start : number, tokenReference : StringReference, errorMessages : StringArrayReference) : boolean{
		var c : string;
		var end : number, i : number;
		var done : boolean, success : boolean;
		var numberString : string [];

		end = json.length;
		done = false;

		for(i = start; i < json.length && !done; i = i + 1){
			c = json[i];
			if(!IsJSONNumberCharacter(c)){
				done = true;
				end = i;
			}
		}

		numberString = Substring(json, start, end);

		success = IsValidJSONNumber(numberString, errorMessages);

		tokenReference.stringx = numberString;

		return success;
	}


	export function IsValidJSONNumber(n : string [], errorMessages : StringArrayReference) : boolean{
		var success : boolean;
		var i : number;

		i = 0;

		/* JSON allows an optional negative sign.*/
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


	export function IsValidJSONNumberAfterSign(n : string [], i : number, errorMessages : StringArrayReference) : boolean{
		var success : boolean;

		if(charIsNumber(n[i])){
			/* 0 first means only 0.*/
			if(n[i] == '0'){
				i = i + 1;
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
			AddStringRef(errorMessages, CreateStringReference("A number must start with 0-9 (after the optional sign).".split('')));
		}

		return success;
	}


	export function IsValidJSONNumberAdvancePastDigits(n : string [], i : number) : number{
		var done : boolean;

		i = i + 1;
		done = false;
		for(; i < n.length && !done; ){
			if(charIsNumber(n[i])){
				i = i + 1;
			}else{
				done = true;
			}
		}

		return i;
	}


	export function IsValidJSONNumberFromDotOrExponent(n : string [], i : number, errorMessages : StringArrayReference) : boolean{
		var wasDotAndOrE : boolean, success : boolean;

		wasDotAndOrE = false;
		success = true;

		if(n[i] == '.'){
			i = i + 1;
			wasDotAndOrE = true;

			if(i < n.length){
				if(charIsNumber(n[i])){
					/* Read digits following decimal point.*/
					i = IsValidJSONNumberAdvancePastDigits(n, i);

					if(i == n.length){
						/* If non-scientific decimal number, we are done.*/
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
			/* If number with decimal point.*/
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


	export function IsValidJSONNumberFromExponent(n : string [], i : number, errorMessages : StringArrayReference) : boolean{
		var success : boolean;

		i = i + 1;

		if(i < n.length){
			/* The exponent sign can either + or -,*/
			if(n[i] == '+' || n[i] == '-'){
				i = i + 1;
			}

			if(i < n.length){
				if(charIsNumber(n[i])){
					/* Read digits following decimal point.*/
					i = IsValidJSONNumberAdvancePastDigits(n, i);

					if(i == n.length){
						/* We found scientific number.*/
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


	export function IsJSONNumberCharacter(c : string) : boolean{
		var numericCharacters : string [];
		var found : boolean;
		var i : number;

		numericCharacters = "0123456789.-+eE".split('');

		found = false;

		for(i = 0; i < numericCharacters.length; i = i + 1){
			if(numericCharacters[i] == c){
				found = true;
			}
		}

		return found;
	}


	export function GetJSONPrimitiveName(stringx : string [], start : number, errorMessages : StringArrayReference, primitive : string [], tokenReference : StringReference) : boolean{
		var c : string, p : string;
		var done : boolean, success : boolean;
		var i : number;
		var str : string [], token : string [];

		done = false;
		success = true;

		token = "".split('');

		for(i = start; i < stringx.length && ((i - start) < primitive.length) && !done; i = i + 1){
			c = stringx[i];
			p = primitive[i - start];
			if(c == p){
				/* OK*/
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

		tokenReference.stringx = token;

		return success;
	}


	export function GetJSONString(json : string [], start : number, tokenReference : StringReference, stringLengthReference : NumberReference, errorMessages : StringArrayReference) : boolean{
		var success : boolean, done : boolean;
		var stringx : string [], hex : string [];
		var characterCount : NumberReference, hexReference : NumberReference;
		var i : number, l : number, c : number;
		var errorMessage : StringReference;

		characterCount = CreateNumberReference(0);
		hex = CreateString(4, '0');
		hexReference = new NumberReference();
		errorMessage = new StringReference();

		success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages);

		if(success){
			l = characterCount.numberValue;
			stringx = new Array<string>(l);

			c = 0;
			stringx[c] = '\"';
			c = c + 1;

			done = false;
			for(i = start + 1; !done; i = i + 1){
				if(json[i] == '\\'){
					i = i + 1;
					if(json[i] == '\"' || json[i] == '\\' || json[i] == '/'){
						stringx[c] = json[i];
						c = c + 1;
					}else if(json[i] == 'b'){
						stringx[c] = String.fromCharCode(8);
						c = c + 1;
					}else if(json[i] == 'f'){
						stringx[c] = String.fromCharCode(12);
						c = c + 1;
					}else if(json[i] == 'n'){
						stringx[c] = String.fromCharCode(10);
						c = c + 1;
					}else if(json[i] == 'r'){
						stringx[c] = String.fromCharCode(13);
						c = c + 1;
					}else if(json[i] == 't'){
						stringx[c] = String.fromCharCode(9);
						c = c + 1;
					}else if(json[i] == 'u'){
						i = i + 1;
						hex[0] = charToUpperCase(json[i + 0]);
						hex[1] = charToUpperCase(json[i + 1]);
						hex[2] = charToUpperCase(json[i + 2]);
						hex[3] = charToUpperCase(json[i + 3]);
						nCreateNumberFromStringWithCheck(hex, 16, hexReference, errorMessage);
						stringx[c] = String.fromCharCode(hexReference.numberValue);
						i = i + 3;
						c = c + 1;
					}
				}else if(json[i] == '\"'){
					stringx[c] = json[i];
					c = c + 1;
					done = true;
				}else{
					stringx[c] = json[i];
					c = c + 1;
				}
			}

			tokenReference.stringx = stringx;
			success = true;
		}else{
			AddStringRef(errorMessages, CreateStringReference("End of string was not found.".split('')));
			success = false;
		}

		return success;
	}


	export function IsValidJSONString(jsonString : string [], errorMessages : StringArrayReference) : boolean{
		var valid : boolean;
		var numberReference : NumberReference, stringLength : NumberReference;

		numberReference = new NumberReference();
		stringLength = new NumberReference();

		valid = IsValidJSONStringInJSON(jsonString, 0, numberReference, stringLength, errorMessages);

		return valid;
	}


	export function IsValidJSONStringInJSON(json : string [], start : number, characterCount : NumberReference, stringLengthReference : NumberReference, errorMessages : StringArrayReference) : boolean{
		var success : boolean, done : boolean;
		var i : number, j : number;
		var c : string;

		success = true;
		done = false;

		characterCount.numberValue = 1;

		for(i = start + 1; i < json.length && !done && success; i = i + 1){
			if(!IsJSONIllegalControllCharacter(json[i])){
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


	export function IsJSONIllegalControllCharacter(c : string) : boolean{
		var cnr : number;
		var isControll : boolean;

		cnr = c.charCodeAt(0);

		if(cnr >= 0 && cnr < 32){
			isControll = true;
		}else{
			isControll = false;
		}

		return isControll;
	}


	export function AddElement(list : Elementx [], a : Elementx) : Elementx []{
		var newlist : Elementx [];
		var i : number;

		newlist = new Array<Elementx>(list.length + 1);

		for(i = 0; i < list.length; i = i + 1){
			newlist[i] = list[i];
		}
		newlist[list.length] = a;

		list = undefined;

		return newlist;
	}


	export function AddElementRef(list : ElementArrayReference, i : Elementx) : void{
		list.array = AddElement(list.array, i);
	}


	export function RemoveElement(list : Elementx [], n : number) : Elementx []{
		var newlist : Elementx [];
		var i : number;

		newlist = new Array<Elementx>(list.length - 1);

		for(i = 0; i < list.length; i = i + 1){
			if(i < n){
				newlist[i] = list[i];
			}
			if(i > n){
				newlist[i - 1] = list[i];
			}
		}

		list = undefined;

		return newlist;
	}


	export function GetElementRef(list : ElementArrayReference, i : number) : Elementx{
		return list.array[i];
	}


	export function RemoveElementRef(list : ElementArrayReference, i : number) : void{
		list.array = RemoveElement(list.array, i);
	}


	export function CreateLinkedListElements() : LinkedListElements{
		var ll : LinkedListElements;

		ll = new LinkedListElements();
		ll.first = new LinkedListNodeElements();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}


	export function LinkedListAddElement(ll : LinkedListElements, value : Elementx) : void{
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeElements();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}


	export function LinkedListElementsToArray(ll : LinkedListElements) : Elementx []{
		var array : Elementx [];
		var length : number, i : number;
		var node : LinkedListNodeElements;

		node = ll.first;

		length = LinkedListElementsLength(ll);

		array = new Array<Elementx>(length);

		for(i = 0; i < length; i = i + 1){
			array[i] = node.value;
			node = node.next;
		}

		return array;
	}


	export function LinkedListElementsLength(ll : LinkedListElements) : number{
		var l : number;
		var node : LinkedListNodeElements;

		l = 0;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1;
		}

		return l;
	}


	export function FreeLinkedListElements(ll : LinkedListElements) : void{
		var node : LinkedListNodeElements, prev : LinkedListNodeElements;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			prev = undefined;
		}

		node = undefined;
	}


	export function ComputeJSONStringLength(element : Elementx) : number{
		var result : number;

		result = 0;

		if(StringsEqual(element.typex, "object".split(''))){
			result = result + ComputeJSONObjectStringLength(element);
		}else if(StringsEqual(element.typex, "string".split(''))){
			result = JSONEscapedStringLength(element.stringx) + 2;
		}else if(StringsEqual(element.typex, "array".split(''))){
			result = result + ComputeJSONArrayStringLength(element);
		}else if(StringsEqual(element.typex, "number".split(''))){
			result = result + ComputeJSONNumberStringLength(element);
		}else if(StringsEqual(element.typex, "null".split(''))){
			result = result + "null".split('').length;
		}else if(StringsEqual(element.typex, "boolean".split(''))){
			result = result + ComputeJSONBooleanStringLength(element);
		}

		return result;
	}


	export function ComputeJSONBooleanStringLength(element : Elementx) : number{
		var result : number;

		if(element.booleanValue){
			result = "true".split('').length;
		}else{
			result = "false".split('').length;
		}

		return result;
	}


	export function ComputeJSONNumberStringLength(element : Elementx) : number{
		var length : number;
		var a : string [];

		if(element.numberx != 0){
			if(Math.abs(element.numberx) >= 10**15 || Math.abs(element.numberx) <= 10**(-15)){
				a = nCreateStringScientificNotationDecimalFromNumber(element.numberx);
				length = a.length;
			}else{
				a = nCreateStringDecimalFromNumber(element.numberx);
				length = a.length;
			}
		}else{
			length = 1;
		}

		return length;
	}


	export function ComputeJSONArrayStringLength(element : Elementx) : number{
		var arrayElement : Elementx;
		var i : number;
		var length : number;

		length = 1;

		for(i = 0; i < element.array.length; i = i + 1){
			arrayElement = element.array[i];

			length = length + ComputeJSONStringLength(arrayElement);

			if(i + 1 != element.array.length){
				length = length + 1;
			}
		}

		length = length + 1;

		return length;
	}


	export function ComputeJSONObjectStringLength(element : Elementx) : number{
		var key : string [];
		var i : number;
		var keys : StringArrayReference;
		var objectElement : Elementx;
		var length : number;

		length = 1;

		keys = GetStringElementMapKeySet(element.object);
		for(i = 0; i < keys.stringArray.length; i = i + 1){
			key = keys.stringArray[i].stringx;
			objectElement = GetObjectValue(element.object, key);

			length = length + 1;
			length = length + JSONEscapedStringLength(key);
			length = length + 1;
			length = length + 1;

			length = length + ComputeJSONStringLength(objectElement);

			if(i + 1 != keys.stringArray.length){
				length = length + 1;
			}
		}

		length = length + 1;

		return length;
	}


	export function CreateStringElement(stringx : string []) : Elementx{
		var element : Elementx;
		element = new Elementx();
		element.typex = "string".split('');
		element.stringx = stringx;
		return element;
	}


	export function CreateBooleanElement(booleanValue : boolean) : Elementx{
		var element : Elementx;
		element = new Elementx();
		element.typex = "boolean".split('');
		element.booleanValue = booleanValue;
		return element;
	}


	export function CreateNullElement() : Elementx{
		var element : Elementx;
		element = new Elementx();
		element.typex = "null".split('');
		return element;
	}


	export function CreateNumberElement(numberx : number) : Elementx{
		var element : Elementx;
		element = new Elementx();
		element.typex = "number".split('');
		element.numberx = numberx;
		return element;
	}


	export function CreateArrayElement(length : number) : Elementx{
		var element : Elementx;
		element = new Elementx();
		element.typex = "array".split('');
		element.array = new Array<Elementx>(length);
		return element;
	}


	export function CreateObjectElement(length : number) : Elementx{
		var element : Elementx;
		element = new Elementx();
		element.typex = "object".split('');
		element.object = new StringElementMap();
		element.object.stringListRef = CreateStringArrayReferenceLengthValue(length, "".split(''));
		element.object.elementListRef = new ElementArrayReference();
		element.object.elementListRef.array = new Array<Elementx>(length);
		return element;
	}


	export function DeleteElement(element : Elementx) : void{
		if(StringsEqual(element.typex, "object".split(''))){
			DeleteObject(element);
		}else if(StringsEqual(element.typex, "string".split(''))){
			element = undefined;
		}else if(StringsEqual(element.typex, "array".split(''))){
			DeleteArray(element);
		}else if(StringsEqual(element.typex, "number".split(''))){
			element = undefined;
		}else if(StringsEqual(element.typex, "null".split(''))){
			element = undefined;
		}else if(StringsEqual(element.typex, "boolean".split(''))){
			element = undefined;
		}else{
		}
	}


	export function DeleteObject(element : Elementx) : void{
		var keys : StringArrayReference;
		var i : number;
		var key : string [];
		var objectElement : Elementx;

		keys = GetStringElementMapKeySet(element.object);
		for(i = 0; i < keys.stringArray.length; i = i + 1){
			key = keys.stringArray[i].stringx;
			objectElement = GetObjectValue(element.object, key);
			DeleteElement(objectElement);
		}
	}


	export function DeleteArray(element : Elementx) : void{
		var i : number;
		var arrayElement : Elementx;

		for(i = 0; i < element.array.length; i = i + 1){
			arrayElement = element.array[i];
			DeleteElement(arrayElement);
		}
	}


	export function WriteJSON(element : Elementx) : string []{
		var result : string [];
		var length : number;
		var index : NumberReference;

		length = ComputeJSONStringLength(element);
		result = new Array<string>(length);
		index = CreateNumberReference(0);

		if(StringsEqual(element.typex, "object".split(''))){
			WriteObject(element, result, index);
		}else if(StringsEqual(element.typex, "string".split(''))){
			WriteString(element, result, index);
		}else if(StringsEqual(element.typex, "array".split(''))){
			WriteArray(element, result, index);
		}else if(StringsEqual(element.typex, "number".split(''))){
			WriteNumber(element, result, index);
		}else if(StringsEqual(element.typex, "null".split(''))){
			WriteStringToStingStream(result, index, "null".split(''));
		}else if(StringsEqual(element.typex, "boolean".split(''))){
			WriteBooleanValue(element, result, index);
		}

		return result;
	}


	export function WriteBooleanValue(element : Elementx, result : string [], index : NumberReference) : void{
		if(element.booleanValue){
			WriteStringToStingStream(result, index, "true".split(''));
		}else{
			WriteStringToStingStream(result, index, "false".split(''));
		}
	}


	export function WriteNumber(element : Elementx, result : string [], index : NumberReference) : void{
		var numberString : string [];

		if(element.numberx != 0){
			if(Math.abs(element.numberx) >= 10**15 || Math.abs(element.numberx) <= 10**(-15)){
				numberString = nCreateStringScientificNotationDecimalFromNumber(element.numberx);
			}else{
				numberString = nCreateStringDecimalFromNumber(element.numberx);
			}
		}else{
			numberString = nCreateStringDecimalFromNumber(element.numberx);
		}

		WriteStringToStingStream(result, index, numberString);
	}


	export function WriteArray(element : Elementx, result : string [], index : NumberReference) : void{
		var s : string [];
		var arrayElement : Elementx;
		var i : number;

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


	export function WriteString(element : Elementx, result : string [], index : NumberReference) : void{
		WriteStringToStingStream(result, index, "\"".split(''));
		element.stringx = JSONEscapeString(element.stringx);
		WriteStringToStingStream(result, index, element.stringx);
		WriteStringToStingStream(result, index, "\"".split(''));
	}


	export function JSONEscapeString(stringx : string []) : string []{
		var i : number, length : number;
		var index : NumberReference, lettersReference : NumberReference;
		var ns : string [], escaped : string [];

		length = JSONEscapedStringLength(stringx);

		ns = new Array<string>(length);
		index = CreateNumberReference(0);
		lettersReference = CreateNumberReference(0);

		for(i = 0; i < stringx.length; i = i + 1){
			if(JSONMustBeEscaped(stringx[i], lettersReference)){
				escaped = JSONEscapeCharacter(stringx[i]);
				WriteStringToStingStream(ns, index, escaped);
			}else{
				WriteCharacterToStingStream(ns, index, stringx[i]);
			}
		}

		return ns;
	}


	export function JSONEscapedStringLength(stringx : string []) : number{
		var lettersReference : NumberReference;
		var i : number, length : number;

		lettersReference = CreateNumberReference(0);
		length = 0;

		for(i = 0; i < stringx.length; i = i + 1){
			if(JSONMustBeEscaped(stringx[i], lettersReference)){
				length = length + lettersReference.numberValue;
			}else{
				length = length + 1;
			}
		}
		return length;
	}


	export function JSONEscapeCharacter(c : string) : string []{
		var code : number;
		var escaped : string [];
		var hexNumber : StringReference;
		var q : number, rs : number, s : number, b : number, f : number, n : number, r : number, t : number;

		code = c.charCodeAt(0);

		q = 34;
		rs = 92;
		s = 47;
		b = 8;
		f = 12;
		n = 10;
		r = 13;
		t = 9;

		hexNumber = new StringReference();

		if(code == q){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = '\"';
		}else if(code == rs){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = '\\';
		}else if(code == s){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = '/';
		}else if(code == b){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = 'b';
		}else if(code == f){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = 'f';
		}else if(code == n){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = 'n';
		}else if(code == r){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = 'r';
		}else if(code == t){
			escaped = new Array<string>(2);
			escaped[0] = '\\';
			escaped[1] = 't';
		}else if(code >= 0 && code <= 31){
			escaped = new Array<string>(6);
			escaped[0] = '\\';
			escaped[1] = 'u';
			escaped[2] = '0';
			escaped[3] = '0';

			nCreateStringFromNumberWithCheck(code, 16, hexNumber);

			if(hexNumber.stringx.length == 1){
				escaped[4] = '0';
				escaped[5] = hexNumber.stringx[0];
			}else if(hexNumber.stringx.length == 2){
				escaped[4] = hexNumber.stringx[0];
				escaped[5] = hexNumber.stringx[1];
			}
		}else{
			escaped = new Array<string>(1);
			escaped[0] = c;
		}

		return escaped;
	}


	export function JSONMustBeEscaped(c : string, letters : NumberReference) : boolean{
		var code : number;
		var mustBeEscaped : boolean;
		var q : number, rs : number, s : number, b : number, f : number, n : number, r : number, t : number;

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
		}else if(code >= 2**16){
			mustBeEscaped = true;
			letters.numberValue = 6;
		}

		return mustBeEscaped;
	}


	export function WriteObject(element : Elementx, result : string [], index : NumberReference) : void{
		var s : string [], key : string [];
		var i : number;
		var keys : StringArrayReference;
		var objectElement : Elementx;

		WriteStringToStingStream(result, index, "{".split(''));

		keys = GetStringElementMapKeySet(element.object);
		for(i = 0; i < keys.stringArray.length; i = i + 1){
			key = keys.stringArray[i].stringx;
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


	export function ReadJSON(stringx : string [], elementReference : ElementReference, errorMessages : StringArrayReference) : boolean{
		var tokenArrayReference : StringArrayReference;
		var success : boolean;

		/* Tokenize.*/
		tokenArrayReference = new StringArrayReference();
		success = JSONTokenize(stringx, tokenArrayReference, errorMessages);

		if(success){
			/* Parse.*/
			success = GetJSONValue(tokenArrayReference.stringArray, elementReference, errorMessages);
		}

		return success;
	}


	export function GetJSONValue(tokens : StringReference [], elementReference : ElementReference, errorMessages : StringArrayReference) : boolean{
		var i : NumberReference;
		var success : boolean;

		i = CreateNumberReference(0);
		success = GetJSONValueRecursive(tokens, i, 0, elementReference, errorMessages);

		return success;
	}


	export function GetJSONValueRecursive(tokens : StringReference [], i : NumberReference, depth : number, elementReference : ElementReference, errorMessages : StringArrayReference) : boolean{
		var str : string [], substr : string [], token : string [];
		var stringToDecimalResult : number;
		var success : boolean;

		success = true;
		token = tokens[i.numberValue].stringx;

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
			if(StringsEqual(tokens[i.numberValue].stringx, "<end>".split(''))){
			}else{
				AddStringRef(errorMessages, CreateStringReference("The outer value cannot have any tokens following it.".split('')));
				success = false;
			}
		}

		return success;
	}


	export function GetJSONObject(tokens : StringReference [], i : NumberReference, depth : number, elementReference : ElementReference, errorMessages : StringArrayReference) : boolean{
		var element : Elementx, value : Elementx;
		var done : boolean, success : boolean;
		var key : string [], colon : string [], comma : string [], closeCurly : string [];
		var keystring : string [], str : string [];
		var valueReference : ElementReference;
		var values : LinkedListElements;
		var keys : LinkedListStrings;

		keys = CreateLinkedListString();
		values = CreateLinkedListElements();
		element = CreateObjectElement(0);
		valueReference = new ElementReference();
		success = true;
		i.numberValue = i.numberValue + 1;

		if(!StringsEqual(tokens[i.numberValue].stringx, "}".split(''))){
			done = false;

			for(; !done && success; ){
				key = tokens[i.numberValue].stringx;

				if(key[0] == '\"'){
					i.numberValue = i.numberValue + 1;
					colon = tokens[i.numberValue].stringx;
					if(StringsEqual(colon, ":".split(''))){
						i.numberValue = i.numberValue + 1;
						success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

						if(success){
							keystring = Substring(key, 1, key.length - 1);
							value = valueReference.element;
							LinkedListAddString(keys, keystring);
							LinkedListAddElement(values, value);

							comma = tokens[i.numberValue].stringx;
							if(StringsEqual(comma, ",".split(''))){
								/* OK*/
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
			closeCurly = tokens[i.numberValue].stringx;

			if(StringsEqual(closeCurly, "}".split(''))){
				/* OK*/
				delete element.object.stringListRef.stringArray;
				delete element.object.elementListRef.array;
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
		valueReference = undefined;

		return success;
	}


	export function GetJSONArray(tokens : StringReference [], i : NumberReference, depth : number, elementReference : ElementReference, errorMessages : StringArrayReference) : boolean{
		var element : Elementx, value : Elementx;
		var nextToken : string [], comma : string [];
		var done : boolean, success : boolean;
		var valueReference : ElementReference;
		var elements : LinkedListElements;

		elements = CreateLinkedListElements();
		i.numberValue = i.numberValue + 1;

		valueReference = new ElementReference();
		success = true;
		element = CreateArrayElement(0);

		nextToken = tokens[i.numberValue].stringx;

		if(!StringsEqual(nextToken, "]".split(''))){
			done = false;
			for(; !done && success; ){
				success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages);

				if(success){
					value = valueReference.element;
					LinkedListAddElement(elements, value);

					comma = tokens[i.numberValue].stringx;

					if(StringsEqual(comma, ",".split(''))){
						/* OK*/
						i.numberValue = i.numberValue + 1;
					}else{
						done = true;
					}
				}
			}
		}

		nextToken = tokens[i.numberValue].stringx;
		if(StringsEqual(nextToken, "]".split(''))){
			/* OK*/
			i.numberValue = i.numberValue + 1;
			delete element.array;
			element.array = LinkedListElementsToArray(elements);
		}else{
			AddStringRef(errorMessages, CreateStringReference("Expected close square bracket at end of array.".split('')));
			success = false;
		}

		elementReference.element = element;
		FreeLinkedListElements(elements);
		valueReference = undefined;

		return success;
	}


	export function GetStringElementMapKeySet(stringElementMap : StringElementMap) : StringArrayReference{
		return stringElementMap.stringListRef;
	}


	export function GetObjectValue(stringElementMap : StringElementMap, key : string []) : Elementx{
		var result : Elementx;
		var i : number;

		result = new Elementx();

		for(i = 0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1){
			if(StringsEqual(stringElementMap.stringListRef.stringArray[i].stringx, key)){
				result = stringElementMap.elementListRef.array[i];
			}
		}

		return result;
	}


	export function GetObjectValueWithCheck(stringElementMap : StringElementMap, key : string [], foundReference : BooleanReference) : Elementx{
		var result : Elementx;
		var i : number;

		result = new Elementx();

		foundReference.booleanValue = false;
		for(i = 0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1){
			if(StringsEqual(stringElementMap.stringListRef.stringArray[i].stringx, key)){
				result = stringElementMap.elementListRef.array[i];
				foundReference.booleanValue = true;
			}
		}

		return result;
	}


	export function PutStringElementMap(stringElementMap : StringElementMap, keystring : string [], value : Elementx) : void{
		AddStringRef(stringElementMap.stringListRef, CreateStringReference(keystring));
		AddElementRef(stringElementMap.elementListRef, value);
	}


	export function SetStringElementMap(stringElementMap : StringElementMap, index : number, keystring : string [], value : Elementx) : void{
		stringElementMap.stringListRef.stringArray[index].stringx = keystring;
		stringElementMap.elementListRef.array[index] = value;
	}


	export function GetStringElementMapNumberOfKeys(stringElementMap : StringElementMap) : number{
		return stringElementMap.stringListRef.stringArray.length;
	}


	export function JSONCompare(a : string [], b : string [], epsilon : number, equal : BooleanReference, errorMessage : StringArrayReference) : boolean{
		var success : boolean;
		var eaRef : ElementReference, ebRef : ElementReference;
		var ea : Elementx, eb : Elementx;

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


	export function JSONCompareElements(ea : Elementx, eb : Elementx, epsilon : number) : boolean{
		var equal : boolean;
		var typeName : string [];

		equal = StringsEqual(ea.typex, eb.typex);
        
		if(equal){
			typeName = ea.typex;
			if(StringsEqual(typeName, "object".split(''))){
				equal = JSONCompareObjects(ea.object, eb.object, epsilon);
			}else if(StringsEqual(typeName, "string".split(''))){
				equal = StringsEqual(ea.stringx, eb.stringx);
			}else if(StringsEqual(typeName, "array".split(''))){
				equal = JSONCompareArrays(ea.array, eb.array, epsilon);
			}else if(StringsEqual(typeName, "number".split(''))){
				equal = EpsilonCompare(ea.numberx, eb.numberx, epsilon);
			}else if(StringsEqual(typeName, "null".split(''))){
				equal = true;
			}else if(StringsEqual(typeName, "boolean".split(''))){
				equal = ea.booleanValue == eb.booleanValue;
			}
		}
        
		return equal;
	}


	export function JSONCompareArrays(ea : Elementx [], eb : Elementx [], epsilon : number) : boolean{
		var equals : boolean;
		var i : number, length : number;

		equals = ea.length == eb.length;

		if(equals){
			length = ea.length;
			for(i = 0; i < length && equals; i = i + 1){
				equals = JSONCompareElements(ea[i], eb[i], epsilon);
			}
		}

		return equals;
	}


	export function JSONCompareObjects(ea : StringElementMap, eb : StringElementMap, epsilon : number) : boolean{
		var equals : boolean;
		var akeys : number, bkeys : number, i : number;
		var keys : StringArrayReference;
		var key : string [];
		var aFoundReference : BooleanReference, bFoundReference : BooleanReference;
		var eaValue : Elementx, ebValue : Elementx;

		aFoundReference = new BooleanReference();
		bFoundReference = new BooleanReference();

		akeys = GetStringElementMapNumberOfKeys(ea);
		bkeys = GetStringElementMapNumberOfKeys(eb);

		equals = akeys == bkeys;

		if(equals){
			keys = GetStringElementMapKeySet(ea);

			for(i = 0; i < keys.stringArray.length && equals; i = i + 1){
				key = keys.stringArray[i].stringx;

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


	export function AddNumber(list : number [], a : number) : number []{
		var newlist : number [];
		var i : number;

		newlist = new Array<number>(list.length + 1);
		for(i = 0; i < list.length; i = i + 1){
			newlist[i] = list[i];
		}
		newlist[list.length] = a;
		
		list = undefined;
		
		return newlist;
	}


	export function AddNumberRef(list : NumberArrayReference, i : number) : void{
		list.numberArray = AddNumber(list.numberArray, i);
	}


	export function RemoveNumber(list : number [], n : number) : number []{
		var newlist : number [];
		var i : number;

		newlist = new Array<number>(list.length - 1);

		if(n >= 0 && n < list.length){
			for(i = 0; i < list.length; i = i + 1){
				if(i < n){
					newlist[i] = list[i];
				}
				if(i > n){
					newlist[i - 1] = list[i];
				}
			}

			list = undefined;
		}else{
			newlist = undefined;
		}
		
		return newlist;
	}


	export function GetNumberRef(list : NumberArrayReference, i : number) : number{
		return list.numberArray[i];
	}


	export function RemoveNumberRef(list : NumberArrayReference, i : number) : void{
		list.numberArray = RemoveNumber(list.numberArray, i);
	}


	export function AddString(list : StringReference [], a : StringReference) : StringReference []{
		var newlist : StringReference [];
		var i : number;

		newlist = new Array<StringReference>(list.length + 1);

		for(i = 0; i < list.length; i = i + 1){
			newlist[i] = list[i];
		}
		newlist[list.length] = a;
		
		list = undefined;
		
		return newlist;
	}


	export function AddStringRef(list : StringArrayReference, i : StringReference) : void{
		list.stringArray = AddString(list.stringArray, i);
	}


	export function RemoveString(list : StringReference [], n : number) : StringReference []{
		var newlist : StringReference [];
		var i : number;

		newlist = new Array<StringReference>(list.length - 1);

		if(n >= 0 && n < list.length){
			for(i = 0; i < list.length; i = i + 1){
				if(i < n){
					newlist[i] = list[i];
				}
				if(i > n){
					newlist[i - 1] = list[i];
				}
			}

			list = undefined;
		}else{
			newlist = undefined;
		}
		
		return newlist;
	}


	export function GetStringRef(list : StringArrayReference, i : number) : StringReference{
		return list.stringArray[i];
	}


	export function RemoveStringRef(list : StringArrayReference, i : number) : void{
		list.stringArray = RemoveString(list.stringArray, i);
	}


	export function CreateDynamicArrayCharacters() : DynamicArrayCharacters{
		var da : DynamicArrayCharacters;

		da = new DynamicArrayCharacters();
		da.array = new Array<string>(10);
		da.length = 0;

		return da;
	}


	export function CreateDynamicArrayCharactersWithInitialCapacity(capacity : number) : DynamicArrayCharacters{
		var da : DynamicArrayCharacters;

		da = new DynamicArrayCharacters();
		da.array = new Array<string>(capacity);
		da.length = 0;

		return da;
	}


	export function DynamicArrayAddCharacter(da : DynamicArrayCharacters, value : string) : void{
		if(da.length == da.array.length){
			DynamicArrayCharactersIncreaseSize(da);
		}

		da.array[da.length] = value;
		da.length = da.length + 1;
	}


	export function DynamicArrayCharactersIncreaseSize(da : DynamicArrayCharacters) : void{
		var newLength : number, i : number;
		var newArray : string [];

		newLength = Math.round(da.array.length*3/2);
		newArray = new Array<string>(newLength);

		for(i = 0; i < da.array.length; i = i + 1){
			newArray[i] = da.array[i];
		}

		delete da.array;

		da.array = newArray;
	}


	export function DynamicArrayCharactersDecreaseSizeNecessary(da : DynamicArrayCharacters) : boolean{
		var needsDecrease : boolean;

		needsDecrease = false;

		if(da.length > 10){
			needsDecrease = da.length <= Math.round(da.array.length*2/3);
		}

		return needsDecrease;
	}


	export function DynamicArrayCharactersDecreaseSize(da : DynamicArrayCharacters) : void{
		var newLength : number, i : number;
		var newArray : string [];

		newLength = Math.round(da.array.length*2/3);
		newArray = new Array<string>(newLength);

		for(i = 0; i < newLength; i = i + 1){
			newArray[i] = da.array[i];
		}

		delete da.array;

		da.array = newArray;
	}


	export function DynamicArrayCharactersLength(da : DynamicArrayCharacters) : number{
		return da.length;
	}


	export function DynamicArrayInsertCharacter(da : DynamicArrayCharacters, index : number, value : string) : void{
		var i : number;

		if(da.length == da.array.length){
			DynamicArrayCharactersIncreaseSize(da);
		}

		for(i = da.length; i > index; i = i - 1){
			da.array[i] = da.array[i - 1];
		}

		da.array[index] = value;

		da.length = da.length + 1;
	}


	export function DynamicArrayCharacterSet(da : DynamicArrayCharacters, index : number, value : string) : boolean{
		var success : boolean;

		if(index < da.length){
			da.array[index] = value;
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function DynamicArrayRemoveCharacter(da : DynamicArrayCharacters, index : number) : void{
		var i : number;

		for(i = index; i < da.length - 1; i = i + 1){
			da.array[i] = da.array[i + 1];
		}

		da.length = da.length - 1;

		if(DynamicArrayCharactersDecreaseSizeNecessary(da)){
			DynamicArrayCharactersDecreaseSize(da);
		}
	}


	export function FreeDynamicArrayCharacters(da : DynamicArrayCharacters) : void{
		delete da.array;
		da = undefined;
	}


	export function DynamicArrayCharactersToArray(da : DynamicArrayCharacters) : string []{
		var array : string [];
		var i : number;

		array = new Array<string>(da.length);

		for(i = 0; i < da.length; i = i + 1){
			array[i] = da.array[i];
		}

		return array;
	}


	export function ArrayToDynamicArrayCharactersWithOptimalSize(array : string []) : DynamicArrayCharacters{
		var da : DynamicArrayCharacters;
		var i : number;
		var c : number, n : number, newCapacity : number;

		c = array.length;
		n = (Math.log(c) - 1)/Math.log(3/2);
		newCapacity = Math.floor(n) + 1;

		da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity);

		for(i = 0; i < array.length; i = i + 1){
			da.array[i] = array[i];
		}

		return da;
	}


	export function ArrayToDynamicArrayCharacters(array : string []) : DynamicArrayCharacters{
		var da : DynamicArrayCharacters;

		da = new DynamicArrayCharacters();
		da.array = CopyString(array);
		da.length = array.length;

		return da;
	}


	export function DynamicArrayCharactersEqual(a : DynamicArrayCharacters, b : DynamicArrayCharacters) : boolean{
		var equal : boolean;
		var i : number;

		equal = true;
		if(a.length == b.length){
			for(i = 0; i < a.length && equal; i = i + 1){
				if(a.array[i] != b.array[i]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}


	export function DynamicArrayCharactersToLinkedList(da : DynamicArrayCharacters) : LinkedListCharacters{
		var ll : LinkedListCharacters;
		var i : number;

		ll = CreateLinkedListCharacter();

		for(i = 0; i < da.length; i = i + 1){
			LinkedListAddCharacter(ll, da.array[i]);
		}

		return ll;
	}


	export function LinkedListToDynamicArrayCharacters(ll : LinkedListCharacters) : DynamicArrayCharacters{
		var da : DynamicArrayCharacters;
		var i : number;
		var node : LinkedListNodeCharacters;

		node = ll.first;

		da = new DynamicArrayCharacters();
		da.length = LinkedListCharactersLength(ll);

		da.array = new Array<string>(da.length);

		for(i = 0; i < da.length; i = i + 1){
			da.array[i] = node.value;
			node = node.next;
		}

		return da;
	}


	export function AddBoolean(list : boolean [], a : boolean) : boolean []{
		var newlist : boolean [];
		var i : number;

		newlist = new Array<boolean>(list.length + 1);
		for(i = 0; i < list.length; i = i + 1){
			newlist[i] = list[i];
		}
		newlist[list.length] = a;
		
		list = undefined;
		
		return newlist;
	}


	export function AddBooleanRef(list : BooleanArrayReference, i : boolean) : void{
		list.booleanArray = AddBoolean(list.booleanArray, i);
	}


	export function RemoveBoolean(list : boolean [], n : number) : boolean []{
		var newlist : boolean [];
		var i : number;

		newlist = new Array<boolean>(list.length - 1);

		if(n >= 0 && n < list.length){
			for(i = 0; i < list.length; i = i + 1){
				if(i < n){
					newlist[i] = list[i];
				}
				if(i > n){
					newlist[i - 1] = list[i];
				}
			}

			list = undefined;
		}else{
			newlist = undefined;
		}
		
		return newlist;
	}


	export function GetBooleanRef(list : BooleanArrayReference, i : number) : boolean{
		return list.booleanArray[i];
	}


	export function RemoveDecimalRef(list : BooleanArrayReference, i : number) : void{
		list.booleanArray = RemoveBoolean(list.booleanArray, i);
	}


	export function CreateLinkedListString() : LinkedListStrings{
		var ll : LinkedListStrings;

		ll = new LinkedListStrings();
		ll.first = new LinkedListNodeStrings();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}


	export function LinkedListAddString(ll : LinkedListStrings, value : string []) : void{
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeStrings();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}


	export function LinkedListStringsToArray(ll : LinkedListStrings) : StringReference []{
		var array : StringReference [];
		var length : number, i : number;
		var node : LinkedListNodeStrings;

		node = ll.first;

		length = LinkedListStringsLength(ll);

		array = new Array<StringReference>(length);

		for(i = 0; i < length; i = i + 1){
			array[i] = new StringReference();
			array[i].stringx = node.value;
			node = node.next;
		}

		return array;
	}


	export function LinkedListStringsLength(ll : LinkedListStrings) : number{
		var l : number;
		var node : LinkedListNodeStrings;

		l = 0;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1;
		}

		return l;
	}


	export function FreeLinkedListString(ll : LinkedListStrings) : void{
		var node : LinkedListNodeStrings, prev : LinkedListNodeStrings;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			prev = undefined;
		}

		node = undefined;
	}


	export function CreateLinkedListNumbers() : LinkedListNumbers{
		var ll : LinkedListNumbers;

		ll = new LinkedListNumbers();
		ll.first = new LinkedListNodeNumbers();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}


	export function CreateLinkedListNumbersArray(length : number) : LinkedListNumbers []{
		var lls : LinkedListNumbers [];
		var i : number;

		lls = new Array<LinkedListNumbers>(length);
		for(i = 0; i < lls.length; i = i + 1){
			lls[i] = CreateLinkedListNumbers();
		}

		return lls;
	}


	export function LinkedListAddNumber(ll : LinkedListNumbers, value : number) : void{
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeNumbers();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}


	export function LinkedListNumbersLength(ll : LinkedListNumbers) : number{
		var l : number;
		var node : LinkedListNodeNumbers;

		l = 0;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1;
		}

		return l;
	}


	export function LinkedListNumbersIndex(ll : LinkedListNumbers, index : number) : number{
		var i : number;
		var node : LinkedListNodeNumbers;

		node = ll.first;
		for(i = 0; i < index; i = i + 1){
			node = node.next;
		}

		return node.value;
	}


	export function LinkedListInsertNumber(ll : LinkedListNumbers, index : number, value : number) : void{
		var i : number;
		var node : LinkedListNodeNumbers, tmp : LinkedListNodeNumbers;

		if(index == 0){
			tmp = ll.first;
			ll.first = new LinkedListNodeNumbers();
			ll.first.next = tmp;
			ll.first.value = value;
			ll.first.end = false;
		}else{
			node = ll.first;
			for(i = 0; i < index - 1; i = i + 1){
				node = node.next;
			}

			tmp = node.next;
			node.next = new LinkedListNodeNumbers();
			node.next.next = tmp;
			node.next.value = value;
			node.next.end = false;
		}
	}


	export function LinkedListSet(ll : LinkedListNumbers, index : number, value : number) : void{
		var i : number;
		var node : LinkedListNodeNumbers;

		node = ll.first;
		for(i = 0; i < index; i = i + 1){
			node = node.next;
		}

		node.next.value = value;
	}


	export function LinkedListRemoveNumber(ll : LinkedListNumbers, index : number) : void{
		var i : number;
		var node : LinkedListNodeNumbers, prev : LinkedListNodeNumbers;

		node = ll.first;
		prev = ll.first;

		for(i = 0; i < index; i = i + 1){
			prev = node;
			node = node.next;
		}

		if(index == 0){
			ll.first = prev.next;
		}
		if(!prev.next.end){
			prev.next = prev.next.next;
		}
	}


	export function FreeLinkedListNumbers(ll : LinkedListNumbers) : void{
		var node : LinkedListNodeNumbers, prev : LinkedListNodeNumbers;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			prev = undefined;
		}

		node = undefined;
	}


	export function FreeLinkedListNumbersArray(lls : LinkedListNumbers []) : void{
		var i : number;

		for(i = 0; i < lls.length; i = i + 1){
			FreeLinkedListNumbers(lls[i]);
		}
		lls = undefined;
	}


	export function LinkedListNumbersToArray(ll : LinkedListNumbers) : number []{
		var array : number [];
		var length : number, i : number;
		var node : LinkedListNodeNumbers;

		node = ll.first;

		length = LinkedListNumbersLength(ll);

		array = new Array<number>(length);

		for(i = 0; i < length; i = i + 1){
			array[i] = node.value;
			node = node.next;
		}

		return array;
	}


	export function ArrayToLinkedListNumbers(array : number []) : LinkedListNumbers{
		var ll : LinkedListNumbers;
		var i : number;

		ll = CreateLinkedListNumbers();

		for(i = 0; i < array.length; i = i + 1){
			LinkedListAddNumber(ll, array[i]);
		}

		return ll;
	}


	export function LinkedListNumbersEqual(a : LinkedListNumbers, b : LinkedListNumbers) : boolean{
		var equal : boolean, done : boolean;
		var an : LinkedListNodeNumbers, bn : LinkedListNodeNumbers;

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


	export function CreateLinkedListCharacter() : LinkedListCharacters{
		var ll : LinkedListCharacters;

		ll = new LinkedListCharacters();
		ll.first = new LinkedListNodeCharacters();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}


	export function LinkedListAddCharacter(ll : LinkedListCharacters, value : string) : void{
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeCharacters();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}


	export function LinkedListCharactersToArray(ll : LinkedListCharacters) : string []{
		var array : string [];
		var length : number, i : number;
		var node : LinkedListNodeCharacters;

		node = ll.first;

		length = LinkedListCharactersLength(ll);

		array = new Array<string>(length);

		for(i = 0; i < length; i = i + 1){
			array[i] = node.value;
			node = node.next;
		}

		return array;
	}


	export function LinkedListCharactersLength(ll : LinkedListCharacters) : number{
		var l : number;
		var node : LinkedListNodeCharacters;

		l = 0;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1;
		}

		return l;
	}


	export function FreeLinkedListCharacter(ll : LinkedListCharacters) : void{
		var node : LinkedListNodeCharacters, prev : LinkedListNodeCharacters;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			prev = undefined;
		}

		node = undefined;
	}


	export function LinkedListCharactersAddString(ll : LinkedListCharacters, str : string []) : void{
		var i : number;

		for(i = 0; i < str.length; i = i + 1){
			LinkedListAddCharacter(ll, str[i]);
		}
	}


	export function CreateDynamicArrayNumbers() : DynamicArrayNumbers{
		var da : DynamicArrayNumbers;

		da = new DynamicArrayNumbers();
		da.array = new Array<number>(10);
		da.length = 0;

		return da;
	}


	export function CreateDynamicArrayNumbersWithInitialCapacity(capacity : number) : DynamicArrayNumbers{
		var da : DynamicArrayNumbers;

		da = new DynamicArrayNumbers();
		da.array = new Array<number>(capacity);
		da.length = 0;

		return da;
	}


	export function DynamicArrayAddNumber(da : DynamicArrayNumbers, value : number) : void{
		if(da.length == da.array.length){
			DynamicArrayNumbersIncreaseSize(da);
		}

		da.array[da.length] = value;
		da.length = da.length + 1;
	}


	export function DynamicArrayNumbersIncreaseSize(da : DynamicArrayNumbers) : void{
		var newLength : number, i : number;
		var newArray : number [];

		newLength = Math.round(da.array.length*3/2);
		newArray = new Array<number>(newLength);

		for(i = 0; i < da.array.length; i = i + 1){
			newArray[i] = da.array[i];
		}

		delete da.array;

		da.array = newArray;
	}


	export function DynamicArrayNumbersDecreaseSizeNecessary(da : DynamicArrayNumbers) : boolean{
		var needsDecrease : boolean;

		needsDecrease = false;

		if(da.length > 10){
			needsDecrease = da.length <= Math.round(da.array.length*2/3);
		}

		return needsDecrease;
	}


	export function DynamicArrayNumbersDecreaseSize(da : DynamicArrayNumbers) : void{
		var newLength : number, i : number;
		var newArray : number [];

		newLength = Math.round(da.array.length*2/3);
		newArray = new Array<number>(newLength);

		for(i = 0; i < newLength; i = i + 1){
			newArray[i] = da.array[i];
		}

		delete da.array;

		da.array = newArray;
	}


	export function DynamicArrayNumbersIndex(da : DynamicArrayNumbers, index : number) : number{
		return da.array[index];
	}


	export function DynamicArrayNumbersLength(da : DynamicArrayNumbers) : number{
		return da.length;
	}


	export function DynamicArrayInsertNumber(da : DynamicArrayNumbers, index : number, value : number) : void{
		var i : number;

		if(da.length == da.array.length){
			DynamicArrayNumbersIncreaseSize(da);
		}

		for(i = da.length; i > index; i = i - 1){
			da.array[i] = da.array[i - 1];
		}

		da.array[index] = value;

		da.length = da.length + 1;
	}


	export function DynamicArrayNumberSet(da : DynamicArrayNumbers, index : number, value : number) : boolean{
		var success : boolean;

		if(index < da.length){
			da.array[index] = value;
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function DynamicArrayRemoveNumber(da : DynamicArrayNumbers, index : number) : void{
		var i : number;

		for(i = index; i < da.length - 1; i = i + 1){
			da.array[i] = da.array[i + 1];
		}

		da.length = da.length - 1;

		if(DynamicArrayNumbersDecreaseSizeNecessary(da)){
			DynamicArrayNumbersDecreaseSize(da);
		}
	}


	export function FreeDynamicArrayNumbers(da : DynamicArrayNumbers) : void{
		delete da.array;
		da = undefined;
	}


	export function DynamicArrayNumbersToArray(da : DynamicArrayNumbers) : number []{
		var array : number [];
		var i : number;

		array = new Array<number>(da.length);

		for(i = 0; i < da.length; i = i + 1){
			array[i] = da.array[i];
		}

		return array;
	}


	export function ArrayToDynamicArrayNumbersWithOptimalSize(array : number []) : DynamicArrayNumbers{
		var da : DynamicArrayNumbers;
		var i : number;
		var c : number, n : number, newCapacity : number;

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


	export function ArrayToDynamicArrayNumbers(array : number []) : DynamicArrayNumbers{
		var da : DynamicArrayNumbers;

		da = new DynamicArrayNumbers();
		da.array = CopyNumberArray(array);
		da.length = array.length;

		return da;
	}


	export function DynamicArrayNumbersEqual(a : DynamicArrayNumbers, b : DynamicArrayNumbers) : boolean{
		var equal : boolean;
		var i : number;

		equal = true;
		if(a.length == b.length){
			for(i = 0; i < a.length && equal; i = i + 1){
				if(a.array[i] != b.array[i]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}


	export function DynamicArrayNumbersToLinkedList(da : DynamicArrayNumbers) : LinkedListNumbers{
		var ll : LinkedListNumbers;
		var i : number;

		ll = CreateLinkedListNumbers();

		for(i = 0; i < da.length; i = i + 1){
			LinkedListAddNumber(ll, da.array[i]);
		}

		return ll;
	}


	export function LinkedListToDynamicArrayNumbers(ll : LinkedListNumbers) : DynamicArrayNumbers{
		var da : DynamicArrayNumbers;
		var i : number;
		var node : LinkedListNodeNumbers;

		node = ll.first;

		da = new DynamicArrayNumbers();
		da.length = LinkedListNumbersLength(ll);

		da.array = new Array<number>(da.length);

		for(i = 0; i < da.length; i = i + 1){
			da.array[i] = node.value;
			node = node.next;
		}

		return da;
	}


	export function DynamicArrayNumbersIndexOf(arr : DynamicArrayNumbers, n : number, foundReference : BooleanReference) : number{
		var found : boolean;
		var i : number;

		found = false;
		for(i = 0; i < arr.length && !found; i = i + 1){
			if(arr.array[i] == n){
				found = true;
			}
		}
		if(!found){
			i = -1;
		}else{
			i = i - 1;
		}

		foundReference.booleanValue = found;

		return i;
	}


	export function DynamicArrayNumbersIsInArray(arr : DynamicArrayNumbers, n : number) : boolean{
		var found : boolean;
		var i : number;

		found = false;
		for(i = 0; i < arr.length && !found; i = i + 1){
			if(arr.array[i] == n){
				found = true;
			}
		}

		return found;
	}


	export function AddCharacter(list : string [], a : string) : string []{
		var newlist : string [];
		var i : number;

		newlist = new Array<string>(list.length + 1);
		for(i = 0; i < list.length; i = i + 1){
			newlist[i] = list[i];
		}
		newlist[list.length] = a;
		
		list = undefined;
		
		return newlist;
	}


	export function AddCharacterRef(list : StringReference, i : string) : void{
		list.stringx = AddCharacter(list.stringx, i);
	}


	export function RemoveCharacter(list : string [], n : number) : string []{
		var newlist : string [];
		var i : number;

		newlist = new Array<string>(list.length - 1);

		if(n >= 0 && n < list.length){
			for(i = 0; i < list.length; i = i + 1){
				if(i < n){
					newlist[i] = list[i];
				}
				if(i > n){
					newlist[i - 1] = list[i];
				}
			}

			list = undefined;
		}else{
			newlist = undefined;
		}

		return newlist;
	}


	export function GetCharacterRef(list : StringReference, i : number) : string{
		return list.stringx[i];
	}


	export function RemoveCharacterRef(list : StringReference, i : number) : void{
		list.stringx = RemoveCharacter(list.stringx, i);
	}


	export function WriteStringToStingStream(stream : string [], index : NumberReference, src : string []) : void{
		var i : number;

		for(i = 0; i < src.length; i = i + 1){
			stream[index.numberValue + i] = src[i];
		}
		index.numberValue = index.numberValue + src.length;
	}


	export function WriteCharacterToStingStream(stream : string [], index : NumberReference, src : string) : void{
		stream[index.numberValue] = src;
		index.numberValue = index.numberValue + 1;
	}


	export function WriteBooleanToStingStream(stream : string [], index : NumberReference, src : boolean) : void{
		if(src){
			WriteStringToStingStream(stream, index, "true".split(''));
		}else{
			WriteStringToStingStream(stream, index, "false".split(''));
		}
	}


	export function SubstringWithCheck(stringx : string [], fromx : number, to : number, stringReference : StringReference) : boolean{
		var success : boolean;

		if(fromx >= 0 && fromx <= stringx.length && to >= 0 && to <= stringx.length && fromx <= to){
			stringReference.stringx = Substring(stringx, fromx, to);
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function Substring(stringx : string [], fromx : number, to : number) : string []{
		var n : string [];
		var i : number, length : number;

		length = to - fromx;

		n = new Array<string>(length);

		for(i = fromx; i < to; i = i + 1){
			n[i - fromx] = stringx[i];
		}

		return n;
	}


	export function AppendString(s1 : string [], s2 : string []) : string []{
		var newString : string [];

		newString = ConcatenateString(s1, s2);

		s1 = undefined;

		return newString;
	}


	export function ConcatenateString(s1 : string [], s2 : string []) : string []{
		var newString : string [];
		var i : number;

		newString = new Array<string>(s1.length + s2.length);

		for(i = 0; i < s1.length; i = i + 1){
			newString[i] = s1[i];
		}

		for(i = 0; i < s2.length; i = i + 1){
			newString[s1.length + i] = s2[i];
		}

		return newString;
	}


	export function AppendCharacter(stringx : string [], c : string) : string []{
		var newString : string [];

		newString = ConcatenateCharacter(stringx, c);

		stringx = undefined;

		return newString;
	}


	export function ConcatenateCharacter(stringx : string [], c : string) : string []{
		var newString : string [];
		var i : number;
		newString = new Array<string>(stringx.length + 1);

		for(i = 0; i < stringx.length; i = i + 1){
			newString[i] = stringx[i];
		}

		newString[stringx.length] = c;

		return newString;
	}


	export function SplitByCharacter(toSplit : string [], splitBy : string) : StringReference []{
		var split : StringReference [];
		var stringToSplitBy : string [];

		stringToSplitBy = new Array<string>(1);
		stringToSplitBy[0] = splitBy;

		split = SplitByString(toSplit, stringToSplitBy);

		stringToSplitBy = undefined;

		return split;
	}


	export function IndexOfCharacter(stringx : string [], character : string, indexReference : NumberReference) : boolean{
		var i : number;
		var found : boolean;

		found = false;
		for(i = 0; i < stringx.length && !found; i = i + 1){
			if(stringx[i] == character){
				found = true;
				indexReference.numberValue = i;
			}
		}

		return found;
	}


	export function SubstringEqualsWithCheck(stringx : string [], fromx : number, substring : string [], equalsReference : BooleanReference) : boolean{
		var success : boolean;

		if(fromx < stringx.length){
			success = true;
			equalsReference.booleanValue = SubstringEquals(stringx, fromx, substring);
		}else{
			success = false;
		}

		return success;
	}


	export function SubstringEquals(stringx : string [], fromx : number, substring : string []) : boolean{
		var i : number;
		var equal : boolean;

		equal = true;
		if(stringx.length - fromx >= substring.length){
			for(i = 0; i < substring.length && equal; i = i + 1){
				if(stringx[fromx + i] != substring[i]){
					equal = false;
				}
			}
		}else{
			equal = false;
		}

		return equal;
	}


	export function IndexOfString(stringx : string [], substring : string [], indexReference : NumberReference) : boolean{
		var i : number;
		var found : boolean;

		found = false;
		for(i = 0; i < stringx.length - substring.length + 1 && !found; i = i + 1){
			if(SubstringEquals(stringx, i, substring)){
				found = true;
				indexReference.numberValue = i;
			}
		}

		return found;
	}


	export function ContainsCharacter(stringx : string [], character : string) : boolean{
		var i : number;
		var found : boolean;

		found = false;
		for(i = 0; i < stringx.length && !found; i = i + 1){
			if(stringx[i] == character){
				found = true;
			}
		}

		return found;
	}


	export function ContainsString(stringx : string [], substring : string []) : boolean{
		return IndexOfString(stringx, substring, new NumberReference());
	}


	export function ToUpperCase(stringx : string []) : void{
		var i : number;

		for(i = 0; i < stringx.length; i = i + 1){
			stringx[i] = charToUpperCase(stringx[i]);
		}
	}


	export function ToLowerCase(stringx : string []) : void{
		var i : number;

		for(i = 0; i < stringx.length; i = i + 1){
			stringx[i] = charToLowerCase(stringx[i]);
		}
	}


	export function EqualsIgnoreCase(a : string [], b : string []) : boolean{
		var equal : boolean;
		var i : number;

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


	export function ReplaceString(stringx : string [], toReplace : string [], replaceWith : string []) : string []{
		var result : string [];
		var i : number, j : number;
		var equalsReference : BooleanReference;
		var success : boolean;
		var da : DynamicArrayCharacters;

		da = CreateDynamicArrayCharacters();

		equalsReference = new BooleanReference();

		for(i = 0; i < stringx.length; ){
			success = SubstringEqualsWithCheck(stringx, i, toReplace, equalsReference);
			if(success){
				success = equalsReference.booleanValue;
			}

			if(success && toReplace.length > 0){
				for(j = 0; j < replaceWith.length; j = j + 1){
					DynamicArrayAddCharacter(da, replaceWith[j]);
				}
				i = i + toReplace.length;
			}else{
				DynamicArrayAddCharacter(da, stringx[i]);
				i = i + 1;
			}
		}

		result = DynamicArrayCharactersToArray(da);

		FreeDynamicArrayCharacters(da);

		return result;
	}


	export function ReplaceCharacterToNew(stringx : string [], toReplace : string, replaceWith : string) : string []{
		var result : string [];
		var i : number;

		result = new Array<string>(stringx.length);

		for(i = 0; i < stringx.length; i = i + 1){
			if(stringx[i] == toReplace){
				result[i] = replaceWith;
			}else{
				result[i] = stringx[i];
			}
		}

		return result;
	}


	export function ReplaceCharacter(stringx : string [], toReplace : string, replaceWith : string) : void{
		var i : number;

		for(i = 0; i < stringx.length; i = i + 1){
			if(stringx[i] == toReplace){
				stringx[i] = replaceWith;
			}
		}
	}


	export function Trim(stringx : string []) : string []{
		var result : string [];
		var i : number, lastWhitespaceLocationStart : number, lastWhitespaceLocationEnd : number;
		var firstNonWhitespaceFound : boolean;

		/* Find whitepaces at the start.*/
		lastWhitespaceLocationStart = -1;
		firstNonWhitespaceFound = false;
		for(i = 0; i < stringx.length && !firstNonWhitespaceFound; i = i + 1){
			if(charIsWhiteSpace(stringx[i])){
				lastWhitespaceLocationStart = i;
			}else{
				firstNonWhitespaceFound = true;
			}
		}

		/* Find whitepaces at the end.*/
		lastWhitespaceLocationEnd = stringx.length;
		firstNonWhitespaceFound = false;
		for(i = stringx.length - 1; i >= 0 && !firstNonWhitespaceFound; i = i - 1){
			if(charIsWhiteSpace(stringx[i])){
				lastWhitespaceLocationEnd = i;
			}else{
				firstNonWhitespaceFound = true;
			}
		}

		if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
			result = Substring(stringx, lastWhitespaceLocationStart + 1, lastWhitespaceLocationEnd);
		}else{
			result = new Array<string>(0);
		}

		return result;
	}


	export function StartsWith(stringx : string [], start : string []) : boolean{
		var startsWithString : boolean;

		startsWithString = false;
		if(stringx.length >= start.length){
			startsWithString = SubstringEquals(stringx, 0, start);
		}

		return startsWithString;
	}


	export function EndsWith(stringx : string [], end : string []) : boolean{
		var endsWithString : boolean;

		endsWithString = false;
		if(stringx.length >= end.length){
			endsWithString = SubstringEquals(stringx, stringx.length - end.length, end);
		}

		return endsWithString;
	}


	export function SplitByString(toSplit : string [], splitBy : string []) : StringReference []{
		var split : StringReference [];
		var next : string [];
		var i : number;
		var c : string;
		var n : StringReference;

		split = new Array<StringReference>(0);

		next = new Array<string>(0);
		for(i = 0; i < toSplit.length; ){
			c = toSplit[i];

			if(SubstringEquals(toSplit, i, splitBy)){
				n = new StringReference();
				n.stringx = next;
				split = AddString(split, n);
				next = new Array<string>(0);
				i = i + splitBy.length;
			}else{
				next = AppendCharacter(next, c);
				i = i + 1;
			}
		}

		n = new StringReference();
		n.stringx = next;
		split = AddString(split, n);

		return split;
	}


	export function StringIsBefore(a : string [], b : string []) : boolean{
		var before : boolean, equal : boolean, done : boolean;
		var i : number;

		before = false;
		equal = true;
		done = false;

		if(a.length == 0 && b.length > 0){
			before = true;
		}else{
			for(i = 0; i < a.length && i < b.length && !done; i = i + 1){
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


	export function JoinStringsWithSeparator(strings : StringReference [], separator : string []) : string []{
		var result : string [], stringx : string [];
		var length : number, i : number;
		var index : NumberReference;

		index = CreateNumberReference(0);

		length = 0;
		for(i = 0; i < strings.length; i = i + 1){
			length = length + strings[i].stringx.length;
		}
		length = length + (strings.length - 1)*separator.length;

		result = new Array<string>(length);

		for(i = 0; i < strings.length; i = i + 1){
			stringx = strings[i].stringx;
			WriteStringToStingStream(result, index, stringx);
			if(i + 1 < strings.length){
				WriteStringToStingStream(result, index, separator);
			}
		}

		index = undefined;

		return result;
	}


	export function JoinStrings(strings : StringReference []) : string []{
		var result : string [], stringx : string [];
		var length : number, i : number;
		var index : NumberReference;

		index = CreateNumberReference(0);

		length = 0;
		for(i = 0; i < strings.length; i = i + 1){
			length = length + strings[i].stringx.length;
		}

		result = new Array<string>(length);

		for(i = 0; i < strings.length; i = i + 1){
			stringx = strings[i].stringx;
			WriteStringToStingStream(result, index, stringx);
		}

		index = undefined;

		return result;
	}


	export function StringToNumberArray(stringx : string []) : number []{
		var i : number;
		var array : number [];

		array = new Array<number>(stringx.length);

		for(i = 0; i < stringx.length; i = i + 1){
			array[i] = stringx[i].charCodeAt(0);
		}
		return array;
	}


	export function NumberArrayToString(array : number []) : string []{
		var i : number;
		var stringx : string [];

		stringx = new Array<string>(array.length);

		for(i = 0; i < array.length; i = i + 1){
			stringx[i] = String.fromCharCode(array[i]);
		}
		return stringx;
	}


	export function NumberArraysEqual(a : number [], b : number []) : boolean{
		var equal : boolean;
		var i : number;

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


	export function BooleanArraysEqual(a : boolean [], b : boolean []) : boolean{
		var equal : boolean;
		var i : number;

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


	export function StringsEqual(a : string [], b : string []) : boolean{
		var equal : boolean;
		var i : number;

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


	export function FillNumberArray(a : number [], value : number) : void{
		var i : number;

		for(i = 0; i < a.length; i = i + 1){
			a[i] = value;
		}
	}


	export function FillString(a : string [], value : string) : void{
		var i : number;

		for(i = 0; i < a.length; i = i + 1){
			a[i] = value;
		}
	}


	export function FillBooleanArray(a : boolean [], value : boolean) : void{
		var i : number;

		for(i = 0; i < a.length; i = i + 1){
			a[i] = value;
		}
	}


	export function FillNumberArrayRange(a : number [], value : number, fromx : number, to : number) : boolean{
		var i : number, length : number;
		var success : boolean;

		if(fromx >= 0 && fromx <= a.length && to >= 0 && to <= a.length && fromx <= to){
			length = to - fromx;
			for(i = 0; i < length; i = i + 1){
				a[fromx + i] = value;
			}

			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function FillBooleanArrayRange(a : boolean [], value : boolean, fromx : number, to : number) : boolean{
		var i : number, length : number;
		var success : boolean;

		if(fromx >= 0 && fromx <= a.length && to >= 0 && to <= a.length && fromx <= to){
			length = to - fromx;
			for(i = 0; i < length; i = i + 1){
				a[fromx + i] = value;
			}

			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function FillStringRange(a : string [], value : string, fromx : number, to : number) : boolean{
		var i : number, length : number;
		var success : boolean;

		if(fromx >= 0 && fromx <= a.length && to >= 0 && to <= a.length && fromx <= to){
			length = to - fromx;
			for(i = 0; i < length; i = i + 1){
				a[fromx + i] = value;
			}

			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function CopyNumberArray(a : number []) : number []{
		var i : number;
		var n : number [];

		n = new Array<number>(a.length);

		for(i = 0; i < a.length; i = i + 1){
			n[i] = a[i];
		}

		return n;
	}


	export function CopyBooleanArray(a : boolean []) : boolean []{
		var i : number;
		var n : boolean [];

		n = new Array<boolean>(a.length);

		for(i = 0; i < a.length; i = i + 1){
			n[i] = a[i];
		}

		return n;
	}


	export function CopyString(a : string []) : string []{
		var i : number;
		var n : string [];

		n = new Array<string>(a.length);

		for(i = 0; i < a.length; i = i + 1){
			n[i] = a[i];
		}

		return n;
	}


	export function CopyNumberArrayRange(a : number [], fromx : number, to : number, copyReference : NumberArrayReference) : boolean{
		var i : number, length : number;
		var n : number [];
		var success : boolean;

		if(fromx >= 0 && fromx <= a.length && to >= 0 && to <= a.length && fromx <= to){
			length = to - fromx;
			n = new Array<number>(length);

			for(i = 0; i < length; i = i + 1){
				n[i] = a[fromx + i];
			}

			copyReference.numberArray = n;
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function CopyBooleanArrayRange(a : boolean [], fromx : number, to : number, copyReference : BooleanArrayReference) : boolean{
		var i : number, length : number;
		var n : boolean [];
		var success : boolean;

		if(fromx >= 0 && fromx <= a.length && to >= 0 && to <= a.length && fromx <= to){
			length = to - fromx;
			n = new Array<boolean>(length);

			for(i = 0; i < length; i = i + 1){
				n[i] = a[fromx + i];
			}

			copyReference.booleanArray = n;
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function CopyStringRange(a : string [], fromx : number, to : number, copyReference : StringReference) : boolean{
		var i : number, length : number;
		var n : string [];
		var success : boolean;

		if(fromx >= 0 && fromx <= a.length && to >= 0 && to <= a.length && fromx <= to){
			length = to - fromx;
			n = new Array<string>(length);

			for(i = 0; i < length; i = i + 1){
				n[i] = a[fromx + i];
			}

			copyReference.stringx = n;
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function IsLastElement(length : number, index : number) : boolean{
		return index + 1 == length;
	}


	export function CreateNumberArray(length : number, value : number) : number []{
		var array : number [];

		array = new Array<number>(length);
		FillNumberArray(array, value);

		return array;
	}


	export function CreateBooleanArray(length : number, value : boolean) : boolean []{
		var array : boolean [];

		array = new Array<boolean>(length);
		FillBooleanArray(array, value);

		return array;
	}


	export function CreateString(length : number, value : string) : string []{
		var array : string [];

		array = new Array<string>(length);
		FillString(array, value);

		return array;
	}


	export function SwapElementsOfNumberArray(A : number [], ai : number, bi : number) : void{
		var tmp : number;

		tmp = A[ai];
		A[ai] = A[bi];
		A[bi] = tmp;
	}


	export function SwapElementsOfStringArray(A : StringArrayReference, ai : number, bi : number) : void{
		var tmp : StringReference;

		tmp = A.stringArray[ai];
		A.stringArray[ai] = A.stringArray[bi];
		A.stringArray[bi] = tmp;
	}


	export function ReverseNumberArray(array : number []) : void{
		var i : number;

		for(i = 0; i < array.length/2; i = i + 1){
			SwapElementsOfNumberArray(array, i, array.length - i - 1);
		}
	}


	export function AssertFalse(b : boolean, failures : NumberReference) : void{
		if(b){
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertTrue(b : boolean, failures : NumberReference) : void{
		if(!b){
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertEquals(a : number, b : number, failures : NumberReference) : void{
		if(a != b){
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertBooleansEqual(a : boolean, b : boolean, failures : NumberReference) : void{
		if(a != b){
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertCharactersEqual(a : string, b : string, failures : NumberReference) : void{
		if(a != b){
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertStringEquals(a : string [], b : string [], failures : NumberReference) : void{
		if(!StringsEqual(a, b)){
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertNumberArraysEqual(a : number [], b : number [], failures : NumberReference) : void{
		var i : number;

		if(a.length == b.length){
			for(i = 0; i < a.length; i = i + 1){
				AssertEquals(a[i], b[i], failures);
			}
		}else{
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertBooleanArraysEqual(a : boolean [], b : boolean [], failures : NumberReference) : void{
		var i : number;

		if(a.length == b.length){
			for(i = 0; i < a.length; i = i + 1){
				AssertBooleansEqual(a[i], b[i], failures);
			}
		}else{
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function AssertStringArraysEqual(a : StringReference [], b : StringReference [], failures : NumberReference) : void{
		var i : number;

		if(a.length == b.length){
			for(i = 0; i < a.length; i = i + 1){
				AssertStringEquals(a[i].stringx, b[i].stringx, failures);
			}
		}else{
			failures.numberValue = failures.numberValue + 1;
		}
	}


	export function nCreateStringScientificNotationDecimalFromNumber(decimal : number) : string []{
		var mantissaReference : StringReference, exponentReference : StringReference;
		var multiplier : number, inc : number;
		var exponent : number;
		var done : boolean, isPositive : boolean;
		var result : string [];

		mantissaReference = new StringReference();
		exponentReference = new StringReference();
		result = new Array<string>(0);
		done = false;
		exponent = 0;

		if(decimal < 0){
			isPositive = false;
			decimal = -decimal;
		}else{
			isPositive = true;
		}

		if(decimal == 0){
			done = true;
		}

		if(!done){
			multiplier = 0;
			inc = 0;

			if(decimal < 1){
				multiplier = 10;
				inc = -1;
			}else if(decimal >= 10){
				multiplier = 0.1;
				inc = 1;
			}else{
				done = true;
			}

			if(!done){
				exponent = Math.round(Math.log10(decimal));
				exponent = Math.min(99, exponent);
				exponent = Math.max(-99, exponent);

				decimal = decimal/10**exponent;

				/* Adjust*/
				for(; (decimal >= 10 || decimal < 1) && Math.abs(exponent) < 99; ){
					decimal = decimal*multiplier;
					exponent = exponent + inc;
				}
			}
		}

		nCreateStringFromNumberWithCheck(decimal, 10, mantissaReference);

		nCreateStringFromNumberWithCheck(exponent, 10, exponentReference);

		if(!isPositive){
			result = AppendString(result, "-".split(''));
		}

		result = AppendString(result, mantissaReference.stringx);
		result = AppendString(result, "e".split(''));
		result = AppendString(result, exponentReference.stringx);

		return result;
	}


	export function nCreateStringDecimalFromNumber(decimal : number) : string []{
		var stringReference : StringReference;

		stringReference = new StringReference();

		/* This will succeed because base = 10.*/
		nCreateStringFromNumberWithCheck(decimal, 10, stringReference);

		return stringReference.stringx;
	}


	export function nCreateStringFromNumberWithCheck(decimal : number, base : number, stringReference : StringReference) : boolean{
		var stringx : DynamicArrayCharacters;
		var maximumDigits : number;
		var digitPosition : number;
		var hasPrintedPoint : boolean, isPositive : boolean;
		var i : number, d : number;
		var success : boolean;
		var characterReference : CharacterReference;
		var c : string;

		stringx = CreateDynamicArrayCharacters();
		isPositive = true;

		if(decimal < 0){
			isPositive = false;
			decimal = -decimal;
		}

		if(decimal == 0){
			DynamicArrayAddCharacter(stringx, '0');
			success = true;
		}else{
			characterReference = new CharacterReference();

			if(IsInteger(base)){
				success = true;

				maximumDigits = nGetMaximumDigitsForBase(base);

				digitPosition = nGetFirstDigitPosition(decimal, base);

				decimal = Math.round(decimal*base**(maximumDigits - digitPosition - 1));

				hasPrintedPoint = false;

				if(!isPositive){
					DynamicArrayAddCharacter(stringx, '-');
				}

				/* Print leading zeros.*/
				if(digitPosition < 0){
					DynamicArrayAddCharacter(stringx, '0');
					DynamicArrayAddCharacter(stringx, '.');
					hasPrintedPoint = true;
					for(i = 0; i < -digitPosition - 1; i = i + 1){
						DynamicArrayAddCharacter(stringx, '0');
					}
				}

				/* Print number.*/
				for(i = 0; i < maximumDigits && success; i = i + 1){
					d = Math.floor(decimal/base**(maximumDigits - i - 1));

					if(d >= base){
						d = base - 1;
					}

					if(!hasPrintedPoint && digitPosition - i + 1 == 0){
						if(decimal != 0){
							DynamicArrayAddCharacter(stringx, '.');
						}
						hasPrintedPoint = true;
					}

					if(decimal == 0 && hasPrintedPoint){
					}else{
						success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
						if(success){
							c = characterReference.characterValue;
							DynamicArrayAddCharacter(stringx, c);
						}
					}

					if(success){
						decimal = decimal - d*base**(maximumDigits - i - 1);
						decimal = Math.max(decimal, 0);
						decimal = Math.round(decimal);
					}
				}

				if(success){
					/* Print trailing zeros.*/
					for(i = 0; i < digitPosition - maximumDigits + 1; i = i + 1){
						DynamicArrayAddCharacter(stringx, '0');
					}
				}
			}else{
				success = false;
			}
		}

		if(success){
			stringReference.stringx = DynamicArrayCharactersToArray(stringx);
			FreeDynamicArrayCharacters(stringx);
		}

		/* Done*/
		return success;
	}


	export function nGetMaximumDigitsForBase(base : number) : number{
		var t : number;

		t = 10**15;
		return Math.floor(Math.log10(t)/Math.log10(base));
	}


	export function nGetFirstDigitPosition(decimal : number, base : number) : number{
		var power : number;
		var t : number;

		power = Math.ceil(Math.log10(decimal)/Math.log10(base));

		t = decimal*base**(-power);
		if(t < base && t >= 1){
		}else if(t >= base){
			power = power + 1;
		}else if(t < 1){
			power = power - 1;
		}

		return power;
	}


	export function nGetSingleDigitCharacterFromNumberWithCheck(c : number, base : number, characterReference : CharacterReference) : boolean{
		var numberTable : string [];
		var success : boolean;

		numberTable = nGetDigitCharacterTable();

		if(c < base || c < numberTable.length){
			success = true;
			characterReference.characterValue = numberTable[c];
		}else{
			success = false;
		}

		return success;
	}


	export function nGetDigitCharacterTable() : string []{
		var numberTable : string [];

		numberTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".split('');

		return numberTable;
	}


	export function nCreateNumberFromDecimalStringWithCheck(stringx : string [], decimalReference : NumberReference, errorMessage : StringReference) : boolean{
		return nCreateNumberFromStringWithCheck(stringx, 10, decimalReference, errorMessage);
	}


	export function nCreateNumberFromDecimalString(stringx : string []) : number{
		var doubleReference : NumberReference;
		var stringReference : StringReference;
		var numberx : number;

		doubleReference = CreateNumberReference(0);
		stringReference = CreateStringReference("".split(''));
		nCreateNumberFromStringWithCheck(stringx, 10, doubleReference, stringReference);
		numberx = doubleReference.numberValue;

		doubleReference = undefined;
		stringReference = undefined;

		return numberx;
	}


	export function nCreateNumberFromStringWithCheck(stringx : string [], base : number, numberReference : NumberReference, errorMessage : StringReference) : boolean{
		var success : boolean;
		var numberIsPositive : BooleanReference, exponentIsPositive : BooleanReference;
		var beforePoint : NumberArrayReference, afterPoint : NumberArrayReference, exponent : NumberArrayReference;

		numberIsPositive = CreateBooleanReference(true);
		exponentIsPositive = CreateBooleanReference(true);
		beforePoint = new NumberArrayReference();
		afterPoint = new NumberArrayReference();
		exponent = new NumberArrayReference();

		if(base >= 2 && base <= 36){
			success = nExtractPartsFromNumberString(stringx, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage);

			if(success){
				numberReference.numberValue = nCreateNumberFromParts(base, numberIsPositive.booleanValue, beforePoint.numberArray, afterPoint.numberArray, exponentIsPositive.booleanValue, exponent.numberArray);
			}
		}else{
			success = false;
			errorMessage.stringx = "Base must be from 2 to 36.".split('');
		}

		return success;
	}


	export function nCreateNumberFromParts(base : number, numberIsPositive : boolean, beforePoint : number [], afterPoint : number [], exponentIsPositive : boolean, exponent : number []) : number{
		var n : number, i : number, p : number, e : number;

		n = 0;

		for(i = 0; i < beforePoint.length; i = i + 1){
			p = beforePoint[beforePoint.length - i - 1];

			n = n + p*base**i;
		}

		for(i = 0; i < afterPoint.length; i = i + 1){
			p = afterPoint[i];

			n = n + p*base**(-(i + 1));
		}

		if(exponent.length > 0){
			e = 0;
			for(i = 0; i < exponent.length; i = i + 1){
				p = exponent[exponent.length - i - 1];

				e = e + p*base**i;
			}

			if(!exponentIsPositive){
				e = -e;
			}

			n = n*base**e;
		}

		if(!numberIsPositive){
			n = -n;
		}

		return n;
	}


	export function nExtractPartsFromNumberString(n : string [], base : number, numberIsPositive : BooleanReference, beforePoint : NumberArrayReference, afterPoint : NumberArrayReference, exponentIsPositive : BooleanReference, exponent : NumberArrayReference, errorMessages : StringReference) : boolean{
		var i : number, j : number, count : number;
		var success : boolean, done : boolean, complete : boolean;

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
			errorMessages.stringx = "Number cannot have length zero.".split('');
		}

		if(success){
			done = false;
			count = 0;
			for(; i + count < n.length && !done; ){
				if(nCharacterIsNumberCharacterInBase(n[i + count], base)){
					count = count + 1;
				}else{
					done = true;
				}
			}

			if(count >= 1){
				beforePoint.numberArray = new Array<number>(count);

				for(j = 0; j < count; j = j + 1){
					beforePoint.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base);
				}

				i = i + count;

				if(i < n.length){
					success = true;
				}else{
					afterPoint.numberArray = new Array<number>(0);
					exponent.numberArray = new Array<number>(0);
					success = true;
					complete = true;
				}
			}else{
				success = false;
				errorMessages.stringx = "Number must have at least one number after the optional sign.".split('');
			}
		}

		if(success && !complete){
			if(n[i] == '.'){
				i = i + 1;

				if(i < n.length){
					done = false;
					count = 0;
					for(; i + count < n.length && !done; ){
						if(nCharacterIsNumberCharacterInBase(n[i + count], base)){
							count = count + 1;
						}else{
							done = true;
						}
					}

					if(count >= 1){
						afterPoint.numberArray = new Array<number>(count);

						for(j = 0; j < count; j = j + 1){
							afterPoint.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base);
						}

						i = i + count;

						if(i < n.length){
							success = true;
						}else{
							exponent.numberArray = new Array<number>(0);
							success = true;
							complete = true;
						}
					}else{
						success = false;
						errorMessages.stringx = "There must be at least one digit after the decimal point.".split('');
					}
				}else{
					success = false;
					errorMessages.stringx = "There must be at least one digit after the decimal point.".split('');
				}
			}else if(base <= 14 && (n[i] == 'e' || n[i] == 'E')){
				if(i < n.length){
					success = true;
					afterPoint.numberArray = new Array<number>(0);
				}else{
					success = false;
					errorMessages.stringx = "There must be at least one digit after the exponent.".split('');
				}
			}else{
				success = false;
				errorMessages.stringx = "Expected decimal point or exponent symbol.".split('');
			}
		}

		if(success && !complete){
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
						for(; i + count < n.length && !done; ){
							if(nCharacterIsNumberCharacterInBase(n[i + count], base)){
								count = count + 1;
							}else{
								done = true;
							}
						}

						if(count >= 1){
							exponent.numberArray = new Array<number>(count);

							for(j = 0; j < count; j = j + 1){
								exponent.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base);
							}

							i = i + count;

							if(i == n.length){
								success = true;
							}else{
								success = false;
								errorMessages.stringx = "There cannot be any characters past the exponent of the number.".split('');
							}
						}else{
							success = false;
							errorMessages.stringx = "There must be at least one digit after the decimal point.".split('');
						}
					}else{
						success = false;
						errorMessages.stringx = "There must be at least one digit after the exponent symbol.".split('');
					}
				}else{
					success = false;
					errorMessages.stringx = "There must be at least one digit after the exponent symbol.".split('');
				}
			}else{
				success = false;
				errorMessages.stringx = "Expected exponent symbol.".split('');
			}
		}

		return success;
	}


	export function nGetNumberFromNumberCharacterForBase(c : string, base : number) : number{
		var numberTable : string [];
		var i : number;
		var position : number;

		numberTable = nGetDigitCharacterTable();
		position = 0;

		for(i = 0; i < base; i = i + 1){
			if(numberTable[i] == c){
				position = i;
			}
		}

		return position;
	}


	export function nCharacterIsNumberCharacterInBase(c : string, base : number) : boolean{
		var numberTable : string [];
		var i : number;
		var found : boolean;

		numberTable = nGetDigitCharacterTable();
		found = false;

		for(i = 0; i < base; i = i + 1){
			if(numberTable[i] == c){
				found = true;
			}
		}

		return found;
	}


	export function nStringToNumberArray(str : string []) : number []{
		var numberArrayReference : NumberArrayReference;
		var stringReference : StringReference;
		var numbers : number [];

		numberArrayReference = new NumberArrayReference();
		stringReference = new StringReference();

		nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference);

		numbers = numberArrayReference.numberArray;

		numberArrayReference = undefined;
		stringReference = undefined;

		return numbers;
	}


	export function nStringToNumberArrayWithCheck(str : string [], numberArrayReference : NumberArrayReference, errorMessage : StringReference) : boolean{
		var numberStrings : StringReference [];
		var numbers : number [];
		var i : number;
		var numberString : string [], trimmedNumberString : string [];
		var success : boolean;
		var numberReference : NumberReference;

		numberStrings = SplitByString(str, ",".split(''));

		numbers = new Array<number>(numberStrings.length);
		success = true;
		numberReference = new NumberReference();

		for(i = 0; i < numberStrings.length; i = i + 1){
			numberString = numberStrings[i].stringx;
			trimmedNumberString = Trim(numberString);
			success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage);
			numbers[i] = numberReference.numberValue;

			FreeStringReference(numberStrings[i]);
			trimmedNumberString = undefined;
		}

		numberStrings = undefined;
		numberReference = undefined;

		numberArrayReference.numberArray = numbers;

		return success;
	}


	export function Negate(x : number) : number{
		return -x;
	}


	export function Positive(x : number) : number{
		return +x;
	}


	export function Factorial(x : number) : number{
		var i : number, f : number;

		f = 1;

		for(i = 2; i <= x; i = i + 1){
			f = f*i;
		}

		return f;
	}


	export function Round(x : number) : number{
		return Math.floor(x + 0.5);
	}


	export function BankersRound(x : number) : number{
		var r : number;

		if(Absolute(x - Truncate(x)) == 0.5){
			if(!DivisibleBy(Round(x), 2)){
				r = Round(x) - 1;
			}else{
				r = Round(x);
			}
		}else{
			r = Round(x);
		}

		return r;
	}


	export function Ceil(x : number) : number{
		return Math.ceil(x);
	}


	export function Floor(x : number) : number{
		return Math.floor(x);
	}


	export function Truncate(x : number) : number{
		var t : number;

		if(x >= 0){
			t = Math.floor(x);
		}else{
			t = Math.ceil(x);
		}

		return t;
	}


	export function Absolute(x : number) : number{
		return Math.abs(x);
	}


	export function Logarithm(x : number) : number{
		return Math.log10(x);
	}


	export function NaturalLogarithm(x : number) : number{
		return Math.log(x);
	}


	export function Sin(x : number) : number{
		return Math.sin(x);
	}


	export function Cos(x : number) : number{
		return Math.cos(x);
	}


	export function Tan(x : number) : number{
		return Math.tan(x);
	}


	export function Asin(x : number) : number{
		return Math.asin(x);
	}


	export function Acos(x : number) : number{
		return Math.acos(x);
	}


	export function Atan(x : number) : number{
		return Math.atan(x);
	}


	export function Atan2(y : number, x : number) : number{
		var a : number;

		/* Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors.*/
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
			a = -Math.PI/2;
		}

		return a;
	}


	export function Squareroot(x : number) : number{
		return Math.sqrt(x);
	}


	export function Exp(x : number) : number{
		return Math.exp(x);
	}


	export function DivisibleBy(a : number, b : number) : boolean{
		return ((a%b) == 0);
	}


	export function Combinations(n : number, k : number) : number{
		var i : number, j : number, c : number;

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


	export function Permutations(n : number, k : number) : number{
		var i : number, c : number;

		c = 1;

		for(i = n - k + 1; i <= n; i = i + 1){
			c = c*i;
		}

		return c;
	}


	export function EpsilonCompare(a : number, b : number, epsilon : number) : boolean{
		return Math.abs(a - b) < epsilon;
	}


	export function GreatestCommonDivisor(a : number, b : number) : number{
		var t : number;

		for(; b != 0; ){
			t = b;
			b = a%b;
			a = t;
		}

		return a;
	}


	export function GCDWithSubtraction(a : number, b : number) : number{
		var g : number;

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


	export function IsInteger(a : number) : boolean{
		return (a - Math.floor(a)) == 0;
	}


	export function GreatestCommonDivisorWithCheck(a : number, b : number, gcdReference : NumberReference) : boolean{
		var success : boolean;
		var gcd : number;

		if(IsInteger(a) && IsInteger(b)){
			gcd = GreatestCommonDivisor(a, b);
			gcdReference.numberValue = gcd;
			success = true;
		}else{
			success = false;
		}

		return success;
	}


	export function LeastCommonMultiple(a : number, b : number) : number{
		var lcm : number;

		if(a > 0 && b > 0){
			lcm = Math.abs(a*b)/GreatestCommonDivisor(a, b);
		}else{
			lcm = 0;
		}

		return lcm;
	}


	export function Sign(a : number) : number{
		var s : number;

		if(a > 0){
			s = 1;
		}else if(a < 0){
			s = -1;
		}else{
			s = 0;
		}

		return s;
	}


	export function Max(a : number, b : number) : number{
		return Math.max(a, b);
	}


	export function Min(a : number, b : number) : number{
		return Math.min(a, b);
	}


	export function Power(a : number, b : number) : number{
		return a**b;
	}


	export function Gamma(x : number) : number{
		return LanczosApproximation(x);
	}


	export function LogGamma(x : number) : number{
		return Math.log(Gamma(x));
	}


	export function LanczosApproximation(z : number) : number{
		var p : number [];
		var i : number, y : number, t : number, x : number;

		p = new Array<number>(8);
		p[0] = 676.5203681218851;
		p[1] = -1259.1392167224028;
		p[2] = 771.32342877765313;
		p[3] = -176.61502916214059;
		p[4] = 12.507343278686905;
		p[5] = -0.13857109526572012;
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
			y = Math.sqrt(2*Math.PI)*t**(z + 0.5)*Math.exp(-t)*x;
		}

		return y;
	}


	export function Beta(x : number, y : number) : number{
		return Gamma(x)*Gamma(y)/Gamma(x + y);
	}


	export function Sinh(x : number) : number{
		return (Math.exp(x) - Math.exp(-x))/2;
	}


	export function Cosh(x : number) : number{
		return (Math.exp(x) + Math.exp(-x))/2;
	}


	export function Tanh(x : number) : number{
		return Sinh(x)/Cosh(x);
	}


	export function Cot(x : number) : number{
		return 1/Math.tan(x);
	}


	export function Sec(x : number) : number{
		return 1/Math.cos(x);
	}


	export function Csc(x : number) : number{
		return 1/Math.sin(x);
	}


	export function Coth(x : number) : number{
		return Cosh(x)/Sinh(x);
	}


	export function Sech(x : number) : number{
		return 1/Cosh(x);
	}


	export function Csch(x : number) : number{
		return 1/Sinh(x);
	}


	export function Errorx(x : number) : number{
		var y : number, t : number, tau : number, c1 : number, c2 : number, c3 : number, c4 : number, c5 : number, c6 : number, c7 : number, c8 : number, c9 : number, c10 : number;

		if(x == 0){
			y = 0;
		}else if(x < 0){
			y = -Errorx(-x);
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

			t = 1/(1 + 0.5*Math.abs(x));

			tau = t*Math.exp(-(x**2) + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))));

			y = 1 - tau;
		}

		return y;
	}


	export function ErrorInverse(x : number) : number{
		var y : number, a : number, t : number;

		a = (8*(Math.PI - 3))/(3*Math.PI*(4 - Math.PI));

		t = 2/(Math.PI*a) + Math.log(1 - x**2)/2;
		y = Sign(x)*Math.sqrt(Math.sqrt(t**2 - Math.log(1 - x**2)/a) - t);

		return y;
	}


	export function FallingFactorial(x : number, n : number) : number{
		var k : number, y : number;

		y = 1;

		for(k = 0; k <= n - 1; k = k + 1){
			y = y*(x - k);
		}

		return y;
	}


	export function RisingFactorial(x : number, n : number) : number{
		var k : number, y : number;

		y = 1;

		for(k = 0; k <= n - 1; k = k + 1){
			y = y*(x + k);
		}

		return y;
	}


	export function Hypergeometric(a : number, b : number, c : number, z : number, maxIterations : number, precision : number) : number{
		var y : number;

		if(Math.abs(z) >= 0.5){
			y = (1 - z)**(-a)*HypergeometricDirect(a, c - b, c, z/(z - 1), maxIterations, precision);
		}else{
			y = HypergeometricDirect(a, b, c, z, maxIterations, precision);
		}

		return y;
	}


	export function HypergeometricDirect(a : number, b : number, c : number, z : number, maxIterations : number, precision : number) : number{
		var y : number, yp : number, n : number;
		var done : boolean;

		y = 0;
		done = false;

		for(n = 0; n < maxIterations && !done; n = n + 1){
			yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*z**n/Factorial(n);
			if(Math.abs(yp) < precision){
				done = true;
			}
			y = y + yp;
		}

		return y;
	}


	export function BernouilliNumber(n : number) : number{
		return AkiyamaTanigawaAlgorithm(n);
	}


	export function AkiyamaTanigawaAlgorithm(n : number) : number{
		var m : number, j : number, B : number;
		var A : number [];

		A = new Array<number>(n + 1);

		for(m = 0; m <= n; m = m + 1){
			A[m] = 1/(m + 1);
			for(j = m; j >= 1; j = j - 1){
				A[j - 1] = j*(A[j - 1] - A[j]);
			}
		}

		B = A[0];

		A = undefined;

		return B;
	}


	export function charToLowerCase(character : string) : string{
		var toReturn : string;

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


	export function charToUpperCase(character : string) : string{
		var toReturn : string;

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


	export function charIsUpperCase(character : string) : boolean{
		var isUpper : boolean;

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


	export function charIsLowerCase(character : string) : boolean{
		var isLower : boolean;

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


	export function charIsLetter(character : string) : boolean{
		return charIsUpperCase(character) || charIsLowerCase(character);
	}


	export function charIsNumber(character : string) : boolean{
		var isNumberx : boolean;

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


	export function charIsWhiteSpace(character : string) : boolean{
		var isWhiteSpacex : boolean;

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


	export function charIsSymbol(character : string) : boolean{
		var isSymbolx : boolean;

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


	export function charCharacterIsBefore(a : string, b : string) : boolean{
		var ad : number, bd : number;

		ad = a.charCodeAt(0);
		bd = b.charCodeAt(0);

		return ad < bd;
	}


	export function charDecimalDigitToCharacter(digit : number) : string{
		var c : string;
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


	export function charCharacterToDecimalDigit(c : string) : number{
		var digit : number;

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



