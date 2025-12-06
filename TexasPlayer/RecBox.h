#pragma once

#include <QWidget>
#include "ui_RecBox.h"
#include <vector>

class showCasePage;
class RecBox : public QWidget
{
	Q_OBJECT

public:
	RecBox(QWidget *parent = nullptr);
	~RecBox();

	//按钮初始化
	void buttonInit(int row = 1);

	//添加内容
	void addPathContent(QString path, QString text);
	//刷新窗口
	void refreshShowCasePage();

private slots:
	//定义换组槽函数
	void on_rightButton_clicked();
	void on_leftButton_clicked(); //左按钮


private:
	Ui::RecBoxClass ui;

	//内容线性数组
	std::vector<std::vector<QString>> _contentText;
	std::vector<std::vector<QString>> _contentPath;

	//记录宽高
	int _row;
	int _col;

	//记录内容组索引
	int _contentIndex;
};

