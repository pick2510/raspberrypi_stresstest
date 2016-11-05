/*
 * Plant.cpp
 *
 *  Created on: 27.07.2016
 *      Author: pick
 */

#include "Plant.h"
#include "curl/curl.h"
#include<locale>
#include "libstrebel.h"
#include "boost/date_time.hpp"
using namespace std;


namespace solarlogdatafetcher {

const locale URL_DATE_FMT(std::locale::classic(),
        new boost::gregorian::date_facet("%y%m%d"));

Plant::Plant() {
    // TODO Auto-generated constructor stub
    this->name = "Test";
    this->inverterCount = 0;
    this->serialnumber = 1234234;
    this->password = "Testpw";
 

}

Plant::Plant(string name, long serialnumber, string password, int invCount) {
    this->name = name;
    this->serialnumber = serialnumber;
    this->password = password;
    this->inverterCount = invCount;
}

Plant::Plant(const Plant& other) {
    std::cout << "Copy Constructor Called!" << std::endl;
}

const vector<Inverter>& Plant::getInverters() const {
    return inverters;
}

const string& Plant::getName() const {
    return name;
}

void Plant::setName(const string& name) {
    this->name = name;
}

const string& Plant::getPassword() const {
    return password;
}

void Plant::setPassword(const string& password) {
    this->password = password;
}

long Plant::getSerialnumber() const {
    return serialnumber;
}

int Plant::getInverterCount() const {
    return inverterCount;
}

void Plant::setInverterCount(int inverterCount) {
    this->inverterCount = inverterCount;
}

void Plant::setHttpResponse(string httpResponse) {
    this->httpResponse = httpResponse;
}

string Plant::getHttpResponse() const {
    return httpResponse;
}

void Plant::setInverters(vector<Inverter> inverters) {
    this->inverters = inverters;
}

void Plant::setSerialnumber(long serialnumber) {
    this->serialnumber = serialnumber;
}

Plant::~Plant() {
    // TODO Auto-generated destructor stub
}

void Plant::addInverter(Inverter inv) {
    this->inverters.push_back(inv);
}



void Plant::fetchData( boost::gregorian::date &d_startdate, boost::gregorian::date &d_enddate){
    string s_serialnumber, s_password;
    stringstream ss_serialnumber;
    ss_serialnumber<<this->serialnumber;
    s_serialnumber = ss_serialnumber.str();
    s_password = this->password;
    boost::gregorian::date d_nowdate = d_startdate;
    boost::gregorian::date_duration dd_daycount = ( d_enddate - d_nowdate);
    cout << dd_daycount.days() << endl;
    while (d_nowdate < d_enddate){
        stringstream ss_file;
        stringstream ss_url;
        ss_file.imbue(URL_DATE_FMT);
        ss_file << "min" << d_nowdate << ".js";
        ss_url << "http://clevergie.solarlog-web.ch/api?access=iphone&file=" << ss_file.str() << "&sn=" 
                << s_serialnumber << "&pwd=" << s_password;
        cout << ss_url.str() << endl;
        CURL *curl;
        CURLcode c_res;
        string s_readBuffer;
        s_readBuffer="";
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, ss_url.str().c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s_readBuffer);
            c_res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        this->content.push_back(s_readBuffer);
        d_nowdate += boost::gregorian::days(1);
        
    }
}

void Plant::generateCSVHeader() {
    for (int i = 0; i<this->inverters.size(); i++){
        stringstream ss_header;
        ss_header << "Date/Time;PAC Inv " << i+1 << "[W];";
        for (int j=0; j < this->inverters[i].getStringcount(); j++) {
            ss_header << "PDC Inv " << i + 1 << " String " << j + 1 << " [W];";
        }
        ss_header << "Tagesertrag Inv $inv [Wh]";
        for (int j=0; j < this->inverters[i].getStringcount(); j++) {
            ss_header<< ";UDC Inv "<< i + 1 << " String " << j + 1 << " [V]";
        }
        ss_header << "\n";
        cout << ss_header.str() << endl;
        this->inverters[i].setCsvHeader(ss_header.str());
        
    }
}

void Plant::setContent(vector<string> content) {
    this->content = content;
}

vector<string> Plant::getContent() const {
    return content;
}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}
} /* namespace solarlogdatafetcher */
