#include "paintscene.h"

PaintScene::PaintScene(QObject *parent)
    : QGraphicsScene{parent} {

}

PaintScene::~PaintScene() {

}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    this->addEllipse(event->scenePos().x() - 20,
                     event->scenePos().y() - 20,
                     40,
                     40,
                     QPen(Qt::NoPen),
                     QBrush(Qt::white));
    m_prev_point = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    addLine(m_prev_point.x(),
            m_prev_point.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::white, 40, Qt::SolidLine, Qt::RoundCap));
    m_prev_point = event->scenePos();
}
