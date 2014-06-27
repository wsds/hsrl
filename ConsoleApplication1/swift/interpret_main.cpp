#include "interpret_main.h"
#include "interface/log.h"

#include "../data_core/data_core.h"

void interpret_test_Simple_Value(){

	char * json_str5 = "[123,567:[012,[\"hello\"],456,'''''',swift_main:[123,'123123'],123],567,,,,]";

	JSON* json = parseJSON(json_str5);

	log((JSObject*)json);

	JSNumber * number = new JSNumber(888006);
	log((JSObject*)number);

	JSString * string = new JSString("hello swift");
	log((JSObject*)string);


	char* shoppingListStr = "[\"catfish\",\"water\",\"tulips\",\"blue paint\"]";
	JSON* shoppingList = shoppingList = parseJSON(shoppingListStr);

	JSString* valueStr1001 = new JSString("bottle of water");
	shoppingList->replace((JSObject*)valueStr1001, 1);
	log((JSObject*)shoppingList);

	char* occupationsStr = "[123,\"Malcolm\":\"Captain\",\"Kaylee\":\"Mechanic\",456]";
	JSON* occupations = parseJSON(occupationsStr);

	JSString* valueStr1002 = new JSString("Public Relations");

	char* keyStr = "Jayne";
	occupations->set(keyStr, (JSObject*)valueStr1002);

	JSString* valueStr1003 = new JSString("private Relations");
	occupations->set(keyStr, (JSObject*)valueStr1003);

	log((JSObject*)occupations);

}

void interpret_test(){
	interpret_test_Simple_Value();

}

void interpret_main(){




}