#include "Node.hpp"

class InputNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    float value;

    InputNode();
    virtual ~InputNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();

    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string InputNode::GetNodeType() { return "InputNode"; }

InputNode::InputNode() {
    this->children = vector<INode*>();
    this->value = 0;
}

InputNode::~InputNode(){}

vector<INode*>& InputNode::GetChildren() {
    return this->children;
}

vector<float>& InputNode::GetInputVector() {
    return this->input;
}

float InputNode::GetOutput() {
    return this->value;
}

INode* InputNode::Clone() {
    return new InputNode();
}

const char* InputNode::Deconstruct() {
    return "";
}

INode* InputNode::Reconstruct(string) {
    return new InputNode();
}

