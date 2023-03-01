#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QPainter>

#include <paintscene.h>
#include <Matrix/neuron_matrix.h>

#include <iostream>
#include <QDebug>


class Assistant {
public:
    Assistant();
    static NeuronMatrix sceneToNeuronMatrix(PaintScene *m_scene);

private:
    ~Assistant();
};

#endif // ASSISTANT_H
