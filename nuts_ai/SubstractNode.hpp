#include "Node.hpp"

class SubstractNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SubstractNode();
    virtual ~SubstractNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

string SubstractNode::getNodeType() { return "SubstractNode"; }

SubstractNode::SubstractNode() {
    this->children = vector<INode*>();
}

SubstractNode::~SubstractNode(){}

vector<INode*>& SubstractNode::getChildren() {
    return this->children;
}

vector<float>& SubstractNode::getInputVector() {
    return this->input;
}

float SubstractNode::getOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum -= val;
    }
    return sum;
}

INode* SubstractNode::clone() {
    return new SubstractNode();
}