#define IDM_LINE 3
#define IDM_TEXT 4
#define IDM_RECT 5
#define IDM_POLYLINE 6
#define IDM_POLYGON 7
#define IDM_ELLIPSE 8
#define IDM_NOTDRAW 9

#include "framework.h"
#include "WindAPI_lab1.h"

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


#define MAX_LOADSTRING 100

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];
int status = 0;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HDC hdc);
VOID MyDrawLine(HWND hwnd, POINT p);
VOID MyDrawText(HWND hwnd, POINT p);
VOID DrawPolyline(HWND hwnd, POINT p);
VOID DrawRect(HWND hwnd, POINT p);
VOID MyDrawEllipse(HWND hwnd, POINT p);
VOID MyDrawPolygon(HWND hwnd, POINT p);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDAPILAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDAPILAB1));

    MSG msg;

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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDAPILAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDAPILAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

VOID InitMenu(HWND hWnd)
{
    HMENU menu = GetMenu(hWnd);
    AppendMenuW(menu, MF_STRING, IDM_LINE, L"&Line");
    AppendMenuW(menu, MF_STRING, IDM_POLYLINE, L"&Polyline");
    AppendMenuW(menu, MF_STRING, IDM_RECT, L"&Rect");
    AppendMenuW(menu, MF_STRING, IDM_POLYGON, L"&Polygon");
    AppendMenuW(menu, MF_STRING, IDM_TEXT, L"&Text");
    AppendMenuW(menu, MF_STRING, IDM_ELLIPSE, L"&Ellipse");      
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   InitMenu(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_RECT:
                status=IDM_RECT;
                break;
            case IDM_LINE:
               status=IDM_LINE;
                break;            
            case IDM_POLYGON:
               status=IDM_POLYGON;
                break;
            case IDM_POLYLINE:
                status=IDM_POLYLINE;
                break;
            case IDM_TEXT:
                status=IDM_TEXT;
                break;
            case IDM_ELLIPSE:
                status=IDM_ELLIPSE;
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        {
            if (status == 0)
                break;
            POINT p;
            p.x = GET_X_LPARAM(lParam);
            p.y = GET_Y_LPARAM(lParam);
            switch (status)
            {
                case IDM_LINE:
                    MyDrawLine(hWnd, p);
                    break;                
                case IDM_POLYLINE:
                    DrawPolyline(hWnd, p);
                    break;               
                case IDM_POLYGON:
                    MyDrawPolygon(hWnd, p);
                    break;                
                case IDM_ELLIPSE:
                    MyDrawEllipse(hWnd, p);
                    break;                
                case IDM_TEXT:
                    MyDrawText(hWnd, p);
                    break;                
                case IDM_RECT:
                    DrawRect(hWnd, p);
                    break;
            }
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            OnPaint(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Pen      pen(Color(0, 0, 0));
    SolidBrush solidBrush(Color(0, 0, 0));

    RectF rect(0.0f, 0.0f, 200.0f, 200.0f);
    RectF rect1(180.0f, 200.0f, 200.0f, 150.0f);

    WCHAR string[] = L"Hello World";
    Font myFont(L"Anoxic Light", 48);
    PointF origin(300.0f, 50.0f);

    Point points[6] = {
                        Point(400,500),
                        Point(400,400),
                        Point(500,300),
                        Point(600,300),
                        Point(700,400),
                        Point(700,500)
    };
    
    
    for (int i=0;i<4;i++)
        graphics.DrawLine(&pen, points[i].X+260, points[i].Y-200, points[i+1].X+260, points[i+1].Y-200);
    graphics.DrawPolygon(&pen, points, 6);
    graphics.DrawString(string, 11, &myFont, origin, &solidBrush);
    graphics.DrawRectangle(&pen, rect1);
    graphics.FillRectangle(&solidBrush, rect1);
    graphics.DrawEllipse(&pen, rect);
    graphics.FillEllipse(&solidBrush, rect);
    graphics.DrawLine(&pen, 400, 150, 300, 100);
}

VOID MyDrawLine(HWND hWnd, POINT p)
{
    HDC hdc = GetDC(hWnd);
    Graphics gr(hdc);
    Pen      pen(Color(255, 0, 0));
    gr.DrawLine(&pen, p.x, p.y, 200, 50);
    ReleaseDC(hWnd, hdc);
}

VOID MyDrawPolygon(HWND hWnd, POINT p)
{
    HDC hdc = GetDC(hWnd);
    Graphics gr(hdc);
    Pen      pen(Color(255, 0, 0));
    Point points[6] = {
                    Point(p.x,p.y),
                    Point(p.x, p.y-100),
                    Point(p.x + 100,p.y - 200),
                    Point(p.x + 200,p.y - 200),
                    Point(p.x + 300,p.y - 100),
                    Point(p.x + 300,p.y)
    };
    gr.DrawPolygon(&pen, points, 6);
    ReleaseDC(hWnd, hdc);
}

VOID MyDrawText(HWND hwnd, POINT p)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Pen      pen(Color(255, 0, 0));
    SolidBrush solidBrush(Color(0, 0, 0));
    WCHAR string[] = L"AAA";
    Font myFont(L"Arial", 18);
    PointF origin(p.x, p.y);

    gr.DrawString(string, 11, &myFont, origin, &solidBrush);
    ReleaseDC(hwnd, hdc);
}

VOID DrawPolyline(HWND hwnd, POINT p)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Pen      pen(Color(255, 0, 0));
    Point points[4] = {
                        Point(p.x,p.y),
                        Point(p.x + 50,p.y),
                        Point(p.x+50,p.y+30),
                        Point(p.x + 100,p.y + 30)
    };
    for (int i = 0; i < 3; i++)
        gr.DrawLine(&pen, points[i].X , points[i].Y , points[i + 1].X , points[i + 1].Y );
}

VOID DrawRect(HWND hwnd, POINT p)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Pen      pen(Color(255, 0, 0));
    Rect r(p.x, p.y, 100, 50);
    gr.DrawRectangle(&pen, r);
    ReleaseDC(hwnd, hdc);
}

VOID MyDrawEllipse(HWND hwnd, POINT p)
{
    HDC hdc = GetDC(hwnd);
    Graphics gr(hdc);
    Pen      pen(Color(255, 0, 0));
    Rect r(p.x, p.y, 50, 50);
    gr.DrawEllipse(&pen, r);
    ReleaseDC(hwnd, hdc);
}
