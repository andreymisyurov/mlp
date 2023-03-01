#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

class PaintScene : public QGraphicsScene
{

public:
    explicit PaintScene(QObject *parent = nullptr);
    ~PaintScene();

private:
    QPointF m_prev_point;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // PAINTSCENE_H
