#include "segment.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265

using namespace std;

Segment::Segment( void )
{
    QPoint start, end;
    double len = 50;
    double ang = 90;
    start.setX(250);
    start.setY(450);
    end.setX(250);
    end.setY(430);
//    end = calculateEnd(start, len, ang);
    this->start = start;
    this->end = end;
    cout << "coordinates x: " << this->start.x()<< " Y: " << this->start.y() << endl;

    cout << "coordinates x: " << this->end.x()<< " Y: " << this->end.y() << endl;
    this->length = len;
    this->angle = ang;
    cout << "segment created" << endl;
}

Segment::Segment(QPoint start, double length, double angle){
    this->start = start;
//    this->end = end;
    this->end = calculateEnd(start, length, angle);
    this->length = length;
    this->angle = angle;
}

void Segment::setEnd(QPoint target){
    this->end = target;
}

Segment::~Segment( void ){
    cout << "segment is destroyed" << endl;
}

double Segment::getLength( void ) {
    return length;
}

QPoint Segment::calculateEnd(QPoint start, double length, double angle) {
    QPoint end;
    // cos = aanliggende/ schuine; aanliggende = cos * schuine
    // sin = overstaande / schuine; overstaande = sin * schuine
    end.setX(start.x() + length * cos(angle*PI/180));
    end.setY(start.y() + length * sin(angle*PI/180));
    cout << "segment x: " << end.x()<< "segment y: " << end.y() << endl;
    return end;
}
