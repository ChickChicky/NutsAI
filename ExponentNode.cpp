#include "Node.hpp"

string ExponentNode::getNodeType() { return "ExponentNode"; }

ExponentNode::ExponentNode() {
    this->children = vector<INode*>();
}

ExponentNode::~ExponentNode(){}

vector<INode*>& ExponentNode::getChildren() {
    return this->children;
}

vector<float>& ExponentNode::getInputVector() {
    return this->input;
}

float ExponentNode::getOutput() {
    float x = 0;
    for (float val : this->input)  {
        if (x == 0) {
            x = val;
        } else {
            x = pow(x,val);
        }
    }
    return x;
}

INode* ExponentNode::clone() {
    return new ExponentNode();
}