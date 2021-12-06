// tenthlbb.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "tenthlbb.h"
#include "CommCtrl.h"
#include "winsock.h"
#include "tchar.h"
#include "assert.h"
#include "string.h"
#include "stdlib.h"
#include "commdlg.h"
#include <WindowsX.h>
#include "Winuser.h"
#include "wingdi.h"
#include <DLGS.H>
#include <ctime>


#define MAX_LOADSTRING 100
#define IDC_BUTTON1 2001
#define IDC_BUTTON2 2002
#define IDC_BUTTON3 2003
#define ID_CHOOSECOL 2004
#define IDD_SETTINGBOX 100
#define IDC_IPADDR  100
#define ID_EXIT 2
#define ID_STATIC 3


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
FINDREPLACE findrep;
UINT uFindMsgString = 0;
HWND ReplDialog;
char szProgName[] = "Имя программы";
COLORREF color = RGB(255, 255, 128);
HDC hdc;
PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода
DWORD dColors[3] = { 255,222,222 };
HWND Date;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TENTHLBB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TENTHLBB));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TENTHLBB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TENTHLBB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   
   HWND hCombo ,hWndIPAddr, hTest, hExit;

   if (!hWnd)
   {
      return FALSE;
   }
   else 
   {

       //Выбрать цвет - Структура CHOOSECOLOR
       hTest = CreateWindowEx(WS_EX_CLIENTEDGE, L"Button", L"Выберите цвет фона", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_DEFPUSHBUTTON,
           30, 40, 170, 45, hWnd, (HMENU)ID_CHOOSECOL, hInst, NULL);

       //Комбобокс
       hCombo = CreateWindow(L"COMBOBOX", L"combobox", WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWNLIST,
           320, 100, 100, 150, hWnd, 0, hInstance, 0);

       //Поле ввода IP-Адреса
       hWndIPAddr = CreateWindow(WC_IPADDRESS, NULL, WS_CHILD | WS_VISIBLE | ES_LEFT,
           400, 51, 150, 22, hWnd, (HMENU)IDC_IPADDR, (HINSTANCE)(LONG_PTR)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
       assert(hWndIPAddr);

       //Кнопка
       CreateWindowEx(0, L"Button", L"Just кнопка", BS_CENTER | WS_CHILD | WS_VISIBLE, 
           290, 50, 100, 25, hWnd, reinterpret_cast<HMENU>(666), hInstance, NULL);
     
       //Поиск
       CreateWindowEx(0, L"Button", L"Найти", BS_CENTER | WS_CHILD | WS_VISIBLE,
          30, 100, 170, 45, hWnd, (HMENU)IDC_BUTTON2, hInstance, NULL);

       //Подкладка для элементов
       CreateWindowEx(0, L"SysTabControl32", L"Закладка", WS_BORDER | WS_VISIBLE | WS_CHILD, 10,
           10, 700, 600, hWnd, NULL, hInstance, NULL);

      

       /*
       CreateWindowEx(0, WC_LINK,
           L"For more information, <A HREF=\"http://www.microsoft.com\">CLICKHERE</A> " \
           L"or <A ID=\"idInfo\">here</A>.",
           WS_CHILD | WS_TABSTOP | WS_GROUP,
           550, 30, 100, 25,
           hWnd, NULL, hInstance, NULL);         
       */
    
   }

   TCHAR Planets[9][10] =
   {
       TEXT("Mercury"), TEXT("Venus"), TEXT("Terra"), TEXT("Mars"),
       TEXT("Jupiter"), TEXT("Saturn"), TEXT("Uranus"), TEXT("Neptune"),
       TEXT("Pluton")
   };

   TCHAR A[8];

   memset(&A, 0, sizeof(A));
   for (int k = 0; k <= 8; k++)
   {
       wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Planets[k]);

        //Add string to combobox.
       SendMessage(hCombo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    WNDCLASS ws;
    ws.lpfnWndProc = WndProc;

    POINTS cb;

    //окна элементов управления
    CHOOSECOLOR cc;
    cc.Flags = CC_RGBINIT | CC_FULLOPEN;
    cc.hInstance = NULL;
    cc.hwndOwner = hWnd;
    cc.lCustData = 0L;
    cc.lpCustColors = dColors;
    cc.lpfnHook = NULL;
    cc.lpTemplateName = NULL;
    cc.lStructSize = sizeof(cc);
    cc.rgbResult = RGB(255, 0, 0);
    HBRUSH hBrush;

    switch (message)
    { 

    case WM_LBUTTONDBLCLK:
    {
        SYSTEMTIME st{};
        DateTime_GetSystemtime(Date, &st);
        TCHAR buf[100]{};
        wsprintf(buf, L"%d-%d-%d", st.wYear, st.wMonth, st.wDay);
        MessageBox(hWnd, buf, L"Time", 0);
       
    } 
    break;

    case WM_CHAR:
    {
        TCHAR let = wParam;
        TCHAR buff[30]{};
        wsprintf(buff, L"Вы нажали символ:%c", let);
        MessageBox(hWnd, buff, L"Системное сообщение", MB_OKCANCEL);
    }
    break;

    case WM_KEYDOWN:
    {
        TCHAR buff[30]{};
        wsprintf(buff, L"Кнопка нажата");
        SetWindowText(hWnd, buff);
        break;
    }
    break;
  

    case WM_LBUTTONDOWN:
    {
        HDC hdc;
        WORD xPos, yPos, nSize;
        TCHAR szBuf[80];
        hdc = GetDC(hWnd);
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
        nSize = wsprintf(szBuf, TEXT("(%d, %d)"), xPos, yPos);
        TextOut(hdc, xPos, yPos, szBuf, nSize);
        ReleaseDC(hWnd, hdc);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        InvalidateRect(hWnd, NULL, TRUE);      
    }
    break;

    case WM_LBUTTONUP:
    {
        SetWindowText(hWnd, L"пути");    
    }
    break;

    case WM_MOUSEMOVE:
    {      
        SetWindowText(hWnd, L"помедленнее");            
    }
    break;

    case WM_MOVE:
    {
        SetWindowText(hWnd, L"Перемести обратно");
    }
    break;

    case WM_COMMAND:
        {
        switch (LOWORD(wParam)) 
        {
            //Действие для кнопки
        case 666:
        {
            MessageBox(hWnd, L"Я кнопка бупка", L"Привет Человек!", MB_ICONINFORMATION);
        }
        break;

            //Окно выбора цвета       
        case ID_CHOOSECOL:
        {
            if (ChooseColor(&cc)) {
                color = (COLORREF)cc.rgbResult;
            }
            InvalidateRect(hWnd, NULL, 1);
        }
            break;

            //Выход из программы
        case ID_EXIT:
        {
            DestroyWindow(hWnd);
            break;
        }
        }

       
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {                      
                case IDC_BUTTON2:
                {
                    TCHAR WhatFind[100], WhatReplace[100] = L"\0";
                    memset(WhatFind, 0, _countof(WhatFind));
                    if (uFindMsgString == 0) 
                    {
                        uFindMsgString = RegisterWindowMessage(FINDMSGSTRING);
                    }
                    if (IsWindow(ReplDialog) == false) 
                    {
                        findrep.lStructSize = sizeof(FINDREPLACE);
                        findrep.hInstance = hInst;
                        findrep.hwndOwner = hWnd;
                        findrep.lpstrFindWhat = WhatFind;
                        findrep.lpstrReplaceWith = WhatReplace;
                        findrep.wFindWhatLen = 80;
                        findrep.wReplaceWithLen = _countof(WhatReplace);
                        findrep.Flags = NULL;
                        ReplDialog = FindText(&findrep);
                    }
                }
                break;
                   
                case IDM_ABOUT:
                {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                }
                    break;

                case IDM_EXIT:
                {
                    DestroyWindow(hWnd);
                }
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            RECT rect;
            GetWindowRect(hWnd, &rect);
            LONG width = rect.right - rect.left;
            LONG height = rect.bottom - rect.top;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            hBrush = CreateSolidBrush(color);
            SelectObject(hdc, hBrush);

            SetBkMode(hdc, TRANSPARENT);
            SetBkColor(hdc, color);
            Rectangle(hdc, 0, 0, width, height);
            TextOut(hdc, 20, 10, TEXT("Выберите цвет главного окна:"), 28);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
         {
        PostQuitMessage(0);
    }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
