#include "paintscene.h"

using namespace s21;

PaintScene::PaintScene(QObject *parent) : QGraphicsScene{parent} {}

PaintScene::~PaintScene() = default;

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    clear();
    return;
  }
  addEllipse(event->scenePos().x() - 20, event->scenePos().y() - 20, 40, 40,
             QPen(Qt::NoPen), QBrush(Qt::black));
  m_prev_point = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  addLine(m_prev_point.x(), m_prev_point.y(), event->scenePos().x(),
          event->scenePos().y(),
          QPen(Qt::black, 40, Qt::SolidLine, Qt::RoundCap));
  m_prev_point = event->scenePos();
}
