

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
	vector<correlatedFeatures> cf;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

	virtual bool isAnomalous(float x, float y, correlatedFeatures c);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
