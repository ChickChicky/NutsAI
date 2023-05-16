#include "Node.hpp"

string DivisorNode::getNodeType() { return "DivisorNode"; }

DivisorNode::DivisorNode() {
    this->children = vector<INode*>();
}

DivisorNode::~DivisorNode(){}

vector<INode*>& DivisorNode::getChildren() {
    return this->children;
}

vector<float>& DivisorNode::getInputVector() {
    return this->input;
}

float DivisorNode::getOutput() {
    float x = 0;
    for (float val : this->input)  {
        if (x == 0) {
            x = val;
        } else {
            x /= val;
        }
    }
    return x;
}

INode* DivisorNode::clone() {
    return new DivisorNode();
}