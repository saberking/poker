#include <stdlib.h>
#include <string>
#include<string.h>
#include <vector>
#include<list>
#include<iostream>
#include <algorithm>
#include <time.h>

using namespace std;

int hand[2][2];

int board[5];
 
int phase = 0;

int pot=0;

int bet[2];

bool hasActed[2];

int action;

int stack[2];

string deck[52];

int usedCards[9];

void addSuit(string suit, int start){
    for(int i=2;i<11;i++){
        deck[start+i-2]=to_string(i)+suit;
    }
    deck[start+9]="J"+suit;
    deck[start+10]="Q"+suit;
    deck[start+11]="K"+suit;
    deck[start+12]="A"+suit;
}
void createDeck(){
    addSuit("s", 0);
    addSuit("h",13);
    addSuit("d",26);
    addSuit("c",39);
}
int draw(){

    int i;
    bool done=false;
    do{
        i=rand()%52;
        done=true;
        for(int j=0;j<9;j++){
            if(i==usedCards[j]){
                done=false;
            }
        }
    }while(!done);
    done=false;
    for(int j=0;!done;j++){
        if(usedCards[j]==-1){
            usedCards[j]=i;
            done=true;
        }
    }
    return i;
}
void displayCards(){
    for(int i=0;i<2;i++){
        string hole=deck[hand[i][0]]+" "+deck[hand[i][1]];
        printf("Player %d has %s\n", i, hole.c_str());

        printf("He has %d in the pot",  bet[i]);

        printf(" and %d in his stack\n",stack[i]);
        system("PAUSE");
    }
    if(phase){
        string boardCards="";
        for(int i=0;i<phase+2;i++){
            boardCards+=deck[board[i]]+" ";
        }
        printf("The board is %s\n", boardCards.c_str());
    }
}
void takeTurn();
void preflop(){
    displayCards();
    takeTurn();
}
void deal(){
    for(int i=0;i<9;i++){
        usedCards[i]=-1;
    }
    phase = 0;
    pot=0;
    bet[0]=1;
    stack[0]-=1;
    bet[1]=2;
    stack[1]-=2;
    printf("stacks: %d %d", stack[0],stack[1]);
    for(int i=0;i<2;i++){
        hasActed[i]=false;
        for(int j=0;j<2;j++){
            hand[i][j]=draw();
        }
    }
    action=0;
    for(int i = 0;i<5;i++){
        board[i]=-1;
    }

    preflop();
}
int getVillain(){
    if(action){
        return 0;
    }else{
        return 1;
    }
}
void flop(){
    for(int i=0;i<3;i++){
        board[i]=draw();
    }
    displayCards();
    takeTurn();
}
void turn(){
    board[3]=draw();
    displayCards();
    takeTurn();
}
void river(){
    board[4]=draw();
    displayCards();
    takeTurn();
}
void showdown(){
    printf("Player 0 wins the pot!\n");
    system("PAUSE");
    deal();
}
void nextRound(){
    system("PAUSE");
    action=1;
    hasActed[0]=hasActed[1]=false;
    phase++;
    if(phase==1){
        flop();
    }else if (phase==2){
        turn();
    }else if(phase==3){
        river();
    }else{
        showdown();
    }
}
void pass(){
    hasActed[action]=true;
    bool consensus=(bet[getVillain()]>=bet[action]&&hasActed[getVillain()]);
    if(consensus||(stack[getVillain()]==0)){
        nextRound();
    }else{
        action=getVillain();
        takeTurn();
    }
}
void match(){
    int amount = max(0, min(bet[getVillain()]-bet[action],stack[action]));
    bet[action]+=amount;
    stack[action]-=amount;
}
void call(){
    printf("Player %d calls\n",action);
    match();
    pass();
}
void makeBet(){
    match();
    int amount =bet[0];
    amount=min(amount, min(stack[action],stack[getVillain()]));
    bet[action]+=amount;
    printf("Player %d raises to %d\n",action, bet[action]);
    pass();
}
void takeTurn(){
    bool shouldBet=false;
    for(int j=0;j<2;j++){
        if(deck[hand[action][j]].find("A")!=-1){
            shouldBet=true;
        }   
    }
    if(shouldBet){
        printf("Player %d likes his hand\n", action);
        makeBet();
    }else{
                printf("Player %d doesn't like his hand\n", action);
        call();
    }
}

int main (){
    srand(time(NULL));
    createDeck();
    stack[0]=100;
    stack[1]=100;
    deal();
}