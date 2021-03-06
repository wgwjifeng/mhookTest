
// HookTestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HookTest.h"
#include "HookTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHookTestDlg 对话框



CHookTestDlg::CHookTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HOOKTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHookTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHookTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ON, &CHookTestDlg::OnBnClickedButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &CHookTestDlg::OnBnClickedButtonOff)
	ON_BN_CLICKED(IDC_BUTTON_BOX, &CHookTestDlg::OnBnClickedButtonBox)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CHookTestDlg::OnBnClickedButtonWrite)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CHookTestDlg 消息处理程序

BOOL CHookTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHookTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHookTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHookTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void sysprintA(LPCSTR format, ...)
{
	va_list ap;
	va_start(ap, format);
	int iNeed = _vscprintf(format, ap) + 1;
	char *pszOut = new char[iNeed];
	pszOut[iNeed - 1] = 0;
	vsprintf_s(pszOut, iNeed, format, ap);
	OutputDebugStringA(pszOut);
	va_end(ap);
	delete[] pszOut;
}

void sysprintW(LPCWSTR format, ...)
{
	va_list ap;
	va_start(ap, format);
	int iNeed = _vscwprintf(format, ap) + 1;
	WCHAR *pszOut = new WCHAR[iNeed];
	pszOut[iNeed - 1] = 0;
	vswprintf_s(pszOut, iNeed, format, ap);
	OutputDebugStringW(pszOut);
	va_end(ap);
	delete[] pszOut;
}


HMODULE g_dll = NULL;

typedef BOOL (CALLBACK *_HookInstall)(void);
typedef BOOL (CALLBACK *_HookUnstall)(void);

void CHookTestDlg::OnBnClickedButtonOn()
{
	// TODO: 在此添加控件通知处理程序代码

	if (g_dll != NULL) 
		return;


	g_dll = LoadLibrary(_T("HookDll"));
	if (NULL == g_dll)
	{
		AfxMessageBox(_T("加载 dll 失败"));
	}

	_HookInstall HookInstall = (_HookInstall)::GetProcAddress(g_dll, "HookInstall");

	if (HookInstall() != NO_ERROR)
	{
		AfxMessageBox(_T("钩子安装失败 ！"));
		::FreeLibrary(g_dll);
		g_dll = NULL;
		return;
	}


	AfxMessageBox(_T("钩子安装成功"));
}

void CHookTestDlg::OnBnClickedButtonOff()
{
	// TODO: 在此添加控件通知处理程序代码

	if (g_dll == NULL)
		return;

	_HookUnstall HookUnstall = (_HookUnstall)::GetProcAddress(g_dll, "HookUnstall");

	if (HookUnstall() != NO_ERROR)
	{
		AfxMessageBox(_T("钩子卸载失败"));
	}

	::FreeLibrary(g_dll);
	g_dll = NULL;

}

void CHookTestDlg::OnBnClickedButtonBox()
{
	// TODO: 在此添加控件通知处理程序代码
	::MessageBoxA(this->GetSafeHwnd(), "", "", 0);
	::MessageBoxW(this->GetSafeHwnd(), L"", L"", 0);

}

#include <tchar.h>
#include <string.h>
#include <psapi.h>
#include <strsafe.h>


#define BUFSIZE 512
#pragma comment(lib, "PSAPI.lib")


//BOOL GetFileNameFromHandle(HANDLE hFile)
//{
//	BOOL bSuccess = FALSE;
//	TCHAR pszFilename[MAX_PATH + 1];
//	HANDLE hFileMap;
//
//	// Get the file size.
//	DWORD dwFileSizeHi = 0;
//	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);
//
//	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
//	{
//		printf("Cannot map a file with a length of zero.\n");
//		return FALSE;
//	}
//
//	// Create a file mapping object.
//	hFileMap = CreateFileMapping(hFile,
//		NULL,
//		PAGE_READONLY,
//		0,
//		0,
//		NULL);
//
//	if (hFileMap)
//	{
//		// Create a file mapping to get the file name.
//		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
//
//		if (pMem)
//		{
//			if (GetMappedFileName(GetCurrentProcess(),
//				pMem,
//				pszFilename,
//				MAX_PATH))
//			{
//
//				// Translate path with device name to drive letters.
//				TCHAR szTemp[BUFSIZE];
//				szTemp[0] = '\0';
//
//				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
//				{
//					TCHAR szName[MAX_PATH];
//					TCHAR szDrive[3] = TEXT(" :");
//					BOOL bFound = FALSE;
//					TCHAR* p = szTemp;
//
//					do
//					{
//						// Copy the drive letter to the template string
//						*szDrive = *p;
//
//						// Look up each device name
//						if (QueryDosDevice(szDrive, szName, BUFSIZE))
//						{
//							UINT uNameLen = _tcslen(szName);
//
//							if (uNameLen < MAX_PATH)
//							{
//								bFound = _tcsnicmp(pszFilename, szName,
//									uNameLen) == 0;
//
//								if (bFound)
//								{
//									// Reconstruct pszFilename using szTempFile
//									// Replace device path with DOS path
//									TCHAR szTempFile[MAX_PATH];
//									StringCchPrintf(szTempFile,
//										MAX_PATH,
//										TEXT("%s%s"),
//										szDrive,
//										pszFilename + uNameLen);
//									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
//								}
//							}
//						}
//
//						// Go to the next NULL character.
//						while (*p++);
//					} while (!bFound && *p); // end of string
//				}
//			}
//			bSuccess = TRUE;
//			UnmapViewOfFile(pMem);
//		}
//
//		CloseHandle(hFileMap);
//	}
//	_tprintf(TEXT("File name is %s\n"), pszFilename);
//	MessageBox(GetActiveWindow(), pszFilename, L"文件名", MB_OK | MB_ICONINFORMATION);
//	return(bSuccess);
//}
BOOL GetFileNameFromHandle(HANDLE hFile)
{
	BOOL bSuccess = FALSE;
	TCHAR pszFilename[MAX_PATH + 1];
	HANDLE hFileMap;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
		printf("Cannot map a file with a length of zero.\n");
		return FALSE;
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);

	if (hFileMap == NULL)
		sysprintA("fail to CreateFileMapping() %d \r\n", GetLastError());

	if (hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(),
				pMem,
				pszFilename,
				MAX_PATH))
			{

				// Translate path with device name to drive letters.
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';

				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(":");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, BUFSIZE))
						{
							UINT uNameLen = _tcslen(szName);

							if (uNameLen < MAX_PATH)
							{
								bFound = _tcsnicmp(pszFilename, szName,
									uNameLen) == 0;

								if (bFound)
								{
									// Reconstruct pszFilename using szTemp
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH];
									_stprintf_s(szTempFile,
										TEXT("%s%s"),
										szDrive,
										pszFilename + uNameLen);
									_tcsncpy(pszFilename, szTempFile, MAX_PATH);
								}
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}
	sysprintW(L"File name is %s\n", pszFilename);

	AfxMessageBox(pszFilename, 0, 0);

	return(bSuccess);
}
void WriteLog(const char* str, const char* path = ".\\test.log")
{
	HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
		return;
	//GetFileNameFromHandle(hFile);
	//设置文件中进行读写的当前位置
	//_llseek((HFILE)hFile, 0, SEEK_END);
	DWORD dw;
	WriteFile(hFile, str, strlen(str), &dw, NULL);
	_lclose((HFILE)hFile);
}

void CHookTestDlg::OnBnClickedButtonWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteLog("1234 \r\n");
}


BOOL CHookTestDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (pCopyDataStruct->lpData != NULL && pCopyDataStruct->cbData > 0)
		sysprintA("%s", pCopyDataStruct->lpData);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
