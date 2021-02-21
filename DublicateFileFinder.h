#ifndef DUBLICATEFILEFINDER_H
#define DUBLICATEFILEFINDER_H

#include <QDir>
#include <QLineEdit>
#include <QTextEdit>
#include <QWidget>

class DublicateFileFinder : public QWidget
{
    Q_OBJECT

private:
    QLineEdit* ptxtDir1;
    QLineEdit* ptxtDir2;
    QTextEdit* ptxtResult;

public:
    DublicateFileFinder(QWidget* pwgt = nullptr);
    void start(const QDir& dir1, const QDir& dir2);
};
#endif // DUBLICATEFILEFINDER_H
