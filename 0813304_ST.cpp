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
int subSizeArr[8];
int bubbleArg;
mArg mergeArg[7];

int main(){
	
	int input;
	//for(int i=0; i<25; ++i){
	//	cin>>input;
	//	num.push_back(input);
	//}
	while(cin>>input){
		num.push_back(input);
	}
	
	listSize = num.size();
	subSize = listSize / 8;
	for(int i=0; i<8-1; ++i){
		subSizeArr[i] = subSize;
	}
	subSizeArr[7] = listSize - 7*(subSize);
	
	
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
	
	mergeArg[2].nLeft=subSizeArr[4];
	mergeArg[2].nRight=subSizeArr[5];
	mergeArg[2].startIndex= 4*subSize;
	
	mergeArg[3].nLeft=subSizeArr[6];
	mergeArg[3].nRight=subSizeArr[7];
	mergeArg[3].startIndex= 6*subSize;
	
	mergeArg[4].nLeft=subSizeArr[8];
    mergeArg[4].nRight=subSizeArr[9];
    mergeArg[4].startIndex= 8*subSize;

    mergeArg[5].nLeft=subSizeArr[10];
    mergeArg[5].nRight=subSizeArr[11];
    mergeArg[5].startIndex= 10*subSize;

    mergeArg[6].nLeft=subSizeArr[12];
    mergeArg[6].nRight=subSizeArr[13];
    mergeArg[6].startIndex= 12*subSize;

    mergeArg[7].nLeft=subSizeArr[14];
    mergeArg[7].nRight=subSizeArr[15];
    mergeArg[7].startIndex= 7*subSize;

	
	mergeArg[8].nLeft=subSizeArr[0]+subSizeArr[1];
	mergeArg[8].nRight=subSizeArr[2]+subSizeArr[3];
	mergeArg[8].startIndex= 0*subSize;
	
	mergeArg[9].nLeft=subSizeArr[4]+subSizeArr[5];
	mergeArg[9].nRight=subSizeArr[6]+subSizeArr[7];
	mergeArg[9].startIndex= 4*subSize;
	
	mergeArg[10].nLeft=subSizeArr[8]+subSizeArr[9];
    mergeArg[10].nRight=subSizeArr[10]+subSizeArr[11];
    mergeArg[10].startIndex= 8*subSize;

    mergeArg[11].nLeft=subSizeArr[12]+subSizeArr[13];
    mergeArg[11].nRight=subSizeArr[14]+subSizeArr[15];
    mergeArg[11].startIndex= 12*subSize;

	
	mergeArg[12].nLeft=subSizeArr[0]+subSizeArr[1]+subSizeArr[2]+subSizeArr[3];
	mergeArg[12].nRight=subSizeArr[4]+subSizeArr[5]+subSizeArr[6]+subSizeArr[7];
	mergeArg[12].startIndex= 0*subSize;
	
	mergeArg[13].nLeft=subSizeArr[8]+subSizeArr[9]+subSizeArr[10]+subSizeArr[11];
    mergeArg[13].nRight=subSizeArr[12]+subSizeArr[13]+subSizeArr[14]+subSizeArr[15];
    mergeArg[13].startIndex= 8*subSize;
    
    mergeArg[14].nLeft=subSizeArr[0]+subSizeArr[1]+subSizeArr[2]+subSizeArr[3]+subSizeArr[4]+subSizeArr[5]+subSizeArr[6]+subSizeArr[7];
    mergeArg[14].nRight=subSizeArr[8]+subSizeArr[9]+subSizeArr[10]+subSizeArr[11]+subSizeArr[12]+subSizeArr[13]+subSizeArr[14]+subSizeArr[15];
    mergeArg[14].startIndex= 0*subSize;

	
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
	
	for(int k=0; k<8; ++k){
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
	
	while(count<15){
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
