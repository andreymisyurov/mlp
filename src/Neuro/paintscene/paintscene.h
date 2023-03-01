#ifndef SRC_NEURO_PAINTSCENE_PAINTSCENE_H
#define SRC_NEURO_PAINTSCENE_PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

namespace s21 {

class PaintScene: public QGraphicsScene {

 public:
  explicit PaintScene(QObject *parent = nullptr);
  ~PaintScene() override;

 private:
  QPointF m_prev_point;

 private:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

} // namespace s21

#endif // SRC_NEURO_PAINTSCENE_PAINTSCENE_H
