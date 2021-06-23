#include "about.h"
#include "ui_about.h"
#include <QPixmap>
#include <QLabel>

about::about(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    setWindowTitle("About FreeSubtitleAssistant (version 0.1)");
    setFixedSize(480,385);
    setWindowModality(Qt::ApplicationModal);

    //iniate picture
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->resize(126,140);
    pix->move(10,10);
    pix->setPixmap(QPixmap(":/res/me.jpg"));
}

about::~about()
{
    delete ui;
}
