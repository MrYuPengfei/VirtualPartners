#ifndef PETWINDOW_H
#define PETWINDOW_H
#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QFileInfoList>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer> // Qt 的多媒体模块用于音频播放
/*
使用 Qt 的信号和槽机制来处理用户交互和定时器事件。
使用 Qt 的图形视图框架来创建自定义窗口和动画效果。
使用 Qt 的多媒体模块来代替 pygame 的音频播放功能。
使用 Qt 的 QSystemTrayIcon 类来实现系统托盘图标和菜单。
使用 Qt 的 QSettings 或自定义的配置文件解析来代替 yaml 配置文件的处理。
*/
class PetWindow : public QMainWindow
{
    Q_OBJECT
public:
    PetWindow();
    ~PetWindow();

private:
    QTimer *timer;
    QMediaPlayer *backgroundMusicPlayer;
    QMediaPlayer *roleVoicePlayer;
    QAudioOutput *audioOutput1;
    QAudioOutput *audioOutput2;
    QString base_path = QString("data");
    QString music_path = QString("music");
    QString image_path = QString("png");
    QString bg_music = QString("稻妻");
    QString role_name = QString("八重神子");
    QFileInfoList image_list;
    QFileInfoList talk_list;
    QFileInfoList know_list;
    QPoint lastMousePosition; // 上一次鼠标的位置
    bool isDragging;          // 是否正在拖动窗口
    // QPoint mouse_drag_pos;
    // bool is_follow_mouse; // 鼠标是否跟随标志

    // bool audio_background = false;
    // bool audio_role = false;
    double scale = 1.0;
    bool isPlayAudio = false;
    bool isPlayBackgroundAudio = false;
    bool isPlayRoleAudio = true;
    QSystemTrayIcon *trayIcon;
    QMenu *menu;
    QAction *knowing;
    QAction *talking;
    QAction *hideAction;
    QAction *showAction;
    QAction *quitAction;
    QMenu *tray_menu;
    QMenu *menu_roles;
    QMenu *menu_voice;
    QLabel *role_figure;
    QPixmap role_pixmap;
    qsizetype role_figure_index = 0;
    int screenheight;
    int screenwidth;
    int wt = 300;
    int ht = 300;
    int pos_x;
    int pos_y;
    int now_time;

    QAction *bbl;  //芭芭拉
    QAction *bcsz; //八重神子
    QAction *bnt;  //班尼特
    QAction *dan;  //迪奥娜

    QAction *actionPlayMontdidor; //menu_voice->addAction("播放蒙德");
    QAction *actionPlayLiyue;     //= menu_voice->addAction("播放璃月");
    QAction *actionPlayInazuma;   //= menu_voice->addAction("播放稻妻");
    QAction *actionPlayRoleVoice; // = menu_voice->addAction("角色语音");
    QAction *actionStopAll;       //= menu_voice->addAction("关闭所有音乐");

private:
    void adjustWindowSize();
    void init_window();  //窗口初始化
    void init_tray();    //任务栏菜单
    void update_lists(); //
    void greeting();     //问好，并调整当前时间)
    void playBackgroundMusic(const QString &musicName);
private slots:
    void set_role();
    void updateAnimation();
private slots:
    void onKnowTriggered();
    void onTalkTriggered();
    void onQuitTriggered();
    void onHideTriggered();
    void onShowTriggered();
    void onPlayMontdidor(bool checked);
    void onPlayLiyue(bool checked);
    void onPlayInazuma(bool checked);
    void onRoleVoice(bool checked);
    void onStopAllVocie(bool checked);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override; //重写上下文菜单
    void mousePressEvent(QMouseEvent *event) override;        // 重写鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;         // 重写鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override;      // 重写鼠标释放事件
    void keyPressEvent(QKeyEvent *event) override;            // 重写键盘敲击事件
    // void closeEvent(QCloseEvent *event) override; // 关闭窗口时隐藏而不是退出程序：
};

#endif // PETWINDOW_H
