#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    setCentralWidget(new QWidget());

    memoryAllocation();

    setLayout(main);

    layOptions->addWidget(labOptions, 0, 0);
    layOptions->addWidget(labFileСount, 1, 0);
    layOptions->addWidget(leFileCount, 1, 1);
    layOptions->addWidget(labWidth, 2, 0);
    layOptions->addWidget(leWidth, 2, 1);
    layOptions->addWidget(labDepth, 3, 0);
    layOptions->addWidget(leDepth, 3, 1);

    main->addLayout(layOptions);

    main->addWidget(converterTextToBinary);
    main->addWidget(getInformationProgramm);
    main->addWidget(selectFile);
    main->addWidget(conclusion);

    QObject::connect(converterTextToBinary, &QPushButton::clicked, this, &MainWindow::buttonClickOpenConverterForm);
    QObject::connect(selectFile, &QPushButton::clicked, this, &MainWindow::buttonClickFile);
    QObject::connect(getInformationProgramm, &QPushButton::clicked, this, &MainWindow::buttonClickInfomation);

}

MainWindow::~MainWindow()
{


}

void MainWindow::memoryAllocation()
{

    main = new QVBoxLayout(centralWidget());
    layOptions = new QGridLayout();

    selectFile = new QPushButton("Выбрать файл",this);
    getInformationProgramm = new QPushButton("Информация",this);
    converterTextToBinary= new QPushButton("Создание бинарного файла", this);

    conclusion = new QTextEdit(this);

    labOptions = new QLabel("Options:", this);
    labFileСount = new QLabel("File count:",this);
    labWidth = new QLabel("Width:", this);
    labDepth = new QLabel("Depth:",this);

    leFileCount = new QLineEdit("1",this);
    leWidth = new QLineEdit("8",this);
    leDepth = new QLineEdit("256",this);

    converterForm = nullptr;
}


bool MainWindow::transformation()
{
    getFileName();
    return false;
}

QString MainWindow::getFileName()
{
    return QFileDialog::getOpenFileName(this,
        tr("Выбор файла"), "", tr("Бинарный файл (*.bin)"));
}

void MainWindow::buttonClickFile()
{
    conclusion->append("Привет");
    transformation();
}

void MainWindow::buttonClickInfomation()
{
    conclusion->append("Поддерживающие параметры преобразования:");
    conclusion->append(" - количество результирующих файлов(File count); допустимое значение - 1/2/4/8, по умолчанию - 1;");
    conclusion->append(" - ширина данных в битах (разрядность слова)(Width); допустимые значения - 8/16/32, по умолчанию - 8;");
    conclusion->append(" - количество слов заданной разрядности в каждом файле(Depth); по умолчанию - 256.");
}

void MainWindow::buttonClickOpenConverterForm()
{
    if (!converterForm)
    {
        converterForm = new ConverterWindow(this);
        converterForm->setAttribute(Qt::WA_DeleteOnClose); // Установим атрибут, чтобы объект формы удалился при закрытии
    }
    converterForm->show();
}
