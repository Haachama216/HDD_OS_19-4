#include "GUI.h"

int GUI::line = 0;
bool GUI::esc = false;

void GUI::setWindows(int height, int width) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    MoveWindow(console, r.left, r.top, height, width, TRUE); // 1024 width, 576 height
}

void GUI::clearBackground() {
    setColor(0, 0);

    printSpace(123); cout << endl;
    printSpace(123);
    gotoXY(0, line + 2);
    printSpace(123); 
    gotoXY(0, line + 25);
    printSpace(123);
    gotoXY(0, 31);
    printSpace(123);
    gotoXY(0, 0);

    setColor(15, 0);
}

void GUI::reset() {
    line = 0;
}

string GUI::hidePassword() {
    //setColor(COLOR::WHITE, COLOR::BLACK);
    string input;
    char temp;
    while (true) {
        temp = _getch();
        if (GetKeyState(VK_ESCAPE) & 0x8000) {
            while (GetKeyState(VK_ESCAPE) & 0x8000) {};
            continue;
        }
        else if (temp == '\0') {
            continue;
        }
        else if (temp == '\r') {
            break;
        }
        else if (input.length() > 0 && temp == 8) {
            input.pop_back();
            std::cout << "\b \b";
        }
        else if (input.length() == 0 && temp == 8) {
            continue;
        }
        else {
            std::cout << temp;
            Sleep(50);
            std::cout << "\b*";
            input.push_back(temp);
        }
    }
    return input;
}

string GUI::enterPassword() {
    string pw, repw;

    while (true) {
        gotoXY(0, 2); 
        cout << "  Password:  ";      pw   = GUI::hidePassword();      cout << endl;
        cout << "  Re-enter:  ";      repw = GUI::hidePassword();      cout << endl;

        if (pw.compare(repw) == 0 && pw.length() >= 8) {
            break;
        }
        else {
            clrscr();
            gotoXY(0, 0);
            cout << "  Error: Password must be the same and have at least 8 characters. " << endl;
        }
    }
    return pw;
}

string GUI::enterPassOnce()
{
    string pw;
    clrscr();
    gotoXY(0, 2);
    cout << "  Password:  ";      pw = GUI::hidePassword();      cout << endl;

    return pw;
}

void GUI::checkPassword(int i) {
    i = i % 3;

    gotoXY(0, 5);
    switch (i) {
    case 0:
        cout << "  Verifing password  .      ";
        break;
    case 1:
        cout << "  Verifing password  .  .   ";
        break;
    case 2:
        cout << "  Verifing password  .  .  .";
        break;
    }
}

void GUI::printProgress(string content, double percentage) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;

    setColor(13, 0);
    cout << content; gotoXY(10 - content.length(), whereY());

    setColor(14, 0);
    printf("%3d%%", val);

    setColor(10, 0);
    printf("  [%.*s%*s]\n", lpad, PBSTR, rpad, ""); 

    setColor(15, 0);
    fflush(stdout);
}

void GUI::instructionScreen() {
    clrscr();
    GUI::clearBackground();
 
    cout << "* When you open an existing volume successfully these are functional key you should know:";

    cout << "Use ARROW 'UP' and ARROW 'DOWN' to move upward and downward between lines.\n";
    cout << "\n[ENTER]: To enter a folder.\n";
    cout << "[P]: To set / reset password for a folder or file.\n";
    cout << "\tIf a folder/file has password, it requires password to do any manipulation on it.\n";
    cout << "[I]: To import a file/folder from a given path.\n"; 
    cout << "[E]: To export a file/folder from a given path.\n"; 
    cout << "[DEL] or [D]: To delete a file or a folder\n";
    cout << "\tIf you delete a folder which has a locked file/folder inside,\n";
    cout << "\tIt won't be deleted totally and still contain the locked files/folders inside.\n";
 
    cout << "[F1]: Show this instruction.\n\n";
    cout << "  ";  system("pause");
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void GUI::printTextAtMid(string const& text, size_t const& left, size_t const& right)
{
    gotoXY((right - left - text.length()) / 2, whereY());
    cout << text;
}

void GUI::displayParent(bool selected) {
    if (selected) setColor(15, 1);

    cout << " .."; printSpace(120);

    int y = whereY();
    gotoXY(73, y);
    cout << "Folder    ";
    gotoXY(0, y + 1);

    if (selected) setColor(15, 0);
}