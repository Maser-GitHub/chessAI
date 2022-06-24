#include "Util.h"

int Util::ReadNumber(string output) {
	string input = "";
	int read;
	while (true) {
		cout << output << endl;
		getline(cin, input);
		stringstream myStream(input);
		if (myStream >> read)
			return read;
		cout << "Invalid number, please try again" << endl;
	}
}
