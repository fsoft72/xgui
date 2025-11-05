#include "../xgui/debug.h"

int main()
{
	DMESSAGE("Doing Assertion");
	ASSERT(5 < 3);
	DMESSAGE("This shouldn't happen");
}
