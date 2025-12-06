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
	QStringList read();
	void write(QString url);

private:
	MusicSQL(QObject* parent = nullptr);
	QSqlDatabase _musicDb;
	QSqlQuery* _query;
};
inline MusicSQL* sqlPtr = nullptr;
MusicSQL* getMusicSQLInstance();