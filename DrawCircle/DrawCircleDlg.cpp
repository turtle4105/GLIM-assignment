#include "pch.h"
#include "framework.h"
#include "DrawCircle.h"
#include "DrawCircleDlg.h"
#include "afxdialogex.h"

#include "CircleDrawer.h"
#include <algorithm>
#include <cmath>
#include <ctime>


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
	virtual void DoDataExchange(CDataExchange* pDX);    

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


// CDrawCircleDlg 대화 상자



CDrawCircleDlg::CDrawCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWCIRCLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_INIT, &CDrawCircleDlg::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CDrawCircleDlg::OnBnClickedBtnRandom)
	ON_MESSAGE(WM_RANDOM_TICK, &CDrawCircleDlg::OnRandomTick)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDrawCircleDlg 메시지 처리기

BOOL CDrawCircleDlg::OnInitDialog()
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

		// UI 기본값
	SetDlgItemInt(IDC_EDIT_RADIUS, 10, FALSE);
	SetDlgItemInt(IDC_EDIT_THICKNESS, 4, FALSE);
	//SetDlgItemText(IDC_STATIC_CENTER, L"-");

	// CImage 생성
	const int w = 800, h = 560, bpp = 8;
	m_image.Destroy();
	m_image.Create(w, h, bpp);
	if (bpp == 8) {
		static RGBQUAD table[256];
		for (int i = 0;i < 256;++i) table[i].rgbRed = table[i].rgbGreen = table[i].rgbBlue = (BYTE)i;
		m_image.SetColorTable(0, 256, table);
	}
	// 흰색으로 지우기
	const int pitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	for (int y = 0;y < h;++y) memset(fm + y * pitch, 255, w);

	// CircleDrawer 연결
	m_drawer.AttachImage(&m_image);
	m_drawer.SetPointRadius(10);
	m_drawer.SetEdgeThickness(4);

	// 상태 초기화
	m_count = 0; m_drag = -1; m_hasCircle = false;


	return TRUE; 
}

void CDrawCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDrawCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 캔버스 사각형
CRect CDrawCircleDlg::ImageRect() const
{
    return CRect(m_imgX, m_imgY,
                 m_imgX + m_image.GetWidth(),
                 m_imgY + m_image.GetHeight());
}

void CDrawCircleDlg::ClearImage(BYTE gray)
{
    const int w = m_image.GetWidth(), h = m_image.GetHeight();
    const int pitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    for (int y=0; y<h; ++y) memset(fm + y*pitch, gray, w);
}

void CDrawCircleDlg::PullUserInputs()
{
    BOOL ok;
    int r = GetDlgItemInt(IDC_EDIT_RADIUS, &ok, FALSE);
    if (ok && r>0) { m_pointRadius = r; m_drawer.SetPointRadius(r); }
    int t = GetDlgItemInt(IDC_EDIT_THICKNESS, &ok, FALSE);
    if (ok && t>0) { m_edgeThickness = t; m_drawer.SetEdgeThickness(t); }
}

void CDrawCircleDlg::RedrawAll()
{
    PullUserInputs();
    ClearImage(255);

    for (int i=0;i<m_count;++i)
        m_drawer.DrawFilledDisk(m_pts[i].x, m_pts[i].y, m_pointRadius, 0);

    m_hasCircle = false;
    if (m_count==3) {
        double cx,cy,r;
        if (m_drawer.CircumcircleFrom3Pts(m_pts[0],m_pts[1],m_pts[2],cx,cy,r)) {
            m_cx=cx; m_cy=cy; m_cr=r;
            m_drawer.DrawRing((int)std::lround(cx), (int)std::lround(cy),
                              (int)std::lround(r), m_edgeThickness, 0);
            m_hasCircle = true;
        }
    }

    CString s;
    if (m_count>0){ s.Format(L"%d %d",m_pts[0].x,m_pts[0].y); SetDlgItemText(IDC_EDIT_C1,s); }
    if (m_count>1){ s.Format(L"%d %d",m_pts[1].x,m_pts[1].y); SetDlgItemText(IDC_EDIT_C2,s); }
    if (m_count>2){ s.Format(L"%d %d",m_pts[2].x,m_pts[2].y); SetDlgItemText(IDC_EDIT_C3,s); }
    //if (m_hasCircle){
    //    s.Format(L"(%d, %d)  r=%d",
    //             (int)std::lround(m_cx),(int)std::lround(m_cy),(int)std::lround(m_cr));
    //    SetDlgItemText(IDC_STATIC_CENTER,s);
    //} else SetDlgItemText(IDC_STATIC_CENTER,L"-");

    Invalidate(FALSE);
}

// 마우스
void CDrawCircleDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
    if (!ImageRect().PtInRect(pt)) { CDialogEx::OnLButtonDown(nFlags,pt); return; }
    const int ix = pt.x - m_imgX, iy = pt.y - m_imgY;

    int hit=-1;
    for (int i=0;i<m_count;++i){
        int dx=ix-m_pts[i].x, dy=iy-m_pts[i].y;
        if (dx*dx+dy*dy <= m_pointRadius*m_pointRadius){ hit=i; break; }
    }
    if (m_count<3 && hit==-1) {
        m_pts[m_count++] = {ix,iy};
        RedrawAll();
    } else if (hit!=-1) {
        m_drag = hit; SetCapture();
    }
    CDialogEx::OnLButtonDown(nFlags,pt);
}

void CDrawCircleDlg::OnMouseMove(UINT nFlags, CPoint pt)
{
    if (m_drag>=0 && (nFlags & MK_LBUTTON) && ImageRect().PtInRect(pt)) {
        m_pts[m_drag].x = pt.x - m_imgX;
        m_pts[m_drag].y = pt.y - m_imgY;
        RedrawAll();
    }
    CDialogEx::OnMouseMove(nFlags,pt);
}

void CDrawCircleDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
    if (m_drag>=0){ ReleaseCapture(); m_drag=-1; }
    CDialogEx::OnLButtonUp(nFlags,pt);
}

// 페인트: CImage 화면에 출력
void CDrawCircleDlg::OnPaint()
{
    if (IsIconic()) { CDialogEx::OnPaint(); return; }
    CPaintDC dc(this);
    if (m_image) m_image.Draw(dc, m_imgX, m_imgY);
}

// 초기화 버튼
void CDrawCircleDlg::OnBnClickedBtnInit()
{
    m_randRunning = false;
    m_count=0; m_drag=-1; m_hasCircle=false;
    SetDlgItemText(IDC_EDIT_C1,nullptr);
    SetDlgItemText(IDC_EDIT_C2,nullptr);
    SetDlgItemText(IDC_EDIT_C3,nullptr);
    //SetDlgItemText(IDC_STATIC_CENTER,L"-");
    ClearImage(255);
    Invalidate(FALSE);
}

// 랜덤 이동 스레드
UINT AFX_CDECL CDrawCircleDlg::RandThreadProc(LPVOID p)
{
    auto* dlg = reinterpret_cast<CDrawCircleDlg*>(p);
    std::srand((unsigned)std::time(nullptr));

    const int W = dlg->m_image.GetWidth();
    const int H = dlg->m_image.GetHeight();

    if (dlg->m_count < 3) {
        while (dlg->m_count < 3) {
            dlg->m_pts[dlg->m_count] = { std::rand() % W, std::rand() % H };
            ++dlg->m_count;
        }
        dlg->PostMessage(WM_RANDOM_TICK);
        ::Sleep(10);
    }

    for (int k = 0; k < 10 && dlg->m_randRunning; ++k) {
        for (int i = 0;i < 3;++i) {
            dlg->m_pts[i].x = std::rand() % W;
            dlg->m_pts[i].y = std::rand() % H;
        }
        dlg->PostMessage(WM_RANDOM_TICK);
        ::Sleep(500);
    }
    dlg->m_randRunning = false;
    return 0;
}


void CDrawCircleDlg::OnBnClickedBtnRandom()
{
    if (m_randRunning) return;
    m_randRunning = true;
    m_pRandTh = AfxBeginThread(&CDrawCircleDlg::RandThreadProc, this,
        THREAD_PRIORITY_NORMAL, 0, 0, nullptr);
}

LRESULT CDrawCircleDlg::OnRandomTick(WPARAM, LPARAM)
{
    RedrawAll();
    return 0;
}

void CDrawCircleDlg::OnDestroy()
{
    m_randRunning = false;
    if (m_pRandTh) {
        WaitForSingleObject(m_pRandTh->m_hThread, 1000);
        m_pRandTh = nullptr;
    }
    if (m_image) m_image.Destroy();
    CDialogEx::OnDestroy();
}

void CDrawCircleDlg::OnOK()
{
   
}
