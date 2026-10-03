// Unit test: decoding of a GDK button number and modifier state into the xgui button name and modifier mask.
#include <cstdio>
#include <string>

#include "base.h"
#include "definitions.h"
#include "gtk/widget_impl.h"

/** Decodes an event and compares the result with the expected one; returns 1 on mismatch. */
static int expect_decoded(unsigned int button, unsigned int state, char const * name, int mask)
{
	std::string decoded_name;
	int decoded_mask = -1;
	xguimpl::Widget::decodeButtonEvent(button, (GdkModifierType)state, decoded_name, decoded_mask);

	if (decoded_name != name || decoded_mask != mask) {
		std::fprintf(stderr, "button %u state 0x%x decoded as '%s' mask 0x%x, expected '%s' mask 0x%x\n",
		             button, state, decoded_name.c_str(), decoded_mask, name, mask);
		return 1;
	}
	return 0;
}

int main()
{
	int failures = 0;
	failures += expect_decoded(1, 0, "left", 0);
	failures += expect_decoded(2, 0, "middle", 0);
	failures += expect_decoded(3, 0, "right", 0);
	failures += expect_decoded(9, 0, "", 0);
	failures += expect_decoded(1, GDK_BUTTON1_MASK | GDK_CONTROL_MASK, "left", xgui::mouseLeftMask | xgui::keyControlMask);
	failures += expect_decoded(3, GDK_BUTTON2_MASK | GDK_BUTTON3_MASK | GDK_SHIFT_MASK | GDK_ALT_MASK, "right",
	                           xgui::mouseMiddleMask | xgui::mouseRightMask | xgui::keyShiftMask | xgui::keyAltMask);
	return failures ? 1 : 0;
}
