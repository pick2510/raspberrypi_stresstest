//============================================================================
// Name        : solarlogdatafetcher.cpp
// Author      : Dominik Strebel
// Version     :
// Copyright   : Your copyright notice
// Description : 
//============================================================================

#define MAXLINES 30

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include "md5.h"
#include <locale>
#include <codecvt>
#include <time.h>
#include "libstrebel.h"
#include "boost/date_time.hpp"
#include "Plant.h"
#include "Inverter.h"
#include "curl/curl.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

const locale OUTPUT_FMT(std::locale::classic(),
        new boost::gregorian::date_facet("%d-%m-%Y"));

const locale URL_DATE_FMT(std::locale::classic(),
        new boost::gregorian::date_facet("%y%m%d"));

const bool DEBUG = true;

void writeExampleconfig() {
    stringstream ss;
    string config;
    ptime now = second_clock::local_time();
    date today = now.date();
    date enddate = today - days(25);
    date startdate = enddate - days(90);
    ss.imbue(OUTPUT_FMT);
    ss << "Startdate=" << startdate << "\n" <<
            "Enddate=" << enddate << "\n" <<
            "Serialnumber=2312342\n" <<
            "Password=sddfsf\n";
    config = ss.str();
    ofstream write("solarlogfetcher.conf");
    if (write.is_open()) {
        write << config;
    } else {
        cout << "Error, couldn't write file" << endl;
    }
    cout << "Please edit \"solarlogfetcher.conf\" for configuring the system" << endl;


}

void readConfigfile(map<string, string> *plant_config) {
    ifstream config("solarlogfetcher.conf");
    if (!config.is_open()) {
        cout << "Error, can't open config!" << endl;
        exit(1);
    }
    stringstream buffer;
    buffer << config.rdbuf();
    string line;
    int counter = 0;
    while (std::getline(buffer, line) && counter < MAXLINES) {
        line = strebel::trim(line);
        if (line.front() == '#' || line.front() == ';') {
            continue;
        }
        if (DEBUG == true) {
            cout << line << endl;
        }
        stringstream is_line(line);
        string key;
        if (std::getline(is_line, key, '=')) {
            if (key != "Serialnumber" && key != "Password" && key != "Startdate" &&
                    key != "Enddate") {
                if (DEBUG == true) {
                    cout << "ERROR, unknown key" << endl;
                }
                continue;
            }
            string value;
            if (getline(is_line, value)) {
                if (plant_config->find(key) == plant_config->end()) {
                    plant_config->insert(pair<string, string>(key, value));
                } else {
                    cout << "Error " << key << ": " << value << " exists already" << endl;
                }
            }
        }
        if (plant_config->find("Serialnumber") != plant_config->end() &&
                plant_config->find("Password") != plant_config->end() &&
                plant_config->find("Startdate") != plant_config->end() &&
                plant_config->find("Enddate") != plant_config->end()) {
            break;
        }
        counter++;
    }
}

void parseInverters(solarlogdatafetcher::Plant &plant) {
    cout << "==============================================================" << endl;
    cout << "Parsing Inverters" << endl;
    cout << "==============================================================" << endl;
    string s_response = plant.getHttpResponse();
    unsigned int i_stringcount = 0;
    for (int i = 0; i < plant.getInverterCount(); i++) {
        stringstream ss_invregex;
        string s_invname;
        ss_invregex << "WRInfo\\[" << i << "\\]=new\\sArray\\(\"([\\w\\s-]*)\"";
        regex r_invsearch(ss_invregex.str());
        smatch m_invresult;
        regex_search(s_response, m_invresult, r_invsearch);
        if (m_invresult.empty()) {
            cout << endl;
            cout << "==============================================================" << endl;
            cout << "ERROR: Empty Match. No WR count found" << endl;
            cout << "==============================================================" << endl;
            exit(1);
        }
        s_invname = m_invresult[1];
        if (DEBUG == true) {
            cout << s_invname << endl;
        }
        ss_invregex.str("");

        string s_invstringnames;
        smatch m_invnameresult;
        ss_invregex << "WRInfo\\[" << i << "\\]\\[6\\]=new\\sArray\\(\"([\\s\\w,.\\-\"]*)";
        regex r_invstringname(ss_invregex.str());
        regex_search(s_response, m_invnameresult, r_invstringname);
        if (m_invnameresult.empty()) {
            cout << endl;
            cout << "==============================================================" << endl;
            cout << "ERROR: Empty Match. No WR count found" << endl;
            cout << "==============================================================" << endl;
            exit(1);
        }
        s_invstringnames = m_invnameresult[1];
        char c_rem[] = "\"";
        strebel::removeCharsFromString(s_invstringnames, c_rem);
        char c_tok = ',';
        vector<string> v_stringnames = strebel::splitString(s_invstringnames, c_tok);
        if (DEBUG == true) {
            cout << s_invstringnames << endl;
        }
        solarlogdatafetcher::Inverter inv = solarlogdatafetcher::Inverter(s_invname, i + 1);
        inv.setModulestrings(v_stringnames);
        inv.setStringcount(v_stringnames.size());
        i_stringcount += v_stringnames.size();
        plant.addInverter(inv);

    }
    cout << "==============================================================" << endl;
    cout << "Got " << i_stringcount << " String(s) at " << plant.getInverterCount() << " Inverter(s)" << endl;
    cout << "==============================================================" << endl;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

solarlogdatafetcher::Plant parseBasevars(map<string, string> &plant_config) {
    string s_serialnumber;
    s_serialnumber = plant_config["Serialnumber"];
    string s_password;
    s_password = plant_config["Password"];
    MD5 md5(s_password);
    s_password = md5.hexdigest();
    plant_config["Password"] = s_password;
    stringstream ss_url;
    ss_url << "http://clevergie.solarlog-web.ch/api?access=iphone&file=base_vars.js&sn=" << s_serialnumber
            << "&pwd=" << s_password;
    string s_BaseURL(ss_url.str());
    if (DEBUG == true) {
        cout << s_BaseURL << endl;
    }
    CURL *curl;
    CURLcode c_res;
    string s_readBuffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, s_BaseURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s_readBuffer);
        c_res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    char c_buffer[s_readBuffer.length()];
    copy(s_readBuffer.begin(), s_readBuffer.end(), c_buffer);
    size_t test = strebel::iso8859_1_to_utf8(c_buffer, s_readBuffer.length()*1.5);
    if (test != 0) {
        cout << "ERROR, couldn't allocate enough space for conversion, need " << test << endl;
        exit(1);
    }
    string s_response;
    s_response.assign(c_buffer);
    if (DEBUG == true) {
        cout << s_response << endl;
    }

    regex plantName("var HPTitel=\"(.*)\"");
    smatch m_result;
    regex_search(s_response, m_result, plantName);
    if (m_result.empty()) {
        cout << endl;
        cout << "==============================================================" << endl;
        cout << "ERROR: Empty Match. No PV Name found" << endl;
        cout << "==============================================================" << endl;
        exit(1);
    }
    string s_plantName = m_result[1];
    cout << endl;
    cout << "==============================================================" << endl;
    cout << "Ok, Working on " << s_plantName << ": " << endl;
    cout << "==============================================================" << endl;
    regex re_wrcount("var\\sWRInfo=new\\sArray\\((.*)\\)");
    smatch m_inv_count;
    regex_search(s_response, m_inv_count, re_wrcount);
    if (m_inv_count.empty()) {
        cout << endl;
        cout << "==============================================================" << endl;
        cout << "ERROR: Empty Match. No WR count found" << endl;
        cout << "==============================================================" << endl;
        exit(1);
    }
    string s_invnumber = m_inv_count[1];
    cout << "==============================================================" << endl;
    cout << "Ok, found " << s_invnumber << " Inverters" << endl;
    cout << "==============================================================" << endl;
    solarlogdatafetcher::Plant plant = solarlogdatafetcher::Plant(s_plantName, stol(s_serialnumber), s_password,
            stoi(s_invnumber));
    plant.setHttpResponse(s_response);
    return plant;
}

int main(int argc, char *argv[]) {
    map<string, string> plant_config;
    if (argc == 1 || argc > 2 || strcmp(argv[1], "-x") != 0) {
        cout << "=============================================================" << endl;
        cout << "Welcome to the Solarlog Data Fetcher." << endl;
        cout << "App Access must be activated to work" << endl;
        cout << "Please configure settings in the Configfile" << endl;
        cout << "\"solarlogfetcher.conf\" " << "in your homedir" << endl;
        cout << "Execute Solarlog Data Fetcher by invoking -x option" << endl;
        cout << "==============================================================" << endl;
        return 1;
    }
    cout << "==============================================================" << endl;
    cout << "Try to open config" << endl;
    cout << "==============================================================" << endl;

    if (!ifstream("solarlogfetcher.conf")) {
        cout << "==============================================================" << endl;
        cout << "Configfile doesn't exists" << endl;
        cout << "Writing example config......" << endl;
        cout << "==============================================================" << endl;
        writeExampleconfig();
        return 1;
    }
    readConfigfile(&plant_config);
    cout << "==============================================================" << endl;
    cout << "Configfile read" << endl;
    cout << "Parse Dates......." << endl;
    cout << "==============================================================" << endl;
    cout << endl;
    tm t_startdate;
    tm t_enddate;
    strptime(plant_config["Startdate"].c_str(), "%d-%m-%Y", &t_startdate);
    strptime(plant_config["Enddate"].c_str(), "%d-%m-%Y", &t_enddate);
    date d_startdate;
    date d_enddate;
    try {
        d_startdate = date_from_tm(t_startdate);
    } catch (...) {
        cout << "==============================================================" << endl;
        cout << "ERROR, wrong Dateformat, use %d-%m-%Y" << endl;
        cout << "==============================================================" << endl;
        exit(1);
    }
    try {
        d_enddate = date_from_tm(t_enddate);
    } catch (...) {
        cout << "==============================================================" << endl;
        cout << "ERROR, wrong Dateformat, use %d-%m-%Y" << endl;
        cout << "==============================================================" << endl;
        exit(1);
    }
    stringstream ss;
    ss.imbue(OUTPUT_FMT);
    ss << d_startdate << " until " << d_enddate;
    cout << "==============================================================" << endl;
    cout << "Ok, I try to read data from: " << ss.str() << endl;
    cout << "==============================================================" << endl;
    solarlogdatafetcher::Plant plant = solarlogdatafetcher::Plant();
    plant = parseBasevars(plant_config);
    cout << plant.getName() << endl;
    parseInverters(plant);
    plant.generateCSVHeader();
    plant.fetchData(d_startdate,d_enddate);
    return 0;
}
