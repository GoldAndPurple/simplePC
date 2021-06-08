#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <locale> //for function isalnum()
using namespace std;

typedef struct asmline{
    int num;
    string com;
    int op;
} asmline;

int preced(char ch)
{
    if (ch == '+' || ch == '-')
    {
        return 1; //Precedence of + or - is 1
    }
    else if (ch == '*' || ch == '/')
    {
        return 2; //Precedence of * or / is 2
    }
    else
    {
        return 0;
    }
}

string inToPost(string infix)
{
    stack<char> stk;
    stk.push('#');       //add some extra character to avoid underflow
    string postfix = ""; //initially the postfix string is empty
    string::iterator it;

    for (it = infix.begin(); it != infix.end(); it++)
    {
        if (isalnum(char(*it)))
            postfix += *it; //add to postfix when character is letter or number
        else if (*it == '(')
            stk.push('(');
        else if (*it == '^')
            stk.push('^');
        else if (*it == ')')
        {
            while (stk.top() != '#' && stk.top() != '(')
            {
                postfix += stk.top(); //store and pop until ( has found
                stk.pop();
            }
            stk.pop(); //remove the '(' from stack
        }
        else
        {
            if (preced(*it) > preced(stk.top()))
                stk.push(*it); //push if precedence is high
            else
            {
                while (stk.top() != '#' && preced(*it) <= preced(stk.top()))
                {
                    postfix += stk.top(); //store and pop until higher precedence is found
                    stk.pop();
                }
                stk.push(*it);
            }
        }
    }

    while (stk.top() != '#')
    {
        postfix += stk.top(); //store and pop until stack is not empty.
        stk.pop();
    }

    return postfix;
}

/* trees */

// Data structure to store a binary tree node
struct Node
{
    char data;
    int loc;
    Node *left, *right;

    Node(int data)
    {
        this->data = data;
        this->left = this->right = nullptr;
    };

    Node(int data, Node *left, Node *right)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    };
};

// Function to check if a given token is an operator
bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Print the postfix expression for an expression tree
void postorder(Node *root)
{
    if (root == nullptr)
    {
        return;
    }

    postorder(root->left);
    postorder(root->right);
    cout << root->data;
}

int avilable_mem(int& a){
    int t = a;
    for (int i = 0; i < 32; i++){
        if (t & 1 == 0){
            a |= (1 << i);
            return 72-i; //72 and below for constants
        }
        t >> 1;
    }
    return -1;
}

void inorder(Node *root, int& line, int& mem, string& as)
{
    if (root == nullptr)
    {
        return;
    }
    inorder(root->left,line,mem,as);
    inorder(root->right,line,mem,as);

    if (isdigit(root->data)){
        root->loc = avilable_mem(mem);
        return;
    } else if (isalpha(root->data)){
        root->loc = 164-(int)root->data;
        return;
    } else if (isOperator(root->data)){
        as.append(to_string(line));
        if (isdigit(root->left->data)){
            as.append(" SET ");
            as.append(to_string(root->left->data));
        } else {
            as.append(" LOAD ");
            as.append(to_string(164-(int)root->left->data));
        };
        as.append("\n");
        line++;

        as.append(to_string(line));
        if (root->data == '+'){
            as.append(" ADD ");
        } else if (root->data == '-'){
            as.append(" SUB ");
        } else if (root->data == '*'){
            as.append(" MUL ");
        }  else if (root->data == '/'){
            as.append(" DIVIDE ");
        }
        as.append(to_string(root->right->loc));
        as.append("\n");
        line++;
        cout << as;
    }
}

// Function to construct an expression tree from the given postfix expression
Node *construct(string postfix)
{
    // create an empty stack to store tree pointers
    stack<Node *> s;

    // traverse the postfix expression
    for (char c : postfix)
    {
        // if the current token is an operator
        if (isOperator(c))
        {
            // pop two nodes `x` and `y` from the stack
            Node *x = s.top();
            s.pop();

            Node *y = s.top();
            s.pop();

            // construct a new binary tree whose root is the operator and whose
            // left and right children point to `y` and `x`, respectively
            Node *node = new Node(c, y, x);

            // push the current node into the stack
            s.push(node);
        }

        // if the current token is an operand, create a new binary tree node
        // whose root is the operand and push it into the stack
        else
        {
            s.push(new Node(c));
        }
    }

    // a pointer to the root of the expression tree remains on the stack
    return s.top();
}

void parse_expr(string e, int& current_line){
    string postfix = inToPost(e);
    Node* root = construct(postfix);
    int a;
    string ass;
    inorder(root,current_line,a,ass);
    cout << ass;
    return;
}


int main()
{
/*
    string infix = "(a+b)*4";
    string postfix = inToPost(infix);
    cout << "Postfix Form Is: " << postfix << endl;
    Node* root = construct(postfix);
    inorder(root);
*/
    ifstream f("fact.bas");
    ofstream of("fact.sa");
    string token, line;
    vector<string> lines;
    while (getline(f, line, '\n'))
    {
        lines.push_back(line);
        /*
        stringstream ss(line);
        while (getline(ss, token, ' '))
        {
            part.push_back(token);
        }

        for (int j = 0; j < syn.size(); j++){
            cout << syn[j] << ' ';
        }
        */
    }

    vector<string> syn;
    int line_basic = 0;
    asmline as;
    for (int i = 0; i < lines.size(); i++){
        //cout << lines[i] << "\n";
        syn.clear();
        stringstream ss(lines[i]);
        while (getline(ss, token, ' '))
        {
            syn.push_back(token);
        }
        as.num = stoi(syn[0])/10;
        if (!syn[1].compare("REM")){
            continue;
        } else if (!syn[1].compare("INPUT")){
            as.com = "READ";
            as.op = 164-syn[2][0];

        } else if (!syn[1].compare("OUTPUT")){
            as.com = "WRITE";
            as.op = 164-syn[2][0];

        } else if (!syn[1].compare("GOTO")){
            as.com = "JMP";
            as.op = stoi(syn[2])/10;

        } else if (!syn[1].compare("IF")){

        } else if (!syn[1].compare("LET")){

        } else if (!syn[1].compare("END")){
            as.com = "HALT";
            as.op = 0;

        } else {
            return 1;
        }
        of << as.num << " " << as.com << " " << as.op << "\n";
    }
    of.close();
    return 0;
}