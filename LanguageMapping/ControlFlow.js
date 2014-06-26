/*************************************** ***************************************
* *    Control Flow
*************************************** ***************************************/
/***************************************
*if
***************************************/


/***************************************
*for
*for var i = 0; i < 3; ++i
***************************************/

var secondForLoop = 0
for var i = 0; i < 3; ++i {
    secondForLoop += 1
}
//-------------------------------------
int secondForLoop=0;
for( int i = 0; i < 3; ++i ){
    secondForLoop += 1;
}

/***************************************
*for
*for score in individualScores
***************************************/
var individualScores = [75, 43, 103, 87, 12]
var teamScore = 0
for score in individualScores {
    if score > 50 {
        teamScore += 3
    } else {
        teamScore += 1
    }
}

//-------------------------------------
JSON* individualScores=new JSON();
char* individualScoresStr="[75, 43, 103, 87, 12]";
individualScores=parseJSON(individualScoresStr);

JSNumber* teamScore=new JSNumber();
teamScore->number=0;

JSNumber* score;
for(int i = 0, i<individualScores->length; i++){
	score=(JSNumber*)individualScores->find(i);
	
	if (jsNumberHelper->isbiger(score,50)) {
		jsNumberHelper->add(teamScore, 3, teamScore);
    } else {
        jsNumberHelper->add(teamScore, 1, teamScore);
    }

}


/***************************************
*for
*for (kind, numbers) in interestingNumbers
***************************************/
var interestingNumbers = [
    "Prime": [2, 3, 5, 7, 11, 13],
    "Fibonacci": [1, 1, 2, 3, 5, 8],
    "Square": [1, 4, 9, 16, 25],
]
var largest = 0
for (kind, numbers) in interestingNumbers {
    for number in numbers {
        if number > largest {
            largest = number
        }
    }
}

//-------------------------------------
JSON* interestingNumbers=new JSON();
char* interestingNumbersStr="[\"Prime\": [2, 3, 5, 7, 11, 13],\"Fibonacci\": [1, 1, 2, 3, 5, 8],\"Square\": [1, 4, 9, 16, 25],]";
interestingNumbers=parseJSON(interestingNumbersStr);

JSNumber* largest=new JSNumber();
largest->number=0;


char* kind;
JSON* numbers;

JSKeyValue* jsKeyValue;

JSNumber* number;

for(int i = 0, i<interestingNumbers->length; i++){

	jsKeyValue=(JSKeyValue)interestingNumbers->find(i);
	if(((JSObject*)jsKeyValue)->type != JSKEYVALUE){
		continue;
	}
	
	for(int ii = 0, ii<numbers->length; ii++){
		number=(JSNumber*)numbers->find(ii);
		
		if (jsNumberHelper->isbiger(number,largest)) {
			jsNumberHelper->assign(largest, number);
		} 
	}
}

/***************************************
*while
*while n < 100
***************************************/
int n = 2
while n < 100 {
    n = n * 2
}
//-------------------------------------
int n = 2;
while (n < 100) {
    n = n * 2;
}


/***************************************
*while
*do while m < 100
***************************************/
int m = 2
do {
    m = m * 2
} while m < 100
//-------------------------------------
int m = 2;
do{
    m = m * 2;
}while (m < 100);




