#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <time.h>
#include <thread>
#include <chrono>
#include <signal.h>

#include "nuts_ai/nuts_ai.hpp"

using namespace std;
using namespace chrono;

size_t loop = 0;
bool running = true;
float bestScore = numeric_limits<float>::infinity();

struct BaseTestCase {
    vector<float> input;
    vector<float> expectedOutput;
};

float basic_fitness(NeuralNetwork network, vector<float> out, vector<float> expected) {
    float err;
    for (int i = 0; i < min(out.size(),expected.size()); i++) {
        err += pow(out[i]-expected[i],2.f);
        //fit += abs(out[i]-expected[i]);
    }
    float bad;
    if (bestScore == 0.f) // I have to modify this so that it stores whether the AI reached 0, because the score will be affected by the code after the 'if' statement
    for (INode* n : network.nodes) {
        if (n->getChildren().size() == 0) bad += log(loop);
    }
    return err + bad;
}

map<int,float> getScores(vector<NeuralNetwork> networks, vector<BaseTestCase> tests) {
    map<int,float> scores;

    for (int i = 0; i < networks.size(); i++) {
        NeuralNetwork net = networks[i];

        //printf("TESTING NETWORK %d\n",i);

        float score = 0;

        //printf("Running %d tests\n",tests.size());
        for (BaseTestCase test : tests) {
            //printf("  Fetching output for test case...\n");
            vector<float> out = net.getOutput(test.input);
            score += basic_fitness(net,out,test.expectedOutput);
            //printf(" -> %f : %f\n",test.expectedOutput[0],out[0]);
        }

        //printf(" B\n");
        scores[i] = score;
        //printf(" C\n");
    }

    return scores;
}

NeuralNetwork getBestNetwork(vector<NeuralNetwork> networks, map<int,float> scores) {
    float bestScore = numeric_limits<float>::infinity();
    int   bestIndex = -1;
    for (int i = 0; i < networks.size(); i++) {
        float score = scores[i];
        if (score <= bestScore) {
            bestIndex = i;
            bestScore = score;
        }
    }
    return networks[bestIndex];
}

float getBestScoreIndex(vector<NeuralNetwork> networks, map<int,float> scores) {
    float bestScore = numeric_limits<float>::infinity();
    int   bestIndex = -1;
    for (int i = 0; i < networks.size(); i++) {
        float score = scores[i];
        if (score < bestScore) {
            bestIndex = i;
            bestScore = score;
        }
    }
    return bestIndex;
}

/*
Creates a new list of networks with mutations from the provided original networks
! Consumes the provided networks
*/
vector<NeuralNetwork> createNewBatch(vector<NeuralNetwork> networks, NeuralNetwork bestNetwork) {
    vector<NeuralNetwork> newNetworks;

    for (int i = 0; i < 10; i++) {
        NeuralNetwork net = bestNetwork.clone();
        for (int m = 0; m < floor(sqrt(loop+1))+100; m++) net.introduceMutation();
        newNetworks.push_back(net);
    }

    newNetworks[0] = bestNetwork.clone(); // At least keeps the best network from the previous batch to prevent regression

    for (NeuralNetwork& net : networks) {
        net.free();
    }

    return newNetworks;
}

float randf() {
    return (float)rand()/(float)RAND_MAX;
}

int main() {

    srand(time(NULL));

    // Initial network state

    vector<INode*> inputLayer = {
        new InputNode(),
        new InputNode()
    };

    vector<INode*> hiddenLayer = {
        //new ProductNode()
    };

    vector<INode*> outputLayer = {
        new OutputNode()  
    };

    NeuralNetwork baseNetwork{inputLayer,hiddenLayer,outputLayer};

    //

    vector<NeuralNetwork> networks;

    for (int i = 0; i < 100; i++) {
        NeuralNetwork net = baseNetwork.clone();
        net.introduceMutation();
        networks.push_back(net);
    }

    // training data

    vector<BaseTestCase> tests = {};

    for (int ti = 0; ti < 100; ti++) {
        float v0 = randf()*100.f;
        float v1 = randf()*100.f;
        tests.push_back({{v0,v1},{max(v0,v1)}});
    }

    printf("=> ...\n\x1b[90mEMPTY\x1b[39m\n");
    int l = 2;

    signal(SIGINT,[](int s){
        running = false;
    });

    while (true) {

        //printf("Calculating score...\n");
        map<int,float> scores = getScores(networks,tests);
        //printf("Finding best network...\n");
        NeuralNetwork bestNetwork = getBestNetwork(networks,scores);

        int bestIndex = getBestScoreIndex(networks,scores);
        //

        bestScore = scores[bestIndex];

        if (loop%5 == 0) {
            printf("[%d] => %d %f\n",loop,bestIndex,scores[bestIndex]);
            printf("Hidden layer:\n");
            for (INode* n : bestNetwork.nodes) {
                printf("  %s (%p)\n",n->getNodeType().c_str(),n);
                for (INode* nn : n->getChildren()) {
                    printf("  -> %p (%s)\n",nn,nn->getNodeType().c_str());
                }
            }
            printf("Input layer:\n");
            for (INode* n : bestNetwork.inputLayer) {
                printf("  %s\n",n->getNodeType().c_str());
                for (INode* nn : n->getChildren()) {
                    printf("  -> %p (%s)\n",nn,nn->getNodeType().c_str());
                }
            }
            printf("Output layer:\n");
            for (INode* n : bestNetwork.outputLayer) {
                printf("  %s\n",n->getNodeType().c_str());
                for (INode* nn : n->getChildren()) {
                    printf("  -> %p (%s)\n",nn,nn->getNodeType().c_str());
                }
            }
            printf("\n");
        }

        if (!running) {
            printf("\nCurrent results:\n");
            for (BaseTestCase test : tests) {
                vector<float> out = bestNetwork.getOutput(test.input);
                printf(" > ");
                for (float v : test.expectedOutput) {
                    printf("%f ",v);
                }
                printf(": ");
                for (float v : out) {
                    printf("%f ",v);
                }
                printf("(%f)\n",basic_fitness(bestNetwork,out,test.expectedOutput));
            }
            break;
        }
        
        networks = createNewBatch(networks,bestNetwork);

        loop++;

        //this_thread::sleep_for(chrono::milliseconds(500));

    }

}