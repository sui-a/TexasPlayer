#pragma once
#include <QtWidgets/QWidget>
#include "ui_generalRepresentationBar.h"
/*
* 定义存储表示栏
*/
class myGeneral;
class generalRepresentationBar : public QWidget
{
	Q_OBJECT

public:
	generalRepresentationBar(myGeneral* musicParent, QString id, QWidget *parent = nullptr);
	~generalRepresentationBar();

	//定义切换喜爱接口
	void setLikeSwitch(bool islike);
	//修改歌名
	void setMusicName(QString name);
	//修改歌手名
	void setMusicAuthor(QString name);

	//重写鼠标处理函数
	void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
	void setLike(QString id, bool like);
	void playMusic(QString id);


private slots:
	//定义点击事件的槽函数
	void on_likeSwitch_clicked(); //名定义错误 //on_quit_clicked
private:
	Ui::generalRepresentationBarClass ui;
	myGeneral* _parent;
	QString _id;
	bool _isLike;
};

