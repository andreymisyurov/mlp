#include "assistant.h"
#include "Parser/parser.h"

//Assistant::Assistant() = default;
//Assistant::~Assistant() = default;

Assistant& Assistant::getInstanse() {
    static Assistant instance;
    return instance;
}

PaintScene* Assistant::getScene() {
    return m_scene;
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

QString Assistant::openFile(const QString &in_tail) {
    return QFileDialog::getOpenFileName(0, "OpenDialog", "", in_tail);
}

QString Assistant::openSaveFile() {
    QString result = QFileDialog::getSaveFileName(0, "OpenDialog", "", "");
    if (result != "") result += "_weights.txt";
    return result;
}

Perceptron* Assistant::createPerceptron(int in_layers, int in_first_layer, bool in_mode) {
    // в правую сторону тернарника втсавить перцептрон графовым методом
    Perceptron *result = in_mode ? new PerzeptronMtx(in_layers, in_first_layer) : new PerzeptronMtx(in_layers, in_first_layer);
    return result;
}
