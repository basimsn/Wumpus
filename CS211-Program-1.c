/*
  Program 1: Wumpus, version 1 (fixed size array)
  Course: CS 251, Fall 2022. Tuesday 2pm Lab
  System: Windows using Visual Studio Code
  Author: Basim Nabulsi
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
//--------------------------------------------------------------------------------
int taskDone =1;
bool endgame = false;
int moveCounter = 1;

void displayCave(){
	//Gameboard
    printf( "\n       ______18______             \n"
    		"      /      |       \\           \n"
    		"     /      _9__      \\          \n"
    		"    /      /    \\      \\        \n"
    		"   /      /      \\      \\       \n"
    		"  17     8        10     19       \n"
    		"  | \\   / \\      /  \\   / |    \n"
    		"  |  \\ /   \\    /    \\ /  |    \n"
    		"  |   7     1---2     11  |       \n"
    		"  |   |    /     \\    |   |      \n"
    		"  |   6----5     3---12   |       \n"
    		"  |   |     \\   /     |   |      \n"
    		"  |   \\       4      /    |      \n"
    		"  |    \\      |     /     |      \n"
    		"  \\     15---14---13     /       \n"
    		"   \\   /            \\   /       \n"
    		"    \\ /              \\ /        \n"
    		"    16---------------20           \n"
    		"\n");
}

void displayInstructions(){
	//Game instructions
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
    		"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
    		"2. The Wumpus is not bothered by the pits, as he has sucker feet. Usually he is \n"
			"   asleep. He will wake up if you enter his room. When you move into the Wumpus'\n"
			"   room, then he wakes and moves if he is in an odd-numbered room, but stays    \n"
			"   still otherwise.  After that, if he is in your room, he snaps your neck and  \n"
			"   you die!                                                                     \n"
    		"                                                                                \n"
    		"Moves:                                                                          \n"
    		"On each move you can do the following, where input can be upper or lower-case:  \n"
    		"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
    		"   then a room number.                                                          \n"
    		"2. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
    		"3. Enter 'C' to cheat and display current board positions.                      \n"
    		"4. Enter 'D' to display this set of instructions.                               \n"
    		"5. Enter 'P' to print the maze room layout.                                     \n"
			"6. Enter 'G' to guess which room Wumpus is in, to win or lose the game!         \n"
    		"7. Enter 'X' to exit the game.                                                  \n"
    		"                                                                                \n"
    		"Good luck!                                                                      \n"
    		" \n\n");
}
//--------------------------------------------------------------------------------
void cheats(int room, int wumpus, int pit1, int pit2){
	//Takes in the room, wumpus, and both the pits and prints where it is
	printf("Cheating! Game elements are in the following rooms: \nPlayer Wumpus Pit1 Pit2  \n");
	printf("  %d      %d     %d     %d \n\n", room, wumpus, pit1, pit2);
}

void posNear(int *pPlayer, int wumpus, int pit1, int pit2, int room, int* roomRepeat){
  //Takes the pointer of player, wumpus room, both pits, the new room, and the pointer of room repeat in return it prints out messages if the new room is near a target room or inside a target room
  //posNear is the function that detects if the player is near the pits or wumpus and if a player moves to one of the pits or wumpus
	int nearWumpus, nearPit1, nearPit2, nearDeath=0;
	int displayBoard[][4]= {{1,2,5,8}, {2,1,3,10}, {3,2,4,12}, {4,3,5,14}, {5,1,4,6}, {6,5,7,15},{7,6,8,17},{8,1,7,9},{9,8,10,18},{10,2,9,11}, {11, 10,12,19}, {12, 3, 11, 13}, {13, 12, 14, 20}, {14, 4, 13, 15}, {15, 6, 14, 16}, {16, 15, 17, 20}, {17, 7, 16, 18},{18, 9, 17, 19},{19,11,18,20}, {20, 13, 16, 19}};
	int smallest = displayBoard[wumpus-1][0];
	//Direct Impact and the program messages
	if(*pPlayer == pit1 || *pPlayer == pit2){
		printf("Aaaaaaaaahhhhhh....   \n    You fall into a pit and die. \n");
		endgame = true;
	}
	
	else if(*pPlayer == wumpus){	//Entering the wumpus room
		if(wumpus %2 ==0){
			printf("You briefly feel a slimy tentacled arm as your neck is snapped.\nIt is over.\n");
			endgame = true;
		}
	
		else{	//If the room is odd wuumpus moves to smallest adjacent loctaion
			printf("You hear a slithering sound, as the Wumpus slips away. \nWhew, that was close! \n");
			for(int i=0; i<4; i++){
				nearWumpus = displayBoard[wumpus-1][i];
				if(smallest>nearWumpus){
					smallest =nearWumpus;
				}
			}
			wumpus = smallest;
		}
	}
	//Near Impact
		//Each of the for loops loops through the adajacent rooms to determine which print statement it should print out
	if(endgame == false){
		for(int i=1; i<4; i++){
			nearWumpus = displayBoard[wumpus-1][i];
			if(nearWumpus == *pPlayer){
				nearDeath=2;}
		}
		for(int i=1; i<4; i++){
			nearPit1 = displayBoard[pit1-1][i];
			nearPit2 = displayBoard[pit2-1][i];
			if(nearPit1 == *pPlayer || nearPit2 == *pPlayer){
				nearDeath=1;
				for(int i=1; i<4; i++){
					nearWumpus = displayBoard[wumpus-1][i];
					if(nearWumpus == *pPlayer){
					nearDeath=3;}
				}
			}
		}
		//Program Messages when near impact
		if(nearDeath>0){
			printf("You are in room %d. ", room);
			if(nearDeath==2){
				printf("You smell a stench. \n\n");
			*roomRepeat=5;}
			else if(nearDeath==1){
				printf("You feel a draft. \n\n");
			*roomRepeat=5;}
			else if(nearDeath==3){
				printf("You smell a stench. You feel a draft. \n\n");
			*roomRepeat=5;}
		}
	}
}

bool changePos(int* pPlayer, int player, int room, int wumpus, int pit1, int pit2, int* roomRepeat){
	//Function takes in the pointer of player, wumpus room, both pits, the new room, and the pointer of room repeat in return it checks if the move is a valid move
	int displayBoard[][4]= {{1,2,5,8}, {2,1,3,10}, {3,2,4,12}, {4,3,5,14}, {5,1,4,6}, {6,5,7,15},{7,6,8,17},{8,1,7,9},{9,8,10,18},{10,2,9,11}, {11, 10,12,19}, {12, 3, 11, 13}, {13, 12, 14, 20}, {14, 4, 13, 15}, {15, 6, 14, 16}, {16, 15, 17, 20}, {17, 7, 16, 18},{18, 9, 17, 19},{19,11,18,20}, {20, 13, 16, 19}};
	int a1;
	//Valid Move checker
	for(int i=1; i<4; i++){
		a1 = displayBoard[*pPlayer-1][i];
		if(a1 == room){
			*pPlayer = room;
			posNear(pPlayer, wumpus, pit1, pit2, room, roomRepeat);
			++moveCounter;
			return true;
		}
	}
	return false;
}

bool guesses(int wumpus){
	//Checks if guess is correct
	int guess;
	scanf("%d", &guess);
	printf("Enter room (1..20) you think Wumpus is in: ");
	if(guess == wumpus){
		printf("You won!\n");
		return true;
	}
	else{
		return false;
	}
}
//--------------------------------------------------------------------------------
int main(void) {
	srand(1);
	//Most variables are being declared below
	int pit1 = rand() % 20 + 1; int pit2 =  rand() % 20 + 1; int wumpus =  rand() % 20 + 1; int player =  rand() % 20 + 1;
	int number= 0; int room; char move; 
	int* roomRepeat =&number; int *pPlayer = &player;
	while(endgame == false){ 	//While loop the game runs on
		if(taskDone == 1){ //Prevents repetition
			if(number!=5){
				printf("You are in room %d. \n\n", player);
				number =1;
			}
			printf("%d. Enter your move (or 'D' for directions): ", moveCounter);
			scanf(" %c", &move);
			++taskDone;
			move = tolower(move);}
		switch(move){	//Switch case that directs all inputs to the correct functions
			case 'm':
				scanf(" %d", &room);
				number =1;
				if (changePos(pPlayer, player, room, wumpus, pit1, pit2, roomRepeat)){	//Checks if the move is valid
					taskDone =1;
				}
				else{
					printf("Invalid move.  Please retry. \n");
					room =player;
					posNear(pPlayer, wumpus, pit1, pit2, room, roomRepeat);
          			taskDone =1;
          		}
				break;
			case 'r':
       			printf("Enter the room locations (1..20) for player, wumpus, pit1, and pit2: \n\n");
        		scanf(" %d %d %d %d", &player, &wumpus, &pit1, &pit2);	//takes in the new room locations
				room = player;
				posNear(pPlayer, wumpus, pit1, pit2, room, roomRepeat);
				taskDone =1;
				break;
			case 'c':
				cheats(player, wumpus, pit1, pit2);
				room = player;
				posNear(pPlayer, wumpus, pit1, pit2, room, roomRepeat);	//used to print program messages 
				taskDone =1;
				break;
			case 'd':	//displays gameboard and instructions
        		displayCave(); 
        		displayInstructions();
				taskDone = 1;
				break;
			case 'p':	//displays gameboard
				displayCave();
				taskDone =1;
				break;
			case 'g':
				if(guesses(wumpus)){	//checks if guess is correct
					endgame = true;}
				break;
			case 'x':
				endgame = true;
				break;
		}
	}
	printf("\nExiting Program ...\n");
	return 0;
}