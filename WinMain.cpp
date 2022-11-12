//1.윈도우클래스등록
//2.윈도우생성
//3.메시지루프
//4.메시지프로시져
#include<sstream>
#include"GameLogic.h"


#pragma comment(lib,"Gdiplus.lib")

const wchar_t gClassName[]{ L"MyWindowClass" };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

solitaire::GameLogic gLogic;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register window class!", L"Error", MB_OK);
		return 0;
	}

	RECT wr{ 0,0,1024,768 };
	AdjustWindowRect(&wr, WS_OVERLAPPED | WS_SYSMENU, FALSE);

	HWND hWnd = CreateWindowEx(
		0,
		gClassName,
		L"Solitair Game",
		WS_OVERLAPPED | WS_SYSMENU,
		0, 0,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create window!", L"Error", MB_OK);
		return 0;
	}

	gLogic.Init(hWnd);


	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gLogic.Release();
	
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);

	//그리기
	Gdiplus::Graphics graphics(hdc);

	gLogic.Draw(graphics);


	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONUP:
			gLogic.OnClick(LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_PAINT:
			OnPaint(hwnd);
			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}


