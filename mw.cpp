#include "mw.h"
#include "ui_mw.h"
#include "about.h"
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>

MW::MW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MW)
{
    //Set windows looking
    ui->setupUi(this);
    setWindowTitle("FreeSubtitleAssistant 0.1");
    ui->RenameBtn->setStyleSheet("color:blue");
    ui->LeftBrowser->setRowCount(100);
    ui->RightBrowser->setRowCount(100);
    ui->LeftBrowser->setColumnWidth(0,300);
    ui->LeftBrowser->setColumnWidth(1,200);
    ui->LeftBrowser->setColumnWidth(3,100);
    ui->RightBrowser->setColumnWidth(0,300);
    ui->RightBrowser->setColumnWidth(1,200);
    ui->RightBrowser->setColumnWidth(3,100);

    //select file and fill table
    connect(ui->LeftOpenBtn, &QPushButton::clicked, [=](){
        LeftFileList = QFileDialog::getOpenFileNames(this,"select file");
        getLeftFileInfo();
        FillLeftTable();
    });
    connect(ui->RightOpenBtn, &QPushButton::clicked, [=](){
        RightFileList = QFileDialog::getOpenFileNames(this,"select file");
        getRightFileInfo();
        FillRightTable();
    });

    //check if table select
    connect(ui->LeftBrowser, &QTableWidget::itemClicked,[=](){
        LeftRow = ui->LeftBrowser->currentRow();
    });
    connect(ui->RightBrowser, &QTableWidget::itemClicked,[=](){
        RightRow = ui->RightBrowser->currentRow();
    });

    //adjust file sequence
    connect(ui->LeftUpBtn, &QPushButton::clicked,[=](){
        UpLeftFile();
        FillLeftTable();
    });
    connect(ui->LeftDnBtn, &QPushButton::clicked,[=](){
        DownLeftFile();
        FillLeftTable();
    });
    connect(ui->LeftReBtn, &QPushButton::clicked,[=](){
        DelLeftFile();
        FillLeftTable();
    });
    connect(ui->RightUpBtn, &QPushButton::clicked,[=](){
        UpRightFile();
        FillRightTable();
    });
    connect(ui->RightDnBtn, &QPushButton::clicked,[=](){
        DownRightFile();
        FillRightTable();
    });
    connect(ui->RightReBtn, &QPushButton::clicked,[=](){
        DelRightFile();
        FillRightTable();
    });


    //Clean selection
    connect(ui->LeftCleanBtn, &QPushButton::clicked, [=](){
        CleanLeft();
    });
    connect(ui->RightCleanBtn, &QPushButton::clicked, [=](){
        CleanRight();
    });

    //Cover file name from left to right
    connect(ui->RenameBtn, &QPushButton::clicked,[=](){
        reName();
    });

    //show about window
    connect(ui->About, &QAction::triggered, this, [=](){
        about* aboutw = new about;
        aboutw->show();
   });
}

MW::~MW()
{
    delete ui;
}

//---------------------------------------function area below---------------------------------//
void MW::getLeftFileInfo()
{
    //traverse "LEFT" QStringList using iterator
    for(QList<QString>::iterator i = LeftFileList.begin(); i!=LeftFileList.end(); i++)
    {
        LeftFileInfo = QFileInfo(*i);
        //acquire file FULL name
        LeftFileFNames.push_back(LeftFileInfo.fileName());
        //acquire file name before the last "."
        LeftFileNames.push_back(LeftFileInfo.completeBaseName());
        //acquire absolute path without file name
        LeftFilePath.push_back(LeftFileInfo.absolutePath());

        LeftFileSizes.push_back(QString::number(LeftFileInfo.size()/1024)+"K");
        LeftFileDates.push_back(LeftFileInfo.lastModified());
    }
}

void MW::getRightFileInfo()
{
    for(QList<QString>::iterator i = RightFileList.begin(); i!=RightFileList.end(); i++)
    {
        RightFileInfo = QFileInfo(*i);
        //acquire file FULL name
        RightFileFNames.push_back(RightFileInfo.fileName());
        //acquire file extension name after the last "."
        RightSuffix.push_back(RightFileInfo.suffix());
        //acquire file name and absolute path
        RightAbsFilePath.push_back(RightFileInfo.absoluteFilePath());

        RightFileSizes.push_back(QString::number(RightFileInfo.size()/1024)+"K");
        RightFileDates.push_back(RightFileInfo.lastModified());
    }
}

void MW::FillLeftTable()
{
//    ui->LeftBrowser->setRowCount(LeftFileFNames.size());
    //set table in-editable
    ui->LeftBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int li=0; li<LeftFileFNames.size();li++)
    {
        ui->LeftBrowser->setItem(li,0,new QTableWidgetItem(LeftFileFNames.at(li)));
        ui->LeftBrowser->setItem(li,1,new QTableWidgetItem(LeftFileDates.at(li).toString()));
        ui->LeftBrowser->setItem(li,2,new QTableWidgetItem(LeftFileSizes.at(li)));
    }
}

void MW::FillRightTable()
{
//    ui->RightBrowser->setRowCount(RightFileFNames.size());
    //set table in-editable
    ui->RightBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int ri=0; ri<RightFileFNames.size();ri++)
    {
        ui->RightBrowser->setItem(ri,0,new QTableWidgetItem(RightFileFNames.at(ri)));
        ui->RightBrowser->setItem(ri,1,new QTableWidgetItem(RightFileDates.at(ri).toString()));
        ui->RightBrowser->setItem(ri,2,new QTableWidgetItem(RightFileSizes.at(ri)));
    }
}

void MW::UpLeftFile()
{
    //check if list if empty
    if(LeftFileFNames.size() == 0)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if clicked
    else if(LeftRow == -1)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if list is too short or reach the top
    else if(LeftFileFNames.size() == 1 || LeftRow == 0 || LeftRow >= LeftFileFNames.size())
    {
        return;
    }
    else
    {
        //swap file full name
        ltemStr=LeftFileFNames.at(LeftRow);
        LeftFileFNames.replace(LeftRow,LeftFileFNames.at(LeftRow-1));
        LeftFileFNames.replace(LeftRow-1,ltemStr);
        //swap file base name
        ltemStr=LeftFileNames.at(LeftRow);
        LeftFileNames.replace(LeftRow,LeftFileNames.at(LeftRow-1));
        LeftFileNames.replace(LeftRow-1,ltemStr);
        //swap file path
        ltemStr=LeftFilePath.at(LeftRow);
        LeftFilePath.replace(LeftRow,LeftFilePath.at(LeftRow-1));
        LeftFilePath.replace(LeftRow-1,ltemStr);
        //swap file date
        ltemDat=LeftFileDates.at(LeftRow);
        LeftFileDates.replace(LeftRow,LeftFileDates.at(LeftRow-1));
        LeftFileDates.replace(LeftRow-1,ltemDat);
        //swap file size
        ltemStr=LeftFileSizes.at(LeftRow);
        LeftFileSizes.replace(LeftRow,LeftFileSizes.at(LeftRow-1));
        LeftFileSizes.replace(LeftRow-1,ltemStr);
        LeftRow--;
    }
}

void MW::DownLeftFile()
{
    //check if list if empty
    if(LeftFileFNames.size()==0)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if clicked
    else if(LeftRow == -1)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if list is too short or reach the top
    else if(LeftFileFNames.size() == 1 || LeftRow == LeftFileFNames.size()-1 || LeftRow >= LeftFileFNames.size())
    {
        return;
    }
    else
    {
        //swap file full name
        ltemStr=LeftFileFNames.at(LeftRow);
        LeftFileFNames.replace(LeftRow,LeftFileFNames.at(LeftRow+1));
        LeftFileFNames.replace(LeftRow+1,ltemStr);
        //swap file base name
        ltemStr=LeftFileNames.at(LeftRow);
        LeftFileNames.replace(LeftRow,LeftFileNames.at(LeftRow+1));
        LeftFileNames.replace(LeftRow+1,ltemStr);
        //swap file path
        ltemStr=LeftFilePath.at(LeftRow);
        LeftFilePath.replace(LeftRow,LeftFilePath.at(LeftRow+1));
        LeftFilePath.replace(LeftRow+1,ltemStr);
        //swap file date
        ltemDat=LeftFileDates.at(LeftRow);
        LeftFileDates.replace(LeftRow,LeftFileDates.at(LeftRow+1));
        LeftFileDates.replace(LeftRow+1,ltemDat);
        //swap file size
        ltemStr=LeftFileSizes.at(LeftRow);
        LeftFileSizes.replace(LeftRow,LeftFileSizes.at(LeftRow+1));
        LeftFileSizes.replace(LeftRow+1,ltemStr);
        LeftRow++;
    }
}

void MW::DelLeftFile()
{
    //check if list if empty
    if(LeftFileFNames.size() == 0)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if clicked
    else if(LeftRow == -1 || LeftRow >= LeftFileFNames.size())
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if list is too short
    else if(LeftFileFNames.size() == 1)
    {
        CleanLeft();
        return;
    }
    else
    {
        //remove file full name
        LeftFileFNames.remove(LeftRow);
        //remove file base name
        LeftFileNames.remove(LeftRow);
        //remove file path
        LeftFilePath.remove(LeftRow);
        //remove file date
        LeftFileDates.remove(LeftRow);
        //remove file size
        LeftFileSizes.remove(LeftRow);
        //remove the last row in file table
        ui->LeftBrowser->removeRow(LeftFileFNames.size());

        LeftRow = -1;
//        qDebug()<<"right now the size is"<<LeftFileNames.size()<<endl;
    }
}


void MW::UpRightFile()
{
    //check if list if empty
    if(RightFileFNames.size() == 0)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if clicked
    else if(RightRow == -1)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if list is too short or reach the top
    else if(RightFileFNames.size() == 1 || RightRow == 0 || RightRow >= RightFileFNames.size())
    {
        return;
    }
    else
    {
        //swap file full name
        rtemStr=RightFileFNames.at(RightRow);
        RightFileFNames.replace(RightRow,RightFileFNames.at(RightRow-1));
        RightFileFNames.replace(RightRow-1,rtemStr);
        //swap file extension name
        rtemStr=RightSuffix.at(RightRow);
        RightSuffix.replace(RightRow,RightSuffix.at(RightRow-1));
        RightSuffix.replace(RightRow-1,rtemStr);
        //swap file absolute file path
        rtemStr=RightAbsFilePath.at(RightRow);
        RightAbsFilePath.replace(RightRow,RightAbsFilePath.at(RightRow-1));
        RightAbsFilePath.replace(RightRow-1,rtemStr);
        //swap file date
        rtemDat=RightFileDates.at(RightRow);
        RightFileDates.replace(RightRow,RightFileDates.at(RightRow-1));
        RightFileDates.replace(RightRow-1,rtemDat);
        //swap file size
        rtemStr=RightFileSizes.at(RightRow);
        RightFileSizes.replace(RightRow,RightFileSizes.at(RightRow-1));
        RightFileSizes.replace(RightRow-1,rtemStr);
        RightRow--;
    }
}

void MW::DownRightFile()
{
    //check if list if empty
    if(RightFileFNames.size() == 0)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if clicked
    else if(RightRow == -1)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if list is too short or reach the top
    else if(RightFileFNames.size() == 1 || RightRow == RightFileFNames.size()-1 || RightRow >= RightFileFNames.size())
    {
        return;
    }
    else
    {
        //swap file full name
        rtemStr=RightFileFNames.at(RightRow);
        RightFileFNames.replace(RightRow,RightFileFNames.at(RightRow+1));
        RightFileFNames.replace(RightRow+1,rtemStr);
        //swap file extension name
        rtemStr=RightSuffix.at(RightRow);
        RightSuffix.replace(RightRow,RightSuffix.at(RightRow+1));
        RightSuffix.replace(RightRow+1,rtemStr);
        //swap file absolute file path
        rtemStr=RightAbsFilePath.at(RightRow);
        RightAbsFilePath.replace(RightRow,RightAbsFilePath.at(RightRow+1));
        RightAbsFilePath.replace(RightRow+1,rtemStr);
        //swap file date
        rtemDat=RightFileDates.at(RightRow);
        RightFileDates.replace(RightRow,RightFileDates.at(RightRow+1));
        RightFileDates.replace(RightRow+1,rtemDat);
        //swap file size
        rtemStr=RightFileSizes.at(RightRow);
        RightFileSizes.replace(RightRow,RightFileSizes.at(RightRow+1));
        RightFileSizes.replace(RightRow+1,rtemStr);
        RightRow++;
    }
}

void MW::DelRightFile()
{
    //check if list if empty
    if(RightFileFNames.size() == 0)
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if clicked
    else if(RightRow == -1 || RightRow >= RightFileFNames.size())
    {
        //QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if list is too short
    else if(RightFileFNames.size() == 1)
    {
        CleanRight();
        return;
    }
    else
    {
        //remove file full name
        RightFileFNames.remove(RightRow);
        //remove file Extension name
        RightSuffix.remove(RightRow);
        //remove absolute file path
        RightAbsFilePath.remove(RightRow);
        //remove file date
        RightFileDates.remove(RightRow);
        //remove file size
        RightFileSizes.remove(RightRow);
        //remove the last row in file table
        ui->RightBrowser->removeRow(RightFileFNames.size());

        RightRow = -1;
//        qDebug()<<"right now the size is"<<LeftFileNames.size()<<endl;
    }
}


void MW::CleanLeft()
{
    ui->LeftBrowser->clearContents();
    LeftFileList.clear();
    LeftFileFNames.clear();
    LeftFileNames.clear();
    LeftFilePath.clear();
    LeftFileSizes.clear();
    LeftFileDates.clear();
    LeftRow = -1;
}

void MW::CleanRight()
{
    ui->RightBrowser->clearContents();
    RightFileList.clear();
    RightFileFNames.clear();
    RightSuffix.clear();
    RightAbsFilePath.clear();
    RightFileSizes.clear();
    RightFileDates.clear();
    RightRow = -1;
}

void MW::reName()
{
    qDebug()<<"the list is"<<LeftFileFNames.size()<<endl;
    //check if both list empty
    if(LeftFileFNames.empty() && RightFileFNames.empty())
    {
        QMessageBox::critical(this, "Warning!", "Are you sure you selected any files? ", QMessageBox::Ok);
        return;
    }
    //check if file quantites of both lists as same
    else if(LeftFileFNames.size()!=RightFileFNames.size())
    {
        QMessageBox::critical(this, "Warning!", "You have to select same quantity files! ", QMessageBox::Ok);
        return;
    }
    else
    {
        for(int i = 0; i<LeftFileFNames.size();i++)
        {
            QFile f((RightAbsFilePath.at(i)));
            qDebug()<<RightAbsFilePath.at(i);
            renamebool = f.rename(LeftFilePath.at(i) + "/" + LeftFileNames.at(i) + "." + RightSuffix.at(i));
            qDebug()<<renamebool<<endl;
        }
        if(renamebool == true)
        {
            QMessageBox::about(this,"Done!","Renaming completed!");
            CleanLeft();
            CleanRight();
            return;
        }
        else
        {
            QMessageBox::critical(this,"Warning!","Looks not working...");
            CleanLeft();
            CleanRight();
            return;
        }
    }

}
