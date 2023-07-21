#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

int l = 22;
int holdCloseBlockIndex = 0;
int addByIndex = 0;//this is for identifier insertion indexing.
int numberOfIdentifiers = 0;
char **holdIdentifiers;
long *valueOfIdentifiers;
const char *keywordArray[] = {"move","int","to","loop","out","newline","add","sub","from","times"};
int intconstants[10] ={'0','1','2','3','4','5','6','7','8','9'};
int linePosition = 0;
char *lineToSplit;
char *seperatedFromComma;
const char *delimiterBetweenTokens = ' ';
int whereLeft = 0;//index number of where we left on reading file.
char *removedEndOfLineCh;
char *removedFromFront;
long *toConvert;
int printTheOutput = 0;

char * removeCommentBlock(char*lineStr, char toCheck){
    int myLen = strlen(lineStr);
    char *toReturn = (char*)malloc(myLen*(sizeof(char)));
    int i = 0;
    int countLen = 0;
    while(lineStr[i] != toCheck){
        toReturn[i] = lineStr[i];
        countLen++;
        i++;
    }
    toReturn[countLen] = '\0';
    return toReturn;
}

int findFirstIndexToDel(char *line){
    int returnTheIndex = 0;
    for(int i = 0; i<strlen(line); i++){
        if(isalpha(line[i]) == 0){
            returnTheIndex++;
        }
        else{
            return returnTheIndex;
        }
    }
    return returnTheIndex;
}

char * delSpaces(char *lineToDel){
    int lenOfDel = strlen(lineToDel);
    int findFirstCh = 0;
    char * toCopy = (char*)malloc(lenOfDel*(sizeof(char)));
    findFirstCh = findFirstIndexToDel(lineToDel);
    lenOfDel = lenOfDel- findFirstCh+1;
    int myIndex = 0;
    for(int i = 0; i<lenOfDel; i++){
        toCopy[i] = lineToDel[findFirstCh];
        findFirstCh++;
    }
    int toCopyLen = strlen(toCopy);
    toCopy[toCopyLen] = '\0';
    return toCopy;
}

char *contentsOfTheLine(FILE *theFileRead, int position){
    char *line1;
    int count = 0;
    char i = "";
    char toCheck = "";
    fseek(theFileRead,position,SEEK_SET);
    while( i != '\n' && i != EOF){
        i = fgetc(theFileRead);
        count++;
    }
    line1 = (char*)malloc(sizeof(char)*count);
    fseek(theFileRead,position,SEEK_SET);
    for(int index = 0; index<count-1; index++){
        line1[index] = (char)fgetc(theFileRead);
    }
    line1[count - 1]='\0';
    return line1;
}
char * getTheNextLine(int lineChCount, FILE * fileReads){
    char *lineToCopy = contentsOfTheLine(fileReads,whereLeft);
    int lineLength = strlen(lineToCopy);
    return lineToCopy;
}
int get_int_len_with_negative (long value){
  int l=!value;
  while(value){ l++; value/=10; }
  return l;
}

bool checkIfExceeds(int lengthToCheck,char * currentToken){
    if(lengthToCheck >10 && currentToken[0] == '-'){
        return false;
    }
    else if (lengthToCheck <= 10){
        return false;
    }
    else{
        return true;
    }
}

bool doesLineContainsAndEnds(char *lineStr, char toBeCheck){
    int lenOfLine = strlen(lineStr);
    for(int i = 0; i<lenOfLine; i++){
        if(lineStr[i] == toBeCheck){
            return true;
        }
    }
    return false;
}

void initializeTheIdentifiersValue(){
    for(int i = 0; i<sizeof(valueOfIdentifiers); i++){
        valueOfIdentifiers[i] = 0;
    }
}
int getIndexOfIdentifier(char *token){
    int toReturn = 0;
    for(int i = 0; i<numberOfIdentifiers; i++){
        if(!strcmp(token,holdIdentifiers[i])){
            return toReturn;
        }
        toReturn++;
    }
}

long getTheValueOfIdentifier(int index){
    long valueToBeReturn = 0;
    valueToBeReturn = valueOfIdentifiers[index];
    return valueToBeReturn;
}

void instertTheIntegerValue(long toBeInserted, int index){
    valueOfIdentifiers[index] = toBeInserted;  
}

void InsertTheIdentifier(char *token){
    holdIdentifiers = (char*)realloc(holdIdentifiers,l*sizeof(char));
    holdIdentifiers[addByIndex] = token;
    addByIndex++;
}

bool checkIfDeclaredBefore(char *token){
    if(holdIdentifiers == NULL){
        return false;//means did not declared before.
    }
    else{
        for(int i = 0; i<numberOfIdentifiers; i++){
            if(!(strcmp(token,holdIdentifiers[i]))){
                return true;//it declared before.
            }
        }
    }
    return false;
}

bool isIntegerConstant(char *token){
    if(isdigit(token[0])){
        if(strlen(token)>100){
            return false;
        }
    }
    if(!isdigit(token[0])){
        if(strlen(token)>101){
            return false;
        }
    }
    int i = 0;
    int lengthOfToken = strlen(token);
    int len = sizeof(intconstants)/sizeof(intconstants[0]);
    int count = 0; //also check if any character between digits.
    if(token[0] == '-' && token[1]!= '-'){
        count++;//only one minus sign allowed.
    }
    else if(token[0] == '+'){//if contains + operator return false
        return false;
    }
    for( i = 0; i<lengthOfToken; i++){
        if(token[i] == '.'){//if token includes . operator somewhere inside then return false.
            return false;
        }
        for(int j = 0; j<len; j++){
            if(token[i] == intconstants[j]){
                count++;
            }
        }
    }
    if(count == lengthOfToken){
        return true;
    }
    else{
        return false;
    }
}

bool isKeyword(char *token){
    int len = sizeof(keywordArray)/sizeof(keywordArray[0]);
    for(int i = 0; i<len; i++){
        if(!strcmp(token,keywordArray[i])){//if token found in keyword array
             return true;
    }
    }
    return false;
}

void seperateToken(char *token, int from, int to){
    int size = strlen(token);
    seperatedFromComma = (char*)malloc(sizeof(size));
    int i = 0;
    if(from != 0){
        from++;
    }
    for(int j = from; j<to; j++){
        if(token[j] == '.'){
            int lengthOfSeperatedToken = i;
            seperatedFromComma[i] = '\0';
            return;
        }
        seperatedFromComma[i] = token[j];
        i++;
    }
    int lengthOfSeperatedToken = i;
    seperatedFromComma[i] = '\0';
}

int findIndexOfComma(char *token, int start){
    start++;
    for(start; start<strlen(token); start++){
        if(token[start] == ','){
            return start;
        }
    }
    return start;
}
int moreThanOneComma(char *token){
    int countComma = 0;
    for(int i = 0; i<strlen(token)-1; i++){
        if(token[i] == ','){
            countComma++;
        }
    }
    return countComma;
}

char *getNextToken(char *line1,const char *delimiter1){
    int count = 0;
    if(line1 != NULL){
        linePosition = 0;
        lineToSplit = line1;
    }
    if(lineToSplit == NULL){
        return NULL;
    }
    if(linePosition >= strlen(lineToSplit)){
        lineToSplit = NULL;
        linePosition = 0;
        return NULL;
    }
    while(linePosition < strlen(lineToSplit) && lineToSplit[linePosition] != delimiter1 && lineToSplit[linePosition] != '\n'){
        count++;
        linePosition++;
    }
    char *token = (char*)malloc(sizeof(char)*count);
    int tokenIndex = 0;
    for(int i = linePosition - count; i<linePosition; i++){
        token[tokenIndex] = lineToSplit[i];
        tokenIndex++;
    }
    token[count] = '\0';
    linePosition++;
    return token;
}
int countLineChNumber(FILE *theFileRead){
    int count = 0;
    int c = 0;
    for (c = getc(theFileRead); c != EOF; c = getc(theFileRead)){
        count = count + 1;
    }
    return count;
}

bool checkIfValidIdentifer(char *toBeChecked,int lineNumber){
    if(isKeyword(toBeChecked)){
        printf("Expected Identifier, instead keyword found at line %d",lineNumber);
        return false;
    }
    else if(isIntegerConstant(toBeChecked)){
        printf("Expected Identifier, instead found integer constant at line %d",lineNumber);
        return false;
    }
    else{
        if(strlen(toBeChecked)>21 || toBeChecked[0] =='+' || toBeChecked[0] == '-'){
            return false;
        }
        else if(isdigit(toBeChecked[0])){
            return false;
        }
        else{
            for(int i = 1; i<strlen(toBeChecked); i++){
                if(!isalnum(toBeChecked[i]) && toBeChecked[i] != '_'){//returns non zero value
                    return false;
                }
            }
        }
    }
    return true;
}

char removeCharFromFront(char *token,int size){
    removedFromFront = (char*)malloc(sizeof(size));
    int count = 0;
    int getLengthOfToken = strlen(token);
    for(int j = 0; j<getLengthOfToken; j++){
        removedFromFront[j] = token[j+1]; 
        count++;
    }
    removedFromFront[count] = '\0';
    return removedFromFront;
}

char removeCharFromLast(char *token, int size){
    removedEndOfLineCh = (char*)malloc(sizeof(size));
    int count = 0;
    int getLengthOfToken = strlen(token)-1;//iterating without last character and copies elements.
    for(int i = 0; i<getLengthOfToken; i++){
        removedEndOfLineCh[i] = token[i];
        count++;
    }
    removedEndOfLineCh[count] = '\0';
    return removedEndOfLineCh;
}
void processLoopLine(char *lineStr, int lineNumber, int lineChCount, FILE* theFile){
    char * currentToken = getNextToken(NULL,delimiterBetweenTokens);
    char * nextToken = getNextToken(NULL,delimiterBetweenTokens);
    char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
        if(!isIntegerConstant(currentToken)){
            if(checkIfDeclaredBefore(currentToken)){
                int myLen = strlen(nextToken);
                if(nextToken[myLen-1] == '[' ){//case like loop size times[
                    removeCharFromLast(nextToken,myLen);
                    if(!strcmp(removedEndOfLineCh,keywordArray[9])){//loop size times[
                        int holdWhereLeft = whereLeft;
                        int holdIndexOfLoopVar = getIndexOfIdentifier(currentToken);
                        long getValOfLoopVar = getTheValueOfIdentifier(holdIndexOfLoopVar);
                        printTheOutput = getValOfLoopVar;
                        int copyGetLoopVar = getValOfLoopVar;
                        int holdUntilCloseBlock = 0;
                        int temp= 0;
                        while(getValOfLoopVar != 0){
                            lineChCount++;
                            temp++;
                            char *line = getTheNextLine(lineChCount,theFile);
                            int lineLengtH = strlen(line);
                            holdUntilCloseBlock+= lineLengtH+2;
                            if(line[0] == ']' || line[lineLengtH-1] == ']'){
                                whereLeft = holdWhereLeft;
                                line = getTheNextLine(whereLeft,theFile);
                                lineChCount -= temp;
                                lineLengtH = strlen(line);
                                temp = 0;
                                if(copyGetLoopVar == getValOfLoopVar){
                                    holdCloseBlockIndex = holdUntilCloseBlock + whereLeft ;
                                }
                                getValOfLoopVar = getValOfLoopVar -1;
                                printTheOutput+=getValOfLoopVar;
                                valueOfIdentifiers[holdIndexOfLoopVar] -=1;
                            }
                            line = delSpaces(line);
                            char *myToken = getNextToken(line,delimiterBetweenTokens);
                            processLine(line,myToken,lineNumber,lineChCount,theFile);
                            int tempIncrease = lineLengtH +2;
                            whereLeft += tempIncrease;
                        }
                        //printf("Sum: %d\n",printTheOutput);
                        valueOfIdentifiers[holdIndexOfLoopVar] = printTheOutput;
                        whereLeft = holdCloseBlockIndex;
                    }
                }
                else if(lastToken!= NULL){
                    if(!strcmp(lastToken,keywordArray[4])){//case like loop size times out "something"
                        char * veryLastToken = getNextToken(NULL,delimiterBetweenTokens);
                        if(veryLastToken == NULL){
                            printf("Expected token but found none at line %d",lineNumber);
                            exit(1);
                        }
                        int lenOfVeryLast = strlen(veryLastToken);
                        if(veryLastToken[0] == '"' && veryLastToken[lenOfVeryLast-1] == '"'){
                            removeCharFromLast(veryLastToken,lenOfVeryLast);
                            removeCharFromFront(removedEndOfLineCh,lenOfVeryLast-1);
                            int getIndexOfVeryLast = getIndexOfIdentifier(currentToken);
                            long getTheValOfVeryLast = getTheValueOfIdentifier(getIndexOfVeryLast);
                            if(getTheValOfVeryLast <0){
                                printf("Negative numbers can't be used to loop statements at line %d",lineNumber);
                                exit(1);
                            }
                            for(int i = 0; i<getTheValOfVeryLast; i++){
                                printf("%s",removedFromFront);
                            }
                        }
                    }    
                    else if(lastToken[0] == '['){//case like loop size times [
                        int holdWhereLeft = whereLeft;
                        int holdIndexOfLoopVar = getIndexOfIdentifier(currentToken);
                        long getValOfLoopVar = getTheValueOfIdentifier(holdIndexOfLoopVar);
                        printTheOutput = getValOfLoopVar;
                        int copyGetLoopVar = getValOfLoopVar;
                        int holdUntilCloseBlock = 0;
                        int temp = 0;
                        while(getValOfLoopVar != 0){
                            lineChCount++;
                            temp++;
                            char *line = getTheNextLine(lineChCount,theFile);
                            int lineLengtH = strlen(line);
                            holdUntilCloseBlock+= lineLengtH+2;
                            if(line[0] == ']' || line[lineLengtH-1] == ']'){
                                whereLeft = holdWhereLeft;
                                line = getTheNextLine(whereLeft,theFile);
                                lineLengtH = strlen(line);
                                lineChCount -= temp;
                                temp = 0;
                                if(copyGetLoopVar == getValOfLoopVar){
                                    holdCloseBlockIndex = holdUntilCloseBlock + whereLeft ;
                                }
                                getValOfLoopVar = getValOfLoopVar -1;
                                printTheOutput+= getValOfLoopVar;
                                valueOfIdentifiers[holdIndexOfLoopVar] -=1;
                            }
                            line = delSpaces(line);
                            char *myToken = getNextToken(line,delimiterBetweenTokens);
                            processLine(line,myToken,lineNumber,lineChCount,theFile);
                            int tempIncrease = lineLengtH +2;
                            whereLeft += tempIncrease;
                        }
                        //printf("Sum: %d",printTheOutput);
                        valueOfIdentifiers[holdIndexOfLoopVar] = printTheOutput;
                        whereLeft = holdCloseBlockIndex;
                    } 
                }
                else if(lastToken == NULL){//case like loop size times
                                           //[
                    char *line = getTheNextLine(lineChCount,theFile);
                    nextToken = getNextToken(line,delimiterBetweenTokens);
                    int holdWhereLeft = whereLeft;
                    int holdIndexOfLoopVar = getIndexOfIdentifier(currentToken);
                    long getValOfLoopVar = getTheValueOfIdentifier(holdIndexOfLoopVar);
                    printTheOutput = getValOfLoopVar;
                    int copyGetLoopVar = getValOfLoopVar;
                    int holdUntilCloseBlock = 0;
                    int temp = 0;
                    if(nextToken[0] == '['){
                        while(getValOfLoopVar !=0){
                            lineChCount++;
                            temp++;
                            char *line = getTheNextLine(lineChCount,theFile);
                            nextToken = getNextToken(line,delimiterBetweenTokens);
                            int lineLengtH = strlen(line);
                            holdUntilCloseBlock+= lineLengtH+2;
                            if(line[0] == ']' || line[lineLengtH-1] == ']'){
                                whereLeft = holdWhereLeft;
                                line = getTheNextLine(whereLeft,theFile);
                                lineLengtH = strlen(line);
                                lineChCount -= temp;
                                temp = 0;
                                if(copyGetLoopVar == getValOfLoopVar){
                                    holdCloseBlockIndex = holdUntilCloseBlock + whereLeft ;
                                }
                                getValOfLoopVar = getValOfLoopVar -1;
                                printTheOutput+= getValOfLoopVar;
                                valueOfIdentifiers[holdIndexOfLoopVar] -=1;
                            }
                            line = delSpaces(line);
                            char *myToken = getNextToken(line,delimiterBetweenTokens);
                            processLine(line,myToken,lineNumber,lineChCount,theFile);
                            int tempIncrease = lineLengtH +2;
                            whereLeft += tempIncrease;
                        }
                        //printf("Sum: %d",printTheOutput);
                        valueOfIdentifiers[holdIndexOfLoopVar] = printTheOutput;
                        whereLeft = holdCloseBlockIndex;
                    }

                }
            }
            else{
                printf("Loop variable is not declared before at line %d",lineNumber);
                exit(1);
            }

        }
        else if(isIntegerConstant(currentToken)){
            int myLen = strlen(nextToken);
            if(nextToken[myLen-1] == '[' ){//case like loop 5 times[
                int holdWhereLeft = whereLeft;
                printTheOutput = atoi(currentToken);
                int currenStrToInt = atoi(currentToken);
                int copyGetLoopVar = currenStrToInt;
                int holdUntilCloseBlock = 0;
                int temp= 0;
                while(currenStrToInt != 1){
                    lineChCount++;
                    temp++;
                    char *line = getTheNextLine(lineChCount,theFile);
                    int lineLengtH = strlen(line);
                    holdUntilCloseBlock+= lineLengtH+2;
                    if(line[0] == ']' || line[lineLengtH-1] == ']'){
                        whereLeft = holdWhereLeft;
                        line = getTheNextLine(whereLeft,theFile);
                        lineChCount -= temp;
                        lineLengtH = strlen(line);
                        temp = 0;
                        if(copyGetLoopVar == currenStrToInt){
                            holdCloseBlockIndex = holdUntilCloseBlock + whereLeft ;
                        }
                        currenStrToInt = currenStrToInt-1;
                        printTheOutput+=currenStrToInt;
                    }
                    line = delSpaces(line);
                    char *myToken = getNextToken(line,delimiterBetweenTokens);
                    processLine(line,myToken,lineNumber,lineChCount,theFile);
                    int tempIncrease = lineLengtH +2;
                    whereLeft += tempIncrease;
                }
                whereLeft = holdCloseBlockIndex;
            }
            else if(lastToken[0] == '['){//case like loop size times [
                int holdWhereLeft = whereLeft;
                printTheOutput = atoi(currentToken);
                int currenStrToInt = atoi(currentToken);
                int copyGetLoopVar = currenStrToInt;
                int holdUntilCloseBlock = 0;
                int temp= 0;
                while(currenStrToInt != 1){
                    lineChCount++;
                    temp++;
                    char *line = getTheNextLine(lineChCount,theFile);
                    int lineLengtH = strlen(line);
                    holdUntilCloseBlock+= lineLengtH+2;
                    if(line[0] == ']' || line[lineLengtH-1] == ']'){
                        whereLeft = holdWhereLeft;
                        line = getTheNextLine(whereLeft,theFile);
                        lineChCount -= temp;
                        lineLengtH = strlen(line);
                        temp = 0;
                        if(copyGetLoopVar == currenStrToInt){
                            holdCloseBlockIndex = holdUntilCloseBlock + whereLeft ;
                        }
                        currenStrToInt = currenStrToInt-1;
                        printTheOutput+=currenStrToInt;
                    }
                    line = delSpaces(line);
                    char *myToken = getNextToken(line,delimiterBetweenTokens);
                    processLine(line,myToken,lineNumber,lineChCount,theFile);
                    int tempIncrease = lineLengtH +2;
                    whereLeft += tempIncrease;
                }
                whereLeft = holdCloseBlockIndex;
            }
            else if(!strcmp(lastToken,keywordArray[4])){//case like loop 10 times out "s" . // dot must be seperated from string constant.
                char * veryLastToken = getNextToken(NULL,delimiterBetweenTokens);
                if(veryLastToken == NULL){
                    printf("Expected token but found none at line %d",lineNumber);
                    exit(1);
                }
                int veryLastOfVal = atoi(currentToken);
                int lenOfVeryLast = strlen(veryLastToken);
                removeCharFromFront(veryLastToken,lenOfVeryLast);
                removeCharFromLast(removedFromFront,lenOfVeryLast-1);
                if(veryLastToken[0] == '"' && veryLastToken[lenOfVeryLast-1] == '"'){
                    if(veryLastOfVal <0){
                        printf("Negative numbers can't be used to loop statements at line %d",lineNumber);
                        exit(1);
                    }
                    for(int i = 0; i<veryLastOfVal; i++){
                        printf("%s",removedEndOfLineCh);
                    }
                    printf("\n");
                }
            
            }
        }

}

void processOutLine(char *lineStr, int lineNumber){
    char * currentToken = getNextToken(NULL,delimiterBetweenTokens);
    char * nextToken = getNextToken(NULL,delimiterBetweenTokens);
    char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
    int lineLength = strlen(lineStr);
    int mySize = strlen(currentToken);
    removeCharFromLast(currentToken, mySize);
    if(lineStr[lineLength-1] == '.'){
        if(currentToken[0] == '"'){
            if(currentToken[mySize-1] == '.'){
                removeCharFromLast(currentToken,mySize);
                if(removedEndOfLineCh[mySize-2] == '"'){
                    removeCharFromLast(removedEndOfLineCh,mySize-1);
                    removeCharFromFront(removedEndOfLineCh,mySize-2);
                    printf("%s",removedFromFront);
                    return;
                }
                else{
                    printf("Unclosed string constant declaration at line %d",lineNumber);
                    exit(1);
                }
            }
            else if(nextToken[0] == '.' && strlen(nextToken) == 1){
                if(currentToken[0] == '"'){
                    if(currentToken[mySize-1] == '"'){
                        removeCharFromLast(currentToken,mySize);
                        removeCharFromFront(removedEndOfLineCh,mySize-1);
                        printf("%s",removedFromFront);
                        return;
                    }
                    else{
                        printf("Unclosed string constant declaration at line %d",lineNumber);
                        exit(1);
                    }
                }
                else{
                    printf("Expected open string character at line %d",lineNumber);
                    exit(1);
                }
            }
            else{
                printf("Sytax error,expected one dot but found more than one at line %d",lineNumber);
                exit(1);
            }
        }
        else if(!strcmp(removedEndOfLineCh, keywordArray[5])){
            if(nextToken[0] == '"'){//if string comes here.
                int lenOfNextToken = strlen(nextToken);
                if(nextToken[lenOfNextToken-1] == '"'){//checks if string closes.
                    removeCharFromLast(nextToken,lenOfNextToken);
                    removeCharFromFront(removedEndOfLineCh, lenOfNextToken-2);
                    printf("\n%s",removedFromFront);
                    char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
                    if(!(lastToken != NULL)){
                        printf("Excepted statement but found none at line %d",lineNumber);
                        exit(1);
                    }
                    if(lastToken[0] == ','){
                        lastToken = getNextToken(NULL,delimiterBetweenTokens);
                        if(lastToken == NULL){
                            printf("Excepted statement but found none at line %d",lineNumber);
                            exit(1);
                        }
                    }
                    int lenOfLastToken = strlen(lastToken);
                    removeCharFromLast(lastToken,lenOfLastToken);
                    lastToken = removedEndOfLineCh;
                    if(checkIfDeclaredBefore(lastToken) && lastToken[0] != ','){
                        int getIndexOfLastToken = getIndexOfIdentifier(lastToken);
                        long getValOfLastToken = getTheValueOfIdentifier(getIndexOfLastToken);
                        printf("%d\n",getValOfLastToken);
                    }
                    else{
                        printf("The variable trying to print is not declared before at line %d",lineNumber);
                        exit(1);
                    }
                }
                else{
                    printf("String constant declaration opened but didn't close at line %d ",lineNumber);
                    exit(1);
                }
            }
            else{
                printf("Expected string constant but found none syntax error at line %d",lineNumber);
                exit(1);
            }
        }
        else if(checkIfDeclaredBefore(removedEndOfLineCh)){
            int getIndexOfCurr = getIndexOfIdentifier(removedEndOfLineCh);
            long getTheVal = getTheValueOfIdentifier(getIndexOfCurr);
            char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
            if(lastToken != NULL){
                if(lastToken == "newLine."){
                    printf("%d\n",getTheVal);
                }
                else{
                    printf("Expected newLine keyword but found none at line %d",lineNumber);
                    exit(1);
                }
            }
            else{
                if(nextToken != NULL){
                    int lenOfNext = strlen(nextToken);
                    removeCharFromLast(nextToken,lenOfNext);
                    if(!strcmp(removedEndOfLineCh,keywordArray[5])){
                        printf("%d\n",getTheVal);
                    }
                }
                else{
                    printf("Expected statement but found none at line %d",lineNumber);
                    exit(1);
                }
            }
            

        }

    }
    else{
        printf("End of line ch is missing at line %d",lineNumber);
        exit(1);
    }

}

void processAddLine(char *lineStr, int lineNumber){
    char * currentToken = getNextToken(NULL,delimiterBetweenTokens);
    char * nextToken = getNextToken(NULL,delimiterBetweenTokens);
    char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
    if(!(lastToken != NULL && currentToken != NULL && nextToken != NULL)){
        printf("Expected statements syntax error at line %d ", lineNumber);
        exit(1);
    }
    int lenOfLast = strlen(lastToken);
    if(lastToken[lenOfLast-1]  == '.'){
        removeCharFromLast(lastToken, lenOfLast);
        lastToken = removedEndOfLineCh; 
        lastToken[lenOfLast] = '\0'; 
    }
    int lineLength = strlen(lineStr);
    int myCondition = (strcmp(keywordArray[2],nextToken));//check if to keyword appeared in line and at the right position
    if(lineStr[lineLength-1] == '.'){
        if(!myCondition){
            if(isIntegerConstant(currentToken)){
                if((checkIfDeclaredBefore(lastToken))){
                    int getIndexOfLastToken = getIndexOfIdentifier(lastToken);
                    long getLastTokenVal = getTheValueOfIdentifier(getIndexOfLastToken);
                    long tempCurrentVal = atol(currentToken) + getLastTokenVal;
                    int lenOfTempCurr = get_int_len_with_negative(tempCurrentVal);
                    
                    if(checkIfExceeds(lenOfTempCurr,currentToken)){
                        printf("The sum is exceed at line %d",lineNumber);
                        exit(1);
                    }
                    instertTheIntegerValue(tempCurrentVal,getIndexOfLastToken);                    
                }
                else{
                    printf("The identifier you are trying to add is not declared before at line %d",lineNumber);
                    exit(1);
                }
            }
            else if(!(isIntegerConstant(currentToken))){
                    if((checkIfDeclaredBefore(lastToken) &&  checkIfDeclaredBefore(currentToken))){
                        int getTheIndexToBeAdd = getIndexOfIdentifier(currentToken);
                        long getTheValueToBeAdded = getTheValueOfIdentifier(getTheIndexToBeAdd);
                        int getTheIndexOfLast = getIndexOfIdentifier(lastToken);
                        long getTheValOfLast = getTheValueOfIdentifier(getTheIndexOfLast);
                        long sumOfTwo = getTheValOfLast + getTheValueToBeAdded;
                        int lenOfSum = get_int_len_with_negative(sumOfTwo);
                        if(checkIfExceeds(lenOfSum,currentToken)){
                            printf("The sum is exceed at line %d",lineNumber);
                            exit(1);

                        }
                        instertTheIntegerValue(sumOfTwo,getTheIndexOfLast);
                    }
                    else{
                        printf("The identifier you are trying to add is not declared before at line %d",lineNumber);
                        exit(1);
                    }
            }
            else{
                printf("Either variable you are trying to add is not declared before or integer value does not exist at line %d ", lineNumber);
                exit(1);
            }

        }
        else{
            printf("Expected to keyword!! at line %d",lineNumber);
            exit(1);
        }
    }
    else{
        printf("End of line character couldn't found at line %d", lineNumber);
        exit(1);
    }

}

void processMoveLine(char *lineStr, int lineNumber){
    char * currentToken = getNextToken(NULL,delimiterBetweenTokens);
    char * nextToken = getNextToken(NULL,delimiterBetweenTokens);
    char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
    int myCondition = (strcmp(keywordArray[2],nextToken));
    if(!(lastToken != NULL && currentToken != NULL && nextToken != NULL)){
        printf("Expected statements syntax error at line %d ", lineNumber);
        exit(1);
    }
    int lenOfLast = strlen(lastToken);
    if(lastToken[lenOfLast-1]  == '.'){
        removeCharFromLast(lastToken, lenOfLast);
        lastToken = removedEndOfLineCh; 
        lastToken[lenOfLast] = '\0'; 
    }
    int lineLength = strlen(lineStr);
    int myCondition1 = (strcmp(keywordArray[2],nextToken));//check if to keyword appeared in line and at the right position
    if(lineStr[lineLength-1] == '.'){
        if(!myCondition1){
            if(isIntegerConstant(currentToken)){
                if((checkIfDeclaredBefore(lastToken))){
                    int lenOfCurrentToken = strlen(currentToken);
                    if(checkIfExceeds(lenOfCurrentToken,currentToken)){
                        printf("The value to trying to initialize is exceeds the limit at line %d",lineNumber );
                        exit(1);
                    }
                    else{
                        int myIndex = getIndexOfIdentifier(lastToken);
                        long currentVal = atol(currentToken);
                        long a = strlen(currentToken);
                        instertTheIntegerValue(currentVal,myIndex);
                    }
                }
                else{
                    printf("Identifier did not created before");
                    exit(1);
                }

            }
            else if((checkIfDeclaredBefore(currentToken)&& checkIfDeclaredBefore(lastToken))){
                int getMyLastTokenIndex = getIndexOfIdentifier(lastToken);
                int getCurrentTokenIndex = getIndexOfIdentifier(currentToken);
                long getTheCurrentVal = getTheValueOfIdentifier(getCurrentTokenIndex);
                long getTheLastTokenVal = getTheValueOfIdentifier(getMyLastTokenIndex);
                getTheLastTokenVal = getTheCurrentVal;
                instertTheIntegerValue(getTheLastTokenVal,getMyLastTokenIndex);
            }
            else{
                printf("The variable did not declared before...");
                exit(1);
            }
        }
        else if(myCondition != 0){
            printf("to keyword expected found none! at line ,%d",lineNumber);
            exit(1);
        }
        else{
            printf("Unexpected character occured at line %d", lineNumber);
        }
    }
}

void processSubLine(char *lineStr, int lineNumber){
    char * currentToken = getNextToken(NULL,delimiterBetweenTokens);
    char * nextToken = getNextToken(NULL,delimiterBetweenTokens);
    char * lastToken = getNextToken(NULL,delimiterBetweenTokens);
    int myCondition = (strcmp(keywordArray[2],nextToken));
    if(!(lastToken != NULL && currentToken != NULL && nextToken != NULL)){
        printf("Expected statements syntax error at line %d ", lineNumber);
        exit(1);
    }
    int lenOfLast = strlen(lastToken);
    if(lastToken[lenOfLast-1]  == '.'){
        removeCharFromLast(lastToken, lenOfLast);
        lastToken = removedEndOfLineCh; 
        lastToken[lenOfLast] = '\0'; 
    }
    int lineLength = strlen(lineStr);
    int myCondition1 = (strcmp(keywordArray[8],nextToken));//check if from keyword appeared in line and at the right position
    if(lineStr[lineLength-1] == '.'){
        if(!myCondition1){
            if(isIntegerConstant(currentToken)){
                if((checkIfDeclaredBefore(lastToken))){
                    long substractedVal = 0;
                    long getIndexOfLastToken = getIndexOfIdentifier(lastToken);
                    long getLastTokenVal = getTheValueOfIdentifier(getIndexOfLastToken);
                    substractedVal = getLastTokenVal - atol(currentToken);
                    int myLength = get_int_len_with_negative(substractedVal);
                    instertTheIntegerValue(substractedVal,getIndexOfLastToken);
                }
                else{
                    printf("The identifier you are trying to substract is not declared before at line %d",lineNumber);
                    exit(1);
                }
            }
            else if(!(isIntegerConstant(currentToken))){
                    if((checkIfDeclaredBefore(lastToken) &&  checkIfDeclaredBefore(currentToken))){
                        long result = 0;
                        int getTheIndexToBeSub = getIndexOfIdentifier(currentToken);
                        long getTheValueToBeSubstract = getTheValueOfIdentifier(getTheIndexToBeSub);
                        int getTheIndexOfLast = getIndexOfIdentifier(lastToken);
                        long getTheValOfLast = getTheValueOfIdentifier(getTheIndexOfLast);
                        result = getTheValOfLast - getTheValueToBeSubstract;
                        instertTheIntegerValue(result,getTheIndexOfLast);
                    }
                    else{
                        printf("The identifier you are trying to Substract is not declared before at line %d",lineNumber);
                        exit(1);
                    }
            }
            else{
                printf("Either variable you are trying to substract is not declared before or integer value does not exist at line %d ", lineNumber);
                exit(1);
            }

        }
        else{
            printf("Expected from keyword!! at line %d",lineNumber);
            exit(1);
        } 
    }
}

void processIntLine(char *lineStr, int lineNumber){
    char *nextToken;
    int lineLength = strlen(lineStr)-1;
    int lastItemInLineStr = lineLength;
    char *currentToken = getNextToken(NULL,delimiterBetweenTokens);
    if(currentToken!= NULL){//check whether there are anything after int dec.
        while(currentToken!=NULL){
            int tokenLength = strlen(currentToken);
            if(lineStr[lastItemInLineStr] == '.'){
                if(moreThanOneComma(currentToken)>1){
                    int counter = strlen(currentToken)-2;//dot+index = 0;
                    int holdIndexOfComma = 0; 
                    int startIndex = 0;
                    int temp = 0;
                    while(counter >= 0){
                        holdIndexOfComma = findIndexOfComma(currentToken,startIndex);
                        if(currentToken[holdIndexOfComma+1] == ','){
                            printf("Syntax error extra comma character found at line %d",lineNumber);
                            exit(1);
                        }
                        seperateToken(currentToken,startIndex,holdIndexOfComma);
                        if(checkIfValidIdentifer(seperatedFromComma,lineNumber)){
                            if(!(checkIfDeclaredBefore(seperatedFromComma))){
                                InsertTheIdentifier(seperatedFromComma);
                                numberOfIdentifiers++;
                                temp = holdIndexOfComma - startIndex;
                                startIndex = holdIndexOfComma;
                                counter -= temp;
                            }
                            else{
                                printf("%s already declared, at line %d",seperatedFromComma,lineNumber);
                                exit(1);
                            }
                        }
                        else{
                            printf("Syntax error, Identifier declared before, no duplicates allowed at line %d",lineNumber);
                            exit(1);
                        }

                    }
                }
                else if(currentToken[tokenLength-1] == ','|| currentToken[tokenLength-1] == '.'){
                    removeCharFromLast(currentToken,tokenLength);
                    if((checkIfValidIdentifer(removedEndOfLineCh,lineNumber)&& (!checkIfDeclaredBefore(removedEndOfLineCh)))){
                        numberOfIdentifiers++;
                        InsertTheIdentifier(removedEndOfLineCh);
                    }
                    else{
                        printf("Unvalid Identifier declaration at line %d",lineNumber);
                        exit(1);
                    }
                }
                else if(currentToken == ',' || currentToken == '.'){

                }
                
                else if((checkIfValidIdentifer(currentToken,lineNumber)&&(!checkIfDeclaredBefore(currentToken)))){
                    InsertTheIdentifier(currentToken);
                    numberOfIdentifiers++;
                }
                else{
                    printf("Unexpected Error at line %d",lineNumber);
                    exit(1);
                }

            }
            else{
                printf("End of Line character expected but not found.");
                exit(1);
            }
        currentToken = getNextToken(NULL,delimiterBetweenTokens);
        }
    }
    else{
        printf("Variable declaration expected... at line %d",lineNumber);
        exit(1);
    }
    return;
   }


void processLine(char *lineStr, char *token, int lineNumber, int lineCount, FILE* theFile){
    if(!strcmp(token,keywordArray[1])){//process int line.
        processIntLine(lineStr,lineNumber);
    }
    else if (!strcmp(token,keywordArray[0])){//process move line.
        processMoveLine(lineStr,lineNumber);
    }
    else if(!strcmp(token,keywordArray[6])){
        processAddLine(lineStr,lineNumber);
    }
    else if(!strcmp(token,keywordArray[7])){
        processSubLine(lineStr,lineNumber);
    }
    else if(!strcmp(token,keywordArray[4])){
        processOutLine(lineStr,lineNumber);
    }
    else if(!strcmp(token,keywordArray[3])){
        processLoopLine(lineStr,lineNumber, lineCount, theFile);
    }
    else{
        printf("Syntax Error at line %d",lineNumber);
        exit(1);
    }
}





int main(){
    FILE *theFileRead;
    theFileRead = fopen("C:\\Users\\batur\\c projects\\project2\\readfile.txt","r");
    if(theFileRead == NULL){
        perror("Error occured while opening file");
        return -1;
    }
    int lineChCounter =  countLineChNumber(theFileRead);//total character number in the file
    int lineCount = 0; //hold the current line number.
    int lineLength = 0;
    char openComm = '{';
    char closeComm = '}';
    valueOfIdentifiers = (long*)malloc(220*sizeof(long));
    initializeTheIdentifiersValue();
    holdIdentifiers = (char*)malloc(l*sizeof(char*));
    while(whereLeft <= lineChCounter){
        lineChCounter++;
        lineCount++;
        char *lineStr = getTheNextLine(lineChCounter,theFileRead);
        lineLength = strlen(lineStr);
        if(doesLineContainsAndEnds(lineStr,openComm)){
            if(doesLineContainsAndEnds(lineStr,closeComm)){
                lineStr = removeCommentBlock(lineStr,openComm);
            }
            else{
                printf("Comment did not closed.. at line %d", lineCount);
                exit(1);
            }
        }
        char *token = getNextToken(lineStr,delimiterBetweenTokens);//first line tokenized
        whereLeft += lineLength+2;
        processLine(lineStr,token,lineCount,lineChCounter,theFileRead);

    }
}