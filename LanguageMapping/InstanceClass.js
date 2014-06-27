/*************************************** ***************************************
* *    Instance and Class
*************************************** ***************************************/


/***************************************
*class
*
***************************************/
class NamedShape {
    var numberOfSides = 0
    var name
    
    func init(name) {
        self.name = name
		this.numberOfSides=3
    }
    
    func simpleDescription() {
        return "A @name@ shape with @numberOfSides@ sides."
    }
}
func test() {
	var namedShape = NamedShape()
	shape.numberOfSides = 7
	var shapeDescription = shape.simpleDescription()
}

//-------------------------------------
class NamedShape:JSObject{

public:
	JSNumber* numberOfSides;
	JSObject* name;
	
	void init();
	void simpleDescription();

}

void NamedShape::init(JSString* name){
	this->name = name
	this->numberOfSides=3;
}


void NamedShape::shapeDescription(JSObject* result){
	Closure* closure= new Closure();
	currentClosure->next=closure;
	closure->previous=currentClosure;
	currentClosure=currentClosure->next;

	currentClosure->variables->set("name",this->name);
	currentClosure->variables->set("numberOfSides",this->numberOfSides);

	
	JSString* string=new JSString();
	char* stringStr="A @name@ shape with @numberOfSides@ sides.";
	string->format(resultStr, currentClosure);
	
	((JSON*)result)->push(string);
}

void test(){

	NamedShape* namedShape= new NamedShape();
	
	jsNumberHelper->assign(namedShape->numberOfSides, 7);
	
	JSON* result=new JSON();
	jsNumberHelper->simpleDescription(result);
	JSString* resultString=result->pop();

}

/***************************************
*instance
*
***************************************/


instance namedShape {
    var numberOfSides = 0
    var name
    
    func init(name) {
        self.name = name
		this.numberOfSides=3
    }
    
    func simpleDescription() {
        return "A @name@ shape with @numberOfSides@ sides."
    }
}

func test() {
	namedShape.init();
	namedShape.numberOfSides = 7
	var shapeDescription = namedShape.simpleDescription()
}
//-------------------------------------

class NamedShape:JSObject{

public:
	static NamedShape *instance;

	static NamedShape * getInstance()
	{
		if (instance == NULL) {  
			instance = new NamedShape();
		}
		return instance;
	}

	JSNumber* numberOfSides;
	JSObject* name;
	
	void init();
	void simpleDescription();

}

ImagePool* NamedShape::instance = NULL;

void NamedShape::init(JSString* name){
	this->name = name
	this->numberOfSides=3;
}

void NamedShape::shapeDescription(JSObject* result){
	Closure* closure= new Closure();
	currentClosure->next=closure;
	closure->previous=currentClosure;
	currentClosure=currentClosure->next;

	currentClosure->variables->set("name",this->name);
	currentClosure->variables->set("numberOfSides",this->numberOfSides);

	
	JSString* string=new JSString();
	char* stringStr="A @name@ shape with @numberOfSides@ sides.";
	string->format(resultStr, currentClosure);
	
	((JSON*)result)->push(string);
}

void test(){

	NamedShape* namedShape= NamedShape::getInstance();
	
	jsNumberHelper->assign(namedShape->numberOfSides, 7);
	
	JSON* result=new JSON();
	jsNumberHelper->simpleDescription(result);
	JSString* resultString=result->pop();

}