
#include "SimpleAnomalyDetector.h"
#define MARGIN 1.1

using namespace std;

SimpleAnomalyDetector::SimpleAnomalyDetector(){normal_threshold=0.9;}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

Point** SimpleAnomalyDetector::toPoints(vector<float> x, vector<float> y){
	Point** ps=new Point*[x.size()];
	for(size_t i=0;i<x.size();i++){
		ps[i]=new Point(x[i],y[i]);
	}
	return ps;
}

// methods ruteuns correlation between features i and j
float calcFeaturesCorrelation(const TimeSeries &ts, int i, int j) {
	// get vector of feature i
	vector<float> v1 = ts.getFeatureVecByIdx(i);
	// get vector of feature i
	vector<float> v2 = ts.getFeatureVecByIdx(j);
	float* x = v1.data();
	float* y = v2.data();
	return abs(pearson(x, y, ts.getEntriesNum()));
}

// checks for every feature j if J-I correlation is bigger than given correlation
// (where i is the 2nd feature)
bool isMaxCorelForFeature(string feature, float correlation, vector<correlatedFeatures> cfVec) {
	for(int i = 0; i < cfVec.size(); i++) {
		if(cfVec[i].feature2 == feature && cfVec[i].corrlation >= correlation) {
			return false;
		}
	}
	return true;
}

// set threshold as max{dev(p in points, linear_reg)} -
// max deviation of all points from linerreg line
float SimpleAnomalyDetector::findThreshold(Point** parray, Line reg, TimeSeries ts) {
	float threshold = 0;
	for (int i = 0; i < ts.getEntriesNum(); i++) {
		float temp_thres = dev(*parray[i], reg);
		if (temp_thres > threshold) {
			threshold = temp_thres;
		}
	}
	return threshold;
}


// input is  timeseries and a correlation of 2 features,
// method sets linearreg line and max deviation (threshold) of all points from linearreg line
void SimpleAnomalyDetector::set_CF_threshold(const TimeSeries& ts, correlatedFeatures& correlf) {
	// set liner regression line and MEC
	// get features' vectors
	vector<float> v1 = ts.getMap()[correlf.feature1];
	vector<float> v2 = ts.getMap()[correlf.feature2];
	// points array in size of timeseries' entries (rows - 1)
	// each point represent (f1,f2) at given time
	Point** points = toPoints(v1,v2);
	// if 2 features correlation is above normal_threshold,
	// anomaly check by max deviation from linearreg Line
	// otherwise (0.5 < correl < normal_threshold),
	// anomaly can be checked by MEC
	correlf.lin_reg = linear_reg(points, ts.getEntriesNum());
	correlf.threshold = findThreshold(points, correlf.lin_reg, ts) * MARGIN; // * 110%
 	correlf.MEC = findMinCircle(points, ts.getEntriesNum());
 	correlf.MEC.radius *= MARGIN; // * 110%
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	correlatedFeatures correlf;
	// for every feature i
	for(int i = 0; i < ts.features.size(); i++) {
		// init feature i (max)correlation with any other feature to be 0
		correlf.feature1 = ts.features[i];
		correlf.corrlation = 0;
		// check correlation with feature j (j > i, upper triangular matrix)
		for (int j = i + 1; j < ts.features.size(); j++) {
			// if i correlate with j more than any previous j,
			// AND i,j correlation is above minimum threshold, update correlf
			float correlIJ = calcFeaturesCorrelation(ts, i, j); 
			if (correlIJ > correlf.corrlation && correlIJ > minimal_correl /* =0.5 */) {
				correlf.corrlation = correlIJ;
				correlf.feature2 = ts.features[j];
				set_CF_threshold(ts, correlf);
			}
		}
		// check for lower triangular matrix
		if(isMaxCorelForFeature(correlf.feature1, correlf.corrlation, cf)) {
			cf.push_back(correlf);
		}
	}
}

bool SimpleAnomalyDetector::isAnomalous(float x, float y, correlatedFeatures c) {
	return abs(y - c.lin_reg.f(x))>c.threshold;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> arVec;	// vector of AnomalyReport s
	int correlNum = this->cf.size();	// number of correlations found (above normal threshold)
	// for every row in TimeSeries
	for (int i = 0; i < ts.getEntriesNum(); i++) {
		// for every correl of 2 features in this.cf
		// check if in row i in TS 2 features of cf[j]
		// are deviated from their linearreg more then allowed (in cf[j].threshold)
		for (int j = 0; j < correlNum; j++) {
			struct correlatedFeatures temp = cf[j];
			float x = ts.getMap()[temp.feature1][i];
			float y = ts.getMap()[temp.feature2][i];
			// if at any time 2 features deviated more than allowed - report anomaly between them
			if (isAnomalous(x,y,temp)) {
				arVec.push_back(AnomalyReport(string(temp.feature1+"-"+temp.feature2), i + 1));
			}
		}
	
	}
	return arVec;
}