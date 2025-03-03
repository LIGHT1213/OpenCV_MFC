#include "stdafx.h"
#include "autocarDlg.h"
#include "strngs.h"

#include <math.h>
#include <stdio.h>
#include "resource.h"

#pragma comment(lib,"libtesseract302.lib")


CautocarDlg::CautocarDlg(CWnd* pParent /*=NULL*/)
  : CDialog(IDD_AUTOCAR_DIALOG, pParent)
  , CComPort(this)
  , _msgSerialSend()
  , _msgSerialReceive()
  //TAG:避免这样的魔法值
  , _mode(0)
  , _cameraForPic(0)
  , appIcon_(AfxGetApp()->LoadIcon(IDR_MAINFRAME)){}

BOOL CautocarDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  /* IDM_ABOUTBOX 必须在系统命令范围内 *******************************/
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != nullptr)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  /* 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动 **********/
  SetIcon(appIcon_, FALSE);

  OnBnClickedBt_OpenSerial();
  OnBnClickedBt_OpenCamera();
  OnBnClickedBt_AutoDrive();

  return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CautocarDlg::OnClose()
{
  // TODO: Add your message handler code here and/or call default
  OnBnClickedBt_CloseSerial();
  OnBnClickedBt_CloseCamera();
  CDialog::OnClose();
}

/* Windows功能函数 -----------------------------------------*/
void CautocarDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDITSEND, _msgSerialSend);
  DDX_Text(pDX, IDC_EDITREV, _msgSerialReceive);
}

void CautocarDlg::OnPaint()
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
    dc.DrawIcon(x, y, appIcon_);
  }
  else
  {
    CDialog::OnPaint();
  }
}

HCURSOR CautocarDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(appIcon_);
}

BEGIN_MESSAGE_MAP(CautocarDlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_TIMER()
  ON_WM_CLOSE()

  ON_MESSAGE(WM_RECV_SERIAL_DATA, OnRecvSerialData)

  ON_BN_CLICKED(IDC_BTOPEN      , OnBnClickedBt_OpenSerial  )
  ON_BN_CLICKED(IDC_BTCLOSE     , OnBnClickedBt_CloseSerial )
  ON_BN_CLICKED(IDC_BTSEND      , OnBnClickedBt_SendToSerial)
  ON_BN_CLICKED(IDC_BTAUTODRIVE , OnBnClickedBt_AutoDrive    )
  ON_BN_CLICKED(IDC_BTOPENVIDEO , OnBnClickedBt_OpenCamera    )
  ON_BN_CLICKED(IDC_BTCLOSEVIDEO, OnBnClickedBt_CloseCamera   )
  ON_BN_CLICKED(IDC_BTSCANNUMBER, OnBnClickedBt_ImageIdentification   )
END_MESSAGE_MAP()

/* 串口相关函数 ---------------------------------------------*/
void CautocarDlg::OnBnClickedBt_OpenSerial()
{
  if (!this->IsOpen())
  {
    _SerialOpen(7);
  }
}

void CautocarDlg::OnBnClickedBt_CloseSerial()
{
  if (this->_serialPort->IsOpen())
  {
    PrintlnToSerial("Computer is closing.");
  }
  Close();
}

void CautocarDlg::OnBnClickedBt_SendToSerial()
{
  UpdateData(true);

  PrintlnToSerial(_msgSerialSend.GetBuffer());

  UpdateData(false);
}

void CautocarDlg::PrintlnToSerial(const string& message)
{
  if (IsOpen())
  {
    Output(message.c_str(), message.length());
    Output("\r\n", 2);
  }
}

void CautocarDlg::PrintToSerial(const string& message)
{
  if (IsOpen())
  {
    Output(message.c_str(), message.length());
  }
}

void CautocarDlg::_OnCommReceive(LPVOID pSender, void* pBuf, DWORD InBufferCount)
{
  BYTE *pRecvBuf = new BYTE[InBufferCount]; //delete at OnRecvSerialData();
  CautocarDlg* pThis = (CautocarDlg*)pSender;

  CopyMemory(pRecvBuf, pBuf, InBufferCount);

  pThis->PostMessage(WM_RECV_SERIAL_DATA, WPARAM(pRecvBuf), InBufferCount);
}

LONG CautocarDlg::OnRecvSerialData(WPARAM wParam, LPARAM lParam)
{
  UpdateData(true);

  CHAR *pBuf = (CHAR*)wParam;
  DWORD dwBufLen = lParam;

  _msgSerialReceive.Format(_T("%s"), wParam);
  
  delete[] pBuf; //new at OnCommReceive();
  UpdateData(false);
  return 0;
}

void CautocarDlg::_OnCommBreak(LPVOID pSender, DWORD dwMask, COMSTAT stat)
{
  //deal with the break of com here
  switch (dwMask)
  {
  case  EV_BREAK:
  {
    break;
  }
  case EV_CTS:
  {
    break;
  }
  case EV_DSR:
  {
    break;
  }
  case EV_ERR:
  {
    break;
  }
  case EV_RING:
  {
    break;
  }
  case EV_RLSD:
  {
    break;
  }
  case EV_RXCHAR:
  {
    break;
  }
  case EV_RXFLAG:
  {
    break;
  }
  case EV_TXEMPTY:
  {
    break;
  }
  default:
  {
  }
  }
}

/* OpenCV相关函数 ------------------------------------------*/
void CautocarDlg::OnBnClickedBt_OpenCamera()
{
  //TAG: 开启逻辑有些奇怪，没能以最简单的方式查询是否有可用摄像机
  //Tips: .open()函数，会先release已打开的摄像头
  _cameraForPic.open(0);
  if (!_cameraForPic.isOpened())
  {
    //TAG:应该抛出异常
    AfxMessageBox("无法打开摄像头，Win10请确认摄像头隐私设置是否开启");
  }
  else
  {
    Mat frame;
    _cameraForPic >> frame;
    _ShowImageOnImageBox(IDC_ImageBox1, frame);
    SetTimer(1, 50, NULL);
  }
}

void CautocarDlg::OnBnClickedBt_CloseCamera()
{
  CDC deviceContext;
  CBitmap bitmap;
  bitmap.LoadBitmap(IDB_BITMAP1);  
  deviceContext.CreateCompatibleDC(nullptr);
  deviceContext.SelectObject(&bitmap);

  KillTimer(1);
  _cameraForPic.release();

  _StretchBlt(IDC_ImageBox1, deviceContext);
  _StretchBlt(IDC_ImageBox2, deviceContext);
  _StretchBlt(IDC_ImageBox3, deviceContext);
  _StretchBlt(IDC_ImageBox4, deviceContext);
}

void CautocarDlg::OnBnClickedBt_ImageIdentification()
{
  if (_cameraForPic.isOpened()) {
    Mat inputFrame, binaryFrame, drawMat;
    vector<vector<Point>>contours_external, contours_all;

    /* 图像输入与二值化处理 ---------------------------------*/
    _cameraForPic >> inputFrame;
    binaryFrame = _Binaryzation(inputFrame);
    drawMat = Mat::zeros(binaryFrame.size(), CV_8UC3);

    _ShowImageOnImageBox(IDC_ImageBox3, binaryFrame);

    /* 寻找轮廓 >最大内轮廓 ---------------------------------*/
    findContours(binaryFrame, contours_all, CV_RETR_TREE, CHAIN_APPROX_NONE);
    findContours(binaryFrame, contours_external, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for (int i = 0; i < contours_all.size(); i++)
    {
      drawContours(drawMat, contours_all, i, Scalar(0, 0, 255), 2);
    }

    if (contours_all.size() == contours_external.size()) return;

    for (int i = 0; i < contours_external.size(); i++)
    {
      for (int j = 0; j < contours_all.size(); j++)
      {
        int conloursize = contours_external[i].size();
        int tem_size = contours_all[j].size();
        if (conloursize == tem_size)
        {
          swap(contours_all[j], contours_all[contours_all.size() - 1]);
          contours_all.pop_back();
          break;
        }
      }
    }

    double maxarea = 0;
    int maxAreaIdx = 0;
    for (int index = contours_all.size() - 1; index >= 0; index--)
    {
      double tmparea = fabs(contourArea(contours_all[index]));
      if (tmparea > maxarea)
      {
        maxarea = tmparea;
        maxAreaIdx = index; //记录最大轮廓的索引号
      }
    }
    
    //绘制最大内轮廓
    drawContours(drawMat, contours_all, maxAreaIdx, Scalar(0,255,0), 2);
    //轮廓长度
    double g_dConLength = arcLength(contours_all[maxAreaIdx], true);
    //轮廓面积
    double g_dConArea = contourArea(contours_all[maxAreaIdx], true);

    _ShowImageOnImageBox(IDC_ImageBox2, drawMat);

    UpdateData(true);
    UpdateData(false);
  }
  else
  {
    //TAG:抛出异常
  }
}

void CautocarDlg::OnBnClickedBt_AutoDrive()
{
  UpdateData(true);
  _mode = 10;//大模式自动开机
  SetTimer(2, 50, NULL);
  UpdateData(false);
}


/* 定时器相关函数 -------------------------------------------*/
void CautocarDlg::OnTimer(UINT_PTR nIDEvent)
{
  UpdateData(true);
  CDialog::OnTimer(nIDEvent);

  if (nIDEvent == 1)
  {
    if (_cameraForPic.isOpened())
    {
      Mat frame;
      _cameraForPic >> frame;
      _ShowImageOnImageBox(IDC_ImageBox1, frame);
    }
    else
    {
      // TAG:抛出异常
      KillTimer(1);
    }
  }
  if (nIDEvent == 2)
  {
  }
  UpdateData(false);
}

void CautocarDlg::_SerialOpen(int commNum /*=2*/, int baudRate /*=115200*/)
{
  DCB portSettingsDCB;
  if (!Open(commNum, CComPort::AutoReceiveBySignal))
  {
    // TAG:遇到错误应该抛出异常
    CString sMsg;
    sMsg.Format("提示:不能打开串口%d!", commNum);
    AfxMessageBox(sMsg, MB_ICONINFORMATION | MB_OK);
  }
  else
  {
    GetSerialPort()->GetState(portSettingsDCB);
    portSettingsDCB.BaudRate = 115200;
    GetSerialPort()->SetState(portSettingsDCB);
  }
}

void CautocarDlg::_ShowImageOnImageBox(int ImageBox, Mat& frame)
{
  CRect rect;
  GetDlgItem(ImageBox)->GetClientRect(&rect);

  _cvvImage.CopyOf(&static_cast<IplImage>(frame), 1);
  _cvvImage.DrawToHDC(GetDlgItem(ImageBox)->GetDC()->GetSafeHdc(), &rect);
}

void CautocarDlg::_StretchBlt(int ImageBox, CDC & cdcSrc, int x, int y, int w, int h)
{
  CRect rect;
  GetDlgItem(ImageBox)->GetClientRect(&rect);
  GetDlgItem(ImageBox)->GetDC()->
    StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &cdcSrc, x, y, w, h, SRCCOPY);
}

void CautocarDlg::_Binaryzation(Mat & inputMat, Mat & outputMat)
{
  cvtColor(inputMat, outputMat, CV_BGR2GRAY);
  blur(outputMat, outputMat, Size(3, 3));
  dilate(outputMat, outputMat, getStructuringElement(MORPH_RECT, Size(1, 1)));
  threshold(outputMat, outputMat, 100, 255, CV_THRESH_OTSU);
}

Mat CautocarDlg::_Binaryzation(Mat & inputMat)
{
  Mat outputMat;

  _Binaryzation(inputMat, outputMat);

  return outputMat;
}
