#include "Node.hpp"

class InputNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    float value;

    InputNode();
    virtual ~InputNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

string InputNode::getNodeType() { return "InputNode"; }

InputNode::InputNode() {
    this->children = vector<INode*>();
    this->value = 0;
}

InputNode::~InputNode(){}

vector<INode*>& InputNode::getChildren() {
    return this->children;
}

vector<float>& InputNode::getInputVector() {
    return this->input;
}

float InputNode::getOutput() {
    return this->value;
}

INode* InputNode::clone() {
    return new InputNode();
}