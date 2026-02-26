#include "Resolution.h"

#define C800x600 1
#define C640x480 2
#define C1024x768 3

cResolution::cResolution()
{

}


cResolution::~cResolution()
{
}

short  cResolution::IsResolution()
{
	return sResolution;
}

/*bool cResolution::Is800x600(const string& name) {
	ifstream f(name.c_str());
	return f.good();
}*/
//Agregado Lalo
bool cResolution::Is1024x768(const string& name) {
	ifstream f(name.c_str());
	return f.good();
}

int cResolution::GetWidth()
{
	int result = 640;
	if (IsResolution() == C800x600)
	{
		result = 800;
	}
	else if (IsResolution() == C1024x768)
	{
		result = 600;
	}
}

int cResolution::GetHeight()
{
	int result = 480;
	if (IsResolution() == C800x600)
	{
		result = 600;
	}
	else if (IsResolution() == C1024x768)
	{
		result = 768;
	}
}