// 311127120
#include "minCircle.h"
using namespace std;

// ------------ DO NOT CHANGE -----------
class Point{
public:
	float x,y;
	Point(float x,float y):x(x),y(y){}
};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

  


// returns circle from 3 points 
Point calcCircleCenter(float bx, float by, float cx, float cy) 
{ 
    float X = bx * bx + by * by; 
    float Y = cx * cx + cy * cy; 
    float Z = bx * cy - by * cx; 
    return { (cy * X - by * Y) / (2 * Z), 
             (bx * Y - cx * X) / (2 * Z) }; 
} 
  

// returns distance between two points 
float dist(const Point& a, const Point& b) 
{ 
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); 
} 


// checks if a point is in circle or on it's perimeter
bool isInCircle(const Circle& c, const Point& p) 
{ 
    return dist(c.center, p) <= c.radius; 
} 

// checks if circle contains given points 
bool isValidCircle(const Circle& c, const vector<Point>& P) 
{ 
    for (const Point& p : P) 
        if (!isInCircle(c, p)) 
            return false; 
    return true; 
} 


// returns a unique circle that intersects three points 
Circle circleFrom3P(const Point& A, const Point& B, const Point& C) 
{ 
    Point center = calcCircleCenter(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y); 
  
    center.x += A.x; 
    center.y += A.y; 
    return { center, dist(center, A) }; 
} 


// returns the smallest circle possible from 2 points
// circle's center is the middle point between A and B' it's radius is half distance AB
Circle minCircleFrom2P(const Point& A, const Point& B) 
{ 
    Point C = { (A.x + B.x) / (float) 2, (A.y + B.y) / (float) 2 }; 
    return { C, dist(A, B) / (float) 2 }; 
} 
    
  
// returns minimum  circle for N <= 3 
Circle minCircleTrivial(vector<Point>& P) 
{ 
    assert(P.size() <= 3); 
    if (P.empty()) { 
        return { { 0, 0 }, 0 }; 
    } 
    else if (P.size() == 1) { 
        return { P[0], 0 }; 
    } 
    else if (P.size() == 2) { 
        return minCircleFrom2P(P[0], P[1]); 
    } 
  
    // checks if min circle can be made by only 2 points
    for (int i = 0; i < 3; i++) { 
        for (int j = i + 1; j < 3; j++) { 
  
            Circle c = minCircleFrom2P(P[i], P[j]); 
            if (isValidCircle(c, P)) 
                return c; 
        } 
    } 
    return circleFrom3P(P[0], P[1], P[2]); 
} 
  
// Returns min enclosing circle applying Welzl algorithm
Circle aux(vector<Point>& P, vector<Point> R, int n) 
{ 
    // base case
    if (n == 0 || R.size() == 3) { 
        return minCircleTrivial(R); 
    } 
  
    // random point
    int idx = rand() % n; 
    Point p = P[idx]; 
  
    swap(P[idx], P[n - 1]); 
  
    // Get the MEC circle circ from the 
    // set of points P without p 
    Circle circ = aux(P, R, n - 1); 
  
    // If circ contains p, return circ 
    if (isInCircle(circ, p)) { 
        return circ; 
    } 
  
    // Otherwise, must be on the boundary of the MEC 
    R.push_back(p); 
  
    // Return the MEC for P - {p} and R U {p} 
    return aux(P, R, n - 1); 
} 
  
Circle welzlsAlgoMEC(const vector<Point>& P) 
{ 
    vector<Point> temp = P; 
    random_shuffle(temp.begin(), temp.end()); 
    return aux(temp, {}, temp.size()); 
} 


Circle findMinCircle(Point** points, size_t size){
	vector<Point> pointsVec;
	for (int i = 0; i < size; i++) {
		pointsVec.push_back(*(points[i]));
	}
	return welzlsAlgoMEC(pointsVec);
}

