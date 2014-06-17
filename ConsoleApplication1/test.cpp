#include <iostream>
#include "test.h"
#include "data_core/base/List.h"
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

































//Entry
void testEntry()
{

	std::cout << "test entry." << std::endl;
	test2();

}
