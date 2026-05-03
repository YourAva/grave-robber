#include "grave-robber.hpp"

int main() {
	grvRbr graveRobber;
	graveRobber.procName = L"{executableName}.exe";
	graveRobber.dumpPath = L"C:\\dir\\dir2\\dir3\\{dumpName}.dmp";
	graveRobber.dumpProc();
}