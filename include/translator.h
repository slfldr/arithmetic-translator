#pragma once

#include "stack.h"

enum types { number, operation, open_bracket, close_bracket };

class Term
{
protected:
	types type;

	std::map<char, size_t> pritorities{ {'*', 2}, {'/', 2}, {'+', 1}, {'-', 1} };

public:
	types get_type() const noexcept
	{
		return type;
	}
};

class Number : public Term
{
private:
	double value;

public:
	Number(double _value) : value(_value)
	{
		type = number;
	}

	double get_value() const noexcept
	{
		return value;
	}
};

class Operation : public Term
{
private:
	char op;

	size_t priority;

public:
	Operation(char _op) : op(_op)
	{
		priority = pritorities[op];
		type = operation;
	}

	char get_op() const noexcept
	{
		return op;
	}

	size_t get_priority() const noexcept
	{
		return priority;
	}
};

class OpenBracket : public Term
{
private:
	char bracket;

public:
	OpenBracket() : bracket('(')
	{
		type = open_bracket;
	}

	char get_op() const noexcept
	{
		return bracket;
	}
};

class CloseBracket : public Term
{
private:
	char bracket;

public:
	CloseBracket() : bracket(')')
	{
		type = close_bracket;
	}

	char get_op() const noexcept
	{
		return bracket;
	}
};

class Translator
{
private:
    sqVector<Term*> terms;
    sqVector<Term*> polish_notation;

    std::string expression;

public:
    Translator(std::string _expression = "2+2*2") : expression(_expression) { }

    ~Translator()
    {
        clear_terms();
    }

private:
    void clear_terms()
    {
        for (size_t i = 0; i < terms.size_(); i++)
        {
            delete terms[i];
        }
    }

public:
    void tokenizer()
    {
        std::string N;

        bool in_number = false;

        for (size_t i = 0; i < expression.size(); i++)
        {
            char ch = expression[i];

            if (in_number)
            {
                if ((ch >= '0' && ch <= '9') || ch == '.')
                {
                    N += ch;
                }
                else
                {
                    terms.push_back(new Number(std::stod(N)));

                    N.clear();

                    in_number = false;

                    i--;
                }
            }
            else
            {
                if (ch == '(')
                {
                    terms.push_back(new OpenBracket);
                }
                else if (ch == '*' || ch == '/' || ch == '+' || ch == '-')
                {
                    terms.push_back(new Operation(ch));
                }
                else if (ch == ')')
                {
                    terms.push_back(new CloseBracket);
                }
                else if ((ch >= '0' && ch <= '9') || ch == '.')
                {
                    N += ch;

                    in_number = true;
                }
                else
                {
                    throw std::logic_error("Invalid syntax");
                }
            }
        }

        if (!N.empty())
        {
            terms.push_back(new Number(std::stod(N)));
        }
    }

    size_t get_terms_size() const noexcept
    {
        return terms.size_();
    }

    size_t get_expression_size() const noexcept
    {
        return expression.size();
    }

    std::string get_expression() const noexcept
    {
        return expression;
    }

    void print_expression()
    {
        for (const char& ch : expression)
        {
            std::cout << ch;
        }

        std::cout << std::endl;
    }

    void parser()
    {
        std::map<types, size_t> states = { {number, 0}, {operation, 1}, {open_bracket, 2}, {close_bracket, 3} };

        size_t state;

        mStack<char> stack;

        size_t sz = terms.size_();

        state = states[terms[0] -> get_type()];

        if (state == 1)
        {
            throw std::logic_error("Operation can't be the first symbol");
        }
        if (state == 2)
        {
            stack.push('(');
        }
        if (state == 3)
        {
            throw std::logic_error("')' can't be the first symbol");
        }

        for (size_t i = 1; i < sz; i++)
        {
            if (terms[i] -> get_type() == open_bracket)
            {
                stack.push('(');
            }
            else if (terms[i] -> get_type() == close_bracket)
            {
                if (!stack.isEmpty())
                {
                    stack.pop();
                }
                else
                {
                    throw std::logic_error("Incorrect brackets");
                }
            }

            switch (state)
            {
            case 0:
                if (terms[i] -> get_type() == open_bracket || terms[i] -> get_type() == number)
                {
                    throw std::logic_error("Invalid syntax");
                }

                state = states[terms[i] -> get_type()];

                break;

            case 1:
                if (terms[i] -> get_type() == operation || terms[i]->get_type() == close_bracket)
                {
                    throw std::logic_error("Invalid syntax");
                }

                state = states[terms[i] -> get_type()];

                break;

            case 2:
                if (terms[i]->get_type() == close_bracket || terms[i]->get_type() == operation)
                {
                    throw std::logic_error("Invalid syntax");
                }

                state = states[terms[i] -> get_type()];

                break;

            case 3:
                if (terms[i]->get_type() == number || terms[i]->get_type() == open_bracket)
                {
                    throw std::logic_error("Invalid syntax");
                }

                state = states[terms[i] -> get_type()];

                break;
            }
        }

        state = states[terms[sz - 1] -> get_type()];

        if (state == 1 || state == 2)
        {
            throw std::logic_error("Invalid syntax");
        }

        if (!stack.isEmpty())
        {
            throw std::logic_error("Incorrect brackets");
        }
    }

    void converter()
    {
        mStack<Term*> st;

        size_t sz = terms.size_();

        for (size_t i = 0; i < sz; i++)
        {
            if (terms[i] -> get_type() == number)
            {
                polish_notation.push_back(terms[i]);
            }
            else if (terms[i] -> get_type() == operation)
            {
                if (st.isEmpty())
                {
                    st.push(terms[i]);
                }
                else
                {
                    while (!st.isEmpty() && st.top() -> get_type() == operation &&
                        static_cast<Operation*>(terms[i])->get_priority() <= static_cast<Operation*>(st.top())->get_priority())
                    {
                        polish_notation.push_back(st.top());

                        st.pop();
                    }
                    st.push(terms[i]);
                }
            }
            else if (terms[i] -> get_type() == open_bracket)
            {
                st.push(terms[i]);
            }
            else if (terms[i] -> get_type() == close_bracket)
            {
                while (st.top() -> get_type() != open_bracket)
                {
                    polish_notation.push_back(st.top());

                    st.pop();
                }
                st.pop();
            }
        }

        while (!st.isEmpty())
        {
            polish_notation.push_back(st.top());

            st.pop();
        }
    }

    std::string get_polish_notation()
    {
        std::string polish_notation_str;

        for (size_t i = 0; i < polish_notation.size_(); i++)
        {
            if (polish_notation[i] -> get_type() == number)
            {
                std::string num = std::to_string(static_cast<Number*>(polish_notation[i]) -> get_value());

                num.erase(num.find_last_not_of('0') + 1, std::string::npos);
                num.erase(num.find_last_not_of('.') + 1, std::string::npos);

                polish_notation_str += num;
            }
            else
            {
                polish_notation_str += static_cast<Operation*>(polish_notation[i]) -> get_op();
            }
        }

        return polish_notation_str;
    }

    void print_polish_notation()
    {
        for (size_t i = 0; i < polish_notation.size_(); i++)
        {
            if (polish_notation[i] -> get_type() == number)
            {
                std::cout << static_cast<Number*>(polish_notation[i]) -> get_value();
            }
            else
            {
                std::cout << static_cast<Operation*>(polish_notation[i]) -> get_op();
            }
        }

        std::cout << std::endl;
    }

    double calculator()
    {
        mStack<double> st;

        double right_numb, left_numb;

        for (size_t i = 0; i < polish_notation.size_(); i++)
        {
            types current_type = polish_notation[i] -> get_type();

            if (current_type == number)
            {
                st.push(static_cast<Number*>(polish_notation[i]) -> get_value());
            }
            else
            {
                right_numb = st.top();
                st.pop();

                left_numb = st.top();
                st.pop();

                switch (static_cast<Operation*>(polish_notation[i]) -> get_op())
                {
                case '*':
                    st.push(left_numb * right_numb);

                    break;

                case '/':
                    if (right_numb == 0)
                    {
                        throw std::runtime_error("Division by zero");
                    }

                    st.push(left_numb / right_numb);

                    break;

                case '+':
                    st.push(left_numb + right_numb);

                    break;

                case '-':
                    st.push(left_numb - right_numb);

                    break;
                }
            }
        }

        return st.top();
    }

    double calculate()
    {
        tokenizer();
        parser();
        converter();

        return calculator();
    }
};