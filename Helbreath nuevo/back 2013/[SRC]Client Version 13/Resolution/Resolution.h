#pragma once
#include <fstream> 
using namespace std;

class cResolution
{
public:
	cResolution();
	~cResolution();
	short IsResolution();
//	bool Is800x600(const std::string& name);
	bool Is1024x768(const std::string& name);
	int GetWidth();
	int GetHeight();
	short sResolution;
};

