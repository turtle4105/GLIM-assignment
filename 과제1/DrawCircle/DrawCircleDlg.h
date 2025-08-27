
// DrawCircleDlg.h: 헤더 파일
#pragma once

#include "CircleDrawer.h"
#define WM_RANDOM_TICK   (WM_APP + 1)

// CDrawCircleDlg 대화 상자
class CDrawCircleDlg : public CDialogEx
{
// 생성입니다.
public:
	CDrawCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
    static UINT AFX_CDECL RandThreadProc(LPVOID p);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWCIRCLE_DIALOG };
#endif

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    virtual void DoDataExchange(CDataExchange* pDX);


    // ====== 그림 버퍼/상태 ======
    CImage m_image;
    int    m_imgX = 50;
    int    m_imgY = 50;

    POINT  m_pts[3] = {};
    int    m_count = 0;
    int    m_drag = -1;

    int    m_pointRadius = 10;
    int    m_edgeThickness = 4;

    bool   m_hasCircle = false;
    double m_cx = 0, m_cy = 0, m_cr = 0;

    
    CircleDrawer m_drawer;

    // 랜덤 이동 스레드
    CWinThread* m_pRandTh = nullptr;
    bool        m_randRunning = false;

    void ClearImage(BYTE gray = 255);
    void PullUserInputs();
    void RedrawAll();
    CRect ImageRect() const;

    // ====== 메시지 핸들러 ======
    afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
    afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
    afx_msg void OnBnClickedBtnInit();
    afx_msg void OnBnClickedBtnRandom();
    afx_msg LRESULT OnRandomTick(WPARAM, LPARAM);
    afx_msg void OnDestroy();
    afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
};
