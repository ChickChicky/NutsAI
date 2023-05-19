#pragma once

#include "Node.hpp"

using lsize_t = unsigned long long;

namespace nuts_ai {

    void resetNodes( vector<INode*> inputLayer ) {

        for ( INode* node : inputLayer ) {
            node->GetInputVector().clear();
        }

    }

    template<typename T>
    static string number_to_string(T v) {
        string out;
        out.resize(sizeof(v));
        for (lsize_t i = 0; i < sizeof(v)*8ll; i++) {
            lsize_t byte_index = i / 8ll;
            out[byte_index] |= (v&(1ll<<i))>>(byte_index*8ll);
        }
        return out;
    }

    template<typename T>
    static T string_to_number(string v) {
        T out = 0;
        for (lsize_t i = 0; i < v.size()*8ll; i++) {
            lsize_t byte_index = i / 8ll;
            out |= (v[byte_index]&(1ll<<(i%8)))<<(byte_index*8ll);
        }
        return out;
    }

    /* Prefixes a string with its length as a U32 */
    string sz_str(string str) {
        string out;
        out += number_to_string(str.size());
        out += str;
        return out;
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
                    float output = node->GetOutput();
                    for (INode* child_node : node->GetChildren()) {
                        child_node->GetInputVector().push_back(output);
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
                out.push_back(node->GetOutput());
            }
            
            return out;
        }

        NeuralNetwork clone() {
            vector<INode*> newInputLayer;
            vector<INode*> newNodes;
            vector<INode*> newOutputLayer;
            map<INode*,INode*> newMapping;
            for (INode* node : this->inputLayer) {
                INode* nn = node->Clone();
                newInputLayer.push_back(nn);
                newMapping[node] = nn;
            }
            for (INode* node : this->nodes) {
                INode* nn = node->Clone();
                newNodes.push_back(nn);
                newMapping[node] = nn;
            }
            for (INode* node : this->outputLayer) {
                INode* nn = node->Clone();
                newOutputLayer.push_back(nn);
                newMapping[node] = nn;
            }

            for (int i = 0; i < this->inputLayer.size(); i++) {
                INode* node = this->inputLayer[i];
                vector<INode*> children = node->GetChildren();
                vector<INode*>& newChilden = newInputLayer[i]->GetChildren();
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
                vector<INode*> children = node->GetChildren();
                vector<INode*>& newChilden = newNodes[i]->GetChildren();
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
                vector<INode*> children = node->GetChildren();
                vector<INode*>& newChilden = newOutputLayer[i]->GetChildren();
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
                    for (INode* child_node : node->GetChildren()) {
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
                    for (INode* child_node : nn->GetChildren()) {
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
                    vector<INode*>& children = parent->GetChildren();
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
                    sourceNode->GetChildren().push_back(endNode);
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
                vector<INode*>& children = node->GetChildren();
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
                        for (INode* child_node : nn->GetChildren()) {
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

        vector<INode*> getNodes() {
            vector<INode*> nodes;
            for (INode* node : this->inputLayer) nodes.push_back(node);
            for (INode* node : this->nodes) nodes.push_back(node);
            for (INode* node : this->outputLayer) nodes.push_back(node);
            return nodes;
        }

        bool operator== (NeuralNetwork& other) {
            return this == &other;
        }

        static string genId() {
            string str = "####";
            for (int i = 0; i < str.size(); i++) str[i] = rand()%93+33;
            return str;
        }

        string serialize() {
            map<INode*,string> ids;

            vector<INode*> nodes = this->getNodes();

            for (INode* node : nodes) 
                ids[node] = NeuralNetwork::genId();

            string tmp = "";

            for (INode* node : nodes) {
                string tmp_;
                tmp_ += ids[node];
                tmp_ += sz_str(node->GetNodeType());
                vector<INode*> children = node->GetChildren();
                tmp_ += number_to_string(children.size());
                for (INode* child : children)
                    tmp_ += ids[child];
                tmp_ += sz_str(node->Deconstruct());
                tmp += sz_str(tmp_);
            }

            return tmp;
        }

        static NeuralNetwork load(string data) {
            map<string,INode*> pointers;
            map<string,vector<string>> children;

            size_t ptr = 0l;
            while (ptr < data.size()) {

                size_t sz = string_to_number<size_t>(data.substr(ptr,8l)); 
                    ptr += 8;

                string node_id = data.substr(ptr,4l); 
                    ptr += 4;

                size_t sz_node_type = string_to_number<size_t>(data.substr(ptr,8l));
                    ptr += 8;
                string node_type = data.substr(ptr,sz_node_type);
                    ptr += sz_node_type;

                size_t len_node_children = string_to_number<size_t>(data.substr(ptr,8l));
                    ptr += 8;
                vector<string> node_children;
                for (size_t i = 0; i < len_node_children; i++) {
                    node_children.push_back(data.substr(ptr,4l));
                        ptr += 4;
                }
                children[node_id] = node_children;

                size_t sz_node_data = string_to_number<size_t>(data.substr(ptr,8l));
                    ptr += 8;
                string node_data = data.substr(ptr,sz_node_data);
                    ptr += sz_node_data;

                INode* node = INode::Reconstruct(node_type,node_data);
                pointers[node_id] = node;

            }

            NeuralNetwork network{{},{},{}};

            for (pair<string,INode*> node_pair : pointers) {
                string node_id = node_pair.first;
                INode* node = node_pair.second;
                vector<INode*>& node_children = node->GetChildren();
                vector<string> raw_node_children = children[node_id];
                for (string child_id : raw_node_children) {
                    INode* child = pointers[child_id];
                    node_children.push_back(child);
                }
                if (node->GetNodeType() == "InputNode") 
                    network.inputLayer.push_back(node);
                else if (node->GetNodeType() == "OutputNode") 
                    network.outputLayer.push_back(node);
                else 
                    network.nodes.push_back(node);
            }

            return network;
        }

    };

    /*
    Creates a new list of networks with mutations from the provided original networks
    ! Consumes the provided networks (the memory of all of the networks is freed)
    */
    vector<NeuralNetwork> createNewBatch(vector<NeuralNetwork> networks, NeuralNetwork bestNetwork, size_t mutations) {
        vector<NeuralNetwork> new_networks;

        for (size_t i = 0; i < 10; i++) {
            NeuralNetwork network = bestNetwork.clone();
            for (size_t m = 0; m < mutations; m++) network.introduceMutation();
            new_networks.push_back(network);
        }

        new_networks[0] = bestNetwork.clone(); // At least keeps the best network from the previous batch to prevent regression

        for (NeuralNetwork& network : networks) {
            network.free();
        }

        return new_networks;
    }

    int GetBestScoreIndex(vector<NeuralNetwork> networks, map<int,float> scores) {
        float best_score = numeric_limits<float>::infinity();
        int   best_index = -1;
        for (int i = 0; i < networks.size(); i++) {
            float score = scores[i];
            if (score < best_score) {
                best_index = i;
                best_score = score;
            }
        }
        return best_index;
    }

    NeuralNetwork GetBestNetwork(vector<NeuralNetwork> networks, map<int,float> scores) {
        return networks[GetBestScoreIndex(networks,scores)];
    }

    float GetBestScore(vector<NeuralNetwork> networks, map<int,float> scores) {
        return scores[GetBestScoreIndex(networks,scores)];
    }

}