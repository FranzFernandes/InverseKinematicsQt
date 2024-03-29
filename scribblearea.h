#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "segment.h"
#include "vector"

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);

    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

    Segment startSeg;
    std::vector<Segment> segList;
    QPoint target;

public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void initSegments();
    void drawSegment();
    void delay();
    void updateSegments(size_t, double);
    void resizeImage(QImage *image, const QSize &newSize);
    void doMagic();
    bool modified;
    bool scribbling;
    bool started;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint firstPoint ,lastPoint;
};

#endif
