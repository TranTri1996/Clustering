#include "qscene.h"
#include <arrow.h>
#include <QDebug>
#include <QPen>
#include <qmath.h>
#include <qalgorithms.h>
#include <QBrush>
#include <QVector>
#include <QGraphicsEllipseItem>
#include <QCoreApplication>
#include <QTimer>
#include <QTime>
#include <QColor>
#include <QPointF>
#include <QPair>
#include <QIODevice>
#include <QGraphicsLineItem>
#include <QPolygon>
#include <QFileDialog>
#include <QMessageBox>
#define BIGNUMBER 9999999999
typedef QVector<QPointF> VP;
typedef QVector<int> ID;
qscene::qscene(QGraphicsScene *parent)
    :QGraphicsScene(parent)
{
    arrow *Oy = new arrow(QPointF(0,0),QPointF(0,Height));
    arrow *Ox = new arrow(QPointF(0,Height),QPointF(Width+dv,Height));
    addItem(Oy); addItem(Ox);
    OxArrow(Width+dv,Height);
    OyArrow(0,0);
    descartes(); // chia truc toa do ra tung dv
    cicle = new QGraphicsEllipseItem();

}
void qscene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!test_mouse){
        QPen pen(Qt::blue);
        QBrush brush(QBrush(Qt::blue));
        if (mouseEvent->button() == Qt::LeftButton){
            QPointF pt = mouseEvent->scenePos();
            if(pt.x()>=0 && pt.x()<=Width-10 && pt.y()>=0 && pt.y()<=Height-18){
                QGraphicsEllipseItem *item = new QGraphicsEllipseItem();
                item = addEllipse(pt.x(),pt.y(),dkht,dkht,pen,brush);
                pointinst.push_back(QPointF(pt.x(),pt.y()));
                eclipinst.push_back(item);
            }
        }
    }
}
void qscene::delay(int time)
{
    QTime dieTime= QTime::currentTime().addMSecs(time);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void qscene::OyArrow(int x, int y) // ve muoi ten o truc oy
{
    QPen pen(Qt::blue);
    QBrush brush(Qt::blue);
    QPolygon polygon;
    polygon<<QPoint(x+5,y);
    polygon<<QPoint(x,y-10);
    polygon<<QPoint(x-5,y);
    addPolygon(polygon,pen,brush);
}

void qscene::OxArrow(int x, int y) // ve muoi ten o truc ox
{
    QPen pen(Qt::blue);
    QBrush brush(Qt::blue);
    QPolygon polygon;
    polygon<<QPoint(x,y+5);
    polygon<<QPoint(x+10,y);
    polygon<<QPoint(x,y-5);
    addPolygon(polygon,pen,brush);
}


void qscene::descartes() // chia truc toa do theo dv
{
    QPen pen(Qt::blue);
    QBrush brush(QBrush(Qt::blue));
    QPointF O;
    O.setX(0);
    O.setY(Height);
    int j=0;
    for(int i=0;i<=Height;i+=dv)
    {
        QGraphicsTextItem * item;
        item = addText(QString::number(i-(dv-1)*j));
        j+=1;
        if(i>0)
        addRect(O.x(),Height-i+5,5,2,pen,brush);
        item->setPos(O.x()-30,Height-i);
    }
    j=0;
    for(int i=1;i<=Width;i+=dv)
    {
        QGraphicsTextItem * item;
        item = addText(QString::number(i-(dv-1)*j));
        j+=1;
        item->setPos(i+dv-10,O.y()+10);
        addRect(i+dv,O.y()-5,2,5,pen,brush);
    }
}

double qscene::distances(QPointF a, QPointF b) // tinh binh ohuong khoang cach giua 2 diem
{
    double u = a.x()-b.x();
    double v = a.y()-b.y();
    return (double)(u*u+v*v);
}

void qscene::random(int k) // random ra k centroid ban dau
{
    QPen pen(Qt::NoPen);
    QBrush brush(QBrush(Qt::blue));
    cicle = addEllipse(0,0,dkht+7,dkht+7,pen); // cai vong tron bao quanh cac diem
    cicle->setVisible(false);
    int count=0;
    pointct.clear();

    color.clear();
    rectct.clear();
    while(count<k)
    {
        int temp = rand() % (pointinst.size());
        int KT=0;
        for(int j=0;j<pointct.size();j++)
            if(pointct[j]==pointinst[temp]) {
                KT=1; break;
            }
        if(!KT){                                 // kiem tra diem centroid nay da ton tai hay chua
            pointct.push_back(pointinst[temp]);

            count++;
        }
    }
    for(int i=0;i<k;i++) // tao ra k hinh vuong the hien cho cac centroid
    {
        QGraphicsRectItem *item = new QGraphicsRectItem();
        item = addRect(pointct[i].x(),pointct[i].y(),chv,chv,pen,brush);
        rectct.push_back(item);
        int x= rand() %255,y= rand() %255,z= rand() %255;
        color.push_back(QColor(x,y,z));
    }
}
void qscene::centroid_run(int i, QPointF b) // dung de di chuyen centroid moi lan cap nhat
{
    double A = pointct[i].y()-b.y();
    double B = b.x()-pointct[i].x();
    double C = -A*b.x()-B*b.y();
    if(qAbs(pointct[i].x()-b.x())>qAbs(pointct[i].y()-b.y())) // kiem tra xem nen chay theo do chenh lech cua hoanh do hay tung do
    {
        if(pointct[i].x()<=b.x())
        {
            for(int j=(int)pointct[i].x();j<=(int)b.x();j++)
            {
                int Y = (int)(-C-A*j)/B;
                pointct[i].setX(j);
                pointct[i].setY(Y);
                rectct.at(i)->setRect(j,Y,chv,chv);
                delay(7);
            }
        }
        else{
            for(int j=(int)pointct[i].x();j>=(int)b.x();j--)
            {
                int Y = (int)(-C-A*j)/B;
                pointct[i].setX(j);
                pointct[i].setY(Y);
                rectct.at(i)->setRect(j,Y,chv,chv);
                delay(7);
            }
        }
    }

    else{
        if(pointct[i].y()<=b.y())
        {
            for(int j=(int)pointct[i].y();j<=(int)b.y();j++)
            {
                int X = (int)(-C-B*j)/A;
                pointct[i].setX(X);
                pointct[i].setY(j);
                rectct.at(i)->setRect(X,j,chv,chv);
                delay(7);
            }
        }
        else{
            for(int j=(int)pointct[i].y();j>=(int)b.y();j--)
            {
                int X = (int)(-C-B*j)/A;
                pointct[i].setX(X);
                pointct[i].setY(j);
                rectct.at(i)->setRect(X,j,chv,chv);
                delay(7);
            }
        }
    }
}
void qscene::kmeans() // giai thuat clustering du lieu
{
    Clustering.clear();
    index.clear();
    for(int i=0;i<pointct.size();i++)
    {
        Clustering.push_back(VP());
        index.push_back(ID());
    }
    unsigned long long SUM1,SUM2=0;
    int check=1;
    while(check)
    {
        SUM1=0;
        for(int i=0;i<pointinst.size();i++)
        {
            int t;
            double Min=BIGNUMBER;
            for(int j=0;j<pointct.size();j++)
            {
                double dis = distances(pointct[j],pointinst[i]);
                if(Min>dis)
                {
                    Min = dis;
                    t = j;
                }
            }
            Clustering[t].push_back(pointinst[i]);
            index[t].push_back(i);
        }

        for(int i=0;i<pointct.size();i++)
        {
            double sum_x=0,sum_y=0;
            int size = Clustering[i].size();
            for(int j=0;j<size;j++)
            {
                sum_x += Clustering[i][j].x();
                sum_y += Clustering[i][j].y();
                SUM1+=distances(Clustering[i][j],pointct[i]);
            }
            centroid_run(i,QPointF(sum_x/size,sum_y/size));
            pointct[i].setX(sum_x/size);
            pointct[i].setY(sum_y/size);
            rectct.at(i)->setRect(pointct[i].x(),pointct[i].y(),chv,chv);
            QPen Pen(Qt::black);
            rectct[i]->setPen(Pen);
            rectct[i]->setBrush(QBrush(color[i]));
            for(int j=0;j<size;j++)
            {
                eclipinst[index[i][j]]->setPen(Pen);
                eclipinst[index[i][j]]->setBrush(QBrush(color[i]));
            }
        }
        if(SUM1==SUM2) check=0;
        else {
            for(int i=0;i<pointct.size();i++) {
                Clustering[i].clear();
                index[i].clear();
            }
            SUM2=SUM1;
        }
    }
}
void qscene::Knn(double x, double y,QVector<QPointF> test,QVector<int> group)
{
    QPen pen(Qt::black);
    // tinh khoang cach tu diem x,y den cac diem con lai trong test
        QVector<double> dist;
        for(int i=0;i<test.size();i++)
        {
            double kc = distances(QPointF(x,y),test[i]);
            dist.push_back(kc);
        }
        // sort lai khoan cach
        for(int i=0;i<dist.size()-1;i++)
            for(int j=i+1;j<dist.size();j++)
                if(dist[i]>dist[j]){
                    qSwap(dist[i],dist[j]);
                    qSwap(test[i],test[j]);
                    qSwap(group[i],group[j]);
                }
        //chon ra 20% diem gan nhat
        int percent = (int)test.size()*20/100;
        if(percent==0) percent=1;
        int count[test.size()];
        for(int j=0;j<test.size();j++) count[j]=0;
        int t,Max=-1;
        for(int i=0;i<percent;i++){
            count[group[i]]+=1;
            if(Max<count[group[i]])
            {
                Max=count[group[i]];
                t=group[i];
            }
        }
        QBrush brush(QBrush(color[t]));
        QPoint P(x*dv,Height-y*dv);
        int angle=60;
        QPolygon polygon;
        polygon<<P;
        for(int i=0;i<2;i++){
            QPoint Q = moving(P,ctg,angle);
            polygon<<Q;
            angle+=120;
            P=Q;
        }
        addPolygon(polygon,pen,brush);
}
void qscene::savefileKmean(QString filename) // save data sau khi thuc hien thuc toan kmean
{
    QFile f(filename);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&f);
    for(int i=0;i<Clustering.size();i++)
        for(int j=0;j<Clustering[i].size();j++)
            out << Clustering[i][j].x()/dv<<" "<<(Height-Clustering[i][j].y())/dv<<" "<<i<<"\n";
    f.close();
}


QPoint qscene::moving(QPointF p, double kc, int huong)
{
    QPoint pnew;
    pnew.setX(p.x()+kc*cos(M_PI*huong/180));
    pnew.setY(p.y()+kc*sin(M_PI*huong/180));
    return pnew;
}

void qscene::appear_file_txt(QVector<QPointF> train)
{
    QPen pen(Qt::black);
    QBrush brush(QBrush(Qt::blue));
    for(int i=0;i<train.size();i++)
    {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem();
        item = addEllipse(train[i].x()*dv,Height-train[i].y()*dv,dkht,dkht,pen,brush);
        pointinst.push_back(QPointF(train[i].x()*dv,Height-train[i].y()*dv));
        eclipinst.push_back(item);
    }
}
void qscene::appear_file_inp(QVector<QPointF> test, QVector<int> group)
{
    QPen pen(Qt::black);
    QBrush brush(QBrush(Qt::black));
    for(int i=0;i<test.size()-1;i++)
        for(int j=i+1;j<test.size();j++)
            if(group[i]>group[j]){
                qSwap(test[i],test[j]);
                qSwap(group[i],group[j]);
            }
    color.clear();
    for(int i=0;i<=group[group.size()-1];i++){
        int x= rand() %255,y= rand() %255,z= rand() %255;
        color.push_back(QColor(x,y,z));
    }
    for(int i=0;i<test.size();i++){
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem();
        item = addEllipse(test[i].x()*dv,Height-test[i].y()*dv,dkht,dkht,pen,brush);
        item->setBrush(QBrush(color[group[i]]));
        pointinst.push_back(QPointF(test[i].x()*dv,Height-test[i].y()*dv));
        eclipinst.push_back(item);
    }
}


