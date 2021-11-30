#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <pthread.h>
#define BOUND 2147483647
#define N 16
using namespace std;

void *bubble_sort(void*);
void *merge(void*);

vector<int> num;
//vector<int> numSorted;
int listSize;
int subSize;
int subSizeArr[N];
//int bubbleArg=0;

struct mArg{
	int nLeft; //#left 
	int nRight;//#right
	int startIndex; // Index of first element in the left array
};

int main(){
	
	int input;
	while(cin>>input){
		num.push_back(input);
	}
	
	listSize = num.size();
	subSize = listSize / N;
	for(int i=0; i<N-1; ++i){
		subSizeArr[i] = subSize;
	}
	subSizeArr[N-1] = listSize - (N-1)*(subSize);

	//create thread to bubble sort
	pthread_t t_s[N];
	int bubbleArg[N];
	for(int i=0; i<N; ++i){
		bubbleArg[i] = i;
	}
	for(int i=0; i<N; ++i){
		pthread_create(&t_s[i], NULL, bubble_sort,(void*)&bubbleArg[i]);
	}
	for(int i=0; i<N; ++i){
		pthread_join(t_s[i], NULL);
	}
	
//	cout<<"bubble sort:"<<endl;
//	for(int i=0; i<num.size(); ++i){
//		cout<<num[i]<<" ";
//		if((i+1)%10==0 && i!=0) cout<<endl;
//	}
//	cout<<endl;
	
	//create thread to merge subarrays
	mArg mergeArg_layer1[4]={
		{subSizeArr[0], subSizeArr[1] , 0*subSize},
		{subSizeArr[2], subSizeArr[3] , 2*subSize},
		{subSizeArr[4], subSizeArr[5] , 4*subSize},
		{subSizeArr[6], subSizeArr[7] , 6*subSize}
	};
	mArg mergeArg_layer2[2]={
		{subSizeArr[0]+subSizeArr[1], subSizeArr[2]+subSizeArr[3] , 0*subSize},
		{subSizeArr[4]+subSizeArr[5], subSizeArr[6]+subSizeArr[7] , 4*subSize}
	};
	mArg mergeArg_layer3={
		subSizeArr[0]+subSizeArr[1]+subSizeArr[2]+subSizeArr[3], subSizeArr[4]+subSizeArr[5]+subSizeArr[6]+subSizeArr[7], 0*subSize
	};
	
	pthread_t t_m[N/2], T1_m, T2_m, T_m;
	for(int i=0; i< N/2; ++i){
		pthread_create(&t_m[i], NULL, merge,(void*)&mergeArg_layer1[i]);
	}
	for(int i=0; i< N/2; ++i){
		pthread_join(t_m[i], NULL);
	}

	pthread_create(&T1_m, NULL, merge,(void*)&mergeArg_layer2[0]);
	pthread_create(&T2_m, NULL, merge,(void*)&mergeArg_layer2[1]);
	pthread_join(T1_m, NULL);
	pthread_join(T2_m, NULL);

	
	pthread_create(&T_m, NULL, merge,(void*)&mergeArg_layer3);
	pthread_join(T_m, NULL);
	
//	cout<<endl;
//	cout<<"merging result:"<<endl;
	for(int i=0; i<num.size(); ++i){
		cout<<num[i]<<" ";
	}
	cout<<endl;
	
	return 0;
}

void *bubble_sort(void* bubbleArg){
	int parameter = *((int*) bubbleArg);
	int startIndex = (parameter) * subSize;
	int lengthToSort = subSizeArr[(parameter)];
	
	for(int i=0; i<lengthToSort-1; ++i){
		for(int j=0; j<lengthToSort-1-i; ++j){
			if( num[startIndex+j] > num[startIndex+j+1] ){
				int tmp = num[startIndex+j];
				num[startIndex+j] = num[startIndex+j+1];
				num[startIndex+j+1] = tmp;
			}
		}
	}
	pthread_exit(NULL);
}

void *merge(void* mergeArg){
	mArg* arg = (mArg*) mergeArg;
	int startLeft, startRight;
	int countLeft=0, countRight=0;
	vector<int> numSorted;
	numSorted.clear();
	startLeft = arg->startIndex;
	startRight = arg->startIndex + arg->nLeft;

	
	//BUILD: left/right array independently
	int *arrLeft = new int[arg->nLeft+1];
	int *arrRight = new int[arg->nRight+1];
	for(int i=0; i<arg->nLeft; i++){
		arrLeft[i] = num[startLeft];
		++startLeft;
	}
	for(int i=0; i<arg->nRight; i++){
		arrRight[i] = num[startRight];
		++startRight;
	}
	arrLeft[arg->nLeft] = BOUND;
	arrRight[arg->nRight] = BOUND;
	
	//COMPARE: the values pointed from left/right array
	for(int i=0; i<((arg->nLeft)+(arg->nRight)); ++i){
		if(arrLeft[countLeft] <= arrRight[countRight]){
			numSorted.push_back(arrLeft[countLeft]);
			++countLeft;
		}
		else{
			numSorted.push_back(arrRight[countRight]);
			++countRight;
		}
	}
	
	/////////test////////////////
//	for(int i=0; i<numSorted.size(); ++i){
//	cout<<numSorted[i]<<" ";
//	}
//	cout<<endl;
	/////////////////////////////
	
	for(int i=0; i<numSorted.size(); ++i){
		num[(arg->startIndex)+i] = numSorted[i];
	}
	pthread_exit(0);
	
}
