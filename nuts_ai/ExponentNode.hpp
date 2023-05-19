#include "Node.hpp"

class ExponentNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ExponentNode();
    virtual ~ExponentNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string ExponentNode::GetNodeType() { return "ExponentNode"; }

ExponentNode::ExponentNode() {
    this->children = vector<INode*>();
}

ExponentNode::~ExponentNode(){}

vector<INode*>& ExponentNode::GetChildren() {
    return this->children;
}

vector<float>& ExponentNode::GetInputVector() {
    return this->input;
}

float ExponentNode::GetOutput() {
    float x = 0;
    for (float val : this->input)  {
        if (x == 0) {
            x = val;
        } else {
            x = pow(x,val);
        }
    }
    return x;
}

INode* ExponentNode::Clone() {
    return new ExponentNode();
}

const char* ExponentNode::Deconstruct() {
    return "";
}

INode* ExponentNode::Reconstruct(string) {
    return new ExponentNode();
}

