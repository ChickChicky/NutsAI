#pragma once

#include <vector>
#include <string>
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
    virtual vector<INode*>& getChildren();
    // Returns a reference to a vector that can be modified to change the input values of the node
    virtual vector<float>& getInputVector();
    // Returns a string representing the type of the node
    virtual string getNodeType();
    // Returns the output value of the node
    virtual float getOutput();

    // Returns a pointer to a new node containing the same data as the original one
    virtual INode* clone();

};

/* Dummy INode virtual function declarations */
/* Returns a string containing the node's type */
string INode::getNodeType() { return "DummyNode"; }
INode::INode() { this->children = vector<INode*>(); }
INode::~INode() {}
/* Returns a reference to a vector that can be manipulated to change a node's childrens */
vector<INode*>& INode::getChildren() { return this->children; }
/* Returns a reference to the vctor used to set the node's inputs */
vector<float>& INode::getInputVector() { return this->input; }
/* Computes the node's output */
float INode::getOutput() { float sum; for (float val : this->input) sum += val; return sum; }
/* Returns a pointer to a new node of the same type, allows to keep some properties of the original node if needed */
INode* INode::clone() { return new INode(); }

// Other Nodes

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