#include "Node.hpp"

string OutputNode::getNodeType() { return "OutputNode"; }

OutputNode::OutputNode() {
    this->children = vector<INode*>();
}

OutputNode::~OutputNode(){}

vector<INode*>& OutputNode::getChildren() {
    return this->children;
}

vector<float>& OutputNode::getInputVector() {
    return this->input;
}

float OutputNode::getOutput() {
    float sum;
    for (float val : this->input) {
        sum = val;
    }
    return sum;
}

INode* OutputNode::clone() {
    return new OutputNode();
}