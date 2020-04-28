#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result);
// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.
int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = { 3,  -9,   6,   2,   4,   1 };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0 &&
        pf == "ae+" && answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("y(*o)", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2 &&
        pf == "ab*" && answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
        pf == "yoau-*+" && answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
        pf == "oyy-/" && answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0 &&
        pf == "a" && answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0 &&
        pf == "a" && answer == 3);
    cout << "Passed all tests" << endl;
}

int evaluate(string infix, const Map& values, string& postfix, int& result) {
    int i = 0;
    if (infix.size() == 0) {
        return 1; //no infix
    }
    while(i != infix.size()) { //removes spaces from infix
        if (infix[i] == ' ') {
            infix.erase(infix.begin() + i);
        }
        else {
            i++;
        }
    }
    postfix = "";
    stack <char> operatorS;
    for (i = 0; i < infix.size(); i++) { //changes infix to postfix if possible, returns 1 if invalid syntax
        if (isalpha(infix[i])) { //if operand (lower letter)
            if ((isalpha(infix[i]) && isalpha(infix[i + 1])) || isupper(infix[i])) {
                return 1; //two consecutive operands or uppercase operand, invalid syntax
            }
            postfix += infix[i];
            continue; //skips other statements
        }
        switch (infix[i]) { //cases for returning 1
        case '(':
            if ((!isalpha(infix[i + 1]) && infix[i+1] != '(' ) || (i - 1 >= 0 && isalpha(infix[i-1]))) {
                return 1; //operator after (, invalid syntax
            }
            break;
        case ')':
            if (isalpha(infix[i + 1]) || infix[i+1] == '(') {
                return 1; //operand after ')' or '(', invalid syntax
            }
            break;
        case '/':
        case '*':
        case '+':
        case '-':
            if ((i == infix.size() - 1) || i == 0 || infix[i + 1] == '*' || infix[i + 1] == '/' || infix[i + 1] == '+' || infix[i + 1] == '-') {
                return 1; //ends or begins with operator or double operator
            }
        default:
            break;
        }
        switch (infix[i]) { //infix to postfix
        case '(':
            operatorS.push(infix[i]);
            break;
        case ')':
            // pop stack until matching '('
            while (!operatorS.empty() && operatorS.top() != '(') {
                postfix += operatorS.top();
                operatorS.pop();
            }
            if (operatorS.top() == '(') {
                operatorS.pop();
            }
            else {
                return 1; //empty stack with no '('
            }
            break;
        case '/':
        case '*':
        case '+':
        case '-':
            while (!operatorS.empty() && operatorS.top() != '(') {
                if ((infix[i] == '*' || infix[i] == '/') && (operatorS.top() == '/' || operatorS.top() == '*')) {
                    postfix += operatorS.top();
                    operatorS.pop();
                }
                else if (infix[i] == '-' || infix[i] == '+') {
                    postfix += operatorS.top();
                    operatorS.pop();
                }
                else {
                    break;
                }
            }
            operatorS.push(infix[i]);
            break;
        default:
            continue; //should never reach
        }
    }
    while (!operatorS.empty()) {
        if (operatorS.top() == '(') {
            return 1; //parantheses are not closed, invalid
        }
        postfix += operatorS.top();
        operatorS.pop();
    }

    //   If infix is syntactically valid but contains at least one lower
    //   case letter operand that does not appear in the values map, then
    //   result is unchanged and the function returns 2.  

    for (i = 0; i < infix.size(); i++) { //checks if there's an operand that doesn't exist in Map, return 2
        if (!values.contains(infix[i]) && isalpha(infix[i])) {
            return 2; //not in Map values
        }
    }

    //   If infix is syntactically valid and all its lower case operand letters
    //   appear in the values map, then if evaluating the expression
    //   (using for each letter in the expression the value in the map
    //   that corresponds to it) attempts to divide by zero, then result
    //   is unchanged and the function returns 3; otherwise, result is
    //   set to the value of the expression and the function returns 0.

    stack <int> eval;
    int currentV = 0;
    for (i = 0; i < postfix.size(); i++) { //evaluates postfix expression (assumes syntactically correct)
        if (values.get(postfix[i], currentV)) { //operand
            eval.push(currentV);
        }
        else { //operator
            int v2 = eval.top();
            eval.pop();
            int v1 = eval.top();
            eval.pop();
            switch (postfix[i]) {
            case '/': 
                if (v2 != 0) {
                eval.push(v1 / v2);
                break;
                } else {
                return 3; //divide by 0, returns 3
            }
            case '*': eval.push(v1 * v2);
                break;
            case '+': eval.push(v1 + v2);
                break;
            case '-': eval.push(v1 - v2);
                break;
            default: break; //should never reach, skips if does
            }
        }
    }
    result = eval.top(); //does not divide by 0, changes result
    return 0;
}