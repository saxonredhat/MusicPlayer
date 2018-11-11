#include "MusicPlayer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG, Cls_OnInitDialog);
		HANDLE_MSG(hDlg, WM_COMMAND, Cls_OnCommand);
		//HANDLE_MSG(hDlg, WM_LBUTTONDBLCLK, Cls_OnLButtonDown);
		HANDLE_MSG(hDlg, WM_CLOSE, Cls_OnClose);
		HANDLE_MSG(hDlg, WM_HSCROLL, Cls_OnHScroll);
		HANDLE_MSG(hDlg, WM_CONTEXTMENU, Cls_OnContextMenu);
		return TRUE;
	}
	return FALSE;
}

BOOL Cls_OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam) {
	hDlg_Com = hDlg;
	TCHAR MenuName[10] = L"����";
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MUSIC));
	SetClassLong(hDlg, GCL_HICON, (long)hIcon);
	//��ȡ�����ļ��ĸ赥
	LoadMusic(hDlg);
	/*SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)L"E:\\BaiduMusic\\Songs\\��������-����.mp3");
	SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)L"E:\\BaiduMusic\\Songs\\�����ѳ�����-�Ź���.mp3");
	SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)L"E:\\BaiduMusic\\Songs\\����ϰ��-���»�.mp3");
	SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)L"E:\\BaiduMusic\\Songs\\����ͺ�-������,�Ź���.mp3");
	SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)L"E:\\BaiduMusic\\Songs\\�Թ�-̷��,���ෲ.mp3");*/
	return TRUE;
}

void Cls_OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify) {
	//��ʼ��ť
	if (id == IDSTART) {
		Start();
	}
	//��ͣ��ť
	if (id == IDPAUSE) {
		//��ȡ��ͣ������ť�ϵ�����
		TCHAR ButtonVal[4];
		Button_GetText(GetDlgItem(hDlg, IDPAUSE), ButtonVal, MAX_PATH);
		//�������ͣ������ͣ���֣�����Ǽ��������������
		if (lstrcmp(ButtonVal, L"��ͣ") == 0) {
			PauseMusic(CurrentMusic);
			Button_SetText(GetDlgItem(hDlg, IDPAUSE), L"����");
		}
		else if (lstrcmp(ButtonVal, L"����") == 0) {
			ResumeMusic(CurrentMusic);
			Button_SetText(GetDlgItem(hDlg, IDPAUSE), L"��ͣ");
		}
	}
	//�����б�
	if (id == IDC_LIST1) {
		switch (codeNotify)
		{
		case LBN_DBLCLK:
			Start();
			break;
		}
	}
	//��ݲ˵�
	if (id == MENUADD) {
		//������ֵ��ļ�
		AddMusic(hDlg);
		//��ճ���ǰ�赥�б�(������ļ��е�����)
		ClsScreen(hDlg);
		//���¼��ص������б�
		LoadMusic(hDlg);
	}
	if (id == MENUDEL) {
		DelMusic(hDlg);
	}
	if (id == MENUCLS) {
		ClsMusic(hDlg);
	}
}

void Cls_OnHScroll(HWND hDlg, HWND hwndCtl, UINT code, int pos) {
	HWND hTemp = GetDlgItem(hDlg, IDC_SLIDER1);
	UINT iCode = code;
	if (hwndCtl == hTemp) {
		switch (code) {
		case TB_THUMBPOSITION:
			StopMusic(CurrentMusic);
			PlayMusicFromPos(CurrentMusic, pos * 1000);
			break;
		}
	}
}

void Cls_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	if (keyFlags == IDC_LIST1) {
		int listIndex = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETCURSEL, 0, 0);
		int MusicTotalSec;
		if (listIndex != LB_ERR) {
			TCHAR MusicName[MAX_PATH];
			SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETTEXT, listIndex, (LPARAM)MusicName);
			if (lstrlen(CurrentMusic) > 0) {
				StopMusic(CurrentMusic);
			}
			lstrcpy(CurrentMusic, MusicName);
			StartMusic(CurrentMusic);
			Button_SetText(GetDlgItem(hDlg, IDPAUSE), L"��ͣ");
			//���û�������䷶Χ
			MusicTotalSec = GetMusicMsec(CurrentMusic) / 1000;
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, TRUE, MAKELPARAM(0, MusicTotalSec));
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, TRUE, 0);
			//����һ����ʱ��
			SetTimer(hDlg, ID_TIMER1, 500, (TIMERPROC)DrawMusicBanner);
		}
		else {

			MessageBox(hDlg, L"��ѡ�����", L"��ѡ�����", MB_OK);
		}
	}
}

void Cls_OnContextMenu(HWND hDlg, HWND hwndContext, UINT xPos, UINT yPos) {
	if (hwndContext == GetDlgItem(hDlg, IDC_LIST1)) {
		hMenu = CreatePopupMenu();
		InsertMenu(hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, MENUADD, L"��Ӹ���");
		InsertMenu(hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, MENUDEL, L"ɾ������");
		InsertMenu(hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, MENUCLS, L"����б�");
		TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN, xPos, yPos, 0, hDlg, NULL);
	}

}


void Cls_OnClose(HWND hDlg) {
	EndDialog(hDlg, 0);
}

void Start() {
	HWND hDlg = hDlg_Com;
	int listIndex = SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETCURSEL, 0, 0);
	TCHAR MusicName[MAX_PATH];
	TCHAR MusicLongPath[MAX_PATH];
	int MusicTotalSec;
	if (listIndex != LB_ERR) {
		SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_GETTEXT, listIndex, (LPARAM)MusicName);
		if (lstrlen(CurrentMusic) > 0) {
			StopMusic(CurrentMusic);
		}
		GetPrivateProfileString(appName, MusicName, L"", MusicLongPath, MAX_PATH, MusicListFile);
		GetShortPathName(MusicLongPath, musicShortPath, MAX_PATH);
		lstrcpy(CurrentMusic, musicShortPath);
		StartMusic(CurrentMusic);
		
		Button_SetText(GetDlgItem(hDlg, IDPAUSE), L"��ͣ");
		//���û�������䷶Χ
		MusicTotalSec = GetMusicMsec(CurrentMusic) / 1000;
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, TRUE, MAKELPARAM(0, MusicTotalSec));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, TRUE, 0);
		//����һ����ʱ��
		SetTimer(hDlg, ID_TIMER1, 500, (TIMERPROC)DrawMusicBanner);
	}
	else {

		MessageBox(hDlg, L"��ѡ�����", L"��ѡ�����", MB_OK);
	}
}

void StartMusic(TCHAR *MusicName) {
	GetShortPathName(MusicName, musicShortPath, MAX_PATH);
	TCHAR Order[MAX_PATH];
	wsprintf(Order, L"open %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
	wsprintf(Order, L"play %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
}

void StopMusic(TCHAR *MusicName) {
	
	TCHAR Order[MAX_PATH];
	wsprintf(Order, L"stop %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
	wsprintf(Order, L"close %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
}

void PauseMusic(TCHAR *MusicName) {
	TCHAR Order[MAX_PATH];
	wsprintf(Order, L"pause %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
}

void ResumeMusic(TCHAR *MusicName) {
	TCHAR Order[MAX_PATH];
	wsprintf(Order, L"resume %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
}

void PlayMusicFromPos(TCHAR *MusicName, int pos) {
	TCHAR Order[MAX_PATH];
	wsprintf(Order, L"open %s", MusicName);
	mciSendString(Order, NULL, 0, NULL);
	wsprintf(Order, L"play %s from %d", MusicName, pos);
	mciSendString(Order, NULL, 0, NULL);
}

void DrawMusicBanner() {
	HWND hDlg = hDlg_Com;
	TCHAR MusicTime[10];
	int MusicMsec;
	int MusicMsecTotal;
	//��ȡ��ǰ���ŵĺ�����
	MusicMsec = GetMusicPosMsec(CurrentMusic);
	wsprintf(MusicTime, L"%02d:%02d", (MusicMsec / 1000) / 60, (MusicMsec / 1000) % 60);
	//�ػ��������ʱ��
	Edit_SetText(GetDlgItem(hDlg, IDCMUSICTIME), MusicTime);
	//�ػ�����ܹ�ʱ��
	MusicMsecTotal = GetMusicMsec(CurrentMusic);
	wsprintf(MusicTime, L"%02d:%02d", (MusicMsecTotal / 1000) / 60, (MusicMsecTotal / 1000) % 60);
	Edit_SetText(GetDlgItem(hDlg, IDCMUSICLEN), MusicTime);
	//���������λ��
	//���õ�ǰ�����λ��
	SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, TRUE, MusicMsec / 1000);
	if (MusicMsec == MusicMsecTotal) {
		StopMusic(CurrentMusic);
	}
}

int GetMusicMsec(TCHAR *MusicName) {
	TCHAR Order[MAX_PATH];
	TCHAR MusicMsec[10];
	wsprintf(Order, L"status %s length", MusicName);
	mciSendString(Order, MusicMsec, sizeof(MusicMsec), NULL);
	return _wtoi(MusicMsec);
}

int GetMusicPosMsec(TCHAR *MusicName) {
	TCHAR Order[MAX_PATH];
	TCHAR MusicMsec[10];
	wsprintf(Order, L"status %s Position", MusicName);
	mciSendString(Order, MusicMsec, sizeof(MusicMsec), NULL);
	return _wtoi(MusicMsec);
}

void AddMusic(HWND hwnd) {
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH * 30];
	TCHAR musicDir[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(szFileName, sizeof(szFileName));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;    //�����ھ��  
	ofn.lpstrFilter = TEXT("MP3�ļ� (*.mp3)\0*.mp3\0WAV�ļ� (*.wav)\0*.wav\0");
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;
	ofn.lpstrInitialDir = TEXT("C:\\Users\\");

	TCHAR *p;
	if (GetOpenFileName(&ofn))
	{
		wcsncpy(musicDir, szFileName, ofn.nFileOffset);
		if (lstrlen(szFileName)+1 == ofn.nFileOffset) {
			lstrcat(musicDir, L"\\");
		}
		p = szFileName + ofn.nFileOffset;
		while (*p){
			TCHAR musicPath[MAX_PATH];
			TCHAR musicName[MAX_PATH];
			lstrcpy(musicName, p);
			lstrcpy(musicPath, musicDir);
			lstrcat(musicPath, musicName);
			CutSuffix(musicName);
			WritePrivateProfileString(appName,musicName, musicPath, MusicListFile);
			//SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)musicPath);
			p = p + lstrlen(p) + 1;
		}
	}
}

void LoadMusic(HWND hwnd) {
	TCHAR lpMusicList[MAX_PATH];
	TCHAR MusicName[MAX_PATH];
	TCHAR *p;
	//��ȡ�赥�ļ�
	GetPrivateProfileString(appName, NULL, L"", lpMusicList, MAX_PATH, MusicListFile);
	p=lpMusicList;
	while (*p) {
		lstrcpy(MusicName,p);
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)MusicName);
		p = p + lstrlen(MusicName) + 1;
	}
	//SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, (WPARAM)NULL, (LPARAM)musicPath);
}

void DelMusic(HWND hwnd) {
	TCHAR MusicName[MAX_PATH];
	int itemIndex=SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETCURSEL, (WPARAM)NULL, (LPARAM)NULL);
	SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETTEXT, itemIndex, (LPARAM)MusicName);
	//ɾ���ļ���Ӧ�ĸ���
	WritePrivateProfileString(appName, MusicName, NULL, MusicListFile);
	//ɾ�������б��Ӧ����Ŀ
	SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)itemIndex, (LPARAM)NULL);
}

void ClsMusic(HWND hwnd) {
	//����ļ��и����嵥
	WritePrivateProfileString(appName, NULL, NULL, MusicListFile);
	//��ճ����б��Ӧ����Ŀ
	ClsScreen(hwnd);
}

void ClsScreen(HWND hwnd) {
	int itemCount = SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
	//��ճ����б��Ӧ����Ŀ
	for (int i = 0; i < itemCount; i++) {
		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)0, (LPARAM)NULL);
	}
}

void CutSuffix(TCHAR *p)
{
	int i = lstrlen(p);
	TCHAR* p2 = p + i;
	while (p2 > p)
	{
		if (*p2 == TEXT('.'))
		{
			*p2 = TEXT('\0');
			break;
		}
		p2--;
	}
}
/*lstrcpy(musicDir, szFileName);
		lstrcat(musicDir, L"\\");

		p = szFileName + ofn.nFileOffset;
		while (*p)
		{
			TCHAR musicPath[MAX_PATH] = L"";
			lstrcpy(musicPath, musicDir);
			lstrcat(musicPath, p);

			TCHAR musicName[50] = L"";
			lstrcpy(musicName, p);
			CutSuffix(musicName);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_INSERTSTRING, -1, (LPARAM)musicName);

			WritePrivateProfileString(appName, musicName, musicPath, iniPath);
			p = p + lstrlen(p) + 1;
		}*/
