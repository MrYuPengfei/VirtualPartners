// SettingsWindow.h
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H
#include <QApplication>
// #include <QDesktopWidget>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>
class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget* parent = nullptr);

private slots:
    void on_closeButton_clicked(); // 槽函数，用于关闭窗口

private:
    void setupUI(); // 私有成员函数，用于设置用户界面
};

#endif // SETTINGSWINDOW_H
