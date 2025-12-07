#include <QList>
#include <QUrl>
#include <iostream>
#include <QtWidgets/QWidget>
#include <string>
#include <QAudioOutput>
#include <QFileInfo>
#include <time.h>
#include "MusicMonomer.hpp"
#include "MusicList.h"
#include "TexasPlayer.h"
#include "MusicSQL.h"

MusicList::MusicList()
	: _likeHead(nullptr)
	, _localHead(nullptr)
	, _historyHead(nullptr)
	, _playingHead(nullptr)
	, _volume(0.5)
	, _playingGroup(0)
	, _playModel(playMode::playSingSong)
	, _likeCount(0)
	, _localCount(0)
	, _historyCount(0)
{
	mapInit();
	playInit();
	srand((unsigned int)time(nullptr));
	dbInit();
}
void MusicList::playInit()
{
	playVolumeInit();
}

void MusicList::dbInit()
{
	QStringList urlOut;
	std::vector<bool> isLikeOut;
	getMusicSQLInstance()->read(urlOut, isLikeOut);
	for(int i = 0; i < urlOut.size(); i++)
	{
		//判断音乐是否还存在，不存在从数据库中删除
		if (QFileInfo::exists(urlOut[i]))
		{
			//存在
			//根据url添加音乐
			size_t n = _defaultHead.size();
			_defaultHead.push_back(new MusicMonomer(urlOut[i]));
			_mapToGroup[urlOut[i]] = n;
			//头插进本地链表
			_defaultHead[n]->setLocalNext(_localHead);
			if (_localHead != nullptr)
			{
				_localHead->setLocalPre(_defaultHead[n]);
			}
			_localCount++;
			_localHead = _defaultHead[n];
			_defaultHead[n]->setLike(isLikeOut[i]);
			if (isLikeOut[i])
			{
				//头插进喜欢链表
				if (_likeHead != nullptr)
				{
					_likeHead->setLikePre(_defaultHead[n]);
				}
				_defaultHead[n]->setLikeNext(_likeHead);
				_likeCount++;
				_likeHead = _defaultHead[n];
			}
			_mapToInt[(_defaultHead[n]->getMusicId())] = n;
		}
		else
		{
			//不存在
			getMusicSQLInstance()->deleteByUrl(urlOut[i]);
		}
		//
	}
}

MusicList::~MusicList()
{
	
}

MusicList* createMusicList()
{
	if (musiclist == nullptr)
	{
		//mutex.lock();  //qt本身线程安全，不需要加锁
		if (musiclist == nullptr)
		{
			musiclist = new MusicList();
		}
		//mutex.unlock();
	}
	return musiclist;
}

void MusicList::addMusic(QStringList url)
{
	//std::cout << url[0].toStdString();

	for (auto it : url)
	{
		//根据url添加音乐
		//去重
		if(_mapToGroup.find(it) != _mapToGroup.end())
		{
			//存在相同音乐，跳过
			continue;
		}
		size_t n = _defaultHead.size();
		_defaultHead.push_back(new MusicMonomer(it));
		_mapToGroup[it] = n;
		//头插进本地链表
		_defaultHead[n]->setLocalNext(_localHead);
		if (_localHead != nullptr)
		{
			_localHead->setLocalPre(_defaultHead[n]);
		}
		_localCount++;
		_localHead = _defaultHead[n];
		_defaultHead[n]->setLike(false);
		_mapToInt[(_defaultHead[n]->getMusicId())] = n;

		getMusicSQLInstance()->write(it, _defaultHead[n]->getLike()); //将url写入数据库中
	}
}


std::vector<MusicInfo> MusicList::getContent(QString style)
{
	/*
	*	根据不同标识返回不同的表
	*/
	//后续修改成使用映射表与处理函数结合 适应动态扩展播放列表
	std::vector<MusicInfo> out;
	if ("local" == style)
	{
		auto current = _localHead;
		while (current)
		{
			out.push_back(current->getMusicInfo());
			current = current->getLocalNext();
		}
	}
	else if ("like" == style)
	{
		auto current = _likeHead;
		while (current)
		{
			out.push_back(current->getMusicInfo());
			current = current->getLikeNext();
		}
	}
	else if ("recently" == style)
	{
		auto current = _historyHead;
		while (current)
		{
			out.push_back(current->getMusicInfo());
			current = current->getHistoryNext();
		}
	}
	return out;
}


void MusicList::setLike(QString id, bool like)
{
	//寻找
	if (_mapToInt.find(id) != _mapToInt.end())
	{
		//找到下标
		int n = _mapToInt[id];
		_defaultHead[n]->setLike(like);
		//头插进喜欢链表
		if (like)
		{
			if (_likeHead != nullptr)
			{
				_likeHead->setLikePre(_defaultHead[n]);
			}
			_defaultHead[n]->setLikeNext(_likeHead);
			_likeCount++;
			_likeHead = _defaultHead[n];
		}
		else
		{
			//从喜欢链表中删除
			MusicMonomer* prev = nullptr;
			MusicMonomer* current = _likeHead;
			while (current)
			{
				if (current->getMusicId() == id)
				{
					if (prev == nullptr)
					{
						_likeHead = current->getLikeNext();
					}
					else
					{
						prev->setLikeNext(current->getLikeNext());
						if (current->getLikeNext())
						{
							current->getLikeNext()->setLikePre(prev);
						}
					}
					current->setLikeNext(nullptr);
					current->setLikePre(nullptr);
					break;
				}
				prev = current;
				current = current->getLikeNext();
				_likeCount--;
			}
			//
		}
		getMusicSQLInstance()->write(_defaultHead[n]->getUrl(), _defaultHead[n]->getLike());
		//
	}
	//没有找到，不操作
}

void MusicList::deleteMusic(int n)
{
	//对数组内容进行删除
	//从本地链表中删除
	MusicMonomer* next = _defaultHead[n]->getLocalNext();
	MusicMonomer* pre = _defaultHead[n]->getLocalPre();
	if( next == nullptr && pre == nullptr)
	{
		//唯一节点
		_localHead = nullptr;
		return;
	}
	else
	{
		if (next)
		{
			next->setLocalPre(pre);
		}
		if (pre)
		{
			pre->setLocalNext(next);
		}
		if (_defaultHead[n] == _localHead)
		{
			_localHead = nullptr;
		}
	}
	//喜欢链表
	next = _defaultHead[n]->getLikeNext();
	pre = _defaultHead[n]->getLikePre();
	if (next == nullptr && pre == nullptr)
	{
		//唯一节点
		_likeHead = nullptr;
		return;
	}
	else
	{
		if (next)
		{
			next->setLikePre(pre);
		}
		if (pre)
		{
			pre->setLikeNext(next);
		}
		if (_defaultHead[n] == _likeHead)
		{
			_likeHead = nullptr;
		}
	}

	//历史链表
	next = _defaultHead[n]->getHistoryNext();
	pre = _defaultHead[n]->getHistoryPre();
	if (next == nullptr && pre == nullptr)
	{
		//唯一节点
		_historyHead = nullptr;
		return;
	}
	else
	{
		if (next)
		{
			next->setHistoryPre(pre);
		}
		if (pre)
		{
			pre->setHistoryNext(next);
		}
		if (_defaultHead[n] == _historyHead)
		{
			_historyHead = nullptr;
		}
	}

	//开始删除默认链表中的节点

	if (n >= 0 && n < _defaultHead.size())
	{
		// 2. 直接定位并删除
		_defaultHead.erase(_defaultHead.begin() + n);
	}
}

void MusicList::playMusic(QString id, QString style)
{
	if (_mapToInt.find(id) != _mapToInt.end())
	{
		//找到下标
		int n = _mapToInt[id];
		if (!QFileInfo::exists(_defaultHead[n]->getUrl()))
		{
			//判断路径失效
			//根据下标进行歌曲删除
			deleteMusic(n);
			//删除数据库中的记录
			getMusicSQLInstance()->deleteByUrl(_defaultHead[n]->getUrl());
			//让ui界面进行刷新

			return;
		}


		_defaultHead[n]->play(_musicPlayer);
		//修改为播放状态
		_playingHead = _defaultHead[n];
		updatePlayingGroup(style);

		/*
		*	添加进最近播放链表
		*/
		if(SearchingThroughHistory(_defaultHead[n]))
		{
			/*
			*	链表中找到目标
			*	将目标提升链表头部
			*/
			if (_defaultHead[n] == _historyHead)
			{
				return;
			}

			if(_defaultHead[n]->getHistoryNext() != nullptr)
			{
				_defaultHead[n]->getHistoryNext()->setHistoryPre(_defaultHead[n]->getHistoryPre());
				
			}
			if (_defaultHead[n]->getHistoryPre() != nullptr)
			{
				_defaultHead[n]->getHistoryPre()->setHistoryNext(_defaultHead[n]->getHistoryNext());
			}
			//初始化
			_defaultHead[n]->setHistoryPre(nullptr);
			_defaultHead[n]->setHistoryNext(_historyHead);
			_historyHead->setHistoryPre(_defaultHead[n]);
			_historyHead = _defaultHead[n];
		}
		else
		{
			//不存在
			if( _historyHead != nullptr )
			{
				_historyHead->setHistoryPre(_defaultHead[n]);
			}
			_defaultHead[n]->setHistoryNext(_historyHead);
			_historyHead = _defaultHead[n];
			_historyCount++;
		}

		//通知父类更新播放状态
		if (_parent)
		{
			//不为空
			_parent->playInit(_musicPlayer.duration());
			_parent->setIrc(_defaultHead[n]->getUrl());

			_parent->setMusicInfo(getMusicNameByUrl(_defaultHead[n]->getUrl()), "未知");
		}

	}
}

QString MusicList:: getMusicNameByUrl(QString url)
{
	QString temp = url;
	temp.replace('\\', '/');

	// 2. 切掉路径（找到最后一个 '/' 的位置）
	int slashIndex = temp.lastIndexOf('/');

	// 如果找到了 '/'，就截取它后面的部分；没找到就说明整个字符串可能就是文件名
	QString fileName = (slashIndex != -1) ? temp.mid(slashIndex + 1) : temp;

	// 3. 去掉后缀（找到文件名中最后一个 '.' 的位置）
	int dotIndex = fileName.lastIndexOf('.');

	// 4. 如果找到了 '.'，只保留它左边的部分
	if (dotIndex != -1) {
		fileName = fileName.left(dotIndex);
	}

	return fileName;
}

bool MusicList::SearchingThroughHistory(MusicMonomer* tar)
{
	//遍历整个链表
	auto current = _historyHead;
	while (current)
	{
		if (current == tar)
		{
			//找到
			return true;
		}
		//向后
		current = current->getHistoryNext();
	}
	return false;
}


void MusicList::updatePlayingGroup(QString style)
{
	/*
	*	使用哈希表可以兼容多种播放列表
	*/
	if (_mapToGroup.find(style) != _mapToGroup.end())
	{
		//寻找到目标组
		_playingGroup = _mapToGroup[style];
	}
}

void MusicList::setVolume(int tar)
{
	_volume = (double)((double)tar / (double)100);
	//更新音量
	playVolumeRefresh();
}

void MusicList::playVolumeInit()
{
	_audioOutput = new QAudioOutput((QWidget*)_parent);
	_audioOutput->setVolume(_volume); // 设置音量，范围是0到100
	_musicPlayer.setAudioOutput(_audioOutput);
}

void MusicList::playVolumeRefresh()
{
	//_musicPlayer
	_audioOutput->setVolume(_volume);
	_musicPlayer.setAudioOutput(_audioOutput);
}

bool MusicList::swapPlay()
{
	//判断是否可以转换
	//返回转换后的状态
	if (_musicPlayer.playbackState() == QMediaPlayer::StoppedState)
	{
		//处于歌曲未选中状态
		return false;
	}
	else if (_musicPlayer.playbackState() == QMediaPlayer::PlayingState)
	{
		//处于播放状态，切换成暂停
		_musicPlayer.pause();
		return false;
	}
	else if (_musicPlayer.playbackState() == QMediaPlayer::PausedState)
	{
		//切换成播放状态
		_musicPlayer.play();
		return true;
	}
}

bool MusicList::getPlay()
{
	/*
	*	问题：切换不到暂停状态 返回值始终为true
	*	定位 _playingHead 一直为true
	*/
	if (_musicPlayer.playbackState() != QMediaPlayer::PlayingState) //为空并且不为播放状态就返回false
	{
		//非播放状态返回false
		return false;
	}
	else
	{
		return true;
	}
}

void MusicList::mapInit()
{
	_mapToGroup["like"] = 1;
	_mapToGroup["local"] = 2;
	_mapToGroup["recently"] = 3;
}

void MusicList::advanceToLeft()
{
	/*
	*	根据组和当前播放模式进行切换歌曲
	*	1表示喜欢，2表示本地，3表示最近播放
	*/
	advance(1);//表示向左切换
}

void MusicList::advanceToRight()
{
	advance(2);//表示向右切换
}

void MusicList::advance(int direction)
{
	switch (_playingGroup)
	{
	case 1:
		//收藏
		nextLikeMusic(direction);
		break;
	case 2:
		//本地
		nextLocalMusic(direction);
		break;
	case 3:
		qDebug() << "还未实现";
		break;
	default:
		break;
	}
	//
}

void MusicList::nextLikeMusic(int direction)
{
	/*
	*	根据方向和播放模式进行切换歌曲
	*	playMusic(QString id, QString style)
	*/
	if (_playModel == playMode::playListLoop)
	{
		MusicMonomer* newPlayer = nullptr;
		if (direction == 2)
		{
			newPlayer = _playingHead->getLikeNext();
			if (newPlayer == nullptr)
			{
				newPlayer = _likeHead;
			}
		}
		else if( direction == 1)
		{
			newPlayer = _playingHead->getLikePre();
			if( newPlayer == nullptr)
			{
				//回到尾部
				newPlayer = _likeHead;
				while (newPlayer->getLikeNext() != nullptr)
				{
					newPlayer = newPlayer->getLikeNext();
				}
			}
		}
		playMusic(newPlayer->getMusicId(), "like");
	}
	else if( _playModel == playMode::playShuffle)
	{
		MusicMonomer* newPlayer = _likeHead;
		auto count = ((((unsigned int)rand())) % (_likeCount - 1));
		int b = 10;
		while (count--)
		{
			newPlayer = newPlayer->getLikeNext();
			if(newPlayer == nullptr)
			{
				newPlayer = _likeHead;
			}
		}
		playMusic(newPlayer->getMusicId(), "like");
	}
	else if (_playModel == playMode::playSingleSongRepeat)
	{
		//将当前播放位置设置为0
		setPosition(0);
		_musicPlayer.play();
	}
	//
}
void MusicList::nextLocalMusic(int direction)
{
	if (_playModel == playMode::playListLoop)
	{
		MusicMonomer* newPlayer = nullptr;
		if (direction == 2)
		{
			newPlayer = _playingHead->getLocalNext();
			if (newPlayer == nullptr)
			{
				newPlayer = _localHead;
			}
		}
		else if (direction == 1)
		{
			newPlayer = _playingHead->getLocalPre();
			if (newPlayer == nullptr)
			{
				//回到尾部
				newPlayer = _localHead;
				while (newPlayer->getLocalNext() != nullptr)
				{
					newPlayer = newPlayer->getLocalNext();
				}
			}
		}
		playMusic(newPlayer->getMusicId(), "local");
	}
	else if (_playModel == playMode::playShuffle)
	{
		MusicMonomer* newPlayer = _localHead;
		auto count = ((((unsigned int)rand())) % (_localCount - 1));
		int b = 10;
		while (count--)
		{
			newPlayer = newPlayer->getLocalNext();
			if (newPlayer == nullptr)
			{
				newPlayer = _localHead;
			}
		}
		playMusic(newPlayer->getMusicId(), "local");
	}
	else if (_playModel == playMode::playSingleSongRepeat)
	{
		//将当前播放位置设置为0
		setPosition(0);
		_musicPlayer.play();
	}
}

void MusicList::nextHistoryMusic(int direction)
{

}

void MusicList::setParent(TexasPlayer* parent)
{
	_parent = parent;

	//绑定信号与槽函数
	QObject::connect(&_musicPlayer, &QMediaPlayer::positionChanged, _parent, &TexasPlayer::on_playTimeChanged);

	//绑定播放完成的处理函数
	QObject::connect(&_musicPlayer, &QMediaPlayer::mediaStatusChanged, _parent, &TexasPlayer::on_playFinished);
}

void MusicList::playModeSwitch()
{
	switch (_playModel)
	{
	case playMode::playSingSong:
		_playModel = playMode::playSingleSongRepeat;
		break;
	case playMode::playSingleSongRepeat:
		_playModel = playMode::playListLoop;
		break;
	case playMode::playListLoop:
		_playModel = playMode::playShuffle;
		break;
	case playMode::playShuffle:
		_playModel = playMode::playSingSong;
		break;
	default:
		break;
	}
}

enum class playMode MusicList::getPlayMode()
{
	return _playModel;
}

void MusicList::setPercentagePosition(int tar)
{
	/*
	*	百分比
	*/

	if (_playingHead)
	{
		//当前播放不为空
		//进行位置切换
		auto time = _musicPlayer.duration();
		time = time * tar / 1000;
		setPosition(time);
	}
}

void MusicList::setPosition(qint64 tar)
{
	if (_playingHead)
	{
		//当前播放不为空
		//进行位置切换
		_musicPlayer.setPosition(tar);
	}
}

void MusicList::advanceEnd()
{
	qDebug() << "开始进行结束处理";
	//判断当前播放类型
	if (_playModel == playMode::playSingSong)
	{
		//播放类型为playSingSong，直接返回并要求顶级窗口刷新播放状态
		_parent->playRefresh();
	}
	//否则向右
	advanceToRight();
}
