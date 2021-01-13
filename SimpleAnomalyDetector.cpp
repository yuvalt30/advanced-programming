
#include "SimpleAnomalyDetector.h"
#define NORMAL_THRESHOLD 0.9
#define MARGIN 1.1

using namespace std;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

float calcFeaturesCorrelation(const TimeSeries &ts, int i, int j) {
	vector<float> v1 = ts.getFeatureVecByIdx(i);
	vector<float> v2 = ts.getFeatureVecByIdx(j);
	float* x = v1.data();
	float* y = v2.data();
	return abs(pearson(x, y, ts.getEntriesNum()));
}

bool isMaxCorelForFeature(string feature, float correlation, vector<correlatedFeatures> cfVec) {
	for(int i = 0; i < cfVec.size(); i++) {
		if(cfVec[i].feature2 == feature && cfVec[i].corrlation >= correlation) {
			return false;
		}
	}
	return true;
}

float set_liner_reg_and_threshold(const TimeSeries& ts, correlatedFeatures& correlf) {
	// set liner regression line
	Point* parray[ts.getEntriesNum()];
	vector<float> v1 = ts.getMap()[correlf.feature1];
	vector<float> v2 = ts.getMap()[correlf.feature2];
	for (int i = 0; i < ts.getEntriesNum(); i++) {
		parray[i] = new Point(v1[i], v2[i]);
	}
	Line reg = linear_reg(parray, ts.getEntriesNum());
	correlf.lin_reg = reg;

	// set threshold as max{dev(p in points, linear_reg)}
	float threshold = 0;
	for (int i = 0; i < ts.getEntriesNum(); i++) {
		float temp_thres = dev(*parray[i], reg);
		if (temp_thres > threshold) {
			threshold = temp_thres;
		}
		delete(parray[i]);
	}
	return threshold *= MARGIN;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	correlatedFeatures correlf;
	for(int i = 0; i < ts.features.size(); i++) {
		correlf.feature1 = ts.features[i];
		correlf.corrlation = 0;
		for (int j = i + 1; j < ts.features.size(); j++) {
			if (calcFeaturesCorrelation(ts, i, j) > correlf.corrlation 
				&& calcFeaturesCorrelation(ts, i, j) > NORMAL_THRESHOLD) {
				correlf.corrlation = calcFeaturesCorrelation(ts, i, j);
				correlf.feature2 = ts.features[j];
				correlf.threshold = set_liner_reg_and_threshold(ts, correlf);
			}
		}
		if(isMaxCorelForFeature(correlf.feature1, correlf.corrlation, this->cf)) {
			this->cf.push_back(correlf);
		}
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> arVec;
	int correlNum = this->cf.size();
	for (int i = 0; i < ts.getEntriesNum(); i++) {
		for (int j = 0; j < correlNum; j++) {
			struct correlatedFeatures temp = this->cf[j];
			float x = ts.getMap()[temp.feature1][i];
			float y = ts.getMap()[temp.feature2][i];
			Point p(x,y);
			if (dev(p, temp.lin_reg) > temp.threshold) {
				arVec.push_back(AnomalyReport(string(temp.feature1+"-"+temp.feature2), i + 1));
			}
		}
	
	}
	return arVec;
}