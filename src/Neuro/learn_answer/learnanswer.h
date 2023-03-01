#ifndef SRC_NEURO_LEARNANSWER_LEARNANSWER_H
#define SRC_NEURO_LEARNANSWER_LEARNANSWER_H

#include <QVector>
#include <QWidget>

#include "qcustomplot.h"
#include "ui_learnanswer.h"

namespace Ui {
class LearnAnswer;
}

class LearnAnswer : public QWidget {
  Q_OBJECT

 public:
  explicit LearnAnswer(QWidget *parent = nullptr);
  ~LearnAnswer() override;

  void drawGraph(const std::pair<int, std::vector<double>> &in_data,
                 bool in_flag_type);

 private:
  Ui::LearnAnswer *m_ui;
};

#endif  // SRC_NEURO_LEARNANSWER_LEARNANSWER_H
