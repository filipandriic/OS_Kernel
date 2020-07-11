#include "locks.h"
#include "MainProg.h"
#include "UserMain.h"

int main(int argc, char* argv[])
{
	MainProgram::start();
	
	userThread* user = new userThread(argc, argv);
	user->start();
	user->waitToComplete();
	int ret = user->getRet();

	MainProgram::finish();
	delete user;
	return ret;
}
