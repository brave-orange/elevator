#pragma once
#include <UIlib.h>
#include<String.h>
#include<../elevator.h>
#include<windows.h>
#include<iostream>
#include<malloc.h>
#include <thread>
#include <future>         // std::promise, std::future
#include<../main.h>

using namespace DuiLib;
#define WM_REDRAW WM_USER+100
#define WM_CHANGEUBTNCOLOR WM_USER+101
#define WM_CHANGEDBTNCOLOR WM_USER+102
#define WM_CHANGENBTNCOLOR WM_USER+103
#define WM_TARBTNOPEN      WM_USER+104
#define WM_TARBTNCLOSE      WM_USER+105
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
Elevator * elevator;
int floor_num = 0;
std::mutex mtx; 
//bool change = false;
//bool isTarget = false;  //��ǰ�Ƿ���Ŀ��¥��
HWND hWnd;
class CDuiFrameWnd :public WindowImplBase
{
public:

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
	virtual CDuiString GetSkinFile()                {   return _T("duilib.xml");  }
	virtual CDuiString GetSkinFolder()              {   return _T("");  }

	void setTarBtn(bool a)   //����Ŀ��Ŷ¥�㰴ť�Ŀ�����
	{
		CDuiString str;
		CButtonUI * tarbtn ;
		for(int i = 1 ; i <= floor_num ; i++)
		{
			str.Format("tar%d",i);
			tarbtn = dynamic_cast <CButtonUI *>(m_PaintManager.FindControl(str));
			tarbtn->SetEnabled(a);

		}
	}
	void screenUpdete()        //����¥��䶯
	{

		CDuiString str;
		CListContainerElementUI * a;
		for(int x = 1 ; x <= floor_num ; x++)
		{		
			str.Format("%d",x);
			a = dynamic_cast <CListContainerElementUI*>(m_PaintManager.FindControl(str));
			a->SetAttribute(_T("bkcolor"),_T("#FFFF9966"));

		}
		str.Format(_T("%d"),elevator->getFloor());
		CListContainerElementUI * one = dynamic_cast <CListContainerElementUI*>(m_PaintManager.FindControl(str));
		one->SetAttribute(_T("bkcolor"),_T("#FFFF0033"));
		CLabelUI * now = dynamic_cast <CLabelUI*>(m_PaintManager.FindControl("now_floor"));
		CListContainerElementUI * up = dynamic_cast <CListContainerElementUI*>(m_PaintManager.FindControl("up"));
		CListContainerElementUI * down = dynamic_cast <CListContainerElementUI*>(m_PaintManager.FindControl("down"));	

		now->SetAttribute(_T("text"),str);
		now->SetAttribute(_T("font"),_T("0"));
		if(elevator->isUp())
		{
			up->SetAttribute(_T("bkcolor"),_T("#FFFF0033"));
			down->SetAttribute(_T("bkcolor"),_T("#FF99CCFF"));
		}
		else
		{
			down->SetAttribute(_T("bkcolor"),_T("#FFFF0033"));
			up->SetAttribute(_T("bkcolor"),_T("#FF99CCFF"));
		}
	}

	virtual void Notify(TNotifyUI& msg) 
	{
		if(msg.sType == _T("click"))
		{
			string name = (string)msg.pSender->GetName().GetData();

			if(msg.pSender->GetName() == _T("go")) 
			{
				//free(elevator);
				//��������
				ClearRequestUp();
				ClearRequestDown();
				ClearTarget();
				elevator->setFloor(1);
				elevator->setUp();
				elevator->stop();
				CEditUI * edit1 = dynamic_cast <CEditUI*>(m_PaintManager.FindControl(_T("floor_num")));
				char * num = (char *) edit1->GetText().GetData();
				char number[500];
				strcpy(number,num);
				if(strlen(number)==0)
				{
					::MessageBox(NULL, _T("�����������������"), _T("����"), NULL);
				}
				else
				{
					int i = 0;
					for(i = 0 ; i < strlen(number) ; i++)
					{
						if(number[i]<48 || number[i]>57)
						{
							break;
						}
					}
					if(i < strlen(number))
					{
						::MessageBox(NULL, _T("�벻Ҫ������������ݣ�"), _T("����"), NULL);
					}
					else
					{
						//��¥��ť��ubtn+¥�� ��¥��ť��dbtn+¥�� 
						int floor = atoi(num);
						floor_num = floor;//��ʼ��¥����

						CDuiString str;

						CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("floor")));

						CVerticalLayoutUI* xList = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("buttons")));
						xList->RemoveAll();
						pList->RemoveAll();

						CHorizontalLayoutUI * btnitem;
						for(int j = floor ; j > 0 ;j--)
						{
							CListContainerElementUI * item = new CListContainerElementUI();
							str.Format(_T("%d"), j);
							item->SetName(str);
							item->SetAttribute(_T("height"),_T("40"));
							item->SetAttribute(_T("borderround"),_T("5,5"));
							item->SetAttribute(_T("padding"),_T("2,2,2,2"));
							item->SetAttribute(_T("bordercolor"),_T("#FFFFFFFF"));
							item->SetAttribute(_T("focusbordercolor"),_T("	#FFFF0033")); 
							CButtonUI * upbutton = new CButtonUI();
							str.Format(_T("ubtn%d"), j);
							upbutton->SetName(str);
							upbutton->SetAttribute(_T("bkcolor"),_T("#FF99CCFF"));
							upbutton->SetAttribute(_T("hotimage"),_T("SysBtn\button_down.bmp"));
							upbutton->SetAttribute(_T("pos"),_T("60,5,0,0"));
							upbutton->SetAttribute(_T("text"),_T("��"));
							upbutton->SetAttribute(_T("height"),_T("15"));
							upbutton->SetAttribute(_T("float"),_T("true"));
							upbutton->SetAttribute(_T("width"),_T("20"));
							CButtonUI * downbutton = new CButtonUI();
							str.Format(_T("dbtn%d"), j);
							downbutton->SetName(str);
							downbutton->SetAttribute(_T("bkcolor"),_T("#FF99CCFF"));
							downbutton->SetAttribute(_T("hotimage"),_T("SysBtn\button_down.bmp"));
							downbutton->SetAttribute(_T("pos"),_T("60,30,0,0"));
							downbutton->SetAttribute(_T("text"),_T("��"));
							downbutton->SetAttribute(_T("height"),_T("15"));
							downbutton->SetAttribute(_T("float"),_T("true"));
							downbutton->SetAttribute(_T("width"),_T("20"));
							pList->Add(item);
							CLabelUI * floor1 = new CLabelUI();
							floor1->SetAttribute(_T("font"),_T("1"));
							str.Format(_T("%dF:"), j);
							floor1->SetText(str);
							item->Add(floor1);
							if(j==floor)
							{   item->Add(downbutton); }
							else if(j==1)
							{	 item->Add(upbutton); }
							else
							{
								item->Add(upbutton);
								item->Add(downbutton);
							}                       	 

							if(((j-floor_num)*-1)%4 == 0)
							{
								btnitem = new CHorizontalLayoutUI();
								btnitem->SetAttribute(_T("height"),_T("30"));
								btnitem->SetAttribute(_T("width"),_T("100"));
								xList->Add(btnitem);
							}
							CButtonUI * tarbtn = new CButtonUI();
							str.Format(_T("tar%d"),j);
							tarbtn->SetName(str);
							tarbtn->SetAttribute(_T("height"),_T("25"));
							tarbtn->SetAttribute(_T("width"),_T("25"));
							str.Format(_T("%d"), j);
							tarbtn->SetAttribute(_T("text"),str);
							tarbtn->SetAttribute(_T("font"),_T("1"));
							tarbtn->SetNormalImage(_T("SysBtn/btnb.png"));
							tarbtn->SetHotImage(_T("SysBtn/btna.png"));

							tarbtn->SetAttribute(_T("enabled"),_T("false"));
							btnitem->Add(tarbtn);
						}

						screenUpdete();
						hWnd = GetHWND();

					}
				}

			}
			else if(!name.substr(0,4).compare(_T("dbtn")))                     //��¥��ť��Ӧ�¼�,nΪ��ǰ¥��
			{
				string  f = name.substr(4,5);
				int n;
				n = atoi(f.c_str());
				CDuiString str;
				str.Format("dbtn%d",n);
				CButtonUI * btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(str));
				btn->SetAttribute(_T("bkcolor"),_T("#FFFF0033"));
				AddRequestDownFloor(n);		

			}
			else if(!name.substr(0,4).compare(_T("ubtn")))                      //��¥��ť��Ӧ�¼�
			{
				string  f = name.substr(4,5);
				int n = atoi(f.c_str());
				CDuiString str;
				str.Format("ubtn%d",n);
				CButtonUI * btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(str));
				btn->SetAttribute(_T("bkcolor"),_T("#FFFF0033"));
				AddRequestUpFloor(n);	
				//::MessageBox(NULL, _T("�㵽����¥��ť"), _T("����"), NULL);
			}
			else if(!name.substr(0,3).compare(_T("tar")))                     //��¥��ť��Ӧ�¼�,nΪ��ǰ¥��
			{
				string  f = name.substr(3,4);
				int n;
				n = atoi(f.c_str());
				CDuiString str;
				str.Format("tar%d",n);
				CButtonUI * btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(str));
				btn->SetNormalImage(_T("SysBtn/btna.png"));
				AddTargetFloor(n);	
			}
			else if(msg.pSender->GetName()== _T("minbtn"))
			{
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			else if(msg.pSender->GetName() == _T("closebtn"))
			{
				exit(0);
			}

		}
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;

		if( uMsg == WM_CREATE ) 
		{
			m_PaintManager.Init(m_hWnd);

			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("duilib.xml"), (UINT)0, NULL, &m_PaintManager);   // duilib.xml��Ҫ�ŵ�exeĿ¼��
			ASSERT(pRoot && "Failed to parse XML");

			m_PaintManager.AttachDialog(pRoot);
			m_PaintManager.AddNotifier(this);   // ��ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib����Ϣѭ�������ǿ�����Notify����������Ϣ����
			return lRes;
		}
		// ����3����ϢWM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT��������ϵͳ������
		else if( uMsg == WM_NCACTIVATE ) 
		{
			if( !::IsIconic(m_hWnd) ) 
			{
				return (wParam == 0) ? TRUE : FALSE;
				
			}

		}
		else if(uMsg == WM_NCHITTEST)
		{
			POINT pt;   
			pt.x = GET_X_LPARAM(lParam);   
			pt.y = GET_Y_LPARAM(lParam);  
			::ScreenToClient(m_hWnd,&pt);  

			RECT rcClient;  
			::GetClientRect(m_hWnd, &rcClient); 
			if(pt.y<rcClient.top+32&&pt.x<rcClient.right-50)
			{
				return HTCAPTION;  
			}

		}
		else if (WM_NCLBUTTONDBLCLK == uMsg)  
		{  
			return 0;  
		}  
		else if(uMsg == WM_SIZE)
		{
			RECT rcClient = { 0 };  
			::GetClientRect(m_hWnd, &rcClient);   
			InvalidateRect(m_hWnd,&rcClient,FALSE);  
		}
		else if( uMsg == WM_NCCALCSIZE ) 
		{
			return 0;
		}
		else if( uMsg == WM_NCPAINT ) 
		{
			return 0;
		}
		else if (uMsg == WM_REDRAW)
		{
			screenUpdete();
			return 0;
		}
		else if(uMsg == WM_CHANGEUBTNCOLOR)
		{
			ChangeUpBtnColor();
			return 0;
		}
		else if(uMsg == WM_CHANGEDBTNCOLOR)
		{
			ChangeDownBtnColor();
			return 0;
		}
		else if(uMsg == WM_CHANGENBTNCOLOR)
		{
			ChangeNumBtnColor();
			return 0;
		}
		else if(uMsg == WM_TARBTNOPEN)
		{
			setTarBtn(true);
			return 0;
		}
		else if(uMsg == WM_TARBTNCLOSE)
		{
			setTarBtn(false);
			return 0;
		}


		if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
		{
			return lRes;
		}

		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	void ChangeDownBtnColor()
	{
		CDuiString str;
		str.Format("dbtn%d",elevator->getFloor());
		CButtonUI * btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(str));
		btn->SetAttribute(_T("bkcolor"),_T("#FF99CCFF"));

	}
	void ChangeUpBtnColor()
	{
		CDuiString str;
		str.Format("ubtn%d",elevator->getFloor());
		CButtonUI * btn1 = static_cast<CButtonUI*>(m_PaintManager.FindControl(str));
		btn1->SetAttribute(_T("bkcolor"),_T("#FF99CCFF"));
	}
	void ChangeNumBtnColor()
	{
		CDuiString str;
		str.Format("tar%d",elevator->getFloor());
		CButtonUI * btn2 = static_cast<CButtonUI*>(m_PaintManager.FindControl(str));
		btn2->SetNormalImage(_T("SysBtn/btnb.png"));
	}

protected:
	CPaintManagerUI m_PaintManager;
};



void mainprocess()    //����¥��䶯����ʾ�������
{
	::Sleep(2000);
	CDuiString str;

	while(1)
	{   
		bool isArrive =false;
	if(elevator->getStatus())                  //�����Ƿ������У����������ٿ����з���
	{
		::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
		if(elevator->isUp())
		{
			elevator->Upstairs();					
			::PostMessage(hWnd, WM_REDRAW, 0L, 0);
			
			if(request_down_head != NULL)
			{
				if(request_down_head->floor == elevator->getFloor())
				{
					::PostMessage(hWnd, WM_CHANGEDBTNCOLOR, 0L, 0);
					//��������ߵ�һ�����������
					FreeDownRequest(request_down_head);
					::MessageBox(NULL, _T("��������¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
					isArrive = true;
					elevator->setDown();

				}
			}
			else if(FindRequestUp(elevator->getFloor()))
			{
				::PostMessage(hWnd, WM_CHANGEUBTNCOLOR, 0L, 0); //���������������
				::MessageBox(NULL, _T("��������¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
				isArrive = true;

			}
			if(FindTarget(elevator->getFloor()))
			{
				::PostMessage(hWnd, WM_CHANGENBTNCOLOR, 0L, 0); 
				::MessageBox(NULL, _T("������Ŀ��¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
				isArrive = true;
			}


		}
		else if(elevator->isDown())
		{
			elevator->Downstairs();
			::PostMessage(hWnd, WM_REDRAW, 0L, 0); 					
			
			if(request_up_head != NULL)
			{
				if(request_up_head->floor == elevator->getFloor())
				{
					::PostMessage(hWnd, WM_CHANGEUBTNCOLOR, 0L, 0);
					::MessageBox(NULL, _T("��������¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
					FreeUpRequest(request_up_head);
					isArrive = true;
					elevator->setUp();
				}
			}
			else if(FindRequestDown(elevator->getFloor()))
			{
				::PostMessage(hWnd, WM_CHANGEDBTNCOLOR, 0L, 0);
				::MessageBox(NULL, _T("��������¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
				isArrive = true;
			}
			if(FindTarget(elevator->getFloor()))
			{
				//::PostMessage(hWnd, WM_CHANGEUBTNCOLOR, 0L, 0); //���������������
				::PostMessage(hWnd, WM_CHANGENBTNCOLOR, 0L, 0);
				::MessageBox(NULL, _T("������Ŀ��¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
				isArrive = true;
			}

		}
		if(target_head == NULL && request_down_head == NULL && request_up_head == NULL) //û��Ŀ���˾�ͣ����
		{
			//::MessageBox(NULL, _T("ͣ��"), _T("ע�ⰲȫ"), NULL);
			elevator->stop();
		}
		if(isArrive)
		{
			::Sleep(5000);
			::MessageBox(NULL, _T("���ݹ��ţ�"), _T("ע�ⰲȫ"), NULL);
			if(target_head == NULL)
			{
				if(elevator->isDown() && request_down_head == NULL && request_up_head != NULL)
				{
					if(request_up_head->floor >= elevator->getFloor())
					{
						elevator->setUp();
					}
				}
				if(elevator->isUp() &&  request_up_head != NULL && request_down_head == NULL)
				{
					if(request_up_head->floor <= elevator->getFloor())
					{
						elevator->setDown();
					}
				}
				
			}
		}
		if(!elevator->getStatus())
		{
			::PostMessage(hWnd, WM_TARBTNCLOSE, 0L, 0);
		}

	}
	else                    //��ǰ״̬Ϊֹͣʱ��
	{
		/**/
		if(request_down_head != NULL)  //�����������ʱ������״̬Ϊ����,����λ�����÷���
		{

			if(request_down_head->floor > elevator->getFloor())
			{
				elevator->setUp();
				elevator->run();
			}else if(request_down_head->floor == elevator->getFloor())
			{
				::MessageBox(NULL, _T("��������¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
				FreeDownRequest(request_down_head);
				::PostMessage(hWnd, WM_CHANGEDBTNCOLOR, 0L, 0);
				::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
				elevator->setDown();
				::Sleep(5000);
				::MessageBox(NULL, _T("���ݹ��ţ�"), _T("ע�ⰲȫ"), NULL);
			}
			else
			{
				elevator->setDown();
				elevator->run();
			}				

			::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
		}
		else if(request_up_head != NULL) //�����������ʱ������״̬Ϊ����,����λ�����÷���
		{

			if(request_up_head->floor > elevator->getFloor())
			{
				elevator->setUp();
				elevator->run();
			}
			else if(request_up_head->floor == elevator->getFloor())
			{

				::MessageBox(NULL, _T("��������¥�㣬���ţ�"), _T("ע�ⰲȫ"), NULL);
				FreeUpRequest(request_up_head);
				::PostMessage(hWnd, WM_CHANGEUBTNCOLOR, 0L, 0);
				::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
				elevator->setUp();
				::Sleep(5000);
				::MessageBox(NULL, _T("���ݹ��ţ�"), _T("ע�ⰲȫ"), NULL);
			}
			else 
			{
				elevator->setDown();
				elevator->run();
			}
			::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
		}
		else if(target_head != NULL)   //��Ŀ���ʱ������״̬Ϊ����,����λ�����÷���
			{
				
				if(target_head->floor > elevator->getFloor())
				{

					elevator->setUp();
					elevator->run();
				}else if(target_head->floor == elevator->getFloor())
				{
					::MessageBox(NULL, _T("����Ŀ���,���ţ�"), _T("ע�ⰲȫ"), NULL);
					FreeTarget(target_head);
					::PostMessage(hWnd, WM_CHANGENBTNCOLOR, 0L, 0);
					::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
					//elevator->setUp();
					::Sleep(20000);
				}
				else
				{
					elevator->setDown();
				    elevator->run();
				}
				::PostMessage(hWnd, WM_TARBTNOPEN, 0L, 0);
			}
	}	

	::Sleep(500);
	}
}

void init()
{
	CDuiFrameWnd duiFrame;	
	elevator = new Elevator();                                  //ʵ����һ������ 
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();

}


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{


	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());   // ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	std::thread t1(init);
	std::thread t2(mainprocess);
	t1.join();
	t2.join();

	return 0;
}