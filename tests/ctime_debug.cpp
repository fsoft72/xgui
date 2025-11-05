#include "../xgui/debug.h"

int main()
{
	STATIC_ASSERT((sizeof(char) < sizeof(void*)), SizeofChar_Is_Greater_Than_SizeofVoidPtr);
	STATIC_ASSERT((sizeof(void*) < sizeof(char)), SizeofVoidPtr_Is_Greater_Than_SizeofChar);	
}
