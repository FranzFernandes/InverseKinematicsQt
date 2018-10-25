#include <QObject>
#include <QPoint>


#ifndef SEGMENT_H
#define SEGMENT_H

class Segment
{
public:
    Segment();
    ~Segment();
    Segment(QPoint, double, double);

    double getLength( void );
//    double getAngle( void );
//    QPoint getStart( void );
//    QPoint getEnd( void );
    void setEnd( QPoint );
    double length;
    double angle;
    QPoint start;
    QPoint end;

private:

    QPoint calculateEnd(QPoint, double, double);

};

#endif // SEGMENT_H
