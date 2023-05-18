#include "Node.hpp"

class ReLuNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ReLuNode();
    virtual ~ReLuNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

string ReLuNode::getNodeType() { return "ReLuNode"; }

ReLuNode::ReLuNode() {
    this->children = vector<INode*>();
}

ReLuNode::~ReLuNode(){}

vector<INode*>& ReLuNode::getChildren() {
    return this->children;
}

vector<float>& ReLuNode::getInputVector() {
    return this->input;
}

float ReLuNode::getOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum += val;
    }
    return sum>=0?sum:0;
}

INode* ReLuNode::clone() {
    return new ReLuNode();
}