#include <QtWidgets>
#include <iostream>
#include <segment.h>
#include <vector>

#include "scribblearea.h"
using namespace std;

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    started = false;
    myPenWidth = 3  ;
    myPenColor = Qt::black;
    vector<Segment> segList;
    Segment startSeg( void );
    QPoint target;
}


void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void ScribbleArea::drawSegment() {
    clearImage();
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    for(Segment i : segList) {
        QPoint startYReverse, endYReverse;
        int hoogte = height();
        startYReverse = i.getReverseY(i.start, hoogte);
        endYReverse = i.getReverseY(i.end, hoogte);
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawLine(startYReverse, endYReverse);
        painter.setPen(QPen(Qt::darkCyan, 5, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawEllipse(endYReverse.x(), endYReverse.y(), 5, 5);
    }
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(firstPoint, lastPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    cout << "painted" << endl;
}


void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        QPoint mousePos = event->pos();
        mousePos.setY(height()-mousePos.y());
        target = mousePos;
        /// first click for drawing the segments
        if (!started){
            initSegments();
            drawSegment();
            update();
            started = true;
        } else {
            doMagic();
        }
    }
}

void ScribbleArea::doMagic(){
    int maxAttempts = 10;
    int counter = 0;
    bool done = false;
    double distTargetX, distTargetY;
    double arrivalRange = 25;
    double dampening = 30;
    while( counter < maxAttempts && !done) {
        for(std::size_t i = (segList.size()); i!=0; --i) {
            QPoint effector, beginJoint;
            effector = segList[segList.size()-1].end;
            beginJoint = segList[i-1].start;
            double dot = segList[i-1].calculateDotProduct(effector, target, beginJoint); // returns teta
            if(dot >= dampening) {
                dot = dampening;
            } else if (dot <= dampening*-1) {
                dot = dampening*-1;
            }
            double cross = segList[i-1].calculateCrossProduct(effector, target, beginJoint);
            if(cross < 0 ) {
                dot *= -1;
            }
            updateSegments(i-1, dot);
            drawSegment();
            update();
            distTargetX = target.x() - segList[segList.size()-1].end.x();
            distTargetY = target.y() - segList[segList.size()-1].end.y();
            if ((distTargetX * distTargetX + distTargetY * distTargetY) <= arrivalRange) {
                cout << "target reached!" << endl;
                done = true;
                break;
            }
            delay();
        }
        ++counter;
    }
}

void ScribbleArea::delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void ScribbleArea::updateSegments(size_t index, double angle) {
    for(size_t i = index; i<=segList.size()-1; ++i){
        segList[i].turnByAngle(angle);
        if(i+1 <segList.size()) {
            segList[i+1].setBegin(segList[i].end);
        }
    }
}

void ScribbleArea::initSegments(){
    cout << "height: "<< height() << "width: " <<width() << endl;
    segList.push_back(startSeg);
    for(std::size_t i=1; i<5; ++i) {
        double length = 70;
        double angle = 90;
        Segment nextSeg(segList[i-1].end, length, angle);
        segList.push_back(nextSeg);
    }
}


void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

