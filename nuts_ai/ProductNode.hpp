#include "Node.hpp"

class ProductNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ProductNode();
    virtual ~ProductNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

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