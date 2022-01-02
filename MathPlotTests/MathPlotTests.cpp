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
#include "StatementParser.h"
#include "Point.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathPlotTests
{
	TEST_CLASS(StatementParserTest)
	{
	public:
		TEST_METHOD(TestParse)
		{
			StatementParser parser;
			std::string s1 = "2+3=5";
			Assert::AreEqual("2 + 3 = 5", parser.parse(s1)->getString().c_str());

			std::string s2 = "10x-9y<7x*x+8";
			Assert::AreEqual("10 * x - 9 * y < 7 * x * x + 8", parser.parse(s2)->getString().c_str());
			{
				std::string s2 = "2 >= 3";
				Assert::AreEqual("2 >= 3", parser.parse(s2)->getString().c_str());
			}

			{
				std::string s2 = "2y <= 3x";
				Assert::AreEqual("2 * y <= 3 * x", parser.parse(s2)->getString().c_str());
			}

			{
				std::string s2 = "2y * y * y - 2x * x <= 3x";
				Assert::AreEqual("2 * y * y * y - 2 * x * x <= 3 * x", parser.parse(s2)->getString().c_str());
			}

			{
				std::string s2 = "2y <= 3xxxx";
				Assert::IsFalse(parser.parse(s2).has_value());
			}

			{
				std::string s2 = "2yx";
				Assert::IsFalse(parser.parse(s2).has_value());
			}

			{
				std::string s2 = "2yx-3f=9";
				Assert::IsFalse(parser.parse(s2).has_value());
			}

			{
				std::string s2 = "x>y>2xy=3";
				Assert::IsFalse(parser.parse(s2).has_value());
			}
		}
	};
	TEST_CLASS(PointTEst)
	{
	public:
		TEST_METHOD(TestConstruction)
		{
			Point p1(2.0, 3.0), p2, p3(1.0, 4.0, 6.0);

			Assert::AreEqual(p1.x, 2.0);
			Assert::AreEqual(p1.y, 3.0);
			Assert::AreEqual(p1.value, 0.0);

			Assert::AreEqual(p2.x, 0.0);
			Assert::AreEqual(p2.y, 0.0);
			Assert::AreEqual(p2.value, 0.0);

			Assert::AreEqual(p3.x, 1.0);
			Assert::AreEqual(p3.y, 4.0);
			Assert::AreEqual(p3.value, 6.0);
		}
		TEST_METHOD(TestArithmatics)
		{
			Point p1(2.0, 3.0);

			p1 = p1 + Point(3.0, -4.0);
			Assert::AreEqual(p1.x, 5.0);
			Assert::AreEqual(p1.y, -1.0);

			p1 = p1 - Point(2.0, -3.0);
			Assert::AreEqual(p1.x, 3.0);
			Assert::AreEqual(p1.y, 2.0);

			p1 = p1 * 3;
			Assert::AreEqual(p1.x, 9.0);
			Assert::AreEqual(p1.y, 6.0);


			p1 = p1 / 6;
			Assert::AreEqual(p1.x, 1.5);
			Assert::AreEqual(p1.y, 1.0);
		}
	};
	TEST_CLASS(ExpressionTest)
	{
	public:
		TEST_METHOD(TestClone)
		{
			ExpressionParser parser;
			auto exp1 = parser.parse("2*3y-1/x");
			auto exp2 = exp1->clone();

			Assert::AreEqual(exp1->getString(), exp2->getString());
		}
	};
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

			Assert::IsFalse(parser.parse("Hello123.456World") != nullptr);
			Assert::IsFalse(parser.parse("Hello World") != nullptr);
			Assert::IsFalse(parser.parse("yy") != nullptr);
			Assert::IsFalse(parser.parse("232 223") != nullptr);
			Assert::IsFalse(parser.parse("+-*/") != nullptr);
			Assert::IsFalse(parser.parse("2 3 *") != nullptr);
		}
	};
	TEST_CLASS(ConstantTest)
	{
	public:
		
		TEST_METHOD(TestEvaluate)
		{
			// Positive
			{
				Constant c1(21.3), c2(99.0), c3(3.14159), c4(std::numeric_limits<double>::infinity());
				Assert::AreEqual(c1.evaluate(0, 0), 21.3);
				Assert::AreEqual(c2.evaluate(0, 0), 99.0);
				Assert::AreEqual(c3.evaluate(0, 0), 3.14159);
				Assert::AreEqual(c4.evaluate(0, 0), std::numeric_limits<double>::infinity());
			}

			// Negative and zero
			{
				Constant c1(0), c2(-998.0);
				Assert::AreEqual(c1.evaluate(0, 0), 0.0);
				Assert::AreEqual(c2.evaluate(0, 0), -998.0);
			}
		}

		TEST_METHOD(TestClone)
		{
			auto c1 = std::make_unique<Constant>(3.1415926);
			auto c2 = c1->clone();
			Assert::AreEqual(c1->getString(), c2->getString());
		}
	};

	TEST_CLASS(UnkownsTest)
	{
	public:

		TEST_METHOD(TestEvaluate)
		{
			X x;
			Assert::AreEqual(x.evaluate(1.2, 2.0), 1.2);

			Y y;
			Assert::AreEqual(y.evaluate(1.2, 2.0), 2.0);
		}

		TEST_METHOD(TestClone)
		{
			auto c1 = std::make_unique<X>();
			auto c2 = c1->clone();
			Assert::AreEqual(c1->getString(), c2->getString());
		}
	};

	TEST_CLASS(PlusTest)
	{
	public:

		TEST_METHOD(TestConstantPlusConstant)
		{
			// Constant + constnat
			{
				Constant c1(21.3), c2(99.0), c3(-99.0);
				Plus exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
				Assert::AreEqual(exp1.evaluate(0, 0), 21.3 + 99.0);
				Assert::AreEqual(exp2.evaluate(0, 0), 0.0);
			}

			// Constant + unkown
			{
				Constant c1(3.0);
				X x;
				Y y;
				Plus exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
				Assert::AreEqual(exp1.evaluate(1.0, 0), 4.0);
				Assert::AreEqual(exp2.evaluate(-5.0, 0), -10.0);
				Assert::AreEqual(exp3.evaluate(3.0, -2.0), 1.0);
			}
		}

		TEST_METHOD(TestClone)
		{
			auto c1 = std::make_unique<Constant>(4.0);
			auto c2 = std::make_unique<Constant>(2.0);
			auto e1 = std::make_unique<Plus>(std::move(c1), std::move(c2));
			auto e2 = e1->clone();
			Assert::AreEqual(e1->getString(), e2->getString());
		}
	};

	TEST_CLASS(MinusTest)
	{
	public:

		TEST_METHOD(TestEvaluate)
		{
			{
				Constant c1(21.3), c2(99.0), c3(-99.0);
				Subtract exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
				Assert::AreEqual(exp1.evaluate(0, 0), 21.3 - 99.0);
				Assert::AreEqual(exp2.evaluate(0, 0), 99.0 - (-99.0));
			}

			{
				Constant c1(3.0);
				X x;
				Y y;
				Subtract exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
				Assert::AreEqual(exp1.evaluate(1.0, 0), 3.0 - 1.0);
				Assert::AreEqual(exp2.evaluate(-5.0, 0), -5.0 - (-5.0));
				Assert::AreEqual(exp3.evaluate(3.0, -2.0), -2.0 - (3.0));
			}
		}
		TEST_METHOD(TestClone)
		{
			auto c1 = std::make_unique<Constant>(4.0);
			auto c2 = std::make_unique<Y>();
			auto e1 = std::make_unique<Subtract>(std::move(c1), std::move(c2));
			auto e2 = e1->clone();
			Assert::AreEqual(e1->getString(), e2->getString());
		}
	};

	TEST_CLASS(MultiplyTest)
	{
	public:

		TEST_METHOD(TestEvaluate)
		{
			{
				Constant c1(21.3), c2(99.0), c3(-99.0);
				Multiply exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
				Assert::AreEqual(exp1.evaluate(0, 0), 21.3 * 99.0);
				Assert::AreEqual(exp2.evaluate(0, 0), 99.0 * (-99.0));
			}

			{
				Constant c1(3.0);
				X x;
				Y y;
				Multiply exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
				Assert::AreEqual(exp1.evaluate(1.0, 0), 3.0 * 1.0);
				Assert::AreEqual(exp2.evaluate(-5.0, 0), -5.0 * (-5.0));
				Assert::AreEqual(exp3.evaluate(3.0, -2.0), -2.0 * (3.0));
			}
		}
		TEST_METHOD(TestClone)
		{
			auto c1 = std::make_unique<Constant>(2);
			auto c2 = std::make_unique<X>();
			auto e1 = std::make_unique<Multiply>(std::move(c1), std::move(c2));
			auto e2 = e1->clone();
			Assert::AreEqual(e1->getString(), e2->getString());
		}
	};

	TEST_CLASS(DivideTest)
	{
	public:

		TEST_METHOD(TestEvaluate)
		{
			{
				Constant c1(21.3), c2(99.0), c3(-99.0);
				Divide exp1(std::make_unique<Constant>(c1), std::make_unique<Constant>(c2)), exp2(std::make_unique<Constant>(c2), std::make_unique<Constant>(c3));
				Assert::AreEqual(exp1.evaluate(0, 0), 21.3 / 99.0);
				Assert::AreEqual(exp2.evaluate(0, 0), 99.0 / (-99.0));
			}

			{
				Constant c1(3.0);
				X x;
				Y y;
				Divide exp1(std::make_unique<Constant>(c1), std::make_unique<X>(x)), exp2(std::make_unique<X>(x), std::make_unique<X>(x)), exp3(std::make_unique<Y>(y), std::make_unique<X>(x));
				Assert::AreEqual(exp1.evaluate(1.0, 0), 3.0 / 1.0);
				Assert::AreEqual(exp2.evaluate(-5.0, 0), -5.0 / (-5.0));
				Assert::AreEqual(exp3.evaluate(3.0, -2.0), -2.0 / (3.0));
			}
		}
		TEST_METHOD(TestClone)
		{
			auto c1 = std::make_unique<Constant>(2);
			auto c2 = std::make_unique<Constant>(3);
			auto d1 = std::make_unique<Divide>(std::move(c1), std::move(c2));
			auto d2 = d1->clone();
			Assert::AreEqual(d1->getString(), d2->getString());
		}
	};
}
