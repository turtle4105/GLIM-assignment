
// MFCStartDlg.h: 헤더 파일
//

#pragma once


// CMFCStartDlg 대화 상자
class CMFCStartDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCStartDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSTART_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	CString m_name; // 이름 바인딩용
	int m_age;      // 나이 바인딩용

	// Static Text 컨트롤은 화면에 "문자열(텍스트)만 표시 가능!
	// m_nNum, m_nNum2 처럼 Edit 컨트롤에 바인딩할 때는 int를 쓸 수 있다.
	// 이유는 MFC의 DDX_Context가 내부적으로 CString <-> int 변환을 자동으로 해주기 때문!
	CString m_result;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnOk();
	int m_nNum;
	int m_nNum2;
};
