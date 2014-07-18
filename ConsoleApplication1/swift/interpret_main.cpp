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
	this->isNew = false;
	this->executable_index = 0;
}
FunctionCall::FunctionCall(){
	this->type = FUNCTIONCALL;
	this->variable_index = 0;
}
FunctionReturn::FunctionReturn(){
	this->type = FUNCTIONRETURN;
	this->variable_index = 0;
}
FunctionDefinition::FunctionDefinition(){
	this->type = FUNCTIONDEFINITION;
	this->executableBlock = new ExecutableBlock();
	this->executableBlock->holder = this;
}
ClassDefinition::ClassDefinition(){
	this->type = CLASSDEFINITION;
	this->executableBlock = new ExecutableBlock();
	this->executableBlock->holder = this;
}
InstanceDefinition::InstanceDefinition(){
	this->type = INSTANCEDEFINITION;
	this->executableBlock = new ExecutableBlock();
	this->executableBlock->holder = this;
}

ExecutableBlock::ExecutableBlock(){
	this->isHolded = false;
	this->holder = NULL;
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

ForBlock::ForBlock(){
	this->type = FORBLOCK;
	this->conditions_index = 0;
	this->executable_index = 0;
}

ForInBlock::ForInBlock(){
	this->type = FORINBLOCK;
	this->executableBlock = new ExecutableBlock();
	this->executableBlock->holder = this;
}

KeyWords * keyWords;
Closure * rootClosure;
Closure * currentClosure;
int funtionLevel;
FunctionDefinition * currentFunctionDefinition;
ExecutableBlock * executableBlocks[10];
int executableBlocksIndex;

ExecutableBlock * currentExecutableBlock;

#define NONE 0
#define STARTBLOCK 1
#define ENDBLOCK 2
int currentExecutableBlockHoldingStatus;
void interpret_main(){
	funtionLevel = 0;
	currentFunctionDefinition = NULL;

	executableBlocksIndex = 0;

	keyWords = KeyWords::getInstance();

	Import* import = Import::getInstance();
	import->regiditFunctions();
	rootClosure = import->rootClosure;
	currentClosure = rootClosure;

	currentExecutableBlock = new ExecutableBlock();
	currentExecutableBlockHoldingStatus = NONE;
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
	ExecutableBlock * executableBlock = NULL;
	MetaExecutable * metaExecutable = NULL;

	CodeElement * codeElement = NULL;
	Expression * expression = NULL;

	FunctionCall * functionCall = NULL;

	FunctionDefinition * functionDefinition = NULL;
	ClassDefinition * classDefinition = NULL;
	InstanceDefinition * instanceDefinition = NULL;

	ForInBlock * forInBlock = NULL;
	ForBlock * forBlock = NULL;
	IfBlock * ifBlock = NULL;
	FunctionReturn * functionReturn = NULL;

	int skipToIndex = -1;
	for (int i = from; i < end; i++){
		if (i < skipToIndex){
			continue;
		}
		codeElement = codeLine->codeElements[i];
		if (codeElement->type == KEYWORD){
			if (0 == strcmp(keyWords->string_var, codeElement->keyword)){
				if (expression == NULL){
					expression = new Expression();
					expression->isNew = true;
				}
			}
			else if (0 == strcmp(keyWords->string_func, codeElement->keyword)){
				functionDefinition = new FunctionDefinition();
				currentExecutableBlock = functionDefinition->executableBlock;
				if (i + 1 < end && codeLine->codeElements[i + 1]->type == NAME){
					functionDefinition->functionName = codeLine->codeElements[i + 1]->variable_name;
				}
			}
			else if (0 == strcmp(keyWords->string_class, codeElement->keyword)){
				classDefinition = new ClassDefinition();
				currentExecutableBlock = classDefinition->executableBlock;
				if (i + 1 < end && codeLine->codeElements[i + 1]->type == NAME){
					classDefinition->className = codeLine->codeElements[i + 1]->variable_name;
				}
			}
			else if (0 == strcmp(keyWords->string_instance, codeElement->keyword)){
				instanceDefinition = new InstanceDefinition();
				currentExecutableBlock = instanceDefinition->executableBlock;
				if (i + 1 < end && codeLine->codeElements[i + 1]->type == NAME){
					instanceDefinition->instanceName = codeLine->codeElements[i + 1]->variable_name;
				}
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
					currentExecutableBlock = forInBlock->executableBlock;
				}
				else{
					forBlock = new ForBlock();
				}
			}
			else if (0 == strcmp(keyWords->string_if, codeElement->keyword)){
				ifBlock = new IfBlock();
			}
			else if (0 == strcmp(keyWords->string_return, codeElement->keyword)){
				functionReturn = new FunctionReturn();
			}
		}
		else if (codeElement->type == BRACKET){

			if (codeElement->bracket == LEFTSMALLBRACKET){
				skipToIndex = codeElement->nextBracketIndex;
			}
			else if (codeElement->bracket == RIGHTSMALLBRACKET){
				CodeElement * preBracket = codeLine->codeElements[codeElement->preBracketIndex];
				if (codeElement->preBracketIndex - 1 >= 0 && codeLine->codeElements[codeElement->preBracketIndex - 1]->type == NAME){
					functionCall = new FunctionCall();
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

					if (expression == NULL){
						expression = new Expression();
					}

					expression->executables[expression->executable_index] = functionCall;
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

					if (expression == NULL){
						expression = new Expression();
					}

					expression->executables[expression->executable_index] = executableBlock;
					expression->executable_index++;

				}

			}
			else if (codeElement->bracket == LEFTBIGBRACKET){
				if (currentExecutableBlock->isHolded == false){
					executableBlocks[executableBlocksIndex] = currentExecutableBlock;
					executableBlocksIndex++;
					currentExecutableBlock->isHolded = true;
					if (currentExecutableBlockHoldingStatus == NONE){
						currentExecutableBlockHoldingStatus = STARTBLOCK;
					}
				}
				else{
					//report error
				}

			}
			else if (codeElement->bracket == RIGHTBIGBRACKET){
				if (executableBlocksIndex > 0){
					executableBlocksIndex--;
					if (currentExecutableBlockHoldingStatus == NONE){
						currentExecutableBlockHoldingStatus = ENDBLOCK;
					}

				}
				else{
					//report error
				}
			}

		}


		else if (codeElement->type == CODEOPERATOR){
			if (expression == NULL){
				expression = new Expression();
			}

			Operator * code_operator = new Operator();
			code_operator->code_operator = codeElement->code_operator;
			code_operator->code_operator2 = codeElement->code_operator2;

			expression->executables[expression->executable_index] = code_operator;
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

			metaExecutable = new MetaExecutable();

			metaExecutable->codeElement = codeElement;

			if (end - from <= 1){
				//executable = metaExecutable;
			}
			else{
				if (expression == NULL){
					expression = new Expression();
				}

				expression->executables[expression->executable_index] = metaExecutable;
				expression->executable_index++;
			}
		}
		else if (codeElement->type == DELIMITER){
			if (i + 1 >= end || expression == NULL){
				continue;
			}
			if (executableBlock == NULL){
				executableBlock = new ExecutableBlock();
			}
			executableBlock->executables[executableBlock->executable_index] = expression;
			executableBlock->executable_index++;
			expression = NULL;
		}
	}
	//end for

	Executable * executable = NULL;
	if (executableBlock != NULL&&expression != NULL){
		executableBlock->executables[executableBlock->executable_index] = expression;
		executableBlock->executable_index++;
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
		if (expression->executable_index == 2){
			if (expression->executables[0]->type == EXCUTEABLEBLOCK){
				ExecutableBlock* executableBlock = (ExecutableBlock*)expression->executables[0];
				if (executableBlock->executable_index == 2){
					MetaExecutable* keyNameExecutable = (MetaExecutable*)executableBlock->executables[0];
					MetaExecutable* valueNameExecutable = (MetaExecutable*)executableBlock->executables[1];

					forInBlock->keyName = keyNameExecutable->codeElement;
					forInBlock->valueName = valueNameExecutable->codeElement;
				}
			}
			if (expression->executables[1]->type == META){
				MetaExecutable* iteratorNameExecutable = (MetaExecutable*)expression->executables[1];
				forInBlock->iteratorName = iteratorNameExecutable->codeElement;
			}
		}
		DEBUGExecutable * iDEBUGExecutable = debugExecutable(expression);
		executable = forInBlock;
		int i = 1 + 1;
	}
	else if (forBlock != NULL){
		DEBUGExecutable * iDEBUGExecutable;
		if (executableBlock != NULL){
			iDEBUGExecutable = debugExecutable(executableBlock);
			if (executableBlock->executable_index >= 3){
				forBlock->pre_executable = executableBlock->executables[0];
				forBlock->conditions[forBlock->conditions_index] = (Condition *)executableBlock->executables[1];
				forBlock->conditions_index++;
				forBlock->last_executable = executableBlock->executables[2];

				executable = forBlock;
			}
			else{
				//report error

			}
		}

		executable = forBlock;
	}
	else if (ifBlock != NULL){
		DEBUGExecutable * iDEBUGExecutable;
		if (expression != NULL){
			iDEBUGExecutable = debugExecutable(expression);
			ifBlock->conditions[ifBlock->conditions_index] = (Condition *)expression;
			ifBlock->conditions_index++;
			executable = ifBlock;
		}
	}
	else if (functionReturn != NULL){
		DEBUGExecutable * iDEBUGExecutable;
		if (expression != NULL){
			iDEBUGExecutable = debugExecutable(expression);
			for (int i = 0; i < expression->executable_index; i++){
				functionReturn->variables[i] = expression->executables[i];
			}
			functionReturn->variable_index = expression->executable_index;
			executable = functionReturn;
		}
	}
	else if (executable == NULL){
		if (end - from <= 1){
			executable = metaExecutable;
		}
		else{
			executable = expression;
		}
	}

	return executable;

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
	if (executable == NULL){
		return NULL;
	}
	DEBUGExecutable * iDEBUGExecutable = new DEBUGExecutable();

	if (executable->type == EXPRESSION){
		iDEBUGExecutable->expression = (Expression*)executable;
		for (int i = 0; i < iDEBUGExecutable->expression->executable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->expression->executables[i]);
			iDEBUGExecutable->children_index++;
		}
	}
	else if (executable->type == FUNCTIONCALL){
		iDEBUGExecutable->functionCall = (FunctionCall*)executable;
		for (int i = 0; i < iDEBUGExecutable->functionCall->variable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(iDEBUGExecutable->functionCall->variables[i]);
			iDEBUGExecutable->children_index++;
		}
	}
	else if (executable->type == FUNCTIONDEFINITION){
		iDEBUGExecutable->functionDefinition = (FunctionDefinition*)executable;
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

		ExecutableBlock* executableBlock = iDEBUGExecutable->forInBlock->executableBlock;
		for (int i = 0; i < executableBlock->executable_index; i++){
			iDEBUGExecutable->children[iDEBUGExecutable->children_index] = debugExecutable(executableBlock->executables[i]);
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
	JSObject* result = NULL;

	CodeLine* codeLine = new CodeLine();
	codeLine->element_index = 0;
	int string_length = strlen(line);

	int resolveResult = resolveElement(line, string_length, codeLine);

	resolveOperators(codeLine);
	resolveBracket(codeLine);

	Executable * executable = analyzeCodeLine(codeLine, 0, codeLine->element_index);

	DEBUGExecutable * iDEBUGExecutable = debugExecutable(executable);

	//currentExecutableBlock->isHolded = true
	if (currentExecutableBlockHoldingStatus == STARTBLOCK){
		currentExecutableBlockHoldingStatus = NONE;
		return;
	}
	if (executableBlocksIndex <= 0){

		if (executable == NULL){
			if (currentExecutableBlockHoldingStatus == ENDBLOCK){
				ExecutableBlock* executableBlock = executableBlocks[executableBlocksIndex];
				if (executableBlock != NULL&&executableBlock->holder != NULL){
					executable = executableBlock->holder;
				}
			}
		}
		result = excute(executable);
	}
	else{
		if (executable == NULL){
			if (currentExecutableBlockHoldingStatus == ENDBLOCK){
				ExecutableBlock* executableBlock = executableBlocks[executableBlocksIndex];
				if (executableBlock != NULL&&executableBlock->holder != NULL){
					executable = executableBlock->holder;
				}
			}
		}
		result = excute(executable);

		ExecutableBlock* executableBlock = executableBlocks[executableBlocksIndex - 1];
		executableBlock->executables[executableBlock->executable_index] = executable;
		executableBlock->executable_index++;
	}

	if (result != NULL){
		log(result);
	}
}


JSObject* excute(Executable * executable){//runtime polymorphism
	JSObject* result = NULL;
	if (executable->type == EXPRESSION){
		result = excute((Expression*)executable);
	}
	else if (executable->type == FUNCTIONCALL){
		result = excute((FunctionCall*)executable);
	}
	else if (executable->type == FUNCTIONDEFINITION){
		excute((FunctionDefinition*)executable);
	}
	else if (executable->type == EXCUTEABLEBLOCK){
		result = excute((ExecutableBlock*)executable);
	}
	else if (executable->type == FUNCTIONRETURN){
		result = excute((FunctionReturn*)executable);
	}
	else if (executable->type == CLASSDEFINITION){
		result = excute((ClassDefinition*)executable);
	}
	return result;
}

JSObject* excute(FunctionReturn* functionReturn){
	JSObject* result = NULL;
	if (functionReturn->variable_index == 1){
		Executable * executable = functionReturn->variables[0];
		
		DEBUGExecutable * iDEBUGExecutable = debugExecutable(executable);
		if (executable->type == META){
			MetaExecutable* metaExecutable = (MetaExecutable*)executable;
			if (metaExecutable->codeElement->type == NAME){
				result = ((JSKeyValue*)currentClosure->lookup(metaExecutable->codeElement->variable_name))->value;
			}
			else if (metaExecutable->codeElement->type == CODE_NUMBER){
				JSNumber * jsNumber = new JSNumber();
				jsNumber->number = metaExecutable->codeElement->number;
				result = jsNumber;
			}
			else if (metaExecutable->codeElement->type == CODE_STRING){
				JSString * jsString = new JSString();
				jsString->char_string = metaExecutable->codeElement->char_string;
				jsString->length = strlen(jsString->char_string);
				result = jsString;
			}
			else if (metaExecutable->codeElement->type == CODE_JSON){
				JSON * json = new JSON();
				json = parseJSON(metaExecutable->codeElement->jsonstr);
				result = json;
			}

		}
	}
	else if (functionReturn->variable_index > 1){
		
	}
	return result;
}

JSObject* excute(ExecutableBlock * executableBlock){
	JSObject* result = NULL;
	for (int i = 0; i < executableBlock->executable_index; i++){
		result = excute(executableBlock->executables[i]);
		if (executableBlock->executables[i]->type == FUNCTIONRETURN){
			break;
		}
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
		object = ((FunctionCall*)executable)->result;
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
	else if (code_operator->code_operator == '/'){
		if (leftObject->type == JSNUMBER&&rightObject->type == JSNUMBER){
			result = new JSNumber();
			result->number = leftObject->number / rightObject->number;
		}
	}
	else if (code_operator->code_operator == '%'){
		if (leftObject->type == JSNUMBER&&rightObject->type == JSNUMBER){
			result = new JSNumber();
			result->number = leftObject->number % rightObject->number;
		}
	}

	else if (code_operator->code_operator == '+'){
		if (leftObject->type == JSNUMBER&&rightObject->type == JSNUMBER){
			result = new JSNumber();
			result->number = leftObject->number + rightObject->number;
		}
	}
	else if (code_operator->code_operator == '-'){
		if (leftObject->type == JSNUMBER&&rightObject->type == JSNUMBER){
			result = new JSNumber();
			result->number = leftObject->number - rightObject->number;
		}
	}
	return result;
}


JSObject* excute(Expression * expression1){
	JSObject* result = NULL;
	bool isNew = expression1->isNew;

	Executable* executables[30];

	int executable_index = expression1->executable_index;
	for (int i = 0; i < executable_index; i++){
		executables[i] = expression1->executables[i];
	}

	if (isNew == true && executable_index == 1){
		MetaExecutable * target = (MetaExecutable *)executables[0];
		if (target->type != META){
			//report error
			return NULL;
		}

		Executable * source = NULL;
		result = excuteAssignment(source, target, isNew);
		return result;
	}

	//处理括号
	for (int i = 0; i < executable_index; i++){
		if (executables[i]->type == EXCUTEABLEBLOCK){
			result = excute(executables[i]);
		}
		else if (executables[i]->type == FUNCTIONCALL){
			result = excute(executables[i]);
			((FunctionCall*)executables[i])->result = result;
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
				MetaExecutable * target = (MetaExecutable *)executables[i - 1];
				if (target->type != META){
					//report error
					return NULL;
				}

				Executable * source = executables[i + 1];
				result = excuteAssignment(source, target, isNew);
			}
		}
	}

	//if (executable_index == 1 && result == NULL&&
	//	executables[0]->type == META){
	//	MetaExecutable* metaExecutable = (MetaExecutable*)executables[0];
	//	result = ((JSKeyValue*)currentClosure->lookup(metaExecutable->codeElement->variable_name))->value;
	//}

	return result;
}


JSObject* excute(FunctionDefinition * functionDefinition){
	JSFunction * jsFunction = new JSFunction();
	jsFunction->function_name = functionDefinition->functionName;
	jsFunction->functionDefinition = functionDefinition;


	currentClosure->set(functionDefinition->functionName, jsFunction);

	JSKeyValue * jsFunctionKeyValue = (JSKeyValue *)currentClosure->lookup(functionDefinition->functionName);
	if (jsFunctionKeyValue == NULL || ((JSObject*)jsFunctionKeyValue)->type != JSKEYVALUE){
		//report error
		return NULL;
	}

	return jsFunction;
}

JSObject* excute(ClassDefinition * classDefinition){
	JSClass * jsClass = new JSClass();
	jsClass->className = classDefinition->className;

	Closure * closure = new Closure();
	closure->initialize();
	currentClosure->next = closure;
	closure->previous = currentClosure;

	currentClosure = currentClosure->next;


	ExecutableBlock* executableBlock = classDefinition->executableBlock;
	JSObject* result = excute(executableBlock);

	jsClass->children = currentClosure->variables;
	char * test = stringifyJSON(jsClass->children);

	currentClosure = currentClosure->previous;

	currentClosure->set(classDefinition->className, jsClass);
	return jsClass;
}


JSObject* excute(FunctionCall * functionCall){
	JSObject* result = NULL;

	if (functionCall->functionName == NULL){
		//report error
		return NULL;
	}
	JSObject * jsVariables[5];
	for (int i = 0; i < functionCall->variable_index; i++){
		MetaExecutable* metaExecutable = (MetaExecutable*)functionCall->variables[i];
		if (metaExecutable->type != META){
			//report error
			return NULL;
		}

		if (metaExecutable->codeElement->type == CODE_NUMBER){
			jsVariables[i] = new JSNumber(metaExecutable->codeElement->number);
		}
		else if (metaExecutable->codeElement->type == CODE_STRING){
			jsVariables[i] = new JSString(metaExecutable->codeElement->char_string);
		}
		else if (metaExecutable->codeElement->type == CODE_JSON){
			jsVariables[i] = parseJSON(metaExecutable->codeElement->jsonstr);
		}
		else if (metaExecutable->codeElement->type == NAME){
			JSKeyValue *jsKeyValue = (JSKeyValue *)currentClosure->lookup(metaExecutable->codeElement->variable_name);
			if (jsKeyValue == NULL){
				//report error
				//return;
			}
			else{
				jsVariables[i] = jsKeyValue->value;
			}
		}
	}


	JSKeyValue * jsFunctionKeyValue;
	jsFunctionKeyValue = (JSKeyValue *)currentClosure->lookup(functionCall->functionName);
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

		if (jsFunction->function != NULL){
			JSON* parameter = new JSON();
			parameter->initialize();
			for (int i = functionCall->variable_index - 1; i >= 0; i--){
				parameter->push(jsVariables[i]);
			}
			result = jsFunction->function(parameter);
		}
		else if (jsFunction->functionDefinition != NULL){

			FunctionDefinition* functionDefinition = jsFunction->functionDefinition;

			JSON* parameter = new JSON();
			parameter->initialize();
			MetaExecutable* metaExecutable;
			for (int i = 0; i < functionDefinition->variable_index; i++){
				metaExecutable = (MetaExecutable*)functionDefinition->variables[i];
				parameter->set(metaExecutable->codeElement->variable_name, jsVariables[i]);
			}


			result = excuteFunction(jsFunction->functionDefinition, parameter);
		}

	}

	return result;

}

JSObject* excuteFunction(FunctionDefinition * functionDefinition, JSON* parameter){
	JSObject* result = NULL;
	Closure * closure = new Closure();
	closure->initialize();
	currentClosure->next = closure;
	closure->previous = currentClosure;

	for (int i = 0; i < parameter->length; i++){
		JSKeyValue* jsKeyValue = (JSKeyValue*)parameter->list->find(i);
		closure->set(jsKeyValue->key, jsKeyValue->value);
	}

	currentClosure = currentClosure->next;


	ExecutableBlock* executableBlock = functionDefinition->executableBlock;
	result = excute(executableBlock);

	currentClosure = currentClosure->previous;

	return result;
}

JSObject* excuteAssignment(Executable * source, MetaExecutable * target, bool isNew){
	JSObject * rightValue = NULL;
	if (source == NULL){
		rightValue = NULL;
	}
	else if (source->type == META){
		MetaExecutable * metaExecutable = (MetaExecutable*)source;
		if (metaExecutable->codeElement->type == CODE_NUMBER){
			rightValue = (JSObject *)(new JSNumber(metaExecutable->codeElement->number));
		}
		else if (metaExecutable->codeElement->type == CODE_STRING){
			rightValue = (JSObject *)(new JSString(metaExecutable->codeElement->char_string));
		}
		else if (metaExecutable->codeElement->type == CODE_JSON){
			JSON* json = parseJSON(metaExecutable->codeElement->jsonstr);
			rightValue = (JSObject *)json;
			//log((JSObject*)json);
		}
		else if (metaExecutable->codeElement->type == NAME){
			JSKeyValue *jsKeyValue = (JSKeyValue *)currentClosure->lookup(metaExecutable->codeElement->variable_name);
			if (jsKeyValue == NULL){
				//report error
			}
			else{
				rightValue = jsKeyValue->value;
			}
		}
	}
	else if (source->type == EXCUTED){
		ExcutedExecutable * excutedExecutable = (ExcutedExecutable*)source;
		rightValue = excutedExecutable->result;
	}

	JSObject * leftVariable = NULL;
	if (isNew){
		currentClosure->set(target->codeElement->variable_name, rightValue);
	}
	else{
		JSKeyValue * leftVariableJSKeyValue = (JSKeyValue*)currentClosure->lookup(target->codeElement->variable_name);
		if (leftVariableJSKeyValue == NULL || leftVariableJSKeyValue->type != JSKEYVALUE){
			//report error
		}
		else{
			//replace or modify???
			//free the old value
			leftVariableJSKeyValue->value = rightValue;
			//currentClosure->set(target->codeElement->variable_name, rightValue);
		}
	}

	return rightValue;
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
			if (value == NULL){
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf(" = ");
				open::logBuf("NULL");
				open::logBufFlush();
			}
			else if (value->type == JSSTRING){
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf(" = \"");
				open::logBuf(((JSObject*)value)->char_string);
				open::logBuf("\"");
				open::logBufFlush();
			}
			else if (value->type == JSNUMBER){
				char num[12];
				parseNubmerToString(((JSObject*)value)->number, num);
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf(" = ");
				open::logBuf(num);
				open::logBufFlush();
			}
			else if (value->type == JSJSON){
				char* jsonStr = stringifyJSON((JSON*)value);
				open::logBuf("2.");
				open::logBuf(((JSKeyValue*)object)->key);
				open::logBuf(" = ");
				open::logBuf(jsonStr);
				open::logBufFlush();
			}
			else if (value->type == JSFUNCTION){
				JSFunction* jsFunction = (JSFunction*)value;
				if (((JSFunction*)value)->functionDefinition != NULL){
					open::logBuf("2.");
					open::logBuf(((JSFunction*)object)->function_name);
					open::logBuf(" = ");
					open::logBuf("func");
					open::logBufFlush();
				}
			}
			else if (value->type == JSCLASS){
				open::logBuf("2.");
				open::logBuf(((JSClass*)value)->className);
				open::logBuf(" = ");
				open::logBuf("class");
				open::logBufFlush();
			}
		}
		current = current->previous;
	}
}