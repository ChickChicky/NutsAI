#include "Node.hpp"

class SignNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SignNode();
    virtual ~SignNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string SignNode::GetNodeType() { return "SignNode"; }

SignNode::SignNode() {
    this->children = vector<INode*>();
}

SignNode::~SignNode(){}

vector<INode*>& SignNode::GetChildren() {
    return this->children;
}

vector<float>& SignNode::GetInputVector() {
    return this->input;
}

float SignNode::GetOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum += val;
    }
    return sum<0?-1:sum>0?1:0;
}

INode* SignNode::Clone() {
    return new SignNode();
}

const char* SignNode::Deconstruct() {
    return "";
}

INode* SignNode::Reconstruct(string) {
    return new SignNode();
}

