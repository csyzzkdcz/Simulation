#include <QFileDialog>
#include <fstream>
#include <string>
#include "mesh_2d\ScanLine.h"
#include "viewwidget.h"

ViewWidget::ViewWidget( QWidget *parent )
{
	ui.setupUi(this);
	mass_spring = NULL;
	cur = NULL;
	is_designing = false;
	is_built = false;
	is_playing = false;
}

ViewWidget::~ViewWidget()
{
	delete mass_spring;
	delete cur;
}

void ViewWidget::mousePressEvent( QMouseEvent *event )
{
	if( is_designing )
	{
		if( cur==NULL && event->button()==Qt::LeftButton )
		{
			cur = new Polygon();
			cur->AddPoint(event->pos());
		}
		cur->AddPoint(event->pos());
		if( cur!=NULL && event->button()==Qt::RightButton )
		{
			cur->finish = true;
			poly.push_back(cur);
			cur = NULL;
		}
	}
}

void ViewWidget::mouseMoveEvent( QMouseEvent *event )
{
	if( is_designing && cur!=NULL )
	{
		setMouseTracking(true);
		cur->ChangeLastPoint(event->pos());
	}
}

void ViewWidget::mouseReleaseEvent( QMouseEvent *event )
{
}

void ViewWidget::paintEvent( QPaintEvent * )
{
	QPainter painter(this);
	int i,j;
	ScanLine scan;
	bool status[1000];
	if( is_designing )
	{
		painter.setPen(Qt::yellow);
		for( i=0; i<poly.size(); i++ )
			scan.Load(poly[i]);
		if( poly.size()>0 )
			for( i=0; i<1000; i++ )
			{
				for( j=0; j<1000; j++ )
					status[j] = false;
				scan.Calculate(i,status);
				for( j=0; j<1000; j++ )
					if( status[j] )
						painter.drawPoint(j,i);
			}
		for( i=0; i<poly.size(); i++ )
			poly[i]->Draw(painter);
		if( cur!=NULL )
			cur->Draw(painter);
	}
	if( is_built )
	{
		mass_spring->Paint(painter);
		if( is_playing )
			mass_spring->Update();
	}
	update();
}

void ViewWidget::OpenFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Mass_Spring(*.ms)"));
	if(filename.isEmpty())
		return;
	string file = filename.toLocal8Bit().constData();
	ifstream infile; 
	infile.open(file.data());
	string s,sx,sy;
	int i,k,id1,id2;
	QPointF new_point;
	bool fixed;

	// build mass spring system
	mass_spring = new MS_System;
	while( getline(infile,s) )
	{
		sx.clear();
		sy.clear();
		if( s[0]=='m' )
		{
			fixed = false;
			k = 2;
			if( s[1]=='f' )
			{
				fixed = true;
				k++;
			}
			while( s[k]!=' ' )
			{
				sx.push_back(s[k]);
				k++;
			}
			k++;
			while( k<s.length() )
			{
				sy.push_back(s[k]);
				k++;
			}
			new_point = QPointF( atof(sx.c_str()), atof(sy.c_str()) );
			mass_spring->InsertMass( new_point,!fixed );
		}
		else if( s[0]=='s' )
		{
			k = 2;
			while( s[k]!=' ' )
			{
				sx.push_back(s[k]);
				k++;
			}
			id1 = atoi(sx.c_str());
			k++;
			while( k<s.length() )
			{
				sy.push_back(s[k]);
				k++;
			}
			id2 = atoi(sy.c_str());
			if(id2==5)
				id2=5;
			mass_spring->InsertSpring( mass_spring->mass_list[id1-1],  mass_spring->mass_list[id2-1] );
		}
	}
	infile.close();
	is_built = true;
	update();
}

void ViewWidget::SaveFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("MS(*.ms)"));
	if ( filename.isNull() || mass_spring==NULL )
		return;
	string file = filename.toLocal8Bit().constData();
	ofstream outfile(file); 
	int i, mass_num, spring_num;
	mass_num = mass_spring->mass_list.size();
	spring_num = mass_spring->spring_list.size();

	if (outfile.is_open())
	{
		outfile << "# Mass Spring System" << endl;
		outfile << "# " << mass_num << " " << spring_num << endl;
		for ( i=0; i<mass_num; i++ )
		{
			outfile << "m";
			if( !mass_spring->mass_list[i]->mobile )
				outfile << "f";
			outfile << " ";
			outfile << mass_spring->mass_list[i]->pos.rx() << " " << mass_spring->mass_list[i]->pos.ry() << endl;
		}
		for ( i=0; i<spring_num; i++ )
		{
			outfile << "s";
			outfile << " ";
			outfile << mass_spring->spring_list[i]->start->id << " " << mass_spring->spring_list[i]->end->id << endl;
		}
		outfile.close();  
	}
}

void ViewWidget::SetDrawStatus()
{
	if( mass_spring!=NULL )
	{
		delete mass_spring;
		mass_spring = NULL;
	}
	mass_spring = new MS_System;
	is_designing = true;
}

void ViewWidget::DeleteFigure()
{
	if( mass_spring!=NULL )
	{
		delete mass_spring;
		mass_spring = NULL;
	}
	poly.clear();
	is_designing = false;
	is_built = false;
	is_playing = false;
}

void ViewWidget::Build()
{
	mass_spring->BuildFromPolygons(poly);
	is_designing = false;
	is_built = true;
}

void ViewWidget::Simulate()
{
	if( mass_spring == NULL )
		return;
	is_playing = true;
}