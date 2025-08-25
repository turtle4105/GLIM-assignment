
// Glim_AssignmentDlg.cpp: 구현 파일
#include "pch.h"
#include "framework.h"
#include "Glim_Assignment.h"
#include "Glim_AssignmentDlg.h"
#include "afxdialogex.h"

#include <atlimage.h>
#include <cstring>
#include <cmath>  


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

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(WM_APP_AUTOTICK, &CGlimAssignmentDlg::OnAppAutoTick)
	ON_BN_CLICKED(IDC_BTN_INIT, &CGlimAssignmentDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CGlimAssignmentDlg::OnBnClickedRandom)
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
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
// 아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
// 이 함수를 호출합니다.
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

	// 클릭된 마커를 순서대로 표시 (회색)
	int phaseIdx = 0;
	if (m_state.phase == Phase::Waiting1) phaseIdx = 0;
	else if (m_state.phase == Phase::Waiting2) phaseIdx = 1;
	else if (m_state.phase == Phase::Waiting3) phaseIdx = 2;
	else /*Ready*/ phaseIdx = 3;

	for (int i = 0; i < phaseIdx && i < 3; ++i) {
		Raster::FillMarker(m_img, m_state.P[i].x, m_state.P[i].y, m_state.markerR, 160);
	}

	// 외접원(3점 지정 완료 시): 내부 비채움, 테두리 두께 적용
	if (m_state.hasCircle) {
		Raster::DrawCircleThick(
			m_img,
			(int)std::lround(m_state.circle.cx),
			(int)std::lround(m_state.circle.cy),
			(int)std::lround(m_state.circle.R),
			m_state.thickness,
			0   // 검정
		);
	}
}


void CGlimAssignmentDlg::OnPaint()
{
	if (IsIconic()) { CDialogEx::OnPaint(); return; }
	std::lock_guard<std::mutex> lk(m_mtx);
	RenderToBackBuffer();
	Present();
	UpdateCenterText();
}

LRESULT CGlimAssignmentDlg::OnAppAutoTick(WPARAM, LPARAM) {
	Invalidate(FALSE);
	return 0;
}


void CGlimAssignmentDlg::OnLButtonDown(UINT, CPoint pt) {
	std::lock_guard<std::mutex> lk(m_mtx);

	if (m_state.phase != Phase::Ready) {
		// 1~3번째 클릭 수집
		int idx =
			(m_state.phase == Phase::Waiting1) ? 0 :
			(m_state.phase == Phase::Waiting2) ? 1 : 2;

		m_state.P[idx] = { pt.x, pt.y };
		if (m_state.phase == Phase::Waiting1) m_state.phase = Phase::Waiting2;
		else if (m_state.phase == Phase::Waiting2) m_state.phase = Phase::Waiting3;
		else {
			// 세 번째 클릭: 외접원 계산
			if (MathCirc::ComputeCircumcircle(m_state.P[0], m_state.P[1], m_state.P[2], m_state.circle)) {
				m_state.hasCircle = true;
				m_state.phase = Phase::Ready;
			}
			else {
				// 일직선 → 3번째 클릭 무효, 다시 3번째 대기
				m_state.phase = Phase::Waiting3;
			}
		}
		Invalidate(FALSE);
		return;
	}
}

void CGlimAssignmentDlg::OnMouseMove(UINT, CPoint) {
	
}

void CGlimAssignmentDlg::OnLButtonUp(UINT, CPoint) {
	
}

bool CGlimAssignmentDlg::HitTestMarker(const Pt& p, const CPoint& m, int r) {
	int dx = m.x - p.x, dy = m.y - p.y;
	return (dx * dx + dy * dy) <= r * r;
}

void CGlimAssignmentDlg::RecomputeCircle() {
	if (MathCirc::ComputeCircumcircle(m_state.P[0], m_state.P[1], m_state.P[2], m_state.circle))
		m_state.hasCircle = true;
	else
		m_state.hasCircle = false;
}

void CGlimAssignmentDlg::ReadInputs() {
	BOOL okR = FALSE, okT = FALSE;
	int r = GetDlgItemInt(IDC_EDIT_RADIUS, &okR, FALSE);
	int t = GetDlgItemInt(IDC_EDIT_THICKNESS, &okT, FALSE);

	// 기본값/범위 가드
	if (!okR || r <= 0) r = 10;
	if (!okT || t <= 0) t = 2;
	r = max(3, min(r, 200));   // 너무 작은/큰 값 제한
	t = max(1, min(t, 20));

	m_state.markerR = r;
	m_state.thickness = t;

	// 에디트에 정제된 값 재반영(사용자 오입력 보정)
	SetDlgItemInt(IDC_EDIT_RADIUS, r, FALSE);
	SetDlgItemInt(IDC_EDIT_THICKNESS, t, FALSE);
}

void CGlimAssignmentDlg::UpdateCenterText() {
	if (m_state.hasCircle) {
		CString s;
		s.Format(L"(%.1f, %.1f)  R=%.1f", m_state.circle.cx, m_state.circle.cy, m_state.circle.R);
		SetDlgItemText(IDC_STATIC_CENTER, s);
	}
	else {
		SetDlgItemText(IDC_STATIC_CENTER, L"-");
	}
}

void CGlimAssignmentDlg::OnBnClickedInit()
{
	std::lock_guard<std::mutex> lk(m_mtx);
	ReadInputs();     // 사용자 입력 반영
	// 상태 완전 초기화 (클릭 0회 상태로)
	m_state.phase = Phase::Waiting1;
	m_state.hasCircle = false;
	m_state.dragging = false;
	m_state.dragHit = -1;
	// 화면 클리어
	ClearBackBuffer();
	Present();
	UpdateCenterText();
}

void CGlimAssignmentDlg::OnBnClickedRandom()
{
	std::lock_guard<std::mutex> lk(m_mtx);
	ReadInputs();                 // 최신 입력 반영

	// 캔버스 안에서 3점 임의 생성
	int L = m_state.canvas.left + m_state.markerR;
	int T = m_state.canvas.top + m_state.markerR;
	int R = m_state.canvas.right - m_state.markerR - 1;
	int B = m_state.canvas.bottom - m_state.markerR - 1;
	auto rnd = [](int a, int b)->int { return a + (rand() % (max(1, b - a + 1))); };

	m_state.P[0] = { rnd(L, R), rnd(T, B) };
	m_state.P[1] = { rnd(L, R), rnd(T, B) };
	m_state.P[2] = { rnd(L, R), rnd(T, B) };

	// 외접원 재계산 → 성공 시 Ready 단계로 전환
	if (MathCirc::ComputeCircumcircle(m_state.P[0], m_state.P[1], m_state.P[2], m_state.circle)) {
		m_state.hasCircle = true;
		m_state.phase = Phase::Ready;
	}
	else {
		// 드물게 일직선이 나오면 다시 시도하거나, 대기 단계로
		m_state.hasCircle = false;
		m_state.phase = Phase::Waiting3;
	}

	Invalidate(FALSE);   // 다시 그리기 트리거
}
