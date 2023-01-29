#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int thief_x, thief_y, police_x, police_y;//Indexes for thief and police locations
char map[16][16];//GUI for game

int dist(int x1, int y1);//Function that finds the distance between input and thief
int movement(int * p1, int * p2);//Function that move the thief 
int easy_police(int * p1, int * p2);//Easy difficulty algorithm
int hard_police(int * p1, int * p2);//Hard difficulty algorithm

int main(){
	int column, row;//Counters for map creation
	int state = -1;//Variable that defines the game activity
	int difficulty;//Variable that defines the game difficulty
	int *p1, *p2;//Pointers for functions
	char choose;//Variable for choice
	char wall_row[] = {0, 1, 2, 3, 5, 6, 8, 9, 10, 11, 14};//Row indexes for walls in the map
	char wall_col[11][4] = {{8},{8},{2, 13},{2, 13},{6, 7, 8},{6},{10},{3, 4, 5, 10},{5, 10},{10, 11},{2, 3, 4}};//Column indexes for walls in the map
	
	srand(time(0));//To get different random numbers for every gameplay
	//While loop to determine to keep the program running after game finish 
	while(state < 0){
		//Instructions
		printf("HOW TO PLAY:\n"
			   "w -> Up\n"
			   "a -> Left\n"
			   "s -> Down\n"
			   "d -> Right\n"
			   "You 'T' must reach the ecape point 'E' without getting caught by the police 'P'\n"
			   "You can not go through the walls 'W'\n"
			   "Have FUN!\n\n");
		printf("Choose Difficulty!\nEasy[e]\t\t\tHard[h]\n");
		//While loop to keep the player from entering unwanted inputs
		while(1){
			//Difficulty choice
			choose = getch();
			if(choose == 'e'){
				difficulty = 1;
				break;
			} 
			else if(choose == 'h'){
				difficulty = 2;
				break;
			}
		}
		
		system("cls");//Clear the consoles screen
		//While loop to keep the game running
		while(1){
			//Defining empty map
			for(row = 0; row < 16; row++){
				for(column = 0; column < 16; column++){
					map[row][column] = '_';
				}
			}
			//Defining walls
			for(row = 0; row < 11; row++){
				for(column = 0; column < 4; column++){
					if(wall_col[row][column] != '\0'){
						map[wall_row[row]][wall_col[row][column]] = 'W';
					}
				}
			}
			
			map[7][15] = 'E';//Defining escape point
			//While loop to make the code run just one time
			while(state == -1){
				//Randomly generating locations
				thief_x = rand() % 16;
				thief_y = rand() % 16;
				police_x = rand() % 16;
				police_y = rand() % 16;
				//Making locations not on the walls
				if(map[thief_y][thief_x] == 'W' || map[police_y][police_x] == 'W' || map[police_y][police_x] == 'E'){
					continue;
				}
				//Keeping the distance of thief between police and escape point are not less than 16 squares
				if(dist(police_x, police_y) >= 16 && dist(15, 7) >= 16){
					state = -2;
				}
			}
			
			map[thief_y][thief_x] = 'T';//Defining thief
			
			map[police_y][police_x] = 'P';//Defining police
			//Printing the map
			for(row = 0; row < 16; row++){
				printf("|");
				for(column = 0; column < 16; column++){
					printf(" %c ", map[row][column]);
					printf("|");
				}
				printf("\n");
			}
			//Checking if the player got caught and ending the game. If distance is 1 or 2 that means police is either on the thief or next to.
			if(dist(police_x, police_y) == 1 || dist(police_x, police_y) == 0){
				state = 0;
				break;
			}
			
			movement(&thief_x, &thief_y);
			
			if(difficulty == 1) easy_police(&police_x, &police_y);
			
			if(difficulty == 2) hard_police(&police_x, &police_y);
			//Checking if the player has reached the escape point
			if(thief_x == 15 && thief_y == 7){
				state = 1;
				break;
			}
			//Checking if the police has reached the escape point
			if(police_x == 15 && police_y == 7){
				state = 0;
				break;
			}
			system("cls");
		}
		//Checking the result of the game
		if(state == 1){
			printf("YOU WON!");
		}
		else if(state == 0){
			printf("YOU LOSE!");
		}
		printf("\nDo You Want To Play Again?\nYes[y]\t\t\tNo[n]\n");
		//While loop to keep the player from entering unwanted inputs
		while(1){
			choose = getch();
			//Restarting the game
			if(choose == 'y'){
				system("cls");
				state = -1;
				break;
			}
			//Ending the program 
			else if(choose == 'n'){
				system("cls");
				printf("Shutting Down!");
				break;
			}
		}
	}
	return 0;
}

int dist(int x1, int y1){
	int distance;
	distance = abs(thief_x - x1) + abs(thief_y - y1);//Finding distance according to Manhattan Distance
	return distance;
}

int movement(int * p1, int * p2){
	char move;
	//While loop to keep the player from entering unwanted inputs
	while(1){
		move = getch();//Player input
		//Checking if the player is already on the edge of the map or the wall then moving the player
		if(move == 'w' && thief_y != 0 && map[thief_y-1][thief_x] != 'W'){
			return *p2-=1;
		}
		else if(move == 'd' && thief_x != 15 && map[thief_y][thief_x+1] != 'W'){
			return *p1+=1;
		}
		else if(move == 's' && thief_y != 15 && map[thief_y+1][thief_x] != 'W'){
			return *p2+=1;
		}
		else if(move == 'a' && thief_x != 0 && map[thief_y][thief_x-1] != 'W'){
			return *p1-=1;
		}
	}
}

int easy_police(int * p1, int * p2){
	//While loop to keep the police from going to the unwanted directions
	while(1){
		//Randomly choosing direction
		switch(rand() % 4){
			case 0:
				//Checking if the police is already on the edge of the map or the wall then moving the police
				if(map[police_y][police_x+1] != 'W' && police_x != 15){
					return *p1+=1;	
				}
				break;
			case 1:
				if(map[police_y][police_x-1] != 'W' && police_x != 0){
					return *p1-=1;
				}
				break;
			case 2:
				if(map[police_y+1][police_x] != 'W' && police_y != 15){
					return *p2+=1;
				}
				break;	
			case 3:
				if(map[police_y-1][police_x] != 'W' && police_y != 0){
					return *p2-=1;
				}
				break;
		}
	}
}

int hard_police(int * p1, int * p2){
	int fSquare[4] = {96, 97, 98, 99};//Array to keep the distance between police and player
	int i, temp = 3;//Variables to find the optimal squares
	int optimal1, optimal2, optimal3;//Possible directions of movement
	int select;//Selected direction of movement
	//Checking if the police is already on the edge of the map or the wall then assigning the distance to the array
	if(map[police_y][police_x+1] != 'W' && police_x != 15){
		fSquare[0] = dist(police_x+1, police_y);	
	}
	if(map[police_y][police_x-1] != 'W' && police_x != 0){
		fSquare[1] = dist(police_x-1, police_y);
	}
	if(map[police_y+1][police_x] != 'W' && police_y != 15){
		fSquare[2] = dist(police_x, police_y+1);
	}
	if(map[police_y-1][police_x] != 'W' && police_y != 0){
		fSquare[3] = dist(police_x, police_y-1);
	}
	//Initialising
	optimal1 = 3;//Assuming optimal direction
	optimal2 = 4;
	optimal3 = 4;

	for(i = 0; i < 3; i++){
		//Finding minimum index
		if(fSquare[temp] > fSquare[i]){
			temp = i;
			optimal1 = temp;
		}
		//Finding other minimum indexes
		if(fSquare[temp] == fSquare[i]){
			if(optimal2 == 4) optimal2 = i;
			else optimal3 = i;
		}
		
	}
	select = optimal1;//Selecting minimum by default
	//If there are more than one minimum direction choosing one randomly
	if(fSquare[optimal1] == fSquare[optimal3] && fSquare[optimal1] == fSquare[optimal2]){
		switch(rand() % 3){
			case 0:
				select = optimal1;
				break;
			case 1:
				select = optimal2;
				break;
			case 2:
				select = optimal3;
				break;
			}
	}
	else if(fSquare[optimal1] == fSquare[optimal2]){
		if(rand() % 2){
			select = optimal2;
		}
	}
	//Moving police according to optimal direction
	switch(select){
		case 0:
			return *p1+=1;
			break;
		case 1:
			return *p1-=1;
			break;
		case 2:
			return *p2+=1;
			break;	
		case 3:
			return *p2-=1;
			break;
	}
}
