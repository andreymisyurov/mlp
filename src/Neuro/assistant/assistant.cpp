#include "assistant.h"

using namespace s21;

Assistant::Assistant() : m_scene(new PaintScene), m_graph_er(new LearnAnswer){};

Assistant::~Assistant() {
  delete m_scene;
  delete m_graph_er;
};

Assistant &Assistant::getInstance() {
  static Assistant instance;
  return instance;
}

PaintScene *Assistant::getScene() { return m_scene; }

void Assistant::showWidget(const QString &in_path,
                           std::optional<QMessageBox::Icon> in_widget,
                           std::optional<QString> in_title,
                           std::optional<QString> in_message) {
  m_effect.setSource(QUrl::fromLocalFile(in_path));
  m_effect.play();
  if (in_widget)
    QMessageBox(in_widget.value(), in_title.value(), in_message.value(),
                QMessageBox::StandardButton::Apply)
        .exec();
}

NeuronMatrix Assistant::sceneToNeuronMatrix(PaintScene *in_scene) {
  NeuronMatrix result(1, 784);
  QImage image(in_scene->sceneRect().size().toSize(), QImage::Format_RGB32);

  QPainter painter(&image);
  in_scene->render(&painter);
  auto el = image.scaled(QSize(28, 28), Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);
  el.invertPixels();

  for (int l = 0, n = el.size().width(); l < n * n; ++l) {
    int i = l / n;
    int j = l % n;
    result(0, i * n + j) = static_cast<double>(qGray(el.pixel(i, j)) / 255.);
  }

  return result;
}

void Assistant::createGraphic(
    const std::pair<int, std::vector<double>> &in_data, bool in_type) {
  m_graph_er->drawGraph(std::move(in_data), in_type);
  m_graph_er->setWindowModality(Qt::ApplicationModal);
  m_graph_er->show();
}

void Assistant::openImg() {
  QString path = openFile("*.bmp");
  if (path == "") return;
  QImage temp(path);
  if (temp.isNull()) return;
  auto el = temp.scaled(m_scene->sceneRect().size().toSize(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  m_scene->addPixmap(QPixmap::fromImage(el));
}

QString Assistant::openFile(const QString &in_tail) {
  return QFileDialog::getOpenFileName(nullptr, "OpenDialog", "", in_tail);
}

QString Assistant::openSaveFile() {
  QString result = QFileDialog::getSaveFileName(nullptr, "OpenDialog", "", "");
  if (result != "") result += "_weights.bin";
  return result;
}

Perceptron *Assistant::createPerceptron(int in_layers, int in_first_layer,
                                        bool in_mode) {
  return in_mode ? (Perceptron *)new PerzeptronMtx(in_layers, in_first_layer)
                 : (Perceptron *)new PerzeptronGrph(in_layers, in_first_layer);
}

auto Assistant::createAnalyticString(const analytical_data &in_date)
    -> QString {
  QString result =
      "Average accuracy = " + QString::number(in_date.average_accuracy) + '\n' +
      "Precision = " + QString::number(in_date.precision) + '\n' +
      "Recall = " + QString::number(in_date.recall) + '\n' +
      "F-measure = " + QString::number(in_date.f_measure) + '\n' +
      "Time = " + QString::number(in_date.time) + '\n';
  return result;
}
