#include <iostream>

#include <QCryptographicHash>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>

#include "DublicateFileFinder.h"

DublicateFileFinder::DublicateFileFinder (QWidget* pwgt) : QWidget(pwgt)
{
    ptxtDir1 = new QLineEdit; ptxtDir1->setReadOnly(true);
    ptxtDir2 = new QLineEdit; ptxtDir2->setReadOnly(true);
    ptxtResult = new QTextEdit;

    QLabel* plblDir1 = new QLabel("Папка №&1");
    QLabel* plblDir2 = new QLabel("Папка №&2");
    QLabel* plblResult = new QLabel("Результаты поиска дубликатов:");
    QPushButton* pcmdDir1 = new QPushButton("&Открыть");
    QPushButton* pcmdDir2 = new QPushButton("О&ткрыть");
    QPushButton* pcmdFind = new QPushButton("&Найти дубликаты файлов");
    QPushButton* pcmdClear = new QPushButton("О&чистить результаты");

    connect(pcmdDir1, &QPushButton::clicked, [&, this](){
            QString str = QFileDialog::getExistingDirectory(nullptr, "Выберите папку", ptxtDir1->text());
            if(!str.isEmpty()) ptxtDir1->setText(str);
    });
    connect(pcmdDir2, &QPushButton::clicked, [&, this](){
            QString str = QFileDialog::getExistingDirectory(nullptr, "Выберите папку", ptxtDir2->text());
            if(!str.isEmpty()) ptxtDir2->setText(str);
    });
    connect(pcmdFind, &QPushButton::clicked, [&, this](){
            start(QDir(ptxtDir1->text()), QDir(ptxtDir2->text()));
    });
    connect(pcmdClear, &QPushButton::clicked, [&, this](){
            ptxtResult->clear();
    });

    QGridLayout* pgrdLayout = new QGridLayout;
    pgrdLayout->setContentsMargins(5, 5, 5, 5);
    pgrdLayout->setSpacing(10);
    pgrdLayout->addWidget(plblDir1, 0, 0);
    pgrdLayout->addWidget(plblDir2, 1, 0);
    pgrdLayout->addWidget(plblResult, 3, 1);
    pgrdLayout->addWidget(ptxtDir1, 0, 1);
    pgrdLayout->addWidget(ptxtDir2, 1, 1);
    pgrdLayout->addWidget(pcmdDir1, 0, 2);
    pgrdLayout->addWidget(pcmdDir2, 1, 2);
    pgrdLayout->addWidget(pcmdFind, 2, 0);
    pgrdLayout->addWidget(pcmdClear, 2, 1);
    pgrdLayout->addWidget(ptxtResult, 4, 0, 1, 3);
    setLayout(pgrdLayout);

    ptxtDir1->setObjectName("ptxtDir1");
    ptxtDir2->setObjectName("ptxtDir2");
    ptxtResult->setObjectName("ptxtResult");
    plblDir1->setObjectName("plblDir1");
    plblDir2->setObjectName("plblDir2");
    plblResult->setObjectName("plblResult");
    pcmdDir1->setObjectName("pcmdDir1");
    pcmdDir2->setObjectName("pcmdDir2");
    pcmdFind->setObjectName("pcmdFind");
    pcmdClear->setObjectName("pcmdClear");
    pgrdLayout->setObjectName("pgrdLayout");
}
void DublicateFileFinder::start(const QDir& dir1, const QDir& dir2)
{
    ptxtResult->clear();

    // Checking directories for existence
    //----------------------------------------------------------------------------
    if(!dir1.exists())
    {
        QMessageBox::critical(nullptr, "Ошибка", "Папка №1 не существует!");
        return;
    }
    if(!dir2.exists())
    {
        QMessageBox::critical(nullptr, "Ошибка", "Папка №2 не существует!");
        return;
    }
    //----------------------------------------------------------------------------

    // Checking that not the same directory is selected
    //----------------------------------------------------------------------------
    if(dir1 == dir2)
    {
        int n = QMessageBox::question(nullptr, "Предупреждение",
                                      "Выбрана одна и та же папка. Продолжить?");
        if(n == QMessageBox::No)
            return;
    }
    //----------------------------------------------------------------------------

    // Start searching for duplicate files

    // Group files by size
    //----------------------------------------------------------------------------
    QMap<qint64, QList<const QFileInfo*> > sizeFiles;

    QFileInfoList listFiles = dir1.entryInfoList(QDir::Files)
                            + dir2.entryInfoList(QDir::Files);
    for(auto i = listFiles.cbegin(); i != listFiles.cend(); ++i)
        sizeFiles[i->size()].append(i);
    //----------------------------------------------------------------------------

    // Group files by hash
    //----------------------------------------------------------------------------
    QMap<QByteArray, QList<const QFileInfo*> > hashFiles;

    for(auto i = sizeFiles.cbegin(); i != sizeFiles.cend(); ++i)
    {
        if(i.value().size() == 1)
            continue;

        for(auto j = i.value().cbegin(); j != i.value().cend(); ++j)
        {
            QFile f((*j)->absoluteFilePath());

            if(!f.open(QIODevice::ReadOnly))
            {
                QMessageBox openError;
                openError.setText("Ошибка открытия для чтения файла "
                                  + f.fileName());
                continue;
            }

            QByteArray hash = QCryptographicHash::hash(f.readAll(), QCryptographicHash::Sha512);
            hashFiles[hash].append(*j);

            f.close();
        }
    }
    //----------------------------------------------------------------------------

    // Display results
    //----------------------------------------------------------------------------
    uint count = 1;
    for(auto i = hashFiles.cbegin(); i != hashFiles.cend(); ++i)
    {
        if(i.value().size() == 1)
            continue;

        ptxtResult->append("<b>Набор дубликатов файлов №"
                           + QString::number(count) + "</b>");

        for(auto j = i.value().cbegin(); j != i.value().cend(); ++j)
            ptxtResult->append((*j)->absoluteFilePath());

        ++count;
    }
    if(count == 1)
        ptxtResult->append("Дубликаты файлов отсутствуют");
    //----------------------------------------------------------------------------
}
