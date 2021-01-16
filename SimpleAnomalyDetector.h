

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include "timeseries.h"
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;			// correlation between 2 features
	Line lin_reg;
	Circle MEC;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	float minimal_correl = 0.5;
protected:
	vector<correlatedFeatures> cf;
	float normal_threshold;
	virtual void setCorrelationThreshold(float threshold){
		this->normal_threshold = threshold;
	}
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	void set_CF_threshold(const TimeSeries& ts, correlatedFeatures& correlf);
	virtual vector<correlatedFeatures> getNormalModel(){return cf;}
	virtual float findThreshold(Point** parray, Line reg, TimeSeries ts);
	Point** toPoints(vector<float> x, vector<float> y);
	virtual bool isAnomalous(float x, float y, correlatedFeatures c); 
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
