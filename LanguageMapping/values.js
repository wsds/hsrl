/*************************************** ***************************************
* *    Simple Values
*************************************** ***************************************/
/***************************************
*Basic type
***************************************/
int myVariable = 42
char myVariable = 50
//-------------------------------------
int myVariable = 42;//如何把JSON里的两级指针化为一级指针？
char myVariable = 50;


/***************************************
*number
***************************************/
var myVariable = 42
a:=myVariable
myVariable = 50

//-------------------------------------
JSNumber* myVariable=new JSNumber(42);
myVariable->number=42;
JSNumber* a=myVariable;
myVariable->number=50;

/***************************************
*string
***************************************/
var label = "The width is"

//-------------------------------------
JSString* label=new JSString("The width is");

/***************************************
*json
***************************************/
var shoppingList = ["catfish", "water", "tulips", "blue paint"]
shoppingList[1] = "bottle of water"
 
var occupations = [
    "Malcolm": "Captain",
    "Kaylee": "Mechanic",
]
occupations["Jayne"] = "Public Relations"

//-------------------------------------

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

log((JSObject*)occupations);




