#include "Node.hpp"

string SigmoidNode::getNodeType() { return "SigmoidNode"; }

SigmoidNode::SigmoidNode() {
    this->children = vector<INode*>();
}

SigmoidNode::~SigmoidNode(){}

vector<INode*>& SigmoidNode::getChildren() {
    return this->children;
}

vector<float>& SigmoidNode::getInputVector() {
    return this->input;
}

float SigmoidNode::getOutput() {
    float x = 0;
    for (float val : this->input)  {
        x += val;
    }
    return 1/(1+exp(-x));
}

INode* SigmoidNode::clone() {
    return new SigmoidNode();
}