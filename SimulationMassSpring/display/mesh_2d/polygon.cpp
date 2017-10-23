#include "polygon.h"
#include <cmath>
#define POINT_SIZE 3
#define INFINITY 5000

Polygon::Polygon()
{
	finish = false;
	selected = INFINITY;
}

Polygon::~Polygon()
{
}

void Polygon::AddPoint( QPoint &point )
{
	QPointF new_point;
	new_point.setX(point.rx());
	new_point.setY(point.ry());
	points.push_back(new_point);
}

void Polygon::ChangeLastPoint( QPoint &point )
{
	QPointF new_point;
	points.pop_back();
	new_point.setX(point.rx());
	new_point.setY(point.ry());
	points.push_back(new_point);
}

void Polygon::SelectPoint( QPoint &point )
{
	int i;
	float distance;
	for( i=0; i<points.size(); i++ )
	{
		distance = sqrt(pow(float(point.rx()-points[i].rx()),2)+pow(float(point.ry()-points[i].ry()),2));
		if( distance<3*POINT_SIZE )
			selected = i;
	}
}

void Polygon::ChangeSelectedPoint( QPoint &point )
{
	if( selected<INFINITY )
	{
		points[selected].setX(point.rx());
		points[selected].setY(point.ry());
	}
}

void Polygon::Draw( QPainter &paint )
{
	int i,x,y;
	paint.setPen( QPen(Qt::blue,3) );
	for( i=1;i<points.size();i++ )
		paint.drawLine( points[i-1], points[i] );
	if( finish ) 
		paint.drawLine( points.back(), points.front() );
	paint.setPen( QPen(Qt::red,8) );
	for( i=0;i<points.size();i++ )
		paint.drawPoint( points[i] );
}
