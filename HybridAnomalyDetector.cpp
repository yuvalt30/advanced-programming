
#include "HybridAnomalyDetector.h"
using namespace std;
HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
	vector<AnomalyReport> hybridARvec;
	int correlNum = cf.size();	// number of correlations found (above normal threshold)
	// for every row in TimeSeries
	for (int i = 0; i < ts.getEntriesNum(); i++) {
		// for every correl of 2 features in this.cf
		// check if in row i in TS 2 features of cf[j]
		// are deviated from their linearreg more then allowed (in cf[j].threshold)
		for (int j = 0; j < correlNum; j++) {
			struct correlatedFeatures temp = this->cf[j];
			float x = ts.getMap()[temp.feature1][i];
			float y = ts.getMap()[temp.feature2][i];
			// if 2 features deviated more than allowed - report anomaly between them in i+1 timestep
			if ((abs(y - temp.lin_reg.f(x)) > normal_threshold) || (!isInCircle(Circle(temp.MEC_center,temp.threshold), Point(x,y)))) {
				hybridARvec.push_back(AnomalyReport(string(temp.feature1+"-"+temp.feature2), i + 1));
			}
		}
	}
	return hybridARvec;
}


