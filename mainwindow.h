#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton* selectFile;
    QPushButton* getInformationProgramm;
    QPushButton* converterTextToBinary;

    QTextEdit* conclusion;

    QLineEdit* leFileCount;
    QLineEdit* leWidth;
    QLineEdit* leDepth;

    QVBoxLayout* main;
    QHBoxLayout* layButton;
    QGridLayout* layOptions;

    void memoryAllocation();

    bool transformation();

    QString getFileName();

    QLabel* labOptions;
    QLabel* labFileСount;
    QLabel* labWidth;
    QLabel* labDepth;

private slots:
    void buttonClickFile();
    void buttonClickInfomation();

};
#endif // MAINWINDOW_H
