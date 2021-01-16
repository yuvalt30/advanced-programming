
#include "HybridAnomalyDetector.h"
using namespace std;
HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
bool HybridAnomalyDetector::isAnomalous(float x, float y, correlatedFeatures c) {
	if (abs(c.corrlation) > abs(normal_threshold)) {
		return abs(y - c.lin_reg.f(x))>c.threshold;
	}
	return !isInCircle(c.MEC, Point(x,y));
}