
// Glim_AssignmentDlg.h: 헤더 파일
#include <atlimage.h>
#include <mutex>
#include "AppState.h"
#include "AutoMover.h"

#pragma once


// CGlimAssignmentDlg 대화 상자
class CGlimAssignmentDlg : public CDialogEx
{
// 생성입니다.
public:
	CGlimAssignmentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_ASSIGNMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// 추가
	afx_msg LRESULT OnAppAutoTick(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

private:
	AppState   m_state;   // 상태
	CImage     m_img;     // 백버퍼
	AutoMover  m_mover;   // 자동 이동 (나중 구현)
	std::mutex m_mtx;

	void EnsureBackBuffer();
	void ClearBackBuffer();
	void Present();
	void RenderToBackBuffer();



};




