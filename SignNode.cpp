#include "Node.hpp"

string SignNode::getNodeType() { return "SignNode"; }

SignNode::SignNode() {
    this->children = vector<INode*>();
}

SignNode::~SignNode(){}

vector<INode*>& SignNode::getChildren() {
    return this->children;
}

vector<float>& SignNode::getInputVector() {
    return this->input;
}

float SignNode::getOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum += val;
    }
    return sum<0?-1:sum>0?1:0;
}

INode* SignNode::clone() {
    return new SignNode();
}