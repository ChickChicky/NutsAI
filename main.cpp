#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <time.h>
#include <thread>
#include <chrono>
#include <signal.h>

#include "Node.hpp"

using namespace std;
using namespace chrono;

size_t loop = 0;
bool running = true;
float bestScore = numeric_limits<float>::infinity();

void resetNodes( vector<INode*> inputLayer ) {

    for ( INode* node : inputLayer ) {
        node->getInputVector().clear();
    }

}

struct NeuralNetwork {

    vector<INode*> inputLayer;
    vector<INode*> nodes;
    vector<INode*> outputLayer;
    bool freed;

    // Frees the memory occupied by the network
    void free() {
        for (INode*& n : this->inputLayer)  if (n != NULL) delete n;
        for (INode*& n : this->nodes)       if (n != NULL) delete n;
        for (INode*& n : this->outputLayer) if (n != NULL) delete n;
        this->freed = true;
    }

    vector<float> getOutput(vector<float> input) {
        // clears the state of the network
        resetNodes(this->inputLayer);
        resetNodes(this->nodes);
        resetNodes(this->outputLayer);

        for (int i = 0; i < input.size(); i++) {
            ((InputNode*)this->inputLayer[i])->value = input[i];
        }

        vector<INode*> currentLayer;
        for (INode* node : inputLayer) {
            currentLayer.push_back(node);
        }

        while (currentLayer.size()) {
            vector<INode*> newLayer;
            for (INode* node : currentLayer) {
                //printf(" > %p\n",node);
                float output = node->getOutput();
                for (INode* child_node : node->getChildren()) {
                    child_node->getInputVector().push_back(output);
                    // &&child_node!=node&&!this->isAncestor(node,child_node)
                    if (find(newLayer.begin(),newLayer.end(),child_node)==newLayer.end()) {
                        newLayer.push_back(child_node);
                    }
                }
            }
            currentLayer = newLayer;
        }

        vector<float> out;
        for (INode* node : this->outputLayer) {
            out.push_back(node->getOutput());
        }
        
        return out;
    }

    NeuralNetwork clone() {
        vector<INode*> newInputLayer;
        vector<INode*> newNodes;
        vector<INode*> newOutputLayer;
        map<INode*,INode*> newMapping;
        for (INode* node : this->inputLayer) {
            INode* nn = node->clone();
            newInputLayer.push_back(nn);
            newMapping[node] = nn;
        }
        for (INode* node : this->nodes) {
            INode* nn = node->clone();
            newNodes.push_back(nn);
            newMapping[node] = nn;
        }
        for (INode* node : this->outputLayer) {
            INode* nn = node->clone();
            newOutputLayer.push_back(nn);
            newMapping[node] = nn;
        }

        for (int i = 0; i < this->inputLayer.size(); i++) {
            INode* node = this->inputLayer[i];
            vector<INode*> children = node->getChildren();
            vector<INode*>& newChilden = newInputLayer[i]->getChildren();
            for (int j = 0; j < children.size(); j++) {
                if (newMapping[children[j]] == NULL) {
                    printf("SHIT IS GOING ON A !!! %p\n",children[j]);
                    exit(1);
                }
                newChilden.push_back( newMapping[children[j]] );
            }
        }
        for (int i = 0; i < this->nodes.size(); i++) {
            INode* node = this->nodes[i];
            vector<INode*> children = node->getChildren();
            vector<INode*>& newChilden = newNodes[i]->getChildren();
            for (int j = 0; j < children.size(); j++) {
                if (newMapping[children[j]] == NULL) {
                    printf("SHIT IS GOING ON B %p\n",children[j]);
                    exit(1);
                }
                newChilden.push_back( newMapping[children[j]] );
            }
        }
        for (int i = 0; i < this->outputLayer.size(); i++) {
            INode* node = this->outputLayer[i];
            vector<INode*> children = node->getChildren();
            vector<INode*>& newChilden = newOutputLayer[i]->getChildren();
            for (int j = 0; j < children.size(); j++) {
                if (newMapping[children[j]] == NULL) {
                    printf("SHIT IS GOING ON C %p\n",children[j]);
                    exit(1);
                }
                newChilden.push_back( newMapping[children[i]] );
            }
        }

        return NeuralNetwork{newInputLayer,newNodes,newOutputLayer};
    }

    // Checks whether the first node is a children of the second one
    bool isAncestor( INode* descendant, INode* ancestor ) {
        vector<INode*> currentLayer = { ancestor };
        map<INode*,bool> visited;
        while (currentLayer.size()) {
            vector<INode*> newLayer;
            for (INode* node : currentLayer) {
                for (INode* child_node : node->getChildren()) {
                    if (child_node == descendant || visited.find(child_node) != visited.end()) return true;
                    if (find(newLayer.begin(),newLayer.end(),child_node)==newLayer.end())
                        newLayer.push_back(child_node);
                }
                visited[node] = true;
            }
            currentLayer = newLayer;
        }
        return false;
    }

    vector<INode*> getParents(INode* node) {
        vector<INode*> currentLayer = { this->inputLayer };
        vector<INode*> parents;
        while (currentLayer.size()) {
            vector<INode*> newLayer;
            for (INode* nn : currentLayer) {
                for (INode* child_node : nn->getChildren()) {
                    if (child_node == node) parents.push_back(child_node);
                    if (find(newLayer.begin(),newLayer.end(),child_node)==newLayer.end()&&nn!=child_node&&!this->isAncestor(nn,child_node))
                        newLayer.push_back(child_node);
                }
            }
            currentLayer = newLayer;
        }
        return parents;
    }

    void introduceMutation() {
        int mutation = (float)(rand() % int(10000 + log((float)loop)*100)) / 100;

        // 0  - 5   New Node
        // 20 - 30  Remove Node
        // 40 - 60  New Connection
        // 60 - 80  Remove Connection
        // 90 -     Fix floating nodes

        if (mutation >= 0 && mutation < 5) { // New Node

            int nodeType = rand() % 7;

                 if (nodeType == 0) this->nodes.push_back(new SumNode());
            else if (nodeType == 1) this->nodes.push_back(new ProductNode());
            else if (nodeType == 2) this->nodes.push_back(new SigmoidNode());
            else if (nodeType == 3) this->nodes.push_back(new ExponentNode());
            else if (nodeType == 4) this->nodes.push_back(new SubstractNode());
            else if (nodeType == 5) this->nodes.push_back(new ReLuNode());
            else if (nodeType == 6) this->nodes.push_back(new DivisorNode());

        }

        if (mutation >= 20 && mutation < 30 && this->nodes.size()>0) { // Remove Node
            INode* node = this->nodes[rand()%this->nodes.size()];
            vector<INode*> parents = this->getParents(node);
            for (INode* parent: parents) {
                vector<INode*>& children = parent->getChildren();
                int i = 0;
                while (i < children.size()) {
                    if (children[i]==node) {
                        delete children[i]; // DO NOT FORGET TO FREE MEMORY !!!
                        children.erase(children.begin() + i);
                        // break; // can be removed if nodes can have the same connection to another node more than once
                    } else i++;
                }
            }
        }

        if (mutation >= 40 && mutation < 60 && this->nodes.size()>0) { // New Connection
            
            int nodeIndex = rand()%(this->nodes.size()+this->inputLayer.size());
            INode* sourceNode = NULL;
            if (nodeIndex < this->nodes.size()) {
                //printf("F HIDDEN\n");
                sourceNode = this->nodes[nodeIndex];
            } else {
                //printf("F INPUT\n");
                sourceNode = this->inputLayer[nodeIndex-this->nodes.size()];
            }

            int endNodeIndex;
            if (nodeIndex < this->nodes.size()) {
                endNodeIndex = rand()%this->nodes.size()+this->outputLayer.size();
            } else {
                endNodeIndex = rand()%this->nodes.size();
            }
            INode* endNode = NULL;
            if (endNodeIndex < this->nodes.size()) {
                //printf("T HIDDEN\n");
                endNode = this->nodes[endNodeIndex];
            } else {
                //printf("T OUTPUT\n");
                endNode = this->outputLayer[endNodeIndex-this->nodes.size()];
            }

            if (sourceNode == NULL || endNode == NULL) {
                printf("VERY BIG PROBLEM %p %p\n",sourceNode,endNode);
                exit(1);
            }
            
            if (!this->isAncestor(endNode,sourceNode) && !this->isAncestor(sourceNode,endNode) && endNode != sourceNode) {
                sourceNode->getChildren().push_back(endNode);
            }

        }

        if (mutation >= 60 && mutation < 80 && this->nodes.size()>0) { // Remove Connection
            int nodeIndex = rand()%this->nodes.size()+this->inputLayer.size();
            INode* node = NULL;
            if (nodeIndex < this->nodes.size()) {
                node = this->nodes[nodeIndex];
            } else {
                node = this->inputLayer[nodeIndex-this->nodes.size()];
            }
            vector<INode*>& children = node->getChildren();
            if (children.size() > 0) {
                int childIndex = rand()%children.size();
                children.erase(children.begin() + childIndex);
            }
        }

        if (mutation >= 98 && this->nodes.size()>0) { // Fix floating nodes
            /*for (int i = 0; i < sqrt(this->nodes.size()); i++) { // Tries to fix more nodes, the more there are
                INode* node = this->nodes[rand()%this->nodes.size()];
                if (node->getChildren().size()==0) {
                    if (rand()%5) { // Removes the node
                        vector<INode*> parents = this->getParents(node);
                        for (INode* parent: parents) {
                            vector<INode*>& children = parent->getChildren();
                            int i = 0;
                            while (i < children.size()) {
                                if (children[i]==node) {
                                    delete children[i]; // DO NOT FORGET TO FREE MEMORY !!!
                                    children.erase(children.begin() + i);
                                    // break; // can be removed if nodes can have the same connection to another node more than once
                                } else i++;
                            }
                        }
                    } else { // Tries to connect it to an output
                        // vector<INode*>& children = node->getChildren();
                        // children.push_back(outputLayer[rand()%outputLayer.size()]);
                    }
                }
            }*/

            map<INode*,bool> explored;

            vector<INode*> currentLayer = { this->inputLayer };

            while (currentLayer.size()) {
                vector<INode*> newLayer;
                for (INode* nn : currentLayer) {
                    explored[nn] = true;
                    for (INode* child_node : nn->getChildren()) {
                        if (!explored[child_node])
                            newLayer.push_back(child_node);
                        explored[child_node] = true;
                    }
                }
                currentLayer = newLayer;
            }

            size_t i = 0;
            while (i < this->nodes.size()) {
                INode* node = this->nodes[i];
                if (!explored[node]) {
                    this->nodes.erase(this->nodes.begin()+i);
                    delete node;
                } else i++;
            }
        }

    }

    bool operator== (NeuralNetwork& other) {
        return this == &other;
    }

};

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
        for (int m = 0; m < floor(log(loop+1))+100; m++) net.introduceMutation();
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

    for (int i = 0; i < 500; i++) {
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
        printf("[%d] => %d %f\n",loop,bestIndex,scores[bestIndex]);

        bestScore = scores[bestIndex];

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

        printf("\n");

        loop++;

        //this_thread::sleep_for(chrono::milliseconds(500));

    }

}