#pragma comment(lib, "vfw32")
#include <iostream>
#include <string>

#include "cv.h"
#include "CvvImage.h"
#include "opencv.hpp"

#include "./serial/ComPort.h"
#include "resource.h"

using namespace std;
using namespace cv;
// �Զ���MFC��Ϣ���� �������ݽ�����Ϣ @PostMessageFunc OnReceiveData()
#define WM_RECV_SERIAL_DATA WM_USER + 101

class CautocarDlg : virtual DebugLabComm::CComPort, public CDialog
{
public:
  explicit CautocarDlg(CWnd* pParent = NULL);
  virtual BOOL OnInitDialog();
  afx_msg void OnClose();

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
  afx_msg LONG OnRecvSerialData(WPARAM wParam, LPARAM lParam);

  /* OpenCV��غ��� *****************************************/
  afx_msg void OnBnClickedBt_OpenCamera();
  afx_msg void OnBnClickedBt_CloseCamera();
  afx_msg void OnBnClickedBt_AutoDrive();
  afx_msg void OnBnClickedBt_ImageIdentification();

  afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
  HICON appIcon_;
private:
  void _SerialOpen(int commNum = 2, int baudRate = 115200);
  void _OnCommReceive(LPVOID pSender, void* pBuf, DWORD InBufferCount);
  void _OnCommBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat);

  //TAG:����ImageBoxӦ����һ��ö�����ͣ��������
  void _ShowImageOnImageBox(int ImageBox, Mat& frame);
  void _StretchBlt(int ImageBox, CDC& cdcSrc,
    int x = 0, int y = 0, int w = 48, int h = 48);

  /* ͼ��ʶ���㷨 ********************************************/
  void _BinaryzationMat(Mat & inputMat, Mat & outputMat);

  CString _msgSerialSend;
  CString _msgSerialReceive;

  VideoCapture _cameraForPic;
  VideoCapture _cameraForPath;

  CvvImage _cvvImage;

  //TAG: _mode������Ӧ������Ϊһ�� ö����
  int _mode;
};
