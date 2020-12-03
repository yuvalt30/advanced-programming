#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include<map>
#include<vector>
#include<iostream>

using namespace std;

class TimeSeries{
	
public:
	void init(const string fileName);
	map<string, vector<float>> fmap;
	vector<string> features;
	TimeSeries(const char* CSVfileName) {init(CSVfileName);	}
	int getFeaturesNum() const {return this->features.size();}
	int getEntriesNum() const {return getMap()[this->features[0]].size();}
	map<string, vector<float>> getMap() const { return fmap;}
	vector<string> getFeatures() const {return features;}
	vector<float> getFeatureVecByIdx(int i) const {return getMap()[getFeatures()[i]];}
};
#endif /* TIMESERIES_H_ */
