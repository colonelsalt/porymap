#ifndef MOVABLERECT_H
#define MOVABLERECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QRgb>

class MovableRect : public QGraphicsItem {
public:
    MovableRect(bool* enabled, int width, int height, QRgb color);
    QRectF boundingRect() const override {
        qreal penWidth = 4;
        return QRectF(-penWidth, -penWidth, 30 * 8 + penWidth * 2, 20 * 8 + penWidth * 2);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        painter->setPen(this->color);
        painter->drawRect(-2, -2, this->width + 3, this->height + 3);
        painter->setPen(QColor(0, 0, 0));
        painter->drawRect(-3, -3, this->width + 5, this->height + 5);
        painter->drawRect(-1, -1, this->width + 1, this->height + 1);
    }
    void updateLocation(int x, int y);
    bool* enabled;

private:
    int width;
    int height;
    QRgb color;
};

#endif // MOVABLERECT_H
