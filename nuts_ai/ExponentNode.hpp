#include "Node.hpp"

class ExponentNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ExponentNode();
    virtual ~ExponentNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

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