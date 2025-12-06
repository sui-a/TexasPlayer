#pragma once

#include <QWidget>
#include "ui_showCasePage.h"

class showCasePage : public QWidget
{
	Q_OBJECT

public:
	showCasePage(QWidget *parent = nullptr);
	~showCasePage();

	//进行事件过滤，对子窗口事件拦截并提前进行处理
	bool eventFilter(QObject* obj, QEvent* ev) override;

	//设置文本
	void setBodyText(QString text);
	//设置图像
	void setImage(QString path);
private:
	Ui::showCasePageClass ui;
	QString _imageQss;
};

