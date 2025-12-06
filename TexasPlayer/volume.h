#pragma once
#include <QWidget>
#include <unordered_map>
#include <functional>
#include "ui_volume.h"

//音量调节窗口
class volume : public QWidget
{
	Q_OBJECT

public:
	volume(QWidget *parent = nullptr);
	~volume();

	//初始化事件处理回调后函数
	void backFunInit();
	void filterInit();
	void dragAndDrop(int& out, QEvent* event, QObject* watched);

	//重写绘制事件函数
	void paintEvent(QPaintEvent* event) override;

	//重写事件过滤器
	bool eventFilter(QObject* watched, QEvent* event) override;

	//坐标修改音量条
	void setVolumeBarCoordinates(QPoint point);
	//比例修改音量条
	void setVolumeBarProportion(int proportion); //100%
	//修正音量数字
	void setValumeLabel(int value);

	//处理函数
	void volumeVertexBack(int& out, QEvent* event);
	void pregressCurrentBack(int& out, QEvent* event);
	void pregressTargetBack(int& out, QEvent* event);

private slots:
	//设置静音槽函数
	void on_volumeButton_clicked();


private:
	Ui::volumeClass ui;
	std::unordered_map<void*, std::function<void(volume*, int&, QEvent*)>> _backFun;
	bool _isMute;
	int _value;
};

