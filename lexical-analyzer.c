#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

const char *keywordArray[] = {"move","int","to","loop","out","newline","add","sub","from","times"};
int intconstants[10] ={'0','1','2','3','4','5','6','7','8','9'};
const char *delimiterBetweenTokens = ' ';
const char *endOfLineCharacter = '.';
const char *commentSymbol = '{';
const char *closeCommentSymbol = '}';
const char *openBlockSymbol = '[';
const char *openStrConst = '"';
char *removedFromFront;
char *removedEndOfLineCh;
int whereLeft = 0;//index number of where we left on reading file.
char *holdContentsOfStringConstant; 
int openBlockCounter = 0;
int linePosition = 0;
char *lineToSplit;
int lineCount = 0;

char *getNextToken(char *line,const char *delimiter){
    int count = 0;
    if(line != NULL){
        linePosition = 0;
        lineToSplit = line;
    }
    if(lineToSplit == NULL){
        return NULL;
    }
    if(linePosition >= strlen(lineToSplit)){
        lineToSplit = NULL;
        linePosition = 0;
        return NULL;
    }
    while(linePosition < strlen(lineToSplit) && lineToSplit[linePosition] != delimiter && lineToSplit[linePosition] != '\n'){
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
bool isValidIdentifier(char *identifier){
    if(strlen(identifier)>21 || identifier[0] =='+' || identifier[0] == '-'){
        return false;
    }
    else if(isdigit(identifier[0])){
        return false;
    }
    else{
        for(int i = 1; i<strlen(identifier); i++){
            if(!isalnum(identifier[i]) && identifier[i] != '_'){//returns non zero value
                return false;
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

bool tokenContains(char *token, char include){
    for(int i = 0; i<strlen(token); i++){
        if(token[i] == include){
            return true;
        }
        
    }
    return false;
}

//checks for comment block or string constant declaration whether they finishes.
//determines if given token is valid integer constant.
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

int countLineChNumber(FILE *theFileRead){
    int count = 0;
    int c = 0;
    for (c = getc(theFileRead); c != EOF; c = getc(theFileRead)){
        count = count + 1;
    }
    return count;
}

char *contentsOfTheLine(FILE *theFileRead, int position){
    char *line1;
    int count = 0;
    char i = "";
    fseek(theFileRead,position,SEEK_SET);
    while(i != '\n' && i != EOF){
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
bool finishesOnLine(char *token){//this functions checks if comment ends on the current line.
    if(!tokenContains(token,closeCommentSymbol) && (whereLeft-2) - strlen(token)!=0){
        token = getNextToken(NULL,delimiterBetweenTokens);
        while(token!=NULL){
            if(token == NULL){
                break;
            }
            else if(tokenContains(token,closeCommentSymbol)){
                return true;
            }
            token = getNextToken(NULL,delimiterBetweenTokens);
        }
    }
    return false;
}
bool checkIfEndsComment(char *token, const char *seperator, char *line, FILE *theFileRead, int lineChCounter){
    if(tokenContains(token,closeCommentSymbol)){
        return true;
    }
    else if(finishesOnLine(token)){
        return true;
    }
    else{
        while(whereLeft <= lineChCounter){
            line = contentsOfTheLine(theFileRead,whereLeft);
            lineChCounter++;
            lineCount++;
            whereLeft += strlen(line)+2;
            token = getNextToken(line,delimiterBetweenTokens);
        while(token!=NULL){
            if(tokenContains(token,closeCommentSymbol)){
                return true;
            }
            token = getNextToken(NULL,delimiterBetweenTokens);
        }
        }
        return false;
    }
    return false;
    
}   
bool checkIfEndsStringConstant(char *token,const char *seperator,int size){
    int excludeTokenLength = 0;
    int j = 0;
    holdContentsOfStringConstant =  malloc(sizeof(char)*size);
    for(int i = 1; i<strlen(token); i++){//ignore token[0] we are sure that is "".
        holdContentsOfStringConstant[j] = token[i];
        j++;
        if(token[i] == seperator){
            holdContentsOfStringConstant[j-1] = '\0';
            return true;
        }
    }            
return false;
}

int main(){
    setlocale(LC_ALL,"Turkish");
    FILE *theFileRead;
    FILE *fptr;
    fptr = fopen("C:\\Users\\batur\\c projects\\project1\\wrt","w");
    theFileRead = fopen("C:\\Users\\batur\\c projects\\project1\\readfile.txt","r");
    if(theFileRead == NULL){
        perror("Error occured while opening file");
        return -1;
    }
    if(fptr == NULL){
        printf("Error occured");
        exit(1);
    }
    char c;
    int tokenLength = 0;
    int lineChCounter =  countLineChNumber(theFileRead);//total character number in the file
    while(whereLeft <= lineChCounter){
            lineChCounter++;
            lineCount++;
            int lineLength;
            char *lineStr = contentsOfTheLine(theFileRead,whereLeft);
            lineLength = strlen(lineStr);
            char *token1 = getNextToken(lineStr,delimiterBetweenTokens);//first line tokenized
            whereLeft += lineLength+2;//to include newLine Character \n
        while(token1!=NULL){
            tokenLength = strlen(token1);  
            if(isKeyword(token1)){
                fprintf(fptr,"Keyword %s\n",token1);
            }
            else if(isIntegerConstant(token1)){
                fprintf(fptr,"IntConstant %s\n",token1);
            }
            else if(token1[0] == '"'){
                if(checkIfEndsStringConstant(token1,openStrConst,tokenLength)){
                    int hold = strlen(holdContentsOfStringConstant);
                    if(holdContentsOfStringConstant[hold-1] == ':'){
                        removeCharFromLast(holdContentsOfStringConstant,(hold));                
                    }
                    if(holdContentsOfStringConstant[hold]==','){
                        removeCharFromLast(holdContentsOfStringConstant,hold);
                        fprintf(fptr,"String Constant %s\nSeperator\n",removedEndOfLineCh);
                    }
                    else{
                        fprintf(fptr,"String Constant %s\n",removedEndOfLineCh);
                    }
                }
                else{
                    fprintf(fptr,"Unclosed string constant at line %d\n",lineCount);
                }
            } 
            else if(tokenContains(token1,commentSymbol)){
                if(checkIfEndsComment(token1,commentSymbol,lineStr,theFileRead,lineChCounter)){
                }
                else{
                    fprintf(fptr,"Unclosed comment block... at line %d\n",lineCount);
                }
            }
            else if(token1[strlen(token1)-1] == endOfLineCharacter){//checks if token ends with dot operator.
                    removeCharFromLast(token1,tokenLength);
                if(isKeyword(removedEndOfLineCh)){
                    fprintf(fptr,"Keyword %s\nEndOfLineCharacter\n",removedEndOfLineCh);
                }
                else if(isValidIdentifier(removedEndOfLineCh)){
                    fprintf(fptr,"Identifier %s\nEndOfLineCharacter\n", removedEndOfLineCh);
                }
                else{
                    fprintf(fptr,"Unrecognized decleration at line %d\n",lineCount);
                }
            }
            else if(token1[0] == openBlockSymbol || token1[tokenLength-1] == openBlockSymbol ){//if token include only [ or last ch of token include [
                openBlockCounter++;
                if(strlen(token1)==1){
                    fprintf(fptr,"OpenBlock\n");
                }
                else if(strlen(token1)>1){
                    if(token1[0] == openBlockSymbol){
                        fprintf(fptr,"OpenBlock\n");
                        removeCharFromFront(token1,tokenLength);
                        if(isKeyword(removedFromFront)){
                            fprintf(fptr,"Keyword %s\n",removedFromFront);
                        }
                        else if(isIntegerConstant(removedFromFront)){
                            fprintf(fptr,"IntegerConstant %s\n",removedFromFront);
                        }
                        else if(isValidIdentifier(removedFromFront)){
                            fprintf(fptr,"Identifier %s\n",removedFromFront); 
                        }
                        else{
                            fprintf(fptr,"Unrecognized character %d\n",lineCount);
                        }
                    }
                } 
            }
            else if(token1[tokenLength-1] == ']' || token1[0] == ']'){
                openBlockCounter--;
                if(tokenLength == 1){
                    fprintf(fptr,"CloseBlock\n");
                }
                else if(tokenLength > 1){
                    if(token1[0] == ']'){
                        removeCharFromFront(token1,tokenLength);
                        fprintf(fptr,"CloseBlock\n");
                        if(isKeyword(removedFromFront)){
                            fprintf(fptr,"Keyword %s\n",removedFromFront);
                        }
                        else if(isIntegerConstant(removedFromFront)){
                            fprintf(fptr,"IntegerConstant %s\n",removedFromFront);
                        }
                        else if(isValidIdentifier(removedFromFront)){
                            fprintf(fptr,"Identifier %s\n",removedFromFront); 
                        }
                        else{
                            fprintf(fptr,"Unrecognized character %d\n",lineCount);
                        } 
                    }
                    else if(token1[tokenLength-1] == ']'){
                        removeCharFromLast(token1,tokenLength);
                        if(isKeyword(removedEndOfLineCh)){
                            fprintf(fptr,"Keyword %s\nCloseBlock\n",removedEndOfLineCh);
                        }
                        else if(isIntegerConstant(removedEndOfLineCh)){
                            fprintf(fptr,"IntegerConstant %s\nCloseBlock\n",removedEndOfLineCh);
                        }
                        else if(isValidIdentifier(removedEndOfLineCh)){
                            fprintf(fptr,"Identifier %s\nCloseBlock\n",removedEndOfLineCh); 
                        }
                        else{
                            fprintf(fptr,"Unrecognized character %d\n",lineCount);
                        } 
                    }
                }
            }
            else if(token1[tokenLength-1] == ','){
                removeCharFromLast(token1,tokenLength);
                if(isKeyword(removedEndOfLineCh)){
                    fprintf(fptr,"Keyword %s\nSeperator\n",removedEndOfLineCh);
                }
                else if(isIntegerConstant(removedEndOfLineCh)){
                    fprintf(fptr,"IntegerConstant %s\nSeperator\n",removedEndOfLineCh);
                }
                else if(isValidIdentifier(removedEndOfLineCh)){
                    fprintf(fptr,"Identifier %s\nSeperator\n",removedEndOfLineCh); 
                }
                else{
                    fprintf(fptr,"Unrecognized character at line %d\n",lineCount);
                } 
            }
            else if(token1 == ","){
                fprintf(fptr,"Seperator\n");
            }
            else if(isValidIdentifier(token1)){
                fprintf(fptr,"Identifier %s\n",token1);
            }
            else{
                fprintf(fptr,"Unrecognized Character or declaration at line %d\n",lineCount);
            }
        token1 = getNextToken(NULL,delimiterBetweenTokens);
        }
    }
    if(openBlockCounter != 0){
            fprintf(fptr,"Error occured unclosed Block\n");
            }
    fprintf(fptr,"Lexical Analyzer is done");
    fclose(theFileRead);
    fclose(fptr);
}
