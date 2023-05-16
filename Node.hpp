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

// Other Nodes

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

class SumNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SumNode();
    virtual ~SumNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class ProductNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ProductNode();
    virtual ~ProductNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class OutputNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    OutputNode();
    virtual ~OutputNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class SigmoidNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SigmoidNode();
    virtual ~SigmoidNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class ExponentNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ExponentNode();
    virtual ~ExponentNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class SubstractNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    SubstractNode();
    virtual ~SubstractNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class ReLuNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    ReLuNode();
    virtual ~ReLuNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};

class DivisorNode : public INode {

    private:

    vector<INode*> children;
    vector<float> input;

    public:

    DivisorNode();
    virtual ~DivisorNode();

    virtual vector<INode*>& getChildren();
    virtual vector<float>& getInputVector();
    virtual string getNodeType();
    virtual float getOutput();

    virtual INode* clone();

};
