#pragma once
#include "polygon.h"
class ScanLine
{
private:
	vector<Polygon*>	boundaries;

public:
	ScanLine(void);
	~ScanLine(void);

public:
	void Load( Polygon *poly );
	void Calculate( int y, bool *status );
};

