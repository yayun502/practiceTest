#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <pthread.h>
#define BOUND 2147483647

using namespace std;

void *bubble_sort(void*);
void *merge(void*);

vector<int> num;
int listSize;
int subSize;
int subSizeArr[4];
int bubbleArg=0;

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
	subSize = listSize / 4;
	for(int i=0; i<4-1; ++i){
		subSizeArr[i] = subSize;
	}
	subSizeArr[3] = listSize - 3*(subSize);

	//create thread to bubble sort
	pthread_t t1_s, t2_s, t3_s, t4_s;
	int bubbleArg1 = 0;
	pthread_create(&t1_s, NULL, bubble_sort,(void*)&bubbleArg1);
	int bubbleArg2 = 1;
	pthread_create(&t2_s, NULL, bubble_sort,(void*)&bubbleArg2);
	int bubbleArg3 = 2;
	pthread_create(&t3_s, NULL, bubble_sort,(void*)&bubbleArg3);
	int bubbleArg4 = 3;
	pthread_create(&t4_s, NULL, bubble_sort,(void*)&bubbleArg4);
    pthread_join(t1_s, NULL);
    pthread_join(t2_s, NULL);
	pthread_join(t3_s, NULL);
	pthread_join(t4_s, NULL);

	
	//create thread to merge subarrays
	mArg mergeArg_layer1[2]={
		{subSizeArr[0], subSizeArr[1] , 0*subSize},
		{subSizeArr[2], subSizeArr[3] , 2*subSize}
	};
	mArg mergeArg_layer2={
		{subSizeArr[0]+subSizeArr[1], subSizeArr[2]+subSizeArr[3] , 0*subSize}
	};
	
	pthread_t t1_m, t2_m, T_m;

	pthread_create(&t1_m, NULL, merge,(void*)&mergeArg_layer1[0]);
	pthread_create(&t2_m, NULL, merge,(void*)&mergeArg_layer1[1]);
	pthread_join(t1_m, NULL);
	pthread_join(t2_m, NULL);

	pthread_create(&T_m, NULL, merge,(void*)&mergeArg_layer2);
	pthread_join(T_m, NULL);
	
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
