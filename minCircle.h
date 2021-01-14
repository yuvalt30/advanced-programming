
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <algorithm> 
#include <assert.h> 
#include <math.h> 

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
	Circle(float x, float y, float r):center(Point(x,y)),radius(r){}
};
// --------------------------------------



float dist(Point a, Point b);

bool isInCircle(const Circle& c, const Point& p) ;

bool isValidCircle(const Circle& c, const vector<Point>& P) ;

Circle circleFrom3P(const Point& A, const Point& B, const Point& C) ;

Circle minCircleFrom2P(const Point& A, const Point& B) ;

Circle minCircleTrivial(vector<Point>& P) ;

Point calcCircleCenter(float bx, float by, float cx, float cy);

Circle welzl(Point** P,vector<Point> R, size_t n);

Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
