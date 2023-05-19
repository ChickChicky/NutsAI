#include "Node.hpp"

class ProductNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ProductNode();
    virtual ~ProductNode();

    virtual vector<INode*>& GetChildren();
    virtual vector<float>& GetInputVector();
    virtual string GetNodeType();
    virtual float GetOutput();

    virtual INode* Clone();
    
    virtual const char* Deconstruct();

    static INode* Reconstruct(string);

};

string ProductNode::GetNodeType() { return "ProductNode"; }

ProductNode::ProductNode() {
    this->children = vector<INode*>();
}

ProductNode::~ProductNode(){}

vector<INode*>& ProductNode::GetChildren() {
    return this->children;
}

vector<float>& ProductNode::GetInputVector() {
    return this->input;
}

float ProductNode::GetOutput() {
    float product = 1;
    for (float val : this->input) {
        product *= val;
    }
    return product;
}

INode* ProductNode::Clone() {
    return new ProductNode();
}

const char* ProductNode::Deconstruct() {
    return "";
}

INode* ProductNode::Reconstruct(string) {
    
    return new ProductNode();
}

