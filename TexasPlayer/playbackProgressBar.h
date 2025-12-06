#pragma once

#include <QWidget>
#include "ui_playbackProgressBar.h"

class playbackProgressBar : public QWidget
{
	Q_OBJECT

public:
	playbackProgressBar(QWidget *parent = nullptr);
	~playbackProgressBar();

	//按照比例修改进度条状态  精度1000
	void setProgress(int current);

	//事件过滤器
	bool eventFilter(QObject* obj, QEvent* ev) override;

signals:
	//定义进度条信号
	void pregressClicked(int press); //以整数形式返回，double会向float退化， 防止数据丢失


private:
	Ui::playbackProgressBarClass ui;
};

