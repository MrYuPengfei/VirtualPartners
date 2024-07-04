#include "PetWindow.h"
#include <QAction>
#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer> // Qt 的多媒体模块用于音频播放
PetWindow::PetWindow()
{
    // 获取屏幕尺寸
    QRect screenRect = this->screen()->geometry();
    this->screenheight = screenRect.height();
    this->screenwidth = screenRect.width();
    // 使用 QRandomGenerator 生成随机数
    this->pos_x = screenRect.width() - 400;
    this->pos_y = screenRect.height() - 800;
    // 设置窗口位置
    this->setGeometry(this->pos_x, this->pos_y, this->width(), this->height());
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    // setAutoFillBackground(false); //设置窗口背景透明
    setAutoFillBackground(true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    // 获取静态资源列表
    this->update_lists();

    // 初始化音频播放器
    // 注意：需要根据实际音频文件路径进行设置
    this->mediaPlayer = new QMediaPlayer();
    this->mediaPlayer->setProperty("Volume", 50); // 设置音量

    // 初始化定时器
    this->now_time = QDateTime::currentDateTime().time().hour();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PetWindow::updateAnimation);
    timer->start(60); // 设置定时器时间间隔

    this->init_tray();
    this->init_window();
}
void PetWindow::showWindow()
{
    this->setWindowOpacity(1.0); // 1.0 表示完全不透明
}

void PetWindow::init_tray()
{
    // 初始化系统托盘图标
    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    this->trayIcon->setIcon(QIcon("data/icon256.ico")); // 设置托盘图标
    this->trayIcon->setToolTip(QString("原来你也玩原神"));
    // 创建菜单
    this->tray_menu = new QMenu(this);
    this->showAction = tray_menu->addAction("显示");
    this->quitAction = tray_menu->addAction("退出");
    connect(showAction, &QAction::triggered, this, &PetWindow::showWindow);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    // 设置系统托盘菜单
    trayIcon->setContextMenu(menu);
    trayIcon->show();

    this->menu_roles = this->tray_menu->addMenu("人物");
    this->menu_voice = this->tray_menu->addMenu("音乐");

    // 创建 QAction 对象
    this->bbl = new QAction("芭芭拉", this);
    bbl->setProperty("role_name", "芭芭拉");
    connect(bbl, &QAction::triggered, this, &PetWindow::set_role);
    this->menu_roles->addAction(bbl);

    this->bcsz = new QAction("八重神子", this);
    bcsz->setProperty("role_name", "八重神子");
    connect(bcsz, &QAction::triggered, this, &PetWindow::set_role);
    this->menu_roles->addAction(bcsz);
    this->bnt = new QAction("班尼特", this);
    bnt->setProperty("role_name", "班尼特");
    connect(bnt, &QAction::triggered, this, &PetWindow::set_role);
    this->menu_roles->addAction(bnt);
    this->dan = new QAction("迪奥娜", this);
    dan->setProperty("role_name", "迪奥娜");
    connect(dan, &QAction::triggered, this, &PetWindow::set_role);
    this->menu_roles->addAction(dan);
    this->md_music = new QAction("蒙德", this);
    dan->setProperty("name", "md_music");
    connect(md_music, &QAction::triggered, this, &PetWindow::set_audio);
    this->menu_voice->addAction(md_music);
    this->ly_music = new QAction("璃月", this);
    ly_music->setProperty("name", "ly_music");
    connect(ly_music, &QAction::triggered, this, &PetWindow::set_audio);
    this->menu_voice->addAction(ly_music);
    this->dq_music = new QAction("稻妻", this);
    dq_music->setProperty("name", "dq_music");
    connect(dq_music, &QAction::triggered, this, &PetWindow::set_audio);
    this->menu_voice->addAction(dq_music);
    this->role_music = new QAction("角色", this);
    role_music->setProperty("name", "role_music");
    connect(role_music, &QAction::triggered, this, &PetWindow::set_audio);
    this->menu_voice->addAction(role_music);
    this->music_off = new QAction("关闭", this);
    music_off->setProperty("name", "role_music");
    connect(music_off, &QAction::triggered, this, &PetWindow::set_audio);
    this->menu_voice->addAction(music_off);
}

void PetWindow::adjustWindowSize()
{
    // 根据 scale 调整窗口大小
    // 假设我们按比例调整宽度和高度
    int newWidth = this->wt * scale;
    int newHeight = this->ht * scale;
    if ((this->screenheight > newHeight) && (this->screenwidth > newWidth)) {
        this->resize(newWidth, newHeight);
    }
}

void PetWindow::init_window()
{
    this->role_figure = new QLabel(this);
    this->role_figure->setScaledContents(true);
    this->setCentralWidget(this->role_figure);
    this->role_pixmap = new QPixmap(this->image_list.at(this->role_figure_index).filePath());
    this->role_pixmap->scaled(static_cast<int>(this->scale * this->wt),
                              static_cast<int>(this->scale * this->wt),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation);
    this->resize(this->role_pixmap->size());
    this->setMask(this->role_pixmap->createMaskFromColor(QColor(0, 0, 0)));
    this->role_figure->setPixmap(*(this->role_pixmap));
}

void PetWindow::updateAnimation()
{
    // // 更新动画的逻辑，例如更改 QPixmap
    this->role_figure_index += 1;
    if (this->role_figure_index >= this->image_list.size())
        this->role_figure_index = 0;
    this->role_figure = new QLabel(this);
    this->role_figure->setScaledContents(true);
    this->setCentralWidget(this->role_figure);
    this->role_pixmap = new QPixmap(this->image_list.at(this->role_figure_index).filePath());
    this->role_pixmap->scaled(static_cast<int>(this->scale * this->wt),
                              static_cast<int>(this->scale * this->wt),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation);
    this->role_figure->setMask(this->role_pixmap->createMaskFromColor(QColor(0, 0, 0)));
    this->resize(this->role_pixmap->size());
    this->setMask(this->role_pixmap->createMaskFromColor(QColor(0, 0, 0)));
    this->role_figure->setPixmap(*(this->role_pixmap));
}

void PetWindow::set_audio()
{
    //pass
}

void PetWindow::set_role()
{
    // 从 QAction 中检索存储的额外信息
    QAction *action = qobject_cast<QAction *>(sender());
    this->role_name = action->property("role_name").toString();
    this->update_lists();
    // QMessageBox::information(this, "role_name", this->role_name);
}
void PetWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // QMenu menu(this);
    // QAction *knowing = menu.addAction("了解");
    // QAction *talking = menu.addAction("闲聊");
    // QAction *hide = menu.addAction("隐藏");
    // QAction *quitAction = menu.addAction("退出");

    // QAction *selectedAction = menu.exec(event->globalPos());

    // if (selectedAction == quitAction) {
    //     qApp->quit();
    // } else if (selectedAction == hide) {
    //     this->setWindowOpacity(0.0);
    // } else if (selectedAction == talking && audio_role) {
    //     //passs
    //     QMessageBox::information(this, "闲聊", "闲聊");
    // } else if (selectedAction == knowing && audio_role) {
    //     //
    //     QMessageBox::information(this, "了解", "了解");
    // }
}
void PetWindow::update_lists()
{
    // 获取目录中所有文件的列表
    QDir image_directory(this->base_path + QDir::separator() + this->image_path + QDir::separator()
                         + this->role_name);
    this->image_list = image_directory.entryInfoList(QDir::Files);
    QDir music_directory(this->base_path + QDir::separator() + this->music_path + QDir::separator()
                         + this->role_name);
    QFileInfoList entries = music_directory.entryInfoList(QDir::Files);
    for (const QFileInfo &entry : entries) {
        if (entry.fileName().startsWith("闲聊")) {
            // 构造完整的文件路径并添加到列表中
            this->talk_list.append(entry);
        } else if (entry.fileName().startsWith("想要了解")) {
            // 构造完整的文件路径并添加到列表中
            this->know_list.append(entry);
        }
    }
}
void PetWindow::keyPressEvent(QKeyEvent *event)
{
    // 命令键 + Q 退出程序
    if (event->key() == Qt::Key_Q && event->modifiers() == Qt::ControlModifier) {
        qApp->quit();
    }
    // 命令键 + "+" 增大比例
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Equal) {
        if (this->scale < 2)
            this->scale += 0.01;
    }
    // 命令键 + "-" 减小比例
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus) {
        if (this->scale > 0.6)
            this->scale -= 0.01;
    }
    adjustWindowSize();
    // 基类的 keyPressEvent 也应该被调用，以便其他事件处理器可以接收事件
    QMainWindow::keyPressEvent(event);
}

void PetWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->globalPosition().toPoint() - this->pos();
        isDragging = true;
        setCursor(Qt::ClosedHandCursor);
    }
}

void PetWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        QPoint newWindowPosition = event->globalPosition().toPoint() - lastMousePosition;
        move(newWindowPosition);
    }
}

void PetWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        unsetCursor();
    }
}
