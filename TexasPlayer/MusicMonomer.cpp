#include <quuid.h>
#include <QtWidgets/QWidget>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <qmediametadata.h>
#include <QCoreApplication>
#include "MusicMonomer.hpp"

MusicMonomer::MusicMonomer(QString url)
	:_likeNext(nullptr)
	, _url(url)
	, _time(-1)
	, _localNext(nullptr)
	, _likePre(nullptr)
	, _isLike(false)
	, _historyNext(nullptr)
{
	_id = QUuid::createUuid().toString(); //创建唯一表示
}

MusicMonomer::~MusicMonomer()
{

}

void MusicMonomer::setLikeNext(MusicMonomer* next)
{
	// 设置默认的下一个单元
	_likeNext = next;
}

void MusicMonomer::getMusicId(QString& id) const
{
	id = _id;
}

QString MusicMonomer::getMusicId() const
{
	return _id;
}

void MusicMonomer::play(QMediaPlayer& player)
{
	/*
	*	
	*/
	//QAudioOutput* audioOutput = new QAudioOutput;

	// 关键步骤：将音频输出设置给播放器
	//audioOutput->setVolume(volume); // 设置音量，范围是0到100
	//player.setAudioOutput(audioOutput);

	player.setSource(QUrl::fromLocalFile(_url));
	while(player.mediaStatus() != QMediaPlayer::LoadedMedia) 
	{
		QCoreApplication::processEvents(); // 处理事件循环，防止界面卡死
	}
	
	_time = player.metaData().value(QMediaMetaData::Duration).toInt();
	player.play();
}

void MusicMonomer::setLocalNext(MusicMonomer* next)
{
	_localNext = next;
}

QString MusicMonomer::getUrl() const
{
	return _url;
}
void MusicMonomer::getUrl(QString& url) const
{
	url = _url;
}

MusicMonomer* MusicMonomer::getLocalNext()
{
	return _localNext;
}

void MusicMonomer::getMusicInfo(MusicInfo* out)
{
	out->_isLike = _isLike;
	out->_id = _id;

	QString::iterator left = _url.end();
	QString::iterator right = _url.end();

	left--;
	right--;
	//向前遍历寻找第一个.和/
	
	while (1)
	{
		if ((*left) != '/')
		{
			left--;
		}
		else if ((*right) != '.')
		{
			right--;
		}
		else
		{
			break;
		}
	}
	//成功寻找到目标
	QString target;
	for (left++; left != right; left++)
	{
		target += (*left);
	}
	out->_musicName = target;
}

MusicInfo MusicMonomer::getMusicInfo()
{
	MusicInfo out;
	out._isLike = _isLike;
	out._id = _id;


	QString::iterator left = _url.end();
	QString::iterator right = _url.end();

	left--;
	right--;
	//向前遍历寻找第一个.和/
	while (1)
	{
		if ((*left) != '/')
		{
			left--;
		}
		else if ((*right) != '.')
		{
			right--;
		}
		else
		{
			break;
		}
	}

	//成功寻找到目标
	QString target;
	for (left++; left != right; left++)
	{
		target += (*left);
	}
	out._musicName = target;
	return out;
}

MusicMonomer* MusicMonomer::getLikeNext()
{
	return _likeNext;
}

void MusicMonomer::setLikePre(MusicMonomer* pre)
{
	_likePre = pre;
}

void MusicMonomer::setLike(bool isLike)
{
	_isLike = isLike;
}


MusicMonomer* MusicMonomer::getHistoryNext()
{
	return _historyNext;
}
MusicMonomer* MusicMonomer::getHistoryPre()
{
	return _historyPre;
}

void MusicMonomer::setLocalPre(MusicMonomer* pre)
{
	_localPre = pre;
}

MusicMonomer* MusicMonomer::getLikePre()
{
	return _likePre;
}

MusicMonomer* MusicMonomer::getLocalPre()
{
	return _localPre;
}

void MusicMonomer::setHistoryNext(MusicMonomer* next)
{
	_historyNext = next;
}

void MusicMonomer::setHistoryPre(MusicMonomer* pre)
{
	_historyPre = pre;
}