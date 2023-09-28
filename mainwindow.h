#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QIntValidator>
#include <climits>

#include "converterwindow.h"

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

    QIntValidator* intValidFile;
    QIntValidator* intValidWidth;
    QIntValidator* intValidDepth;

    QVBoxLayout* main;
    QHBoxLayout* layButton;
    QGridLayout* layOptions;

    void memoryAllocation();
    bool dataChecking() const;

    bool transformation(QFile &mainFile);

    QString pruningFileName(QFile &mainFile); //pruning - обрезка
    void checkPosStream(QDataStream &dat_stream, int fileNumber) const;
    void fileCompletion(QDataStream &dat_stream, QFile &intermediateFile, int fileNumber, bool &flag) const;

    QLabel* labOptions;
    QLabel* labFileСount;
    QLabel* labWidth;
    QLabel* labDepth;

    ConverterWindow* converterForm;

private slots:
    void buttonClickFile();
    void buttonClickInfomation() const;
    void buttonClickOpenConverterForm();

//    void checkInput();
};
#endif // MAINWINDOW_H
