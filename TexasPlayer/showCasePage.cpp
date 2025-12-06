#include "showCasePage.h"

showCasePage::showCasePage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
	//setVisible(true);
	ui.image->installEventFilter(this);
}

showCasePage::~showCasePage()
{}

unsigned int a = 0;
bool showCasePage::eventFilter(QObject* obj, QEvent* ev)
{
	/*
	*	进入事件，但是设置图像无法触发，
	*	1.判断是否是设置了布局器  解决方案：查看
	*	2.查看是否因为任务太多来不及更新  解决方案：显式放入任务队列  
	* 
	*	//窗口成功放大，图像没有刷新
	*/
	//进行事件判断
	if (obj == ui.image)
	{
		//可以进入事件
		if (QEvent::Enter == ev->type())
		{
			//鼠标进入 且触发窗口为image
			//对窗口进行放大处理
			ui.image->setGeometry(15, 5, 150, 150);
			if (!_imageQss.isEmpty())
			{
				ui.image->setStyleSheet(_imageQss);
			}
			return true;
		}
		else if (QEvent::Leave == ev->type())
		{
			//鼠标退出
			//对窗口进行缩小处理
			ui.image->setGeometry(25, 15, 130, 130);
			if (!_imageQss.isEmpty())
			{
				ui.image->setStyleSheet(_imageQss);
			}
			return true;
		}
	}
	return QObject::eventFilter(obj, ev);
}

void showCasePage::setBodyText(QString text)
{
	ui.showLabel->setText(text);
}

void showCasePage::setImage(QString path)
{
	QString imageQss = "border: none;image: url(" + path + ");";
	_imageQss = imageQss;
	ui.image->setStyleSheet(imageQss);
}