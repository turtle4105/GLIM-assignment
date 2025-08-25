﻿
// Glim_AssignmentDlg.cpp: 구현 파일
#include "pch.h"
#include "framework.h"
#include "Glim_Assignment.h"
#include "Glim_AssignmentDlg.h"
#include "afxdialogex.h"

#include <atlimage.h>
#include <cstring>

#include "AppState.h"
#include "Circumcircle.h"
#include "RasterCircle.h"
#include "AutoMover.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CGlimAssignmentDlg 대화 상자



CGlimAssignmentDlg::CGlimAssignmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_ASSIGNMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlimAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGlimAssignmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_PAINT()
	ON_MESSAGE(WM_APP_AUTOTICK, &CGlimAssignmentDlg::OnAppAutoTick)
END_MESSAGE_MAP()


// CGlimAssignmentDlg 메시지 처리기

BOOL CGlimAssignmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	GetClientRect(&m_state.canvas);
	EnsureBackBuffer();
	ClearBackBuffer();
	Present();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGlimAssignmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGlimAssignmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGlimAssignmentDlg::EnsureBackBuffer() {
	if (m_img) m_img.Destroy();
	m_img.Create(m_state.bufW, -m_state.bufH, m_state.bufBpp); // -height = 상하 반전
	if (m_state.bufBpp == 8) {
		static RGBQUAD tbl[256];
		for (int i = 0;i < 256;++i) tbl[i] = { (BYTE)i,(BYTE)i,(BYTE)i,0 };
		m_img.SetColorTable(0, 256, tbl);
	}
}
void CGlimAssignmentDlg::ClearBackBuffer() {
	memset(m_img.GetBits(), 0xFF, m_state.bufW * m_state.bufH);
}
void CGlimAssignmentDlg::Present() {
	CClientDC dc(this);
	m_img.Draw(dc, 0, 0);
}
void CGlimAssignmentDlg::RenderToBackBuffer() {
	ClearBackBuffer();
	// 아직 그림 없음(마커/원은 다음 단계에)
}

void CGlimAssignmentDlg::OnPaint()
{
	if (IsIconic()) { CDialogEx::OnPaint(); return; }
	std::lock_guard<std::mutex> lk(m_mtx);
	RenderToBackBuffer();
	Present();
}

LRESULT CGlimAssignmentDlg::OnAppAutoTick(WPARAM, LPARAM) {
	Invalidate(FALSE);
	return 0;
}
