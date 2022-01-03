#include "Maxwell.h"
#include "Application.h"


int main( int argc, char * argv[] ) {
	Application::Init();

	Maxwell tp_engine;
	tp_engine.Run();

	Application::Terminate();
	return 0;
}