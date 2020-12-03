#include "timeseries.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void TimeSeries::init(const string fileName)
{
    ifstream f_stream;
    f_stream.open(fileName);
    string line, parsed;

    //initialize vector of features' list
    getline(f_stream, line, '\n');
    stringstream line_sstream(line);
    while(line_sstream.good()) {
        getline(line_sstream, parsed, ',');
        this->features.push_back(parsed);
    }

    // read data and push to matching feature's vector in map
    getline(f_stream, line, '\n');
    while(!f_stream.eof()) {
        stringstream line_sstream(line);
        for(int i = 0; i < this->features.size(); i++) {
            getline(line_sstream, parsed, ',');
            this->fmap[this->features[i]].push_back(stof(parsed));
        }
        getline(f_stream, line, '\n');
    }
    f_stream.close();
}