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
private:
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
    QTimer *timer;

    bool audio_background = false;
    bool audio_role = false;
    double scale = 1.0;

    QMediaPlayer *mediaPlayer;
    QSystemTrayIcon *trayIcon;
    QMenu *menu;
    QMenu *tray_menu;
    QMenu *menu_roles;
    QMenu *menu_voice;
    QLabel *role_figure;
    QPixmap *role_pixmap;
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
    QAction *md_music;
    QAction *ly_music;
    QAction *dq_music;
    QAction *role_music;
    QAction *music_off;
    QAction *showAction;
    QAction *quitAction;

private:
    void adjustWindowSize();
    void init_window();  //窗口初始化
    void init_tray();    //任务栏菜单
    void update_lists(); //
private slots:
    void set_role();
    void set_audio();
    void updateAnimation();

public:
    PetWindow();
    void showWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override; //重写上下文菜单
    void mousePressEvent(QMouseEvent *event) override;        // 重写鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;         // 重写鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override;      // 重写鼠标释放事件
    void keyPressEvent(QKeyEvent *event) override;            // 重写键盘敲击事件
};

#endif // PETWINDOW_H
