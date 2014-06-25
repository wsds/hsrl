#include "JSON.h"
#include <iostream>

JSON::JSON(){
	this->type = JSJSON;
	this->initialize();
}


bool JSON::initialize()
{
	this->list = new LIST();//to optimize the memory use, get it from a managed pool instead of new it.//to do
	this->list->initialize();
	this->hashTable = new HashTable();
	this->hashTable->initialize();

	this->length = 0;
	this->is_initialized = true;

	return true;
}

bool JSON::free()
{
	//JSFree(this->elements);
	return true;
}


//list
JSObject* JSON::pop()
{
	JSObject* object = this->list->pop();
	if (object != NULL&&object->type == JSKEYVALUE){
		this->hashTable->del(((JSKeyValue*)object)->key);
	}
	this->length--;
	return object;
}

bool JSON::push(JSObject* object)
{
	this->list->push(object);
	if (object->type == JSKEYVALUE){
		this->hashTable->set(((JSKeyValue*)object)->key, object);
	}
	this->length++;
	return true;
}

bool JSON::insert(JSObject* object, int index)
{
	this->list->insert(object, index);
	if (object->type == JSKEYVALUE){
		this->hashTable->set(((JSKeyValue*)object)->key, object);
	}
	this->length++;
	return true;
}

bool JSON::replace(JSObject* object, int index)
{
	this->list->replace(object, index);
	if (object->type == JSKEYVALUE){
		this->hashTable->set(((JSKeyValue*)object)->key, object);
	}
	return true;
}

JSObject*  JSON::del(int index)
{
	JSObject* object = this->list->del(index);

	if (object != NULL)//deleted
	{
		if (object->type == JSKEYVALUE){
			this->hashTable->del(((JSKeyValue*)object)->key);
		}
		this->length--;
	}
	else //not found
	{
	}
	return object;
}

JSObject* JSON::find(int index)
{
	JSObject* object = this->list->find(index);
	return object;
}


//hash
JSObject* JSON::get(char* key)
{
	JSObject* object = this->hashTable->get(key);
	return object;
}

bool JSON::set(char* key, JSObject* value)
{
	JSKeyValue* object = new JSKeyValue();
	object->key = key;
	object->value = value;

	int status = this->hashTable->set(((JSKeyValue*)object)->key, (JSObject*)object);

	if (status == 2)//new entry
	{
		this->list->push((JSObject*)object);
		this->length++;
	}
	else if (status == 1)//replace entry
	{
		int index = this->list->findValue((JSObject*)object);
		this->list->replace((JSObject*)object, index);
	}

	return true;
}

JSObject* JSON::del(char* key)
{
	JSObject* object = this->hashTable->del(key);

	if (object != NULL)//deleted
	{
		int index = this->list->findValue((JSObject*)object);
		this->list->del(index);
		this->length--;
	}
	else //not found
	{
	}
	return object;
}

char* JSON::stringify()
{
	return NULL;
}

bool JSON::parse(char* string)
{

	JSONIndicator* root_json_indicator = new JSONIndicator();
	root_json_indicator->head = 0;
	root_json_indicator->tail = strlen(string);

	parseJSON(string, root_json_indicator);
	//bool parseFlag = checkJSON(string);
	//if (parseFlag){

	//}
	//return parseFlag;
	return true;
}

int parsingStatus = 1;//[1:normal,2:quote_start,3:string,4:second_quote_start,5:quote_stop,6:second_quote_stop],[1-2,2-3,3-1,2-4,4-5,5-3,4-1]
int QuoteStatus = 11;//[ 11 = ' , 13 = ''' , 21 = " , 23 = """]
int last_COMMA_index = 0;

char LEFTBRACKET = '[';
char RIGHTBRACKET = ']';
char SINGLEQUOTE = '\'';
char DOUBLEQUOTES = '"';

char COMMA = ',';
char COLON = ':';

char NUMBERCHARSTART = '0';
char NUMBERCHAREND = '9';

int json_indicators_stack_size = 10;
int json_indicators_stack_top = 0;

JSON* parseJSON(char* string, JSONIndicator* root_json_indicator1212){

	char localChar;
	int string_length = strlen(string);
	last_COMMA_index = 0;

	JSONIndicator* json_indicator = NULL;
	JSONIndicator* object_indicator = NULL;
	JSON* result_json = NULL;

	JSKeyValue * jsKeyValue = NULL;

	int mem_size = json_indicators_stack_size*sizeof(JSONIndicator*);
	JSONIndicator** json_indicators = (JSONIndicator**)JSMalloc(mem_size);

	for (int i = 0; i< string_length; i++){
		localChar = string[i];
		if (parsingStatus == 1){
			//*************************************parsingStatus == 1*****************************************************************************************************
			if (localChar == COMMA){
				if (i>last_COMMA_index){
					object_indicator = new JSONIndicator();//get from pool//to do
					object_indicator->head = last_COMMA_index;
					object_indicator->tail = i;
					object_indicator->quotes_count = json_indicator->quotes_count;
					json_indicator->quotes_count = 0;

					last_COMMA_index = i + 1;

					JSObject* object = parseObject(string, object_indicator, false);
					if (jsKeyValue != NULL){
						jsKeyValue->value = object;
						jsKeyValue = NULL;
					}
					else{
						((JSON*)(json_indicator->json))->push(object);
					}
				}
			}
			else if (localChar == COLON){
				if (i > last_COMMA_index){
					object_indicator = new JSONIndicator();//get from pool//to do
					object_indicator->head = last_COMMA_index;
					object_indicator->tail = i;
					object_indicator->quotes_count = json_indicator->quotes_count;
					json_indicator->quotes_count = 0;

					last_COMMA_index = i + 1;

					JSObject* object = parseObject(string, object_indicator, true);
					if (jsKeyValue == NULL){
						jsKeyValue = (JSKeyValue *)object;
						((JSON*)(json_indicator->json))->push(object);
					}
					else{
						//report error;
					}
				}
			}
			else if (localChar == SINGLEQUOTE){
				QuoteStatus = 11;
				parsingStatus = 2;
			}
			else if (localChar == DOUBLEQUOTES){
				QuoteStatus = 21;
				parsingStatus = 2;
			}
			else if (localChar == LEFTBRACKET){
				if (json_indicators_stack_top < json_indicators_stack_size){


					JSON* json = new JSON();
					json->initialize();

					json->json_indicator = new JSONIndicator();//get from pool//to do
					json->json_indicator->head = i;
					json->json_indicator->json = (JSObject*)json;
					json->json_indicator->quotes_count = 0;

					if (i == 0 && json_indicator == NULL){
						result_json = json;
					}
					else{
						((JSON*)(json_indicator->json))->push((JSObject*)json);
					}


					json_indicators[json_indicators_stack_top] = json->json_indicator;
					json_indicators_stack_top++;

					json_indicator = json->json_indicator;

					last_COMMA_index++;
				}
				else{
					//report error
				}
			}
			else if (localChar == RIGHTBRACKET){

				object_indicator = new JSONIndicator();//get from pool//to do
				object_indicator->head = last_COMMA_index;
				object_indicator->tail = i;
				object_indicator->quotes_count = json_indicator->quotes_count;
				json_indicator->quotes_count = 0;

				JSObject* object = parseObject(string, object_indicator, false);
				if (jsKeyValue != NULL){
					jsKeyValue->value = object;
					jsKeyValue = NULL;
				}
				else{
					((JSON*)(json_indicator->json))->push(object);
				}


				if (i + 1 == string_length || string[i + 1] == COMMA){
					last_COMMA_index = i + 2;
				}


				if (json_indicators_stack_top > 1){
					//resolve the last element spited by COMMA
					json_indicator = json_indicators[json_indicators_stack_top - 1];
					json_indicator->tail = i;

					json_indicator = json_indicators[json_indicators_stack_top - 2];
					//json_indicator->json = parseJSON(string, json_indicator);
					json_indicators_stack_top--;
				}
				else if (json_indicators_stack_top = 1){
					if (i + 1 == string_length){
						//resolve the last element spited by COMMA
						int p = 10;
						p++;
						break;
						//return result_json;
					}
					else{
						//report error
					}
				}
				else{
					//report error
				}
			}
			//*************************************parsingStatus == 1*****************************************************************************************************

		}
		else if (parsingStatus == 3){

			if ((localChar == SINGLEQUOTE && QuoteStatus == 11) || (localChar == DOUBLEQUOTES && QuoteStatus == 21)){
				json_indicator->quotes_count = 1;
				parsingStatus = 1;
			}
			else if ((localChar == SINGLEQUOTE && QuoteStatus == 13) || (localChar == DOUBLEQUOTES && QuoteStatus == 23)){
				parsingStatus = 5;
			}
		}
		else if (parsingStatus == 2){
			if ((localChar == SINGLEQUOTE && QuoteStatus == 11) || (localChar == DOUBLEQUOTES && QuoteStatus == 21)){
				parsingStatus = 4;
			}
			else{
				parsingStatus = 3;
			}
		}
		else if (parsingStatus == 4){
			if (localChar == SINGLEQUOTE && QuoteStatus == 11){
				parsingStatus = 3;
				QuoteStatus = 13;
			}
			else 	if (localChar == DOUBLEQUOTES && QuoteStatus == 21){
				parsingStatus = 3;
				QuoteStatus = 23;
			}
			else{
				json_indicator->quotes_count = 1;
				parsingStatus = 1;
			}
		}
		else if (parsingStatus == 5){
			if ((localChar == SINGLEQUOTE && QuoteStatus == 13) || (localChar == DOUBLEQUOTES && QuoteStatus == 23)){
				parsingStatus = 6;
			}
			else{
				parsingStatus = 3;
			}
		}
		else if (parsingStatus == 6){
			if ((localChar == SINGLEQUOTE && QuoteStatus == 13) || (localChar == DOUBLEQUOTES && QuoteStatus == 23)){
				json_indicator->quotes_count = 3;
				parsingStatus = 1;
			}
			else{
				parsingStatus = 3;
			}
		}


	}

	JSONIndicator* e[7];

	JSON* j[7];
	for (int i = 0; i < 7; i++){
		e[i] = json_indicators[i];
		j[i] = (JSON*)json_indicators[i]->json;
	}
	return NULL;

}



int parseStringToNubmer(char* string, int length){
	char number_char;
	int result = 0;
	for (int i = 0; i < length; i++){
		number_char = string[i];
		if (number_char >= NUMBERCHARSTART&&number_char <= NUMBERCHAREND){
			result = number_char - NUMBERCHARSTART + result * 10;
		}
		else{
			//parse error
		}
	}
	return result;
}

JSObject* parseObject(char* string, JSONIndicator* object_indicator, bool isJSKeyValue){
	JSObject* object;
	int length = object_indicator->tail - object_indicator->head;
	char *tartget_string = (char*)JSMalloc(length+1);
	for (int i = object_indicator->head + object_indicator->quotes_count; i < object_indicator->tail - object_indicator->quotes_count; i++){
		tartget_string[i - (object_indicator->head + object_indicator->quotes_count)] = string[i];
	}
	tartget_string[object_indicator->tail - object_indicator->quotes_count] = '\0';

	if (isJSKeyValue == false){

		object = new JSObject();

		if (object_indicator->quotes_count != 0){
			std::cout << "string: ";
			JSString * js_string = new JSString();
			js_string->char_string = tartget_string;
			js_string->length = length;
			object = (JSObject *)js_string;
		}
		else{
			JSNumber * js_nubmer = new JSNumber();
			js_nubmer->number = parseStringToNubmer(tartget_string, length);
			object = (JSObject *)js_nubmer;

			std::cout << "number: " << js_nubmer->number << std::endl;
		}

		std::cout << tartget_string << std::endl;

	}
	else{
		object = (JSObject*)new JSKeyValue();

		std::cout << "JSKey: ";
		std::cout << tartget_string << std::endl;
		((JSKeyValue*)object)->key = tartget_string;

		JSString * js_string = new JSString();
		js_string->char_string = tartget_string;
		js_string->length = length;
		object = (JSObject *)js_string;
	}
	return object;
}



void testJSONParse(){

	char * json_str = "[a:1,'''abc''',123,[1,2,[a:1,b:[123,[1,12],456],123],'a':2,'b:3':\"123456\"],'''a''', '''''']";
	char * json_str1 = "[123,[1,12,13,[123,[1,12],456],14,15,16,17],456,789,101112]";
	char * json_str2 = "[123,[1,'''abc'''],456,\"def\"]";
	char * json_str3 = "[123,[1,'''abc'''],\"a\":\"\"\"456\"\"\",\"def\"]";

	JSON* josn = parseJSON(json_str3, NULL);

}