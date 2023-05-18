#include "Node.hpp"

class SigmoidNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SigmoidNode();
    virtual ~SigmoidNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

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