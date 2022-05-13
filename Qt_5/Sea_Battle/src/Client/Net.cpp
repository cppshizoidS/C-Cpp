#include "Net.h"

#include <QMessageBox>

#ifdef NEURAL_NET_DEBUG
#include <iostream>
#endif

#include <stdio.h>

using namespace NeuralNetwork;

Net::Net(int layers, int layerSizes[],
	 real initLearningRate = 0.25,
	 real initMomentumFactor = 0.9,
	 real initGain = 1.0)
    :momentumFactor(initMomentumFactor),
     learningRate(initLearningRate),
     gain(initGain)
{
    // Allocate and initialise layers
    numLayers = layers;
    layer = new NetLayer*[layers];
    layer[0] = new NetLayer(layerSizes[0], 0);
    for (int i=1; i<layers; i++)
    {
	layer[i] = new NetLayer(layerSizes[i], layer[i-1]);
    }

    inputLayer  = layer[0];
    outputLayer = layer[layers-1];

    // Allocate these here to avoid massive allocation slowdowns later
    int inputSize = inputLayer->getUnits();
    input = new real[inputSize];
    int outputSize = outputLayer->getUnits();
    actualOutput   = new real[outputSize];
    expectedOutput = new real[outputSize];

    // Let weights initially be random
    randomizeWeights();
}


// Loads a network from a stream where it was previous saved with save()
Net::Net(std::istream& in)
{
    // Allocate and initialise layers
    readRaw(in, numLayers);
    readRaw(in, momentumFactor);
    readRaw(in, learningRate);
    readRaw(in, gain);

    layer = new NetLayer*[numLayers];
    layer[0] = new NetLayer(in, 0);
    for (int i=1; i<numLayers; i++)
    {
	layer[i] = new NetLayer(in, layer[i-1]);
    }

    inputLayer  = layer[0];
    outputLayer = layer[numLayers-1];

    // Allocate these here to avoid massive allocation slowdowns later
    int inputSize = inputLayer->getUnits();
    input = new real[inputSize];
    int outputSize = outputLayer->getUnits();

    printf("Out: %d\n", outputSize);
    actualOutput   = new real[outputSize];
    expectedOutput = new real[outputSize];


}

// Saves network to a stream, to be read back in with the istream& constructor
void Net::save(std::ostream& out)
{
    writeRaw(out, numLayers);
    writeRaw(out, momentumFactor);
    writeRaw(out, learningRate);
    writeRaw(out, gain);

    // Save each layer
    for (int i=0; i<numLayers; i++)
    {
	layer[i]->save(out);
    }
}

// Frees all memory allocated for network
Net::~Net()
{
    for (int i=0; i<numLayers; i++)
    {
	delete layer[i];
    }
    delete[] layer;

    delete[] input;
    delete[] actualOutput;
    delete[] expectedOutput;
}

// Initializes all weights in network to random values
void Net::randomizeWeights()
{
    // Simply call randomizeWeights on each layer except the first
    for (int layerNum=1; layerNum < numLayers; layerNum++) {
	layer[layerNum]->randomizeWeights();
    }
}

// Initializes all weights in network to zero
void Net::clearWeights()
{
    // Simply call randomizeWeights on each layer except the first
    for (int layerNum=1; layerNum < numLayers; layerNum++) {
	layer[layerNum]->clearWeights();
    }
}



void Net::saveWeights()
{
    for (int layerNum=1; layerNum < numLayers; layerNum++) {
	layer[layerNum]->saveWeights();
    }
}

void Net::restoreWeights()
{
    for (int layerNum=1; layerNum < numLayers; layerNum++) {
	layer[layerNum]->restoreWeights();
    }
}

void Net::propagate()
{
    for (int layerNum=1; layerNum < numLayers; layerNum++) {
	layer[layerNum]->propagate(gain);
    }
}

void Net::backpropagate()
{
    for (int layerNum=numLayers-1; layerNum > 0; layerNum--) {
	layer[layerNum]->backpropagate(gain);
    }
}

void Net::computeOutputError(real* target)
{
    error = outputLayer->computeError(gain, target);
}

void Net::adjustWeights()
{
    for (int layerNum=1; layerNum < numLayers; layerNum++) {
	layer[layerNum]->adjustWeights(momentumFactor, learningRate);
    }
}

void Net::setInputs(real* inputs)
{
    inputLayer->setOutputs(inputs);
}

void Net::getOutputs(real* outputs)
{
    outputLayer->getOutputs(outputs);
}

void Net::simpleTrain(real* input, real* expectedOutput)
{
    setInputs(input);
    propagate();
    computeOutputError(expectedOutput);

    // Backpropagate that error data and then adjust the weights based
    // on it to reduce total error as quickly as possible.
    backpropagate();
    adjustWeights();
}

// Trains for a given number of epochs on an entire training set
void Net::train(int epochs, ExampleFactory &trainingExamples)
{
    int inputSize = inputLayer->getUnits();
    int outputSize = outputLayer->getUnits();

    // Train on each training example an average of epochs times
    for (int n=0; n < epochs*trainingExamples.numExamples(); n++) {
	trainingExamples.getExample(inputSize, input, outputSize, expectedOutput);
	simpleTrain(input, expectedOutput);
    }
}

real Net::test(ExampleFactory &testExamples)
{
    int inputSize = inputLayer->getUnits();
    int outputSize = outputLayer->getUnits();

    real totalError = 0;


    for (int n=0; n < testExamples.numExamples(); n++) {
	testExamples.getExample(inputSize, input, outputSize, expectedOutput);
	run(input, actualOutput);
	computeOutputError(expectedOutput);
	totalError += error;
    }

#ifdef NEURAL_NET_DEBUG
   // std::cout << "Error: " << totalError << std::endl;
#endif

    return totalError;
}


real Net::autotrain(ExampleFactory &trainingExamples,
		    ExampleFactory &testExamples,
		    int epochsBetweenTests,
		    float cutOffError)
{
    real minTestError = test(testExamples);
    real testError = minTestError;

    while (testError <= cutOffError*minTestError && !m_stop) {
	train(epochsBetweenTests, trainingExamples);

	testError = test(testExamples);
	fprintf(stderr, "Test Error: %f %f\n", testError, minTestError);
	if (testError < minTestError) {
	    saveWeights();
	    minTestError = testError;
	}
    }

    restoreWeights();

    return minTestError;
}

void Net::setStop(bool stop)
{
	m_stop = stop;
}


void Net::run(real* input, real* output)
{
    setInputs(input);
    propagate();
    getOutputs(output);
}

void Net::doneTraining()
{
    for (int layerNum=0; layerNum < numLayers; layerNum++) {
	layer[layerNum]->doneTraining();
    }
}
