#pragma once

#include <QWidget>
#include "ui_lrcInterface.h"

class TexasPlayer;
class QPropertyAnimation;

struct LyricLine
{
	qint64 _timeStamp; // 时间戳，单位为毫秒
	QString _text;  // 歌词文本

	LyricLine(qint64 timeStamp, QString text)
		: _timeStamp(timeStamp)
		, _text(text)
	{

	}

};


class lrcInterface : public QWidget
{
	Q_OBJECT

public:
	lrcInterface(QWidget *parent = nullptr);
	~lrcInterface();

	//初始化歌词界面
	void lrcInit(int x, int y, int width, int height);
	//显示初始化
	void showInit(int x, int y);

	//出场运动
	void showExercise();

	//根据索引设置歌词
	void setIrcText(int index, QString irc);
	//更新歌词
	void refreshLyrics();
	//读取歌词
	void readLyrics(QString path);

	//解析字符串并存储
	void parseAndStoreLyrics(QString& lrcContent);
	//计算时间
	qint64 calculateTheTimestamp(QString& timeString);

private slots:
	void on_pullPush_clicked();
	void playOver();

private:
	Ui::lrcInterfaceClass ui;
	QPropertyAnimation* animation;
	bool _isShow;
	TexasPlayer* _parent;

	//存储歌词
	std::vector<LyricLine> _lyric;
};

