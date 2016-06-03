#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <Windowsx.h>
#include "Model.h"
/// Global variables
/// The main window.
static TCHAR szWindowClass[] = _T("win32app");
/// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("EmAzing Valleyball V4.2.1 Коваленко Владислав");
HINSTANCE hInst;///< istance
HBITMAP hBitmap = NULL;///< bitmap
WCHAR n[30];///< char array
/// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#define VY_MAX 250 ///< max Y speed
#define G 150 ///< acceleration of gravity
#define BALL_COUNT 1 ///< The amount generated balls
#define P_SIZE 34 ///< player size
#define P1_POSITION 500 ///< 1-st player start position
#define P1_V_MAX 300 ///< 1-st player max speed
#define P1_A 400 ///< 1-st player acceleration
#define WINDOW_WIDTH 570 ///< display high
#define WINDOW_WIDTH2 1170 ///< display width
bool pause = false;///< gamepause
int s1 = 0;///< 1-st player save counter
int s2 = 0;///< 2-nd player save counter
int Player1Score = 0;///< 1-st player game score
int Player2Score = 0;///< 2-nd player game score
int p1w = 0;///< 1-st player win counter
int p2w = 0;///< 2-nd player win counter
int anrialMode = 1;///< bot player game counter
int gs = 0;///< game save counter
int gl = 0;///< game load counter
Player1 p1;///< 1-st player object initialization
Player2 p2;///< 2-nd player object initialization
Ball ball[BALL_COUNT];///< ball array
void DoPhysics(ULONGLONG dT);///< run physics
void StartGame();///< run game
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }
    hInst = hInstance; ///< Store instance handle in our global variable
    /** The parameters to CreateWindow explained:
     * szWindowClass: the name of the application
     * szTitle: the text that appears in the title bar
     * WS_OVERLAPPEDWINDOW: the type of window to create
     * CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
     * 500, 100: initial size (width, length)
     * NULL: the parent of this window
     * NULL: this application does not have a menu bar
     * hInstance: the first parameter from WinMain
     * NULL: not used in this application
	 */
		HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1200, 650,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);
        return 1;
    }
    /// The parameters to ShowWindow explained:
    /// hWnd: the value returned from CreateWindow
    /// nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);
    MSG msg;
	ULONGLONG prevTime = -1, curTime, deltaT;
	StartGame();
/// Enter the infinite message loop
while(TRUE)
{

	/**
	*	\brief Check to see if any messages are waiting in the queue
	*/
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      
		/**
		*	\brief Translate the message and dispatch it to WindowProc()
		*/
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
	/**
	*	\brief If the message is WM_QUIT, exit the while loop
	*/
    if(msg.message == WM_QUIT)
        break;
	curTime = GetTickCount64();
	if (prevTime == -1)
		deltaT = 0;
	else deltaT = curTime - prevTime;
	prevTime = curTime;
	if (!pause)
	{
		DoPhysics(deltaT);
	}
	InvalidateRect(hWnd,NULL,1);
}
    return (int) msg.wParam;
}
/*!
\brief Start game function

	initialized all need params
*/
void StartGame(){
	srand(GetTickCount());
	RECT r = {100,100,700,400};
	for (int i=0;i<BALL_COUNT;i++)
	{
		ball[i] = CreateBall(r);
	}
	p1.vx = 0;
	p1.vy = 0;
	p1.y = 570;
	p1.X = 400;
	p1.a = 0;
	p1.v = 0;
	p2.z = 800;
	p2.t = 570;	
}
/*!
\brief Game physics function

Engine of game
*/
void DoPhysics(ULONGLONG dT)
{
	RECT r = { 100, 100, 700, 400 };
	
	for (int i = 0; i < BALL_COUNT; i++)
	{
		ball[i].X += ((double)dT / 1000)*ball[i].dX;
		ball[i].Y += ((double)dT / 1000)*ball[i].dY;
		ball[i].dY += ((double)dT / 1000)*G;
		/**
		*	\brief Conditions repulsion of the ball on the wall
		*
		*	The condition code is as follows:
		*	\code
		*	if ((ball[i].X - ball[i].r < 0) && (ball[i].dX < 0) || (ball[i].X + ball[i].r> 1200) && (ball[i].dX>0))
		*	{
		*		ball[i].dX = -1 * ball[i].dX;
		*	}
		*	\endcode
		*/
		if ((ball[i].X - ball[i].r < 0) && (ball[i].dX < 0) || (ball[i].X + ball[i].r> 1200) && (ball[i].dX>0))
		{
			ball[i].dX = -1 * ball[i].dX;
		}
		/**
		*	\brief Conditions repulsion of the ball on the ceiling
		*
		*	The condition code is as follows:
		*	\code
		*	if (ball[i].Y - ball[i].r < 0){
		*		ball[i].dY = -1 * ball[i].dY;
		*	}
		*	\endcode
		*/
		if (ball[i].Y - ball[i].r < 0){
			ball[i].dY = -1 * ball[i].dY;
		}
		/**
		*	\brief Conditions repulsion of the ball on the grid
		*
		*	The condition code is as follows:
		*	\code
		*	if ((ball[i].Y < 600) && (ball[i].Y>300)) {
		*		if (ball[i].X > 560 && ball[i].X < 640)
		*			ball[i].dX = -1 * ball[i].dX;
		*	}
		*	\endcode
		*/
		if ((ball[i].Y < 600) && (ball[i].Y>300)) {
			if (ball[i].X > 560 && ball[i].X < 640)
				ball[i].dX = -1 * ball[i].dX;
		}
		/**
		*	\brief Conditions for the outlet of the ball area
		*
		*	The condition code is as follows:
		*	\code
		*	if (ball[i].Y>600 && ball[i].X < 599){
		*		Player2Score++;
		*		ball[i] = CreateBall(r);
		*	}
		*	if (ball[i].Y>600 && ball[i].X > 601){
		*		Player1Score++;
		*		ball[i] = CreateBall(r);
		*		ball[i].dX = -1 * ball[i].dX;
		*	}
		*	
		*	\endcode
		*/
		if (ball[i].Y>600 && ball[i].X < 599){
			Player2Score++;
			ball[i] = CreateBall(r);
		}
		if (ball[i].Y>600 && ball[i].X > 601){
			Player1Score++;
			ball[i] = CreateBall(r);
			ball[i].dX = -1 * ball[i].dX;
		}
		
		if (ball[i].Y < -100){
			ball[i] = CreateBall(r);
		}
		double dx = ball[i].X - p1.X;
		double dy = ball[i].Y - p1.y;
		double dz = ball[i].X - p2.z;
		double dt = ball[i].Y - p2.t;
		/**
		*	\brief logic of clash ball with 2-nd player
		*
		*	The condition code is as follows:
		*	\code
		*	if (sqrt(dx*dx + dy*dy) <= ball[i].r + P_SIZE)
		*	{
		*		ball[i].dX = 60 + (p1.v / 4);
		*		ball[i].dY = -255 + (p1.vy / 4);			
		*	}	
		*	\endcode
		*/
		if (sqrt(dx*dx + dy*dy) <= ball[i].r + P_SIZE)
		{
			ball[i].dX = 60 + (p1.v / 4);
			ball[i].dY = -255 + (p1.vy / 4);			
		}		
		/**
		*	\brief logic of clash ball with 2-nd player
		*
		*	The condition code is as follows:
		*	\code
		*	if (sqrt(dz*dz + dt*dt) <= ball[i].r + P_SIZE)
		*	{
		*		ball[i].dX = -60 + (p2.q / 4);
		*		ball[i].dY = -255+ (p2.vt /4);			
		*	}	
		*	\endcode
		*/
		if (sqrt(dz*dz + dt*dt) <= ball[i].r + P_SIZE)
		{
			ball[i].dX = -60 + (p2.q / 4);
			ball[i].dY = -255+ (p2.vt /4);			
		}				
		
		if (anrialMode > 0){		
			if (ball[i].X > 600){
				/**
				*	\brief bot 2 game logic
				*
				*	The condition code is as follows:
				*	\code
				*	p2.z = ball[i].X;
				*		if ((ball[i].X > 1050) && (ball[i].X < 1150)){
				*			if (p2.t >= 550){
				*				p2.vt = -400;
				*			}
				*		}
				*		if ((ball[i].X > 650) && (ball[i].X < 750)){
				*			if (p2.t >= 550){
				*				p2.vt = -400;
				*			}
				*		}
				*	}
				*	\endcode
				*/
				p2.z = ball[i].X;
				if ((ball[i].X > 1050) && (ball[i].X < 1150)){
					if (p2.t >= 550){
						p2.vt = -400;
					}
				}
				if ((ball[i].X > 650) && (ball[i].X < 750)){
					if (p2.t >= 550){
						p2.vt = -400;
					}
				}
			}
			/**
			*	\brief bot 1 game logic
			*
			*	The condition code is as follows:
			*	\code
			*	if (ball[i].X < 600){
			*		p1.X = ball[i].X;
			*		if ((ball[i].X > 50) && (ball[i].X < 250)){
			*			if (p1.y >= 550){
			*				p1.vy = -400;
			*			}
			*		}
			*		if ((ball[i].X > 450) && (ball[i].X < 550)){
			*			if (p1.y >= 550){
			*				p1.vy = -400;
			*			}
			*		}
			*	}
			*	\endcode
			*/
			if (ball[i].X < 600){
				p1.X = ball[i].X;
				if ((ball[i].X > 50) && (ball[i].X < 250)){
					if (p1.y >= 550){
						p1.vy = -400;
					}
				}
				if ((ball[i].X > 450) && (ball[i].X < 550)){
					if (p1.y >= 550){
						p1.vy = -400;
					}
				}
			}
		}
		else
		{
			p1w = 0;
			p2w = 0;
		}
		/**
		*	\brief start game, ball generated
		*
		*	The condition code is as follows:
		*	\code
		*	if (anrialMode == 0){
		*		p1.x = 400;
		*		p2.z = 800;
		*		ball[i] = CreateBall(r);
		*		anrialMode = -1;
		*	}
		*	\endcode
		*/
		if (anrialMode == 0){
			p1.x = 400;
			p2.z = 800;
			ball[i] = CreateBall(r);
			anrialMode = -1;
		}
		/**
		*	\brief fix grid
		*
		*	The condition code is as follows:
		*	\code
		*	if ((ball[i].X >560 && ball[i].X < 630) && (ball[i].Y >300 && ball[i].Y < 330) && (ball[i].dY>0)){
		*		ball[i].dY = (ball[i].dY - 50) * -1;
		*	}
		*	\endcode
		*/
		if ((ball[i].X >560 && ball[i].X < 630) && (ball[i].Y >300 && ball[i].Y < 330) && (ball[i].dY>0)){
				ball[i].dY = (ball[i].dY - 50) * -1;
		}
		/**
		*	\brief 1-st player jump logic
		*
		*	The code is as follows:
		*	\code
		*	p1.y += ((double)dT / 1000)*p1.vy;
		*	p1.vy += ((double)dT / 1000)*3*G;
		*	if (p1.vy > VY_MAX)
		*		p1.vy = VY_MAX;
		*	if (p1.y + P_SIZE > 600){
		*		p1.vy = 0;
		*	}
		*	p1.X += ((double)dT / 1000)*p1.v;
		*	if (p2.z > (WINDOW_WIDTH2-15)){
		*		p2.z = (WINDOW_WIDTH2-15);
		*		p2.q = 0;
		*	}
		*	if (p1.X > (WINDOW_WIDTH - 12))
		*	{
		*		p1.X = (WINDOW_WIDTH - 12);
		*		p1.v = 0;
		*	}
		*	if (p1.X < 30)
		*	{
		*		p1.X = 30;
		*		p1.v = 0;
		*	}
		*	if (p2.z < 642)
		*	{
		*		p2.z = 642;
		*		p2.q = 0;
		*	}
		*	if (p1.X < 0)
		*	{
		*		p1.X = 0;
		*		p1.v = 0;
		*	}
		*	p1.v += ((double)dT / 1000)*p1.a;
		*	if (p1.v > P1_V_MAX)
		*		p1.v = P1_V_MAX;
		*	if (p1.v < -1 * P1_V_MAX)
		*		p1.v = -1 * P1_V_MAX;
		*	\endcode
		*/
		p1.y += ((double)dT / 1000)*p1.vy;
		p1.vy += ((double)dT / 1000)*3*G;
		if (p1.vy > VY_MAX)
			p1.vy = VY_MAX;
		if (p1.y + P_SIZE > 600){
			p1.vy = 0;
		}
		p1.X += ((double)dT / 1000)*p1.v;
		if (p2.z > (WINDOW_WIDTH2-15)){
			p2.z = (WINDOW_WIDTH2-15);
			p2.q = 0;
		}
		if (p1.X > (WINDOW_WIDTH - 12))
		{
			p1.X = (WINDOW_WIDTH - 12);
			p1.v = 0;
		}
		if (p1.X < 30)
		{
			p1.X = 30;
			p1.v = 0;
		}
		if (p2.z < 642)
		{
			p2.z = 642;
			p2.q = 0;
		}
		if (p1.X < 0)
		{
			p1.X = 0;
			p1.v = 0;
		}
		p1.v += ((double)dT / 1000)*p1.a;
		if (p1.v > P1_V_MAX)
			p1.v = P1_V_MAX;
		if (p1.v < -1 * P1_V_MAX)
			p1.v = -1 * P1_V_MAX;
		/**
		*	\brief 2-nd player jump logic
		*
		*	The code is as follows:
		*	\code
		*	p2.t += ((double)dT / 1000)*p2.vt;
		*	p2.vt += ((double)dT / 1000)*3*G;
		*	if (p2.vt > VY_MAX)
		*		p2.vt = VY_MAX;
		*	if (p2.t + P_SIZE > 600){
		*		p2.vt = 0;
		*	}
		*	p2.z += ((double)dT / 1000)*p2.q;
		*	if (p2.z < 630)
		*	{
		*		p2.z = 630;
		*		p2.q = 0;
		*	}
		*	if (p2.z < 0)
		*	{
		*		p2.z = 0;
		*		p2.q = 0;
		*	}
		*	p2.q += ((double)dT / 1000)*p2.l;
		*	if (p2.q > P1_V_MAX){
		*		p2.q = P1_V_MAX;
		*	}
		*	if (p2.q < -1 * P1_V_MAX){
		*		p2.q = -1 * P1_V_MAX;
		*	}
		*	\endcode
		*/
		p2.t += ((double)dT / 1000)*p2.vt;
		p2.vt += ((double)dT / 1000)*3*G;
		if (p2.vt > VY_MAX)
			p2.vt = VY_MAX;
		if (p2.t + P_SIZE > 600){
			p2.vt = 0;
		}
		p2.z += ((double)dT / 1000)*p2.q;
		if (p2.z < 630)
		{
			p2.z = 630;
			p2.q = 0;
		}
		if (p2.z < 0)
		{
			p2.z = 0;
			p2.q = 0;
		}
		p2.q += ((double)dT / 1000)*p2.l;
		if (p2.q > P1_V_MAX){
			p2.q = P1_V_MAX;
		}
		if (p2.q < -1 * P1_V_MAX){
			p2.q = -1 * P1_V_MAX;
		}
	}	
}
/**
*	\brief Draw initialised function
*
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int savedDC;
	HDC backbuffDC;
	int width, height;
	int wmId, wmEvent; 
	HBITMAP backbuffer;
	HBRUSH hBrush;
	int xPos, yPos;
	double dx, dy;
	HPEN hPen, hOldPen;
	double alpha, pieX, pieY;
	int r;
	switch (message)
	{
	case WM_CREATE: //< load picture to background	
		hBitmap = (HBITMAP)LoadImage(hInst, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);		
		break;
	case WM_KEYUP: //< Keyup case
		switch (wParam)
		{
		case 'A':
			p1.a = 0;
			break;
		case 'D':
			p1.a = 0;
			break;
		case 'H':
			p2.l = 0;
			break;
		case 'L':
			p2.l = 0;
			break;
		case VK_F5: //< save game logic
			gs = 0;
			break;
		case VK_F8: //< run save game logic
			p1w = 0;
			p2w = 0;
			gl = 0;
			break;
		}
		break;
	case WM_KEYDOWN: //< Keydown case
		switch (wParam)
		{
		case VK_F5: //< save game
			s1 = Player1Score;
			s2 = Player2Score;
			gs = 1;
			break;
		case VK_F8: //< Start save game
			anrialMode = 1;
			Player1Score = s1;
			Player2Score = s2;
			gl = 1;
			break;
		case 'A':
			p1.a = -1 * P1_A;
			break;
		case 'H':
			p2.l = -1 * P1_A;
			break;
		case 'D':
			p1.a = P1_A;
			break;
		case 'L':
			p2.l = P1_A;
			break;
		case 'U': 
			Player1Score++;
			break;
		case 'Y': 
			Player2Score++;
			break;
		case 'P': //< Bot ON logic
			anrialMode = 1;
			break;
		case 'O': //< Bot OFF logic
			if (anrialMode == 1){
				anrialMode = 0; 
			}
			break;
		case ' ':	
			if (p1.y >= 550){
				p1.vy = -400;
			}
			break;
		case 'M':
			if (p2.t >= 550){
				p2.vt = -400;
			}
			break;
		case 'I':
			pause = !(pause);
			break;
		}
		InvalidateRect(hWnd, NULL, 1);
		break;
	case WM_PAINT://< Display case
		
		RECT rect;
		BITMAP 			bitmap;
		HDC 			hdcMem;
		HGDIOBJ 		oldBitmap;
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		width = rect.right;
		height = rect.bottom;
		backbuffDC = CreateCompatibleDC(hdc);
		backbuffer = CreateCompatibleBitmap(hdc, width, height);
		savedDC = SaveDC(backbuffDC);
		SelectObject(backbuffDC, backbuffer);		
		/**
		*	\brief Clear window
		*
		*	The code is as follows:
		*	\code
		*	hBrush = CreateSolidBrush(RGB(255, 255, 255));
		*	FillRect(backbuffDC, &rect, hBrush);
		*	DeleteObject(hBrush);
		*	hdcMem = CreateCompatibleDC(hdc);
		*	oldBitmap = SelectObject(hdcMem, hBitmap);
		*	\endcode
		*/
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(backbuffDC, &rect, hBrush);
		DeleteObject(hBrush);
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);
		/**
		*	\brief Displays the background image
		*
		*	The code is as follows:
		*	\code
		*	GetObject(hBitmap, sizeof(bitmap), &bitmap);		
		*	BitBlt(backbuffDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		*	\endcode
		*/
		GetObject(hBitmap, sizeof(bitmap), &bitmap);		
		BitBlt(backbuffDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		/**
		*	\brief Displays the Ball
		*	
		*	\param BALL_COUNT say how many balls you need to draw 
		*
		*	This block of code create a pen, draw ellipse and fills it in green. 
		*
		*	The code is as follows:
		*	\code
		*	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		*	hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		*	Ellipse(backbuffDC, p1.X - P_SIZE, p1.y - P_SIZE, p1.X + P_SIZE, p1.y + P_SIZE);
		*	SelectObject(backbuffDC, hOldPen);
		*	DeleteObject(hPen);
		*	hPen = CreatePen(PS_SOLID, P_SIZE, RGB(255, 0, 0));
		*	hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		*	Ellipse(backbuffDC, p1.X - (P_SIZE / 2), p1.y - (P_SIZE / 2), p1.X + (P_SIZE / 2), p1.y + (P_SIZE / 2));
		*	SelectObject(backbuffDC, hOldPen);
		*	DeleteObject(hPen);
		*	\endcode
		*/
		for (int i = 0; i < BALL_COUNT; i++)
		{
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
			Ellipse(backbuffDC, ball[i].X - ball[i].r, ball[i].Y - ball[i].r,
				ball[i].X + ball[i].r, ball[i].Y + ball[i].r);
			SelectObject(backbuffDC, hOldPen);
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, ball[i].r, RGB(0, 255, 0));
			hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
			Ellipse(backbuffDC, ball[i].X - (ball[i].r / 2), ball[i].Y - (ball[i].r / 2),
				ball[i].X + (ball[i].r / 2), ball[i].Y + (ball[i].r / 2));
			SelectObject(backbuffDC, hOldPen);
			DeleteObject(hPen);
		}
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hOldPen = (HPEN)SelectObject(backbuffDC, hPen);	
		SelectObject(backbuffDC, hOldPen);
		DeleteObject(hPen);
		/**
		 *	\brief Displays the 1-st player
		 *
		 *	This block of code create a pen, draw ellipse and fills it in red.
		 *
		 *	The code is as follows:
		 *	\code
		 *	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		 *	hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		 *	Ellipse(backbuffDC, p1.X - P_SIZE, p1.y - P_SIZE, p1.X + P_SIZE, p1.y + P_SIZE);
		 *	SelectObject(backbuffDC, hOldPen);
		 *	DeleteObject(hPen);
		 *	hPen = CreatePen(PS_SOLID, P_SIZE, RGB(255, 0, 0));
		 *	hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		 *	Ellipse(backbuffDC, p1.X - (P_SIZE / 2), p1.y - (P_SIZE / 2), p1.X + (P_SIZE / 2), p1.y + (P_SIZE / 2));
		 *	SelectObject(backbuffDC, hOldPen);
		 *	DeleteObject(hPen);
		 *	\endcode
		 */
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		Ellipse(backbuffDC, p1.X - P_SIZE, p1.y - P_SIZE, p1.X + P_SIZE, p1.y + P_SIZE);
		SelectObject(backbuffDC, hOldPen);
		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, P_SIZE, RGB(255, 0, 0));
		hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		Ellipse(backbuffDC, p1.X - (P_SIZE / 2), p1.y - (P_SIZE / 2), p1.X + (P_SIZE / 2), p1.y + (P_SIZE / 2));
		SelectObject(backbuffDC, hOldPen);
		DeleteObject(hPen);
		/**
		 *	\brief Displays the 2-nd player
		 *
		 *	This block of code create a pen, draw ellipse and fills it in blue.
		 *
		 *	The code is as follows:
		 *	\code
		 *	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		 *	hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		 *	Ellipse(backbuffDC, p2.z - P_SIZE, p2.t - P_SIZE, p2.z + P_SIZE, p2.t + P_SIZE);
		 *	SelectObject(backbuffDC, hOldPen);
		 *	DeleteObject(hPen);
		 *	hPen = CreatePen(PS_SOLID, P_SIZE, RGB(0, 0, 255));
		 *	hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		 *	Ellipse(backbuffDC, p2.z - (P_SIZE / 2), p2.t - (P_SIZE / 2), p2.z + (P_SIZE / 2), p2.t + (P_SIZE / 2));
		 *	SelectObject(backbuffDC, hOldPen);
		 *	DeleteObject(hPen);
		 *	\endcode
		 */
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		Ellipse(backbuffDC, p2.z - P_SIZE, p2.t - P_SIZE, p2.z + P_SIZE, p2.t + P_SIZE);
		SelectObject(backbuffDC, hOldPen);
		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, P_SIZE, RGB(0, 0, 255));
		hOldPen = (HPEN)SelectObject(backbuffDC, hPen);
		Ellipse(backbuffDC, p2.z - (P_SIZE / 2), p2.t - (P_SIZE / 2), p2.z + (P_SIZE / 2), p2.t + (P_SIZE / 2));
		SelectObject(backbuffDC, hOldPen);
		DeleteObject(hPen);
		
		WCHAR s[300]; //< text size 
		/**
		 *	\brief Displays the game rules
		 *	\param anrialMode monitors bot game state
		 *
		 *	This block of code monitors bot game state
		 *	and displays game rules when bot's is playing
		 *
		 *	The condition code is as follows:
		 *	\code
		 *	if (anrialMode==1){
		 *		wsprintf(s, _T("Press 'O' to play =)"));
		 *		TextOut(backbuffDC, 550, 250, s, wcslen(s));
		 *		wsprintf(s, _T("Bot playing: on - 'P'/off - 'O'"));
		 *		TextOut(backbuffDC, 100, 100, s, wcslen(s));
		 *		wsprintf(s, _T("1 Player control: 'A' - left, 'D' - right, 'Space' - jump"));
		 *		TextOut(backbuffDC, 100, 120, s, wcslen(s));
		 *		wsprintf(s, _T("2 Player control: 'H' - left, 'L' - right, 'M' - jump"));
		 *		TextOut(backbuffDC, 100, 140, s, wcslen(s));
		 *		wsprintf(s, _T("Save - 'F5', Load - 'F8', Pause - 'I' "));
		 *		TextOut(backbuffDC, 100, 160, s, wcslen(s));
		 *
		 *	}
		 *	\endcode
		 */
		if (anrialMode==1){
			wsprintf(s, _T("Press 'O' to play =)"));
			TextOut(backbuffDC, 550, 250, s, wcslen(s));
			wsprintf(s, _T("Bot playing: on - 'P'/off - 'O'"));
			TextOut(backbuffDC, 100, 100, s, wcslen(s));
			wsprintf(s, _T("1 Player control: 'A' - left, 'D' - right, 'Space' - jump"));
			TextOut(backbuffDC, 100, 120, s, wcslen(s));
			wsprintf(s, _T("2 Player control: 'H' - left, 'L' - right, 'M' - jump"));
			TextOut(backbuffDC, 100, 140, s, wcslen(s));
			wsprintf(s, _T("Save - 'F5', Load - 'F8', Pause - 'I' "));
			TextOut(backbuffDC, 100, 160, s, wcslen(s));

		}
		/**
		*	\brief Displays the "Game saved..."
		*	\param gs monitors game save condition
		*
		*	This block of code monitors game load condition
		*	and displays state
		*
		*	The condition code is as follows:
		*	\code
		*	if (gs == 1){
		*		wsprintf(n, _T("Game save..."));
		*		TextOut(backbuffDC, 550, 100, n, wcslen(n));
		*	}
		*	\endcode
		*/
		if (gs == 1){
			wsprintf(n, _T("Game saved..."));
			TextOut(backbuffDC, 550, 100, n, wcslen(n));			
		}
		/**
		*	\brief Displays the "Game load..." 
		*	\param gl monitors game load condition
		*
		*	This block of code monitors game load condition
		*	and displays state
		*
		*	The condition code is as follows:
		*	\code
		*	if (gl == 1){
		*		wsprintf(n, _T("Game load..."));
		*		TextOut(backbuffDC, 550, 100, n, wcslen(n));
		*	}
		*	\endcode
		*/
		if (gl == 1){
			wsprintf(n, _T("Game load..."));
			TextOut(backbuffDC, 550, 100, n, wcslen(n));
		}
		/**
		*	\brief Displays the "Motivate Crab"
		*	\param Player1Score, Player2Score is player's game score
		*
		*	This block of code checks for both players score
		*	and if one of them is inferior to the other 5 
		*	points on his side of the field is drawn "Motivate Crab".
		*
		*	The condition code is as follows:
		*	\code
		*	if (Player1Score>Player2Score+5){
		*		wsprintf(s, _T("(\/)_O,,.o_(\/)"));
		*		TextOut(backbuffDC, 1050, 500, s, wcslen(s));
		*	}
		*	if (Player2Score>Player1Score + 5){
		*		wsprintf(s, _T("(\/)_O,,.o_(\/)"));
		*		TextOut(backbuffDC, 50, 500, s, wcslen(s));
		*	}
		*	\endcode
		*/
		if (Player1Score>Player2Score+5){
			wsprintf(s, _T("(\/)_O,,.o_(\/)"));
			TextOut(backbuffDC, 1050, 500, s, wcslen(s));
		}
		if (Player2Score>Player1Score + 5){
			wsprintf(s, _T("(\/)_O,,.o_(\/)"));
			TextOut(backbuffDC, 50, 500, s, wcslen(s));
		}
		/**
		*	\brief Displays the 1st player score online
		*	\param Player1Score is second player game score
		*
		*	This block of code displays the player score online
		*	
		*	The code is as follows:
		*	\code
		*	wsprintf(s, _T("Score = %d"), Player1Score);
		*	TextOut(backbuffDC, 10, 10, s, wcslen(s)); 
		*	\endcode
		*/
		wsprintf(s, _T("Score = %d"), Player1Score);
		TextOut(backbuffDC, 10, 10, s, wcslen(s)); 
		/**
		*	\brief Displays the 2nd player score online
		*	\param Player1Score is second player's game score
		*
		*
		*	This block of code displays the player's score online
		*
		*	The code is as follows:
		*	\code
		*	wsprintf(s, _T("Score = %d"), Player2Score);
		*	TextOut(backbuffDC, 1100, 10, s, wcslen(s));
		*	\endcode
		*/
		wsprintf(s, _T("Score = %d"), Player2Score);
		TextOut(backbuffDC, 1100, 10, s, wcslen(s));
		/**
		*	\brief Conclusion inscriptions informing about pressing pause
		*	\param pause checks pressed pause 
		*
		*
		*	This condition checks whether a pause is pressed
		*	and when it is clicked displays the inscription "Pause..."
		*
		*	The condition code is as follows:
		*	\code
		*	if (pause){
		*		wsprintf(s, _T("Pause..."));
		*		TextOut(backbuffDC, 580, 200, s, wcslen(s));
		*	}
		*	\endcode
		*/
		if (pause){
			wsprintf(s, _T("Pause..."));
			TextOut(backbuffDC, 580, 200, s, wcslen(s));
		}
		/**
		*	\brief Conclusion inscription announces the winning player
		*	\param p1w, p2w is checking which player wins
		*
		*
		*	These conditions are checked whether won any
		*	of the players, and in accordance with this 
		*	displays the inscription stating the victory of the player
		*
		*
		*	The conditions code is as follows:
		*	\code
		*	if (p1w == 1){
		*		wsprintf(s, _T("Player1 - Win!"));
		*		TextOut(backbuffDC, 550, 100, s, wcslen(s));
		*	}
		*	if (p2w == 1){
		*		wsprintf(s, _T("Player2 - Win!"));
		*		TextOut(backbuffDC, 550, 100, s, wcslen(s));
		*	}
		*	\endcode
		*/
		if (p1w == 1){
			wsprintf(s, _T("Player1 - Win!"));
			TextOut(backbuffDC, 550, 100, s, wcslen(s));
		}
		if (p2w == 1){
			wsprintf(s, _T("Player2 - Win!"));
			TextOut(backbuffDC, 550, 100, s, wcslen(s));
		}
		/**
		*	\brief Condition for the victory of the 1st player
		*	\param Player1Score is first player game score
		*
		*
		*	This provision implements the checking score
		*	of the 1st player in accordance with it performs
		*	or does not perform the algorithm output of the winner
		*	and reset accounts for replayability
		*
		*
		*	The condition code is as follows:
		*	\code
		*	if (Player1Score >= 10){
		*		anrialMode = 1;
		*	if (p2w = 1){
		*		p2w--;
		*	}
		*		p1w++;
		*		Player1Score = 0;
		*		Player2Score = 0;
		*	}
		*	\endcode
		*/
		if (Player1Score >= 10){
			anrialMode = 1;
			if (p2w = 1){
				p2w--;
			}
			p1w++;
			Player1Score = 0;
			Player2Score = 0;
		}
		/**
		 *	\brief Condition for the victory of the 2nd player 
		 *	\param Player2Score is second player game score
		 *
		 *
		 *	This provision implements the checking score
		 *	of the 2nd player in accordance with it performs
		 *	or does not perform the algorithm output of the winner
		 *	and reset accounts for replayability
		 *
		 *	
		 *	The condition code is as follows:
		 *	\code
		 *	if (Player2Score >= 10){
		 *		anrialMode = 1;
		 *		if (p1w = 1){
		 *			p1w--;
		 *		}
		 *		p2w++;
		 *		Player1Score = 0;
		 *		Player2Score = 0;
		 *	}
		 *	\endcode
		 */
		if (Player2Score >= 10){
			anrialMode = 1;
			if (p1w = 1){
				p1w--;
			}
			p2w++;
			Player1Score = 0;
			Player2Score = 0;
		}
		/// End application-specific layout section.
		BitBlt(hdc, 0, 0, width, height, backbuffDC, 0, 0, SRCCOPY);
		RestoreDC(backbuffDC, savedDC);
		DeleteObject(backbuffer);
		DeleteDC(backbuffDC);
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}