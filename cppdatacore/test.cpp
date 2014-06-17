#include <iostream>
#include "test.h"
#include "data_core/base/List.h"
#include "data_core/JSNumber.h"


//test base list

void showList(LIST *list)
{
    std::cout << "The length of the list is: " << list->length << std::endl;
    for(int i = 0; i < list->length; i++)
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

    for(int i = 0; i < 10; i++)
    {
        JSNumber * number = new JSNumber();
        number->number = 1000 + i;
        list->push((JSObject*)number);
    }

    showList(list);
}

































//Entry
void testEntry()
{

    std::cout << "test entry." << std::endl;
    test2();

}
