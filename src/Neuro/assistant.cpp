#include "assistant.h"
#include "Parser/parser.h"

Assistant::Assistant() = default;
Assistant::~Assistant() = default;

NeuronMatrix Assistant::sceneToNeuronMatrix(PaintScene *in_scene) {
    // зарефакторить цикл
    NeuronMatrix result(1,784);
    QImage image(in_scene->sceneRect().size().toSize(), QImage::Format_RGB32);

    QPainter painter(&image);
    in_scene->render(&painter);
    auto el = image.scaled(QSize(28, 28), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    for(int l = 0, n = el.size().width(); l < n * n; ++l) {
        int i = l / n;
        int j = l % n;
        result(0, i * n + j) = static_cast<double>(qGray(el.pixel(i,j)) / 255.);
    }

    return result;
}
