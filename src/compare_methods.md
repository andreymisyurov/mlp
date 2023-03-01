# Info about structure of the MLP project
In this application you can create neuro-network with a matrix or graph data structure.
Both classes are inherited from the abstract class "Perceptron".

### Matrix Method

Developing this method, I used multi-thread programming in functions for parsing, learning, and testing.
I upgraded my own class "Matrix" to a template class and inherited the new class NeuronMatrix.
And result of my work is pleased me.

### Graph metod

Developing graph method, and I didn't use a multi-thread programming, and for data storage I used
std::vector (I think convertation data from NeuronMatrix to std::vector take away some time too).
Let's compare results...

### Research
MacBook Pro (Retina, 15-inch, Mid 2015)
CPU 2,8 GHz 4-core Intel Core i7
RAM 16 ГБ 1600 MHz DDR3

layers: 3
neurons first layer: 256

| Data Sruct  | 10 loops | 100 loops | 1000 loops | middle time for 1 loop |
| ------ | ------ | ------ | ------ | ------ |
| Matrix | 78.2126 sec | 876.835 sec | 7660.74 sec | ~7,7619 sec
| Graph | 723.99 sec | 6975.95 sec | couldn't wait | ~69,999 sec
