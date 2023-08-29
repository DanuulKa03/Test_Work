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

bool ConverterWindow::dataChecking()
{
    if (leText->text() == "")
    {
        return false;
    }
    return true;
}

void ConverterWindow::saveFileToBinary()
{
    if (dataChecking())
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
        else
        {
            QMessageBox msgBox;
            msgBox.critical(0, "Произошла ошибка", "Выберите место для сохранения. Потом нажмиет кнопку сохранить.");
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.warning(0, "Произошла ошибка", "Введите в поле TEXT, символы.");
    }


}
