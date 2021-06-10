#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

vector<int> basic_line;
vector<int> asm_line;

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
	stk.push('#');		 //add some extra character to avoid underflow
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

int avilable_mem(int &a)
{
	int t = a;
	for (int i = 0; i < 32; i++)
	{
		if (!((t >> i) & 1))
		{
			a |= (1 << i);
			return i; //72 and below for temp constants
		}
	}
	return 72;
}

// CONST sets 99 to operand
void expr_traversal(Node *root, int &line, int &mem, string &as)
{
	if (root == nullptr)
	{
		return;
	}
	expr_traversal(root->left, line, mem, as);
	expr_traversal(root->right, line, mem, as);

	if (isdigit(root->data))
	{
		root->loc = 99;
		return;
	}
	else if (isalpha(root->data))
	{
		root->loc = 163 - (int)root->data;
		return;
	}
	else if (isOperator(root->data))
	{
		if (isdigit(root->left->data))
		{
			as.append(to_string(line));
			as.append(" CONST ");
			as.append(to_string(root->left->data - 48));
			as.append("\n");
			line++;
		}

		as.append(to_string(line));
		as.append(" LOAD ");
		as.append(to_string(root->left->loc));
		as.append("\n");
		line++;

		if (isdigit(root->right->data))
		{
			as.append(to_string(line));
			as.append(" CONST ");
			as.append(to_string(root->right->data - 48));
			as.append("\n");
			line++;
		}

		as.append(to_string(line));
		if (root->data == '+')
		{
			as.append(" ADD ");
		}
		else if (root->data == '-')
		{
			as.append(" SUB ");
		}
		else if (root->data == '*')
		{
			as.append(" MUL ");
		}
		else if (root->data == '/')
		{
			as.append(" DIVIDE ");
		}
		as.append(to_string(root->right->loc));
		as.append("\n");
		line++;
		root->loc = 72 - avilable_mem(mem);
		as.append(to_string(line));
		as.append(" STORE ");
		as.append(to_string(root->loc));
		as.append("\n");
		line++;
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

//result of expression in accumulator
string parse_expr(string e, int &current_line)
{
	string postfix = inToPost(e);
	Node *root = construct(postfix);
	int a = 0;
	string ass;
	expr_traversal(root, current_line, a, ass);
	if (root->loc == 99)
	{
		ass.append(to_string(current_line++) + " CONST " + to_string(root->data - 48) + "\n");
	}
	ass.append(to_string(current_line) + " LOAD " + to_string(root->loc) + "\n");

	int count = 1;
	for (int i = 0; i < ass.length(); i++)
	{
		if (ass[i] == '\n')
			count++;
	}
	asm_line.push_back(count);
	return ass;
}

void find_and_replace(string &file_contents,
					  const string &from, const string &to)
{
	size_t pos = file_contents.find(from);
	while (pos != string::npos)
	{
		file_contents.replace(pos, from.length(), to);
		pos = file_contents.find(from, pos);
	}
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s input.bas output.sa\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	ifstream f(argv[1]);
	stringstream of;
	string token, line;
	vector<string> lines;
	while (getline(f, line, '\n'))
	{
		lines.push_back(line);
	}

	vector<string> syn;
	int current_line = 0;
	string asm_com;
	int asm_op;
	int asm_num;
	for (int i = 0; i < lines.size(); i++)
	{
		//cout << lines[i] << "\n";
		syn.clear();
		stringstream ss(lines[i]);
		while (getline(ss, token, ' '))
		{
			syn.push_back(token);
		}
		basic_line.push_back(stoi(syn[0]));
		if (!syn[1].compare("REM"))
		{
			asm_line.push_back(1);
			continue;
		}
		else if (!syn[1].compare("INPUT"))
		{
			asm_com = "READ";
			asm_op = 163 - syn[2][0];
			asm_line.push_back(1);
		}
		else if (!syn[1].compare("OUTPUT"))
		{
			asm_com = "WRITE";
			asm_op = 163 - syn[2][0];
			asm_line.push_back(1);
		}
		else if (!syn[1].compare("GOTO"))
		{
			asm_com = "JMPX";
			asm_op = stoi(syn[2]);
			asm_line.push_back(1);
		}
		else if (!syn[1].compare("IF")) //jump if condition is false - skip following THEN statement
		{
			if (!syn[3].compare("="))
			{
				of << parse_expr(syn[2] + '-' + syn[4], current_line);
				current_line++;
				asm_com = "JNZ";
			}
			else if (!syn[3].compare("<"))
			{
				of << parse_expr(syn[4] + '-' + syn[2], current_line);
				current_line++;
				asm_com = "JNEG";
			}
			else if (!syn[3].compare(">"))
			{
				of << parse_expr(syn[2] + '-' + syn[4], current_line);
				current_line++;
				asm_com = "JNEG";
			}
			else
			{
				return 1;
			}
			asm_num = current_line++;
			asm_op = current_line + 1; //unless LET

			string then_statement(to_string(current_line));
			//repeat of other statements
			if (!syn[5].compare("REM"))
			{
				continue;
			}
			else if (!syn[5].compare("INPUT"))
			{
				then_statement.append(" READ " + to_string(163 - syn[6][0]));
			}
			else if (!syn[5].compare("OUTPUT"))
			{
				then_statement.append(" WRITE " + to_string(163 - syn[6][0]));
			}
			else if (!syn[5].compare("GOTO"))
			{
				then_statement.append(" JMPX " + to_string(stoi(syn[6])));
			}
			else if (!syn[5].compare("LET"))
			{
				current_line++;
				then_statement = parse_expr(syn[8], current_line);

				current_line++;
				then_statement.append(to_string(current_line) + " STORE " + to_string((163 - syn[6][0])));
				asm_num = current_line + asm_line.back() + 2;
				continue;
			}
			else if (!syn[5].compare("END"))
			{
				then_statement.append(" HALT");
			}
			else
			{
				return 1;
			}
			of << asm_num << " " << asm_com << " " << asm_op << "\n";
			of << then_statement << "\n";

			current_line++;
			asm_line.back() += 1;
			continue;
			//repeat end
		}
		else if (!syn[1].compare("LET"))
		{
			of << parse_expr(syn[4], current_line);
			current_line++;
			asm_com = "STORE";
			asm_op = (163 - syn[2][0]);
		}
		else if (!syn[1].compare("END"))
		{
			asm_line.push_back(1);
			of << current_line << " HALT\n";
			break;
		}
		else
		{
			return 1;
		}
		asm_num = current_line;
		of << asm_num << " " << asm_com << " " << asm_op << "\n";

		current_line++;
	}

	//goto fixup

	for (int i = 1; i < basic_line.size(); i++)
	{

		asm_line[i] += asm_line[i - 1];
	}
	asm_line.insert(asm_line.begin(), 0);

	for (int i = 0; i < basic_line.size(); i++)
	{

		cout << basic_line[i] << " " << asm_line[i] << "\n";
	}

	f.close();
	of.clear();
	of.seekg(0, of.beg);

	ofstream ans(argv[2]);

	lines.clear();
	while (getline(of, line, '\n'))
	{
		for (int i = 0; i < basic_line.size(); i++)
		{
			find_and_replace(line, "JMPX " + to_string(basic_line[i]), "JUMP " + to_string(asm_line[i]));
		}
		ans << line << "\n";
	}

	ans.close();
	return 0;
}