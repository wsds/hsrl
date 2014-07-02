#include "interpret_main.h"
#include "interface/log.h"



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
				resolveElement(from, i, codeLine);

				//resolve the right code
				resolveElement(from + i, length - i, codeLine);

				return 1;

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

	if (codeElementType == CODE_STRING){
		char* string = (char*)JSMalloc(length - pre_blank - 2);
		strcopy(from + pre_blank + 1, string, length - pre_blank - 3);


		std::cout << "[string]: ";
		std::cout << string << std::endl;
	}
	else {
		char* name = (char*)JSMalloc(length - pre_blank);
		strcopy(from + pre_blank, name, length - pre_blank);
		if (codeElementType == CODE_JSON){



			std::cout << "[json]: ";
			std::cout << name << std::endl;
		}
		else if (isNumber(name)){



			std::cout << "[number]: ";
			std::cout << name << std::endl;
		}

		else if (isKeyWord(name)){

			CodeElement * keyWord = new CodeElement();
			codeLine->codeElements[codeLine->element_index] = keyWord;
			codeLine->element_index++;

			keyWord->type = KEYWORD;
			keyWord->keyword = name;

			std::cout << "[key word]: ";
			std::cout << name << std::endl;
		}
		else{
			CodeElement * string = new CodeElement();
			codeLine->codeElements[codeLine->element_index] = string;
			codeLine->element_index++;

			string->type = NAME;
			string->name = name;

			std::cout << "[name]: ";
			std::cout << name << std::endl;
		}
	}

	return 1;
}

void resolveCodeLine(char* line){

	CodeLine* codeLine = new CodeLine();
	//CodeElement** codeElements = (CodeElement**)JSMalloc(8);

	char localChar;
	int string_length = strlen(line);


	for (int i = 0; i < string_length; i++){
		localChar = line[i];
		if (localChar == EQUALITY){
			Assignment * assignment = new Assignment();


			//"="
			CodeElement * equality = new CodeElement();
			codeLine->codeElements[codeLine->element_index] = equality;
			codeLine->element_index++;

			equality->type = CODEOPERATOR;
			equality->code_operator = localChar;

			assignment->codeOperator = equality;

			//resolve the left code
			resolveElement(line, i, codeLine);

			//resolve the right code
			resolveElement(line + i + 1, string_length - i, codeLine);

		}
	}

	std::cout << "element_index: " << codeLine->element_index << std::endl;;

}