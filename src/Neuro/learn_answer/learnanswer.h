#ifndef LEARNANSWER_H
#define LEARNANSWER_H

#include <QWidget>
#include <qcustomplot.h>
#include <QVector>

namespace Ui {
class LearnAnswer;
}

class LearnAnswer: public QWidget {
 Q_OBJECT

 public:
  explicit LearnAnswer(QWidget *parent = nullptr);
  ~LearnAnswer() override;

  void drawGraph(std::pair<int, std::vector<double>> in_data);

 private:
  Ui::LearnAnswer *m_ui;
};

#endif // LEARNANSWER_H
