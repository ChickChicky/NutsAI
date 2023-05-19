#include "Node.hpp"

class ReLuNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ReLuNode();
    virtual ~ReLuNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string ReLuNode::GetNodeType() { return "ReLuNode"; }

ReLuNode::ReLuNode() {
    this->children = vector<INode*>();
}

ReLuNode::~ReLuNode(){}

vector<INode*>& ReLuNode::GetChildren() {
    return this->children;
}

vector<float>& ReLuNode::GetInputVector() {
    return this->input;
}

float ReLuNode::GetOutput() {
    float sum = 0;
    for (float val : this->input)  {
        sum += val;
    }
    return sum>=0?sum:0;
}

INode* ReLuNode::Clone() {
    return new ReLuNode();
}

const char* ReLuNode::Deconstruct() {
    return "";
}

INode* ReLuNode::Reconstruct(string) {
    return new ReLuNode();
}

