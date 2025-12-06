#include "generalRepresentationBar.h"
#include <QMouseEvent>

generalRepresentationBar::generalRepresentationBar(myGeneral* musicParent, QString id, QWidget* parent)
	: QWidget(parent)
	, _parent(musicParent)
	, _id(id)
	, _isLike(false)
{
	ui.setupUi(this);
}

generalRepresentationBar::~generalRepresentationBar()
{}

void generalRepresentationBar::setLikeSwitch(bool islike)
{
	/*
	* //根据传入参数修改背景
	*/
	QString imageQss;
	if (!islike)
	{
		//喜爱
		imageQss = "image: url(:/music/musicUnlike.png);";
	}
	else
	{
		imageQss = "image: url(:/music/musicLike.png);";
	}
	_isLike = islike;
	ui.likeSwitch->setStyleSheet(imageQss);

	//发出信号
	emit setLike(_id, _isLike);
}

void generalRepresentationBar::setMusicName(QString name)
{
	//
	ui.musicNameLabel->setText(name);
}

void generalRepresentationBar::setMusicAuthor(QString name)
{
	//
	ui.authorLabel->setText(name);
}

void generalRepresentationBar::on_likeSwitch_clicked()
{
	//后续关联至总数据库的槽函数
	if (_isLike)
	{
		setLikeSwitch(false);
		return;
	}
	setLikeSwitch(true);
}


void generalRepresentationBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) 
	{
		//进行播放
		playMusic(_id);
	}
	QWidget::mouseDoubleClickEvent(event); //向后传递
}