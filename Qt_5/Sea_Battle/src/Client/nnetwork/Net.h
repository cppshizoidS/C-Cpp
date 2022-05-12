/**********************************************************************
Declares everything in the NeuralNetwork namespace: declares the Net
class representing the entire network, the NetLayer class representing
a single layer of the network, and defines several I/O helpers,
accessors, and fixed parameters.

For latest version see: http://moonflare.com/code/nnetwork.php

Copyright (c) 2003, Derrick Coetzee
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

- The name of Derrick Coetzee may not be used to endorse or promote
  products derived from this software without specific prior
  written permission.

This software is provided by the copyright holders and contributors
"as is" and any express or implied warranties, including, but not
limited to, the implied warranties of merchantability and fitness
for a particular purpose are disclaimed. In no event shall the
copyright owner or contributors be liable for any direct, indirect,
incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or
services; loss of use, data, or profits; or business interruption)
however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise)
arising in any way out of the use of this software, even if advised
of the possibility of such damage.
**********************************************************************/

#ifndef _NET_H_
#define _NET_H_

#include <iostream>

#include <string.h>

namespace NeuralNetwork
{

// The floating-point type used in computations. Using float
// makes negligible speed difference.
typedef double real;

// Useful for reading raw (stored byte-for-byte) objects off an istream
template <class T>
inline void readRaw(std::istream& in, T& obj)
{
    in.read(reinterpret_cast<char*>(&obj), sizeof(T));
}

// Useful for reading raw (stored byte-for-byte) arrays off an istream
template <class T>
inline void readRawArray(std::istream& in, T* array, int size)
{
    in.read(reinterpret_cast<char*>(array), sizeof(T)*size);
}

// Useful for storing objects raw (byte-for-byte) on an ostream
template <class T>
inline void writeRaw(std::ostream& out, const T& obj)
{
    out.write(reinterpret_cast<const char*>(&obj), sizeof(T));
}

// Useful for storing arrays raw (byte-for-byte) on an ostream
template <class T>
inline void writeRawArray(std::ostream& out, const T* array, int size)
{
    out.write(reinterpret_cast<const char*>(array), sizeof(T)*size);
}

// A layer of a neural network, used by NeuralNetwork::Net
class NetLayer
{
public:
    // Creates a new net layer with the given number of units,
    // and the given immediately-preceding layer (0 for none).
    NetLayer(int initUnits, NetLayer* prevLayer);
    
    // Loads this layer from a stream where it was previous saved with save()
    NetLayer(std::istream& in, NetLayer* initPrevLayer);

    // Frees buffers used to hold weights, etc.
    ~NetLayer();

    // Gets number of perceptrons in this layer
    int getUnits() const;

    // Gets the output of the outputNum'th perceptron in this layer
    real getOutput(int outputNum) const;

    // Sets the error on the output of the given perceptron
    void setError(int errorNum, real value);

    // Initialises weights of edges going into this layer to random values
    void randomizeWeights();

    // Initialises weights of edges going into this layer to zero
    void clearWeights();

    // Saves weights for later restoring by restoreWeights. Only one
    // set of weights can be saved, usually the best seen so far.    
    void saveWeights();

    // Restores the set of weights most recently saved by saveWeights.
    void restoreWeights();
    
    // Propagates outputs of the previous layer to the outputs of this layer
    void propagate(real gain);
    
    // Propagates error from this layer back to the previous layer,
    // in preparation for adjustWeights, which uses the error info.
    void backpropagate(real gain);

    // Computes error of the outputs of this layer from a given set of
    // target values, stores these, and returns the mean square error of
    // them all. Usually used on output layer.
    real computeError(real gain, real target[]);

    // Adjusts weights in order to decrease the error as established
    // by previous computeError/backpropagate calls.
    void adjustWeights(real momentum, real learningRate);

    // Gets the values outputted by the perceptrons in this layer and
    // places them in the array outputsHolder
    void getOutputs(real* outputsHolder);

    // Sets the values outputted by the perceptrons in this layer
    void setOutputs(real* newValues);

    // Saves network so it can be later loaded by the (istream&) constructor
    void save(std::ostream& out);

    // Deallocates storage used only during training
    void doneTraining();

private:
    // Gets the weight on the weightnum'th edge coming into unit unitNum
    real& getWeight(int unitNum, int weightNum);
    // Gets the delta-weight on the weightnum'th edge coming into unit unitNum
    real& getDWeight(int unitNum, int weightNum);

private:
    int     units;          // number of units in this layer
    int     weightsPerUnit; // number of conns going into each unit
    real*   output;         // output of ith unit
    real*   error;          // error term of ith unit
    real*   weight;         // connection weights to ith unit
    real*   weightSave;     // saved weights for stopped training
    real*   dWeight;        // last weight deltas for momentum
    NetLayer* prevLayer;    // Pointer to next layer

    // A buffer used and allocated only once for efficiency
    real* weightIntermediate;
};

// Gets number of perceptrons in this layer
inline int NetLayer::getUnits() const
{
    return units;
}

// Gets the output of the outputNum'th perceptron in this layer
inline real NetLayer::getOutput(int outputNum) const
{
    return output[outputNum];
}

// Sets the error on the output of the given perceptron
inline void NetLayer::setError(int nodeNum, real value)
{
    error[nodeNum] = value;
}

// Sets the values outputted by the perceptrons in this layer
inline void NetLayer::setOutputs(real* newValues)
{
    memcpy(output+1, newValues, sizeof(real)*units);
}

// Gets the values outputted by the perceptrons in this layer and
// places them in the array outputsHolder
inline void NetLayer::getOutputs(real* outputsHolder)
{
    memcpy(outputsHolder, output+1, sizeof(real)*units);
}

// Gets the weight on the weightnum'th edge coming into unit unitNum
inline real& NetLayer::getWeight(int unitNum, int weightNum)
{
    return weight[unitNum*weightsPerUnit + weightNum];
}

// Gets the delta-weight on the weightnum'th edge coming into unit unitNum
inline real& NetLayer::getDWeight(int unitNum, int weightNum)
{
    return dWeight[unitNum*weightsPerUnit + weightNum];
}

// Saves weights for later restoring by restoreWeights. Only one
// set of weights can be saved, usually the best seen so far.
inline void NetLayer::saveWeights()
{
    memcpy(weightSave, weight, (units+1)*weightsPerUnit*sizeof(real));
}

// Restores the set of weights most recently saved by saveWeights.
inline void NetLayer::restoreWeights()
{
    memcpy(weight, weightSave, (units+1)*weightsPerUnit*sizeof(real));
}

// Generates training/test examples for the network. Inherit from this
// and pass instances of that subclass into autotrain and test.
class ExampleFactory
{
public:
    // Fills the given arrays with input values and expected output
    // values based on the next training example. Training values
    // should each be enumerated once on average per numOfExamples
    // calls.
    virtual void getExample(int inputSize, real* input,
			    int outputSize, real* output) = 0;

    // Returns number of training examples. If randomly generated,
    // pick something large but reasonable.
    virtual int numExamples() = 0;
};

// A complete multilayer feed-forward neural network
class Net
{
public:
    // Creates a new feed-forward neural network with the given number of
    // layers with the specified number of nodes in each, and the learning
    // rate, momentum factor, and gain of the sigmoid function.
    Net(int layers, int layerSizes[],
	real momentumFactor, real learningRate, real gain);

    // Loads a network from a stream where it was previous saved with save()
    Net(std::istream& in);

    // Frees all memory allocated for network
    ~Net();

    // Initializes all weights in network to random values
    void randomizeWeights();

    // Initializes all weights in network to zero
    void clearWeights();

    // Automatically trains the network until its performance on the
    // test set appears to have achieved a maximum. Returns total
    // error on the test set at completion.
    // - epochsBetweenTests establishes how many tests are done between
    //   test set checks for accuracy.
    // - cutOffError establishes how much worse, as a multiple, error has
    //   to be than the minimum error seen before we stop training. Must be >1.
    real autotrain(ExampleFactory &trainingExamples,
		   ExampleFactory &testExamples,
		   int epochsBetweenTests = 10,
		   float cutOffError = 1.1);

    // Runs the network on an input and feeds it forward to produce an
    // output. For usage after training with autotrain.
    void run(real* input, real* output);

    // Tests the network using the given example set, returning total error
    real test(ExampleFactory &testExamples);

    // Deallocates storage used only during training
    void doneTraining();

    // Saves network to a stream, to be read back in with the istream& constructor
    void save(std::ostream& out);

    void setStop(bool);
private:
    // Saves weights for later restoring by restoreWeights. Only one
    // set of weights can be saved, usually the best seen so far.
    void saveWeights();

    // Restores the set of weights most recently saved by saveWeights.
    void restoreWeights();

    // Propagates inputs of the net all the way through to outputs
    void propagate();

    // Backpropagates output errors all the way back through the network
    void backpropagate();

    // Computes and stores error of output layer and each of its components
    void computeOutputError(real* target);

    // Adjusts all weights in network to decrease error recorded by
    // previous calls to backpropagate or computeOutputError.
    void adjustWeights();

    // Sets the values of the inputs to the network
    void setInputs(real* inputs);

    // Gets the outputs and places them in the array outputs
    void getOutputs(real* outputs);

    // Trains a single training example once
    void simpleTrain(real* input, real* expectedOutput);

    // Trains for a given number of epochs on an entire training set
    void train(int epochs, ExampleFactory &trainingExamples);

private:
    int           numLayers;      // number of layers
    NetLayer**    layer;          // layers of this net
    NetLayer*     inputLayer;     // input layer
    NetLayer*     outputLayer;    // output layer
    real          momentumFactor; // momentum factor
    real          learningRate;   // learning rate
    real          gain;           // gain of sigmoid function
    real          error;          // total net error

    // These are used as temporary buffers in member funcs,
    // allocated once in the constructor for efficiency.
    real*         input;
    real*         expectedOutput;
    real*         actualOutput;

    bool m_stop;
};
    
}

#endif /* #ifndef _NET_H_ */
