// Unit test (no display needed): xgui::semantic_cast conversions between strings and numbers.
#include <cstdio>
#include <string>
#include <typeinfo>

#include "base.h"

/** Prints the message and returns 1 when the condition is false. */
static int expect(bool condition, char const * message)
{
	if (condition) return 0;
	std::fprintf(stderr, "%s\n", message);
	return 1;
}

int main()
{
	int failures = 0;

	failures += expect(xgui::semantic_cast<int>(std::string("42")) == 42, "string -> int");
	failures += expect(xgui::semantic_cast<int>(std::string("-7")) == -7, "negative string -> int");
	failures += expect(xgui::semantic_cast<std::string>(42) == "42", "int -> string");
	failures += expect(xgui::semantic_cast<std::string>(1.5) == "1.5", "double -> string");
	failures += expect(xgui::semantic_cast<double>(std::string("2.25")) == 2.25, "string -> double");
	failures += expect(xgui::semantic_cast<bool>(std::string("1")) == true, "\"1\" -> bool");
	failures += expect(xgui::semantic_cast<bool>(std::string("0")) == false, "\"0\" -> bool");
	failures += expect(xgui::semantic_cast<int>(5) == 5, "same type is returned as is");
	failures += expect(xgui::semantic_cast<std::string>(std::string("x")) == "x", "string -> string");

	bool threw = false;
	try {
		xgui::semantic_cast<int>(std::string("not a number"));
	}
	catch (std::bad_cast const &) {
		threw = true;
	}
	failures += expect(threw, "invalid string -> int must throw std::bad_cast");

	threw = false;
	try {
		xgui::semantic_cast<int>(3.5);
	}
	catch (std::bad_cast const &) {
		threw = true;
	}
	failures += expect(threw, "unsupported double -> int must throw std::bad_cast");

	return failures ? 1 : 0;
}
