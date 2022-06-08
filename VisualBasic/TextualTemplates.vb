' Downloaded from https://repo.progsbase.com - Code Developed Using progsbase.

Imports System.Math

Public Class LinkedListNodeNodes
	Public endx As Boolean
	Public value As Node
	Public nextx As LinkedListNodeNodes
End Class

Public Class LinkedListNodes
	Public first As LinkedListNodeNodes
	Public last As LinkedListNodeNodes
End Class

Public Class Node
	Public type As Char ()
	Public p1 As Char ()
	Public p2 As Char ()
	Public block1 As Node
	Public hasElseBlock As Boolean
	Public block2 As Node
	Public nodes As LinkedListNodes
End Class

Public Class BooleanArrayReference
	Public booleanArray As Boolean ()
End Class

Public Class BooleanReference
	Public booleanValue As Boolean
End Class

Public Class CharacterReference
	Public characterValue As Char
End Class

Public Class NumberArrayReference
	Public numberArray As Double ()
End Class

Public Class NumberReference
	Public numberValue As Double
End Class

Public Class StringArrayReference
	Public stringArray As StringReference ()
End Class

Public Class StringReference
	Public stringx As Char ()
End Class

Public Class ElementArrayReference
	Public arrayx As Element ()
End Class

Public Class LinkedListElements
	Public first As LinkedListNodeElements
	Public last As LinkedListNodeElements
End Class

Public Class LinkedListNodeElements
	Public endx As Boolean
	Public value As Element
	Public nextx As LinkedListNodeElements
End Class

Public Class Element
	Public type As Char ()
	Public objectx As StringElementMap
	Public arrayx As Element ()
	Public stringx As Char ()
	Public number As Double
	Public booleanValue As Boolean
End Class

Public Class ElementReference
	Public element As Element
End Class

Public Class ElementType
	Public name As Char ()
End Class

Public Class StringElementMap
	Public stringListRef As StringArrayReference
	Public elementListRef As ElementArrayReference
End Class

Public Class DynamicArrayCharacters
	Public arrayx As Char ()
	Public length As Double
End Class

Public Class LinkedListNodeStrings
	Public endx As Boolean
	Public value As Char ()
	Public nextx As LinkedListNodeStrings
End Class

Public Class LinkedListStrings
	Public first As LinkedListNodeStrings
	Public last As LinkedListNodeStrings
End Class

Public Class LinkedListNodeNumbers
	Public nextx As LinkedListNodeNumbers
	Public endx As Boolean
	Public value As Double
End Class

Public Class LinkedListNumbers
	Public first As LinkedListNodeNumbers
	Public last As LinkedListNodeNumbers
End Class

Public Class LinkedListCharacters
	Public first As LinkedListNodeCharacters
	Public last As LinkedListNodeCharacters
End Class

Public Class LinkedListNodeCharacters
	Public endx As Boolean
	Public value As Char
	Public nextx As LinkedListNodeCharacters
End Class

Public Class DynamicArrayNumbers
	Public arrayx As Double ()
	Public length As Double
End Class

Module TextualTemplates
	Public Function CreateLinkedListNodes() As LinkedListNodes
		Dim ll As LinkedListNodes

		ll = New LinkedListNodes()
		ll.first = New LinkedListNodeNodes()
		ll.last = ll.first
		ll.last.endx = true

		Return ll
	End Function


	Public Sub LinkedListAddNode(ByRef ll As LinkedListNodes, ByRef value As Node)
		ll.last.endx = false
		ll.last.value = value
		ll.last.nextx = New LinkedListNodeNodes()
		ll.last.nextx.endx = true
		ll.last = ll.last.nextx
	End Sub


	Public Function LinkedListNodesToArray(ByRef ll As LinkedListNodes) As Node ()
		Dim arrayx As Node ()
		Dim length, i As Double
		Dim node As LinkedListNodeNodes

		node = ll.first

		length = LinkedListNodesLength(ll)

		arrayx = New Node (length - 1){}

		i = 0
		While i < length
			arrayx(i) = node.value
			node = node.nextx
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function LinkedListNodesLength(ByRef ll As LinkedListNodes) As Double
		Dim l As Double
		Dim node As LinkedListNodeNodes

		l = 0
		node = ll.first
		
		While Not node.endx
			node = node.nextx
			l = l + 1
		End While

		Return l
	End Function


	Public Sub FreeLinkedListNode(ByRef ll As LinkedListNodes)
		Dim node, prev As LinkedListNodeNodes

		node = ll.first

		
		While Not node.endx
			prev = node
			node = node.nextx
			prev = Nothing
		End While

		node = Nothing
	End Sub


	Public Function IsValidTemplate(ByRef template As Char ()) As Boolean
		Dim errorMessage As StringReference
		Dim success As Boolean
		Dim tokens As LinkedListStrings
		Dim ts As StringReference ()
		Dim root As Node

		tokens = CreateLinkedListString()
		errorMessage = New StringReference()

		success = GenerateTokensFromTemplate(template, tokens, errorMessage)

		If success
			root = New Node()

			ts = LinkedListStringsToArray(tokens)

			success = ParseTemplate(ts, root, errorMessage)
		End If

		If Not success
			Call FreeStringReference(errorMessage)
		End If

		Return success
	End Function


	Public Function GenerateTokensFromTemplate(ByRef template As Char (), ByRef tokens As LinkedListStrings, ByRef errorMessage As StringReference) As Boolean
		Dim success, found As Boolean
		Dim i, j, l As Double
		Dim pc, c As Char
		Dim cs, a As Char ()
		Dim da As DynamicArrayCharacters

		success = true
		cs = New Char (4 - 1){}

		da = CreateDynamicArrayCharacters()

		pc = "x"C
		i = 0
		While i < template.Length And success
			c = template(i)

			If c <> "{"C
				Call DynamicArrayAddCharacter(da, c)
				i = i + 1
			Else
				Call FillString(cs, "x"C)
				j = 0
				While i + j < template.Length And j < cs.Length
					cs(j) = template(i + j)
					j = j + 1
				End While

				If StringsEqual(cs, "{use".ToCharArray()) Or StringsEqual(cs, "{end".ToCharArray()) Or StringsEqual(cs, "{pri".ToCharArray()) Or StringsEqual(cs, "{for".ToCharArray()) Or StringsEqual(cs, "{if ".ToCharArray()) Or StringsEqual(cs, "{els".ToCharArray())
					If pc <> "\"C
						' Find end.
						found = false
						l = 0
						While i + l < template.Length And Not found
							If template(i + l) = "}"C
								found = true
							End If
							l = l + 1
						End While

						If found
							If da.length > 0
								a = DynamicArrayCharactersToArray(da)
								Call LinkedListAddString(tokens, a)
								Call FreeDynamicArrayCharacters(da)
								da = CreateDynamicArrayCharacters()
							End If

							j = 0
							While j < l
								Call DynamicArrayAddCharacter(da, template(i + j))
								j = j + 1
							End While

							a = DynamicArrayCharactersToArray(da)
							Call LinkedListAddString(tokens, a)
							Call FreeDynamicArrayCharacters(da)
							da = CreateDynamicArrayCharacters()

							i = i + l
						Else
							success = false
							errorMessage.stringx = "Template command found, but not ended properly.".ToCharArray()
						End If
					Else
						Call DynamicArrayAddCharacter(da, c)
						i = i + 1
					End If
				Else
					Call DynamicArrayAddCharacter(da, c)
					i = i + 1
				End If
			End If

			pc = c
		End While

		If da.length > 0
			a = DynamicArrayCharactersToArray(da)
			Call LinkedListAddString(tokens, a)
		End If

		Call FreeDynamicArrayCharacters(da)

		Return success
	End Function


	Public Function GenerateDocument(ByRef template As Char (), ByRef json As Char (), ByRef document As StringReference, ByRef errorMessage As StringReference) As Boolean
		Dim data As ElementReference
		Dim errorMessages As StringArrayReference
		Dim success As Boolean

		data = New ElementReference()
		errorMessages = New StringArrayReference()

		success = ReadJSON(json, data, errorMessages)

		If success
			success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage)
		Else
			errorMessage.stringx = JoinStringsWithSeparator(errorMessages.stringArray, ", ".ToCharArray())
			Call FreeStringArrayReference(errorMessages)
		End If

		Return success
	End Function


	Public Function GenerateDocumentBasedOnElement(ByRef template As Char (), ByRef data As Element, ByRef document As StringReference, ByRef errorMessage As StringReference) As Boolean
		Dim ll As LinkedListCharacters
		Dim success As Boolean
		Dim tokens As LinkedListStrings
		Dim ts As StringReference ()
		Dim root As Node

		tokens = CreateLinkedListString()

		success = GenerateTokensFromTemplate(template, tokens, errorMessage)

		If success
			root = New Node()

			ts = LinkedListStringsToArray(tokens)
			Call FreeLinkedListString(tokens)

			success = ParseTemplate(ts, root, errorMessage)

			If success
				ll = CreateLinkedListCharacter()

				success = GenerateDocumentFromBlock(root, data, ll, errorMessage)

				If success
					document.stringx = LinkedListCharactersToArray(ll)
					Call FreeLinkedListCharacter(ll)
				End If
			End If
		End If

		Return success
	End Function


	Public Function GenerateDocumentFromBlock(ByRef root As Node, ByRef data As Element, ByRef ll As LinkedListCharacters, ByRef errorMessage As StringReference) As Boolean
		Dim n As LinkedListNodeNodes
		Dim success As Boolean

		n = root.nodes.first
		success = true

		
		While Not n.endx And success
			success = GenerateDocumentFromNode(n.value, data, ll, errorMessage)
			n = n.nextx
		End While

		Return success
	End Function


	Public Function GenerateDocumentFromNode(ByRef n As Node, ByRef data As Element, ByRef ll As LinkedListCharacters, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim found As BooleanReference
		Dim valueString As Char ()
		Dim value As Element

		success = true
		found = New BooleanReference()

		If StringsEqual(n.type, "block".ToCharArray())
			success = GenerateDocumentFromBlock(n, data, ll, errorMessage)
		ElseIf StringsEqual(n.type, "use".ToCharArray())
			success = GenerateDocumentFromUse(n, data, ll, errorMessage)
		ElseIf StringsEqual(n.type, "if".ToCharArray())
			success = GenerateDocumentFromIf(n, data, ll, errorMessage)
		ElseIf StringsEqual(n.type, "foreach".ToCharArray())
			success = GenerateDocumentFromForeach(n, data, ll, errorMessage)
		ElseIf StringsEqual(n.type, "text".ToCharArray())
			success = true
			Call LinkedListCharactersAddString(ll, n.p1)
		ElseIf StringsEqual(n.type, "print".ToCharArray())
			If StringsEqual(data.type, "object".ToCharArray())
				value = GetObjectValueWithCheck(data.objectx, n.p1, found)

				If found.booleanValue
					If StringsEqual(value.type, "string".ToCharArray())
						valueString = value.stringx
					Else
						valueString = WriteJSON(value)
					End If
					Call LinkedListCharactersAddString(ll, valueString)
				Else
					success = false
					errorMessage.stringx = "Key for printing not found in JSON object: ".ToCharArray()
					errorMessage.stringx = ConcatenateString(errorMessage.stringx, n.p1)
				End If
			Else
				success = false
				errorMessage.stringx = "Data structure for print command is not a JSON object.".ToCharArray()
			End If
		End If

		Return success
	End Function


	Public Function GenerateDocumentFromUse(ByRef n As Node, ByRef data As Element, ByRef ll As LinkedListCharacters, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim found As BooleanReference
		Dim value As Element

		found = New BooleanReference()

		If StringsEqual(data.type, "object".ToCharArray())
			value = GetObjectValueWithCheck(data.objectx, n.p1, found)

			If found.booleanValue
				success = GenerateDocumentFromNode(n.block1, value, ll, errorMessage)
			Else
				success = false
				errorMessage.stringx = "Key for use not found in JSON object.".ToCharArray()
			End If
		Else
			success = false
			errorMessage.stringx = "Data structure for use command is not a JSON object.".ToCharArray()
		End If

		Return success
	End Function


	Public Function GenerateDocumentFromIf(ByRef n As Node, ByRef data As Element, ByRef ll As LinkedListCharacters, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim found As BooleanReference
		Dim value As Element

		success = true
		found = New BooleanReference()

		If StringsEqual(data.type, "object".ToCharArray())
			value = GetObjectValueWithCheck(data.objectx, n.p1, found)

			If found.booleanValue
				If StringsEqual(value.type, "boolean".ToCharArray())
					If value.booleanValue
						success = GenerateDocumentFromBlock(n.block1, data, ll, errorMessage)
					End If

					If n.hasElseBlock
						If Not value.booleanValue
							success = GenerateDocumentFromBlock(n.block2, data, ll, errorMessage)
						End If
					End If
				Else
					success = false
					errorMessage.stringx = "Value for if not boolean.".ToCharArray()
				End If
			Else
				success = false
				errorMessage.stringx = "Key for if not found in JSON object: ".ToCharArray()
				errorMessage.stringx = ConcatenateString(errorMessage.stringx, n.p1)
			End If
		Else
			success = false
			errorMessage.stringx = "Data structure for if command is not a JSON object.".ToCharArray()
		End If

		Return success
	End Function


	Public Function GenerateDocumentFromForeach(ByRef n As Node, ByRef data As Element, ByRef ll As LinkedListCharacters, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim found As BooleanReference
		Dim value, loopVar, arrayElement As Element
		Dim i As Double

		success = true
		found = New BooleanReference()
		loopVar = CreateObjectElement(0)

		Call PutStringElementMap(loopVar.objectx, n.p1, New Element())

		If StringsEqual(data.type, "object".ToCharArray())
			value = GetObjectValueWithCheck(data.objectx, n.p2, found)

			If found.booleanValue
				If StringsEqual(value.type, "array".ToCharArray())

					i = 0
					While i < value.arrayx.Length
						arrayElement = value.arrayx(i)
						If StringsEqual(arrayElement.type, "object".ToCharArray())
							success = GenerateDocumentFromBlock(n.block1, arrayElement, ll, errorMessage)
						Else
							Call SetStringElementMap(loopVar.objectx, 0, n.p1, arrayElement)
							success = GenerateDocumentFromBlock(n.block1, loopVar, ll, errorMessage)
						End If
						i = i + 1
					End While
				Else
					success = false
					errorMessage.stringx = "Value for foreach is not an array.".ToCharArray()
				End If
			Else
				success = false
				errorMessage.stringx = "Key for foreach not found in JSON object: ".ToCharArray()
				errorMessage.stringx = ConcatenateString(errorMessage.stringx, n.p2)
			End If
		Else
			success = false
			errorMessage.stringx = "Data structure for foreach command is not a JSON object.".ToCharArray()
		End If

		Return success
	End Function


	Public Function ParseTemplate(ByRef tokens As StringReference (), ByRef node As Node, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim position As NumberReference

		position = CreateNumberReference(0)
		success = ParseTemplateBlock(tokens, position, node, errorMessage)

		If success
			If position.numberValue <> tokens.Length
				success = false
				errorMessage.stringx = "Unexpected token at the end of template.".ToCharArray()
			End If
		End If

		position = Nothing

		Return success
	End Function


	Public Function ParseTemplateBlock(ByRef tokens As StringReference (), ByRef position As NumberReference, ByRef node As Node, ByRef errorMessage As StringReference) As Boolean
		Dim success, done As Boolean
		Dim tn, nb As Node

		success = true
		done = false

		node.type = "block".ToCharArray()
		node.nodes = CreateLinkedListNodes()

		
		While position.numberValue < tokens.Length And success And Not done
			tn = New Node()
			success = ParseNodeString(tokens(position.numberValue).stringx, tn, errorMessage)

			If success
				If StringsEqual(tn.type, "text".ToCharArray()) Or StringsEqual(tn.type, "print".ToCharArray())
					Call LinkedListAddNode(node.nodes, tn)
					position.numberValue = position.numberValue + 1
				ElseIf StringsEqual(tn.type, "use".ToCharArray())
					nb = New Node()
					success = ParseUseBlock(tokens, position, nb, errorMessage)
					Call LinkedListAddNode(node.nodes, nb)
				ElseIf StringsEqual(tn.type, "if".ToCharArray())
					nb = New Node()
					success = ParseIfBlock(tokens, position, nb, errorMessage)
					Call LinkedListAddNode(node.nodes, nb)
				ElseIf StringsEqual(tn.type, "foreach".ToCharArray())
					nb = New Node()
					success = ParseForeachBlock(tokens, position, nb, errorMessage)
					Call LinkedListAddNode(node.nodes, nb)
				Else
					done = true
				End If
			End If
		End While

		Return success
	End Function


	Public Function ParseUseBlock(ByRef tokens As StringReference (), ByRef position As NumberReference, ByRef useBlock As Node, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim n As Node

		n = New Node()
		ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

		useBlock.type = CopyString(n.type)
		useBlock.p1 = CopyString(n.p1)
		useBlock.block1 = New Node()

		position.numberValue = position.numberValue + 1

		success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage)

		If success
			If position.numberValue < tokens.Length
				n = New Node()
				ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

				If StringsEqual(n.type, "end".ToCharArray())
					success = true
					position.numberValue = position.numberValue + 1
				Else
					success = false
					errorMessage.stringx = "End block expected at the end of use block.".ToCharArray()
				End If
			Else
				success = false
				errorMessage.stringx = "End block expected at the end of use block.".ToCharArray()
			End If
		End If

		Return success
	End Function


	Public Function ParseIfBlock(ByRef tokens As StringReference (), ByRef position As NumberReference, ByRef ifBlock As Node, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim n As Node

		n = New Node()
		ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

		ifBlock.type = CopyString(n.type)
		ifBlock.p1 = CopyString(n.p1)
		ifBlock.block1 = New Node()
		ifBlock.hasElseBlock = false

		position.numberValue = position.numberValue + 1

		success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage)

		If success
			If position.numberValue < tokens.Length
				n = New Node()
				ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

				If StringsEqual(n.type, "end".ToCharArray())
					success = true
					position.numberValue = position.numberValue + 1
				ElseIf StringsEqual(n.type, "else".ToCharArray())
					position.numberValue = position.numberValue + 1
					ifBlock.hasElseBlock = true
					ifBlock.block2 = New Node()
					success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage)

					If success
						If position.numberValue < tokens.Length
							n = New Node()
							ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

							If StringsEqual(n.type, "end".ToCharArray())
								success = true
								position.numberValue = position.numberValue + 1
							Else
								success = false
								errorMessage.stringx = "End block expected at the end of else block.".ToCharArray()
							End If
						Else
							success = false
							errorMessage.stringx = "End block expected at the end of else block.".ToCharArray()
						End If
					End If
				Else
					success = false
					errorMessage.stringx = "End or else block expected at the end of if block.".ToCharArray()
				End If
			Else
				success = false
				errorMessage.stringx = "End or else block expected at the end of if block.".ToCharArray()
			End If
		End If

		Return success
	End Function


	Public Function ParseForeachBlock(ByRef tokens As StringReference (), ByRef position As NumberReference, ByRef foreachBlock As Node, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim n As Node

		n = New Node()
		ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

		foreachBlock.type = CopyString(n.type)
		foreachBlock.p1 = CopyString(n.p1)
		foreachBlock.p2 = CopyString(n.p2)
		foreachBlock.block1 = New Node()

		position.numberValue = position.numberValue + 1

		success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage)

		If success
			If position.numberValue < tokens.Length
				n = New Node()
				ParseNodeString(tokens(position.numberValue).stringx, n, errorMessage)

				If StringsEqual(n.type, "end".ToCharArray())
					success = true
					position.numberValue = position.numberValue + 1
				Else
					success = false
					errorMessage.stringx = "End block expected at the end of for each block.".ToCharArray()
				End If
			Else
				success = false
				errorMessage.stringx = "End block expected at the end of foreach block.".ToCharArray()
			End If
		End If

		Return success
	End Function


	Public Function ParseNodeString(ByRef token As Char (), ByRef node As Node, ByRef errorMessage As StringReference) As Boolean
		Dim command As Char ()
		Dim success, isText As Boolean
		Dim parts As StringReference ()

		success = true
		isText = false

		If token.Length <= 2
			isText = true
		ElseIf token(0) = "\"C And token(1) = "{"C
			isText = true
		ElseIf token(0) <> "{"C
			isText = true
		Else
			command = Substring(token, 1, token.Length - 1)
			parts = SplitByCharacter(command, " "C)

			If command.Length > 0
				If StringsEqual(parts(0).stringx, "use".ToCharArray())
					If parts.Length = 2
						node.type = CopyString(parts(0).stringx)
						node.p1 = CopyString(parts(1).stringx)
					Else
						success = false
						errorMessage.stringx = "The use command takes one parameter.".ToCharArray()
					End If
				ElseIf StringsEqual(parts(0).stringx, "end".ToCharArray())
					If parts.Length = 1
						node.type = CopyString(parts(0).stringx)
					Else
						success = false
						errorMessage.stringx = "The end command takes no parameters.".ToCharArray()
					End If
				ElseIf StringsEqual(parts(0).stringx, "print".ToCharArray())
					If parts.Length = 2
						node.type = CopyString(parts(0).stringx)
						node.p1 = CopyString(parts(1).stringx)
					Else
						success = false
						errorMessage.stringx = "The print command takes one parameter.".ToCharArray()
					End If
				ElseIf StringsEqual(parts(0).stringx, "foreach".ToCharArray())
					If parts.Length = 4
						If StringsEqual(parts(2).stringx, "in".ToCharArray())
							node.type = CopyString(parts(0).stringx)
							node.p1 = CopyString(parts(1).stringx)
							node.p2 = CopyString(parts(3).stringx)
						Else
							success = false
							errorMessage.stringx = "The foreach command must have ""in"" after the first parameter.".ToCharArray()
						End If
					Else
						success = false
						errorMessage.stringx = "The foreach command takes three parameters.".ToCharArray()
					End If
				ElseIf StringsEqual(parts(0).stringx, "if".ToCharArray())
					If parts.Length = 2
						node.type = CopyString(parts(0).stringx)
						node.p1 = CopyString(parts(1).stringx)
					Else
						success = false
						errorMessage.stringx = "The if command takes one parameter.".ToCharArray()
					End If
				ElseIf StringsEqual(parts(0).stringx, "else".ToCharArray())
					If parts.Length = 1
						node.type = CopyString(parts(0).stringx)
					Else
						success = false
						errorMessage.stringx = "The else command takes no parameters.".ToCharArray()
					End If
				Else
					isText = true
				End If
			Else
				isText = true
			End If
		End If

		If isText
			node.type = "text".ToCharArray()
			node.p1 = ReplaceString(token, "\{print ".ToCharArray(), "{print ".ToCharArray())
			node.p1 = ReplaceString(node.p1, "\{use ".ToCharArray(), "{use ".ToCharArray())
			node.p1 = ReplaceString(node.p1, "\{if ".ToCharArray(), "{if ".ToCharArray())
			node.p1 = ReplaceString(node.p1, "\{end}".ToCharArray(), "{end}".ToCharArray())
			node.p1 = ReplaceString(node.p1, "\{foreach ".ToCharArray(), "{foreach ".ToCharArray())
			node.p1 = ReplaceString(node.p1, "\{else}".ToCharArray(), "{else}".ToCharArray())
		End If

		Return success
	End Function


	Public Function test() As Double
		Dim failures As NumberReference

		failures = CreateNumberReference(0)

		Call testTokenGeneration(failures)

		Call testGenerateDocument1(failures)
		Call testGenerateDocument2(failures)
		Call testGenerateDocument3(failures)
		Call testGenerateDocument4(failures)
		Call testGenerateDocument5(failures)
		Call testGenerateDocument6(failures)
		Call testGenerateDocument7(failures)
		Call testGenerateDocument8(failures)

		Return failures.numberValue
	End Function


	Public Sub testGenerateDocument8(ByRef failures As NumberReference)
		Dim document, errorMessage As StringReference
		Dim success As Boolean
		Dim template, json As Char ()

		document = New StringReference()
		errorMessage = New StringReference()

		template = "This is a test: {print b} {foreach x in a}{print x}{end}.".ToCharArray()
		json = "{""a"": [1, 2, 3], ""b"": 4}".ToCharArray()
		success = GenerateDocument(template, json, document, errorMessage)

		If success
			Call AssertStringEquals("This is a test: 4 123.".ToCharArray(), document.stringx, failures)
		End If

		Call AssertTrue(success, failures)
	End Sub


	Public Sub testTokenGeneration(ByRef failures As NumberReference)
		Dim template As Char ()
		Dim tokens As LinkedListStrings
		Dim success As Boolean
		Dim errorMessage As StringReference

		errorMessage = New StringReference()

		tokens = CreateLinkedListString()
		template = "This is a template, this is a value: {print a}.".ToCharArray()
		success = GenerateTokensFromTemplate(template, tokens, errorMessage)
		Call AssertTrue(success, failures)
		Call AssertEquals(3, LinkedListStringsLength(tokens), failures)

		tokens = CreateLinkedListString()
		template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".ToCharArray()
		success = GenerateTokensFromTemplate(template, tokens, errorMessage)
		Call AssertTrue(success, failures)
		Call AssertEquals(9, LinkedListStringsLength(tokens), failures)

		tokens = CreateLinkedListString()
		template = "This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".ToCharArray()
		success = GenerateTokensFromTemplate(template, tokens, errorMessage)
		Call AssertTrue(success, failures)
		Call AssertEquals(16, LinkedListStringsLength(tokens), failures)

		tokens = CreateLinkedListString()
		template = "T: {foreach a in b}{print a}{end}.".ToCharArray()
		success = GenerateTokensFromTemplate(template, tokens, errorMessage)
		Call AssertTrue(success, failures)
		Call AssertEquals(5, LinkedListStringsLength(tokens), failures)
	End Sub


	Public Sub testGenerateDocument1(ByRef failures As NumberReference)
		Call AssertTemplateResult("This is a template, this is a value: {print a}.".ToCharArray(), "{""c"": 5, ""a"": 6}".ToCharArray(), "This is a template, this is a value: 6.".ToCharArray(), failures)
	End Sub


	Public Sub testGenerateDocument2(ByRef failures As NumberReference)
		Call AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".ToCharArray(), "{""b"": {""a"": 1, ""b"": 2}, ""a"": 6}".ToCharArray(), "This is a template, this is a value: 6 1 2.".ToCharArray(), failures)
	End Sub


	Public Sub testGenerateDocument3(ByRef failures As NumberReference)
		Call AssertTemplateResult("This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".ToCharArray(), "{""b"": {""a"": 1, ""b"": 2, ""c"": {""a"": 3, ""b"": 4}}, ""a"": 6}".ToCharArray(), "This is a template, this is a value: 6 1 23 416.".ToCharArray(), failures)
	End Sub


	Public Sub testGenerateDocument4(ByRef failures As NumberReference)
		Call AssertTemplateResult("T: {if a}a{end}.".ToCharArray(), "{""b"": {""a"": 1, ""b"": 2, ""c"": {""a"": 3, ""b"": 4}}, ""a"": true}".ToCharArray(), "T: a.".ToCharArray(), failures)

		Call AssertTemplateResult("T: {if a}a{else}b{end}.".ToCharArray(), "{""b"": {""a"": 1, ""b"": 2, ""c"": {""a"": 3, ""b"": 4}}, ""a"": false}".ToCharArray(), "T: b.".ToCharArray(), failures)
	End Sub


	Public Sub testGenerateDocument5(ByRef failures As NumberReference)
		Call AssertTemplateResult("T: {foreach a in b}{print a}{end}.".ToCharArray(), "{""b"": [1, 2, 3, 4]}".ToCharArray(), "T: 1234.".ToCharArray(), failures)
	End Sub


	Public Sub testGenerateDocument6(ByRef failures As NumberReference)
		Call AssertTemplateResult("{test}\{print a}.".ToCharArray(), "{""c"": 5, ""a"": 6}".ToCharArray(), "{test}{print a}.".ToCharArray(), failures)
	End Sub


	Public Sub testGenerateDocument7(ByRef failures As NumberReference)
		Call AssertTemplateResult("{".ToCharArray(), "{}".ToCharArray(), "{".ToCharArray(), failures)

		Call AssertTemplateResult("{  ".ToCharArray(), "{}".ToCharArray(), "{  ".ToCharArray(), failures)

		Call AssertTemplateResult("{use a}\{print a}{end}".ToCharArray(), "{""a"": {}}".ToCharArray(), "{print a}".ToCharArray(), failures)

		Call AssertTemplateResult("\{print a}{print a}}".ToCharArray(), "{""a"": 3}".ToCharArray(), "{print a}3}".ToCharArray(), failures)

		Call AssertTemplateResult("\\{print a}{print a}}".ToCharArray(), "{""a"": 3}".ToCharArray(), "\{print a}3}".ToCharArray(), failures)

		Call AssertTemplateResult("\\{print a}{print a}\\{print a}}".ToCharArray(), "{""a"": 3}".ToCharArray(), "\{print a}3\{print a}}".ToCharArray(), failures)

		Call AssertTemplateResult("\{print a}{print a}\{".ToCharArray(), "{""a"": 3}".ToCharArray(), "{print a}3\{".ToCharArray(), failures)

		Call AssertTemplateResult("    <div>Pris</div>" + vblf + "    {foreach p in products}" + vblf + "    <div>{print productCode}</div>" + vblf + "    <div>1</div>" + vblf + "    <div>{print price}</div>" + vblf + "    {end}" + vblf + "    <div>Totalt</div>".ToCharArray(), "{""products"": [{""productCode"": ""kl"", ""price"": ""1.2""}, {""productCode"": ""skl"", ""price"": ""20.0""}]}".ToCharArray(), "    <div>Pris</div>" + vblf + "    " + vblf + "    <div>kl</div>" + vblf + "    <div>1</div>" + vblf + "    <div>1.2</div>" + vblf + "    " + vblf + "    <div>skl</div>" + vblf + "    <div>1</div>" + vblf + "    <div>20.0</div>" + vblf + "    " + vblf + "    <div>Totalt</div>".ToCharArray(), failures)

		Call AssertTemplateError("{print".ToCharArray(), "{}".ToCharArray(), "Template command found, but not ended properly.".ToCharArray(), failures)

		Call AssertTemplateError("{print a}".ToCharArray(), "{}".ToCharArray(), "Key for printing not found in JSON object: a".ToCharArray(), failures)

		Call AssertTemplateError("This is a template, this is a value: {print a {print a}.".ToCharArray(), "{""a"": 5}".ToCharArray(), "The print command takes one parameter.".ToCharArray(), failures)

		Call AssertTemplateError("This is a {use a}\{print a}template, this is a value: {print a}.{end}".ToCharArray(), "{""a"": 5}".ToCharArray(), "Data structure for print command is not a JSON object.".ToCharArray(), failures)

		Call AssertTemplateError("{use a}".ToCharArray(), "{}".ToCharArray(), "End block expected at the end of use block.".ToCharArray(), failures)

		Call AssertTemplateError("{if a}".ToCharArray(), "{""a"": true}".ToCharArray(), "End or else block expected at the end of if block.".ToCharArray(), failures)

		Call AssertTemplateError("{if a}{else}".ToCharArray(), "{""a"": true}".ToCharArray(), "End block expected at the end of else block.".ToCharArray(), failures)

		Call AssertTemplateError("{foreach x in a}".ToCharArray(), "{""a"": [1, 2, 3, 4]}".ToCharArray(), "End block expected at the end of foreach block.".ToCharArray(), failures)
	End Sub


	Public Sub AssertTemplateResult(ByRef template As Char (), ByRef json As Char (), ByRef result As Char (), ByRef failures As NumberReference)
		Dim data As ElementReference
		Dim errorMessages As StringArrayReference
		Dim document As StringReference
		Dim errorMessage As StringReference
		Dim success As Boolean

		data = New ElementReference()
		errorMessages = New StringArrayReference()
		document = New StringReference()
		errorMessage = New StringReference()

		success = ReadJSON(json, data, errorMessages)

		Call AssertTrue(success, failures)

		If success
			success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage)

			Call AssertTrue(success, failures)

			If success
				Call AssertStringEquals(document.stringx, result, failures)
			End If
		End If
	End Sub


	Public Sub AssertTemplateError(ByRef template As Char (), ByRef json As Char (), ByRef errorMessage As Char (), ByRef failures As NumberReference)
		Dim data As ElementReference
		Dim errorMessages As StringArrayReference
		Dim document As StringReference
		Dim errorMessageRef As StringReference
		Dim success As Boolean

		data = New ElementReference()
		errorMessages = New StringArrayReference()
		document = New StringReference()
		errorMessageRef = New StringReference()

		success = ReadJSON(json, data, errorMessages)

		Call AssertTrue(success, failures)

		If success
			success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessageRef)

			Call AssertFalse(success, failures)

			If Not success
				Call AssertStringEquals(errorMessage, errorMessageRef.stringx, failures)
			End If
		End If
	End Sub


	Public Function CreateBooleanReference(value As Boolean) As BooleanReference
		Dim ref As BooleanReference

		ref = New BooleanReference()
		ref.booleanValue = value

		Return ref
	End Function


	Public Function CreateBooleanArrayReference(ByRef value As Boolean ()) As BooleanArrayReference
		Dim ref As BooleanArrayReference

		ref = New BooleanArrayReference()
		ref.booleanArray = value

		Return ref
	End Function


	Public Function CreateBooleanArrayReferenceLengthValue(length As Double, value As Boolean) As BooleanArrayReference
		Dim ref As BooleanArrayReference
		Dim i As Double

		ref = New BooleanArrayReference()
		ref.booleanArray = New Boolean (length - 1){}

		i = 0
		While i < length
			ref.booleanArray(i) = value
			i = i + 1
		End While

		Return ref
	End Function


	Public Sub FreeBooleanArrayReference(ByRef booleanArrayReference As BooleanArrayReference)
		Erase booleanArrayReference.booleanArray 
		booleanArrayReference = Nothing
	End Sub


	Public Function CreateCharacterReference(value As Char) As CharacterReference
		Dim ref As CharacterReference

		ref = New CharacterReference()
		ref.characterValue = value

		Return ref
	End Function


	Public Function CreateNumberReference(value As Double) As NumberReference
		Dim ref As NumberReference

		ref = New NumberReference()
		ref.numberValue = value

		Return ref
	End Function


	Public Function CreateNumberArrayReference(ByRef value As Double ()) As NumberArrayReference
		Dim ref As NumberArrayReference

		ref = New NumberArrayReference()
		ref.numberArray = value

		Return ref
	End Function


	Public Function CreateNumberArrayReferenceLengthValue(length As Double, value As Double) As NumberArrayReference
		Dim ref As NumberArrayReference
		Dim i As Double

		ref = New NumberArrayReference()
		ref.numberArray = New Double (length - 1){}

		i = 0
		While i < length
			ref.numberArray(i) = value
			i = i + 1
		End While

		Return ref
	End Function


	Public Sub FreeNumberArrayReference(ByRef numberArrayReference As NumberArrayReference)
		Erase numberArrayReference.numberArray 
		numberArrayReference = Nothing
	End Sub


	Public Function CreateStringReference(ByRef value As Char ()) As StringReference
		Dim ref As StringReference

		ref = New StringReference()
		ref.stringx = value

		Return ref
	End Function


	Public Function CreateStringReferenceLengthValue(length As Double, value As Char) As StringReference
		Dim ref As StringReference
		Dim i As Double

		ref = New StringReference()
		ref.stringx = New Char (length - 1){}

		i = 0
		While i < length
			ref.stringx(i) = value
			i = i + 1
		End While

		Return ref
	End Function


	Public Sub FreeStringReference(ByRef stringReference As StringReference)
		Erase stringReference.stringx 
		stringReference = Nothing
	End Sub


	Public Function CreateStringArrayReference(ByRef strings As StringReference ()) As StringArrayReference
		Dim ref As StringArrayReference

		ref = New StringArrayReference()
		ref.stringArray = strings

		Return ref
	End Function


	Public Function CreateStringArrayReferenceLengthValue(length As Double, ByRef value As Char ()) As StringArrayReference
		Dim ref As StringArrayReference
		Dim i As Double

		ref = New StringArrayReference()
		ref.stringArray = New StringReference (length - 1){}

		i = 0
		While i < length
			ref.stringArray(i) = CreateStringReference(value)
			i = i + 1
		End While

		Return ref
	End Function


	Public Sub FreeStringArrayReference(ByRef stringArrayReference As StringArrayReference)
		Dim i As Double

		i = 0
		While i < stringArrayReference.stringArray.Length
			stringArrayReference.stringArray(i) = Nothing
			i = i + 1
		End While
		Erase stringArrayReference.stringArray 
		stringArrayReference = Nothing
	End Sub


	Public Function IsValidJSON(ByRef json As Char (), ByRef errorMessage As StringArrayReference) As Boolean
		Dim success As Boolean
		Dim elementReference As ElementReference

		elementReference = New ElementReference()

		success = ReadJSON(json, elementReference, errorMessage)

		If success
			Call DeleteElement(elementReference.element)
		End If

		Return success
	End Function


	Public Function JSONTokenize(ByRef json As Char (), ByRef tokensReference As StringArrayReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim i As Double
		Dim c As Char
		Dim str As Char ()
		Dim stringReference, tokenReference As StringReference
		Dim stringLength As NumberReference
		Dim success As Boolean
		Dim ll As LinkedListStrings

		ll = CreateLinkedListString()
		success = true

		stringLength = New NumberReference()
		tokenReference = New StringReference()

		i = 0
		While i < json.Length And success
			c = json(i)

			If c = "{"C
				Call LinkedListAddString(ll, "{".ToCharArray())
				i = i + 1
			ElseIf c = "}"C
				Call LinkedListAddString(ll, "}".ToCharArray())
				i = i + 1
			ElseIf c = "["C
				Call LinkedListAddString(ll, "[".ToCharArray())
				i = i + 1
			ElseIf c = "]"C
				Call LinkedListAddString(ll, "]".ToCharArray())
				i = i + 1
			ElseIf c = ":"C
				Call LinkedListAddString(ll, ":".ToCharArray())
				i = i + 1
			ElseIf c = ","C
				Call LinkedListAddString(ll, ",".ToCharArray())
				i = i + 1
			ElseIf c = "f"C
				success = GetJSONPrimitiveName(json, i, errorMessages, "false".ToCharArray(), tokenReference)
				If success
					Call LinkedListAddString(ll, "false".ToCharArray())
					i = i + "false".ToCharArray().Length
				End If
			ElseIf c = "t"C
				success = GetJSONPrimitiveName(json, i, errorMessages, "true".ToCharArray(), tokenReference)
				If success
					Call LinkedListAddString(ll, "true".ToCharArray())
					i = i + "true".ToCharArray().Length
				End If
			ElseIf c = "n"C
				success = GetJSONPrimitiveName(json, i, errorMessages, "null".ToCharArray(), tokenReference)
				If success
					Call LinkedListAddString(ll, "null".ToCharArray())
					i = i + "null".ToCharArray().Length
				End If
			ElseIf c = " "C Or c = vblf Or c = vbTab Or c = vbcr
				' Skip.
				i = i + 1
			ElseIf c = """"C
				success = GetJSONString(json, i, tokenReference, stringLength, errorMessages)
				If success
					Call LinkedListAddString(ll, tokenReference.stringx)
					i = i + stringLength.numberValue
				End If
			ElseIf IsJSONNumberCharacter(c)
				success = GetJSONNumberToken(json, i, tokenReference, errorMessages)
				If success
					Call LinkedListAddString(ll, tokenReference.stringx)
					i = i + tokenReference.stringx.Length
				End If
			Else
				str = ConcatenateCharacter("Invalid start of Token: ".ToCharArray(), c)
				stringReference = CreateStringReference(str)
				Call AddStringRef(errorMessages, stringReference)
				i = i + 1
				success = false
			End If
		End While

		If success
			Call LinkedListAddString(ll, "<end>".ToCharArray())
			tokensReference.stringArray = LinkedListStringsToArray(ll)
			Call FreeLinkedListString(ll)
		End If

		Return success
	End Function


	Public Function GetJSONNumberToken(ByRef json As Char (), start As Double, ByRef tokenReference As StringReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim c As Char
		Dim endx, i As Double
		Dim done, success As Boolean
		Dim numberString As Char ()

		endx = json.Length
		done = false

		i = start
		While i < json.Length And Not done
			c = json(i)
			If Not IsJSONNumberCharacter(c)
				done = true
				endx = i
			End If
			i = i + 1
		End While

		numberString = Substring(json, start, endx)

		success = IsValidJSONNumber(numberString, errorMessages)

		tokenReference.stringx = numberString

		Return success
	End Function


	Public Function IsValidJSONNumber(ByRef n As Char (), ByRef errorMessages As StringArrayReference) As Boolean
		Dim success As Boolean
		Dim i As Double

		i = 0

		' JSON allows an optional negative sign.
		If n(i) = "-"C
			i = i + 1
		End If

		If i < n.Length
			success = IsValidJSONNumberAfterSign(n, i, errorMessages)
		Else
			success = false
			Call AddStringRef(errorMessages, CreateStringReference("Number must contain at least one digit.".ToCharArray()))
		End If

		Return success
	End Function


	Public Function IsValidJSONNumberAfterSign(ByRef n As Char (), i As Double, ByRef errorMessages As StringArrayReference) As Boolean
		Dim success As Boolean

		If charIsNumber(n(i))
			' 0 first means only 0.
			If n(i) = "0"C
				i = i + 1
			Else
				' 1-9 first, read following digits.
				i = IsValidJSONNumberAdvancePastDigits(n, i)
			End If

			If i < n.Length
				success = IsValidJSONNumberFromDotOrExponent(n, i, errorMessages)
			Else
				' If integer, we are done now.
				success = true
			End If
		Else
			success = false
			Call AddStringRef(errorMessages, CreateStringReference("A number must start with 0-9 (after the optional sign).".ToCharArray()))
		End If

		Return success
	End Function


	Public Function IsValidJSONNumberAdvancePastDigits(ByRef n As Char (), i As Double) As Double
		Dim done As Boolean

		i = i + 1
		done = false
		
		While i < n.Length And Not done
			If charIsNumber(n(i))
				i = i + 1
			Else
				done = true
			End If
		End While

		Return i
	End Function


	Public Function IsValidJSONNumberFromDotOrExponent(ByRef n As Char (), i As Double, ByRef errorMessages As StringArrayReference) As Boolean
		Dim wasDotAndOrE, success As Boolean

		wasDotAndOrE = false
		success = true

		If n(i) = "."C
			i = i + 1
			wasDotAndOrE = true

			If i < n.Length
				If charIsNumber(n(i))
					' Read digits following decimal point.
					i = IsValidJSONNumberAdvancePastDigits(n, i)

					If i = n.Length
						' If non-scientific decimal number, we are done.
						success = true
					End If
				Else
					success = false
					Call AddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".ToCharArray()))
				End If
			Else
				success = false
				Call AddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".ToCharArray()))
			End If
		End If

		If i < n.Length And success
			If n(i) = "e"C Or n(i) = "E"C
				wasDotAndOrE = true
				success = IsValidJSONNumberFromExponent(n, i, errorMessages)
			Else
				success = false
				Call AddStringRef(errorMessages, CreateStringReference("Expected e or E.".ToCharArray()))
			End If
		ElseIf i = n.Length And success
			' If number with decimal point.
			success = true
		Else
			success = false
			Call AddStringRef(errorMessages, CreateStringReference("There must be numbers after the decimal point.".ToCharArray()))
		End If

		If wasDotAndOrE
		Else
			success = false
			Call AddStringRef(errorMessages, CreateStringReference("Exprected decimal point or e or E.".ToCharArray()))
		End If

		Return success
	End Function


	Public Function IsValidJSONNumberFromExponent(ByRef n As Char (), i As Double, ByRef errorMessages As StringArrayReference) As Boolean
		Dim success As Boolean

		i = i + 1

		If i < n.Length
			' The exponent sign can either + or -,
			If n(i) = "+"C Or n(i) = "-"C
				i = i + 1
			End If

			If i < n.Length
				If charIsNumber(n(i))
					' Read digits following decimal point.
					i = IsValidJSONNumberAdvancePastDigits(n, i)

					If i = n.Length
						' We found scientific number.
						success = true
					Else
						success = false
						Call AddStringRef(errorMessages, CreateStringReference("There was characters following the exponent.".ToCharArray()))
					End If
				Else
					success = false
					Call AddStringRef(errorMessages, CreateStringReference("There must be a digit following the optional exponent sign.".ToCharArray()))
				End If
			Else
				success = false
				Call AddStringRef(errorMessages, CreateStringReference("There must be a digit following optional the exponent sign.".ToCharArray()))
			End If
		Else
			success = false
			Call AddStringRef(errorMessages, CreateStringReference("There must be a sign or a digit following e or E.".ToCharArray()))
		End If

		Return success
	End Function


	Public Function IsJSONNumberCharacter(c As Char) As Boolean
		Dim numericCharacters As Char ()
		Dim found As Boolean
		Dim i As Double

		numericCharacters = "0123456789.-+eE".ToCharArray()

		found = false

		i = 0
		While i < numericCharacters.Length
			If numericCharacters(i) = c
				found = true
			End If
			i = i + 1
		End While

		Return found
	End Function


	Public Function GetJSONPrimitiveName(ByRef stringx As Char (), start As Double, ByRef errorMessages As StringArrayReference, ByRef primitive As Char (), ByRef tokenReference As StringReference) As Boolean
		Dim c, p As Char
		Dim done, success As Boolean
		Dim i As Double
		Dim str, token As Char ()

		done = false
		success = true

		token = "".ToCharArray()

		i = start
		While i < stringx.Length And ((i - start) < primitive.Length) And Not done
			c = stringx(i)
			p = primitive(i - start)
			If c = p
				' OK
				If (i + 1 - start) = primitive.Length
					done = true
				End If
			Else
				str = "".ToCharArray()
				str = ConcatenateString(str, "Primitive invalid: ".ToCharArray())
				str = AppendCharacter(str, c)
				str = AppendString(str, " vs ".ToCharArray())
				str = AppendCharacter(str, p)

				Call AddStringRef(errorMessages, CreateStringReference(str))
				done = true
				success = false
			End If
			i = i + 1
		End While

		If done
			If StringsEqual(primitive, "false".ToCharArray())
				token = "false".ToCharArray()
			End If
			If StringsEqual(primitive, "true".ToCharArray())
				token = "true".ToCharArray()
			End If
			If StringsEqual(primitive, "null".ToCharArray())
				token = "null".ToCharArray()
			End If
		Else
			Call AddStringRef(errorMessages, CreateStringReference("Primitive invalid".ToCharArray()))
			success = false
		End If

		tokenReference.stringx = token

		Return success
	End Function


	Public Function GetJSONString(ByRef json As Char (), start As Double, ByRef tokenReference As StringReference, ByRef stringLengthReference As NumberReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim success, done As Boolean
		Dim stringx, hex As Char ()
		Dim characterCount, hexReference As NumberReference
		Dim i, l, c As Double
		Dim errorMessage As StringReference

		characterCount = CreateNumberReference(0)
		hex = CreateString(4, "0"C)
		hexReference = New NumberReference()
		errorMessage = New StringReference()

		success = IsValidJSONStringInJSON(json, start, characterCount, stringLengthReference, errorMessages)

		If success
			l = characterCount.numberValue
			stringx = New Char (l - 1){}

			c = 0
			stringx(c) = """"C
			c = c + 1

			done = false
			i = start + 1
			While Not done
				If json(i) = "\"C
					i = i + 1
					If json(i) = """"C Or json(i) = "\"C Or json(i) = "/"C
						stringx(c) = json(i)
						c = c + 1
					ElseIf json(i) = "b"C
						stringx(c) = Convert.ToChar(Convert.ToInt64(8))
						c = c + 1
					ElseIf json(i) = "f"C
						stringx(c) = Convert.ToChar(Convert.ToInt64(12))
						c = c + 1
					ElseIf json(i) = "n"C
						stringx(c) = Convert.ToChar(Convert.ToInt64(10))
						c = c + 1
					ElseIf json(i) = "r"C
						stringx(c) = Convert.ToChar(Convert.ToInt64(13))
						c = c + 1
					ElseIf json(i) = "t"C
						stringx(c) = Convert.ToChar(Convert.ToInt64(9))
						c = c + 1
					ElseIf json(i) = "u"C
						i = i + 1
						hex(0) = charToUpperCase(json(i + 0))
						hex(1) = charToUpperCase(json(i + 1))
						hex(2) = charToUpperCase(json(i + 2))
						hex(3) = charToUpperCase(json(i + 3))
						nCreateNumberFromStringWithCheck(hex, 16, hexReference, errorMessage)
						stringx(c) = Convert.ToChar(Convert.ToInt64(hexReference.numberValue))
						i = i + 3
						c = c + 1
					End If
				ElseIf json(i) = """"C
					stringx(c) = json(i)
					c = c + 1
					done = true
				Else
					stringx(c) = json(i)
					c = c + 1
				End If
				i = i + 1
			End While

			tokenReference.stringx = stringx
			success = true
		Else
			Call AddStringRef(errorMessages, CreateStringReference("End of string was not found.".ToCharArray()))
			success = false
		End If

		Return success
	End Function


	Public Function IsValidJSONString(ByRef jsonString As Char (), ByRef errorMessages As StringArrayReference) As Boolean
		Dim valid As Boolean
		Dim numberReference, stringLength As NumberReference

		numberReference = New NumberReference()
		stringLength = New NumberReference()

		valid = IsValidJSONStringInJSON(jsonString, 0, numberReference, stringLength, errorMessages)

		Return valid
	End Function


	Public Function IsValidJSONStringInJSON(ByRef json As Char (), start As Double, ByRef characterCount As NumberReference, ByRef stringLengthReference As NumberReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim success, done As Boolean
		Dim i, j As Double
		Dim c As Char

		success = true
		done = false

		characterCount.numberValue = 1

		i = start + 1
		While i < json.Length And Not done And success
			If Not IsJSONIllegalControllCharacter(json(i))
				If json(i) = "\"C
					i = i + 1
					If i < json.Length
						If json(i) = """"C Or json(i) = "\"C Or json(i) = "/"C Or json(i) = "b"C Or json(i) = "f"C Or json(i) = "n"C Or json(i) = "r"C Or json(i) = "t"C
							characterCount.numberValue = characterCount.numberValue + 1
						ElseIf json(i) = "u"C
							If i + 4 < json.Length
								j = 0
								While j < 4 And success
									c = json(i + j + 1)
									If nCharacterIsNumberCharacterInBase(c, 16) Or c = "a"C Or c = "b"C Or c = "c"C Or c = "d"C Or c = "e"C Or c = "f"C
									Else
										success = false
										Call AddStringRef(errorMessages, CreateStringReference("\u must be followed by four hexadecimal digits.".ToCharArray()))
									End If
									j = j + 1
								End While
								characterCount.numberValue = characterCount.numberValue + 1
								i = i + 4
							Else
								success = false
								Call AddStringRef(errorMessages, CreateStringReference("\u must be followed by four characters.".ToCharArray()))
							End If
						Else
							success = false
							Call AddStringRef(errorMessages, CreateStringReference("Escaped character invalid.".ToCharArray()))
						End If
					Else
						success = false
						Call AddStringRef(errorMessages, CreateStringReference("There must be at least two character after string escape.".ToCharArray()))
					End If
				ElseIf json(i) = """"C
					characterCount.numberValue = characterCount.numberValue + 1
					done = true
				Else
					characterCount.numberValue = characterCount.numberValue + 1
				End If
			Else
				success = false
				Call AddStringRef(errorMessages, CreateStringReference("Unicode code points 0-31 not allowed in JSON string.".ToCharArray()))
			End If
			i = i + 1
		End While

		If done
			stringLengthReference.numberValue = i - start
		Else
			success = false
			Call AddStringRef(errorMessages, CreateStringReference("String must end with "".".ToCharArray()))
		End If

		Return success
	End Function


	Public Function IsJSONIllegalControllCharacter(c As Char) As Boolean
		Dim cnr As Double
		Dim isControll As Boolean

		cnr = Convert.ToInt16(c)

		If cnr >= 0 And cnr < 32
			isControll = true
		Else
			isControll = false
		End If

		Return isControll
	End Function


	Public Function AddElement(ByRef list As Element (), ByRef a As Element) As Element ()
		Dim newlist As Element ()
		Dim i As Double

		newlist = New Element (list.Length + 1 - 1){}

		i = 0
		While i < list.Length
			newlist(i) = list(i)
			i = i + 1
		End While
		newlist(list.Length) = a

		Erase list 

		Return newlist
	End Function


	Public Sub AddElementRef(ByRef list As ElementArrayReference, ByRef i As Element)
		list.arrayx = AddElement(list.arrayx, i)
	End Sub


	Public Function RemoveElement(ByRef list As Element (), n As Double) As Element ()
		Dim newlist As Element ()
		Dim i As Double

		newlist = New Element (list.Length - 1 - 1){}

		i = 0
		While i < list.Length
			If i < n
				newlist(i) = list(i)
			End If
			If i > n
				newlist(i - 1) = list(i)
			End If
			i = i + 1
		End While

		Erase list 

		Return newlist
	End Function


	Public Function GetElementRef(ByRef list As ElementArrayReference, i As Double) As Element
		Return list.arrayx(i)
	End Function


	Public Sub RemoveElementRef(ByRef list As ElementArrayReference, i As Double)
		list.arrayx = RemoveElement(list.arrayx, i)
	End Sub


	Public Function CreateLinkedListElements() As LinkedListElements
		Dim ll As LinkedListElements

		ll = New LinkedListElements()
		ll.first = New LinkedListNodeElements()
		ll.last = ll.first
		ll.last.endx = true

		Return ll
	End Function


	Public Sub LinkedListAddElement(ByRef ll As LinkedListElements, ByRef value As Element)
		ll.last.endx = false
		ll.last.value = value
		ll.last.nextx = New LinkedListNodeElements()
		ll.last.nextx.endx = true
		ll.last = ll.last.nextx
	End Sub


	Public Function LinkedListElementsToArray(ByRef ll As LinkedListElements) As Element ()
		Dim arrayx As Element ()
		Dim length, i As Double
		Dim node As LinkedListNodeElements

		node = ll.first

		length = LinkedListElementsLength(ll)

		arrayx = New Element (length - 1){}

		i = 0
		While i < length
			arrayx(i) = node.value
			node = node.nextx
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function LinkedListElementsLength(ByRef ll As LinkedListElements) As Double
		Dim l As Double
		Dim node As LinkedListNodeElements

		l = 0
		node = ll.first
		
		While Not node.endx
			node = node.nextx
			l = l + 1
		End While

		Return l
	End Function


	Public Sub FreeLinkedListElements(ByRef ll As LinkedListElements)
		Dim node, prev As LinkedListNodeElements

		node = ll.first

		
		While Not node.endx
			prev = node
			node = node.nextx
			prev = Nothing
		End While

		node = Nothing
	End Sub


	Public Function ComputeJSONStringLength(ByRef element As Element) As Double
		Dim result As Double

		result = 0

		If StringsEqual(element.type, "object".ToCharArray())
			result = result + ComputeJSONObjectStringLength(element)
		ElseIf StringsEqual(element.type, "string".ToCharArray())
			result = JSONEscapedStringLength(element.stringx) + 2
		ElseIf StringsEqual(element.type, "array".ToCharArray())
			result = result + ComputeJSONArrayStringLength(element)
		ElseIf StringsEqual(element.type, "number".ToCharArray())
			result = result + ComputeJSONNumberStringLength(element)
		ElseIf StringsEqual(element.type, "null".ToCharArray())
			result = result + "null".ToCharArray().Length
		ElseIf StringsEqual(element.type, "boolean".ToCharArray())
			result = result + ComputeJSONBooleanStringLength(element)
		End If

		Return result
	End Function


	Public Function ComputeJSONBooleanStringLength(ByRef element As Element) As Double
		Dim result As Double

		If element.booleanValue
			result = "true".ToCharArray().Length
		Else
			result = "false".ToCharArray().Length
		End If

		Return result
	End Function


	Public Function ComputeJSONNumberStringLength(ByRef element As Element) As Double
		Dim length As Double
		Dim a As Char ()

		If element.number <> 0
			If Abs(element.number) >= 10 ^ 15 Or Abs(element.number) <= 10 ^ (-15)
				a = nCreateStringScientificNotationDecimalFromNumber(element.number)
				length = a.Length
			Else
				a = nCreateStringDecimalFromNumber(element.number)
				length = a.Length
			End If
		Else
			length = 1
		End If

		Return length
	End Function


	Public Function ComputeJSONArrayStringLength(ByRef element As Element) As Double
		Dim arrayElement As Element
		Dim i As Double
		Dim length As Double

		length = 1

		i = 0
		While i < element.arrayx.Length
			arrayElement = element.arrayx(i)

			length = length + ComputeJSONStringLength(arrayElement)

			If i + 1 <> element.arrayx.Length
				length = length + 1
			End If
			i = i + 1
		End While

		length = length + 1

		Return length
	End Function


	Public Function ComputeJSONObjectStringLength(ByRef element As Element) As Double
		Dim key As Char ()
		Dim i As Double
		Dim keys As StringArrayReference
		Dim objectElement As Element
		Dim length As Double

		length = 1

		keys = GetStringElementMapKeySet(element.objectx)
		i = 0
		While i < keys.stringArray.Length
			key = keys.stringArray(i).stringx
			objectElement = GetObjectValue(element.objectx, key)

			length = length + 1
			length = length + JSONEscapedStringLength(key)
			length = length + 1
			length = length + 1

			length = length + ComputeJSONStringLength(objectElement)

			If i + 1 <> keys.stringArray.Length
				length = length + 1
			End If
			i = i + 1
		End While

		length = length + 1

		Return length
	End Function


	Public Function CreateStringElement(ByRef stringx As Char ()) As Element
		Dim element As Element
		element = New Element()
		element.type = "string".ToCharArray()
		element.stringx = stringx
		Return element
	End Function


	Public Function CreateBooleanElement(booleanValue As Boolean) As Element
		Dim element As Element
		element = New Element()
		element.type = "boolean".ToCharArray()
		element.booleanValue = booleanValue
		Return element
	End Function


	Public Function CreateNullElement() As Element
		Dim element As Element
		element = New Element()
		element.type = "null".ToCharArray()
		Return element
	End Function


	Public Function CreateNumberElement(number As Double) As Element
		Dim element As Element
		element = New Element()
		element.type = "number".ToCharArray()
		element.number = number
		Return element
	End Function


	Public Function CreateArrayElement(length As Double) As Element
		Dim element As Element
		element = New Element()
		element.type = "array".ToCharArray()
		element.arrayx = New Element (length - 1){}
		Return element
	End Function


	Public Function CreateObjectElement(length As Double) As Element
		Dim element As Element
		element = New Element()
		element.type = "object".ToCharArray()
		element.objectx = New StringElementMap()
		element.objectx.stringListRef = CreateStringArrayReferenceLengthValue(length, "".ToCharArray())
		element.objectx.elementListRef = New ElementArrayReference()
		element.objectx.elementListRef.arrayx = New Element (length - 1){}
		Return element
	End Function


	Public Sub DeleteElement(ByRef element As Element)
		If StringsEqual(element.type, "object".ToCharArray())
			Call DeleteObject(element)
		ElseIf StringsEqual(element.type, "string".ToCharArray())
			element = Nothing
		ElseIf StringsEqual(element.type, "array".ToCharArray())
			Call DeleteArray(element)
		ElseIf StringsEqual(element.type, "number".ToCharArray())
			element = Nothing
		ElseIf StringsEqual(element.type, "null".ToCharArray())
			element = Nothing
		ElseIf StringsEqual(element.type, "boolean".ToCharArray())
			element = Nothing
		Else
		End If
	End Sub


	Public Sub DeleteObject(ByRef element As Element)
		Dim keys As StringArrayReference
		Dim i As Double
		Dim key As Char ()
		Dim objectElement As Element

		keys = GetStringElementMapKeySet(element.objectx)
		i = 0
		While i < keys.stringArray.Length
			key = keys.stringArray(i).stringx
			objectElement = GetObjectValue(element.objectx, key)
			Call DeleteElement(objectElement)
			i = i + 1
		End While
	End Sub


	Public Sub DeleteArray(ByRef element As Element)
		Dim i As Double
		Dim arrayElement As Element

		i = 0
		While i < element.arrayx.Length
			arrayElement = element.arrayx(i)
			Call DeleteElement(arrayElement)
			i = i + 1
		End While
	End Sub


	Public Function WriteJSON(ByRef element As Element) As Char ()
		Dim result As Char ()
		Dim length As Double
		Dim index As NumberReference

		length = ComputeJSONStringLength(element)
		result = New Char (length - 1){}
		index = CreateNumberReference(0)

		If StringsEqual(element.type, "object".ToCharArray())
			Call WriteObject(element, result, index)
		ElseIf StringsEqual(element.type, "string".ToCharArray())
			Call WriteString(element, result, index)
		ElseIf StringsEqual(element.type, "array".ToCharArray())
			Call WriteArray(element, result, index)
		ElseIf StringsEqual(element.type, "number".ToCharArray())
			Call WriteNumber(element, result, index)
		ElseIf StringsEqual(element.type, "null".ToCharArray())
			Call WriteStringToStingStream(result, index, "null".ToCharArray())
		ElseIf StringsEqual(element.type, "boolean".ToCharArray())
			Call WriteBooleanValue(element, result, index)
		End If

		Return result
	End Function


	Public Sub WriteBooleanValue(ByRef element As Element, ByRef result As Char (), ByRef index As NumberReference)
		If element.booleanValue
			Call WriteStringToStingStream(result, index, "true".ToCharArray())
		Else
			Call WriteStringToStingStream(result, index, "false".ToCharArray())
		End If
	End Sub


	Public Sub WriteNumber(ByRef element As Element, ByRef result As Char (), ByRef index As NumberReference)
		Dim numberString As Char ()

		If element.number <> 0
			If Abs(element.number) >= 10 ^ 15 Or Abs(element.number) <= 10 ^ (-15)
				numberString = nCreateStringScientificNotationDecimalFromNumber(element.number)
			Else
				numberString = nCreateStringDecimalFromNumber(element.number)
			End If
		Else
			numberString = nCreateStringDecimalFromNumber(element.number)
		End If

		Call WriteStringToStingStream(result, index, numberString)
	End Sub


	Public Sub WriteArray(ByRef element As Element, ByRef result As Char (), ByRef index As NumberReference)
		Dim s As Char ()
		Dim arrayElement As Element
		Dim i As Double

		Call WriteStringToStingStream(result, index, "[".ToCharArray())

		i = 0
		While i < element.arrayx.Length
			arrayElement = element.arrayx(i)

			s = WriteJSON(arrayElement)
			Call WriteStringToStingStream(result, index, s)

			If i + 1 <> element.arrayx.Length
				Call WriteStringToStingStream(result, index, ",".ToCharArray())
			End If
			i = i + 1
		End While

		Call WriteStringToStingStream(result, index, "]".ToCharArray())
	End Sub


	Public Sub WriteString(ByRef element As Element, ByRef result As Char (), ByRef index As NumberReference)
		Call WriteStringToStingStream(result, index, """".ToCharArray())
		element.stringx = JSONEscapeString(element.stringx)
		Call WriteStringToStingStream(result, index, element.stringx)
		Call WriteStringToStingStream(result, index, """".ToCharArray())
	End Sub


	Public Function JSONEscapeString(ByRef stringx As Char ()) As Char ()
		Dim i, length As Double
		Dim index, lettersReference As NumberReference
		Dim ns, escaped As Char ()

		length = JSONEscapedStringLength(stringx)

		ns = New Char (length - 1){}
		index = CreateNumberReference(0)
		lettersReference = CreateNumberReference(0)

		i = 0
		While i < stringx.Length
			If JSONMustBeEscaped(stringx(i), lettersReference)
				escaped = JSONEscapeCharacter(stringx(i))
				Call WriteStringToStingStream(ns, index, escaped)
			Else
				Call WriteCharacterToStingStream(ns, index, stringx(i))
			End If
			i = i + 1
		End While

		Return ns
	End Function


	Public Function JSONEscapedStringLength(ByRef stringx As Char ()) As Double
		Dim lettersReference As NumberReference
		Dim i, length As Double

		lettersReference = CreateNumberReference(0)
		length = 0

		i = 0
		While i < stringx.Length
			If JSONMustBeEscaped(stringx(i), lettersReference)
				length = length + lettersReference.numberValue
			Else
				length = length + 1
			End If
			i = i + 1
		End While
		Return length
	End Function


	Public Function JSONEscapeCharacter(c As Char) As Char ()
		Dim code As Double
		Dim escaped As Char ()
		Dim hexNumber As StringReference
		Dim q, rs, s, b, f, n, r, t As Double

		code = Convert.ToInt16(c)

		q = 34
		rs = 92
		s = 47
		b = 8
		f = 12
		n = 10
		r = 13
		t = 9

		hexNumber = New StringReference()

		If code = q
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = """"C
		ElseIf code = rs
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "\"C
		ElseIf code = s
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "/"C
		ElseIf code = b
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "b"C
		ElseIf code = f
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "f"C
		ElseIf code = n
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "n"C
		ElseIf code = r
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "r"C
		ElseIf code = t
			escaped = New Char (2 - 1){}
			escaped(0) = "\"C
			escaped(1) = "t"C
		ElseIf code >= 0 And code <= 31
			escaped = New Char (6 - 1){}
			escaped(0) = "\"C
			escaped(1) = "u"C
			escaped(2) = "0"C
			escaped(3) = "0"C

			nCreateStringFromNumberWithCheck(code, 16, hexNumber)

			If hexNumber.stringx.Length = 1
				escaped(4) = "0"C
				escaped(5) = hexNumber.stringx(0)
			ElseIf hexNumber.stringx.Length = 2
				escaped(4) = hexNumber.stringx(0)
				escaped(5) = hexNumber.stringx(1)
			End If
		Else
			escaped = New Char (1 - 1){}
			escaped(0) = c
		End If

		Return escaped
	End Function


	Public Function JSONMustBeEscaped(c As Char, ByRef letters As NumberReference) As Boolean
		Dim code As Double
		Dim mustBeEscaped As Boolean
		Dim q, rs, s, b, f, n, r, t As Double

		code = Convert.ToInt16(c)
		mustBeEscaped = false

		q = 34
		rs = 92
		s = 47
		b = 8
		f = 12
		n = 10
		r = 13
		t = 9

		If code = q Or code = rs Or code = s Or code = b Or code = f Or code = n Or code = r Or code = t
			mustBeEscaped = true
			letters.numberValue = 2
		ElseIf code >= 0 And code <= 31
			mustBeEscaped = true
			letters.numberValue = 6
		ElseIf code >= 2 ^ 16
			mustBeEscaped = true
			letters.numberValue = 6
		End If

		Return mustBeEscaped
	End Function


	Public Sub WriteObject(ByRef element As Element, ByRef result As Char (), ByRef index As NumberReference)
		Dim s, key As Char ()
		Dim i As Double
		Dim keys As StringArrayReference
		Dim objectElement As Element

		Call WriteStringToStingStream(result, index, "{".ToCharArray())

		keys = GetStringElementMapKeySet(element.objectx)
		i = 0
		While i < keys.stringArray.Length
			key = keys.stringArray(i).stringx
			key = JSONEscapeString(key)
			objectElement = GetObjectValue(element.objectx, key)

			Call WriteStringToStingStream(result, index, """".ToCharArray())
			Call WriteStringToStingStream(result, index, key)
			Call WriteStringToStingStream(result, index, """".ToCharArray())
			Call WriteStringToStingStream(result, index, ":".ToCharArray())

			s = WriteJSON(objectElement)
			Call WriteStringToStingStream(result, index, s)

			If i + 1 <> keys.stringArray.Length
				Call WriteStringToStingStream(result, index, ",".ToCharArray())
			End If
			i = i + 1
		End While

		Call WriteStringToStingStream(result, index, "}".ToCharArray())
	End Sub


	Public Function ReadJSON(ByRef stringx As Char (), ByRef elementReference As ElementReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim tokenArrayReference As StringArrayReference
		Dim success As Boolean

		' Tokenize.
		tokenArrayReference = New StringArrayReference()
		success = JSONTokenize(stringx, tokenArrayReference, errorMessages)

		If success
			' Parse.
			success = GetJSONValue(tokenArrayReference.stringArray, elementReference, errorMessages)
		End If

		Return success
	End Function


	Public Function GetJSONValue(ByRef tokens As StringReference (), ByRef elementReference As ElementReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim i As NumberReference
		Dim success As Boolean

		i = CreateNumberReference(0)
		success = GetJSONValueRecursive(tokens, i, 0, elementReference, errorMessages)

		Return success
	End Function


	Public Function GetJSONValueRecursive(ByRef tokens As StringReference (), ByRef i As NumberReference, depth As Double, ByRef elementReference As ElementReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim str, substr, token As Char ()
		Dim stringToDecimalResult As Double
		Dim success As Boolean

		success = true
		token = tokens(i.numberValue).stringx

		If StringsEqual(token, "{".ToCharArray())
			success = GetJSONObject(tokens, i, depth + 1, elementReference, errorMessages)
		ElseIf StringsEqual(token, "[".ToCharArray())
			success = GetJSONArray(tokens, i, depth + 1, elementReference, errorMessages)
		ElseIf StringsEqual(token, "true".ToCharArray())
			elementReference.element = CreateBooleanElement(true)
			i.numberValue = i.numberValue + 1
		ElseIf StringsEqual(token, "false".ToCharArray())
			elementReference.element = CreateBooleanElement(false)
			i.numberValue = i.numberValue + 1
		ElseIf StringsEqual(token, "null".ToCharArray())
			elementReference.element = CreateNullElement()
			i.numberValue = i.numberValue + 1
		ElseIf charIsNumber(token(0)) Or token(0) = "-"C
			stringToDecimalResult = nCreateNumberFromDecimalString(token)
			elementReference.element = CreateNumberElement(stringToDecimalResult)
			i.numberValue = i.numberValue + 1
		ElseIf token(0) = """"C
			substr = Substring(token, 1, token.Length - 1)
			elementReference.element = CreateStringElement(substr)
			i.numberValue = i.numberValue + 1
		Else
			str = "".ToCharArray()
			str = ConcatenateString(str, "Invalid token first in value: ".ToCharArray())
			str = AppendString(str, token)
			Call AddStringRef(errorMessages, CreateStringReference(str))
			success = false
		End If

		If success And depth = 0
			If StringsEqual(tokens(i.numberValue).stringx, "<end>".ToCharArray())
			Else
				Call AddStringRef(errorMessages, CreateStringReference("The outer value cannot have any tokens following it.".ToCharArray()))
				success = false
			End If
		End If

		Return success
	End Function


	Public Function GetJSONObject(ByRef tokens As StringReference (), ByRef i As NumberReference, depth As Double, ByRef elementReference As ElementReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim element, value As Element
		Dim done, success As Boolean
		Dim key, colon, comma, closeCurly As Char ()
		Dim keystring, str As Char ()
		Dim valueReference As ElementReference
		Dim values As LinkedListElements
		Dim keys As LinkedListStrings

		keys = CreateLinkedListString()
		values = CreateLinkedListElements()
		element = CreateObjectElement(0)
		valueReference = New ElementReference()
		success = true
		i.numberValue = i.numberValue + 1

		If Not StringsEqual(tokens(i.numberValue).stringx, "}".ToCharArray())
			done = false

			
			While Not done And success
				key = tokens(i.numberValue).stringx

				If key(0) = """"C
					i.numberValue = i.numberValue + 1
					colon = tokens(i.numberValue).stringx
					If StringsEqual(colon, ":".ToCharArray())
						i.numberValue = i.numberValue + 1
						success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages)

						If success
							keystring = Substring(key, 1, key.Length - 1)
							value = valueReference.element
							Call LinkedListAddString(keys, keystring)
							Call LinkedListAddElement(values, value)

							comma = tokens(i.numberValue).stringx
							If StringsEqual(comma, ",".ToCharArray())
								' OK
								i.numberValue = i.numberValue + 1
							Else
								done = true
							End If
						End If
					Else
						str = "".ToCharArray()
						str = ConcatenateString(str, "Expected colon after key in object: ".ToCharArray())
						str = AppendString(str, colon)
						Call AddStringRef(errorMessages, CreateStringReference(str))

						success = false
						done = true
					End If
				Else
					Call AddStringRef(errorMessages, CreateStringReference("Expected string as key in object.".ToCharArray()))

					done = true
					success = false
				End If
			End While
		End If

		If success
			closeCurly = tokens(i.numberValue).stringx

			If StringsEqual(closeCurly, "}".ToCharArray())
				' OK
				Erase element.objectx.stringListRef.stringArray 
				Erase element.objectx.elementListRef.arrayx 
				element.objectx.stringListRef.stringArray = LinkedListStringsToArray(keys)
				element.objectx.elementListRef.arrayx = LinkedListElementsToArray(values)
				elementReference.element = element
				i.numberValue = i.numberValue + 1
			Else
				Call AddStringRef(errorMessages, CreateStringReference("Expected close curly brackets at end of object value.".ToCharArray()))
				success = false
			End If
		End If

		Call FreeLinkedListString(keys)
		Call FreeLinkedListElements(values)
		valueReference = Nothing

		Return success
	End Function


	Public Function GetJSONArray(ByRef tokens As StringReference (), ByRef i As NumberReference, depth As Double, ByRef elementReference As ElementReference, ByRef errorMessages As StringArrayReference) As Boolean
		Dim element, value As Element
		Dim nextToken, comma As Char ()
		Dim done, success As Boolean
		Dim valueReference As ElementReference
		Dim elements As LinkedListElements

		elements = CreateLinkedListElements()
		i.numberValue = i.numberValue + 1

		valueReference = New ElementReference()
		success = true
		element = CreateArrayElement(0)

		nextToken = tokens(i.numberValue).stringx

		If Not StringsEqual(nextToken, "]".ToCharArray())
			done = false
			
			While Not done And success
				success = GetJSONValueRecursive(tokens, i, depth, valueReference, errorMessages)

				If success
					value = valueReference.element
					Call LinkedListAddElement(elements, value)

					comma = tokens(i.numberValue).stringx

					If StringsEqual(comma, ",".ToCharArray())
						' OK
						i.numberValue = i.numberValue + 1
					Else
						done = true
					End If
				End If
			End While
		End If

		nextToken = tokens(i.numberValue).stringx
		If StringsEqual(nextToken, "]".ToCharArray())
			' OK
			i.numberValue = i.numberValue + 1
			Erase element.arrayx 
			element.arrayx = LinkedListElementsToArray(elements)
		Else
			Call AddStringRef(errorMessages, CreateStringReference("Expected close square bracket at end of array.".ToCharArray()))
			success = false
		End If

		elementReference.element = element
		Call FreeLinkedListElements(elements)
		valueReference = Nothing

		Return success
	End Function


	Public Function GetStringElementMapKeySet(ByRef stringElementMap As StringElementMap) As StringArrayReference
		Return stringElementMap.stringListRef
	End Function


	Public Function GetObjectValue(ByRef stringElementMap As StringElementMap, ByRef key As Char ()) As Element
		Dim result As Element
		Dim i As Double

		result = New Element()

		i = 0
		While i < GetStringElementMapNumberOfKeys(stringElementMap)
			If StringsEqual(stringElementMap.stringListRef.stringArray(i).stringx, key)
				result = stringElementMap.elementListRef.arrayx(i)
			End If
			i = i + 1
		End While

		Return result
	End Function


	Public Function GetObjectValueWithCheck(ByRef stringElementMap As StringElementMap, ByRef key As Char (), ByRef foundReference As BooleanReference) As Element
		Dim result As Element
		Dim i As Double

		result = New Element()

		foundReference.booleanValue = false
		i = 0
		While i < GetStringElementMapNumberOfKeys(stringElementMap)
			If StringsEqual(stringElementMap.stringListRef.stringArray(i).stringx, key)
				result = stringElementMap.elementListRef.arrayx(i)
				foundReference.booleanValue = true
			End If
			i = i + 1
		End While

		Return result
	End Function


	Public Sub PutStringElementMap(ByRef stringElementMap As StringElementMap, ByRef keystring As Char (), ByRef value As Element)
		Call AddStringRef(stringElementMap.stringListRef, CreateStringReference(keystring))
		Call AddElementRef(stringElementMap.elementListRef, value)
	End Sub


	Public Sub SetStringElementMap(ByRef stringElementMap As StringElementMap, index As Double, ByRef keystring As Char (), ByRef value As Element)
		stringElementMap.stringListRef.stringArray(index).stringx = keystring
		stringElementMap.elementListRef.arrayx(index) = value
	End Sub


	Public Function GetStringElementMapNumberOfKeys(ByRef stringElementMap As StringElementMap) As Double
		Return stringElementMap.stringListRef.stringArray.Length
	End Function


	Public Function JSONCompare(ByRef a As Char (), ByRef b As Char (), epsilon As Double, ByRef equal As BooleanReference, ByRef errorMessage As StringArrayReference) As Boolean
		Dim success As Boolean
		Dim eaRef, ebRef As ElementReference
		Dim ea, eb As Element

		eaRef = New ElementReference()
		ebRef = New ElementReference()

		success = ReadJSON(a, eaRef, errorMessage)

		If success
			ea = eaRef.element

			success = ReadJSON(b, ebRef, errorMessage)

			If success
				eb = ebRef.element

				equal.booleanValue = JSONCompareElements(ea, eb, epsilon)

				Call DeleteElement(eb)
			End If

			Call DeleteElement(ea)
		End If

		Return success
	End Function


	Public Function JSONCompareElements(ByRef ea As Element, ByRef eb As Element, epsilon As Double) As Boolean
		Dim equal As Boolean
		Dim typeName As Char ()

		equal = StringsEqual(ea.type, eb.type)
        
		If equal
			typeName = ea.type
			If StringsEqual(typeName, "object".ToCharArray())
				equal = JSONCompareObjects(ea.objectx, eb.objectx, epsilon)
			ElseIf StringsEqual(typeName, "string".ToCharArray())
				equal = StringsEqual(ea.stringx, eb.stringx)
			ElseIf StringsEqual(typeName, "array".ToCharArray())
				equal = JSONCompareArrays(ea.arrayx, eb.arrayx, epsilon)
			ElseIf StringsEqual(typeName, "number".ToCharArray())
				equal = EpsilonCompare(ea.number, eb.number, epsilon)
			ElseIf StringsEqual(typeName, "null".ToCharArray())
				equal = true
			ElseIf StringsEqual(typeName, "boolean".ToCharArray())
				equal = ea.booleanValue = eb.booleanValue
			End If
		End If
        
		Return equal
	End Function


	Public Function JSONCompareArrays(ByRef ea As Element (), ByRef eb As Element (), epsilon As Double) As Boolean
		Dim equals As Boolean
		Dim i, length As Double

		equals = ea.Length = eb.Length

		If equals
			length = ea.Length
			i = 0
			While i < length And equals
				equals = JSONCompareElements(ea(i), eb(i), epsilon)
				i = i + 1
			End While
		End If

		Return equals
	End Function


	Public Function JSONCompareObjects(ByRef ea As StringElementMap, ByRef eb As StringElementMap, epsilon As Double) As Boolean
		Dim equals As Boolean
		Dim akeys, bkeys, i As Double
		Dim keys As StringArrayReference
		Dim key As Char ()
		Dim aFoundReference, bFoundReference As BooleanReference
		Dim eaValue, ebValue As Element

		aFoundReference = New BooleanReference()
		bFoundReference = New BooleanReference()

		akeys = GetStringElementMapNumberOfKeys(ea)
		bkeys = GetStringElementMapNumberOfKeys(eb)

		equals = akeys = bkeys

		If equals
			keys = GetStringElementMapKeySet(ea)

			i = 0
			While i < keys.stringArray.Length And equals
				key = keys.stringArray(i).stringx

				eaValue = GetObjectValueWithCheck(ea, key, aFoundReference)
				ebValue = GetObjectValueWithCheck(eb, key, bFoundReference)

				If aFoundReference.booleanValue And bFoundReference.booleanValue
					equals = JSONCompareElements(eaValue, ebValue, epsilon)
				Else
					equals = false
				End If
				i = i + 1
			End While
		End If

		Return equals
	End Function


	Public Function AddNumber(ByRef list As Double (), a As Double) As Double ()
		Dim newlist As Double ()
		Dim i As Double

		newlist = New Double (list.Length + 1 - 1){}
		i = 0
		While i < list.Length
			newlist(i) = list(i)
			i = i + 1
		End While
		newlist(list.Length) = a
		
		Erase list 
		
		Return newlist
	End Function


	Public Sub AddNumberRef(ByRef list As NumberArrayReference, i As Double)
		list.numberArray = AddNumber(list.numberArray, i)
	End Sub


	Public Function RemoveNumber(ByRef list As Double (), n As Double) As Double ()
		Dim newlist As Double ()
		Dim i As Double

		newlist = New Double (list.Length - 1 - 1){}

		If n >= 0 And n < list.Length
			i = 0
			While i < list.Length
				If i < n
					newlist(i) = list(i)
				End If
				If i > n
					newlist(i - 1) = list(i)
				End If
				i = i + 1
			End While

			Erase list 
		Else
			Erase newlist 
		End If
		
		Return newlist
	End Function


	Public Function GetNumberRef(ByRef list As NumberArrayReference, i As Double) As Double
		Return list.numberArray(i)
	End Function


	Public Sub RemoveNumberRef(ByRef list As NumberArrayReference, i As Double)
		list.numberArray = RemoveNumber(list.numberArray, i)
	End Sub


	Public Function AddString(ByRef list As StringReference (), ByRef a As StringReference) As StringReference ()
		Dim newlist As StringReference ()
		Dim i As Double

		newlist = New StringReference (list.Length + 1 - 1){}

		i = 0
		While i < list.Length
			newlist(i) = list(i)
			i = i + 1
		End While
		newlist(list.Length) = a
		
		Erase list 
		
		Return newlist
	End Function


	Public Sub AddStringRef(ByRef list As StringArrayReference, ByRef i As StringReference)
		list.stringArray = AddString(list.stringArray, i)
	End Sub


	Public Function RemoveString(ByRef list As StringReference (), n As Double) As StringReference ()
		Dim newlist As StringReference ()
		Dim i As Double

		newlist = New StringReference (list.Length - 1 - 1){}

		If n >= 0 And n < list.Length
			i = 0
			While i < list.Length
				If i < n
					newlist(i) = list(i)
				End If
				If i > n
					newlist(i - 1) = list(i)
				End If
				i = i + 1
			End While

			Erase list 
		Else
			Erase newlist 
		End If
		
		Return newlist
	End Function


	Public Function GetStringRef(ByRef list As StringArrayReference, i As Double) As StringReference
		Return list.stringArray(i)
	End Function


	Public Sub RemoveStringRef(ByRef list As StringArrayReference, i As Double)
		list.stringArray = RemoveString(list.stringArray, i)
	End Sub


	Public Function CreateDynamicArrayCharacters() As DynamicArrayCharacters
		Dim da As DynamicArrayCharacters

		da = New DynamicArrayCharacters()
		da.arrayx = New Char (10 - 1){}
		da.length = 0

		Return da
	End Function


	Public Function CreateDynamicArrayCharactersWithInitialCapacity(capacity As Double) As DynamicArrayCharacters
		Dim da As DynamicArrayCharacters

		da = New DynamicArrayCharacters()
		da.arrayx = New Char (capacity - 1){}
		da.length = 0

		Return da
	End Function


	Public Sub DynamicArrayAddCharacter(ByRef da As DynamicArrayCharacters, value As Char)
		If da.length = da.arrayx.Length
			Call DynamicArrayCharactersIncreaseSize(da)
		End If

		da.arrayx(da.length) = value
		da.length = da.length + 1
	End Sub


	Public Sub DynamicArrayCharactersIncreaseSize(ByRef da As DynamicArrayCharacters)
		Dim newLength, i As Double
		Dim newArray As Char ()

		newLength = Round(da.arrayx.Length*3/2)
		newArray = New Char (newLength - 1){}

		i = 0
		While i < da.arrayx.Length
			newArray(i) = da.arrayx(i)
			i = i + 1
		End While

		Erase da.arrayx 

		da.arrayx = newArray
	End Sub


	Public Function DynamicArrayCharactersDecreaseSizeNecessary(ByRef da As DynamicArrayCharacters) As Boolean
		Dim needsDecrease As Boolean

		needsDecrease = false

		If da.length > 10
			needsDecrease = da.length <= Round(da.arrayx.Length*2/3)
		End If

		Return needsDecrease
	End Function


	Public Sub DynamicArrayCharactersDecreaseSize(ByRef da As DynamicArrayCharacters)
		Dim newLength, i As Double
		Dim newArray As Char ()

		newLength = Round(da.arrayx.Length*2/3)
		newArray = New Char (newLength - 1){}

		i = 0
		While i < newLength
			newArray(i) = da.arrayx(i)
			i = i + 1
		End While

		Erase da.arrayx 

		da.arrayx = newArray
	End Sub


	Public Function DynamicArrayCharactersLength(ByRef da As DynamicArrayCharacters) As Double
		Return da.length
	End Function


	Public Sub DynamicArrayInsertCharacter(ByRef da As DynamicArrayCharacters, index As Double, value As Char)
		Dim i As Double

		If da.length = da.arrayx.Length
			Call DynamicArrayCharactersIncreaseSize(da)
		End If

		i = da.length
		While i > index
			da.arrayx(i) = da.arrayx(i - 1)
			i = i - 1
		End While

		da.arrayx(index) = value

		da.length = da.length + 1
	End Sub


	Public Function DynamicArrayCharacterSet(ByRef da As DynamicArrayCharacters, index As Double, value As Char) As Boolean
		Dim success As Boolean

		If index < da.length
			da.arrayx(index) = value
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Sub DynamicArrayRemoveCharacter(ByRef da As DynamicArrayCharacters, index As Double)
		Dim i As Double

		i = index
		While i < da.length - 1
			da.arrayx(i) = da.arrayx(i + 1)
			i = i + 1
		End While

		da.length = da.length - 1

		If DynamicArrayCharactersDecreaseSizeNecessary(da)
			Call DynamicArrayCharactersDecreaseSize(da)
		End If
	End Sub


	Public Sub FreeDynamicArrayCharacters(ByRef da As DynamicArrayCharacters)
		Erase da.arrayx 
		da = Nothing
	End Sub


	Public Function DynamicArrayCharactersToArray(ByRef da As DynamicArrayCharacters) As Char ()
		Dim arrayx As Char ()
		Dim i As Double

		arrayx = New Char (da.length - 1){}

		i = 0
		While i < da.length
			arrayx(i) = da.arrayx(i)
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function ArrayToDynamicArrayCharactersWithOptimalSize(ByRef arrayx As Char ()) As DynamicArrayCharacters
		Dim da As DynamicArrayCharacters
		Dim i As Double
		Dim c, n, newCapacity As Double

		c = arrayx.Length
		n = (Log(c) - 1)/Log(3/2)
		newCapacity = Floor(n) + 1

		da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity)

		i = 0
		While i < arrayx.Length
			da.arrayx(i) = arrayx(i)
			i = i + 1
		End While

		Return da
	End Function


	Public Function ArrayToDynamicArrayCharacters(ByRef arrayx As Char ()) As DynamicArrayCharacters
		Dim da As DynamicArrayCharacters

		da = New DynamicArrayCharacters()
		da.arrayx = CopyString(arrayx)
		da.length = arrayx.Length

		Return da
	End Function


	Public Function DynamicArrayCharactersEqual(ByRef a As DynamicArrayCharacters, ByRef b As DynamicArrayCharacters) As Boolean
		Dim equal As Boolean
		Dim i As Double

		equal = true
		If a.length = b.length
			i = 0
			While i < a.length And equal
				If a.arrayx(i) <> b.arrayx(i)
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function DynamicArrayCharactersToLinkedList(ByRef da As DynamicArrayCharacters) As LinkedListCharacters
		Dim ll As LinkedListCharacters
		Dim i As Double

		ll = CreateLinkedListCharacter()

		i = 0
		While i < da.length
			Call LinkedListAddCharacter(ll, da.arrayx(i))
			i = i + 1
		End While

		Return ll
	End Function


	Public Function LinkedListToDynamicArrayCharacters(ByRef ll As LinkedListCharacters) As DynamicArrayCharacters
		Dim da As DynamicArrayCharacters
		Dim i As Double
		Dim node As LinkedListNodeCharacters

		node = ll.first

		da = New DynamicArrayCharacters()
		da.length = LinkedListCharactersLength(ll)

		da.arrayx = New Char (da.length - 1){}

		i = 0
		While i < da.length
			da.arrayx(i) = node.value
			node = node.nextx
			i = i + 1
		End While

		Return da
	End Function


	Public Function AddBoolean(ByRef list As Boolean (), a As Boolean) As Boolean ()
		Dim newlist As Boolean ()
		Dim i As Double

		newlist = New Boolean (list.Length + 1 - 1){}
		i = 0
		While i < list.Length
			newlist(i) = list(i)
			i = i + 1
		End While
		newlist(list.Length) = a
		
		Erase list 
		
		Return newlist
	End Function


	Public Sub AddBooleanRef(ByRef list As BooleanArrayReference, i As Boolean)
		list.booleanArray = AddBoolean(list.booleanArray, i)
	End Sub


	Public Function RemoveBoolean(ByRef list As Boolean (), n As Double) As Boolean ()
		Dim newlist As Boolean ()
		Dim i As Double

		newlist = New Boolean (list.Length - 1 - 1){}

		If n >= 0 And n < list.Length
			i = 0
			While i < list.Length
				If i < n
					newlist(i) = list(i)
				End If
				If i > n
					newlist(i - 1) = list(i)
				End If
				i = i + 1
			End While

			Erase list 
		Else
			Erase newlist 
		End If
		
		Return newlist
	End Function


	Public Function GetBooleanRef(ByRef list As BooleanArrayReference, i As Double) As Boolean
		Return list.booleanArray(i)
	End Function


	Public Sub RemoveDecimalRef(ByRef list As BooleanArrayReference, i As Double)
		list.booleanArray = RemoveBoolean(list.booleanArray, i)
	End Sub


	Public Function CreateLinkedListString() As LinkedListStrings
		Dim ll As LinkedListStrings

		ll = New LinkedListStrings()
		ll.first = New LinkedListNodeStrings()
		ll.last = ll.first
		ll.last.endx = true

		Return ll
	End Function


	Public Sub LinkedListAddString(ByRef ll As LinkedListStrings, ByRef value As Char ())
		ll.last.endx = false
		ll.last.value = value
		ll.last.nextx = New LinkedListNodeStrings()
		ll.last.nextx.endx = true
		ll.last = ll.last.nextx
	End Sub


	Public Function LinkedListStringsToArray(ByRef ll As LinkedListStrings) As StringReference ()
		Dim arrayx As StringReference ()
		Dim length, i As Double
		Dim node As LinkedListNodeStrings

		node = ll.first

		length = LinkedListStringsLength(ll)

		arrayx = New StringReference (length - 1){}

		i = 0
		While i < length
			arrayx(i) = New StringReference()
			arrayx(i).stringx = node.value
			node = node.nextx
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function LinkedListStringsLength(ByRef ll As LinkedListStrings) As Double
		Dim l As Double
		Dim node As LinkedListNodeStrings

		l = 0
		node = ll.first
		
		While Not node.endx
			node = node.nextx
			l = l + 1
		End While

		Return l
	End Function


	Public Sub FreeLinkedListString(ByRef ll As LinkedListStrings)
		Dim node, prev As LinkedListNodeStrings

		node = ll.first

		
		While Not node.endx
			prev = node
			node = node.nextx
			prev = Nothing
		End While

		node = Nothing
	End Sub


	Public Function CreateLinkedListNumbers() As LinkedListNumbers
		Dim ll As LinkedListNumbers

		ll = New LinkedListNumbers()
		ll.first = New LinkedListNodeNumbers()
		ll.last = ll.first
		ll.last.endx = true

		Return ll
	End Function


	Public Function CreateLinkedListNumbersArray(length As Double) As LinkedListNumbers ()
		Dim lls As LinkedListNumbers ()
		Dim i As Double

		lls = New LinkedListNumbers (length - 1){}
		i = 0
		While i < lls.Length
			lls(i) = CreateLinkedListNumbers()
			i = i + 1
		End While

		Return lls
	End Function


	Public Sub LinkedListAddNumber(ByRef ll As LinkedListNumbers, value As Double)
		ll.last.endx = false
		ll.last.value = value
		ll.last.nextx = New LinkedListNodeNumbers()
		ll.last.nextx.endx = true
		ll.last = ll.last.nextx
	End Sub


	Public Function LinkedListNumbersLength(ByRef ll As LinkedListNumbers) As Double
		Dim l As Double
		Dim node As LinkedListNodeNumbers

		l = 0
		node = ll.first
		
		While Not node.endx
			node = node.nextx
			l = l + 1
		End While

		Return l
	End Function


	Public Function LinkedListNumbersIndex(ByRef ll As LinkedListNumbers, index As Double) As Double
		Dim i As Double
		Dim node As LinkedListNodeNumbers

		node = ll.first
		i = 0
		While i < index
			node = node.nextx
			i = i + 1
		End While

		Return node.value
	End Function


	Public Sub LinkedListInsertNumber(ByRef ll As LinkedListNumbers, index As Double, value As Double)
		Dim i As Double
		Dim node, tmp As LinkedListNodeNumbers

		If index = 0
			tmp = ll.first
			ll.first = New LinkedListNodeNumbers()
			ll.first.nextx = tmp
			ll.first.value = value
			ll.first.endx = false
		Else
			node = ll.first
			i = 0
			While i < index - 1
				node = node.nextx
				i = i + 1
			End While

			tmp = node.nextx
			node.nextx = New LinkedListNodeNumbers()
			node.nextx.nextx = tmp
			node.nextx.value = value
			node.nextx.endx = false
		End If
	End Sub


	Public Sub LinkedListSet(ByRef ll As LinkedListNumbers, index As Double, value As Double)
		Dim i As Double
		Dim node As LinkedListNodeNumbers

		node = ll.first
		i = 0
		While i < index
			node = node.nextx
			i = i + 1
		End While

		node.nextx.value = value
	End Sub


	Public Sub LinkedListRemoveNumber(ByRef ll As LinkedListNumbers, index As Double)
		Dim i As Double
		Dim node, prev As LinkedListNodeNumbers

		node = ll.first
		prev = ll.first

		i = 0
		While i < index
			prev = node
			node = node.nextx
			i = i + 1
		End While

		If index = 0
			ll.first = prev.nextx
		End If
		If Not prev.nextx.endx
			prev.nextx = prev.nextx.nextx
		End If
	End Sub


	Public Sub FreeLinkedListNumbers(ByRef ll As LinkedListNumbers)
		Dim node, prev As LinkedListNodeNumbers

		node = ll.first

		
		While Not node.endx
			prev = node
			node = node.nextx
			prev = Nothing
		End While

		node = Nothing
	End Sub


	Public Sub FreeLinkedListNumbersArray(ByRef lls As LinkedListNumbers ())
		Dim i As Double

		i = 0
		While i < lls.Length
			Call FreeLinkedListNumbers(lls(i))
			i = i + 1
		End While
		Erase lls 
	End Sub


	Public Function LinkedListNumbersToArray(ByRef ll As LinkedListNumbers) As Double ()
		Dim arrayx As Double ()
		Dim length, i As Double
		Dim node As LinkedListNodeNumbers

		node = ll.first

		length = LinkedListNumbersLength(ll)

		arrayx = New Double (length - 1){}

		i = 0
		While i < length
			arrayx(i) = node.value
			node = node.nextx
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function ArrayToLinkedListNumbers(ByRef arrayx As Double ()) As LinkedListNumbers
		Dim ll As LinkedListNumbers
		Dim i As Double

		ll = CreateLinkedListNumbers()

		i = 0
		While i < arrayx.Length
			Call LinkedListAddNumber(ll, arrayx(i))
			i = i + 1
		End While

		Return ll
	End Function


	Public Function LinkedListNumbersEqual(ByRef a As LinkedListNumbers, ByRef b As LinkedListNumbers) As Boolean
		Dim equal, done As Boolean
		Dim an, bn As LinkedListNodeNumbers

		an = a.first
		bn = b.first

		equal = true
		done = false
		
		While equal And Not done
			If an.endx = bn.endx
				If an.endx
					done = true
				ElseIf an.value = bn.value
					an = an.nextx
					bn = bn.nextx
				Else
					equal = false
				End If
			Else
				equal = false
			End If
		End While

		Return equal
	End Function


	Public Function CreateLinkedListCharacter() As LinkedListCharacters
		Dim ll As LinkedListCharacters

		ll = New LinkedListCharacters()
		ll.first = New LinkedListNodeCharacters()
		ll.last = ll.first
		ll.last.endx = true

		Return ll
	End Function


	Public Sub LinkedListAddCharacter(ByRef ll As LinkedListCharacters, value As Char)
		ll.last.endx = false
		ll.last.value = value
		ll.last.nextx = New LinkedListNodeCharacters()
		ll.last.nextx.endx = true
		ll.last = ll.last.nextx
	End Sub


	Public Function LinkedListCharactersToArray(ByRef ll As LinkedListCharacters) As Char ()
		Dim arrayx As Char ()
		Dim length, i As Double
		Dim node As LinkedListNodeCharacters

		node = ll.first

		length = LinkedListCharactersLength(ll)

		arrayx = New Char (length - 1){}

		i = 0
		While i < length
			arrayx(i) = node.value
			node = node.nextx
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function LinkedListCharactersLength(ByRef ll As LinkedListCharacters) As Double
		Dim l As Double
		Dim node As LinkedListNodeCharacters

		l = 0
		node = ll.first
		
		While Not node.endx
			node = node.nextx
			l = l + 1
		End While

		Return l
	End Function


	Public Sub FreeLinkedListCharacter(ByRef ll As LinkedListCharacters)
		Dim node, prev As LinkedListNodeCharacters

		node = ll.first

		
		While Not node.endx
			prev = node
			node = node.nextx
			prev = Nothing
		End While

		node = Nothing
	End Sub


	Public Sub LinkedListCharactersAddString(ByRef ll As LinkedListCharacters, ByRef str As Char ())
		Dim i As Double

		i = 0
		While i < str.Length
			Call LinkedListAddCharacter(ll, str(i))
			i = i + 1
		End While
	End Sub


	Public Function CreateDynamicArrayNumbers() As DynamicArrayNumbers
		Dim da As DynamicArrayNumbers

		da = New DynamicArrayNumbers()
		da.arrayx = New Double (10 - 1){}
		da.length = 0

		Return da
	End Function


	Public Function CreateDynamicArrayNumbersWithInitialCapacity(capacity As Double) As DynamicArrayNumbers
		Dim da As DynamicArrayNumbers

		da = New DynamicArrayNumbers()
		da.arrayx = New Double (capacity - 1){}
		da.length = 0

		Return da
	End Function


	Public Sub DynamicArrayAddNumber(ByRef da As DynamicArrayNumbers, value As Double)
		If da.length = da.arrayx.Length
			Call DynamicArrayNumbersIncreaseSize(da)
		End If

		da.arrayx(da.length) = value
		da.length = da.length + 1
	End Sub


	Public Sub DynamicArrayNumbersIncreaseSize(ByRef da As DynamicArrayNumbers)
		Dim newLength, i As Double
		Dim newArray As Double ()

		newLength = Round(da.arrayx.Length*3/2)
		newArray = New Double (newLength - 1){}

		i = 0
		While i < da.arrayx.Length
			newArray(i) = da.arrayx(i)
			i = i + 1
		End While

		Erase da.arrayx 

		da.arrayx = newArray
	End Sub


	Public Function DynamicArrayNumbersDecreaseSizeNecessary(ByRef da As DynamicArrayNumbers) As Boolean
		Dim needsDecrease As Boolean

		needsDecrease = false

		If da.length > 10
			needsDecrease = da.length <= Round(da.arrayx.Length*2/3)
		End If

		Return needsDecrease
	End Function


	Public Sub DynamicArrayNumbersDecreaseSize(ByRef da As DynamicArrayNumbers)
		Dim newLength, i As Double
		Dim newArray As Double ()

		newLength = Round(da.arrayx.Length*2/3)
		newArray = New Double (newLength - 1){}

		i = 0
		While i < newLength
			newArray(i) = da.arrayx(i)
			i = i + 1
		End While

		Erase da.arrayx 

		da.arrayx = newArray
	End Sub


	Public Function DynamicArrayNumbersIndex(ByRef da As DynamicArrayNumbers, index As Double) As Double
		Return da.arrayx(index)
	End Function


	Public Function DynamicArrayNumbersLength(ByRef da As DynamicArrayNumbers) As Double
		Return da.length
	End Function


	Public Sub DynamicArrayInsertNumber(ByRef da As DynamicArrayNumbers, index As Double, value As Double)
		Dim i As Double

		If da.length = da.arrayx.Length
			Call DynamicArrayNumbersIncreaseSize(da)
		End If

		i = da.length
		While i > index
			da.arrayx(i) = da.arrayx(i - 1)
			i = i - 1
		End While

		da.arrayx(index) = value

		da.length = da.length + 1
	End Sub


	Public Function DynamicArrayNumberSet(ByRef da As DynamicArrayNumbers, index As Double, value As Double) As Boolean
		Dim success As Boolean

		If index < da.length
			da.arrayx(index) = value
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Sub DynamicArrayRemoveNumber(ByRef da As DynamicArrayNumbers, index As Double)
		Dim i As Double

		i = index
		While i < da.length - 1
			da.arrayx(i) = da.arrayx(i + 1)
			i = i + 1
		End While

		da.length = da.length - 1

		If DynamicArrayNumbersDecreaseSizeNecessary(da)
			Call DynamicArrayNumbersDecreaseSize(da)
		End If
	End Sub


	Public Sub FreeDynamicArrayNumbers(ByRef da As DynamicArrayNumbers)
		Erase da.arrayx 
		da = Nothing
	End Sub


	Public Function DynamicArrayNumbersToArray(ByRef da As DynamicArrayNumbers) As Double ()
		Dim arrayx As Double ()
		Dim i As Double

		arrayx = New Double (da.length - 1){}

		i = 0
		While i < da.length
			arrayx(i) = da.arrayx(i)
			i = i + 1
		End While

		Return arrayx
	End Function


	Public Function ArrayToDynamicArrayNumbersWithOptimalSize(ByRef arrayx As Double ()) As DynamicArrayNumbers
		Dim da As DynamicArrayNumbers
		Dim i As Double
		Dim c, n, newCapacity As Double

		'
		'         c = 10*(3/2)^n
		'         log(c) = log(10*(3/2)^n)
		'         log(c) = log(10) + log((3/2)^n)
		'         log(c) = 1 + log((3/2)^n)
		'         log(c) - 1 = log((3/2)^n)
		'         log(c) - 1 = n*log(3/2)
		'         n = (log(c) - 1)/log(3/2)
		'        
		c = arrayx.Length
		n = (Log(c) - 1)/Log(3/2)
		newCapacity = Floor(n) + 1

		da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity)

		i = 0
		While i < arrayx.Length
			da.arrayx(i) = arrayx(i)
			i = i + 1
		End While

		Return da
	End Function


	Public Function ArrayToDynamicArrayNumbers(ByRef arrayx As Double ()) As DynamicArrayNumbers
		Dim da As DynamicArrayNumbers

		da = New DynamicArrayNumbers()
		da.arrayx = CopyNumberArray(arrayx)
		da.length = arrayx.Length

		Return da
	End Function


	Public Function DynamicArrayNumbersEqual(ByRef a As DynamicArrayNumbers, ByRef b As DynamicArrayNumbers) As Boolean
		Dim equal As Boolean
		Dim i As Double

		equal = true
		If a.length = b.length
			i = 0
			While i < a.length And equal
				If a.arrayx(i) <> b.arrayx(i)
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function DynamicArrayNumbersToLinkedList(ByRef da As DynamicArrayNumbers) As LinkedListNumbers
		Dim ll As LinkedListNumbers
		Dim i As Double

		ll = CreateLinkedListNumbers()

		i = 0
		While i < da.length
			Call LinkedListAddNumber(ll, da.arrayx(i))
			i = i + 1
		End While

		Return ll
	End Function


	Public Function LinkedListToDynamicArrayNumbers(ByRef ll As LinkedListNumbers) As DynamicArrayNumbers
		Dim da As DynamicArrayNumbers
		Dim i As Double
		Dim node As LinkedListNodeNumbers

		node = ll.first

		da = New DynamicArrayNumbers()
		da.length = LinkedListNumbersLength(ll)

		da.arrayx = New Double (da.length - 1){}

		i = 0
		While i < da.length
			da.arrayx(i) = node.value
			node = node.nextx
			i = i + 1
		End While

		Return da
	End Function


	Public Function DynamicArrayNumbersIndexOf(ByRef arr As DynamicArrayNumbers, n As Double, ByRef foundReference As BooleanReference) As Double
		Dim found As Boolean
		Dim i As Double

		found = false
		i = 0
		While i < arr.length And Not found
			If arr.arrayx(i) = n
				found = true
			End If
			i = i + 1
		End While
		If Not found
			i = -1
		Else
			i = i - 1
		End If

		foundReference.booleanValue = found

		Return i
	End Function


	Public Function DynamicArrayNumbersIsInArray(ByRef arr As DynamicArrayNumbers, n As Double) As Boolean
		Dim found As Boolean
		Dim i As Double

		found = false
		i = 0
		While i < arr.length And Not found
			If arr.arrayx(i) = n
				found = true
			End If
			i = i + 1
		End While

		Return found
	End Function


	Public Function AddCharacter(ByRef list As Char (), a As Char) As Char ()
		Dim newlist As Char ()
		Dim i As Double

		newlist = New Char (list.Length + 1 - 1){}
		i = 0
		While i < list.Length
			newlist(i) = list(i)
			i = i + 1
		End While
		newlist(list.Length) = a
		
		Erase list 
		
		Return newlist
	End Function


	Public Sub AddCharacterRef(ByRef list As StringReference, i As Char)
		list.stringx = AddCharacter(list.stringx, i)
	End Sub


	Public Function RemoveCharacter(ByRef list As Char (), n As Double) As Char ()
		Dim newlist As Char ()
		Dim i As Double

		newlist = New Char (list.Length - 1 - 1){}

		If n >= 0 And n < list.Length
			i = 0
			While i < list.Length
				If i < n
					newlist(i) = list(i)
				End If
				If i > n
					newlist(i - 1) = list(i)
				End If
				i = i + 1
			End While

			Erase list 
		Else
			Erase newlist 
		End If

		Return newlist
	End Function


	Public Function GetCharacterRef(ByRef list As StringReference, i As Double) As Char
		Return list.stringx(i)
	End Function


	Public Sub RemoveCharacterRef(ByRef list As StringReference, i As Double)
		list.stringx = RemoveCharacter(list.stringx, i)
	End Sub


	Public Sub WriteStringToStingStream(ByRef stream As Char (), ByRef index As NumberReference, ByRef src As Char ())
		Dim i As Double

		i = 0
		While i < src.Length
			stream(index.numberValue + i) = src(i)
			i = i + 1
		End While
		index.numberValue = index.numberValue + src.Length
	End Sub


	Public Sub WriteCharacterToStingStream(ByRef stream As Char (), ByRef index As NumberReference, src As Char)
		stream(index.numberValue) = src
		index.numberValue = index.numberValue + 1
	End Sub


	Public Sub WriteBooleanToStingStream(ByRef stream As Char (), ByRef index As NumberReference, src As Boolean)
		If src
			Call WriteStringToStingStream(stream, index, "true".ToCharArray())
		Else
			Call WriteStringToStingStream(stream, index, "false".ToCharArray())
		End If
	End Sub


	Public Function SubstringWithCheck(ByRef stringx As Char (), from As Double, tox As Double, ByRef stringReference As StringReference) As Boolean
		Dim success As Boolean

		If from >= 0 And from <= stringx.Length And tox >= 0 And tox <= stringx.Length And from <= tox
			stringReference.stringx = Substring(stringx, from, tox)
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function Substring(ByRef stringx As Char (), from As Double, tox As Double) As Char ()
		Dim n As Char ()
		Dim i, length As Double

		length = tox - from

		n = New Char (length - 1){}

		i = from
		While i < tox
			n(i - from) = stringx(i)
			i = i + 1
		End While

		Return n
	End Function


	Public Function AppendString(ByRef s1 As Char (), ByRef s2 As Char ()) As Char ()
		Dim newString As Char ()

		newString = ConcatenateString(s1, s2)

		Erase s1 

		Return newString
	End Function


	Public Function ConcatenateString(ByRef s1 As Char (), ByRef s2 As Char ()) As Char ()
		Dim newString As Char ()
		Dim i As Double

		newString = New Char (s1.Length + s2.Length - 1){}

		i = 0
		While i < s1.Length
			newString(i) = s1(i)
			i = i + 1
		End While

		i = 0
		While i < s2.Length
			newString(s1.Length + i) = s2(i)
			i = i + 1
		End While

		Return newString
	End Function


	Public Function AppendCharacter(ByRef stringx As Char (), c As Char) As Char ()
		Dim newString As Char ()

		newString = ConcatenateCharacter(stringx, c)

		Erase stringx 

		Return newString
	End Function


	Public Function ConcatenateCharacter(ByRef stringx As Char (), c As Char) As Char ()
		Dim newString As Char ()
		Dim i As Double
		newString = New Char (stringx.Length + 1 - 1){}

		i = 0
		While i < stringx.Length
			newString(i) = stringx(i)
			i = i + 1
		End While

		newString(stringx.Length) = c

		Return newString
	End Function


	Public Function SplitByCharacter(ByRef toSplit As Char (), splitBy As Char) As StringReference ()
		Dim split As StringReference ()
		Dim stringToSplitBy As Char ()

		stringToSplitBy = New Char (1 - 1){}
		stringToSplitBy(0) = splitBy

		split = SplitByString(toSplit, stringToSplitBy)

		Erase stringToSplitBy 

		Return split
	End Function


	Public Function IndexOfCharacter(ByRef stringx As Char (), character As Char, ByRef indexReference As NumberReference) As Boolean
		Dim i As Double
		Dim found As Boolean

		found = false
		i = 0
		While i < stringx.Length And Not found
			If stringx(i) = character
				found = true
				indexReference.numberValue = i
			End If
			i = i + 1
		End While

		Return found
	End Function


	Public Function SubstringEqualsWithCheck(ByRef stringx As Char (), from As Double, ByRef substring As Char (), ByRef equalsReference As BooleanReference) As Boolean
		Dim success As Boolean

		If from < stringx.Length
			success = true
			equalsReference.booleanValue = SubstringEquals(stringx, from, substring)
		Else
			success = false
		End If

		Return success
	End Function


	Public Function SubstringEquals(ByRef stringx As Char (), from As Double, ByRef substring As Char ()) As Boolean
		Dim i As Double
		Dim equal As Boolean

		equal = true
		If stringx.Length - from >= substring.Length
			i = 0
			While i < substring.Length And equal
				If stringx(from + i) <> substring(i)
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function IndexOfString(ByRef stringx As Char (), ByRef substring As Char (), ByRef indexReference As NumberReference) As Boolean
		Dim i As Double
		Dim found As Boolean

		found = false
		i = 0
		While i < stringx.Length - substring.Length + 1 And Not found
			If SubstringEquals(stringx, i, substring)
				found = true
				indexReference.numberValue = i
			End If
			i = i + 1
		End While

		Return found
	End Function


	Public Function ContainsCharacter(ByRef stringx As Char (), character As Char) As Boolean
		Dim i As Double
		Dim found As Boolean

		found = false
		i = 0
		While i < stringx.Length And Not found
			If stringx(i) = character
				found = true
			End If
			i = i + 1
		End While

		Return found
	End Function


	Public Function ContainsString(ByRef stringx As Char (), ByRef substring As Char ()) As Boolean
		Return IndexOfString(stringx, substring, New NumberReference())
	End Function


	Public Sub ToUpperCase(ByRef stringx As Char ())
		Dim i As Double

		i = 0
		While i < stringx.Length
			stringx(i) = charToUpperCase(stringx(i))
			i = i + 1
		End While
	End Sub


	Public Sub ToLowerCase(ByRef stringx As Char ())
		Dim i As Double

		i = 0
		While i < stringx.Length
			stringx(i) = charToLowerCase(stringx(i))
			i = i + 1
		End While
	End Sub


	Public Function EqualsIgnoreCase(ByRef a As Char (), ByRef b As Char ()) As Boolean
		Dim equal As Boolean
		Dim i As Double

		If a.Length = b.Length
			equal = true
			i = 0
			While i < a.Length And equal
				If charToLowerCase(a(i)) <> charToLowerCase(b(i))
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function ReplaceString(ByRef stringx As Char (), ByRef toReplace As Char (), ByRef replaceWith As Char ()) As Char ()
		Dim result As Char ()
		Dim i, j As Double
		Dim equalsReference As BooleanReference
		Dim success As Boolean
		Dim da As DynamicArrayCharacters

		da = CreateDynamicArrayCharacters()

		equalsReference = New BooleanReference()

		i = 0
		While i < stringx.Length
			success = SubstringEqualsWithCheck(stringx, i, toReplace, equalsReference)
			If success
				success = equalsReference.booleanValue
			End If

			If success And toReplace.Length > 0
				j = 0
				While j < replaceWith.Length
					Call DynamicArrayAddCharacter(da, replaceWith(j))
					j = j + 1
				End While
				i = i + toReplace.Length
			Else
				Call DynamicArrayAddCharacter(da, stringx(i))
				i = i + 1
			End If
		End While

		result = DynamicArrayCharactersToArray(da)

		Call FreeDynamicArrayCharacters(da)

		Return result
	End Function


	Public Function ReplaceCharacterToNew(ByRef stringx As Char (), toReplace As Char, replaceWith As Char) As Char ()
		Dim result As Char ()
		Dim i As Double

		result = New Char (stringx.Length - 1){}

		i = 0
		While i < stringx.Length
			If stringx(i) = toReplace
				result(i) = replaceWith
			Else
				result(i) = stringx(i)
			End If
			i = i + 1
		End While

		Return result
	End Function


	Public Sub ReplaceCharacter(ByRef stringx As Char (), toReplace As Char, replaceWith As Char)
		Dim i As Double

		i = 0
		While i < stringx.Length
			If stringx(i) = toReplace
				stringx(i) = replaceWith
			End If
			i = i + 1
		End While
	End Sub


	Public Function Trim(ByRef stringx As Char ()) As Char ()
		Dim result As Char ()
		Dim i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd As Double
		Dim firstNonWhitespaceFound As Boolean

		' Find whitepaces at the start.
		lastWhitespaceLocationStart = -1
		firstNonWhitespaceFound = false
		i = 0
		While i < stringx.Length And Not firstNonWhitespaceFound
			If charIsWhiteSpace(stringx(i))
				lastWhitespaceLocationStart = i
			Else
				firstNonWhitespaceFound = true
			End If
			i = i + 1
		End While

		' Find whitepaces at the end.
		lastWhitespaceLocationEnd = stringx.Length
		firstNonWhitespaceFound = false
		i = stringx.Length - 1
		While i >= 0 And Not firstNonWhitespaceFound
			If charIsWhiteSpace(stringx(i))
				lastWhitespaceLocationEnd = i
			Else
				firstNonWhitespaceFound = true
			End If
			i = i - 1
		End While

		If lastWhitespaceLocationStart < lastWhitespaceLocationEnd
			result = Substring(stringx, lastWhitespaceLocationStart + 1, lastWhitespaceLocationEnd)
		Else
			result = New Char (0 - 1){}
		End If

		Return result
	End Function


	Public Function StartsWith(ByRef stringx As Char (), ByRef start As Char ()) As Boolean
		Dim startsWithString As Boolean

		startsWithString = false
		If stringx.Length >= start.Length
			startsWithString = SubstringEquals(stringx, 0, start)
		End If

		Return startsWithString
	End Function


	Public Function EndsWith(ByRef stringx As Char (), ByRef endx As Char ()) As Boolean
		Dim endsWithString As Boolean

		endsWithString = false
		If stringx.Length >= endx.Length
			endsWithString = SubstringEquals(stringx, stringx.Length - endx.Length, endx)
		End If

		Return endsWithString
	End Function


	Public Function SplitByString(ByRef toSplit As Char (), ByRef splitBy As Char ()) As StringReference ()
		Dim split As StringReference ()
		Dim nextx As Char ()
		Dim i As Double
		Dim c As Char
		Dim n As StringReference

		split = New StringReference (0 - 1){}

		nextx = New Char (0 - 1){}
		i = 0
		While i < toSplit.Length
			c = toSplit(i)

			If SubstringEquals(toSplit, i, splitBy)
				n = New StringReference()
				n.stringx = nextx
				split = AddString(split, n)
				nextx = New Char (0 - 1){}
				i = i + splitBy.Length
			Else
				nextx = AppendCharacter(nextx, c)
				i = i + 1
			End If
		End While

		n = New StringReference()
		n.stringx = nextx
		split = AddString(split, n)

		Return split
	End Function


	Public Function StringIsBefore(ByRef a As Char (), ByRef b As Char ()) As Boolean
		Dim before, equal, done As Boolean
		Dim i As Double

		before = false
		equal = true
		done = false

		If a.Length = 0 And b.Length > 0
			before = true
		Else
			i = 0
			While i < a.Length And i < b.Length And Not done
				If a(i) <> b(i)
					equal = false
				End If
				If charCharacterIsBefore(a(i), b(i))
					before = true
				End If
				If charCharacterIsBefore(b(i), a(i))
					done = true
				End If
				i = i + 1
			End While

			If equal
				If a.Length < b.Length
					before = true
				End If
			End If
		End If

		Return before
	End Function


	Public Function JoinStringsWithSeparator(ByRef strings As StringReference (), ByRef separator As Char ()) As Char ()
		Dim result, stringx As Char ()
		Dim length, i As Double
		Dim index As NumberReference

		index = CreateNumberReference(0)

		length = 0
		i = 0
		While i < strings.Length
			length = length + strings(i).stringx.Length
			i = i + 1
		End While
		length = length + (strings.Length - 1)*separator.Length

		result = New Char (length - 1){}

		i = 0
		While i < strings.Length
			stringx = strings(i).stringx
			Call WriteStringToStingStream(result, index, stringx)
			If i + 1 < strings.Length
				Call WriteStringToStingStream(result, index, separator)
			End If
			i = i + 1
		End While

		index = Nothing

		Return result
	End Function


	Public Function JoinStrings(ByRef strings As StringReference ()) As Char ()
		Dim result, stringx As Char ()
		Dim length, i As Double
		Dim index As NumberReference

		index = CreateNumberReference(0)

		length = 0
		i = 0
		While i < strings.Length
			length = length + strings(i).stringx.Length
			i = i + 1
		End While

		result = New Char (length - 1){}

		i = 0
		While i < strings.Length
			stringx = strings(i).stringx
			Call WriteStringToStingStream(result, index, stringx)
			i = i + 1
		End While

		index = Nothing

		Return result
	End Function


	Public Function StringToNumberArray(ByRef stringx As Char ()) As Double ()
		Dim i As Double
		Dim arrayx As Double ()

		arrayx = New Double (stringx.Length - 1){}

		i = 0
		While i < stringx.Length
			arrayx(i) = Convert.ToInt16(stringx(i))
			i = i + 1
		End While
		Return arrayx
	End Function


	Public Function NumberArrayToString(ByRef arrayx As Double ()) As Char ()
		Dim i As Double
		Dim stringx As Char ()

		stringx = New Char (arrayx.Length - 1){}

		i = 0
		While i < arrayx.Length
			stringx(i) = Convert.ToChar(Convert.ToInt64(arrayx(i)))
			i = i + 1
		End While
		Return stringx
	End Function


	Public Function NumberArraysEqual(ByRef a As Double (), ByRef b As Double ()) As Boolean
		Dim equal As Boolean
		Dim i As Double

		equal = true
		If a.Length = b.Length
			i = 0
			While i < a.Length And equal
				If a(i) <> b(i)
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function BooleanArraysEqual(ByRef a As Boolean (), ByRef b As Boolean ()) As Boolean
		Dim equal As Boolean
		Dim i As Double

		equal = true
		If a.Length = b.Length
			i = 0
			While i < a.Length And equal
				If a(i) <> b(i)
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Function StringsEqual(ByRef a As Char (), ByRef b As Char ()) As Boolean
		Dim equal As Boolean
		Dim i As Double

		equal = true
		If a.Length = b.Length
			i = 0
			While i < a.Length And equal
				If a(i) <> b(i)
					equal = false
				End If
				i = i + 1
			End While
		Else
			equal = false
		End If

		Return equal
	End Function


	Public Sub FillNumberArray(ByRef a As Double (), value As Double)
		Dim i As Double

		i = 0
		While i < a.Length
			a(i) = value
			i = i + 1
		End While
	End Sub


	Public Sub FillString(ByRef a As Char (), value As Char)
		Dim i As Double

		i = 0
		While i < a.Length
			a(i) = value
			i = i + 1
		End While
	End Sub


	Public Sub FillBooleanArray(ByRef a As Boolean (), value As Boolean)
		Dim i As Double

		i = 0
		While i < a.Length
			a(i) = value
			i = i + 1
		End While
	End Sub


	Public Function FillNumberArrayRange(ByRef a As Double (), value As Double, from As Double, tox As Double) As Boolean
		Dim i, length As Double
		Dim success As Boolean

		If from >= 0 And from <= a.Length And tox >= 0 And tox <= a.Length And from <= tox
			length = tox - from
			i = 0
			While i < length
				a(from + i) = value
				i = i + 1
			End While

			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function FillBooleanArrayRange(ByRef a As Boolean (), value As Boolean, from As Double, tox As Double) As Boolean
		Dim i, length As Double
		Dim success As Boolean

		If from >= 0 And from <= a.Length And tox >= 0 And tox <= a.Length And from <= tox
			length = tox - from
			i = 0
			While i < length
				a(from + i) = value
				i = i + 1
			End While

			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function FillStringRange(ByRef a As Char (), value As Char, from As Double, tox As Double) As Boolean
		Dim i, length As Double
		Dim success As Boolean

		If from >= 0 And from <= a.Length And tox >= 0 And tox <= a.Length And from <= tox
			length = tox - from
			i = 0
			While i < length
				a(from + i) = value
				i = i + 1
			End While

			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function CopyNumberArray(ByRef a As Double ()) As Double ()
		Dim i As Double
		Dim n As Double ()

		n = New Double (a.Length - 1){}

		i = 0
		While i < a.Length
			n(i) = a(i)
			i = i + 1
		End While

		Return n
	End Function


	Public Function CopyBooleanArray(ByRef a As Boolean ()) As Boolean ()
		Dim i As Double
		Dim n As Boolean ()

		n = New Boolean (a.Length - 1){}

		i = 0
		While i < a.Length
			n(i) = a(i)
			i = i + 1
		End While

		Return n
	End Function


	Public Function CopyString(ByRef a As Char ()) As Char ()
		Dim i As Double
		Dim n As Char ()

		n = New Char (a.Length - 1){}

		i = 0
		While i < a.Length
			n(i) = a(i)
			i = i + 1
		End While

		Return n
	End Function


	Public Function CopyNumberArrayRange(ByRef a As Double (), from As Double, tox As Double, ByRef copyReference As NumberArrayReference) As Boolean
		Dim i, length As Double
		Dim n As Double ()
		Dim success As Boolean

		If from >= 0 And from <= a.Length And tox >= 0 And tox <= a.Length And from <= tox
			length = tox - from
			n = New Double (length - 1){}

			i = 0
			While i < length
				n(i) = a(from + i)
				i = i + 1
			End While

			copyReference.numberArray = n
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function CopyBooleanArrayRange(ByRef a As Boolean (), from As Double, tox As Double, ByRef copyReference As BooleanArrayReference) As Boolean
		Dim i, length As Double
		Dim n As Boolean ()
		Dim success As Boolean

		If from >= 0 And from <= a.Length And tox >= 0 And tox <= a.Length And from <= tox
			length = tox - from
			n = New Boolean (length - 1){}

			i = 0
			While i < length
				n(i) = a(from + i)
				i = i + 1
			End While

			copyReference.booleanArray = n
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function CopyStringRange(ByRef a As Char (), from As Double, tox As Double, ByRef copyReference As StringReference) As Boolean
		Dim i, length As Double
		Dim n As Char ()
		Dim success As Boolean

		If from >= 0 And from <= a.Length And tox >= 0 And tox <= a.Length And from <= tox
			length = tox - from
			n = New Char (length - 1){}

			i = 0
			While i < length
				n(i) = a(from + i)
				i = i + 1
			End While

			copyReference.stringx = n
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function IsLastElement(length As Double, index As Double) As Boolean
		Return index + 1 = length
	End Function


	Public Function CreateNumberArray(length As Double, value As Double) As Double ()
		Dim arrayx As Double ()

		arrayx = New Double (length - 1){}
		Call FillNumberArray(arrayx, value)

		Return arrayx
	End Function


	Public Function CreateBooleanArray(length As Double, value As Boolean) As Boolean ()
		Dim arrayx As Boolean ()

		arrayx = New Boolean (length - 1){}
		Call FillBooleanArray(arrayx, value)

		Return arrayx
	End Function


	Public Function CreateString(length As Double, value As Char) As Char ()
		Dim arrayx As Char ()

		arrayx = New Char (length - 1){}
		Call FillString(arrayx, value)

		Return arrayx
	End Function


	Public Sub SwapElementsOfNumberArray(ByRef A As Double (), ai As Double, bi As Double)
		Dim tmp As Double

		tmp = A(ai)
		A(ai) = A(bi)
		A(bi) = tmp
	End Sub


	Public Sub SwapElementsOfStringArray(ByRef A As StringArrayReference, ai As Double, bi As Double)
		Dim tmp As StringReference

		tmp = A.stringArray(ai)
		A.stringArray(ai) = A.stringArray(bi)
		A.stringArray(bi) = tmp
	End Sub


	Public Sub ReverseNumberArray(ByRef arrayx As Double ())
		Dim i As Double

		i = 0
		While i < arrayx.Length/2
			Call SwapElementsOfNumberArray(arrayx, i, arrayx.Length - i - 1)
			i = i + 1
		End While
	End Sub


	Public Sub AssertFalse(b As Boolean, ByRef failures As NumberReference)
		If b
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertTrue(b As Boolean, ByRef failures As NumberReference)
		If Not b
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertEquals(a As Double, b As Double, ByRef failures As NumberReference)
		If a <> b
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertBooleansEqual(a As Boolean, b As Boolean, ByRef failures As NumberReference)
		If a <> b
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertCharactersEqual(a As Char, b As Char, ByRef failures As NumberReference)
		If a <> b
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertStringEquals(ByRef a As Char (), ByRef b As Char (), ByRef failures As NumberReference)
		If Not StringsEqual(a, b)
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertNumberArraysEqual(ByRef a As Double (), ByRef b As Double (), ByRef failures As NumberReference)
		Dim i As Double

		If a.Length = b.Length
			i = 0
			While i < a.Length
				Call AssertEquals(a(i), b(i), failures)
				i = i + 1
			End While
		Else
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertBooleanArraysEqual(ByRef a As Boolean (), ByRef b As Boolean (), ByRef failures As NumberReference)
		Dim i As Double

		If a.Length = b.Length
			i = 0
			While i < a.Length
				Call AssertBooleansEqual(a(i), b(i), failures)
				i = i + 1
			End While
		Else
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Sub AssertStringArraysEqual(ByRef a As StringReference (), ByRef b As StringReference (), ByRef failures As NumberReference)
		Dim i As Double

		If a.Length = b.Length
			i = 0
			While i < a.Length
				Call AssertStringEquals(a(i).stringx, b(i).stringx, failures)
				i = i + 1
			End While
		Else
			failures.numberValue = failures.numberValue + 1
		End If
	End Sub


	Public Function nCreateStringScientificNotationDecimalFromNumber(decimalx As Double) As Char ()
		Dim mantissaReference, exponentReference As StringReference
		Dim multiplier, inc As Double
		Dim exponent As Double
		Dim done, isPositive As Boolean
		Dim result As Char ()

		mantissaReference = New StringReference()
		exponentReference = New StringReference()
		result = New Char (0 - 1){}
		done = false
		exponent = 0

		If decimalx < 0
			isPositive = false
			decimalx = -decimalx
		Else
			isPositive = true
		End If

		If decimalx = 0
			done = true
		End If

		If Not done
			multiplier = 0
			inc = 0

			If decimalx < 1
				multiplier = 10
				inc = -1
			ElseIf decimalx >= 10
				multiplier = 0.1
				inc = 1
			Else
				done = true
			End If

			If Not done
				exponent = Round(Log10(decimalx))
				exponent = Min(99, exponent)
				exponent = Max(-99, exponent)

				decimalx = decimalx/10 ^ exponent

				' Adjust
				
				While (decimalx >= 10 Or decimalx < 1) And Abs(exponent) < 99
					decimalx = decimalx*multiplier
					exponent = exponent + inc
				End While
			End If
		End If

		nCreateStringFromNumberWithCheck(decimalx, 10, mantissaReference)

		nCreateStringFromNumberWithCheck(exponent, 10, exponentReference)

		If Not isPositive
			result = AppendString(result, "-".ToCharArray())
		End If

		result = AppendString(result, mantissaReference.stringx)
		result = AppendString(result, "e".ToCharArray())
		result = AppendString(result, exponentReference.stringx)

		Return result
	End Function


	Public Function nCreateStringDecimalFromNumber(decimalx As Double) As Char ()
		Dim stringReference As StringReference

		stringReference = New StringReference()

		' This will succeed because base = 10.
		nCreateStringFromNumberWithCheck(decimalx, 10, stringReference)

		Return stringReference.stringx
	End Function


	Public Function nCreateStringFromNumberWithCheck(decimalx As Double, base As Double, ByRef stringReference As StringReference) As Boolean
		Dim stringx As DynamicArrayCharacters
		Dim maximumDigits As Double
		Dim digitPosition As Double
		Dim hasPrintedPoint, isPositive As Boolean
		Dim i, d As Double
		Dim success As Boolean
		Dim characterReference As CharacterReference
		Dim c As Char

		stringx = CreateDynamicArrayCharacters()
		isPositive = true

		If decimalx < 0
			isPositive = false
			decimalx = -decimalx
		End If

		If decimalx = 0
			Call DynamicArrayAddCharacter(stringx, "0"C)
			success = true
		Else
			characterReference = New CharacterReference()

			If IsInteger(base)
				success = true

				maximumDigits = nGetMaximumDigitsForBase(base)

				digitPosition = nGetFirstDigitPosition(decimalx, base)

				decimalx = Round(decimalx*base ^ (maximumDigits - digitPosition - 1))

				hasPrintedPoint = false

				If Not isPositive
					Call DynamicArrayAddCharacter(stringx, "-"C)
				End If

				' Print leading zeros.
				If digitPosition < 0
					Call DynamicArrayAddCharacter(stringx, "0"C)
					Call DynamicArrayAddCharacter(stringx, "."C)
					hasPrintedPoint = true
					i = 0
					While i < -digitPosition - 1
						Call DynamicArrayAddCharacter(stringx, "0"C)
						i = i + 1
					End While
				End If

				' Print number.
				i = 0
				While i < maximumDigits And success
					d = Floor(decimalx/base ^ (maximumDigits - i - 1))

					If d >= base
						d = base - 1
					End If

					If Not hasPrintedPoint And digitPosition - i + 1 = 0
						If decimalx <> 0
							Call DynamicArrayAddCharacter(stringx, "."C)
						End If
						hasPrintedPoint = true
					End If

					If decimalx = 0 And hasPrintedPoint
					Else
						success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference)
						If success
							c = characterReference.characterValue
							Call DynamicArrayAddCharacter(stringx, c)
						End If
					End If

					If success
						decimalx = decimalx - d*base ^ (maximumDigits - i - 1)
						decimalx = Max(decimalx, 0)
						decimalx = Round(decimalx)
					End If
					i = i + 1
				End While

				If success
					' Print trailing zeros.
					i = 0
					While i < digitPosition - maximumDigits + 1
						Call DynamicArrayAddCharacter(stringx, "0"C)
						i = i + 1
					End While
				End If
			Else
				success = false
			End If
		End If

		If success
			stringReference.stringx = DynamicArrayCharactersToArray(stringx)
			Call FreeDynamicArrayCharacters(stringx)
		End If

		' Done
		Return success
	End Function


	Public Function nGetMaximumDigitsForBase(base As Double) As Double
		Dim t As Double

		t = 10 ^ 15
		Return Floor(Log10(t)/Log10(base))
	End Function


	Public Function nGetFirstDigitPosition(decimalx As Double, base As Double) As Double
		Dim power As Double
		Dim t As Double

		power = Ceiling(Log10(decimalx)/Log10(base))

		t = decimalx*base ^ (-power)
		If t < base And t >= 1
		ElseIf t >= base
			power = power + 1
		ElseIf t < 1
			power = power - 1
		End If

		Return power
	End Function


	Public Function nGetSingleDigitCharacterFromNumberWithCheck(c As Double, base As Double, ByRef characterReference As CharacterReference) As Boolean
		Dim numberTable As Char ()
		Dim success As Boolean

		numberTable = nGetDigitCharacterTable()

		If c < base Or c < numberTable.Length
			success = true
			characterReference.characterValue = numberTable(c)
		Else
			success = false
		End If

		Return success
	End Function


	Public Function nGetDigitCharacterTable() As Char ()
		Dim numberTable As Char ()

		numberTable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".ToCharArray()

		Return numberTable
	End Function


	Public Function nCreateNumberFromDecimalStringWithCheck(ByRef stringx As Char (), ByRef decimalReference As NumberReference, ByRef errorMessage As StringReference) As Boolean
		Return nCreateNumberFromStringWithCheck(stringx, 10, decimalReference, errorMessage)
	End Function


	Public Function nCreateNumberFromDecimalString(ByRef stringx As Char ()) As Double
		Dim doubleReference As NumberReference
		Dim stringReference As StringReference
		Dim number As Double

		doubleReference = CreateNumberReference(0)
		stringReference = CreateStringReference("".ToCharArray())
		nCreateNumberFromStringWithCheck(stringx, 10, doubleReference, stringReference)
		number = doubleReference.numberValue

		doubleReference = Nothing
		stringReference = Nothing

		Return number
	End Function


	Public Function nCreateNumberFromStringWithCheck(ByRef stringx As Char (), base As Double, ByRef numberReference As NumberReference, ByRef errorMessage As StringReference) As Boolean
		Dim success As Boolean
		Dim numberIsPositive, exponentIsPositive As BooleanReference
		Dim beforePoint, afterPoint, exponent As NumberArrayReference

		numberIsPositive = CreateBooleanReference(true)
		exponentIsPositive = CreateBooleanReference(true)
		beforePoint = New NumberArrayReference()
		afterPoint = New NumberArrayReference()
		exponent = New NumberArrayReference()

		If base >= 2 And base <= 36
			success = nExtractPartsFromNumberString(stringx, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage)

			If success
				numberReference.numberValue = nCreateNumberFromParts(base, numberIsPositive.booleanValue, beforePoint.numberArray, afterPoint.numberArray, exponentIsPositive.booleanValue, exponent.numberArray)
			End If
		Else
			success = false
			errorMessage.stringx = "Base must be from 2 to 36.".ToCharArray()
		End If

		Return success
	End Function


	Public Function nCreateNumberFromParts(base As Double, numberIsPositive As Boolean, ByRef beforePoint As Double (), ByRef afterPoint As Double (), exponentIsPositive As Boolean, ByRef exponent As Double ()) As Double
		Dim n, i, p, e As Double

		n = 0

		i = 0
		While i < beforePoint.Length
			p = beforePoint(beforePoint.Length - i - 1)

			n = n + p*base ^ i
			i = i + 1
		End While

		i = 0
		While i < afterPoint.Length
			p = afterPoint(i)

			n = n + p*base ^ (-(i + 1))
			i = i + 1
		End While

		If exponent.Length > 0
			e = 0
			i = 0
			While i < exponent.Length
				p = exponent(exponent.Length - i - 1)

				e = e + p*base ^ i
				i = i + 1
			End While

			If Not exponentIsPositive
				e = -e
			End If

			n = n*base ^ e
		End If

		If Not numberIsPositive
			n = -n
		End If

		Return n
	End Function


	Public Function nExtractPartsFromNumberString(ByRef n As Char (), base As Double, ByRef numberIsPositive As BooleanReference, ByRef beforePoint As NumberArrayReference, ByRef afterPoint As NumberArrayReference, ByRef exponentIsPositive As BooleanReference, ByRef exponent As NumberArrayReference, ByRef errorMessages As StringReference) As Boolean
		Dim i, j, count As Double
		Dim success, done, complete As Boolean

		i = 0
		complete = false

		If i < n.Length
			If n(i) = "-"C
				numberIsPositive.booleanValue = false
				i = i + 1
			ElseIf n(i) = "+"C
				numberIsPositive.booleanValue = true
				i = i + 1
			End If

			success = true
		Else
			success = false
			errorMessages.stringx = "Number cannot have length zero.".ToCharArray()
		End If

		If success
			done = false
			count = 0
			
			While i + count < n.Length And Not done
				If nCharacterIsNumberCharacterInBase(n(i + count), base)
					count = count + 1
				Else
					done = true
				End If
			End While

			If count >= 1
				beforePoint.numberArray = New Double (count - 1){}

				j = 0
				While j < count
					beforePoint.numberArray(j) = nGetNumberFromNumberCharacterForBase(n(i + j), base)
					j = j + 1
				End While

				i = i + count

				If i < n.Length
					success = true
				Else
					afterPoint.numberArray = New Double (0 - 1){}
					exponent.numberArray = New Double (0 - 1){}
					success = true
					complete = true
				End If
			Else
				success = false
				errorMessages.stringx = "Number must have at least one number after the optional sign.".ToCharArray()
			End If
		End If

		If success And Not complete
			If n(i) = "."C
				i = i + 1

				If i < n.Length
					done = false
					count = 0
					
					While i + count < n.Length And Not done
						If nCharacterIsNumberCharacterInBase(n(i + count), base)
							count = count + 1
						Else
							done = true
						End If
					End While

					If count >= 1
						afterPoint.numberArray = New Double (count - 1){}

						j = 0
						While j < count
							afterPoint.numberArray(j) = nGetNumberFromNumberCharacterForBase(n(i + j), base)
							j = j + 1
						End While

						i = i + count

						If i < n.Length
							success = true
						Else
							exponent.numberArray = New Double (0 - 1){}
							success = true
							complete = true
						End If
					Else
						success = false
						errorMessages.stringx = "There must be at least one digit after the decimal point.".ToCharArray()
					End If
				Else
					success = false
					errorMessages.stringx = "There must be at least one digit after the decimal point.".ToCharArray()
				End If
			ElseIf base <= 14 And (n(i) = "e"C Or n(i) = "E"C)
				If i < n.Length
					success = true
					afterPoint.numberArray = New Double (0 - 1){}
				Else
					success = false
					errorMessages.stringx = "There must be at least one digit after the exponent.".ToCharArray()
				End If
			Else
				success = false
				errorMessages.stringx = "Expected decimal point or exponent symbol.".ToCharArray()
			End If
		End If

		If success And Not complete
			If base <= 14 And (n(i) = "e"C Or n(i) = "E"C)
				i = i + 1

				If i < n.Length
					If n(i) = "-"C
						exponentIsPositive.booleanValue = false
						i = i + 1
					ElseIf n(i) = "+"C
						exponentIsPositive.booleanValue = true
						i = i + 1
					End If

					If i < n.Length
						done = false
						count = 0
						
						While i + count < n.Length And Not done
							If nCharacterIsNumberCharacterInBase(n(i + count), base)
								count = count + 1
							Else
								done = true
							End If
						End While

						If count >= 1
							exponent.numberArray = New Double (count - 1){}

							j = 0
							While j < count
								exponent.numberArray(j) = nGetNumberFromNumberCharacterForBase(n(i + j), base)
								j = j + 1
							End While

							i = i + count

							If i = n.Length
								success = true
							Else
								success = false
								errorMessages.stringx = "There cannot be any characters past the exponent of the number.".ToCharArray()
							End If
						Else
							success = false
							errorMessages.stringx = "There must be at least one digit after the decimal point.".ToCharArray()
						End If
					Else
						success = false
						errorMessages.stringx = "There must be at least one digit after the exponent symbol.".ToCharArray()
					End If
				Else
					success = false
					errorMessages.stringx = "There must be at least one digit after the exponent symbol.".ToCharArray()
				End If
			Else
				success = false
				errorMessages.stringx = "Expected exponent symbol.".ToCharArray()
			End If
		End If

		Return success
	End Function


	Public Function nGetNumberFromNumberCharacterForBase(c As Char, base As Double) As Double
		Dim numberTable As Char ()
		Dim i As Double
		Dim position As Double

		numberTable = nGetDigitCharacterTable()
		position = 0

		i = 0
		While i < base
			If numberTable(i) = c
				position = i
			End If
			i = i + 1
		End While

		Return position
	End Function


	Public Function nCharacterIsNumberCharacterInBase(c As Char, base As Double) As Boolean
		Dim numberTable As Char ()
		Dim i As Double
		Dim found As Boolean

		numberTable = nGetDigitCharacterTable()
		found = false

		i = 0
		While i < base
			If numberTable(i) = c
				found = true
			End If
			i = i + 1
		End While

		Return found
	End Function


	Public Function nStringToNumberArray(ByRef str As Char ()) As Double ()
		Dim numberArrayReference As NumberArrayReference
		Dim stringReference As StringReference
		Dim numbers As Double ()

		numberArrayReference = New NumberArrayReference()
		stringReference = New StringReference()

		nStringToNumberArrayWithCheck(str, numberArrayReference, stringReference)

		numbers = numberArrayReference.numberArray

		numberArrayReference = Nothing
		stringReference = Nothing

		Return numbers
	End Function


	Public Function nStringToNumberArrayWithCheck(ByRef str As Char (), ByRef numberArrayReference As NumberArrayReference, ByRef errorMessage As StringReference) As Boolean
		Dim numberStrings As StringReference ()
		Dim numbers As Double ()
		Dim i As Double
		Dim numberString, trimmedNumberString As Char ()
		Dim success As Boolean
		Dim numberReference As NumberReference

		numberStrings = SplitByString(str, ",".ToCharArray())

		numbers = New Double (numberStrings.Length - 1){}
		success = true
		numberReference = New NumberReference()

		i = 0
		While i < numberStrings.Length
			numberString = numberStrings(i).stringx
			trimmedNumberString = Trim(numberString)
			success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, numberReference, errorMessage)
			numbers(i) = numberReference.numberValue

			Call FreeStringReference(numberStrings(i))
			Erase trimmedNumberString 
			i = i + 1
		End While

		Erase numberStrings 
		numberReference = Nothing

		numberArrayReference.numberArray = numbers

		Return success
	End Function


	Public Function Negate(x As Double) As Double
		Return -x
	End Function


	Public Function Positive(x As Double) As Double
		Return +x
	End Function


	Public Function Factorial(x As Double) As Double
		Dim i, f As Double

		f = 1

		i = 2
		While i <= x
			f = f*i
			i = i + 1
		End While

		Return f
	End Function


	Public Function Roundx(x As Double) As Double
		Return Floor(x + 0.5)
	End Function


	Public Function BankersRound(x As Double) As Double
		Dim r As Double

		If Absolute(x - Truncatex(x)) = 0.5
			If Not DivisibleBy(Roundx(x), 2)
				r = Roundx(x) - 1
			Else
				r = Roundx(x)
			End If
		Else
			r = Roundx(x)
		End If

		Return r
	End Function


	Public Function Ceil(x As Double) As Double
		Return Ceiling(x)
	End Function


	Public Function Floorx(x As Double) As Double
		Return Floor(x)
	End Function


	Public Function Truncatex(x As Double) As Double
		Dim t As Double

		If x >= 0
			t = Floor(x)
		Else
			t = Ceiling(x)
		End If

		Return t
	End Function


	Public Function Absolute(x As Double) As Double
		Return Abs(x)
	End Function


	Public Function Logarithm(x As Double) As Double
		Return Log10(x)
	End Function


	Public Function NaturalLogarithm(x As Double) As Double
		Return Log(x)
	End Function


	Public Function Sinx(x As Double) As Double
		Return Sin(x)
	End Function


	Public Function Cosx(x As Double) As Double
		Return Cos(x)
	End Function


	Public Function Tanx(x As Double) As Double
		Return Tan(x)
	End Function


	Public Function Asinx(x As Double) As Double
		Return Asin(x)
	End Function


	Public Function Acosx(x As Double) As Double
		Return Acos(x)
	End Function


	Public Function Atanx(x As Double) As Double
		Return Atan(x)
	End Function


	Public Function Atan2x(y As Double, x As Double) As Double
		Dim a As Double

		' Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors.
		a = 0

		If x > 0
			a = Atanx(y/x)
		ElseIf x < 0 And y >= 0
			a = Atanx(y/x) + Pi
		ElseIf x < 0 And y < 0
			a = Atanx(y/x) - Pi
		ElseIf x = 0 And y > 0
			a = Pi/2
		ElseIf x = 0 And y < 0
			a = -Pi/2
		End If

		Return a
	End Function


	Public Function Squareroot(x As Double) As Double
		Return Sqrt(x)
	End Function


	Public Function Expx(x As Double) As Double
		Return Exp(x)
	End Function


	Public Function DivisibleBy(a As Double, b As Double) As Boolean
		Return ((a Mod b) = 0)
	End Function


	Public Function Combinations(n As Double, k As Double) As Double
		Dim i, j, c As Double

		c = 1
		j = 1
		i = n - k + 1

		
		While i <= n
			c = c*i
			c = c/j

			i = i + 1
			j = j + 1
		End While

		Return c
	End Function


	Public Function Permutations(n As Double, k As Double) As Double
		Dim i, c As Double

		c = 1

		i = n - k + 1
		While i <= n
			c = c*i
			i = i + 1
		End While

		Return c
	End Function


	Public Function EpsilonCompare(a As Double, b As Double, epsilon As Double) As Boolean
		Return Abs(a - b) < epsilon
	End Function


	Public Function GreatestCommonDivisor(a As Double, b As Double) As Double
		Dim t As Double

		
		While b <> 0
			t = b
			b = a Mod b
			a = t
		End While

		Return a
	End Function


	Public Function GCDWithSubtraction(a As Double, b As Double) As Double
		Dim g As Double

		If a = 0
			g = b
		Else
			
			While b <> 0
				If a > b
					a = a - b
				Else
					b = b - a
				End If
			End While

			g = a
		End If

		Return g
	End Function


	Public Function IsInteger(a As Double) As Boolean
		Return (a - Floor(a)) = 0
	End Function


	Public Function GreatestCommonDivisorWithCheck(a As Double, b As Double, ByRef gcdReference As NumberReference) As Boolean
		Dim success As Boolean
		Dim gcd As Double

		If IsInteger(a) And IsInteger(b)
			gcd = GreatestCommonDivisor(a, b)
			gcdReference.numberValue = gcd
			success = true
		Else
			success = false
		End If

		Return success
	End Function


	Public Function LeastCommonMultiple(a As Double, b As Double) As Double
		Dim lcm As Double

		If a > 0 And b > 0
			lcm = Abs(a*b)/GreatestCommonDivisor(a, b)
		Else
			lcm = 0
		End If

		Return lcm
	End Function


	Public Function Signx(a As Double) As Double
		Dim s As Double

		If a > 0
			s = 1
		ElseIf a < 0
			s = -1
		Else
			s = 0
		End If

		Return s
	End Function


	Public Function Maxx(a As Double, b As Double) As Double
		Return Max(a, b)
	End Function


	Public Function Minx(a As Double, b As Double) As Double
		Return Min(a, b)
	End Function


	Public Function Power(a As Double, b As Double) As Double
		Return a ^ b
	End Function


	Public Function Gamma(x As Double) As Double
		Return LanczosApproximation(x)
	End Function


	Public Function LogGamma(x As Double) As Double
		Return Log(Gamma(x))
	End Function


	Public Function LanczosApproximation(z As Double) As Double
		Dim p As Double ()
		Dim i, y, t, x As Double

		p = New Double (8 - 1){}
		p(0) = 676.5203681218851
		p(1) = -1259.1392167224028
		p(2) = 771.32342877765313
		p(3) = -176.61502916214059
		p(4) = 12.507343278686905
		p(5) = -0.13857109526572012
		p(6) = 9.9843695780195716e-6
		p(7) = 1.5056327351493116e-7

		If z < 0.5
			y = Pi/(Sin(Pi*z)*LanczosApproximation(1 - z))
		Else
			z = z - 1
			x = 0.99999999999980993
			i = 0
			While i < p.Length
				x = x + p(i)/(z + i + 1)
				i = i + 1
			End While
			t = z + p.Length - 0.5
			y = Sqrt(2*Pi)*t ^ (z + 0.5)*Exp(-t)*x
		End If

		Return y
	End Function


	Public Function Beta(x As Double, y As Double) As Double
		Return Gamma(x)*Gamma(y)/Gamma(x + y)
	End Function


	Public Function Sinhx(x As Double) As Double
		Return (Exp(x) - Exp(-x))/2
	End Function


	Public Function Coshx(x As Double) As Double
		Return (Exp(x) + Exp(-x))/2
	End Function


	Public Function Tanhx(x As Double) As Double
		Return Sinhx(x)/Coshx(x)
	End Function


	Public Function Cot(x As Double) As Double
		Return 1/Tan(x)
	End Function


	Public Function Sec(x As Double) As Double
		Return 1/Cos(x)
	End Function


	Public Function Csc(x As Double) As Double
		Return 1/Sin(x)
	End Function


	Public Function Coth(x As Double) As Double
		Return Coshx(x)/Sinhx(x)
	End Function


	Public Function Sech(x As Double) As Double
		Return 1/Coshx(x)
	End Function


	Public Function Csch(x As Double) As Double
		Return 1/Sinhx(x)
	End Function


	Public Function Errorx(x As Double) As Double
		Dim y, t, tau, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10 As Double

		If x = 0
			y = 0
		ElseIf x < 0
			y = -Errorx(-x)
		Else
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

			t = 1/(1 + 0.5*Abs(x))

			tau = t*Exp(-x ^ 2 + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))))

			y = 1 - tau
		End If

		Return y
	End Function


	Public Function ErrorInverse(x As Double) As Double
		Dim y, a, t As Double

		a = (8*(Pi - 3))/(3*Pi*(4 - Pi))

		t = 2/(Pi*a) + Log(1 - x ^ 2)/2
		y = Signx(x)*Sqrt(Sqrt(t ^ 2 - Log(1 - x ^ 2)/a) - t)

		Return y
	End Function


	Public Function FallingFactorial(x As Double, n As Double) As Double
		Dim k, y As Double

		y = 1

		k = 0
		While k <= n - 1
			y = y*(x - k)
			k = k + 1
		End While

		Return y
	End Function


	Public Function RisingFactorial(x As Double, n As Double) As Double
		Dim k, y As Double

		y = 1

		k = 0
		While k <= n - 1
			y = y*(x + k)
			k = k + 1
		End While

		Return y
	End Function


	Public Function Hypergeometric(a As Double, b As Double, c As Double, z As Double, maxIterations As Double, precision As Double) As Double
		Dim y As Double

		If Abs(z) >= 0.5
			y = (1 - z) ^ (-a)*HypergeometricDirect(a, c - b, c, z/(z - 1), maxIterations, precision)
		Else
			y = HypergeometricDirect(a, b, c, z, maxIterations, precision)
		End If

		Return y
	End Function


	Public Function HypergeometricDirect(a As Double, b As Double, c As Double, z As Double, maxIterations As Double, precision As Double) As Double
		Dim y, yp, n As Double
		Dim done As Boolean

		y = 0
		done = false

		n = 0
		While n < maxIterations And Not done
			yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*z ^ n/Factorial(n)
			If Abs(yp) < precision
				done = true
			End If
			y = y + yp
			n = n + 1
		End While

		Return y
	End Function


	Public Function BernouilliNumber(n As Double) As Double
		Return AkiyamaTanigawaAlgorithm(n)
	End Function


	Public Function AkiyamaTanigawaAlgorithm(n As Double) As Double
		Dim m, j, B As Double
		Dim A As Double ()

		A = New Double (n + 1 - 1){}

		m = 0
		While m <= n
			A(m) = 1/(m + 1)
			j = m
			While j >= 1
				A(j - 1) = j*(A(j - 1) - A(j))
				j = j - 1
			End While
			m = m + 1
		End While

		B = A(0)

		Erase A 

		Return B
	End Function


	Public Function charToLowerCase(character As Char) As Char
		Dim toReturn As Char

		toReturn = character
		If character = "A"C
			toReturn = "a"C
		ElseIf character = "B"C
			toReturn = "b"C
		ElseIf character = "C"C
			toReturn = "c"C
		ElseIf character = "D"C
			toReturn = "d"C
		ElseIf character = "E"C
			toReturn = "e"C
		ElseIf character = "F"C
			toReturn = "f"C
		ElseIf character = "G"C
			toReturn = "g"C
		ElseIf character = "H"C
			toReturn = "h"C
		ElseIf character = "I"C
			toReturn = "i"C
		ElseIf character = "J"C
			toReturn = "j"C
		ElseIf character = "K"C
			toReturn = "k"C
		ElseIf character = "L"C
			toReturn = "l"C
		ElseIf character = "M"C
			toReturn = "m"C
		ElseIf character = "N"C
			toReturn = "n"C
		ElseIf character = "O"C
			toReturn = "o"C
		ElseIf character = "P"C
			toReturn = "p"C
		ElseIf character = "Q"C
			toReturn = "q"C
		ElseIf character = "R"C
			toReturn = "r"C
		ElseIf character = "S"C
			toReturn = "s"C
		ElseIf character = "T"C
			toReturn = "t"C
		ElseIf character = "U"C
			toReturn = "u"C
		ElseIf character = "V"C
			toReturn = "v"C
		ElseIf character = "W"C
			toReturn = "w"C
		ElseIf character = "X"C
			toReturn = "x"C
		ElseIf character = "Y"C
			toReturn = "y"C
		ElseIf character = "Z"C
			toReturn = "z"C
		End If

		Return toReturn
	End Function


	Public Function charToUpperCase(character As Char) As Char
		Dim toReturn As Char

		toReturn = character
		If character = "a"C
			toReturn = "A"C
		ElseIf character = "b"C
			toReturn = "B"C
		ElseIf character = "c"C
			toReturn = "C"C
		ElseIf character = "d"C
			toReturn = "D"C
		ElseIf character = "e"C
			toReturn = "E"C
		ElseIf character = "f"C
			toReturn = "F"C
		ElseIf character = "g"C
			toReturn = "G"C
		ElseIf character = "h"C
			toReturn = "H"C
		ElseIf character = "i"C
			toReturn = "I"C
		ElseIf character = "j"C
			toReturn = "J"C
		ElseIf character = "k"C
			toReturn = "K"C
		ElseIf character = "l"C
			toReturn = "L"C
		ElseIf character = "m"C
			toReturn = "M"C
		ElseIf character = "n"C
			toReturn = "N"C
		ElseIf character = "o"C
			toReturn = "O"C
		ElseIf character = "p"C
			toReturn = "P"C
		ElseIf character = "q"C
			toReturn = "Q"C
		ElseIf character = "r"C
			toReturn = "R"C
		ElseIf character = "s"C
			toReturn = "S"C
		ElseIf character = "t"C
			toReturn = "T"C
		ElseIf character = "u"C
			toReturn = "U"C
		ElseIf character = "v"C
			toReturn = "V"C
		ElseIf character = "w"C
			toReturn = "W"C
		ElseIf character = "x"C
			toReturn = "X"C
		ElseIf character = "y"C
			toReturn = "Y"C
		ElseIf character = "z"C
			toReturn = "Z"C
		End If

		Return toReturn
	End Function


	Public Function charIsUpperCase(character As Char) As Boolean
		Dim isUpper As Boolean

		isUpper = true
		If character = "A"C
		ElseIf character = "B"C
		ElseIf character = "C"C
		ElseIf character = "D"C
		ElseIf character = "E"C
		ElseIf character = "F"C
		ElseIf character = "G"C
		ElseIf character = "H"C
		ElseIf character = "I"C
		ElseIf character = "J"C
		ElseIf character = "K"C
		ElseIf character = "L"C
		ElseIf character = "M"C
		ElseIf character = "N"C
		ElseIf character = "O"C
		ElseIf character = "P"C
		ElseIf character = "Q"C
		ElseIf character = "R"C
		ElseIf character = "S"C
		ElseIf character = "T"C
		ElseIf character = "U"C
		ElseIf character = "V"C
		ElseIf character = "W"C
		ElseIf character = "X"C
		ElseIf character = "Y"C
		ElseIf character = "Z"C
		Else
			isUpper = false
		End If

		Return isUpper
	End Function


	Public Function charIsLowerCase(character As Char) As Boolean
		Dim isLower As Boolean

		isLower = true
		If character = "a"C
		ElseIf character = "b"C
		ElseIf character = "c"C
		ElseIf character = "d"C
		ElseIf character = "e"C
		ElseIf character = "f"C
		ElseIf character = "g"C
		ElseIf character = "h"C
		ElseIf character = "i"C
		ElseIf character = "j"C
		ElseIf character = "k"C
		ElseIf character = "l"C
		ElseIf character = "m"C
		ElseIf character = "n"C
		ElseIf character = "o"C
		ElseIf character = "p"C
		ElseIf character = "q"C
		ElseIf character = "r"C
		ElseIf character = "s"C
		ElseIf character = "t"C
		ElseIf character = "u"C
		ElseIf character = "v"C
		ElseIf character = "w"C
		ElseIf character = "x"C
		ElseIf character = "y"C
		ElseIf character = "z"C
		Else
			isLower = false
		End If

		Return isLower
	End Function


	Public Function charIsLetter(character As Char) As Boolean
		Return charIsUpperCase(character) Or charIsLowerCase(character)
	End Function


	Public Function charIsNumber(character As Char) As Boolean
		Dim isNumberx As Boolean

		isNumberx = true
		If character = "0"C
		ElseIf character = "1"C
		ElseIf character = "2"C
		ElseIf character = "3"C
		ElseIf character = "4"C
		ElseIf character = "5"C
		ElseIf character = "6"C
		ElseIf character = "7"C
		ElseIf character = "8"C
		ElseIf character = "9"C
		Else
			isNumberx = false
		End If

		Return isNumberx
	End Function


	Public Function charIsWhiteSpace(character As Char) As Boolean
		Dim isWhiteSpacex As Boolean

		isWhiteSpacex = true
		If character = " "C
		ElseIf character = vbTab
		ElseIf character = vblf
		ElseIf character = vbcr
		Else
			isWhiteSpacex = false
		End If

		Return isWhiteSpacex
	End Function


	Public Function charIsSymbol(character As Char) As Boolean
		Dim isSymbolx As Boolean

		isSymbolx = true
		If character = "!"C
		ElseIf character = """"C
		ElseIf character = "#"C
		ElseIf character = "$"C
		ElseIf character = "%"C
		ElseIf character = "&"C
		ElseIf character = "'"C
		ElseIf character = "("C
		ElseIf character = ")"C
		ElseIf character = "*"C
		ElseIf character = "+"C
		ElseIf character = ","C
		ElseIf character = "-"C
		ElseIf character = "."C
		ElseIf character = "/"C
		ElseIf character = ":"C
		ElseIf character = ";"C
		ElseIf character = "<"C
		ElseIf character = "="C
		ElseIf character = ">"C
		ElseIf character = "?"C
		ElseIf character = "@"C
		ElseIf character = "["C
		ElseIf character = "\"C
		ElseIf character = "]"C
		ElseIf character = "^"C
		ElseIf character = "_"C
		ElseIf character = "`"C
		ElseIf character = "{"C
		ElseIf character = "|"C
		ElseIf character = "}"C
		ElseIf character = "~"C
		Else
			isSymbolx = false
		End If

		Return isSymbolx
	End Function


	Public Function charCharacterIsBefore(a As Char, b As Char) As Boolean
		Dim ad, bd As Double

		ad = Convert.ToInt16(a)
		bd = Convert.ToInt16(b)

		Return ad < bd
	End Function


	Public Function charDecimalDigitToCharacter(digit As Double) As Char
		Dim c As Char
		If digit = 1
			c = "1"C
		ElseIf digit = 2
			c = "2"C
		ElseIf digit = 3
			c = "3"C
		ElseIf digit = 4
			c = "4"C
		ElseIf digit = 5
			c = "5"C
		ElseIf digit = 6
			c = "6"C
		ElseIf digit = 7
			c = "7"C
		ElseIf digit = 8
			c = "8"C
		ElseIf digit = 9
			c = "9"C
		Else
			c = "0"C
		End If
		Return c
	End Function


	Public Function charCharacterToDecimalDigit(c As Char) As Double
		Dim digit As Double

		If c = "1"C
			digit = 1
		ElseIf c = "2"C
			digit = 2
		ElseIf c = "3"C
			digit = 3
		ElseIf c = "4"C
			digit = 4
		ElseIf c = "5"C
			digit = 5
		ElseIf c = "6"C
			digit = 6
		ElseIf c = "7"C
			digit = 7
		ElseIf c = "8"C
			digit = 8
		ElseIf c = "9"C
			digit = 9
		Else
			digit = 0
		End If

		Return digit
	End Function


End Module

