#include "pch.h"
#include "CppUnitTest.h"

#include "Constant.h"
#include "X.h"
#include "Y.h"
#include "Plus.h"
#include "Subtract.h"
#include "Multiply.h"
#include "Divide.h"
#include "ExpressionParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathPlotTests
{
	TEST_CLASS(ExpressionParserTest)
	{
	public:
		TEST_METHOD(TestParse)
		{
			ExpressionParser parser;
			std::string s = "2+3";
			Assert::AreEqual("2 + 3", parser.parse(s)->getString().c_str());
			Assert::AreEqual("5 - 2 * x", parser.parse("5 - 2 * x")->getString().c_str());
			Assert::AreEqual("x * 2", parser.parse("x *2")->getString().c_str());
			Assert::AreEqual("y / x", parser.parse("y/x")->getString().c_str());


			Assert::AreEqual("5 * x + 9 * y", parser.parse("5x + 9y")->getString().c_str());
			Assert::AreEqual("115 * y + 35 * x", parser.parse("115y + 35x")->getString().c_str());
			Assert::AreEqual("37 * x + 6", parser.parse("37x + 6")->getString().c_str());
			Assert::AreEqual("102 * x * 400", parser.parse("102x * 400")->getString().c_str());
			Assert::AreEqual("x * y + 2 * x", parser.parse("xy + 2x")->getString().c_str());

			Assert::IsFalse(parser.parse("yy") != nullptr);
			Assert::IsFalse(parser.parse("232 223") != nullptr);
			Assert::IsFalse(parser.parse("+-*/") != nullptr);
			Assert::IsFalse(parser.parse("2 3 *") != nullptr);
		}

		TEST_METHOD(TestExtractNumber)
		{
			ExpressionParser parser;
			std::string s = "Hello123.456World";
			int i = 5;
			double d = parser.extractNumber(s, i);
			Assert::AreEqual(d, 123.456);

			s = "123 456";
			i = 0;
			Assert::AreEqual(123.0, parser.extractNumber(s, i));
			i++;
			Assert::AreEqual(456.0, parser.extractNumber(s, i));
		}
		TEST_METHOD(TestDecimalStartWithDot)
		{
			ExpressionParser parser;
			std::string s = ".233World";
			int i = 0;
			double d = parser.extractNumber(s, i);
			Assert::AreEqual(0.233, d);
		}
		TEST_METHOD(TestDoubleDot)
		{
			ExpressionParser parser;
			std::string s = ".2.33World";
			int i = 0;
			double d = parser.extractNumber(s, i);
			Assert::AreEqual(0.2, d);
			d = parser.extractNumber(s, i);
			Assert::AreEqual(0.33, d);
		}
		TEST_METHOD(TestFailure)
		{
			ExpressionParser parser;
			std::string s = "HelloWorld";
			int i = 3;
			double d = parser.extractNumber(s, i);
			Assert::AreEqual(d, 0.0);
			Assert::AreEqual(i, 3);
		}
	};
	TEST_CLASS(ConstantTest)
	{
	public:
		
		TEST_METHOD(TestPositive)
		{
			Constant c1(21.3), c2(99.0), c3(3.14159), c4(std::numeric_limits<double>::infinity());
			Assert::AreEqual(c1.evaluate(0, 0), 21.3);
			Assert::AreEqual(c2.evaluate(0, 0), 99.0);
			Assert::AreEqual(c3.evaluate(0, 0), 3.14159);
			Assert::AreEqual(c4.evaluate(0, 0), std::numeric_limits<double>::infinity());
		}

		TEST_METHOD(TestNegativeAndZero)
		{
			Constant c1(0), c2(-998.0);
			Assert::AreEqual(c1.evaluate(0, 0), 0.0);
			Assert::AreEqual(c2.evaluate(0, 0), -998.0);
		}
	};

	TEST_CLASS(UnkownsTest)
	{
	public:

		TEST_METHOD(TestX)
		{
			X x;
			Assert::AreEqual(x.evaluate(1.2, 2.0), 1.2);
		}

		TEST_METHOD(TestY)
		{
			Y y;
			Assert::AreEqual(y.evaluate(1.2, 2.0), 2.0);
		}
	};

	TEST_CLASS(PlusTest)
	{
	public:

		TEST_METHOD(TestConstantPlusConstant)
		{
			Constant c1(21.3), c2(99.0), c3(-99.0);
			Plus exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
			Assert::AreEqual(exp1.evaluate(0, 0), 21.3 + 99.0);
			Assert::AreEqual(exp2.evaluate(0, 0), 0.0);
		}

		TEST_METHOD(TestConstantPlusUnkown)
		{
			Constant c1(3.0);
			X x;
			Y y;
			Plus exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
			Assert::AreEqual(exp1.evaluate(1.0, 0), 4.0);
			Assert::AreEqual(exp2.evaluate(-5.0, 0),-10.0);
			Assert::AreEqual(exp3.evaluate(3.0, -2.0), 1.0);
		}
	};

	TEST_CLASS(MinusTest)
	{
	public:

		TEST_METHOD(TestConstantMinusConstant)
		{
			Constant c1(21.3), c2(99.0), c3(-99.0);
			Subtract exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
			Assert::AreEqual(exp1.evaluate(0, 0), 21.3 - 99.0);
			Assert::AreEqual(exp2.evaluate(0, 0), 99.0 - (-99.0));
		}

		TEST_METHOD(TestConstantMinusUnkown)
		{
			Constant c1(3.0);
			X x;
			Y y;
			Subtract exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
			Assert::AreEqual(exp1.evaluate(1.0, 0), 3.0 - 1.0);
			Assert::AreEqual(exp2.evaluate(-5.0, 0),-5.0 - (-5.0));
			Assert::AreEqual(exp3.evaluate(3.0, -2.0), -2.0 - (3.0));
		}
	};

	TEST_CLASS(MultiplyTest)
	{
	public:

		TEST_METHOD(TestConstantMultiplyConstant)
		{
			Constant c1(21.3), c2(99.0), c3(-99.0);
			Multiply exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
			Assert::AreEqual(exp1.evaluate(0, 0), 21.3 * 99.0);
			Assert::AreEqual(exp2.evaluate(0, 0), 99.0 * (-99.0));
		}

		TEST_METHOD(TestConstantMultiplyUnkown)
		{
			Constant c1(3.0);
			X x;
			Y y;
			Multiply exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
			Assert::AreEqual(exp1.evaluate(1.0, 0), 3.0 * 1.0);
			Assert::AreEqual(exp2.evaluate(-5.0, 0), -5.0 * (-5.0));
			Assert::AreEqual(exp3.evaluate(3.0, -2.0), -2.0 * (3.0));
		}
	};

	TEST_CLASS(DivideTest)
	{
	public:

		TEST_METHOD(TestConstantDivideConstant)
		{
			Constant c1(21.3), c2(99.0), c3(-99.0);
			Divide exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
			Assert::AreEqual(exp1.evaluate(0, 0), 21.3 / 99.0);
			Assert::AreEqual(exp2.evaluate(0, 0), 99.0 / (-99.0));
		}

		TEST_METHOD(TestConstantDivideUnkown)
		{
			Constant c1(3.0);
			X x;
			Y y;
			Divide exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
			Assert::AreEqual(exp1.evaluate(1.0, 0), 3.0 / 1.0);
			Assert::AreEqual(exp2.evaluate(-5.0, 0), -5.0 / (-5.0));
			Assert::AreEqual(exp3.evaluate(3.0, -2.0), -2.0 / (3.0));
		}
	};
}
