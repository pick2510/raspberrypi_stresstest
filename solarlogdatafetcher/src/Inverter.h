/*
 * Inverter.h
 *
 *  Created on: 27.07.2016
 *      Author: pick
 */

#ifndef INVERTER_H_
#define INVERTER_H_
#include <vector>
#include <string>
using namespace std;
namespace solarlogdatafetcher {

using std::vector;
using std::string;

class Inverter {
private:
    vector<string> modulestrings;
    int stringcount;
    string name;
    int number;
    string csvHeader;

public:
    Inverter();
    Inverter(string name, int number);
    virtual ~Inverter();
    const vector<string>& getModulestrings() const;
    const string& getName() const;
    void setName(const string& name);
    int getNumber() const;
    void setNumber(int number);
    int getStringcount() const;
    void setCsvHeader(string csvHeader);
    string getCsvHeader() const;
    void setModulestrings(vector<string> modulestrings);
    void setStringcount(int stringcount);
};

} /* namespace solarlogdatafetcher */

#endif /* INVERTER_H_ */
