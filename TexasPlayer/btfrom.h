#pragma once
#include <QPropertyAnimation>
#include <QWidget>
#include <QString>
#include "ui_btfrom.h"

class btfrom : public QWidget
{
	Q_OBJECT

public:
	btfrom(QWidget *parent = nullptr);
	~btfrom();

	//设置图标和文本
	void setIconAndText(QString path, QString text, int number);

	//返回本地序号
	int getNumber();

	//修改窗口颜色接口
	void setWindowSelect(bool isSelect);
	
protected:
	//重写鼠标点击事件
	void mousePressEvent(QMouseEvent* event) override;
	//鼠标释放事件
	void mouseReleaseEvent(QMouseEvent* event) override;

	
public: 
signals:
	//定义点击信号
	void modifiesMainInterface(int inp);

private:
	Ui::btfromClass ui;

	//记录选中事件状态
	bool _isSelect;
	//记录本地序号
	int _number;

	//动画驱动器
	QPropertyAnimation* animation1;
	QPropertyAnimation* animation2;
	QPropertyAnimation* animation3;
	QPropertyAnimation* animation4;
};

