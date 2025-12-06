#include "volume.h"
#include <QPainter>
#include <QMouseEvent>
#include "MusicList.h"
//鼠标位置初始状态
QPoint mouseLastRound;
volume::volume(QWidget *parent)
	: QWidget(parent)
	, _isMute(false)
{
	ui.setupUi(this);
	filterInit();
	backFunInit();

	setValumeLabel(50);
}

volume::~volume()
{}

void volume::volumeVertexBack(int& out, QEvent*  event)
{
	/*
	*	无需判断鼠标事件 进入则说明进度条顶点点击事件触发
	*/
	out = 1;
	auto mouseEvent = static_cast<QMouseEvent*>(event);
	mouseLastRound = mouseEvent->globalPosition().toPoint();
}

void volume::pregressCurrentBack(int& out, QEvent* event)
{
	/*
	*	获取鼠标位置，直接修改参数 抛出信号
	*/
	QPoint currentMouse = ((QMouseEvent*)event)->globalPosition().toPoint(); //当前位置
	QPoint currentWidget = ui.volumePregress->mapToGlobal(QPoint(0, 0));
	int y = currentMouse.y() - currentWidget.y();   //获取鼠标相对于窗口中的y位置
	ui.pregressCurrent->resize(ui.pregressCurrent->width(), 145 - y);
	ui.pregressCurrent->move(ui.pregressCurrent->x(), y);
	ui.volumeVertex->move(ui.volumeVertex->x(), y - 7);
	setValumeLabel((ui.pregressCurrent->height() * 100) / 140);
}

void volume::pregressTargetBack(int& out, QEvent* event)
{
	/*
	*	获取鼠标位置，直接修改参数 抛出信号
	*/

	QPoint currentMouse = ((QMouseEvent*)event)->globalPosition().toPoint(); //当前位置
	QPoint currentWidget = ui.volumePregress->mapToGlobal(QPoint(0, 0));
	int y = currentMouse.y() - currentWidget.y();   //获取鼠标相对于窗口中的y位置
	ui.pregressCurrent->resize(ui.pregressCurrent->width(), 145 - y);
	ui.pregressCurrent->move(ui.pregressCurrent->x(), y);
	ui.volumeVertex->move(ui.volumeVertex->x(), y - 7);
	setValumeLabel((ui.pregressCurrent->height() * 100) / 140);
}


void volume::paintEvent(QPaintEvent* event)
{
	(void*)event;
	//创建绘画者
	QPainter painter(this);  //将bottom部件设置透明，底部绘画放在主窗口中
	//设置画笔
	QPen pen;
	pen.setStyle(Qt::SolidLine); //什么类型的线条
	pen.setWidth(3);  //粗细
	pen.setBrush(Qt::green);  //颜色
	pen.setCapStyle(Qt::RoundCap); //画笔端点样式
	pen.setJoinStyle(Qt::RoundJoin); //画笔连接样式
	//
	//设置画刷   画刷无效
	QBrush brush;
	brush.setStyle(Qt::SolidPattern); //设置填充方式
	brush.setColor(Qt::black);
	//
	pen.setBrush(brush);
	painter.setPen(pen);
	painter.setBrush(brush);
	//
	//设置顶点
	static const QPointF trianglePoints[3] = {
	QPointF(47, 235),   // 顶点
	QPointF(30, 215),   // 左下角点
	QPointF(64, 215)   // 右下角点
	};

	painter.drawConvexPolygon(trianglePoints, 3);
}

bool volume::eventFilter(QObject* watched, QEvent* event)
{
	/*
	*	判断事件是否由目标窗口触发
	*/

	static int solve = 0;
	{
		/*
		*	设置状态机
		*	solve:
		*	0:常规状态 对点击事件进行检测 进度条组件点下直接修改数据，并不继续检测； 检测到有效进度条顶点 进行拖拽
		*	1：拖拽状态
		*/

		if (0 == solve)
		{
			if ((((QMouseEvent*)event)->button() == Qt::LeftButton &&
				QEvent::MouseButtonPress == event->type()))
			{
				_backFun[(void*)watched](this, solve, event);
				event->accept();
			}
			//
		}
		else if (1 == solve)
		{
			/*
			*	鼠标移动或是释放都会触发
			*	移动：与点击位置求差进行修改
			*	释放： 修改状态
			*/
			if(event->type() == QEvent::MouseMove || 
				(((QMouseEvent*)event)->button() == Qt::LeftButton &&
				QEvent::MouseButtonRelease == event->type()))
			{
				dragAndDrop(solve, event, watched);
			}
		}
		//
	}
	return false;
}


void volume::backFunInit()
{
	/*
	*	注册驱动
	*/
	_backFun[(void*)ui.pregressCurrent] = &volume::pregressCurrentBack;
	_backFun[(void*)ui.pregressTarget] = &volume::pregressTargetBack;
	_backFun[(void*)ui.volumeVertex] = &volume::volumeVertexBack;
}

void volume::filterInit()
{
	//将过滤器嵌入控件
	ui.volumeVertex->installEventFilter(this);
	ui.pregressCurrent->installEventFilter(this);
	ui.pregressTarget->installEventFilter(this);
}

void volume::dragAndDrop(int& out, QEvent* event, QObject* watched)
{
	/*
	*	对拖拽事件进行处理
	*	左键释放修改输出参数 重新开始状态检测
	*/

	if ((((QMouseEvent*)event)->button() == Qt::LeftButton &&
		QEvent::MouseButtonRelease == event->type()))
	{
		//检测到释放事件
		out = 0;
		return;
	}

	QPoint currentPoint = ((QMouseEvent*)event)->globalPosition().toPoint();
	//计算坐标偏移量
	QPoint currentPointoffset = mouseLastRound - currentPoint;
	//将很坐标清除为零
	
	//刷新音量条
	setVolumeBarCoordinates(currentPointoffset);

	//刷新坐标
	mouseLastRound = ((QMouseEvent*)event)->globalPosition().toPoint();
	//qDebug() << "Drag and drop event";
}


void volume::setVolumeBarCoordinates(QPoint point)   //坐标不对
{
	/*
	*	根据坐标y轴量来修改位置
	*/
	int y = ui.volumeVertex->y() - point.y();
	if (y < -2)	y = -2;
	else if (y > 138) y = 138;

	ui.volumeVertex->move(ui.volumeVertex->x(), y);
	ui.pregressCurrent->move(ui.pregressCurrent->x(), y + 7);
	ui.pregressCurrent->resize(ui.pregressCurrent->width(), 145 - y - 7);

	setValumeLabel((ui.pregressCurrent->height() * 100) / 140);
}

void volume::setVolumeBarProportion(int proportion)
{
	/*
	*	通过比例计算位置
	*/

}

void volume::setValumeLabel(int value)
{
	if (value > 100)	value = 100;
	else if (value < 0) value = 0;

	ui.valumeLabel->setText(QString("%1%").arg(value));
	_value = value;
	createMusicList()->setVolume(value);
}

void volume::on_volumeButton_clicked()
{
	if (_isMute)
	{
		//调整为正常
		ui.volumeButton->setStyleSheet("#volumeButton{image: url(:/barVolume.png);border:none;}#volumeButton::hover{background-color: rgb(0, 85, 255);}");
		_isMute = false;
		createMusicList()->setVolume(_value);
	}
	else
	{
		//调整为静音
		ui.volumeButton->setStyleSheet("#volumeButton{image:url(:/barMute.png);border:none;}#volumeButton::hover{background-color:rgb(0, 85, 255);}");
		_isMute = true;
		createMusicList()->setVolume(0);
	}
}


