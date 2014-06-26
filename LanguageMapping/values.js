/*************************************** ***************************************
* *    Simple Values
*************************************** ***************************************/
/***************************************
*number
***************************************/
var myVariable = 42
myVariable = 50

//-------------------------------------
JSNumber* myVariable=new JSNumber();
myVariable->number=42;

myVariable->number=50;

/***************************************
*string
***************************************/
var label = "The width is"

//-------------------------------------
JSString* label=new JSString();
label->char_string="The width is";


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

JSON* shoppingList=new JSON();
char* shoppingListStr="[\"catfish\", \"water\", \"tulips\", \"blue paint\"]"
shoppingList=parseJSON(shoppingListStr);

JSString* valueStr=new JSString();
valueStr->char_string="bottle of water";
shoppingList->replace(JSObject*(valueStr), 1);


JSON* occupations=new JSON();
char* occupationsStr="[\"Malcolm\": \"Captain\",\"Kaylee\": \"Mechanic\",]"
occupations=parseJSON(occupationsStr);

JSString* valueStr=new JSString();
valueStr->char_string="Public Relations";

char* keyStr="Jayne";
occupations->set(keyStr, JSObject*(valueStr));




