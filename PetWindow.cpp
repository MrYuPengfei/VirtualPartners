#include "PetWindow.h"
#include "config.h"
PetWindow::PetWindow()
{
    this->base_path = QString(DATAPATH);
    this->music_path = QString(MUSIC_PATH);
    this->image_path = QString(PNG_PATH);
    this->bgm_path = QString(BGM_PATH);
    this->bgm_name = QString(BACKGROUND_MUSIC);
    this->role_name = QString(ROLE_NAME);
    this->roles = {};
    this->bgms = {};
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
    // this->textEdit = new QTextEdit(this);
    // this->textEdit->setReadOnly(true);
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
    for (QAction *role : this->roles) {
        if (role) {
            delete role;
            role = nullptr;
        }
    }
    for (QAction *bgm : this->bgms) {
        if (bgm) {
            delete bgm;
            bgm = nullptr;
        }
    }

    // 删除动作
    delete knowing;
    delete talking;
    delete hideAction;
    delete showAction;
    delete helpAction;
    delete quitAction;
    delete actionPlayRoleVoice;
    delete actionStopAll;
    delete role_figure;
}
void PetWindow::init_tray()
{
    // 初始化系统托盘图标
    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    this->trayIcon->setIcon(QIcon(ICON_PATH)); // 设置托盘图标
    this->trayIcon->setToolTip(QString(TOOL_TIP));
    // 创建菜单
    this->tray_menu = new QMenu(this);

    // 设置系统托盘菜单
    this->trayIcon->setContextMenu(this->tray_menu);
    this->trayIcon->show();

    this->menu_roles = this->tray_menu->addMenu("人物");
    {
        this->group1 = new QActionGroup(this);
        this->group1->setExclusive(true); // 设置为互斥
        // 指定要打开的文件夹路径
        QString folderPath = this->base_path + QDir::separator() + this->image_path;
        // 创建QDir对象
        QDir dir(folderPath);
        // 获取目录下所有文件的列表
        QStringList dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        // 遍历文件列表
        foreach (const QString &fileName, dirList) {
            QAction *role = new QAction(fileName, this);
            role->setProperty("role_name", fileName);
            role->setCheckable(true);
            this->roles.append(role); //append(role);
            this->group1->addAction(role);
            this->menu_roles->addAction(role);
            connect(role, &QAction::triggered, this, &PetWindow::set_role);
        }
    }

    this->menu_bgms = this->tray_menu->addMenu("BGM");
    {
        this->group2 = new QActionGroup(this);
        this->group2->setExclusive(true); // 设置为互斥
        // 指定要打开的文件夹路径
        QString folderPath = this->base_path + QDir::separator() + this->bgm_path;
        // 创建QDir对象
        QDir dir(folderPath);
        // 获取目录下所有文件的列表
        QStringList dirList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        // 遍历文件列表
        foreach (const QString &fileName, dirList) {
            QAction *bgm = new QAction(QString(fileName).replace(".mp3", ""), this);
            bgm->setProperty("bgm_name", fileName);
            bgm->setCheckable(true);
            this->bgms.append(bgm); //append(role);
            this->group2->addAction(bgm);
            this->menu_bgms->addAction(bgm);
            connect(bgm, &QAction::triggered, this, &PetWindow::set_bgm);
        }
    }

    this->menu_voice = this->tray_menu->addMenu("声音");
    {
        this->actionPlayBackgroundMusic = menu_voice->addAction("背景音乐");
        this->actionPlayBackgroundMusic->setCheckable(true);
        this->actionPlayRoleVoice = menu_voice->addAction("角色语音");
        this->actionPlayRoleVoice->setCheckable(true);
        this->actionStopAll = menu_voice->addAction("关闭所有");
        this->actionStopAll->setCheckable(true);

        connect(actionPlayBackgroundMusic,
                &QAction::triggered,
                this,
                &PetWindow::onPlayBackgroundMusic);
        connect(actionPlayRoleVoice, &QAction::triggered, this, &PetWindow::onPlayRoleVoice);
        connect(actionStopAll, &QAction::triggered, this, &PetWindow::onStopAllVocie);
    }
    this->showAction = tray_menu->addAction("显示");
    connect(showAction, &QAction::triggered, this, &PetWindow::onShowTriggered);
    this->menu_markdown = this->tray_menu->addMenu("帮助文档");
    {
        this->helpAction = this->menu_markdown->addAction("程序说明");
        connect(helpAction, &QAction::triggered, this, &PetWindow::onHelpTriggered);
        this->showVersionAction = this->menu_markdown->addAction("版本信息");
        connect(showVersionAction, &QAction::triggered, this, &PetWindow::onShowVersionMarkdown);
    }

    this->quitAction = tray_menu->addAction("退出");
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    this->roles.at(0)->setChecked(true);
    this->bgms.at(0)->setChecked(true);
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
    this->role_figure->setScaledContents(true);
    this->setCentralWidget(this->role_figure);
    role_pixmap = QPixmap(this->image_list.at(this->role_figure_index).filePath());
    role_pixmap.scaled(static_cast<int>(this->scale * this->wt),
                       static_cast<int>(this->scale * this->wt),
                       Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
    this->role_figure->setPixmap(this->role_pixmap);
    this->setAutoFillBackground(true); // 设置窗口背景透明
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
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void PetWindow::playBackgroundMusic(const QString &musicName)
{
    this->isPlayAudio = true;
    this->isPlayBackgroundAudio = true;
    this->bgm_name = musicName;
    QString musicFilePath = this->base_path + QDir::separator() + this->bgm_path + QDir::separator()
                            + this->bgm_name;
    this->backgroundMusicPlayer->setSource(QUrl::fromLocalFile(musicFilePath));
    this->backgroundMusicPlayer->setProperty("Volume", 100);
    this->backgroundMusicPlayer->loops();
    this->backgroundMusicPlayer->play();
}

void PetWindow::onPlayRoleVoice(bool checked)
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
void PetWindow::onPlayBackgroundMusic(bool checked)
{
    if (checked) {
        this->actionStopAll->setChecked(false);
        this->isPlayBackgroundAudio = true;
        this->isPlayAudio = true;
        this->playBackgroundMusic(this->bgm_name);
    } else {
        this->isPlayBackgroundAudio = false;
        this->roleVoicePlayer->stop();
    }
}

void PetWindow::onStopAllVocie(bool checked)
{
    if (checked) {
        this->actionPlayRoleVoice->setChecked(false);
        this->actionPlayBackgroundMusic->setChecked(false);
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

void PetWindow::onHelpTriggered()
{
    QString filePath = HELP_DOCUMENT; // 假设VERSION.md文件位于项目目录中
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString markdownContent = in.readAll();
        file.close();
        QMessageBox::about(this, "程序说明", markdownContent);
    } else {
        QMessageBox::warning(this, "Error", "Cannot open file: " + filePath);
    }
}

void PetWindow::onShowVersionMarkdown()
{
    QString filePath = VERSION_DOCUMENT; // 假设VERSION.md文件位于项目目录中
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString markdownContent = in.readAll();
        file.close();
        QMessageBox::about(this, "版本文档", markdownContent);
    } else {
        QMessageBox::warning(this, "Error", "Cannot open file: " + filePath);
    }
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
        QString file = this->base_path + QDir::separator() + this->music_path + QDir::separator()
                       + this->role_name + QDir::separator() + filename;
        if (QFile::exists(file)) {
            //QMessageBox::information(this, "role_name", file);
            this->roleVoicePlayer->setSource(QFileInfo(file).filePath());
            this->roleVoicePlayer->play();
        }
    }
}
void PetWindow::set_role(bool checked)
{
    // 从 QAction 中检索存储的额外信息
    if (checked) {
        QAction *action = qobject_cast<QAction *>(sender());
        this->role_name = action->property("role_name").toString();
        this->update_lists();
        this->greeting();
        // QMessageBox::information(this, "role_name", this->role_name);
    }
}

void PetWindow::set_bgm(bool checked)
{
    // 从 QAction 中检索存储的额外信息
    if (checked) {
        QAction *action = qobject_cast<QAction *>(sender());
        this->bgm_name = action->property("bgm_name").toString();
        // QMessageBox::information(this, "role_name", this->role_name);
        if (this->isPlayAudio && this->isPlayBackgroundAudio) {
            this->playBackgroundMusic(this->bgm_name);
        }
    }
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
