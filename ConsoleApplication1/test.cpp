#include <iostream>
#include "test.h"
#include "data_core/base/List.h"
#include "data_core/base/HashTable.h"
#include "data_core/JSNumber.h"


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

		std::cout << number->number << std::endl;
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
		JSNumber * number = new JSNumber();
		number->number = 1000 + i;
		list->push((JSObject*)number);
	}
	JSNumber * number = new JSNumber();
	number->number = 500001;
	list->insert((JSObject*)number, 5);

	list->del(14);


	JSNumber * number1 = new JSNumber();
	number1->number = 800001;
	list->replace((JSObject*)number1, 20);

	showList(list);
}















//test base hashmap
void test3(){

	std::cout << "test3" << std::endl;
	HashTable *hashTable = new HashTable();
	hashTable->initialize();
	char *test = "ab0";
	for (int i = 0; i < 31; i++)
	{
		JSNumber * number = new JSNumber();
		number->number = 1000 + i;
		char* key = (char*)JSMalloc(4);
		key[0] = 97;
		key[1] = 98;
		key[2] = 48 + i;
		key[3] = 0;

		hashTable->set(key, (JSObject*)number);
	}

	JSNumber * number = new JSNumber();
	number->number = 888006;
	char* key = "ab5";
	hashTable->set(key, (JSObject*)number);

	hashTable->set(key, (JSObject*)number);

	hashTable->del("ab4");
	hashTable->del("abG");
	hashTable->del("abA");

	for (int i = 0; i < 31; i++)
	{

		char* key = (char*)JSMalloc(4);
		key[0] = 97;
		key[1] = 98;
		key[2] = 48 + i;
		key[3] = 0;

		JSNumber * number = (JSNumber *)hashTable->get(key);
		if (number == NULL){
			std::cout << "key:" << key << " û��" << std::endl;
		}
		else{
			std::cout << "key:" << key << "    value:" <<number->number << std::endl;

		}
	}
	HashEntry* e[32];


	for (int i = 0; i < 32; i++){
		e[i] = hashTable->elements[i];
	}

	std::cout << "test3 end" << std::endl;
}

















//Entry
void testEntry()
{

	std::cout << "test entry." << std::endl;
	test3();
	std::cout << "test end." << std::endl;
}
