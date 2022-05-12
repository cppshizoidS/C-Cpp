
#include "Net.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <signal.h>

using namespace NeuralNetwork;

Net *net;

void handler(int sig)
{
	net->setStop(true);
}

real ** xx;
real ** yy;
int * d;

int DS, TS, VS;

class ArrayRangeExampleFactory : public ExampleFactory {
public:
    ArrayRangeExampleFactory(int mode): currentExample(0), m_mode(mode){}
    void getExample(int inputSize, real* input, int outputSize, real* output)
    {
        //   printf("auto %d\n", m_mode);
	while( d[currentExample] != m_mode)
	{
		currentExample++;
		if(currentExample > DS)
			currentExample = 0;
	}
        //printf("mode: %d %d\n", m_mode, currentExample);
        memcpy(input, xx[currentExample],
	      100 *sizeof(real));

	output[0] = yy[currentExample][0];
	output[1] = yy[currentExample][1];

	currentExample++;
	if (currentExample > DS)
	    currentExample = 0;
    }
    int numExamples() { return m_mode ? VS : TS; }

private:
    int currentExample;
    int m_mode;
};


int main(int argc, char* argv[])
{

    signal(SIGINT, handler);

    using std::cout;
    using std::endl;

    srand(time(NULL));


    int NL = atoi(argv[1]);
    DS = 0;

    FILE * fp = fopen(argv[2], "r");

    char str[256];
    while(fgets(str, 255, fp) != NULL)
	DS++;
    

    rewind(fp);

    xx = (real **) calloc(DS, sizeof(real *));
    yy = (real **) calloc(DS, sizeof(real *));

    for(int i=0; i< DS; ++i)
    {
        xx[i] = (real *) calloc(110, sizeof(real));
        yy[i] = (real *) calloc(3, sizeof(real));
    }

    int i = 0;
    while(fgets(str, 255, fp) != NULL)
    {
        char * p = str;
	int j = 0;
	while(*p++ != ' ')
	{
		xx[i][j] = (*p - '0') / 10.0;	
		j++;
	}	
	p+=3;
	sscanf(p, "%lf %lf", &yy[i][0], &yy[i][1]);
         
        yy[i][0] /= 10.0;
        yy[i][1] /= 10.0;
 
        i++;
    }
      
    d = (int *) calloc(DS, sizeof(int));
    VS = 0;
    for(i =0; i < DS; ++i)
       if(i % 10 == 0)
       {
	  d[i] = 1; //проверочный образец
          VS++;
       }

    DS--;
    TS = DS - VS;
    printf("Total data: %d, validation: %d, training: %d.\n", DS, VS, TS);

    int layerSizes[] = { 100, NL, 2 };

    std::ifstream st;
    st.open("1", std::ifstream::binary);
    net = new Net(st);

    return 0;

    net = new Net(3, layerSizes, 0.05, 0.5, 1.0);

    // Initialize to random weights, then autotrain with training and test sets
    net->randomizeWeights();
    net->setStop(false);

    ArrayRangeExampleFactory training(0);
    ArrayRangeExampleFactory test    (1);
    
    real error = net->autotrain(training, test, 10, 1.05f);

    // Show final test set error, which should be virtually minimized
    cout << "Final test set error: " << error << endl;

    net->doneTraining();

    double * rr = (double *) calloc(2, sizeof(double));
    for(i =0; i< DS; i++)
	if(d[i])
	{             
		net->run(xx[i], rr);
		printf(": %.1f %.1f => %.1f %.1f\n", yy[i][0], yy[i][1], rr[0], rr[1]);
	}

    std::ofstream out("backprop.nnw", std::ios::binary);
    net->save(out);

    return 0;
}
