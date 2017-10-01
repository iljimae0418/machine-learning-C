#include <cstdio> 
#include <cstdlib>
#include <algorithm> 
using namespace std; 

#define GENMAX 1000
#define NODENO 15
#define ALPHA 0.1 
#define GAMMA 0.9 
#define EPSILON 0.3 
#define SEED 32767 

int rand100();  // returns an integer between 0 and 100
int rand01();  // returns 0 or 1
double rand1(); // returns a double between 0 and 1 inclusive
void printqvalue(int qvalue[NODENO]); 
int selecta(int s,int qvalue[NODENO]); 
int updateq(int s,int qvalue[NODENO]); 
int main(){
	int i; 
	int s; // state 
	int t; // view 
	int qvalue[NODENO];  
	srand(SEED); 
	for (i = 0; i < NODENO; i++){
		qvalue[i] = rand100(); 
	}
	printqvalue(qvalue); 
	// Q-learning step 
	for (i = 0; i < GENMAX; i++){
		s = 0; 
		for (t = 0; t < 3; t++){
			s = selecta(s,qvalue); // choose an action 
			qvalue[s] = updateq(s,qvalue); // update Q value  
		}
		printqvalue(qvalue); 
	}
	return 0; 
}
int updateq(int s,int qvalue[NODENO]){
	int qv; // updated Q value 
	int qmax; // the maximum Q value
	if (s > 6){ // the last stage
		if (s == 14){ // the node with a reward 
			qv = qvalue[s]+ALPHA*(1000-qvalue[s]);  
		}else{
			qv = qvalue[s];  
		}
	}else{
		if (qvalue[2*s+1] > qvalue[2*s+2]){
			qmax = qvalue[2*s+1];  
  		}else{
  			qmax = qvalue[2*s+2];  
  		}
  		qv = qvalue[s]+ALPHA*(GAMMA*qmax - qvalue[s]);  
	}
	return qv; 
}
int selecta(int olds,int qvalue[NODENO]){
	int s;  
	// epsilon-greedy method 
	if (rand1() < EPSILON){
		if (rand01() == 0){
			s = 2*olds+1;  
		}else{
			s = 2*olds+2;  
		}
	}else{
		if (qvalue[2*olds+1] > qvalue[2*olds+2]){
			s = 2*olds+1; 
		}else{
			s = 2*olds+2;  
		}
	}
	return s;  
}
void printqvalue(int qvalue[NODENO]){
	for (int i = 1; i < NODENO; i++){
		printf("%d ",qvalue[i]); 
	}
	printf("\n"); 
}
double rand1(){
	return (double)rand()/RAND_MAX; 
}
int rand01(){
	int rnd; 
	while ((rnd = rand()) == RAND_MAX){}
	return (int)((double)rnd/RAND_MAX*2); 
}
int rand100(){
	int rnd; 
	while ((rnd = rand()) == RAND_MAX){}
	return (int)((double)rnd/RAND_MAX*101);  
}
