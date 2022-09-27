#define UNICODE
#define _UNICODE

#include<Windows.h>
#include<stdio.h>
#include<CommCtrl.h>
#include<io.h>
#include".\include\AMCMathClientHeader.h"
#include".\include\AMCMathServices_Resources.h"
#import".\dotnet\import\DotNetTrigServer.tlb" no_namespace named_guids raw_interfaces_only

/* Macros */
#define AMC_MAXLOGENTRIES 255
#define AMC_MAXNAMELENGTH 25

#define INV_FNAME 1
#define INV_MNAME 2
#define INV_LNAME 4

#define RET_MAINDLG_SHOWLOG 2

#define LOG_XBEGIN 500

#define CUSTOM_PRINTLOG ((WM_APP) + 0x0001)

#define AMC_COUNTOFOPERATIONS 5

/* Global Function Declarations */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ResultDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

/* Global Variable Declarations */
// Strings
TCHAR szIntro[] = TEXT("ASTROMEDICOMP");
TCHAR szSpacePrompt[] = TEXT("PRESS THE SPACE BAR TO CONTINUE");
TCHAR szExit[] = TEXT("You have closed the Application");
TCHAR gszResult[1024] = TEXT("Following are the results of operations you requested for:\n\n");

// Declaration of Global Interface Pointers
ISum *gpISum = NULL;
ISubtract *gpISubtract = NULL;
IMultiply *gpIMultiply = NULL;
IDivide *gpIDivide = NULL;
ISquare *gpISquare = NULL;
ICube *gpICube = NULL;
IDispatch *gpIDispatchPowerTwo = NULL, *gpIDispatchTrig = NULL;

// Declarations of all global flags
BOOL bRegisteredUser = FALSE, bNewUser = FALSE, bRetiringUser = FALSE;	// for Radio Buttons
BOOL bSum = FALSE, bSubtract = FALSE;	// for checkboxes in Basic 1
BOOL bMultiply = FALSE, bDivide = FALSE;	// for checkboxes in Basic 2
BOOL bSquare = FALSE, bCube = FALSE;	// for checkboxes in Power 1
BOOL bSquareRoot = FALSE, bCubeRoot = FALSE;	// for checkboxes in Power 2
BOOL bSine = FALSE, bCosine = FALSE;

// Concerned with the users
UINT giCurrentUserId;
TCHAR szUserFName[AMC_MAXNAMELENGTH], szUserMName[AMC_MAXNAMELENGTH], szUserLName[AMC_MAXNAMELENGTH];

// Misc
HINSTANCE ghInst = NULL;
int cxChar = 0, cyChar = 0;
char gRegisteredUsersTable[100], gNewUserId = 0;

// Entry-Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Local variable declarations
	INITCOMMONCONTROLSEX icc;
	HACCEL hAccel;
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("AMCMathServer");
	TCHAR str[255];
	HRESULT hr = NULL;

	// Code
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		wsprintf(str, TEXT("CoInitialize(): failed with return value 0x%x"), hr);
		MessageBox(NULL, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
		ExitProcess(EXIT_FAILURE);
	}

	ghInst = hInstance;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NAMASAKARAM));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NAMASAKARAM));

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName,
		TEXT("AstroMediComp Math Server"),
		WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		hInstance, NULL);

	// initializing common controls
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc);

	// initializing accelerators for use during log display
	hAccel = LoadAccelerators(hInstance, TEXT("LogMenuAccels"));

	ShowWindow(hwnd, SHOW_FULLSCREEN);
	UpdateWindow(hwnd);

	// Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	DestroyAcceleratorTable(hAccel);
	CoUninitialize();

	return(static_cast<int>(msg.wParam));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Local function declarations
	void SafeInterfaceRelease(void);

	// Local Variable Declarations
	static HBITMAP		hbmLogo, hbmMask, hbmIntro, hbmSpacePrompt, hbmExit;
	static HINSTANCE	hInstance;
	static HDC			hdcMemIntro, hdcMemSpacePrompt, hdcMemExit;
	static HFONT		hfontIntro, hfontSpacePrompt, hfontExit;
	static LOGFONT		lf;
	static int			cxClient, cyClient, cxLogo, cyLogo, cxIntro, cyIntro, cxSpacePrompt, cySpacePrompt, cxExit, cyExit;
	static BOOL			bShowIntro, bShowLogo, bShowSpacePrompt, bShowExit;
	BITMAP				bitmap;
	HDC					hdc, hdcMemLogo, hdcMemMask;
	int					xLogo, yLogo, xIntro, yIntro, xSpacePrompt, ySpacePrompt, xExit, yExit, iRetValDlg;
	PAINTSTRUCT			ps;
	SIZE				sizeIntro, sizeSpacePrompt, sizeExit;

	// for the log
	static HFONT		hfontLogEntry;
	static BOOL			bShowLog = FALSE, bShowScroll;
	static int			cxChar, cyChar, iVscrollPos = 0, iLogLineCount = 0, iLogEntry;
	static char			szFullname[50], szAddress[50], szUserCreationDate[11], szUserCreationTime[9], szUserID[3];
	static char			szDateOfOperation[AMC_MAXLOGENTRIES + 1][11], szTimeOfOperation[AMC_MAXLOGENTRIES + 1][9], szOperation[AMC_MAXLOGENTRIES + 1][15];
	static HMENU		hMenuLog;
	TEXTMETRIC			tmLogEntry;
	FILE *stream;
	SIZE				sizeLogEntry;
	char				szFilename[20], c;
	TCHAR				wszDateOfOperation[11], wszTimeOfOperation[9], wszOperation[15];
	int					size, iTemp, yLogEntry;

	// Code
	switch (iMsg)
	{
	case WM_CREATE:
		/* Setup for the Introduction */
		hdc = GetDC(hwnd);	/* create this window's dc */

		lf.lfHeight = 64l;
		lf.lfWidth = 0l;
		lf.lfEscapement = 0l;
		lf.lfOrientation = 0l;
		lf.lfWeight = FW_NORMAL;
		lf.lfItalic = (BYTE)0;
		lf.lfUnderline = (BYTE)0;
		lf.lfStrikeOut = (BYTE)0;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = PROOF_QUALITY;
		lf.lfPitchAndFamily = FF_DONTCARE | FIXED_PITCH;
		wcscpy(lf.lfFaceName, TEXT("Tahoma"));

		/* Setup for Intro */
		bShowIntro = TRUE;

		hfontIntro = CreateFontIndirect(&lf);
		SelectObject(hdc, hfontIntro);

		GetTextExtentPoint32(hdc, szIntro, lstrlen(szIntro), &sizeIntro);
		cxIntro = sizeIntro.cx;
		cyIntro = sizeIntro.cy;
		hbmIntro = CreateCompatibleBitmap(hdc, cxIntro, cyIntro);

		/* Setup for Space Prompt */
		bShowSpacePrompt = TRUE;	/* set the prompt visible until spacebar is hit for the first time */

		lf.lfHeight = 24l;

		hfontSpacePrompt = CreateFontIndirect(&lf);
		SelectObject(hdc, hfontSpacePrompt);

		GetTextExtentPoint32(hdc, szSpacePrompt, lstrlen(szSpacePrompt), &sizeSpacePrompt);
		cxSpacePrompt = sizeSpacePrompt.cx;
		cySpacePrompt = sizeSpacePrompt.cy;
		hbmSpacePrompt = CreateCompatibleBitmap(hdc, cxSpacePrompt, cySpacePrompt);

		/* Setup for Exit Prompt */
		bShowExit = FALSE;

		hfontExit = CreateFontIndirect(&lf);
		SelectObject(hdc, hfontExit);

		GetTextExtentPoint32(hdc, szExit, lstrlen(szExit), &sizeExit);
		cxExit = sizeExit.cx;
		cyExit = sizeExit.cy;
		hbmExit = CreateCompatibleBitmap(hdc, cxExit, cyExit);

		ReleaseDC(hwnd, hdc);	/* release this window's dc */

		/* Setup for AMC Logo Bitmap */
		bShowLogo = TRUE;	/* set the logo visible until spacebar is hit for the first time */

		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

		// Load Original Image and get its size
		hbmLogo = LoadBitmap(hInstance, MAKEINTRESOURCE(IDBM_AMCLOGO));
		GetObject(hbmLogo, sizeof(BITMAP), &bitmap);
		cxLogo = bitmap.bmWidth;
		cyLogo = bitmap.bmHeight;

		/*
		 * Create a generic memory DC compatible with current screen
		 * Select logo bitmap into the DC
		 */
		hdcMemLogo = CreateCompatibleDC(NULL);	/* create a default dc for the Logo */
		SelectObject(hdcMemLogo, hbmLogo);

		/*
		 * Create an empty monochrome masking bitmap to hide white portions from the logo
		 * Create also a memory context for the mask compatible with current screen
		 * Select the mask bitmap into its memory context
		 */
		hbmMask = CreateBitmap(cxLogo, cyLogo, 1, 1, NULL);
		hdcMemMask = CreateCompatibleDC(NULL);	/* create a default dc for the Mask */
		SelectObject(hdcMemMask, hbmMask);

		/*
		 * First, select blackness for the entire mask
		 * Next, select whiteness in the mask for parts of the logo to be displayed
		 * Finally, select blackness once again, to hide out the text in logo (it must be printed programmatically)
		 */
		SelectObject(hdcMemMask, GetStockObject(BLACK_BRUSH));
		Rectangle(hdcMemMask, 0, 0, cxLogo, cyLogo);
		SelectObject(hdcMemMask, GetStockObject(WHITE_BRUSH));
		Ellipse(hdcMemMask, 5, 5, cxLogo - 5, cyLogo - 5);
		SelectObject(hdcMemMask, GetStockObject(BLACK_BRUSH));
		Rectangle(hdcMemMask, 0, 0, cxLogo, 100);

		// Use the processed mask on the logo
		BitBlt(hdcMemLogo, 0, 0, cxLogo, cyLogo,
			hdcMemMask, 0, 0, SRCAND);

		DeleteDC(hdcMemMask);	/* delete mask dc */
		DeleteDC(hdcMemLogo);	/* delete logo dc */

		/* Hide the scroll bar for now */
		ShowScrollBar(hwnd, SB_VERT, FALSE);
		break;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		/* Painting AMC Logo Bitmap */

		if (bShowLogo)
		{
			/*
			 * Create memory device contexts compatible with this window's display
			 * Select both bitmaps into their memory DCs
			 */
			hdcMemLogo = CreateCompatibleDC(hdc);	/* create a dc for logo */
			SelectObject(hdcMemLogo, hbmLogo);
			hdcMemMask = CreateCompatibleDC(hdc);	/* create a dc for mask */
			SelectObject(hdcMemMask, hbmMask);

			// Choose x, y so that the bitmap is placed centrally in the window
			xLogo = (cxClient - cxLogo) / 2;
			yLogo = (cyClient - cyLogo) / 2;

			/* Transfer the mask to screen, with Raster Operation
			   corresponding to (D & ~S), i.e. Destination bits
			   ANDed with the complement of Source bits */
			BitBlt(hdc, xLogo, yLogo, cxLogo, cyLogo,
				hdcMemMask, 0, 0, (DWORD)0x00220326);

			// Transfer the logo to screen
			BitBlt(hdc, xLogo, yLogo, cxLogo, cyLogo,
				hdcMemLogo, 0, 0, SRCPAINT);

			DeleteDC(hdcMemMask);	/* delete mask dc */
			DeleteDC(hdcMemLogo);	/* delete logo dc */
		}

		if (bShowIntro)
		{
			/* Painting Introduction */
			hdcMemIntro = CreateCompatibleDC(hdc);	/* create a dc for intro-text */

			xIntro = (cxClient - cxIntro) / 2;
			yIntro = cyClient / 20;

			SelectObject(hdcMemIntro, hbmIntro);
			SelectObject(hdcMemIntro, hfontIntro);
			SetBkColor(hdcMemIntro, RGB(0, 0, 0));
			SetTextColor(hdcMemIntro, RGB(28, 158, 208));
			TextOut(hdcMemIntro, 0, 0, szIntro, lstrlen(szIntro));

			BitBlt(hdc, xIntro, yIntro, cxIntro, cyIntro,
				hdcMemIntro, 0, 0, SRCPAINT);

			DeleteDC(hdcMemIntro);	/* delete intro-text dc */
		}

		/* Painting Space Prompt */
		if (bShowSpacePrompt)
		{
			hdcMemSpacePrompt = CreateCompatibleDC(hdc);	/* create a dc for SpacePrompt */

			xSpacePrompt = (cxClient - cxSpacePrompt) / 2;
			ySpacePrompt = cyClient - cySpacePrompt - 50;

			SelectObject(hdcMemSpacePrompt, hbmSpacePrompt);
			SelectObject(hdcMemSpacePrompt, hfontSpacePrompt);
			SetBkColor(hdcMemSpacePrompt, RGB(0, 0, 0));
			SetTextColor(hdcMemSpacePrompt, RGB(0, 70, 236));
			TextOut(hdcMemSpacePrompt, 0, 0, szSpacePrompt, lstrlen(szSpacePrompt));

			BitBlt(hdc, xSpacePrompt, ySpacePrompt, cxSpacePrompt, cySpacePrompt,
				hdcMemSpacePrompt, 0, 0, SRCPAINT);

			DeleteDC(hdcMemSpacePrompt);	/* delete SpacePrompt dc */
		}

		/* Painting Closing Text */
		if (bShowExit)
		{
			hdcMemExit = CreateCompatibleDC(hdc);

			xExit = (cxClient - cxExit) / 2;
			yExit = (cyClient - cySpacePrompt) / 2;

			SelectObject(hdcMemExit, hbmExit);
			SelectObject(hdcMemExit, hfontExit);
			SetBkColor(hdcMemExit, RGB(0, 0, 0));
			SetTextColor(hdcMemExit, RGB(0, 70, 236));
			TextOut(hdcMemExit, 0, 0, szExit, lstrlen(szExit));

			BitBlt(hdc, xExit, yExit, cxExit, cyExit,
				hdcMemExit, 0, 0, SRCPAINT);

			DeleteDC(hdcMemExit);
		}

		/* Painting the Log */
		if (bShowLog)
		{
			SelectObject(hdc, hfontLogEntry);
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(0, 255, 0));
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));

			TextOutA(hdc, 5, 10, "Name : ", strlen("Name : "));
			TextOutA(hdc, 5 + cxChar * strlen("Name : "), 10, szFullname, strlen(szFullname));

			TextOutA(hdc, 5, 10 + cyChar, "RegID: ", strlen("RegID: "));
			TextOutA(hdc, 5 + cxChar * strlen("RegID: "), 10 + cyChar, szUserID, strlen(szUserID));

			TextOutA(hdc, 5, 10 + (2 * cyChar), "Addr : ", strlen("Addr : "));
			TextOutA(hdc, 5 + cxChar * strlen("Addr : "), 10 + (2 * cyChar), szAddress, strlen(szAddress));

			TextOutA(hdc, 5, 10 + (3 * cyChar), "RegDt: ", strlen("RegDt: "));
			TextOutA(hdc, 5 + cxChar * strlen("RegDt: "), 10 + (3 * cyChar), szUserCreationDate, strlen(szUserCreationDate));

			TextOutA(hdc, 5, 10 + (4 * cyChar), "RegTm: ", strlen("RegTm: "));
			TextOutA(hdc, 5 + cxChar * strlen("RegTm: "), 10 + (4 * cyChar), szUserCreationTime, strlen(szUserCreationTime));

			for (iTemp = 0; iTemp < iLogEntry; iTemp++)
			{
				/**
				* scrolling down => uncovering the upper part of the window
				* scrolling up => uncovering the lower part of the window
				* To flip the sign, cyChar is multiplied with (iTemp - iVscrollPos)
				* instead of (iVscrollPos - iTemp)
				*/
				yLogEntry = cyChar * (iTemp - iVscrollPos + 1);

				TextOutA(hdc, LOG_XBEGIN, cyChar + yLogEntry, szDateOfOperation[iTemp], strlen(szDateOfOperation[iTemp]));

				TextOutA(hdc, LOG_XBEGIN + (cxChar * 10) + 10,
					cyChar + yLogEntry, szTimeOfOperation[iTemp], strlen(szTimeOfOperation[iTemp]));

				TextOutA(hdc, LOG_XBEGIN + (cxChar * 10) + 10 + (cxChar * 9) + 2,
					cyChar + yLogEntry, szOperation[iTemp], strlen(szOperation[iTemp]));

				Rectangle(hdc, LOG_XBEGIN - 5, 0, LOG_XBEGIN - 5 + (cxChar * 10) + 10 + (cxChar * 9) + (cxChar * 15), cyChar * 2);
			}

			TextOutA(hdc, LOG_XBEGIN, 10, "Date", strlen("Date"));
			TextOutA(hdc, LOG_XBEGIN + (cxChar * 10) + 10, 10, "Time", strlen("Time"));
			TextOutA(hdc, LOG_XBEGIN + (cxChar * 10) + 10 + (cxChar * 9) + 2, 10, "Operation", strlen("Operation"));
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_VSCROLL:
		if (bShowScroll)
		{
			switch (LOWORD(wParam))
			{
			case SB_LINEUP:
				iVscrollPos = iVscrollPos - 1;
				break;

			case SB_LINEDOWN:
				iVscrollPos = iVscrollPos + 1;
				break;

			case SB_PAGEUP:
				iVscrollPos = iVscrollPos - (cyClient / cyChar);
				break;

			case SB_PAGEDOWN:
				iVscrollPos = iVscrollPos - (cyClient / cyChar);
				break;

			case SB_THUMBPOSITION:
				iVscrollPos = HIWORD(wParam);
				break;

			default:
				break;
			}

			iVscrollPos = max(0, min(iVscrollPos, iLogLineCount - 1));

			if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))
			{
				SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_HOME:
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
			break;

		case VK_END:
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;

		case VK_UP:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
			break;

		case VK_DOWN:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;

		case VK_PRIOR:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;

		case VK_NEXT:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;

		default:
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case ' ':
			if (bShowSpacePrompt)
			{
				bShowLogo = FALSE;
				bShowSpacePrompt = FALSE;
				InvalidateRect(hwnd, NULL, TRUE);

				if (hbmLogo)
				{
					DeleteObject(hbmLogo);
					hbmLogo = NULL;
				}
				if (hbmMask)
				{
					DeleteObject(hbmMask);
					hbmMask = NULL;
				}
				if (hfontSpacePrompt)
				{
					DeleteObject(hfontSpacePrompt);
					hfontSpacePrompt = NULL;
				}
				if (hbmSpacePrompt)
				{
					DeleteObject(hbmSpacePrompt);
					hbmSpacePrompt = NULL;
				}

				iRetValDlg = DialogBox(ghInst, TEXT("DATAENTRY"), hwnd, (DLGPROC)MainDlgProc);  // DialogBox() is a blocking API
				bShowIntro = FALSE;
				if (iRetValDlg == RET_MAINDLG_SHOWLOG)
					bShowExit = FALSE;
				else
					bShowExit = TRUE;

				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
			}
			break;

		default:
			break;
		}
		break;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
		else
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDMI_RDLG_ABOUT:
			DialogBox(ghInst, TEXT("About"), hwnd, (DLGPROC)AboutDlgProc);
			break;

		case IDMI_RDLG_QUIT:
			DestroyWindow(hwnd);
			break;

		default:
			break;
		}
		break;

	case CUSTOM_PRINTLOG:
		sprintf(szFilename, "./Users/%d.amcur\0", giCurrentUserId);

		stream = fopen(szFilename, "r");
		if (!stream)
		{
			MessageBox(hwnd, TEXT("Cannot open user log"), TEXT("CRT Error"), MB_OK | MB_ICONERROR);
			DestroyWindow(hwnd);
		}

		// extrating the header
		size = 0;
		while ((c = (char)fgetc(stream)) != '\n' && size < 50)	// line 1
			szFullname[size++] = c;
		szFullname[size] = '\0';

		size = 0;
		while ((c = (char)fgetc(stream)) != '\n' && size < 40)	// line 2
			szAddress[size++] = c;
		szAddress[size] = '\0';

		size = 0;
		while ((c = (char)fgetc(stream)) != ' ' && size < 11)	// line 3, until the 1st whitespace
			szUserCreationDate[size++] = c;
		szUserCreationDate[size] = '\0';

		size = 0;
		while ((c = (char)fgetc(stream)) != '\n' && size < 9)	// line 3, to the end
			szUserCreationTime[size++] = c;
		szUserCreationTime[size] = '\0';

		while (fgetc(stream) != '\n');	// line 4
		fgetc(stream);	// consume the newline on line 5

		// extracting logged data
		c = (char)0;
		iLogEntry = 0;
		while (c != EOF && iLogEntry < (AMC_MAXLOGENTRIES + 1))
		{
			size = 0;
			while (size < 10)
			{
				if ((c = (char)fgetc(stream)) == EOF)
					break;
				szDateOfOperation[iLogEntry][size++] = c;
			}
			fgetc(stream);	// consume the whitespace
			szDateOfOperation[iLogEntry][size] = '\0';

			size = 0;
			while (size < 8)
			{
				if ((c = (char)fgetc(stream)) == EOF)
					break;
				szTimeOfOperation[iLogEntry][size++] = c;
			}
			fgetc(stream);	// consume the whitespace
			szTimeOfOperation[iLogEntry][size] = '\0';

			size = 0;
			while ((c = (char)fgetc(stream)) != '\n' && size < 15)
			{
				if (c == EOF)
					break;
				szOperation[iLogEntry][size++] = c;
			}
			szOperation[iLogEntry][size] = '\0';

			iLogEntry++;
		}
		fclose(stream);

		/* Setup for log */
		sprintf(szUserID, "%02d\0", giCurrentUserId);

		lf.lfHeight = 20l;
		lf.lfWidth = 0l;
		lf.lfEscapement = 0l;
		lf.lfOrientation = 0l;
		lf.lfWeight = FW_BOLD;
		lf.lfItalic = (BYTE)0;
		lf.lfUnderline = (BYTE)0;
		lf.lfStrikeOut = (BYTE)0;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = ANTIALIASED_QUALITY;
		lf.lfPitchAndFamily = FF_DONTCARE | FIXED_PITCH;
		wcscpy(lf.lfFaceName, TEXT("Courier New"));

		hfontLogEntry = CreateFontIndirect(&lf);

		hdc = GetDC(hwnd);
		SelectObject(hdc, hfontLogEntry);

		GetTextMetrics(hdc, &tmLogEntry);
		cxChar = tmLogEntry.tmAveCharWidth;
		cyChar = tmLogEntry.tmHeight + tmLogEntry.tmExternalLeading;

		ReleaseDC(hwnd, hdc);

		iLogLineCount = iLogEntry;

		bShowScroll = iLogLineCount > (cyClient / cyChar) ? TRUE : FALSE;
		if (bShowScroll)
		{
			SetScrollRange(hwnd, SB_VERT, 0, iLogLineCount - 1, FALSE);
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, FALSE);
		}

		/* Now that the Window is entirely occupied by the Log, add the menu to it */
		hMenuLog = LoadMenu(ghInst, TEXT("ContentsLogWnd"));
		SetMenu(hwnd, hMenuLog);

		bShowLog = TRUE;
		break;

	case WM_DESTROY:
		if (hfontLogEntry)
		{
			DeleteObject(hfontLogEntry);
			hfontLogEntry = NULL;
		}
		if (hfontIntro)
		{
			DeleteObject(hfontIntro);
			hfontIntro = NULL;
		}
		if (hbmIntro)
		{
			DeleteObject(hbmIntro);
			hbmIntro = NULL;
		}
		if (hMenuLog)
		{
			DestroyMenu(hMenuLog);
			hMenuLog = NULL;
		}

		SafeInterfaceRelease();
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

// MYDLGBOX Procedure
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Local function declarations
	void EnableGroupBoxThree(HWND, BOOL);
	BOOL IsRegIDInvalid(HWND, TCHAR *);
	BOOL GetNameOrError(HWND, int, TCHAR *);
	BOOL AreNamesInvalid(HWND, BOOL **);
	HRESULT GetRequiredInterfaces(HWND);
	int GetState(HWND);
	int AddAddressesToComboBox(HWND);
	void GetControlRectRelativeTo(HWND, int, LPRECT);
	HWND AttachTipToRect(HWND, LPRECT, LPWSTR);
	int AppendResult(TCHAR *);
	int AppendLog(UINT, TCHAR *);
	int ShowResult(HWND);
	FILE *CreateUserFile(int, TCHAR *, TCHAR *, TCHAR *, TCHAR *, TCHAR *);
	void EnumerateRegisteredUserIDs(void);
	void DestroyToolTips(HWND *);

	// Local variable declarations
	// for the Automation client
	float fX = 0.0f, fY = 0.0f, fResult = 0.0f;
	HRESULT hr;
	DISPID dispid;
	OLECHAR *szSquareRootFunc = L"SquareRootOfNumber";
	OLECHAR *szCubeRootFunc = L"CubeRootOfNumber";
	OLECHAR *szSineFunc = L"Sine";
	OLECHAR *szCosineFunc = L"Cosine";
	VARIANT vArg[2], vRet;
	DISPPARAMS params;

	// handle of the parent
	static HWND hParent;

	// for tooltips
	static HWND hwndToolTips[AMC_COUNTOFOPERATIONS];
	static RECT rectBasicOne;
	TCHAR szTTBasicOne[] = TEXT("Class Factory");

	// for drawing
	HDC hdcMainDlg = NULL;
	int iSpare = 0;

	// for the Address Combo box
	static TEXTMETRIC tm;
	static TCHAR szGB3UserEntry[255], endch = '\0', *pendch = &endch, szComboText[255];
	static BOOL fUserAddressEditEntryValidity;
	int iState;

	// for File I/O
	FILE *fdUserFile = NULL;
	size_t sizeUserFile = 0;
	TCHAR wszUserFileName[20], szFName[15], szMName[15], szLName[15], szState[20], szCity[20];
	TCHAR wszUserRetired[35];
	char szUserFileName[20];

	// for Validations
	// to paint edit texts red
	static BOOL bPaintEditTextRed = FALSE;
	static BOOL bRedEdit_RegID = FALSE, bRedEdit_FName = FALSE, bRedEdit_MName = FALSE, bRedEdit_LName = FALSE, bRedEdit_Addr = FALSE, bRedEdit_GB3 = FALSE;
	static HBRUSH hbrEditText = NULL;
	TCHAR wszErrorMsg[255], wszNameOrError[255], wszAddressOrError[255];
	BOOL *pbNameValidity[3], bNamesInvalid;

	// New users
	static TCHAR szFNameUserEntry[15], szMNameUserEntry[15], szLNameUserEntry[15], szNameUserEntry[15];
	HWND hwndLastInFocus = NULL;
	TCHAR wszNotifyNewUserId[40];
	int iNameBegin, iNameEnd, iNameLength;

	// Group Box 3
	static TCHAR wszGB3Operand[5];
	static HWND hwndErroneousOperand = NULL;

	// Misc.
	BOOL bError = FALSE;

	// Code
	switch (iMsg)
	{
	case WM_INITDIALOG:
		// GUI Initializations
		hdcMainDlg = GetDC(hDlg);
		GetTextMetrics(hdcMainDlg, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hDlg, hdcMainDlg);

		EnableWindow(GetDlgItem(hDlg, IDET_FNAME), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDET_MNAME), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDET_LNAME), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDCOMBO_ADDRESS), FALSE);

		/* The RegID edit control is empty at start. The user should hence not be able to click the OK button */
		EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
		EnableGroupBoxThree(hDlg, FALSE);
		UpdateWindow(hDlg);

		AddAddressesToComboBox(GetDlgItem(hDlg, IDCOMBO_ADDRESS));
		SetDlgItemText(hDlg, IDST_ADDRESS, pszAddresses[0][1]);

		CreateDirectory(TEXT("./Users"), NULL);
		EnumerateRegisteredUserIDs();

		SendDlgItemMessage(hDlg, IDRB_REGISTERED, BM_SETCHECK, 1, 0);
		bRegisteredUser = TRUE;

		hParent = GetWindow(hDlg, GW_OWNER);
		if (!hParent)
		{
			MessageBox(hDlg, TEXT("Unable to print log as owner window's handle cannot be obtained"), TEXT("Win32 Error"), MB_OK | MB_ICONERROR);
			EndDialog(hDlg, 0);
		}

		/* Attach tooltips to GroupBox 3 headings */
		GetControlRectRelativeTo(hDlg, IDST_BASIC1, &rectBasicOne);
		hwndToolTips[0] = AttachTipToRect(hDlg, &rectBasicOne, szTTBasicOne);

		return(TRUE);

	case WM_PAINT:
		hdcMainDlg = GetDC(hDlg);

		SelectObject(hdcMainDlg, GetStockObject(DC_PEN));
		SelectObject(hdcMainDlg, GetStockObject(NULL_BRUSH));
		SetDCPenColor(hdcMainDlg, RGB(150, 150, 250));

		Rectangle(hdcMainDlg, (5 * cxChar / 4), (40 * cyChar / 8), (639 * cxChar / 4), (85.5 * cyChar / 8));
		Rectangle(hdcMainDlg, (7 * cxChar / 4), (40 * cyChar / 8), (637 * cxChar / 4), (85.5 * cyChar / 8));

		Rectangle(hdcMainDlg, (7 * cxChar / 4), (85 * cyChar / 8), (125.5 * cxChar / 4), (140 * cyChar / 8));
		Rectangle(hdcMainDlg, (127 * cxChar / 4), (85 * cyChar / 8), (255 * cxChar / 4), (140 * cyChar / 8));
		Rectangle(hdcMainDlg, (257 * cxChar / 4), (85 * cyChar / 8), (385.5 * cxChar / 4), (140 * cyChar / 8));
		Rectangle(hdcMainDlg, (387 * cxChar / 4), (85 * cyChar / 8), (515 * cxChar / 4), (140 * cyChar / 8));
		Rectangle(hdcMainDlg, (517 * cxChar / 4), (85 * cyChar / 8), (637 * cxChar / 4), (140 * cyChar / 8));
		Rectangle(hdcMainDlg, (5 * cxChar / 4), (85 * cyChar / 8), (639 * cxChar / 4), (140 * cyChar / 8));

		Rectangle(hdcMainDlg, (5 * cxChar / 4), (5 * cyChar / 8), (639 * cxChar / 4), (100 * cyChar / 8));

		ReleaseDC(hDlg, hdcMainDlg);
		break;		// IMPORTANT: if you don't return FALSE here (which is the last statement of this function)
					// you will not see your message boxes!

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDPB_OK:
			/* New user registration */
			if (bNewUser)
			{
				GetDlgItemText(hDlg, IDET_FNAME, szFName, 15);
				GetDlgItemText(hDlg, IDET_MNAME, szMName, 15);
				GetDlgItemText(hDlg, IDET_LNAME, szLName, 15);
				GetDlgItemText(hDlg, IDCOMBO_ADDRESS, szState, 20);
				GetDlgItemText(hDlg, IDST_ADDRESS, szCity, 20);

				fdUserFile = CreateUserFile(gNewUserId, szFName, szMName, szLName, szState, szCity);

				swprintf_s(wszNotifyNewUserId, 40, TEXT("Your Registration ID(RegID) is %02d"), gNewUserId);
				MessageBox(hDlg, wszNotifyNewUserId, TEXT("New User Registration Successful"), MB_OK | MB_ICONINFORMATION);

				giCurrentUserId = gNewUserId;

				EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
				UpdateWindow(hDlg);
			}
			/* Retiring User */
			else if (bRetiringUser)
			{
				giCurrentUserId = GetDlgItemInt(hDlg, IDET_REGID, NULL, FALSE);

				sprintf(szUserFileName, "./Users/%d.amcur", giCurrentUserId);
				DeleteFileA(szUserFileName);
				swprintf_s(wszUserRetired, 35, TEXT("User %02d retired successfully\0"), giCurrentUserId);
				MessageBox(hDlg, wszUserRetired, TEXT("Retirement Successful"), MB_OK | MB_ICONINFORMATION);
			}
			/* Registered User */
			else
			{
				giCurrentUserId = GetDlgItemInt(hDlg, IDET_REGID, NULL, FALSE);

				if (!bError)
				{
					/* Calculations begin post user validation/registration */
					hr = GetRequiredInterfaces(hDlg);
					if (FAILED(hr))
						EndDialog(hDlg, 1);

					if (SendMessage(GetDlgItem(hDlg, IDCB_ADDITION), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_ADDITION_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);
						GetDlgItemText(hDlg, IDET_ADDITION_Y, szGB3UserEntry, 255);
						fY = wcstof(szGB3UserEntry, &pendch);

						/*
						 * Instead of keeping the else part bare and open, enclose it explicitly in the else block,
						 * because the user might have selected more options to calculate, and if there's no problem
						 * in fetching others, the application should continue to work.
						 *
						 * Ex: getting square root may fail if a negative value was entered, but the cube root, that
						 * comes later, will produce correct results.
						 *
						 * The application shouldn't close because square root failed => EndDialog() must not appear
						 * in the if-block => statements following the if-block must be a part of the else-block.
						 */

						hr = gpISum->SumOfTwoNumbers(fX, fY, &fResult);
						if (FAILED(hr))
							swprintf_s(szGB3UserEntry, 255, TEXT("SumOfTwoIntegers(): cannot obtain the sum (Return Code: 0x%x)\n"), hr);
						else
							swprintf_s(szGB3UserEntry, 255, TEXT("Addition of %.2f and %.2f is %.2f\n"), fX, fY, fResult);

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Addition"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_SUBTRACTION), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_SUBTRACTION_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);
						GetDlgItemText(hDlg, IDET_SUBTRACTION_Y, szGB3UserEntry, 255);
						fY = wcstof(szGB3UserEntry, &pendch);

						hr = gpISubtract->SubtractionOfTwoNumbers(fX, fY, &fResult);
						if (FAILED(hr))
							swprintf_s(szGB3UserEntry, 255, TEXT("SubtrationOfTwoIntegers(): cannot obtain the subtraction (Return Code: 0x%x)\n"), hr);
						else
							swprintf_s(szGB3UserEntry, 255, TEXT("Subtraction of %.2f and %.2f is %.2f\n"), fX, fY, fResult);

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Subtraction"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_MULTIPLICATION), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_MULTIPLICATION_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);
						GetDlgItemText(hDlg, IDET_MULTIPLICATION_Y, szGB3UserEntry, 255);
						fY = wcstof(szGB3UserEntry, &pendch);

						hr = gpIMultiply->MultiplicationOfTwoNumbers(fX, fY, &fResult);
						if (FAILED(hr))
							swprintf_s(szGB3UserEntry, 255, TEXT("MultiplicationOfTwoIntegers(): cannot obtain the multiplication (Return Code: 0x%x)\n"), hr);
						else
							swprintf_s(szGB3UserEntry, 255, TEXT("Multiplication of %.2f and %.2f is %.2f\n"), fX, fY, fResult);

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Multiplication"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_DIVISION), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_DIVISION_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);
						GetDlgItemText(hDlg, IDET_DIVISION_Y, szGB3UserEntry, 255);
						fY = wcstof(szGB3UserEntry, &pendch);

						hr = gpIDivide->DivisionOfTwoNumbers(fX, fY, &fResult);
						if (FAILED(hr))
							swprintf_s(szGB3UserEntry, TEXT("DivisionOfTwoIntegers(): cannot obtain the division (Return Code: 0x%x)\n"), hr);
						else
							swprintf_s(szGB3UserEntry, 255, TEXT("Division of %.2f and %.2f is %.2f\n"), fX, fY, fResult);

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Division"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_SQUARE), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_SQUARE_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);

						hr = gpISquare->SquareOfNumber(fX, &fResult);
						if (FAILED(hr))
							swprintf_s(szGB3UserEntry, TEXT("SquareOfNumber(): cannot obtain the square (Return Code: 0x%x)\n"), hr);
						else
							swprintf_s(szGB3UserEntry, 255, TEXT("Square of %.2f is %.2f\n"), fX, fResult);

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Square"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_CUBE), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_CUBE_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);

						hr = gpICube->CubeOfNumber(fX, &fResult);
						if (FAILED(hr))
							swprintf_s(szGB3UserEntry, TEXT("CubeOfNumber(): cannot obtain the cube (Return Code: 0x%x)\n"), hr);
						else
							swprintf_s(szGB3UserEntry, 255, TEXT("Cube of %.2f is %.2f\n"), fX, fResult);

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Cube"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_SQUAREROOT), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_SQUAREROOT_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);

						VariantInit(vArg);
						vArg[0].vt = VT_R4;
						vArg[0].fltVal = fX;

						params.rgvarg = vArg;
						params.cArgs = 1;
						params.rgdispidNamedArgs = NULL;
						params.cNamedArgs = 0;

						VariantInit(&vRet);

						hr = gpIDispatchPowerTwo->GetIDsOfNames(IID_NULL,
							&szSquareRootFunc,
							1,
							GetUserDefaultLCID(),
							&dispid);
						if (FAILED(hr))
						{
							wsprintf(szGB3UserEntry, TEXT("GetIDsOfNames(): cannot obtain the DISPID of SquareRootOfNumber() (Return Code: 0x%x)\n"), hr);
							MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
						}
						else
						{
							hr = gpIDispatchPowerTwo->Invoke(dispid,
								IID_NULL,
								GetUserDefaultLCID(),
								DISPATCH_METHOD,
								&params,
								&vRet,
								NULL,
								NULL);
							if (FAILED(hr))
							{
								wsprintf(szGB3UserEntry, TEXT("Invoke(): cannot invoke SquareRootOfNumber() (Return Code: 0x%x)\n"), hr);
								MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
							}

							swprintf_s(szGB3UserEntry, 255, TEXT("Square Root of %.2f is %.2f\n"), fX, vRet.fltVal);
						}

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("SquareRoot"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_CUBEROOT), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_CUBEROOT_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);

						VariantInit(vArg);
						vArg[0].vt = VT_R4;
						vArg[0].fltVal = fX;

						params.rgvarg = vArg;
						params.cArgs = 1;
						params.rgdispidNamedArgs = NULL;
						params.cNamedArgs = 0;

						VariantInit(&vRet);

						hr = gpIDispatchPowerTwo->GetIDsOfNames(IID_NULL,
							&szCubeRootFunc,
							1,
							GetUserDefaultLCID(),
							&dispid);
						if (FAILED(hr))
						{
							wsprintf(szGB3UserEntry, TEXT("GetIDsOfNames(): cannot obtain the DISPID of CubeRootOfNumber() (Return Code: 0x%x)\n"), hr);
							MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
						}
						else
						{
							hr = gpIDispatchPowerTwo->Invoke(dispid,
								IID_NULL,
								GetUserDefaultLCID(),
								DISPATCH_METHOD,
								&params,
								&vRet,
								NULL,
								NULL);
							if (FAILED(hr))
							{
								wsprintf(szGB3UserEntry, TEXT("Invoke(): cannot invoke CubeRootOfNumber() (Return Code: 0x%x)\n"), hr);
								MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
							}

							swprintf_s(szGB3UserEntry, 255, TEXT("Cube Root of %.2f is %.2f\n"), fX, vRet.fltVal);
						}

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("CubeRoot"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_SINE), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_SINE_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);

						VariantInit(vArg);
						vArg[0].vt = VT_R4;
						vArg[0].fltVal = fX;

						params.rgvarg = vArg;
						params.cArgs = 1;
						params.rgdispidNamedArgs = NULL;
						params.cNamedArgs = 0;

						VariantInit(&vRet);

						hr = gpIDispatchTrig->GetIDsOfNames(IID_NULL,
							&szSineFunc,
							1,
							GetUserDefaultLCID(),
							&dispid);
						if (FAILED(hr))
						{
							wsprintf(szGB3UserEntry, TEXT("GetIDsOfNames(): cannot obtain the DISPID of SineFunc (Return Code: 0x%x)\n"), hr);
							MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
						}
						else
						{
							hr = gpIDispatchTrig->Invoke(dispid,
								IID_NULL,
								GetUserDefaultLCID(),
								DISPATCH_METHOD,
								&params,
								&vRet,
								NULL,
								NULL);
							if (FAILED(hr))
							{
								wsprintf(szGB3UserEntry, TEXT("Invoke(): cannot invoke SineFunc (Return Code: 0x%x)\n"), hr);
								MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
							}

							swprintf_s(szGB3UserEntry, 255, TEXT("Sine of angle %.2f is %.2f\n"), fX, vRet.fltVal);
						}

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Sine"));
					}

					if (SendMessage(GetDlgItem(hDlg, IDCB_COSINE), BM_GETCHECK, 0, 0) == BST_CHECKED)
					{
						GetDlgItemText(hDlg, IDET_COSINE_X, szGB3UserEntry, 255);
						fX = wcstof(szGB3UserEntry, &pendch);

						VariantInit(vArg);
						vArg[0].vt = VT_R4;
						vArg[0].fltVal = fX;

						params.rgvarg = vArg;
						params.cArgs = 1;
						params.rgdispidNamedArgs = NULL;
						params.cNamedArgs = 0;

						VariantInit(&vRet);

						hr = gpIDispatchTrig->GetIDsOfNames(IID_NULL,
							&szCosineFunc,
							1,
							GetUserDefaultLCID(),
							&dispid);
						if (FAILED(hr))
						{
							wsprintf(szGB3UserEntry, TEXT("GetIDsOfNames(): cannot obtain the DISPID of CosineFunc (Return Code: 0x%x)\n"), hr);
							MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
						}
						else
						{
							hr = gpIDispatchTrig->Invoke(dispid,
								IID_NULL,
								GetUserDefaultLCID(),
								DISPATCH_METHOD,
								&params,
								&vRet,
								NULL,
								NULL);
							if (FAILED(hr))
							{
								wsprintf(szGB3UserEntry, TEXT("Invoke(): cannot invoke CosineFunc (Return Code: 0x%x)\n"), hr);
								MessageBox(hDlg, szGB3UserEntry, TEXT("COM Error"), MB_OK | MB_ICONERROR);
							}

							swprintf_s(szGB3UserEntry, 255, TEXT("Cosine of angle %.2f is %.2f\n"), fX, vRet.fltVal);
						}

						AppendResult(szGB3UserEntry);
						AppendLog(giCurrentUserId, TEXT("Cosine"));
					}
					
					ShowResult(hDlg);
				}
			}

			EndDialog(hDlg, 0);
			break;

		case IDRB_REGISTERED:
		case IDRB_RETIRING:
		case IDRB_REGISTERING:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				bRegisteredUser = FALSE;
				bRetiringUser = FALSE;
				bNewUser = FALSE;

				if ((HWND)lParam == GetDlgItem(hDlg, IDRB_REGISTERED))
				{
					// reset error messages if any are present
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

					// disable all unrelated controls, enable only required ones
					EnableWindow(GetDlgItem(hDlg, IDET_REGID), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDET_FNAME), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDET_MNAME), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDET_LNAME), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDCOMBO_ADDRESS), FALSE);
					
					// check for any remainent errors from previous operations before enabling submission controls
					bRedEdit_RegID = IsRegIDInvalid(hDlg, wszErrorMsg);
					if (bRedEdit_RegID)
					{
						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszErrorMsg);

						EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
						EnableGroupBoxThree(hDlg, FALSE);
					}
					else
					{
						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

						EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
						EnableGroupBoxThree(hDlg, TRUE);
					}

					// set default focus
					SetFocus(GetDlgItem(hDlg, IDET_REGID));

					// update radio-button group
					SendMessage(GetDlgItem(hDlg, IDRB_REGISTERED), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendMessage(GetDlgItem(hDlg, IDRB_RETIRING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hDlg, IDRB_REGISTERING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

					// set appropriate group flag
					bRegisteredUser = TRUE;

					// set all flags not belonging to this group box to FALSE before repainting
					bRedEdit_GB3 = FALSE;
					bRedEdit_Addr = FALSE;
					bRedEdit_LName = FALSE;
					bRedEdit_MName = FALSE;
					bRedEdit_FName = FALSE;
				}
				else if ((HWND)lParam == GetDlgItem(hDlg, IDRB_RETIRING))
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

					EnableWindow(GetDlgItem(hDlg, IDET_REGID), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDET_FNAME), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDET_MNAME), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDET_LNAME), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDCOMBO_ADDRESS), FALSE);
					EnableGroupBoxThree(hDlg, FALSE);

					bRedEdit_RegID = IsRegIDInvalid(hDlg, wszErrorMsg);
					if (bRedEdit_RegID)
					{
						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszErrorMsg);
						EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					}
					else
					{
						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));
						EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
					}
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);

					SetFocus(GetDlgItem(hDlg, IDET_REGID));

					SendMessage(GetDlgItem(hDlg, IDRB_REGISTERED), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hDlg, IDRB_RETIRING), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendMessage(GetDlgItem(hDlg, IDRB_REGISTERING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

					bRetiringUser = TRUE;

					bRedEdit_GB3 = FALSE;
					bRedEdit_Addr = FALSE;
					bRedEdit_LName = FALSE;
					bRedEdit_MName = FALSE;
					bRedEdit_FName = FALSE;
				}
				else if ((HWND)lParam == GetDlgItem(hDlg, IDRB_REGISTERING))
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

					EnableWindow(GetDlgItem(hDlg, IDET_REGID), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDET_FNAME), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDET_MNAME), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDET_LNAME), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDCOMBO_ADDRESS), TRUE);

					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					EnableGroupBoxThree(hDlg, FALSE);

					SetFocus(GetDlgItem(hDlg, IDET_FNAME));

					ZeroMemory(wszErrorMsg, lstrlen(wszErrorMsg));
					*pbNameValidity = &bRedEdit_FName;
					*(pbNameValidity + 1) = &bRedEdit_MName;
					*(pbNameValidity + 2) = &bRedEdit_LName;
					if (AreNamesInvalid(hDlg, pbNameValidity))
					{
						if (**pbNameValidity)
							lstrcat(wszErrorMsg, TEXT("First"));
						if (*(*pbNameValidity + 1))
						{
							if(**pbNameValidity)
								lstrcat(wszErrorMsg, TEXT(", middle"));
							else
								lstrcat(wszErrorMsg, TEXT("Middle"));
						}
						if (*(*pbNameValidity + 2))
						{
							if (**pbNameValidity || *(*pbNameValidity + 1))
								lstrcat(wszErrorMsg, TEXT(", last"));
							else
								lstrcat(wszErrorMsg, TEXT("Last"));
						}

						lstrcat(wszErrorMsg, TEXT(" name(s) is/are invalid."));

						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszErrorMsg);
						EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					}
					else
					{
						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));
						EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					}

					SendMessage(GetDlgItem(hDlg, IDRB_REGISTERED), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hDlg, IDRB_RETIRING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendMessage(GetDlgItem(hDlg, IDRB_REGISTERING), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

					bNewUser = TRUE;

					bRedEdit_GB3 = FALSE;
					bRedEdit_Addr = FALSE;
					bRedEdit_FName = **pbNameValidity;
					bRedEdit_MName = *(*pbNameValidity + 1);
					bRedEdit_LName = *(*pbNameValidity + 2);
					bRedEdit_RegID = FALSE;
				}

				UpdateWindow(hDlg);
			}
			break;

		case IDPB_CANCEL:
			EndDialog(hDlg, 0);
			break;

		case IDPB_LOG:
			giCurrentUserId = GetDlgItemInt(hDlg, IDET_REGID, NULL, FALSE);

			if (gRegisteredUsersTable[giCurrentUserId] == 0)
			{
				MessageBox(hDlg, TEXT("User with that RegID has either retired or not been created yet."), TEXT("Invalid RegID"), MB_OK | MB_ICONERROR);
				EndDialog(hDlg, 0);
			}
			else
			{
				SendMessage(hParent, CUSTOM_PRINTLOG, 0, 0);
				EndDialog(hDlg, RET_MAINDLG_SHOWLOG);
			}
			break;

		default:
			break;
		}

		switch (HIWORD(wParam))
		{
		case EN_UPDATE:
			switch (LOWORD(wParam))
			{
			case IDET_REGID:
				bRedEdit_RegID = IsRegIDInvalid(hDlg, wszErrorMsg);

				if (bRedEdit_RegID)
				{
					if (bRegisteredUser)
						EnableGroupBoxThree(hDlg, FALSE);

					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszErrorMsg);

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);

					return(TRUE);
				}
				
				SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

				if (bRegisteredUser)
					EnableGroupBoxThree(hDlg, TRUE);

				EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
				UpdateWindow(hDlg);
				break;

			case IDET_FNAME:
				bRedEdit_FName = GetNameOrError(hDlg, INV_FNAME, wszNameOrError);
				if (bRedEdit_FName)
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszNameOrError);

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);

					return(TRUE);
				}

				wcscpy_s(szFNameUserEntry, wszNameOrError);
				
				if (!bRedEdit_MName && !bRedEdit_LName)
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
					UpdateWindow(hDlg);
				}
				if (bRedEdit_Addr)	// if address is invalid, means of submission must remain disabled
				{
					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);
				}
				break;

			case IDET_MNAME:
				bRedEdit_MName = GetNameOrError(hDlg, INV_MNAME, wszNameOrError);
				if (bRedEdit_MName)
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszNameOrError);

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);

					return(TRUE);
				}

				wcscpy_s(szMNameUserEntry, wszNameOrError);

				if (!bRedEdit_FName && !bRedEdit_LName)
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
					UpdateWindow(hDlg);
				}
				if (bRedEdit_Addr)
				{
					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);
				}
				break;

			case IDET_LNAME:
				bRedEdit_LName = GetNameOrError(hDlg, INV_LNAME, wszNameOrError);
				if (bRedEdit_LName)
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, wszNameOrError);

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);

					return(TRUE);
				}

				wcscpy_s(szLNameUserEntry, wszNameOrError);

				if (!bRedEdit_FName && !bRedEdit_MName)
				{
					SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

					EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
					UpdateWindow(hDlg);
				}
				if (bRedEdit_Addr)
				{
					EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
					EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
					UpdateWindow(hDlg);
				}
				break;

			case IDET_ADDITION_X:
			case IDET_ADDITION_Y:
			case IDET_SUBTRACTION_X:
			case IDET_SUBTRACTION_Y:
			case IDET_MULTIPLICATION_X:
			case IDET_MULTIPLICATION_Y:
			case IDET_DIVISION_X:
			case IDET_DIVISION_Y:
			case IDET_SQUARE_X:
			case IDET_CUBE_X:
			case IDET_SQUAREROOT_X:
			case IDET_CUBEROOT_X:
			case IDET_SINE_X:
			case IDET_COSINE_X:
				GetDlgItemText(hDlg, LOWORD(wParam), wszGB3Operand, 5);
				for (iSpare = 0; iSpare < lstrlen(wszGB3Operand); iSpare++)
				{
					if (iswdigit(wszGB3Operand[iSpare]) == 0 && wszGB3Operand[iSpare] != '-' && wszGB3Operand[iSpare] != '.')
					{
						bRedEdit_GB3 = TRUE;
						hwndErroneousOperand = (HWND)lParam;

						SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT("Operands cannot contain letters or special characters."));

						EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
						EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
						UpdateWindow(hDlg);

						return(TRUE);
					}

					bRedEdit_GB3 = FALSE;
					hwndErroneousOperand = NULL;
				}

				SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

				EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
				UpdateWindow(hDlg);
				break;

			default:
				break;
			}
			break;

		case EN_KILLFOCUS:
			/**
			 * the purpose of processing this notification is to prevent the
			 * caret from moving in a group box that has atleast 1 erroneously
			 * filled control, while still letting the user switch group boxes
			 */
			switch (LOWORD(wParam))
			{
			case IDET_FNAME:
				if (bRedEdit_FName)
				{
					hwndLastInFocus = SetFocus((HWND)lParam);

					if (hwndLastInFocus == GetDlgItem(hDlg, IDPB_CANCEL))
						SendMessage(hDlg, WM_COMMAND, (WPARAM)IDPB_CANCEL, (LPARAM)GetDlgItem(hDlg, IDPB_CANCEL));
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_REGISTERED))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_REGISTERED, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_REGISTERED));
					}
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_RETIRING))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_RETIRING, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_RETIRING));
					}
				}
				else
					SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)szFNameUserEntry);
				break;

			case IDET_MNAME:
				if (bRedEdit_MName)
				{
					hwndLastInFocus = SetFocus((HWND)lParam);

					if (hwndLastInFocus == GetDlgItem(hDlg, IDPB_CANCEL))
						SendMessage(hDlg, WM_COMMAND, (WPARAM)IDPB_CANCEL, (LPARAM)GetDlgItem(hDlg, IDPB_CANCEL));
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_REGISTERED))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_REGISTERED, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_REGISTERED));
					}
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_RETIRING))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_RETIRING, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_RETIRING));
					}
				}
				else
					SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)szMNameUserEntry);
				break;

			case IDET_LNAME:
				if (bRedEdit_LName)
				{
					hwndLastInFocus = SetFocus((HWND)lParam);

					if (hwndLastInFocus == GetDlgItem(hDlg, IDPB_CANCEL))
						SendMessage(hDlg, WM_COMMAND, (WPARAM)IDPB_CANCEL, (LPARAM)GetDlgItem(hDlg, IDPB_CANCEL));
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_REGISTERED))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_REGISTERED, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_REGISTERED));
					}
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_RETIRING))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_RETIRING, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_RETIRING));
					}
				}
				else
					SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)szLNameUserEntry);
				break;

			case IDET_ADDITION_X:
			case IDET_ADDITION_Y:
			case IDET_SUBTRACTION_X:
			case IDET_SUBTRACTION_Y:
			case IDET_MULTIPLICATION_X:
			case IDET_MULTIPLICATION_Y:
			case IDET_DIVISION_X:
			case IDET_DIVISION_Y:
			case IDET_SQUARE_X:
			case IDET_CUBE_X:
			case IDET_SQUAREROOT_X:
			case IDET_CUBEROOT_X:
			case IDET_SINE_X:
			case IDET_COSINE_X:
				if (bRedEdit_GB3)
				{
					hwndLastInFocus = SetFocus((HWND)lParam);

					if (hwndLastInFocus == GetDlgItem(hDlg, IDPB_CANCEL))
						SendMessage(hDlg, WM_COMMAND, (WPARAM)IDPB_CANCEL, (LPARAM)GetDlgItem(hDlg, IDPB_CANCEL));
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_REGISTERED))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_REGISTERED, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_REGISTERED));
					}
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_RETIRING))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_RETIRING, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_RETIRING));
					}
				}
				break;

			default:
				break;
			}
			break;

		case CBN_SELCHANGE:
			/* Set the static text beside the combo box to the selected state's capital city */
			iSpare = SendMessage(GetDlgItem(hDlg, IDCOMBO_ADDRESS), CB_GETCURSEL, 0, 0);
			SetDlgItemText(hDlg, IDST_ADDRESS, pszAddresses[iSpare][1]);

			if (!bRedEdit_FName && !bRedEdit_MName && !bRedEdit_LName)
			{
				SetDlgItemText(hDlg, IDST_ERRORMESSAGE, TEXT(""));

				EnableWindow(GetDlgItem(hDlg, IDPB_OK), TRUE);
				EnableWindow(GetDlgItem(hDlg, IDPB_LOG), TRUE);
				UpdateWindow(hDlg);
			}
			break;

		case CBN_EDITUPDATE:
			iState = GetState(hDlg);
			if (iState == -1)
			{
				bRedEdit_Addr = TRUE;
				SetDlgItemText(hDlg, IDST_ADDRESS, TEXT("That state does not exist."));

				EnableWindow(GetDlgItem(hDlg, IDPB_OK), FALSE);
				EnableWindow(GetDlgItem(hDlg, IDPB_LOG), FALSE);
				UpdateWindow(hDlg);

				return(TRUE);
			}

			bRedEdit_Addr = FALSE;

			SendMessage(GetDlgItem(hDlg, IDCOMBO_ADDRESS), CB_SETCURSEL, (WPARAM)iState, 0); // set new selection to the typed state
			SendMessage(hDlg, WM_COMMAND,
				MAKEWPARAM(IDCOMBO_ADDRESS, CBN_SELCHANGE),
				(LPARAM)GetDlgItem(hDlg, IDCOMBO_ADDRESS));	// because CBN_SELCHANGE is not sent after CB_SETCURSEL, send it artificially
			break;

		case CBN_KILLFOCUS:
			switch (LOWORD(wParam))
			{
			case IDCOMBO_ADDRESS:
				if (bRedEdit_Addr)
				{
					hwndLastInFocus = SetFocus((HWND)lParam);

					if (hwndLastInFocus == GetDlgItem(hDlg, IDPB_CANCEL))
						SendMessage(hDlg, WM_COMMAND, (WPARAM)IDPB_CANCEL, (LPARAM)GetDlgItem(hDlg, IDPB_CANCEL));
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_REGISTERED))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_REGISTERED, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_REGISTERED));
					}
					else if (hwndLastInFocus == GetDlgItem(hDlg, IDRB_RETIRING))
					{
						SendMessage(hDlg, WM_COMMAND,
							MAKEWPARAM(IDRB_RETIRING, BN_CLICKED),
							(LPARAM)GetDlgItem(hDlg, IDRB_RETIRING));
					}
				}
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
		return(TRUE);

	case WM_CTLCOLOREDIT:
		if (bRedEdit_RegID && ((HWND)lParam == GetDlgItem(hDlg, IDET_REGID)))
		{
			if (!hbrEditText)
				hbrEditText = CreateSolidBrush(RGB(255, 127, 127));

			SetBkColor((HDC)wParam, RGB(255, 127, 127));
			return((INT_PTR)hbrEditText);
		}
		else if (bNewUser)
		{
			if (bRedEdit_FName && ((HWND)lParam == GetDlgItem(hDlg, IDET_FNAME)))
			{
				if (!hbrEditText)
					hbrEditText = CreateSolidBrush(RGB(255, 127, 127));

				SetBkColor((HDC)wParam, RGB(255, 127, 127));
				return((INT_PTR)hbrEditText);
			}
			else if (bRedEdit_MName && ((HWND)lParam == GetDlgItem(hDlg, IDET_MNAME)))
			{
				if (!hbrEditText)
					hbrEditText = CreateSolidBrush(RGB(255, 127, 127));

				SetBkColor((HDC)wParam, RGB(255, 127, 127));
				return((INT_PTR)hbrEditText);
			}
			else if (bRedEdit_LName && ((HWND)lParam == GetDlgItem(hDlg, IDET_LNAME)))
			{
				if (!hbrEditText)
					hbrEditText = CreateSolidBrush(RGB(255, 127, 127));

				SetBkColor((HDC)wParam, RGB(255, 127, 127));
				return((INT_PTR)hbrEditText);
			}
			else if (bRedEdit_Addr && ((HWND)lParam == GetDlgItem(hDlg, IDCOMBO_ADDRESS)))
			{
				if (!hbrEditText)
					hbrEditText = CreateSolidBrush(RGB(255, 127, 127));

				SetBkColor((HDC)wParam, RGB(255, 127, 127));
				return((INT_PTR)hbrEditText);
			}
		}

		if (bRedEdit_GB3)
		{
			if (!hbrEditText)
				hbrEditText = CreateSolidBrush(RGB(255, 127, 127));

			SetBkColor((HDC)wParam, RGB(255, 127, 127));
			return((INT_PTR)hbrEditText);
		}

		if (hbrEditText)
		{
			DeleteObject(hbrEditText);
			hbrEditText = NULL;
		}
		return((INT_PTR)GetStockObject(WHITE_BRUSH));

	case WM_DESTROY:
		DestroyToolTips(hwndToolTips);
		break;

	default:
		break;
	}

	return(FALSE);
}

int AddAddressesToComboBox(HWND hComboBox)
{
	// Local variable declarations
	int i = 0;

	// Code
	for (i = 0; i < 36; i++)
		SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)pszAddresses[i][0]);

	SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return(0);
}

void GetControlRectRelativeTo(HWND hwndParent, int iControlId, LPRECT prectControl)
{
	// Local variable declarations
	POINT pointUpperLeft = { 0, 0 }, pointBottomRight = { 0, 0 };
	RECT rectDeviceCoordControl = { 0, 0, 0, 0 };

	// Code
	// Get the control's coordinates relative to the screen device
	GetWindowRect(GetDlgItem(hwndParent, iControlId), &rectDeviceCoordControl);

	// To Convert these coordinates to those relative to the parent window,
	// first decompose the RECT into 2 points
	pointUpperLeft = { rectDeviceCoordControl.left, rectDeviceCoordControl.top };
	pointBottomRight = { rectDeviceCoordControl.right, rectDeviceCoordControl.bottom };

	// Now, using the following Win32 API, get the control's coordinates with
	// respect the the *PARENT* window (the tool & it's tip - both lie on the
	// parent, so you must have the control's parameters with respect neither
	// to itself, nor to the screen, but to it's parent)
	ScreenToClient(hwndParent, &pointUpperLeft);
	ScreenToClient(hwndParent, &pointBottomRight);

	// Finally, initialize the out parameter with its final values, which are
	// currently held by the 2 points
	prectControl->left = pointUpperLeft.x;
	prectControl->top = pointUpperLeft.y;
	prectControl->right = pointBottomRight.x;
	prectControl->bottom = pointBottomRight.y;
}

// Implementation of Locally Declared Functions
HWND AttachTipToRect(HWND hwndParent, LPRECT lpToolRect, LPWSTR lpszTip)
{
	// Local variable declarations
	HWND hwndTT = NULL;
	static TOOLINFO ti;
	static BOOL bInitialized = FALSE;

	// Code
	hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwndParent, NULL,
		ghInst, NULL);
	if (!hwndTT)
		return((HWND)NULL);

	SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	if (!bInitialized)
	{
		ZeroMemory(&ti, sizeof(TOOLINFO));
		ti.cbSize = sizeof(TOOLINFO);
		ti.hinst = ghInst;
		ti.hwnd = hwndParent;
		ti.uFlags = TTF_SUBCLASS | TTF_CENTERTIP;
		ti.lpszText = lpszTip;
		ti.rect.left = lpToolRect->left;
		ti.rect.top = lpToolRect->top;
		ti.rect.right = lpToolRect->right;
		ti.rect.bottom = lpToolRect->bottom;

		bInitialized = TRUE;
	}

	if (!SendMessage(hwndTT, TTM_ADDTOOL, (WPARAM)0, (LPARAM)(LPTOOLINFO)&ti))
		return((HWND)NULL);

	MessageBox(NULL, TEXT("Here"), NULL, NULL);

	return(hwndTT);
}

void DestroyToolTips(HWND *hwndTTs)
{
	// variable declarations
	int i;

	// code
	for (i = 0; i < AMC_COUNTOFOPERATIONS; i++)
	{
		if (hwndTTs[i])
		{
			DestroyWindow(hwndTTs[i]);
			hwndTTs[i] = NULL;
		}
	}
}

BOOL IsRegIDInvalid(HWND hDlg, TCHAR* szErrOut)
{
	// variable declarations
	TCHAR wszUserIdEntry[5];
	int i, iUserIdEntry;

	// code
	/* check if the entered Registration ID contains any special characters */
	GetDlgItemText(hDlg, IDET_REGID, wszUserIdEntry, 5);
	for (i = 0; i < lstrlen(wszUserIdEntry); i++)
	{
		if (iswdigit(wszUserIdEntry[i]) == 0)
		{
			wcscpy(szErrOut, TEXT("Registration ID cannot contain letters or special characters."));
			return(TRUE);
		}
	}

	/* check if the entered Registration ID is within the valid range or not */
	iUserIdEntry = GetDlgItemInt(hDlg, IDET_REGID, NULL, FALSE);
	if (iUserIdEntry < 1 || iUserIdEntry > 99)
	{
		wcscpy(szErrOut, TEXT("Registration ID must lie between 01 and 99."));
		return(TRUE);
	}

	/* check if the user exists or not */
	if (gRegisteredUsersTable[iUserIdEntry] == 0)
	{
		wcscpy(szErrOut, TEXT("User with that RegID has either retired or is yet to register."));
		return(TRUE);
	}
	
	wcscpy(szErrOut, TEXT(""));
	return(FALSE);	// return FALSE if no error
}

BOOL AreNamesInvalid(HWND hDlg, BOOL **ppbValidityOut)
{
	// function prototypes
	BOOL GetNameOrError(HWND, int, TCHAR *);

	// code
	**ppbValidityOut = GetNameOrError(hDlg, INV_FNAME, NULL);
	*(*ppbValidityOut + 1) = GetNameOrError(hDlg, INV_MNAME, NULL);
	*(*ppbValidityOut + 2) = GetNameOrError(hDlg, INV_LNAME, NULL);

	if (**ppbValidityOut || *(*ppbValidityOut + 1) || *(*ppbValidityOut + 2))
		return TRUE;
	else
		return FALSE;
}

BOOL GetNameOrError(HWND hDlg, int index, TCHAR *szOut)
{
	// variable declarations
	TCHAR szNameUserEntry[15];
	int i, iNameBegin, iNameEnd, iNameLength;

	// code
	switch (index)
	{
	case INV_FNAME:
		GetDlgItemText(hDlg, IDET_FNAME, szNameUserEntry, 15);
		break;
	case INV_MNAME:
		GetDlgItemText(hDlg, IDET_MNAME, szNameUserEntry, 15);
		break;
	case INV_LNAME:
		GetDlgItemText(hDlg, IDET_LNAME, szNameUserEntry, 15);
		break;
	default:
		break;
	}

	iNameBegin = 0;	// integer index at which the name actually begins
	while (szNameUserEntry[iNameBegin] == L' ' && iNameBegin < 15)
		iNameBegin++; // skip all spaces from the beginning

	// lstrlen() returns the last index and not the number of characters
	if (iNameBegin == lstrlen(szNameUserEntry))
	{
		if (szOut)
			wcscpy(szOut, TEXT("User name entry cannot remain empty."));

		return(TRUE);
	}

	// integer index at which the name actually ends
	iNameEnd = lstrlen(szNameUserEntry) - 1;
	while (szNameUserEntry[iNameEnd] == L' ' && iNameEnd >= 0)
		iNameEnd--;	// skip all spaces from the end

	// calculate the actual length of name
	iNameLength = (iNameEnd - iNameBegin + 1);

	// check for special characters in the name
	for (i = iNameBegin; i <= iNameEnd; i++)
	{
		if (iswalpha(szNameUserEntry[i]) == 0)
		{
			if (szOut)
				wcscpy(szOut, TEXT("User name cannot contain numbers or special characters."));

			return(TRUE);
		}
	}

	if (szOut)
	{
		// clip spaces from both ends
		for (i = 0; i < iNameLength; i++)
			szOut[i] = szNameUserEntry[i + iNameBegin];
		szOut[i] = L'\0';
	}

	return(FALSE);	// return FALSE when the name is valid
}

int GetState(HWND hDlg)
{
	// variable declarations
	TCHAR szInput[20];
	int i;

	// code
	// checking whether the string entered by the user matches any of the states
	GetDlgItemText(hDlg, IDCOMBO_ADDRESS, szInput, 20);
	CharUpper(szInput);
	for (i = 0; i < 36; i++)
	{
		if (wcscmp(szInput, pszAddresses[i][0]) == 0)
			return i;
	}

	return -1;
}

void EnableGroupBoxThree(HWND hDlg, BOOL bEnable)
{
	// code
	EnableWindow(GetDlgItem(hDlg, IDCB_ADDITION), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_ADDITION_X), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_ADDITION_Y), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_SUBTRACTION), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_SUBTRACTION_X), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_SUBTRACTION_Y), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_MULTIPLICATION), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_MULTIPLICATION_X), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_MULTIPLICATION_Y), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_DIVISION), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_DIVISION_X), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_DIVISION_Y), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_SQUARE), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_SQUARE_X), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_CUBE), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_CUBE_X), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_SQUAREROOT), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_SQUAREROOT_X), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_CUBEROOT), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_CUBEROOT_X), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_SINE), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_SINE_X), bEnable);

	EnableWindow(GetDlgItem(hDlg, IDCB_COSINE), bEnable);
	EnableWindow(GetDlgItem(hDlg, IDET_COSINE_X), bEnable);

	UpdateWindow(hDlg);
}

HRESULT GetRequiredInterfaces(HWND hDlg)
{
	TCHAR str[100];
	HRESULT hr;

	// Getting interfaces exposing services the user has requested for
	// Interfaces exposed by component Basic 1: ISum, ISubtract
	if (SendMessage(GetDlgItem(hDlg, IDCB_ADDITION), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		hr = CoCreateInstance(CLSID_BasicOne, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&gpISum);
		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface ISum\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}
	if (SendMessage(GetDlgItem(hDlg, IDCB_SUBTRACTION), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		if (gpISum)
			hr = gpISum->QueryInterface(IID_ISubtract, (void **)&gpISubtract);
		else
			hr = CoCreateInstance(CLSID_BasicOne, NULL, CLSCTX_INPROC_SERVER, IID_ISubtract, (void **)&gpISubtract);

		if (FAILED(hr))
		{
			wsprintf(str, TEXT("Cannot obtain interface ISubtract\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}

	// Interfaces exposed by component Basic 2: IMultiply, IDivide
	if (SendMessage(GetDlgItem(hDlg, IDCB_MULTIPLICATION), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		hr = CoCreateInstance(CLSID_BasicTwo, NULL, CLSCTX_INPROC_SERVER, IID_IMultiply, (void **)&gpIMultiply);
		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface IMultiply\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}
	if (SendMessage(GetDlgItem(hDlg, IDCB_DIVISION), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		if (gpIMultiply)
			hr = gpIMultiply->QueryInterface(IID_IDivide, (void **)&gpIDivide);
		else
			hr = CoCreateInstance(CLSID_BasicTwo, NULL, CLSCTX_INPROC_SERVER, IID_IDivide, (void **)&gpIDivide);

		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface IDivide\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}

	// Interfaces exposed by component Power 1: ISquare, ICube
	if (SendMessage(GetDlgItem(hDlg, IDCB_SQUARE), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		hr = CoCreateInstance(CLSID_PowerOne, NULL, CLSCTX_INPROC_SERVER, IID_ISquare, (void **)&gpISquare);
		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface ISquare\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}
	if (SendMessage(GetDlgItem(hDlg, IDCB_CUBE), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		if (gpISquare)
			hr = gpISquare->QueryInterface(IID_ICube, (void **)&gpICube);
		else
			hr = CoCreateInstance(CLSID_PowerOne, NULL, CLSCTX_INPROC_SERVER, IID_ICube, (void **)&gpICube);

		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface ICube\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}

	// Interface exposed by component Power 2
	if ((SendMessage(GetDlgItem(hDlg, IDCB_SQUAREROOT), BM_GETCHECK, 0, 0) == BST_CHECKED) ||
		(SendMessage(GetDlgItem(hDlg, IDCB_CUBEROOT), BM_GETCHECK, 0, 0) == BST_CHECKED))
	{
		hr = CoCreateInstance(CLSID_PowerTwo, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&gpIDispatchPowerTwo);
		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface IDispatch for Power 2\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}

	// For the dotnet server
	if ((SendMessage(GetDlgItem(hDlg, IDCB_SINE), BM_GETCHECK, 0, 0) == BST_CHECKED) ||
		(SendMessage(GetDlgItem(hDlg, IDCB_COSINE), BM_GETCHECK, 0, 0) == BST_CHECKED))
	{
		hr = CoCreateInstance(CLSID_DotNetTrigServer, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&gpIDispatchTrig);
		if (FAILED(hr))
		{
			wsprintf(str, TEXT("CoCreateInstance(): cannot obtain interface IDispatch for the .Net Server\nReturn Code: 0x%x"), hr);
			MessageBox(hDlg, str, TEXT("COM Error"), MB_OK | MB_ICONERROR);
			return(hr);
		}
	}

	return(hr);
}

void SafeInterfaceRelease(void)
{
	// code
	if (gpIDispatchTrig)
	{
		gpIDispatchTrig->Release();
		gpIDispatchTrig = NULL;
	}
	if (gpIDispatchPowerTwo)
	{
		gpIDispatchPowerTwo->Release();
		gpIDispatchPowerTwo = NULL;
	}
	if (gpICube)
	{
		gpICube->Release();
		gpICube = NULL;
	}
	if (gpISquare)
	{
		gpISquare->Release();
		gpISquare = NULL;
	}
	if (gpIDivide)
	{
		gpIDivide->Release();
		gpIDivide = NULL;
	}
	if (gpIMultiply)
	{
		gpIMultiply->Release();
		gpIMultiply = NULL;
	}
	if (gpISubtract)
	{
		gpISubtract->Release();
		gpISubtract = NULL;
	}
	if (gpISum)
	{
		gpISum->Release();
		gpISum = NULL;
	}
}

int AppendResult(TCHAR *result)
{
	// local variable declarations
	static int size = 0;
	int i = 0;

	// Code
	while (gszResult[size] != '\0')
		size++;

	while (result[i] != '\0')
		gszResult[size++] = result[i++];

	gszResult[size] = '\n';
	gszResult[size + 1] = '\0';

	return size;
}

int AppendLog(UINT iCurrentUser, TCHAR *wszOperation)
{
	// local variable declarations
	SYSTEMTIME LocalTime;
	FILE *stream;
	char szFilename[17];
	char szTimestamp[40];
	char szOperation[20];
	int ret;

	// code
	GetLocalTime(&LocalTime);
	sprintf_s(szTimestamp,
		40,
		"%02d-%02d-%02d %02d:%02d:%02d\0",
		LocalTime.wDay,
		LocalTime.wMonth,
		LocalTime.wYear,
		LocalTime.wHour,
		LocalTime.wMinute,
		LocalTime.wSecond);

	wcstombs(szOperation, wszOperation, 20);

	sprintf(szFilename, ".\\Users\\%u.amcur\0", iCurrentUser);
	stream = fopen(szFilename, "a");
	ret = fprintf(stream, "\n%s %s", szTimestamp, szOperation);
	fclose(stream);

	return ret;
}

int ShowResult(HWND hDlg)
{
	// Code
	DialogBox(ghInst, TEXT("RESULT"), hDlg, (DLGPROC)ResultDlgProc);
	return(0);
}

BOOL CALLBACK ResultDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Local variable declarations
	static HMENU hMenu;
	static HBRUSH hDlgBrush;
	static RECT rbResult;	// Result text's bounding rectangle
	HDC hdc;
	PAINTSTRUCT ps;

	// Code
	switch (iMsg)
	{
	case WM_INITDIALOG:
		/* Adding the Menu */
		hMenu = LoadMenu(ghInst, TEXT("ContentsResultDlg"));
		SetMenu(hDlg, hMenu);

		hDlgBrush = CreateSolidBrush(RGB(0xff, 0xf9, 0xa6));

		/* Setting up the necessary parameters to display the result text */
		GetClientRect(hDlg, &rbResult);
		rbResult.left = rbResult.left + 10;
		rbResult.top = rbResult.top + 10;
		rbResult.right = rbResult.right - (rbResult.right / 30);
		rbResult.bottom = rbResult.bottom - (rbResult.bottom / 15);
		return(TRUE);

	case WM_CTLCOLORDLG:
		return((INT_PTR)hDlgBrush);

	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);

		/* Drawing the result text using system font for the Windows-native feel */
		SetBkColor(hdc, RGB(0xff, 0xf9, 0xa6));
		SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
		DrawText(hdc, gszResult, -1, &rbResult, DT_LEFT | DT_TOP | DT_WORDBREAK);
		EndPaint(hDlg, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDPB_RDLG_OK:
			DeleteObject(hDlgBrush);
			EndDialog(hDlg, 0);
			break;

		case IDMI_RDLG_ABOUT:
			DialogBox(ghInst, TEXT("About"), hDlg, (DLGPROC)AboutDlgProc);
			break;

		case IDMI_RDLG_QUIT:
			EndDialog(hDlg, 0);
			break;

		default:
			break;
		}
		return(TRUE);

	case WM_DESTROY:
		if (hMenu)
		{
			DestroyMenu(hMenu);
			hMenu = NULL;
		}
		break;

	default:
		break;
	}

	return(FALSE);
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Local variable declarations
	static HBRUSH hDlgBrush = NULL;
	HDC hdc;

	// Code
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return(TRUE);

	case WM_CTLCOLORDLG:
		/* Colouring the dialog box */
		if (!hDlgBrush)
			hDlgBrush = CreateSolidBrush(RGB(0xa, 0xa9, 0xa6));
		return((INT_PTR)hDlgBrush);

	case WM_CTLCOLORSTATIC:
		/* Colouring all static controls (in this dlg, the static controls are static texts) */
		hdc = (HDC)wParam;
		SetBkColor(hdc, RGB(0xa, 0xa9, 0xa6));
		if (!hDlgBrush)
			hDlgBrush = CreateSolidBrush(RGB(0xa, 0xa9, 0xa6));
		return((INT_PTR)hDlgBrush);

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDPB_ADLG_EXIT:
			if (hDlgBrush)
			{
				DeleteObject(hDlgBrush);
				hDlgBrush = NULL;
			}
			EndDialog(hDlg, 0);
			break;
		}
		return(TRUE);

	default:
		break;
	}

	return(FALSE);
}

FILE *CreateUserFile(int iUserId, TCHAR *szUserFName, TCHAR *szUserMName, TCHAR *szUserLName, TCHAR *szAddressState, TCHAR *szAddressCity)
{
	// Local variable declarations
	FILE *fdUser;
	SYSTEMTIME LocalTime;
	size_t sizeFilename, sizeFullName, sizeAddress;
	TCHAR wszFilename[20], wszFullName[50], wszAddress[50];
	char szFilename[20], szFullName[50], szAddress[50], szTimestamp[40];
	char szSeparator[] = "============================================\n\0";

	// Code
	swprintf_s(wszFilename, 20, TEXT("./Users/%d.amcur\0"), iUserId);
	wcstombs_s(&sizeFilename, szFilename, sizeof(szFilename) / sizeof(char), wszFilename, lstrlen(wszFilename));

	swprintf_s(wszFullName, 50, TEXT("%s %s %s\n\0"), szUserFName, szUserMName, szUserLName);
	wcstombs_s(&sizeFullName, szFullName, sizeof(szFullName) / sizeof(char), wszFullName, lstrlen(wszFullName));

	swprintf_s(wszAddress, 50, TEXT("%s, %s\n\0"), szAddressCity, szAddressState);
	wcstombs_s(&sizeAddress, szAddress, sizeof(szAddress) / sizeof(char), wszAddress, lstrlen(wszAddress));

	GetLocalTime(&LocalTime);
	sprintf_s(szTimestamp,
		40,
		"%02d-%02d-%02d %02d:%02d:%02d\n\0",
		LocalTime.wDay,
		LocalTime.wMonth,
		LocalTime.wYear,
		LocalTime.wHour,
		LocalTime.wMinute,
		LocalTime.wSecond);

	fdUser = fopen(szFilename, "w");
	fwrite(szFullName, strlen(szFullName), sizeof(char), fdUser);
	fwrite(szAddress, strlen(szAddress), sizeof(char), fdUser);
	fwrite(szTimestamp, strlen(szTimestamp), sizeof(char), fdUser);
	fwrite(szSeparator, strlen(szSeparator), sizeof(char), fdUser);
	fclose(fdUser);

	return(fdUser);
}

void EnumerateRegisteredUserIDs(void)
{
	// Local variable declarations
	int id;
	char iFirstEmptySlot = 0;
	char szFilename[20], temp[_MAX_ITOSTR_BASE10_COUNT];
	INT_PTR ipSearchHandle;
	_finddata_t finddata;

	// Code
	for (id = 1; id < 99; id++)
	{
		strcpy(szFilename, "./Users/");
		itoa(id, temp, 10);
		strcat(szFilename, temp);
		strcat(szFilename, ".amcur");

		ipSearchHandle = _findfirst(szFilename, &finddata);
		if (ipSearchHandle == -1)
		{
			if (errno == ENOENT)
			{
				// the filename that causes _findfirst() to fail with ENOENT when iFirstEmptySlot
				// is not set is the first filename that is not occupied, and thus the right
				// candidate for a new user's ID
				if (!iFirstEmptySlot)
					iFirstEmptySlot = id;

				gRegisteredUsersTable[id] = 0;
				memset(szFilename, 0, strlen(szFilename));
				_findclose(ipSearchHandle);
				continue;
			}
		}

		gRegisteredUsersTable[id] = 1;

		memset(szFilename, 0, strlen(szFilename));
		_findclose(ipSearchHandle);
	}

	gNewUserId = iFirstEmptySlot;
}
