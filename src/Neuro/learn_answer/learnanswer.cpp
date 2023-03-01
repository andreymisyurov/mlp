#include "learnanswer.h"

LearnAnswer::LearnAnswer(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::LearnAnswer) {
  m_ui->setupUi(this);
  m_ui->w_qplot->setInteraction(QCP::iRangeDrag, true);
  m_ui->w_qplot->setInteraction(QCP::iRangeZoom, true);
}

LearnAnswer::~LearnAnswer() { delete m_ui; }

void LearnAnswer::drawGraph(const std::pair<int, std::vector<double>> &in_data,
                            bool in_flag_type) {
  int type = 0;
  in_flag_type ? type = 1 : type = 0;
  m_ui->w_qplot->clearGraphs();
  // переносим данные из std::vector в QVector, добавляем один элемент,
  // соединяющий с нулем для графика кросс валидации
  QVector<double> val_err(in_data.second.begin(), in_data.second.end());
  if (in_flag_type) val_err.push_front(*in_data.second.begin());
  // также добавляем нулевую точку к точкам значений абсцисс
  QVector<double> val_epochs((long long)in_data.second.size() + type);
  // Вычисляем шаг и инициализируем начальное значение. То что слева от дробной
  // черты это то какая целочисленная цифра элементов будет на оси абсцисс
  double step = 1. * (in_data.first + type) / (double)val_epochs.size();
  double value = 0.;
  //заполняем вектор значений оси абсцисс
  for (auto &&el : val_epochs) {
    el = value;
    value += step;
  }

  // задаем начальные данные для построения графика
  m_ui->w_qplot->xAxis->setLabel("epochs");
  m_ui->w_qplot->xAxis->setRange(0., double(in_data.first));
  m_ui->w_qplot->yAxis->setLabel("error");
  m_ui->w_qplot->yAxis->setRange(
      0., *(std::max_element(in_data.second.begin(), in_data.second.end())));

  m_ui->w_qplot->addGraph();
  m_ui->w_qplot->graph(0)->addData(val_epochs, val_err);
  m_ui->w_qplot->graph(0)->setPen(
      QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
  m_ui->w_qplot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDiamond, 2.0));
  m_ui->w_qplot->replot();
}
