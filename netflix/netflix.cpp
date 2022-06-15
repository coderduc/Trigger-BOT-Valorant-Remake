#pragma once

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include "HSV.h"
#include "Screen.h"

#ifdef _WIN32
#include <io.h> 
#define access    _access_s
#else
#include <unistd.h>
#endif

#define INAME L"VALORANT  " 
using namespace std;

int colorMode = 1;
HWND deeznuts;
#define isDown 0x8000

int get_screen_width(void) {
    return GetSystemMetrics(SM_CXSCREEN);
}

int get_screen_height(void) {
    return GetSystemMetrics(SM_CYSCREEN);
}

struct point {
    double x;
    double y;
    point(double x, double y) : x(x), y(y) {}
};

inline bool is_color(int red, int green, int blue) {
    
    //original color purple
    if (colorMode == 0) {
        if (green >= 190) {
            return false;
        }

        if (green >= 140) {
            return abs(red - blue) <= 8 &&
                red - green >= 50 &&
                blue - green >= 50 &&
                red >= 105 &&
                blue >= 105;
        }

        return abs(red - blue) <= 13 &&
            red - green >= 60 &&
            blue - green >= 60 &&
            red >= 110 &&
            blue >= 100;
    }

    // yellow
    else {
        if (red < 160)
        {
            return false;
        }
        if (red > 161 && red < 255) {
            return green > 150 && green < 255 && blue > 0 && blue < 79;
        }
        return false;
    }
}

BYTE* screenData = 0;
bool run_threads = true;
const int screen_width = get_screen_width(), screen_height = get_screen_height();

//bot with purple (original (again not default))
void bot() {
    int w = 8, h = 10;
    auto t_start = std::chrono::high_resolution_clock::now();
    auto t_end = std::chrono::high_resolution_clock::now();

    HDC hScreen = GetDC(NULL);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    screenData = (BYTE*)malloc(5 * screen_width * screen_height);
    HDC hDC = CreateCompatibleDC(hScreen);
    point middle_screen(screen_width / 2, screen_height / 2);

    BITMAPINFOHEADER bmi = { 0 };
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = w;
    bmi.biHeight = -h;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;


    //Screen::getSize();

    //int x = Screen::width / 2;  //DONT NEED TO BE CALLED EVERY TICK, THE DESKTOP MONITOR SIZE DOESNT CHANGE!
    //int y = Screen::height / 2; //DONT NEED TO BE CALLED EVERY TICK, THE DESKTOP MONITOR SIZE DOESNT CHANGE!

    //RGBTRIPLE rgbTriple;
    //HDC desktopHdc = GetDC(NULL);

    //int addX = 0, addY = 0;

    //while (1) {
    //    int TRIGGER_SIZE = 8;

    //    if ((GetAsyncKeyState(VK_XBUTTON1) && isDown) && !(GetAsyncKeyState(VK_LBUTTON) && isDown)) { //IS PRESSING TRIGGER KEY AND ISNT PRESSING THE MOUSE 1
    //        for (int iX = 0; iX < TRIGGER_SIZE; iX++) {
    //            for (int iY = 0; iY < TRIGGER_SIZE; iY++) {
    //                if (iX >= TRIGGER_SIZE / 2) addX = ((TRIGGER_SIZE / 2) - iX); //MAKES A x RECT AROUND THE MIDDLE SCREEN
    //                if (iX < TRIGGER_SIZE / 2) addX = ((TRIGGER_SIZE / 2) - iX); //MAKES A x RECT AROUND THE MIDDLE SCREEN

    //                if (iY >= TRIGGER_SIZE / 2) addY = (iY - (TRIGGER_SIZE / 2)); //MAKES A y RECT AROUND THE MIDDLE SCREEN
    //                if (iY < TRIGGER_SIZE / 2) addY = (iY - (TRIGGER_SIZE / 2)); //MAKES A y RECT AROUND THE MIDDLE SCREEN

    //                COLORREF color = GetPixel(desktopHdc, x + addX, y + addY);

    //                rgbTriple.rgbtRed = GetRValue(color);
    //                rgbTriple.rgbtGreen = GetGValue(color);
    //                rgbTriple.rgbtBlue = GetBValue(color);

    //                Color::HSV rgbToHue = toHSV((float)rgbTriple.rgbtRed, (float)rgbTriple.rgbtGreen, (float)rgbTriple.rgbtBlue);
    //                Color::ColorName name = Categorize(rgbToHue);
    //                string cname = Color::toString(name);
    //                if (cname == "magenta") {
    //                    if (rgbToHue.s * 100 <= 80 && rgbToHue.s * 100 > 15) {
    //                        if ((GetAsyncKeyState(VK_XBUTTON1) && isDown) && !(GetAsyncKeyState(VK_LBUTTON) && isDown)) { //IS PRESSING TRIGGER KEY AND ISNT PRESSING THE MOUSE 1
    //                            Sleep(1);
    //                            PostMessage(deeznuts, WM_LBUTTONDOWN, 1, 0);
    //                            Sleep(1);
    //                            PostMessage(deeznuts, WM_LBUTTONUP, 1, 0);
    //                        }
    //                        else {
    //                            Sleep(1);
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
    
    while (run_threads) {
        Sleep(6);
        HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
        BOOL bRet = BitBlt(hDC, 0, 0, w, h, hScreen, middle_screen.x - (w/2), middle_screen.y - (h/2), SRCCOPY);
        SelectObject(hDC, old_obj);
        GetDIBits(hDC, hBitmap, 0, h, screenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w * 4; i += 4) {
                #define red screenData[i + (j*w*4) + 2]
                #define green screenData[i + (j*w*4) + 1]
                #define blue screenData[i + (j*w*4) + 0]
                if ((GetAsyncKeyState(VK_XBUTTON1) && isDown) && !(GetAsyncKeyState(VK_LBUTTON) && isDown)) { //IS PRESSING TRIGGER KEY AND ISNT PRESSING THE MOUSE 1
                    if (is_color(red, green, blue)) {
                        PostMessage(deeznuts, WM_LBUTTONDOWN, 1, 0);
                        Sleep(1);
                        PostMessage(deeznuts, WM_LBUTTONUP, 1, 0);
                        Sleep(1);
                        Sleep(100);
                    }
                }
            }
        }
    
    }
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    deeznuts = FindWindowW(NULL, INAME);
    HDC nDC = GetDC(deeznuts);
    string color;
    int mode = 0;
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    auto w_f = freopen("CON", "w", stdout);
    auto r_f = freopen("CON", "r", stdin);
    bot();
}
