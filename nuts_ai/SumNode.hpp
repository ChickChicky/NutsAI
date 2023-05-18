#include "Node.hpp"

class SumNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SumNode();
    virtual ~SumNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

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