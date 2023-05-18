#pragma once

#include "Node.hpp"

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
        //int mutation = (float)(rand() % int(10000 + log((float)loop)*100)) / 100;
        float mutation = (float)(rand() % (int)(1000))/10.f;

        if (mutation >= 0 && mutation < 5) { // New Node

            int nodeType = rand() % 8;

                 if (nodeType == 0) this->nodes.push_back(new SumNode());
            else if (nodeType == 1) this->nodes.push_back(new ProductNode());
            else if (nodeType == 2) this->nodes.push_back(new SigmoidNode());
            else if (nodeType == 3) this->nodes.push_back(new ExponentNode());
            else if (nodeType == 4) this->nodes.push_back(new SubstractNode());
            else if (nodeType == 5) this->nodes.push_back(new ReLuNode());
            else if (nodeType == 6) this->nodes.push_back(new DivisorNode());
            else if (nodeType == 7) this->nodes.push_back(new SignNode());

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

        if (mutation >= 95 && mutation < 100 && this->nodes.size()>0) { // Fix floating nodes
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