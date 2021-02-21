#include <QApplication>

#include "DublicateFileFinder.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    DublicateFileFinder dff;
//    dff.dumpObjectTree();
//    QObjectList childs = dff.children();
//    foreach(QObject* pobj, childs)
//    {
//        //qDebug() << pobj->objectName();
//        pobj->dumpObjectTree();
//        qDebug() << "\n-----------------------\n";
//    }
    dff.resize(500, 350);
    dff.setWindowTitle("Поиск дубликатов файлов");
    dff.show();
    return app.exec();
}
