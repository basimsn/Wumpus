/*
	Program 1: Wumpus, version 1 (fixed size array)
	CS 211, UIC, Fall 2022
	System: Replit
	Author: Dale Reed and Ellen Kidane

	Hunt the Wumpus is a classic text-based adventure game by Gregory Yob from 1972.
	The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms.
	This program implements this game, without superbats or arrows, using a fixed size array.
*/

#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()
// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Room connection values are set as global constants, and don't need to be passed.
// The rooms array is indexed from 0..22, but we don't use the 0th entry and 
// only use 1..20.  This approach helps prevent off-by-one indexing errors.

// Used to more conveniently pass all game information between functions.
struct GameInfo {
	int moveNumber;  // Counts up from 1, incrementing for each move
	int personRoom;  // Room 1..20 the person currently is in
	int wumpusRoom;  // Room 1..20 the Wumpus is in
	int pitRoom1;    // Room 1..20 the first pit is in
	int pitRoom2;    // Room 1..20 the second pit is in
    int bats1;
    int bats2;
    int arrows;
};

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *personIsAlive, int **Rooms);

//--------------------------------------------------------------------------------
void displayCave()
{
    printf( "\n"
			"       ______18______             \n"
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

//--------------------------------------------------------------------------------
void displayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
    		"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
    		"2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
            "   fly you to some random room, which could be troublesome.  Then those bats go \n"
            "   to a new room different from where they came from and from the other bats.   \n"
            "3. The Wumpus is not bothered by the pits, as he has sucker feet. Usually he is \n"
			"   asleep. He will wake up if you enter his room. When you move into the Wumpus'\n"
			"   room, then he wakes and moves if he is in an odd-numbered room, but stays    \n"
			"   still otherwise.  After that, if he is in your room, he snaps your neck and  \n"
			"   you die!                                                                     \n"
    		"                                                                                \n"
    		"Moves:                                                                          \n"
    		"On each move you can do the following, where input can be upper or lower-case:  \n"
    		"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
    		"   then a room number.                                                          \n"
    		"2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"  
            "   you specify.  Your arrow ends up in the final room.                          \n"
            "   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
            "   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
            "   by spaces. If an arrow can't go a direction because there is no connecting   \n"
            "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
            "   continues doing this until it has traveled the designated number of rooms.   \n"
            "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
            "   automatically pick up the arrow if you go through a room with the arrow in   \n"
            "   it.                                                                          \n"
            "3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
    		"4. Enter 'C' to cheat and display current board positions.                      \n"
    		"5. Enter 'D' to display this set of instructions.                               \n"
    		"6. Enter 'P' to print the maze room layout.                                     \n"
    		"7. Enter 'X' to exit the game.                                                  \n"
    		"                                                                                \n"
    		"Good luck!                                                                      \n"
    		" \n\n");
}//end displayInstructions()

//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue, int randomNumbers[], int limit)            // How many numbers previously found
{
    int returnValue = false;
    
    // compare random value against all previously found values
    for( int i = 0; i<limit; i++) {
        if( randomValue == randomNumbers[i]) {
            returnValue = true;   // It is already there
            break;
        }
    }
    
    return returnValue;
} 

//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int randomNumbers[], int size)
{
    int randomValue = -1;
    
    for(int i = 0; i<size; i++) {
        do {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
} 


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)   // All game settings variables
{
    // Array of 6 unique values 1..20, to be used in initializing cave hazards locations
    int randomNumbers[7];

    setUniqueValues(randomNumbers, 7);
    // Use the unique random numbers to set initial locations of hazards, which
    //    should be non-overlapping.
    gameInfo->pitRoom1 = 18; //randomNumbers[0];   
    gameInfo->pitRoom2 = 16; //randomNumbers[1];
    gameInfo->wumpusRoom = 7; //randomNumbers[2];
    gameInfo->personRoom = 4; //randomNumbers[3];
    gameInfo->bats1 =14; //randomNumbers[4];
    gameInfo->bats2 = 13; //randomNumbers[5];
    gameInfo->arrows = 10; //randomNumbers[6];

    gameInfo->moveNumber = 1;
}// end initializeBoard(...)

//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[3], int nextRoom)        // Desired room to move to
{
    return(tunnels[0] == nextRoom ||
           tunnels[1] == nextRoom ||
           tunnels[2] == nextRoom
          );
}

//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo)
{
    if(gameInfo.personRoom == gameInfo.arrows){
        gameInfo.arrows=-1;
    }
    printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus pit1 pit2 bats1 bats2 arrow \n"
    		"%4d %7d %5d %3d %4d %4d %5d\n\n",
			gameInfo.personRoom,
			gameInfo.wumpusRoom,
			gameInfo.pitRoom1,
			gameInfo.pitRoom2,
            gameInfo.bats1,
            gameInfo.bats2,
            gameInfo.arrows
		);
    if(gameInfo.arrows == -1){
        gameInfo.arrows == gameInfo.personRoom;
    }
}

//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo(struct GameInfo gameInfo, int** Rooms)
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.personRoom;
    printf("You are in room %d. ", currentRoom);
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = Rooms[currentRoom][0];
    int room2 = Rooms[currentRoom][1];
    int room3 = Rooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
        printf("You smell a stench. ");
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room ||
        room2 == pit1Room || room2 == pit2Room ||
        room3 == pit1Room || room3 == pit2Room
      ) {
        printf("You feel a draft. ");
    }
    int bat1Room = gameInfo.bats1;
    int bat2Room = gameInfo.bats2;
    if( room1 == bat1Room || room1 == bat2Room ||
        room2 == bat1Room || room2 == bat2Room ||
        room3 == bat1Room || room3 == bat2Room
      ) {
        printf("You hear rustling of bat wings.");
      }
    
    
    printf("\n\n");
}//end displayRoomInfo(...)

void arrowCheck(struct GameInfo *gameInfo, bool *wumpusIsAlive, bool *personIsAlive, bool ricochet){
    int arrow = gameInfo->arrows;
    int wumpus = gameInfo->wumpusRoom;
    int person = gameInfo->personRoom;
    if(arrow == person){
        if(ricochet == false){
            printf("You just shot yourself.\nMaybe Darwin was right. You're dead.\n");
        }
        else{
            printf("You just shot yourself, and are dying.\nIt didn't take long, you're dead.\n");
        }
        *personIsAlive = false;
    }
    if(arrow == wumpus){
        if(ricochet == false){
            printf("Wumpus has just been pierced by your deadly arrow!\nCongratulations on your victory, you awesome hunter you.\n");
        }
        else{
            printf("Your arrow ricochet killed the Wumpus, you lucky dog!\nAccidental victory, but still you win!\n");
        }
        *wumpusIsAlive = false;
    }
    
}
//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
void checkForHazards(struct GameInfo *gameInfo, bool *personIsAlive, int **Rooms)       // Person is alive, but could die depending on the hazards
{
    // retrieve the room the person is in
    int personRoom = gameInfo->personRoom;

    if( personRoom == gameInfo->wumpusRoom) {
        // if(  (rand() % 100) < 75) {
        if( gameInfo->wumpusRoom %2 == 1) {
            // You got lucky and the Wumpus moves away
            printf( "You hear a slithering sound, as the Wumpus slips away. \n"
					"Whew, that was close! \n");
            // Choose a random adjacent room for the Wumpus to go into
            // gameInfo.wumpusRoom = rooms[ personRoom][ rand() % 3];
            gameInfo->wumpusRoom = Rooms[ personRoom][ 0];  // Choose the lowest-numbered adjacent room
        }
        else {
            // Wumpus kills you
            printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
					"It is over.\n");
            *personIsAlive = false;
            return;
        }
    }
    // Check whether there is a pit
    if( personRoom == gameInfo->pitRoom1 || personRoom == gameInfo->pitRoom2) {
        // Person falls into pit
        printf("Aaaaaaaaahhhhhh....   \n");
        printf("    You fall into a pit and die. \n");
        *personIsAlive = false;
        return;
    }

}//end checkForHazards(...)

void batMove(struct GameInfo *gameInfo, bool *personIsAlive, int nextRoom){
    int* random=(int*)malloc(2*sizeof(int));
    int person = gameInfo->personRoom;
    int rand1= rand()%20+1;
    if(rand1 == nextRoom){
        gameInfo->personRoom = rand()%20 +1;
    }
    else{
        gameInfo->personRoom = rand1;
    }
    if(gameInfo->bats1== person){
        gameInfo->bats1 = rand()%20 +1;
    }
    else{
        gameInfo->bats2 = rand()% 20 +1;
    }
    

    printf("Woah... you're flying!\nYou've just been transported by bats to room %d.\n", gameInfo->personRoom);
}

void shooting(struct GameInfo *gameInfo, bool *arrows1, bool *wumpusIsAlive, bool *personIsAlive, int **Rooms){
    int n;
    int wumpusRoomGuess = 0;
    bool ricochet = false;
    printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
    scanf("%d", &n);
    int *a1 = (int*)malloc(n* sizeof(int));
    for(int i=0; i<n; i++){
        scanf(" %d", &a1[i]);
    }
    for(int i=0; i<n; i++){
        if(*wumpusIsAlive == false || *personIsAlive == false){
            break;
        }
        else{
            if(roomIsAdjacent(Rooms[gameInfo->arrows], a1[i])){
                gameInfo->arrows = a1[i];
                arrowCheck(gameInfo, wumpusIsAlive, personIsAlive, ricochet);
            }
            else{
                ricochet = true;
                printf("Room %d is not adjacent. Arrow ricochets...\n", a1[i]);
                gameInfo->arrows= Rooms[gameInfo->arrows][0];
                arrowCheck(gameInfo, wumpusIsAlive, personIsAlive, ricochet);
            }
        }
    }
    gameInfo->wumpusRoom = Rooms[gameInfo->wumpusRoom][0];


    *arrows1 = false;
}
//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo, bool* arrows1)
{
    printf("Enter the room locations (1..20) for player, Wumpus, pit1, pit2, bats1, bats2, and arrow:\n");
	// In the scanf below note that we put the space at the beginning of the scanf so  
    // that any newline left over from a previous input is not read in and used as 
	// the next move. Another option is having getchar() after the scanf() statement.
	scanf(" %d %d %d %d %d %d %d", &theGameInfo->personRoom, &theGameInfo->wumpusRoom, 
                          &theGameInfo->pitRoom1, &theGameInfo->pitRoom2, &theGameInfo->bats1,
                          &theGameInfo->bats2, &theGameInfo->arrows);
    if(theGameInfo->arrows == -1){
        *arrows1 = true;
        theGameInfo->arrows = theGameInfo->personRoom;
    }
    printf("\n");
}


//--------------------------------------------------------------------------------
int main(void) {
    struct GameInfo gameInfo;   // Used to more easily pass game info variables around
    bool personIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    bool arrows1 = false;
    bool bats = false;
    char typeOfMove;            // Used to handle user input letter
    int nextRoom;               // User input of destination room number, used on a 'M' type move
    int **Rooms;
    srand(1);
    int board[21][3] = {
      // Room 0 is unused, to help avoid off-by-one indexing errors.
                            {0,0,0},
      //                    Room 1     Room 2      Room 3      Room 4      Room 5
      /* Rooms 1-5   */     {2,5,8},  {1,3,10},   {2,4,12},   {3,5,14},    {1,4,6},
      //                    Room 6     Room 7      Room 8      Room 9     Room 10
      /* Rooms 6-10  */    {5,7,15},  {6,8,17},    {1,7,9},  {8,10,18},   {2,9,11},
      //                   Room 11    Room 12     Room 13     Room 14     Room 15
      /* Rooms 11-15 */  {10,12,19}, {3,11,13}, {12,14,20},  {4,13,15},  {6,14,16},
      //                   Room 16    Room 17     Room 18     Room 19     Room 20
      /* Rooms 16-20 */  {15,17,20}, {7,16,18},  {9,17,19}, {11,18,20}, {13,16,19},
    };
    int row = 21;
    int col =3;
    Rooms= (int**) malloc(row*sizeof(int*));
    for(int i=0; i<row; i++){
        Rooms[i]= (int*) malloc(row*sizeof(int));
    }
    
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            Rooms[i][j] = board[i][j];
        }
    }
    // Set random initial values for person, Wumpus, bats and pits
    initializeGame( &gameInfo);
	// Main playing loop.  Break when player dies, or player kills Wumpus
    while (personIsAlive && wumpusIsAlive) {
		// Display current room information: Room number, hazards detected
        if(typeOfMove == 'S' || typeOfMove =='C' || bats == true){
            printf("You are in room %d.\n\n", gameInfo.personRoom);
            bats = false;
        }
        else{
		    displayRoomInfo( gameInfo, Rooms); 
        }
		// Prompt for and handle move
		printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
		scanf(" %c", &typeOfMove);	
									
		typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
		
		// Check all types of user input and handle them.  This uses if-else-if code
		//   rather than switch-case, so that we can take advantage of break and continue.
		if( typeOfMove == 'D') {
			displayCave();
			displayInstructions();
			continue;       // Loop back up to reprompt for the same move
		}
		else if( typeOfMove == 'P') {
			// To assist with play, display the cave layout
			displayCave();
			continue;       // Loop back up to reprompt for the same move
		}
		else if( typeOfMove == 'M') {
			scanf(" %d", &nextRoom);
			
			if(roomIsAdjacent( Rooms[ gameInfo.personRoom], nextRoom)) {
				gameInfo.personRoom = nextRoom;
                if(gameInfo.personRoom == gameInfo.arrows){
                    arrows1 = true;
                    gameInfo.arrows = gameInfo.personRoom;
                    printf("Congratulations, you found the arrow and can once again shoot.\n");
                }
                if(arrows1 == true){
                    gameInfo.arrows = nextRoom;
                }
                if(gameInfo.bats1 == nextRoom || gameInfo.bats2 == nextRoom){
                    batMove(&gameInfo, &personIsAlive, nextRoom);
                    bats = true;
                }
				checkForHazards(&gameInfo, &personIsAlive, Rooms);
			}
			else {
				printf("Invalid move.  Please retry. \n");
				continue;        // Doesn't count as a move, so retry same move.
			}
		}
		else if( typeOfMove == 'C') {
			// Display Cheat information
			displayCheatInfo(gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'R') {
			// Specify resetting the hazards and person positions, useful for testing
			resetPositions( &gameInfo, &arrows1);
			continue;        // Doesn't count as a move, so retry same move.
		}
        else if( typeOfMove == 'S') {
			// Prompt for room number guess and see if user has found the Wumpus
            if(arrows1 == true){
                shooting(&gameInfo, &arrows1, &wumpusIsAlive, &personIsAlive, Rooms);
            }
            else{
                printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
            }
            
			
		}
		else if( typeOfMove == 'X') {
			// Exit program
			personIsAlive = false;   // Indicate person is dead as a way to exit playing loop
			break;
		}

		// Increment the move number
		gameInfo.moveNumber = gameInfo.moveNumber + 1;
        
    }//end while(personIsAlive && wumpusIsAlive)
    
    printf("\nExiting Program ...\n");
	
	return 0;
}