package TextualTemplates.TextualTemplates;

import JSON.structures.*;
import TextualTemplates.LinkedListNodeFunctions.LinkedListNodeNodes;
import lists.DynamicArrayCharacters.Structures.*;
import lists.LinkedListCharacters.Structures.*;
import lists.LinkedListStrings.Structures.LinkedListStrings;
import references.references.*;

import static JSON.StringElementMaps.StringElementMaps.*;
import static JSON.json.json.*;
import static JSON.parser.parser.ReadJSON;
import static JSON.writer.writer.*;
import static TextualTemplates.LinkedListNodeFunctions.LinkedListNodeFunctions.*;
import static arrays.arrays.arrays.*;
import static lists.DynamicArrayCharacters.DynamicArrayCharactersFunctions.DynamicArrayCharactersFunctions.*;
import static lists.LinkedListCharacters.LinkedListCharactersFunctions.LinkedListCharactersFunctions.*;
import static lists.LinkedListStrings.LinkedListStringsFunctions.LinkedListStringsFunctions.*;
import static references.references.references.*;
import static strings.strings.strings.*;
import static strings.stream.stream.WriteStringToStingStream;

public class TextualTemplates {

    public static boolean IsValidTemplate(char [] template){
        StringReference errorMessage;
        boolean success;
        LinkedListStrings tokens;
        StringReference[] ts;
        Node root;

        tokens = CreateLinkedListString();
        errorMessage = new StringReference();

        success = GenerateTokensFromTemplate(template, tokens, errorMessage);

        if(success) {
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
        for(i = 0d; i < template.length && success;){
            c = template[(int)i];

            if(c != '{') {
                DynamicArrayAddCharacter(da, c);
                i = i + 1d;
            }else{
                FillString(cs, 'x');
                for(j = 0; i+j < template.length && j < cs.length; j = j + 1d){
                    cs[(int)j] = template[(int)(i+j)];
                }

                if(
                    StringsEqual(cs, "{use".toCharArray()) ||
                    StringsEqual(cs, "{end".toCharArray()) ||
                    StringsEqual(cs, "{pri".toCharArray()) ||
                    StringsEqual(cs, "{for".toCharArray()) ||
                    StringsEqual(cs, "{if ".toCharArray()) ||
                    StringsEqual(cs, "{els".toCharArray())
                ){
                    if(pc != '\\'){
                        // Find end.
                        found = false;
                        for(l = 0; i+l < template.length && !found; l = l + 1d){
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

    public static boolean GenerateDocument(char [] template, char [] json, StringReference document, StringReference errorMessage){
        ElementReference data;
        StringArrayReference errorMessages;
        boolean success;

        data = new ElementReference();
        errorMessages = new StringArrayReference();

        success = ReadJSON(json, data, errorMessages);

        if(success) {
            success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage);
        }else{
            errorMessage.string = JoinStringsWithSeparator(errorMessages.stringArray, ", ".toCharArray());
            FreeStringArrayReference(errorMessages);
        }

        return success;
    }

    public static boolean GenerateDocumentBasedOnElement(char [] template, Element data, StringReference document, StringReference errorMessage){
        LinkedListCharacters ll;
        boolean success;
        LinkedListStrings tokens;
        StringReference[] ts;
        Node root;

        tokens = CreateLinkedListString();

        success = GenerateTokensFromTemplate(template, tokens, errorMessage);

        if(success) {
            root = new Node();

            ts = LinkedListStringsToArray(tokens);
            FreeLinkedListString(tokens);

            success = ParseTemplate(ts, root, errorMessage);

            if(success) {
                ll = CreateLinkedListCharacter();

                success = GenerateDocumentFromBlock(root, data, ll, errorMessage);

                if(success) {
                    document.string = LinkedListCharactersToArray(ll);
                    FreeLinkedListCharacter(ll);
                }
            }
        }

        return success;
    }

    public static boolean GenerateDocumentFromBlock(Node root, Element data, LinkedListCharacters ll, StringReference errorMessage) {
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

    public static boolean GenerateDocumentFromNode(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage) {
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

                if(found.booleanValue) {
                    if(StringsEqual(value.type, "string".toCharArray())){
                        valueString = value.string;
                    }else {
                        valueString = WriteJSON(value);
                    }
                    LinkedListCharactersAddString(ll, valueString);
                }else{
                    success = false;
                    errorMessage.string = "Key for printing not found in JSON object: ".toCharArray();
                    errorMessage.string = ConcatenateString(errorMessage.string, n.p1);
                }
            }else{
                success = false;
                errorMessage.string = "Data structure for print command is not a JSON object.".toCharArray();
            }
        }

        return success;
    }

    public static boolean GenerateDocumentFromUse(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage) {
        boolean success;
        BooleanReference found;
        Element value;

        found = new BooleanReference();

        if(StringsEqual(data.type, "object".toCharArray())){
            value = GetObjectValueWithCheck(data.object, n.p1, found);

            if(found.booleanValue) {
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

    public static boolean GenerateDocumentFromIf(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage) {
        boolean success;
        BooleanReference found;
        Element value;

        success = true;
        found = new BooleanReference();

        if(StringsEqual(data.type, "object".toCharArray())){
            value = GetObjectValueWithCheck(data.object, n.p1, found);

            if(found.booleanValue){
                if(StringsEqual(value.type, "boolean".toCharArray())){
                    if(value.booleanValue) {
                        success = GenerateDocumentFromBlock(n.block1, data, ll, errorMessage);
                    }

                    if(n.hasElseBlock){
                        if(!value.booleanValue) {
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
                errorMessage.string = ConcatenateString(errorMessage.string, n.p1);
            }
        }else{
            success = false;
            errorMessage.string = "Data structure for if command is not a JSON object.".toCharArray();
        }

        return success;
    }

    public static boolean GenerateDocumentFromForeach(Node n, Element data, LinkedListCharacters ll, StringReference errorMessage) {
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

                    for(i = 0d; i < value.array.length; i = i + 1d) {
                        arrayElement = value.array[(int) i];
                        if (StringsEqual(arrayElement.type, "object".toCharArray())){
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
                errorMessage.string = ConcatenateString(errorMessage.string, n.p2);
            }
        }else{
            success = false;
            errorMessage.string = "Data structure for foreach command is not a JSON object.".toCharArray();
        }

        return success;
    }

    public static boolean ParseTemplate(StringReference [] tokens, Node node, StringReference errorMessage) {
        boolean success;
        NumberReference position;

        position = CreateNumberReference(0d);
        success = ParseTemplateBlock(tokens, position, node, errorMessage);

        if(success) {
            if (position.numberValue != tokens.length) {
                success = false;
                errorMessage.string = "Unexpected token at the end of template.".toCharArray();
            }
        }

        delete(position);

        return success;
    }

    public static boolean ParseTemplateBlock(StringReference [] tokens, NumberReference position, Node node, StringReference errorMessage) {
        boolean success, done;
        Node tn, nb;

        success = true;
        done = false;

        node.type = "block".toCharArray();
        node.nodes = CreateLinkedListNodes();

        for(; position.numberValue < tokens.length && success && !done;) {
            tn = new Node();
            success = ParseNodeString(tokens[(int) position.numberValue].string, tn, errorMessage);

            if(success) {
                if (StringsEqual(tn.type, "text".toCharArray()) || StringsEqual(tn.type, "print".toCharArray())) {
                    LinkedListAddNode(node.nodes, tn);
                    position.numberValue = position.numberValue + 1d;
                } else if (StringsEqual(tn.type, "use".toCharArray())) {
                    nb = new Node();
                    success = ParseUseBlock(tokens, position, nb, errorMessage);
                    LinkedListAddNode(node.nodes, nb);
                } else if (StringsEqual(tn.type, "if".toCharArray())) {
                    nb = new Node();
                    success = ParseIfBlock(tokens, position, nb, errorMessage);
                    LinkedListAddNode(node.nodes, nb);
                } else if (StringsEqual(tn.type, "foreach".toCharArray())) {
                    nb = new Node();
                    success = ParseForeachBlock(tokens, position, nb, errorMessage);
                    LinkedListAddNode(node.nodes, nb);
                } else {
                    done = true;
                }
            }
        }

        return success;
    }

    public static boolean ParseUseBlock(StringReference[] tokens, NumberReference position, Node useBlock, StringReference errorMessage) {
        boolean success;
        Node n;

        n = new Node();
        ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

        useBlock.type = CopyString(n.type);
        useBlock.p1 = CopyString(n.p1);
        useBlock.block1 = new Node();

        position.numberValue = position.numberValue + 1d;

        success = ParseTemplateBlock(tokens, position, useBlock.block1, errorMessage);

        if(success) {
            if(position.numberValue < tokens.length) {
                n = new Node();
                ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

                if (StringsEqual(n.type, "end".toCharArray())) {
                    success = true;
                    position.numberValue = position.numberValue + 1d;
                } else {
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

    public static boolean ParseIfBlock(StringReference[] tokens, NumberReference position, Node ifBlock, StringReference errorMessage) {
        boolean success;
        Node n;

        n = new Node();
        ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

        ifBlock.type = CopyString(n.type);
        ifBlock.p1 = CopyString(n.p1);
        ifBlock.block1 = new Node();
        ifBlock.hasElseBlock = false;

        position.numberValue = position.numberValue + 1d;

        success = ParseTemplateBlock(tokens, position, ifBlock.block1, errorMessage);

        if(success) {
            if(position.numberValue < tokens.length){
                n = new Node();
                ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

                if(StringsEqual(n.type, "end".toCharArray())) {
                    success = true;
                    position.numberValue = position.numberValue + 1d;
                }else if(StringsEqual(n.type, "else".toCharArray())) {
                    position.numberValue = position.numberValue + 1d;
                    ifBlock.hasElseBlock = true;
                    ifBlock.block2 = new Node();
                    success = ParseTemplateBlock(tokens, position, ifBlock.block2, errorMessage);

                    if(success) {
                        if(position.numberValue < tokens.length){
                            n = new Node();
                            ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

                            if(StringsEqual(n.type, "end".toCharArray())) {
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

    public static boolean ParseForeachBlock(StringReference[] tokens, NumberReference position, Node foreachBlock, StringReference errorMessage) {
        boolean success;
        Node n;

        n = new Node();
        ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

        foreachBlock.type = CopyString(n.type);
        foreachBlock.p1 = CopyString(n.p1);
        foreachBlock.p2 = CopyString(n.p2);
        foreachBlock.block1 = new Node();

        position.numberValue = position.numberValue + 1d;

        success = ParseTemplateBlock(tokens, position, foreachBlock.block1, errorMessage);

        if(success) {
            if(position.numberValue < tokens.length){
                n = new Node();
                ParseNodeString(tokens[(int) position.numberValue].string, n, errorMessage);

                if(StringsEqual(n.type, "end".toCharArray())) {
                    success = true;
                    position.numberValue = position.numberValue + 1d;
                } else {
                    success = false;
                    errorMessage.string = "End block expected at the end of for each block.".toCharArray();
                }
            } else {
                success = false;
                errorMessage.string = "End block expected at the end of foreach block.".toCharArray();
            }
        }

        return success;
    }

    public static boolean ParseNodeString(char [] token, Node node, StringReference errorMessage) {
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
            command = Substring(token, 1d, token.length - 1d);
            parts = SplitByCharacter(command, ' ');

            if(command.length > 0d) {
                if (StringsEqual(parts[0].string, "use".toCharArray())) {
                    if(parts.length == 2d) {
                        node.type = CopyString(parts[0].string);
                        node.p1 = CopyString(parts[1].string);
                    }else{
                        success = false;
                        errorMessage.string = "The use command takes one parameter.".toCharArray();
                    }
                } else if (StringsEqual(parts[0].string, "end".toCharArray())) {
                    if(parts.length == 1d) {
                        node.type = CopyString(parts[0].string);
                    }else{
                        success = false;
                        errorMessage.string = "The end command takes no parameters.".toCharArray();
                    }
                } else if (StringsEqual(parts[0].string, "print".toCharArray())) {
                    if(parts.length == 2d) {
                        node.type = CopyString(parts[0].string);
                        node.p1 = CopyString(parts[1].string);
                    }else{
                        success = false;
                        errorMessage.string = "The print command takes one parameter.".toCharArray();
                    }
                } else if (StringsEqual(parts[0].string, "foreach".toCharArray())) {
                    if(parts.length == 4d) {
                        if(StringsEqual(parts[2].string, "in".toCharArray())) {
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
                } else if (StringsEqual(parts[0].string, "if".toCharArray())) {
                    if(parts.length == 2d) {
                        node.type = CopyString(parts[0].string);
                        node.p1 = CopyString(parts[1].string);
                    }else{
                        success = false;
                        errorMessage.string = "The if command takes one parameter.".toCharArray();
                    }
                } else if (StringsEqual(parts[0].string, "else".toCharArray())) {
                    if(parts.length == 1d) {
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
            node.p1 = ReplaceString(token, "\\{print ".toCharArray(), "{print ".toCharArray());
            node.p1 = ReplaceString(node.p1, "\\{use ".toCharArray(), "{use ".toCharArray());
            node.p1 = ReplaceString(node.p1, "\\{if ".toCharArray(), "{if ".toCharArray());
            node.p1 = ReplaceString(node.p1, "\\{end}".toCharArray(), "{end}".toCharArray());
            node.p1 = ReplaceString(node.p1, "\\{foreach ".toCharArray(), "{foreach ".toCharArray());
            node.p1 = ReplaceString(node.p1, "\\{else}".toCharArray(), "{else}".toCharArray());
        }

        return success;
    }

    public static void delete(Object object) {
        // Java has garbage collection.
    }
}
