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

bool checkJSON(char* string);
bool checkParenthesesMatching(char* string);
int checkJSONGrammar(char* string, int* from);

bool JSON::parse(char* string)
{
	bool parseFlag = checkJSON(string);
	if (parseFlag){

	}
	return parseFlag;
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