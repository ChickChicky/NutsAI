#include "Node.hpp"

class SubstractNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SubstractNode();
    virtual ~SubstractNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string SubstractNode::GetNodeType() { return "SubstractNode"; }

SubstractNode::SubstractNode() {
    this->children = vector<INode*>();
}

SubstractNode::~SubstractNode(){}

vector<INode*>& SubstractNode::GetChildren() {
    return this->children;
}

vector<float>& SubstractNode::GetInputVector() {
    return this->input;
}

float SubstractNode::GetOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum -= val;
    }
    return sum;
}

INode* SubstractNode::Clone() {
    return new SubstractNode();
}

const char* SubstractNode::Deconstruct() {
    return "";
}

INode* SubstractNode::Reconstruct(string) {
    return new SubstractNode();
}

