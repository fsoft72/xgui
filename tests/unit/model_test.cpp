// Regression test: Model::removeChild must ignore out-of-range positions.
// The test runner builds libxgui with -D_GLIBCXX_ASSERTIONS so that an out-of-range vector access aborts.
#include <cstdio>
#include <string>
#include <vector>

#include "master.h"
#include "model.h"

int main()
{
	xgui::Model * model = xgui::Master::CreateModel();
	std::vector<std::string> row;
	row.push_back("a");
	model->appendChild(row);
	model->appendChild(row);

	model->removeChild(5);
	model->removeChild(-1);
	if (model->size() != 2) {
		std::fprintf(stderr, "out-of-range removeChild changed the model (size %u)\n", model->size());
		return 1;
	}

	return 0;
}
