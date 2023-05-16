#include "Node.hpp"

// Dummy implementations

string INode::getNodeType() { return "DummyNode"; }

INode::INode() {
    this->children = vector<INode*>();
}

INode::~INode() {
    
}

vector<INode*>& INode::getChildren() {
    return this->children;
}

vector<float>& INode::getInputVector() {
    return this->input;
}

float INode::getOutput() {
    float sum;
    for (float val : this->input) {
        sum += val;
    }
    return sum;
}

INode* INode::clone() {
    return new INode();
}