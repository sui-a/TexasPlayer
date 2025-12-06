#pragma once
#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

/*
*	本地存储音乐路径数据
*	设置饿汉模式
*/
class QSqlDatabase;
class QSqlQuery;
class MusicSQL  : public QObject
{
	Q_OBJECT

public:
	friend MusicSQL* getMusicSQLInstance();
	~MusicSQL();

	//开始读取数据库
	void read(QStringList& urlOut, std::vector<bool>& isLikeOut);
	//通过url写
	void write(QString url, bool isLike);
	//删
	void deleteByUrl(QString url);

private:
	MusicSQL(QObject* parent = nullptr);
	QSqlDatabase _musicDb;
	QSqlQuery* _query;
};
inline MusicSQL* sqlPtr = nullptr;
MusicSQL* getMusicSQLInstance();