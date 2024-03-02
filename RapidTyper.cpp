#include <iostream>
#include <Windows.h>
#include <map>

void PressKey(WORD key, bool press) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = press ? 0 : KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void typeText(const std::string &text, int delay) {
    std::map<char, std::pair<WORD, bool>> symbolMap = {
        // Pair: Virtual keycode, Shift required
        {'!', {0x31, true}},
        {'@', {0x32, true}},
        {'#', {0x33, true}},
        {'$', {0x34, true}},
        {'%', {0x35, true}},
        {'^', {0x36, true}},
        {'&', {0x37, true}},
        {'*', {0x38, true}},
        {'(', {0x39, true}},
        {')', {0x30, true}},
        {'_', {0xBD, true}},
        {'+', {0xBB, true}},
        {'{', {0xDB, true}},
        {'}', {0xDD, true}},
        {':', {0xBA, true}},
        {'"', {0xDE, true}},
        {'<', {0xBC, true}},
        {'>', {0xBE, true}},
        {'?', {0xBF, true}},
        // The rest of the symbols that don't require shift
        {'-', {0xBD, false}},
        {'=', {0xBB, false}},
        {'[', {0xDB, false}},
        {']', {0xDD, false}},
        {'\\', {0xDC, false}},
        {';', {0xBA, false}},
        {'\'', {0xDE, false}},
        {',', {0xBC, false}},
        {'.', {0xBE, false}},
        {'/', {0xBF, false}},
        {'`', {0xC0, false}},
    };

    for (char c : text) {
        bool needsShift = isupper(c) || (symbolMap.find(c) != symbolMap.end() && symbolMap[c].second);
        WORD keycode = 0;

        if (symbolMap.find(c) != symbolMap.end()) {
            keycode = symbolMap[c].first;
        } else {
            keycode = VkKeyScan(c) & 0xFF; // Low-order byte contains the virtual-key code
        }

        if (needsShift) {
            PressKey(VK_SHIFT, true); // Press Shift key
        }

        PressKey(keycode, true);  // Press the character key
        PressKey(keycode, false); // Release the character key

        if (needsShift) {
            PressKey(VK_SHIFT, false); // Release Shift key
        }

        Sleep(delay);
    }
}


void MainTyper(int delay,bool loop,bool skip){
    std::string textToType;
    std::cout << "Enter the text to be typed: ";
    if(!skip){
        std::cin.ignore();
    }
    std::getline(std::cin, textToType);

    if(loop){
        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0) != 0) {
            if (msg.message == WM_HOTKEY) {
                    std::cout << "Hotkey pressed , Loop enabled -> Typing in Loop Until you press ctrl + c ! \n";
                    while(true){
                        typeText(textToType, delay);
                    }
            }    
        }
    }
    else{
        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0) != 0) {
            if (msg.message == WM_HOTKEY) {
                std::cout << "Hotkey pressed. Typing...\n";
                typeText(textToType, delay);
                MainTyper(delay,loop,true);
            }
        }    
    }
}

int main() {
    std::cout << "Welcome to RapidTyper !" << std::endl;
    std::cout << "Version : 1.0" << std::endl;
    std::cout << "Author : https://github.com/hmnhghprst" << std::endl;
    std::cout << "After config and entering text press F11 for start typing (Hotkey)" << std::endl;
    std::cout << "----      choose your config      ----" << std::endl;

    RegisterHotKey(NULL, 1, 0, VK_F11);

    // Get delay between characters from user
    std::cout << "\nChoose delay between characters (in milliseconds): ";
    int delay;
    std::cin >> delay;

    // Ask if user wants loop
    char choice;
    std::cout << "Do you want to loop the typing? (y/n): ";
    std::cin >> choice;

    bool loop = (choice == 'y' || choice == 'Y');

    MainTyper(delay,loop,false);

    // Unregister hotkey
    UnregisterHotKey(NULL, 1);

    return 0;
}
