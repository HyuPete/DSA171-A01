/*
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen, Nguyen Hoang Minh
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 *               This file implements functions used for database management
 * =========================================================================================
 */

#include "dbLib.h"

#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

// structure for global data
struct PointerToGData
{
	L1List<ninjaEvent_t> pLE;        // copy list of events for reprint
	L1List<L1List<NinjaInfo_t>> pLL; // list of lists
	char *c;                         // sorted ninja's ID array
	double *totalDistance;           // total distance of traveling array
	time_t *totalUnmovedTime; // array of total unmoved time of each ninja

};

void    strPrintTime(char *des, time_t &t) {
	tm *pTime = localtime(&t);//gmtime(&t);
    strftime(des, 26, "%Y-%m-%d %H:%M:%S", pTime);
}

// Load database
void loadNinjaDB(char *fName, L1List<NinjaInfo_t> &db) {
	// TODO: write code to load information from file into db
	ifstream dbFile(fName);
	if (!dbFile)
	{
		db.clean();
		cerr << "\nCannot open the database file for reading!\n";
		return;
	}
	else
	{
		stringstream buf;
		string str;
		tm t;
		int in;
		getline(dbFile, str); // discard the first line
		while (!dbFile.eof())
		{
			NinjaInfo_t n;
			getline(dbFile, str, ','); // discard the 1st item on each line
			// handle time data
			if (str[0] == '\n' || str[0] == '\r' || str.empty()) continue;
			getline(dbFile, str, ','); // "Report time"
			buf << str;
			buf >> in;
			t.tm_mon = in - 1; // month [0-11]
			buf.ignore(); // discard '/'
			buf >> t.tm_mday; // day
			buf.ignore(); // discard '/'
			buf >> in;
			t.tm_year = in - 1900; // number of years from 1900
			buf.ignore(); // discard ' '
			buf >> t.tm_hour;
			buf.ignore(); // discard ':'
			buf >> t.tm_min;
			buf.ignore(); // discard ':'
			buf >> t.tm_sec;
			n.timestamp = mktime(&t); // convert from tm to time_t
			buf.str("");
			buf.clear();
			// handle ninja id
			getline(dbFile, str, ','); // Ninja tag
			strcpy(n.id, str.data());
			// handle longitude
			getline(dbFile, str, ','); // Longitude
			buf << str;
			buf >> n.longitude;
			buf.str("");
			buf.clear();
			// handle latitude
			getline(dbFile, str, ','); // Latitude
			buf << str;
			buf >> n.latitude;
			buf.str("");
			buf.clear();
			getline(dbFile, str); // read the rest of the line
			db.push_back(n);
			//printNinjaInfo(n);
		}
		dbFile.close();
	}
	return;
}

// Not use
bool parseNinjaInfo(char* pBuf, NinjaInfo_t& nInfo) {
    // TODO: write code to parse information from a string buffer, ignore if you don't use it
    return true;
}

// forward declaration of some function use for sorting a list
void traverse(L1List<L1List<NinjaInfo>> &L, NinjaInfo &a);
void traverse(L1List<NinjaInfo> &lN, L1List<L1List<NinjaInfo>> &lL);
void printListNinja(L1List<NinjaInfo> &l);
// function process called by main to execute the whole program
void process(L1List<ninjaEvent_t> &eventList, L1List<NinjaInfo_t> &bList) {
    void*   pGData = NULL;
	if (initNinjaGlobalData(&pGData))
	{
		// copy list eventList
		eventList.traverse(static_cast<PointerToGData*>(pGData)->pLE);
		// create a list of lists of each ninja's data
		traverse(bList, static_cast<PointerToGData*>(pGData)->pLL);
		static_cast<PointerToGData*>(pGData)->pLL.traverse(printListNinja);
	}
    while (!eventList.isEmpty()) {
        if(!processEvent(eventList[0], bList, pGData))
            cout << eventList[0].code << " is an invalid event\n";
        eventList.removeHead();
    }

    releaseNinjaGlobalData(pGData);
}

bool initNinjaGlobalData(void* *pGData) {
    /// TODO: You should define this function if you would like to use some extra data
    /// the data should be allocated and pass the address into pGData
	*pGData = new PointerToGData();
    return true;
}

void releaseNinjaGlobalData(void *pGData) {
    /// TODO: You should define this function if you allocated extra data at initialization stage
    /// The data pointed by pGData should be released
	delete static_cast<PointerToGData*>(pGData);
	return;
}

void printNinjaInfo(NinjaInfo_t& b) {
    printf("%s: (%0.5f, %0.5f), %s\n", b.id, b.longitude, b.latitude, ctime(&b.timestamp));
}

/// This function converts decimal degrees to radians
double deg2rad(double deg) {
    return (deg * pi / 180);
}

///  This function converts radians to decimal degrees
double rad2deg(double rad) {
    return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}