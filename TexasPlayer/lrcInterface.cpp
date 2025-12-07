#include <QPropertyAnimation>
#include <QFile>
#include "lrcInterface.h"
#include "TexasPlayer.h"

lrcInterface::lrcInterface(QWidget *parent)
	: QWidget(parent)
	, _parent((TexasPlayer*)parent)
	, _isShow(false)
{
	ui.setupUi(this);

	animation = new QPropertyAnimation(this, "pos", this);
	animation->setDuration(1000);
	QObject::connect(animation, &QPropertyAnimation::finished, this, &lrcInterface::playOver);
}

lrcInterface::~lrcInterface()
{}

void lrcInterface::lrcInit(int x, int y, int width, int height)
{
	//设置宽高
	resize(width - 2, height - 52);
	move(x - 1, y);
}

void lrcInterface::setIrcText(int index, QString irc)
{
	/*
	*	根据序号填写内容
	*/
	switch (index) 
	{
	case 1:
		ui.upThree->setText(irc);
		break;
	case 2:
		ui.upTwo->setText(irc);
		break;
	case 3:
		ui.upOne->setText(irc);
		break;
	case 4:
		ui.lrcMain->setText(irc);
		break;
	case 5:
		ui.downOne->setText(irc);
		break;
	case 6:
		ui.downTwo->setText(irc);
		break;
	case 7:
		ui.downThree->setText(irc);
		break;
	default:
		break;
	}
}

void lrcInterface::on_pullPush_clicked()
{
	animation->setStartValue(QPoint(10, 59));
	animation->setEndValue(QPoint(10, 650));
	animation->start();
}

void lrcInterface::showInit(int x, int y)
{
	move(x, y);
}

void lrcInterface::showExercise()
{
	/*
	*	设置动态控制器
	*/	
	animation->setDuration(2000); //设置运动时间
	//auto point = _parent->getVisiblePos(); //获取可见区域宽高
	//point.setX((_parent->width() - point.x()) / 2);
	//qDebug() << point;

	//设置初末位置
	//从可见区域最底端开始运动
	//animation->setStartValue(point);
	//运动到可见区域顶端位置
	//animation->setEndValue(QPoint(point.x(), (_parent->height() - point.y()) / 2));

	//qDebug() << QPoint(point.x(), (_parent->height() - point.y()) / 2);
	show();
	animation->setStartValue(QPoint(10, 700));
	animation->setEndValue(QPoint(10, 59));
	animation->start(); //开始动画
}

void lrcInterface::playOver()
{
	if (_isShow)
	{
		//显示
		_isShow = false;
		hide();
	}
	else 
	{
		_isShow = true;
	}
}

void lrcInterface::refreshLyrics()
{

}

void lrcInterface::readLyrics(QString path)
{
	/*
	*	搜索路径，判断是否使歌词文件
	*/
	_lyric.clear();
	int pos = path.indexOf(".");
	if (-1 == pos)
	{
		//传入的文件错误，不分配接口，清空歌词链表后退出
		return;
	}
	//把后缀切换成.lrc
	bool isFalse = true;
	while (!path.isEmpty())
	{
		if (path.back() == '.')
		{
			isFalse = false;
			break;
		}
		path.chop(1);
	}
	if (isFalse)
	{
		//路径不符合规范，退出
		return;
	}

	path.push_back("lrc");

	//读取歌词文件
	QFile file("test.txt");
	if (!file.open(QIODevice::ReadOnly)) //以只读形式打开 
	{
		//找不到文件路径，清空歌词链表后退出
		_lyric.clear();
		return;
	}

	while (!file.atEnd()) 
	{
		QString line = file.readLine();
		//解析歌词和时间戳
		parseAndStoreLyrics(line);
	}
	//
}

void lrcInterface::parseAndStoreLyrics(QString& lrcContent)
{
	/*
	*	解析并存储
	*	歌词存储格式[分钟:秒.毫秒]词
	*/

	int timeStart = lrcContent.indexOf("[");
	int timeEnd = lrcContent.indexOf("]");
	QString timeResult = lrcContent.mid(timeStart + 1, timeEnd - timeStart - 1);

	qint64 timestamp = calculateTheTimestamp(timeResult);
	
	QString lrcResult = lrcContent.mid(timeEnd + 1);

	_lyric.push_back(LyricLine(timestamp, lrcResult));
}

qint64 lrcInterface::calculateTheTimestamp(QString& timeString)
{
	QStringList parts = timeString.split(":");
	if (parts.size() != 2) 
		return -1;

	int minutes = parts[0].toInt();

	QStringList parts2 = parts[1].split(".");
	if (parts2.size() != 2) 
		return -1;

	int seconds = parts2[0].toInt();
	int milliseconds = parts2[1].toInt();

	qint64 totalMs = (minutes * 60 + seconds) * 1000 + milliseconds;
	return totalMs;
}

void lrcInterface::setMusicName(QString name)
{
	ui.name->setText(name);
}

