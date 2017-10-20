// back-propagation for three layered neural net 
// rough code template 
// supply with input data 
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#define INPUTNO 3  // 입력층 셀 개수 
#define HIDDENNO 3 // 중간층 셀 개수 
#define ALPHA 10 // 학습 계수 
#define SEED 65535 // 난수 시드 
#define MAXINPUTNO 100 // 학습 데이터의 최대 개수 
#define BIGNUM 100 // 오차의 초깃값  
#define LIMIT 0.001 // 오차의 상한값 
// function prototypes  
double f(double u); // sigmoid function 
void initwh(double wh[HIDDENNO][INPUTNO+1]);  
// 중간층 가중치의 초기화 
void initwo(double wo[HIDDENNO+1]); 
// 출력층 가중치의 초기화 
double drnd(void); // 난수 생성 
void print(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO+1]);  
// 결과 출력 
double forward(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO],
	double hi[],double e[INPUTNO+1]); // 순방향 계산
void olearn(double wo[HIDDENNO+1],double hi[],double e[INPUTNO+1],double o);
// 출력층 가중치의 조정  
int getdata(double e[][INPUTNO+1]); // 학습 데이터 읽기 
void hlearn(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO+1],double hi[],
	double e[INPUTNO+1],double o);  // 중간층 가중치의 조정 
int main(){
	double wh[HIDDENNO][INPUTNO+1]; // 중간층 가중치  
	double wo[HIDDENNO+1]; // 출력층 가중치 
	double e[MAXINPUTNO][INPUTNO+1]; // 학습 데이터 집합 
	double hi[HIDDENNO+1]; // 중간층 출력 
	double o; // 출력 
	double err = BIGNUM; // 오차 평가 
	int i,j; // for iteration 
	int n_of_e; // 학습 데이터 개수 
	int count = 0; // 반복 횟수 카운터 
	// seed pseudo random generator 
	srand(SEED); 
	// 가중치 초기화 
	initwh(wh); // 중간층 가중치의 초기화 
	initwo(wo); // 출력층 가중치의 초기화 
	print(wh,wo);  
	// reading test data (학습 데이터 읽어들이기)
	n_of_e = getdata(e); 
	printf("학습 데이터 개수: %d\n",n_of_e); 
	// training 
	while (err > LIMIT){
		err = 0.0; 
		for (j = 0; j < n_of_e; j++){
			// feed-forward algorithm calculation 
			o = forward(wh,wo,hi,e[j]);  
			// 출력층 가중치의 조정 
			olearn(wo,hi,e[j],o); 
			// 중간층 가중치의 조정 
			hlearn(wh,wo,hi,e[j],o); 
			// 오차 계산 
			err += (o-e[j][INPUTNO])*(o-e[j][INPUTNO]);  
		}
		++count; 
		// 오차 출력 
		fprintf(stderr,"%d\t%lf\n",count,err); 
	} // end of training  
	print(wh,wo);  
	// 학습 데이터에 대한 출력 
	for (i = 0; i < n_of_e; i++){
		printf("%d ",i);  
		for (j = 0; j < INPUTNO+1; j++){
			printf("%lf ",e[i][j]);  
		}
		o = forward(wh,wo,hi,e[i]); 
		printf("%lf\n",o);  
	}
	return 0; 
}  
void hlearn(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO+1],double hi[],double e[INPUTNO+1],double o){
	int i,j; // for iteration 
	double dj; // 중간층 가중치 계산에 이용  
	for (j = 0; j < HIDDENNO; j++){
		dj = hi[j]*(1-hi[j])*wo[j]*(e[INPUTNO]-o)*o*(1-o); 
		for (i = 0; i < INPUTNO; i++){
			wh[j][i] += ALPHA*e[i]*dj;  
		}
		wh[j][i] += ALPHA*(-1.0)*dj; // 문턱값 학습 
	}
}
int getdata(double e[][INPUTNO+1]){
	int n_of_e = 0; // number of data  
	int j = 0; 
	while (scanf("%lf",&e[n_of_e][j]) != EOF){
		++j; 
		if (j > INPUTNO){
			n_of_e++; 
			j = 0; 
		}
	}
	return n_of_e; 
}
void olearn(double wo[HIDDENNO+1],double hi[],double e[INPUTNO+1],double o){
	int i; 
	double d; // for weight calculation 
	d = (e[INPUTNO]-o)*o*(1-o);  
	for (i = 0; i < HIDDENNO; i++){
		wo[i] += ALPHA*hi[i]*d; // weight training  
	}
	wo[i] += ALPHA*(-1.0)*d; // doorstep training 
}
double forward(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO],double hi[],
	double e[INPUTNO+1]){
	int i,j; 
	double u;  
	double o;  
	for (i = 0; i < HIDDENNO; i++){
		u = 0;  
		for (j = 0; j <INPUTNO; j++){
			u += e[j]*wh[i][j]; 
		}
		u -= wh[i][j]; // doorstep value 
		hi[i] = f(u); 
	}
	// 출력 o 계산 
	for (i = 0; i < HIDDENNO; i++){
		o += hi[i]*wo[i];  
	}
	o -= wo[i]; 
	return f(o);  
}
void print(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO+1]){
	for (int i = 0; i< HIDDENNO; i++){
		for (int j = 0; j < INPUTNO+1; j++){
			printf("%lf ",wh[i][j]); 
		}
	}
	printf("\n"); 
	for (int i = 0; i< HIDDENNO; i++){
		printf("%lf",wo[i]); 
	}
	printf("\n");
}
void initwh(double wh[HIDDENNO][INPUTNO+1]){
	int i,j; 
	for (i = 0; i < HIDDENNO; i++){
		for (j = 0; j < INPUTNO+1; j++){
			wh[i][j] = drnd();  
		}
	}
}
void initwo(double wo[HIDDENNO+1]){
	int i; 
	for (i = 0; i < HIDDENNO+1; i++){
		wo[i] = drnd(); 
	}
}
double drnd(void){
	double rndno; 
	while ((rndno = (double)rand()/RAND_MAX) == 1.0); 
	rndno = rndno*2 - 1;  
	return rndno;  
}
double f(double u){
	return 1.0/(1.0+exp(-u)); 
}
