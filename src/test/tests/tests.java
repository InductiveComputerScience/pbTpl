package tests;

import JSON.structures.*;
import lists.LinkedListStrings.Structures.LinkedListStrings;
import references.references.*;

import static JSON.parser.parser.*;
import static TextualTemplates.TextualTemplates.TextualTemplates.*;
import static lists.LinkedListStrings.LinkedListStringsFunctions.LinkedListStringsFunctions.CreateLinkedListString;
import static lists.LinkedListStrings.LinkedListStringsFunctions.LinkedListStringsFunctions.LinkedListStringsLength;
import static references.references.references.*;
import static testing.testing.testing.*;

public class tests {
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
        testGenerateDocument8(failures);

        return failures.numberValue;
    }

    public static void testGenerateDocument8(NumberReference failures) {
        StringReference document, errorMessage;
        boolean success;
        char[] template, json;

        document = new StringReference();
        errorMessage = new StringReference();

        template = "This is a test: {print b} {foreach x in a}{print x}{end}.".toCharArray();
        json = "{\"a\": [1, 2, 3], \"b\": 4}".toCharArray();
        success = GenerateDocument(template, json, document, errorMessage);

        if(success){
            AssertStringEquals("This is a test: 4 123.".toCharArray(), document.string, failures);
        }

        AssertTrue(success, failures);
    }

    public static void testTokenGeneration(NumberReference failures) {
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

    public static void testGenerateDocument1(NumberReference failures) {
        AssertTemplateResult(
            "This is a template, this is a value: {print a}.".toCharArray(),
            "{\"c\": 5, \"a\": 6}".toCharArray(),
            "This is a template, this is a value: 6.".toCharArray(),
            failures
        );
    }

    public static void testGenerateDocument2(NumberReference failures) {
        AssertTemplateResult(
            "This is a template, this is a value: {print a} {use b}{print a} {print b}{end}.".toCharArray(),
            "{\"b\": {\"a\": 1, \"b\": 2}, \"a\": 6}".toCharArray(),
            "This is a template, this is a value: 6 1 2.".toCharArray(),
            failures
        );
    }

    public static void testGenerateDocument3(NumberReference failures) {
        AssertTemplateResult(
            "This is a template, this is a value: {print a} {use b}{print a} {print b}{use c}{print a} {print b}{end}{print a}{end}{print a}.".toCharArray(),
            "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": 6}".toCharArray(),
            "This is a template, this is a value: 6 1 23 416.".toCharArray(),
            failures
        );
    }

    public static void testGenerateDocument4(NumberReference failures) {
        AssertTemplateResult(
            "T: {if a}a{end}.".toCharArray(),
            "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": true}".toCharArray(),
            "T: a.".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "T: {if a}a{else}b{end}.".toCharArray(),
            "{\"b\": {\"a\": 1, \"b\": 2, \"c\": {\"a\": 3, \"b\": 4}}, \"a\": false}".toCharArray(),
            "T: b.".toCharArray(),
            failures
        );
    }

    public static void testGenerateDocument5(NumberReference failures) {
        AssertTemplateResult(
            "T: {foreach a in b}{print a}{end}.".toCharArray(),
            "{\"b\": [1, 2, 3, 4]}".toCharArray(),
            "T: 1234.".toCharArray(),
            failures
        );
    }

    public static void testGenerateDocument6(NumberReference failures) {
        AssertTemplateResult(
            "{test}\\{print a}.".toCharArray(),
            "{\"c\": 5, \"a\": 6}".toCharArray(),
            "{test}{print a}.".toCharArray(),
            failures
        );
    }

    public static void testGenerateDocument7(NumberReference failures) {
        AssertTemplateResult(
            "{".toCharArray(),
            "{}".toCharArray(),
            "{".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "{  ".toCharArray(),
            "{}".toCharArray(),
            "{  ".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "{use a}\\{print a}{end}".toCharArray(),
            "{\"a\": {}}".toCharArray(),
            "{print a}".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "\\{print a}{print a}}".toCharArray(),
            "{\"a\": 3}".toCharArray(),
            "{print a}3}".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "\\\\{print a}{print a}}".toCharArray(),
            "{\"a\": 3}".toCharArray(),
            "\\{print a}3}".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "\\\\{print a}{print a}\\\\{print a}}".toCharArray(),
            "{\"a\": 3}".toCharArray(),
            "\\{print a}3\\{print a}}".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "\\{print a}{print a}\\{".toCharArray(),
            "{\"a\": 3}".toCharArray(),
            "{print a}3\\{".toCharArray(),
            failures
        );

        AssertTemplateResult(
            "    <div>Pris</div>\n    {foreach p in products}\n    <div>{print productCode}</div>\n    <div>1</div>\n    <div>{print price}</div>\n    {end}\n    <div>Totalt</div>".toCharArray(),
            "{\"products\": [{\"productCode\": \"kl\", \"price\": \"1.2\"}, {\"productCode\": \"skl\", \"price\": \"20.0\"}]}".toCharArray(),
            "    <div>Pris</div>\n    \n    <div>kl</div>\n    <div>1</div>\n    <div>1.2</div>\n    \n    <div>skl</div>\n    <div>1</div>\n    <div>20.0</div>\n    \n    <div>Totalt</div>".toCharArray(),
            failures
        );

        AssertTemplateError(
            "{print".toCharArray(),
            "{}".toCharArray(),
            "Template command found, but not ended properly.".toCharArray(),
            failures
        );

        AssertTemplateError(
            "{print a}".toCharArray(),
            "{}".toCharArray(),
            "Key for printing not found in JSON object: a".toCharArray(),
            failures
        );

        AssertTemplateError(
            "This is a template, this is a value: {print a {print a}.".toCharArray(),
            "{\"a\": 5}".toCharArray(),
            "The print command takes one parameter.".toCharArray(),
            failures
        );

        AssertTemplateError(
            "This is a {use a}\\{print a}template, this is a value: {print a}.{end}".toCharArray(),
            "{\"a\": 5}".toCharArray(),
            "Data structure for print command is not a JSON object.".toCharArray(),
            failures
        );

        AssertTemplateError(
            "{use a}".toCharArray(),
            "{}".toCharArray(),
            "End block expected at the end of use block.".toCharArray(),
            failures
        );

        AssertTemplateError(
            "{if a}".toCharArray(),
            "{\"a\": true}".toCharArray(),
            "End or else block expected at the end of if block.".toCharArray(),
            failures
        );

        AssertTemplateError(
            "{if a}{else}".toCharArray(),
            "{\"a\": true}".toCharArray(),
            "End block expected at the end of else block.".toCharArray(),
            failures
        );

        AssertTemplateError(
            "{foreach x in a}".toCharArray(),
            "{\"a\": [1, 2, 3, 4]}".toCharArray(),
            "End block expected at the end of foreach block.".toCharArray(),
            failures
        );
    }

    public static void AssertTemplateResult(char[] template, char[] json, char[] result, NumberReference failures) {
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

        if(success) {
            success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessage);

            AssertTrue(success, failures);

            if (success) {
                AssertStringEquals(document.string, result, failures);
            }
        }
    }

    public static void AssertTemplateError(char[] template, char[] json, char[] errorMessage, NumberReference failures) {
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

        if(success) {
            success = GenerateDocumentBasedOnElement(template, data.element, document, errorMessageRef);

            AssertFalse(success, failures);

            if (!success) {
                AssertStringEquals(errorMessage, errorMessageRef.string, failures);
            }
        }
    }
}
