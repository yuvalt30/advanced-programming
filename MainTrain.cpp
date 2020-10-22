/*
 * MainTrain.cpp
 *
 *  Created on: 11 ����� 2020
 *      Author: yuva
 */

#include <iostream>
#include "anomaly_detection_util.h"

using namespace std;

bool wrong(float val, float expected){
	return val<expected-0.001 || val>expected+0.001;
}

// this is a simple test to put you on the right track
int main(){
	const int N=10;
	float x[]={1,2,3,4,5,6,7,8,9,10};
	float y[]={2.1,4.2,6.1,8.1,10.3,12.2,14.4,16.1,18.2,20.3};


}