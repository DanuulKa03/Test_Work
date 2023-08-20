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


/*bool MainWindow::transformation()
{
    bool flag = true;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбор файла"), "", "");

    QFile initialFile(fileName);

    if(initialFile.open(QIODevice::ReadOnly))
    {
        QByteArray contents = initialFile.readAll();

        for (int fileNumber = 0; fileNumber < leFileCount->text().toInt(); fileNumber++)
        {
            QFile finalFile("abs" + QString::number(fileNumber+1) + ".mif");
            if(finalFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                conclusion->append("Создание файла " + finalFile.fileName() + ".");
                QTextStream stream(&finalFile);

                stream << "WIDTH = " + leWidth->text() + ";\n";
                stream << "DEPTH = " + leDepth->text() + ";\n\n";

                stream << "ADDRESS_RADIX=HEX;\n";
                stream << "DATA_RADIX=HEX;\n\n";

                stream << "CONTENT BEGIN\n";

                // Пометка: Используем переменную j для контроля индекса в QByteArray.
                // Логика следующая: длину WIDTH делим на 8, чтобы получить шаг, с которым будем записывать данные.
                // Этот алгоритм гарантирует поочередное заполнение файлов, что может показаться излишне сложным.
                // Рассмотрим возможность упрощения алгоритма на будущем этапе разработки.
                int widthBytes = leWidth->text().toInt() / 8;
                for (int wordNumber = 0, indexContents = fileNumber * widthBytes; wordNumber < leDepth->text().toInt(); ++wordNumber)
                {
                    stream << "    " + QString::number(wordNumber, 16).toUpper() + ":";

                    //Что происходит дальше? Я буду считывать данные по байтово, чтобы отслеживать недостающие байты
                    for (int var = 0; var < widthBytes; ++var, ++indexContents)
                    {
                        if ( contents.size() <= indexContents ) stream << "FF";
                        else stream << contents.sliced(indexContents,1).toHex().toUpper();
                    }

                    stream << ";\n";
                    //Тут мы отслеживаем, сколько байтов нужно пропустить, в зависимости кокое кол-во файлов указано
                    //Мы отнимаем "-1" от leFileCount->text().toInt(), чтобы программа не пропускала свои нужные ей байты
                    if (leFileCount->text().toInt() > 1)
                    {
                        indexContents += widthBytes * (leFileCount->text().toInt() - 1);
                    }
                }

                stream << "END;";

                conclusion->append("Файл " + finalFile.fileName() + " создан.\n");
                finalFile.close();
            }
            else
            {
                conclusion->append("Произошла ошибка при создании файла " + finalFile.fileName());
                finalFile.close();
            }

        }
    }
    else
    {
        conclusion->append("Произошла ошибка при открытии файла " + initialFile.fileName());
        initialFile.close();
        return false;
    }

    initialFile.close();
    return true;

}*/

bool MainWindow::transformation()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбор файла"), "", "");

    QFile mainFile(fileName);

    if(mainFile.open(QIODevice::ReadOnly))
    {
        QDataStream dat_stream(&mainFile);
        for (int fileNumber = 0; fileNumber < leFileCount->text().toInt(); fileNumber++)
        {
            QFile intermediateFile("abs" + QString::number(fileNumber+1) + ".mif");
            if(intermediateFile.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                fileCompletion(dat_stream, intermediateFile, fileNumber);
            }
            else
            {
                QMessageBox msgBox;
                msgBox.critical(0, "Произошла ошибка", "Произошла ошибка сохранения файла, попробуйте в другой раз.");
            }
            intermediateFile.close();
        }
        mainFile.close();
        return true;
    }
    else
    {
        mainFile.close();
        QMessageBox msgBox;
        msgBox.critical(0, "Произошла ошибка", "Произошла ошибка открытия файла, попробуйте в другой раз.");
        return false;
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

void MainWindow::fileCompletion(QDataStream &dat_stream, QFile &intermediateFile, int fileNumber)
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

    tex_stream << "ADDRESS_RADIX=HEX;\n";
    tex_stream << "DATA_RADIX=HEX;\n\n";

    tex_stream << "CONTENT BEGIN\n";


    for (int wordNumber = 0; wordNumber < leDepth->text().toInt(); ++wordNumber)
    {
        tex_stream << "    " + QString::number(wordNumber, 16).toUpper() + ":";

        //Что происходит дальше? Я буду считывать данные по байтово, чтобы отслеживать недостающие байты
        for (int var = 0; var < widthBytes; ++var)
        {
            if ( dat_stream.atEnd() )
            {
                tex_stream << "FF";
            }
            else
            {
                char letter;
                dat_stream.readRawData(&letter, 1);
                tex_stream << QString::number(letter, 16).toUpper();
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
    if (transformation())
    {
        conclusion->append("Размер исходного файла идеально подходит для заполнения всех файлов.");
    }
    else
    {
        conclusion->append("Размер исходного файла недостаточен для заполнения всех файлов");
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
    if (!converterForm)
    {
        converterForm = new ConverterWindow(this);
        converterForm->setAttribute(Qt::WA_DeleteOnClose); // Установим атрибут, чтобы объект формы удалился при закрытии
    }
    converterForm->show();
}
