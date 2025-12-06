#include "TexasPlayer.h"
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QTimer>
#include<QFileDialog>
#include "lrcInterface.h"
#include "MusicList.h"
#include "volume.h"

TexasPlayer::TexasPlayer(QWidget *parent)
    : QWidget(parent)
    , _isDragging(false)
    , _timer(nullptr)
    , _volume(nullptr)
    , _totalTime(0)
{
    ui.setupUi(this);

    titleBlockSet();
    systemTraySet();
    shadowSet();

    //主题初始化
    
	logoInit();
    smallLabelInit();
    searchBarInit();
    taskBarInit();
    buttonLeftInit();
    recoInit();
    likePageInit();
    localInit();
    recentlyInit();
    volumeInit();
    MusicInit();
    processInit();
    lrcInit();
    //进度条测试
    //processTesting(); //
}

void TexasPlayer::MusicInit()
{
    createMusicList()->setParent(this);
}
TexasPlayer::~TexasPlayer()
{}

void TexasPlayer::titleBlockSet()
{
    //关闭标题栏 设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);

    //设置窗口图标
    setWindowIcon(QIcon(":/texas_head.jpg"));
}


void TexasPlayer::systemTraySet()
{
    //构造系统托盘
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    //设置图标
    trayIcon->setIcon(QIcon(":/texas_head.jpg"));

    //绑定左键单机时间
    connect(trayIcon, &QSystemTrayIcon::activated, this,
        [this, trayIcon](QSystemTrayIcon::ActivationReason reason)
        {
            if (reason == QSystemTrayIcon::DoubleClick) //判断是否为双击
            {
                //将窗口激活至窗前
                this->show();
                this->activateWindow();
            }
            else if (reason == QSystemTrayIcon::Context)
            {
                //将菜单显示至当前鼠标位置
                if (trayIcon->contextMenu()) //判断菜单是否存在
                {
                    trayIcon->contextMenu()->exec(QCursor::pos());
                }
                //
            }
            //
        });

    //创建菜单
    QMenu* trayMenu = new QMenu(this);
    //创建内部的动作
    QAction* openWindowAction = new QAction("打开", this);
    connect(openWindowAction, &QAction::triggered, this,
        [this]()
        {
            this->show();
            this->activateWindow();
        });

    QAction* quitAction = new QAction("退出", this);
    connect(quitAction, &QAction::triggered, this,
        [this]()
        {
            this->close();
        });

    //将动作添加至菜单
    trayMenu->addAction(openWindowAction);
    trayMenu->addAction(quitAction);

    //将菜单绑定至托盘
    trayIcon->setContextMenu(trayMenu);

    //将托盘传递至系统进行实例化
    trayIcon->show();
}

void TexasPlayer::on_quit_clicked()
{
	this->close();
}

void TexasPlayer::mousePressEvent(QMouseEvent* event)
{
    //判断点击的位置是否为可修改状态
    if (event->button() == Qt::LeftButton)
    {
       
        //记录每一次鼠标点击的位置相对于窗口左上角的值
        _startPos = event->globalPosition().toPoint() - geometry().topLeft();
        

        if(_startPos.y() <= 50 + 9 && _startPos.y() >= 0 + 9) //标题栏高度为50像素, 当点击位置小于等于五十时视为可拖动
        {
            _isDragging = true;
            setCursor(Qt::ClosedHandCursor);
        }
        //
        event->accept();
    }
}
void TexasPlayer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        _isDragging = false; // 结束拖拽
        setCursor(Qt::ArrowCursor); // 恢复默认光标
        event->accept();
    }
}
void TexasPlayer::mouseMoveEvent(QMouseEvent* event)
{
    if (_isDragging)
    {
        // 更新窗口位置
        move(event->globalPosition().toPoint() - _startPos);
        event->accept();
    }
}

void TexasPlayer::shadowSet()
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    //setStyleSheet("#TexasPlayerClass {background-color: rgba(255, 255, 255, 0);}");  //设置失效问题
    

    
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor("#000000");
    //设置阴影圆角
    shadow->setBlurRadius(10);
    //给嵌套QWidget设置阴影
    setGraphicsEffect(shadow);
}

void TexasPlayer::logoInit()
{
    //加载图像
    //ui.logoInoc->setAlignment(Qt::AlignCenter); //设置内容居中
    //QPixmap logoPixmap(":/texas_head.jpg");
    //if (logoPixmap.isNull())
    //{
    //    //加载失败
    //    qDebug() << "fail to loading pixmap";
    //    exit(-1);
    //}

    ////通过qpalette设置背景
    //QPalette palette = ui.logoInoc->palette();
    //palette.setBrush(QPalette::Window, QBrush(logoPixmap.scaled({50, 50}, Qt::KeepAspectRatio)));
    //ui.logoInoc->setPalette(palette);

    //ui.logoInoc->setAutoFillBackground(true);
    //ui.logoInoc->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void TexasPlayer::smallLabelInit()
{
	ui.quit->setIcon(QIcon(":/shut.png"));
	ui.max->setIcon(QIcon(":/maxMize.png"));
    ui.min->setIcon(QIcon(":/min.png"));
    //ui.skin->setIcon(QIcon(":/clothes.png"));
}

void TexasPlayer::searchBarInit()
{
	ui.search->setPlaceholderText("搜索");
    //ui.lineEdit->setStyleSheet("background-color: #808080;");
}

void TexasPlayer::taskBarInit()
{
    ui.playMode->setIcon(QIcon(":/playSingleSong.png"));

    ui.playUp->setIcon(QIcon(":/lessThan.png"));
    ui.playDown->setIcon(QIcon(":/greaterThan.png"));
    ui.play->setIcon(QIcon(":/play.png"));
    ui.volume->setIcon(QIcon(":/volume.png"));
    ui.add->setIcon(QIcon(":/add.png"));
    ui.lyrics->setIcon(QIcon(":/lyrics.png"));
}

void TexasPlayer::buttonLeftInit()
{
    ui.one->setIconAndText(":/likeIcon.png", "音乐推荐", 0);
    ui.two->setIconAndText(":/tv.png", "个性电台", 1);
    ui.three->setIconAndText(":/musicNote.png", "音乐馆", 2);
    
    ui.four->setIconAndText(":/musicLike.png", "我喜欢", 3);
    ui.five->setIconAndText(":/download.png", "本地音乐", 4);
    ui.six->setIconAndText(":/history.png", "最近播放", 5);

    //进行槽信号绑定
    //QList<QWidget *> widgets = parentWidget.findChildren<QWidget *>("widgetname"); 
    QList<btfrom*> children = this->findChildren<btfrom*>();
    for (int i = 0; i < children.size(); i++)
    {
        connect(children[i], &btfrom::modifiesMainInterface,
            this, &TexasPlayer::btfromFeedback);
    }
    //for
    ui.one->setWindowSelect(true);

    ui.mainWindow->setCurrentIndex(0);

}

void TexasPlayer::btfromFeedback(int num)
{
    //进行子控件遍历  清除并修改现选中的按钮
    QList<btfrom*> children = this->findChildren<btfrom*>();
    for (auto current : children)
    {
        //进行参数判断
        if (current->getNumber() == num)
        {
            //选中
            current->setWindowSelect(true);
        }
        else
        {
            //恢复
            current->setWindowSelect(false);
        }
        //if
    }
    //for

    //根据页号进行修改
    ui.mainWindow->setCurrentIndex(num);

    //进行界面更新
    if(num > 2)
    {
        ((myGeneral*)ui.mainWindow->currentWidget())->refresh();
    }
}

void TexasPlayer::contentTesting()
{
    /*
    *   //添加测试内容
    */
    ui.recoPageBodyOneBody->addPathContent(":/image/001.jpg", "推荐-001");
    ui.recoPageBodyOneBody->addPathContent(":/image/002.jpg", "推荐-002");
    ui.recoPageBodyOneBody->addPathContent(":/image/003.jpg", "推荐-003");
    ui.recoPageBodyOneBody->addPathContent(":/image/004.jpg", "推荐-004");

    ui.recoPageBodyOneBody->addPathContent(":/image/005.jpg", "推荐-005");
    ui.recoPageBodyOneBody->addPathContent(":/image/006.jpg", "推荐-006");
    ui.recoPageBodyOneBody->addPathContent(":/image/007.jpg", "推荐-007");
    ui.recoPageBodyOneBody->addPathContent(":/image/008.jpg", "推荐-008");

    ui.recoPageBodyTwoBody->addPathContent(":/image/005.jpg", "补给-001");
    ui.recoPageBodyTwoBody->addPathContent(":/image/006.jpg", "补给-002");
    ui.recoPageBodyTwoBody->addPathContent(":/image/007.jpg", "补给-003");
    ui.recoPageBodyTwoBody->addPathContent(":/image/008.jpg", "补给-004");
    ui.recoPageBodyTwoBody->addPathContent(":/image/009.jpg", "补给-005");
    ui.recoPageBodyTwoBody->addPathContent(":/image/010.jpg", "补给-006");
    ui.recoPageBodyTwoBody->addPathContent(":/image/011.jpg", "补给-007");
    ui.recoPageBodyTwoBody->addPathContent(":/image/012.jpg", "补给-008");

    ui.recoPageBodyTwoBody->addPathContent(":/image/001.jpg", "补给-009");
    ui.recoPageBodyTwoBody->addPathContent(":/image/002.jpg", "补给-010");
    ui.recoPageBodyTwoBody->addPathContent(":/image/003.jpg", "补给-011");
    ui.recoPageBodyTwoBody->addPathContent(":/image/004.jpg", "补给-012");
    ui.recoPageBodyTwoBody->addPathContent(":/image/013.jpg", "补给-013");
    ui.recoPageBodyTwoBody->addPathContent(":/image/014.jpg", "补给-014");
    ui.recoPageBodyTwoBody->addPathContent(":/image/015.jpg", "补给-015");
}

void TexasPlayer::recoInit()
{
    /*
    * //推荐界面初始化
    */

    ui.recoPageBodyTwoBody->buttonInit(2);
    ui.recoPageBodyOneBody->buttonInit(1);

    //添加内容
    contentTesting();
}

void TexasPlayer::likePageInit()
{
    /*
    * //我的音乐界面初始化
    */

    //对我的音乐界面初始化
    ui.likePageFour->headAndLogoInit("我的音乐", ":/texas_head.jpg", "like");
	ui.likePageFour->setParent(this);
}

void TexasPlayer::localInit()
{
    /*
    * //本地音乐界面初始化
    */
    ui.localPageFive->headAndLogoInit("本地音乐", ":/texas_head.jpg", "local");
    ui.localPageFive->setParent(this);

}

void TexasPlayer::recentlyInit()
{
    /*
    * //最近界面初始化
    */
    ui.recentPageSix->headAndLogoInit("最近播放", ":/texas_head.jpg", "recently");
    ui.recentPageSix->setParent(this);
}

void TexasPlayer::recentlyTesting()
{
    //将generalRe***添加一份进入musicList
    //ui.likePageFour->contentAdd("稻香", "周杰伦");
}

void TexasPlayer::playRefresh()
{
    auto ret = createMusicList()->getPlay();
    if (ret)
    {
        //在播放状态 换成点击暂停按钮
        ui.play->setIcon(QIcon(":/pause.png"));
    }
    else
    {
        //暂停或者没有选中歌曲 换成点击播放按钮
        ui.play->setIcon(QIcon(":/play.png"));
    }
}

void TexasPlayer::on_play_clicked()
{
    createMusicList()->swapPlay();
    playRefresh();
}


void TexasPlayer::processTesting()
{
    /*
    *   创建全局定时器自加数据进行调整进度条
    */
    _timer = new QTimer(this);
    _timer->setInterval(100); //设置触发时间

    //绑定信号槽
    connect(_timer, &QTimer::timeout, this, [this]()-> void{
        static int timerCount = 0;

        if (timerCount == 1000)
            this->_timer->stop();
        //传入位置
        ui.pressBody->setProgress(timerCount);
        timerCount += 100;
        
        });
    _timer->start();
}

void TexasPlayer::volumeInit()
{
    _volume = new volume(this);

    //设置为弹窗模式
    _volume->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);//
    _volume->setAttribute(Qt::WA_TranslucentBackground, true); //实现顶级窗口透明
}


void TexasPlayer::on_volume_clicked()
{
    //弹出窗口
    if (_volume != nullptr)
    {
        //获取按钮全局位置
        QPoint point = ui.volume->mapToGlobal(QPoint(0, 0));
        point = point - QPoint(30, 235);
        _volume->move(point);
        _volume->show(); // 轮询模式弹出 事件处理交于主线程
    }
}

void TexasPlayer::on_add_clicked()
{
    //创建文件模态对话框并阻塞显示
    QFileDialog fileDevice(this);
    fileDevice.setWindowTitle("选择你的音乐");
    //设置选项
    const QStringList filters({
    "音乐文件 (*.mp3 *.mp4)",
    "歌词文件 (*.lrc)",
	"所有文件 (*.*)"
        });

    fileDevice.setFileMode(QFileDialog::ExistingFiles);
    
    fileDevice.setNameFilters(filters);
    fileDevice.exec();

    //将路径添加进去
    auto myMusic = createMusicList();
    //获取选中的文件
    if (!fileDevice.selectedFiles().empty())
    {
        myMusic->addMusic(fileDevice.selectedFiles());
    }
}


void TexasPlayer::on_skin_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("外观");
    dialog.resize(300, 150);
    QLabel label(&dialog);
    label.setText("功能还未完善");
    dialog.exec();
}

void TexasPlayer::on_playUp_clicked()
{
    createMusicList()->advanceToLeft();
}

void TexasPlayer::on_playDown_clicked()
{

    createMusicList()->advanceToRight();
}

void TexasPlayer::on_playMode_clicked()
{
    /*
    *   传递更新
    */
    createMusicList()->playModeSwitch();
    playModeRefresh();
}


void TexasPlayer::playModeRefresh()
{
    auto ret = createMusicList()->getPlayMode();
    switch (ret)
    {
    case playMode::playSingSong:
        ui.playMode->setIcon(QIcon(":/playSingleSong.png"));
        break;
    case playMode::playSingleSongRepeat:
        ui.playMode->setIcon(QIcon(":/playSingleSongRepeat.png"));
        break;
    case playMode::playListLoop:
        ui.playMode->setIcon(QIcon(":/playListLoop.png"));
        break;
    case playMode::playShuffle:
        ui.playMode->setIcon(QIcon(":/randomness.png"));
        break;
    default:
        qDebug() << "出现错误";
        break;
    }
}

void TexasPlayer::playInit(qint64 time)
{
    playTotalTimeRefresh(time);
    //初始化当前时间
    playCurrentTimeRefresh(0);

    //修改进度条位置
    setPregress(0);
}

void TexasPlayer::playTotalTimeRefresh(qint64 time)
{
    QString minute = QString("%1").arg(time / 1000 / 60, 2, 10, QChar('0'));
    QString seconds = QString("%1").arg(time / 1000 % 60, 2, 10, QChar('0'));
	ui.targetTime->setText(minute + ":" + seconds); //设置总时间
	_totalTime = time; //记录总时间
}

void TexasPlayer::setPregress(int target)
{
    /* 
    *   0 -> 1000
    *   多次判断是否有效，放置出现越界
    */

    if (1000 < target)
    {
        target = 1000;
    }
    else if (target < 0)
    {
        target = 0;
    }

    ui.pressBody->setProgress(target);
    //
}

void TexasPlayer::processInit()
{
    /*
    *   将进度条数据初始化为0
    */
	setPregress(0);
    playTimeInit();
    //绑定点击处理函数
    connect(ui.pressBody, &playbackProgressBar::pregressClicked, this, &TexasPlayer::on_precessClicked);
}

void TexasPlayer::playTimeInit()
{
    /*
    *   将播放时间初始化为0
    */

    playTotalTimeRefresh(0);
    playCurrentTimeRefresh(0);
}

void TexasPlayer::on_precessClicked(int press)
{
    //通知切换
    createMusicList()->setPercentagePosition(press);
}

void TexasPlayer::playCurrentTimeRefresh(qint64 time)
{
    QString minute = QString("%1").arg(time / 1000 / 60, 2, 10, QChar('0'));
    QString seconds = QString("%1").arg(time / 1000 % 60, 2, 10, QChar('0'));
    ui.currentTime->setText(minute + ":" + seconds);
}

void TexasPlayer::on_playTimeChanged(qint64 time)
{
    playCurrentTimeRefresh(time);
    int target = (time * 1000) / _totalTime; //计算百分比
    setPregress(target);
}

void TexasPlayer::on_playFinished(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        //播放结束导致的状态改变
        //自动跳转下一个
        createMusicList()->advanceEnd();
    }
}

void TexasPlayer::lrcInit()
{
    //
    _lrc = new lrcInterface(this);
    _lrc->hide();//默认隐藏
    //初始化歌词
    _lrc->setIrcText(4, "暂时未提供歌词");
}


void TexasPlayer::showEvent(QShowEvent* event)
{
    _lrc->lrcInit((width() - ui.background->width()) / 2, (height() - ui.background->height()) / 2, ui.background->width(), ui.background->height());
}

void TexasPlayer::on_lyrics_clicked()
{
    _lrc->showInit(width(), height()); //显示前设置设置位置
    _lrc->showExercise();  //开始运动
}

QPoint TexasPlayer::getVisiblePos()
{
    /*
    *   获取可见窗口宽高并返回
    */

    int width = ui.background->width();
    int height = ui.background->height();
    return QPoint(width, height);
}


void TexasPlayer::setIrc(QString path)
{
    _lrc->readLyrics(path);
}