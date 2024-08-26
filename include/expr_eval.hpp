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

#ifndef MATH_EXPR_EVAL_HPP
#define MATH_EXPR_EVAL_HPP

#include <string>
#include <stdexcept>
#include <vector>

enum class token_type
{
 number,
 plus,
 minus,
 // Custom handling start //
 unary_plus,
 unary_minus,
 // Custom handling end //
 multiply,
 divide,
 lparen,
 rparen,
 end
};

struct token
{
 token_type type;
 double number;
};

class token_parser
{
public:
 token_parser(const std::string& input) :
  expr(input), pos(0) {}

 token get_next_token();

private:
 void skip_space();
 token get_number();

 std::string expr;
 uint64_t pos;
};

std::vector<token> infix_to_postfix(std::string infix);
double evaluate(std::vector<token> tks);

#endif
