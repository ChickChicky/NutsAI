#include "Node.hpp"

string ProductNode::getNodeType() { return "ProductNode"; }

ProductNode::ProductNode() {
    this->children = vector<INode*>();
}

ProductNode::~ProductNode(){}

vector<INode*>& ProductNode::getChildren() {
    return this->children;
}

vector<float>& ProductNode::getInputVector() {
    return this->input;
}

float ProductNode::getOutput() {
    float product = 1;
    for (float val : this->input) {
        product *= val;
    }
    return product;
}

INode* ProductNode::clone() {
    return new ProductNode();
}