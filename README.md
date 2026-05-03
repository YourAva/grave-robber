# grave-robber 🪦

This program was made following [this post](https://medium.com/@s12deff/dumping-edge-browser-to-steal-cookies-cb5d659917d2) by **0x12 Dark Development**, I've removed the use of strings with character arrays to minimise the number of outside calls to make the program less detectable to any form of antivirus.
### How to use
```
#include "grave-robber.hpp"

int main() {
	grvRbr graveRobber;
	graveRobber.procName = L"{executableName}.exe";
	graveRobber.dumpPath = L"C:\\dir\\dir2\\dir3\\{dumpName}.dmp";
	graveRobber.dumpProc();
}
```
1. Import the grave-robber header file
2. Instantiate the grvRbr class
3. Define `procName` and `dumpPath` attributes for the instantiated object
4. Run the `dumpProc` method
### Debugging
If you face any errors during runtime, you'll reach a GetLastError() print. Look at [MSDOC System Error Codes](https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-) for more information to debug..

