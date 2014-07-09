#include "interpret_main.h"
#include "interface/log.h"
#include "FunctionsClosures\Closure.h"


Assignment::Assignment(){
	this->type = ASSIGNMENT;
}

FunctionCall::FunctionCall(){
	this->type = FUNCTIONCALL;
}

FunctionDefinition::FunctionDefinition(){
	this->executable_index = 0;
	this->type = FUNCTIONDEFINITION;
}

Condition::Condition(){
	this->type = CONDITION;
}

IfBlock::IfBlock(){
	this->type = IFBLOCK;
	this->conditions_index = 0;
	this->executable_index = 0;
	this->else_executable_index = 0;

	this->next = NULL;
}

Closure * rootClosure;
Closure * currentClosure;
int funtionLevel;
FunctionDefinition * currentFunctionDefinition;

void interpret_main(){
	funtionLevel = 0;
	currentFunctionDefinition = NULL;

	Import* import = Import::getInstance();
	import->regiditFunctions();
	rootClosure = import->rootClosure;
	currentClosure = rootClosure;
}

KeyWords::KeyWords(){
	string_func = "func";



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

	int tail_blank = 0;

	for (int i = length - 1; i > 0; i--){
		localChar = from[i];
		if (localChar == BLANK){
			tail_blank++;
		}
		else{
			break;
		}
	}
	if (pre_blank + tail_blank == length){
		return 0;
	}

	for (int i = pre_blank; i < length - tail_blank; i++){
		localChar = from[i];
		if (resolveElementStatus == 1){
			if (localChar == BLANK){
				//resolve the left code
				int elementCount = resolveElement(from, i, codeLine);

				//resolve the right code
				elementCount = elementCount + resolveElement(from + i, length - i, codeLine);

				return elementCount;

			}
			else if (localChar == COMMA || localChar == SEMICOLON){

				//resolve the left code
				int elementCount = resolveElement(from, i, codeLine);

				CodeElement * delimiter = new CodeElement();
				codeLine->codeElements[codeLine->element_index] = delimiter;
				codeLine->element_index++;
				delimiter->type = DELIMITER;
				//resolve the right code
				elementCount = elementCount + 1 + resolveElement(from + i + 1, length - i - 1, codeLine);

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
			else if (localChar == LEFTSMALLBRACKET || localChar == RIGHTSMALLBRACKET || localChar == LEFTBIGBRACKET || localChar == RIGHTBIGBRACKET){
				//resolve the left code
				int elementCount = resolveElement(from, i, codeLine);

				CodeElement * bracket = new CodeElement();
				codeLine->codeElements[codeLine->element_index] = bracket;
				bracket->code_operator = localChar;
				codeLine->element_index++;
				bracket->type = BRACKET;

				//resolve the right code
				elementCount = elementCount + 1 + resolveElement(from + i + 1, length - i - 1, codeLine);

				return elementCount;

			}
			else if ((64 >= localChar && localChar >= 58) || (47 >= localChar && localChar >= 32) || localChar == 94 || localChar == 124){
				//resolve the left code
				int elementCount = resolveElement(from, i, codeLine);

				CodeElement * code_operator = new CodeElement();
				codeLine->codeElements[codeLine->element_index] = code_operator;
				code_operator->code_operator = localChar;
				codeLine->element_index++;
				code_operator->type = CODEOPERATOR;

				//resolve the right code
				elementCount = elementCount + 1 + resolveElement(from + i + 1, length - i - 1, codeLine);

				return elementCount;
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
		char* string = (char*)JSMalloc(length - pre_blank - tail_blank - 1);
		strcopy(from + pre_blank + 1, string, length - pre_blank - tail_blank - 2);

		element->type = CODE_STRING;
		element->char_string = string;

		//std::cout << "[string]: ";
		//std::cout << string << std::endl;
	}
	else {
		char* name = (char*)JSMalloc(length - pre_blank - tail_blank);
		strcopy(from + pre_blank, name, length - pre_blank - tail_blank);
		if (codeElementType == CODE_JSON){

			element->type = CODE_JSON;
			element->jsonstr = name;

			//std::cout << "[json]: ";
			//std::cout << name << std::endl;
		}
		else if (isNumber(name)){

			element->type = CODE_NUMBER;
			element->number = parseStringToNubmer(name, strlen(name));

			//std::cout << "[number]: ";
			//std::cout << name << std::endl;
		}

		else if (isKeyWord(name)){

			element->type = KEYWORD;
			element->keyword = name;

			//std::cout << "[key word]: ";
			//std::cout << name << std::endl;
		}
		else{
			CodeElement * string = new CodeElement();

			element->type = NAME;
			element->variable_name = name;

			//std::cout << "[variable name]: ";
			//std::cout << name << std::endl;
		}
	}

	return 1;
}

char* string_var = "var";
char* string_func = "func";
void resolveAssignment(char* line){

	CodeLine* codeLine = new CodeLine();
	Assignment * assignment = NULL;

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
			elementCount = resolveElement(line + ii + 1, string_length - ii - 1, codeLine);
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

	//std::cout << "element_index: " << codeLine->element_index << std::endl;
	if (assignment != NULL){
		if (funtionLevel == 0){
			excute(assignment);
		}
		else if (currentFunctionDefinition != NULL){
			currentFunctionDefinition->executables[currentFunctionDefinition->executable_index] = assignment;
			currentFunctionDefinition->executable_index++;
		}
		else{
			//report error
		}
	}

}

void resolveFunctionCall(char* line){
	CodeLine* codeLine = new CodeLine();
	FunctionCall * functionCall = NULL;
	FunctionDefinition * functionDefinition = NULL;

	char localChar;
	int string_length = strlen(line);

	int index_LEFTSMALLBRACKET = 0;
	int index_RIGHTSMALLBRACKET = 0;

	for (int ii = 0; ii < string_length; ii++){
		localChar = line[ii];
		if (localChar == LEFTSMALLBRACKET){
			index_LEFTSMALLBRACKET = ii;
		}
		else if (localChar == RIGHTSMALLBRACKET){
			index_RIGHTSMALLBRACKET = ii;
		}
		else if (localChar == LEFTBIGBRACKET){
			funtionLevel++;
			functionDefinition = new FunctionDefinition();
			currentFunctionDefinition = functionDefinition;
		}
		else if (localChar == RIGHTBIGBRACKET){
			funtionLevel--;
			if (funtionLevel == 0){
				currentFunctionDefinition = NULL;
			}
			else if (funtionLevel < 0){
				//report error
			}
		}
	}

	if (index_RIGHTSMALLBRACKET > index_LEFTSMALLBRACKET && index_LEFTSMALLBRACKET != 0){

		functionCall = new FunctionCall();

		//resolve the small brackt outer code
		int elementCount = resolveElement(line, index_LEFTSMALLBRACKET, codeLine);
		for (int i = codeLine->element_index - elementCount; i < codeLine->element_index; i++){
			if (codeLine->codeElements[i]->type == NAME){
				functionCall->functionName = codeLine->codeElements[i];
			}
			else if (codeLine->codeElements[i]->type == KEYWORD && 0 == strcmp(string_func, codeLine->codeElements[i]->keyword)){
			}
		}

		//resolve the small brackt inner code
		elementCount = resolveElement(line + index_LEFTSMALLBRACKET + 1, index_RIGHTSMALLBRACKET - index_LEFTSMALLBRACKET - 1, codeLine);
		for (int i = codeLine->element_index - elementCount; i < codeLine->element_index; i++){
			if (codeLine->codeElements[i]->type == NAME){
				functionCall->variables = codeLine->codeElements[i];
			}
			else if (codeLine->codeElements[i]->type == CODE_NUMBER){
				functionCall->variables = codeLine->codeElements[i];
			}
			else if (codeLine->codeElements[i]->type == CODE_STRING){
				functionCall->variables = codeLine->codeElements[i];
			}
			else if (codeLine->codeElements[i]->type == CODE_JSON){
				functionCall->variables = codeLine->codeElements[i];
			}
		}
	}

	//std::cout << "element_index: " << codeLine->element_index << std::endl;
	if (functionDefinition != NULL){
		functionDefinition->functionName = functionCall->functionName;
		functionDefinition->variables = functionCall->variables;
		std::cout << "excute functionDefinition: " << functionDefinition->functionName->variable_name << std::endl;
		excute(functionDefinition);
	}
	else if (functionCall != NULL){

		if (funtionLevel == 0){
			std::cout << "excute functionCall: " << functionCall->functionName->variable_name << std::endl;
			excute(functionCall);
		}
		else if (currentFunctionDefinition != NULL){
			currentFunctionDefinition->executables[currentFunctionDefinition->executable_index] = functionCall;
			currentFunctionDefinition->executable_index++;
		}
		else{
			//report error
		}

	}

}

void resolveOperators(CodeLine* codeLine){

	for (int i = 0; i < codeLine->element_index - 1; i++){
		if (codeLine->codeElements[i]->type == CODEOPERATOR&&codeLine->codeElements[i + 1]->type == CODEOPERATOR){
			codeLine->codeElements[i]->code_operator2 = codeLine->codeElements[i + 1]->code_operator;
			codeLine->codeElements[i + 1]->type = SKIP;
		}
	}

}

void resolveCodeLine(char* line){
	//CodeLine* codeLine = new CodeLine();
	//codeLine->element_index = 0;
	//int string_length = strlen(line);

	//int result = resolveElement(line, string_length, codeLine);

	//resolveOperators(codeLine);
	resolveAssignment(line);
	resolveFunctionCall(line);
}


void excute(FunctionDefinition * functionDefinition){
	JSFunction * jsFunction = new JSFunction();
	jsFunction->function_name = functionDefinition->functionName->variable_name;
	jsFunction->functionDefinition = functionDefinition;


	currentClosure->set(functionDefinition->functionName->variable_name, jsFunction);
	JSKeyValue * jsFunctionKeyValue = (JSKeyValue *)currentClosure->lookup(functionDefinition->functionName->variable_name);
	if (jsFunctionKeyValue == NULL || ((JSObject*)jsFunctionKeyValue)->type != JSKEYVALUE){
		//report error
		return;
	}
}


void excute(FunctionCall * functionCall){

	if (functionCall->functionName->type != NAME){
		//report error
		return;
	}
	JSObject * jsVariables;
	if (functionCall->variables->type == CODE_NUMBER){
		jsVariables = (JSObject *)(new JSNumber(functionCall->variables->number));
	}
	else if (functionCall->variables->type == CODE_STRING){
		jsVariables = (JSObject *)(new JSString(functionCall->variables->char_string));
	}
	else if (functionCall->variables->type == CODE_JSON){
		JSON* json = parseJSON(functionCall->variables->jsonstr);
		jsVariables = (JSObject *)json;
	}
	else if (functionCall->variables->type == NAME){
		JSKeyValue *jsKeyValue = (JSKeyValue *)currentClosure->lookup(functionCall->variables->variable_name);
		if (jsKeyValue == NULL){
			//report error
			//return;
		}
		else{
			jsVariables = jsKeyValue->value;
		}
	}

	JSKeyValue * jsFunctionKeyValue;
	jsFunctionKeyValue = (JSKeyValue *)currentClosure->lookup(functionCall->functionName->variable_name);
	if (jsFunctionKeyValue == NULL || ((JSObject*)jsFunctionKeyValue)->type != JSKEYVALUE){
		//report error
		return;
	}
	JSFunction * jsFunction = (JSFunction *)jsFunctionKeyValue->value;
	if (jsFunction == NULL || ((JSObject*)jsFunction)->type != JSFUNCTION){
		//report error
		return;
	}
	else{
		JSON* parameter = new JSON();
		parameter->initialize();
		parameter->push(jsVariables);
		if (jsFunction->function != NULL){
			JSON* result = jsFunction->function(parameter);
		}
		else if (jsFunction->functionDefinition != NULL){
			excuteFunction(jsFunction->functionDefinition, parameter);
		}

	}

}

void excuteFunction(FunctionDefinition * functionDefinition, JSON* parameter){

	Closure * closure = new Closure();
	closure->initialize();
	currentClosure->next = closure;
	closure->previous = currentClosure;

	closure->set(functionDefinition->variables->variable_name, parameter->pop());


	currentClosure = currentClosure->next;

	Executable * executable;
	for (int i = 0; i < functionDefinition->executable_index; i++){
		executable = functionDefinition->executables[i];
		if (executable->type == ASSIGNMENT){
			excute((Assignment*)executable);
		}
		else if (executable->type == FUNCTIONCALL){
			excute((FunctionCall*)executable);
		}
		else if (executable->type == FUNCTIONDEFINITION){
			excute((FunctionDefinition*)executable);
		}
	}

	currentClosure = currentClosure->previous;
}

void excute(Assignment * assignment){
	JSObject * rightValue;
	if (assignment->right->type == CODE_NUMBER){
		rightValue = (JSObject *)(new JSNumber(assignment->right->number));
	}
	else if (assignment->right->type == CODE_STRING){
		rightValue = (JSObject *)(new JSString(assignment->right->char_string));
	}
	else if (assignment->right->type == CODE_JSON){
		JSON* json = parseJSON(assignment->right->jsonstr);
		rightValue = (JSObject *)json;
		//log((JSObject*)json);
	}
	else if (assignment->right->type == NAME){
		JSKeyValue *jsKeyValue = (JSKeyValue *)currentClosure->lookup(assignment->right->variable_name);
		if (jsKeyValue == NULL){
			//report error
		}
		else{
			rightValue = jsKeyValue->value;
		}
	}
	JSObject * leftVariable;
	if (assignment->isNew){

		currentClosure->set(assignment->left->variable_name, rightValue);
	}
	else{
		leftVariable = currentClosure->lookup(assignment->right->variable_name);
		if (leftVariable == NULL){
			//report error
		}
		else{
			//replace or modify???
			currentClosure->set(assignment->left->variable_name, rightValue);
		}
	}
}

void getAllVariablesToString(){
	Closure * current = currentClosure;
	int currentLen = 0;
	int i = 0;
	while (current){
		currentLen = current->variables->list->length;
		JSObject* object;
		if (i == 0){
			open::logBuf("2.");
			open::logBuf("current closure->");
			open::logBufFlush();
		}
		else{
			open::logBuf("2.");
			open::logBuf("parent->");
			open::logBufFlush();
		}
		i++;
		for (int i = 0; i < currentLen; i++){
			object = current->variables->find(i);
			JSObject* value = ((JSKeyValue*)object)->value;
			if (value->type == JSSTRING){
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf("=");
				open::logBuf(((JSObject*)value)->char_string);
				open::logBufFlush();
			}
			else if (value->type == JSNUMBER){
				char num[12];
				parseNubmerToString(((JSObject*)value)->number, num);
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf("=");
				open::logBuf(num);
				open::logBufFlush();
			}
			else if (value->type == JSJSON){
				char* jsonStr = stringifyJSON((JSON*)value);
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf("=");
				open::logBuf(jsonStr);
				open::logBufFlush();
			}
		}
		current = current->previous;
	}
}