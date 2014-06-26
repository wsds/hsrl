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
	var shape = Shape()
	shape.numberOfSides = 7
	var shapeDescription = shape.simpleDescription()
}

//-------------------------------------
class NamedShape:JSObject{

public:
	JSNumber* numberOfSides;
	JSObject* name;
	
	void init();


}