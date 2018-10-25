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
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawLine(i.start, i.end);
        painter.setPen(QPen(Qt::darkCyan, 5, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawEllipse(i.end.x(), i.end.y(), 5, 5);
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
        target = event->pos();
        /// first click for drawing the segments
        if (!started){
            initSegments();
            drawSegment();
            update();
            started = true;
        } else {
//            startSeg.setEnd(event->pos());
            segList[0].setEnd(event->pos());
            drawSegment();
            update();
            cout << "mouse x:" <<event->pos().x() << "mouse y:" << event->pos().y() << endl;
        }
    }
}

void ScribbleArea::initSegments(){
    cout << "height: "<< height() << "width: " <<width() << endl;
    segList.push_back(startSeg);
    for(std::size_t i=1; i<3; ++i) {
        double length = 50;
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

