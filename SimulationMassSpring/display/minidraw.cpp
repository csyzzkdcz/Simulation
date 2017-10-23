#include <QtWidgets/QLayout>
#include "minidraw.h"

MiniDraw::MiniDraw(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view_widget_ = new ViewWidget();
	setCentralWidget(view_widget_);
	setGeometry(300, 150, 700, 450);

	CreateButtons();
}

MiniDraw::~MiniDraw()
{
}

void MiniDraw::CreateButtons()
{
	open_action_ = new QAction( QIcon(":/Resources/images/open.png"), tr("&Open"), this );
	save_action_ = new QAction( QIcon(":/Resources/images/save.png"), tr("&Save"), this );
	draw_action_ = new QAction( QIcon(":/Resources/images/design.png"), tr("&Draw"), this );
	dele_action_ = new QAction( QIcon(":/Resources/images/delete.png"), tr("&Delete"), this );
	buil_action_ = new QAction( QIcon(":/Resources/images/build.png"), tr("&Build"), this );
	play_action_ = new QAction( QIcon(":/Resources/images/play.png"), tr("&Play"), this );

	connect(open_action_, SIGNAL(triggered()), view_widget_, SLOT(OpenFile()));
	connect(save_action_, SIGNAL(triggered()), view_widget_, SLOT(SaveFile()));
	connect(draw_action_, SIGNAL(triggered()), view_widget_, SLOT(SetDrawStatus()));
	connect(dele_action_, SIGNAL(triggered()), view_widget_, SLOT(DeleteFigure()));
	connect(buil_action_, SIGNAL(triggered()), view_widget_, SLOT(Build()));
	connect(play_action_, SIGNAL(triggered()), view_widget_, SLOT(Simulate()));

	open_action_->setStatusTip(tr("Open a .ms file"));
	save_action_->setStatusTip(tr("Save as .ms file"));
	draw_action_->setStatusTip(tr("Design a new mass-spring system"));
	dele_action_->setStatusTip(tr("Delete last system"));
	buil_action_->setStatusTip(tr("Build mass-spring system"));
	play_action_->setStatusTip(tr("Mass-spring simulation"));

	menu = menuBar()->addMenu(tr("&Menu"));
	menu->addAction(open_action_);
	menu->addAction(save_action_);
	menu->addAction(draw_action_);
	menu->addAction(dele_action_);
	menu->addAction(buil_action_);
	menu->addAction(play_action_);

	toolbar_main_ = addToolBar(tr("&Main"));
	toolbar_main_->addAction(open_action_);
	toolbar_main_->addAction(save_action_);
	toolbar_main_->addAction(draw_action_);
	toolbar_main_->addAction(dele_action_);
	toolbar_main_->addAction(buil_action_);
	toolbar_main_->addAction(play_action_);
}