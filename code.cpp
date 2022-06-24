#include<Windows.h>
#include <CommDlg.h>
#include <mmsystem.h>
#include <time.h>
#include<Commctrl.h>
#include"resource.h"
#include"clases.h"
#pragma warning(disable : 4996) 

#define TM_RELOJ 3000
#define IDR_MENU1 MENU

// aber 
HWND txbUser;
HWND txbPass;

#pragma region Instancias
HINSTANCE hInst;
HINSTANCE hInstGlob;
HWND hMain = 0;
HWND hLogin = 0;
HWND hCits = 0;
HWND hMecs = 0;
HWND hClients = 0;
HWND hServs = 0;
HMENU hMenu = 0;
#pragma endregion

#pragma region Variables de Timer
time_t unixsecs;
time_t unixsecs2;
struct tm* _actualTime;
struct tm* _actualTime2;
HWND hLbl_Reloj;
HWND hLbl_Reloj2;

#pragma endregion

#pragma region Funciones
string validNames(char tempbrand[50], bool& OK);
bool validDate(char fecha[],char hora[]);
bool validAttend(char fecha[], char hora[], bool attend);
string shortDateChanger(char fecha[]);
void txtReport(char nombredeArchivo[], HWND listbox, HWND hDlg, int option);
#pragma endregion

#pragma region Variables Globales de busqueda
Mechs::Nodo* found = NULL;
Servs::Node* serFound = NULL;
Servs::Node* ser2Found = NULL;
Clts::Node* clfound=NULL;
Clts::Node* findCl = NULL;

#pragma endregion

BOOL CALLBACK hMainFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hLoginFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hMecsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hCitsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hClientsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hServsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

//funcion principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR cmdLine, int showCmd)
{
	hInst = hInstance;
	// Create the modeless dialog window. This program is a pure
	// dialog window application, and the dialog window is the
	// "main" window.
	hMain = CreateDialog(hInstance, // Application instance.
		MAKEINTRESOURCE(IDD_MAIN), // Dialog resource ID.
		0, // Parent window--null for no parent.
		(DLGPROC)hMainFun);// Dialog window procedure.
	
	hMenu = GetMenu(hMain);
	
	//se inicia el timer en las ventanas que queramos
	SetTimer(hMain, TM_RELOJ, 1000, NULL);


	// Show the dialog.
	ShowWindow(hMain, 0);
	hInst = hInstance;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (hMain == 0 || !IsDialogMessage(hMain, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//callback ventana principal
BOOL CALLBACK hMainFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		//vamos a crear Servs
		/*Servs* Especialidades = new Servs();
		Especialidades->ServsIsEmpty();*/
		//vamos a ver si jala
		Mechanics = new Mechs();
		                               // esto es un label, un static text en la ventana
		hLbl_Reloj = GetDlgItem(hDlg, Lbl_Timer);
		
		Usuario.loadData();
		//LO PRIMERO QUE SE VE ES EL LOGIN
		ShowWindow(hMain, SW_HIDE);
		hLogin=CreateDialog(hInstGlob, MAKEINTRESOURCE(IDD_LOGIN), hMain, hLoginFun);
		ShowWindow(hLogin, SW_SHOW);
		//DESPUES DE ESTO CARGAMOS EL NOMBRE COMPLETO DEL USUARIO Y SU FOTO PA Q SE VEA BIEN
		//LBL_USERNAME ES EL LABEL DONDE PONDREMOS EL NOMBRE DEL USUARIO QUE ACCESO
		/*SendMessageA(GetDlgItem(hMain, LBL_USERNAME) , WM_SETTEXT, 0, (LPARAM)Usuario.cUser);*/
		
		SetWindowTextA(GetDlgItem(hDlg, LBL_USERNAME), Usuario.cUser);

		HWND PICM = GetDlgItem(hDlg, PC_USERMA);
		loadPic(PICM, Usuario.cpicU, 150, 250);

	}
	break;
	case WM_COMMAND:
	{

		switch (wParam) {
		case 40014:
			ShowWindow(hMain, SW_HIDE);
			hServs=CreateDialog(hInst, MAKEINTRESOURCE(IDD_SERVICES), hMain, hServsFun);
			ShowWindow(hServs, SW_SHOW);
			break;
		case 40015:
			ShowWindow(hMain, SW_HIDE);
			hMecs=CreateDialog(hInst, MAKEINTRESOURCE(IDD_MECHS), hMain, hMecsFun);
			ShowWindow(hMecs, SW_SHOW);
			break;
		case 40016:
			ShowWindow(hMain, SW_HIDE);
			hCits=CreateDialog(hInst, MAKEINTRESOURCE(IDD_CITAS), hMain, hCitsFun);
			ShowWindow(hCits, SW_SHOW);
			break;
		case 40017:
			ShowWindow(hMain, SW_HIDE);
			hClients=CreateDialog(hInst, MAKEINTRESOURCE(IDD_CLIENTS), hMain, hClientsFun);
			ShowWindow(hClients, SW_SHOW);
			break;
		case ID_SALIR:
			delete Mechanics;
			DestroyWindow(hDlg);
			break;
		}
		break;
	}
	case WM_TIMER: {
		time(&unixsecs);
		_actualTime = localtime(&unixsecs);

		char reloj[80];
		strftime(reloj, 80, "%d-%m-%Y %I:%M:%S", _actualTime);
		/*SetWindowText(hLbl_Reloj, reloj);*/
		SendMessageA(hLbl_Reloj, WM_SETTEXT, 0, (LPARAM)reloj);
	}
	break;
	case WM_CLOSE:
	{
		KillTimer(hMain,TM_RELOJ);
		/*DestroyWindow(hDlg);*/
		break;
	}
	case WM_DESTROY:
	{
		//aqui se guarda todo
		PostQuitMessage(0);
		break;;
	}
	}
	return 0;
}
BOOL CALLBACK hLoginFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		txbUser = GetDlgItem(hDlg, TB_USER);
		txbPass = GetDlgItem(hDlg, TB_PASS);
		HWND PICL= GetDlgItem(hDlg, PB_AppL);
		loadPic(PICL, Usuario.cpicA, 200, 120);
	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{
		case IDOK: {
			char userN[20] = { NULL };
			char passN[20] = { NULL };

			int lengthUser = GetWindowTextLength(txbUser) + 1;
			int lengthPass = GetWindowTextLength(txbPass) + 1;

			GetWindowTextA(txbUser, userN, lengthUser);
			GetWindowTextA(txbPass, passN, lengthPass);

			string sUseR(userN);
			string sPasS(passN);
	
			if (!Usuario.loadData()) {
				MessageBoxA(hLogin,"Error de archivo", "error", MB_ICONERROR);
			}
			else {
		
				if (!Usuario.compData(sUseR, sPasS)) {
					MessageBoxA(hLogin,"Usuario o contrasena incorrectos", "error", MB_ICONERROR);
					SendMessageA(GetDlgItem(hDlg, TB_USER), WM_SETTEXT, 0, (LPARAM)"");
					SendMessageA(GetDlgItem(hDlg, TB_PASS), WM_SETTEXT, 0, (LPARAM)"");
				}else {
				
					
					//PUEDO ESCONDER EL LOGIN SI QUIERO, PERO YA NO ME SIRVE, QUE SE MUERA
					ShowWindow(hMain, SW_SHOW);
					EndDialog(hLogin, 0);
				}
			}

			//Recuerda validar para un usuario, aunque sea una sola letra :3
			//Pd: TQM ;v

			break;
		}
		case BN_SALIR: {
			EndDialog(hDlg, 0);
			DestroyWindow(hMain);
			
			break;
		}
		}
		break;
	}

	case WM_CLOSE:
	{
	/*	DestroyWindow(hMain);*/
		EndDialog(hDlg, 0);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hMecsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		SendMessageA(GetDlgItem(hDlg, LBL_USERM), WM_SETTEXT, 0, (LPARAM)Usuario.cUser);
		//sin servicios no registras ni mecanicos
		if (Services.lista == NULL) {
			MessageBoxA(NULL, "No hay servicios. Registre uno primero", "Error", MB_ICONEXCLAMATION);
		}
		else {
			Services.aux = Services.lista;
			while (Services.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
				Services.aux = Services.aux->next;
			}
			Services.aux = Services.origen;

			Mechanics->aux = Mechanics->lista;
			while (Mechanics->aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_ADDSTRING, 0, (LPARAM)Mechanics->aux->name);
				Mechanics->aux = Mechanics->aux->next;
			}
			Mechanics->aux = Mechanics->origen;

			/*SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_SETCURSEL, -1, NULL);*/
		}

		EnableWindow(GetDlgItem(hDlg, BTN_EDITM), false);
		EnableWindow(GetDlgItem(hDlg, BTN_DELETEM), false);
		
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == CB_SERVM && HIWORD(wParam) == CBN_SELCHANGE) {
			char testServ[50];
		
			int selBIndex = (int)SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_GETCURSEL, 0, 0);
			if (selBIndex == -1) {

			}
			else {
				SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_GETLBTEXT, (WPARAM)selBIndex, (LPARAM)testServ);

				Servs::Node* var = Services.findItem(testServ);
				if (!Services.fullSpace(var)) {
					if (Services.freeSpace(var, 0) || Services.freeSpace(var, 1) || Services.freeSpace(var, 2)) {
						SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_ADDSTRING, 0, (LPARAM)"LUNES A VIERNES");
						if (Services.freeSpace(var, 0)) {
							SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"8AM-12PM");
						}
						if (Services.freeSpace(var, 1)) {
							SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"12PM-4PM");
						}
						if (Services.freeSpace(var, 2)) {
							SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"4PM-8PM");
						}
					}
					
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);
				}
				else {
					MessageBoxA(NULL, "El servicio ya no tiene mas espacios", "Error", MB_ICONEXCLAMATION);
					SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);
				}
			}
		}
		else if (LOWORD(wParam) == LB_MECS && HIWORD(wParam) == LBN_SELCHANGE) {
			char testMec[50];

			int selIndex = (int)SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETCURSEL, 0, 0);
			if (selIndex == -1) {

			}
			else {
#pragma region Deactivate buttons
				EnableWindow(GetDlgItem(hMecs, TB_NAMEM), false);
				EnableWindow(GetDlgItem(hMecs, TB_TELM), false);
				EnableWindow(GetDlgItem(hMecs, CB_SERVM), false);
				EnableWindow(GetDlgItem(hMecs, CB_SCHEDW), false);
				EnableWindow(GetDlgItem(hMecs, CB_SCHEDH), false);
				//BUTTONS deactivate
				EnableWindow(GetDlgItem(hMecs, BTN_LOADM), false);
				//buttons activate
				EnableWindow(GetDlgItem(hMecs, BTN_EDITM), true);
				EnableWindow(GetDlgItem(hMecs, BTN_DELETEM), true);
				EnableWindow(GetDlgItem(hMecs, IDOK), false);




#pragma endregion


				SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testMec);
				found = Mechanics->findItem(testMec);
				if (found != NULL) {
#pragma region fillCombos
					
					SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_RESETCONTENT, 0, 0);
					Services.aux = Services.lista;
					while (Services.aux != NULL) {
						SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
						Services.aux = Services.aux->next;
					}Services.aux = Services.lista;

					SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_RESETCONTENT, 0, 0);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_ADDSTRING, 0, (LPARAM)"LUNES A VIERNES");
					
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_RESETCONTENT, 0, 0);
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"8AM-12PM");
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"12PM-4PM");
					SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_ADDSTRING, 0, (LPARAM)"4PM-8PM");
				


#pragma endregion

					char limpia[20] = "";
					SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)found->name);
					SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)found->telephone);
					string numemp=to_string(found->idEmp);
					SendMessageA(GetDlgItem(hDlg, LBL_NUMEMP), WM_SETTEXT, 0, (LPARAM)numemp.c_str());

					SetWindowTextA(GetDlgItem(hDlg, CB_SERVM),found->occupation);
					char weekSched[20] = { "" };
					char hourSched[20] = { "" };
					if (found->empSched.weekSche == 1) {
						strcpy(weekSched, "LUNES A VIERNES");

						if (found->empSched.hourSche == 1) {
							strcpy(hourSched, "8AM-12PM");
						}
						else if (found->empSched.hourSche == 2) {
							strcpy(hourSched, "12PM-4PM");
						}
						else if (found->empSched.hourSche == 3) {
							strcpy(hourSched, "4PM-8PM");
						}
					}
				

					SetWindowTextA(GetDlgItem(hDlg, CB_SCHEDW), weekSched);
					SetWindowTextA(GetDlgItem(hDlg, CB_SCHEDH), hourSched);
					
					SendMessageA(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, 0, (LPARAM)found->pic);
					SendMessageA(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
					loadPic(GetDlgItem(hDlg, PC_MECH),found->pic,50,50);
					
				}
				
			}
		}
		else if (LOWORD(wParam) == BTN_LOADM && HIWORD(wParam) == BN_CLICKED) {
			char PICBuff[MAX_PATH] = "";
			HWND PIC = GetDlgItem(hMecs, PC_MECH);
			examinar(hMecs, GetDlgItem(hMecs, LBL_PIC));
			GetDlgItemTextA(hDlg, LBL_PIC, PICBuff, MAX_PATH);
			loadPic(PIC, PICBuff, 50, 50);
		}
		else if (LOWORD(wParam) == BTN_EDITM && HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hMecs, TB_NAMEM), true);
		EnableWindow(GetDlgItem(hMecs, TB_TELM), true);
		EnableWindow(GetDlgItem(hMecs, CB_SERVM), true);
		EnableWindow(GetDlgItem(hMecs, CB_SCHEDW), true);
		EnableWindow(GetDlgItem(hMecs, CB_SCHEDH), true);
		EnableWindow(GetDlgItem(hMecs, BTN_LOADM), true);
		/*EnableWindow(GetDlgItem(hMecs, LB_MECS), false);*/
		EnableWindow(GetDlgItem(hMecs, IDOK), true);
}
		else if (LOWORD(wParam) == BTN_DELETEM && HIWORD(wParam) == BN_CLICKED) {
		char tempname[50] = "";
		SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
		found = Mechanics->findItem(tempname);
		if (found != NULL) {
			//ELIMINAR DE LA LISTBOX
			int items = GetListBoxInfo(GetDlgItem(hMecs, LB_MECS));
			int selIndex = 0;
			for (selIndex = 0; selIndex < items; selIndex++) {
				char testring[50] = "";
				SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testring);
				if (strcmp(found->name, testring) == 0) {
					SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_DELETESTRING, (WPARAM)selIndex, 0);
				}
			}
			Mechanics->delItem(found);
		}
		char limpia[20] = "";
		SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, LBL_NUMEMP), WM_SETTEXT, 0, (LPARAM)"");

		SendMessage(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
		SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
		SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);

		SendMessage(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
		SendMessage(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);

		EnableWindow(GetDlgItem(hDlg, LB_MECS), true);
		SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL, -1, NULL);
		EnableWindow(GetDlgItem(hMecs, BTN_DELETEM), false);
		EnableWindow(GetDlgItem(hMecs, BTN_EDITM), false);
}
		else if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(hMecs, RB_REGM) == BST_CHECKED) {
				char tempname[50];
				char temphone[13];
				char tempserv[30];
				char tempwsched[30];
				char temphsched[20];
				char temppic[MAX_PATH];
				bool OK1=false, OK2 = false, OK3 = false, OK4 = false, OK5 = false, OK6 = false;
				
				//ok1

#pragma region NAME
				SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
				int cont = 0;
				int j = 0;
				if (tempname[j] == '\0') {
					MessageBoxA(NULL, "El nombre esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempname[j] != '\0')
					{
						if (((tempname[j] >= 97) && (tempname[j] <= 122)) || ((tempname[j] >= 65) && (tempname[j] <= 90)) || (tempname[j] == 32))
						{
							if (j == 0)
							{
								if ((tempname[j] >= 65) && (tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}

								else if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j] - 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}
							}
							else if (j > 0)
							{
								if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j];
								}
								else if ((tempname[j] >= 65) && (tempname[j] <= 90))
								{
									tempname[j] = tempname[j] + 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}

								if (tempname[j - 1] == 32 && (tempname[j] >= 65 && tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}
								else if (tempname[j - 1] == 32 && (tempname[j] >= 97 && tempname[j] <= 122)) {
									tempname[j] = tempname[j]-32;
								}

							}
							j++;
						}

						else {
							tempname[j] = tempname[j];
							cont = cont + 1;
							j++;
						}

					}
					if (cont > 0) {
						MessageBoxA(NULL, "El nombre solo debe tener letras", "ERROR", MB_ICONERROR);
						OK1 = false;
					}
					else {
						OK1 = true;
					}
				}

#pragma endregion

				//ok2

#pragma region SERVICE
				int indexS = SendMessageA(GetDlgItem(hMecs, CB_SERVM), CB_GETCURSEL, 0, 0);
				if (indexS == -1) {
					if (IsWindowEnabled(GetDlgItem(hDlg, BTN_EDITM)) == 0) {
						MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
						break;
					}
					else {
						int lengthS = GetWindowTextLength(GetDlgItem(hMecs, CB_SERVM));
						GetWindowTextA(GetDlgItem(hMecs, CB_SERVM), tempserv, lengthS + 1);
						OK2 = true;
					}
				}
				else {
					/*Obtenemos el texto del combo y lo mostramos*/
					int lengthS = GetWindowTextLength(GetDlgItem(hMecs, CB_SERVM));
					GetWindowTextA(GetDlgItem(hMecs, CB_SERVM), tempserv, lengthS + 1);
					OK2 = true;
				}

#pragma endregion

				// ok3 & ok4 

#pragma region Schedule 

				int indexW = SendMessageA(GetDlgItem(hMecs, CB_SCHEDW), CB_GETCURSEL, 0, 0);
				if (indexW == -1) {
				
						MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
						OK3 = false;
						break;
				
				}
				else {
					/*Obtenemos el texto del combo y lo mostramos*/
					int lengthW = GetWindowTextLength(GetDlgItem(hMecs, CB_SCHEDW));
					GetWindowTextA(GetDlgItem(hMecs, CB_SCHEDW), tempwsched, lengthW + 1);
					OK3 = true;

					int indexH = SendMessageA(GetDlgItem(hMecs, CB_SCHEDH), CB_GETCURSEL, 0, 0);
					if (indexH == -1) {
						MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
						OK4 = false;
						break;
					}
					else {
						/*Obtenemos el texto del combo y lo mostramos*/
						int lengthH = GetWindowTextLength(GetDlgItem(hMecs, CB_SCHEDH));
						GetWindowTextA(GetDlgItem(hMecs, CB_SCHEDH), temphsched, lengthH + 1);

					}
					if (strcmp(tempwsched, "LUNES A VIERNES") == 0) {
						if (strcmp(temphsched, "8AM-12PM") == 0 || strcmp(temphsched, "12PM-4PM") == 0 || strcmp(temphsched, "4PM-8PM") == 0) {
							OK4 = true;
						}
						else {
							OK4 = false;
						}
					}
					
				}


#pragma endregion
				//ok5

#pragma region Phone
				SendMessageA(GetDlgItem(hMecs, TB_TELM), WM_GETTEXT, sizeof(temphone), (LPARAM)temphone);
				int cont3 = 0, f = 0;
				int car = 0;
				if (temphone[f] == '\0') {
					MessageBoxA(hDlg, "Llene la informacion", "Error", MB_ICONASTERISK);
				}
				else {
					while (temphone[f] != '\0') {
						if (isdigit(temphone[f])) {
							cont3 = cont3 + 1;
						}
						else {
							car = car + 1;
						}
						f++;
					}
				}
				if (car > 0) {
					MessageBoxA(hDlg, "El numero no debe contenerletras", "Error", MB_ICONERROR);
				}
				else if (cont3 > 0) {
					if (cont3 == 8) {
						OK5 = true;
					}
					else if (cont3 == 10) {
						OK5 = true;
					}
					else if (cont3 == 12) {
						OK5 = true;
					}
					else {
						MessageBoxA(hDlg, "El numero debe tener 8, 10 o 12 digitos", "Error", MB_ICONERROR);
						OK5 = false;
					}
				}
#pragma endregion

				//ok6
#pragma region Picture
				SendMessageA(GetDlgItem(hMecs, LBL_PIC), WM_GETTEXT, sizeof(temppic), (LPARAM)temppic);
				if (!(strcmp(temppic, "") == 0)) {
					OK6 = true;
				}
				else {
					MessageBoxA(hMecs, "No selecciono foto", "error", MB_ICONASTERISK);
					OK6 = false;
				}
#pragma endregion

				//si no fue edicion
				if (IsWindowEnabled(GetDlgItem(hMecs, BTN_EDITM)) == 0) {
					if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6) {
						if (!Mechanics->lookDouble(tempname)) {
							
							//SE EMPALMAN LOS MECANICOS, revisar
							
							//ya hallo el servicio
							Servs::Node* itemS = Services.findItem(tempserv);
							int option = 0;
							if (strcmp(temphsched, "8AM-12PM")==0) {
								option = 0;
							}
							else if (strcmp(temphsched, "12PM-4PM")==0) {
								option = 1;
							}
							else if (strcmp(temphsched, "4PM-8PM")==0) {
								option = 2;
							}
							
							//ya sabe cual es el horario
							//si devuelve verdadero, no hay nada ahi, de lo contrario ya esta ocupado
							if (Services.freeSpace(itemS, option)) {
								Mechs::Nodo* nuevo = Mechanics->creItem(tempname, temppic, temphone, tempserv, tempwsched, temphsched);
								Mechanics->inItem(nuevo);
								bool done = Services.addMechs(nuevo, itemS, option);
								if (done) {
									MessageBoxA(hMecs, "MECANICO GUARDADO CON EXITO", "ALELUYA", MB_OK);
								}

								char limpia[20] = "";
								SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)"");

								SendMessage(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
								SendMessage(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
								SendMessage(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);

								SendMessage(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, sizeof(limpia), (LPARAM)"");
								SendMessage(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), NULL);


								SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_ADDSTRING, 0, (LPARAM)Mechanics->tail->name);
								SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL, -1, NULL);
							}
							else {
								MessageBoxA(hServs, "Error", "Ya existe un mecánico en ese horario", MB_ICONERROR);
							}
						}
						else {
							MessageBoxA(hServs, "Error", "Ya existe ese mecánico", MB_ICONERROR);
						}
					}
					else {
						MessageBoxA(hMecs, "Algo hiciste mal", "error", MB_ICONERROR);
					}
				}
				//si si fue edicion
				else {
					if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6) {
						
						int week = 0;
						int hour = 0;
						int hourM = 0;
						if (strcmp(tempwsched, "LUNES A VIERNES") == 0) {
							week = 1;
						}
				
						if (strcmp(temphsched, "8AM-12PM") == 0) {
							hour = 0;
							hourM = 1;
						}
						else if (strcmp(temphsched, "12PM-4PM") == 0) {
							hour = 1;
							hourM = 2;
						}
						else if (strcmp(temphsched, "4PM-8PM") == 0) {
							hour = 2;
							hourM = 3;
						}
				
						//buscar lo que hay en la lista, no en el textbox
						char testring[50] = "";

						int selIndex = (int)SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETCURSEL, 0, 0);
						SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testring);
						Mechs::Nodo* foundM = Mechanics->findItem(testring);
						if (foundM != NULL) {
							serFound = Services.findItem(foundM->occupation);
							ser2Found = Services.findItem(tempserv);
							bool regist = false;
							//el servicio es el mismo
							if (strcmp(foundM->occupation, tempserv) == 0) {
								//la semana SIEMPRE es la misma
									//las horas son las mismas
									if (foundM->empSched.hourSche == hourM) {
										//no se cambia nada en servicios uwu
									}
									else {
										//horas diferentes
										Services.deleteMechs(foundM, serFound, found->empSched.hourSche);
										regist = Services.addMechs(foundM, ser2Found, hour);
									}
							}
							//el servicio es diferente
							else {
								Services.deleteMechs(foundM, serFound, foundM->empSched.hourSche);
								regist = Services.addMechs(foundM, ser2Found, hour);
							}
							if (strcmp(foundM->name,tempname) != 0) {
								//borramos el anterior nombre
								SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_DELETESTRING, (WPARAM)selIndex,0 );
								Mechanics->editItem(foundM, tempname, temppic, temphone, tempserv, tempwsched, temphsched);
								SendMessageA(GetDlgItem(hMecs, LB_MECS), LB_ADDSTRING,0,(LPARAM)foundM->name);
							}
							else {
								Mechanics->editItem(foundM, tempname, temppic, temphone, tempserv, tempwsched, temphsched);
							}
						}
						char limpia[20] = "";
						SendMessageA(GetDlgItem(hDlg, TB_NAMEM), WM_SETTEXT, 0, (LPARAM)"");
						SendMessageA(GetDlgItem(hDlg, TB_TELM), WM_SETTEXT, 0, (LPARAM)"");
						SendMessageA(GetDlgItem(hDlg, LBL_NUMEMP), WM_SETTEXT, 0, (LPARAM)"");

						SendMessageA(GetDlgItem(hDlg, CB_SERVM), CB_SETCURSEL, -1, NULL);
						SendMessageA(GetDlgItem(hDlg, CB_SCHEDW), CB_SETCURSEL, -1, NULL);
						SendMessageA(GetDlgItem(hDlg, CB_SCHEDH), CB_SETCURSEL, -1, NULL);

						SendMessageA(GetDlgItem(hDlg, LBL_PIC), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
						SendMessageA(GetDlgItem(hDlg, PC_MECH), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
						loadPic(GetDlgItem(hDlg, PC_MECH),limpia,50,50);


						SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL, -1, NULL);

						EnableWindow(GetDlgItem(hDlg, BTN_DELETEM), false);
						EnableWindow(GetDlgItem(hDlg, BTN_EDITM), false);

					}
					else {
						MessageBoxA(NULL, "NO SE NI QUE SALIO MAL, PERO ESTA MAL", "ERROR", MB_ICONERROR);
					}
				}

			}
		}
		else if (LOWORD(wParam) == IDCANCEL && HIWORD(wParam) == BN_CLICKED) {
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hMecs, 0);
		
}
		else if (IsDlgButtonChecked(hDlg, RB_REPM) == BST_CHECKED) {
		int size = Mechanics->sizeOfTree(Mechanics->tree);
		if (size > 0) {
			int* arrL = new int[size];
			//LLENAR EL ARREGLO MUY IMPORTANTE JAJAJ
			int k = 0;
			Mechanics->AddToArray(Mechanics->tree, arrL, k);
			//el arreglo tiene enteros como numeros de nomina
			Mechanics->Quicksort(arrL, 0, size - 1);
			char mechname[50];
			int i = 0;
			//en el listbox todo se pone por nombre del mecanico
			SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_RESETCONTENT, 0, 0);
			for (i = 0; i <= size; i++) {
				Mechs::Nodo* stored=Mechanics->findItemI(arrL[i]);
				if (stored != NULL) {
					SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_ADDSTRING, 0, (LPARAM)stored->name);
				}
			}
			//generar el reporte en un archivo de texto
			HWND listBoxMechs = GetDlgItem(hDlg, LB_MECS);
			char nombreDeArchivo[] = "Reporte_de_mecanicos.txt";
			txtReport(nombreDeArchivo, listBoxMechs, hDlg, 3);
			SendMessageA(GetDlgItem(hDlg, LB_MECS), LB_SETCURSEL,-1, 0);
			delete[]arrL;
			SendMessageA(GetDlgItem(hDlg, RB_REPM), BM_SETCHECK, BST_UNCHECKED, 0);

		}

		
		}
		return 0;
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hMecs, 0);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hCitsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		SendMessageA(GetDlgItem(hDlg, LBL_USERCT), WM_SETTEXT, 0, (LPARAM)Usuario.cUser);
		//sin servicios no registras ni mecanicos
		if (Services.lista == NULL) {
			MessageBoxA(NULL, "No hay servicios. Registre uno primero", "Error", MB_ICONEXCLAMATION);
		}
		else {
			Services.aux = Services.lista;
			while (Services.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
				Services.aux = Services.aux->next;
			}
			Services.aux = Services.origen;
		}
		//cargar las placas de los clientes
		if (Clients.lista == NULL) {
			MessageBoxA(NULL, "No hay clientes. Registre uno primero", "Error", MB_ICONEXCLAMATION);
		}
		else {
			Clients.aux = Clients.lista;
			while (Clients.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_ADDSTRING, 0, (LPARAM)Clients.aux->placas);
				Clients.aux = Clients.aux->next;
			}
			Clients.aux = Clients.origen;
		}
#pragma region Deactivate buttons
		EnableWindow(GetDlgItem(hDlg, IDEDIT), false);
		EnableWindow(GetDlgItem(hDlg, IDDELETE), false);

#pragma endregion

#pragma region Hide Search stuff
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO4), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO5), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO6), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO7), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, TB_FOLIOB), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_SERVB), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_MECHB), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, DTP_BUS), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, LBL_INST), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT2), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICBIN), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, BTN_BINSEARCH), SW_HIDE);
#pragma endregion
		//LA PANTALLA DE BUSQUEDA
	/*	if (Mechanics->lista != NULL) {
			Mechanics->aux = Mechanics->origen;
			while (Mechanics->aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, CB_MECHB), CB_ADDSTRING, 0, (LPARAM)Mechanics->aux->name);
				Mechanics->aux = Mechanics->aux->next;
			}
			Mechanics->aux = Mechanics->origen;
		}
		else {
			MessageBoxA(hDlg, "No hay mecanicos", "Error", MB_ICONASTERISK);
		}*/

		if (Services.lista != NULL) {
			Services.aux = Services.origen;
			while (Services.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, CB_SERVB), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
				Services.aux = Services.aux->next;
			}
		}
		else {
			MessageBoxA(hDlg, "No hay servicios", "Error", MB_ICONASTERISK);
		}

		//hide cancel stuff
		ShowWindow(GetDlgItem(hDlg, LBL_CANCELDESC), false);
		ShowWindow(GetDlgItem(hDlg, TB_CANCELDESC), false);

		if (Citas.lista != NULL) {
			Citas.aux = Citas.origen;
			while (Citas.aux != NULL) {
				string folio = to_string(Citas.aux->ID);
				SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)folio.c_str());
				Citas.aux = Citas.aux->next;
			}
			Citas.aux= Citas.origen;
		}//else no hay citas

	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == LB_CITAS && HIWORD(wParam) == LBN_SELCHANGE){
			char testCit[20];

			int selIndex = (int)SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETCURSEL, 0, 0);
			if (selIndex == -1) {

			}
			else {
#pragma region Deactivate buttons
				//DEACTIVATE OPTIONS
				EnableWindow(GetDlgItem(hDlg, CB_CLIENT), false);
				EnableWindow(GetDlgItem(hDlg, DTP_CITS), false);
				EnableWindow(GetDlgItem(hDlg, CB_SERVCT), false);
				EnableWindow(GetDlgItem(hDlg, CB_MECH), false);
				EnableWindow(GetDlgItem(hDlg, CB_HOUR), false);
				EnableWindow(GetDlgItem(hDlg, CH_ATT), false);
				EnableWindow(GetDlgItem(hDlg, CH_CANC), false);
				EnableWindow(GetDlgItem(hDlg, TB_TEL), false);
				EnableWindow(GetDlgItem(hDlg, TB_NAME), false);
				//buttons activate
				EnableWindow(GetDlgItem(hDlg, IDEDIT), true);
				EnableWindow(GetDlgItem(hDlg, IDDELETE), true);

#pragma endregion
				/*int txtl = GetWindowTextLength(GetDlgItem(hDlg, LB_CITAS));
				GetWindowTextA(GetDlgItem(hDlg, LB_CITAS), testCit,txtl+1 );*/
				SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testCit);
				Cits::Node *look = Citas.findItem(testCit);
				if (look != NULL) {
#pragma region fillCombos
					//SERVICIOS
					SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_RESETCONTENT, 0, 0);
					Services.aux = Services.lista;
					while (Services.aux != NULL) {
						SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
						Services.aux = Services.aux->next;
					}Services.aux = Services.lista;
					//PLACAS CLIENTES
					SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_RESETCONTENT, 0, 0);
					if (Clients.lista == NULL) {
						MessageBoxA(NULL, "No hay clientes. Registre uno primero", "Error", MB_ICONEXCLAMATION);
					}
					else {
						Clients.aux = Clients.lista;
						while (Clients.aux != NULL) {
							SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_ADDSTRING, 0, (LPARAM)Clients.aux->placas);
							Clients.aux = Clients.aux->next;
						}
						Clients.aux = Clients.origen;
					}
					


#pragma endregion

					char limpia[20] = "";
					
					SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)look->clientTel);
					SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)look->clientName);
					string folio = "";
					folio = to_string(look->ID);
					SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)folio.c_str());
					
					
					SetWindowTextA(GetDlgItem(hDlg, CB_SERVCT), look->specialC);
					int selIndex = 0;
			
					int items = (int)SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_GETCOUNT, 0, 0);

					while (selIndex < items) {
						char testringQ[30];
						SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_SETCURSEL, selIndex, 0);
						int txtQ = (int)GetWindowTextLengthA(GetDlgItem(hDlg, CB_SERVCT));
						GetWindowTextA(GetDlgItem(hDlg, CB_SERVCT),testringQ,txtQ+1);
						if (strcmp(testringQ, look->specialC) == 0) {
							break;
						}
						
						selIndex++;
					}
					//YA SE LLENO SERVICIOS, LLENAMOS EL COMBO DE MECANICOS
					char testringR[30];
					int txtR = (int)GetWindowTextLengthA(GetDlgItem(hDlg, CB_SERVCT));
					GetWindowTextA(GetDlgItem(hDlg, CB_SERVCT), testringR, txtR + 1);
					Mechanics->aux = Mechanics->lista;
						while (Mechanics->aux != NULL) {
							if (strcmp(Mechanics->aux->occupation,testringR)==0) {
								SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_ADDSTRING, 0, (LPARAM)Mechanics->aux->name);
							}
							Mechanics->aux = Mechanics->aux->next;
						}

						char testrMForHour[50];
						int selIndex4 = 0;

						int items4 = (int)SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_GETCOUNT, 0, 0);

						while (selIndex4 < items4) {
							char testringQ[50];
							SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_SETCURSEL, selIndex4, 0);
							int txtQ = (int)GetWindowTextLengthA(GetDlgItem(hDlg, CB_MECH));
							GetWindowTextA(GetDlgItem(hDlg, CB_MECH), testringQ, txtQ + 1);
							GetWindowTextA(GetDlgItem(hDlg, CB_MECH), testrMForHour, txtQ + 1);
							if (strcmp(testringQ, look->mechName) == 0) {
								break;
							}

							selIndex4++;
						}
						//fill hour
						Mechs::Nodo* hor = Mechanics->findItem(testrMForHour);
						if (hor != NULL) {
							string horario;
							if (hor->empSched.hourSche == 1) {
								horario = "8AM-12PM";
							}
							else if (hor->empSched.hourSche == 2) {
								horario = "12PM-4PM";
							}
							else if (hor->empSched.hourSche == 3) {
								horario = "4PM-8PM";
							}
							SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_ADDSTRING, 0, (LPARAM)horario.c_str());
							SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_SETCURSEL, 0, 0);
						}

					int selIndex2 = 0;

					int items2 = (int)SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_GETCOUNT, 0, 0);

					while (selIndex2 < items2) {
						char testringQ[20];
						SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_SETCURSEL, selIndex2, 0);
						int txtQ = (int)GetWindowTextLengthA(GetDlgItem(hDlg, CB_CLIENT));
						GetWindowTextA(GetDlgItem(hDlg, CB_CLIENT), testringQ, txtQ + 1);
						if (strcmp(testringQ, look->placas) == 0) {
							break;
						}

						selIndex2++;
					}

					
					SetWindowTextA(GetDlgItem(hCits, CB_MECH), look->mechName);
					SetWindowTextW(GetDlgItem(hDlg, CB_CLIENT), (LPCWSTR)look->placas);
					SendMessageA(GetDlgItem(hDlg, LBL_FECHA), WM_SETTEXT, 0, (LPARAM)look->shortDate);
					SetWindowTextA(GetDlgItem(hCits, CB_HOUR), look->shortHour);
					if (look->canceled) {
						SendMessageA(GetDlgItem(hDlg, CH_CANC), BM_SETCHECK, BST_CHECKED, 0);
						SendMessageA(GetDlgItem(hDlg, TB_CANCELDESC), WM_SETTEXT, 0, (LPARAM)look->cancelMotive);
						EnableWindow(GetDlgItem(hDlg, TB_CANCELDESC), false);
					}
					else if (look->attend) {
						SendMessageA(GetDlgItem(hDlg, CH_ATT), BM_SETCHECK, BST_CHECKED, 0);
					}
					
					//CAMBIAR FOLIO
					SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)look->ID);
					//CARGAR LA FOTO
					found = Mechanics->findItem(look->mechName);
					if (found != NULL) {
						SendMessageA(GetDlgItem(hDlg, PC_CITAS), WM_SETTEXT, sizeof(limpia), (LPARAM)limpia);
						loadPic(GetDlgItem(hDlg, PC_CITAS), found->pic, 50, 100);
						EnableWindow(GetDlgItem(hDlg, IDOK), false);
					}

				}

			}
		}
		//CUANDO CAMBIE EL SERVICIO CAMBIAN MECÁNICOS
		else if (LOWORD(wParam) == CB_SERVCT && HIWORD(wParam) == CBN_SELCHANGE) {
			SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_RESETCONTENT, 0, 0);
			char testServ[50];

			int selBIndex = (int)SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_GETCURSEL, 0, 0);
			if (selBIndex == -1) {

			}
			else {
				//SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_GETLBTEXT, selBIndex, (LPARAM)testServ);
				int txtL = GetWindowTextLength(GetDlgItem(hDlg, CB_SERVCT));
				GetWindowTextA(GetDlgItem(hDlg, CB_SERVCT), testServ, txtL + 1);
				/*Servs::Node* var = Services.findItem(testServ);*/
				Mechs::Nodo* var2 = Mechanics->lista;
				if (var2 != NULL) {
					while (var2 != NULL) {
						if (strcmp(var2->occupation, testServ) == 0) {
							SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_ADDSTRING, 0, (LPARAM)var2->name);
						}
						var2 = var2->next;
					}
				}else {
					MessageBoxA(NULL, "No hay Mecanicos de este servicio", "Error", MB_ICONEXCLAMATION);
				}
			}
		}
		//CUANDO CAMBIEN MECANICOS CAMBIA HORARIO y se pone la foto
		else if (LOWORD(wParam) == CB_MECH && HIWORD(wParam) == CBN_SELCHANGE) {
			SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_RESETCONTENT, 0, 0);
			char testMech[50];

			int selBIndex = (int)SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_GETCURSEL, 0, 0);
			if (selBIndex == -1) {

			}
			else {
				
				int txtL = GetWindowTextLength(GetDlgItem(hDlg, CB_MECH));
				GetWindowTextA(GetDlgItem(hDlg, CB_MECH), testMech, txtL + 1);
				Mechs::Nodo* var = Mechanics->findItem(testMech);
				if (var != NULL) {
					char texto[50];
					//con esto se llenan los horarios si hay
					if (var->empSched.hourSche == 1) {
						strcpy(texto, "8AM-12PM");
					}
					else if (var->empSched.hourSche == 2) {
						strcpy(texto, "12PM-4PM");
					}
					else if (var->empSched.hourSche == 3) {
						strcpy(texto, "4PM-8PM");
					}
					SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_ADDSTRING, 0, (LPARAM)texto);

					//cargar foto
					loadPic(GetDlgItem(hDlg, PC_CITAS), var->pic, 50, 100);

				}
			}
		}
		//CUANDO CAMBIAN LAS PLACAS DEL CLIENTE CAMBIA EL TELEFONO Y EL NOMBRE
		else if(LOWORD(wParam) == CB_CLIENT && HIWORD(wParam) == CBN_SELCHANGE) {
			char testClient[50];

			int selBIndex = (int)SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_GETCURSEL, 0, 0);
			if (selBIndex == -1) {

			}
			else {
				//SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_GETLBTEXT, (WPARAM)selBIndex, (LPARAM)testClient);
				int txtL = GetWindowTextLength(GetDlgItem(hDlg, CB_CLIENT));
				GetWindowTextA(GetDlgItem(hDlg, CB_CLIENT), testClient, txtL + 1);
				//testclient son placas
				Clts::Node* var = Clients.findItem(testClient,testClient,2);
				if (var != NULL) {
					string fullname = var->nombre;
					fullname.append(" ");
					fullname.append(var->apellido);
					SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)fullname.c_str());
					SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)var->telefono);
				}
			}
		}
		else if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
		if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED) {
			//BUSQUEDA POR FOLIO
			if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED) {
				
				char testFolio[12];
				SendMessageA(GetDlgItem(hDlg, TB_FOLIOB), WM_GETTEXT, sizeof(testFolio), (LPARAM)testFolio);
				Cits::Node* foundC = Citas.findItem(testFolio);
				if (foundC != NULL) {
					//primero, resetear el listbox y mandarlo ahi
					SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_RESETCONTENT, 0, 0);
					SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)testFolio);


					//QUE MIS RADIOS ESTEN EN SU LUGAR
					SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED,0);//RADIO DE REGISTRAR/EDITAR
					SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED,0);//RADIO DE BUSCAR
					
					
					//CARGAR TODOS LOS DATOS
#pragma region fillCombos
//SERVICIOS
					SendMessage(GetDlgItem(hDlg, CB_SERVCT), CB_RESETCONTENT, 0, 0);
					Services.aux = Services.lista;
					while (Services.aux != NULL) {
						SendMessage(GetDlgItem(hDlg, CB_SERVCT), CB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
						Services.aux = Services.aux->next;
					}Services.aux = Services.lista;
					//PLACAS CLIENTES
					SendMessage(GetDlgItem(hDlg, CB_CLIENT), CB_RESETCONTENT, 0, 0);
					if (Clients.lista == NULL) {
						MessageBoxA(NULL, "No hay clientes. Registre uno primero", "Error", MB_ICONEXCLAMATION);
					}
					else {
						Clients.aux = Clients.lista;
						while (Clients.aux != NULL) {
							SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_ADDSTRING, 0, (LPARAM)Clients.aux->placas);
							Clients.aux = Clients.aux->next;
						}
						Clients.aux = Clients.origen;
					}


#pragma endregion

					SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, 0);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICLBL), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICS), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICM), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICP), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICC), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICT), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICF), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICH), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, LBL_FOLIO), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, LBL_FECHA), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, IDC_STATICPM), SW_SHOW);
					//AHORA LOS BOTONES
					ShowWindow(GetDlgItem(hDlg, PC_CITAS), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, CB_SERVCT), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, CB_MECH), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, CB_CLIENT), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, CB_HOUR), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, TB_NAME), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, TB_TEL), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, DTP_CITS), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, CH_ATT), SW_SHOW);
					ShowWindow(GetDlgItem(hDlg, CH_CANC), SW_SHOW);
					//POR SI LAS DUDAS, INHABILITAR CANCELAR Y EDITAR
					EnableWindow(GetDlgItem(hDlg, IDEDIT), true);
					EnableWindow(GetDlgItem(hDlg, IDDELETE), true);
					//MOSTRAR TODO LO DE LA VENTANA BUSCAR
					ShowWindow(GetDlgItem(hDlg, RB_BFO), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, RB_BSER), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, RB_MECH), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, RB_MON), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, RB_WEEK), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, TB_FOLIOB), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, CB_SERVB), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, CB_MECHB), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, DTP_BUS), SW_HIDE);
					ShowWindow(GetDlgItem(hDlg, LBL_INST), SW_HIDE);

				}
				else {
					MessageBoxA(hDlg, "Error", "El folio introducido no existe", MB_ICONASTERISK);
				}
			}
			//BUSQUEDA POR SERVICIO Y SEMANA
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO6) == BST_CHECKED) {
				char testCit[50];
				

				int selIndex = (int)SendMessageA(GetDlgItem(hDlg, CB_SERVB), CB_GETCURSEL, 0, 0);
				if (selIndex == -1) {

				}
				else {
					int lengthC = GetWindowTextLength(GetDlgItem(hDlg, CB_SERVB));
					GetWindowTextA(GetDlgItem(hDlg, CB_SERVB), testCit, lengthC + 1);
					if (Citas.origen != NULL) {
						char tempdate[50];
						char tempshortdate[12];

						SendMessageA(GetDlgItem(hDlg, DTP_BUS), WM_GETTEXT, sizeof(tempdate), (LPARAM)tempdate);
						if (tempdate[0] != '\0') {
							if (Citas.origen != NULL) {
								string date(tempdate);
								int n = 0;
								n = date.find(",");
								string diaSem = date.substr(0, n);
								int contN = 0;
								int contP = 0;
								int dayInit = 0, dayEnd = 0;
								string shortdate = shortDateChanger(tempdate);
								strcpy(tempshortdate, shortdate.c_str());
								//esto es pa la semana
								if (strcmp(diaSem.c_str(), "lunes") == 0) {
									contP = 0;
									contN = 4;
								}
								else if (strcmp(diaSem.c_str(), "martes") == 0) {
									contP = 1;
									contN = 3;
								}
								else if (strcmp(diaSem.c_str(), "miércoles") == 0) {
									contP = 2;
									contN = 2;
								}
								else if (strcmp(diaSem.c_str(), "jueves") == 0) {
									contP = 3;
									contN = 1;
								}
								else if (strcmp(diaSem.c_str(), "viernes") == 0) {
									contP = 4;
									contN = 0;
								}
								else if (strcmp(diaSem.c_str(), "sábado") == 0 || strcmp(diaSem.c_str(), "domingo") == 0) {
									//Error, seleccione un di entre semana
									MessageBoxA(hDlg, "Error", "seleccione dia entre semana", MB_ICONERROR);
								}
								if (strcmp(diaSem.c_str(), "sábado") != 0 && strcmp(diaSem.c_str(), "domingo") != 0) {
									//pal dia, mes y año usamos esto

									int n = 0;
									n = shortdate.find("/");
									string dia = shortdate.substr(0, n);
									shortdate = shortdate.substr(n + 1, shortdate.length());
									int dayI = atoi(dia.c_str());
									n = shortdate.find("/");
									string mes = shortdate.substr(0, n);
									shortdate = shortdate.substr(n + 1, shortdate.length());
									int mesI = atoi(mes.c_str());
									string year = shortdate;
									int yearI = atoi(year.c_str());
									//buscas de un solo lado
									if (contN == 0 || contP == 0) {
										if (contN == 0) {
											//solo buscas para atras.
											if (dayI - contP < 1) {
												dayInit = dayEnd = 1;
											}
											else {
												dayInit = dayI - contP;
												dayEnd = dayI;
											}
										}
										else if (contP == 0) {
											//solo buscas adelante.
											if (mesI == 1 || mesI == 3 || mesI == 5 || mesI == 7 ||
												mesI == 8 || mesI == 10 || mesI == 12) {
												if (dayI + contN > 31) {
													dayEnd = 31;
													dayInit = dayI;
												}
												else {
													dayEnd = dayI + contN;
													dayInit = dayI;
												}

											}
											else if (mesI == 4 || mesI == 6 || mesI == 9 || mesI == 11) {
												if (dayI + contN > 30) {
													dayEnd = 30;
													dayInit = dayI;
												}
												else {
													dayEnd = dayI + contN;
													dayInit = dayI;
												}
											}
											else if (mesI == 2) {

												if ((yearI % 2) == 0) {
													//año bisiesto, feb tiene 29 dias
													if (dayI + contN > 29) {
														dayEnd = 29;
														dayInit = dayI;
													}
													else {
														dayEnd = dayI + contN;
														dayInit = dayI;
													}
												}
												else {
													//feb tiene 28 dias
													if (dayI + contN > 28) {
														dayEnd = 28;
														dayInit = dayI;
													}
													else {
														dayEnd = dayI + contN;
														dayInit = dayI;
													}
												}
											}

										}

									}
									//buscas adelante y atras
									else {
										if (mesI == 1 || mesI == 3 || mesI == 5 || mesI == 7 ||
											mesI == 8 || mesI == 10 || mesI == 12) {
											if (dayI - contP < 1) {
												dayInit = 1;
											}
											else {
												dayInit = dayI - contP;
											}
											if (dayI + contN > 31) {
												dayEnd = 31;
											}
											else {
												dayEnd = dayI + contN;
											}
										}
										else if (mesI == 4 || mesI == 6 || mesI == 9 || mesI == 11) {
											if (dayI - contP < 1) {
												dayInit = 1;
											}
											else {
												dayInit = dayI - contP;
											}
											if (dayI + contN > 30) {
												dayEnd = 30;
											}
											else {
												dayEnd = dayI + contN;
											}

										}
										else if (mesI == 2) {

											if ((yearI % 2) == 0) {
												//año bisiesto, feb tiene 29 dias
												if (dayI - contP < 1) {
													dayInit = 1;
												}
												else {
													dayInit = dayI - contP;
												}
												if (dayI + contN > 29) {
													dayEnd = 29;
												}
												else {
													dayEnd = dayI + contN;
												}

											}
											else {
												//feb tiene 28 dias
												if (dayI - contP < 1) {
													dayInit = 1;
												}
												else {
													dayInit = dayI - contP;
												}
												if (dayI + contN > 28) {
													dayEnd = 28;
												}
												else {
													dayEnd = dayI + contN;
												}
											}
										}

									}
									//genera una lista de fechas
									int size = 0;
									if (dayEnd == dayInit) {
										//busca una sola fecha
										string dy = to_string(dayInit);
										string fecha = dy + "/" + mes + "/" + year;
										Cits::Node* aux = Citas.origen;
										SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_RESETCONTENT, 0, 0);
										int cont = 0;
										if (aux != NULL) {
											while (aux != NULL) {
												if (strcmp(fecha.c_str(), aux->shortDate) == 0 && strcmp(aux->specialC,testCit)==0&& aux->attend == false && aux->canceled == false) {
													string myFolio = to_string(aux->ID);
													SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)myFolio.c_str());
													cont++;
												}
												aux = aux->next;
											}
											//CARGAR TODOS LOS DATOS
											if (cont > 0) {

												char nameArchive[50] = "Reporte_citas_servicios_semanal.txt";
												HWND LISTBOX = GetDlgItem(hDlg, LB_CITAS);
												txtReport(nameArchive, LISTBOX, hDlg, 1);
												SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, 0);
												SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0);//RADIO DE REGISTRAR/EDITAR
												SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED, 0);//RADIO DE BUSCAR
											}
											else {
												MessageBoxA(hDlg, "Busqueda finalizada", "No se encontraron elementos coincidentes", MB_OK);
											}

										}
										else {
											//no hay citas
											MessageBoxA(hDlg, "ERROR", "No hay citas", MB_ICONERROR);
										}
									}
									//busca todas las fechas que tenga que buscar
									else {
										size = dayEnd - dayInit;
										string* arr = new string[size]();
										for (int i = 0; i < size; i++) {
											if (dayInit < 10) {
												arr[i] = "0";
												string dy = to_string(dayInit);
												arr[i].append(dy);

											}
											else {
												string dy = to_string(dayInit);
												arr[i] = dy;
											}
											arr[i].append("/");
											arr[i].append(mes);
											arr[i].append("/");
											arr[i].append(year);

											dayInit++;
										}
										//empieza la busqueda de verdad
										Cits::Node* aux = Citas.origen;
										SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_RESETCONTENT, 0, 0);
										if (aux != NULL) {
											int cont = 0;
											while (aux != NULL) {
												for (int i = 0; i < size; i++) {
													if (strcmp(arr[i].c_str(), aux->shortDate) == 0 &&strcmp(aux->specialC, testCit) == 0 && aux->attend == false && aux->canceled == false) {
														string myFolio = to_string(aux->ID);
														SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)myFolio.c_str());
														cont++;
													}
												}
												aux = aux->next;
											}
											//CARGAR TODOS LOS DATOS
											if (cont > 0) {
												MessageBoxA(hDlg, "Busqueda exitosa", "Haga clic en la lista. Elementos coincidentes:" + cont, MB_OK);
												char nameArchive[50] = "Reporte_citas_servicios_semanal.txt";
												HWND LISTBOX = GetDlgItem(hDlg, LB_CITAS);
												txtReport(nameArchive, LISTBOX, hDlg, 1);
												SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, 0);
												SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0);//RADIO DE REGISTRAR/EDITAR
												SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED, 0);//RADIO DE BUSCAR
											}
											else {
												MessageBoxA(hDlg, "Busqueda finalizada", "No se encontraron elementos coincidentes", MB_OK);
											}
										}
										else {
											//no hay citas
											MessageBoxA(hDlg, "ERROR", "No hay citas", MB_ICONERROR);
										}
									}
								}

							}
							else {
								//no hay citas
								MessageBoxA(hDlg, "ERROR", "No hay citas", MB_ICONERROR);
							}

						}
						else {
							MessageBoxA(NULL, "Seleccione una fecha", "Error", MB_OK);
						}

					}
					else {
						//no hay citas
						MessageBoxA(hDlg, "error", "No hay citas registradas", MB_ICONERROR);
					}

				}
				SendMessageA(GetDlgItem(hDlg, RB_BSER), BM_SETCHECK, BST_UNCHECKED, 0);
			}
			//BUSQUEDA POR MECANICO Y MES
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO5) == BST_CHECKED) {
				char testCit[50];

				int selIndex = (int)SendMessageA(GetDlgItem(hDlg, CB_MECHB), CB_GETCURSEL, 0, 0);
				if (selIndex == -1) {

				}
				else {
					int lengthC = GetWindowTextLength(GetDlgItem(hDlg, CB_MECHB));
					GetWindowTextA(GetDlgItem(hDlg, CB_MECHB), testCit, lengthC + 1);
					if (Citas.origen != NULL) {
						//BUSCAR POR MECANICO
						char tempmonth[50];
						int month, year, cont;
						cont = 0;
						SendMessageA(GetDlgItem(hDlg, DTP_BUS), WM_GETTEXT, sizeof(tempmonth), (LPARAM)tempmonth);
						if (tempmonth[0] != NULL) {
							string shDate = shortDateChanger(tempmonth);
							string mes = shDate.substr(3, 5);
							string anio = shDate.substr(6, shDate.length());
							month = atoi(mes.c_str());
							year = atoi(anio.c_str());
						}
						Cits::Node* binServ = Citas.binarySearchM(Citas.origen, testCit);
						Cits::Node* busqueda = Citas.origen;
						while (busqueda != NULL) {
							if (strcmp(busqueda->mechName, testCit) == 0 && busqueda->date.año == year && busqueda->date.mes == month
								&&busqueda->attend==false&&busqueda->canceled==false) {
								string folio = to_string(busqueda->ID);
								SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)folio.c_str());
								cont++;
							}
							busqueda = busqueda->next;
						}
						if (cont > 0) {
							MessageBoxA(hDlg, "enhorabuena", "elementos encontrados", MB_OK);

							char nameArchive[50] = "Reporte_citas_mecanico.txt";
							//int selIndexA = 0;
							//int itemsA = (int)GetListBoxInfo(GetDlgItem(hDlg, LB_CITAS));
							//if (itemsA == 1) {
							//	string arrCits;
							//	SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETTEXT, selIndexA, (LPARAM)arrCits.c_str());
							//	//aqui paso la funcion de generar archivo
							//}
							//else {
							//	string* arrCitsL = new string[itemsA];
							//	for (selIndexA = 0; selIndexA < itemsA; selIndexA++) {
							//		string myFolio;
							//		SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETTEXT, selIndexA, (LPARAM)myFolio.c_str());
							//		arrCitsL[selIndexA] = myFolio;
							//	}
							//	//se termina de llenar, lo pasamos a el reporte
							//	/*txtReport(nameArchive, LISTBOX, hDlg, 1)*/;
							//}/*
							HWND LISTBOX = GetDlgItem(hDlg, LB_CITAS);
							txtReport(nameArchive, LISTBOX, hDlg, 1);
							SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1,0);
							SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0);//RADIO DE REGISTRAR/EDITAR
							SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED, 0);//RADIO DE BUSCAR
						}
						else {
							MessageBoxA(hDlg, "Error", "No hay elementos que coincidan", MB_OK);
						}
						
					}
					else {
						MessageBoxA(hDlg, "error", "No hay citas registradas", MB_ICONERROR);
					}
				}
			}
			//BUSQUEDA POR SEMANA
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO7) == BST_CHECKED) {
			
			char tempdate[50];
			char tempshortdate[12];

			SendMessageA(GetDlgItem(hDlg, DTP_BUS), WM_GETTEXT, sizeof(tempdate), (LPARAM)tempdate);
			if (tempdate[0] != '\0') {
				if (Citas.origen != NULL) {
					string date(tempdate);
					int n = 0;
					n = date.find(",");
					string diaSem = date.substr(0, n);
					int contN = 0;
					int contP = 0;
					int dayInit = 0, dayEnd = 0;
					string shortdate = shortDateChanger(tempdate);
					strcpy(tempshortdate, shortdate.c_str());
					//esto es pa la semana
					if (strcmp(diaSem.c_str(), "lunes") == 0) {
						contP = 0;
						contN = 4;
					}
					else if (strcmp(diaSem.c_str(), "martes") == 0) {
						contP = 1;
						contN = 3;
					}
					else if (strcmp(diaSem.c_str(), "miércoles") == 0) {
						contP = 2;
						contN = 2;
					}
					else if (strcmp(diaSem.c_str(), "jueves") == 0) {
						contP = 3;
						contN = 1;
					}
					else if (strcmp(diaSem.c_str(), "viernes") == 0) {
						contP = 4;
						contN = 0;
					}
					else if (strcmp(diaSem.c_str(), "sábado") == 0 || strcmp(diaSem.c_str(), "domingo") == 0) {
						//Error, seleccione un di entre semana
						MessageBoxA(hDlg, "Error", "seleccione dia entre semana", MB_ICONERROR);
					}
					if (strcmp(diaSem.c_str(), "sábado") != 0 && strcmp(diaSem.c_str(), "domingo") != 0) {
						//pal dia, mes y año usamos esto
						
						int n = 0;
						n = shortdate.find("/");
						string dia = shortdate.substr(0, n);
						shortdate = shortdate.substr(n + 1, shortdate.length());
						int dayI = atoi(dia.c_str());
						n = shortdate.find("/");
						string mes = shortdate.substr(0, n);
						shortdate = shortdate.substr(n + 1, shortdate.length());
						int mesI = atoi(mes.c_str());
						string year = shortdate;
						int yearI = atoi(year.c_str());
						//buscas de un solo lado
						if (contN == 0 || contP == 0) {
							if (contN == 0) {
								//solo buscas para atras.
								if (dayI - contP < 1) {
									dayInit = dayEnd = 1;
								}
								else {
									dayInit = dayI - contP;
									dayEnd = dayI;
								}
							}
							else if (contP == 0) {
								//solo buscas adelante.
								if (mesI == 1 || mesI == 3 || mesI == 5 || mesI == 7 ||
									mesI == 8 || mesI == 10 || mesI == 12) {
									if (dayI + contN > 31) {
										dayEnd = 31;
										dayInit = dayI;
									}
									else {
										dayEnd = dayI + contN;
										dayInit = dayI;
									}

								}
								else if (mesI == 4 || mesI == 6 || mesI == 9 || mesI == 11) {
									if (dayI + contN > 30) {
										dayEnd = 30;
										dayInit = dayI;
									}
									else {
										dayEnd = dayI + contN;
										dayInit = dayI;
									}
								}
								else if (mesI == 2) {

									if ((yearI % 2) == 0) {
										//año bisiesto, feb tiene 29 dias
										if (dayI + contN > 29) {
											dayEnd = 29;
											dayInit = dayI;
										}
										else {
											dayEnd = dayI + contN;
											dayInit = dayI;
										}
									}
									else {
										//feb tiene 28 dias
										if (dayI + contN > 28) {
											dayEnd = 28;
											dayInit = dayI;
										}
										else {
											dayEnd = dayI + contN;
											dayInit = dayI;
										}
									}
								}

							}

						}
						//buscas adelante y atras
						else {
							if (mesI == 1 || mesI == 3 || mesI == 5 || mesI == 7 ||
								mesI == 8 || mesI == 10 || mesI == 12) {
								if (dayI - contP < 1) {
									dayInit = 1;
								}
								else {
									dayInit = dayI - contP;
								}
								if (dayI + contN > 31) {
									dayEnd = 31;
								}
								else {
									dayEnd = dayI + contN;
								}
							}
							else if (mesI == 4 || mesI == 6 || mesI == 9 || mesI == 11) {
								if (dayI - contP < 1) {
									dayInit = 1;
								}
								else {
									dayInit = dayI - contP;
								}
								if (dayI + contN > 30) {
									dayEnd = 30;
								}
								else {
									dayEnd = dayI + contN;
								}

							}
							else if (mesI == 2) {

								if ((yearI % 2) == 0) {
									//año bisiesto, feb tiene 29 dias
									if (dayI - contP < 1) {
										dayInit = 1;
									}
									else {
										dayInit = dayI - contP;
									}
									if (dayI + contN > 29) {
										dayEnd = 29;
									}
									else {
										dayEnd = dayI + contN;
									}

								}
								else {
									//feb tiene 28 dias
									if (dayI - contP < 1) {
										dayInit = 1;
									}
									else {
										dayInit = dayI - contP;
									}
									if (dayI + contN > 28) {
										dayEnd = 28;
									}
									else {
										dayEnd = dayI + contN;
									}
								}
							}

						}
						//genera una lista de fechas
						int size = 0;
						if (dayEnd == dayInit) {
							//busca una sola fecha
							string dy = to_string(dayInit);
							string fecha = dy + "/" + mes + "/" + year;
							Cits::Node* aux = Citas.origen;
							SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_RESETCONTENT, 0, 0);
							int cont = 0;
							if (aux != NULL) {
								while (aux != NULL) {
									if (strcmp(fecha.c_str(), aux->shortDate) == 0 && aux->attend == false && aux->canceled == false) {
										string myFolio = to_string(aux->ID);
										SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)myFolio.c_str());
										cont++;
									}
									aux = aux->next;
								}
								//CARGAR TODOS LOS DATOS
								if (cont > 0) {
									
									char nameArchive[50] = "Reporte_citas_semanal.TXT";
									HWND LISTBOX = GetDlgItem(hDlg, LB_CITAS);
									txtReport(nameArchive, LISTBOX, hDlg, 1);
									SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, 0);
									SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0);//RADIO DE REGISTRAR/EDITAR
									SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED, 0);//RADIO DE BUSCAR
								}
								else {
									MessageBoxA(hDlg, "Busqueda finalizada", "No se encontraron elementos coincidentes", MB_OK);
								}

							}
							else {
								//no hay citas
								MessageBoxA(hDlg, "ERROR", "No hay citas", MB_ICONERROR);
							}
						}
						//busca todas las fechas que tenga que buscar
						else {
							size = dayEnd - dayInit;
							string* arr = new string[size]();
							for (int i = 0; i < size; i++) {
								if (dayInit < 10) {
									arr[i] = "0";
									string dy = to_string(dayInit);
									arr[i].append(dy);

								}
								else {
									string dy = to_string(dayInit);
									arr[i] = dy;
								}
								arr[i].append("/");
								arr[i].append(mes);
								arr[i].append("/");
								arr[i].append(year);
								
								dayInit++;
							}
							//empieza la busqueda de verdad
							Cits::Node* aux = Citas.origen;
							SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_RESETCONTENT, 0, 0);
							if (aux != NULL) {
								int cont = 0;
								while (aux != NULL) {
									for (int i = 0; i < size; i++) {
										if (strcmp(arr[i].c_str(), aux->shortDate) == 0 && aux->attend == false && aux->canceled == false) {
											string myFolio = to_string(aux->ID);
											SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)myFolio.c_str());
											cont++;
										}
									}
									aux = aux->next;
								}
								//CARGAR TODOS LOS DATOS
								if (cont > 0) {
									MessageBoxA(hDlg, "Busqueda exitosa", "Haga clic en la lista. Elementos coincidentes:" + cont, MB_OK);
									char nameArchive[50] = "Reporte_citas_semanal.txt";
									HWND LISTBOX = GetDlgItem(hDlg, LB_CITAS);
									txtReport(nameArchive, LISTBOX, hDlg, 1);
									SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, 0);
									SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED, 0);//RADIO DE REGISTRAR/EDITAR
									SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED, 0);//RADIO DE BUSCAR
								}
								else {
									MessageBoxA(hDlg, "Busqueda finalizada", "No se encontraron elementos coincidentes", MB_OK);
								}
							}
							else {
								//no hay citas
								MessageBoxA(hDlg, "ERROR", "No hay citas", MB_ICONERROR);
							}
						}
					}
					
				}
				else {
					//no hay citas
					MessageBoxA(hDlg, "ERROR", "No hay citas", MB_ICONERROR);
				}

			}
			else {
				MessageBoxA(NULL, "Seleccione una fecha", "Error", MB_OK);
			}

			}

		}
		else if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED) {
		char tempcancel[250];
		char tempservC[30];
		char tempmech[50];
		char tempplacs[12];
		char tempname[50];
		char tempnum[13];
		char tempdate[50];
		char temphour[30];
		char tempShortDate[12];
		bool OK1 = false, OK2 = false, OK3 = false, OK4 = false, OK5 = false, OK6 = false, OK7 = false, OK8 = false, OK9 = false;

#pragma region service

		int indexS = SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_GETCURSEL, 0, 0);
		if (indexS == -1) {
			MessageBoxA(NULL, "Seleccione servicios", "Error", MB_OK);
			break;
		}
		else {
			/*Obtenemos el texto del combo y lo mostramos*/
			int lengthS = GetWindowTextLengthA(GetDlgItem(hDlg, CB_SERVCT));
			GetWindowTextA(GetDlgItem(hDlg, CB_SERVCT), tempservC, lengthS + 1);
			OK1 = true;
		}
#pragma endregion

#pragma region Mechanic
		int indexM = SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_GETCURSEL, 0, 0);
		if (indexM == -1) {
			MessageBoxA(NULL, "Seleccione mecanico", "Error", MB_OK);
			break;
		}
		else {
			/*Obtenemos el texto del combo y lo mostramos*/
			int lengthM = GetWindowTextLength(GetDlgItem(hDlg, CB_MECH));
			GetWindowTextA(GetDlgItem(hDlg, CB_MECH), tempmech, lengthM + 1);
			OK2 = true;
		}
#pragma endregion

#pragma region client placas

		int indexC = SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_GETCURSEL, 0, 0);
		if (indexC == -1) {
			MessageBoxA(NULL, "Seleccione una placa", "Error", MB_OK);
			break;
		}
		else {
			/*Obtenemos el texto del combo y lo mostramos*/
			int lengthC = GetWindowTextLength(GetDlgItem(hDlg, CB_CLIENT));
			GetWindowTextA(GetDlgItem(hDlg, CB_CLIENT), tempplacs, lengthC + 1);
			OK3 = true;
		}
#pragma endregion

#pragma region tel y nombre
		SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
		SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_GETTEXT, sizeof(tempnum), (LPARAM)tempnum);
		if (tempname[0] != '\0') {
			OK4 = true;
			if (tempnum[0] != '\0') {
				OK5 = true;
			}
			else {
				OK5 = false;
			}
		}
		else {
			OK4 = false;
		}

#pragma endregion


#pragma region hour  
		int indexH = SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_GETCURSEL, 0, 0);
		if (indexH == -1) {
			MessageBoxA(NULL, "Seleccione hora", "Error", MB_OK);
			break;
		}
		else {
			/*Obtenemos el texto del combo y lo mostramos*/
			int lengthH = GetWindowTextLength(GetDlgItem(hDlg, CB_HOUR));
			GetWindowTextA(GetDlgItem(hDlg, CB_HOUR), temphour, lengthH + 1);
			if (temphour[0] != '\0') {
				OK6 = true;
			}
			else {
				OK6 = false;
			}
		}

#pragma endregion

#pragma region fecha

		if (IsWindowEnabled(GetDlgItem(hDlg, IDEDIT)) != 0) {

			SendMessageA(GetDlgItem(hDlg, DTP_CITS), WM_GETTEXT, sizeof(tempdate), (LPARAM)tempdate);
			string shDate = shortDateChanger(tempdate);
			strcpy(tempShortDate, shDate.c_str());

			int year = _actualTime->tm_year + 1900;
			int month = _actualTime->tm_mon + 1;
			int day = _actualTime->tm_mday;
			string fechaActual;
			if (day < 10) {
				fechaActual = "0";
				fechaActual.append(to_string(day));
			}
			else {
				fechaActual = to_string(day);
			}
			fechaActual.append("/");
			if (month < 10) {
				fechaActual.append("0");
				fechaActual.append(to_string(month));
			}
			else {
				fechaActual.append(to_string(month));
			}
			fechaActual.append("/");
			fechaActual.append(to_string(year));
			char date[12];
			SendMessageA(GetDlgItem(hDlg, LBL_FECHA), WM_GETTEXT, sizeof(date), (LPARAM)date);

			if (strcmp(fechaActual.c_str(), tempShortDate) == 0 && strcmp(fechaActual.c_str(), date) != 0) {
				SendMessageA(GetDlgItem(hDlg, LBL_FECHA), WM_GETTEXT, sizeof(tempShortDate), (LPARAM)tempShortDate);
			}
			OK7 = true;
		}
		else {

			SendMessageA(GetDlgItem(hDlg, DTP_CITS), WM_GETTEXT, sizeof(tempdate), (LPARAM)tempdate);
			if (tempdate[0] != '\0') {
				OK7 = validDate(tempdate, temphour);
				if (OK7) {
					string shDate = shortDateChanger(tempdate);
					strcpy(tempShortDate, shDate.c_str());
				}
				else {
					OK7 = false;
					MessageBoxA(NULL, "Seleccione fecha valida. No hay servicio sabados ni domingos", "Error", MB_OK);
				}
			}
			else {
				OK7 = false;
				MessageBoxA(NULL, "Seleccione una fecha", "Error", MB_OK);
			}
		}

#pragma endregion

#pragma region checkboxes
		//si le das atendido, no la puedes cancelar
		if (IsDlgButtonChecked(hDlg, CH_ATT) == BST_CHECKED) {
			OK8 = true;
			OK9 = false;
		}
		else if (IsDlgButtonChecked(hDlg, CH_CANC) == BST_CHECKED) {
			//si le das cancelar, no la puedes atender
			OK9 = true;
			OK8 = false;
			//MOTIVO DE CANCELACION
			SendMessageA(GetDlgItem(hDlg, TB_CANCELDESC), WM_GETTEXT, sizeof(tempcancel), (LPARAM)tempcancel);
		}
#pragma endregion
		//si no fue edicion
		if (IsWindowEnabled(GetDlgItem(hDlg, IDEDIT)) == 0) {
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED) {
				//VALIDAR PARA CREAR NUEVO NODO POR PRIMERA VEZ
				if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6 && OK7) {

					bool repetido = Citas.lookDouble(tempmech, tempShortDate, temphour);

					if (!repetido) {
						//como la cita es nueva, ni lo atienden ni lo cancelan 
						Cits::Node* nuevo = Citas.creItem(tempservC, tempmech, tempplacs, tempname, tempnum, tempShortDate, temphour, false, false);
						bool insertar = Citas.inItem(nuevo);
						if (insertar) {
							MessageBoxA(hDlg, "Enhorabuena", "Cita creada", MB_OK);
							string folio = to_string(nuevo->ID);
							SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)folio.c_str());
							SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, NULL);

							//limpieza
							SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, CH_ATT), BM_SETCHECK, BST_UNCHECKED, 0);
							SendMessageA(GetDlgItem(hDlg, CH_CANC), BM_SETCHECK, BST_UNCHECKED, 0);
							SYSTEMTIME DAYSYST;
							GetLocalTime(&DAYSYST);
							LPSYSTEMTIME ptrdaysyst = &DAYSYST;
							SendMessageA(GetDlgItem(hDlg, DTP_CITS), DTM_SETSYSTEMTIME, 0, (LPARAM)ptrdaysyst);
							char limpia[30] = "";
							loadPic(GetDlgItem(hDlg, PC_CITAS), limpia, 50, 50);

						}
						else {
							MessageBoxA(hDlg, "Error", "Cita  NO creada", MB_ICONERROR);
						}

					}
					else {
						MessageBoxA(hDlg, "Error", "Cita  NO creada, ya esta ocupada esa fecha", MB_ICONERROR);
					}

				}
			}
		}
		else {
			//si si fue edicion
			if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6 && OK7) {
				char folio[10];
				SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_GETTEXT, sizeof(folio), (LPARAM)folio);
				Cits::Node* edit = Citas.findItem(folio);
				if (edit != NULL) {
					if (strcmp(edit->specialC, tempservC) == 0 && strcmp(edit->mechName, tempmech) == 0
						&& strcmp(edit->shortDate, tempShortDate) == 0 && strcmp(edit->shortHour, temphour) == 0) {
						//el criterio del sort no cambio, vemos si cambia lo demas
						//Como no cambia nada, esto es para ver si la marcas como atendida o no
						if (strcmp(edit->clientName, tempname) == 0 && strcmp(edit->placas, tempplacs) == 0
							&& strcmp(edit->clientTel, tempnum) == 0) {
							//como la cita es editada, podemos ver si se puede atender o no segun el sistema
							if (OK8) {
								bool validEditAtt = validAttend(tempdate, temphour, OK8);
								if (validEditAtt) {
									edit->attend = true;
									edit->canceled = false;
								}
							}
							else if (OK9) {
								edit->canceled = true;
								strcpy(edit->cancelMotive, tempcancel);
							}
							else {
								edit->attend = false;
								edit->canceled = false;
							}
							MessageBoxA(hDlg, "CITA EDITADA", "ENHORABUENA", MB_OK);
							SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, LBL_FECHA), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, TB_CANCELDESC), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, CH_ATT), BM_SETCHECK, BST_UNCHECKED, 0);
							SendMessageA(GetDlgItem(hDlg, CH_CANC), BM_SETCHECK, BST_UNCHECKED, 0);
							SYSTEMTIME DAYSYST;
							GetLocalTime(&DAYSYST);
							LPSYSTEMTIME ptrdaysyst = &DAYSYST;
							SendMessageA(GetDlgItem(hDlg, DTP_CITS), DTM_SETSYSTEMTIME, 0, (LPARAM)ptrdaysyst);
							char limpia[30] = "";
							loadPic(GetDlgItem(hDlg, PC_CITAS), limpia, 50, 50);

						}
						//aqui cambio algo del cliente
						else {
							//cambiamos lo del cliente
							if (OK9) {
								//si hay motivo de cancelacion
								Citas.editItem(edit, tempplacs, tempname, tempnum, OK8, OK9, tempcancel);
							}
							else {
								//si no hay motivo de cancelacion
								char empty[20] = "";
								Citas.editItem(edit, tempplacs, tempname, tempnum, OK8, OK9, empty);
							}
							MessageBoxA(hDlg, "cita editada", "enhorabuena", MB_OK);
							SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_SETCURSEL, -1, 0);
							SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, TB_CANCELDESC), WM_SETTEXT, 0, (LPARAM)"");
							SendMessageA(GetDlgItem(hDlg, CH_ATT), BM_SETCHECK, BST_UNCHECKED, 0);
							SendMessageA(GetDlgItem(hDlg, CH_CANC), BM_SETCHECK, BST_UNCHECKED, 0);
							SYSTEMTIME DAYSYST;
							GetLocalTime(&DAYSYST);
							LPSYSTEMTIME ptrdaysyst = &DAYSYST;
							SendMessageA(GetDlgItem(hDlg, DTP_CITS), DTM_SETSYSTEMTIME, 0, (LPARAM)ptrdaysyst);
							char limpia[30] = "";
							loadPic(GetDlgItem(hDlg, PC_CITAS), limpia, 50, 50);
						}

					}
					else {
						//si el criterio del sort cambia, se elimina el nodo y se crea uno nuevo mejor
						//antes de eliminar, quitamos el objeto del listbox
						char testString[12];
						int selIndex = 0;
						int items = GetListBoxInfo(GetDlgItem(hDlg, LB_CITAS));
						while (selIndex < items) {
							SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETTEXT, selIndex, (LPARAM)testString);
							int folioComp = 0;
							folioComp = atoi(testString);
							if (edit->ID == folioComp) {
								SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_DELETESTRING, selIndex, 0);
								break;
							}
							selIndex++;
						}
						//ahora si lo elimina
						Citas.delItem(edit);
						bool repetido = Citas.lookDouble(tempmech, tempShortDate, temphour);

						if (!repetido) {
							//como la cita es nueva, ni lo atienden ni lo cancelan 
							Cits::Node* nuevo = Citas.creItem(tempservC, tempmech, tempplacs, tempname, tempnum, tempShortDate, temphour, false, false);
							bool insertar = Citas.inItem(nuevo);
							if (insertar) {
								string folio = to_string(nuevo->ID);
								MessageBoxA(hDlg, folio.c_str(), "Cita nueva creada. Folio:", MB_OK);
								SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_ADDSTRING, 0, (LPARAM)folio.c_str());
								SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_SETCURSEL, -1, NULL);

								SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, LBL_FECHA), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_SETCURSEL, -1, 0);
								SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_SETCURSEL, -1, 0);
								SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_SETCURSEL, -1, 0);
								SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_SETCURSEL, -1, 0);
								SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_CANCELDESC), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, CH_ATT), BM_SETCHECK, BST_UNCHECKED, 0);
								SendMessageA(GetDlgItem(hDlg, CH_CANC), BM_SETCHECK, BST_UNCHECKED, 0);
								char limpia[30] = "";
								loadPic(GetDlgItem(hDlg, PC_CITAS), limpia, 50, 50);
							}
							else {
								MessageBoxA(hDlg, "Error", "Cita  NO creada", MB_ICONERROR);
							}

						}
						else {
							MessageBoxA(hDlg, "Error", "Cita  NO creada, ya esta ocupada esa fecha", MB_ICONERROR);
						}

					}
					EnableWindow(GetDlgItem(hDlg, IDEDIT), false);
					EnableWindow(GetDlgItem(hDlg, IDDELETE), false);
					ShowWindow(GetDlgItem(hDlg, LBL_CANCELDESC), false);
					ShowWindow(GetDlgItem(hDlg, TB_CANCELDESC), false);
				}
			}
			else {
				MessageBoxA(hDlg, "Error", "Introduzca datos válidos", MB_ICONERROR);
			}
		}
			}
		}
		else if (LOWORD(wParam) == IDEDIT && HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hDlg, CB_CLIENT), true);
		EnableWindow(GetDlgItem(hDlg, DTP_CITS), true);
		EnableWindow(GetDlgItem(hDlg, CB_SERVCT), true);
		EnableWindow(GetDlgItem(hDlg, CB_MECH), true);
		EnableWindow(GetDlgItem(hDlg, CB_HOUR), true);
		EnableWindow(GetDlgItem(hDlg, CH_ATT), true);
		EnableWindow(GetDlgItem(hDlg, CH_CANC), true);
		EnableWindow(GetDlgItem(hDlg, TB_TEL), true);
		EnableWindow(GetDlgItem(hDlg, TB_NAME), true);
		EnableWindow(GetDlgItem(hDlg, IDOK), true);
		
		}
		else if (LOWORD(wParam) == IDDELETE && HIWORD(wParam) == BN_CLICKED) {
		char testString[12];
		int index=(int)SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETCURSEL, 0,0);
		SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETTEXT, index, (LPARAM)testString);
		Cits::Node* del = Citas.findItem(testString);
		
		int selIndex = 0;
		int items = GetListBoxInfo(GetDlgItem(hDlg, LB_CITAS));
		while (selIndex < items) {
			SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_GETTEXT, selIndex, (LPARAM)testString);
			int folioComp = 0;
			folioComp = atoi(testString);
			if (del->ID == folioComp) {
				SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_DELETESTRING, selIndex, 0);
				break;
			}
			selIndex++;
		}

		//ahora si borramos el nodo
		Citas.delItem(del);
		//desactivamos los botones
		EnableWindow(GetDlgItem(hDlg, IDEDIT), false);
		EnableWindow(GetDlgItem(hDlg, IDDELETE), false);
		//LIMPIEZA DE CONTENIDO
		SendMessageA(GetDlgItem(hDlg, LBL_FOLIO), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, LBL_FECHA), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, CB_SERVCT), CB_SETCURSEL, -1, 0);
		SendMessageA(GetDlgItem(hDlg, CB_MECH), CB_SETCURSEL, -1, 0);
		SendMessageA(GetDlgItem(hDlg, CB_HOUR), CB_SETCURSEL, -1, 0);
		SendMessageA(GetDlgItem(hDlg, CB_CLIENT), CB_SETCURSEL, -1, 0);
		SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, TB_TEL), WM_SETTEXT, 0, (LPARAM)"");
		SendMessageA(GetDlgItem(hDlg, CH_ATT), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessageA(GetDlgItem(hDlg, CH_CANC), BM_SETCHECK, BST_UNCHECKED, 0);
		SYSTEMTIME DAYSYST;
		GetLocalTime(&DAYSYST);
		LPSYSTEMTIME ptrdaysyst = &DAYSYST;
		SendMessageA(GetDlgItem(hDlg, DTP_CITS), DTM_SETSYSTEMTIME, 0, (LPARAM)ptrdaysyst);
		char limpia[30] = "";
		loadPic(GetDlgItem(hDlg, PC_CITAS), limpia, 50, 50);


		}
		else if (LOWORD(wParam) == ID_CANCELC && HIWORD(wParam) == BN_CLICKED) {
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hCits, 0);
		break;
		}
		else if (LOWORD(wParam) == BTN_BINSEARCH && HIWORD(wParam) == BN_CLICKED) {
		char testMat[12];
		SendMessageA(GetDlgItem(hDlg, IDC_EDIT2),WM_GETTEXT,sizeof(testMat),(LPARAM)testMat);
		if (testMat[0] != '\0') {
			int matbb = atoi(testMat);
			Mechs::leaf* hojita = Mechanics->buscarHoja(Mechanics->tree, matbb);
			if (hojita != NULL) {
				MessageBoxA(hDlg, "mecanico encontrado", "Enhorabuena", MB_OK);
				SendMessageA(GetDlgItem(hDlg, CB_MECHB), CB_ADDSTRING, 0, (LPARAM)hojita->name);
				SendMessageA(GetDlgItem(hDlg, CB_MECHB), CB_SETCURSEL, 0, 0);

			}
			else {
				MessageBoxA(hDlg, "mecanico NO encontrado", "error", MB_ICONERROR);
			}
		}
		else {
			MessageBoxA(hDlg, "Introduzca un numero de empleado", "error", MB_ICONERROR);
		}
		}
		if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED) {
		SendMessageA(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_UNCHECKED, 0);
		
#pragma region Esconder y mostrar botones
		//ESCONDER LOS LBL
		ShowWindow(GetDlgItem(hDlg, IDC_STATICLBL), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICS), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICM), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICP), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICC), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICT), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICF), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICH), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, LBL_FOLIO), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, LBL_FECHA), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICPM), SW_HIDE);
		//AHORA LOS BOTONES
		ShowWindow(GetDlgItem(hDlg, PC_CITAS), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_SERVCT), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_MECH), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_CLIENT), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_HOUR), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, TB_NAME), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, TB_TEL), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, DTP_CITS), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CH_ATT), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CH_CANC), SW_HIDE);
		//POR SI LAS DUDAS, INHABILITAR CANCELAR Y EDITAR
		EnableWindow(GetDlgItem(hDlg, IDEDIT), false);
		EnableWindow(GetDlgItem(hDlg, IDDELETE), false);
		EnableWindow(GetDlgItem(hDlg, IDOK), true);
		//MOSTRAR TODO LO DE LA VENTANA BUSCAR
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO4), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO5), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO6), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO7), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT2), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICBIN), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, BTN_BINSEARCH), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, TB_FOLIOB), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CB_SERVB), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CB_MECHB), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, DTP_BUS), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, LBL_INST), SW_SHOW);
#pragma endregion
		//limpiar el listbox
		SendMessageA(GetDlgItem(hDlg, LB_CITAS), LB_RESETCONTENT, 0, 0);

}
		else if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED) {
	/*	CheckDlgButton(hDlg, IDC_RADIO3, BST_UNCHECKED);*/
		SendMessageA(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED, 0);
				//ESCONDER LOS LBL
		ShowWindow(GetDlgItem(hDlg, IDC_STATICLBL), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICS), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICM), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICP), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICC), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICF), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICH), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, LBL_FOLIO), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, LBL_FECHA), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICPM), SW_SHOW);
		//AHORA LOS BOTONES
		ShowWindow(GetDlgItem(hDlg, PC_CITAS), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CB_SERVCT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CB_MECH), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CB_CLIENT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CB_HOUR), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, TB_NAME), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, TB_TEL), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, DTP_CITS), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CH_ATT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, CH_CANC), SW_SHOW);
		//POR SI LAS DUDAS, INHABILITAR CANCELAR Y EDITAR
		EnableWindow(GetDlgItem(hDlg, IDEDIT), true);
		EnableWindow(GetDlgItem(hDlg, IDDELETE), true);
		//MOSTRAR TODO LO DE LA VENTANA BUSCAR
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO4), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO5), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO6), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO7), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT2), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICBIN), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, BTN_BINSEARCH), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, TB_FOLIOB), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_SERVB), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, CB_MECHB), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, DTP_BUS), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, LBL_INST), SW_HIDE);
#pragma endregion
		}
		if (IsDlgButtonChecked(hDlg, CH_CANC) == BST_CHECKED) {
		ShowWindow(GetDlgItem(hDlg, LBL_CANCELDESC), true);
		ShowWindow(GetDlgItem(hDlg, TB_CANCELDESC), true);
		
		}
		else if (IsDlgButtonChecked(hDlg, CH_CANC) == BST_UNCHECKED) {
		ShowWindow(GetDlgItem(hDlg, LBL_CANCELDESC), false);
		ShowWindow(GetDlgItem(hDlg, TB_CANCELDESC), false);
        }
		//buscar cita
		
		
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hCits, 0);
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hClientsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		
	SendMessageA(GetDlgItem(hDlg, LBL_USERCL), WM_SETTEXT, 0, (LPARAM)Usuario.cUser);
#pragma region Combo Kilometraje
		SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"Menos de 30,000 km");
		SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"De 30,000 -100,000 km");
		SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"De 100,000 -200,000 km");
		SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"De 200,000 -300,000 km");
		SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"Mas de 300,000 km");

#pragma endregion

#pragma region Combo Estados
		ifstream StateAR;
		string State;
		StateAR.open("Data\\Estados.txt");
		if (!StateAR.is_open()) {
			MessageBoxA(hDlg, "no hay estados", "que no hay estados dije", MB_ICONERROR);
		}
		else {
			for (int i = 0; i < 33; i++) {
				getline(StateAR, State);
				char state1[50];
				strcpy(state1, State.c_str());
				SendMessageA(GetDlgItem(hDlg, IDC_COMBO4), CB_ADDSTRING, 0, (LPARAM)state1);
				strcpy(state1, "");
				
			}
			StateAR.close();
		}
#pragma endregion
		//LLENAR CLIENTES
		SendMessageA(GetDlgItem(hDlg, LB_CL), LB_RESETCONTENT, 0, 0);
		if (!Clients.ClientIsEmpty()) {
			Clients.aux = Clients.lista;
			while (Clients.aux != NULL) {
				SendMessageA(GetDlgItem(hDlg, LB_CL), LB_ADDSTRING, 0, (LPARAM)Clients.aux->placas);;
				Clients.aux = Clients.aux->next;
			}
			Clients.aux = Clients.origen;
		}
		else {
			//no hay clientes registrados
		}

		ShowWindow(GetDlgItem(hDlg, TB_BIGDIR), SW_HIDE);
		EnableWindow(GetDlgItem(hDlg, IDEDITCL), false);
		EnableWindow(GetDlgItem(hDlg, IDDELCL), false);

	}
	case WM_COMMAND:
	{
		
		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(hClients, RB_CL) == BST_CHECKED) {
				char tempname[30];
				char tempapel[30];
				char temphone[15];
				char templacs[12];
				char tempbrand[30];
				char tempmodel[30];
				char tempyear[6];
				char tempkm[50];
				//direccion
				char tempst[50];
				char tempnum[10];
				char tempcol[50];
				char tempmun[50];
				char tempstate[30];
				bool OK1=false, OK2 = false, OK3 = false, OK4 = false, OK5 = false, OK6 = false, OK7 = false, OK8 = false, OK9 = false, OK10 = false, OK11 = false, OK12 = false;
				bool OKDIR = false;
				char tempdir[250];

#pragma region NAME
				SendMessageA(GetDlgItem(hClients, IDC_EDIT1), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
				int cont = 0;
				int j = 0;
				if (tempname[j] == '\0') {
					MessageBoxA(NULL, "El nombre esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempname[j] != '\0')
					{
						if (((tempname[j] >= 97) && (tempname[j] <= 122)) || ((tempname[j] >= 65) && (tempname[j] <= 90)) || (tempname[j] == 32))
						{
							if (j == 0)
							{
								if ((tempname[j] >= 65) && (tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}

								else if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j] - 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}
							}
							else if (j > 0)
							{
								if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j];
								}
								else if ((tempname[j] >= 65) && (tempname[j] <= 90))
								{
									tempname[j] = tempname[j] + 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}

								if (tempname[j - 1] == 32 && (tempname[j] >= 65 && tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}
								else if (tempname[j - 1] == 32 && (tempname[j] >= 97 && tempname[j] <= 122)) {
									tempname[j] = tempname[j] - 32;
								}

							}
							j++;
						}

						else {
							tempname[j] = tempname[j];
							cont = cont + 1;
							j++;
						}

					}
					if (cont > 0) {
						MessageBoxA(NULL, "El nombre solo debe tener letras", "ERROR", MB_ICONERROR);
						OK1 = false;
					}
					else {
						OK1 = true;
					}
				}
				SendMessageA(GetDlgItem(hDlg, TB_APEL), WM_GETTEXT, sizeof(tempapel), (LPARAM)tempapel);
				int cont1 = 0;
				j = 0;
				if (tempapel[j] == '\0') {
					MessageBoxA(NULL, "El apellido esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempapel[j] != '\0')
					{
						if (((tempapel[j] >= 97) && (tempapel[j] <= 122)) || ((tempapel[j] >= 65) && (tempapel[j] <= 90)) || (tempapel[j] == 32))
						{
							if (j == 0)
							{
								if ((tempapel[j] >= 65) && (tempapel[j] <= 90)) {
									tempapel[j] = tempapel[j];
								}

								else if ((tempapel[j] >= 97) && (tempapel[j] <= 122))
								{
									tempapel[j] = tempapel[j] - 32;
								}
								else if (tempapel[j] == 32) {
									tempapel[j] = tempapel[j];
								}
							}
							else if (j > 0)
							{
								if ((tempapel[j] >= 97) && (tempapel[j] <= 122))
								{
									tempapel[j] = tempapel[j];
								}
								else if ((tempapel[j] >= 65) && (tempapel[j] <= 90))
								{
									tempapel[j] = tempapel[j] + 32;
								}
								else if (tempapel[j] == 32) {
									tempapel[j] = tempapel[j];
								}

								if (tempapel[j - 1] == 32 && (tempapel[j] >= 65 && tempapel[j] <= 90)) {
									tempapel[j] = tempapel[j];
								}
								else if (tempapel[j - 1] == 32 && (tempapel[j] >= 97 && tempapel[j] <= 122)) {
									tempapel[j] = tempapel[j] - 32;
								}

							}
							j++;
						}

						else {
							tempapel[j] = tempapel[j];
							cont1 = cont1 + 1;
							j++;
						}

					}
					if (cont1 > 0) {
						MessageBoxA(NULL, "El apellido solo debe tener letras", "ERROR", MB_ICONERROR);
						OK1 = false;
					}
					else {
						OK1 = true;
					}
				}
#pragma endregion
#pragma region Phone
				SendMessageA(GetDlgItem(hClients, IDC_EDIT2), WM_GETTEXT, sizeof(temphone), (LPARAM)temphone);
				int cont3 = 0, f = 0;
				int car = 0;
				if (temphone[f] == '\0') {
					MessageBoxA(hDlg, "Llene la informacion", "Error", MB_ICONASTERISK);
				}
				else {
					while (temphone[f] != '\0') {
						if (isdigit(temphone[f])) {
							cont3 = cont3 + 1;
						}
						else {
							car = car + 1;
						}
						f++;
					}
				}
				if (car > 0) {
					MessageBoxA(hDlg, "El numero no debe contenerletras", "Error", MB_ICONERROR);
				}
				else if (cont3 > 0) {
					if (cont3 == 8) {
						OK2 = true;
					}
					else if (cont3 == 10) {
						OK2 = true;
					}
					else if (cont3 == 12) {
						OK2 = true;
					}
					else {
						MessageBoxA(hDlg, "El numero debe tener 8, 10 o 12 digitos", "Error", MB_ICONERROR);
						OK2 = false;
					}
				}
#pragma endregion
#pragma region PLACAS
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT12), WM_GETTEXT, sizeof(templacs), (LPARAM)templacs);
				int lets=0;
				int nums = 0;
				int i = 0;
				if (templacs[i] == '\0') {
					MessageBoxA(hDlg, "Error", "Placas vacias", MB_ICONERROR);
					OK3 = false;
				}
				while (templacs[i]!='\0') {
					if (i < 3) {
						if (isalpha(templacs[i])) {
							lets++;
						}
					}
					else if (i >= 3) {
						if (isdigit(templacs[i])) {
							nums++;
						}
					}
					i++;
				}
				if (lets == 3 && nums == 4) {
					//QUE LAS LETRAS SEAN MAYUSCULAS
					for (int i = 0; i < 3; i++) {
						if (templacs[i] > 96 && templacs[i] < 123) {
							templacs[i] = templacs[i] - 32;
						}
						else {
							templacs[i] = templacs[i];
						}
					}
					//placas listas
					OK3 = true;
				}
				else {
					MessageBoxA(hDlg, "Error", "Las placas deben ser formato AAA0000", MB_ICONASTERISK);
					OK3 = false;
				}
#pragma endregion
#pragma region Marca
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT4), WM_GETTEXT, sizeof(tempbrand), (LPARAM)tempbrand);
				string tempbrand1= validNames(tempbrand, OK4);
				strcpy(tempbrand, tempbrand1.c_str());
		
#pragma endregion

#pragma region Modelo
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT3), WM_GETTEXT, sizeof(tempmodel), (LPARAM)tempmodel);
				string tempmodel1=validNames(tempmodel, OK5);
				strcpy(tempmodel, tempmodel1.c_str());
#pragma endregion
#pragma region año
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT11), WM_GETTEXT, sizeof(tempyear), (LPARAM)tempyear);
				i = 0;
				int numy = 0;
				int alpy = 0;
				while (tempyear[i] != '\0') {
					if (isdigit(tempyear[i])) {
						numy++;
					}
					else {
						alpy++;
					}
					i++;
				}
				if (numy == 4&&alpy==0) {
					int year=atoi(tempyear);
					if (year > 1949 && year < 2023) {
						OK6 = true;
					}
					else {
						OK6 = false;
						MessageBoxA(hDlg, "error", "escriba un año valido mayor a 1950 y menor a 2023", MB_ICONASTERISK);
					}
				}
				else {
					OK6 = false;
					MessageBoxA(hDlg, "error", "escriba un año valido, solo numeros", MB_ICONASTERISK);
				}
#pragma endregion
#pragma region Kilometraje
				int indexK = SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);
				if (indexK == -1) {
					MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
					OK7 = false;
					break;
				}
				else {
					/*Obtenemos el texto del combo y lo mostramos*/
					int lengthK = GetWindowTextLength(GetDlgItem(hDlg, IDC_COMBO2));
					GetWindowTextA(GetDlgItem(hDlg, IDC_COMBO2), tempkm, lengthK + 1);
					OK7 = true;
				}

#pragma endregion
				if (IsWindowEnabled(GetDlgItem(hDlg, IDEDITCL)) == 0) {
#pragma region Calle
					SendMessageA(GetDlgItem(hDlg, IDC_EDIT6), WM_GETTEXT, sizeof(tempst), (LPARAM)tempst);
					string tempst1 = validNames(tempst, OK8);
					strcpy(tempst, tempst1.c_str());
#pragma endregion
#pragma region Numero
					SendMessageA(GetDlgItem(hDlg, IDC_EDIT7), WM_GETTEXT, sizeof(tempnum), (LPARAM)tempnum);
					i = 0;
					int numst = 0, alpst = 0;
					while (tempnum[i] != '\0') {
						if (isdigit(tempnum[i])) {
							numst++;
						}
						else {
							alpst++;
						}
						i++;
					}
					if (alpst == 0 && numst > 0) {
						int numST = atoi(tempnum);
						if (numST > 0) {
							OK9 = true;
						}
						else {
							OK9 = false;
							MessageBoxA(hDlg, "error", "escriba un numero mayor a 0", MB_ICONASTERISK);
						}
					}
					else {
						OK9 = false;
						MessageBoxA(hDlg, "error", "solo numeros", MB_ICONERROR);
					}
#pragma endregion
#pragma region Colonia
					SendMessageA(GetDlgItem(hDlg, IDC_EDIT8), WM_GETTEXT, sizeof(tempcol), (LPARAM)tempcol);
					string tempcol1 = validNames(tempcol, OK10);
					strcpy(tempcol, tempcol1.c_str());
#pragma endregion
#pragma region Municipio
					SendMessageA(GetDlgItem(hDlg, IDC_EDIT9), WM_GETTEXT, sizeof(tempmun), (LPARAM)tempmun);
					string tempmun1 = validNames(tempmun, OK11);
					strcpy(tempmun, tempmun1.c_str());
#pragma endregion
#pragma region Estado
					int indexE = SendMessageA(GetDlgItem(hDlg, IDC_COMBO4), CB_GETCURSEL, 0, 0);
					if (indexE == -1) {
						MessageBoxA(NULL, "Seleccione algo del combo", "Error", MB_OK);
						OK12 = false;
						break;
					}
					else {
						/*Obtenemos el texto del combo y lo mostramos*/
						int lengthE = GetWindowTextLength(GetDlgItem(hDlg, IDC_COMBO4));
						GetWindowTextA(GetDlgItem(hDlg, IDC_COMBO4), tempstate, lengthE + 1);
						OK12 = true;
					}

#pragma endregion
				}

				if (IsWindowEnabled(GetDlgItem(hDlg, TB_BIGDIR)) != 0) {
					SendMessageA(GetDlgItem(hDlg, TB_BIGDIR), WM_GETTEXT, sizeof(tempdir), (LPARAM)tempdir);
					
					if (tempdir[0] != '\0') {
						OKDIR = true;
					}
					else {
						OKDIR = false;
					}
				}
				if (IsWindowEnabled(GetDlgItem(hDlg, IDEDITCL)) != 0) {
					if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6 && OK7 && OKDIR) {
						char testPlac[12];
						int selIndex = (int)SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETCURSEL, 0, 0);
						SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testPlac);
						findCl = Clients.findItemP(testPlac);
						if (findCl != NULL) {
							if (strcmp(findCl->placas, templacs) == 0) {
								Clients.editItem(findCl, tempname, tempapel, templacs, tempbrand, tempmodel, tempyear, tempkm, temphone, tempdir);
							}
							else {
								if (!Clients.lookDouble(templacs)) {

									char placaDel[12];
									strcpy(placaDel, findCl->placas);
									Clients.editItem(findCl, tempname, tempapel, templacs, tempbrand, tempmodel, tempyear, tempkm, temphone, tempdir);

									char testString[12];
									int selIndex = 0;
									int items = GetListBoxInfo(GetDlgItem(hDlg, LB_CL));

									while (selIndex < items) {
										SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETTEXT, selIndex, (LPARAM)testString);
										if (strcmp(testString, placaDel) == 0) {
											SendMessageA(GetDlgItem(hDlg, LB_CL), LB_DELETESTRING, selIndex, NULL);
										}
										selIndex++;
									}
									
									MessageBoxA(hDlg, "Exito", "Se edito el cliente", MB_OK);
									SendMessageA(GetDlgItem(hDlg, LB_CL), LB_ADDSTRING, 0, (LPARAM)findCl->placas);
								}
								else {
									MessageBoxA(hDlg, "Cliente no guardado", "Matricula repetida", MB_OK);
								}
								strcpy(tempname, "");
								strcpy(templacs, "");
								strcpy(tempbrand, "");
								strcpy(tempmodel, "");
								strcpy(tempyear, "");
								strcpy(tempkm, "");
								strcpy(temphone, "");
								strcpy(tempst, "");
								strcpy(tempnum, "");
								strcpy(tempcol, "");
								strcpy(tempmun, "");
								strcpy(tempstate, "");

								ShowWindow(GetDlgItem(hDlg, TB_BIGDIR), SW_HIDE);
								//HIDE
								ShowWindow(GetDlgItem(hDlg, IDC_STATICCA), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_STATICNUM), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_STATICCOL), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_STATICMUN), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_STATICSTATE), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_EDIT6), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_EDIT7), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_EDIT8), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_EDIT9), SW_SHOW);
								ShowWindow(GetDlgItem(hDlg, IDC_COMBO4), SW_SHOW);
								//DEACTIVATE
								EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), true);
								EnableWindow(GetDlgItem(hDlg, IDC_EDIT2), true);
								EnableWindow(GetDlgItem(hDlg, IDC_EDIT11), true);
								EnableWindow(GetDlgItem(hDlg, IDC_EDIT12), true);
								EnableWindow(GetDlgItem(hDlg, IDC_EDIT3), true);
								EnableWindow(GetDlgItem(hDlg, IDC_EDIT4), true);
								EnableWindow(GetDlgItem(hDlg, IDC_COMBO2), true);
								EnableWindow(GetDlgItem(hDlg, TB_BIGDIR), false);
								//CLEAN BOXES
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT12), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT11), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, -1, NULL);
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT7), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT8), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_EDIT9), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, IDC_COMBO4), CB_SETCURSEL, -1, NULL);

								//deactivate edit
								EnableWindow(GetDlgItem(hDlg, IDEDITCL), false);
								EnableWindow(GetDlgItem(hDlg, IDDELCL), false);
							}
						}
					}
				}
				else {
					if (OK1 && OK2 && OK3 && OK4 && OK5 && OK6 && OK7 && OK8 && OK9 && OK10 && OK11 && OK12) {
						if (!Clients.lookDouble(templacs)) {
							Clts::Node* nuevo = Clients.creItem(tempname,tempapel, templacs, tempbrand, tempmodel, tempyear, tempkm, temphone, tempst, tempnum, tempcol, tempmun, tempstate);
							Clients.inItem(nuevo);
							MessageBoxA(hDlg, "Cliente guardado CON EXITO", "ALELUYA", MB_OK);
							//AÑADIR AL LISTBOX
							SendMessageA(GetDlgItem(hDlg,LB_CL),LB_ADDSTRING,0,(LPARAM)nuevo->placas);
							
						}
					}
				}//clean all
				strcpy(tempname, "");
				strcpy(tempapel, "");
				strcpy(templacs, "");
				strcpy(tempbrand, "");
				strcpy(tempmodel, "");
				strcpy(tempyear, "");
				strcpy(tempkm, "");
				strcpy(temphone, "");
				strcpy(tempst, "");
				strcpy(tempnum, "");
				strcpy(tempcol, "");
				strcpy(tempmun, "");
				strcpy(tempstate, "");
				//CLEAN BOXES

				SendMessageA(GetDlgItem(hDlg, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, TB_APEL), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT12), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT11), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, -1, NULL);
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT7), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT8), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT9), WM_SETTEXT, 0, (LPARAM)"");
				SendMessageA(GetDlgItem(hDlg, IDC_COMBO4), CB_SETCURSEL, -1, NULL);




			}
		}
		else if (LOWORD(wParam) == IDCANCEL && HIWORD(wParam) == BN_CLICKED) {
			 ShowWindow(hMain, SW_SHOW);
			 EndDialog(hClients, 0);
			 break;
		 }
		else if (LOWORD(wParam) == LB_CL && HIWORD(wParam) == LBN_SELCHANGE) {
		
#pragma region Hide and show stuff
		ShowWindow(GetDlgItem(hDlg, TB_BIGDIR), true);
		//HIDE
		ShowWindow(GetDlgItem(hDlg, IDC_STATICCA), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICNUM), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICCOL), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICMUN), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_STATICSTATE), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT6), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT7), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT8), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT9), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, IDC_COMBO4), SW_HIDE);
		//DEACTIVATE
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), false);
		EnableWindow(GetDlgItem(hDlg, TB_APEL), false);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT2), false);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT11), false);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT12), false);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT3), false);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT4), false);
		EnableWindow(GetDlgItem(hDlg, IDC_COMBO2), false);
		EnableWindow(GetDlgItem(hDlg, TB_BIGDIR), false);



#pragma endregion
		char testPlac[12];

		int selIndex = (int)SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETCURSEL, 0, 0);
		if (selIndex == -1) {

		}
		else {
			
			/*int txtl = GetWindowTextLength(GetDlgItem(hDlg, LB_CL));*/
			SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testPlac);
			/*GetWindowTextA(GetDlgItem(hDlg, LB_CL), testPlac, txtl + 1);*/
			findCl = Clients.findItemP(testPlac);
			if (findCl != NULL) {
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT1), WM_SETTEXT, 0,(LPARAM)findCl->nombre);
				SendMessageA(GetDlgItem(hDlg, TB_APEL), WM_SETTEXT, 0,(LPARAM)findCl->apellido);
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT2), WM_SETTEXT, 0,(LPARAM)findCl->telefono);
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0,(LPARAM)findCl->modelo);
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT4), WM_SETTEXT, 0,(LPARAM)findCl->marca);
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT11), WM_SETTEXT, 0,(LPARAM)findCl->year);
				SendMessageA(GetDlgItem(hDlg, IDC_EDIT12), WM_SETTEXT, 0,(LPARAM)findCl->placas);
				SendMessageA(GetDlgItem(hDlg, TB_BIGDIR), WM_SETTEXT, 0,(LPARAM)findCl->direccion);
				SetWindowTextA(GetDlgItem(hDlg, IDC_COMBO2), findCl->kilometraje);

				//activar los botones
				EnableWindow(GetDlgItem(hDlg, IDEDITCL), true);
				EnableWindow(GetDlgItem(hDlg, IDDELCL), true);
			}
			else {
				MessageBoxA(hDlg, "error", "no se encontro el cliente", MB_ICONERROR);
			}



		}

		}
		else if (LOWORD(wParam) == IDEDITCL && HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), true);
		EnableWindow(GetDlgItem(hDlg, TB_APEL), true);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT2), true);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT11), true);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT12), true);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT3), true);
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT4), true);
		EnableWindow(GetDlgItem(hDlg, IDC_COMBO2), true);
		EnableWindow(GetDlgItem(hDlg, TB_BIGDIR), true);
		}
		else if (LOWORD(wParam) == IDDELCL && HIWORD(wParam) == BN_CLICKED) {
			char placas[12];
			int selIndex = (int)SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETCURSEL, 0, 0);
			SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)placas);
			Clts::Node* look = Clients.findItemP(placas);
			if (look != NULL) {
				char testStringA[12];
				int selIndex = 0;
				int items = GetListBoxInfo(GetDlgItem(hDlg, LB_CL));
				while (selIndex < items) {
					SendMessageA(GetDlgItem(hDlg, LB_CL), LB_GETTEXT, selIndex, (LPARAM)testStringA);
					if (strcmp(testStringA,look->placas) == 0) {
						SendMessageA(GetDlgItem(hDlg, LB_CL), LB_DELETESTRING, selIndex, 0);
					}
					selIndex++;
				}
				Clients.delItem(look);
				MessageBoxA(hDlg, "Exito", "Se elimino el cliente", MB_OK);
			}
			else {
				MessageBoxA(hDlg, "error", "no se encontro el cliente", MB_ICONERROR);
			}

			EnableWindow(GetDlgItem(hDlg, IDEDITCL), false);
			EnableWindow(GetDlgItem(hDlg, IDDELCL), false);

			ShowWindow(GetDlgItem(hDlg, TB_BIGDIR), SW_HIDE);
			//HIDE
			ShowWindow(GetDlgItem(hDlg, IDC_STATICCA), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_STATICNUM), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_STATICCOL), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_STATICMUN), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_STATICSTATE), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_EDIT6), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_EDIT7), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_EDIT8), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_EDIT9), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_COMBO4), SW_SHOW);
			//DEACTIVATE
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), true);
			EnableWindow(GetDlgItem(hDlg, TB_APEL), true);
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT11), true);
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT12), true);
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT4), true);
			EnableWindow(GetDlgItem(hDlg, IDC_COMBO2), true);
			EnableWindow(GetDlgItem(hDlg, TB_BIGDIR), false);

			//CLEAN
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT12), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, TB_APEL), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT11), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, IDC_COMBO2), WM_SETTEXT, 0, (LPARAM)"");

			SendMessageA(GetDlgItem(hDlg, LB_CL), LB_SETCURSEL, -1, 0);


		}
		else if (IsDlgButtonChecked(hDlg, RB_RCL) == BST_CHECKED) {
		int size = Clients.sizeofList(Clients.origen)+1;
		if (size > 1) {
			string* arrL = new string[size];
			int i = 0;
			Clients.AddtoArray(Clients.origen, arrL, i);

			Clients.heapSort(arrL, size - 1);
			//se supone que para este punto el heap sort esta listo

			//mandamos la lista al listbox, pero solo con el numero de placa
			char placaC[8];
			SendMessageA(GetDlgItem(hDlg, LB_CL), LB_RESETCONTENT, 0, 0);
			for (i = 0; i <= size -1; i++) {
				if (strcmp(arrL[i].c_str(), "") != 0) {
					int n = arrL[i].find_last_of(" ");
					string placa = arrL[i].substr(n + 1, arrL[i].length());
					strcpy(placaC, placa.c_str());
					SendMessageA(GetDlgItem(hDlg, LB_CL), LB_ADDSTRING, 0, (LPARAM)placaC);
				}
			}
			//generar el reporte en un archivo de texto
			HWND listBoxClientes = GetDlgItem(hDlg, LB_CL);
			char nombreDeArchivo[] = "Reporte_de_clientes.txt";
			txtReport(nombreDeArchivo, listBoxClientes, hDlg, 2);
			delete[]arrL;
			SendMessageA(GetDlgItem(hDlg, RB_RCL), BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessageA(GetDlgItem(hDlg, LB_CL), LB_SETCURSEL, -1, 0);
		}
		else {
			//no hay clientes
		}
		}
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hClients, 0);
		break;
	}
	case WM_DESTROY:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hClients, 0);
		break;
	}
	}
	return 0;
}
BOOL CALLBACK hServsFun(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		SendMessageA(GetDlgItem(hDlg, LBL_USERS), WM_SETTEXT, 0, (LPARAM)Usuario.cUser);
		//aqui se deben cargar las especialidades

		Services.aux = Services.lista;
		while (Services.aux != NULL) {
			SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_ADDSTRING, 0, (LPARAM)Services.aux->serv);
			Services.aux = Services.aux->next;
		}
		Services.aux = Services.origen;
		//DEACTIVATE BUTTONS
		EnableWindow(GetDlgItem(hDlg, BN_EDIT), false);
		EnableWindow(GetDlgItem(hDlg, BTN_DELETES), false);
	}
	case WM_COMMAND:
	{

	
		if (LOWORD(wParam) == BN_OKESP && HIWORD(wParam) == BN_CLICKED) {
			if (IsDlgButtonChecked(hServs, RB_REGIST) == BST_CHECKED) {
				bool fail = true;
				bool OK1 = false, OK2 = false, OK3 = false;
				char tempname[30] = { "" };
				char tempdesc[300] = { "" };
				char tempcost[10] = { "" };
				//validacion de la especialidad y de su descripcion

				//obtener nombre de la especialidad

				SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
				//obtener la descripcion
				SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_GETTEXT, sizeof(tempdesc), (LPARAM)tempdesc);
				//obtener el precio
				SendMessageA(GetDlgItem(hDlg, TB_COST), WM_GETTEXT, sizeof(tempcost), (LPARAM)tempcost);
				//valida el nombre
				int cont = 0;
				int j = 0;
				if (tempname[j] == '\0') {
					MessageBoxA(NULL, "El nombre esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempname[j] != '\0')
					{
						if (((tempname[j] >= 97) && (tempname[j] <= 122)) || ((tempname[j] >= 65) && (tempname[j] <= 90)) || (tempname[j] == 32))
						{
							if (j == 0)
							{
								if ((tempname[j] >= 65) && (tempname[j] <= 90)) {
									tempname[j] = tempname[j];
								}

								else if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j] - 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}
							}
							else if (j > 0)
							{
								if ((tempname[j] >= 97) && (tempname[j] <= 122))
								{
									tempname[j] = tempname[j];
								}
								else if ((tempname[j] >= 65) && (tempname[j] <= 90))
								{
									tempname[j] = tempname[j] + 32;
								}
								else if (tempname[j] == 32) {
									tempname[j] = tempname[j];
								}

							}
							j++;
						}

						else {
							tempname[j] = tempname[j];
							cont = cont + 1;
							j++;
						}

					}
					if (cont > 0) {
						MessageBox(NULL, (LPCWSTR)"El nombre solo debe tener letras", (LPCWSTR)"ERROR", MB_ICONERROR);
						OK2 = false;
					}
					else {
						OK1 = true;
					}
				}
				//valida la descripcion
				int cont1 = 0;
				int k = 0;
				if (tempdesc[k] == '\0') {
					MessageBoxA(NULL, "Descripcion vacía", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempdesc[k] != '\0')
					{
						if (((tempdesc[k] >= 97) && (tempdesc[k] <= 122)) || ((tempdesc[k] >= 65) && (tempdesc[k] <= 90)) || (tempdesc[k] == 32))
						{
							if (k == 0)
							{
								if ((tempdesc[k] >= 65) && (tempdesc[k] <= 90)) {
									tempdesc[k] = tempdesc[k];
								}

								else if ((tempdesc[k] >= 97) && (tempdesc[k] <= 122))
								{
									tempdesc[k] = tempdesc[k] - 32;
								}
								else if (tempdesc[k] == 32) {
									tempdesc[k] = tempdesc[k];
								}
							}
							else if (k > 0)
							{
								if ((tempdesc[k] >= 97) && (tempdesc[k] <= 122))
								{
									tempdesc[k] = tempdesc[k];
								}
								else if ((tempdesc[k] >= 65) && (tempdesc[k] <= 90))
								{
									tempdesc[k] = tempdesc[k] + 32;
								}
								else if (tempdesc[k] == 32) {
									tempdesc[k] = tempdesc[k];
								}

							}
							k++;
						}

						else {
							tempdesc[k] = tempdesc[k];
							cont1 = cont1 + 1;
							k++;
						}

					}
					if (cont1 > 0) {
						MessageBox(NULL, (LPCWSTR)"El nombre solo debe tener letras", (LPCWSTR)"ERROR", MB_ICONERROR);
						OK2 = false;
					}
					else {
						OK2 = true;
					}
				}
				//valida el costo
				int cont2 = 0;
				int point = 0;
				int car2 = 0;
				int large = strlen(tempcost);
				int l = 0;
				if (tempcost[l] == '\0') {
					MessageBoxA(NULL, "El costo esta vacio.", "ERROR", MB_ICONERROR);
				}
				else {
					while (tempcost[l] != '\0') {
						if (isdigit(tempcost[l])) {
							cont2 = cont2 + 1;
						}
						else if (tempcost[l] == 46) {
							point = point + 1;
						}
						else {
							car2 = car2 + 1;
						}
						l++;
					}
					if (car2 > 0) {
						MessageBoxA(hDlg, "El precio no puede contener letras", "Error", MB_ICONERROR);
					}
					else if (large > 0 && large < 10) {
						if (cont2 > 0) {
							if (point < 2) {
								float tmpCost=stof(tempcost);
								if (tmpCost > 0) {
									OK3 = true;
								}
								else {
									MessageBoxA(hDlg, "El precio debe ser mayor a 0", "Error", MB_ICONERROR);
									OK3 = false;
								}
							}
							else if (point > 1) {
								MessageBoxA(hDlg, "El precio contiene mas de un punto decimal", "Error", MB_ICONERROR);
								OK3 = false;
							}
						}
					}
					else {
						MessageBoxA(hDlg, "El precio tiene muchos digitos", "Error", MB_ICONERROR);
						OK3 = false;
					}

				}

			

			if (OK1 && OK2&&OK3)
				fail = false;
			else
				fail = true;

		
					if (!fail) {
						//SI NO FUE EDICION
						if (IsWindowEnabled(GetDlgItem(hServs, BN_EDIT)) == 0) {
							if (!Services.lookDouble(tempname)) {
								//aqui se crea el nodo y todo feliz
								//aqui hay un error, no puedo crear el nodo 
								//por que no es de acceso publico
								Servs::Node* nuevo = Services.creItem(tempname, tempdesc, tempcost);
								Services.inItem(nuevo);
								MessageBoxA(hDlg, "SERVICIO GUARDADO CON EXITO", "ALELUYA", MB_OK);
								SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)"");
								SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)"");
								strcpy(tempname, "");
								strcpy(tempdesc, "");
								strcpy(tempcost, "");


								SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_ADDSTRING, 0, (LPARAM)Services.tail->serv);

							}
							else {
								MessageBoxA(hServs, "Error", "Ya existe ese servicio", MB_ICONERROR);
							}
						}
						//si si fue edición
						else {
							//QUE BUSQUE MEJOR CON LA LISTA
							char lastname[30] = "";
							int selIndexI = (int)SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETCURSEL, 0, 0);
							SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETTEXT, selIndexI, (LPARAM)lastname);
							serFound = Services.findItem(lastname);
							if (serFound != NULL) {
								if (Services.emptySpaceM(serFound)) {
									//si no cambia el nombre
									if (strcmp(serFound->serv, tempname) == 0) {
										Services.editItem(serFound, tempdesc, tempcost);
									}
									//si si cambia el nombre
									else {
										int selindex = 0;
										int items = GetListBoxInfo(GetDlgItem(hDlg, LB_SERV));
										for (selindex = 0; selindex < items; selindex++) {
											char testString[30];
											SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETTEXT, selindex, (LPARAM)testString);
											if (strcmp(serFound->serv,testString)==0) {
												SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_DELETESTRING, selindex, 0);
											}
										}
										Services.editItemS(serFound,tempname, tempdesc, tempcost);
										SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_ADDSTRING, 0, (LPARAM)serFound->serv);
										SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_SETCURSEL, -1, NULL);
									}

									SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
									SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)"");
									SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)"");
									MessageBoxA(NULL, "YA SE EDITO", "ENHORABUENA", MB_OK);

									EnableWindow(GetDlgItem(hDlg, BN_EDIT), false);
									EnableWindow(GetDlgItem(hDlg, BTN_DELETES), false);
									EnableWindow(GetDlgItem(hDlg, LB_SERV), true);
									SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_SETCURSEL, -1, NULL);


								}
								else {
									MessageBoxA(NULL, "NO SE PUEDE EDITAR SI TIENE MECANICOS", "ERROR", MB_ICONERROR);
								}
							}
						}
					}else { //si si fall+o, se indica donde
						
						MessageBoxA(hServs, "EN ALGO TE EQUIVOCASTE", "ERROR", MB_ICONERROR);

					}

			
			}
	
		}
		if (LOWORD(wParam) == BN_EDIT && HIWORD(wParam) == BN_CLICKED) {
			EnableWindow(GetDlgItem(hDlg, TB_NAME), true);
			EnableWindow(GetDlgItem(hDlg, TB_DESC), true);
			EnableWindow(GetDlgItem(hDlg, TB_COST), true);
			EnableWindow(GetDlgItem(hDlg, LB_SERV), false);
			EnableWindow(GetDlgItem(hDlg, BN_OKESP), true);

		}
		if (LOWORD(wParam) == LB_SERV && HIWORD(wParam) == LBN_SELCHANGE) {
			char testServ[50];

			int selIndex = (int)SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETCURSEL, 0, 0);
			if (selIndex == -1) {

			}
			else {
#pragma region Deactivate buttons
				EnableWindow(GetDlgItem(hDlg, TB_NAME), false);
				EnableWindow(GetDlgItem(hDlg, TB_DESC), false);
				EnableWindow(GetDlgItem(hDlg, TB_COST), false);
				//buttons activate
				EnableWindow(GetDlgItem(hDlg, BN_EDIT), true);
				EnableWindow(GetDlgItem(hDlg, BTN_DELETES), true);




#pragma endregion


				SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testServ);
				serFound = Services.findItem(testServ);
				if (serFound != NULL) {
					SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)serFound->serv);
					SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)serFound->desc);
					string costo = to_string(serFound->costSer);
					SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)costo.c_str());
					/*SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_SETCURSEL, -1, NULL);*/
					EnableWindow(GetDlgItem(hDlg, BN_OKESP), false);
					
				}

			}
		}
		if (LOWORD(wParam) == BTN_DELETES && HIWORD(wParam) == BN_CLICKED) {
			char tempname[50] = "";
			SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_GETTEXT, sizeof(tempname), (LPARAM)tempname);
			serFound = Services.findItem(tempname);
			bool empty=Services.emptySpaceM(serFound);
			if (empty) {
				//HAY que borrar del listbox
				int selindex = 0;
				int items = GetListBoxInfo(GetDlgItem(hDlg, LB_SERV));
				for (selindex = 0; selindex < items; selindex++) {
					char testString[30];
					SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_GETTEXT, selindex, (LPARAM)testString);
					if (strcmp(serFound->serv, testString) == 0) {
						SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_DELETESTRING, selindex, 0);
					}
				}
				//ahora si borramos el nodo
				Services.delItem(serFound);
				MessageBoxA(hDlg, "ALELUYA", "ELEMENTO ELIMINADO", MB_OK);
			}
			else {
				MessageBoxA(hDlg, "NO PUEDES ELIMINAR SI HAY MECANICOS REGISTRADOS", "ERROR", MB_ICONERROR);
			}
			char limpia[20] = "";
			SendMessageA(GetDlgItem(hDlg, TB_NAME), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, TB_DESC), WM_SETTEXT, 0, (LPARAM)"");
			SendMessageA(GetDlgItem(hDlg, TB_COST), WM_SETTEXT, 0, (LPARAM)"");
			EnableWindow(GetDlgItem(hDlg, LB_SERV), true);
			SendMessageA(GetDlgItem(hDlg, LB_SERV), LB_SETCURSEL, -1, NULL);
			EnableWindow(GetDlgItem(hDlg, BN_EDIT), false);
			EnableWindow(GetDlgItem(hDlg, BTN_DELETES), false);
		}
		if (LOWORD(wParam) ==BN_CANCELESP && HIWORD(wParam) == BN_CLICKED) {
			ShowWindow(hMain, SW_SHOW);
			EndDialog(hServs, 0);

		}
		
	}
	break;
	case WM_CLOSE:
	{
		ShowWindow(hMain, SW_SHOW);
		EndDialog(hServs, 0);
		break;
	}
	case WM_DESTROY:
	{
	
		break;
	}
	}
	return 0;
}

string validNames(char X[50],bool &OK) {
	int cont = 0;
	int k = 0;
	
		while (X[k] != '\0')
		{
			if (((X[k] >= 97) && (X[k] <= 122)) || ((X[k] >= 65) && (X[k] <= 90)) || (X[k] == 32))
			{
				if (k == 0)
				{
					if ((X[k] >= 65) && (X[k] <= 90)) {
						X[k] = X[k];
					}

					else if ((X[k] >= 97) && (X[k] <= 122))
					{
						X[k] = X[k] - 32;
					}
					else if (X[k] == 32) {
						X[k] = X[k];
					}
				}
				else if (k > 0)
				{
					if ((X[k] >= 97) && (X[k] <= 122))
					{
						X[k] = X[k];
					}
					else if ((X[k] >= 65) && (X[k] <= 90))
					{
						X[k] = X[k] + 32;
					}
					else if (X[k] == 32) {
						X[k] = X[k];
					}

					if (X[k - 1] == 32 && (X[k] >= 65 && X[k] <= 90)) {
						X[k] = X[k];
					}
					else if (X[k - 1] == 32 && (X[k] >= 97 && X[k] <= 122)) {
						X[k] = X[k] - 32;
					}

				}
				k++;
			}

			else {
				X[k] = X[k];
				cont = cont + 1;
				k++;
			}

		}
		if (cont > 0) {
			MessageBoxA(NULL, "Solo debe tener letras", "ERROR", MB_ICONERROR);
			OK = false;
		}
		else {
			OK = true;
		}
	
	return X;
}
bool validDate(char fecha[],char hora[]) {
	string date(fecha);
	int n = 0;
	n = date.find(",");
	string diaSem = date.substr(0, n);
	date=date.substr(n + 2, date.length());
	n = date.find(" ");
	string diaMes = date.substr(0, n);
	date = date.substr(n + 4, date.length());
	n = date.find(" ");
	string mes = date.substr(0, n);
	date = date.substr(n + 4, date.length());
	string year = date;
	//-------------para cambiar la hora
	int horaP = 0;
	if (strcmp(hora, "8AM-12PM") == 0) {
		horaP = 8;
	}
	else if (strcmp(hora, "12PM-4PM") == 0) {
		horaP = 12;
	}
	else if (strcmp(hora, "4PM-8PM") == 0) {
		horaP = 16;
	}
	
	char sat[12] = "sábado";
	char sun[12] = "domingo";
	//si es fin de semana vale queso
	if (strcmp(sat,diaSem.c_str())==0) {
		return false;
	}
	else if (strcmp(sun, diaSem.c_str()) == 0) {
	return false;
	}else {
		//mas validaciones pendientes
		int rYear = _actualTime->tm_year + 1900;
		int rMonth = _actualTime->tm_mon + 1;
		int rDay = _actualTime->tm_mday;
		int rHour = _actualTime->tm_hour;
		char yearC[5] = "";
		strcpy(yearC, year.c_str());
		int year = atoi(yearC);
		//el mes esta en letras
		int mesR = 0;
		if (mes.compare("enero") == 0) {
			mesR = 1;
		}
		else if (mes.compare("febrero") == 0){
			mesR = 2;
		}
		else if (mes.compare("marzo") == 0) {
			mesR = 3;
		}
		else if (mes.compare("abril") == 0) {
			mesR = 4;
		}
		else if (mes.compare("mayo") == 0) {
			mesR = 5;
		}
		else if (mes.compare("junio") == 0) {
			mesR = 6;
		}
		else if (mes.compare("julio") == 0) {
			mesR = 7;
		}
		else if (mes.compare("agosto") == 0) {
			mesR = 8;
		}
		else if (mes.compare("septiembre") == 0) {
			mesR = 9;
		}
		else if (mes.compare("octubre") == 0) {
			mesR = 10;
		}
		else if (mes.compare("noviembre") == 0) {
			mesR = 11;
		}
		else if (mes.compare("diciembre") == 0) {
			mesR = 12;
		}
		
		char dayC[3] = "";
		strcpy(dayC, diaMes.c_str());
		int dia = atoi(dayC);
		if (year >= rYear && year < rYear + 2) {
			//si es para el siguiente año, que todo se pueda hacer
			if (year > rYear) {
				return true;
			}//puede ser el mismo mes o despues
			//puede ser el mismo mes o despues
			if (mesR >= rMonth) {
				//puede ser el mismo dia o despues
				if (dia >= rDay) {
					//DEBE SER MAYOR A LA HORA ACTUAL
					//No puede ser a la misma hora bc impuntual
					if (dia == rDay) {
						if (horaP > rHour) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return true;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}

	}
}
bool validAttend(char fecha[], char hora[],bool attend) {
	string date(fecha);
	int n = 0;
	n = date.find(",");
	string diaSem = date.substr(0, n);
	date = date.substr(n + 2, date.length());
	n = date.find(" ");
	string diaMes = date.substr(0, n);
	date = date.substr(n + 4, date.length());
	n = date.find(" ");
	string mes = date.substr(0, n);
	date = date.substr(n + 4, date.length());
	string year = date;
	//-------------para cambiar la hora
	int horaP = 0;
	if (strcmp(hora, "8AM-12PM") == 0) {
		horaP = 8;
	}
	else if (strcmp(hora, "12PM-4PM") == 0) {
		horaP = 12;
	}
	else if (strcmp(hora, "4PM-8PM") == 0) {
		horaP = 16;
	}

	//si es fin de semana vale queso
	if (diaSem.compare("sábado") == 0 || diaSem.compare("domingo") == 0) {
		return false;
	}//si es entre semana ------
	else {
		//mas validaciones pendientes
		int rYear = _actualTime->tm_year + 1900;
		int rMonth = _actualTime->tm_mon + 1;
		int rDay = _actualTime->tm_mday;
		int rHour = _actualTime->tm_hour;
		char yearC[5] = "";
		strcpy(yearC, year.c_str());
		int year = atoi(yearC);
		//el mes esta en letras
		int mesR = 0;
		if (mes.compare("enero") == 0) {
			mesR = 1;
		}
		else if (mes.compare("febrero") == 0) {
			mesR = 2;
		}
		else if (mes.compare("marzo") == 0) {
			mesR = 3;
		}
		else if (mes.compare("abril") == 0) {
			mesR = 4;
		}
		else if (mes.compare("mayo") == 0) {
			mesR = 5;
		}
		else if (mes.compare("junio") == 0) {
			mesR = 6;
		}
		else if (mes.compare("julio") == 0) {
			mesR = 7;
		}
		else if (mes.compare("agosto") == 0) {
			mesR = 8;
		}
		else if (mes.compare("septiembre") == 0) {
			mesR = 9;
		}
		else if (mes.compare("octubre") == 0) {
			mesR = 10;
		}
		else if (mes.compare("noviembre") == 0) {
			mesR = 11;
		}
		else if (mes.compare("diciembre") == 0) {
			mesR = 12;
		}

		char dayC[3] = "";
		strcpy(dayC, diaMes.c_str());
		int dia = atoi(dayC);
		// cita     chequeo
		if (attend) {
			if (year <= rYear && year < rYear + 2) {
				//no puedes darle atendido a una cita que no se ha llevado a cabo del sig año
				//puede ser el mismo mes o despues
				if (mesR <= rMonth) {
					//no se puede atender una cita del proximo mes
					//no se puede atender una cita del proximo dia
					if (dia <= rDay) {
						//DEBE SER MAYOR A LA HORA ACTUAL
						//Para darle atendido a una cita debe ser a la misma hora o despues
						if (horaP <= rHour) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}
string shortDateChanger(char fecha[]) {
	string date(fecha);
	int n = 0;
	n = date.find(",");
	string diaSem = date.substr(0, n);
	date = date.substr(n + 2, date.length());
	n = date.find(" ");
	string diaMes = date.substr(0, n);
	date = date.substr(n + 4, date.length());
	n = date.find(" ");
	string mes = date.substr(0, n);
	date = date.substr(n + 4, date.length());
	string year = date;

	string mesR = "";
	if (mes.compare("enero") == 0) {
		mesR = "01";
	}
	else if (mes.compare("febrero") == 0) {
		mesR = "02";
	}
	else if (mes.compare("marzo") == 0) {
		mesR = "03";
	}
	else if (mes.compare("abril") == 0) {
		mesR ="04";
	}
	else if (mes.compare("mayo") == 0) {
		mesR = "05";
	}
	else if (mes.compare("junio") == 0) {
		mesR = "06";
	}
	else if (mes.compare("julio") == 0) {
		mesR = "07";
	}
	else if (mes.compare("agosto") == 0) {
		mesR = "08";
	}
	else if (mes.compare("septiembre") == 0) {
		mesR = "09";
	}
	else if (mes.compare("octubre") == 0) {
		mesR = "10";
	}
	else if (mes.compare("noviembre") == 0) {
		mesR = "11";
	}
	else if (mes.compare("diciembre") == 0) {
		mesR = "12";
	}

	string myShortDate = diaMes+"/"+mesR+"/"+year;
	return myShortDate;
	
}
void txtReport( char nombredeArchivo[],HWND listbox,HWND hDlg, int option) {
	ofstream reportArch;
	string name="Data\\";
	name = name + nombredeArchivo;

	char testStringR[12];
	int selIndex = 0;
	int items = GetListBoxInfo(listbox);

	reportArch.open(name.c_str(), ios::out | ios::trunc );
	if (!reportArch.is_open()) {
		MessageBoxA(NULL, "No se creo el archivo", "error", MB_ICONASTERISK);
	}
	else {
		while (selIndex < items) {
			//opcion 1 es para reporte de cITAS en archivo de texto
			//REVISAAAARRRRRR UWU
			if (option == 1) {

				SendMessageA(listbox, LB_GETTEXT, selIndex, (LPARAM)testStringR);
				
				Cits::Node* move = Citas.findItem(testStringR);

				if (move != NULL) {
					reportArch << "Folio: " << move->ID << endl;
					reportArch << "Servicio: " << move->specialC << endl;
					reportArch << "Mecánico: " << move->mechName << endl;
					reportArch << "Nombre del cliente: " << move->clientName << endl;
					reportArch << "Placas del auto: " << move->placas << endl;
					reportArch << "Número del cliente: " << move->clientTel << endl;
					reportArch << "Fecha de la cita: " << move->shortDate << endl;
					reportArch << "Hora: " << move->shortHour << endl;
					if (move->attend) {
						reportArch << "Atendido: Si"<<endl;
					}
					else {
						if (move->canceled) {
							reportArch << "Cancelado: Si"<<endl;
						}
						else {
							reportArch << "Cancelado: No"<<endl;
						}
						reportArch << "Atendido: No"<<endl;
					}
					reportArch << "--------------------------------" << endl;
				}
				
			}
			//opcion 2 es para reporte de clientes por apellido
			else if (option == 2) { 
				SendMessageA(listbox, LB_GETTEXT, selIndex, (LPARAM)testStringR);
				Clts::Node* move = Clients.findItemP(testStringR);
				if (move != NULL) {
					
					reportArch << "Nombre: " << move->nombre << " " << move->apellido << endl;
					reportArch << "Telefono: " << move->telefono << endl;
					reportArch << "Placas del auto: " << move->placas << endl;
					reportArch << "Modelo: " << move->modelo << endl;
					reportArch << "Marca: " << move->marca<< endl;
					reportArch << "Año: " << move->year << endl;
					reportArch << "Kilometraje: " << move->kilometraje << endl;
					reportArch << "Dirección completa: " << move->direccion << endl;
					reportArch << "--------------------------------" << endl;
				}
			}
			//opcion 3 es para reporte de mecánicos por nombre
			else if (option == 3) {
				char testStringM[50];
				SendMessageA(listbox, LB_GETTEXT, selIndex, (LPARAM)testStringM);
				Mechs::Nodo* move = Mechanics->findItem(testStringM);
				if (move != NULL) {
					reportArch << "Nombre: " << move->name << endl;
					reportArch << "Nomina: " << move->idEmp << endl;
					reportArch << "Servicio: " << move->occupation << endl;
					reportArch << "Telefono: " << move->telephone << endl;
					char horario[20];
					if (move->empSched.hourSche == 1) {
						strcpy(horario, "8AM-12PM");
					}
					else if (move->empSched.hourSche == 2) {
						strcpy(horario, "12PM-4PM");
					}
					else if (move->empSched.hourSche == 3) {
						strcpy(horario, "4PM-8PM");
					}
					reportArch << "Horario : LUNES A VIERNES  de "<<horario << endl;
					reportArch << "--------------------------------" << endl;

				}
			}
			selIndex++;
		}
		selIndex = 0;
		
	}
	reportArch.close();

	MessageBoxA(hDlg, "ARCHIVO CREADO", name.c_str(), MB_OK);
}