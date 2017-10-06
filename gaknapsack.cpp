// genetic algorithm 
#include <iostream>
#include <cstdlib>
#include <climits>
using namespace std;  
#define MAXVALUE 100 // 무게, 가치의 최대값 
#define N 30 // 짐 개수 
#define WEIGHTLIMIT (N*MAXVALUE/4) // 중량 제한 
#define POOLSIZE 30 // gene pool 크기  
#define LASTG 50 // 끝 세대  
#define MRATE 0.01 // 돌연변이의 확률 
#define SEED 32767 // 난수 시드 
#define YES 1 // yes에 해당하는 정수 
#define NO 0 // no에 해당하는 정수 
// declaring function prototype 
void initparcel(); // 짐 초기화 
int evalfit(int gene[]); // 적응도 계산 
void mating(int pool[POOLSIZE][N],int ngpool[POOLSIZE*2][N]); // 교차 
void mutation(int ngpool[POOLSIZE*2][N]); // 돌연변이  
void printp(int pool[POOLSIZE][N]); // 결과 출력 
void initpool(int pool[POOLSIZE][N]);  // 초기 집단 생성 
int rndn(int l);  // n 미만의 난수 생성 
int notval(int v); // 진리값 반전 
int selectp(int roulett[POOLSIZE],int totalfitness); // 부모 선택 
void crossing(int m[],int p[],int c1[],int c2[]); // 특정 염색체 2개를 교차 
void selectng(int ngpool[POOLSIZE*2][N],int pool[POOLSIZE][N]); // 다음 세대 선택 
// 전역 변수 (짐)
int parcel[N][2];  
// main function 
int main(int argc,char *argv[]){
	int pool[POOLSIZE][N];  // 염색체 풀 
	int ngpool[POOLSIZE*2][N]; // 다음 세대 염색채 풀 
	int generation; // 현재 세대 수 
	srand(SEED); // 난수 초기화 
	initparcel(); // initializing bag 
	initpool(pool); // 초기 집단 생성 
	// continue until last generation 
	for (generation = 0; generation < LASTG; generation++){
		printf("%d세대\n",generation); 
		mating(pool,ngpool); // 교차 
		mutation(ngpool);  // 돌연변이 
		selectng(ngpool,pool);  // 다음 세대 선택 
		printp(pool); // 결과 출력 
	}
	return 0;  
} 
// initparcel 
void initparcel(){
	int i = 0; 
	while ((i < N) && (scanf("%d %d",&parcel[i][0],&parcel[i][1]) != EOF)){
		i++; 
	} 
}
// selectng: choosing the next generation 
void selectng(int ngpool[POOLSIZE*2][N],int pool[POOLSIZE][N]){
	int i,j,c;  // for iteration 
	int totalfitness = 0; // 적응도 합계 
	int roulette[POOLSIZE*2]; // 적응도 저장 
	int ball;  //  공 (선택 위치의 수치)
	int acc = 0; // 적응도 누적 계산 값 
	// selecting iteratively 
	for (i = 0; i < POOLSIZE; i++){
		// constructing roulette  
		totalfitness = 0;  
		for (c = 0; c < POOLSIZE*2; c++){
			roulette[c] = evalfit(ngpool[c]);  
			// 적응도 합계 계산 
			totalfitness += roulette[c];  
		}
		// 염색체 하나를 선택 
		ball = rndn(totalfitness); 
		acc = 0; 
		for (c = 0; c < POOLSIZE*2; c++){
			acc += roulette[c]; // 적응도 누적 계산 
			if (acc > ball) break; //  해당하는 유전자 
		}
		for (j = 0; j < N; j++){
			pool[i][j] = ngpool[c][j]; 
		}
	}
}
int selectp(int roulette[POOLSIZE],int totalfitness){
	int i; // 반복 제어 변수 
	int ball; // 공 (선택 위치의 수치)
	int acc = 0; // 적응도 누적 계산 값 
	ball = rndn(totalfitness); 
	for (i = 0; i < POOLSIZE; i++){
		acc += roulette[i];  
		if (acc > ball) break; 
	}
	return i; 
}
// mating() function 
void mating(int pool[POOLSIZE][N],int ngpool[POOLSIZE*2][N]){
	int i;  
	int totalfitness = 0;  
	int roulette[POOLSIZE]; // 적응도 저장 
	int mama,papa; 
	for (i = 0; i < POOLSIZE; i++){
		roulette[i] = evalfit(pool[i]);  
		totalfitness += roulette[i]; 
	}
	for (i = 0; i < POOLSIZE; i++){
		do{
			mama = selectp(roulette,totalfitness); 
			papa = selectp(roulette,totalfitness); 
		}while (mama == papa); // get rid of overlaps 
		crossing(pool[mama],pool[papa],ngpool[i*2],ngpool[i*2+1]);  
	}
}
// crossing() function 
// 특정 염색체 2개를 교차 
void crossing(int m[],int p[],int c1[],int c2[]){
	int j; // 반복 제어 변수 
	int cp; // 교차하는 점 
	cp = rndn(N); // selecting intersecting point 
	// copy front 
	for (j = 0; j < cp; j++){
		c1[j] = m[j]; 
		c2[j] = p[j]; 
	}
	// copy back 
	for (;j < N; j++){
		c2[j] = m[j]; 
		c1[j] = p[j]; 
	}
}
// evalfit() function 
int evalfit(int g[]){
	int pos; 
	int value = 0; 
	int weight = 0;  
	for (pos = 0; pos < N; pos++){
		weight += parcel[pos][0] * g[pos]; 
		value += parcel[pos][1] * g[pos]; 
	}
	if (weight >= WEIGHTLIMIT) value = 0; 
	return value; 
}
// printp() 
void printp(int pool[POOLSIZE][N]){
	int i,j; 
	int fitness; 
	double totalfitness = 0;  
	int elite,bestfit = 0; // 앨리트 유전자 처리용 비용 
	for (i = 0; i < POOLSIZE; i++){
		for (j = 0; j < N; j++){
			printf("%1d",pool[i][j]);  
		}
		fitness = evalfit(pool[i]);  
		printf("\t%d\n",fitness); 
		if (fitness > bestfit){
			bestfit = fitness; 
			elite = i; 
		}
		totalfitness += fitness; 
	}
	printf("%d\t%d \t",elite,bestfit); 
	printf("%lf\n",totalfitness/POOLSIZE); 
}
void initpool(int pool[POOLSIZE][N]){
	int i,j; 
	for (i = 0; i < POOLSIZE;i++){
		for (j = 0; j < N; j++){
			pool[i][j] = rndn(2); 
		}
	}
}
int rndn(int l){
	int rndno; 
	while ((rndno = ((double)rand()/RAND_MAX)*l) == l);  
	return rndno; 
}
void mutation(int ngpool[POOLSIZE][N]){
	int i,j; 
	for (i = 0; i < POOLSIZE*2; i++){
		for (j = 0; j < N; j++){
			if ((double)rndn(100)/100.0 <= MRATE){
				ngpool[i][j] = notval(ngpool[i][j]);  
			}
		}
	}
}
int notval(int v){
	if (v == YES) return NO; 
	else return YES; 
}
