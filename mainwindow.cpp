#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    setCentralWidget(new QWidget());

    memoryAllocation();

    setLayout(main);



    main->addLayout(layOptions);
    main->addWidget(selectFile);
    main->addWidget(conclusion);

}

MainWindow::~MainWindow()
{


}

void MainWindow::memoryAllocation()
{

    main = new QVBoxLayout(centralWidget());
    layOptions = new QGridLayout();

    selectFile = new QPushButton("Выбрать файл",this);

    conclusion = new QTextEdit(this);

    labOptions = new QLabel("Options:", this);
    labFileСount = new QLabel("File count:",this);
    labWidth = new QLabel("Width:", this);
    labDepth = new QLabel("Depth:",this);

    leFileCount = new QLineEdit("1",this);
    leWidth = new QLineEdit("8",this);
    leDepth = new QLineEdit("256",this);
}

bool MainWindow::transformation()
{
    return false;
}

