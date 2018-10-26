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

/*
 *  Draws the segments based on the Segment vector list
 */
void ScribbleArea::drawSegment() {
    clearImage();
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    for(Segment i : segList) {
        QPoint startYReverse, endYReverse;
        int windowHeight = height();
        // reverses the coordinates from a perspective where the bottom left is 0,0 to
        // coordinates where the bottom equals the windowHeight
        startYReverse = i.getReverseY(i.start, windowHeight);
        endYReverse = i.getReverseY(i.end, windowHeight);
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawLine(startYReverse, endYReverse);
        // draws a circle at the end of the segment.
        painter.setPen(QPen(Qt::darkCyan, 5, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawEllipse(endYReverse.x(), endYReverse.y(), 5, 5);
    }
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(firstPoint, lastPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
}


void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        QPoint mousePos = event->pos();
        mousePos.setY(height()-mousePos.y());
        target = mousePos;
        // first click for drawing the segments
        if (!started){
            initSegments();
            drawSegment();
            update();
            started = true;
        } else {
            //calculates the position of the segments based on the mouseclick and draws them
            doMagic();
        }
    }
}

/*  where the crazy stuff happens
    calculates the position of the segments based on the mouseclick and draws them
    This function calculates the rotation of the joints and based on that, draws them.
    The difference between this version of CCD and a regular version of CCD, is that this version
    will prioritize the end joints. In regular CCD, the enire arm moves, while this version
    will keep on trying to reach the target with the top joints first and if that's not possible,
    will try to rotate using the bottom joints.
 */
void ScribbleArea::doMagic(){
    int maxAttempts = 10;
    int counter = 0;
    bool done = false;
    double distTargetX, distTargetY;
    double arrivalRange = 25;
    //some small dampening so the robotarm won't move too crazy
    double dampening = 30;
    while( counter < maxAttempts && !done) {
        for(std::size_t i = (segList.size()); i!=0; --i) {
            QPoint effector, beginJoint;
            // grabs the effector position and the current joint bottom position.
            effector = segList[segList.size()-1].end;
            beginJoint = segList[i-1].start;
            // returns the angle needed to rotate
            double dot = segList[i-1].calculateDotProduct(effector, target, beginJoint);
            // dampening, so the joint can't rotate more than the amount of dampening.
            if(dot >= dampening) {
                dot = dampening;
            } else if (dot <= dampening*-1) {
                dot = dampening*-1;
            }
            // Using the cross product to calculate the direction of the rotation.
            double cross = segList[i-1].calculateCrossProduct(effector, target, beginJoint);
            if(cross < 0 ) {
                dot *= -1;
            }
            //updates the values of the segments using the new positions.
            updateSegments(i-1, dot);
            drawSegment();
            update();
            // if the end effector is in range, quit
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

//delay function, so the arm animates
void ScribbleArea::delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

// based of the current segment and angle, calculates the turn and changes the positions
//of the beginnings and ends of all the segments
void ScribbleArea::updateSegments(size_t index, double angle) {
    for(size_t i = index; i<=segList.size()-1; ++i){
        segList[i].turnByAngle(angle);
        if(i+1 <segList.size()) {
            segList[i+1].setBegin(segList[i].end);
        }
    }
}


//initialize the segments and put them in a list of Segments.
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

