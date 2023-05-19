#include "Node.hpp"

class SigmoidNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SigmoidNode();
    virtual ~SigmoidNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string SigmoidNode::GetNodeType() { return "SigmoidNode"; }

SigmoidNode::SigmoidNode() {
    this->children = vector<INode*>();
}

SigmoidNode::~SigmoidNode(){}

vector<INode*>& SigmoidNode::GetChildren() {
    return this->children;
}

vector<float>& SigmoidNode::GetInputVector() {
    return this->input;
}

float SigmoidNode::GetOutput() {
    float x = 0;
    for (float val : this->input)  {
        x += val;
    }
    return 1/(1+exp(-x));
}

INode* SigmoidNode::Clone() {
    return new SigmoidNode();
}

const char* SigmoidNode::Deconstruct() {
    return "";
}

INode* SigmoidNode::Reconstruct(string) {
    return new SigmoidNode();
}

