#include "Node.hpp"

class SumNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SumNode();
    virtual ~SumNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string SumNode::GetNodeType() { return "SumNode"; }

SumNode::SumNode() {
    this->children = vector<INode*>();
}

SumNode::~SumNode(){}

vector<INode*>& SumNode::GetChildren() {
    return this->children;
}

vector<float>& SumNode::GetInputVector() {
    return this->input;
}

float SumNode::GetOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum += val;
    }
    return sum;
}

INode* SumNode::Clone() {
    return new SumNode();
}

const char* SumNode::Deconstruct() {
    return "";
}

INode* SumNode::Reconstruct(string) {
    return new SumNode();
}

