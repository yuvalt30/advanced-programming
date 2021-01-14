

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
	vector<correlatedFeatures> cf;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts) {
		SimpleAnomalyDetector::learnNormal(ts);
	}
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	bool isAnomalous(float x, float y, correlatedFeatures c) {
	return !isInCircle(Circle(c.MEC_center,c.threshold), Point(x,y));
	}
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
