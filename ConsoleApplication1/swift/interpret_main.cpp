#include "interpret_main.h"
#include "interface/log.h"
#include "FunctionsClosures\Closure.h"


MetaExecutable::MetaExecutable(){
	this->type = META;
}
ExcutedExecutable::ExcutedExecutable(){
	this->type = EXCUTED;
}
Operator::Operator(){
	this->type = OPERATOR;
	this->code_operator = 0;
	this->code_operator2 = 0;
}

Expression::Expression(){
	this->type = EXPRESSION;

	this->executable_index = 0;
}
FunctionCall1::FunctionCall1(){
	this->type = FUNCTIONCALL;
	this->variable_index = 0;
}
FunctionDefinition1::FunctionDefinition1(){
	this->type = FUNCTIONDEFINITION;
}
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

ExecutableBlock::ExecutableBlock(){
	this->isHolded = false;
	this->executable_index = 0;
	this->type = EXCUTEABLEBLOCK;
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

ForInBlock::ForInBlock(){
	this->type = FORINBLOCK;
	this->executable_index = 0;
}

KeyWords * keyWords;
Closure * rootClosure;
Closure * currentClosure;
int funtionLevel;
FunctionDefinition * currentFunctionDefinition;

ExecutableBlock * currentExecutableBlock;

void interpret_main(){
	funtionLevel = 0;
	currentFunctionDefinition = NULL;
	keyWords = KeyWords::getInstance();

	Import* import = Import::getInstance();
	import->regiditFunctions();
	rootClosure = import->rootClosure;
	currentClosure = rootClosure;

	currentExecutableBlock = new ExecutableBlock();
}
KeyWords * KeyWords::instance = NULL;
KeyWords::KeyWords(){

	this->keyWordMap = new HashTable();
	this->keyWordMap->initialize();

	this->string_var = "var";

	this->string_if = "if";
	this->string_else = "else";

	this->string_for = "for";
	this->string_in = "in";
	this->string_while = "while";
	this->string_do = "do";


	this->string_func = "func";
	this->string_return = "return";

	this->string_class = "class";
	this->string_instance = "instance";
	this->string_this = "this";
	this->string_self = "self";


}
bool is_initialized = false;
void initializeKeyWordMap(KeyWords * keyWords){

	HashTable * keyWordMap = keyWords->keyWordMap;

	JSNumber * number1 = new JSNumber();
	JSObject* number = (JSObject*)number1;
	number->number = 888006;

	keyWordMap->set(keyWords->string_var, number);

	keyWordMap->set(keyWords->string_if, number);
	keyWordMap->set(keyWords->string_else, number);

	keyWordMap->set(keyWords->string_for, number);
	keyWordMap->set(keyWords->string_in, number);

	keyWordMap->set(keyWords->string_while, number);
	keyWordMap->set(keyWords->string_do, number);

	keyWordMap->set(keyWords->string_func, number);
	keyWordMap->set(keyWords->string_return, number);

	keyWordMap->set(keyWords->string_class, number);
	keyWordMap->set(keyWords->string_instance, number);
	keyWordMap->set(keyWords->string_this, number);
	keyWordMap->set(keyWords->string_self, number);
	is_initialized = true;
}

bool isKeyWord(char* name){
	if (!is_initialized){
		initializeKeyWordMap(keyWords);
	}
	JSObject* number = keyWords->keyWordMap->get(name);
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
				delimiter->isResolvedDelimiter = false;
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
				bracket->bracket = localChar;
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


Executable*  analyzeCodeLine(CodeLine * codeLine, int from, int end){
	Executable* executable = NULL;
	CodeElement* codeElement = NULL;
	Expression* expressionDEBUG;

	FunctionCall1 * functionCall = NULL;
	FunctionDefinition1 * functionDefinition = NULL;
	ForInBlock * forInBlock = NULL;

	int skipToIndex = -1;
	for (int i = from; i < end; i++){
		if (i < skipToIndex){
			continue;
		}
		codeElement = codeLine->codeElements[i];
		if (codeElement->type == KEYWORD){

			if (0 == strcmp(keyWords->string_func, codeElement->keyword)){

				functionDefinition = new FunctionDefinition1();
				//executable = functionDefinition;
				if (currentExecutableBlock->isHolded == false){
					functionDefinition->executableBlock = currentExecutableBlock;
					currentExecutableBlock->isHolded = false;
					if (i + 1 < end && codeLine->codeElements[i + 1]->type == NAME){
						functionDefinition->functionName = codeLine->codeElements[i + 1]->variable_name;
					}
				}
				else{
					//report error
				}
			}
			else if (0 == strcmp(keyWords->string_class, codeElement->keyword)){

			}
			else if (0 == strcmp(keyWords->string_instance, codeElement->keyword)){

			}
			else if (0 == strcmp(keyWords->string_for, codeElement->keyword)){
				bool isForIn = false;
				for (int ii = i + 1; ii < end; ii++){
					CodeElement * innerElement = codeLine->codeElements[ii];
					if (innerElement->type == KEYWORD && 0 == strcmp(keyWords->string_in, innerElement->keyword)){
						isForIn = true;
					}
				}

				if (isForIn == true){
					forInBlock = new ForInBlock();
				}

			}
		}
		else if (codeElement->type == BRACKET){

			if (codeElement->bracket == LEFTSMALLBRACKET){
				skipToIndex = codeElement->nextBracketIndex;
			}
			else if (codeElement->bracket == RIGHTSMALLBRACKET){
				CodeElement * preBracket = codeLine->codeElements[codeElement->preBracketIndex];
				if (codeElement->preBracketIndex - 1 >= 0 && codeLine->codeElements[codeElement->preBracketIndex - 1]->type == NAME){
					functionCall = new FunctionCall1();
					//executable = functionCall;

					functionCall->functionName = codeLine->codeElements[codeElement->preBracketIndex - 1]->variable_name;
					int lastDelimiterindex = preBracket->preBracketIndex + 1;
					for (int ii = lastDelimiterindex; ii < preBracket->nextBracketIndex; ii++){
						CodeElement * innerElement = codeLine->codeElements[ii];
						if (innerElement->type == DELIMITER&&innerElement->isResolvedDelimiter == false){

							Executable* innerExecutable = analyzeCodeLine(codeLine, lastDelimiterindex, ii);
							functionCall->variables[functionCall->variable_index] = innerExecutable;
							functionCall->variable_index++;

							lastDelimiterindex = ii + 1;
							innerElement->isResolvedDelimiter = true;
						}
						else if (ii == preBracket->nextBracketIndex - 1){
							Executable* innerExecutable = analyzeCodeLine(codeLine, lastDelimiterindex, ii + 1);
							functionCall->variables[functionCall->variable_index] = innerExecutable;
							functionCall->variable_index++;
						}
					}

					if (executable == NULL){
						executable = new Expression();
						expressionDEBUG = (Expression*)executable;
					}
					else if (executable->type != EXPRESSION){
						//report error
					}

					Expression* expression = (Expression*)executable;

					expression->executable[expression->executable_index] = functionCall;
					expression->executable_index++;

				}
				else{


					ExecutableBlock * executableBlock = new ExecutableBlock();
					//executable = executableBlock;

					int lastDelimiterindex = preBracket->preBracketIndex + 1;
					for (int ii = lastDelimiterindex; ii < preBracket->nextBracketIndex; ii++){
						CodeElement * innerElement = codeLine->codeElements[ii];
						if (innerElement->type == DELIMITER&&innerElement->isResolvedDelimiter == false){

							Executable* innerExecutable = analyzeCodeLine(codeLine, lastDelimiterindex, ii);
							executableBlock->executables[executableBlock->executable_index] = innerExecutable;
							executableBlock->executable_index++;

							lastDelimiterindex = ii + 1;
							innerElement->isResolvedDelimiter = true;
						}
						else if (ii == preBracket->nextBracketIndex - 1){
							Executable* innerExecutable = analyzeCodeLine(codeLine, lastDelimiterindex, ii + 1);
							executableBlock->executables[executableBlock->executable_index] = innerExecutable;
							executableBlock->executable_index++;
						}
					}

					if (executable == NULL){
						executable = new Expression();
						expressionDEBUG = (Expression*)executable;
					}
					else if (executable->type != EXPRESSION){
						//report error
					}

					Expression* expression = (Expression*)executable;

					expression->executable[expression->executable_index] = executableBlock;
					expression->executable_index++;

				}

			}

		}


		else if (codeElement->type == CODEOPERATOR){
			if (executable == NULL){
				executable = new Expression();
				expressionDEBUG = (Expression*)executable;
			}
			else if (executable->type != EXPRESSION){
				//report error
			}

			Expression* expression = (Expression*)executable;

			Operator * code_operator = new Operator();
			code_operator->code_operator = codeElement->code_operator;
			code_operator->code_operator2 = codeElement->code_operator2;

			expression->executable[expression->executable_index] = code_operator;
			expression->executable_index++;


			//Executable* innerExecutable = analyzeCodeLine(codeLine, i-1, i);

		}

		else if (codeElement->type == NAME || codeElement->type == CODE_NUMBER ||
			codeElement->type == CODE_STRING || codeElement->type == CODE_JSON){
			if (i + 1 < end && codeLine->codeElements[i + 1]->type == BRACKET){
				if (codeLine->codeElements[i + 1]->bracket == LEFTSMALLBRACKET){
					continue;
				}
			}

			MetaExecutable * metaExecutable = new MetaExecutable();

			metaExecutable->codeElement = codeElement;

			if (end - from <= 1){
				executable = metaExecutable;
			}
			else{
				if (executable == NULL){
					executable = new Expression();
					expressionDEBUG = (Expression*)executable;
				}
				else if (executable->type != EXPRESSION){
					//report error
				}

				Expression* expression = (Expression*)executable;


				expression->executable[expression->executable_index] = metaExecutable;
				expression->executable_index++;
			}
		}
	}

	if (functionDefinition != NULL){
		if (0 != strcmp(functionCall->functionName, functionDefinition->functionName)){
			//report error;
		}
		for (int i = 0; i < functionCall->variable_index; i++){
			functionDefinition->variables[i] = functionCall->variables[i];
		}
		functionDefinition->variable_index = functionCall->variable_index;
		executable = functionDefinition;
	}
	else if (forInBlock != NULL){
		Expression* expression = (Expression*)executable;
		if (expression->executable_index == 2){
			if (expression->executable[0]->type == EXCUTEABLEBLOCK){
				ExecutableBlock* executableBlock = (ExecutableBlock*)expression->executable[0];
				if (executableBlock->executable_index == 2){
					MetaExecutable* keyNameExecutable = (MetaExecutable*)executableBlock->executables[0];
					MetaExecutable* valueNameExecutable = (MetaExecutable*)executableBlock->executables[1];

					forInBlock->keyName = keyNameExecutable->codeElement;
					forInBlock->valueName = valueNameExecutable->codeElement;
				}
			}
			if (expression->executable[1]->type == META){
				MetaExecutable* iteratorNameExecutable = (MetaExecutable*)expression->executable[1];
				forInBlock->iteratorName = iteratorNameExecutable->codeElement;
			}
		}
		DEBUGExecutable * iDEBUGExecutable = debugExecutable(expression);
		executable = forInBlock;
		int i = 1 + 1;
	}

	expressionDEBUG = (Expression*)executable;
	return executable;

}

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
				else if (codeLine->codeElements[i]->type == KEYWORD && 0 == strcmp(keyWords->string_var, codeLine->codeElements[i]->keyword)){
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
			else if (codeLine->codeElements[i]->type == KEYWORD && 0 == strcmp(keyWords->string_func, codeLine->codeElements[i]->keyword)){
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

void resolveBracket(CodeLine* codeLine){
	ExecutableBlock* executableBlock;
	int index_LEFTSMALLBRACKET = 0;
	int index_RIGHTSMALLBRACKET = 0;

	CodeElement * codeElement;

	CodeElement * smallBrackets[5];
	int smallBracketsStackTop = 0;

	for (int ii = 0; ii < codeLine->element_index; ii++){
		codeElement = codeLine->codeElements[ii];
		if (codeElement->type != BRACKET){
			continue;
		}
		if (codeElement->bracket == LEFTSMALLBRACKET){
			smallBrackets[smallBracketsStackTop] = codeElement;
			codeElement->preBracketIndex = ii;
			smallBracketsStackTop++;
		}
		else if (codeElement->bracket == RIGHTSMALLBRACKET){
			codeElement->preBracketIndex = smallBrackets[smallBracketsStackTop - 1]->preBracketIndex;
			codeElement->nextBracketIndex = ii;
			smallBrackets[smallBracketsStackTop - 1]->nextBracketIndex = ii;

			smallBracketsStackTop--;
			if (smallBracketsStackTop < 0){
				break;
				//report error
			}
		}//todo many bracket
		else if (codeElement->bracket == LEFTBIGBRACKET){
			funtionLevel++;
			executableBlock = new ExecutableBlock();
			currentExecutableBlock = executableBlock;
		}
		else if (codeElement->bracket == RIGHTBIGBRACKET){
			funtionLevel--;
			if (funtionLevel == 0){
				currentExecutableBlock = NULL;
			}
			else if (funtionLevel < 0){
				break;
				//report error
			}
		}
	}

}








DEBUGExecutable * debugExecutable(Executable * executable){

	DEBUGExecutable * iDEBUGExecutable = new DEBUGExecutable();

	if (executable->type == EXPRESSION){
		iDEBUGExecutable->expression = (Expression*)executable;
		for (int i = 0; i < iDEBUGExecutable->expression->executable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->expression->executable[i]);
			iDEBUGExecutable->children_index++;
		}
	}
	else if (executable->type == FUNCTIONCALL){
		iDEBUGExecutable->functionCall = (FunctionCall1*)executable;
		for (int i = 0; i < iDEBUGExecutable->functionCall->variable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->functionCall->variables[i]);
			iDEBUGExecutable->children_index++;
		}
	}
	else if (executable->type == FUNCTIONDEFINITION){
		iDEBUGExecutable->functionDefinition = (FunctionDefinition1*)executable;
		for (int i = 0; i < iDEBUGExecutable->functionDefinition->variable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->functionDefinition->variables[i]);
			iDEBUGExecutable->children_index++;
		}
		iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->functionDefinition->executableBlock);
	}
	else if (executable->type == EXCUTEABLEBLOCK){
		iDEBUGExecutable->executableBlock = (ExecutableBlock*)executable;
		for (int i = 0; i < iDEBUGExecutable->executableBlock->executable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->executableBlock->executables[i]);
			iDEBUGExecutable->children_index++;
		}
	}

	else if (executable->type == FORINBLOCK){
		iDEBUGExecutable->forInBlock = (ForInBlock*)executable;
		for (int i = 0; i < iDEBUGExecutable->forInBlock->executable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->forInBlock->executables[i]);
			iDEBUGExecutable->children_index++;
		}
	}

	else if (executable->type == META){
		iDEBUGExecutable->metaExecutable = (MetaExecutable*)executable;
	}
	else if (executable->type == OPERATOR){
		iDEBUGExecutable->codeOperator = (Operator*)executable;
	}

	return iDEBUGExecutable;

}



void resolveCodeLine(char* line){
	CodeLine* codeLine = new CodeLine();
	codeLine->element_index = 0;
	int string_length = strlen(line);

	int result = resolveElement(line, string_length, codeLine);

	resolveOperators(codeLine);
	resolveBracket(codeLine);

	Executable * executable = analyzeCodeLine(codeLine, 0, codeLine->element_index);

	DEBUGExecutable * iDEBUGExecutable = debugExecutable(executable);

	excute(executable);
}


JSObject* excute(Executable * executable){//runtime polymorphism
	JSObject* result = NULL;
	if (executable->type == EXPRESSION){
		result = excute((Expression*)executable);
	}
	else if (executable->type == FUNCTIONCALL){
		//excute((FunctionCall*)executable);
	}
	else if (executable->type == FUNCTIONDEFINITION){
		//excute((FunctionDefinition*)executable);
	}
	else if (executable->type == EXCUTEABLEBLOCK){
		result = excute((ExecutableBlock*)executable);
	}
	return result;
}


JSObject* excute(ExecutableBlock * executableBlock){
	JSObject* result = NULL;
	for (int i = 0; i < executableBlock->executable_index; i++){
		result = excute(executableBlock->executables[i]);
	}
	executableBlock->result = result;
	return result;
}

JSObject* executableToJSObject(Executable* executable){
	JSObject* object = NULL;

	if (executable->type == META){
		CodeElement* codeElement = ((MetaExecutable*)executable)->codeElement;
		if (codeElement->type == CODE_NUMBER){
			object = (JSObject *)(new JSNumber(codeElement->number));
		}
		else if (codeElement->type == CODE_STRING){
			object = (JSObject *)(new JSString(codeElement->char_string));
		}
		else if (codeElement->type == CODE_JSON){
			JSON* json = parseJSON(codeElement->jsonstr);
			object = (JSObject *)json;
		}
		else if (codeElement->type == NAME){
			JSKeyValue *jsKeyValue = (JSKeyValue *)currentClosure->lookup(codeElement->variable_name);
			if (jsKeyValue == NULL){
				//report error
			}
			else{
				object = jsKeyValue->value;
			}
		}
	}
	else if (executable->type == EXCUTEABLEBLOCK){
		object = ((ExecutableBlock*)executable)->result;
	}
	else if (executable->type == EXCUTED){
		object = ((ExcutedExecutable*)executable)->result;
	}
	else if (executable->type == EXPRESSION){
		object = ((Expression*)executable)->result;
	}
	else if (executable->type == FUNCTIONCALL){
		object = ((FunctionCall1*)executable)->result;
	}
	return object;
}

JSObject* excuteOperator(Executable* left, Executable* right, Operator* code_operator){
	JSObject* result = NULL;

	JSObject* leftObject = executableToJSObject(left);
	JSObject* rightObject = executableToJSObject(right);
	if (code_operator->code_operator == '*'){
		if (leftObject->type == JSNUMBER&&rightObject->type == JSNUMBER){
			result = new JSNumber();
			result->number = leftObject->number * rightObject->number;
		}
	}
	else if (code_operator->code_operator == '+'){
		if (leftObject->type == JSNUMBER&&rightObject->type == JSNUMBER){
			result = new JSNumber();
			result->number = leftObject->number + rightObject->number;
		}
	}
	else if (code_operator->code_operator == '='){

	}
	return result;
}


JSObject* excute(Expression * expression1){
	JSObject* result = NULL;

	Executable* executables[30];
	int executable_index = expression1->executable_index;
	for (int i = 0; i < executable_index; i++){
		executables[i] = expression1->executable[i];
	}


	//处理括号
	for (int i = 0; i < executable_index; i++){
		if (executables[i]->type == EXCUTEABLEBLOCK){
			result = excute(executables[i]);
		}
		else if (executables[i]->type == FUNCTIONCALL){
			result = excute(executables[i]);
		}

	}

	//处理复合运算符
	for (int i = 0; i < executable_index; i++){
		if (executables[i]->type == OPERATOR){
			Operator* codeOperator = (Operator*)executables[i];
			if (codeOperator->code_operator2 != 0){

			}
		}

	}


	//处理运算符：* / %
	bool hasCodeOperator = false;
	do{
		hasCodeOperator = false;

		ExecutableBlock* executableBlock = new ExecutableBlock();
		for (int j = 0; j < executable_index; j++){
			executableBlock->executables[j] = executables[j];
		}
		executableBlock->executable_index = executable_index;
		DEBUGExecutable * iDEBUGExecutable = debugExecutable(executableBlock);



		for (int i = 0; i < executable_index; i++){
			if (executables[i]->type == OPERATOR){
				Operator* codeOperator = (Operator*)executables[i];
				if (codeOperator->code_operator == '*' || codeOperator->code_operator == '/' || codeOperator->code_operator == '%'){
					ExcutedExecutable* excutedExecutable = new ExcutedExecutable();

					result = excuteOperator(executables[i - 1], executables[i + 1], codeOperator);
					excutedExecutable->result = result;

					executables[i - 1] = excutedExecutable;
					for (int ii = i; ii < executable_index; ii++){
						executables[ii] = executables[ii + 2];
					}
					executable_index = executable_index - 2;
					hasCodeOperator = true;
					break;
				}
			}
		}
	} while (hasCodeOperator == true);



	//处理运算符：+ -
	do{
		hasCodeOperator = false;
		for (int i = 0; i < executable_index; i++){
			if (executables[i]->type == OPERATOR){
				Operator* codeOperator = (Operator*)executables[i];
				if (codeOperator->code_operator == '+' || codeOperator->code_operator == '-'){
					ExcutedExecutable* excutedExecutable = new ExcutedExecutable();

					result = excuteOperator(executables[i - 1], executables[i + 1], codeOperator);
					excutedExecutable->result = result;

					executables[i - 1] = excutedExecutable;
					for (int ii = i; ii < executable_index; ii++){
						executables[ii] = executables[ii + 2];
					}
					executable_index = executable_index - 2;
					hasCodeOperator = true;
					break;
				}
			}
		}
	} while (hasCodeOperator == true);


	//处理运算符：=
	for (int i = 0; i < executable_index; i++){
		if (executables[i]->type == OPERATOR){
			Operator* codeOperator = (Operator*)executables[i];
			if (codeOperator->code_operator == '='){

			}
		}
	}
	return result;
}




JSObject* excute(FunctionDefinition * functionDefinition){
	JSFunction * jsFunction = new JSFunction();
	jsFunction->function_name = functionDefinition->functionName->variable_name;
	jsFunction->functionDefinition = functionDefinition;


	currentClosure->set(functionDefinition->functionName->variable_name, jsFunction);
	JSKeyValue * jsFunctionKeyValue = (JSKeyValue *)currentClosure->lookup(functionDefinition->functionName->variable_name);
	if (jsFunctionKeyValue == NULL || ((JSObject*)jsFunctionKeyValue)->type != JSKEYVALUE){
		//report error
		return NULL;
	}

	return NULL;
}


JSObject* excute(FunctionCall * functionCall){

	if (functionCall->functionName->type != NAME){
		//report error
		return NULL;
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
		return NULL;
	}
	JSFunction * jsFunction = (JSFunction *)jsFunctionKeyValue->value;
	if (jsFunction == NULL || ((JSObject*)jsFunction)->type != JSFUNCTION){
		//report error
		return NULL;
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

	return NULL;

}

JSObject* excuteFunction(FunctionDefinition * functionDefinition, JSON* parameter){

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

	return NULL;
}

JSObject* excute(Assignment * assignment){
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

	return NULL;
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