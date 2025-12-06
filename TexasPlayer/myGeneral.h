#pragma once
#include <QWidget>
#include "ui_myGeneral.h"
#include "MusicMonomer.hpp"

class TexasPlayer;
class myGeneral : public QWidget
{
	Q_OBJECT

public:
	myGeneral(QWidget *parent = nullptr);
	~myGeneral();
	//设置父窗口地址
	void setParent(TexasPlayer* parent);

	//
	void headAndLogoInit(QString text, QString path, QString style);
	//标题接口
	void headInit(QString text);
	//logo接口
	void logoInit(QString path);
	//内容添加接口
	void contentAdd(QString id, QString musicName, QString author, bool isLike = false, bool isVip = false, bool isQS = false);
	//从本地寻找
	bool findGeneralRepresentationBarClass(QString id);

	//更新内容
	void refresh();

	//事件重载
	void paintEvent(QPaintEvent* event) override;
public: signals:
	void refreshPlay();

public slots:
	void likeSwitchHendle(QString id, bool like);
	void playSwitchHendle(QString id);

private:
	TexasPlayer* _parent;
	Ui::myGeneralClass ui;
	QString _style;

	//本模块音乐元数据链表
	std::list<MusicInfo> _musicList;
};

