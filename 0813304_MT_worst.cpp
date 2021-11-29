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
//vector<int> numSorted;
int listSize;
int subSize;
int subSizeArr[8];
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
	subSize = listSize / 8;
	for(int i=0; i<8-1; ++i){
		subSizeArr[i] = subSize;
	}
	subSizeArr[7] = listSize - 7*(subSize);

	//create thread to bubble sort
	pthread_t t1_s, t2_s, t3_s, t4_s, t5_s, t6_s, t7_s, t8_s;
	int bubbleArg1 = 0;
	pthread_create(&t1_s, NULL, bubble_sort,(void*)&bubbleArg1);
	int bubbleArg2 = 1;
	pthread_create(&t2_s, NULL, bubble_sort,(void*)&bubbleArg2);
	int bubbleArg3 = 2;
	pthread_create(&t3_s, NULL, bubble_sort,(void*)&bubbleArg3);
	int bubbleArg4 = 3;
	pthread_create(&t4_s, NULL, bubble_sort,(void*)&bubbleArg4);
	int bubbleArg5 = 4;
	pthread_create(&t5_s, NULL, bubble_sort,(void*)&bubbleArg5);
	int bubbleArg6 = 5;
	pthread_create(&t6_s, NULL, bubble_sort,(void*)&bubbleArg6);
	int bubbleArg7 = 6;
	pthread_create(&t7_s, NULL, bubble_sort,(void*)&bubbleArg7);
	int bubbleArg8 = 7;
	pthread_create(&t8_s, NULL, bubble_sort,(void*)&bubbleArg8);
    pthread_join(t1_s, NULL);
    pthread_join(t2_s, NULL);
	pthread_join(t3_s, NULL);
	pthread_join(t4_s, NULL);
	pthread_join(t5_s, NULL);
	pthread_join(t6_s, NULL);
	pthread_join(t7_s, NULL);
	pthread_join(t8_s, NULL);
	
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
	
	pthread_t t1_m, t2_m, t3_m, t4_m,
			  T1_m, T2_m, T_m;
//	int res;
	pthread_create(&t1_m, NULL, merge,(void*)&mergeArg_layer1[0]);
	pthread_create(&t2_m, NULL, merge,(void*)&mergeArg_layer1[1]);
	pthread_create(&t3_m, NULL, merge,(void*)&mergeArg_layer1[2]);
	pthread_create(&t4_m, NULL, merge,(void*)&mergeArg_layer1[3]);
//	if(res!=0) cout<<"create error!"<<endl;
	pthread_join(t1_m, NULL);
//	if(res!=0) cout<<"wait error!"<<endl;
	pthread_join(t2_m, NULL);
	pthread_join(t3_m, NULL);
	pthread_join(t4_m, NULL);
	
	pthread_create(&T1_m, NULL, merge,(void*)&mergeArg_layer2[0]);
	pthread_create(&T2_m, NULL, merge,(void*)&mergeArg_layer2[1]);
	pthread_join(T1_m, NULL);
	pthread_join(T2_m, NULL);

	pthread_create(&T_m, NULL, merge,(void*)&mergeArg_layer3);
	pthread_join(T_m, NULL);
	
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
	//cout<<"thread"<<parameter+1<<endl;
	//cout<<"startIndex: "<<startIndex<<endl<<endl;
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
	//cout<<arg->nLeft<<" "<<arg->nRight<<" "<<arg->startIndex<<endl;
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
		//cout<<"["<<startLeft<<"]="<<arrLeft[i]<<" ";
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
//		cout<<"left="<<arrLeft[countLeft]<<" right="<<arrRight[countRight]<<endl;
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
