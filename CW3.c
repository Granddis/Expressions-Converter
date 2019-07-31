#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<conio.h>
#define SIZE 100

struct numnode{
    double num;
    struct numnode *nextptr;
}*topnptr= NULL;


struct opnode{
    char op;
    struct opnode *nextptr;
}*topptr= NULL;

struct opdnode{
    char opd[50];
    struct opdnode *nextptr;
}*topdptr= NULL;


/*----------- prototype -----------*/
void pushd(char *string);
char* popd();
void push(char element);
char pop();
void pushn(double data);
double popn();
int isoperator(char element);
int precedence(char element);
int check_expression(char string[][50],int a);
double evaluate(char postfix[][50],int a);
int converttopostfix(char string[][50],char postfix[][50],int a, int type);
int converttoprefix(char string[][50],char postfix[][50],int a, int type);
char *converttoinfix(char string[][50],int a);


void pushd(char *string){
    struct opdnode *newptr; // create a new node
    newptr= (struct opdnode*)malloc(sizeof(struct opdnode));    // assign memory location
    strcpy(newptr->opd,string); // copy the string
    if(topdptr == NULL)
        newptr->nextptr=NULL;   //point to null if top is empty
    else{
        newptr->nextptr=topdptr;
    }
        topdptr = newptr;   // top stack take the new data
}
char* popd(){
    struct opdnode *tempdptr;   // create a new node
    char string[50];
    tempdptr =topdptr;  // temporary ptr take the top pointer
    strcpy(string,topdptr->opd);    //cop out the data from the top stack
    topdptr=topdptr->nextptr;   // the new top pointer is the next pointer
    free(tempdptr); // clear the temporary pointer
    return string;
}

void push(char element){
    struct opnode *newptr;  // create a new node
    newptr =(struct opnode*) malloc(sizeof(struct opnode)); // assign memory location to the new node
    newptr->op=element; // new node take the element
    if(topptr==NULL)
        newptr->nextptr=NULL;   // new node point to null if empty
    else
        newptr->nextptr=topptr; // new node point to the top
        topptr=newptr;  // the top is now the new node
    }

char pop(){
    struct opnode *tempptr;
    char c;
    tempptr=topptr; // temp take the top ptr
    c=topptr->op;   // assign data of top ptr to c
    topptr=topptr->nextptr; // top pointer is now the next pointer
    free(tempptr);  // clear the temp pointer
    return c;

}

void pushn(double data){
    struct numnode *newptr; // create a new node
    newptr =(struct numnode*) malloc(sizeof(struct numnode));   // assign memory location to the new node
    newptr->num=data;   // new pointer take the data
    if(topptr==NULL)
        newptr->nextptr=NULL;   // new node point of null if top is empty
    else
        newptr->nextptr=topnptr;    //new node point to the top
        topnptr=newptr; // new node is now the top
    }

double popn(){
    struct numnode *tempnptr;
    double value;
    tempnptr=topnptr;   // temp take the top
    value=topnptr->num; // assign data of top to value
    topnptr=topnptr->nextptr;   // top stack is now the next pointer
    free(tempnptr); // clear the temp pointer
    return value;

}
int isparentheses(char element){
    switch(element){
        case'(': return 1;
        case')': return 2;
        default: return 0;
    }
}

int isoperator(char element){
    switch(element){
        case'^':
        case'+':
        case'-':
        case'*':
        case'/': return 1;
        default : return 0;

    }
}
int precedence(char element){
    switch(element){
        case ')':
        case '(': return 1;
        case '+':
        case '-': return 2;
        case '*':
        case '/': return 3;
        case '^': return 4;
        default : return 0;
    }
}


int main(){
    int size,type=0,e=-1,a,b=-1,c,d=-1,i,x,y,lengthpost,lengthpre,extra=0,error=0,charcount=0,decimals=0,f=2;
    char *sptr,*ssptr,expression[50],s[50],string[SIZE][50],infix[SIZE][50],prefix[SIZE][50],postfix[SIZE][50],choice[50],sel[50],pick[50],locations[50],location[50],*bptr,z,instring[50],prestring[50],poststring[50],space[2];
    char ans[50];
    instring[0]='\0';
    prestring[0]='\0';
    poststring[0]='\0';
    space[0]=' '; space[1]='\0';
    double sum;
    FILE *fptr;

    push('#');  // push a random thing

    printf("This is an infix , prefix or postfix converter Program.\n ");
    printf("First you need to choose the input type:\n\n");
     do{    // let user select their input method
        type=0;
        f=2;
        printf("Please choose the following input options:\n");
        printf("A. read from file (exact file location is required)\n");
        printf("B. Read from keyboard input\n");
        printf("C. to quit.\n\n");
        printf("\nYour Selection: ");
        error=0;
        charcount=0;
        gets(pick);
        printf("\n");
        sptr=pick;
        while(*sptr!='\0'){//error proof the selection
            if(isalpha(*sptr)){ // if alphabet , copy into character z
                z=*sptr;
                charcount++;
                sptr++;
            }
            else if(isspace(*sptr)){ // if space , ignore
                sptr++;
            }
            else { // other than space or alphabet gives error
                error++;
                sptr++;
            }
        }
        charcount+=error;
        if(charcount==1){
            switch (toupper(z)){ //check character z
                case 'A': e=1; break;
                case 'B': e=2; break;
                case 'C': e=3; break;
                default : e=-1;
            }
        }
        else{   // if non of the above is inputted
            e=-1;
        }
        if(e==3){   // if quit
            return 0;
        }
        if(e==1){   // if read from file is selected
            printf("Please type the file location(default file locations: C:\\C51PGA\\default.txt ):\n");
            printf("\nYour file locations:");
            gets(location);
            sptr=strtok(location," ");  // to remove spaces
            strcpy(location,sptr);
            strcpy(locations,location);
            while((fptr=fopen(locations,"r+"))==NULL){  // if file cannot be opened , loop back
                puts("File could not be opened.\n");
                printf("\nYour file locations:");
                gets(location);
                sptr=strtok(location," ");  // to remove spaces
                strcpy(location,sptr);
                strcpy(locations,location);
            }
            fseek (fptr, 0, SEEK_END);  // read to the last line to ready
            size = ftell(fptr);
            if(size==0){
                printf("the file is empty!Please Try Again\n");
                fclose(fptr);
                type=-1;
                f=1;
                e=-1;
            }
            else{
                rewind(fptr);
            }
            while(type==0){
                while(f==2){
                    fgets(s,50,fptr);
                    sptr=strtok(s,"\n");
                    strcpy(s,sptr);
                    strcpy(expression,s);
                    printf("\nExpression: %s\n",s);
                    printf("Is this the expression u want to evaluate?\n");
                    printf("A-to continue.\n");
                    printf("B-to read the next line of the file.\n");
                    printf("C-to choose input method again.\n\n");
                    error=0;
                    charcount=0;
                    printf("Your Selection: ");
                    gets(pick);
                    printf("\n");
                    sptr=pick;
                    while(*sptr!='\0'){//check selection
                        if(isalpha(*sptr)){ // if alphabet , copy into character z
                            z=*sptr;
                            charcount++;
                            sptr++;
                        }
                        else if(isspace(*sptr)){ // if space , ignore
                            sptr++;
                        }
                        else { // other than space or alphabet gives error
                            error++;
                            sptr++;
                        }
                    }
                    charcount+=error;
                    if(charcount==1){
                        switch (toupper(z)){ //check character z
                            case 'A': f=1; break;
                            case 'B': f=2; break;
                            case 'C': f=3; break;
                            default : f=3;
                        }
                    }
                    else{   // if more than 1 character
                        e=-1;
                    }
                }
                if(f==1){   // if A is selected
                    ssptr=s;
                    while(*ssptr!='\0'){    // expression error checking
                        if(isparentheses(*ssptr)==0 && isoperator(*ssptr)==0 && isdigit(*ssptr)==0 && isspace(*ssptr)==0 && *ssptr !='.') // anything other than operator, parentheses or digit, gives error
                            error++;
                            ssptr++;
                    }
                if(error==0){   // if is the correct input syntax
                    i=0;
                    sptr=strtok(s," "); // tokenized the string
                    while(sptr!=NULL){  // tokenized the string
                        sprintf(string[i++],"%s",sptr);
                        sptr=strtok(NULL," ");
                    }
                    type=check_expression(string,i);//check for expression or error
                    if(type != 0){
                            if(type==1 || type==2)  // case prefix or infix expression , convert to postfix for calculation
                                lengthpost=converttopostfix(string,postfix,i,type);
                                else{   // else postfix , copy the whole 2-dimension string into 2-dimension postfix array
                                lengthpost=i;
                                for(a=0; a<i; a++)
                                    strcpy(postfix[a],string[a]);
                                }
                                sum=evaluate(postfix,lengthpost);   // calculation
                                if(sum==-999){  // if divisor is 0, -999 is my magic number
                                    printf("Divisor cannot be zero.Please try Again.\n");
                                    type=0;
                                    }
                    }
                    else if(type==0){   //if is neither prefix , infix or postfix then loop back
                        f=2;
                        e=-1;
                        break;
                    }
            }
                else{   // if wrong input syntax , return error and loop back
                    printf("This is not an expression.\n");
                    f=2;
                    e=-1;
                    break;
                }
                }
                else if(f==3){  //if C is selected, choose another input method
                    f=2;
                    e=-1;
                    break;

                }
            }
            if(f==1 && type!=0){    // if user selected A and correct expression is inputted
            strcpy(s,expression);   // copy the expression
            fseek(fptr,0,SEEK_END); // read to the end and ready for append
             fputs(" \n",fptr);     // append a new line
            }
    }
    }while(e==-1); // loop back while there is error or no matched character found

    if(e==2){
        strcpy(locations,"C:\\G51PGA\\input.txt");
        if((fptr=fopen(locations,"w"))==NULL){  //if file cannot be opened
            printf("file could not be opened\n");
            return 0;   // terminate the program
        }
        while(type==0){ // loop back when there is any error or when inputted from keyboard is selected
            error =0;
            printf("Please Enter An Expression (Prefix,Infix,Postfix)\n");
            printf("With the following rules:\n");
            printf("1.)Maximum of 6 operands with digits from -99-99 for each operands.\n");
            printf("2.)Maximum of 5 operators e.g(+,-,*,/,^).\n");
            printf("3.)Spaces is mandatory in between any operators and operands.\n");
            printf("\n");
            printf("Your Expression: ");
            i=0;
            gets(expression);   // get expression
            strcpy(s,expression);
            ssptr=s;
            while(*ssptr!='\0'){    // error checking
                if(isparentheses(*ssptr)==0 && isoperator(*ssptr)==0 && isdigit(*ssptr)==0 && isspace(*ssptr)==0 && *ssptr !='.') // anything other than operator, parentheses or digit, gives error
                    error++;
                    ssptr++;

            }
            if(error==0){   // if no syntax error
                sptr=strtok(s," "); // tokenized the string
                while(sptr!=NULL){
                    sprintf(string[i++],"%s",sptr);
                    sptr=strtok(NULL," ");
                }
                type=check_expression(string,i);//check for error
                if(type != 0){
                        if(type==1 || type==2)  // case prefix or infix expression , convert to postfix for calculation
                            lengthpost=converttopostfix(string,postfix,i,type);
                            else{   // else postfix , copy the whole 2-dimension string into 2-dimension postfix array
                            lengthpost=i;
                            for(a=0; a<i; a++)
                                strcpy(postfix[a],string[a]);
                            }
                            sum=evaluate(postfix,lengthpost);   // calculated sum
                            if(sum==-999){  // if the divisor is 0  , return error
                                printf("Divisor cannot be zero.Please try Again.\n");
                                type=0;
                                }
                }
            }
            else{   // if there is syntax error
                printf("Please input integer and operator only.\n");
            }
            if(type!=0){    // if no error in expression
                strcpy(s,expression);
                fprintf(fptr,"%s\n",s); // print in to the input.txt
                    fclose(fptr);   // close input.txt
                    strcpy(locations,"C:\\G51PGA\\output.txt");
                    if((fptr=fopen(locations,"w"))==NULL){  // if file cannot be opened
                        printf("file could not be opened\n");
                        return 0;   // terminate the program
                    }
            }
        }
    }
    sprintf(ans,"%f",sum);  // print sum into a string
    sptr=ans;
    while(*sptr!='\0'){ // read from last
        sptr++;
    }
    sptr--;
    while(*sptr!='.'){  //  calculate number for decimals
        if(*sptr!='0'){     // read the smallest non zero decimals
            while(*sptr!='.'){
                decimals++;
                sptr--;
            }
        }
        else{   // move to the next character
            sptr--;
        }
    }
    sprintf(ans,"%.*f",decimals,sum);   // convert to the correct number of decimals

    if(type==1){ //if infix or postfix expression
        strcpy(instring,converttoinfix(postfix,i)); //since prefix is converted to postfix for calculation , hence give the postfix expression to convert
        for(x=0; x<lengthpost; x++){    // print the postfix array into a string
            strcat(poststring,postfix[x]);
            strcat(poststring,space);
        }
    }

     else if(type==2){  // if  infix expression
        lengthpre=converttoprefix(string,prefix,i,type)-1;  // convert to prefix
        for(x=lengthpre;x>=0; x--){ // print the prefix array into a string
            strcat(prestring,prefix[x]);
            strcat(prestring,space);
        }

        for(x=0; x<lengthpost; x++){    // print the postfix array into a string
            strcat(poststring,postfix[x]);
            strcat(poststring,space);
        }
     }
    else if(type==3) {   // if postfix expression
        strcpy(instring,converttoinfix(postfix,i)); //convert to infix
        lengthpre=converttoprefix(postfix,prefix,i,type)-1;
        for(x=lengthpre;x>=0; x--){     // print the prefix array into string
            strcat(prestring,prefix[x]);
            strcat(prestring,space);
        }
    }
    do {
        if(extra!=0){ // case returned 2nd time
            printf("Your inputted Expression:");
            printf("%s\n",s);
            printf("\n");
        }
        if(type==1){ // if prefix
            printf("This is a prefix Expression.\n");
        }
        else if(type==2){   // if infix
            printf("This is an infix Expression.\n");
        }
        else if(type==3){ //if postfix
            printf("This is a postfix Expression.\n");
        }
        printf("A.Convert to infix,prefix or postfix\n");
        printf("B.Evaluate any type of expression(infix,postfix,prefix)\n");
        printf("C.Exit\n");
        do{
        printf("\nYour Selection: ");
        error=0;
        charcount=0;
        gets(choice);
        printf("\n");
        ssptr=choice;
            while(*ssptr!='\0'){ //check inputted string
                if(isalpha(*ssptr)){ // if alphabet, copy into character
                    z=*ssptr;
                    charcount++;
                    ssptr++;
                }
                else if(isspace(*ssptr)){ // if space , point to next character
                    ssptr++;
                }
                else { //input other than alphabet , gives error
                    error++;
                    ssptr++;
                }
            }
            charcount+=error;
            if(charcount==1){

                switch (toupper(z)){ //check inputted character
                    case 'A': b=0; break;
                    case 'B': b=1; break;
                    case 'C': b=2; break;
                    default : b=-1;
                }
            }
            else{ // if more than 1 character inputted
                b=-1;
            }
                }while(b==-1);

        if(b==1){   //if selection B, print the output
                printf("Calculated output:%s\n",ans);
                fprintf(fptr,"Evaluated sum: %s\n",ans);
            }

        if(b==0){ //if selection A
            printf("\n");
            printf("Do you want to Convert your Expression to:\n");
            if(type==1 && b==0){    //type 1 is prefix
               printf("A.infix \nB.postfix  \nC.for both \nSelect A,B or C option. \n");

            }
            else if(type==2 && b==0){   //type 2 is infix
                printf("A.prefix \nB.postfix \nC.for both \nSelect A,B or C option \n");
            }
            else if(type==3 && b==0){   //type 3 is postfix
                printf("A.prefix \nB.infix \nC.for both \nSelect A,B or C option \n");
            }
            do{
                printf("\nYour Selection: ");
                error=0;
                charcount=0;
                gets(sel);
                sptr=sel;
                while(*sptr!='\0'){//check selection
                    if(isalpha(*sptr)){ // if alphabet , copy into character z
                        z=*sptr;
                        charcount++;
                        sptr++;
                    }
                    else if(isspace(*sptr)){ // if space , ignore
                        sptr++;
                    }
                    else { // other than space or alphabet gives error
                        error++;
                        sptr++;
                    }
                }
                    charcount+=error;
                    if(charcount==1){
                        switch (toupper(z)){ //check character z
                            case 'A': d=0; break;
                            case 'B': d=1; break;
                            case 'C': d=2; break;
                            default : d=-1;
                        }
                    }
                    else{
                        d=-1;
                    }
            }while(d==-1); // loop back while there is error or no matched character found



            if((type==2 || type==3) && d==0){ //if infix or postfix expression and selection A,convert to prefix
                printf("Converted Prefix Expression: ");
                printf("%s\n",prestring);
                fprintf(fptr,"Converted Prefix expression: %s\n",prestring);
            }
             else if((type==2 || type==1 )&& d==1){  // if prefix or infix expression and selection B, convert to Postfix
                printf("Converted Postfix Expression: ");
                printf("%s\n",poststring);
                fprintf(fptr,"Converted Postfix Expression: %s\n",poststring);
             }
            else if(type==1 && d==0) {   // if prefix expression and selection A, convert to infix
                printf("Converted Prefix Expression: ");
                printf("%s\n",prestring); //since prefix is converted to postfix for calculation , hence give the postfix expression to convert
                fprintf(fptr,"Converted Prefix expression: %s\n",prestring);
            }
            else if(type==3 && d==1){    // if postfix expression and selection B , convert to infix
                printf("Converted Infix Expression: ");
                printf("%s\n",instring);
                fprintf(fptr,"Converted Infix Expression: %s\n",instring);
        }
            else if(type==1 && d==2){   // if prefix expression and selection C, convert to infix and postfix
                printf("Converted Infix Expression: ");
                printf("%s\n",instring);
                fprintf(fptr,"Converted Infix Expression: %s\n",instring);
                printf("Converted Postfix Expression: ");
                printf("%s\n",poststring);
                fprintf(fptr,"Converted Postfix Expression: %s\n",poststring);
            }
            else if(type==2 && d ==2) { // if infix and selection C, convert to prefix and postfix
                printf("Converted Prefix Expression: ");
                printf("%s\n",prestring);
                fprintf(fptr,"Converted Prefix expression: %s\n",prestring);
                printf("Converted Postfix Expression: ");
                printf("%s\n",poststring);
                fprintf(fptr,"Converted Postfix Expression: %s\n",poststring);
            }
            else if(type==3 && d==2){   // if post expression and selection C, convert to prefix and infix
                printf("Converted Prefix Expression:");
                printf("%s\n",prestring);
                fprintf(fptr,"Converted Prefix expression: %s\n",prestring);
                printf("Converted Infix Expression: ");
                printf("%s\n",instring);
                fprintf(fptr,"Converted Infix Expression: %s\n",instring);

            }

                printf("\n");
        }
        if(b==2){   //if choice is C stop the function
            fclose(fptr);
            return 0;
        }

        extra++;    // for loop back
        printf("\n");
        printf("Please Enter to Continue.");
        do{
        }while(getchar()!='\n');    //wait for user to enter
       system("cls");  //clear screen
    }while(b !=2);
}


int check_expression(char string[][50], int a){
    int i,countopd=0,countopr=0,error=0,b=0,c,d,e,leftparentheses=0,rightparentheses=0,z,decimals;
    char *ssptr,*sptr,s[50];
    c=a-1;

    for(i=0; i<a; i++){
        if(isoperator(string[i][0])==1){    //if operator
            if((string[i][0]=='-' || string[i][0]=='+') && isdigit(string[i][1])){  // if is - + operator and a number , then it is positive or negative number
                decimals=0;
                sptr=string[i];
                ssptr=string[i];
                while(*ssptr!='\0'){    // calculate number of decimal
                    if(*ssptr=='.'){    // when read until a .
                      while(*ssptr!='\0'){
                        if(isdigit(*ssptr)) // calculate number of decimals
                            decimals++;
                        ssptr++;
                      }
                    }
                    else{   // if is number before .
                        ssptr++;
                    }
                }
                double val = strtod(string[i],&sptr);   // convert then to double
                sprintf(string[i],"%.*f",decimals,val); // print into the string according to the number of decimals
                countopd++; // opdcount ++
                if(*sptr!='\0'){    // if there is anything after the number return error
                    printf("Spaces is required in between\n");
                    return 0;
                }
            }
            else if(isoperator(string[i][1])==1){   // if there is another operator after an operator without spaces
                printf("Spaces is needed in between operator\n");
                return 0;
            }
            else{   // if  is non of the above or no error , consider as operator
            countopr++;
            }
        }
        else if(isdigit(string[i][0])){  // if digit, operand count++
            sptr=string[i];
            while(*sptr!='\0'){ //check string
                if(isdigit(*sptr)==0  && *sptr!='.'){   // if anything other than number, gives error
                    printf("Spaces is required in between asd\n");
                    return 0;
                }
                sptr++;
            }
            countopd++;
        }
        else if(isparentheses(string[i][0])==1){    // if left parentheses , left parentheses count ++
            leftparentheses++;
        }
         else if(isparentheses(string[i][0])==2){   // if right parentheses , right parentheses count++
            rightparentheses++;
            if(rightparentheses> leftparentheses){  // case right parentheses appeared before left parentheses, return error
                printf("Invalid Expression.Please Try Again\n");
                return 0;
            }
        }


    }
    if(countopd==1 && countopr==0){ // if only 1 operand is entered, return error
        printf("1 integer only? Please try again.\n");
        return 0;
    }
    else if(countopd==0 && countopr==1){    // if only 1 operator is inputted, return error
        printf("1 operator only? Please try again.\n");
        return 0;
    }
    else if(countopd==0 && countopr==0){    // if nothing is inputted,return error
        printf("Please input a Valid Expression?\n");
        return 0;
    }

    else if(error!=0){  // if anything other than operator , parentheses or digit is inputted, return error
        printf("This is an invalid expression.Please try again\n");
        return 0;
    }
    else if(leftparentheses>rightparentheses){  // if there are more left parentheses than right parentheses, return error
        printf("There is lacking of %d rightparentheses.Please Try again\n",leftparentheses-rightparentheses);
        return 0;
    }
      else if(rightparentheses>leftparentheses){    // if there are more right parentheses than left parentheses, return error

        printf("There is lacking of %d leftparentheses,Please Try again\n",rightparentheses-leftparentheses);
        return 0;
    }
    else if(countopr> 5 || countopr>countopd){  // if there are more than 5 operators or there are operator than operand, return error
        printf("There are too many operators.Please Try again\n");
        return 0;
    }
    else if(countopd> 6 || countopd-1 != countopr){ // if there are more than 6 operands or lacking of operators, return error
        printf("There are too many operands.Please Try again.\n");
        return 0;
    }

    do{
        if(isparentheses(string[b][0])!=0)
            b++;
    }while(isparentheses(string[b][0])!=0);// loop until it reads a operator or operand

    do{
        if(isparentheses(string[c][0])!=0)
            c--;
    }while(isparentheses(string[c][0])!=0); // read from back until it reads a operator or operand

    if((isoperator(string[b][0])==1 && string[b][1]=='\0' ) && (isdigit(string[c][0])  || isdigit(string[c][1]))){    // if the first character is operator and the last character is digit,return prefix
        countopd=0;
        countopr=0;
        for(d=a-1; d>=0; d--){
            if(isparentheses(string[d][0])!=0){ // if there is parentheses in the expression gives error
            printf("Parentheses is not allowed in prefix.Please Try Again\n");
            return 0;
            }
            else if(isdigit(string[d][0]) || isdigit(string[d][1]))  // if a number is detected countopd ++
                countopd++;
            else if(isoperator(string[d][0]) && string[d][1]=='\0')   // if an operator is detected countopr++
                countopr++;
            if(countopr>=countopd){ //  there are more or equal operator than operand at anytime , return error
            printf("Invalid Prefix Expression.Please Try Again.\n");
            return 0;
        }
        }

        return 1;// if no  error return prefix

    }
    else if((isdigit(string[b][0]) ||isdigit(string[b][1])) && (isdigit(string[c][0]) || isdigit(string[c][1]))){  // if the first character is digit and the last character is digit , return infix
        for(d=0; d<a; d++){
            // if there is a consecutive operator return error
            if(ispunct(string[d][0]) && string[d][0]!='(' && string[d][0]!=')' && isdigit(string[d][1])==0 && string[d+1][0]!='(' && ispunct(string[d+1][0]) && isdigit(string[d+1][1])==0){
                printf("invalid infix expression1\n");
                return 0;
            }
            // if an operator comes after a left parentheses    return error
            if( string[d][0]=='(' && ispunct(string[d+1][0]) && string[d+1][0]!='(' && isdigit(string[d+1][1])==0){
                printf("Invalid Infix Expression2\n");
                return 0;
               }
        }
        return 2;   // if no error return infix
    }
    else if((isdigit(string[b][0]) || isdigit(string[b][1]) )&& (isoperator(string[c][0])==1 && string[c][1]=='\0' )){   //if the first character is a digit and the last character is an operator
        countopd=0;
        countopr=0;
        for(d=0; d<a; d++){ // read from the first character
            if(isparentheses(string[d][0])!=0){ //if a parentheses is seen in prefix , return error
            printf("Parentheses is not allowed in postfix.Please Try Again\n");
                return 0;
            }
            else if(isdigit(string[d][0]) || isdigit(string[d][1]))
                countopd++;
            else if(isoperator(string[d][0]) && string[d][1]=='\0')
                countopr++;
            if(countopr>=countopd){ // if there are more or equal operator than operand at anytime , return error
            printf("Invalid Postfix Expression.Please Try Again.\n");
            return 0;
        }
        }

        return 3;// if no error return postfix
    }
    else{   // if it is not the above 3 conditions return error
        printf("This is not a legit expression.Please Try Again.\n");
        return 0;
    }
}

int converttopostfix(char string[][50],char postfix[][50],int a, int type){
    int x,y=0;
    char c;

    if(type==2){    // case infix
        for(x=0; x<a; x++){ // read from first character to the last
            if(isdigit(string[x][0]) || isdigit(string[x][1])){   // if is digit copy to postfix array
                sprintf(postfix[y++],"%s",string[x]);
            }//  end if
            else if(string[x][0]=='(')  //if a left parentheses, push to opstack
                push(string[x][0]);
            else if(string[x][0]==')'){ // if a right parentheses is detected
                while((c=pop())!='(')    // pop everything up to (
                sprintf(postfix[y++],"%c",c);
            }   // end else if
            else{
                while(precedence(topptr->op)>= precedence(string[x][0])){  // while the precedence in the opstack is higher than the current detected operator
                    if(topptr->op=='#')
                        break;
                    sprintf(postfix[y++],"%c",pop());   // pop everything that is higher or equal precedence into postfix array
                }
                    push(string[x][0]);     // only then push in the current operator
            }// end else
        }//end for
        while(topptr->op != '#')   // pop everything out
            sprintf(postfix[y++],"%c",pop());
        }// end if type 2


        if(type==1){
            int opdcounter=0,opdc=0;
            char s[50],opnd1[50],opnd2[50],opr[3],space[2],*sptr;
            space[0]=' ';space[1]='\0';
            for(x=a-1; x>=0; x--){  // read from the back
                if(isdigit(string[x][0]) || isdigit(string[x][1])){  // if a digit is detected
                    strcpy(s,string[x]);    // copy into a string
                    pushd(s);   // push into opdstack
                }
                else{   // if an operator is detected
                    strcpy(opnd1,popd());   // pop the first operand
                    strcpy(opnd2,popd());   // pop the second operand
                    strcpy(s,opnd1);    // copy the first operand into the string
                    strcat(s,space);    // add space
                    strcat(s,opnd2);    // catenate the 2nd operand
                    strcat(s,space);    // add space
                    c=string[x][0];
                    opr[0]=c;opr[1]=' ' ;opr[2]='\0';
                    strcat(s,opr);  // add operator
                    pushd(s);   // push back the whole string into the opdstack
                }
        }
         // if opdstack is not empty
        strcpy(s,popd());   // pop the while string and copy into s

        sptr=strtok(s," "); // tokenized the string and print into a 2d array
        while(sptr!=NULL){
            strcpy(postfix[y++],sptr);
            sptr=strtok(NULL," ");
        }
    }// end if type 1
    return y;   // return the length of the postfix
}

int converttoprefix(char string[][50],char prefix[][50],int a, int type){
    int x,y=0;
    char c;

    if(type==2){    // if infix
        for(x=a-1; x>=0; x--){  // read from the last character
            if(isdigit(string[x][0]) || isdigit(string[x][1])){   // if digit
                sprintf(prefix[y++],"%s",string[x]);    // copy into prefix array
            }
            else if(string[x][0]==')')  // if right parentheses detected
                push(string[x][0]); // push into the stack
            else if(string[x][0]=='('){ // if left parentheses is detected
                while((c=pop())!=')')    // pop everything up to )
                sprintf(prefix[y++],"%c",c);
            }
            else{   // else if operator
                while(precedence(topptr->op)>precedence(string[x][0])) {   // if precedence in the opstack is higher than the current operator
                    if(topptr->op=='#')
                        break;
                    sprintf(prefix[y++],"%c",pop());    //  pop everything up to the same precedence operator
                }
                    push(string[x][0]); // push the current operator into the stack
            }

        }//end for
        while(topptr->op != '#') // print everything out
        sprintf(prefix[y++],"%c",pop());
        return y;
    }// end if type 2

    if(type==3){    // if postfix

        char s[50],opnd1[50],opnd2[50],opr[3],space[2],*sptr;
        space[0]=' ';space[1]='\0';
        for(x=0; x<a; x++){ // read from the first character
            if(isdigit(string[x][0]) || isdigit(string[x][1])){  // if a digit is detected
                strcpy(s,string[x]);    // copy the number into s
                pushd(s);   // push the string into the stack
            }
            else{
                strcpy(opnd1,popd());   // pop and copy the first operand into opnd1
                strcpy(opnd2,popd());   // pop and copy the second operand inti opnd2
                strcpy(s,opnd1);    // copy the first operand into s
                strcat(s,space);    // add space
                strcat(s,opnd2);    // catenate the 2nd operand into
                strcat(s,space);    // add space
                c=string[x][0];
                opr[0]=c;opr[1]=' ' ;opr[2]='\0';
                strcat(s,opr);  // catenate the operator into the string
                pushd(s);   // push back the string into the stack
            }
    }
    // if opdstack not empty
        strcpy(s,popd());

    sptr=strtok(s," ");    // tokenized the string into a 2d array
        while(sptr!=NULL){
            strcpy(prefix[y++],sptr);
            sptr=strtok(NULL," ");
        }
        return y;   // return the length of prefix array


    }
}

char *converttoinfix(char string[][50],int a){    // for both prefix and postfix , take the postfix array, since prefix is converted to postfix array for calculation previously
int x,opdc1=0,opdc2=0;
char c,prec,s[50],opnd1[50],opnd2[50],leftparenth[3],rightparenth[3],space[2],opr[3],*sptr;
leftparenth[0]='(';leftparenth[1]=' '; leftparenth[2]='\0';
rightparenth[0]=' ';rightparenth[1]=')'; rightparenth[2]='\0';
space[0]=' ';space[1]='\0';

        for(x=0; x<a; x++){ // read from the first character
            if(isdigit(string[x][0]) || isdigit(string[x][1])){   // if a digit is detected
                strcpy(s,string[x]);    // copy the number into the string
                pushd(s);   // push the string into the stack
            }

            else {  //if an operator is read

                if(precedence(string[x][0])==4 || string[x][0]=='/' || string[x][0]=='-'){   // if ^ / or - operator
                    strcpy(opnd2,popd());   //pop and flip both operand
                    strcpy(opnd1,popd());
                    sptr=opnd1;
                    while(*sptr!='\0'){ // check first operand
                        if (isdigit(*sptr)) { // if digit operand count++
                            long val = strtol(sptr, &sptr, 10);
                            opdc1++;
                            if(*sptr=='\0') // if digit is before null then break
                                break;

                        }
                        else if(precedence(*sptr)==4){  // if ^ is in the operand then break , parentheses is not needed
                            opdc1=0;
                            break;
                        }
                        else if(*sptr=='/' && string[x][0]=='/'){// if the current operator is divide and divide is also in the operand , parentheses is not needed
                            opdc1=0;
                            break;
                        }
                        else if(precedence(*sptr)>precedence(string[x][0]) && string[x][0]=='-'){   // if precedence of an operator in the string is > than '-', parentheses is not needed
                            opdc1=0;
                            break;
                        }
                        else{   // else point to the next character
                            sptr++;
                        }
                    }
                    if(opdc1>1 ){   // if there are more than one operand is detected in opnd1, add parentheses
                        strcpy(s,leftparenth);  // add left parentheses
                        strcat(s,opnd1);    // copy operand1
                        strcat(s,rightparenth); // add right parentheses
                        strcpy(opnd1,s);    // copy back to operand1
                    }
                    opdc1=0;
                    sptr=opnd2;
                    while(*sptr!='\0'){ // check 2nd operand
                        if (isdigit(*sptr)) { // if digit operand count++
                            long val = strtol(sptr, &sptr, 10);
                            opdc2++;
                            if(*sptr=='\0')
                                break;
                        }
                        else if(precedence(*sptr)==4){  // if ^ is in the operand then break , parentheses is not needed
                            opdc2=0;
                            break;
                        }
                         else if(*sptr=='/' && string[x][0]=='/'){  // if the current operator is divide and divide is also in the operand , parentheses is not needed
                            opdc2=0;
                            break;
                        }
                        else if(precedence(*sptr)>precedence(string[x][0]) && string[x][0]=='-'){   // if precedence of an operator in the string is > than '-', parentheses is not needed
                            opdc2=0;
                            break;
                        }
                        else{   // else point to the next line
                            sptr++;
                        }
                    }
                    if(opdc2>1){    // if there are more than one operand is detected in opnd 2, add parentheses
                        strcpy(s,leftparenth);  // add left parenthses
                        strcat(s,opnd2);    // copy operand2
                        strcat(s,rightparenth); // add right parentheses
                        strcpy(opnd2,s);    // copy back to operand2
                    }
                    opdc2=0;
                    strcpy(s,opnd1);    // copy operand1 to string
                    strcat(s,space);    // add space
                    c=string[x][0];
                    opr[0]=c;opr[1]=' ' ;opr[2]='\0';
                    strcat(s,opr);  // add operator
                    strcat(s,opnd2);    // catenate operand2 to string
                    pushd(s);   // push back the string to stack
                }
                else if(string[x][0]=='*'){   // if operator is *
                    strcpy(opnd2,popd());    // pop and flip both operands
                    strcpy(opnd1,popd());
                    sptr=opnd1;
                    while(*sptr!='\0'){ // check operand 1
                        if (isdigit(*sptr)) { // if a digit is detected operand count ++
                            long val = strtol(sptr, &sptr, 10);
                            opdc1++;
                            if(*sptr=='\0') // if number is before null break
                                break;

                        }
                        else if(isparentheses(*sptr)==2){   // if a parentheses is detected opdc1, parentheses is not needed
                            opdc1=0;
                            sptr++;
                        }
                        else if(precedence(*sptr)==precedence(string[x][0])){   // if a the same precedence is detected , parentheses is not needed
                            opdc1=0;
                            sptr++;
                        }

                        else{   // else point to the next character
                            sptr++;
                        }
                    }
                    if(opdc1>1 ){   // if there are more than 1 operand in string 1, add parentheses
                        strcpy(s,leftparenth);
                        strcat(s,opnd1);
                        strcat(s,rightparenth);
                        strcpy(opnd1,s);
                    }
                    opdc1=0;
                    sptr=opnd2;
                    while(*sptr!='\0'){ // check operand2
                        if (isdigit(*sptr)) { // if digit , operand count++
                            long val = strtol(sptr, &sptr, 10);
                            opdc2++;
                            if(*sptr=='\0') // if number is before NULL then break after number
                                break;
                        }
                        else if(isparentheses(*sptr)==2){   // if a right parentheses is detected, operand count go back to zero
                                opdc2=0;
                                sptr++;

                        }
                        else if(precedence(*sptr)==precedence(string[x][0])){   // if * / is detected , operand count go back to zero
                            opdc2=0;
                            sptr++;
                        }
                        else{   // point to the next character
                            sptr++;
                        }
                    }
                    if(opdc2>1){    // if there are more than 1 operand in the string, add parentheses
                        strcpy(s,leftparenth);
                        strcat(s,opnd2);
                        strcat(s,rightparenth);
                        strcpy(opnd2,s);
                    }
                    opdc2=0;
                    strcpy(s,opnd1);    // copy first operand
                    strcat(s,space);    // add space
                    c=string[x][0];
                    opr[0]=c;opr[1]=' ' ;opr[2]='\0';
                    strcat(s,opr);  // add operator in between
                    strcat(s,opnd2);    // catenate the second operand
                    pushd(s);   // push the operand back into stack
                }


                else{   // else operator is + parentheses is not needed
                        strcpy(opnd2,popd());   // pop and flip both operand
                        strcpy(opnd1,popd());
                        strcpy(s,opnd1);    // copy first operand into the string
                        strcat(s,space);    // add space
                        c=string[x][0];
                        opr[0]=c;opr[1]=' ' ;opr[2]='\0';
                        strcat(s,opr);  // add operator in between
                        strcat(s,opnd2);    // catenate the 2nd operand to the string
                        pushd(s);   // push the string back to stack
                }

                prec=string[x][0];  // record the current operator
            }
        }

            strcpy(s,popd());


return s; // print the string

}


double evaluate(char postfix[][50],int a){
    int i;
    double value1,value2,num;
    double sum;
    char c,s[50],*sptr;
    for(i=0; i<a; i++){ // read from the first character
        if(isdigit(postfix[i][0]) || isdigit(postfix[i][1])){ //if is digit
            strcpy(s,postfix[i]);
            num=strtod(s,&sptr);
            pushn(num);//push into stack

        }
        else{   // else operator is detected
            value2=popn();   // pop 2 number and swap;
            value1=popn();
            c=postfix[i][0];
            switch(c){  // check operator type
            case '-':
                sum=(value1-value2);
                break;
            case '+':
                sum=(value1+value2);
                break;
            case '*':
                sum=(value1*value2);
                break;
            case '/':
                if(value2==0){ // divisor is 0 , return error
                    return -999;
                }
                sum=(value1/value2);
                break;
            case '^':
                sum=pow(value1,value2);
                break;
            }
            pushn(sum);  // after calculation ,push back into the stack
        }
    }
    sum=popn();  // pop the final result
    return sum;
}
