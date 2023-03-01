#include "learnanswer.h"
#include "ui_learnanswer.h"
#include <iostream>

LearnAnswer::LearnAnswer(QWidget *parent): QWidget(parent), m_ui(new Ui::LearnAnswer) {
  m_ui->setupUi(this);
  m_ui->w_qplot->setInteraction(QCP::iRangeDrag, true);
  m_ui->w_qplot->setInteraction(QCP::iRangeZoom, true);

}

LearnAnswer::~LearnAnswer() {
  delete m_ui;
}

void LearnAnswer::drawGraph(std::pair<int, std::vector<double>> in_data) {
  QVector<double> val_err(in_data.second.begin(), in_data.second.end());
  QVector<double> val_epochs(in_data.second.size());
  double coutn = 0;
  for(auto &&el: val_epochs) {
    el = (double)(coutn++)*in_data.first/(double)in_data.second.size();
  }

  m_ui->w_qplot->xAxis->setLabel("epochs");
  m_ui->w_qplot->xAxis->setRange(0., double(in_data.first));
  m_ui->w_qplot->yAxis->setLabel("error");
  m_ui->w_qplot->yAxis->setRange(0., *(std::max_element(in_data.second.begin(), in_data.second.end())));
//  m_ui->w_qplot->yAxis->setRange(0., 1.);

  m_ui->w_qplot->addGraph();
  m_ui->w_qplot->graph(0)->addData(val_epochs, val_err);
//  m_ui->w_qplot->graph(0)->setLineStyle((QCPGraph::LineStyle)QCPGraph::lsNone);
//  m_ui->w_qplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4.0));
  m_ui->w_qplot->replot();
  qDebug() << val_err.size() << val_epochs.size();
}
