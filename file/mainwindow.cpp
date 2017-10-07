#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsView>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPen>
#include <QFileDialog>
#include <QIODevice>
#include <QTableWidget>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new qscene();
    ui->graphicsView->setScene(scene);
    interface();
}
void MainWindow::wheelEvent(QWheelEvent *event)
{
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        double tile=1.15;
    if(event->delta()>0)
        ui->graphicsView->scale(tile,tile);
    else
        ui->graphicsView->scale(1/tile,1/tile);
}

void MainWindow::interface()
{
    ui->savekmeanfile->setEnabled(false);
    ui->run_knn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    ui->Clustering->setEnabled(true);
    ui->savekmeanfile->setEnabled(false);
    ui->run_knn->setEnabled(false);
    ui->open_kmean_file->setEnabled(true);
    ui->open_knn_file->setEnabled(true);
    ui->spinBox->setValue(0);
    scene->Clustering.clear();
    scene->rectct.clear();
    scene->pointinst.clear();
    scene->color.clear();
    scene->eclipinst.clear();
    scene->pointct.clear();

    scene->index.clear();
    scene->test_mouse=0;
    test.clear();
    group.clear();
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("X"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Y"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Group"));

    ui->link_file_knn->setText("");
    ui->edit_x->setText("");
    ui->edit_y->setText("");
    scene->clear();
    scene = new qscene();
    ui->graphicsView->setScene(scene);
}

void MainWindow::on_btnReset_clicked()
{
    reset();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    QPen pen(Qt::black);
    if(column!=2)
    {

        QString temp = ui->tableWidget->item(row,0)->text();
        double value_x=temp.toDouble();
        temp = ui->tableWidget->item(row,1)->text();
        double value_y=temp.toDouble();
        scene->cicle->setPen(pen);
        for(int i=0;i<scene->pointinst.size();i++)
        {
            if(value_x*scene->dv==scene->pointinst[i].x() && scene->Height-value_y*scene->dv == scene->pointinst[i].y())
            {
                scene->cicle->setPos(scene->pointinst[i].x()-3,scene->pointinst[i].y()-3);
                scene->cicle->setVisible(true);
                break;
            }
        }
    }
}

void MainWindow::on_open_kmean_file_clicked()
{
    reset();
    ui->open_knn_file->setEnabled(false);
    ui->run_knn->setEnabled(false);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("X"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Y"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Group"));
    QVector<QPointF> train;
    QString s = ui->link_file_kmean->text();
    ui->link_file_kmean->setText(QFileDialog::getOpenFileName(this,"Open",s,"*.txt"));
    if(ui->link_file_kmean->text()!="")
    {
        QFile file(ui->link_file_kmean->text());
        if(file.open(QIODevice::ReadOnly))
        {
            int count=0;
            QTextStream input(&file);
            while (!input.atEnd())
            {
                double x,y;
                input >> x >> y;
                ui->tableWidget->setRowCount(count+1);
                ui->tableWidget->setRowCount(count+1);
                ui->tableWidget->setItem(count,0,new QTableWidgetItem(QString::number(x)));
                ui->tableWidget->setItem(count,1,new QTableWidgetItem(QString::number(y)));
                train.append(QPointF(x,y));
                count++;
            }
               file.close();
        }
        train.pop_back();
            scene->appear_file_txt(train);
    }
}
void MainWindow::on_open_knn_file_clicked()
{
    reset();
    ui->run_knn->setEnabled(true);
    ui->open_kmean_file->setEnabled(false);
    ui->savekmeanfile->setEnabled(false);
    ui->Clustering->setEnabled(false);
    scene->test_mouse=1;
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("X"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Y"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Group"));
    QString s = ui->link_file_knn->text();
    ui->link_file_knn->setText(QFileDialog::getOpenFileName(this,"Open",s,"*.INP"));
    if(ui->link_file_knn->text()!="")
    {
        QFile file(ui->link_file_knn->text());
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream input(&file);
            int count=0;
            while (!input.atEnd())
            {
                double x,y;
                int z;
                input >> x >> y >> z;
                if(x==0 && y==0 && z==0) break;
                ui->tableWidget->setRowCount(count+1);
                ui->tableWidget->setItem(count,0,new QTableWidgetItem(QString::number(x)));
                ui->tableWidget->setItem(count,1,new QTableWidgetItem(QString::number(y)));
                ui->tableWidget->setItem(count,2,new QTableWidgetItem(QString::number(z+1)));
                test.push_back(QPointF(x,y));
                group.push_back(z);
                count++;
            }
                file.close();
        }
        scene->appear_file_inp(test,group);
    }
}

void MainWindow::on_savekmeanfile_clicked()
{
    ui->run_knn->setEnabled(false);
    ui->open_knn_file->setEnabled(false);
    ui->savekmeanfile->setToolTip(tr("Save data to a file"));
    QString filename = QFileDialog::getSaveFileName(this,tr("Save data trained"), "",tr("Save data trained (*.INP)"));
    if(filename.isEmpty())
        return;
    else
        scene->savefileKmean(filename);
}

void MainWindow::on_Clustering_clicked()
{
    ui->run_knn->setEnabled(false);
    ui->open_knn_file->setEnabled(false);
    ui->savekmeanfile->setEnabled(true);

    if(scene->pointinst.size()==0)
        QMessageBox::warning(this,"Warning","Ban chua nhap diem!!");
    else
    {
        if(ui->spinBox->value()==0)
            QMessageBox::warning(this,"Warning","Ban chua chon so luong layer!!");
        else if(ui->spinBox->value()>scene->pointinst.size())
            QMessageBox::warning(this,"Warning","So luong cum nen nho hon so luong doi tuong");
        else{
            K = ui->spinBox->value();
            for(int i=0;i<scene->pointct.size();i++)
                scene->removeItem(scene->rectct[i]);
            for(int i=0;i<scene->pointinst.size();i++)
                scene->eclipinst[i]->setBrush(QBrush(Qt::blue));
            scene->random(K);
            scene->kmeans();

            ui->tableWidget->setColumnCount(3);
            ui->tableWidget->setColumnWidth(2,100);
            ui->tableWidget->setRowCount(scene->pointinst.size());
            int count=0;
            ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("X"));
            ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Y"));
            ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Group"));
            for(int i=0;i<scene->Clustering.size();i++)
                for(int j=0;j<scene->Clustering[i].size();j++)
                {
                    ui->tableWidget->setItem(count,0,new QTableWidgetItem(QString::number(scene->Clustering[i][j].x()/scene->dv)));
                    ui->tableWidget->setItem(count,1,new QTableWidgetItem(QString::number((scene->Height-scene->Clustering[i][j].y())/scene->dv)));
                    ui->tableWidget->setItem(count,2,new QTableWidgetItem(QString::number(i+1)));
                    for(int j=0;j<2;j++)
                    ui->tableWidget->item(count,j)->setTextAlignment(Qt::AlignCenter);
                    count++;
                }
        }
    }
}

void MainWindow::on_run_knn_clicked()
{
    scene->test_mouse=1;
    ui->open_kmean_file->setEnabled(false);
    ui->savekmeanfile->setEnabled(false);
    ui->Clustering->setEnabled(false);
    if(ui->edit_x->text()!=""&&ui->edit_y->text()!="")
        scene->Knn((ui->edit_x->text()).toDouble(),(ui->edit_y->text()).toDouble(),test,group);
    else
        QMessageBox::warning(this,"Warning","Ban chua nhap toa do x va y!!");
}

