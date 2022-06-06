
# Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

class LinkedListNodeNodes
	attr_accessor :endx
	attr_accessor :value
	attr_accessor :nextx
end
class LinkedListNodes
	attr_accessor :first
	attr_accessor :last
end
class Node
	attr_accessor :type
	attr_accessor :p1
	attr_accessor :p2
	attr_accessor :block1
	attr_accessor :hasElseBlock
	attr_accessor :block2
	attr_accessor :nodes
end
class BooleanArrayReference
	attr_accessor :booleanArray
end
class BooleanReference
	attr_accessor :booleanValue
end
class CharacterReference
	attr_accessor :characterValue
end
class NumberArrayReference
	attr_accessor :numberArray
end
class NumberReference
	attr_accessor :numberValue
end
class StringArrayReference
	attr_accessor :stringArray
end
class StringReference
	attr_accessor :string
end
class ElementArrayReference
	attr_accessor :array
end
class LinkedListElements
	attr_accessor :first
	attr_accessor :last
end
class LinkedListNodeElements
	attr_accessor :endx
	attr_accessor :value
	attr_accessor :nextx
end
class Element
	attr_accessor :type
	attr_accessor :object
	attr_accessor :array
	attr_accessor :string
	attr_accessor :number
	attr_accessor :booleanValue
end
class ElementReference
	attr_accessor :element
end
class ElementType
	attr_accessor :name
end
class StringElementMap
	attr_accessor :stringListRef
	attr_accessor :elementListRef
end
class DynamicArrayCharacters
	attr_accessor :array
	attr_accessor :length
end
class LinkedListNodeStrings
	attr_accessor :endx
	attr_accessor :value
	attr_accessor :nextx
end
class LinkedListStrings
	attr_accessor :first
	attr_accessor :last
end
class LinkedListNodeNumbers
	attr_accessor :nextx
	attr_accessor :endx
	attr_accessor :value
end
class LinkedListNumbers
	attr_accessor :first
	attr_accessor :last
end
class LinkedListCharacters
	attr_accessor :first
	attr_accessor :last
end
class LinkedListNodeCharacters
	attr_accessor :endx
	attr_accessor :value
	attr_accessor :nextx
end
class DynamicArrayNumbers
	attr_accessor :array
	attr_accessor :length
end
class LLinkedListNodeStrings
	attr_accessor :endx
	attr_accessor :value
	attr_accessor :nextx
end
class LLinkedListStrings
	attr_accessor :first
	attr_accessor :last
end
class LLinkedListNodeNumbers
	attr_accessor :nextx
	attr_accessor :endx
	attr_accessor :value
end
class LLinkedListNumbers
	attr_accessor :first
	attr_accessor :last
end
class LLinkedListCharacters
	attr_accessor :first
	attr_accessor :last
end
class LLinkedListNodeCharacters
	attr_accessor :endx
	attr_accessor :value
	attr_accessor :nextx
end
class LDynamicArrayNumbers
	attr_accessor :array
	attr_accessor :length
end
def CreateLinkedListNodes()

	ll = LinkedListNodes.new
	ll.first = LinkedListNodeNodes.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def LinkedListAddNode(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LinkedListNodeNodes.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def LinkedListNodesToArray(ll)

	node = ll.first

	length = LinkedListNodesLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def LinkedListNodesLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def FreeLinkedListNode(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def IsValidTemplate(template)

	tokens = CreateLinkedListString()
	errorMessage = StringReference.new

	success = GenerateTokensFromTemplate(template, tokens, errorMessage)

	if success
		root = Node.new

		ts = LinkedListStringsToArray(tokens)

		success = ParseTemplate(ts, root, errorMessage)
	end

	if !success
		FreeStringReference(errorMessage)
	end

	return success
end


def GenerateTokensFromTemplate(template, tokens, errorMessage)

	success = true
	cs = Array.new(4)

	da = CreateDynamicArrayCharacters()

	pc = "x"
	i = 0.0
	while(i < template.length && success)
		c = template[i]

		if c != "{"
			DynamicArrayAddCharacter(da, c)
			i = i + 1.0
		else
			FillString(cs, "x")
			j = 0.0
			while(i + j < template.length && j < cs.length)
				cs[j] = template[i + j]
				j = j + 1.0
			end

			if StringsEqual(cs, "{use".split("")) || StringsEqual(cs, "{end".split("")) || StringsEqual(cs, "{pri".split("")) || StringsEqual(cs, "{for".split("")) || StringsEqual(cs, "{if ".split("")) || StringsEqual(cs, "{els".split(""))
				if pc != "\\"
					# Find end.
					found = false
					l = 0.0
					while(i + l < template.length && !found)
						if template[i + l] == "}"
							found = true
						end
						l = l + 1.0
					end

					if found
						if da.length > 0.0
							a = DynamicArrayCharactersToArray(da)
							LinkedListAddString(tokens, a)
							FreeDynamicArrayCharacters(da)
							da = CreateDynamicArrayCharacters()
						end

						j = 0.0
						while(j < l)
							DynamicArrayAddCharacter(da, template[i + j])
							j = j + 1.0
						end

						a = DynamicArrayCharactersToArray(da)
						LinkedListAddString(tokens, a)
						FreeDynamicArrayCharacters(da)
						da = CreateDynamicArrayCharacters()

						i = i + l
					else
						success = false
						errorMessage.string = "Template command found, but not ended properly.".split("")
					end
				else
					DynamicArrayAddCharacter(da, c)
					i = i + 1.0
				end
			else
				DynamicArrayAddCharacter(da, c)
				i = i + 1.0
			end
		end

		pc = c
	end

	if da.length > 0.0
		a = DynamicArrayCharactersToArray(da)
		LinkedListAddString(tokens, a)
	end

	FreeDynamicArrayCharacters(da)

	return success
end


def GenerateDocument(template, data, document, errorMessage)

	tokens = CreateLinkedListString()

	success = GenerateTokensFromTemplate(template, tokens, errorMessage)

	if success
		root = Node.new

		ts = LinkedListStringsToArray(tokens)
		FreeLinkedListString(tokens)

		success = ParseTemplate(ts, root, errorMessage)

		if success
			ll = CreateLinkedListCharacter()

			success = GenerateDocumentFromBlock(root, data, ll, errorMessage)

			if success
				document.string = LinkedListCharactersToArray(ll)
				FreeLinkedListCharacter(ll)
			end
		end
	end

	return success
end


def GenerateDocumentFromBlock(root, data, ll, errorMessage)

	n = root.nodes.first
	success = true

	while(!n.endx && success)
		success = GenerateDocumentFromNode(n.value, data, ll, errorMessage)
		n = n.nextx
	end

	return success
end


def GenerateDocumentFromNode(n, data, ll, errorMessage)

	success = true
	found = BooleanReference.new

	if StringsEqual(n.type, "block".split(""))
		success = GenerateDocumentFromBlock(n, data, ll, errorMessage)
	elsif StringsEqual(n.type, "use".split(""))
		success = GenerateDocumentFromUse(n, data, ll, errorMessage)
	elsif StringsEqual(n.type, "if".split(""))
		success = GenerateDocumentFromIf(n, data, ll, errorMessage)
	elsif StringsEqual(n.type, "foreach".split(""))
		success = GenerateDocumentFromForeach(n, data, ll, errorMessage)
	elsif StringsEqual(n.type, "text".split(""))
		success = true
		LinkedListCharactersAddString(ll, n.p1)
	elsif StringsEqual(n.type, "print".split(""))
		if StringsEqual(data.type, "object".split(""))
			value = GetObjectValueWithCheck(data.object, n.p1, found)

			if found.booleanValue
				if StringsEqual(value.type, "string".split(""))
					valueString = value.string
				else
					valueString = WriteJSON(value)
				end
				LinkedListCharactersAddString(ll, valueString)
			else
				success = false
				errorMessage.string = "Key for printing not found in JSON object: ".split("")
				errorMessage.string = sConcatenateString(errorMessage.string, n.p1)
			end
		else
			success = false
			errorMessage.string = "Data structure for print command is not a JSON object.".split("")
		end
	end

	return success
end


def GenerateDocumentFromUse(n, data, ll, errorMessage)

	found = BooleanReference.new

	if StringsEqual(data.type, "object".split(""))
		value = GetObjectValueWithCheck(data.object, n.p1, found)

		if found.booleanValue
			success = GenerateDocumentFromNode(n.block1, value, ll, errorMessage)
		else
			success = false
			errorMessage.string = "Key for use not found in JSON object.".split("")
		end
	else
		success = false
		errorMessage.string = "Data structure for use command is not a JSON object.".split("")
	end

	return success
end


def GenerateDocumentFromIf(n, data, ll, errorMessage)

	success = true
	found = BooleanReference.new

	if StringsEqual(data.type, "object".split(""))
		value = GetObjectValueWithCheck(data.object, n.p1, found)

		if found.booleanValue
			if StringsEqual(value.type, "boolean".split(""))
				if value.booleanValue
					success = GenerateDocumentFromBlock(n.block1, data, ll, errorMessage)
				end

				if n.hasElseBlock
					if !value.booleanValue
						success = GenerateDocumentFromBlock(n.block2, data, ll, errorMessage)
					end
				end
			else
				success = false
				errorMessage.string = "Value for if not boolean.".split("")
			end
		else
			success = false
			errorMessage.string = "Key for if not found in JSON object: ".split("")
			errorMessage.string = sConcatenateString(errorMessage.string, n.p1)
		end
	else
		success = false
		errorMessage.string = "Data structure for if command is not a JSON object.".split("")
	end

	return success
end


def GenerateDocumentFromForeach(n, data, ll, errorMessage)

	success = true
	found = BooleanReference.new
	loopVar = CreateObjectElement(0.0)

	PutStringElementMap(loopVar.object, n.p1, Element.new)

	if StringsEqual(data.type, "object".split(""))
		value = GetObjectValueWithCheck(data.object, n.p2, found)

		if found.booleanValue
			if StringsEqual(value.type, "array".split(""))

				i = 0.0
				while(i < value.array.length)
					arrayElement = value.array[i]
					if StringsEqual(arrayElement.type, "object".split(""))
						success = GenerateDocumentFromBlock(n.block1, arrayElement, ll, errorMessage)
					else
						SetStringElementMap(loopVar.object, 0.0, n.p1, arrayElement)
						success = GenerateDocumentFromBlock(n.block1, loopVar, ll, errorMessage)
					end
					i = i + 1.0
				end
			else
				success = false
				errorMessage.string = "Value for foreach is not an array.".split("")
			end
		else
			success = false
			errorMessage.string = "Key for foreach not found in JSON object: ".split("")
			errorMessage.string = sConcatenateString(errorMessage.string, n.p2)
		end
	else
		success = false
		errorMessage.string = "Data structure for foreach command is not a JSON object.".split("")
	end

	return success
end


def ParseTemplate(tokens, node, errorMessage)

	position = CreateNumberReference(0.0)
	success = ParseTemplateBlock(tokens, position, node, errorMessage)

	if success
		if position.numberValue != tokens.length
			success = false
			errorMessage.string = "Unexpected token at the end of template.".split("")
		end
	end

	delete(position)

	return success
end


def ParseTemplateBlock(tokens, position, node, errorMessage)

	success = true
	done = false

	node.type = "block".split("")
	node.nodes = CreateLinkedListNodes()

	while(position.numberValue < tokens.length && success && !done)
		tn = Node.new
		success = ParseNodeString(tokens[position.numberValue].string, tn, errorMessage)

		if success
			if StringsEqual(tn.type, "text".split("")) || StringsEqual(tn.type, "print".split(""))
				LinkedListAddNode(node.nodes, tn)
				position.numberValue = position.numberValue + 1.0
			elsif StringsEqual(tn.type, "use".split(""))
				nb = Node.new
				success = ParseUseBlock(tokens, position, nb, errorMessage)
				LinkedListAddNode(node.nodes, nb)
			elsif StringsEqual(tn.type, "if".split(""))
				nb = Node.new
				success = ParseIfBlock(tokens, position, nb, errorMessage)
				LinkedListAddNode(node.nodes, nb)
			elsif StringsEqual(tn.type, "foreach".split(""))
				nb = Node.new
				success = ParseForeachBlock(tokens, position, nb, errorMessage)
				LinkedListAddNode(node.nodes, nb)
			else
				done = true
			end
		end
	end

	return success
end


def ParseUseBlock(tokens, position, useBlock, errorMessage)

	n = Node.new
	ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

	useBlock.type = CopyString(n.type)
	useBlock.p1 = CopyString(n.p1)
	useBlock.block1 = Node.new

	position.numberValue = position.numberValue + 1.0

	success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage)

	if success
		if position.numberValue < tokens.length
			n = Node.new
			ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

			if StringsEqual(n.type, "end".split(""))
				success = true
				position.numberValue = position.numberValue + 1.0
			else
				success = false
				errorMessage.string = "End block expected at the end of use block.".split("")
			end
		else
			success = false
			errorMessage.string = "End block expected at the end of use block.".split("")
		end
	end

	return success
end


def ParseIfBlock(tokens, position, ifBlock, errorMessage)

	n = Node.new
	ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

	ifBlock.type = CopyString(n.type)
	ifBlock.p1 = CopyString(n.p1)
	ifBlock.block1 = Node.new
	ifBlock.hasElseBlock = false

	position.numberValue = position.numberValue + 1.0

	success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage)

	if success
		if position.numberValue < tokens.length
			n = Node.new
			ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

			if StringsEqual(n.type, "end".split(""))
				success = true
				position.numberValue = position.numberValue + 1.0
			elsif StringsEqual(n.type, "else".split(""))
				position.numberValue = position.numberValue + 1.0
				ifBlock.hasElseBlock = true
				ifBlock.block2 = Node.new
				success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage)

				if success
					if position.numberValue < tokens.length
						n = Node.new
						ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

						if StringsEqual(n.type, "end".split(""))
							success = true
							position.numberValue = position.numberValue + 1.0
						else
							success = false
							errorMessage.string = "End block expected at the end of else block.".split("")
						end
					else
						success = false
						errorMessage.string = "End block expected at the end of else block.".split("")
					end
				end
			else
				success = false
				errorMessage.string = "End or else block expected at the end of if block.".split("")
			end
		else
			success = false
			errorMessage.string = "End or else block expected at the end of if block.".split("")
		end
	end

	return success
end


def ParseForeachBlock(tokens, position, foreachBlock, errorMessage)

	n = Node.new
	ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

	foreachBlock.type = CopyString(n.type)
	foreachBlock.p1 = CopyString(n.p1)
	foreachBlock.p2 = CopyString(n.p2)
	foreachBlock.block1 = Node.new

	position.numberValue = position.numberValue + 1.0

	success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage)

	if success
		if position.numberValue < tokens.length
			n = Node.new
			ParseNodeString(tokens[position.numberValue].string, n, errorMessage)

			if StringsEqual(n.type, "end".split(""))
				success = true
				position.numberValue = position.numberValue + 1.0
			else
				success = false
				errorMessage.string = "End block expected at the end of for each block.".split("")
			end
		else
			success = false
			errorMessage.string = "End block expected at the end of foreach block.".split("")
		end
	end

	return success
end


def ParseNodeString(token, node, errorMessage)

	success = true
	isText = false

	if token.length <= 2.0
		isText = true
	elsif token[0] == "\\" && token[1] == "{"
		isText = true
	elsif token[0] != "{"
		isText = true
	else
		command = strSubstring(token, 1.0, token.length - 1.0)
		parts = sSplitByCharacter(command, " ")

		if command.length > 0.0
			if StringsEqual(parts[0].string, "use".split(""))
				if parts.length == 2.0
					node.type = CopyString(parts[0].string)
					node.p1 = CopyString(parts[1].string)
				else
					success = false
					errorMessage.string = "The use command takes one parameter.".split("")
				end
			elsif StringsEqual(parts[0].string, "end".split(""))
				if parts.length == 1.0
					node.type = CopyString(parts[0].string)
				else
					success = false
					errorMessage.string = "The end command takes no parameters.".split("")
				end
			elsif StringsEqual(parts[0].string, "print".split(""))
				if parts.length == 2.0
					node.type = CopyString(parts[0].string)
					node.p1 = CopyString(parts[1].string)
				else
					success = false
					errorMessage.string = "The print command takes one parameter.".split("")
				end
			elsif StringsEqual(parts[0].string, "foreach".split(""))
				if parts.length == 4.0
					if StringsEqual(parts[2].string, "in".split(""))
						node.type = CopyString(parts[0].string)
						node.p1 = CopyString(parts[1].string)
						node.p2 = CopyString(parts[3].string)
					else
						success = false
						errorMessage.string = "The foreach command must have \"in\" after the first parameter.".split("")
					end
				else
					success = false
					errorMessage.string = "The foreach command takes three parameters.".split("")
				end
			elsif StringsEqual(parts[0].string, "if".split(""))
				if parts.length == 2.0
					node.type = CopyString(parts[0].string)
					node.p1 = CopyString(parts[1].string)
				else
					success = false
					errorMessage.string = "The if command takes one parameter.".split("")
				end
			elsif StringsEqual(parts[0].string, "else".split(""))
				if parts.length == 1.0
					node.type = CopyString(parts[0].string)
				else
					success = false
					errorMessage.string = "The else command takes no parameters.".split("")
				end
			else
				isText = true
			end
		else
			isText = true
		end
	end

	if isText
		node.type = "text".split("")
		node.p1 = sReplaceString(token, "\\{print ".split(""), "{print ".split(""))
		node.p1 = sReplaceString(node.p1, "\\{use ".split(""), "{use ".split(""))
		node.p1 = sReplaceString(node.p1, "\\{if ".split(""), "{if ".split(""))
		node.p1 = sReplaceString(node.p1, "\\{end}".split(""), "{end}".split(""))
		node.p1 = sReplaceString(node.p1, "\\{foreach ".split(""), "{foreach ".split(""))
		node.p1 = sReplaceString(node.p1, "\\{else}".split(""), "{else}".split(""))
	end

	return success
end


def test()

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
end


def testTokenGeneration(failures)

	errorMessage = StringReference.new

	tokens = CreateLinkedListString()
	template = "This is a template, this is a value: {print a}.".split("")
	success = GenerateTokensFromTemplate(template, tokens, errorMessage)
	AssertTrue(success, failures)
	AssertEquals(3.0, LinkedListStringsLength(tokens), failures)

	tokens = CreateLinkedListString()
	template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".split("")
	success = GenerateTokensFromTemplate(template, tokens, errorMessage)
	AssertTrue(success, failures)
	AssertEquals(9.0, LinkedListStringsLength(tokens), failures)

	tokens = CreateLinkedListString()
	template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".split("")
	success = GenerateTokensFromTemplate(template, tokens, errorMessage)
	AssertTrue(success, failures)
	AssertEquals(16.0, LinkedListStringsLength(tokens), failures)

	tokens = CreateLinkedListString()
	template = "T: {foreach a in b}{print a}{end}.".split("")
	success = GenerateTokensFromTemplate(template, tokens, errorMessage)
	AssertTrue(success, failures)
	AssertEquals(5.0, LinkedListStringsLength(tokens), failures)
end


def testGenerateDocument1(failures)
	AssertTemplateResult("This is a template, this is a value: {print a}.".split(""), "{\"c\": 5, \"a\": 6}".split(""), "This is a template, this is a value: 6.".split(""), failures)
end


def testGenerateDocument2(failures)
	AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".split(""), "{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}".split(""), "This is a template, this is a value: 6 1 2.".split(""), failures)
end


def testGenerateDocument3(failures)
	AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".split(""), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}".split(""), "This is a template, this is a value: 6 1 23 416.".split(""), failures)
end


def testGenerateDocument4(failures)
	AssertTemplateResult("T: {if a}a{end}.".split(""), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}".split(""), "T: a.".split(""), failures)

	AssertTemplateResult("T: {if a}a{else}b{end}.".split(""), "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}".split(""), "T: b.".split(""), failures)
end


def testGenerateDocument5(failures)
	AssertTemplateResult("T: {foreach a in b}{print a}{end}.".split(""), "{\"b\": [1, 2, 3, 4]}".split(""), "T: 1234.".split(""), failures)
end


def testGenerateDocument6(failures)
	AssertTemplateResult("{test}\\{print a}.".split(""), "{\"c\": 5, \"a\": 6}".split(""), "{test}{print a}.".split(""), failures)
end


def testGenerateDocument7(failures)
	AssertTemplateResult("{".split(""), "{}".split(""), "{".split(""), failures)

	AssertTemplateResult("{  ".split(""), "{}".split(""), "{  ".split(""), failures)

	AssertTemplateResult("{use a}\\{print a}{end}".split(""), "{\"a\": {}}".split(""), "{print a}".split(""), failures)

	AssertTemplateResult("\\{print a}{print a}}".split(""), "{\"a\": 3}".split(""), "{print a}3}".split(""), failures)

	AssertTemplateResult("\\\\{print a}{print a}}".split(""), "{\"a\": 3}".split(""), "\\{print a}3}".split(""), failures)

	AssertTemplateResult("\\\\{print a}{print a}\\\\{print a}}".split(""), "{\"a\": 3}".split(""), "\\{print a}3\\{print a}}".split(""), failures)

	AssertTemplateResult("\\{print a}{print a}\\{".split(""), "{\"a\": 3}".split(""), "{print a}3\\{".split(""), failures)

	AssertTemplateResult("    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>".split(""), "{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}".split(""), "    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>".split(""), failures)

	AssertTemplateError("{print".split(""), "{}".split(""), "Template command found, but not ended properly.".split(""), failures)

	AssertTemplateError("{print a}".split(""), "{}".split(""), "Key for printing not found in JSON object: a".split(""), failures)

	AssertTemplateError("This is a template, this is a value: {print a {print a}.".split(""), "{\"a\": 5}".split(""), "The print command takes one parameter.".split(""), failures)

	AssertTemplateError("This is a {use a}\\{print a}template, this is a value: {print a}.{end}".split(""), "{\"a\": 5}".split(""), "Data structure for print command is not a JSON object.".split(""), failures)

	AssertTemplateError("{use a}".split(""), "{}".split(""), "End block expected at the end of use block.".split(""), failures)

	AssertTemplateError("{if a}".split(""), "{\"a\": true}".split(""), "End or else block expected at the end of if block.".split(""), failures)

	AssertTemplateError("{if a}{else}".split(""), "{\"a\": true}".split(""), "End block expected at the end of else block.".split(""), failures)

	AssertTemplateError("{foreach x in a}".split(""), "{\"a\": [1, 2, 3, 4]}".split(""), "End block expected at the end of foreach block.".split(""), failures)
end


def AssertTemplateResult(template, json, result, failures)

	data = ElementReference.new
	errorMessages = StringArrayReference.new
	document = StringReference.new
	errorMessage = StringReference.new

	success = ReadJSON(json, data, errorMessages)

	AssertTrue(success, failures)

	if success
		success = GenerateDocument(template, data.element, document, errorMessage)

		AssertTrue(success, failures)

		if success
			AssertStringEquals(document.string, result, failures)
		end
	end
end


def AssertTemplateError(template, json, errorMessage, failures)

	data = ElementReference.new
	errorMessages = StringArrayReference.new
	document = StringReference.new
	errorMessageRef = StringReference.new

	success = ReadJSON(json, data, errorMessages)

	AssertTrue(success, failures)

	if success
		success = GenerateDocument(template, data.element, document, errorMessageRef)

		AssertFalse(success, failures)

		if !success
			AssertStringEquals(errorMessage, errorMessageRef.string, failures)
		end
	end
end


def CreateBooleanReference(value)

	ref = BooleanReference.new
	ref.booleanValue = value

	return ref
end


def CreateBooleanArrayReference(value)

	ref = BooleanArrayReference.new
	ref.booleanArray = value

	return ref
end


def CreateBooleanArrayReferenceLengthValue(length, value)

	ref = BooleanArrayReference.new
	ref.booleanArray = Array.new(length)

	i = 0.0
	while(i < length)
		ref.booleanArray[i] = value
		i = i + 1.0
	end

	return ref
end


def FreeBooleanArrayReference(booleanArrayReference)
	delete(booleanArrayReference.booleanArray)
	delete(booleanArrayReference)
end


def CreateCharacterReference(value)

	ref = CharacterReference.new
	ref.characterValue = value

	return ref
end


def CreateNumberReference(value)

	ref = NumberReference.new
	ref.numberValue = value

	return ref
end


def CreateNumberArrayReference(value)

	ref = NumberArrayReference.new
	ref.numberArray = value

	return ref
end


def CreateNumberArrayReferenceLengthValue(length, value)

	ref = NumberArrayReference.new
	ref.numberArray = Array.new(length)

	i = 0.0
	while(i < length)
		ref.numberArray[i] = value
		i = i + 1.0
	end

	return ref
end


def FreeNumberArrayReference(numberArrayReference)
	delete(numberArrayReference.numberArray)
	delete(numberArrayReference)
end


def CreateStringReference(value)

	ref = StringReference.new
	ref.string = value

	return ref
end


def CreateStringReferenceLengthValue(length, value)

	ref = StringReference.new
	ref.string = Array.new(length)

	i = 0.0
	while(i < length)
		ref.string[i] = value
		i = i + 1.0
	end

	return ref
end


def FreeStringReference(stringReference)
	delete(stringReference.string)
	delete(stringReference)
end


def CreateStringArrayReference(strings)

	ref = StringArrayReference.new
	ref.stringArray = strings

	return ref
end


def CreateStringArrayReferenceLengthValue(length, value)

	ref = StringArrayReference.new
	ref.stringArray = Array.new(length)

	i = 0.0
	while(i < length)
		ref.stringArray[i] = CreateStringReference(value)
		i = i + 1.0
	end

	return ref
end


def FreeStringArrayReference(stringArrayReference)

	i = 0.0
	while(i < stringArrayReference.stringArray.length)
		delete(stringArrayReference.stringArray[i])
		i = i + 1.0
	end
	delete(stringArrayReference.stringArray)
	delete(stringArrayReference)
end


def IsValidJSON(json, errorMessage)

	elementReference = ElementReference.new

	success = ReadJSON(json, elementReference, errorMessage)

	if success
		DeleteElement(elementReference.element)
	end

	return success
end


def JSONTokenize(json, tokensReference, errorMessages)

	ll = lCreateLinkedListString()
	success = true

	stringLength = NumberReference.new
	tokenReference = StringReference.new

	i = 0.0
	while(i < json.length && success)
		c = json[i]

		if c == "{"
			lLinkedListAddString(ll, "{".split(""))
			i = i + 1.0
		elsif c == "}"
			lLinkedListAddString(ll, "}".split(""))
			i = i + 1.0
		elsif c == "["
			lLinkedListAddString(ll, "[".split(""))
			i = i + 1.0
		elsif c == "]"
			lLinkedListAddString(ll, "]".split(""))
			i = i + 1.0
		elsif c == ":"
			lLinkedListAddString(ll, ":".split(""))
			i = i + 1.0
		elsif c == ","
			lLinkedListAddString(ll, ",".split(""))
			i = i + 1.0
		elsif c == "f"
			success = GetJSONPrimitiveName(json, i, errorMessages, "false".split(""), tokenReference)
			if success
				lLinkedListAddString(ll, "false".split(""))
				i = i + "false".split("").length
			end
		elsif c == "t"
			success = GetJSONPrimitiveName(json, i, errorMessages, "true".split(""), tokenReference)
			if success
				lLinkedListAddString(ll, "true".split(""))
				i = i + "true".split("").length
			end
		elsif c == "n"
			success = GetJSONPrimitiveName(json, i, errorMessages, "null".split(""), tokenReference)
			if success
				lLinkedListAddString(ll, "null".split(""))
				i = i + "null".split("").length
			end
		elsif c == " " || c == "\n" || c == "\t" || c == "\r"
			# Skip.
			i = i + 1.0
		elsif c == "\""
			success = GetJSONString(json, i, tokenReference, stringLength, errorMessages)
			if success
				lLinkedListAddString(ll, tokenReference.string)
				i = i + stringLength.numberValue
			end
		elsif IsJSONNumberCharacter(c)
			success = GetJSONNumberToken(json, i, tokenReference, errorMessages)
			if success
				lLinkedListAddString(ll, tokenReference.string)
				i = i + tokenReference.string.length
			end
		else
			str = strConcatenateCharacter("Invalid start of Token: ".split(""), c)
			stringReference = CreateStringReference(str)
			lAddStringRef(errorMessages, stringReference)
			i = i + 1.0
			success = false
		end
	end

	if success
		lLinkedListAddString(ll, "<end>".split(""))
		tokensReference.stringArray = lLinkedListStringsToArray(ll)
		lFreeLinkedListString(ll)
	end

	return success
end


def GetJSONNumberToken(json, start, tokenReference, errorMessages)

	endx = json.length
	done = false

	i = start
	while(i < json.length && !done)
		c = json[i]
		if !IsJSONNumberCharacter(c)
			done = true
			endx = i
		end
		i = i + 1.0
	end

	numberString = strSubstring(json, start, endx)

	success = IsValidJSONNumber(numberString, errorMessages)

	tokenReference.string = numberString

	return success
end


def IsValidJSONNumber(n, errorMessages)

	i = 0.0

	# JSON allows an optional negative sign.
	if n[i] == "-"
		i = i + 1.0
	end

	if i < n.length
		success = IsValidJSONNumberAfterSign(n, i, errorMessages)
	else
		success = false
		lAddStringRef(errorMessages, CreateStringReference("Number must contain at least one digit.".split("")))
	end

	return success
end


def IsValidJSONNumberAfterSign(n, i, errorMessages)

	if charIsNumber(n[i])
		# 0 first means only 0.
		if n[i] == "0"
			i = i + 1.0
		else
			# 1-9 first, read following digits.
			i = IsValidJSONNumberAdvancePastDigits(n, i)
		end

		if i < n.length
			success = IsValidJSONNumberFromDotOrExponent(n, i, errorMessages)
		else
			# If integer, we are done now.
			success = true
		end
	else
		success = false
		lAddStringRef(errorMessages, CreateStringReference("A number must start with 0-9 (after the optional sign).".split("")))
	end

	return success
end


def IsValidJSONNumberAdvancePastDigits(n, i)

	i = i + 1.0
	done = false
	while(i < n.length && !done)
		if charIsNumber(n[i])
			i = i + 1.0
		else
			done = true
		end
	end

	return i
end


def IsValidJSONNumberFromDotOrExponent(n, i, errorMessages)

	wasDotAndOrE = false
	success = true

	if n[i] == "."
		i = i + 1.0
		wasDotAndOrE = true

		if i < n.length
			if charIsNumber(n[i])
				# Read digits following decimal point.
				i = IsValidJSONNumberAdvancePastDigits(n, i)

				if i == n.length
					# If non-scientific decimal number, we are done.
					success = true
				end
			else
				success = false
				lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".split("")))
			end
		else
			success = false
			lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".split("")))
		end
	end

	if i < n.length && success
		if n[i] == "e" || n[i] == "E"
			wasDotAndOrE = true
			success = IsValidJSONNumberFromExponent(n, i, errorMessages)
		else
			success = false
			lAddStringRef(errorMessages, CreateStringReference("Expected e or E.".split("")))
		end
	elsif i == n.length && success
		# If number with decimal point.
		success = true
	else
		success = false
		lAddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".split("")))
	end

	if wasDotAndOrE
	else
		success = false
		lAddStringRef(errorMessages, CreateStringReference("Exprected decimal point or e or E.".split("")))
	end

	return success
end


def IsValidJSONNumberFromExponent(n, i, errorMessages)

	i = i + 1.0

	if i < n.length
		# The exponent sign can either + or -,
		if n[i] == "+" || n[i] == "-"
			i = i + 1.0
		end

		if i < n.length
			if charIsNumber(n[i])
				# Read digits following decimal point.
				i = IsValidJSONNumberAdvancePastDigits(n, i)

				if i == n.length
					# We found scientific number.
					success = true
				else
					success = false
					lAddStringRef(errorMessages, CreateStringReference("There was characters following the exponent.".split("")))
				end
			else
				success = false
				lAddStringRef(errorMessages, CreateStringReference("There must be a digit following the optional exponent sign.".split("")))
			end
		else
			success = false
			lAddStringRef(errorMessages, CreateStringReference("There must be a digit following optional the exponent sign.".split("")))
		end
	else
		success = false
		lAddStringRef(errorMessages, CreateStringReference("There must be a sign or a digit following e or E.".split("")))
	end

	return success
end


def IsJSONNumberCharacter(c)

	numericCharacters = "0123456789.-+eE".split("")

	found = false

	i = 0.0
	while(i < numericCharacters.length)
		if numericCharacters[i] == c
			found = true
		end
		i = i + 1.0
	end

	return found
end


def GetJSONPrimitiveName(string, start, errorMessages, primitive, tokenReference)

	done = false
	success = true

	token = "".split("")

	i = start
	while(i < string.length && ((i - start) < primitive.length) && !done)
		c = string[i]
		p = primitive[i - start]
		if c == p
			# OK
			if (i + 1.0 - start) == primitive.length
				done = true
			end
		else
			str = "".split("")
			str = strConcatenateString(str, "Primitive invalid: ".split(""))
			str = strAppendCharacter(str, c)
			str = strAppendString(str, " vs ".split(""))
			str = strAppendCharacter(str, p)

			lAddStringRef(errorMessages, CreateStringReference(str))
			done = true
			success = false
		end
		i = i + 1.0
	end

	if done
		if StringsEqual(primitive, "false".split(""))
			token = "false".split("")
		end
		if StringsEqual(primitive, "true".split(""))
			token = "true".split("")
		end
		if StringsEqual(primitive, "null".split(""))
			token = "null".split("")
		end
	else
		lAddStringRef(errorMessages, CreateStringReference("Primitive invalid".split("")))
		success = false
	end

	tokenReference.string = token

	return success
end


def GetJSONString(json, start, tokenReference, stringLengthReference, errorMessages)

	characterCount = CreateNumberReference(0.0)
	hex = CreateString(4.0, "0")
	hexReference = NumberReference.new
	errorMessage = StringReference.new

	success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages)

	if success
		l = characterCount.numberValue
		string = Array.new(l)

		c = 0.0
		string[c] = "\""
		c = c + 1.0

		done = false
		i = start + 1.0
		while(!done)
			if json[i] == "\\"
				i = i + 1.0
				if json[i] == "\"" || json[i] == "\\" || json[i] == "/"
					string[c] = json[i]
					c = c + 1.0
				elsif json[i] == "b"
					string[c] = (8.0).truncate.chr('UTF-8')
					c = c + 1.0
				elsif json[i] == "f"
					string[c] = (12.0).truncate.chr('UTF-8')
					c = c + 1.0
				elsif json[i] == "n"
					string[c] = (10.0).truncate.chr('UTF-8')
					c = c + 1.0
				elsif json[i] == "r"
					string[c] = (13.0).truncate.chr('UTF-8')
					c = c + 1.0
				elsif json[i] == "t"
					string[c] = (9.0).truncate.chr('UTF-8')
					c = c + 1.0
				elsif json[i] == "u"
					i = i + 1.0
					hex[0] = charToUpperCase(json[i + 0.0])
					hex[1] = charToUpperCase(json[i + 1.0])
					hex[2] = charToUpperCase(json[i + 2.0])
					hex[3] = charToUpperCase(json[i + 3.0])
					nCreateNumberFromStringWithCheck(hex, 16.0, hexReference, errorMessage)
					string[c] = (hexReference.numberValue).truncate.chr('UTF-8')
					i = i + 3.0
					c = c + 1.0
				end
			elsif json[i] == "\""
				string[c] = json[i]
				c = c + 1.0
				done = true
			else
				string[c] = json[i]
				c = c + 1.0
			end
			i = i + 1.0
		end

		tokenReference.string = string
		success = true
	else
		lAddStringRef(errorMessages, CreateStringReference("End of string was not found.".split("")))
		success = false
	end

	return success
end


def IsValidJSONString(jsonString, errorMessages)

	numberReference = NumberReference.new
	stringLength = NumberReference.new

	valid = IsValidJSONStringInJSON(jsonString, 0.0, numberReference, stringLength, errorMessages)

	return valid
end


def IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages)

	success = true
	done = false

	characterCount.numberValue = 1.0

	i = start + 1.0
	while(i < json.length && !done && success)
		if !IsJSONIllegalControllCharacter(json[i])
			if json[i] == "\\"
				i = i + 1.0
				if i < json.length
					if json[i] == "\"" || json[i] == "\\" || json[i] == "/" || json[i] == "b" || json[i] == "f" || json[i] == "n" || json[i] == "r" || json[i] == "t"
						characterCount.numberValue = characterCount.numberValue + 1.0
					elsif json[i] == "u"
						if i + 4.0 < json.length
							j = 0.0
							while(j < 4.0 && success)
								c = json[i + j + 1.0]
								if nCharacterIsNumberCharacterInBase(c, 16.0) || c == "a" || c == "b" || c == "c" || c == "d" || c == "e" || c == "f"
								else
									success = false
									lAddStringRef(errorMessages, CreateStringReference("\\u must be followed by four hexadecimal digits.".split("")))
								end
								j = j + 1.0
							end
							characterCount.numberValue = characterCount.numberValue + 1.0
							i = i + 4.0
						else
							success = false
							lAddStringRef(errorMessages, CreateStringReference("\\u must be followed by four characters.".split("")))
						end
					else
						success = false
						lAddStringRef(errorMessages, CreateStringReference("Escaped character invalid.".split("")))
					end
				else
					success = false
					lAddStringRef(errorMessages, CreateStringReference("There must be at least two character after string escape.".split("")))
				end
			elsif json[i] == "\""
				characterCount.numberValue = characterCount.numberValue + 1.0
				done = true
			else
				characterCount.numberValue = characterCount.numberValue + 1.0
			end
		else
			success = false
			lAddStringRef(errorMessages, CreateStringReference("Unicode code points 0-31 not allowed in JSON string.".split("")))
		end
		i = i + 1.0
	end

	if done
		stringLengthReference.numberValue = i - start
	else
		success = false
		lAddStringRef(errorMessages, CreateStringReference("String must end with \".".split("")))
	end

	return success
end


def IsJSONIllegalControllCharacter(c)

	cnr = (c).ord

	if cnr >= 0.0 && cnr < 32.0
		isControll = true
	else
		isControll = false
	end

	return isControll
end


def AddElement(list, a)

	newlist = Array.new(list.length + 1.0)

	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a

	delete(list)

	return newlist
end


def AddElementRef(list, i)
	list.array = AddElement(list.array, i)
end


def RemoveElement(list, n)

	newlist = Array.new(list.length - 1.0)

	i = 0.0
	while(i < list.length)
		if i < n
			newlist[i] = list[i]
		end
		if i > n
			newlist[i - 1.0] = list[i]
		end
		i = i + 1.0
	end

	delete(list)

	return newlist
end


def GetElementRef(list, i)
	return list.array[i]
end


def RemoveElementRef(list, i)
	list.array = RemoveElement(list.array, i)
end


def CreateLinkedListElements()

	ll = LinkedListElements.new
	ll.first = LinkedListNodeElements.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def LinkedListAddElement(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LinkedListNodeElements.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def LinkedListElementsToArray(ll)

	node = ll.first

	length = LinkedListElementsLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def LinkedListElementsLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def FreeLinkedListElements(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def ComputeJSONStringLength(element)

	result = 0.0

	if StringsEqual(element.type, "object".split(""))
		result = result + ComputeJSONObjectStringLength(element)
	elsif StringsEqual(element.type, "string".split(""))
		result = JSONEscapedStringLength(element.string) + 2.0
	elsif StringsEqual(element.type, "array".split(""))
		result = result + ComputeJSONArrayStringLength(element)
	elsif StringsEqual(element.type, "number".split(""))
		result = result + ComputeJSONNumberStringLength(element)
	elsif StringsEqual(element.type, "null".split(""))
		result = result + "null".split("").length
	elsif StringsEqual(element.type, "boolean".split(""))
		result = result + ComputeJSONBooleanStringLength(element)
	end

	return result
end


def ComputeJSONBooleanStringLength(element)

	if element.booleanValue
		result = "true".split("").length
	else
		result = "false".split("").length
	end

	return result
end


def ComputeJSONNumberStringLength(element)

	if (element.number).abs >= 10.0**15.0 || (element.number).abs <= 10.0**(-15.0)
		a = nCreateStringScientificNotationDecimalFromNumber(element.number)
		length = a.length
	else
		a = nCreateStringDecimalFromNumber(element.number)
		length = a.length
	end

	return length
end


def ComputeJSONArrayStringLength(element)

	length = 1.0

	i = 0.0
	while(i < element.array.length)
		arrayElement = element.array[i]

		length = length + ComputeJSONStringLength(arrayElement)

		if i + 1.0 != element.array.length
			length = length + 1.0
		end
		i = i + 1.0
	end

	length = length + 1.0

	return length
end


def ComputeJSONObjectStringLength(element)

	length = 1.0

	keys = GetStringElementMapKeySet(element.object)
	i = 0.0
	while(i < keys.stringArray.length)
		key = keys.stringArray[i].string
		objectElement = GetObjectValue(element.object, key)

		length = length + 1.0
		length = length + JSONEscapedStringLength(key)
		length = length + 1.0
		length = length + 1.0

		length = length + ComputeJSONStringLength(objectElement)

		if i + 1.0 != keys.stringArray.length
			length = length + 1.0
		end
		i = i + 1.0
	end

	length = length + 1.0

	return length
end


def CreateStringElement(string)
	element = Element.new
	element.type = "string".split("")
	element.string = string
	return element
end


def CreateBooleanElement(booleanValue)
	element = Element.new
	element.type = "boolean".split("")
	element.booleanValue = booleanValue
	return element
end


def CreateNullElement()
	element = Element.new
	element.type = "null".split("")
	return element
end


def CreateNumberElement(number)
	element = Element.new
	element.type = "number".split("")
	element.number = number
	return element
end


def CreateArrayElement(length)
	element = Element.new
	element.type = "array".split("")
	element.array = Array.new(length)
	return element
end


def CreateObjectElement(length)
	element = Element.new
	element.type = "object".split("")
	element.object = StringElementMap.new
	element.object.stringListRef = CreateStringArrayReferenceLengthValue(length, "".split(""))
	element.object.elementListRef = ElementArrayReference.new
	element.object.elementListRef.array = Array.new(length)
	return element
end


def DeleteElement(element)
	if StringsEqual(element.type, "object".split(""))
		DeleteObject(element)
	elsif StringsEqual(element.type, "string".split(""))
		delete(element)
	elsif StringsEqual(element.type, "array".split(""))
		DeleteArray(element)
	elsif StringsEqual(element.type, "number".split(""))
		delete(element)
	elsif StringsEqual(element.type, "null".split(""))
		delete(element)
	elsif StringsEqual(element.type, "boolean".split(""))
		delete(element)
	else
	end
end


def DeleteObject(element)

	keys = GetStringElementMapKeySet(element.object)
	i = 0.0
	while(i < keys.stringArray.length)
		key = keys.stringArray[i].string
		objectElement = GetObjectValue(element.object, key)
		DeleteElement(objectElement)
		i = i + 1.0
	end
end


def DeleteArray(element)

	i = 0.0
	while(i < element.array.length)
		arrayElement = element.array[i]
		DeleteElement(arrayElement)
		i = i + 1.0
	end
end


def WriteJSON(element)

	length = ComputeJSONStringLength(element)
	result = Array.new(length)
	index = CreateNumberReference(0.0)

	if StringsEqual(element.type, "object".split(""))
		WriteObject(element, result, index)
	elsif StringsEqual(element.type, "string".split(""))
		WriteString(element, result, index)
	elsif StringsEqual(element.type, "array".split(""))
		WriteArray(element, result, index)
	elsif StringsEqual(element.type, "number".split(""))
		WriteNumber(element, result, index)
	elsif StringsEqual(element.type, "null".split(""))
		strWriteStringToStingStream(result, index, "null".split(""))
	elsif StringsEqual(element.type, "boolean".split(""))
		WriteBooleanValue(element, result, index)
	end

	return result
end


def WriteBooleanValue(element, result, index)
	if element.booleanValue
		strWriteStringToStingStream(result, index, "true".split(""))
	else
		strWriteStringToStingStream(result, index, "false".split(""))
	end
end


def WriteNumber(element, result, index)

	if (element.number).abs >= 10.0**15.0 || (element.number).abs <= 10.0**(-15.0)
		numberString = nCreateStringScientificNotationDecimalFromNumber(element.number)
	else
		numberString = nCreateStringDecimalFromNumber(element.number)
	end

	strWriteStringToStingStream(result, index, numberString)
end


def WriteArray(element, result, index)

	strWriteStringToStingStream(result, index, "[".split(""))

	i = 0.0
	while(i < element.array.length)
		arrayElement = element.array[i]

		s = WriteJSON(arrayElement)
		strWriteStringToStingStream(result, index, s)

		if i + 1.0 != element.array.length
			strWriteStringToStingStream(result, index, ",".split(""))
		end
		i = i + 1.0
	end

	strWriteStringToStingStream(result, index, "]".split(""))
end


def WriteString(element, result, index)
	strWriteStringToStingStream(result, index, "\"".split(""))
	element.string = JSONEscapeString(element.string)
	strWriteStringToStingStream(result, index, element.string)
	strWriteStringToStingStream(result, index, "\"".split(""))
end


def JSONEscapeString(string)

	length = JSONEscapedStringLength(string)

	ns = Array.new(length)
	index = CreateNumberReference(0.0)
	lettersReference = CreateNumberReference(0.0)

	i = 0.0
	while(i < string.length)
		if JSONMustBeEscaped(string[i], lettersReference)
			escaped = JSONEscapeCharacter(string[i])
			strWriteStringToStingStream(ns, index, escaped)
		else
			strWriteCharacterToStingStream(ns, index, string[i])
		end
		i = i + 1.0
	end

	return ns
end


def JSONEscapedStringLength(string)

	lettersReference = CreateNumberReference(0.0)
	length = 0.0

	i = 0.0
	while(i < string.length)
		if JSONMustBeEscaped(string[i], lettersReference)
			length = length + lettersReference.numberValue
		else
			length = length + 1.0
		end
		i = i + 1.0
	end
	return length
end


def JSONEscapeCharacter(c)

	code = (c).ord

	q = 34.0
	rs = 92.0
	s = 47.0
	b = 8.0
	f = 12.0
	n = 10.0
	r = 13.0
	t = 9.0

	hexNumber = StringReference.new

	if code == q
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "\""
	elsif code == rs
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "\\"
	elsif code == s
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "/"
	elsif code == b
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "b"
	elsif code == f
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "f"
	elsif code == n
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "n"
	elsif code == r
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "r"
	elsif code == t
		escaped = Array.new(2)
		escaped[0] = "\\"
		escaped[1] = "t"
	elsif code >= 0.0 && code <= 31.0
		escaped = Array.new(6)
		escaped[0] = "\\"
		escaped[1] = "u"
		escaped[2] = "0"
		escaped[3] = "0"

		nCreateStringFromNumberWithCheck(code, 16.0, hexNumber)

		if hexNumber.string.length == 1.0
			escaped[4] = "0"
			escaped[5] = hexNumber.string[0]
		elsif hexNumber.string.length == 2.0
			escaped[4] = hexNumber.string[0]
			escaped[5] = hexNumber.string[1]
		end
	else
		escaped = Array.new(1)
		escaped[0] = c
	end

	return escaped
end


def JSONMustBeEscaped(c, letters)

	code = (c).ord
	mustBeEscaped = false

	q = 34.0
	rs = 92.0
	s = 47.0
	b = 8.0
	f = 12.0
	n = 10.0
	r = 13.0
	t = 9.0

	if code == q || code == rs || code == s || code == b || code == f || code == n || code == r || code == t
		mustBeEscaped = true
		letters.numberValue = 2.0
	elsif code >= 0.0 && code <= 31.0
		mustBeEscaped = true
		letters.numberValue = 6.0
	elsif code >= 2.0**16.0
		mustBeEscaped = true
		letters.numberValue = 6.0
	end

	return mustBeEscaped
end


def WriteObject(element, result, index)

	strWriteStringToStingStream(result, index, "{".split(""))

	keys = GetStringElementMapKeySet(element.object)
	i = 0.0
	while(i < keys.stringArray.length)
		key = keys.stringArray[i].string
		key = JSONEscapeString(key)
		objectElement = GetObjectValue(element.object, key)

		strWriteStringToStingStream(result, index, "\"".split(""))
		strWriteStringToStingStream(result, index, key)
		strWriteStringToStingStream(result, index, "\"".split(""))
		strWriteStringToStingStream(result, index, ":".split(""))

		s = WriteJSON(objectElement)
		strWriteStringToStingStream(result, index, s)

		if i + 1.0 != keys.stringArray.length
			strWriteStringToStingStream(result, index, ",".split(""))
		end
		i = i + 1.0
	end

	strWriteStringToStingStream(result, index, "}".split(""))
end


def ReadJSON(string, elementReference, errorMessages)

	# Tokenize.
	tokenArrayReference = StringArrayReference.new
	success = JSONTokenize(string, tokenArrayReference, errorMessages)

	if success
		# Parse.
		success = GetJSONValue(tokenArrayReference.stringArray, elementReference, errorMessages)
	end

	return success
end


def GetJSONValue(tokens, elementReference, errorMessages)

	i = CreateNumberReference(0.0)
	success = GetJSONValueRecursive(tokens, i, 0.0, elementReference, errorMessages)

	return success
end


def GetJSONValueRecursive(tokens, i, depth, elementReference, errorMessages)

	success = true
	token = tokens[i.numberValue].string

	if StringsEqual(token, "{".split(""))
		success = GetJSONObject(tokens, i, depth + 1.0, elementReference, errorMessages)
	elsif StringsEqual(token, "[".split(""))
		success = GetJSONArray(tokens, i, depth + 1.0, elementReference, errorMessages)
	elsif StringsEqual(token, "true".split(""))
		elementReference.element = CreateBooleanElement(true)
		i.numberValue = i.numberValue + 1.0
	elsif StringsEqual(token, "false".split(""))
		elementReference.element = CreateBooleanElement(false)
		i.numberValue = i.numberValue + 1.0
	elsif StringsEqual(token, "null".split(""))
		elementReference.element = CreateNullElement()
		i.numberValue = i.numberValue + 1.0
	elsif charIsNumber(token[0]) || token[0] == "-"
		stringToDecimalResult = nCreateNumberFromDecimalString(token)
		elementReference.element = CreateNumberElement(stringToDecimalResult)
		i.numberValue = i.numberValue + 1.0
	elsif token[0] == "\""
		substr = strSubstring(token, 1.0, token.length - 1.0)
		elementReference.element = CreateStringElement(substr)
		i.numberValue = i.numberValue + 1.0
	else
		str = "".split("")
		str = strConcatenateString(str, "Invalid token first in value: ".split(""))
		str = strAppendString(str, token)
		lAddStringRef(errorMessages, CreateStringReference(str))
		success = false
	end

	if success && depth == 0.0
		if StringsEqual(tokens[i.numberValue].string, "<end>".split(""))
		else
			lAddStringRef(errorMessages, CreateStringReference("The outer value cannot have any tokens following it.".split("")))
			success = false
		end
	end

	return success
end


def GetJSONObject(tokens, i, depth, elementReference, errorMessages)

	keys = lCreateLinkedListString()
	values = CreateLinkedListElements()
	element = CreateObjectElement(0.0)
	valueReference = ElementReference.new
	success = true
	i.numberValue = i.numberValue + 1.0

	if !StringsEqual(tokens[i.numberValue].string, "}".split(""))
		done = false

		while(!done && success)
			key = tokens[i.numberValue].string

			if key[0] == "\""
				i.numberValue = i.numberValue + 1.0
				colon = tokens[i.numberValue].string
				if StringsEqual(colon, ":".split(""))
					i.numberValue = i.numberValue + 1.0
					success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages)

					if success
						keystring = strSubstring(key, 1.0, key.length - 1.0)
						value = valueReference.element
						lLinkedListAddString(keys, keystring)
						LinkedListAddElement(values, value)

						comma = tokens[i.numberValue].string
						if StringsEqual(comma, ",".split(""))
							# OK
							i.numberValue = i.numberValue + 1.0
						else
							done = true
						end
					end
				else
					str = "".split("")
					str = strConcatenateString(str, "Expected colon after key in object: ".split(""))
					str = strAppendString(str, colon)
					lAddStringRef(errorMessages, CreateStringReference(str))

					success = false
					done = true
				end
			else
				lAddStringRef(errorMessages, CreateStringReference("Expected string as key in object.".split("")))

				done = true
				success = false
			end
		end
	end

	if success
		closeCurly = tokens[i.numberValue].string

		if StringsEqual(closeCurly, "}".split(""))
			# OK
			delete(element.object.stringListRef.stringArray)
			delete(element.object.elementListRef.array)
			element.object.stringListRef.stringArray = lLinkedListStringsToArray(keys)
			element.object.elementListRef.array = LinkedListElementsToArray(values)
			elementReference.element = element
			i.numberValue = i.numberValue + 1.0
		else
			lAddStringRef(errorMessages, CreateStringReference("Expected close curly brackets at end of object value.".split("")))
			success = false
		end
	end

	lFreeLinkedListString(keys)
	FreeLinkedListElements(values)
	delete(valueReference)

	return success
end


def GetJSONArray(tokens, i, depth, elementReference, errorMessages)

	elements = CreateLinkedListElements()
	i.numberValue = i.numberValue + 1.0

	valueReference = ElementReference.new
	success = true
	element = CreateArrayElement(0.0)

	nextToken = tokens[i.numberValue].string

	if !StringsEqual(nextToken, "]".split(""))
		done = false
		while(!done && success)
			success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages)

			if success
				value = valueReference.element
				LinkedListAddElement(elements, value)

				comma = tokens[i.numberValue].string

				if StringsEqual(comma, ",".split(""))
					# OK
					i.numberValue = i.numberValue + 1.0
				else
					done = true
				end
			end
		end
	end

	nextToken = tokens[i.numberValue].string
	if StringsEqual(nextToken, "]".split(""))
		# OK
		i.numberValue = i.numberValue + 1.0
		delete(element.array)
		element.array = LinkedListElementsToArray(elements)
	else
		lAddStringRef(errorMessages, CreateStringReference("Expected close square bracket at end of array.".split("")))
		success = false
	end

	elementReference.element = element
	FreeLinkedListElements(elements)
	delete(valueReference)

	return success
end


def GetStringElementMapKeySet(stringElementMap)
	return stringElementMap.stringListRef
end


def GetObjectValue(stringElementMap, key)

	result = Element.new

	i = 0.0
	while(i < GetStringElementMapNumberOfKeys(stringElementMap))
		if StringsEqual(stringElementMap.stringListRef.stringArray[i].string, key)
			result = stringElementMap.elementListRef.array[i]
		end
		i = i + 1.0
	end

	return result
end


def GetObjectValueWithCheck(stringElementMap, key, foundReference)

	result = Element.new

	foundReference.booleanValue = false
	i = 0.0
	while(i < GetStringElementMapNumberOfKeys(stringElementMap))
		if StringsEqual(stringElementMap.stringListRef.stringArray[i].string, key)
			result = stringElementMap.elementListRef.array[i]
			foundReference.booleanValue = true
		end
		i = i + 1.0
	end

	return result
end


def PutStringElementMap(stringElementMap, keystring, value)
	lAddStringRef(stringElementMap.stringListRef, CreateStringReference(keystring))
	AddElementRef(stringElementMap.elementListRef, value)
end


def SetStringElementMap(stringElementMap, index, keystring, value)
	stringElementMap.stringListRef.stringArray[index].string = keystring
	stringElementMap.elementListRef.array[index] = value
end


def GetStringElementMapNumberOfKeys(stringElementMap)
	return stringElementMap.stringListRef.stringArray.length
end


def JSONCompare(a, b, epsilon, equal, errorMessage)

	eaRef = ElementReference.new
	ebRef = ElementReference.new

	success = ReadJSON(a, eaRef, errorMessage)

	if success
		ea = eaRef.element

		success = ReadJSON(b, ebRef, errorMessage)

		if success
			eb = ebRef.element

			equal.booleanValue = JSONCompareElements(ea, eb, epsilon)

			DeleteElement(eb)
		end

		DeleteElement(ea)
	end

	return success
end


def JSONCompareElements(ea, eb, epsilon)

	equal = StringsEqual(ea.type, eb.type)
        
	if equal
		typeName = ea.type
		if StringsEqual(typeName, "object".split(""))
			equal = JSONCompareObjects(ea.object, eb.object, epsilon)
		elsif StringsEqual(typeName, "string".split(""))
			equal = StringsEqual(ea.string, eb.string)
		elsif StringsEqual(typeName, "array".split(""))
			equal = JSONCompareArrays(ea.array, eb.array, epsilon)
		elsif StringsEqual(typeName, "number".split(""))
			equal = EpsilonCompare(ea.number, eb.number, epsilon)
		elsif StringsEqual(typeName, "null".split(""))
			equal = true
		elsif StringsEqual(typeName, "boolean".split(""))
			equal = ea.booleanValue == eb.booleanValue
		end
	end
        
	return equal
end


def JSONCompareArrays(ea, eb, epsilon)

	equals = ea.length == eb.length

	if equals
		length = ea.length
		i = 0.0
		while(i < length && equals)
			equals = JSONCompareElements(ea[i], eb[i], epsilon)
			i = i + 1.0
		end
	end

	return equals
end


def JSONCompareObjects(ea, eb, epsilon)

	aFoundReference = BooleanReference.new
	bFoundReference = BooleanReference.new

	akeys = GetStringElementMapNumberOfKeys(ea)
	bkeys = GetStringElementMapNumberOfKeys(eb)

	equals = akeys == bkeys

	if equals
		keys = GetStringElementMapKeySet(ea)

		i = 0.0
		while(i < keys.stringArray.length && equals)
			key = keys.stringArray[i].string

			eaValue = GetObjectValueWithCheck(ea, key, aFoundReference)
			ebValue = GetObjectValueWithCheck(eb, key, bFoundReference)

			if aFoundReference.booleanValue && bFoundReference.booleanValue
				equals = JSONCompareElements(eaValue, ebValue, epsilon)
			else
				equals = false
			end
			i = i + 1.0
		end
	end

	return equals
end


def AddNumber(list, a)

	newlist = Array.new(list.length + 1.0)
	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def AddNumberRef(list, i)
	list.numberArray = AddNumber(list.numberArray, i)
end


def RemoveNumber(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end
		
	return newlist
end


def GetNumberRef(list, i)
	return list.numberArray[i]
end


def RemoveNumberRef(list, i)
	list.numberArray = RemoveNumber(list.numberArray, i)
end


def AddString(list, a)

	newlist = Array.new(list.length + 1.0)

	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def AddStringRef(list, i)
	list.stringArray = AddString(list.stringArray, i)
end


def RemoveString(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end
		
	return newlist
end


def GetStringRef(list, i)
	return list.stringArray[i]
end


def RemoveStringRef(list, i)
	list.stringArray = RemoveString(list.stringArray, i)
end


def CreateDynamicArrayCharacters()

	da = DynamicArrayCharacters.new
	da.array = Array.new(10)
	da.length = 0.0

	return da
end


def CreateDynamicArrayCharactersWithInitialCapacity(capacity)

	da = DynamicArrayCharacters.new
	da.array = Array.new(capacity)
	da.length = 0.0

	return da
end


def DynamicArrayAddCharacter(da, value)
	if da.length == da.array.length
		DynamicArrayCharactersIncreaseSize(da)
	end

	da.array[da.length] = value
	da.length = da.length + 1.0
end


def DynamicArrayCharactersIncreaseSize(da)

	newLength = (da.array.length*3.0.to_f / 2.0).round
	newArray = Array.new(newLength)

	i = 0.0
	while(i < da.array.length)
		newArray[i] = da.array[i]
		i = i + 1.0
	end

	delete(da.array)

	da.array = newArray
end


def DynamicArrayCharactersDecreaseSizeNecessary(da)

	needsDecrease = false

	if da.length > 10.0
		needsDecrease = da.length <= (da.array.length*2.0.to_f / 3.0).round
	end

	return needsDecrease
end


def DynamicArrayCharactersDecreaseSize(da)

	newLength = (da.array.length*2.0.to_f / 3.0).round
	newArray = Array.new(newLength)

	i = 0.0
	while(i < newLength)
		newArray[i] = da.array[i]
		i = i + 1.0
	end

	delete(da.array)

	da.array = newArray
end


def DynamicArrayCharactersIndex(da, index)
	return da.array[index]
end


def DynamicArrayCharactersLength(da)
	return da.length
end


def DynamicArrayInsertCharacter(da, index, value)

	if da.length == da.array.length
		DynamicArrayCharactersIncreaseSize(da)
	end

	i = da.length
	while(i > index)
		da.array[i] = da.array[i - 1.0]
		i = i - 1.0
	end

	da.array[index] = value

	da.length = da.length + 1.0
end


def DynamicArrayCharacterSet(da, index, value)

	if index < da.length
		da.array[index] = value
		success = true
	else
		success = false
	end

	return success
end


def DynamicArrayRemoveCharacter(da, index)

	i = index
	while(i < da.length - 1.0)
		da.array[i] = da.array[i + 1.0]
		i = i + 1.0
	end

	da.length = da.length - 1.0

	if DynamicArrayCharactersDecreaseSizeNecessary(da)
		DynamicArrayCharactersDecreaseSize(da)
	end
end


def FreeDynamicArrayCharacters(da)
	delete(da.array)
	delete(da)
end


def DynamicArrayCharactersToArray(da)

	array = Array.new(da.length)

	i = 0.0
	while(i < da.length)
		array[i] = da.array[i]
		i = i + 1.0
	end

	return array
end


def ArrayToDynamicArrayCharactersWithOptimalSize(array)

	c = array.length
	n = (Math.log(c) - 1.0).to_f / Math.log(3.0.to_f / 2.0)
	newCapacity = (n).floor + 1.0

	da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity)

	i = 0.0
	while(i < array.length)
		da.array[i] = array[i]
		i = i + 1.0
	end

	return da
end


def ArrayToDynamicArrayCharacters(array)

	da = DynamicArrayCharacters.new
	da.array = CopyString(array)
	da.length = array.length

	return da
end


def DynamicArrayCharactersEqual(a, b)

	equal = true
	if a.length == b.length
		i = 0.0
		while(i < a.length && equal)
			if a.array[i] != b.array[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def DynamicArrayCharactersToLinkedList(da)

	ll = CreateLinkedListCharacter()

	i = 0.0
	while(i < da.length)
		LinkedListAddCharacter(ll, da.array[i])
		i = i + 1.0
	end

	return ll
end


def LinkedListToDynamicArrayCharacters(ll)

	node = ll.first

	da = DynamicArrayCharacters.new
	da.length = LinkedListCharactersLength(ll)

	da.array = Array.new(da.length)

	i = 0.0
	while(i < da.length)
		da.array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return da
end


def AddBoolean(list, a)

	newlist = Array.new(list.length + 1.0)
	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def AddBooleanRef(list, i)
	list.booleanArray = AddBoolean(list.booleanArray, i)
end


def RemoveBoolean(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end
		
	return newlist
end


def GetBooleanRef(list, i)
	return list.booleanArray[i]
end


def RemoveDecimalRef(list, i)
	list.booleanArray = RemoveBoolean(list.booleanArray, i)
end


def CreateLinkedListString()

	ll = LinkedListStrings.new
	ll.first = LinkedListNodeStrings.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def LinkedListAddString(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LinkedListNodeStrings.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def LinkedListStringsToArray(ll)

	node = ll.first

	length = LinkedListStringsLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = StringReference.new
		array[i].string = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def LinkedListStringsLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def FreeLinkedListString(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def CreateLinkedListNumbers()

	ll = LinkedListNumbers.new
	ll.first = LinkedListNodeNumbers.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def CreateLinkedListNumbersArray(length)

	lls = Array.new(length)
	i = 0.0
	while(i < lls.length)
		lls[i] = CreateLinkedListNumbers()
		i = i + 1.0
	end

	return lls
end


def LinkedListAddNumber(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LinkedListNodeNumbers.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def LinkedListNumbersLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def LinkedListNumbersIndex(ll, index)

	node = ll.first
	i = 0.0
	while(i < index)
		node = node.nextx
		i = i + 1.0
	end

	return node.value
end


def LinkedListInsertNumber(ll, index, value)

	if index == 0.0
		tmp = ll.first
		ll.first = LinkedListNodeNumbers.new
		ll.first.nextx = tmp
		ll.first.value = value
		ll.first.endx = false
	else
		node = ll.first
		i = 0.0
		while(i < index - 1.0)
			node = node.nextx
			i = i + 1.0
		end

		tmp = node.nextx
		node.nextx = LinkedListNodeNumbers.new
		node.nextx.nextx = tmp
		node.nextx.value = value
		node.nextx.endx = false
	end
end


def LinkedListSet(ll, index, value)

	node = ll.first
	i = 0.0
	while(i < index)
		node = node.nextx
		i = i + 1.0
	end

	node.nextx.value = value
end


def LinkedListRemoveNumber(ll, index)

	node = ll.first
	prev = ll.first

	i = 0.0
	while(i < index)
		prev = node
		node = node.nextx
		i = i + 1.0
	end

	if index == 0.0
		ll.first = prev.nextx
	end
	if !prev.nextx.endx
		prev.nextx = prev.nextx.nextx
	end
end


def FreeLinkedListNumbers(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def FreeLinkedListNumbersArray(lls)

	i = 0.0
	while(i < lls.length)
		FreeLinkedListNumbers(lls[i])
		i = i + 1.0
	end
	delete(lls)
end


def LinkedListNumbersToArray(ll)

	node = ll.first

	length = LinkedListNumbersLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def ArrayToLinkedListNumbers(array)

	ll = CreateLinkedListNumbers()

	i = 0.0
	while(i < array.length)
		LinkedListAddNumber(ll, array[i])
		i = i + 1.0
	end

	return ll
end


def LinkedListNumbersEqual(a, b)

	an = a.first
	bn = b.first

	equal = true
	done = false
	while(equal && !done)
		if an.endx == bn.endx
			if an.endx
				done = true
			elsif an.value == bn.value
				an = an.nextx
				bn = bn.nextx
			else
				equal = false
			end
		else
			equal = false
		end
	end

	return equal
end


def CreateLinkedListCharacter()

	ll = LinkedListCharacters.new
	ll.first = LinkedListNodeCharacters.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def LinkedListAddCharacter(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LinkedListNodeCharacters.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def LinkedListCharactersToArray(ll)

	node = ll.first

	length = LinkedListCharactersLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def LinkedListCharactersLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def FreeLinkedListCharacter(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def LinkedListCharactersAddString(ll, str)

	i = 0.0
	while(i < str.length)
		LinkedListAddCharacter(ll, str[i])
		i = i + 1.0
	end
end


def CreateDynamicArrayNumbers()

	da = DynamicArrayNumbers.new
	da.array = Array.new(10)
	da.length = 0.0

	return da
end


def CreateDynamicArrayNumbersWithInitialCapacity(capacity)

	da = DynamicArrayNumbers.new
	da.array = Array.new(capacity)
	da.length = 0.0

	return da
end


def DynamicArrayAddNumber(da, value)
	if da.length == da.array.length
		DynamicArrayNumbersIncreaseSize(da)
	end

	da.array[da.length] = value
	da.length = da.length + 1.0
end


def DynamicArrayNumbersIncreaseSize(da)

	newLength = (da.array.length*3.0.to_f / 2.0).round
	newArray = Array.new(newLength)

	i = 0.0
	while(i < da.array.length)
		newArray[i] = da.array[i]
		i = i + 1.0
	end

	delete(da.array)

	da.array = newArray
end


def DynamicArrayNumbersDecreaseSizeNecessary(da)

	needsDecrease = false

	if da.length > 10.0
		needsDecrease = da.length <= (da.array.length*2.0.to_f / 3.0).round
	end

	return needsDecrease
end


def DynamicArrayNumbersDecreaseSize(da)

	newLength = (da.array.length*2.0.to_f / 3.0).round
	newArray = Array.new(newLength)

	i = 0.0
	while(i < newLength)
		newArray[i] = da.array[i]
		i = i + 1.0
	end

	delete(da.array)

	da.array = newArray
end


def DynamicArrayNumbersIndex(da, index)
	return da.array[index]
end


def DynamicArrayNumbersLength(da)
	return da.length
end


def DynamicArrayInsertNumber(da, index, value)

	if da.length == da.array.length
		DynamicArrayNumbersIncreaseSize(da)
	end

	i = da.length
	while(i > index)
		da.array[i] = da.array[i - 1.0]
		i = i - 1.0
	end

	da.array[index] = value

	da.length = da.length + 1.0
end


def DynamicArrayNumberSet(da, index, value)

	if index < da.length
		da.array[index] = value
		success = true
	else
		success = false
	end

	return success
end


def DynamicArrayRemoveNumber(da, index)

	i = index
	while(i < da.length - 1.0)
		da.array[i] = da.array[i + 1.0]
		i = i + 1.0
	end

	da.length = da.length - 1.0

	if DynamicArrayNumbersDecreaseSizeNecessary(da)
		DynamicArrayNumbersDecreaseSize(da)
	end
end


def FreeDynamicArrayNumbers(da)
	delete(da.array)
	delete(da)
end


def DynamicArrayNumbersToArray(da)

	array = Array.new(da.length)

	i = 0.0
	while(i < da.length)
		array[i] = da.array[i]
		i = i + 1.0
	end

	return array
end


def ArrayToDynamicArrayNumbersWithOptimalSize(array)

=begin

         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
        
=end

	c = array.length
	n = (Math.log(c) - 1.0).to_f / Math.log(3.0.to_f / 2.0)
	newCapacity = (n).floor + 1.0

	da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity)

	i = 0.0
	while(i < array.length)
		da.array[i] = array[i]
		i = i + 1.0
	end

	return da
end


def ArrayToDynamicArrayNumbers(array)

	da = DynamicArrayNumbers.new
	da.array = CopyNumberArray(array)
	da.length = array.length

	return da
end


def DynamicArrayNumbersEqual(a, b)

	equal = true
	if a.length == b.length
		i = 0.0
		while(i < a.length && equal)
			if a.array[i] != b.array[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def DynamicArrayNumbersToLinkedList(da)

	ll = CreateLinkedListNumbers()

	i = 0.0
	while(i < da.length)
		LinkedListAddNumber(ll, da.array[i])
		i = i + 1.0
	end

	return ll
end


def LinkedListToDynamicArrayNumbers(ll)

	node = ll.first

	da = DynamicArrayNumbers.new
	da.length = LinkedListNumbersLength(ll)

	da.array = Array.new(da.length)

	i = 0.0
	while(i < da.length)
		da.array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return da
end


def AddCharacter(list, a)

	newlist = Array.new(list.length + 1.0)
	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def AddCharacterRef(list, i)
	list.string = AddCharacter(list.string, i)
end


def RemoveCharacter(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end

	return newlist
end


def GetCharacterRef(list, i)
	return list.string[i]
end


def RemoveCharacterRef(list, i)
	list.string = RemoveCharacter(list.string, i)
end


def sWriteStringToStingStream(stream, index, src)

	i = 0.0
	while(i < src.length)
		stream[index.numberValue + i] = src[i]
		i = i + 1.0
	end
	index.numberValue = index.numberValue + src.length
end


def sWriteCharacterToStingStream(stream, index, src)
	stream[index.numberValue] = src
	index.numberValue = index.numberValue + 1.0
end


def sWriteBooleanToStingStream(stream, index, src)
	if src
		sWriteStringToStingStream(stream, index, "true".split(""))
	else
		sWriteStringToStingStream(stream, index, "false".split(""))
	end
end


def sSubstringWithCheck(string, from, to, stringReference)

	if from >= 0.0 && from <= string.length && to >= 0.0 && to <= string.length && from <= to
		stringReference.string = sSubstring(string, from, to)
		success = true
	else
		success = false
	end

	return success
end


def sSubstring(string, from, to)

	length = to - from

	n = Array.new(length)

	i = from
	while(i < to)
		n[i - from] = string[i]
		i = i + 1.0
	end

	return n
end


def sAppendString(s1, s2)

	newString = sConcatenateString(s1, s2)

	delete(s1)

	return newString
end


def sConcatenateString(s1, s2)

	newString = Array.new(s1.length + s2.length)

	i = 0.0
	while(i < s1.length)
		newString[i] = s1[i]
		i = i + 1.0
	end

	i = 0.0
	while(i < s2.length)
		newString[s1.length + i] = s2[i]
		i = i + 1.0
	end

	return newString
end


def sAppendCharacter(string, c)

	newString = sConcatenateCharacter(string, c)

	delete(string)

	return newString
end


def sConcatenateCharacter(string, c)
	newString = Array.new(string.length + 1.0)

	i = 0.0
	while(i < string.length)
		newString[i] = string[i]
		i = i + 1.0
	end

	newString[string.length] = c

	return newString
end


def sSplitByCharacter(toSplit, splitBy)

	stringToSplitBy = Array.new(1)
	stringToSplitBy[0] = splitBy

	split = sSplitByString(toSplit, stringToSplitBy)

	delete(stringToSplitBy)

	return split
end


def sIndexOfCharacter(string, character, indexReference)

	found = false
	i = 0.0
	while(i < string.length && !found)
		if string[i] == character
			found = true
			indexReference.numberValue = i
		end
		i = i + 1.0
	end

	return found
end


def sSubstringEqualsWithCheck(string, from, substring, equalsReference)

	if from < string.length
		success = true
		equalsReference.booleanValue = sSubstringEquals(string, from, substring)
	else
		success = false
	end

	return success
end


def sSubstringEquals(string, from, substring)

	equal = true
	if string.length - from >= substring.length
		i = 0.0
		while(i < substring.length && equal)
			if string[from + i] != substring[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def sIndexOfString(string, substring, indexReference)

	found = false
	i = 0.0
	while(i < string.length - substring.length + 1.0 && !found)
		if sSubstringEquals(string, i, substring)
			found = true
			indexReference.numberValue = i
		end
		i = i + 1.0
	end

	return found
end


def sContainsCharacter(string, character)

	found = false
	i = 0.0
	while(i < string.length && !found)
		if string[i] == character
			found = true
		end
		i = i + 1.0
	end

	return found
end


def sContainsString(string, substring)
	return sIndexOfString(string, substring, NumberReference.new)
end


def sToUpperCase(string)

	i = 0.0
	while(i < string.length)
		string[i] = charToUpperCase(string[i])
		i = i + 1.0
	end
end


def sToLowerCase(string)

	i = 0.0
	while(i < string.length)
		string[i] = charToLowerCase(string[i])
		i = i + 1.0
	end
end


def sEqualsIgnoreCase(a, b)

	if a.length == b.length
		equal = true
		i = 0.0
		while(i < a.length && equal)
			if charToLowerCase(a[i]) != charToLowerCase(b[i])
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def sReplaceString(string, toReplace, replaceWith)

	da = CreateDynamicArrayCharacters()

	equalsReference = BooleanReference.new

	i = 0.0
	while(i < string.length)
		success = sSubstringEqualsWithCheck(string, i, toReplace, equalsReference)
		if success
			success = equalsReference.booleanValue
		end

		if success && toReplace.length > 0.0
			j = 0.0
			while(j < replaceWith.length)
				DynamicArrayAddCharacter(da, replaceWith[j])
				j = j + 1.0
			end
			i = i + toReplace.length
		else
			DynamicArrayAddCharacter(da, string[i])
			i = i + 1.0
		end
	end

	result = DynamicArrayCharactersToArray(da)

	FreeDynamicArrayCharacters(da)

	return result
end


def sReplaceCharacterToNew(string, toReplace, replaceWith)

	result = Array.new(string.length)

	i = 0.0
	while(i < string.length)
		if string[i] == toReplace
			result[i] = replaceWith
		else
			result[i] = string[i]
		end
		i = i + 1.0
	end

	return result
end


def sReplaceCharacter(string, toReplace, replaceWith)

	i = 0.0
	while(i < string.length)
		if string[i] == toReplace
			string[i] = replaceWith
		end
		i = i + 1.0
	end
end


def sTrim(string)

	# Find whitepaces at the start.
	lastWhitespaceLocationStart = -1.0
	firstNonWhitespaceFound = false
	i = 0.0
	while(i < string.length && !firstNonWhitespaceFound)
		if charIsWhiteSpace(string[i])
			lastWhitespaceLocationStart = i
		else
			firstNonWhitespaceFound = true
		end
		i = i + 1.0
	end

	# Find whitepaces at the end.
	lastWhitespaceLocationEnd = string.length
	firstNonWhitespaceFound = false
	i = string.length - 1.0
	while(i >= 0.0 && !firstNonWhitespaceFound)
		if charIsWhiteSpace(string[i])
			lastWhitespaceLocationEnd = i
		else
			firstNonWhitespaceFound = true
		end
		i = i - 1.0
	end

	if lastWhitespaceLocationStart < lastWhitespaceLocationEnd
		result = sSubstring(string, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd)
	else
		result = Array.new(0)
	end

	return result
end


def sStartsWith(string, start)

	startsWithString = false
	if string.length >= start.length
		startsWithString = sSubstringEquals(string, 0.0, start)
	end

	return startsWithString
end


def sEndsWith(string, endx)

	endsWithString = false
	if string.length >= endx.length
		endsWithString = sSubstringEquals(string, string.length - endx.length, endx)
	end

	return endsWithString
end


def sSplitByString(toSplit, splitBy)

	split = Array.new(0)

	nextx = Array.new(0)
	i = 0.0
	while(i < toSplit.length)
		c = toSplit[i]

		if sSubstringEquals(toSplit, i, splitBy)
			n = StringReference.new
			n.string = nextx
			split = AddString(split, n)
			nextx = Array.new(0)
			i = i + splitBy.length
		else
			nextx = sAppendCharacter(nextx, c)
			i = i + 1.0
		end
	end

	n = StringReference.new
	n.string = nextx
	split = AddString(split, n)

	return split
end


def sStringIsBefore(a, b)

	before = false
	equal = true
	done = false

	if a.length == 0.0 && b.length > 0.0
		before = true
	else
		i = 0.0
		while(i < a.length && i < b.length && !done)
			if a[i] != b[i]
				equal = false
			end
			if charCharacterIsBefore(a[i], b[i])
				before = true
			end
			if charCharacterIsBefore(b[i], a[i])
				done = true
			end
			i = i + 1.0
		end

		if equal
			if a.length < b.length
				before = true
			end
		end
	end

	return before
end


def strWriteStringToStingStream(stream, index, src)

	i = 0.0
	while(i < src.length)
		stream[index.numberValue + i] = src[i]
		i = i + 1.0
	end
	index.numberValue = index.numberValue + src.length
end


def strWriteCharacterToStingStream(stream, index, src)
	stream[index.numberValue] = src
	index.numberValue = index.numberValue + 1.0
end


def strWriteBooleanToStingStream(stream, index, src)
	if src
		strWriteStringToStingStream(stream, index, "true".split(""))
	else
		strWriteStringToStingStream(stream, index, "false".split(""))
	end
end


def strSubstringWithCheck(string, from, to, stringReference)

	if from >= 0.0 && from <= string.length && to >= 0.0 && to <= string.length && from <= to
		stringReference.string = strSubstring(string, from, to)
		success = true
	else
		success = false
	end

	return success
end


def strSubstring(string, from, to)

	length = to - from

	n = Array.new(length)

	i = from
	while(i < to)
		n[i - from] = string[i]
		i = i + 1.0
	end

	return n
end


def strAppendString(s1, s2)

	newString = strConcatenateString(s1, s2)

	delete(s1)

	return newString
end


def strConcatenateString(s1, s2)

	newString = Array.new(s1.length + s2.length)

	i = 0.0
	while(i < s1.length)
		newString[i] = s1[i]
		i = i + 1.0
	end

	i = 0.0
	while(i < s2.length)
		newString[s1.length + i] = s2[i]
		i = i + 1.0
	end

	return newString
end


def strAppendCharacter(string, c)

	newString = strConcatenateCharacter(string, c)

	delete(string)

	return newString
end


def strConcatenateCharacter(string, c)
	newString = Array.new(string.length + 1.0)

	i = 0.0
	while(i < string.length)
		newString[i] = string[i]
		i = i + 1.0
	end

	newString[string.length] = c

	return newString
end


def strSplitByCharacter(toSplit, splitBy)

	stringToSplitBy = Array.new(1)
	stringToSplitBy[0] = splitBy

	split = strSplitByString(toSplit, stringToSplitBy)

	delete(stringToSplitBy)

	return split
end


def strIndexOfCharacter(string, character, indexReference)

	found = false
	i = 0.0
	while(i < string.length && !found)
		if string[i] == character
			found = true
			indexReference.numberValue = i
		end
		i = i + 1.0
	end

	return found
end


def strSubstringEqualsWithCheck(string, from, substring, equalsReference)

	if from < string.length
		success = true
		equalsReference.booleanValue = strSubstringEquals(string, from, substring)
	else
		success = false
	end

	return success
end


def strSubstringEquals(string, from, substring)

	equal = true
	i = 0.0
	while(i < substring.length && equal)
		if string[from + i] != substring[i]
			equal = false
		end
		i = i + 1.0
	end

	return equal
end


def strIndexOfString(string, substring, indexReference)

	found = false
	i = 0.0
	while(i < string.length - substring.length + 1.0 && !found)
		if strSubstringEquals(string, i, substring)
			found = true
			indexReference.numberValue = i
		end
		i = i + 1.0
	end

	return found
end


def strContainsCharacter(string, character)

	found = false
	i = 0.0
	while(i < string.length && !found)
		if string[i] == character
			found = true
		end
		i = i + 1.0
	end

	return found
end


def strContainsString(string, substring)
	return strIndexOfString(string, substring, NumberReference.new)
end


def strToUpperCase(string)

	i = 0.0
	while(i < string.length)
		string[i] = charToUpperCase(string[i])
		i = i + 1.0
	end
end


def strToLowerCase(string)

	i = 0.0
	while(i < string.length)
		string[i] = charToLowerCase(string[i])
		i = i + 1.0
	end
end


def strEqualsIgnoreCase(a, b)

	if a.length == b.length
		equal = true
		i = 0.0
		while(i < a.length && equal)
			if charToLowerCase(a[i]) != charToLowerCase(b[i])
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def strReplaceString(string, toReplace, replaceWith)

	equalsReference = BooleanReference.new
	result = Array.new(0)

	i = 0.0
	while(i < string.length)
		success = strSubstringEqualsWithCheck(string, i, toReplace, equalsReference)
		if success
			success = equalsReference.booleanValue
		end

		if success && toReplace.length > 0.0
			result = strConcatenateString(result, replaceWith)
			i = i + toReplace.length
		else
			result = strConcatenateCharacter(result, string[i])
			i = i + 1.0
		end
	end

	return result
end


def strReplaceCharacter(string, toReplace, replaceWith)

	result = Array.new(0)

	i = 0.0
	while(i < string.length)
		if string[i] == toReplace
			result = strConcatenateCharacter(result, replaceWith)
		else
			result = strConcatenateCharacter(result, string[i])
		end
		i = i + 1.0
	end

	return result
end


def strTrim(string)

	# Find whitepaces at the start.
	lastWhitespaceLocationStart = -1.0
	firstNonWhitespaceFound = false
	i = 0.0
	while(i < string.length && !firstNonWhitespaceFound)
		if charIsWhiteSpace(string[i])
			lastWhitespaceLocationStart = i
		else
			firstNonWhitespaceFound = true
		end
		i = i + 1.0
	end

	# Find whitepaces at the end.
	lastWhitespaceLocationEnd = string.length
	firstNonWhitespaceFound = false
	i = string.length - 1.0
	while(i >= 0.0 && !firstNonWhitespaceFound)
		if charIsWhiteSpace(string[i])
			lastWhitespaceLocationEnd = i
		else
			firstNonWhitespaceFound = true
		end
		i = i - 1.0
	end

	if lastWhitespaceLocationStart < lastWhitespaceLocationEnd
		result = strSubstring(string, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd)
	else
		result = Array.new(0)
	end

	return result
end


def strStartsWith(string, start)

	startsWithString = false
	if string.length >= start.length
		startsWithString = strSubstringEquals(string, 0.0, start)
	end

	return startsWithString
end


def strEndsWith(string, endx)

	endsWithString = false
	if string.length >= endx.length
		endsWithString = strSubstringEquals(string, string.length - endx.length, endx)
	end

	return endsWithString
end


def strSplitByString(toSplit, splitBy)

	split = Array.new(0)

	nextx = Array.new(0)
	i = 0.0
	while(i < toSplit.length)
		c = toSplit[i]

		if strSubstringEquals(toSplit, i, splitBy)
			if split.length != 0.0 || i != 0.0
				n = StringReference.new
				n.string = nextx
				split = lAddString(split, n)
				nextx = Array.new(0)
				i = i + splitBy.length
			end
		else
			nextx = strAppendCharacter(nextx, c)
			i = i + 1.0
		end
	end

	if nextx.length > 0.0
		n = StringReference.new
		n.string = nextx
		split = lAddString(split, n)
	end

	return split
end


def strStringIsBefore(a, b)

	before = false
	equal = true
	done = false

	if a.length == 0.0 && b.length > 0.0
		before = true
	else
		i = 0.0
		while(i < a.length && i < b.length && !done)
			if a[i] != b[i]
				equal = false
			end
			if charCharacterIsBefore(a[i], b[i])
				before = true
			end
			if charCharacterIsBefore(b[i], a[i])
				done = true
			end
			i = i + 1.0
		end

		if equal
			if a.length < b.length
				before = true
			end
		end
	end

	return before
end


def StringToNumberArray(string)

	array = Array.new(string.length)

	i = 0.0
	while(i < string.length)
		array[i] = (string[i]).ord
		i = i + 1.0
	end
	return array
end


def NumberArrayToString(array)

	string = Array.new(array.length)

	i = 0.0
	while(i < array.length)
		string[i] = (array[i]).truncate.chr('UTF-8')
		i = i + 1.0
	end
	return string
end


def NumberArraysEqual(a, b)

	equal = true
	if a.length == b.length
		i = 0.0
		while(i < a.length && equal)
			if a[i] != b[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def BooleanArraysEqual(a, b)

	equal = true
	if a.length == b.length
		i = 0.0
		while(i < a.length && equal)
			if a[i] != b[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def StringsEqual(a, b)

	equal = true
	if a.length == b.length
		i = 0.0
		while(i < a.length && equal)
			if a[i] != b[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def FillNumberArray(a, value)

	i = 0.0
	while(i < a.length)
		a[i] = value
		i = i + 1.0
	end
end


def FillString(a, value)

	i = 0.0
	while(i < a.length)
		a[i] = value
		i = i + 1.0
	end
end


def FillBooleanArray(a, value)

	i = 0.0
	while(i < a.length)
		a[i] = value
		i = i + 1.0
	end
end


def FillNumberArrayRange(a, value, from, to)

	if from >= 0.0 && from <= a.length && to >= 0.0 && to <= a.length && from <= to
		length = to - from
		i = 0.0
		while(i < length)
			a[from + i] = value
			i = i + 1.0
		end

		success = true
	else
		success = false
	end

	return success
end


def FillBooleanArrayRange(a, value, from, to)

	if from >= 0.0 && from <= a.length && to >= 0.0 && to <= a.length && from <= to
		length = to - from
		i = 0.0
		while(i < length)
			a[from + i] = value
			i = i + 1.0
		end

		success = true
	else
		success = false
	end

	return success
end


def FillStringRange(a, value, from, to)

	if from >= 0.0 && from <= a.length && to >= 0.0 && to <= a.length && from <= to
		length = to - from
		i = 0.0
		while(i < length)
			a[from + i] = value
			i = i + 1.0
		end

		success = true
	else
		success = false
	end

	return success
end


def CopyNumberArray(a)

	n = Array.new(a.length)

	i = 0.0
	while(i < a.length)
		n[i] = a[i]
		i = i + 1.0
	end

	return n
end


def CopyBooleanArray(a)

	n = Array.new(a.length)

	i = 0.0
	while(i < a.length)
		n[i] = a[i]
		i = i + 1.0
	end

	return n
end


def CopyString(a)

	n = Array.new(a.length)

	i = 0.0
	while(i < a.length)
		n[i] = a[i]
		i = i + 1.0
	end

	return n
end


def CopyNumberArrayRange(a, from, to, copyReference)

	if from >= 0.0 && from <= a.length && to >= 0.0 && to <= a.length && from <= to
		length = to - from
		n = Array.new(length)

		i = 0.0
		while(i < length)
			n[i] = a[from + i]
			i = i + 1.0
		end

		copyReference.numberArray = n
		success = true
	else
		success = false
	end

	return success
end


def CopyBooleanArrayRange(a, from, to, copyReference)

	if from >= 0.0 && from <= a.length && to >= 0.0 && to <= a.length && from <= to
		length = to - from
		n = Array.new(length)

		i = 0.0
		while(i < length)
			n[i] = a[from + i]
			i = i + 1.0
		end

		copyReference.booleanArray = n
		success = true
	else
		success = false
	end

	return success
end


def CopyStringRange(a, from, to, copyReference)

	if from >= 0.0 && from <= a.length && to >= 0.0 && to <= a.length && from <= to
		length = to - from
		n = Array.new(length)

		i = 0.0
		while(i < length)
			n[i] = a[from + i]
			i = i + 1.0
		end

		copyReference.string = n
		success = true
	else
		success = false
	end

	return success
end


def IsLastElement(length, index)
	return index + 1.0 == length
end


def CreateNumberArray(length, value)

	array = Array.new(length)
	FillNumberArray(array, value)

	return array
end


def CreateBooleanArray(length, value)

	array = Array.new(length)
	FillBooleanArray(array, value)

	return array
end


def CreateString(length, value)

	array = Array.new(length)
	FillString(array, value)

	return array
end


def SwapElementsOfNumberArray(a, ai, bi)

	tmp = a[ai]
	a[ai] = a[bi]
	a[bi] = tmp
end


def SwapElementsOfStringArray(a, ai, bi)

	tmp = a.stringArray[ai]
	a.stringArray[ai] = a.stringArray[bi]
	a.stringArray[bi] = tmp
end


def ReverseNumberArray(array)

	i = 0.0
	while(i < array.length.to_f / 2.0)
		SwapElementsOfNumberArray(array, i, array.length - i - 1.0)
		i = i + 1.0
	end
end


def AssertFalse(b, failures)
	if b
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertTrue(b, failures)
	if !b
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertEquals(a, b, failures)
	if a != b
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertBooleansEqual(a, b, failures)
	if a != b
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertCharactersEqual(a, b, failures)
	if a != b
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertStringEquals(a, b, failures)
	if !StringsEqual(a, b)
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertNumberArraysEqual(a, b, failures)

	if a.length == b.length
		i = 0.0
		while(i < a.length)
			AssertEquals(a[i], b[i], failures)
			i = i + 1.0
		end
	else
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertBooleanArraysEqual(a, b, failures)

	if a.length == b.length
		i = 0.0
		while(i < a.length)
			AssertBooleansEqual(a[i], b[i], failures)
			i = i + 1.0
		end
	else
		failures.numberValue = failures.numberValue + 1.0
	end
end


def AssertStringArraysEqual(a, b, failures)

	if a.length == b.length
		i = 0.0
		while(i < a.length)
			AssertStringEquals(a[i].string, b[i].string, failures)
			i = i + 1.0
		end
	else
		failures.numberValue = failures.numberValue + 1.0
	end
end


def nCreateStringScientificNotationDecimalFromNumber(decimal)

	mantissaReference = StringReference.new
	exponentReference = StringReference.new
	result = Array.new(0)
	done = false
	exponent = 0.0

	if decimal < 0.0
		isPositive = false
		decimal = -decimal
	else
		isPositive = true
	end

	if decimal == 0.0
		done = true
	end

	if !done
		multiplier = 0.0
		inc = 0.0

		if decimal < 1.0
			multiplier = 10.0
			inc = -1.0
		elsif decimal >= 10.0
			multiplier = 0.1
			inc = 1.0
		else
			done = true
		end

		if !done
			while(decimal >= 10.0 || decimal < 1.0)
				decimal = decimal*multiplier
				exponent = exponent + inc
			end
		end
	end

	nCreateStringFromNumberWithCheck(decimal, 10.0, mantissaReference)

	nCreateStringFromNumberWithCheck(exponent, 10.0, exponentReference)

	if !isPositive
		result = strAppendString(result, "-".split(""))
	end

	result = strAppendString(result, mantissaReference.string)
	result = strAppendString(result, "e".split(""))
	result = strAppendString(result, exponentReference.string)

	return result
end


def nCreateStringDecimalFromNumber(decimal)

	stringReference = StringReference.new

	# This will succeed because base = 10.
	nCreateStringFromNumberWithCheck(decimal, 10.0, stringReference)

	return stringReference.string
end


def nCreateStringFromNumberWithCheck(decimal, base, stringReference)

	isPositive = true

	if decimal < 0.0
		isPositive = false
		decimal = -decimal
	end

	if decimal == 0.0
		stringReference.string = "0".split("")
		success = true
	else
		characterReference = CharacterReference.new

		if IsInteger(base)
			success = true

			string = Array.new(0)

			maximumDigits = nGetMaximumDigitsForBase(base)

			digitPosition = nGetFirstDigitPosition(decimal, base)

			decimal = (decimal*base**(maximumDigits - digitPosition - 1.0)).round

			hasPrintedPoint = false

			if !isPositive
				string = strAppendCharacter(string, "-")
			end

			# Print leading zeros.
			if digitPosition < 0.0
				string = strAppendCharacter(string, "0")
				string = strAppendCharacter(string, ".")
				hasPrintedPoint = true
				i = 0.0
				while(i < -digitPosition - 1.0)
					string = strAppendCharacter(string, "0")
					i = i + 1.0
				end
			end

			# Print number.
			i = 0.0
			while(i < maximumDigits && success)
				d = (decimal.to_f / base**(maximumDigits - i - 1.0)).floor

				if d >= base
					d = base - 1.0
				end

				if !hasPrintedPoint && digitPosition - i + 1.0 == 0.0
					if decimal != 0.0
						string = strAppendCharacter(string, ".")
					end
					hasPrintedPoint = true
				end

				if decimal == 0.0 && hasPrintedPoint
				else
					success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference)
					if success
						c = characterReference.characterValue
						string = strAppendCharacter(string, c)
					end
				end

				if success
					decimal = decimal - d*base**(maximumDigits - i - 1.0)
				end
				i = i + 1.0
			end

			if success
				# Print trailing zeros.
				i = 0.0
				while(i < digitPosition - maximumDigits + 1.0)
					string = strAppendCharacter(string, "0")
					i = i + 1.0
				end

				stringReference.string = string
			end
		else
			success = false
		end
	end

	# Done
	return success
end


def nGetMaximumDigitsForBase(base)

	t = 10.0**15.0
	return (Math.log10(t).to_f / Math.log10(base)).floor
end


def nGetFirstDigitPosition(decimal, base)

	power = (Math.log10(decimal).to_f / Math.log10(base)).ceil

	t = decimal*base**(-power)
	if t < base && t >= 1.0
	elsif t >= base
		power = power + 1.0
	elsif t < 1.0
		power = power - 1.0
	end

	return power
end


def nGetSingleDigitCharacterFromNumberWithCheck(c, base, characterReference)

	numberTable = nGetDigitCharacterTable()

	if c < base || c < numberTable.length
		success = true
		characterReference.characterValue = numberTable[c]
	else
		success = false
	end

	return success
end


def nGetDigitCharacterTable()

	numberTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".split("")

	return numberTable
end


def nCreateNumberFromDecimalStringWithCheck(string, decimalReference, errorMessage)
	return nCreateNumberFromStringWithCheck(string, 10.0, decimalReference, errorMessage)
end


def nCreateNumberFromDecimalString(string)

	doubleReference = CreateNumberReference(0.0)
	stringReference = CreateStringReference("".split(""))
	nCreateNumberFromStringWithCheck(string, 10.0, doubleReference, stringReference)
	number = doubleReference.numberValue

	delete(doubleReference)
	delete(stringReference)

	return number
end


def nCreateNumberFromStringWithCheck(string, base, numberReference, errorMessage)

	numberIsPositive = CreateBooleanReference(true)
	exponentIsPositive = CreateBooleanReference(true)
	beforePoint = NumberArrayReference.new
	afterPoint = NumberArrayReference.new
	exponent = NumberArrayReference.new

	if base >= 2.0 && base <= 36.0
		success = nExtractPartsFromNumberString(string, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage)

		if success
			numberReference.numberValue = nCreateNumberFromParts(base, numberIsPositive.booleanValue, beforePoint.numberArray, afterPoint.numberArray, exponentIsPositive.booleanValue, exponent.numberArray)
		end
	else
		success = false
		errorMessage.string = "Base must be from 2 to 36.".split("")
	end

	return success
end


def nCreateNumberFromParts(base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent)

	n = 0.0

	i = 0.0
	while(i < beforePoint.length)
		p = beforePoint[beforePoint.length - i - 1.0]

		n = n + p*base**i
		i = i + 1.0
	end

	i = 0.0
	while(i < afterPoint.length)
		p = afterPoint[i]

		n = n + p*base**(-(i + 1.0))
		i = i + 1.0
	end

	if exponent.length > 0.0
		e = 0.0
		i = 0.0
		while(i < exponent.length)
			p = exponent[exponent.length - i - 1.0]

			e = e + p*base**i
			i = i + 1.0
		end

		if !exponentIsPositive
			e = -e
		end

		n = n*base**e
	end

	if !numberIsPositive
		n = -n
	end

	return n
end


def nExtractPartsFromNumberString(n, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages)

	i = 0.0

	if i < n.length
		if n[i] == "-"
			numberIsPositive.booleanValue = false
			i = i + 1.0
		elsif n[i] == "+"
			numberIsPositive.booleanValue = true
			i = i + 1.0
		end

		success = nExtractPartsFromNumberStringFromSign(n, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages)
	else
		success = false
		errorMessages.string = "Number cannot have length zero.".split("")
	end

	return success
end


def nExtractPartsFromNumberStringFromSign(n, base, i, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessages)

	done = false
	count = 0.0
	while(i + count < n.length && !done)
		if nCharacterIsNumberCharacterInBase(n[i + count], base)
			count = count + 1.0
		else
			done = true
		end
	end

	if count >= 1.0
		beforePoint.numberArray = Array.new(count)

		j = 0.0
		while(j < count)
			beforePoint.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base)
			j = j + 1.0
		end

		i = i + count

		if i < n.length
			success = nExtractPartsFromNumberStringFromPointOrExponent(n, base, i, afterPoint, exponentIsPositive, exponent, errorMessages)
		else
			afterPoint.numberArray = Array.new(0)
			exponent.numberArray = Array.new(0)
			success = true
		end
	else
		success = false
		errorMessages.string = "Number must have at least one number after the optional sign.".split("")
	end

	return success
end


def nExtractPartsFromNumberStringFromPointOrExponent(n, base, i, afterPoint, exponentIsPositive, exponent, errorMessages)

	if n[i] == "."
		i = i + 1.0

		if i < n.length
			done = false
			count = 0.0
			while(i + count < n.length && !done)
				if nCharacterIsNumberCharacterInBase(n[i + count], base)
					count = count + 1.0
				else
					done = true
				end
			end

			if count >= 1.0
				afterPoint.numberArray = Array.new(count)

				j = 0.0
				while(j < count)
					afterPoint.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base)
					j = j + 1.0
				end

				i = i + count

				if i < n.length
					success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages)
				else
					exponent.numberArray = Array.new(0)
					success = true
				end
			else
				success = false
				errorMessages.string = "There must be at least one digit after the decimal point.".split("")
			end
		else
			success = false
			errorMessages.string = "There must be at least one digit after the decimal point.".split("")
		end
	elsif base <= 14.0 && (n[i] == "e" || n[i] == "E")
		if i < n.length
			success = nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages)
			afterPoint.numberArray = Array.new(0)
		else
			success = false
			errorMessages.string = "There must be at least one digit after the exponent.".split("")
		end
	else
		success = false
		errorMessages.string = "Expected decimal point or exponent symbol.".split("")
	end

	return success
end


def nExtractPartsFromNumberStringFromExponent(n, base, i, exponentIsPositive, exponent, errorMessages)

	if base <= 14.0 && (n[i] == "e" || n[i] == "E")
		i = i + 1.0

		if i < n.length
			if n[i] == "-"
				exponentIsPositive.booleanValue = false
				i = i + 1.0
			elsif n[i] == "+"
				exponentIsPositive.booleanValue = true
				i = i + 1.0
			end

			if i < n.length
				done = false
				count = 0.0
				while(i + count < n.length && !done)
					if nCharacterIsNumberCharacterInBase(n[i + count], base)
						count = count + 1.0
					else
						done = true
					end
				end

				if count >= 1.0
					exponent.numberArray = Array.new(count)

					j = 0.0
					while(j < count)
						exponent.numberArray[j] = nGetNumberFromNumberCharacterForBase(n[i + j], base)
						j = j + 1.0
					end

					i = i + count

					if i == n.length
						success = true
					else
						success = false
						errorMessages.string = "There cannot be any characters past the exponent of the number.".split("")
					end
				else
					success = false
					errorMessages.string = "There must be at least one digit after the decimal point.".split("")
				end
			else
				success = false
				errorMessages.string = "There must be at least one digit after the exponent symbol.".split("")
			end
		else
			success = false
			errorMessages.string = "There must be at least one digit after the exponent symbol.".split("")
		end
	else
		success = false
		errorMessages.string = "Expected exponent symbol.".split("")
	end

	return success
end


def nGetNumberFromNumberCharacterForBase(c, base)

	numberTable = nGetDigitCharacterTable()
	position = 0.0

	i = 0.0
	while(i < base)
		if numberTable[i] == c
			position = i
		end
		i = i + 1.0
	end

	return position
end


def nCharacterIsNumberCharacterInBase(c, base)

	numberTable = nGetDigitCharacterTable()
	found = false

	i = 0.0
	while(i < base)
		if numberTable[i] == c
			found = true
		end
		i = i + 1.0
	end

	return found
end


def nStringToNumberArray(str)

	numberArrayReference = NumberArrayReference.new
	stringReference = StringReference.new

	nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference)

	numbers = numberArrayReference.numberArray

	delete(numberArrayReference)
	delete(stringReference)

	return numbers
end


def nStringToNumberArrayWithCheck(str, numberArrayReference, errorMessage)

	numberStrings = strSplitByString(str, ",".split(""))

	numbers = Array.new(numberStrings.length)
	success = true
	numberReference = NumberReference.new

	i = 0.0
	while(i < numberStrings.length)
		numberString = numberStrings[i].string
		trimmedNumberString = strTrim(numberString)
		success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage)
		numbers[i] = numberReference.numberValue

		FreeStringReference(numberStrings[i])
		delete(trimmedNumberString)
		i = i + 1.0
	end

	delete(numberStrings)
	delete(numberReference)

	numberArrayReference.numberArray = numbers

	return success
end


def lAddNumber(list, a)

	newlist = Array.new(list.length + 1.0)
	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def lAddNumberRef(list, i)
	list.numberArray = lAddNumber(list.numberArray, i)
end


def lRemoveNumber(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end
		
	return newlist
end


def lGetNumberRef(list, i)
	return list.numberArray[i]
end


def lRemoveNumberRef(list, i)
	list.numberArray = lRemoveNumber(list.numberArray, i)
end


def lAddString(list, a)

	newlist = Array.new(list.length + 1.0)

	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def lAddStringRef(list, i)
	list.stringArray = lAddString(list.stringArray, i)
end


def lRemoveString(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end
		
	return newlist
end


def lGetStringRef(list, i)
	return list.stringArray[i]
end


def lRemoveStringRef(list, i)
	list.stringArray = lRemoveString(list.stringArray, i)
end


def lAddBoolean(list, a)

	newlist = Array.new(list.length + 1.0)
	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def lAddBooleanRef(list, i)
	list.booleanArray = lAddBoolean(list.booleanArray, i)
end


def lRemoveBoolean(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end
		
	return newlist
end


def lGetBooleanRef(list, i)
	return list.booleanArray[i]
end


def lRemoveDecimalRef(list, i)
	list.booleanArray = lRemoveBoolean(list.booleanArray, i)
end


def lCreateLinkedListString()

	ll = LLinkedListStrings.new
	ll.first = LLinkedListNodeStrings.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def lLinkedListAddString(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LLinkedListNodeStrings.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def lLinkedListStringsToArray(ll)

	node = ll.first

	length = lLinkedListStringsLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = StringReference.new
		array[i].string = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def lLinkedListStringsLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def lFreeLinkedListString(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def lCreateLinkedListNumbers()

	ll = LLinkedListNumbers.new
	ll.first = LLinkedListNodeNumbers.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def lCreateLinkedListNumbersArray(length)

	lls = Array.new(length)
	i = 0.0
	while(i < lls.length)
		lls[i] = lCreateLinkedListNumbers()
		i = i + 1.0
	end

	return lls
end


def lLinkedListAddNumber(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LLinkedListNodeNumbers.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def lLinkedListNumbersLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def lLinkedListNumbersIndex(ll, index)

	node = ll.first
	i = 0.0
	while(i < index)
		node = node.nextx
		i = i + 1.0
	end

	return node.value
end


def lLinkedListInsertNumber(ll, index, value)

	if index == 0.0
		tmp = ll.first
		ll.first = LLinkedListNodeNumbers.new
		ll.first.nextx = tmp
		ll.first.value = value
		ll.first.endx = false
	else
		node = ll.first
		i = 0.0
		while(i < index - 1.0)
			node = node.nextx
			i = i + 1.0
		end

		tmp = node.nextx
		node.nextx = LLinkedListNodeNumbers.new
		node.nextx.nextx = tmp
		node.nextx.value = value
		node.nextx.endx = false
	end
end


def lLinkedListSet(ll, index, value)

	node = ll.first
	i = 0.0
	while(i < index)
		node = node.nextx
		i = i + 1.0
	end

	node.nextx.value = value
end


def lLinkedListRemoveNumber(ll, index)

	node = ll.first
	prev = ll.first

	i = 0.0
	while(i < index)
		prev = node
		node = node.nextx
		i = i + 1.0
	end

	if index == 0.0
		ll.first = prev.nextx
	end
	if !prev.nextx.endx
		prev.nextx = prev.nextx.nextx
	end
end


def lFreeLinkedListNumbers(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def lFreeLinkedListNumbersArray(lls)

	i = 0.0
	while(i < lls.length)
		lFreeLinkedListNumbers(lls[i])
		i = i + 1.0
	end
	delete(lls)
end


def lLinkedListNumbersToArray(ll)

	node = ll.first

	length = lLinkedListNumbersLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def lArrayToLinkedListNumbers(array)

	ll = lCreateLinkedListNumbers()

	i = 0.0
	while(i < array.length)
		lLinkedListAddNumber(ll, array[i])
		i = i + 1.0
	end

	return ll
end


def lLinkedListNumbersEqual(a, b)

	an = a.first
	bn = b.first

	equal = true
	done = false
	while(equal && !done)
		if an.endx == bn.endx
			if an.endx
				done = true
			elsif an.value == bn.value
				an = an.nextx
				bn = bn.nextx
			else
				equal = false
			end
		else
			equal = false
		end
	end

	return equal
end


def lCreateLinkedListCharacter()

	ll = LLinkedListCharacters.new
	ll.first = LLinkedListNodeCharacters.new
	ll.last = ll.first
	ll.last.endx = true

	return ll
end


def lLinkedListAddCharacter(ll, value)
	ll.last.endx = false
	ll.last.value = value
	ll.last.nextx = LLinkedListNodeCharacters.new
	ll.last.nextx.endx = true
	ll.last = ll.last.nextx
end


def lLinkedListCharactersToArray(ll)

	node = ll.first

	length = lLinkedListCharactersLength(ll)

	array = Array.new(length)

	i = 0.0
	while(i < length)
		array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return array
end


def lLinkedListCharactersLength(ll)

	l = 0.0
	node = ll.first
	while(!node.endx)
		node = node.nextx
		l = l + 1.0
	end

	return l
end


def lFreeLinkedListCharacter(ll)

	node = ll.first

	while(!node.endx)
		prev = node
		node = node.nextx
		delete(prev)
	end

	delete(node)
end


def lCreateDynamicArrayNumbers()

	da = LDynamicArrayNumbers.new
	da.array = Array.new(10)
	da.length = 0.0

	return da
end


def lCreateDynamicArrayNumbersWithInitialCapacity(capacity)

	da = LDynamicArrayNumbers.new
	da.array = Array.new(capacity)
	da.length = 0.0

	return da
end


def lDynamicArrayAddNumber(da, value)
	if da.length == da.array.length
		lDynamicArrayNumbersIncreaseSize(da)
	end

	da.array[da.length] = value
	da.length = da.length + 1.0
end


def lDynamicArrayNumbersIncreaseSize(da)

	newLength = (da.array.length*3.0.to_f / 2.0).round
	newArray = Array.new(newLength)

	i = 0.0
	while(i < da.array.length)
		newArray[i] = da.array[i]
		i = i + 1.0
	end

	delete(da.array)

	da.array = newArray
end


def lDynamicArrayNumbersDecreaseSizeNecessary(da)

	needsDecrease = false

	if da.length > 10.0
		needsDecrease = da.length <= (da.array.length*2.0.to_f / 3.0).round
	end

	return needsDecrease
end


def lDynamicArrayNumbersDecreaseSize(da)

	newLength = (da.array.length*2.0.to_f / 3.0).round
	newArray = Array.new(newLength)

	i = 0.0
	while(i < da.array.length)
		newArray[i] = da.array[i]
		i = i + 1.0
	end

	delete(da.array)

	da.array = newArray
end


def lDynamicArrayNumbersIndex(da, index)
	return da.array[index]
end


def lDynamicArrayNumbersLength(da)
	return da.length
end


def lDynamicArrayInsertNumber(da, index, value)

	if da.length == da.array.length
		lDynamicArrayNumbersIncreaseSize(da)
	end

	i = da.length
	while(i > index)
		da.array[i] = da.array[i - 1.0]
		i = i - 1.0
	end

	da.array[index] = value

	da.length = da.length + 1.0
end


def lDynamicArraySet(da, index, value)
	da.array[index] = value
end


def lDynamicArrayRemoveNumber(da, index)

	i = index
	while(i < da.length - 1.0)
		da.array[i] = da.array[i + 1.0]
		i = i + 1.0
	end

	da.length = da.length - 1.0

	if lDynamicArrayNumbersDecreaseSizeNecessary(da)
		lDynamicArrayNumbersDecreaseSize(da)
	end
end


def lFreeDynamicArrayNumbers(da)
	delete(da.array)
	delete(da)
end


def lDynamicArrayNumbersToArray(da)

	array = Array.new(da.length)

	i = 0.0
	while(i < da.length)
		array[i] = da.array[i]
		i = i + 1.0
	end

	return array
end


def lArrayToDynamicArrayNumbersWithOptimalSize(array)

=begin

         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
        
=end

	c = array.length
	n = (Math.log(c) - 1.0).to_f / Math.log(3.0.to_f / 2.0)
	newCapacity = (n).floor + 1.0

	da = lCreateDynamicArrayNumbersWithInitialCapacity(newCapacity)

	i = 0.0
	while(i < array.length)
		da.array[i] = array[i]
		i = i + 1.0
	end

	return da
end


def lArrayToDynamicArrayNumbers(array)

	da = LDynamicArrayNumbers.new
	da.array = CopyNumberArray(array)
	da.length = array.length

	return da
end


def lDynamicArrayNumbersEqual(a, b)

	equal = true
	if a.length == b.length
		i = 0.0
		while(i < a.length && equal)
			if a.array[i] != b.array[i]
				equal = false
			end
			i = i + 1.0
		end
	else
		equal = false
	end

	return equal
end


def lDynamicArrayNumbersToLinkedList(da)

	ll = lCreateLinkedListNumbers()

	i = 0.0
	while(i < da.length)
		lLinkedListAddNumber(ll, da.array[i])
		i = i + 1.0
	end

	return ll
end


def lLinkedListToDynamicArrayNumbers(ll)

	node = ll.first

	da = LDynamicArrayNumbers.new
	da.length = lLinkedListNumbersLength(ll)

	da.array = Array.new(da.length)

	i = 0.0
	while(i < da.length)
		da.array[i] = node.value
		node = node.nextx
		i = i + 1.0
	end

	return da
end


def lAddCharacter(list, a)

	newlist = Array.new(list.length + 1.0)
	i = 0.0
	while(i < list.length)
		newlist[i] = list[i]
		i = i + 1.0
	end
	newlist[list.length] = a
		
	delete(list)
		
	return newlist
end


def lAddCharacterRef(list, i)
	list.string = lAddCharacter(list.string, i)
end


def lRemoveCharacter(list, n)

	newlist = Array.new(list.length - 1.0)

	if n >= 0.0 && n < list.length
		i = 0.0
		while(i < list.length)
			if i < n
				newlist[i] = list[i]
			end
			if i > n
				newlist[i - 1.0] = list[i]
			end
			i = i + 1.0
		end

		delete(list)
	else
		delete(newlist)
	end

	return newlist
end


def lGetCharacterRef(list, i)
	return list.string[i]
end


def lRemoveCharacterRef(list, i)
	list.string = lRemoveCharacter(list.string, i)
end


def Negate(x)
	return -x
end


def Positive(x)
	return +x
end


def Factorial(x)

	f = 1.0

	i = 2.0
	while(i <= x)
		f = f*i
		i = i + 1.0
	end

	return f
end


def Round(x)
	return (x + 0.5).floor
end


def BankersRound(x)

	if Absolute(x - Truncate(x)) == 0.5
		if !DivisibleBy(Round(x), 2.0)
			r = Round(x) - 1.0
		else
			r = Round(x)
		end
	else
		r = Round(x)
	end

	return r
end


def Ceil(x)
	return (x).ceil
end


def Floor(x)
	return (x).floor
end


def Truncate(x)

	if x >= 0.0
		t = (x).floor
	else
		t = (x).ceil
	end

	return t
end


def Absolute(x)
	return (x).abs
end


def Logarithm(x)
	return Math.log10(x)
end


def NaturalLogarithm(x)
	return Math.log(x)
end


def Sin(x)
	return Math.sin(x)
end


def Cos(x)
	return Math.cos(x)
end


def Tan(x)
	return Math.tan(x)
end


def Asin(x)
	return Math.asin(x)
end


def Acos(x)
	return Math.acos(x)
end


def Atan(x)
	return Math.atan(x)
end


def Atan2(y, x)

	# Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors.
	a = 0.0

	if x > 0.0
		a = Atan(y.to_f / x)
	elsif x < 0.0 && y >= 0.0
		a = Atan(y.to_f / x) + Math::PI
	elsif x < 0.0 && y < 0.0
		a = Atan(y.to_f / x) - Math::PI
	elsif x == 0.0 && y > 0.0
		a = Math::PI.to_f / 2.0
	elsif x == 0.0 && y < 0.0
		a = -Math::PI.to_f / 2.0
	end

	return a
end


def Squareroot(x)
	return Math.sqrt(x)
end


def Exp(x)
	return Math.exp(x)
end


def DivisibleBy(a, b)
	return ((a%b) == 0.0)
end


def Combinations(n, k)

	c = 1.0
	j = 1.0
	i = n - k + 1.0

	while(i <= n)
		c = c*i
		c = c.to_f / j

		i = i + 1.0
		j = j + 1.0
	end

	return c
end


def Permutations(n, k)

	c = 1.0

	i = n - k + 1.0
	while(i <= n)
		c = c*i
		i = i + 1.0
	end

	return c
end


def EpsilonCompare(a, b, epsilon)
	return (a - b).abs < epsilon
end


def GreatestCommonDivisor(a, b)

	while(b != 0.0)
		t = b
		b = a%b
		a = t
	end

	return a
end


def GCDWithSubtraction(a, b)

	if a == 0.0
		g = b
	else
		while(b != 0.0)
			if a > b
				a = a - b
			else
				b = b - a
			end
		end

		g = a
	end

	return g
end


def IsInteger(a)
	return (a - (a).floor) == 0.0
end


def GreatestCommonDivisorWithCheck(a, b, gcdReference)

	if IsInteger(a) && IsInteger(b)
		gcd = GreatestCommonDivisor(a, b)
		gcdReference.numberValue = gcd
		success = true
	else
		success = false
	end

	return success
end


def LeastCommonMultiple(a, b)

	if a > 0.0 && b > 0.0
		lcm = (a*b).abs.to_f / GreatestCommonDivisor(a, b)
	else
		lcm = 0.0
	end

	return lcm
end


def Sign(a)

	if a > 0.0
		s = 1.0
	elsif a < 0.0
		s = -1.0
	else
		s = 0.0
	end

	return s
end


def Max(a, b)
	return [a, b].max
end


def Min(a, b)
	return [a, b].min
end


def Power(a, b)
	return a**b
end


def Gamma(x)
	return LanczosApproximation(x)
end


def LogGamma(x)
	return Math.log(Gamma(x))
end


def LanczosApproximation(z)

	p = Array.new(8)
	p[0] = 676.5203681218851
	p[1] = -1259.1392167224028
	p[2] = 771.32342877765313
	p[3] = -176.61502916214059
	p[4] = 12.507343278686905
	p[5] = -0.13857109526572012
	p[6] = 9.9843695780195716e-6
	p[7] = 1.5056327351493116e-7

	if z < 0.5
		y = Math::PI.to_f / (Math.sin(Math::PI*z)*LanczosApproximation(1.0 - z))
	else
		z = z - 1.0
		x = 0.99999999999980993
		i = 0.0
		while(i < p.length)
			x = x + p[i].to_f / (z + i + 1.0)
			i = i + 1.0
		end
		t = z + p.length - 0.5
		y = Math.sqrt(2.0*Math::PI)*t**(z + 0.5)*Math.exp(-t)*x
	end

	return y
end


def Beta(x, y)
	return Gamma(x)*Gamma(y).to_f / Gamma(x + y)
end


def Sinh(x)
	return (Math.exp(x) - Math.exp(-x)).to_f / 2.0
end


def Cosh(x)
	return (Math.exp(x) + Math.exp(-x)).to_f / 2.0
end


def Tanh(x)
	return Sinh(x).to_f / Cosh(x)
end


def Cot(x)
	return 1.0.to_f / Math.tan(x)
end


def Sec(x)
	return 1.0.to_f / Math.cos(x)
end


def Csc(x)
	return 1.0.to_f / Math.sin(x)
end


def Coth(x)
	return Cosh(x).to_f / Sinh(x)
end


def Sech(x)
	return 1.0.to_f / Cosh(x)
end


def Csch(x)
	return 1.0.to_f / Sinh(x)
end


def Error(x)

	if x == 0.0
		y = 0.0
	elsif x < 0.0
		y = -Error(-x)
	else
		c1 = -1.26551223
		c2 = +1.00002368
		c3 = +0.37409196
		c4 = +0.09678418
		c5 = -0.18628806
		c6 = +0.27886807
		c7 = -1.13520398
		c8 = +1.48851587
		c9 = -0.82215223
		c10 = +0.17087277

		t = 1.0.to_f / (1.0 + 0.5*(x).abs)

		tau = t*Math.exp(-x**2.0 + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))))

		y = 1.0 - tau
	end

	return y
end


def ErrorInverse(x)

	a = (8.0*(Math::PI - 3.0)).to_f / (3.0*Math::PI*(4.0 - Math::PI))

	t = 2.0.to_f / (Math::PI*a) + Math.log(1.0 - x**2.0).to_f / 2.0
	y = Sign(x)*Math.sqrt(Math.sqrt(t**2.0 - Math.log(1.0 - x**2.0).to_f / a) - t)

	return y
end


def FallingFactorial(x, n)

	y = 1.0

	k = 0.0
	while(k <= n - 1.0)
		y = y*(x - k)
		k = k + 1.0
	end

	return y
end


def RisingFactorial(x, n)

	y = 1.0

	k = 0.0
	while(k <= n - 1.0)
		y = y*(x + k)
		k = k + 1.0
	end

	return y
end


def Hypergeometric(a, b, c, z, maxIterations, precision)

	if (z).abs >= 0.5
		y = (1.0 - z)**(-a)*HypergeometricDirect(a, c - b, c, z.to_f / (z - 1.0), maxIterations, precision)
	else
		y = HypergeometricDirect(a, b, c, z, maxIterations, precision)
	end

	return y
end


def HypergeometricDirect(a, b, c, z, maxIterations, precision)

	y = 0.0
	done = false

	n = 0.0
	while(n < maxIterations && !done)
		yp = RisingFactorial(a, n)*RisingFactorial(b, n).to_f / RisingFactorial(c, n)*z**n.to_f / Factorial(n)
		if (yp).abs < precision
			done = true
		end
		y = y + yp
		n = n + 1.0
	end

	return y
end


def BernouilliNumber(n)
	return AkiyamaTanigawaAlgorithm(n)
end


def AkiyamaTanigawaAlgorithm(n)

	a = Array.new(n + 1.0)

	m = 0.0
	while(m <= n)
		a[m] = 1.0.to_f / (m + 1.0)
		j = m
		while(j >= 1.0)
			a[j - 1.0] = j*(a[j - 1.0] - a[j])
			j = j - 1.0
		end
		m = m + 1.0
	end

	b = a[0]

	delete(a)

	return b
end


def charToLowerCase(character)

	toReturn = character
	if character == "A"
		toReturn = "a"
	elsif character == "B"
		toReturn = "b"
	elsif character == "C"
		toReturn = "c"
	elsif character == "D"
		toReturn = "d"
	elsif character == "E"
		toReturn = "e"
	elsif character == "F"
		toReturn = "f"
	elsif character == "G"
		toReturn = "g"
	elsif character == "H"
		toReturn = "h"
	elsif character == "I"
		toReturn = "i"
	elsif character == "J"
		toReturn = "j"
	elsif character == "K"
		toReturn = "k"
	elsif character == "L"
		toReturn = "l"
	elsif character == "M"
		toReturn = "m"
	elsif character == "N"
		toReturn = "n"
	elsif character == "O"
		toReturn = "o"
	elsif character == "P"
		toReturn = "p"
	elsif character == "Q"
		toReturn = "q"
	elsif character == "R"
		toReturn = "r"
	elsif character == "S"
		toReturn = "s"
	elsif character == "T"
		toReturn = "t"
	elsif character == "U"
		toReturn = "u"
	elsif character == "V"
		toReturn = "v"
	elsif character == "W"
		toReturn = "w"
	elsif character == "X"
		toReturn = "x"
	elsif character == "Y"
		toReturn = "y"
	elsif character == "Z"
		toReturn = "z"
	end

	return toReturn
end


def charToUpperCase(character)

	toReturn = character
	if character == "a"
		toReturn = "A"
	elsif character == "b"
		toReturn = "B"
	elsif character == "c"
		toReturn = "C"
	elsif character == "d"
		toReturn = "D"
	elsif character == "e"
		toReturn = "E"
	elsif character == "f"
		toReturn = "F"
	elsif character == "g"
		toReturn = "G"
	elsif character == "h"
		toReturn = "H"
	elsif character == "i"
		toReturn = "I"
	elsif character == "j"
		toReturn = "J"
	elsif character == "k"
		toReturn = "K"
	elsif character == "l"
		toReturn = "L"
	elsif character == "m"
		toReturn = "M"
	elsif character == "n"
		toReturn = "N"
	elsif character == "o"
		toReturn = "O"
	elsif character == "p"
		toReturn = "P"
	elsif character == "q"
		toReturn = "Q"
	elsif character == "r"
		toReturn = "R"
	elsif character == "s"
		toReturn = "S"
	elsif character == "t"
		toReturn = "T"
	elsif character == "u"
		toReturn = "U"
	elsif character == "v"
		toReturn = "V"
	elsif character == "w"
		toReturn = "W"
	elsif character == "x"
		toReturn = "X"
	elsif character == "y"
		toReturn = "Y"
	elsif character == "z"
		toReturn = "Z"
	end

	return toReturn
end


def charIsUpperCase(character)

	isUpper = false
	if character == "A"
		isUpper = true
	elsif character == "B"
		isUpper = true
	elsif character == "C"
		isUpper = true
	elsif character == "D"
		isUpper = true
	elsif character == "E"
		isUpper = true
	elsif character == "F"
		isUpper = true
	elsif character == "G"
		isUpper = true
	elsif character == "H"
		isUpper = true
	elsif character == "I"
		isUpper = true
	elsif character == "J"
		isUpper = true
	elsif character == "K"
		isUpper = true
	elsif character == "L"
		isUpper = true
	elsif character == "M"
		isUpper = true
	elsif character == "N"
		isUpper = true
	elsif character == "O"
		isUpper = true
	elsif character == "P"
		isUpper = true
	elsif character == "Q"
		isUpper = true
	elsif character == "R"
		isUpper = true
	elsif character == "S"
		isUpper = true
	elsif character == "T"
		isUpper = true
	elsif character == "U"
		isUpper = true
	elsif character == "V"
		isUpper = true
	elsif character == "W"
		isUpper = true
	elsif character == "X"
		isUpper = true
	elsif character == "Y"
		isUpper = true
	elsif character == "Z"
		isUpper = true
	end

	return isUpper
end


def charIsLowerCase(character)

	isLower = false
	if character == "a"
		isLower = true
	elsif character == "b"
		isLower = true
	elsif character == "c"
		isLower = true
	elsif character == "d"
		isLower = true
	elsif character == "e"
		isLower = true
	elsif character == "f"
		isLower = true
	elsif character == "g"
		isLower = true
	elsif character == "h"
		isLower = true
	elsif character == "i"
		isLower = true
	elsif character == "j"
		isLower = true
	elsif character == "k"
		isLower = true
	elsif character == "l"
		isLower = true
	elsif character == "m"
		isLower = true
	elsif character == "n"
		isLower = true
	elsif character == "o"
		isLower = true
	elsif character == "p"
		isLower = true
	elsif character == "q"
		isLower = true
	elsif character == "r"
		isLower = true
	elsif character == "s"
		isLower = true
	elsif character == "t"
		isLower = true
	elsif character == "u"
		isLower = true
	elsif character == "v"
		isLower = true
	elsif character == "w"
		isLower = true
	elsif character == "x"
		isLower = true
	elsif character == "y"
		isLower = true
	elsif character == "z"
		isLower = true
	end

	return isLower
end


def charIsLetter(character)
	return charIsUpperCase(character) || charIsLowerCase(character)
end


def charIsNumber(character)

	isNumberx = false
	if character == "0"
		isNumberx = true
	elsif character == "1"
		isNumberx = true
	elsif character == "2"
		isNumberx = true
	elsif character == "3"
		isNumberx = true
	elsif character == "4"
		isNumberx = true
	elsif character == "5"
		isNumberx = true
	elsif character == "6"
		isNumberx = true
	elsif character == "7"
		isNumberx = true
	elsif character == "8"
		isNumberx = true
	elsif character == "9"
		isNumberx = true
	end

	return isNumberx
end


def charIsWhiteSpace(character)

	isWhiteSpacex = false
	if character == " "
		isWhiteSpacex = true
	elsif character == "\t"
		isWhiteSpacex = true
	elsif character == "\n"
		isWhiteSpacex = true
	elsif character == "\r"
		isWhiteSpacex = true
	end

	return isWhiteSpacex
end


def charIsSymbol(character)

	isSymbolx = false
	if character == "!"
		isSymbolx = true
	elsif character == "\""
		isSymbolx = true
	elsif character == "#"
		isSymbolx = true
	elsif character == "$"
		isSymbolx = true
	elsif character == "%"
		isSymbolx = true
	elsif character == "&"
		isSymbolx = true
	elsif character == "\'"
		isSymbolx = true
	elsif character == "("
		isSymbolx = true
	elsif character == ")"
		isSymbolx = true
	elsif character == "*"
		isSymbolx = true
	elsif character == "+"
		isSymbolx = true
	elsif character == ","
		isSymbolx = true
	elsif character == "-"
		isSymbolx = true
	elsif character == "."
		isSymbolx = true
	elsif character == "/"
		isSymbolx = true
	elsif character == ":"
		isSymbolx = true
	elsif character == ";"
		isSymbolx = true
	elsif character == "<"
		isSymbolx = true
	elsif character == "="
		isSymbolx = true
	elsif character == ">"
		isSymbolx = true
	elsif character == "?"
		isSymbolx = true
	elsif character == "@"
		isSymbolx = true
	elsif character == "["
		isSymbolx = true
	elsif character == "\\"
		isSymbolx = true
	elsif character == "]"
		isSymbolx = true
	elsif character == "^"
		isSymbolx = true
	elsif character == "_"
		isSymbolx = true
	elsif character == "`"
		isSymbolx = true
	elsif character == "{"
		isSymbolx = true
	elsif character == "|"
		isSymbolx = true
	elsif character == "}"
		isSymbolx = true
	elsif character == "~"
		isSymbolx = true
	end

	return isSymbolx
end


def charCharacterIsBefore(a, b)

	ad = (a).ord
	bd = (b).ord

	return ad < bd
end


def delete(x)
	# Ruby has garbage collection.
end

