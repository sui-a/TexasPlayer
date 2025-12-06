#include "myGeneral.h"
#include "MusicList.h"
#include "generalRepresentationBar.h"
#include "MusicMonomer.hpp"
#include "TexasPlayer.h"

myGeneral::myGeneral(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void myGeneral::setParent(TexasPlayer* parent)
{
	_parent = parent;
}


myGeneral::~myGeneral()
{}

void myGeneral::headInit(QString text)
{
	ui.head->setText(text);
}

void myGeneral::logoInit(QString path)
{
	//方法1：
	/*image: url(: / texas_head.jpg);
	QString pathQss = "image: url(" + path + ");";
	ui.image->setStyleSheet(pathQss);*/

	//方法2：
	/*QImage img;
	img.load(QString(path));
	QImage imgProcessed; //进行指定缩放
	imgProcessed = img.scaled(ui.image->width(), ui.image->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation); //保持原比例和平滑变换
	ui.image->setPixmap(QPixmap::fromImage(imgProcessed));*/


	//方法3：使用纯pixmap
	QPixmap pixmap(path);
	ui.image->setPixmap(pixmap);
	ui.image->setScaledContents(true);
}

void myGeneral::headAndLogoInit(QString text, QString path, QString style)
{
	logoInit(path);
	headInit(text);
	_style = style;
}

void myGeneral::contentAdd(QString id, QString musicName, QString author,bool isLike, bool isVip, bool isQS)
{
	/*
	*	//音乐名
	*	//作者名
	*	//是否需要vip
	*	//是否需要清晰
	*/
	generalRepresentationBar* newMusic = new generalRepresentationBar(this, id);
	//设置内容
	newMusic->setLikeSwitch(isLike);
	newMusic->setMusicName(musicName);
	newMusic->setMusicAuthor(author);

	QListWidgetItem* item = new QListWidgetItem(ui.musicList);
	//设置推荐大小
	item->setSizeHint(QSize(newMusic->width(), newMusic->height()));

	ui.musicList->setItemWidget(item, newMusic);

	//绑定处理
	connect(newMusic, &generalRepresentationBar::setLike, this, &myGeneral::likeSwitchHendle);
	connect(newMusic, &generalRepresentationBar::playMusic, this, &myGeneral::playSwitchHendle);

}

void myGeneral::paintEvent(QPaintEvent* event)
{
	//更新太多，不好在这里检测
}

void myGeneral::refresh()
{
	/*
	*	从musiclist中获取
	* 
	*/
	//qDebug() << "myGeneral refresh called" << _style;
	auto content = createMusicList()->getContent(_style);
	_musicList.clear();
	ui.musicList->clear();

	//添加
	for (auto it : content)
	{
		if (!findGeneralRepresentationBarClass(it._id))
		{
			//找不到
			//进行添加
			_musicList.push_back(it);
			//"unknown"
			contentAdd(it._id, it._musicName, QString("未知"), it._isLike, false, false);
		}
	}
	repaint(); //不放入任务队列
}

bool myGeneral::findGeneralRepresentationBarClass(QString id)
{
	for (auto it : _musicList)
	{
		if (it._id == id)
		{
			//找到
			return true;
		}
	}
	return false;
}


void myGeneral::likeSwitchHendle(QString id, bool like)
{
	//向上传递
	createMusicList()->setLike(id, like);
}

void myGeneral::playSwitchHendle(QString id)
{
	//进行播放
	createMusicList()->playMusic(id, _style);
	//向父组件传递
	_parent->playRefresh();
}



