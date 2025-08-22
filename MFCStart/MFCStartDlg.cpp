
// MFCStartDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCStart.h"
#include "MFCStartDlg.h"
#include "afxdialogex.h"



// 아래 세 줄을 통해서 콘솔창에 디버깅을 출력할 수 있음
#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
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


// CMFCStartDlg 대화 상자



CMFCStartDlg::CMFCStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSTART_DIALOG, pParent)
	, m_nNum(0)
	, m_nNum2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// Edit IDC_EDIT_NAME <-> m_name
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_AGE, m_age);
	DDV_MinMaxInt(pDX, m_age, 0, 120); // 검증은 DDX 뒤에
	DDX_Text(pDX, IDC_EDIT_NUM, m_nNum);
	DDX_Text(pDX, IDC_EDIT_NUM2, m_nNum2);

	// CString 멤버 변수 선언한 다음에 바인딩 해주기
	DDX_Text(pDX, IDC_STATIC_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(CMFCStartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, &CMFCStartDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_OK, &CMFCStartDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// CMFCStartDlg 메시지 처리기

BOOL CMFCStartDlg::OnInitDialog()
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

	
	// 초기화 작업
	SetDlgItemText(IDC_STATIC_RESULT, _T("0"));

	m_name = _T("홍길동");
	m_age = 20;
	UpdateData(FALSE); // 변수 → UI로 뿌리기

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCStartDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCStartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCStartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include "iostream"
using namespace std;

void CMFCStartDlg::OnBnClickedBtnTest()
{

	// 프로그램에서 = 은 변수에다가 값을 넣겠다는 말이 된다.
	//int nTest = 85;
	//m_nNum = 99; 		
	//m_nNum = nTest;

	UpdateData(TRUE);
	int result = m_nNum + m_nNum2;

	//int nSum = 0;
	//// 이거 C++ 에서 편하게 쓰는 또 다른 for문 있었는데
	//for (int i = 0; i < m_nNum; i++) {
	//	nSum += i;
	//	cout << "값: " << i << endl;
	//}
	//m_nNum = nSum;
	//SetDlgItemText(IDC_STATIC_RESULT, _T(result));
	 
	m_result.Format(_T("%d"), result); // int → Cstring

	// 위에처럼만 하고 실행시켜보면 m_nNum의 값이 바뀌지 않는다 항상 UpdataData를 해줘야 한다.
	UpdateData(false);


}

void CMFCStartDlg::OnBnClickedBtnOk() {
	// UI → 변수로 가져오면서 동시에 DDV 검증 수행
	if (!UpdateData(TRUE)) return;
	
	if (m_name.IsEmpty()) {
		AfxMessageBox(_T("이름을 입력해주세요"));
		return;
	}

	CString msg;
	msg.Format(_T("%s님의 나이는 %d세입니다."), m_name, m_age);
	AfxMessageBox(msg);

	// 성공적으로 읽었으면 UI에 다시 친절 메시지 넣기
	m_name.Format(_T("%s (확인됨)"), m_name);
	UpdateData(FALSE); // 변수 → UI 
}