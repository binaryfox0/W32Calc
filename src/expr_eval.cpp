/*
MIT License

Copyright (c) 2023 Duy Pham Duc

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "expr_eval.hpp"
#include <map>
#include <stack>

token token_parser::get_next_token()
{
    if(pos >= expr.length())
    return { token_type::end, 0.0 };
    skip_space();
    char c = expr[pos++];
    switch(c)
    {
    case '+': return { token_type::plus, 0.0 };
    case '-': return { token_type::minus, 0.0 };
    case '*': return { token_type::multiply, 0.0 };
    case '/': return { token_type::divide, 0.0 };
    case '(': return { token_type::lparen, 0.0 };
    case ')': return { token_type::rparen, 0.0 };
    default:
    {
    if(std::isdigit(c) || c == '.')
    {
    pos--;
    return get_number();
    } else {
    throw std::runtime_error("Invalid character: '"+std::string(1,c)+"'");
    }
    }
    }
}

void token_parser::skip_space()
{
    for(;pos < expr.length() && std::isspace(expr[pos]); pos++)
    {}
}

token token_parser::get_number()
{
    std::string num_str;
    bool dec_pnt = false;

    while(pos < expr.length() && (std::isdigit(expr[pos]) || expr[pos] == '.'))
    {
    if(expr[pos] == '.')
    {
    if(dec_pnt) // Already have a decimal point
    throw std::runtime_error("Invalid number format");
    dec_pnt = true;
    }
    num_str += expr[pos++];
    }
    return { token_type::number, std::stod(num_str) };
}

struct op_properties
{
    int precedence;
    bool left_assoc;
    bool unary;
    bool binary;
};
std::map<token_type, op_properties> op_info =
{
    {token_type::plus, {1, true, false, true}},
    {token_type::minus,{1, true, false, true}},
    {token_type::multiply, {2, true, false, true}},
    {token_type::divide, {2, true, false, true}},
    // 3 Reserved for power
    {token_type::unary_plus, {4, false, true, false}},
    {token_type::unary_minus, {4, false, true, false}},
};

std::vector<token> infix_to_postfix(std::string infix)
{
    std::vector<token> out;
    std::stack<token_type> operators;
    bool first = true;
    bool after_open_paren = false;
    token_parser tp(infix);
    token tk, ltk;
    while((tk = tp.get_next_token()).type != token_type::end)
    {
    switch(tk.type)
    {
    case token_type::number:
    {
    out.push_back(tk);
    break;
    }
    case token_type::lparen:
    {
    operators.push(tk.type);
    break;
    }
    case token_type::rparen:
    {
        while (!operators.empty() && operators.top() != token_type::lparen)
        {
            out.push_back({ operators.top(),0 });
            operators.pop();
        }
        if(operators.empty())
            throw std::runtime_error("Parenthesis mismatched, missing open parenthesis");
        operators.pop(); // Remove open parenthesis
        after_open_paren = true; // Avoid the confusion of the mechanism :))))
        break;
    }
    default: // Unary operator handlimg may have bug potential
    {
    if(op_info[tk.type].binary || op_info[tk.type].unary)
    {
    token_type cop = tk.type;
    if(first)
    {
        switch(cop)
        {
        case token_type::plus:
        {
        cop = token_type::unary_plus;
        break;
        }
        case token_type::minus:
        {
        cop = token_type::unary_minus;
        break;
        }
        default:
        throw std::runtime_error("Unknown unary operator");
        }
    }
    else if(operators.size() > 0)
    {
        if(!after_open_paren && ((op_info[operators.top()].binary || op_info[operators.top()].unary) || operators.top() == token_type::lparen) &&
        /*!is_un(operators.top()) &&*/ ltk.type != token_type::number)
        {
        switch(cop)
        {
        case token_type::plus:
        {
        cop = token_type::unary_plus;
        break;
        }
        case token_type::minus:
        {
        cop = token_type::unary_minus;
        break;
        }
        default:
        throw std::runtime_error("Unknown unary operator");
        }
        }
        while (!operators.empty() &&
            (op_info[operators.top()].binary || op_info[operators.top()].unary) &&
        ((op_info[cop].left_assoc && op_info[cop].precedence <= op_info[operators.top()].precedence) ||
        (!op_info[cop].left_assoc && op_info[cop].precedence < op_info[operators.top()].precedence))
        )
        {
            if (operators.empty())
                break;
        out.push_back({operators.top(),0});
        operators.pop();
        }
    }
    operators.push(cop);
    after_open_paren = false;
    }
    else
    throw std::runtime_error("Unhandle case. Internal error!");
    }
    }
    first = false;
    ltk = tk;
    }
    while(!operators.empty())
    {
    if(operators.top() == token_type::lparen)
    throw std::runtime_error("Parenthesis mismatched, missing close parenthesis");
    out.push_back({operators.top(),0});
    operators.pop();
    }
    return out;
}

double apply_op(double a, double b, token_type op)
{
    switch(op)
    {
    case token_type::plus: return a + b;
    case token_type::minus: return a-b;
    case token_type::multiply: return a*b;
    case token_type::divide: return a/b;
    default: return 0;
    }
    return 0.0;
}

// No bug detected
double evaluate(std::vector<token> tks)
{
    std::stack<double> operands;
    for(int i = 0; i < tks.size(); i++)
    {
    token tk = tks[i];
    switch(tk.type)
    {
    case token_type::number:
    {
        operands.push(tk.number);
        break;
    }
    case token_type::plus:
    case token_type::minus:
    case token_type::multiply:
    case token_type::divide:
    {
        if(operands.size() < 2)
        throw std::runtime_error("Operator imbalance");

        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();
        operands.push(apply_op(a,b,tk.type));
        break;
    }
    case token_type::unary_plus: break; // Nothing to do
    case token_type::unary_minus:
    {
        double apply = operands.top() * -1;
        operands.pop();
        operands.push(apply);
        break;
    }
    default:
        throw std::runtime_error("Internal error");
    }
    }
    return operands.top();
}