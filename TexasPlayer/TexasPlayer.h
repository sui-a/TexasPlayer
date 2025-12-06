#pragma once
#include <QtWidgets/QWidget>
#include "ui_TexasPlayer.h"
#include <QMediaPlayer>
class lrcInterface;
class volume;
class TexasPlayer : public QWidget
{
    Q_OBJECT
    
public:
    TexasPlayer(QWidget *parent = nullptr);
    ~TexasPlayer();

    //设置标题栏
    void titleBlockSet();
    //加载系统托盘
    void systemTraySet();
    void shadowSet();
    //初始化logo
    void logoInit();
    //右上角小标初始化
    void smallLabelInit();
    //搜索栏初始化
	void searchBarInit();
    //初始化任务栏图标
    void taskBarInit();
    //按钮初始化
    void buttonLeftInit();
    //推荐界面初始化
    void recoInit();
    //我喜欢初始化
    void likePageInit();
    //本地初始化
    void localInit();
    //最近初始化
    void recentlyInit();
    //进度条初始化
    void processInit();
    //初始化播放时间
    void playTimeInit();
    //音量调节窗口初始化
    void volumeInit();
    void MusicInit();
    void playInit(qint64 time);
    //歌词界面初始化
    void lrcInit();


    //
    //内容测试接口
    void contentTesting();  //推荐页内容测试
    void recentlyTesting();  //收藏页内容测试
    void processTesting();  //进度条业务测试

    //更新
    //更新进度条总时间
	void playTotalTimeRefresh(qint64 time);
    //更新进度条当前时间
    void playCurrentTimeRefresh(qint64 time);
    //更新按键状态
    void playRefresh();
    void playModeRefresh();

    //设置内部进度条
    void setPregress(int target);

    //获取窗口可见位置的宽高
	QPoint getVisiblePos();

    //通过路径读取歌词
    void setIrc(QString path);
    
public slots:
    //播放时间接收槽函数
    void on_playTimeChanged(qint64 time);
    //进度条点击的处理函数
    void on_precessClicked(int press); //获取值0-1000

    //播放结束处理
	void on_playFinished(QMediaPlayer::MediaStatus status);

private slots:
    void on_quit_clicked();
    void on_play_clicked();
    void btfromFeedback(int num);
    void on_volume_clicked();
    void on_add_clicked();
    void on_skin_clicked();
    void on_playUp_clicked();
    void on_playDown_clicked();
    void on_playMode_clicked();
    void on_lyrics_clicked();
protected:
    //对鼠标点击事件进行重载
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    //对鼠标移动事件进行重载
    void mouseMoveEvent(QMouseEvent* event) override;
    //对显示事件进行重写
    void showEvent(QShowEvent* event) override;

private:
    Ui::TexasPlayerClass ui;

    QPoint _startPos;
    bool _isDragging;

    //音量窗口
    volume* _volume;

    //测试定时器
    QTimer* _timer;

    qint64 _totalTime;

    //歌词接口
    lrcInterface* _lrc;
};


