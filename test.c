#include "test_functions.h"

int main()
{
	TestDeleteCharacter();
	TestDynStrCat();
	TestRemoveLastDir();
	TestIsCommand();
	TestDynStrPushBack();
	TestExpandVariables();
	TestReplaceSubStr();
	TestBuildPath();
	TestPathEnvBuildPath();
	TestArrayOps();
	TestVecContainsStr();
	TestProcessQueue();
	TestArgvToString();
	TestOnePipe();
	return 0;
}
