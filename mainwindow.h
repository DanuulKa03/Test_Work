#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton* selectFile;

    QTextEdit* conclusion;

    QLineEdit* leFileCount;
    QLineEdit* leWidth;
    QLineEdit* leDepth;

    QVBoxLayout* main;
    QHBoxLayout* layButton;
    QGridLayout* layOptions;

    void memoryAllocation();

    bool transformation();

    QLabel* labOptions;
    QLabel* labFileСount;
    QLabel* labWidth;
    QLabel* labDepth;

};
#endif // MAINWINDOW_H
