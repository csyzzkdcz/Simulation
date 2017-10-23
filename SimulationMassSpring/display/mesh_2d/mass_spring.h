/*
	Data Structure: Adjacant linking list ( like Oriented Graph )
	Different from the Half-Edge structre in 3D Mesh!!
	Use the Mass-Spring model to simulate the motion of an elastic system
*/
#pragma once
#include <QPainter>
#include <vector>
#include "polygon.h"
#include "ODEsolver.h"

#define POINT_SIZE 6
#define LINE_WIDTH 3
#define INIT_MASS 3
#define INIT_STIF 20
#define INIT_DAMP 0.25
#define GRAVITY 10
#define WITH_GRAV true
#define INFINITY 100000

using namespace std;

class Mass;
class Half_Spring;
class MS_System;

class Mass
{
public:
	QPointF				pos;				// position of the point
	QPointF				velo;				// velosity of the point
	float				mass;				// mass of the point
	bool				mobile;				// whether the point is able to move
	Half_Spring			*first;				// first spring started from this point
	int					id;					// index of the point

public:
	Mass( QPointF point, float m, bool bv );
	~Mass();

public:
	QPointF Cal_Acclrt( bool grav );
};

class Half_Spring
{
public:
	Mass				*start, *end;			// masses connected to the spring
	float				org_len;				// origin length of the spring
	float				stif;					// stiffness of the spring
	float				damp;					// damping of the spring
	Half_Spring			*next;					// next spring (same start point)
	Half_Spring			*pair;					// pair spring (start point as end point)

public:
	Half_Spring( Mass *p1, Mass *p2, float k, float d );
	~Half_Spring();

public:
	QPointF Cal_Tense();
};

class MS_System
{
public:
	vector<Mass*>		mass_list;				// list of masses
	vector<Half_Spring*>	spring_list;		// list of half-springs
	ODEsolver			solver;					// solver for ordinary differential equation
	int					selected;				// index of selected point

public:
	MS_System();
	~MS_System();

public:
	Mass* Select( QPointF pos );				// select point in the system
	bool Connected( Mass *m1, Mass *m2 );		// whether two points are connected
	void InsertMass( QPointF v, bool bv );		// insert new mass to the system
	void InsertSpring( Mass *m1, Mass *m2 );	// insert new spring to the system
	void BuildFromPolygons( vector<Polygon*> poly );		// build mass-spring system from polygons
	void Update();								// update status of masses and springs
	void Paint( QPainter &paint );				// paint all masses and springs on the screen
};
