#include <iostream>
#include <fstream>
#include <string>
#include "testSystem.pb.h"
using namespace std;

void SetName(int32_t* num) {
	int32_t number;
	cout << "Enter a number: ";
	cin >> number;

	*num = number;
	cout << "Input read: " << number << '\n';
	cout << "New number: " << *num << '\n';
}

int main(int argc, char* argv[]) {
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	testsystem::Shape shape;

	fstream input(argv[1], ios::in | ios::binary);

	if (!input) {
		cout << argv[1] << ": File not found, creating a new file." << '\n';
	}
	else if (!shape.ParseFromIstream(&input)) {
		cout << "Failed to parse address book." << '\n';
		return -1;
	}

	shape.add_number(5);

	fstream output(argv[1], ios::out | ios::trunc | ios::binary);
	if (!shape.SerializeToOstream(&output)) {
		cout << "Failed to write." << '\n';
		return -1;
	}

	cout << argv[1] << '\n';
	cout << shape.number(0) << '\n';

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

