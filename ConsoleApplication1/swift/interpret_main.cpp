#include "interpret_main.h"
#include "interface/log.h"
#include "FunctionsClosures\Closure.h"


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
Closure * rootClosure;

void interpret_main(){
	rootClosure = new Closure();
	rootClosure->initialize();
}

HashTable * keyWordMap;
bool is_initialized = false;
void initializeKeyWordMap(){

	JSNumber * number1 = new JSNumber();
	JSObject* number = (JSObject*)number1;
	number->number = 888006;


	keyWordMap = new HashTable();
	keyWordMap->initialize();


	keyWordMap->set("var", number);

	keyWordMap->set("if", number);
	keyWordMap->set("else", number);

	keyWordMap->set("for", number);
	keyWordMap->set("in", number);

	keyWordMap->set("while", number);
	keyWordMap->set("do", number);

	keyWordMap->set("func", number);
	keyWordMap->set("return", number);

	keyWordMap->set("class", number);
	keyWordMap->set("instance", number);
	keyWordMap->set("this", number);
	keyWordMap->set("self", number);
	is_initialized = true;
}

bool isKeyWord(char* name){
	if (!is_initialized){
		initializeKeyWordMap();
	}
	JSObject* number = keyWordMap->get(name);
	if (number == NULL){
		return false;
	}
	else{
		return true;
	}
}
int BRACKET_element_counter = 0;
int SINGLEQUOTE_element_counter = 0;
int DOUBLEQUOTES_element_counter = 0;
int resolveElementStatus = 1;//[1:normal,2:json,[321,323,311,313]:string,1-2,2-1,1-3,3-1]

int resolveElement(char* from, int length, CodeLine* codeLine){

	char localChar;
	int codeElementType = CODE_NUMBER;

	BRACKET_element_counter = 0;

	int pre_blank = 0;
	if (resolveElementStatus != 1){
		localChar = 10;
	}

	for (int i = 0; i < length; i++){
		localChar = from[i];
		if (localChar == BLANK){
			pre_blank++;
		}
		else{
			break;
		}
	}
	for (int i = pre_blank; i < length - pre_blank; i++){
		localChar = from[i];
		if (resolveElementStatus == 1){
			if (localChar == BLANK){
				//resolve the left code
				int elementCount = resolveElement(from, i, codeLine);

				//resolve the right code
				elementCount = elementCount + resolveElement(from + i, length - i, codeLine);

				return elementCount;

			}
			else if (localChar == SINGLEQUOTE){
				resolveElementStatus = 311;
			}
			else if (localChar == DOUBLEQUOTES){
				resolveElementStatus = 321;
			}
			else if (localChar == LEFTBRACKET){
				BRACKET_element_counter++;
				resolveElementStatus = 2;
			}
			else if (localChar == RIGHTBRACKET){
				//report error
			}
		}
		else if (resolveElementStatus == 2){
			if (localChar == RIGHTBRACKET){
				BRACKET_element_counter--;
				if (BRACKET_element_counter == 0){
					resolveElementStatus = 1;
					codeElementType = CODE_JSON;
				}
			}
			else if (localChar == LEFTBRACKET){
				BRACKET_element_counter++;
			}

		}
		else if (int(resolveElementStatus / 100) == 3){
			if (resolveElementStatus == 311){
				if (localChar == SINGLEQUOTE){
					resolveElementStatus = 1;
					codeElementType = CODE_STRING;
				}
			}

			else if (resolveElementStatus == 321){
				if (localChar == DOUBLEQUOTES){
					resolveElementStatus = 1;
					codeElementType = CODE_STRING;
				}
			}
		}

	}
	CodeElement * element = new CodeElement();
	codeLine->codeElements[codeLine->element_index] = element;
	codeLine->element_index++;

	if (codeElementType == CODE_STRING){
		char* string = (char*)JSMalloc(length - pre_blank - 2);
		strcopy(from + pre_blank + 1, string, length - pre_blank - 3);

		element->type = CODE_STRING;
		element->char_string = string;

		std::cout << "[string]: ";
		std::cout << string << std::endl;
	}
	else {
		char* name = (char*)JSMalloc(length - pre_blank);
		strcopy(from + pre_blank, name, length - pre_blank);
		if (codeElementType == CODE_JSON){

			element->type = CODE_JSON;
			element->jsonstr = name;

			std::cout << "[json]: ";
			std::cout << name << std::endl;
		}
		else if (isNumber(name)){

			element->type = CODE_NUMBER;
			element->number = parseStringToNubmer(name, strlen(name));

			std::cout << "[number]: ";
			std::cout << name << std::endl;
		}

		else if (isKeyWord(name)){

			element->type = KEYWORD;
			element->keyword = name;

			std::cout << "[key word]: ";
			std::cout << name << std::endl;
		}
		else{
			CodeElement * string = new CodeElement();

			element->type = NAME;
			element->variable_name = name;

			std::cout << "[variable name]: ";
			std::cout << name << std::endl;
		}
	}

	return 1;
}

char* string_var = "var";

void resolveCodeLine(char* line){

	CodeLine* codeLine = new CodeLine();
	Assignment * assignment = NULL;
	//CodeElement** codeElements = (CodeElement**)JSMalloc(8);

	char localChar;
	int string_length = strlen(line);

	for (int ii = 0; ii < string_length; ii++){
		localChar = line[ii];
		if (localChar == EQUALITY){
			assignment = new Assignment();
			assignment->isNew = false;

			//"="
			CodeElement * equality = new CodeElement();
			codeLine->codeElements[codeLine->element_index] = equality;
			codeLine->element_index++;
			equality->type = CODEOPERATOR;
			equality->code_operator = localChar;
			assignment->codeOperator = equality;

			//resolve the left code
			int elementCount = resolveElement(line, ii, codeLine);
			for (int i = codeLine->element_index - elementCount; i < codeLine->element_index; i++){
				if (codeLine->codeElements[i]->type == NAME){
					assignment->left = codeLine->codeElements[i];
				}
				else if (codeLine->codeElements[i]->type == KEYWORD && 0 == strcmp(string_var, codeLine->codeElements[i]->keyword)){
					assignment->isNew = true;
				}
			}

			//resolve the right code
			elementCount = resolveElement(line + ii + 1, string_length - ii, codeLine);
			for (int i = codeLine->element_index - elementCount; i < codeLine->element_index; i++){
				if (codeLine->codeElements[i]->type == NAME){
					assignment->right = codeLine->codeElements[i];
				}
				else if (codeLine->codeElements[i]->type == CODE_NUMBER){
					assignment->right = codeLine->codeElements[i];
				}
				else if (codeLine->codeElements[i]->type == CODE_STRING){
					assignment->right = codeLine->codeElements[i];
				}
				else if (codeLine->codeElements[i]->type == CODE_JSON){
					assignment->right = codeLine->codeElements[i];
				}
			}

		}
	}

	std::cout << "element_index: " << codeLine->element_index << std::endl;
	if (assignment != NULL){
		excute(assignment);
	}
}

void excute(Assignment * assignment){
	JSObject * jsObject;
	if (assignment->right->type == CODE_NUMBER){
		jsObject = (JSObject *)(new JSNumber(assignment->right->number));
	}
	else if (assignment->right->type == CODE_STRING){
		jsObject = (JSObject *)(new JSString(assignment->right->char_string));
	}
	else if (assignment->right->type == CODE_JSON){
		JSON* json = parseJSON(assignment->right->jsonstr);
		jsObject = (JSObject *)json;
	}
	else if (assignment->right->type == NAME){
		JSKeyValue *jsKeyValue = (JSKeyValue *)rootClosure->variables->get(assignment->right->variable_name);
		if (jsKeyValue == NULL){
			//report error
		}
		else{
			jsObject = jsKeyValue->value;
		}
	}
	if (assignment->isNew){
		rootClosure->variables->set(assignment->left->variable_name, jsObject);
	}
	else{
		jsObject = rootClosure->variables->get(assignment->right->variable_name);
		if (jsObject == NULL){
			//report error
		}
		else{
			//replace or modify???
			rootClosure->variables->set(assignment->left->variable_name, jsObject);
		}
	}
}