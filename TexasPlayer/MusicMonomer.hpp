#pragma once
#include <qstring.h>
#include <qurl.h>

//音乐遍历模块
struct MusicInfo
{
	QString _musicName;
	QString _singer; //先全部设置未知
	bool _isLike;
	QString _id;
};

class QMediaPlayer;
class MusicMonomer
{
public:
	MusicMonomer(QString url);
	~MusicMonomer();

	void getMusicId(QString& id) const;
	QString getMusicId() const;
	QString getUrl() const;
	void getUrl(QString& url) const;

	void play(QMediaPlayer& player);

	void setLikeNext(MusicMonomer* next);
	void setLocalNext(MusicMonomer* next);
	void setLikePre(MusicMonomer* pre);
	void setLocalPre(MusicMonomer* pre);
	void setHistoryNext(MusicMonomer* next);
	void setHistoryPre(MusicMonomer* pre);

	MusicMonomer* getLocalNext();
	MusicMonomer* getLocalPre();
	MusicMonomer* getLikeNext();
	MusicMonomer* getLikePre();
	MusicMonomer* getHistoryNext();
	MusicMonomer* getHistoryPre();


	void getMusicInfo(MusicInfo* out);
	MusicInfo getMusicInfo();

	//修改本地状态
	void setLike(bool isLike);
private:
	//id
	QString _id;
	//地址
	QString _url;
	//时间
	qlonglong _time;

	//喜欢下一个
	MusicMonomer* _likeNext;
	MusicMonomer* _likePre;
	//本地
	MusicMonomer* _localNext;
	MusicMonomer* _localPre;
	//历史
	MusicMonomer* _historyNext;
	MusicMonomer* _historyPre;

	bool _isLike;
};

