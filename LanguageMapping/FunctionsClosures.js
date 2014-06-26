/*************************************** ***************************************
* *    Functions and Closures
*************************************** ***************************************/


/***************************************
*func
*formatted string
***************************************/

func greet(name, day) {
    return "Hello @name@, today is @day@."
}
//-------------------------------------
JSON* result=new JSON();
void greet(JSObject* name, JSObject* day, JSObject* result){
	Closure* closure= new Closure();
	currentClosure->next=closure;
	closure->previous=currentClosure;
	currentClosure=currentClosure->next;

	currentClosure->variables->set("name",name);
	currentClosure->variables->set("day",day);

	
	JSString* string=new JSString();
	char* stringStr="Hello @name@, today is @day@.";
	string->format(resultStr, currentClosure);
	
	((JSON*)result)->push(string);
}

/***************************************
*func
*Use a JSON to return multiple values 
***************************************/
func getGasPrices(){
    return (3.59, 3.69, 3.79)
}
//-------------------------------------
JSON* result=new JSON();
void getGasPrices(JSObject* result){
	((JSON*)result)->push(3.59);
	((JSON*)result)->push(3.69);
	((JSON*)result)->push(3.79);
}


/***************************************
*func
*inner function
***************************************/
func returnFifteen() -> Int {
    var y = 10
    func add() {
        y += 5
    }
    add()
    return y
}

//-------------------------------------
JSON* result=new JSON();
void returnFifteen(JSObject* result){
	Closure* closure= new Closure();
	currentClosure->next=closure;
	closure->previous=currentClosure;
	currentClosure=currentClosure->next;

	JSNumber* y=new JSNumber();
	y->number=10;
	currentClosure->set("y",y);

	add(currentClosure);
	
	((JSON*)result)->push(y);
}

void add(JSObject* closure){
	JSNumber* y=currentClosure->get("y");
	jsNumberHelper->add(y, 5, y);
}

