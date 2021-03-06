// Author : Oguz Bilgic
// Date	  : 20-October-2010
// File   : broke_par.c
// Project: CS 100 - Programing Assignment #4 - Need for Speed
// Desc	  : Code Parallelization  
// Repo   : https://gist.github.com/295a0516f0fe5f525e34
// Version: 

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
// Flips the coin
// Returns 0 or 1 depends on the "probability of heads" input
// Gets "probability of heads" as an input
int flip_coin(double pro_of_heads){	   
	
	unsigned int ri;
	// If (random number divided by RAND_MAX) is bigger than "probability of heads" return tail
	// Else return head		   	
	return ((double) rand_r(&ri) / (double) RAND_MAX) > pro_of_heads ? 0 : 1 ;
}
 
// Plays 1 round of game
// Returns final number of coins for 3 players
// Gets "probablity of heads" as an input
// Gets "players" array as an input which is the array of number of coins before this round
void play_new_round(int *players, double pro_of_heads){
	   
	// Array which stores flipped coins for this round
	int i, coins_on_table[3] = {};
	  
	// All players get a coin form their pockets
	// All players flip their coins and put it on the table	
	for (i = 0; i < 3; i++){
		players[i] --;
		coins_on_table[i] = flip_coin(pro_of_heads);
	}
	   
	// If all coins on the table are same, this is tie game
	// Else find the distinct coin and give all coins to owner of this coin
	if (coins_on_table[0] + coins_on_table[1] + coins_on_table[2] == 0 ||
		coins_on_table[0] + coins_on_table[1] + coins_on_table[2] == 3){
		players[0] ++; players[1] ++; players[2] ++;
	} else if(coins_on_table[0] == coins_on_table[1]){
		players[2] += 3;
	} else if(coins_on_table[1] == coins_on_table[2]){
		players[0] += 3;
	} else if(coins_on_table[0] == coins_on_table[2]){
		players[1] += 3;
	}
}
 
// Plays 1 complate game
// Returns the number of rounds played in this game
// Gets "number initial coins per player" as an input
// Gets "probablity of heads" as an input
int play_new_game(int num_of_coins, double pro_of_heads){
	   
	// We give same number of coins to every player
	int players[3] = {num_of_coins, num_of_coins, num_of_coins};
	  
	// This variable stores number of rounds played in this game
	int num_of_round = 0;
	  
	// While none of the players has 0 coin , play new round
	while(players[0] != 0 && players[1] != 0 && players[2] != 0){
		play_new_round(players, pro_of_heads);
		   
		// This round is played, so increase number of played rounds
		num_of_round ++;
	}
	  
	return num_of_round;
}
 
// Simulates requested number of games
// Returns the average number of rounds played per game
// Gets "number of games to simulate" as an input
// Gets "number initial coins per player" as an input
// Gets "probablity of heads" as an input
double simulate_games(int num_of_games, int num_of_coins, double pro_of_heads){
	   
	// Number of current game
	  
	// Average number of played rounds per each game
	// Total number of round played in this simulation
	double average, tot_num_of_rounds = 0;
	  
	// Always be uniqe!
	// This forces rand() function to create unique sequences
	//srand(time(NULL));
	  
	// Plays given number of games
	#pragma omp parallel for reduction(+:tot_num_of_rounds)
	for(int current_game = 1; current_game <= num_of_games; current_game++){
		tot_num_of_rounds += play_new_game(num_of_coins, pro_of_heads);
	}
	  
	// Calculates average rounds per game
	average = tot_num_of_rounds / (double) num_of_games;
	return average;
}

// Main function
int main(int argc, char *argv[]){
	
	// Initial number of coins per player
	// Number of games in this simulations
	int num_of_coins, num_of_games, num_of_threads;
		
	// Average number of played rounds per each game
	// Probability of heads (Fairness)
	double average, pro_of_heads;
		
	num_of_coins = atof(argv[1]);
	pro_of_heads = atof(argv[2]);
	num_of_games = atof(argv[3]);
	num_of_threads = atof(argv[4]);
	omp_set_num_threads(num_of_threads);

	//------Record Time------//
//	clock_t start, finish;
//	double sec;
//	start = clock();
	struct timeval start, finish;
	unsigned long long usec;
	gettimeofday(&start, NULL);
	//------Record Time------//

	  
	// Runs actual function with given variables	   
	average = simulate_games(num_of_games, num_of_coins, pro_of_heads);
	
	//------Record Time------//
//	finish = clock();
//	sec = (double) (finish-start) / (double) CLOCKS_PER_SEC ;
//	printf("Time taken with is %f seconds\n", sec);	
	gettimeofday(&finish, NULL);
	usec = finish.tv_sec*1000*1000 + finish.tv_usec;
	usec -= (start.tv_sec*1000*1000 + start.tv_usec);
	printf("Time taken with is %lld seconds\n", usec);
	//------Record Time------//	  
	  
	printf("Average is: %f \n", average);
	
	
	
	
	return 0;
}
