#include "segment.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

using namespace std;


//constructor function for the segments.
// in this case, initialize the base Segment with fixed values
Segment::Segment( void )
{
    QPoint start, end;
    double len = 50;
    double ang = 90;
    start.setX(250);
    start.setY(10);
    end = calculateEnd(start, len, ang);
    this->start = start;
    this->end = end;
    cout << "coordinates x: " << this->start.x()<< " Y: " << this->start.y() << endl;

    cout << "coordinates x: " << this->end.x()<< " Y: " << this->end.y() << endl;
    this->length = len;
    this->angle = ang;
    cout << "segment created" << endl;
}

// constructor funciton with parameters. Creates a Segment with given parameters.
Segment::Segment(QPoint start, double length, double angle){
    this->start = start;
//    this->end = end;
    this->end = calculateEnd(start, length, angle);
    this->length = length;
    this->angle = angle;
}


// Calculates the dotproduct and returns the angle of rotation.
double Segment::calculateDotProduct(QPoint effector, QPoint target, QPoint beginJoint) {
    //Dotproduct = Ax * Bx + Ay * By
    double ax, bx, ay, by, dotprod, teta, alength, blength, magnitude;
    ax = effector.x() - beginJoint.x();
    bx = target.x() - beginJoint.x();
    ay = effector.y() - beginJoint.y();
    by = target.y() - beginJoint.y();
    alength = sqrt(ax * ax + ay * ay);
    blength = sqrt(bx * bx + by * by);
    magnitude = alength * blength;
    dotprod = (ax * bx + ay * by) / magnitude;
    if(dotprod >= 1) {
        dotprod = 0.99;
    } else if(dotprod <= -1) {
        dotprod = -0.99;
    }
    cout << "ax: " << ax << " bx: " << bx << " ay: " << ay << " by " << by << endl;
    cout << "de dotprod is: " <<  dotprod << endl;
    teta = acos(dotprod) * 180 / PI;
    cout << "de teta is: " << teta << endl;
    return teta;
}

// Calculates the cross product of the two vectors.
double Segment::calculateCrossProduct(QPoint effector, QPoint target, QPoint beginJoint) {
    // crossproduct 2d = Ax * By - Bx * Ay
    double ax, ay, bx, by, cross;
    ax = effector.x() - beginJoint.x();
    bx = target.x() - beginJoint.x();
    ay = effector.y() - beginJoint.y();
    by = target.y() - beginJoint.y();
    cross = ax * by - bx * ay;
    cout << "de cross is: " << cross << endl;
    return cross;
}

// Destroy function
Segment::~Segment( void ){
}


//void Segment::turnToTarget(QPoint target) {
//    QPoint _start = this->start;
//    int x = target.x() - _start.x();
//    int y = target.y() - _start.y();
//    //toa -> tan() = overstaand/aanliggend.
//    double angle = atan2 (y, x) * 180 / PI;
//    this->end = calculateEnd(this->start, this->length, angle);
//}

void Segment::turnByAngle(double teta) {
    this->angle += teta;
    cout << "current angle  is: "<< this->angle << endl;
    this->end = calculateEnd(this->start, this->length, angle);
}

// Sets the end point to parameter target
void Segment::setEnd(QPoint target){
    this->end = target;
}
// Sets the begin point to parameter target. Needed for updating begin coordinates for upcoming segments
void Segment::setBegin(QPoint target) {
    this->start = target;
}


double Segment::getLength( void ) {
    return length;
}

// reverses the Y coordinates so it fits in the animation coordinates.
QPoint Segment::getReverseY( QPoint point, double height) {
    point.setY((height - point.y()));
    return point;
}

// calculates the end point based on the length, starting position and angle of the segment
QPoint Segment::calculateEnd(QPoint start, double length, double angle) {
    QPoint end;
    // cos = aanliggende/ schuine; aanliggende = cos * schuine
    // sin = overstaande / schuine; overstaande = sin * schuine
    end.setX(start.x() + length * cos(angle*PI/180));
    end.setY(start.y() + length * sin(angle*PI/180));
    cout << "segment x: " << end.x()<< "segment y: " << end.y() << endl;
    return end;
}
