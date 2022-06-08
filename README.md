# pbTpl - pb Templates
A template library available in many programming languages made with [progsbase](https://progsbase.com/). The goals of this library are to 

1) Have a single library for templating that is stable accross programming languages and over time.
2) Is easy to build in any programming language and to include in any project.
3) Is easy to use.


## Try Online
Try the library [online here](https://repo.progsbase.com/repoviewer/no.inductive.libraries/TextualTemplates/0.1.10///GenerateDocument/).



## Available in the Following Languages

[TypeScript](TypeScript/) |
[Java](Java/) |
[C](C/) | 
[C++](Cpp/) | 
[JavaScript](JavaScript/) | 
[PHP](PHP/) | 
[C#](CS/) | 
[Python](Python/) | 
[Ruby](Ruby/) |
[Visual Basic](VisualBasic/)


## Main Function
The library is available in the programming languages above. The main function for using a template is as follows:

```
boolean GenerateDocument(char [] template, char [] json, StringReference document, StringReference errorMessage);
```

This function takes a template string `template` and JSON string `json`. It returns true on success or false on failure. On success, `document.string` is set to the result; on failure `errorMessage.string` is filled with an error message.


## General About Templates
A textual template is text with certain parts left out that will be filled in by a generation step. The source for filling in is a data structure generated from a JSON file.

Templates are used for many different things in production systems. Most websites are generated based on a template, be it directly in the front end, such as with Angular, React and Vue, or in the backend. Templates are also used for production of formal documents and emails.


## Tags and Their Functions

### Print Value
```
{print a}
```

Will print the value of a. If it is a JSON object or structure, the JSON repressentation will be printed.
 
### Use structure
```
{use a}
{print b}
{end}
```

This will cause a.b to be printed. Inside the use structure, the variables inside structure a can be used.

### Iterate
```
{foreach a in b}
{end}
```

This will iterate over all elements of the array `b`. Each element will be known as `a` inside. If `a` is an object, it will automatically be used inside the foreach loop, so there is no need to write `{use a}`.

### If-else
```
{if a}
{else}
{end}
```

If a is true, the first block is printed, else the else block. The else block is optional.

### Escape
If you want to output `{` followed directly by one of the command strings, use a backslash in front. If the `{` is not followed directly by one of the commands, the text is simply output.



## How to Work with Templates
There are three main issues with using templates:

### The data model
Generate all the data that is to be filled into the document. This includes decision variables if some information is included or left out based on conditions.

### Templates
Write the template with the tags to fill in information. There is no need for complex logic or formatting commands, as this should be taken care of during data model generation. The reason is that the code should be in the code and not in the template.

### Locale and Formatting
Special formatting of numbers should be taken care of during data model generation. This is better as a full fledged programming language is available.


