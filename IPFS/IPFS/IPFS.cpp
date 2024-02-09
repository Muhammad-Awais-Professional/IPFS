#include <iostream>
#include "CircularList.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Hash.h"
using namespace std;

bool copyFile(const string& sourcePath, filesystem::path dest) {

	filesystem::path source(sourcePath);

	ifstream srcFile(sourcePath, ios::binary);
	if (!srcFile) {
		cout << "Cannot open source file: " << sourcePath << endl;
		return false;
	}


	ofstream destFile(dest, ios::binary);
	if (!destFile) {
		cout << "Cannot open destination file: " << dest << endl;
		return false;
	}


	destFile << srcFile.rdbuf();


	if (!srcFile.good() || !destFile.good()) {
		cout << "Error occurred during file copy." << endl;
		return false;
	}

	return true;
}

std::string readAnyFile(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cerr << "Cannot open file: " << filePath << std::endl;
		return "";  
	}

	std::ostringstream contentStream;
	contentStream << file.rdbuf();

	if (!file.good()) {
		std::cerr << "Error occurred while reading file: " << filePath << std::endl;
		return "";  
	}

	return contentStream.str();
}

string reverseString(string str) {
	int start = 0;
	int end = str.length() - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;

		start++;
		end--;
	}
	return str;
}


string Filename(string path) {
	int len = path.size();
	string ans;
	for (int i = len - 1; i >= 0; i--)
	{
		if (path[i] == '\\') {
			break;
		}
		ans += path[i];
	}

	return reverseString(ans);
}

bool insertFile(Pair SelectedFile, string upp, int id_space, DHT IPFS, Machine* machinesarr,unsigned long long num_on_machines) {
	string filecontent = readAnyFile(upp);
	if (filecontent == "") {
		return false;
	}

	File filetoinsert;

	HashMap hash;

	filetoinsert.key = hash.HashingFunction(filecontent, id_space);

	cout << filetoinsert.key << endl;

	string fn;

	fn = Filename(upp);

	Pair temp;

	temp.Key = filetoinsert.key;

	if (IPFS.search(temp, SelectedFile)) {
		;
	}
	else {
		for (int i = 0;i < num_on_machines;i++) {
			if (filetoinsert.key < machinesarr[i].data.Key) {
				SelectedFile = machinesarr[i].data;
				break;
			}
		}
	}

	filetoinsert.filePath = SelectedFile.machinePath / fn;

	SelectedFile.Files->insert(filetoinsert);

	cout << "Displaying B-Tree: " << endl;

	SelectedFile.Files->printTree();

	return copyFile(upp, filetoinsert.filePath);
}

int main()
{
	fs::path directorypath = "Machines";

	if (!fs::exists(directorypath)) {
		fs::create_directory(directorypath);
		cout << "Directory created: " << directorypath << endl;
	}
	else {
		fs::remove_all(directorypath);
		cout << "Old Directory deleted: " << directorypath << endl;
		fs::create_directory(directorypath);
		cout << "New Directory created: " << directorypath << endl;
	}

	fs::path subdirectorypath;

	DHT IPFS;
	string name;
	BigBin key;
	HashMap hash;

	int id_space;

	unsigned long long max;

	unsigned long long num_on_machines = 0;

	char choice = 'y';

	cout << "Enter identifier space: ";

	cin >> id_space;

	cout << "Please specify the order of B-Tree: ";

	int order = 3;

	cin >> order;

	while (1) {

		if (order >= 3) {
			break;
		}
		cout << "The minimum order must be 3, please try again!" << endl;

		cout << "Please specify the order of B-Tree: ";

		cin >> order;

	}

	max = pow(2, id_space);

	cout << "Enter the number of ON machines: ";

	cin >> num_on_machines;

	Machine* machinesarr = new Machine[num_on_machines];

	cout << "Do you want to manually assign the ids? (y/n) ";

	cin >> choice;

	while (!(choice == 'y' || choice == 'n')) {
		cout << "Please enter only small y or n." << endl;

		cout << "Do you want to manually assign the ids? (y/n) ";

		cin >> choice;
	}

	while (num_on_machines >= max) {
		cout << "Enter the number of ON machines: ";

		cin >> num_on_machines;
	}

	for (int i = 0; i < num_on_machines; i++) {
		cout << "Enter the name of machine: ";

		cin >> name;

		Pair DataBot;

		DataBot.Files = new BTree<File>(order);

		DataBot.Name = name;

		DataBot.id_space = id_space;

		if (choice == 'y') {
			bool valid = false;
			cout << "Enter a hash key in binary: ";
			cin >> DataBot.Key;

			while (!valid) {
				int i = 0;
				for (i = 0;DataBot.Key.binary[i] != '\0';i++) {
					if (!(DataBot.Key.binary[i] == '0' || DataBot.Key.binary[i] == '1')) {
						cout << "Please enter a valid binary key!" << endl;
						valid = false;
						cout << "Enter a hash key again, in binary: ";
						cin >> DataBot.Key;
					}
					else {
						valid = true;
					}
				}
				if (!(i == id_space)) {
					valid = false;
					cout << i << " The binary key must have number of bits equal to identifier space!" << endl;
					valid = false;
					cout << "Enter a hash key again, in binary: ";
					cin >> DataBot.Key;
				}
			}
		}
		else {
			key = hash.HashingFunction(name, id_space);
			DataBot.Key = key;
		}

		Pair temp;

		if (IPFS.search(DataBot, temp))
		{
			cout << "Already have a Machine with this Name or Key!\nKindly Insert Something Different\n\n";
			i--;
			continue;
		}
		else
		{
			subdirectorypath = directorypath / DataBot.Key.binary;
			DataBot.machinePath = subdirectorypath;
			IPFS.insert(DataBot);
			machinesarr[i] = DataBot;
			
			Machine* temp = IPFS.head;

			fs::create_directory(DataBot.machinePath);
			cout << "Subdirectory created: " << DataBot.machinePath << endl;

		}
	}
	IPFS.CreateRoutingTable();

	IPFS.display();

	Pair DataBot;

	Pair SelectedMachine;

	system("cls");

	cout << "======================================================================================" << endl;

	IPFS.display();

	cout << endl;

	cout << "Enter a machine's key to select it for traversal: ";
	cin >> DataBot.Key;
	bool valid = false;

	while (!valid) {
		int i = 0;
		for (i = 0;DataBot.Key.binary[i] != '\0';i++) {
			if (!(DataBot.Key.binary[i] == '0' || DataBot.Key.binary[i] == '1')) {
				cout << "Please enter a valid binary key!" << endl;
				valid = false;
				cout << "Enter a hash key again, in binary: ";
				cin >> DataBot.Key;
			}
			else {
				valid = true;
			}
		}
		if (!(i == id_space)) {
			valid = false;
			cout << i << " The binary key must have number of bits equal to identifier space!" << endl;
			valid = false;
			cout << "Enter a hash key again, in binary: ";
			cin >> DataBot.Key;
		}
		if (IPFS.search(DataBot, SelectedMachine)) {
			cout << "Machine was found and selected!" << endl;
			cout<< SelectedMachine.Name << endl;
		}
		else {
			cout << i << " The binary key did not match with any machine available!" << endl;
			valid = false;
			cout << "Enter a hash key again, in binary: ";
			cin >> DataBot.Key;
		}
	}

	system("cls");
	
	int option = 0;

	cout << "======================================================================================" << endl;

	do {
		cout << "Select an option:" << endl;
		cout << "0) Exit" << endl;
		cout << "1) Insert File." << endl;
		cout << "2) Display Machines." << endl;
		cout << "3) Display B-Tree of selected machine." << endl;
		cout << "4) Select another machine." << endl;
		cout << "5) Display routing table of all machines." << endl;
		cout << "6) Display routing table of current machines." << endl;
		cout << "Enter your choice: ";
		cin >> option;
		if (option > 6 || option < 0) {
			cout << "Not a valid choice!" << endl;
			continue;
		}

		if (option == 1) {
			cout << "Enter source file path: ";
			string file;
			cin >> file;
			cout << endl << file << endl;

			if (insertFile(SelectedMachine, file, id_space, IPFS, machinesarr, num_on_machines)) {
				cout << "File inserted successfully!" << endl;
				continue;
			}
			else {
				cout << "An error occoured!" << endl;
				continue;
			}
		}
		else if (option == 2) {
			
			cout << endl;
			IPFS.display();
			cout << endl;
			continue;
		}
		else if (option == 3) {
			cout << "Displaying B-Tree of selected Machine: "<<endl;
			SelectedMachine.Files->printTree();
			cout << endl;
			continue;
		}
		else if (option == 4) {
			system("cls");

			cout << "======================================================================================" << endl;

			IPFS.display();

			cout << endl;

			cout << "Enter a machine's key to select it for traversal: ";
			cin >> DataBot.Key;
			bool valid = false;

			while (!valid) {
				int i = 0;
				for (i = 0;DataBot.Key.binary[i] != '\0';i++) {
					if (!(DataBot.Key.binary[i] == '0' || DataBot.Key.binary[i] == '1')) {
						cout << "Please enter a valid binary key!" << endl;
						valid = false;
						cout << "Enter a hash key again, in binary: ";
						cin >> DataBot.Key;
					}
					else {
						valid = true;
					}
				}
				if (!(i == id_space)) {
					valid = false;
					cout << i << " The binary key must have number of bits equal to identifier space!" << endl;
					valid = false;
					cout << "Enter a hash key again, in binary: ";
					cin >> DataBot.Key;
				}
				if (IPFS.search(DataBot, SelectedMachine)) {
					cout << "Machine was found and selected!" << endl;
					cout << SelectedMachine.Name << endl;
				}
				else {
					cout << i << " The binary key did not match with any machine available!" << endl;
					valid = false;
					cout << "Enter a hash key again, in binary: ";
					cin >> DataBot.Key;
				}
			}

			system("cls");
			continue;
		}
		else if (option == 5) {
			cout << endl;
			cout << endl;
			cout << "Displaying rotuing table of all machines: " << endl;
			Machine* temp = IPFS.head;
			temp->data = SelectedMachine;
			do {
				temp->RT->display();
				temp = temp->next;
			} while (temp != IPFS.head);
			cout << endl;
			cout << endl;
			continue;
		}
		else if (option == 6) {
			cout << endl;
			cout << endl;
			cout << "Displaying rotuing table of current machine: " << endl;
			Machine* temp = IPFS.head;
			temp->data = SelectedMachine;
			temp->RT->display();
			cout << endl;
			cout << endl;
			continue;
		}

	} while (option !=0 );
	cout << "Exiting... " << endl;
}