#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAnimated(true);
    animationTimer = new QTimer;
    connect(animationTimer,SIGNAL(timeout()),this,SLOT(animationTriggered()));
    animationTimer->start(10);
    ui->lineEdit_granularity->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    ui->lineEdit_granularity->setMinimumWidth(50);
    ui->lineEdit_granularity->setText(QString::number(COptions::granularity));
    ui->lineEdit_granularity->setValidator(new QIntValidator(0,9999,this));
    ui->horizontalSlider->setValue(CMandelbrotOptions::iIterations);
    ui->horizontalSlider_2->setValue(CJuliaOptions::iIterations);
    ui->label_2->setNum(CMandelbrotOptions::iIterations);
    ui->label_4->setNum(CJuliaOptions::iIterations);
    ui->groupAnimate->setChecked(COptions::animating);
    connect(ui->horizontalSlider,&QSlider::valueChanged,ui->label_2,(void (QLabel::*) (int))&QLabel::setNum);
    connect(ui->horizontalSlider_2,&QSlider::valueChanged,ui->label_4,(void (QLabel::*) (int))&QLabel::setNum);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionProperties_triggered()
{
    ui->dockWidget_3->show();
    ui->dockPropertiesV->show();
}

void MainWindow::animationTriggered()
{
    if (ui->groupAnimate->isChecked())
    {
        COptions::animating=true;
        COptions::animationOffset-=0.01;
        ui->openGLWidget->update();
        ui->openGLWidget_2->update();
        animationTimer->start(10);
    }else{
        COptions::animating=false;
    }
}

void MainWindow::on_groupAnimate_toggled(bool arg1)
{
    if (arg1)
        animationTriggered();
    else
        animationTimer->stop();
}

void MainWindow::on_lineEdit_granularity_textChanged(const QString &arg1)
{
    bool bOk;
    int iNewGran=arg1.toInt(&bOk);
    if (bOk)
    {
            if (iNewGran>0)
                COptions::granularity=iNewGran;
    }
}


