/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen, Nguyen Hoang Minh
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "eventLib.h"

/// NOTE: each event will be separated by spaces, or endline character
void loadEvents(char* fName, L1List<ninjaEvent_t> &eList) {
	//TODO    
	fstream inF(fName);
	if (!inF)
	{
		cerr << "\nCannot open the file for reading!\n";
		return;
	}
	else
	{	
		string str;
		stringstream buf;
		getline(inF, str, ';');
		inF.close();
		buf << str;
		ninjaEvent n;
		while (!buf.eof())
		{
			buf >> str;
			strncpy(n.code, str.c_str(), EVENT_CODE_SIZE - 1);
			str.clear();
			eList.push_back(n);
		}
	}
	return;
}

