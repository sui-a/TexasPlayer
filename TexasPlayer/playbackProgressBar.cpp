#include "playbackProgressBar.h"
#include <QMouseEvent>

playbackProgressBar::playbackProgressBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//嵌入过滤器
	ui.currentPress->installEventFilter(this);
	ui.targetPress->installEventFilter(this);

}

playbackProgressBar::~playbackProgressBar()
{}

void playbackProgressBar::setProgress(int current)
{
	/*
	*	//俩进度条没有做到线程安全 双方更改一个 另一个未修改 导致数据错误, 把宽度修改统一到这里修改
	*/
	if (1000 < current)
	{
		current = 1000;
	}
	double proportion = ((double)current) / 1000; //浮点数计算需要双方至少其中一个为浮点数，使双方和结果数据向上扩展才会正常存储
	if (proportion > 1)	proportion = (double)1.0;
	//获取基座总长
	//int baseWidth = ui.currentPress->width() + ui.targetPress->width();  
	int baseWidth = 810;
	//通过

	//修改左条
	int currentPressWidth = ((int)(baseWidth * proportion));
	//qDebug() << "baseWidth: " << baseWidth;
	//qDebug() << "currentPressWidth: " << currentPressWidth;

	//ui.currentPress->resize(currentPressWidth, ui.currentPress->height());
	//修改右条
	//ui.targetPress->move(currentPressWidth, 0);     不修改targetPress的位置和大小  目标效果有布局管理器触发
	//ui.targetPress->resize(baseWidth - currentPressWidth, ui.targetPress->height());

	//窗口resize被窗口布局管理器覆盖，导致没有按照预定方式显示
	//调整窗口最小和最大值

	ui.currentPress->setMinimumSize(currentPressWidth, ui.currentPress->height());
	ui.currentPress->setMaximumSize(currentPressWidth, ui.currentPress->height());
	ui.targetPress->setMinimumSize(810 - currentPressWidth, ui.currentPress->height());
	ui.targetPress->setMaximumSize(810 - currentPressWidth, ui.currentPress->height());
	//检查获取父组件宽高是否正确
	//qDebug() << "father height: " << ui.pressBaseLink->height();
	//qDebug() << "father width: " << ui.pressBaseLink->width();
	//qDebug() << "two -- currentPress width: " << ui.currentPress->width();  
	//qDebug() << "two -- targetPress width: " << ui.targetPress->width();
	//qDebug() << "height: " << ui.currentPress->height(); //首次获取是正确的
}

bool playbackProgressBar::eventFilter(QObject* obj, QEvent* ev)  //有延迟
{
	//开始实现进度条拖动效果
	//基窗口对鼠标时间进行拦截，判断点击事件还是进入退出事件 点击事件修改进度条并发出信号 进入事件修改进度条高度
	if (QEvent::Enter == ev->type())
	{
		//鼠标进入
		ui.currentPress->setMaximumHeight(8);
		ui.targetPress->setMaximumHeight(8);
		ui.currentPress->setMinimumHeight(8);
		ui.targetPress->setMinimumHeight(8);

		/*ui.currentPress->setMinimumSize(ui.currentPress->width(), 8);
		ui.targetPress->setMinimumSize(ui.targetPress->width(), 8);
		ui.currentPress->setMinimumSize(ui.currentPress->width(), 8);
		ui.targetPress->setMinimumSize(ui.targetPress->width(), 8);*/


		return true;
	}
	else if (QEvent::Leave == ev->type())
	{
		//离开
		ui.currentPress->setMaximumHeight(4);
		ui.targetPress->setMaximumHeight(4);
		ui.currentPress->setMinimumHeight(4);
		ui.targetPress->setMinimumHeight(4);

		/*ui.currentPress->setMinimumSize(ui.currentPress->width(), 4);
		ui.targetPress->setMinimumSize(ui.targetPress->width(), 4);
		ui.currentPress->setMinimumSize(ui.currentPress->width(), 4);
		ui.targetPress->setMinimumSize(ui.targetPress->width(), 4);*/
		return true;
	}

	if (ui.currentPress == obj || ui.targetPress == obj)
	{
		if (QEvent::MouseButtonPress == ev->type() &&
			((QMouseEvent*)ev)->button() == Qt::LeftButton)  //事件错误
		{
			//进度条两端其中一端被点击
			//根据leftTop 于点击位置的坐标差
			//修改当前current状态

			//获取坐标差
			QPoint mousePressPoint = ((QMouseEvent*)ev)->globalPosition().toPoint(); //全局坐标
			QPoint currentProgressPoint = ui.currentPress->mapToGlobal(QPoint(0, 0)); //qpoint传入参数是为了容纳多态 无实际作用
			QPoint poor = mousePressPoint - currentProgressPoint;

			/*
			*	//bug: 无论何止结果都为零
			*	//原因: 给double(8字节)变量初始化的数据为int类型，强转后也是拷贝为int类型的临时变量  造成数据丢失
			*	//修正: double数据丢失由于cout非格式化写入 原因出在setProgress
			*/
			int poorProportion = ((poor.x() * 1000) / ((ui.currentPress->width() + ui.targetPress->width())));

			//预防不必要错误
			if (1000 < poorProportion)  poorProportion = 1000;
			else if (poorProportion < 0)	poorProportion = 0;
			//qDebug() << "--------------------------------------";
			setProgress(poorProportion); //数据没问题了 显示效果丢失  //*外部调整没有问题内部调整丢失
			pregressClicked(poorProportion);  //poorProportion传入0 - 1000
			return true;
		}
	}
	
	return QObject::eventFilter(obj, ev);
}



