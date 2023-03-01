#ifndef SRC_NEURO_ASSISTANT_ASSISTANT_H
#define SRC_NEURO_ASSISTANT_ASSISTANT_H

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QSoundEffect>
#include <QString>
#include <optional>

#include "learnanswer.h"
#include "mtx_method/perzeptron_mtx.h"
#include "neuron_matrix.h"
#include "paintscene.h"
#include "parser.h"
#include "perzeptron_grph.h"

namespace s21 {

class Assistant {
 public:
  Assistant(const Assistant &other) = delete;
  Assistant(Assistant &&other) = delete;
  Assistant &operator=(const Assistant &other) = delete;
  Assistant &operator=(Assistant &&other) = delete;
  ~Assistant();

 public:
  auto static getInstance()                                                             -> Assistant &;
  auto getScene()                                                                       -> PaintScene *;
  auto sceneToNeuronMatrix(PaintScene *in_scene)                                        -> NeuronMatrix;
  auto openFile(const QString &in_tail)                                                 -> QString;
  auto openSaveFile()                                                                   -> QString;
  auto openImg()                                                                        -> void;
  auto createPerceptron(int in_layers, int in_first_layer, bool in_mode)                 -> Perceptron *;
  auto createGraphic(const std::pair<int, std::vector<double>> &in_data, bool in_type)  -> void;
  auto createAnalyticString(const analytical_data &in_date)                             -> QString;
  auto showWidget(const QString &in_path,
                  std::optional<QMessageBox::Icon> in_widget,
                  std::optional<QString> in_title = std::nullopt,
                  std::optional<QString> in_message = std::nullopt)                     -> void;

 private:
  Assistant();
  PaintScene *m_scene;
  LearnAnswer *m_graph_er;
  QSoundEffect m_effect;
};

}  // namespace s21

#endif  // SRC_NEURO_ASSISTANT_ASSISTANT_H
