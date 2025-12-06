#include <qsqlerror.h>
#include "MusicSQL.h"

MusicSQL::MusicSQL(QObject* parent)
	: QObject(parent)
	, _query(nullptr)
{
	// 1. 添加数据库驱动
	_musicDb = QSqlDatabase::addDatabase("QSQLITE");
	// 2. 设置数据库名称（实际上就是文件路径）
	// 如果文件不存在，Qt 会自动创建；如果存在，则直接打开
	_musicDb.setDatabaseName("TexasMusic.db");
	_query = new QSqlQuery(_musicDb);
	// 3. 打开连接
	if (!_musicDb.open())
	{
		qDebug() << "Error: connection with database failed 16";
	}

	QString createSql = "CREATE TABLE IF NOT EXISTS music ("
		"url TEXT UNIQUE)";  //创建或者打开一个只存储唯一url的表

	if (!_query->exec(createSql))
	{

		qDebug() << "建表失败: " << _query->lastError().text();
		exit(0);
	}
}

MusicSQL::~MusicSQL()
{}

MusicSQL* getMusicSQLInstance()
{
	if (nullptr == sqlPtr)
	{
		sqlPtr = new MusicSQL();
	}
	return sqlPtr;
}


QStringList MusicSQL::read()
{
	QStringList out;
	if (!_query->exec("SELECT * FROM music")) 
	{
		qDebug() << "读取失败:" << _query->lastError().text();
	}
	else
	{
		// 2. 开始逐行读取
		// query.next() 每调用一次，指针就会移动到下一行数据
		// 当没有更多数据时，它会返回 false，循环结束
		
		while (_query->next())
		{
			// 3. 获取具体字段的值
			// 方式 A：通过字段名获取 (最推荐，清晰不易错)
			QString url = _query->value("url").toString();

			// 如果你有 id 字段
			// int id = query.value("id").toInt();

			// 4. 在这里处理你的数据 (比如打印，或者添加到列表里)
			out.append(url);
		}
	}
	return out;
}

void MusicSQL::write(QString url)
{
	QString insertSql = "insert or ignore into music (url) values (':url')"; 
	insertSql.replace(":url", url);

	if (!_query->exec(insertSql))
	{
		qDebug() << "插入失败: 64" << _query->lastError().text();
	}
}

