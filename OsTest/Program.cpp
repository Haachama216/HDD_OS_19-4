#include "Program.h"

Program::Program()
{
	FixConsoleWindow();
	FixSizeWindow(123, 200);
	this->Vol = nullptr;
}

Program::~Program()
{
	if (this->Vol != nullptr) {
		delete this->Vol;
		this->Vol = nullptr;
	}
}

void Program::run()
{
	this->homeNavigate();
}

void Program::openVolume(string str)
{
	system("cls");

	while (true) {

		// If the input is ""
		if (str == "") {
			return;
		}

		// Open this volume
		this->initializeVolume(str);

		if (this->Vol->isVolumeFile())
			this->Vol->open();
		else {
			cout << "\n";
			cout << "  Program: Cannot open this volume." << "\n\n";

			cout << "  ";
			system("pause");
		}

		this->closeVolume();
		break;
	}
}

void Program::createVolume()
{
	system("cls");

	// Input a path
	cout << "  Program: * Input a path to create a volume in\n";
	cout << "           * Do not input anything then press Enter to EXIT";
	cout << "\n\n" << "  User: ";
	string volumeFilePath;
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	getline(cin, volumeFilePath);

	// If the input is ""
	if (volumeFilePath == "") {
		return;
	}

	// Create this volume
	this->initializeVolume(volumeFilePath + Volume::Extension);
	//save path in path.txt
	ofstream fout("path.txt");
	fout << (volumeFilePath + Volume::Extension);

	if (this->Vol->create()) {
		cout << "\n";
		cout << "  Program: * The new volume is created successfully!" << "\n";
		cout << "           * Press Enter to OPEN this volume" << "\n";
		cout << "           * Press any key except for Enter to EXIT";
		cout << "\n\n";

		while (true) {
			if (_kbhit()) {
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				if ((GetKeyState(VK_RETURN) & 0x8000)) {
					while ((GetKeyState(VK_RETURN) & 0x8000)) {};
					
					system("cls");
					this->Vol->open();
					break;
				}
				else {
					break;
				}
			}
		}
	}
	else {
		cout << endl;
		cout << "  Program: Cannot create a new volume." << "\n\n";
		cout << "           This path does not exist OR it is taken." << "\n\n";
		cout << "           Please input another path." << "\n\n";
		cout << "  ";
		system("pause");
	}

	this->closeVolume();
}

void Program::initializeVolume(string const& volumeFilePath)
{
	this->Vol = new Volume(volumeFilePath);
}

void Program::closeVolume()
{
	delete this->Vol;
	this->Vol = nullptr;
}

void Program::homeNavigate() {
	clrscr();
	GUI::clearBackground();

	string path;
	
	string dummy, pw;
	
	SHA256 sha256;

	GUI::line = 0;
	int run = 1;
	
	while (run)
	{
		system("cls");
		int work = 0;
		cout << R"(*Press [F1] after open volume to see instruction*
Chon 1 trong cac cong viec sau (nhan phim khac de thoat):
1. Tao / dinh dang volume MyFS.Dat (Format: <Path>\MyFS)
2. Thiet lap/ Doi/ Kiem tra mat khau truy xuat MyFS
3. Liet ke danh sach cac tap tin trong MyFS (Open MyFS)
> Your choice: )";
		cin >> work;
		cin.ignore();

		ifstream inPath("path.txt");
		inPath >> path;

		switch (work)
		{
		case 1:
			if (path.compare("") == 0)
				this->createVolume();
			else
				cout << "\nMyFS.dat has been existed!";
			break;
		case 2: {
			if (path.compare("") == 0) {
				cout << "\nThere hasn't been any MyFS.dat volume! Please create one!";
				break;
			}
			fstream fin("dummy.txt");
			fin >> dummy;
			fin.close();
			//Thiet lap pass
			if (dummy.compare("") == 0) {
				clrscr();
				pw = GUI::enterPassword();
				ofstream fout("dummy.txt");
				fout << sha256(addPepper(addSalt(pw)));
				fout.close();
			}
			//Doi pass
			else {
				pw = GUI::enterPassOnce();
				if (checkPassword(pw, dummy)) {
					clrscr();
					cout << "Enter new password:";
					pw = GUI::enterPassword();
					ofstream fout("dummy.txt");
					fout << sha256(addPepper(addSalt(pw)));
					fout.close();
				}
				else
					cout << "Wrong password!";
			}
			break;
		}
		case 3:
		{
			if (path.compare("") != 0) {
				fstream fin("dummy.txt");
				fin >> dummy;
				fin.close();
				if (dummy.compare("") != 0) {
					pw = GUI::enterPassOnce();
					if (!checkPassword(pw, dummy))
						cout << "\nWrong password!";
					else
						this->openVolume(path);
				}
				else
					this->openVolume(path);
			}
			else
				cout << "\nThere hasn't been any MyFS.dat volume! Please create one!";
			break;
		}
		default: run = 0;
		}
		cout << "\n";
		system("pause");
	}
}

