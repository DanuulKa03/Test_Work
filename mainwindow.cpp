#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    setCentralWidget(new QWidget());

    memoryAllocation();
//    validatorSetup();

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

//    QObject::connect(leFileCount, &QLineEdit::editingFinished, this, &MainWindow::checkInput);
//    QObject::connect(leWidth, &QLineEdit::editingFinished, this, &MainWindow::checkInput);
//    QObject::connect(leDepth, &QLineEdit::editingFinished, this, &MainWindow::checkInput);


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
    conclusion->setReadOnly(true);

    labOptions = new QLabel("Options:", this);
    labFileСount = new QLabel("File count:",this);
    labWidth = new QLabel("Width:", this);
    labDepth = new QLabel("Depth:",this);

    leFileCount = new QLineEdit("1",this);
    leWidth = new QLineEdit("8",this);
    leDepth = new QLineEdit("256",this);

//    intValidFile = new QIntValidator(1, 8,this);
//    intValidWidth = new QIntValidator(8, 32, this);
//    intValidDepth = new QIntValidator(1, INT_MAX, this);

    converterForm = nullptr;
}

//void MainWindow::validatorSetup()
//{
//    leFileCount->setValidator(intValidFile);
//    leWidth->setValidator(intValidWidth);
//    leDepth->setValidator(intValidDepth);
//}


bool MainWindow::dataChecking()
{
    if ( ceil( log2( leFileCount->text().toInt() ) ) != floor( log2( leFileCount->text().toInt() ) ) && ( leFileCount->text().toInt() < 1 || leFileCount->text().toInt() > 8) )
    {
        return false;
    }
    if ( ceil( log2( leWidth->text().toInt() ) ) != floor( log2( leWidth->text().toInt() ) ) && ( leWidth->text().toInt() < 8 || leWidth->text().toInt() > 32) )
    {
        return false;
    }
    if ( leDepth->text().toInt() < 1 )
    {
        return false;
    }
    return true;
}

bool MainWindow::transformation(QFile &mainFile)
{
    bool flag = true;

    QDataStream dat_stream(&mainFile);

    QString strPruning = pruningFileName(mainFile);
    for (int fileNumber = 0; fileNumber < leFileCount->text().toInt(); fileNumber++)
    {
        QFile intermediateFile(strPruning + QString::number(fileNumber+1) + ".mif");
        if(intermediateFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            fileCompletion(dat_stream, intermediateFile, fileNumber, flag);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Произошла ошибка", "Произошла ошибка сохранения файла, попробуйте в другой раз.");
        }
        intermediateFile.close();
    }

    return flag;
}

QString MainWindow::pruningFileName(QFile &mainFile) //pruning - обрезка
{
    if ( mainFile.fileName().contains(".") )
    {
        return mainFile.fileName().first(mainFile.fileName().indexOf(".")).remove(0, mainFile.fileName().lastIndexOf("/") + 1 );
    }
    else
    {
        return mainFile.fileName().remove(0, mainFile.fileName().lastIndexOf("/") + 1 );
    }
}

void MainWindow::checkPosStream(QDataStream &dat_stream, int fileNumber)
{
    if (dat_stream.device()->pos() != 0)
    {
        dat_stream.device()->seek(0);
    }
    if (fileNumber > 0)
    {
        dat_stream.skipRawData( (leWidth->text().toInt() / 8) * fileNumber );
    }
}

void MainWindow::fileCompletion(QDataStream &dat_stream, QFile &intermediateFile, int fileNumber, bool &flag)
{

    int widthBytes = leWidth->text().toInt() / 8;

    if (dat_stream.device()->pos() != 0)
    {
        dat_stream.device()->seek(0);
    }
    if (fileNumber > 0)
    {
        dat_stream.skipRawData(widthBytes * fileNumber);
    }

    conclusion->append("Создание файла " + intermediateFile.fileName() + ".");

    QTextStream tex_stream(&intermediateFile);

    tex_stream << "WIDTH = " + leWidth->text() + ";\n";
    tex_stream << "DEPTH = " + leDepth->text() + ";\n\n";

    tex_stream << "ADDRESS_RADIX = HEX;\n";
    tex_stream << "DATA_RADIX = HEX;\n\n";

    tex_stream << "CONTENT BEGIN\n";


    for (int wordNumber = 0; wordNumber < leDepth->text().toInt(); ++wordNumber)
    {
        tex_stream << "    " + QString::number(wordNumber, 16).toUpper() + " : ";

        //Что происходит дальше? Я буду считывать данные по байтово, чтобы отслеживать недостающие байты
        for (int var = 0; var < widthBytes; ++var)
        {
            if ( dat_stream.atEnd() )
            {
                tex_stream << "FF";
                flag = false;
            }
            else
            {
                char letter;
                dat_stream.readRawData(&letter, 1);
                tex_stream << QString::number(static_cast<quint8>(letter), 16).toUpper();
            }
        }

        tex_stream << ";\n";
        //Тут мы отслеживаем, сколько байтов нужно пропустить, в зависимости кокое кол-во файлов указано
        //Мы отнимаем "-1" от leFileCount->text().toInt(), чтобы программа не пропускала свои нужные ей байты
        if (leFileCount->text().toInt() > 1 && !dat_stream.atEnd())
        {
            dat_stream.skipRawData(widthBytes * (leFileCount->text().toInt() - 1));
        }
    }

    tex_stream << "END;";

    conclusion->append("Файл " + intermediateFile.fileName() + " создан.\n");

}

void MainWindow::buttonClickFile()
{
    if (dataChecking())
    {
        QFile mainFile( QFileDialog::getOpenFileName(this, tr("Выбор файла"), "", "") );

        if(mainFile.open(QIODevice::ReadOnly))
        {

            if (transformation(mainFile))
            {
                conclusion->append("Размер исходного файла идеально подходит для заполнения всех файлов.");
            }
            else
            {
                conclusion->append("Размер исходного файла недостаточен для заполнения всех файлов");
            }

        }
        else
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Произошла ошибка", "Выберите файл и нажмите открыть.");
        }

        mainFile.close();

    }
    else
    {
        QMessageBox msgBox;
        msgBox.critical(0, "Произошла ошибка", "Введены некорректные данные. Ознакомьтесь с информацией ниже.");
        buttonClickInfomation();

    }
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
    converterForm = new ConverterWindow(this);
    converterForm->setAttribute(Qt::WA_DeleteOnClose); // Установим атрибут, чтобы объект формы удалился при закрытии

    converterForm->show();
}

//void  MainWindow::checkInput()
//{
//    int valueFile = leFileCount->text().toInt();
//    if (valueFile < 1)
//    {
//        leFileCount->setText("1");
//    }
//    else if (valueFile > 8)
//    {
//        leFileCount->setText("8");
//    }
//    int valueWidth = leWidth->text().toInt();
//    if (valueWidth < 8)
//    {
//        leWidth->setText("8");
//    }
//    else if (valueWidth > 32)
//    {
//        leWidth->setText("32");
//    }
//    int valueDepth = leDepth->text().toInt();
//    if (valueDepth < 1)
//    {
//        leDepth->setText("1");
//    }
//}
