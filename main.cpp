#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <codecvt>
#include <locale>
#include <algorithm>

using namespace std;

vector<pair<HWND, string>> windows = vector<pair<HWND, string>>();

string GetText(HWND hWnd)
{
    int length = GetWindowTextLength(hWnd);
    char *buffer = new char[length + 1];
    GetWindowText(hWnd, buffer, length + 1);
    return (string)buffer;
}

string GetClass(HWND hWnd)
{
    char *buffer = new char[1024];
    RealGetWindowClass(hWnd, buffer, 1024);
    return (string)buffer;
}

bool CheckWindow(HWND hWnd, string title)
{
    if (!IsWindowVisible(hWnd))
    {
        return false;
    }
    if (title.length() == 0)
    {
        return false;
    }
    if (GetParent(hWnd) != 0)
    {
        return false;
    }
    string windowClass = GetClass(hWnd);
    if (windowClass == "Windows.UI.Core.CoreWindow" || windowClass == "ApplicationFrameWindow" || windowClass == "Progman")
    {
        return false;
    }
    if(GetConsoleWindow() == hWnd)
    {
        return false;
    }
    return true;
}

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam)
{
    string windowTitle = GetText(hWnd);

    if (CheckWindow(hWnd, windowTitle))
    {
        windows.push_back(make_pair(hWnd, windowTitle));
    }
    return TRUE;
}

string x(string s, int times)
{
    string output = "";
    for (int i = 0; i < times; i++)
    {
        output += s;
    }
    return output;
}

bool IsTopmost(HWND hWnd)
{
    LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
    return style & WS_EX_TOPMOST;
}

void ListWindows()
{
    int maxIndex = 0;
    int maxText = 0;
    int size = windows.size();
    int *indexLengths = new int[size];
    int *textLengths = new int[size];
    for (int i = 0; i < size; i++)
    {
        pair<HWND, string> window = windows[i];
        int indexLength = to_string(i + 1).length();
        if(IsTopmost(window.first))
        {
            indexLength += 1;
        }
        indexLengths[i] = indexLength;
        if (indexLength > maxIndex)
        {
            maxIndex = indexLength;
        }
        int textLength = window.second.length();
        textLengths[i] = textLength;
        if (textLength > maxText)
        {
            maxText = textLength;
        }
    }
    maxText = min(maxText, 50);
    string lt = " ¢z¢w";
    string lb = " ¢|¢w";
    string rt = "¢w¢{";
    string rb = "¢w¢}";
    string lh = "¢w";
    string lv = " ¢x ";
    string cross = "¢w¢q¢w";
    string lc = " ¢u¢w";
    string rc = "¢w¢t";
    string tc = "¢w¢s¢w";
    string bc = "¢w¢r¢w";
    string space = " ";
    cout << lt << x(lh, maxIndex) << tc << x(lh, maxText) << rt << endl;
    for (int i = 0; i < size; i++)
    {
        pair<HWND, string> window = windows[i];
        int indexLength = indexLengths[i];
        int textLength = textLengths[i];
        if(IsTopmost(window.first))
        {
            cout << lv << x(space, maxIndex - indexLength - 1) << "*" << i + 1 << lv << window.second << x(space, maxText - textLength) << lv << endl;
        }
        else
        {
            cout << lv << x(space, maxIndex - indexLength) << i + 1 << lv << window.second << x(space, maxText - textLength) << lv << endl;
        }
        if (i != size - 1)
        {
            cout << lc << x(lh, maxIndex) << cross << x(lh, maxText) << rc << endl;
        }
    }
    cout << lb << x(lh, maxIndex) << bc << x(lh, maxText) << rb << endl;
}

void SetTopmost(HWND hWnd)
{
    ShowWindow(hWnd, SWP_SHOWWINDOW);
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
void SetNoTopmost(HWND hWnd)
{
    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void SwitchInput()
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event(VK_SPACE, 0, 0, 0);
    keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}

bool compare(pair<HWND, string> win1, pair<HWND, string> win2)
{
    return win1.second > win2.second;
}

int main()
{
    SetConsoleTitleA("¸m³»µe­±.exe");
    HWND cmd = GetConsoleWindow();
    SwitchInput();
    EnumWindows(enumWindowCallback, 0);
    sort(windows.begin(), windows.end(), compare);
    while(true)
    {
        SetTopmost(cmd);
        system("cls");
        ListWindows();
        string mode;
        int index;
        cout << "  pin/unpin [id]> ";
        cin >> mode;
        if (mode == "exit")
        {
            break;
        }
        cin >> index;
        HWND hWnd = windows[index - 1].first;
        if (mode == "pin")
        {
            SetTopmost(hWnd);
        }
        else if (mode == "unpin")
        {
            SetNoTopmost(hWnd);
        }
    }
    SetNoTopmost(cmd);
    return 0;
}