#include "assistant.h"

#include <utility>

//Assistant::Assistant() = default;
//Assistant::~Assistant() = default;

Assistant& Assistant::getInstance() {
    static Assistant instance;
    return instance;
}

std::string Assistant::getFileFromResources(const QString& in_res_name, const QString& in_filename) {
  std::string result = (QCoreApplication::applicationDirPath() + in_filename).toStdString();
  QFile crunch(in_res_name);
  crunch.open(QIODevice::ReadOnly);
  std::ofstream input(result, std::ios::binary);
  if(!input) throw MyException("can't open this file");
  input << crunch.readAll().toStdString();
  return result;
}

PaintScene* Assistant::getScene() {
    return m_scene;
}

void Assistant::playSound(const QString& path) {
  m_effect.setSource(QUrl::fromLocalFile(path));
  m_effect.play();
}

NeuronMatrix Assistant::sceneToNeuronMatrix(PaintScene *in_scene) {
    // зарефакторить цикл
    NeuronMatrix result(1,784);
    QImage image(in_scene->sceneRect().size().toSize(), QImage::Format_RGB32);

    QPainter painter(&image);
    in_scene->render(&painter);
    auto el = image.scaled(QSize(28, 28), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    el.invertPixels();

    for(int l = 0, n = el.size().width(); l < n * n; ++l) {
        int i = l / n;
        int j = l % n;
        result(0, i * n + j) = static_cast<double>(qGray(el.pixel(i,j)) / 255.);
    }

    return result;
}

void Assistant::createGraphic(std::pair<int, std::vector<double>> in_data) {
  m_graph_er->drawGraph(std::move(in_data));
  m_graph_er->setWindowModality(Qt::ApplicationModal);
  m_graph_er->show();
}

void Assistant::openImg() {
  QImage temp(openFile("*.bmp"));
  auto el = temp.scaled(m_scene->sceneRect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  m_scene->addPixmap(QPixmap::fromImage(el));
}

QString Assistant::openFile(const QString &in_tail) {
    return QFileDialog::getOpenFileName(0, "OpenDialog", "", in_tail);
}

QString Assistant::openSaveFile() {
    QString result = QFileDialog::getSaveFileName(0, "OpenDialog", "", "");
//    if (result != "") result += "_weights.txt";
    if (result != "") result += "_weights.bin";
    return result;
}

Perceptron* Assistant::createPerceptron(int in_layers, int in_first_layer, bool in_mode) {
    // в правую сторону тернарника вставить перцептрон графовым методом
    Perceptron *result = nullptr;
    if(in_mode) {
      result = new PerzeptronMtx(in_layers, in_first_layer);
    } else {
      result = new PerzeptronGrph(in_layers, in_first_layer);
    }
    return result;
}
