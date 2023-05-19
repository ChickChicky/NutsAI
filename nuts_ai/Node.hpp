#pragma once

#include <vector>
#include <string>
#include <string.h>
#include <cmath>

using namespace std;

/* Node interface class */
class INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    INode();
    virtual ~INode();

    // Returns a reference to a vector containing the children of this node
    virtual vector<INode*>& GetChildren();
    // Returns a reference to a vector that can be modified to change the input values of the node
    virtual vector<float>& GetInputVector();
    // Returns a string representing the type of the node
    virtual string GetNodeType();
    // Returns the output value of the node
    virtual float GetOutput();

    // Returns a pointer to a new node containing the same data as the original one
    virtual INode* Clone();

    // Returns a string containing the data required to rebuild the node using INode::Reconstruct
    virtual const char* Deconstruct();
    // Creates a new instance of the node based on its deconstructed data
    static INode* Reconstruct(string, string);

};

/* Other Nodes */

#include "InputNode.hpp"
#include "SumNode.hpp"
#include "ProductNode.hpp"
#include "OutputNode.hpp"
#include "SigmoidNode.hpp"
#include "ExponentNode.hpp"
#include "SubstractNode.hpp"
#include "ReLuNode.hpp"
#include "DivisorNode.hpp"
#include "SignNode.hpp"

/* Dummy INode virtual function declarations */

string INode::GetNodeType() { return "DummyNode"; }
INode::INode() { this->children = vector<INode*>(); }
INode::~INode() {}
vector<INode*>& INode::GetChildren() { return this->children; }
vector<float>& INode::GetInputVector() { return this->input; }
float INode::GetOutput() { float sum; for (float val : this->input) sum += val; return sum; }
INode* INode::Clone() { return new INode(); }
const char* INode::Deconstruct() { return ""; }

INode* INode::Reconstruct(string type, string dat) { 
    if (type == "InputNode") return InputNode::Reconstruct(dat);
    if (type == "OutputNode") return OutputNode::Reconstruct(dat);
    if (type == "SumNode") return SumNode::Reconstruct(dat);
    if (type == "SubstractNode") return SubstractNode::Reconstruct(dat);
    if (type == "ProductNode") return ProductNode::Reconstruct(dat);
    if (type == "DivisorNode") return DivisorNode::Reconstruct(dat);
    if (type == "SignNode") return SignNode::Reconstruct(dat);
    if (type == "ExponentNode") return ExponentNode::Reconstruct(dat);
    if (type == "SigmoidNode") return SigmoidNode::Reconstruct(dat);
    if (type == "ReLuNode") return ReLuNode::Reconstruct(dat);
    return new INode();
}