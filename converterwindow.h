#ifndef CONVERTERWINDOW_H
#define CONVERTERWINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>


class ConverterWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ConverterWindow(QWidget *parent = nullptr);

private:
    QPushButton* btnConverterTextToBinary;

    QLabel* labText;

    QLineEdit* leText;

    QVBoxLayout* main;

    QHBoxLayout* assistantMain;

    void memoryAllocation();
    bool dataChecking();

private slots:

    void saveFileToBinary();

};

#endif // CONVERTERWINDOW_H
