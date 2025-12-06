#include "RecBox.h"
#include "showCasePage.h"
RecBox::RecBox(QWidget *parent)
	: QWidget(parent)
	, _row(2)
	, _col(8)
	, _contentIndex(0)
{
	ui.setupUi(this);
}

RecBox::~RecBox()
{}

void RecBox::buttonInit(int row)
{
	//根据参数进行初始化
	if (1 == row)
	{
		//将下列表隐藏
		ui.bodyDown->hide();
		_row = 1;
		_col = 4;
	}
	else
	{
		setMinimumSize(width(), height() * 2);
		_row = 2;
		_col = 8;
	}
}

void RecBox::addPathContent(QString path, QString text)
{
	//通过路径添加内容
	int n1 = _contentPath.size();
	if(n1 == 0)
	{
		_contentPath.push_back(std::vector<QString>());
		_contentText.push_back(std::vector<QString>());
		n1++;
	}

	int n2 = _contentPath[n1 - 1].size();
	if (n2 >= _col)
	{
		//切换到下一组
		_contentPath.push_back(std::vector<QString>());
		_contentText.push_back(std::vector<QString>());
		n1++;
		n2 = 0;
	}

	//添加内容
	_contentPath[n1 - 1].push_back(path);
	_contentText[n1 - 1].push_back(text);
	
	//刷新窗口
	refreshShowCasePage();
	//进行位置判断
}

void RecBox::refreshShowCasePage()
{
	/*
	*	删除原有的窗口
	*	创建新窗口来展示内容
	*	!: 不进行缓存操作
	*/

	//遍历本地所有子窗口
	QList<showCasePage*> subControls = findChildren<showCasePage*>();
	for (auto subControl : subControls)
	{
		//全部释放
		delete subControl;
	}

	//重新加载控件
	for (int i = 0; i < _contentPath[_contentIndex].size(); i++)
	{
		//根据本地格式进行排布
		if (1 == _row)
		{
			//单行排布
			auto show = new showCasePage(ui.bodyUp);

			//设置文本和图像
			show->setBodyText(_contentText[_contentIndex][i]);
			show->setImage(_contentPath[_contentIndex][i]);

			show->move(show->width() * i, 0);
			show->show();
		}
		else if (2 == _row)
		{
			//双行排布
			showCasePage* show = nullptr;
			if (0 == i % 2)
			{
				//上
				show = new showCasePage(ui.bodyUp);
				show->move(show->width() * ((int)(i / 2)), 0);
			}
			else
			{
				//下
				show = new showCasePage(ui.bodyDown);
				show->move(show->width() * ((int)(i / 2)), 0);
			}
			show->setBodyText(_contentText[_contentIndex][i]);
			show->setImage(_contentPath[_contentIndex][i]);
			show->show();
		}
		//if
	}
	//for
}

void RecBox::on_rightButton_clicked()
{
	/*
	*	右侧按钮按下，组索引循环增加
	*	重新调用显示函数
	*/

	_contentIndex++;
	if (_contentIndex >= _contentPath.size())
	{
		//索引超出数量
		_contentIndex = 0;
	}
	

	//刷新
	refreshShowCasePage();
}

void RecBox::on_leftButton_clicked()
{
	/*
	*	左侧按钮按下，组索引循环递减
	*	重新调用显示函数
	*/

	_contentIndex--;
	if (0 > _contentIndex)
	{
		//索引处于最低值
		_contentIndex = _contentPath.size() - 1;
	}


	//刷新
	refreshShowCasePage();
}