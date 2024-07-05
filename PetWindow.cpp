#include "PetWindow.h"
#include <QAction>
#include <QApplication>
#include <QAudioOutput>
#include <QCursor>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
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
#include <QRandomGenerator>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer> // Qt 的多媒体模块用于音频播放
PetWindow::PetWindow()
{
    // 设置窗口标志以隐藏任务栏图标，同时使窗口无边框并始终置顶
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setWindowIcon(QIcon("data/icon256.ico"));
    this->hide();
    // 获取静态资源列表
    this->update_lists();

    // 初始化音频播放器

    this->backgroundMusicPlayer = new QMediaPlayer(this);
    this->roleVoicePlayer = new QMediaPlayer(this);

    this->audioOutput1 = new QAudioOutput(this); //音频输出，指向默认的音频输出设备
    this->audioOutput2 = new QAudioOutput(this); //音频输出，指向默认的音频输出设备
    this->backgroundMusicPlayer->setAudioOutput(audioOutput1); //设置音频输出
    this->roleVoicePlayer->setAudioOutput(audioOutput2);       //设置音频输出
    this->backgroundMusicPlayer->setProperty("Volume", 100);
    this->roleVoicePlayer->setProperty("Volume", 100);

    this->init_tray();
    this->init_window();

    // 初始化定时器
    this->now_time = QDateTime::currentDateTime().time().hour();
    this->timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PetWindow::updateAnimation);
    timer->start(60); // 设置定时器时间间隔
}

PetWindow::~PetWindow()
{
    // 停止所有音乐播放
    if (backgroundMusicPlayer) {
        backgroundMusicPlayer->stop();
        delete backgroundMusicPlayer;
        backgroundMusicPlayer = nullptr;
    }
    if (roleVoicePlayer) {
        roleVoicePlayer->stop();
        delete roleVoicePlayer;
        roleVoicePlayer = nullptr;
    }

    // 删除音频输出
    delete audioOutput1;
    delete audioOutput2;

    // 停止定时器
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    // 删除托盘图标和菜单
    if (trayIcon) {
        trayIcon->hide();
        delete trayIcon;
        trayIcon = nullptr;
    }
    delete tray_menu;
    delete menu;
    delete menu_roles;
    delete menu_voice;

    // 删除动作
    delete knowing;
    delete talking;
    delete hideAction;
    delete showAction;
    delete quitAction;
    delete actionPlayMontdidor;
    delete actionPlayLiyue;
    delete actionPlayInazuma;
    delete actionPlayRoleVoice;
    delete actionStopAll;

    // 删除角色图片标签
    delete role_figure;
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

    // 设置系统托盘菜单
    this->trayIcon->setContextMenu(this->tray_menu);
    this->trayIcon->show();

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

    // this->actionPlayMontdidor = menu_voice->addAction("播放蒙德");
    // this->actionPlayLiyue = menu_voice->addAction("播放璃月");
    // this->actionPlayInazuma = menu_voice->addAction("播放稻妻");
    // this->actionPlayRoleVoice = menu_voice->addAction("角色语音");
    // this->actionStopAll = menu_voice->addAction("关闭所有音乐");

    // 创建音乐菜单相关的QAction对象，并设置为可勾选
    this->actionPlayMontdidor = menu_voice->addAction("播放蒙德");
    this->actionPlayMontdidor->setCheckable(true);
    this->actionPlayLiyue = menu_voice->addAction("播放璃月");
    this->actionPlayLiyue->setCheckable(true);
    this->actionPlayInazuma = menu_voice->addAction("播放稻妻");
    this->actionPlayInazuma->setCheckable(true);
    this->actionPlayRoleVoice = menu_voice->addAction("角色语音");
    this->actionPlayRoleVoice->setCheckable(true);

    this->actionStopAll = menu_voice->addAction("关闭所有音乐");
    this->actionStopAll->setCheckable(true);

    connect(actionPlayMontdidor, &QAction::triggered, this, &PetWindow::onPlayMontdidor);
    connect(actionPlayLiyue, &QAction::triggered, this, &PetWindow::onPlayLiyue);
    connect(actionPlayInazuma, &QAction::triggered, this, &PetWindow::onPlayInazuma);
    connect(actionPlayRoleVoice, &QAction::triggered, this, &PetWindow::onRoleVoice);
    connect(actionStopAll, &QAction::triggered, this, &PetWindow::onStopAllVocie);

    this->showAction = tray_menu->addAction("显示");
    this->quitAction = tray_menu->addAction("退出");
    connect(showAction, &QAction::triggered, this, &PetWindow::onShowTriggered);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    this->actionPlayRoleVoice->setChecked(true);
    this->greeting();
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
    // 获取屏幕尺寸
    QRect screenRect = this->screen()->geometry();
    this->screenheight = screenRect.height();
    this->screenwidth = screenRect.width();
    this->pos_x = screenRect.width() - 400;
    this->pos_y = screenRect.height() - 800;
    // 设置窗口位置
    this->setGeometry(this->pos_x, this->pos_y, this->width(), this->height());
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAutoFillBackground(true);

    // 初始化动作
    knowing = new QAction("了解", this);
    talking = new QAction("闲聊", this);
    hideAction = new QAction("隐藏", this);
    quitAction = new QAction("退出", this);

    // 创建菜单并添加动作
    this->menu = new QMenu(this);
    this->menu->addAction(knowing);
    this->menu->addAction(talking);
    this->menu->addAction(hideAction);
    this->menu->addAction(quitAction);

    // 连接信号和槽
    connect(knowing, &QAction::triggered, this, &PetWindow::onKnowTriggered);
    connect(talking, &QAction::triggered, this, &PetWindow::onTalkTriggered);
    connect(hideAction, &QAction::triggered, this, &PetWindow::onHideTriggered);
    connect(quitAction, &QAction::triggered, this, &PetWindow::onQuitTriggered);

    this->resize(static_cast<int>(this->scale * this->wt), static_cast<int>(this->scale * this->wt));
    // 创建一个QLabel对象，用于在窗口中显示图像
    this->role_figure = new QLabel(this);

    // 设置QLabel对象，使其内容能够缩放以适应标签的大小
    this->role_figure->setScaledContents(true);

    // 将创建的QLabel设置为当前窗口的中心部件
    this->setCentralWidget(this->role_figure);

    // 根据当前索引从图像列表中获取图像文件路径，并创建一个QPixmap对象
    role_pixmap = QPixmap(this->image_list.at(this->role_figure_index).filePath());

    // 对QPixmap对象进行缩放，保持长宽比，并使用平滑缩放转换
    // 'scale'变量用于确定缩放比例，'wt'变量作为基准宽度
    role_pixmap.scaled(static_cast<int>(this->scale * this->wt),
                       static_cast<int>(this->scale * this->wt),
                       Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);

    // 这一行被注释掉了，它用于调整窗口大小以适应缩放后的图像尺寸
    // 如果您想在不调整窗口大小时显示图像，您可以取消注释这行
    // this->resize(this->role_pixmap->size());

    // 为窗口设置掩膜，掩膜颜色为黑色，这可能会隐藏与掩膜颜色相同的像素
    // 注意：这种方法可能不适用于所有情况，具体效果取决于图像内容和掩膜的使用方式
    // 由于您不希望调整窗口大小，您可能需要重新考虑是否使用掩膜
    // this->setMask(this->role_pixmap->createMaskFromColor(QColor(0, 0, 0)));

    // 将处理后的QPixmap设置到role_figure标签上，这样图像就会显示在标签上
    this->role_figure->setPixmap(this->role_pixmap);

    this->setAutoFillBackground(true); // 设置窗口背景透明
    // 这一行也被注释掉了，它用于设置窗口为半透明背景，但同样您没有启用它
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void PetWindow::updateAnimation()
{
    // // 更新动画的逻辑，例如更改 QPixmap
    this->role_figure_index += 1; // 增加当前动画帧的索引
    if (this->role_figure_index >= this->image_list.size()) {
        this->role_figure_index = 0; // 循环回到动画列表的开始
    }

    // 加载新的图像并缩放它
    QPixmap newPixmap(this->image_list.at(this->role_figure_index).filePath());
    QPixmap scaledPixmap = newPixmap.scaled(static_cast<int>(this->scale * this->wt),
                                            static_cast<int>(
                                                this->scale
                                                * this->ht), // 确保这里使用正确的高度基准 'ht'
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation);

    // 更新 QLabel 的 QPixmap，而不是创建一个新的 QLabel
    this->role_figure->setPixmap(scaledPixmap);

    // 如果您希望窗口背景透明，使用以下属性
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    // 不要再设置掩膜，除非您有特定的理由需要它
    // 如果您希望窗口具有透明背景，请确保不要设置掩膜
    // this->setMask(this->role_pixmap.createMaskFromColor(QColor(0, 0, 0)));

    // 根据需要调整窗口大小，允许用户调整窗口大小查看不同部分的图像
    // 如果您希望窗口大小与图像大小一致，可以取消注释以下行
    // this->resize(scaledPixmap.size());
}

void PetWindow::playBackgroundMusic(const QString &musicName)
{
    this->isPlayAudio = true;
    this->isPlayBackgroundAudio = true;
    this->bg_music = musicName;
    QString musicFilePath = this->base_path + QDir::separator() + this->music_path
                            + QDir::separator() + this->bg_music + QDir::separator()
                            + "background.mp3";
    this->backgroundMusicPlayer->setSource(QUrl::fromLocalFile(musicFilePath));
    this->backgroundMusicPlayer->setProperty("Volume", 100);
    this->backgroundMusicPlayer->loops();
    this->backgroundMusicPlayer->play();
}

void PetWindow::onPlayMontdidor(bool checked)
{
    if (checked) {
        this->actionStopAll->setChecked(false);
        this->actionPlayLiyue->setChecked(false);
        this->actionPlayInazuma->setChecked(false);
        this->playBackgroundMusic("蒙德");
    } else {
        this->isPlayBackgroundAudio = false;
        this->backgroundMusicPlayer->stop();
    }
}
void PetWindow::onPlayLiyue(bool checked)
{
    if (checked) {
        this->actionStopAll->setChecked(false);
        this->actionPlayMontdidor->setChecked(false);
        this->actionPlayInazuma->setChecked(false);
        this->playBackgroundMusic("璃月");
    } else {
        this->isPlayBackgroundAudio = false;
        this->backgroundMusicPlayer->stop();
    }
}
void PetWindow::onPlayInazuma(bool checked)
{
    if (checked) {
        this->actionStopAll->setChecked(false);
        this->actionPlayMontdidor->setChecked(false);
        this->actionPlayLiyue->setChecked(false);
        this->playBackgroundMusic("稻妻");
    } else {
        this->isPlayBackgroundAudio = false;
        this->backgroundMusicPlayer->stop();
    }
}
void PetWindow::onRoleVoice(bool checked)
{
    if (checked) {
        this->actionStopAll->setChecked(false);
        this->isPlayRoleAudio = true;
        this->isPlayAudio = true;
        this->greeting();
    } else {
        this->isPlayRoleAudio = false;
        this->roleVoicePlayer->stop();
    }
}

void PetWindow::onStopAllVocie(bool checked)
{
    if (checked) {
        this->actionPlayMontdidor->setChecked(false);
        this->actionPlayInazuma->setChecked(false);
        this->actionPlayLiyue->setChecked(false);
        this->actionPlayRoleVoice->setChecked(false);
        // 关闭所有音乐和语音
        this->isPlayBackgroundAudio = false;
        this->isPlayRoleAudio = false;
        this->isPlayAudio = false;
        this->backgroundMusicPlayer->stop();
        this->roleVoicePlayer->stop();
    }
}

void PetWindow::onKnowTriggered()
{
    if (true == this->isPlayRoleAudio) {
        for (const QFileInfo &info_iter : this->know_list) {
            // QMessageBox::information(this, "了解", info_iter.filePath());
            this->roleVoicePlayer->setSource(info_iter.filePath());
            this->roleVoicePlayer->play();
        }
    }
}

void PetWindow::onTalkTriggered()
{
    if (this->isPlayRoleAudio) {
        for (const QFileInfo &info_iter : this->talk_list) {
            // QMessageBox::information(this, "了解", info_iter.filePath());
            this->roleVoicePlayer->setSource(info_iter.filePath());
            this->roleVoicePlayer->play();
        }
    }
}

void PetWindow::onQuitTriggered()
{
    qApp->quit();
}

void PetWindow::onHideTriggered()
{
    this->setWindowOpacity(0.0);
}

void PetWindow::onShowTriggered()
{
    this->setWindowOpacity(1.0);
}
void PetWindow::greeting()
{
    if (this->isPlayRoleAudio) {
        QString filename;
        this->now_time = QDateTime::currentDateTime().time().hour();
        if (this->now_time >= 4 && this->now_time < 10) {
            filename = "早上好.mp3";
        } else if (this->now_time >= 10 && this->now_time < 14) {
            filename = "中午好.mp3";
        } else if (this->now_time >= 17 && this->now_time <= 23) {
            filename = "晚上好.mp3";
        } else {
            filename = "晚安.mp3"; // 使用空字符串表示没有问候语
        }
        this->roleVoicePlayer->setSource(this->base_path + QDir::separator() + this->music_path
                                         + QDir::separator() + this->role_name + QDir::separator()
                                         + filename);
        this->roleVoicePlayer->play();
    }
}
void PetWindow::set_role()
{
    // 从 QAction 中检索存储的额外信息
    QAction *action = qobject_cast<QAction *>(sender());
    this->role_name = action->property("role_name").toString();
    this->update_lists();
    this->greeting();
    // QMessageBox::information(this, "role_name", this->role_name);
}
void PetWindow::contextMenuEvent(QContextMenuEvent *event)
{
    menu->exec(event->globalPos());
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

// void PetWindow::closeEvent(QCloseEvent *event)
// {
//     event->ignore(); // 忽略关闭事件
//     this->hide();    // 隐藏窗口
// }
