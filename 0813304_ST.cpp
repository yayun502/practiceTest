#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <pthread.h>
#define BOUND 2147483647
// 17 16 15 14 13 12 11 10 8 9 7 6 5 4 3 2 1 25 23 24 22 21 18 19 20
using namespace std;

void *bubble_sort(void*);
void *merge(void*);
void merge(int&, int&, int);

struct mArg{
	int nLeft; //#left 
	int nRight;//#right
	int startIndex; // Index of first element in the left array
};

vector<int> num;
//vector<int> numSorted;
int listSize;
int subSize;
int subSizeArr[4];
int bubbleArg;
mArg mergeArg[3];

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
	
	
	bubbleArg = 0;
	//create thread to bubble sort
	pthread_t t1_s;
	pthread_create(&t1_s, NULL, bubble_sort,(void*)&bubbleArg);
	pthread_join(t1_s, NULL);
    
	
//	for(int i=0; i<num.size(); ++i){
//		cout<<num[i]<<" ";
//		if((i+1)%subSize==0) cout<<endl;
//	}
//	cout<<endl;
	
	//create thread to merge subarrays
	mergeArg[0].nLeft=subSizeArr[0];
	mergeArg[0].nRight=subSizeArr[1];
	mergeArg[0].startIndex= 0*subSize;
	
	mergeArg[1].nLeft=subSizeArr[2];
	mergeArg[1].nRight=subSizeArr[3];
	mergeArg[1].startIndex= 2*subSize;
	
	mergeArg[2].nLeft=subSizeArr[0]+subSizeArr[1];
	mergeArg[2].nRight=subSizeArr[2]+subSizeArr[3];
	mergeArg[2].startIndex= 0*subSize;
	
		
	pthread_t t1_m;
	pthread_create(&t1_m, NULL, merge,(void*)&mergeArg[0]);
	pthread_join(t1_m, NULL);

	//cout<<endl;
	for(int i=0; i<num.size(); ++i){
		cout<<num[i]<<" ";
	}
	cout<<endl;
	
	return 0;
}

void *bubble_sort(void* arg){
	
	for(int k=0; k<4; ++k){
		int start = k * subSize;
		for(int i=0; i<subSizeArr[k]-1; ++i){
			for(int j=0; j<subSizeArr[k]-1-i; ++j){
				if( num[start+j] > num[start+j+1] ){
					int tmp = num[start+j];
					num[start+j] = num[start+j+1];
					num[start+j+1] = tmp;
				}
			}
		}
	}
	pthread_exit(NULL);
}

void *merge(void* a){
	//cout<<"stage3"<<endl;
	mArg* arg = (mArg*) a;
	int count = 0;
	
	while(count<3){
		merge(arg->nLeft, arg->nRight, arg->startIndex);
		++count;
		arg = &mergeArg[count];
	}
	
	pthread_exit(NULL);
}

void merge(int& nLeft, int& nRight, int startIndex){
	
	int startLeft, startRight;
	int countLeft=0, countRight=0;
	vector<int> numSorted;
	numSorted.clear();
	startLeft = startIndex;
	startRight = startIndex + nLeft;
	
	//BUILD: left/right array independently
	int *arrLeft = new int[nLeft+1];
	int *arrRight = new int[nRight+1];
	for(int i=0; i<nLeft; ++i){
		arrLeft[i] = num[startLeft];
		++startLeft;
	}
	for(int i=0; i<nRight; ++i){
		arrRight[i] = num[startRight];
		++startRight;
	}
	arrLeft[nLeft] = BOUND;
	arrRight[nRight] = BOUND;
	
	//COMPARE: the values pointed from left/right array
	for(int i=0; i<nLeft+nRight; ++i){
		//cout<<"left="<<arrLeft[countLeft]<<" right="<<arrRight[countRight]<<endl;
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
//	cout<<endl;
//	cout<<"Sorted = ";
//	for(int i=0; i<numSorted.size(); ++i){
//		cout<<numSorted[i]<<" ";
//	}
//	cout<<endl;
	/////////////////////////////
	
	for(int i=0; i<numSorted.size(); ++i){
		num[startIndex+i] = numSorted[i];
	}

}
