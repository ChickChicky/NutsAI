#include "Node.hpp"

class OutputNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    OutputNode();
    virtual ~OutputNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string OutputNode::GetNodeType() { return "OutputNode"; }

OutputNode::OutputNode() {
    this->children = vector<INode*>();
}

OutputNode::~OutputNode(){}

vector<INode*>& OutputNode::GetChildren() {
    return this->children;
}

vector<float>& OutputNode::GetInputVector() {
    return this->input;
}

float OutputNode::GetOutput() {
    float sum;
    for (float val : this->input) {
        sum = val;
    }
    return sum;
}

INode* OutputNode::Clone() {
    return new OutputNode();
}

const char* OutputNode::Deconstruct() {
    return "";
}

INode* OutputNode::Reconstruct(string) {
    return new OutputNode();
}

