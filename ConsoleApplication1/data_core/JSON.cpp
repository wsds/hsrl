#include "JSON.h"

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

char leftBracket = '[';
char rightBracket = ']';
char comma = ',';
char colon = ':';
char singleQuotes = '\'';
char doubleQuotes = '"';
char decimalPoint = '.';
char numberFrom = '0';
char numberEnd = '9';








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



void testJSONParse(){

	char * json_str = "[a:1,'''abc''',123,[1,2,[a:1,b:[123,[1,12],456],123],'a':2,'b:3':\"123456\"],'''a''', '''''']";
	char * json_str1 = "[123,[1,12,13,[123,[1,12],456],14,15,16,17],456,789,101112]";

	JSONIndicator* root_json_indicator = new JSONIndicator();
	root_json_indicator->head = 0;
	root_json_indicator->tail = strlen(json_str1);

	JSON* json = new JSON();
	json->initialize();

	root_json_indicator->json = (JSObject*)json;

	JSON* josn = parseJSON(json_str1, root_json_indicator);

}
int parsingStatus = 1;//[1:normal,2:quote_start,3:string,4:second_quote_start,5:quote_stop,6:second_quote_stop],[1-2,2-3,3-1,2-4,4-5,5-3,4-1]
int QuoteStatus = 11;//[ 11 = ' , 13 = ''' , 21 = " , 23 = """]
int last_COMMA_index = 0;

char LEFTBRACKET = '[';
char RIGHTBRACKET = ']';
char SINGLEQUOTE = '\'';
char DOUBLEQUOTES = '"';

char COMMA = ',';

int json_indicators_stack_size = 10;
int json_indicators_stack_top = 0;

JSON* parseJSON(char* string, JSONIndicator* root_json_indicator){

	char localChar;
	last_COMMA_index = 0;

	JSONIndicator* json_indicator = NULL;
	JSON* result_json = NULL;

	int mem_size = json_indicators_stack_size*sizeof(JSONIndicator*);
	JSONIndicator** json_indicators = (JSONIndicator**)JSMalloc(mem_size);

	for (int i = root_json_indicator->head; i < root_json_indicator->tail; i++){
		localChar = string[i];
		if (parsingStatus == 1){
			//*************************************parsingStatus == 1*****************************************************************************************************
			if (localChar == COMMA){
				if (i>last_COMMA_index){
					JSON* json = new JSON();
					json->json_indicator = new JSONIndicator();//get from pool//to do
					json->json_indicator->head = last_COMMA_index;
					json->json_indicator->tail = i;
					json->json_indicator->json = (JSObject*)json;

					((JSON*)(json_indicator->json))->push((JSObject*)json);

					last_COMMA_index = i + 1;
					//resolve elememt
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
				JSON* json = new JSON();
				json->json_indicator = new JSONIndicator();//get from pool//to do
				json->json_indicator->head = last_COMMA_index;
				json->json_indicator->tail = i;
				json->json_indicator->json = (JSObject*)json;

				((JSON*)(json_indicator->json))->push((JSObject*)json);
				if (i + 1 == root_json_indicator->tail || string[i + 1] == COMMA){
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
					if (i + 1 == root_json_indicator->tail){
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






bool checkJSON(char* string){
	bool checkFlag = checkParenthesesMatching(string);
	if (checkFlag){
		checkFlag = (checkJSONGrammar(string, 0) >= 0);
	}
	return checkFlag;
}

const int depthCapacity = 100;

class JSONValue{
public:
	int head = -1;
	int colon = -1;
	int tail = -1;
};

class QuotationMark{
public:
	int head = -1;
	int tail = -1;
	int type = 0; // 11 = ' , 13 = ''' , 21 = " , 23 = """
};

bool checkParenthesesMatching(char* string){
	bool checkFlag = false;
	if (string[0] == leftBracket){
		int leftBracketsIndex = 0;
		int leftBracketsPositions[depthCapacity];
		bool parseQuotationMark = false;
		int quotationMarkType = 0; //11 = ',13 = ''',21 = ",23 = """
		int i;
		char ch;
		for (i = 0; ch = string[i]; i++){
			if (ch == singleQuotes){
				if (!parseQuotationMark&&!quotationMarkType){
					parseQuotationMark = true;
					if (string[i + 1] == singleQuotes && string[i + 2] == singleQuotes){
						quotationMarkType = 13;
						i += 2;
					}
					else{
						quotationMarkType = 11;
					}
				}
				else if (quotationMarkType == 13){
					if (string[i + 1] == singleQuotes && string[i + 2] == singleQuotes){
						parseQuotationMark = false;
						quotationMarkType = 0;
					}
				}
				else if (quotationMarkType == 11){
					parseQuotationMark = false;
					quotationMarkType = 0;
				}
			}
			else if (ch == doubleQuotes){
				if (!parseQuotationMark&&!quotationMarkType){
					parseQuotationMark = true;
					if (string[i + 1] == doubleQuotes && string[i + 2] == doubleQuotes){
						quotationMarkType = 23;
						i += 2;
					}
					else{
						quotationMarkType = 21;
					}
				}
				else if (quotationMarkType == 23){
					if (string[i + 1] == doubleQuotes && string[i + 2] == doubleQuotes){
						parseQuotationMark = false;
						quotationMarkType = 0;
					}
				}
				else if (quotationMarkType == 21){
					parseQuotationMark = false;
					quotationMarkType = 0;
				}
			}
			else if (!parseQuotationMark&&ch == leftBracket){
				leftBracketsPositions[leftBracketsIndex++] = i;
			}
			else if (!parseQuotationMark&&ch == rightBracket){
				int from = leftBracketsPositions[leftBracketsIndex--];
			}
		}
		if (!leftBracketsIndex && !ch && string[i - 1] == rightBracket){
			checkFlag = true;
		}
	}
	return checkFlag;
}

int checkJSONGrammar(char* string, int from){
	int checkLength = -1;
	bool parseQuotationMark = false;
	int quotationMarkType = 0; //11 = ',13 = ''',21 = ",23 = """
	char ch;
	JSONValue value;
	QuotationMark keyMark;
	QuotationMark valueMark;
	for (int i = from; ch = string[i]; i++){
		if (value.head == -1){
			value.head = i;
		}
		if (ch == singleQuotes){
			if (!parseQuotationMark){
				parseQuotationMark = true;
				if (valueMark.head == -1){
					valueMark.head = i;
				}
				else{
					break;
				}
				if (string[i + 1] == singleQuotes && string[i + 2] == singleQuotes){
					valueMark.type = 13;
					quotationMarkType = 13;
					i += 2;
				}
				else{
					valueMark.type = 11;
					quotationMarkType = 11;
				}
			}
			else{
				if (quotationMarkType == 13 && string[i + 1] == singleQuotes && string[i + 2] == singleQuotes){
					parseQuotationMark = false;
					quotationMarkType = 0;
					i += 2;
					valueMark.tail = i;
				}
				else if (quotationMarkType == 11){
					parseQuotationMark = false;
					quotationMarkType = 0;
					valueMark.tail = i;
				}
			}
		}
		else if (ch == doubleQuotes){
			if (!parseQuotationMark){
				parseQuotationMark = true;
				if (valueMark.head == -1){
					valueMark.head = i;
				}
				else{
					break;
				}
				if (string[i + 1] == doubleQuotes && string[i + 2] == doubleQuotes){
					quotationMarkType = 23;
					i += 2;
				}
				else{
					quotationMarkType = 21;
				}
			}
			else{
				if (quotationMarkType == 23 && string[i + 1] == doubleQuotes && string[i + 2] == doubleQuotes){
					parseQuotationMark = false;
					quotationMarkType = 0;
					i += 2;
					valueMark.tail = i;
				}
				else if (quotationMarkType == 21){
					parseQuotationMark = false;
					quotationMarkType = 0;
					valueMark.tail = i;
				}
			}

		}
		else if (!parseQuotationMark&&ch == colon){
			if (value.colon != -1){
				break;
			}

			if (keyMark.head == -1 && value.colon == -1){
				keyMark.head = valueMark.head;
				keyMark.tail = valueMark.tail;
				keyMark.type = valueMark.type;
				value.colon = i;
				valueMark.head = valueMark.tail = -1;
				valueMark.type = 0;
			}

			if (value.head == value.colon){
				break;
			}
		}
		else if (!parseQuotationMark&&ch == comma){
			value.tail = i;
			if (value.head != value.tail){
				if (value.colon != -1){
					if (value.colon == value.tail){
						break;
					}
					if (valueMark.head != -1){

					}
					else{
						char valueChar;
						bool checkValueChar = true;
						bool hasDecimalPoint = false;
						for (int j = value.colon + 1; j < value.tail; j++){
							valueChar = string[j];
							if (!hasDecimalPoint&&valueChar == decimalPoint){
								hasDecimalPoint = true;
							}
							else if (!(valueChar >= numberFrom &&valueChar <= numberEnd)){
								checkValueChar = false;
								break;
							}
						}
						if (!checkValueChar){
							break;
						}
					}
				}
				else{
					if (valueMark.head != -1){

					}
					else{
						char valueChar;
						bool checkValueChar = true;
						bool hasDecimalPoint = false;
						for (int j = value.head; j < value.tail; j++){
							valueChar = string[j];
							if (!hasDecimalPoint&&valueChar == decimalPoint){
								hasDecimalPoint = true;
							}
							else if (!(valueChar >= numberFrom &&valueChar <= numberEnd)){
								checkValueChar = false;
								break;
							}
						}
						if (!checkValueChar){
							break;
						}
					}
				}
			}
			value.colon = value.head = value.tail = valueMark.head = valueMark.tail = keyMark.head = keyMark.tail = -1;
			valueMark.type = keyMark.type = 0;
		}
		else if (!parseQuotationMark&&ch == leftBracket){
			i += (checkJSONGrammar(string, i + 1) - 1);
		}
		else if (!parseQuotationMark&&ch == rightBracket){
			value.tail = i;
			if (string[value.head] != leftBracket && string[value.tail] != rightBracket){
				if (value.head != value.tail){
					if (value.colon != -1){
						if (value.colon == value.tail){
							break;
						}
						if (valueMark.head != -1){

						}
						else{
							char valueChar;
							bool checkValueChar = true;
							bool hasDecimalPoint = false;
							for (int j = value.colon + 1; j < value.tail; j++){
								valueChar = string[j];
								if (!hasDecimalPoint&&valueChar == decimalPoint){
									hasDecimalPoint = true;
								}
								else if (!(valueChar >= numberFrom &&valueChar <= numberEnd)){
									checkValueChar = false;
									break;
								}
							}
							if (!checkValueChar){
								break;
							}
						}
					}
					else{
						if (valueMark.head != -1){

						}
						else{
							char valueChar;
							bool checkValueChar = true;
							bool hasDecimalPoint = false;
							for (int j = value.head; j < value.tail; j++){
								valueChar = string[j];
								if (!hasDecimalPoint&&valueChar == decimalPoint){
									hasDecimalPoint = true;
								}
								else if (!(valueChar >= numberFrom &&valueChar <= numberEnd)){
									checkValueChar = false;
									break;
								}
							}
							if (!checkValueChar){
								break;
							}
						}
					}
				}
			}
			checkLength = i + 1 - from;
			break;
		}
	}
	return checkLength;
}