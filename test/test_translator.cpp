#include "gtest.h"
#include "translator.h"

TEST(Number, can_create_number)
{
	ASSERT_NO_THROW(Number num(4.5));
}

TEST(Number, can_get_number_value)
{
	Number num(4.5);

	EXPECT_EQ(num.get_value(), 4.5);
}

TEST(Operation, can_create_operation)
{
	ASSERT_NO_THROW(Operation op(4.5));
}

TEST(Operation, can_get_operation)
{
	Operation op('*');

	EXPECT_EQ(op.get_op(), '*');
}

TEST(OpenBracket, can_create_open_bracket)
{
	ASSERT_NO_THROW(OpenBracket ob);
}

TEST(OpenBracket, can_get_open_bracket)
{
	OpenBracket ob;

	EXPECT_EQ(ob.get_op(), '(');
}

TEST(CloseBracket, can_create_close_bracket)
{
	ASSERT_NO_THROW(CloseBracket ob);
}

TEST(CloseBracket, can_get_close_bracket)
{
	CloseBracket cb;

	EXPECT_EQ(cb.get_op(), ')');
}

TEST(Translator, can_create_translator)
{
	ASSERT_NO_THROW(Translator T);
}

TEST(Translator, can_create_translator_with_expression)
{
	ASSERT_NO_THROW(Translator T("3.14*(5.71+1)-5/3.5+(8-4)"));
}

TEST(Translator, can_get_expression)
{
	std::string expression = "3.14*(5.71+1)-5/3.5+(8-4)";

	Translator T(expression);

	EXPECT_EQ(T.get_expression(), expression);
}

TEST(Translator, can_get_expression_size)
{
	std::string expression = "3.14*(5.71+1)-5/3.5+(8-4)";

	Translator T(expression);

	EXPECT_EQ(T.get_expression_size(), expression.size());
}

TEST(Translator, can_parse_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	T.tokenizer();

	ASSERT_NO_THROW(T.parser());
}

TEST(Translator, can_tokenize_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	ASSERT_NO_THROW(T.tokenizer());
}

TEST(Translator, can_get_terms_size_after_tokenizer)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	T.tokenizer();

	EXPECT_EQ(T.get_terms_size(), 17);
}

TEST(Translator, throws_when_tokenize_incorrect_syntax_expression)
{
	Translator T("35+d97+(8*8)");

	ASSERT_ANY_THROW(T.tokenizer());
}

TEST(Translator, throws_when_parse_incorrect_syntax_expression)
{
	Translator T("3.14*(5.71+1))-5/3.5+(8-4)");

	T.tokenizer();

	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_when_parse_incorrect_bracket_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4))");

	T.tokenizer();

	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_when_parse_incorrect_bracket_order_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4");

	T.tokenizer();

	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_when_parse_incorrect_first_symbol_expression)
{
	Translator T(")3.14*(5.71+1)-5/3.5+(8-4)");

	T.tokenizer();

	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, throws_when_parse_incorrect_last_symbol_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)*");

	T.tokenizer();

	ASSERT_ANY_THROW(T.parser());
}

TEST(Translator, can_convert_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	T.tokenizer();
	T.parser();

	ASSERT_NO_THROW(T.converter());
}

TEST(Translator, converter_works_correctly)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	T.tokenizer();
	T.parser();
	T.converter();

	EXPECT_EQ(T.get_polish_notation(), "3.145.711+*53.5/-84-+");
}

TEST(Translator, can_calculate_expression)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	ASSERT_NO_THROW(T.calculate());
}

TEST(Translator, calculator_works_correctly)
{
	Translator T("3.14*(5.71+1)-5/3.5+(8-4)");

	EXPECT_EQ(T.calculate(), 23.640828571428575);
}

TEST(Translator, throws_when_division_by_zero)
{
	Translator T("3.14/(5.71-5.71)");

	ASSERT_ANY_THROW(T.calculate());
}