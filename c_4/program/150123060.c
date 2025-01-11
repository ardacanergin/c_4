
/*
Arda Can Ergin 150123060
----------------------------------- INPUT RULES -----------------------------
format should be as following

multiplicand: [number_1]
multiplier: [number_2]

new lines and white spaces does not effect input. However the places of 
multiplicand and multiplier strings shouldn't be interchanged and also
there had to be ':' character after these strings.

    Example input is as following

multiplicand: 12456.789
multiplier: 10987.65432s

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

/*structure for floating point number */
struct fpNum {
    int num; /*for number (if num is -1 it represents the point)*/
    struct fpNum *next;
    struct fpNum *prev;
};

typedef struct fpNum FpNum;
typedef FpNum *FpNumptr;

void outputF(const char *fileName, FpNumptr *sResult,FpNumptr *dCounter, FpNumptr *startD, FpNumptr *startR, double clock); /*print content of the result to the output file*/
void insert(FpNumptr *sPtr, int n); /*insert node*/
void insertR(FpNumptr *sPtr, FpNumptr *ePtr, int n);
void multi(FpNumptr *startD, FpNumptr *startR, FpNumptr *sResult, FpNumptr *eResult); /*multiply the numbers*/
void placePP(FpNumptr *sResult, FpNumptr *dCounter); /*place decimal point to the correct position in the result*/
void printList(FpNumptr *sPtr); /*printlist for error check*/
void orderPtr(FpNumptr *sResult, FpNumptr *ordRes);/*extra computational method*/

int main(void){

    FpNumptr startD = NULL;     /*start of multiplacanD */
    FpNumptr startR = NULL;     /*start of multiplieR*/
    FpNumptr sResult = NULL;    /*start of the result*/

    FpNumptr endD = NULL;
    FpNumptr endR = NULL;
    FpNumptr eResult = NULL;

    FILE *input; /*create file pointer*/

    char boolean = 'd'; /*if d linked list of multiplacand will be written if r link list of multiplier will be written*/
    char in;
    double n=1;
    int gate =0;
    clock_t start,end;
    double exTime;

    /*counter as a linked list*/
    FpNumptr dCounter = NULL;
    insert(&dCounter,0);

    /*open file*/
    puts("file open attempt...");
    if((input = fopen("input.txt","r"))==NULL){
        puts("file open error");
        exit(0);
    }
    puts("file opened successfully");

    /*get the ipnut from file*/
    while((in= fgetc(input)) !=EOF){
        if(in==':' || (in<='9' && in>='0') || in =='.' || in=='d' || in=='r' || in=='\n'){

            printf("%c",in);

            if(in==':' || in=='\n'){
                gate=0;
            }
            if(gate){
                if(dCounter->num==INT_MAX){
                    insert(&dCounter,0);
                }
                dCounter->num++;
            }
            if(in=='.'){
                gate=1;
            }

            if(in=='d' || in=='r'){
                boolean=in; 
            }
            /*write the multiplicanD or multiplieR based on the boolean value*/
            if((in=='.' || (in<='9' && in>='0')) && boolean=='d'){
                if(in=='.'){
                    insert(&startD,-1);
                } else
                insert(&startD,(in-'0'));
            }
            if((in=='.' || (in<='9' && in>='0')) && boolean=='r'){
                if(in=='.'){
                    insert(&startR,-1);
                } else
                insert(&startR,(in-'0'));
            }
        } 
        /*simple error check*/
        else if(in!=':' && (in>='9' ||in<='0') && in !='.' && in!='m' && in!='u' && in!='l' && in!='t'
        && in!='i' && in!='p' && in!='c' && in!='a' && in!='n' && in!='d' && in!='e' && in!='r' && in !='\n' && in!= ' '){
            puts("error : file input format missmatch");
            printf("in = %c",in);
            
            return 0;
        }
    }

    puts("input has been reed");
    printf("startD: ");
    printList(&startD);
    printf("\n");
    printf("startR: ");
    printList(&startR);
    printf("\n");
    printf("dCounter:");
    printList(&dCounter);
    printf("\n");


    /*start capturing the time at the start of the computation*/
    start = clock();

    /*computing the result*/
    multi(&startD,&startR,&sResult,&eResult);
    puts("multi is done!");
    printList(&sResult);
    
    /*end of computation*/
    end = clock();
    exTime= ((double)(end-start)) * 1000 / CLOCKS_PER_SEC; /*execution time in miliseconds*/

    /*print the result to the output file*/
    outputF("output.txt",&eResult,&dCounter,&startD,&startR, exTime);


    return 0;

}

void insert(FpNumptr *sPtr, int n){

    FpNumptr newPtr =  malloc(sizeof(FpNum)); /*create node*/
    FpNumptr previous = NULL;

    if(newPtr == NULL){
        puts("memory allocation failed : error");
        return;
    }

    /*update the fields of the node*/
    newPtr->num=n;
    newPtr->next=*sPtr;
    newPtr->prev=NULL;
    /*if the list is not empty append the previous pointer of the node*/
    if(*sPtr!=NULL){
        (*sPtr)->prev=newPtr;
    }

    *sPtr = newPtr;

    /*printList(sPtr);*/
}

void insertR(FpNumptr *sPtr, FpNumptr *ePtr, int n){
    FpNumptr newPtr =  malloc(sizeof(FpNum)); /*create node*/
    FpNumptr previous = NULL;

    if(newPtr == NULL){
        puts("memory allocation failed : error");
        return;
    }

    /*update the fields of the node*/
    newPtr->num=n;
    newPtr->next=*sPtr;
    newPtr->prev=NULL;

    /*if the list is not empty append the previous pointer of the node*/
    if(*sPtr!=NULL){
        (*sPtr)->prev=newPtr;
    }
    else{
        *ePtr=newPtr;
    }

    *sPtr = newPtr;
}

void multi(FpNumptr *startD, FpNumptr *startR, FpNumptr *sResult, FpNumptr *endR){
    int remainder=0;
    int sumRem =0;
    int resNum =0;
    int num=0;
    int stepCounter=0; /*in the first iteration of inner while it sets the result linked list*/
    int actNum=0; /*stores the value of 1 digit times 1 digits multiply result needed for an if condition*/

    FpNumptr firstNum = *startD;
    FpNumptr secondNum = *startR;
    FpNumptr result = *sResult;
    /*FpNumptr result = *sResult;*/

    puts("multi function starts...");

    while(secondNum!=NULL){
        remainder=0;
        sumRem=0;
        resNum=0;
        num=0;
        firstNum=*startD;

        if(secondNum->num==-1){
            secondNum=secondNum->next;
            continue; /*pass the . node when computing*/
        }
       
        if((*sResult)==NULL){
            puts("sresult first insert...");
            insertR(sResult,endR,0);
            puts("first assaign of sResult node succ");
        }
        while(firstNum!=NULL){
            if(firstNum->num==-1){
                firstNum = firstNum->next;
                continue;
            }
            if(stepCounter==0){

                num = firstNum->num * secondNum->num + remainder;
                remainder = num/10;
                num= num%10;

                (*sResult)->num=num;
            }
            /*after the first iteration addition operation starts*/
            else{

                num = firstNum->num * secondNum->num;

                resNum = (*sResult)->num + (num%10) + remainder + sumRem;

                sumRem = resNum/10;
                remainder = num/10;

                (*sResult)->num = resNum%10;
                
            }

            /* add a new node for every single multip*/
            if(firstNum->next!=NULL && (*sResult)->prev==NULL){ /* first node added at the start */
                insert(sResult,0);
            } else if((*sResult)->prev!=NULL){
                (*sResult)=(*sResult)->prev;
            }

            firstNum = firstNum->next; /*continue with the next digit*/
        }

        if(firstNum==NULL && (remainder!=0 || sumRem !=0)){
             if(sumRem + remainder <10){
                    insert(sResult,(sumRem+remainder)%10);
                    /*insert(sResult, (sumRem+remainder)/10);*/
                } else {
                    insert(sResult,sumRem+remainder);
                }

            }

        
        secondNum=secondNum->next;

        if(stepCounter == INT_MAX){ /*ensure that theorically we can make infinite computation*/
            stepCounter=1;
        }

        stepCounter++;

        orderPtr(sResult,&result);
        result = (*sResult);
        printList(sResult);
        
    }
}

void orderPtr(FpNumptr *sResult, FpNumptr *ordRes){

    FpNumptr current = *sResult;

/* in the first iteration of multi function*/
    if((*ordRes) == NULL){
    /*if there is only one element in the list*/
    if(current->next==NULL){
        return;
    }
    /*required step for digit shitfing*/
    while(current->next->next!=NULL){
        current = current->next;
    }

    } else{
        while(current->next!=(*ordRes)){
            current = current->next;
        }
    }
    
    *sResult=current;

    return;
    
}

void outputF(const char *fileName, FpNumptr *sResult, FpNumptr *dCounter, FpNumptr *startD, FpNumptr *startR, double clock){

    FILE *out = fopen(fileName,"w");

    if(out==NULL){
        puts("file cannot opened : error outputF");
        return;
    }

    /*traverse to the end of each linked list does not effect the execution time (out of computation)*/
    do{
        /*walk to*/
        if((*startD)->next!=NULL)
        *startD = (*startD)->next;
    }while((*startD)->next!=NULL);

    do{
        if((*startR)->next!=NULL)
        *startR = (*startR)->next; 
    } while((*startR)->next!=NULL);
    
    /*print multiplicand*/
    fprintf(out,"multiplicand: ");
    while((*startD)!=NULL){
        if((*startD)->num==-1){
            fputc('.',out);
        } else
        fputc((*startD)->num+'0',out);

        *startD=(*startD)->prev;
    }
    fprintf(out,"\n");

    /*print multiplier*/
    fprintf(out,"multiplier: ");
    while((*startR)!=NULL){
        if((*startR)->num==-1){
            fputc('.',out);
        } else
        fputc((*startR)->num+'0',out);

        *startR=(*startR)->prev;
    }
    fprintf(out,"\n");

    /*to print the decimal point correctly add it to the node*/
    placePP(sResult,dCounter);

    /*print result*/
    fprintf(out,"result: ");
    while((*sResult)!=NULL){
        if((*sResult)->num==-1){
            fputc('.',out);
        } else
        fputc((*sResult)->num+'0',out);

        *sResult=(*sResult)->next;
    }
    fprintf(out,"\n");
    fprintf(out,"\n");

    /*print clock*/
    fprintf(out,"execution time: %f milliseconds\n",clock);

    fclose(out);

    return;
}

    void placePP(FpNumptr *sResult, FpNumptr *dCounter){

        FpNumptr current = (*sResult); /*it is actually end of the result but since we are going to change it to start 
        I've left it like that*/

        FpNumptr previous = NULL;
        
    if((*dCounter)->num!=0){

        (*dCounter)->num--;/*neccessarry operation for solving the problem : skips one extra node because of the 
        operation current->prev that is instead of counting 3 digits it makes 3 skip operations which means counting
        4 digits that's way incremented by 1 */

        do{
            while((*dCounter)!=NULL && (*dCounter)->num!=0){
                current=current->prev; /*walk to next until all the digits counted*/
                (*dCounter)->num--;
            }
            /*walk to second node of the dCounter this is necessary for theoricly infinite result*/
            if((*dCounter)->next!=NULL)
            (*dCounter)=(*dCounter)->next; else
            break;

        } while((*dCounter)->next!=NULL);

        previous = current; /*store the node that will come before the decimal point (.)*/
        current = current ->prev; /*walk one more*/
        insert(&previous,-1); /*insert . node ---- new current will be showing the . node*/

        previous->prev=current;
        previous->next=current->next;

        current->next->prev=previous; 

        current->next=previous;

    }
    /*make sure sResult points to the start of the result linked list*/
    do{
        (*sResult)= (*sResult)->prev;

    }while((*sResult)->prev!=NULL);

        return;

    }

    void printList(FpNumptr *sPtr){
        FpNumptr current = *sPtr;

        while(current!=NULL){
            printf("%d ",current->num);
            current=current->next;
        }
        printf("\n");

        return;
    }