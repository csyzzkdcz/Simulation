#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <qevent.h>
#include <qpainter.h>
#include "ui_viewwidget.h"
#include "mesh_2d\polygon.h"
#include "mesh_2d\mass_spring.h"

class ViewWidget : public QWidget
{
	Q_OBJECT

private:
	bool		is_designing;			// designing the system
	bool		is_built;				// the system has been built
	bool		is_playing;				// simulating the system
	MS_System	*mass_spring;			// mass-spring system
	std::vector<Polygon*>		poly;
	Polygon*	cur;

public:
	ViewWidget(QWidget *parent = 0);
	~ViewWidget();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *);

public slots:
	void OpenFile();
	void SaveFile();
	void SetDrawStatus();
	void DeleteFigure();
	void Build();
	void Simulate();

private:
	Ui::ViewWidget ui;
};

#endif // VIEWWIDGET_H