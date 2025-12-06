#pragma once
#include <vector>
#include <QMediaPlayer>
#include <string>
#include <functional>
#include <unordered_map>

//音乐管理模块
class MusicMonomer;
struct MusicInfo;
class volume;
class TexasPlayer;

//模式枚举
enum class playMode
{
	playSingSong,  //单曲播放一次
	playSingleSongRepeat,  //单曲循环
	playListLoop,  //当前列表循环
	playShuffle   //随机播放
};


class MusicList
{
	//
	friend MusicList* createMusicList();
public:
	~MusicList();
	//映射表初始化
	void mapInit();
	//你牌坊初始化
	void playInit();
	//根据数据库初始化
	void dbInit();

	//本地音乐链表接口
	void addMusic(QStringList url);
	std::vector<MusicInfo> getContent(QString style);

	//设置喜欢
	void setLike(QString id, bool like);
	//播放音乐
	void playMusic(QString id, QString style);
	//音量处理函数
	void setVolume(int tar);
	//设置目标窗口指针
	void setParent(TexasPlayer* parent);

	//音量更新
	void playVolumeInit();
	void playVolumeRefresh();
	//更新音乐组号
	void updatePlayingGroup(QString style);

	//切换播放状态
	bool swapPlay();
	bool getPlay(); //获取
	//根据列表切换组
	void advanceToLeft();
	void advanceToRight();
	void advanceEnd();//结束自动跳转

	//切换播放模式
	void playModeSwitch();
	enum class playMode getPlayMode();

	//修改播放的音乐
	void nextLikeMusic(int direction);
	void nextLocalMusic(int direction);
	void nextHistoryMusic(int direction);

	//切换当前播放位置
	void setPosition(qint64 tar);
	//百分比设置
	void setPercentagePosition(int tar);

private:
	MusicList();
private:
	void advance(int direction);
	bool SearchingThroughHistory(MusicMonomer* tar);
	
	//根据方向切换歌曲


private:
	//播放列表映射 url->下标
	std::unordered_map<QString, int> _mapToGroup;

	//播放列表处理函数
	//

	//使用map存储url与音乐下标映射

	//使用map存储id和下标映射
	std::unordered_map<QString, int> _mapToInt;
	//

	//使用链表存储主音乐列表中失效的元素，后续回收利用

	//使用四层链表存储音乐类
	//默认链表
	std::vector<MusicMonomer*> _defaultHead;
	//我喜欢
	MusicMonomer* _likeHead;
	int _likeCount;
	//本地音乐
	MusicMonomer* _localHead;
	int _localCount;
	//最近播放
	MusicMonomer* _historyHead;
	int _historyCount;

	//正在播放的目标
	MusicMonomer* _playingHead;
	//播放目标的组号
	int _playingGroup;  //0表示无组, 1表示喜欢，2表示本地，3表示最近播放
	//播放器由本地维护
	QMediaPlayer _musicPlayer;
	QAudioOutput* _audioOutput;

	//音量
	double _volume;

	//设置顶级窗口指针
	TexasPlayer* _parent;

	//播放模式
	playMode _playModel;
};

inline MusicList* musiclist = nullptr;

//饿汉式单例
MusicList* createMusicList();