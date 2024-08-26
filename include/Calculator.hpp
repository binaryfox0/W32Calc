/*
MIT License

Copyright(c) 2023 Duy Pham Duc

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#define UNICODE

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <codecvt>
#include <Windows.h>
#include <wchar.h>
#include <locale>

struct Vector2i
{
    int x, y;

    Vector2i() : x(0), y(0) {};
    Vector2i(int x, int y) : x(x), y(y) {};
};

class Calculator
{
public:
    Calculator() {};

    void SetupCalculator(HWND hWnd);
    void HandleCustomButton(LPARAM lParam);
    void ResizeCalculator(HWND hWnd);
    void HandleButtonInput(WPARAM wParam, HWND hWnd);
    void HandleKeyboardInput(WPARAM wParam);
    LRESULT ChangeStaticColor(WPARAM wParam);
    void UpdateInputbox(HWND hWnd);

private:
    const std::vector<std::vector<std::wstring>> buttonsText = {
        {L"\uE94D", L"CE", L"C", L"\uE94F"},
        {L"7",      L"8",  L"9", L"\uE94A"},
        {L"4",      L"5",  L"6", L"\uE947"},
        {L"1",      L"2",  L"3", L"\uE949"},
        {L"\uE94E", L"0",  L".", L"\uE948"}
    };
    std::vector<HWND> buttons;
    const std::unordered_map<wchar_t, wchar_t> UnicodeMap =
    {
        { L'\uE948', L'+' }, // Calculator Addition
        { L'\uE949', L'-' }, // Calculator Subtract
        { L'\uE947', L'*' }, // Calculator Multiply
        { L'\uE94A', L'/' }, // Calculator Divide
    };

    const int DEFAULT_SIZE = 32;
    const int BUTTON_SPACING = 10;

    HWND inputBox;
    std::wstring input = L"0", prevInput, answer;

    HFONT GENERATE_FONT(int FontSize);
    Vector2i BOARD_POS(int height);

    // Helper function
    bool Filter(const std::vector<std::wstring>& filter, const wchar_t* target);
    std::wstring TranslateUnicode(std::wstring wstr);
    void InputParser(std::wstring input, double& num1, double& num2, wchar_t& op);
    double CalculateInput(std::wstring input);
    void NegateNumber(std::wstring& input);
    void EraseFinalNumber(std::wstring& input);

    // Converter function
    std::string wstr2str(std::wstring wstr);
    std::wstring str2wstr(std::string str);
    std::wstring double2wstr(double number);
};

#endif