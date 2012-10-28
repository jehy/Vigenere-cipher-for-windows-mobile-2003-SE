// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define ENCODE 1
#define DECODE 0
// CtestDlg dialog

CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, text);
	DDX_Control(pDX, IDC_EDIT2, offset);
	DDX_Control(pDX, IDC_EDIT3, key);
	DDX_Control(pDX, IDC_EDIT4, from);
	DDX_Control(pDX, IDC_CHECK1, enc);
	DDX_Control(pDX, IDC_CHECK2, dec);
	DDX_Control(pDX, IDC_EDIT5, alphabet);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON3, &CtestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &CtestDlg::OnEnChangeEdit2)
//	ON_BN_CLICKED(IDC_BUTTON1, &CtestDlg::OnBnClickedButton1)
ON_EN_UPDATE(IDC_EDIT3, &CtestDlg::OnEnUpdateEdit3)
ON_BN_CLICKED(IDC_BUTTON1, &CtestDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_CHECK2, &CtestDlg::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK1, &CtestDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CtestDlg message handlers

BOOL CtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CString temp;
	temp="0";
	offset.SetWindowTextW(temp);
	//state=0;
	temp="SampleKey";
	key.SetWindowTextW(temp);
	enc.SetCheck(1);
	dec.SetCheck(0);
	state=ENCODE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
//void CtestDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
/*{
	DRA::RelayoutDialog(
		AfxGetInstanceHandle(), 
		this->m_hWnd, 
		DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_TEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_TEST_DIALOG));
}
#endif
*/

void CtestDlg::OnBnClickedButton3()
{
	ChangeOffset(1);
}

void CtestDlg::OnBnClickedButton2()
{
	ChangeOffset(-1);
}

void CtestDlg::ChangeOffset(int add)
{	CString temp;
	offset.GetWindowTextW(temp);
	__int64 NewState=_wcstoi64(temp.GetBuffer(),NULL,10)+add;
	temp="%i";
	temp.Format(temp,NewState);
	offset.SetWindowTextW(temp);
}

void CtestDlg::SetOffset(int off)
{	CString temp;
	temp="%i";
	temp.Format(temp,off);
	offset.SetWindowTextW(temp);
}

void CtestDlg::OnEnChangeEdit2()
{
	Decrypt();
}

void CtestDlg::Decrypt()
{
	CString temp,Key,Text,Alph,err;
	offset.GetWindowTextW(temp);
	__int64 off=_wcstoi64(temp.GetBuffer(),NULL,10);
	from.GetWindowTextW(Text);
	key.GetWindowTextW(Key);
	alphabet.GetWindowTextW(Alph);
	int o=(int)off;
	int i=0;
	for(; i<Text.GetLength();)
	{
		for(;; o++) 
		{
			if(i==Text.GetLength())break;
			if(o==Key.GetLength())o=0;
			int g=Alph.Find(Key[o]);
			if(g==-1)
			{
				err="Key char not found in alphabet!";
				MessageBox(err);
				return;
			}
			int g2=Alph.Find(Text[i]);
			if(g2==-1)
			{
				err="text char not found in alphabet!";
				MessageBox(err);
				return;
			}
			if(state==ENCODE)
			{
				g=(g+g2)%Alph.GetLength();
				Text.SetAt(i,Alph[g]);
			}
			else
			{
				g=g2-g;
				if(g<0)g+=Alph.GetLength();
				Text.SetAt(i,Alph[g]);
			}
			i++; 
		}
	}
	text.SetWindowTextW(Text);
}

void CtestDlg::OnEnUpdateEdit3()
{
	Decrypt();
}

void CtestDlg::OnBnClickedButton1()
{
	CString Key,Text;
	key.GetWindowTextW(Key);
	int max=0;
	int off=0;
	for(int i=0;i<Key.GetLength();i++)
	{
		int cur=0;
		SetOffset(i);
		Decrypt();
		text.GetWindowTextW(Text);
		for(int z=0;z<Text.GetLength();z++)
			if(Text[z]==' ')cur++;
		if(cur>max)
		{
			max=cur;
			off=i;
		}
		//Sleep(1000);
	}

	SetOffset(off);
	Decrypt();
}

void CtestDlg::OnBnClickedCheck2()
{
	enc.SetCheck(0);
	dec.SetCheck(1);
	state=DECODE;
	Decrypt();
}

void CtestDlg::OnBnClickedCheck1()
{
	enc.SetCheck(1);
	dec.SetCheck(0);
	state=ENCODE;
	Decrypt();
}
