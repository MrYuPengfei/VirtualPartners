// SettingsWindow.cpp
#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    QMessageBox::information(this, "了解", "new SettingsWindow(this)");
    setupUI();
}

void SettingsWindow::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Settings Window", this);
    layout->addWidget(label);

    QPushButton *closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &SettingsWindow::on_closeButton_clicked);
    layout->addWidget(closeButton);

    setLayout(layout);
    setWindowTitle("Settings");
    QRect screenRect = this->screen()->geometry();
    // 设置窗口位置
    this->setGeometry(screenRect.width() / 2, screenRect.width() / 2, this->width(), this->height());
    this->setStyleSheet("background-color: white;");

    QMessageBox::information(this, "了解", "new SettingsWindow(this)");
    this->show();
}

void SettingsWindow::on_closeButton_clicked() {
    this->close(); // 关闭设置窗口
}
