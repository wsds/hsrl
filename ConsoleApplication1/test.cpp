#include <iostream>

#include <sstream>
#include <fstream>

#include <string>
#include<conio.h>

#include "test.h"
#include "data_core/base/List.h"
#include "data_core/base/HashTable.h"
#include "data_core/JSNumber.h"
#include "data_core/JSKeyValue.h"
#include "data_core/JSON.h"

#include "swift\interpret_main.h"

//test base list
void testMM(){
	int size = 10;
	void *pointer = malloc(size);


	free(pointer);


	int s1 = sizeof(void*);
	int s2 = sizeof(char);
	int s3 = sizeof(int);
	int s4 = sizeof(JSNumber*);

}


void showList(LIST *list)
{
	std::cout << "The length of the list is: " << list->length << std::endl;
	for (int i = 0; i < list->length; i++)
	{
		JSNumber * number = (JSNumber*)list->find(i);

		std::cout << ((JSObject*)number)->number << std::endl;
	}
}

void showJSObject(JSObject *object, int indent)
{
	char *indentation = (char*)JSMalloc(indent * 2 + 1);

	for (int i = 0; i < indent * 2 + 1; i++){
		*((char*)indentation + i) = 32;
	}

	if (object->type == JSNUMBER){
		JSNumber * js_number = (JSNumber*)object;
		std::cout << "JSNumber: " << ((JSObject*)js_number)->number << std::endl;
	}
	else if (object->type == JSKEYVALUE){
		JSKeyValue * keyValue = (JSKeyValue*)object;
		std::cout << "JSKeyValue: key:" << keyValue->key << "            value:" << keyValue->value << std::endl;
	}
	else if (object->type == JSJSON){
		JSON* json = (JSON*)object;
		for (int i = 0; i < json->length; i++)
		{
			JSObject * child = json->find(i);
			showJSObject(child, indent + 1);
		}
	}
}

void test1()
{
	std::cout << "test1" << std::endl;
	LIST *list = new LIST();
	list->initialize();

}

void test2()
{
	std::cout << "test2" << std::endl;
	LIST *list = new LIST();
	list->initialize();

	for (int i = 0; i < 31; i++)
	{
		JSNumber * js_number = new JSNumber();
		((JSObject*)js_number)->number = 1000 + i;
		list->push((JSObject*)js_number);
	}
	JSNumber * js_number = new JSNumber();
	((JSObject*)js_number)->number = 500001;
	list->insert((JSObject*)js_number, 5);

	list->del(14);


	JSNumber * number1 = new JSNumber();
	((JSObject*)number1)->number = 800001;
	list->replace((JSObject*)number1, 20);

	showList(list);
}















//test base hashmap
void test3(){

	std::cout << "test3" << std::endl;
	HashTable *hashTable = new HashTable();
	hashTable->initialize();
	char *test = "ab0";
	for (int i = 0; i < 43; i++)
	{
		JSNumber * number = new JSNumber();
		((JSObject*)number)->number = 1000 + i;
		char* key = (char*)JSMalloc(4);
		key[0] = 97;
		key[1] = 98;
		key[2] = 48 + i;
		key[3] = 0;

		hashTable->set(key, (JSObject*)number);
	}

	JSNumber * number = new JSNumber();
	((JSObject*)number)->number = 888006;
	char* key = "ab5";
	hashTable->set(key, (JSObject*)number);

	hashTable->set(key, (JSObject*)number);

	hashTable->del("ab4");
	hashTable->del("abG");
	hashTable->del("abA");

	for (int i = 0; i < 43; i++)
	{

		char* key = (char*)JSMalloc(4);
		key[0] = 97;
		key[1] = 98;
		key[2] = 48 + i;
		key[3] = 0;

		JSNumber * number = (JSNumber *)hashTable->get(key);
		if (number == NULL){
			std::cout << "key:" << key << " ц╩сп" << std::endl;
		}
		else{
			std::cout << "key:" << key << "    value:" << ((JSObject*)number)->number << std::endl;

		}
	}
	//HashEntry* e[113];


	//for (int i = 0; i < 113; i++){
	//	//e[i] = hashTable->elements[i];
	//}

	std::cout << "test3 end" << std::endl;
}

//char LEFTBRACKET = '[';
//char RIGHTBRACKET = ']';
//char SINGLEQUOTE = '\'';
//char DOUBLEQUOTES = '"';

int BRACKET_Counter = 0;
int SINGLEQUOTE_Counter = 0;
int DOUBLEQUOTES_Counter = 0;

//int strlen(char *str)
//{
//	char *eos = str;
//
//	while (*eos++);
//
//	return(eos - str - 1);
//}


int lineNumberRead = 0;
char line[128] = { 0 };

void interpret_line(char* line){

	//std::cout << "resolving: " << line << std::endl;
	resolveCodeLine(line);
	std::cout << "-------------------------------line:" << lineNumberRead + 2 << std::endl;

}


char resolvedLine[256] = { 0 };


void resolveLine(char* line){

	char localChar;
	int string_length = strlen(line);
	int pre_blank = 0;
	for (int i = 0; i < string_length; i++){
		localChar = line[i];
		if (localChar == BLANK || localChar == TAB){
			pre_blank++;
		}
		else{
			break;
		}
	}
	if (string_length - pre_blank > 2){
		if (line[pre_blank + 0] == SLASH&&line[pre_blank + 1] == SLASH){
			//std::cout << "comment line:" << lineNumberRead + 2 << std::endl;
			return;
		}
	}
	else{
		//std::cout << "empty line:" << lineNumberRead + 2 << std::endl;
		return;
	}

	for (int i = 0; i < string_length; i++){
		localChar = line[i];
		if (localChar == SINGLEQUOTE){
			SINGLEQUOTE_Counter = (SINGLEQUOTE_Counter + 1) % 2;
		}
		else if (localChar == DOUBLEQUOTES){
			DOUBLEQUOTES_Counter = (DOUBLEQUOTES_Counter + 1) % 2;
		}
		else if (localChar == LEFTBRACKET){
			BRACKET_Counter++;
		}
		else if (localChar == RIGHTBRACKET){
			BRACKET_Counter--;
		}
	}
	strappend(resolvedLine, line + pre_blank);
	if (SINGLEQUOTE_Counter == 0 && DOUBLEQUOTES_Counter == 0 && BRACKET_Counter == 0){

		interpret_line(resolvedLine);

		strclear(resolvedLine);
	}
	else{
		strappend(resolvedLine, "\r\n");
	}


}




char* getLine(){

	std::ifstream fin("swift.js", std::ios::in);

	int lineNumber = 0;
	while (fin.getline(line, sizeof(line)))
	{
		if (lineNumber > lineNumberRead){
			//std::cout << line << std::endl;
			resolveLine(line);
			lineNumberRead++;
		}
		lineNumber++;
	}

	fin.clear();
	fin.close();
	return line;

}


int test_file()
{
	interpret_main();
	while (int key = _getch())
	{
		std::cout << "***************************************************************: " << key << std::endl;
		if (key == 103){
			getLine();
		}
		else if (key == 114){
			lineNumberRead = 0;
		}
	}
	return 0;
}




//test JSON
void test4(){
	testJSONParse();
}


//Entry
void testEntry()
{

	//std::cout << "test entry." << std::endl;
	//test4();
	//std::cout << "test end." << std::endl;
	test_file();
	//interpret_test();
}
