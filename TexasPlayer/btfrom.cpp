#include "btfrom.h"
#include <QMouseEvent>

btfrom::btfrom(QWidget *parent)
	: QWidget(parent)
	, _isSelect(false)
	, _number(-1)
{
	ui.setupUi(this);

	{
		//初始化动画效果
		animation1 = new QPropertyAnimation(ui.one, "geometry", this);
		//设置循环时间
		animation1->setDuration(1000); //10帧

		//设置关键帧
		animation1->setKeyValueAt(0, QRect(12, 15, 4, 0)); //初始最小值
		animation1->setKeyValueAt(0.5, QRect(12, 0, 4, 15)); //中间帧
		animation1->setKeyValueAt(1, QRect(12, 15, 4, 0)); //结束帧

		//设置循环轮数
		animation1->setLoopCount(-1);
	}

	{
		animation2 = new QPropertyAnimation(ui.two, "geometry", this);
		animation2->setDuration(1200);
		animation2->setKeyValueAt(0, QRect(19, 15, 4, 0));
		animation2->setKeyValueAt(0.5, QRect(19, 0, 4, 15));
		animation2->setKeyValueAt(1, QRect(19, 15, 4, 0));
		animation2->setLoopCount(-1);
	}

	{
		animation3 = new QPropertyAnimation(ui.three, "geometry", this);
		animation3->setDuration(1400);
		animation3->setKeyValueAt(0, QRect(26, 15, 3, 0));
		animation3->setKeyValueAt(0.5, QRect(26, 0, 3, 15));
		animation3->setKeyValueAt(1, QRect(26, 15, 3, 0));
		animation3->setLoopCount(-1);
	}

	{
		animation4 = new QPropertyAnimation(ui.four, "geometry", this);
		animation4->setDuration(1600);
		animation4->setKeyValueAt(0, QRect(32, 15, 2, 0));
		animation4->setKeyValueAt(0.5, QRect(32, 0, 2, 15));
		animation4->setKeyValueAt(1, QRect(32, 15, 2, 0));
		animation4->setLoopCount(-1);
	}
	animation1->start();
	animation2->start();
	animation3->start();
	animation4->start();

	//隐藏
	ui.musicalNoteBox->hide();
}

btfrom::~btfrom()
{
	
}


void btfrom::setIconAndText(QString path, QString text, int number)
{
	//进行图标设置
	QPixmap pixmap = QPixmap(path).scaledToHeight(ui.logo->height(), Qt::SmoothTransformation);
	ui.logo->setPixmap(pixmap);


	//进行文本设置
	ui.text->setText(text);

	//设置序号
	_number = number;
}

void btfrom::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		_isSelect = true;
	}
}

void btfrom::mouseReleaseEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		//释放事件为鼠标左键 后续判断是否出去按钮内

		QPoint releasePos = event->pos();

		// 判断是否在窗口客户区内（不含标题栏和边框）
		if (rect().contains(releasePos)) 
		{
			
			//触发信号
			emit modifiesMainInterface(_number);
		}
		//
		_isSelect = false;
	}
	//
}

int btfrom::getNumber()
{
	return _number;
}

void btfrom::setWindowSelect(bool isSelect)
{
	//判断是否被选中
	if (isSelect)
	{
		//选中 修改成选中状态
		ui.btStyle->setStyleSheet("#btStyle{background-color: #71757E;}  #btStyle::hover{background-color: #a1a5aE;}");
		ui.musicalNoteBox->show();
	}
	else
	{
		//未选中 修改成释放状态
		ui.btStyle->setStyleSheet("#btStyle::hover{background-color: #51555E;}");
		ui.musicalNoteBox->hide();
	}
}