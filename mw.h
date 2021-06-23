#ifndef MW_H
#define MW_H

#include <QMainWindow>
#include <QStringList>
#include <QFileDialog>
#include <QList>
#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <QVector>
#include <QtGlobal>
#include <QDateTime>
#include <QTableWidget>
#include <QFile>
#include <QList>
#include <QMenuBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MW; }
QT_END_NAMESPACE

class MW : public QMainWindow
{
    Q_OBJECT

public:
    MW(QWidget *parent = nullptr);
    ~MW();

    //acquire file list
    void getLeftFileInfo();
    void getRightFileInfo();
    //display file list
    void FillRightTable();
    void FillLeftTable();
    //cover file name from left to right
    void reName();
    //clean file list
    void CleanRight();
    void CleanLeft();
    //adjust file sequence in list
    void UpLeftFile();
    void DownLeftFile();
    void DelLeftFile();
    void UpRightFile();
    void DownRightFile();
    void DelRightFile();

    bool renamebool=false;

private:
    Ui::MW *ui;

    //file list
    QStringList LeftFileList;
    QStringList RightFileList;
    //temp QFileInfo variate
    QFileInfo LeftFileInfo;
    QFileInfo RightFileInfo;
    //file info vector (left list)
    QVector<QString>LeftFileNames;
    QVector<QString>LeftFileFNames;
    QVector<QString>LeftFilePath;
    QVector<QString>LeftFileSizes;
    QVector<QDateTime>LeftFileDates;
    //file info vector (right list)
    QVector<QString>RightFileFNames;
    QVector<QString>RightSuffix;
    QVector<QString>RightAbsFilePath;
    QVector<QString>RightFileSizes;
    QVector<QDateTime>RightFileDates;

    int LeftRow=-1;
    int RightRow=-1;
    QString ltemStr;
    QDateTime ltemDat;
    QString rtemStr;
    QDateTime rtemDat;
};
#endif // MW_H
