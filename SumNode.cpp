#include "Node.hpp"

string SumNode::getNodeType() { return "SumNode"; }

SumNode::SumNode() {
    this->children = vector<INode*>();
}

SumNode::~SumNode(){}

vector<INode*>& SumNode::getChildren() {
    return this->children;
}

vector<float>& SumNode::getInputVector() {
    return this->input;
}

float SumNode::getOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum += val;
    }
    return sum;
}

INode* SumNode::clone() {
    return new SumNode();
}