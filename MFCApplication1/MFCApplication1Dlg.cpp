﻿
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx {
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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Control(pDX, IDC_BUTTON3, pause_button);
	DDX_Control(pDX, IDC_TREE1, PacketDataCtrl);
	DDX_Control(pDX, IDC_LIST1, PacketDumpList);
	DDX_Control(pDX, IDC_EDIT1, m_FilterEditCtrl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CMFCApplication1Dlg::OnCustomdrawList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMFCApplication1Dlg::OnNMDblclkList2)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CMFCApplication1Dlg::OnHdnItemclick)
	//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CMFCApplication1Dlg::OnNMCustomdrawList1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog() {
	netInterfaceDlg.DoModal();
	bool cancelButtonClickedChecker = netInterfaceDlg.CancelButtonClickedFunction();

	if (cancelButtonClickedChecker) {
		::PostQuitMessage(WM_QUIT);
		netInterfaceDlg.EndDialog(IDOK);
		//DestroyWindow();
	}

	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.



	/* 파일에 쓰기 */
	std::remove("test.txt");
	/* 파일에 쓰기 */

	m_strSelectedNetworkInterface = netInterfaceDlg.InterfaceDescription;
	SetDlgItemText(IDC_STATIC_NET, L"Interface: " + m_strSelectedNetworkInterface);

	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON2);
	pButton->EnableWindow(FALSE);

	CButton* pButton3 = (CButton*)GetDlgItem(IDC_BUTTON3);
	pButton3->EnableWindow(FALSE);

	SetWindowText(_T("Wire Dolphin"));

	m_FilterEditCtrl.SetWindowTextW(L"Enter Filter....");

	CRect rt;
	m_ListCtrl.GetWindowRect(&rt);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	LV_COLUMN add_column;
	// 컬럼 속성을 지정한다. 텍스트 형식을 사용하고 폭을 명시하겠다고 설정한다.

	add_column.mask = LVCF_TEXT | LVCF_WIDTH;

	LPWSTR column_name[9] = { L"No",L"Time", L"Source", L"Destination", L"Protocol", L"Length", L"Info" ,L"Dump Data" };
	int count = 0;
	double column_width[9] = { 0.1, 0.17, 0.15, 0.15, 0.075, 0.075, 0.28, 0 };

	for (int i = 0; i < 8; i++) {
		add_column.pszText = column_name[i];
		add_column.cx = (double)rt.Width() * column_width[i];
		m_ListCtrl.InsertColumn(i, &add_column);
	}

	PacketDumpList.GetWindowRect(&rt);
	PacketDumpList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// 컬럼 속성을 지정한다. 텍스트 형식을 사용하고 폭을 명시하겠다고 설정한다.

	add_column.mask = LVCF_TEXT | LVCF_WIDTH;
	LPWSTR packet_dump_header[4] = { L"Seq",L"Hex 1",L"HEX 2", L"ASCII" };
	double pakcet_dump_header_width[4] = { 0.1,0.27,0.27,0.3 };

	for (int i = 0; i < 4; i++) {
		add_column.pszText = packet_dump_header[i];
		add_column.cx = rt.Width() * pakcet_dump_header_width[i];
		PacketDumpList.InsertColumn(i, &add_column);
	}
	// 패킷의 갯수 카운트
	ChangeStaticText(packet_cnt, tcp_pkt_cnt, udp_pkt_cnt, arp_pkt_cnt, icmp_pkt_cnt);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplication1Dlg::OnBnClickedButton1() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pThread == NULL) {
		m_pThread = AfxBeginThread(ThreadFunctionFirstTest, this);
		//AfxMessageBox(_T("캡처를 시작합니다"));
		CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON1);
		pButton->EnableWindow(FALSE);

		CButton* pButton2 = (CButton*)GetDlgItem(IDC_BUTTON2);
		pButton2->EnableWindow(TRUE);

		CButton* pButton3 = (CButton*)GetDlgItem(IDC_BUTTON3);
		pButton3->EnableWindow(TRUE);

		if (m_pThread == NULL) {
			AfxMessageBox(_T("캡처 시작을 할 수 없습니다."));
		}

		if (m_pThread != NULL) {
			m_pThread->m_bAutoDelete = FALSE;
		}
		m_ThreadWorkType = RUNNING;
	} else {
		if (m_ThreadWorkType == RUNNING || m_ThreadWorkType == PAUSE) {
			//m_pThread->ResumeThread();
			//m_ThreadWorkType = RUNNING;
		}
	}
}


UINT CMFCApplication1Dlg::ThreadFunctionFirstTest(LPVOID _mothod) {
	CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->m_pMainWnd;
	pcap_if_t* alldevs;
	pcap_if_t* d = NULL;
	int inum;
	int i = 0;
	pcap_t* adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];

	const char* filter = "tcp or udp or arp or icmp";
	//const char* filter = "arp or tcp or udp or icmp";
	struct bpf_program fcode;
	bpf_u_int32 NetMask;

	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		AfxMessageBox(CString(errbuf));
		//fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	for (d = alldevs; d; d = d->next) {
		++i;
	}

	if (i == 0) {
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):", i);
	//scanf("%d", &inum);
	inum = pDlg->netInterfaceDlg.m_nSelectedIndex + 1;

	if (inum < 1 || inum > i) {
		printf("\nInterface number out of range.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);


	if ((adhandle = pcap_open_live(d->name, 65536, 1, 1000, errbuf)) == NULL) {
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		pcap_freealldevs(alldevs);
		return -1;
	}

	NetMask = 0xffffff;
	if (pcap_compile(adhandle, &fcode, filter, 1, NetMask) < 0) {
		fprintf(stderr, "\nError compiling filter: wrong syntax.\n");
		pcap_close(adhandle);
		return -3;
	}

	if (pcap_setfilter(adhandle, &fcode) < 0) {
		fprintf(stderr, "\nError setting the filter\n");
		pcap_close(adhandle);
		return -4;
	}

	pcap_freealldevs(alldevs);
	pcap_loop(adhandle, 0, packet_handler, NULL);

	return 0;
}

void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data) {
	CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->m_pMainWnd;
	int i;
	ip_header* ih;
	udp_header* uh;
	tcp_header* th;
	icmp_header* icmp_hdr;
	arp_header* arp_hdr = NULL;
	u_int ip_len;
	ether_header* ethhdr;

	ethhdr = (ether_header*)pkt_data;

	ih = (ip_header*)(pkt_data + 14);
	ip_len = (ih->ver_ihl & 0xf) * 4;

	int size = sizeof(pkt_data);

	if (!pDlg->IsFilterApply) {
		if (ntohs(ethhdr->frame_type) == 0x0800) {
			if (ih->proto == IPPROTO_TCP) {
				// TCP
				th = (tcp_header*)((u_char*)ih + ip_len);

				CString source_ip = pDlg->GetIPAddr(ih->saddr);
				CString destionation_ip = pDlg->GetIPAddr(ih->daddr);

				int column_count = pDlg->m_ListCtrl.GetItemCount();

				CString column_count_str;
				column_count_str.Format(_T("%d"), column_count + 1);
				pDlg->m_ListCtrl.InsertItem(column_count, column_count_str);

				pDlg->m_ListCtrl.SetItem(column_count, 1, LVIF_TEXT, CString(pDlg->GetCurrentTimeStr().c_str()), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 2, LVIF_TEXT, source_ip, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 3, LVIF_TEXT, destionation_ip, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 4, LVIF_TEXT, _T("TCP"), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 5, LVIF_TEXT, (CString)(std::to_string(header->caplen).c_str()), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 6, LVIF_TEXT, (CString)(std::to_string(htons(th->sport)).c_str())
					+ " -> " +
					(CString)(std::to_string(ntohs(th->dport)).c_str())
					, NULL, NULL, NULL, NULL);

				++pDlg->tcp_pkt_cnt;
				++pDlg->packet_cnt;

				pDlg->ChangeStaticText(pDlg->packet_cnt, pDlg->tcp_pkt_cnt, pDlg->udp_pkt_cnt, pDlg->arp_pkt_cnt, pDlg->icmp_pkt_cnt);
			} else if (ih->proto == 4) {
				printf("IP\n");
			} else if (ih->proto == IPPROTO_UDP) {
				// UDP

				uh = (udp_header*)((u_char*)ih + ip_len);

				CString source_ip = pDlg->GetIPAddr(ih->saddr);
				CString destionation_ip = pDlg->GetIPAddr(ih->daddr);

				int column_count = pDlg->m_ListCtrl.GetItemCount();

				CString column_count_str;
				column_count_str.Format(_T("%d"), column_count + 1);
				pDlg->m_ListCtrl.InsertItem(column_count, column_count_str);

				pDlg->m_ListCtrl.SetItem(column_count, 1, LVIF_TEXT, CString(pDlg->GetCurrentTimeStr().c_str()), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 2, LVIF_TEXT, source_ip, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 3, LVIF_TEXT, destionation_ip, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 4, LVIF_TEXT, _T("UDP"), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 5, LVIF_TEXT, (CString)(std::to_string(header->caplen).c_str()), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 6, LVIF_TEXT, (CString)(std::to_string(htons(uh->sport)).c_str())
					+ " -> " +
					(CString)(std::to_string(ntohs(uh->dport)).c_str())
					, NULL, NULL, NULL, NULL);

				++pDlg->udp_pkt_cnt;
				++pDlg->packet_cnt;
				pDlg->ChangeStaticText(pDlg->packet_cnt, pDlg->tcp_pkt_cnt, pDlg->udp_pkt_cnt, pDlg->arp_pkt_cnt, pDlg->icmp_pkt_cnt);
			} else if (ih->proto == IPPROTO_ICMP) {
				// ICMP

				icmp_hdr = (icmp_header*)(ih + ip_len);

				CString source_ip = pDlg->GetIPAddr(ih->saddr);
				CString destionation_ip = pDlg->GetIPAddr(ih->daddr);

				int column_count = pDlg->m_ListCtrl.GetItemCount();

				CString column_count_str;
				column_count_str.Format(_T("%d"), column_count + 1);
				pDlg->m_ListCtrl.InsertItem(column_count, column_count_str);

				pDlg->m_ListCtrl.SetItem(column_count, 1, LVIF_TEXT, CString(pDlg->GetCurrentTimeStr().c_str()), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 2, LVIF_TEXT, source_ip, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 3, LVIF_TEXT, destionation_ip, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 4, LVIF_TEXT, _T("ICMP"), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 5, LVIF_TEXT, (CString)(std::to_string(header->caplen).c_str()), NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 6, LVIF_TEXT, (CString)(std::to_string(icmp_hdr->code).c_str()), NULL, NULL, NULL, NULL);


				++pDlg->icmp_pkt_cnt;
				++pDlg->packet_cnt;
				pDlg->ChangeStaticText(pDlg->packet_cnt, pDlg->tcp_pkt_cnt, pDlg->udp_pkt_cnt, pDlg->arp_pkt_cnt, pDlg->icmp_pkt_cnt);
			} else {
				printf("Unknown Protocol\n");
				unsigned char temp = ih->proto;

			}

			pDlg->ChangeStaticText(pDlg->packet_cnt, pDlg->tcp_pkt_cnt, pDlg->udp_pkt_cnt, pDlg->arp_pkt_cnt, pDlg->icmp_pkt_cnt);

			int nCount = pDlg->m_ListCtrl.GetItemCount();
			pDlg->m_ListCtrl.EnsureVisible(nCount - 1, FALSE);


			std::string packet_dump_data;

			for (i = 1; (i < header->caplen + 1); i++) {
				char* temp = NULL;

				int temp2 = pkt_data[i - 1];

				std::stringstream stream;
				stream << std::hex << temp2;
				packet_dump_data += stream.str() + " ";

			}
		} else if (ntohs(ethhdr->frame_type) == 0x0806) {
			arp_hdr = (struct arp_header*)(pkt_data + 14);

			CString source_ip = pDlg->GetIPAddr(ih->saddr);
			CString destionation_ip = pDlg->GetIPAddr(ih->daddr);

			int column_count = pDlg->m_ListCtrl.GetItemCount();

			CString column_count_str;
			column_count_str.Format(_T("%d"), column_count + 1);
			pDlg->m_ListCtrl.InsertItem(column_count, column_count_str);

			char soure_hw_addr[4];
			char target_hw_addr[4];

			CString sender_hw_addr, target_hw_adr;
			for (int i = 0; i < 5; i++) {
				sprintf(soure_hw_addr, "%02x:", arp_hdr->sha[i]);
				sender_hw_addr += soure_hw_addr;

				sprintf(target_hw_addr, "%02x:", arp_hdr->tha[i]);
				target_hw_adr += target_hw_addr;
			}

			sprintf(soure_hw_addr, "%02x", arp_hdr->sha[5]);
			sender_hw_addr += soure_hw_addr;

			sprintf(target_hw_addr, "%02x", arp_hdr->tha[5]);
			target_hw_adr += target_hw_addr;


			pDlg->m_ListCtrl.SetItem(column_count, 1, LVIF_TEXT, CString(pDlg->GetCurrentTimeStr().c_str()), NULL, NULL, NULL, NULL);
			pDlg->m_ListCtrl.SetItem(column_count, 2, LVIF_TEXT, source_ip, NULL, NULL, NULL, NULL);
			pDlg->m_ListCtrl.SetItem(column_count, 3, LVIF_TEXT, destionation_ip, NULL, NULL, NULL, NULL);
			pDlg->m_ListCtrl.SetItem(column_count, 4, LVIF_TEXT, _T("ARP"), NULL, NULL, NULL, NULL);
			pDlg->m_ListCtrl.SetItem(column_count, 5, LVIF_TEXT, (CString)(std::to_string(header->caplen).c_str()), NULL, NULL, NULL, NULL);

			// Info는 프로토콜 마다 달라야함
			pDlg->m_ListCtrl.SetItem(column_count, 6, LVIF_TEXT, sender_hw_addr + L" -> " + target_hw_adr, NULL, NULL, NULL, NULL);


			++pDlg->arp_pkt_cnt;
			++pDlg->packet_cnt;
			pDlg->ChangeStaticText(pDlg->packet_cnt, pDlg->tcp_pkt_cnt, pDlg->udp_pkt_cnt, pDlg->arp_pkt_cnt, pDlg->icmp_pkt_cnt);
		} else {
			return;
		}
		std::string result;

		for (i = 1; (i < header->caplen + 1); i++) {
			char* temp = NULL;

			int temp2 = pkt_data[i - 1];
			std::stringstream stream;
			stream << std::setw(2) << std::setfill('0') << std::hex << temp2;

			result += stream.str();
		}

		CString packet_dump_data(result.c_str());
		int column_count = pDlg->m_ListCtrl.GetItemCount() - 1;
		pDlg->m_ListCtrl.SetItem(column_count, 7, LVIF_TEXT, packet_dump_data, NULL, NULL, NULL, NULL);

		//====================================================================================
		//pDlg->m_ListCtrl.SetItem(column_count, 1, LVIF_TEXT, CString(pDlg->GetCurrentTimeStr().c_str()), NULL, NULL, NULL, NULL);
		//pDlg->m_ListCtrl.SetItem(column_count, 2, LVIF_TEXT, source_ip, NULL, NULL, NULL, NULL);
		//pDlg->m_ListCtrl.SetItem(column_count, 3, LVIF_TEXT, destionation_ip, NULL, NULL, NULL, NULL);
		//pDlg->m_ListCtrl.SetItem(column_count, 4, LVIF_TEXT, _T("ARP"), NULL, NULL, NULL, NULL);
		//pDlg->m_ListCtrl.SetItem(column_count, 5, LVIF_TEXT, (CString)(std::to_string(header->caplen).c_str()), NULL, NULL, NULL, NULL);
		//pDlg->m_ListCtrl.SetItem(column_count, 6, LVIF_TEXT, sender_hw_addr + L" -> " + target_hw_adr, NULL, NULL, NULL, NULL);
		//pDlg->m_ListCtrl.SetItem(column_count, 7, LVIF_TEXT, packet_dump_data, NULL, NULL, NULL, NULL);
	} else {

	}

	/* 파일에 쓰기 */

	if (ntohs(ethhdr->frame_type) == 0x0806 || ntohs(ethhdr->frame_type) == 0x0800) {

		unsigned char c;
		int packet_size = header->caplen;
		std::ofstream out("test.txt", std::ios::app);

		CT2CA pszConvertedAnsiString(pDlg->GetIPAddr(ih->saddr));
		std::string s(pszConvertedAnsiString);
		std::string sip = s;

		CT2CA pszConvertedAnsiString2(pDlg->GetIPAddr(ih->daddr));
		std::string s2(pszConvertedAnsiString2);
		std::string dip = s2;

		std::string protocol;
		if (ntohs(ethhdr->frame_type) == 0x0800) {
			if (ih->proto == IPPROTO_TCP) {
				protocol = "TCP";
			} else if (ih->proto == IPPROTO_UDP) {
				protocol = "UDP";
			}if (ih->proto == IPPROTO_ICMP) {
				protocol = "ICMP";
			}
		} else if (ntohs(ethhdr->frame_type) == 0x0806) {
			protocol = "ARP";
		}


		out << pDlg->GetCurrentTimeStr() << "\n";
		out << sip << " \n";
		out << dip << " \n";
		out << protocol << " \n";
		out << header->caplen << " \n";
		out << "Info" << " \n";

		for (int i = 0; i < packet_size; i++) {
			c = pkt_data[i];
			out.width(2);
			out << std::hex << std::setfill('0') << (unsigned int)pkt_data[i] << " ";

			if ((i != 0 && (i + 1) % 16 == 0) || i == packet_size - 1) {
				out << "\n";
			}
		}
		out << "-----------------------------------------------\n";
		//====================================================================================
	}
}

void CMFCApplication1Dlg::OnBnClickedButton2() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pThread == NULL) {

	} else {
		m_pThread->SuspendThread();

		DWORD dwResult;
		::GetExitCodeThread(m_pThread->m_hThread, &dwResult);

		delete m_pThread;
		m_pThread = NULL;

		m_ThreadWorkType = STOP;

		CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON1);
		pButton->EnableWindow(TRUE);

		CButton* pButton2 = (CButton*)GetDlgItem(IDC_BUTTON2);
		pButton2->EnableWindow(FALSE);

		CButton* pButton3 = (CButton*)GetDlgItem(IDC_BUTTON3);
		pButton3->EnableWindow(FALSE);


		ClearPacketCnt();
		ChangeStaticText(packet_cnt, tcp_pkt_cnt, udp_pkt_cnt, arp_pkt_cnt, icmp_pkt_cnt);
		m_ListCtrl.DeleteAllItems();
		PacketDataCtrl.DeleteAllItems();
		PacketDumpList.DeleteAllItems();
	}
}

void CMFCApplication1Dlg::ChangeStaticText(int all_pkt_cnt, int tcp_pkt_cnt, int udp_pkt_cnt, int arp_pkt_cnt, int icmp_pkt_cnt) {
	SetDlgItemText(IDC_STATIC,
		L"ALL : " + (CString)(std::to_string(all_pkt_cnt).c_str()) +
		L" TCP : " + (CString)(std::to_string(tcp_pkt_cnt).c_str()) +
		L" UDP : " + (CString)(std::to_string(udp_pkt_cnt).c_str()) +
		L" ARP : " + (CString)(std::to_string(arp_pkt_cnt).c_str()) +
		L" ICMP : " + (CString)(std::to_string(icmp_pkt_cnt).c_str())
	);
}

void CMFCApplication1Dlg::ClearPacketCnt() {
	packet_cnt = 0;
	tcp_pkt_cnt = 0;
	udp_pkt_cnt = 0;
	arp_pkt_cnt = 0;
	icmp_pkt_cnt = 0;
}

void CMFCApplication1Dlg::OnBnClickedButton3() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pThread == NULL) {
	} else {
		if (m_ThreadWorkType == RUNNING) {
			pause_button.SetWindowText(L"Resume");
			m_pThread->SuspendThread();
			m_ThreadWorkType = PAUSE;
		} else {
			pause_button.SetWindowText(L"Pause");
			m_pThread->ResumeThread();
			m_ThreadWorkType = RUNNING;
		}
	}
}


std::string CMFCApplication1Dlg::GetCurrentTimeStr() {
	time_t     tm_time;
	struct tm* st_time;
	char       buff[1024];

	time(&tm_time);
	st_time = localtime(&tm_time);
	strftime(buff, 1024, "%Y-%m-%d %p %H:%M:%S", st_time);

	std::string temp_buf = buff;

	return temp_buf;
}


CString CMFCApplication1Dlg::GetIPAddr(ip_address ip_addr) {
	CString temp_ip_addr;
	temp_ip_addr += CString(std::to_string(int(ip_addr.byte1)).c_str()) + L".";
	temp_ip_addr += CString(std::to_string(int(ip_addr.byte2)).c_str()) + L".";
	temp_ip_addr += CString(std::to_string(int(ip_addr.byte3)).c_str()) + L".";
	temp_ip_addr += CString(std::to_string(int(ip_addr.byte4)).c_str());

	return temp_ip_addr;
}

void CMFCApplication1Dlg::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	} else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage) {
		CString Protocl = m_ListCtrl.GetItemText(pLVCD->nmcd.dwItemSpec, 4);

		if (Protocl == L"TCP") {
			pLVCD->clrTextBk = RGB(231, 230, 255);
		} else if (Protocl == L"UDP") {
			pLVCD->clrTextBk = RGB(218, 238, 255);
		} else if (Protocl == L"ICMP") {
			pLVCD->clrTextBk = RGB(252, 224, 255);
		} else if (Protocl == L"ARP") {
			pLVCD->clrTextBk = RGB(250, 240, 215);
		}
		*pResult = CDRF_DODEFAULT;
	}
}

void CMFCApplication1Dlg::OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	pNMItemActivate->iItem;
	if (pNMItemActivate->iItem != -1) {
		CString FrameNumber = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
		CString Time = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
		CString Source = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 2);
		CString Destination = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 3);
		CString Protocol = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 4);
		CString Length = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 5);
		CString Info = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 6);
		CString Packet_Dump_Data = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 7);

		SetData(FrameNumber, Time, Source, Destination, Protocol, Length, Info, Packet_Dump_Data);
	}

	PacketDumpList.DeleteAllItems();
	CString Packet_dump_data = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 7);

	if (Packet_dump_data != L"") {
		for (int i = 0; i < Packet_dump_data.GetLength() + 1; i += 32) {
			int column_count = PacketDumpList.GetItemCount();
			CString column_count_str;
			column_count_str.Format(_T("%d"), column_count + 1);

			std::stringstream stream;
			stream << std::setw(6) << std::setfill('0') << std::hex << (i / 2);

			std::string seq_number_str = stream.str();
			LPCSTR lpcstrSeqNum = (LPCSTR)seq_number_str.c_str();
			USES_CONVERSION;
			CString CstrSeqNum = A2CT(lpcstrSeqNum);
			CstrSeqNum.MakeUpper();
			PacketDumpList.InsertItem(column_count, CstrSeqNum);

			CString allHex = Packet_dump_data.Mid(i, 32);
			CString AsciiAllHex = allHex;
			allHex = allHex.MakeUpper();

			CString hex1, hex2;

			for (int i = 0; i < 16; i += 2) {
				hex1 += allHex.Mid(i, 2) + L"  ";
			}

			for (int i = 16; i < 32; i += 2) {
				hex2 += allHex.Mid(i, 2) + L"  ";
			}

			PacketDumpList.SetItem(column_count, 1, LVIF_TEXT, hex1, NULL, NULL, NULL, NULL);
			PacketDumpList.SetItem(column_count, 2, LVIF_TEXT, hex2, NULL, NULL, NULL, NULL);

			CString convAscii;
			CString PacketAscii1;
			CString PacketAscii2;

			for (int i = 0; i < AsciiAllHex.GetLength(); i += 2) {
				PacketAscii1 = ChangeHexToAscii(AsciiAllHex.Mid(i, 1));
				PacketAscii2 = ChangeHexToAscii(AsciiAllHex.Mid(i + 1, 1));

				int ten = _ttoi(PacketAscii1) * 16;
				int one = _ttoi(PacketAscii2);

				int sum = ten + one;
				ten = 0;
				one = 0;

				if (sum < 32 || sum>128) {
					sum = 46;
				}

				char ascii[4];
				ascii[0] = (char)sum;
				if (sum == 46) {
					sprintf(ascii, "%2c", ascii[0]);
				} else {
					sprintf(ascii, "%c", ascii[0]);
				}
				convAscii += ascii;
			}

			PacketDumpList.SetItem(column_count, 3, LVIF_TEXT, convAscii, NULL, NULL, NULL, NULL);
		}
	}
}

void CMFCApplication1Dlg::OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMHEADER pNMLV = reinterpret_cast<LPNMHEADER>(pNMHDR);

	int nColumn = pNMLV->iItem;
	// 선택된 헤더 숫자로 정렬해야되는 값은 0번 (No)와 5번 (Length)이다.

	for (int i = 0; i < (m_ListCtrl.GetItemCount()); i++) {
		m_ListCtrl.SetItemData(i, i);
	}

	if (m_bAscending) {
		m_bAscending = false;
	} else {
		m_bAscending = true;
	}

	SORTPARAM sortparams;
	sortparams.pList = &m_ListCtrl;
	sortparams.iSrotColumn = nColumn;
	sortparams.bSortDirect = m_bAscending;

	if (nColumn == 0 || nColumn == 5) {
		m_ListCtrl.SortItems(&SortFuncNum, (LPARAM)& sortparams);
	} else {
		m_ListCtrl.SortItems(&SortFuncStr, (LPARAM)& sortparams);
	}

	*pResult = 0;
}


int CALLBACK CMFCApplication1Dlg::SortFuncStr(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CListCtrl* pList = ((SORTPARAM*)lParamSort)->pList;
	int iSortColumn = ((SORTPARAM*)lParamSort)->iSrotColumn;
	bool bSortDirect = ((SORTPARAM*)lParamSort)->bSortDirect;


	LVFINDINFO info1, info2;
	info1.flags = LVFI_PARAM;
	info1.lParam = lParam1;
	info2.flags = LVFI_PARAM;
	info2.lParam = lParam2;

	int irow1 = pList->FindItem(&info1, -1);
	int irow2 = pList->FindItem(&info2, -1);

	CString strItem1 = pList->GetItemText(irow1, iSortColumn);
	CString strItem2 = pList->GetItemText(irow2, iSortColumn);

	return bSortDirect ? strItem1.Compare(strItem2) : -strItem1.Compare(strItem2);
}

int CALLBACK CMFCApplication1Dlg::SortFuncNum(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	CListCtrl* pList = ((SORTPARAM*)lParamSort)->pList;
	int iSortColumn = ((SORTPARAM*)lParamSort)->iSrotColumn;
	bool bSortDirect = ((SORTPARAM*)lParamSort)->bSortDirect;


	LVFINDINFO info1, info2;
	info1.flags = LVFI_PARAM;
	info1.lParam = lParam1;
	info2.flags = LVFI_PARAM;
	info2.lParam = lParam2;

	int irow1 = pList->FindItem(&info1, -1);
	int irow2 = pList->FindItem(&info2, -1);

	int numItem1 = _ttoi(pList->GetItemText(irow1, iSortColumn));
	int numItem2 = _ttoi(pList->GetItemText(irow2, iSortColumn));

	return !bSortDirect ? numItem1 < numItem2 : numItem1 > numItem2;
}

BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg) {
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

CString CMFCApplication1Dlg::HexToDec(CString _number) {
	if (_number == L"a" || _number == L"A") {
		_number = L"10";
	} else 	if (_number == L"b" || _number == L"B") {
		_number = L"11";
	} else 	if (_number == L"c" || _number == L"C") {
		_number = L"12";
	} else 	if (_number == L"d" || _number == L"D") {
		_number = L"13";
	} else 	if (_number == L"e" || _number == L"E") {
		_number = L"14";
	} else 	if (_number == L"f" || _number == L"F") {
		_number = L"15";
	} else {
		_number = _number;
	}

	return _number;
}

CString CMFCApplication1Dlg::HexToBinary(CString _number) {
	CString result;
	CString temp1;
	CString temp2;
	CString temp3;
	CString temp4;

	temp1 = CString((std::to_string(_ttoi(_number) % 2)).c_str());
	_number = CString((std::to_string(_ttoi(_number) / 2)).c_str());
	temp2 = CString((std::to_string(_ttoi(_number) % 2)).c_str());
	_number = CString((std::to_string(_ttoi(_number) / 2)).c_str());
	temp3 = CString((std::to_string(_ttoi(_number) % 2)).c_str());
	temp4 = CString((std::to_string(_ttoi(_number) / 2)).c_str());

	result = temp4 + temp3 + temp2 + temp1;

	return result;
}

CString CMFCApplication1Dlg::GetTCPFlagToBin(CString _Flag) {
	CString Result;
	CString FirstByte = _Flag.Mid(0, 1);
	CString SecondByte = _Flag.Mid(1, 1);
	CString ThirdByte = _Flag.Mid(2, 1);

	FirstByte = HexToDec(FirstByte);
	SecondByte = HexToDec(SecondByte);
	ThirdByte = HexToDec(ThirdByte);

	FirstByte = HexToBinary(FirstByte);
	SecondByte = HexToBinary(SecondByte);
	ThirdByte = HexToBinary(ThirdByte);

	Result = FirstByte + SecondByte + ThirdByte;

	return Result;
}

CString CMFCApplication1Dlg::GetTCPFlagToStr(CString _Flag) {
	CString Result;

	CString Urgent = _Flag.Mid(0, 1);
	CString Acknowledgment = _Flag.Mid(0, 1);
	CString Push = _Flag.Mid(0, 1);
	CString Reset = _Flag.Mid(0, 1);
	CString Syn = _Flag.Mid(0, 1);
	CString Fin = _Flag.Mid(0, 1);

	if (Urgent == L"1") {
		Result = "URG";
	} else if (Acknowledgment = L"1") {
		if (Result.IsEmpty() == true) {
			Result = "ACK";
		} else {
			Result.Append(L", ACK");
		}
	} else if (Push = L"1") {
		if (Result.IsEmpty() == true) {
			Result = "PUSH";
		} else {
			Result.Append(L", PUSH");
		}
	} else if (Reset = L"1") {
		if (Result.IsEmpty() == true) {
			Result = "RST";
		} else {
			Result.Append(L", RST");
		}
	} else if (Syn = L"1") {
		if (Result.IsEmpty() == true) {
			Result = "SYN";
		} else {
			Result.Append(L", SYN");
		}
	} else if (Fin = L"1") {
		if (Result.IsEmpty() == true) {
			Result = "FIN";
		} else {
			Result.Append(L", FIN");
		}
	}

	return Result;
}


CString CMFCApplication1Dlg::GetTCPFlagToLongStr(CString _Flag) {
	CString Result = L"";
	CString FlagArray[6] = { L"U",L"A",L"P",L"R",L"S",L"F" };

	for (int i = 0; i < _Flag.GetLength(); i++) {
		if (_Flag.Mid(i, 1) == L"1" && i > 5) {
			Result.Append(FlagArray[i - 6]);
		} else {
			Result.Append(L". ");
		}
	}

	return Result;
}

void CMFCApplication1Dlg::SetData(CString FrameNumber, CString Time, CString Source, CString Destination, CString Protocol, CString Length, CString Info, CString Packet_Dump_Data) {
	HTREEITEM  PacketDataRoot1 = NULL;
	HTREEITEM  PacketDataRoot2 = NULL;
	HTREEITEM  PacketDataRoot3 = NULL;
	HTREEITEM  PacketDataRoot4 = NULL;
	HTREEITEM  PacketDataRoot5 = NULL;

	PacketDataCtrl.DeleteAllItems();
	PacketDataCtrl.Invalidate();

	CString PacketDataLine1;
	CString PacketDataLine2;
	CString PacketDataLine3;
	CString PacketDataLine4;

	PacketDataLine1 = L"Frame " + FrameNumber + L": "
		+ Length + L"bytes on wire (" + CString(std::to_string(_ttoi(Length) * 8).c_str()) + L" bits), "
		+ Length + L"bytes captured (" + CString(std::to_string(_ttoi(Length) * 8).c_str()) + L" bits) on interface 0";

	CString PakcetDataLine1by1 = L"Interface id: 0 (" + netInterfaceDlg.InterfaceName + L")";
	CString PakcetDataLine1by1by1 = L"Interface name: " + netInterfaceDlg.InterfaceName;
	CString PakcetDataLine1by1by2 = L"Interface desciption: " + netInterfaceDlg.InterfaceDescription;

	CString PakcetDataLine1by2 = L"Encapsulation type: Ethernet (1)";
	CString PakcetDataLine1by3 = L"Arrival Time: " + Time;
	CString PakcetDataLine1by4 = L"Frame Number: " + FrameNumber;
	CString PakcetDataLine1by5 = L"Frame Length: " + Length + L" bytes (" + CString(std::to_string(_ttoi(Length) * 8).c_str()) + L" bits)";
	CString PakcetDataLine1by6 = L"Capture Length: " + Length + L" bytes (" + CString(std::to_string(_ttoi(Length) * 8).c_str()) + L" bits)";

	CString Destination_addr = MakeIPAddressV6(Packet_Dump_Data.Mid(0, 2), Packet_Dump_Data.Mid(2, 2), Packet_Dump_Data.Mid(4, 2), Packet_Dump_Data.Mid(6, 2), Packet_Dump_Data.Mid(8, 2), Packet_Dump_Data.Mid(10, 2));
	CString Source_addr = MakeIPAddressV6(Packet_Dump_Data.Mid(12, 2), Packet_Dump_Data.Mid(14, 2), Packet_Dump_Data.Mid(16, 2), Packet_Dump_Data.Mid(18, 2), Packet_Dump_Data.Mid(20, 2), Packet_Dump_Data.Mid(22, 2));

	PacketDataLine2 = L"Ethernet ⅠⅠ, Src: " + Source_addr + L", Dst: " + Destination_addr;
	CString PakcetDataLine2by1 = L"Destination: " + Destination_addr;
	CString PakcetDataLine2by2 = L"Source: " + Source_addr;

	// IPv6 일때 작동하도록 수정
	CString Type = Packet_Dump_Data.Mid(24, 4);
	CString TypeName;
	CString Padding;
	if (Type == L"0800") {
		TypeName = L"IPv4";
		Padding = Packet_Dump_Data.Mid(108, 12);
	} else if (Type == L"0806") {
		TypeName = L"ARP";
		Padding = Packet_Dump_Data.Mid(84, 36);
	}
	CString PakcetDataLine2by3 = L"Type: " + TypeName + L" (0x" + Type + L")";


	CString PakcetDataLine2by4 = L"Padding: " + Padding;

	CString ipVersion = Packet_Dump_Data.Mid(28, 1);
	CString headerLength = Packet_Dump_Data.Mid(29, 1);

	CString totalLength = Calculate4HexNumber(Packet_Dump_Data.Mid(32, 1), Packet_Dump_Data.Mid(33, 1), Packet_Dump_Data.Mid(34, 1), Packet_Dump_Data.Mid(35, 1));
	CString identification = Calculate4HexNumber(Packet_Dump_Data.Mid(36, 1), Packet_Dump_Data.Mid(37, 1), Packet_Dump_Data.Mid(38, 1), Packet_Dump_Data.Mid(39, 1));

	CString timeToLive = Calculate2HexNumber(Packet_Dump_Data.Mid(44, 1), Packet_Dump_Data.Mid(45, 1));
	CString ptotocol = Calculate2HexNumber(Packet_Dump_Data.Mid(46, 1), Packet_Dump_Data.Mid(47, 1));

	PacketDataLine3 = L"Internet Protocol Version " + ipVersion + L", Src: " + Source + L", Dst: " + Destination;
	CString ipVersionBinary = HexToBinary(HexToDec(ipVersion));
	CString headerLengthBinary = HexToBinary(HexToDec(headerLength));

	CString PacketDataLine3by1 = ipVersionBinary + L"  . . . . = Version: " + ipVersion;
	CString PacketDataLine3by2 = L". . . .  " + headerLengthBinary + " = Header Length: " + CString(std::to_string((_ttoi(headerLength) * 4)).c_str()) + L" bytes (" + headerLength + L")";
	CString PacketDataLine3by3 = L"Differentinated Services Field: 0x" + Packet_Dump_Data.Mid(30, 2);
	CString PacketDataLine3by4 = L"Total Length: " + totalLength;
	CString PacketDataLine3by5 = L"Identification: 0x" + Packet_Dump_Data.Mid(36, 4) + L" (" + identification + L")";
	CString PacketDataLine3by6 = L"Flags: 0x" + Packet_Dump_Data.Mid(40, 4);
	CString PacketDataLine3by7 = L"Time to live: " + timeToLive;
	CString PacketDataLine3by8 = L"Protocol: " + Protocol + L"(" + ptotocol + L")";
	CString PacketDataLine3by9 = L"Header checksum: " + Packet_Dump_Data.Mid(48, 4); +L")";
	CString PacketDataLine3by10 = L"Source: " + Source;
	CString PacketDataLine3by11 = L"Destination: " + Destination;

	CString Line4SourcePort = Calculate4HexNumber(Packet_Dump_Data.Mid(68, 1), Packet_Dump_Data.Mid(69, 1), Packet_Dump_Data.Mid(70, 1), Packet_Dump_Data.Mid(71, 1));
	CString Line4DestinationPort = Calculate4HexNumber(Packet_Dump_Data.Mid(72, 1), Packet_Dump_Data.Mid(73, 1), Packet_Dump_Data.Mid(74, 1), Packet_Dump_Data.Mid(75, 1));

	// Line 1
	PacketDataRoot1 = PacketDataCtrl.InsertItem(PacketDataLine1);
	HTREEITEM PacketDataRoot1Child1 = PacketDataCtrl.InsertItem(PakcetDataLine1by1, PacketDataRoot1);
	HTREEITEM PacketDataRoot1Child1Child1 = PacketDataCtrl.InsertItem(PakcetDataLine1by1by1, PacketDataRoot1Child1);
	HTREEITEM PacketDataRoot1Child1Child2 = PacketDataCtrl.InsertItem(PakcetDataLine1by1by2, PacketDataRoot1Child1);

	HTREEITEM PacketDataRoot1Child2 = PacketDataCtrl.InsertItem(PakcetDataLine1by2, PacketDataRoot1);
	HTREEITEM PacketDataRoot1Child3 = PacketDataCtrl.InsertItem(PakcetDataLine1by3, PacketDataRoot1);
	HTREEITEM PacketDataRoot1Child4 = PacketDataCtrl.InsertItem(PakcetDataLine1by4, PacketDataRoot1);
	HTREEITEM PacketDataRoot1Child5 = PacketDataCtrl.InsertItem(PakcetDataLine1by5, PacketDataRoot1);
	HTREEITEM PacketDataRoot1Child6 = PacketDataCtrl.InsertItem(PakcetDataLine1by6, PacketDataRoot1);

	// Line 2
	PacketDataRoot2 = PacketDataCtrl.InsertItem(PacketDataLine2);
	HTREEITEM PacketDataRoot2Child1 = PacketDataCtrl.InsertItem(PakcetDataLine2by1, PacketDataRoot2);
	HTREEITEM PacketDataRoot2Child2 = PacketDataCtrl.InsertItem(PakcetDataLine2by2, PacketDataRoot2);
	HTREEITEM PacketDataRoot2Child3 = PacketDataCtrl.InsertItem(PakcetDataLine2by3, PacketDataRoot2);
	if (Length == L"60") {
		HTREEITEM PacketDataRoot2Child4 = PacketDataCtrl.InsertItem(PakcetDataLine2by4, PacketDataRoot2);
	}
	// Line 3
	PacketDataRoot3 = PacketDataCtrl.InsertItem(PacketDataLine3);
	HTREEITEM PacketDataRoot3Child1 = PacketDataCtrl.InsertItem(PacketDataLine3by1, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child2 = PacketDataCtrl.InsertItem(PacketDataLine3by2, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child3 = PacketDataCtrl.InsertItem(PacketDataLine3by3, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child4 = PacketDataCtrl.InsertItem(PacketDataLine3by4, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child5 = PacketDataCtrl.InsertItem(PacketDataLine3by5, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child6 = PacketDataCtrl.InsertItem(PacketDataLine3by6, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child7 = PacketDataCtrl.InsertItem(PacketDataLine3by7, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child8 = PacketDataCtrl.InsertItem(PacketDataLine3by8, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child9 = PacketDataCtrl.InsertItem(PacketDataLine3by9, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child10 = PacketDataCtrl.InsertItem(PacketDataLine3by10, PacketDataRoot3);
	HTREEITEM PacketDataRoot3Child11 = PacketDataCtrl.InsertItem(PacketDataLine3by11, PacketDataRoot3);

	if (Protocol == L"TCP") {
		PacketDataLine4 = L"Transmission Control Protocol, Src Port: " + Line4SourcePort + L", Dst Port: " + Line4DestinationPort;
		CString PacketDataLine4by1 = L"Source Port: " + Line4SourcePort;
		CString PacketDataLine4by2 = L"Destination Port: " + Line4DestinationPort;
		CString PacketDataLine4by3 = L"Sequence number: " + Packet_Dump_Data.Mid(76, 8);
		CString PacketDataLine4by4 = L"Acknowledge number: " + Packet_Dump_Data.Mid(84, 8);
		CString PacketDataLine4by5 = HexToBinary(HexToDec(Packet_Dump_Data.Mid(92, 1))) + L" . . . . = Header Length: "
			+ CString(std::to_string(_ttoi(Packet_Dump_Data.Mid(92, 1)) * 4).c_str()) + " bytes ("
			+ CString(std::to_string(_ttoi(Packet_Dump_Data.Mid(92, 1))).c_str()) + ")";

		// Reserver+Flag;
		// 6bits -> Reserved
		CString BinaryTCPFlag = GetTCPFlagToBin(Packet_Dump_Data.Mid(93, 3));

		CString Reserved = BinaryTCPFlag.Mid(0, 3);
		CString Nonce = BinaryTCPFlag.Mid(3, 1);
		CString CongestionWindowReduced = BinaryTCPFlag.Mid(4, 1);
		CString ECN_Echo = BinaryTCPFlag.Mid(5, 1);

		// Flag
		// 6bits -> Flags
		CString Urgent = BinaryTCPFlag.Mid(6, 1);
		CString Acknowledgment = BinaryTCPFlag.Mid(7, 1);
		CString Push = BinaryTCPFlag.Mid(8, 1);
		CString Reset = BinaryTCPFlag.Mid(9, 1);
		CString Syn = BinaryTCPFlag.Mid(10, 1);
		CString Fin = BinaryTCPFlag.Mid(11, 1);

		// Only Binary Flag;
		CString TCPFlagBinaryOnly;

		for (int i = 6; i < 12; i++) {
			TCPFlagBinaryOnly.Append(BinaryTCPFlag.Mid(i, 1));
		}

		// TCP Flags:  . . . . . . .A . . .F 의 형식
		CString TCPFlagLongStr = GetTCPFlagToLongStr(BinaryTCPFlag);

		CString PacketDataLine4by6 = L"Flags: 0x" + Packet_Dump_Data.Mid(93, 3) + L"(" + GetTCPFlagToStr(TCPFlagBinaryOnly) + L")";

		CString PacketDataLine4by6by1 = Reserved;
		CString PacketDataLine4by6by2 = Nonce;
		CString PacketDataLine4by6by3 = CongestionWindowReduced;
		CString PacketDataLine4by6by4 = ECN_Echo;
		CString PacketDataLine4by6by5 = Urgent;
		CString PacketDataLine4by6by6 = Acknowledgment;
		CString PacketDataLine4by6by7 = Push;
		CString PacketDataLine4by6by8 = Reset;
		CString PacketDataLine4by6by9 = Syn;
		CString PacketDataLine4by6by10 = Fin;
		CString PacketDataLine4by6by11 = L"[TCP Flags: " + TCPFlagLongStr + "]";

		PacketDataLine4by6by1 = Reserved + L".  . . . .  . . . . = Reserved: " + GetFlagSetNotSet(Reserved);		// Reserved
		PacketDataLine4by6by2 = L". . ." + Nonce + L"  . . . .  . . . . = Nonce: " + GetFlagSetNotSet(Nonce);		// Nonce
		PacketDataLine4by6by3 = L". . . .  " + CongestionWindowReduced + L". . .  . . . . = CongestionWindowReduced (CWR) : " + GetFlagSetNotSet(CongestionWindowReduced);		// CongestionWindowReduced
		PacketDataLine4by6by4 = L". . . .  . " + ECN_Echo + L". .  . . . . = ECN-Echo : " + GetFlagSetNotSet(ECN_Echo);		// ECN_Echo
		PacketDataLine4by6by5 = L". . . .  . . " + Urgent + L".  . . . . = Urgent : " + GetFlagSetNotSet(Urgent);		// Urgent
		PacketDataLine4by6by6 = L". . . .  . . ." + Acknowledgment + L"  . . . . = Acknowledgment : " + GetFlagSetNotSet(Acknowledgment);		// Acknowledgment
		PacketDataLine4by6by7 = L". . . .  . . . .  " + Push + L". . . = Push : " + GetFlagSetNotSet(Push);		// Push
		PacketDataLine4by6by8 = L". . . .  . . . .  . " + Reset + L". . = Reset : " + GetFlagSetNotSet(Reset);		// Reset
		PacketDataLine4by6by9 = L". . . .  . . . .  . . " + Syn + L". = Syn : " + GetFlagSetNotSet(Syn);		// Syn
		PacketDataLine4by6by10 = L". . . .  . . . .  . . ." + Fin + L" = Fin : " + GetFlagSetNotSet(Fin);		// Fin

		CString windowSize = Calculate4HexNumber(Packet_Dump_Data.Mid(96, 1), Packet_Dump_Data.Mid(97, 1), Packet_Dump_Data.Mid(98, 1), Packet_Dump_Data.Mid(99, 1));
		CString urgentPointer = Calculate4HexNumber(Packet_Dump_Data.Mid(104, 1), Packet_Dump_Data.Mid(105, 1), Packet_Dump_Data.Mid(106, 1), Packet_Dump_Data.Mid(107, 1));

		CString PacketDataLine4by7 = L"Window size value: " + windowSize;
		CString PacketDataLine4by8 = L"[Calculated window size: " + windowSize + L"]";
		CString PacketDataLine4by9 = L"Checksum: 0x" + Packet_Dump_Data.Mid(100, 4);
		CString PacketDataLine4by10 = L"Urgent pointer: " + urgentPointer;

		// Line 4
		PacketDataRoot4 = PacketDataCtrl.InsertItem(PacketDataLine4);
		HTREEITEM PacketDataRoot4Child1 = PacketDataCtrl.InsertItem(PacketDataLine4by1, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child2 = PacketDataCtrl.InsertItem(PacketDataLine4by2, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child3 = PacketDataCtrl.InsertItem(PacketDataLine4by3, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child4 = PacketDataCtrl.InsertItem(PacketDataLine4by4, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child5 = PacketDataCtrl.InsertItem(PacketDataLine4by5, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child6 = PacketDataCtrl.InsertItem(PacketDataLine4by6, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child6Child1 = PacketDataCtrl.InsertItem(PacketDataLine4by6by1, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child2 = PacketDataCtrl.InsertItem(PacketDataLine4by6by2, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child3 = PacketDataCtrl.InsertItem(PacketDataLine4by6by3, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child4 = PacketDataCtrl.InsertItem(PacketDataLine4by6by4, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child5 = PacketDataCtrl.InsertItem(PacketDataLine4by6by5, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child6 = PacketDataCtrl.InsertItem(PacketDataLine4by6by6, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child7 = PacketDataCtrl.InsertItem(PacketDataLine4by6by7, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child8 = PacketDataCtrl.InsertItem(PacketDataLine4by6by8, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child9 = PacketDataCtrl.InsertItem(PacketDataLine4by6by9, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child10 = PacketDataCtrl.InsertItem(PacketDataLine4by6by10, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child6Child11 = PacketDataCtrl.InsertItem(PacketDataLine4by6by11, PacketDataRoot4Child6);
		HTREEITEM PacketDataRoot4Child7 = PacketDataCtrl.InsertItem(PacketDataLine4by7, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child8 = PacketDataCtrl.InsertItem(PacketDataLine4by8, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child9 = PacketDataCtrl.InsertItem(PacketDataLine4by9, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child10 = PacketDataCtrl.InsertItem(PacketDataLine4by10, PacketDataRoot4);
	} else if (Protocol == L"UDP") {
		PacketDataLine4 = L"User Datagram protocol, Src Port: " + Line4SourcePort + L", Dst Port: " + Line4DestinationPort;

		CString Length = Calculate4HexNumber(Packet_Dump_Data.Mid(76, 1), Packet_Dump_Data.Mid(77, 1), Packet_Dump_Data.Mid(78, 1), Packet_Dump_Data.Mid(79, 1));

		CString PacketDataLine4by1 = L"Source Port: " + Line4SourcePort;
		CString PacketDataLine4by2 = L"Destination Port: " + Line4DestinationPort;
		CString PacketDataLine4by3 = L"Length: " + Length;
		CString PacketDataLine4by4 = L"Checksum: 0x" + Packet_Dump_Data.Mid(80, 4);

		PacketDataRoot4 = PacketDataCtrl.InsertItem(PacketDataLine4);
		HTREEITEM  PacketDataRoot4Child1 = PacketDataCtrl.InsertItem(PacketDataLine4by1, PacketDataRoot4);
		HTREEITEM  PacketDataRoot4Child2 = PacketDataCtrl.InsertItem(PacketDataLine4by2, PacketDataRoot4);
		HTREEITEM  PacketDataRoot4Child4 = PacketDataCtrl.InsertItem(PacketDataLine4by3, PacketDataRoot4);
		HTREEITEM  PacketDataRoot4Child5 = PacketDataCtrl.InsertItem(PacketDataLine4by4, PacketDataRoot4);

		CString UDPData = Packet_Dump_Data.Mid(84, _ttoi(Length));
		CString UDPDataLength = CString(std::to_string(UDPData.GetLength()).c_str());

		CString PacketDataLine5 = L"Data (" + UDPDataLength + " bytes )";
		CString PacketDataLine5by1 = L"Data: " + UDPData.Mid(0, 40) + L"...";
		CString PacketDataLine5by2 = L"[Length: " + UDPDataLength + L"]";

		PacketDataRoot5 = PacketDataCtrl.InsertItem(PacketDataLine5);
		HTREEITEM PacketDataRoot5Child1 = PacketDataCtrl.InsertItem(PacketDataLine5by1, PacketDataRoot5);
		HTREEITEM PacketDataRoot5Child2 = PacketDataCtrl.InsertItem(PacketDataLine5by2, PacketDataRoot5);

	} else if (Protocol == L"ARP") {
		PacketDataLine4 = L"Address Resolution Protocol";

		CString HardwareTypeNumber = Calculate4HexNumber(Packet_Dump_Data.Mid(28, 1), Packet_Dump_Data.Mid(29, 1), Packet_Dump_Data.Mid(30, 1), Packet_Dump_Data.Mid(31, 1));
		CString HardwareTypeStr = ArpHardwareType(HardwareTypeNumber);
		CString ProtocolType = Packet_Dump_Data.Mid(32, 4);
		CString HardwareSize = Calculate2HexNumber(Packet_Dump_Data.Mid(36, 1), Packet_Dump_Data.Mid(37, 1));
		CString ProtocolSize = Calculate2HexNumber(Packet_Dump_Data.Mid(38, 1), Packet_Dump_Data.Mid(39, 1));
		CString OpCodeNumber = Calculate4HexNumber(Packet_Dump_Data.Mid(40, 1), Packet_Dump_Data.Mid(41, 1), Packet_Dump_Data.Mid(42, 1), Packet_Dump_Data.Mid(43, 1));
		CString OpCodeStr = ArpOpcde(OpCodeNumber);
		CString SenderMacAddr = MakeIPAddressV6(Packet_Dump_Data.Mid(44, 2), Packet_Dump_Data.Mid(46, 2), Packet_Dump_Data.Mid(48, 2), Packet_Dump_Data.Mid(50, 2), Packet_Dump_Data.Mid(52, 2), Packet_Dump_Data.Mid(54, 2));
		CString SenderIpAddr = Source;
		CString TargetMacAddr = MakeIPAddressV6(Packet_Dump_Data.Mid(64, 2), Packet_Dump_Data.Mid(66, 2), Packet_Dump_Data.Mid(68, 2), Packet_Dump_Data.Mid(70, 2), Packet_Dump_Data.Mid(72, 2), Packet_Dump_Data.Mid(74, 2));;
		CString TargetIpAddr = Destination;

		CString PacketDataLine4by1 = L"Hardware type: " + HardwareTypeStr + L" (" + HardwareTypeNumber + L")";
		CString PacketDataLine4by2 = L"Protocol type: IPv4 (0x" + ProtocolType + L")";
		CString PacketDataLine4by3 = L"Hardware size: " + HardwareSize;
		CString PacketDataLine4by4 = L"Protocol size: " + ProtocolSize;
		CString PacketDataLine4by5 = L"Opcode: " + OpCodeStr + L" (" + OpCodeNumber + L")";
		CString PacketDataLine4by6 = L"Sender MAC address: " + SenderMacAddr;
		CString PacketDataLine4by7 = L"Sender IP address: " + SenderIpAddr;
		CString PacketDataLine4by8 = L"Target MAC address: " + TargetMacAddr;
		CString PacketDataLine4by9 = L"Target IP address: " + TargetIpAddr;

		PacketDataRoot4 = PacketDataCtrl.InsertItem(PacketDataLine4);
		HTREEITEM PacketDataRoot4Child1 = PacketDataCtrl.InsertItem(PacketDataLine4by1, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child2 = PacketDataCtrl.InsertItem(PacketDataLine4by2, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child3 = PacketDataCtrl.InsertItem(PacketDataLine4by3, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child4 = PacketDataCtrl.InsertItem(PacketDataLine4by4, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child5 = PacketDataCtrl.InsertItem(PacketDataLine4by5, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child6 = PacketDataCtrl.InsertItem(PacketDataLine4by6, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child7 = PacketDataCtrl.InsertItem(PacketDataLine4by7, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child8 = PacketDataCtrl.InsertItem(PacketDataLine4by8, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child9 = PacketDataCtrl.InsertItem(PacketDataLine4by9, PacketDataRoot4);

	} else if (Protocol == L"ICMP") {
		PacketDataLine4 = L"Ineternet Control Message Protocol";

		CString ICMPType = Calculate2HexNumber(Packet_Dump_Data.Mid(68, 1), Packet_Dump_Data.Mid(69, 1));
		CString ICMPCode = Calculate2HexNumber(Packet_Dump_Data.Mid(70, 1), Packet_Dump_Data.Mid(71, 1));
		CString ICMPChecksum = Packet_Dump_Data.Mid(72, 4);

		CString ICMPIdentifierBEDec = Calculate4HexNumber(Packet_Dump_Data.Mid(76, 1), Packet_Dump_Data.Mid(77, 1), Packet_Dump_Data.Mid(78, 1), Packet_Dump_Data.Mid(79, 1));
		CString ICMPIdentifierBEHex = Packet_Dump_Data.Mid(76, 4);

		CString ICMPIdentifierLEDec = Calculate4HexNumber(Packet_Dump_Data.Mid(78, 1), Packet_Dump_Data.Mid(79, 1), Packet_Dump_Data.Mid(76, 1), Packet_Dump_Data.Mid(77, 1));
		CString ICMPIdentifierLEHex = Packet_Dump_Data.Mid(78, 2) + Packet_Dump_Data.Mid(76, 2);

		CString ICMPSquenceNumberBEDec = Calculate4HexNumber(Packet_Dump_Data.Mid(80, 1), Packet_Dump_Data.Mid(81, 1), Packet_Dump_Data.Mid(82, 1), Packet_Dump_Data.Mid(83, 1));
		CString ICMPSquenceNumberBEHex = Packet_Dump_Data.Mid(80, 4);

		CString ICMPSquenceNumberLEDec = Calculate4HexNumber(Packet_Dump_Data.Mid(82, 1), Packet_Dump_Data.Mid(83, 1), Packet_Dump_Data.Mid(80, 1), Packet_Dump_Data.Mid(81, 1));
		CString ICMPSquenceNumberLEHex = Packet_Dump_Data.Mid(82, 2) + Packet_Dump_Data.Mid(80, 2);
		CString ICMPData = Packet_Dump_Data.Mid(84, _ttoi(Length));
		CString ICMPDataLength = CString(std::to_string(ICMPData.GetLength()).c_str());

		CString PacketDataLine4by1 = L"Type: " + ICMPType;
		CString PacketDataLine4by2 = L"Code: " + ICMPCode;
		CString PacketDataLine4by3 = L"Checksum: " + ICMPChecksum;
		CString PacketDataLine4by4 = L"Identifier (BE): " + ICMPIdentifierBEDec + L" (0x" + ICMPIdentifierBEHex + L")";
		CString PacketDataLine4by5 = L"Identifier (LE): " + ICMPIdentifierLEDec + L" (0x" + ICMPIdentifierLEHex + L")";
		CString PacketDataLine4by6 = L"Sequence number (BE): " + ICMPSquenceNumberBEDec + L" (0x" + ICMPSquenceNumberBEHex + L")";
		CString PacketDataLine4by7 = L"Sequence number (LE): " + ICMPSquenceNumberLEDec + L" (0x" + ICMPSquenceNumberLEHex + L")";
		CString PacketDataLine4by8 = L"Data (" + ICMPDataLength + " bytes )";
		CString PacketDataLine4by8by1 = L"Data :" + ICMPData;

		PacketDataRoot4 = PacketDataCtrl.InsertItem(PacketDataLine4);
		HTREEITEM PacketDataRoot4Child1 = PacketDataCtrl.InsertItem(PacketDataLine4by1, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child2 = PacketDataCtrl.InsertItem(PacketDataLine4by2, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child3 = PacketDataCtrl.InsertItem(PacketDataLine4by3, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child4 = PacketDataCtrl.InsertItem(PacketDataLine4by4, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child5 = PacketDataCtrl.InsertItem(PacketDataLine4by5, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child6 = PacketDataCtrl.InsertItem(PacketDataLine4by6, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child7 = PacketDataCtrl.InsertItem(PacketDataLine4by7, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child8 = PacketDataCtrl.InsertItem(PacketDataLine4by8, PacketDataRoot4);
		HTREEITEM PacketDataRoot4Child8Child1 = PacketDataCtrl.InsertItem(PacketDataLine4by8by1, PacketDataRoot4Child8);
	}

	//PacketDataCtrl.Expand(PacketDataRoot1, TVE_EXPAND);
	//PacketDataCtrl.Expand(PacketDataRoot2, TVE_EXPAND);
	//PacketDataCtrl.Expand(PacketDataRoot3, TVE_EXPAND);
	//PacketDataCtrl.Expand(PacketDataRoot4, TVE_EXPAND);
	//PacketDataCtrl.Expand(PacketDataRoot5, TVE_EXPAND);

	PacketDataCtrl.Invalidate();
	PacketDataCtrl.UpdateWindow();
}

CString CMFCApplication1Dlg::GetFlagSetNotSet(CString _Flag) {
	int Length = _Flag.GetLength();

	if (Length == 3) {
		return (_Flag.Compare(L"000") == 0) ? L"Not set" : L"Set";
	}
	if (Length == 1) {
		return (_Flag.Compare(L"0") == 0) ? L"Not set" : L"Set";
	}
	return L"";
}

CString CMFCApplication1Dlg::Calculate4HexNumber(CString num1, CString num2, CString num3, CString num4) {
	return CString(std::to_string((
		_ttoi(HexToDec(num1)) * 16 * 16 * 16 +
		_ttoi(HexToDec(num2)) * 16 * 16 +
		_ttoi(HexToDec(num3)) * 16 +
		_ttoi(HexToDec(num4)) * 1
		)).c_str());
}

CString CMFCApplication1Dlg::Calculate2HexNumber(CString num1, CString num2) {
	return CString(std::to_string((
		_ttoi(HexToDec(num1)) * 16 +
		_ttoi(HexToDec(num2)) * 1
		)).c_str());
}

CString CMFCApplication1Dlg::MakeIPAddressV6(CString Aclass, CString Bclass, CString Cclass, CString Dclass, CString Eclass, CString Fclass) {
	return Aclass + L":" + Bclass + L":" + Cclass + L":" + Dclass + L":" + Eclass + L":" + Fclass;
}

CString CMFCApplication1Dlg::ChangeHexToAscii(CString HexData) {
	if (HexData == L"a") {
		HexData = L"10";
	} else if (HexData == L"b") {
		HexData = L"11";
	} else if (HexData == L"c") {
		HexData = L"12";
	} else if (HexData == L"d") {
		HexData = L"13";
	} else if (HexData == L"e") {
		HexData = L"14";
	} else if (HexData == L"f") {
		HexData = L"15";
	}

	if (HexData == L"a") {
		HexData = L"10";
	} else if (HexData == L"b") {
		HexData = L"11";
	} else if (HexData == L"c") {
		HexData = L"12";
	} else if (HexData == L"d") {
		HexData = L"13";
	} else if (HexData == L"e") {
		HexData = L"14";
	} else if (HexData == L"f") {
		HexData = L"15";
	}

	return HexData;
}

void CMFCApplication1Dlg::OnBnClickedButton4() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT1, Filter);

	if (IsFilterApply) {
		IsFilterApply = false;
	} else {
		IsFilterApply = true;
	}

	m_ListCtrl.DeleteAllItems();

	if (m_pThread123 == NULL) {
		m_ListCtrl.DeleteAllItems();
		m_pThread123 = AfxBeginThread(ThreadFunctionSecondTest, this);

		m_ThreadWorkType123 = RUNNING;
	} else {
		if (m_ThreadWorkType123 == RUNNING) {
			m_pThread123->SuspendThread();
			m_ThreadWorkType123 = PAUSE;
		} else if (m_ThreadWorkType123 == PAUSE) {
			m_pThread123->ResumeThread();
			m_ThreadWorkType123 = RUNNING;
		}
	}
}

CString CMFCApplication1Dlg::ArpOpcde(CString OpcodeNumber) {
	CString OpcodeStr = L"";
	if (OpcodeNumber.Compare(L"1") == 0) {
		OpcodeStr = "Request";
	} else if (OpcodeNumber.Compare(L"2") == 0) {
		OpcodeStr = "Reply";
	}
	return OpcodeStr;
}

CString CMFCApplication1Dlg::ArpHardwareType(CString HardwareTypeNumber) {
	CString HardwareTypeStr = L"";
	if (HardwareTypeNumber.Compare(L"1") == 0) {
		HardwareTypeStr = "Ethernet";
	} else if (HardwareTypeNumber.Compare(L"2") == 0) {
		HardwareTypeStr = "Experimental Ethernet";
	} else if (HardwareTypeNumber.Compare(L"3") == 0) {
		HardwareTypeStr = "Amateur Radio";
	} else if (HardwareTypeNumber.Compare(L"4") == 0) {
		HardwareTypeStr = "Proteon ProNet Token Ring";
	} else if (HardwareTypeNumber.Compare(L"5") == 0) {
		HardwareTypeStr = "IEEE 802.3 networks";
	}
	return HardwareTypeStr;
}

UINT CMFCApplication1Dlg::ThreadFunctionSecondTest(LPVOID _mothod) {
	CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->m_pMainWnd;

	/* 파일에서 읽어오기 */
	CString Time;
	CString SIP;
	CString DIP;
	CString PROTO;
	CString LENGTH;
	CString INFO;
	CString DUMP;


	std::ifstream is("test.txt");
	long length = 1024;
	char* buffer = NULL;
	std::string temp;

	buffer = new char[length];
	memset(buffer, 0, length);

	int cnt = 0;
	int i = 0;
	while (1) {
		if (is) {
			is.getline(buffer, 49);

			if (strcmp(buffer, "-----------------------------------------------") == 0) {
				DUMP = temp.c_str();
				DUMP.Replace(L" ", L"");
				DUMP.Replace(L"\n", L"");

				//마지막에 공백 한칸 있음
				//PROTO.Replace(L" ", L"");

				int column_count = pDlg->m_ListCtrl.GetItemCount();

				CString column_count_str;
				column_count_str.Format(_T("%d"), column_count + 1);
				pDlg->m_ListCtrl.InsertItem(column_count, column_count_str);

				pDlg->m_ListCtrl.SetItem(column_count, 1, LVIF_TEXT, Time, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 2, LVIF_TEXT, SIP, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 3, LVIF_TEXT, DIP, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 4, LVIF_TEXT, PROTO, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 5, LVIF_TEXT, LENGTH, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 6, LVIF_TEXT, INFO, NULL, NULL, NULL, NULL);
				pDlg->m_ListCtrl.SetItem(column_count, 7, LVIF_TEXT, DUMP, NULL, NULL, NULL, NULL);

				i++;
				cnt = 0;
				temp = "";
			} else {
				if (cnt == 0) {
					Time = buffer;
				} else if (cnt == 1) {
					SIP = buffer;
				} else if (cnt == 2) {
					DIP = buffer;
				} else if (cnt == 3) {
					PROTO = buffer;
				} else if (cnt == 4) {
					LENGTH = buffer;
				} else if (cnt == 5) {
					INFO = buffer;
				} else {
					temp += buffer;
				}
				cnt++;
			}
		}
	}

	return 0;
}