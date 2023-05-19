#include "Node.hpp"

class DivisorNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    DivisorNode();
    virtual ~DivisorNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string DivisorNode::GetNodeType() { return "DivisorNode"; }

DivisorNode::DivisorNode() {
    this->children = vector<INode*>();
}

DivisorNode::~DivisorNode(){}

vector<INode*>& DivisorNode::GetChildren() {
    return this->children;
}

vector<float>& DivisorNode::GetInputVector() {
    return this->input;
}

float DivisorNode::GetOutput() {
    float x = 0;
    for (float val : this->input)  {
        if (x == 0) {
            x = val;
        } else {
            x /= val!=0 ? val:1;
        }
    }
    return x;
}

INode* DivisorNode::Clone() {
    return new DivisorNode();
}

const char* DivisorNode::Deconstruct() {

    return "";
}

INode* DivisorNode::Reconstruct(string) {
    return new DivisorNode();
}
