#include "Net.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

using namespace NeuralNetwork;

static const float BIAS = 1.0f;

// Gets a uniform random value in [Low, High]
static real RandomEqualReal(real Low, real High)
{
    return ((real) rand() / RAND_MAX) * (High-Low) + Low;
}      

// Creates a new net layer with the given number of units,
// and the given immediately-preceding layer (0 for none).
NetLayer::NetLayer(const int initUnits,
		   NetLayer* initPrevLayer)
    : units(initUnits), prevLayer(initPrevLayer)
{
	output = new real[units+1];
    error  = new real[units+1];

    if (prevLayer)
    {
	weightsPerUnit = prevLayer->getUnits()+1;

	int weightArraySize = (units+1)*weightsPerUnit;
	weight     = new real[weightArraySize];
	weightSave = new real[weightArraySize];
	dWeight    = new real[weightArraySize];
    }
    else
    {
	weightsPerUnit = 0;
	weight = 0;
	weightSave = 0;
	dWeight = 0;
    }

    weightIntermediate = new real[weightsPerUnit+1];

    output[0] = BIAS;
}

// Loads this layer from a stream where it was previous saved with save()
NetLayer::NetLayer(std::istream& in, NetLayer* initPrevLayer)
    : prevLayer(initPrevLayer)
{
    readRaw(in, units);

    printf("D %d\n", units);

    output = new real[units+1];
    error  = new real[units+1];

    if (prevLayer)
    {
	weightsPerUnit = prevLayer->getUnits()+1;

	int weightArraySize = (units+1)*weightsPerUnit;
	weight     = new real[weightArraySize];
	weightSave = new real[weightArraySize];
	dWeight    = new real[weightArraySize];
	clearWeights();
	readRawArray(in, weight, weightArraySize);
    }
    else
    {
	weightsPerUnit = 0;
	weight = 0;
	weightSave = 0;
	dWeight = 0;
    }

    weightIntermediate = new real[weightsPerUnit+1];

    output[0] = BIAS;
}

// Saves network so it can be later loaded by the (istream&) constructor
void NetLayer::save(std::ostream& out)
{
    writeRaw(out, units);
    printf("Save %d\n", units);

    if (prevLayer)
    {
	writeRawArray(out, weight, (units+1)*weightsPerUnit);
    }
}

// Frees buffers used to hold layer info
NetLayer::~NetLayer()
{
    delete[] output;
    delete[] error;

    if (weight)
    {
	delete[] weight;
	delete[] weightSave;
	delete[] dWeight;
    }

    delete [] weightIntermediate;
}

// Initialises weights of edges going into this layer to random values
void NetLayer::randomizeWeights()
{
    for (int i=0; i < (units+1)*weightsPerUnit; i++)
    {
	weight[i]  = RandomEqualReal(-0.5, 0.5);
	dWeight[i] = 0;
    }
}

// Initialises weights of edges going into this layer to random values
void NetLayer::clearWeights()
{
    for (int i=0; i < (units+1)*weightsPerUnit; i++)
    {
	weight[i]  = 0;
	dWeight[i] = 0;
    }
}

// Propagates outputs of the previous layer to the outputs of this layer
void NetLayer::propagate(real gain)
{
    real* currentWeight = &getWeight (1,0);

    for (int unitNum=1; unitNum <= units; unitNum++)
    {
	real sum = 0;
	for (int outputNum=0; outputNum < weightsPerUnit; outputNum++) {
	    sum += *currentWeight * prevLayer->getOutput(outputNum);
	    currentWeight++;
	}
	output[unitNum] = 1 / (1 + exp(-gain * sum));
    }
}

// Propagates error from this layer back to the previous layer,
// in preparation for adjustWeights, which uses the error info.
void NetLayer::backpropagate(real gain)
{
    int prevUnits = prevLayer->getUnits();

    for (int prevUnitNum=prevUnits; prevUnitNum != 0; prevUnitNum--) {
	real out = prevLayer->getOutput(prevUnitNum);
	real err = 0;
	for (int unitNum=1; unitNum <= units; unitNum++) {
	    err += getWeight(unitNum, prevUnitNum) * error[unitNum];
	}
	prevLayer->setError(prevUnitNum, gain * out * (1-out) * err);
    }
}

// Computes error of the outputs of this layer from a given set of
// target values, stores these, and returns the mean square error of
// them all. Usually used on output layer.
real NetLayer::computeError(real gain, real target[])
{
    real out, err;
    real totalError = 0;
   
    real* currentTarget = target + 0;
    real* currentOutput = output + 1;
    real* currentError  = error  + 1;

    for(int unitNum=units; unitNum != 0; unitNum--) 
    {
	out = *currentOutput;
	err = *currentTarget - out;
	*currentError = gain * out * (1-out) * err;
	totalError += err*err;
	currentOutput++;
	currentTarget++;
	currentError++;
    }

    return 0.5*totalError;
}

// Adjusts weights in order to decrease the error as established
// by previous computeError/backpropagate calls.
void NetLayer::adjustWeights(real momentum, real learningRate)
{
    int localWeightsPerUnit = weightsPerUnit;

    real* currentWeight  = &getWeight (units,prevLayer->getUnits());
    real* currentDWeight = &getDWeight (units,prevLayer->getUnits());
    
    for (int prevUnitNum=0; prevUnitNum < weightsPerUnit; prevUnitNum++) {
	weightIntermediate[prevUnitNum+1] = learningRate * prevLayer->getOutput(prevUnitNum);
    }

    for (int unitNum=units; unitNum != 0; unitNum--) {
	for (int prevUnitNum=localWeightsPerUnit; prevUnitNum != 0; prevUnitNum--) {
	    real newDeltaWeight = weightIntermediate[prevUnitNum] * error[unitNum];
	    real oldDeltaWeight = *currentDWeight;
	    *currentDWeight  = newDeltaWeight;
	    *currentWeight  += newDeltaWeight + momentum*oldDeltaWeight;
	    currentDWeight--;
	    currentWeight--;
	}
    }
}

// Deallocates storage used only during training
void NetLayer::doneTraining()
{
    delete[] error;
    error = 0;
    delete[] dWeight;
    dWeight = 0;
    delete[] weightSave;
    weightSave = 0;
}
