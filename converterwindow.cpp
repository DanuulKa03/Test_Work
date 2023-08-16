#include "converterwindow.h"




ConverterWindow::ConverterWindow(QWidget *parent)
    : QMainWindow{parent}
{
    resize(400, 150);

    setWindowTitle("Create to binary file");

    setCentralWidget(new QWidget());

    setAttribute(Qt::WA_DeleteOnClose);

    memoryAllocation();

    assistantMain->addWidget(labText);
    assistantMain->addWidget(leText);

    main->addLayout(assistantMain);

    main->addWidget(btnConverterTextToBinary);

    setLayout(main);

    QObject::connect(btnConverterTextToBinary, &QPushButton::clicked, this, &ConverterWindow::saveFileToBinary);

}

void ConverterWindow::memoryAllocation()
{

    btnConverterTextToBinary = new QPushButton("Create to binary file",this);

    labText = new QLabel("Text:", this);

    leText = new QLineEdit(this);

    main = new QVBoxLayout(centralWidget());

    assistantMain = new QHBoxLayout(this);
}

void ConverterWindow::saveFileToBinary()
{
    QString str = QFileDialog::getSaveFileName(0, "Save file", "", "");

    if (!str.isEmpty()) {
        // Пользователь нажал кнопку "Сохранить"


        QFile file(str);
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Qt_6_4);

            QByteArray stringBytes = leText->text().toUtf8();

            if (stream.writeRawData(stringBytes.constData(), stringBytes.size()) == -1 )
            {
                QMessageBox msgBox;
                msgBox.critical(0, "Произошла ошибка", "Произошла ошибка сохранения файла, попробуйте в другой раз.");
            }
        }
        file.close();
        close();
    }
}
