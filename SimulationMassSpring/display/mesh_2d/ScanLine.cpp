#include "ScanLine.h"

ScanLine::ScanLine(void)
{
}

ScanLine::~ScanLine(void)
{
}

void ScanLine::Load( Polygon *poly )
{
	boundaries.push_back(poly);
}

void ScanLine::Calculate( int y, bool *status )
{
	int k,i,j,n,x;
	float s;
	vector<int> intersections;
	bool tmp;
	for( k=0; k<boundaries.size(); k++ )
	{
		n = boundaries[k]->points.size();
		for( i=0; i<n; i++ )
			if( boundaries[k]->points[i].ry() < y )
				if( boundaries[k]->points[(i+1)%n].ry() > y )
				{
					s = (boundaries[k]->points[(i+1)%n].ry()-y) / (float)(boundaries[k]->points[(i+1)%n].ry()-boundaries[k]->points[i].ry());
					x = s*boundaries[k]->points[i].rx() + (1-s)*boundaries[k]->points[(i+1)%n].rx();
					intersections.push_back(x);
				}
				else
				{
					j=1;
					while( boundaries[k]->points[(i+j)%n].ry() == y )
						j++;
					if( boundaries[k]->points[(i+j)%n].ry() > y )
						intersections.push_back(boundaries[k]->points[(i+j-1)%n].rx());
				}
			else if( boundaries[k]->points[i].ry() > y )
				if( boundaries[k]->points[(i+1)%n].ry() < y )
				{
					s = (boundaries[k]->points[(i+1)%n].ry()-y) / (float)(boundaries[k]->points[(i+1)%n].ry()-boundaries[k]->points[i].ry());
					x = s*boundaries[k]->points[i].rx() + (1-s)*boundaries[k]->points[(i+1)%n].rx();
					intersections.push_back(x);
				}
				else
				{
					j=1;
					while( boundaries[k]->points[(i+j)%n].ry() == y )
						j++;
					if( boundaries[k]->points[(i+j)%n].ry() < y )
						intersections.push_back(boundaries[k]->points[(i+j-1)%n].rx());
				}
	}
	sort(intersections.begin(),intersections.end());
	x = 0;
	i = 0;
	tmp = false;
	while( i<intersections.size() )
	{
		while( x<intersections[i] )
		{
			status[x] = tmp;
			x++;
		}
		tmp = !tmp;
		i++;
	}
}
