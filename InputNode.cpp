#include "Node.hpp"

string InputNode::getNodeType() { return "InputNode"; }

InputNode::InputNode() {
    this->children = vector<INode*>();
    this->value = 0;
}

InputNode::~InputNode(){}

vector<INode*>& InputNode::getChildren() {
    return this->children;
}

vector<float>& InputNode::getInputVector() {
    return this->input;
}

float InputNode::getOutput() {
    return this->value;
}

INode* InputNode::clone() {
    return new InputNode();
}