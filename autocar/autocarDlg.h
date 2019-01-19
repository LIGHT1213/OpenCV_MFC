#include <string>

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "zbar.h"            

#include "./serial/ComPort.h"
#include "resource.h"

using namespace zbar;
using namespace std;

// �Զ���MFC��Ϣ���� �������ݽ�����Ϣ @PostMessageFunc OnReceiveData()
#define WM_RECV_SERIAL_DATA WM_USER + 101

class CautocarDlg : virtual DebugLabComm::CComPort, public CDialog
{
public:
  explicit CautocarDlg(CWnd* pParent = NULL);
  virtual BOOL OnInitDialog();

  /**
   * @func: DoDataExchange - DDX/DDV֧��
   *      : OnPaint - ��Ի��������С����ť
   *      : OnQueryDragIcon - ���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ
   * @Message function mapping
   *      DECLARE_MESSAGE_MAP() 
   *      @see autocarDlg.cpp BEGIN_MESSAGE_MAP
   */
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()

  /* ������غ��� ********************************************/
  afx_msg void OnBnClickedBt_OpenSerial();
  afx_msg void OnBnClickedBt_CloseSerial();
  afx_msg void OnBnClickedBt_SendToSerial();
  void PrintlnToSerial(const string& message);
  void PrintToSerial(const string& message);
  friend void OnCommReceive(LPVOID pSender, void* pBuf, DWORD InBufferCount);
  afx_msg LONG OnRecvSerialData(WPARAM wParam, LPARAM lParam);
  friend void OnCommBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat);

  /* OpenCV��غ��� *****************************************/
  afx_msg void OnBnClickedBtopenvideo();
  afx_msg void OnBnClickedBtclosevideo();
  afx_msg void OnBnClickedBtautodrive();
  afx_msg void OnBnClickedBtscannumber();

  int m_clockbuffleft;
  int m_clockbufffright;

  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnClose();

  CvCapture* capture;
  CvCapture* capture1;

  CRect rect;
  CDC *pDC;
  HDC hDC;
  CWnd *pWnd;

  CRect rect1;
  HDC hDC1;
  CDC *pDC1;
  CWnd *pwnd1;

  CRect rect2;
  CDC *pDC2;
  HDC hDC2;
  CWnd *pwnd2;

  CRect rect3;
  CDC *pDC3;
  HDC hDC3;
  CWnd *pwnd3;

protected:
  HICON appIcon_;

private:
  void _SerialOpen(int commNum = 2, int baudRate = 115200);

  CString _msgSerialSend;
  CString _msgSerialReceive;
  
  IplImage* _cameraFrame;
  CvvImage _cvvImage;

  //TAG: _mode������Ӧ������Ϊһ�� ö����
  int _mode;
};
