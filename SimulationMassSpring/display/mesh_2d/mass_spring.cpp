#include <cmath>
#include <Eigen>
#include <iostream>
#include "mass_spring.h"
#include "ScanLine.h"

Mass::Mass( QPointF point, float m, bool bv )
{
	pos = point;
	velo.setX(0);
	velo.setY(0);
	mass = m;
	mobile = bv;
	first = NULL;
	id = INFINITY;
}

Mass::~Mass()
{
}

QPointF Mass::Cal_Acclrt( bool grav )
{
	Half_Spring *curs;
	QPointF force, acc;
	curs = first;
	force = QPointF(0,0);
	do{
		force += curs->Cal_Tense() + curs->damp * (curs->end->velo - velo);
		curs = curs->next;
	} while(curs!=first);
	acc = force/mass;
	if(grav)
		acc += QPointF(0,GRAVITY);
	return acc;
}

Half_Spring::Half_Spring( Mass *p1, Mass *p2, float k, float d ) 
{
	start = p1;
	end = p2;
	org_len = sqrt( pow((p2->pos.rx()-p1->pos.rx()),2) + pow((p2->pos.ry()-p1->pos.ry()),2) );
	stif = k;
	damp = d;
	next = NULL;
	pair = NULL;
}

Half_Spring::~Half_Spring()
{
}

QPointF Half_Spring::Cal_Tense()
{
	float dis;
	QPointF dir;
	dis = sqrt( pow(end->pos.rx()-start->pos.rx(),2) + pow(end->pos.ry()-start->pos.ry(),2) );
	dir = (end->pos-start->pos) / dis;
	return stif*(dis-org_len)*dir;
}

MS_System::MS_System()
{
	selected = INFINITY;
}

MS_System::~MS_System()
{
}

Mass* MS_System::Select( QPointF pos )
{
	int i;
	float dis;
	for( i=0; i<mass_list.size(); i++ )
	{
		dis = sqrt( pow( mass_list[i]->pos.rx()-pos.rx(), 2 ) + pow( mass_list[i]->pos.ry()-pos.ry(), 2 ) );
		if( dis < 3*POINT_SIZE )
		{
			selected = i;
			return mass_list[i];
		}
	}
	return NULL;
}

bool MS_System::Connected( Mass *m1, Mass *m2 )
{
	Half_Spring *spring;
	spring = m1->first;
	if( spring==NULL )
		return false;
	do{
		if( spring->end==m2 )
			return true;
		spring = spring->next;
	}while( spring!=m1->first );
	return false;
}

void MS_System::InsertMass( QPointF v, bool bv )
{
	Mass *newm;
	newm = new Mass( v, INIT_MASS, bv );
	mass_list.push_back(newm);
	newm->id = mass_list.size();
}

void MS_System::InsertSpring( Mass *m1, Mass *m2 )
{
	if( Connected(m1,m2)||m1==m2 )
		return;
	Half_Spring *news1, *news2, *tmp;
	// insert a new half-spring into m1's spring list
	news1 = new Half_Spring( m1, m2, INIT_STIF, INIT_DAMP );
	if( m1->first==NULL )
	{
		m1->first = news1;
		news1->next = news1;
	}
	else
	{
		tmp = m1->first;
		while( tmp->next!=m1->first )
			tmp = tmp->next;
		tmp->next = news1;
		news1->next = m1->first;
	}
	spring_list.push_back(news1);
	// insert a new half-spring into m2's spring list
	news2 = new Half_Spring( m2, m1, INIT_STIF, INIT_DAMP );
	if( m2->first==NULL )
	{
		m2->first = news2;
		news2->next = news2;
	}
	else
	{
		tmp = m2->first;
		while( tmp->next!=m2->first )
			tmp = tmp->next;
		tmp->next = news2;
		news2->next = m2->first;
	}
	spring_list.push_back(news2);
	// relationship between the two half-springs
	news1->pair = news2;
	news2->pair = news1;
}

void MS_System::BuildFromPolygons( vector<Polygon*> poly )
{

}

void MS_System::Update()
{
	int i,j;
	float test;
	VectorXd acc(mass_list.size());
	VectorXd vel(mass_list.size());
	VectorXd pos(mass_list.size());
	for( i=0; i<mass_list.size(); i++ )
		if( mass_list[i]->mobile )
		{
			acc(i) = mass_list[i]->Cal_Acclrt( WITH_GRAV ).rx();
			vel(i) = mass_list[i]->velo.rx();
			pos(i) = mass_list[i]->pos.rx();
		}
	//solver.Euler(acc,vel,pos);
	solver.Verlet(acc,vel,pos);
	for( i=0; i<mass_list.size(); i++ )
		if( mass_list[i]->mobile )
		{
			mass_list[i]->velo.setX(vel(i));
			mass_list[i]->pos.setX(pos(i));
		}
	for( i=0; i<mass_list.size(); i++ )
		if( mass_list[i]->mobile )
		{
			acc(i) = mass_list[i]->Cal_Acclrt( WITH_GRAV ).ry();
			vel(i) = mass_list[i]->velo.ry();
			pos(i) = mass_list[i]->pos.ry();
		}
	solver.Euler(acc,vel,pos);
	for( i=0; i<mass_list.size(); i++ )
		if( mass_list[i]->mobile )
		{
			mass_list[i]->velo.setY(vel(i));
			mass_list[i]->pos.setY(pos(i));
		}
}


void MS_System::Paint(QPainter &paint)
{
	int i;
	Mass *curm;
	Half_Spring *curs;
	for( i=0;i<spring_list.size();i++ )
	{
		curs = spring_list[i];
		paint.setPen( QPen( Qt::blue, LINE_WIDTH, Qt::SolidLine, Qt::RoundCap ) );
		paint.drawLine( curs->start->pos, curs->end->pos );
	}
	for( i=0;i<mass_list.size();i++ )
	{
		curm = mass_list[i];
		if( i==selected )
		{
			paint.setPen( QPen( Qt::yellow, 2*POINT_SIZE,Qt::SolidLine, Qt::RoundCap ) );
			paint.drawPoint( curm->pos );
		}
		paint.setPen( QPen( Qt::red, POINT_SIZE, Qt::SolidLine, Qt::RoundCap ) );
		paint.drawPoint( curm->pos );
	}
}