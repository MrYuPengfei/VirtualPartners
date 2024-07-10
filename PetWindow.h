#ifndef PETWINDOW_H
#define PETWINDOW_H
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QAudioOutput>
#include <QCursor>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include <QRandomGenerator>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer> // Qt 的多媒体模块用于音频播放
class PetWindow : public QMainWindow
{
    Q_OBJECT
public:
    PetWindow();
    ~PetWindow();

private:
    QString base_path;
    QString music_path;
    QString image_path;
    QString bgm_path;
    QString bgm_name;
    QString role_name;
    QFileInfoList image_list;
    QFileInfoList talk_list;
    QFileInfoList know_list;
    QTimer *timer;
    QMediaPlayer *backgroundMusicPlayer;
    QMediaPlayer *roleVoicePlayer;
    QAudioOutput *audioOutput1;
    QAudioOutput *audioOutput2;
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
    QAction *helpAction;
    QAction *quitAction;
    QMenu *tray_menu;
    QMenu *menu_roles;
    QMenu *menu_voice;
    QMenu *menu_bgms;
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
    QList<QAction *> roles;
    QList<QAction *> bgms;
    QActionGroup *group1;
    QActionGroup *group2;
    // QAction *actionPlayMontdidor; //menu_voice->addAction("播放蒙德");
    // QAction *actionPlayLiyue;     //= menu_voice->addAction("播放璃月");
    // QAction *actionPlayInazuma;   //= menu_voice->addAction("播放稻妻");
    QAction *actionPlayRoleVoice;
    QAction *actionPlayBackgroundMusic; // = menu_voice->addAction("角色语音");
    QAction *actionStopAll;       //= menu_voice->addAction("关闭所有音乐");

private:
    void adjustWindowSize();
    void init_window();  //窗口初始化
    void init_tray();    //任务栏菜单
    void update_lists(); //
    void greeting();     //问好，并调整当前时间)
    void playBackgroundMusic(const QString &musicName);
private slots:
    void set_bgm(bool checked);
    void set_role(bool checked);
    void updateAnimation();
private slots:
    void onKnowTriggered();
    void onTalkTriggered();
    void onQuitTriggered();
    void onHideTriggered();
    void onShowTriggered();
    void onHelpTriggered();
    void onPlayBackgroundMusic(bool checked);
    void onPlayRoleVoice(bool checked);
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
