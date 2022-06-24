#ifndef  _clases_
#define _clases_


#include"mechs.h"

int folioBase = 100000;
int folioCita = 800000;




class Servs
{
public:

	struct Node
	{
		int ID=0;
		char serv[30] = {};
		char desc[300];
		float costSer=0;
		struct sched {
			int weekSche=0;
			int hourSche=0;
		}ConEsp[3];
		Node* next;
		Node* prev;
		Mechs::Nodo* mechs[3] = { 0,0,0};
		}*lista,*origen,*tail ,* aux;
	ifstream servArchive;
	ofstream servArchiveO;

	//-------------------------------------------------
	Servs()
	{
		lista=tail=origen = aux = NULL;
		//aqui se cargan las especialidades del archivo binario
		readArchive();
	}
	Node *creItem(char esps[],char desc[],char cost[]) {
		Node* nuevo_nodo = new Node;
		strcpy_s(nuevo_nodo->serv, esps);
		strcpy_s(nuevo_nodo->desc, desc);
		nuevo_nodo->costSer = stof(cost);

		nuevo_nodo->ID = folioBase;
		folioBase++;

		//agregar el horario semanal 1=lun a viernes, 2 el sabado
		for (int i = 0; i < 3; i++) {
			nuevo_nodo->ConEsp[i].weekSche = 1;
			nuevo_nodo->ConEsp[i].hourSche = 0;
		}
		//como recien se creo la eservialidad, los horarios estan vacios
		//cuando se llenen 1 es de 8am -12pm , 2 es de 12-4 pm y 3 es de 4-8pm
		

		nuevo_nodo->prev = NULL;
		nuevo_nodo->next = NULL;

		//nomas va a haber 1 espacio de trabajo por cada especialidad, nada de 2 mecanicos
		//con la misma especialidad, atendiendo el mismo dia y horario, en lados diferentes
		//no caben
		return nuevo_nodo;
	}
	//revisar el edit, a lo mejor esta de mas
	void editItem(Node*& edit, char desc[], char cost[]) {
		strcpy(edit->desc, desc);
		edit->costSer = stof(cost);

	}
	void editItemS(Node*& edit, char esps[], char desc[], char cost[]) {
		strcpy(edit->desc, desc);
		edit->costSer = stof(cost);

		//buscar a los mecanicos
		for (int i = 0; i < 3; i++) {
			if (edit->mechs[i] != NULL) {
				strcpy(edit->mechs[i]->occupation,esps);
			}
		}

		//buscar las citas con esa especialidad
		//no me conviene hacer esto a menos que la clase citas este en otro archivo

		//Cits::Node* change = Citas.origen;
		//if (change != NULL) {
		//	while (change != NULL) {
		//		if (strcmp(change->special, edit->serv) == 0) {
		//			strcpy(change->special, esps);
		//		}
		//		change = change->next;
		//	}
		//}
		//delete change;

		//ahora si cambiamos el nombre del servicio
		strcpy(edit->serv, esps);
	}
	bool addMechs(Mechs::Nodo*&mechs, Servs::Node*& ptr,int x) {
		if (freeSpace(ptr,x)) {
			for (int i = 0; i < 3; i++) {
				if (x == i) {
					ptr->mechs[i] = mechs;
					return true;
				}
			}
		}
		else {
			return false;
		}
	}
	void deleteMechs(Mechs::Nodo*& mechs, Servs::Node*& ptr,int x) {
		for (int i =0 ; i < 3; i++) {
			if (x == i && ptr->mechs[i] != NULL) {
				ptr->mechs[i] = NULL;
				break;
			}
		}
	}
	bool freeSpace(Servs::Node*&var, int x) {
		if (var->mechs[x] == NULL) {
			return true;
		}
		return false;
	}
	bool fullSpace(Servs::Node*& var) {
		int cont = 0;
		for (int i = 0; i < 3; i++) {
			if (var->mechs[i]!=NULL) {
				cont++;
			}
		}
		if (cont == 3) {
			return true;
		}
		else {
			return false;
		}
	}
	bool emptySpaceM(Servs::Node*& var) {
		int cont = 0;
		for (int i = 0; i < 3; i++) {
			if (var->mechs[i] == NULL) {
				cont++;
			}
		}
		if (cont == 3) {
			return true;
		}
		else {
			return false;
		}
	}
	void inItem(Node *&nuevo)
	{
	
		aux = origen;
		if (origen == NULL)
		{
			nuevo->next = NULL;
			this->origen = nuevo;
			this->tail = nuevo;
			this->lista = origen;
		}
		else
		{
			//esto es por si quiero insertar ordenado por nombre

//#pragma region Ordenamiento
//			while (aux->next != NULL && nuevo->serv[0] <= aux->serv[0]) {
//
//				aux = aux->next;
//			}
//			//si es el primer elemento
//			if (aux = origen) {
//				nuevo->prev = NULL;
//				nuevo->next = aux;
//				aux->prev = nuevo;
//				origen = nuevo;
//			}
//			//caso 2: nada de la lista es mayor al nuevo nodo. 
//			else if (aux->next == NULL) {
//				nuevo->prev = aux;
//				nuevo->next = NULL;
//				aux->next = nuevo;
//			}
//			else {
//				//caso 3: Hay algo mayor que el nuevo nodo y se coloca en medio
//				nuevo->prev = aux->prev;
//				nuevo->next = aux->next;
//				aux->next->prev = nuevo;
//				aux->prev->next = nuevo;
//			}
//#pragma endregion

			//insertar todo al final de la lista
			this->tail->next= nuevo;
			nuevo->prev =this-> tail;
			nuevo->next = NULL;
			this->tail = nuevo;
		}
	}
	bool lookDouble(char servName[]) {
		if (!ServsIsEmpty()) {
			aux = origen;
			while (aux!= NULL) {
				if (strcmp(aux->serv, servName) == 0) {
					return true;
				}
					aux = aux->next;
			}
			return false;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return false;
		}
		
	}
	Node *findItem(char servName[]) {
		if (!ServsIsEmpty()) {
			aux = origen;
			while (aux!= NULL) {
				if (strcmp(aux->serv, servName) == 0) {
					return aux;
				}
					aux = aux->next;
			}
			return NULL;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return NULL;
		}
	}
	bool ServsIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	string returnMechName(Servs::Node*& var,int i) {
		if (var->mechs[i] != NULL) {
			return var->mechs[i]->name;
		}
		else
			return "";
	}
	void writeArchive() {
		servArchiveO.open("Data\\servs.bin", ios::out | ios::trunc | ios::binary);
		if (!servArchiveO.is_open()) {
			MessageBoxA(NULL, "No se GUARDO SERVICIOS", "error", MB_ICONASTERISK);
		}else {
			Node* temp = origen;
			while (temp != NULL) {
				servArchiveO.write(reinterpret_cast<char*> (temp), sizeof(Node));
				temp = temp->next;
			}
			MessageBoxA(NULL, "Guardado de servicios exitoso", "ok", MB_OK);
		}
		servArchiveO.close();
		
	}
	void readArchive() {
		servArchive.open("Data\\servs.bin", ios::in | ios::binary | ios::ate);
		if (!servArchive.is_open()) {
			aux = origen = NULL;
			MessageBoxA(NULL, "No se cargo el archivo", "error", MB_ICONASTERISK);
			servArchive.close();
		}
		else {
			int size = servArchive.tellg();
			if (size == 0) {
				MessageBoxA(NULL, "Archivo de citas vacio", "error", MB_ICONASTERISK);

			}
			for (unsigned int i = 0; i < (size / sizeof(Node)); i++) {

				if (origen == NULL) {
					origen = new Node;
					aux = origen;
					aux->prev = NULL;
					lista = origen;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Node;
					aux->next->prev = aux;
					aux = aux->next;
				}

				Node* Temp = new Node;
				servArchive.seekg(i * sizeof(Node));
				servArchive.read(reinterpret_cast<char*>(Temp), sizeof(Node));


				aux->ID = Temp->ID;
				for (int f = 0; f < 4; f++) {
					aux->ConEsp[f] = Temp->ConEsp[f];
				}
				aux->costSer = Temp->costSer;
				strcpy(aux->desc, Temp->desc);
				for (int f = 0; f < 4; f++) {
					aux->mechs[f] = Temp->mechs[f];
				}
				strcpy(aux->serv, Temp->serv);

				delete reinterpret_cast<char*>(Temp);

				aux->next = NULL;
				aux = origen;
			}
			servArchive.close();
			//aqui hay que reemplazar el global id con el ultimo id
			if (origen != NULL) {
				aux = origen;
				while (aux != NULL) {
					if (aux->next == NULL) {
						folioBase = aux->ID;
						folioBase++;
					}
					aux = aux->next;
				}

				aux = origen;
				while (aux->next != NULL) {
					aux = aux->next;
				}
				this->tail = aux;
				aux = origen;
				lista = origen;
			}
		/*	MessageBoxA(NULL, "Cargado de servicios exitoso", "OK", MB_OK);*/
		}
	}
	void delItem(Node*& var)
	{
		Node* auxD = var;

		if ((auxD->prev == NULL) && (auxD->next == NULL)) /*Caso de unico elemento*/ {
			delete auxD;
			origen = NULL;
			lista = NULL;
			tail = NULL;
		}
		else if (auxD->next == NULL) /*Caso de ultimo elemento*/ {
			this->tail = auxD->prev;
			this->tail->next = NULL;
			delete auxD;
		}
		else if (auxD->prev == NULL) /*Caso de primer elemento*/ {
			auxD->next->prev = NULL;
			origen = auxD->next;
			lista = origen;
			delete aux;
		}
		else { /*cualquier otro*/
			auxD->next->prev = auxD->prev;
			auxD->prev->next = auxD->next;
			delete auxD;
		}
	}
	~Servs()
	{
		writeArchive();
		aux = lista;
		while (aux!= NULL) {
			Node* borr = aux;
			aux = aux->next;
			delete borr;
		}
	}
}Services;

class Clts {
public:
	struct Node {
		char placas[12];
		char nombre[30];
		char apellido[30];
		char reverseName[80];
		char telefono[13];
		char marca[30];
		char modelo[30];
		char year[6];
		char kilometraje[50];
		char direccion[250];
		Node* next = NULL;
		Node* prev = NULL;

	}*lista, * origen, * tail, * aux;
	ifstream clntArchv;
	ofstream clntOArchv;
	//-------------------------------
	Clts(){
		lista = tail = origen = aux = NULL;
		//aqui se cargan las especialidades del archivo binario
		readArchive();
	}
	Node* creItem(char name[], char apellido[],char placas[], char marca[], char modelo[], 
		char year[], char km[], char tel[],
		char calle[], char num[], char col[], 
		char mun[], char estado[]){
		Node* nuevo_nodo = new Node;
		strcpy_s(nuevo_nodo->nombre, name);
		strcpy_s(nuevo_nodo->apellido, apellido);
		strcpy_s(nuevo_nodo->placas, placas);
		strcpy_s(nuevo_nodo->marca, marca);
		strcpy_s(nuevo_nodo->modelo, modelo);
		strcpy_s(nuevo_nodo->year, year);
		strcpy_s(nuevo_nodo->kilometraje, km);
		strcpy_s(nuevo_nodo->telefono, tel);

		string reverse;
		reverse = nuevo_nodo->apellido;
		reverse.append(" ");
		reverse.append(nuevo_nodo->nombre);
		reverse.append(" ");
		reverse.append(nuevo_nodo->placas);

		strcpy(nuevo_nodo->reverseName, reverse.c_str());

		char  direccion[250]="";
		strcpy(direccion, calle);
		strcat(direccion, "_");
		strcat(direccion, num);
		strcat(direccion, "_");
		strcat(direccion, col);
		strcat(direccion, "_");
		strcat(direccion, mun);
		strcat(direccion, "_");
		strcat(direccion, estado);

		strcpy(nuevo_nodo->direccion, direccion);


		nuevo_nodo->prev = NULL;
		nuevo_nodo->next = NULL;


		return nuevo_nodo;

	}
	void editItem(Node*&edit, char name[], char apellido[],char placas[], char marca[], char modelo[],
		char year[], char km[], char tel[],char dir[]) {
		strcpy(edit->nombre, name);
		strcpy(edit->apellido, apellido);
		strcpy(edit->direccion, dir);
		strcpy(edit->marca, marca);
		strcpy(edit->modelo, modelo);
		strcpy(edit->year, year);
		strcpy(edit->placas, placas);
		strcpy(edit->telefono, tel);
		strcpy(edit->kilometraje, km);
		

		string reverse;
		reverse = edit->apellido;
		reverse.append(" ");
		reverse.append(edit->nombre);
		reverse.append(" ");
		reverse.append(edit->placas);

		strcpy(edit->reverseName, reverse.c_str());


	}
	void inItem(Node*& nuevo) {
		aux = origen;
		if (origen == NULL)
		{
			nuevo->next = NULL;
			this->origen = nuevo;
			this->tail = nuevo;
			this->lista = origen;
		}
		else {
			this->tail->next = nuevo;
			nuevo->prev = this->tail;
			nuevo->next = NULL;
			this->tail = nuevo;
		}
	}
	bool lookDouble(char placas[]) {
		if (!ClientIsEmpty()) {
			aux = origen;
			while (aux!= NULL) {
				if (strcmp(aux->placas, placas) == 0) {
					return true;
				}
				aux = aux->next;
			}
			return false;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return false;
		}

	}
	Node* findItem(char name[], char placas[], int opt) {
		
		if (!ClientIsEmpty()) {
			aux = origen;
			while (aux != NULL) {
				if (opt == 1) {
					if (strcmp(aux->nombre, name) == 0) {
						return aux;
					}
					else {
						aux = aux->next;
					}
				}
				else if (opt == 2) {
					if (strcmp(aux->placas, placas) == 0) {
						return aux;
					}
					else {
						aux = aux->next;
					}
				}
			}
			return NULL;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return NULL;
		}
	}
	Node* findItemP(char placas[]){
		if (!ClientIsEmpty()) {
			aux = origen;
			while (aux != NULL) {
				
				if (strcmp(aux->placas, placas) == 0) {
						return aux;
				}
				else {
					aux = aux->next;
				}
				
			}
			return NULL;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return NULL;
		}
	}
	void delItem(Node*& var)
	{
		Node* auxD = var;

		if ((auxD->prev == NULL) && (auxD->next == NULL)) /*Caso de unico elemento*/ {
			delete auxD;
			origen = NULL;
			lista = NULL;
			tail = NULL;
		}
		else if (auxD->next == NULL) /*Caso de ultimo elemento*/ {
			this->tail = auxD->prev;
			this->tail->next = NULL;
			delete auxD;
		}
		else if (auxD->prev == NULL) /*Caso de primer elemento*/ {
			auxD->next->prev = NULL;
			origen = auxD->next;
			lista = origen;
			delete aux;
		}
		else { /*cualquier otro*/
			auxD->next->prev = auxD->prev;
			auxD->prev->next = auxD->next;
			delete auxD;
		}
	}
	bool ClientIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	//-------------heapsort
	int sizeofList(Node*origen) {
		if (origen != NULL) {
			aux = origen;
			int cont = 0;
			while (aux != NULL) {
				cont++;
				aux = aux->next;
			}
			return cont;
		}
		return 0;
	}
	void AddtoArray(Node* origen, string arr[],int i) {
		if (origen != NULL) {
			aux = origen;
			while (aux != NULL) {
				arr[i] = aux->reverseName;
				i++;
				aux = aux->next;
			}
		}
	}
	//nuestro array debe tener un espacio extra
	void heapify(string* array, int iterationSize) {
		int i, par, node;
		for (i = 1; i <= iterationSize; i++) {
			node = i; par = (int)node / 2;
		

			while (par >= 1) {
			

				if (array[par] < array[node])
					swap(array[par], array[node]);

				node = par;
				par = (int)node / 2;

				
			}
		}
	}
	//al llamarla creamos el arreglo y su tamaño tambien y ya lo agregamos al LB 
	//e imprimimos en el archivo binario
	void heapSort(string* array, int size) {
		for (int i = size; i >= 1; i--) {

			heapify(array, i);

			swap(array[1], array[i]);
		}
	}
	
	//----------guardar en archivos binarios
	void readArchive() {
		clntArchv.open("Data\\clients.bin", ios::in | ios::binary | ios::ate);
		if (!clntArchv.is_open()) {
			aux = origen = NULL;
			MessageBoxA(NULL, "No se ABRIO EL ARCHIVO CLIENTES", "error", MB_ICONASTERISK);
			clntArchv.close();
		}
		else {
			int size = clntArchv.tellg();
			if (size == 0) {
				MessageBoxA(NULL, "Archivo de clientes vacio", "error", MB_ICONASTERISK);

			}
			for (unsigned int i = 0; i < (size / sizeof(Node)); i++) {

				if (origen == NULL) {
					origen = new Node;
					aux = origen;
					aux->prev = NULL;
					lista = origen;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Node;
					aux->next->prev = aux;
					aux = aux->next;
				}

				Node* Temp = new Node;
				clntArchv.seekg(i * sizeof(Node));
				clntArchv.read(reinterpret_cast<char*>(Temp), sizeof(Node));


				
				strcpy(aux->nombre, Temp->nombre);
				strcpy(aux->apellido, Temp->apellido);
				strcpy(aux->reverseName, Temp->reverseName);
				strcpy(aux->placas, Temp->placas);
				strcpy(aux->marca, Temp->marca);
				strcpy(aux->modelo, Temp->modelo);
				strcpy(aux->year, Temp->year);
				strcpy(aux->kilometraje, Temp->kilometraje);
				strcpy(aux->telefono, Temp->telefono);
				strcpy(aux->direccion, Temp->direccion);

				delete reinterpret_cast<char*>(Temp);

				aux->next = NULL;
				aux = origen;
			}
			clntArchv.close();
			if (origen != NULL) {
				while (aux->next != NULL) {
					aux = aux->next;
				}
				this->tail = aux;
				aux = origen;
				lista = origen;
			}
			MessageBoxA(NULL, "Cargado de clientes exitoso", "OK", MB_OK);
		}
	}
	void writeArchive() {
		clntOArchv.open("Data\\clients.bin", ios::out | ios::trunc | ios::binary);
		if (!clntOArchv.is_open()) {
			MessageBoxA(NULL, "No se GUARDO el archivo CLIENTES", "error", MB_ICONASTERISK);
		}
		else {
			Node* temp = origen;
			while (temp != NULL) {
				clntOArchv.write(reinterpret_cast<char*> (temp), sizeof(Node));
				temp = temp->next;
			}
			MessageBoxA(NULL, "Guardado de clientes exitoso", "ok", MB_OK);
		}
		clntOArchv.close();
		
	}

	~Clts() {
		writeArchive();
		aux = lista;
		while (aux != NULL) {
			Node* borr = aux;
			aux = aux->next;
			delete borr;
		}
	}
	
}Clients;

class Cits {
public:
	struct Node {
		int ID = 0;
		struct fecha {
			int dia;
			int mes;
			int año;
			int hour;
		}date;
		char shortHour[20];
		char shortDate[11];
		char specialC[30];
		char mechName[50];
		char clientName[50];
		char clientTel[13];
		char placas[12];
		bool attend = false;
		bool canceled = false;
		char cancelMotive[250];
		Node* next = NULL;
		Node* prev = NULL;
	}*lista, * origen, * tail, * aux;

	ifstream CitasArchv;
	ofstream CitasOArchv;
	//--------------------
	Cits() {
		lista = tail = origen = aux = NULL;
		//aqui se cargan las especialidades del archivo binario
		readArchive();
	}

	bool inItem(Node*& nuevo)
	{

		aux = origen;
		//si es el primerisimo primer Y UNICO elemento
		if (origen == NULL)
		{
			nuevo->next = NULL;
			this->origen = nuevo;
			this->tail = nuevo;
			this->lista = origen;
			return true;
		}
		else
		{
			//esto es por si quiero insertar ordenado por fecha uwu
			bool repetido = false;
			bool inserted = false;
#pragma region Ordenamiento
			while (aux->next != NULL) {
				//comprobamos la especialidad, debe ser la misma para funcionar, avanza hasta encontrarla o no
				if (strcmp(nuevo->specialC, aux->specialC) == 0) {
					//comprobamos el mecanico. Debe ser el mismo. Si no existe todavia, verificar
					if (strcmp(nuevo->mechName, aux->mechName) == 0) {
						//si el año es igual, compara lo siguiente
						if (date_compare(nuevo, aux, 1) == 0) {
							//si el mes es igual, compara lo siguiente
							if (date_compare(nuevo, aux, 2) == 0) {
								//si el dia es igual, compara lo siguiente
								if (date_compare(nuevo, aux, 3) == 0) {
									//si la hora es igual que lo dudo, a lo mejor inserta *guiño**guiño*
									if (date_compare(nuevo, aux, 4) == 0) {
										//tas mal mija, no hay repetidos, pero por si las dudas
										repetido = true;
										break;
									}
									else if (date_compare(nuevo, aux, 4) == -1) {
										//si la hora es menor, inserta ahi mismo ANTES DE AUX
										if (aux == origen) {
											origen = nuevo;

											nuevo->next = aux;
											nuevo->prev = NULL;
											aux->prev = nuevo;
											inserted = true;
										}
										else {
											nuevo->next = aux;
											nuevo->prev = aux->prev;
											aux->prev->next = nuevo;
											aux->prev = nuevo;
											inserted = true;
											break;
										}
									}
									else {
										//si la hora es mayor, debe insertar justo despues de ese nodo
										//verificar que el nodo siguiente de aux no sea nulo
										//y que sea exactamente igual pero con hora diferente y si es mayor o menor
										if (aux->next != NULL) {
											if (strcmp(nuevo->specialC, aux->next->specialC) == 0&&
												strcmp(nuevo->mechName, aux->next->mechName) == 0&&
												date_compare(nuevo, aux->next, 1) == 0 &&
												date_compare(nuevo, aux->next, 2) == 0&&
												date_compare(nuevo, aux->next, 3) == 0) {
												if (date_compare(nuevo, aux->next, 4) == 0) {
													//esto no existe jajaja pero por si las dudas
													repetido = true;
												}//es menor, se inserta antes
												else if (date_compare(nuevo, aux->next, 1) == -1) {
													if (aux == origen) {
														origen = nuevo;
														nuevo->next = aux;
														nuevo->prev = NULL;
														aux->prev = nuevo;
														inserted = true;
													}
													else {
														nuevo->next = aux;
														nuevo->prev = aux->prev;
														aux->prev->next = nuevo;
														aux->prev = nuevo;
														inserted = true;
														break;
													}
												}//es mayor, se inserta despues
												else {
													nuevo->next = aux->next;
													nuevo->prev = aux;
													aux->next->prev = nuevo;
													aux->next = nuevo;
													inserted = true;
													break;
												}
											}//si ya el siguiente elemento es diferente en cualquier cosa
											else {
												nuevo->next = aux->next;
												nuevo->prev = aux;
												aux->next->prev = nuevo;
												aux->next = nuevo;
												inserted = true;
												break;
											}
										}
										
									}
								}
								else if (date_compare(nuevo, aux, 3) == -1) {
									//si el dia es menor, inserta ahi mismo
									if (aux == origen) {
										origen = nuevo;
										nuevo->next = aux;
										nuevo->prev = NULL;
										aux->prev = nuevo;
										inserted = true;
									}
									else {
									nuevo->next = aux;
									nuevo->prev = aux->prev;
									aux->prev->next = nuevo;
									aux->prev = nuevo;
									inserted = true;
									break;
									}
								}//si el dia es mayor, avanza

							}
							else if (date_compare(nuevo, aux, 2) == -1) {
								//si el mes es menor, inserta ahi mismo
								if (aux == origen) {
									origen = nuevo;
									nuevo->next = aux;
									nuevo->prev = NULL;
									aux->prev = nuevo;
									inserted = true;
								}
								else {
									nuevo->next = aux;
									nuevo->prev = aux->prev;
									aux->prev->next = nuevo;
									aux->prev = nuevo;
									inserted = true;
									break;
								}
							}//si el mes es mayor avanza
						}
						else if (date_compare(nuevo, aux, 1) == -1) {
							//si el año es menor, inserta ahi mismo
							if (aux == origen) {
								//SI ES EL NODO ORIGEN EL PROCESO ES DIFERENTE
								origen = nuevo;
								nuevo->next = aux;
								nuevo->prev = NULL;
								aux->prev = nuevo;
								inserted = true;
							}
							else {
								nuevo->next = aux;
								nuevo->prev = aux->prev;
								aux->prev->next = nuevo;
								aux->prev = nuevo;
								inserted = true;
								break;
							}
						}
						//si el año es mayor, avanza
					}
					else {
						//verificamos que su siguiente no sea nulo
						if (aux->next != NULL) {
							//si todavia el siguiente nodo tiene la misma especialidad, avanza, pero si no, SE INSERTA YA
							if (strcmp(nuevo->specialC, aux->next->specialC) != 0) {
								//aqui se debe insertar el nodo justo DESPUES
								nuevo->next = aux->next;
								nuevo->prev = aux;
								aux->next->prev = nuevo;
								aux->next = nuevo;
								inserted = true;
								break;
							}
						}
					}
				}
			
			
				aux = aux->next;
				//para revisar el ultimo elemento
				if (aux->next == NULL) {
					if (strcmp(nuevo->specialC, aux->specialC) == 0) {
						//comprobamos el mecanico. Debe ser el mismo. Si no existe todavia, verificar
						if (strcmp(nuevo->mechName, aux->mechName) == 0) {
							//si el año es igual, compara lo siguiente
							if (date_compare(nuevo, aux, 1) == 0) {
								//si el mes es igual, compara lo siguiente
								if (date_compare(nuevo, aux, 2) == 0) {
									//si el dia es igual, compara lo siguiente
									if (date_compare(nuevo, aux, 3) == 0) {
										//si la hora es igual que lo dudo, a lo mejor inserta *guiño**guiño*
										if (date_compare(nuevo, aux, 4) == 0) {
											//tas mal mija, no hay repetidos, pero por si las dudas
											repetido = true;
											break;
										}
										else if (date_compare(nuevo, aux, 4) == -1) {
											//si la hora es menor, inserta ahi mismo ANTES DE AUX
										
												nuevo->next = aux;
												nuevo->prev = aux->prev;
												aux->prev->next = nuevo;
												aux->prev = nuevo;
												inserted = true;
												break;
											
										}
										else {
											//si la hora es mayor, debe insertar justo despues de ese nodo
											//verificar que el nodo siguiente de aux no sea nulo
											//y que sea exactamente igual pero con hora diferente y si es mayor o menor
											nuevo->prev = aux;
											nuevo->next = NULL;
											aux->next = nuevo;
											this->tail = nuevo;
											inserted = true;

										}
									}
									else if (date_compare(nuevo, aux, 3) == -1) {
										//si el dia es menor, inserta ahi mismo
											nuevo->next = aux;
											nuevo->prev = aux->prev;
											aux->prev->next = nuevo;
											aux->prev = nuevo;
											inserted = true;
											break;
										
									}//si el dia es mayor, avanza

								}
								else if (date_compare(nuevo, aux, 2) == -1) {
									//si el mes es menor, inserta ahi mismo
										nuevo->next = aux;
										nuevo->prev = aux->prev;
										aux->prev->next = nuevo;
										aux->prev = nuevo;
										inserted = true;
										break;
									
								}//si el mes es mayor avanza
							}
							else if (date_compare(nuevo, aux, 1) == -1) {
								//si el año es menor, inserta ahi mismo
									nuevo->next = aux;
									nuevo->prev = aux->prev;
									aux->prev->next = nuevo;
									aux->prev = nuevo;
									inserted = true;
									break;
								
							}
							//si el año es mayor, avanza
						}
						
					}
				}
			}
			//como se salio de insertar por especialidad O LLEGO AL ULTIMO, 
			//lo más probable es que se inserte al final
			
			//si es el primer elemento
			//if (aux = origen) {
			//	nuevo->prev = NULL;
			//	nuevo->next = aux;
			//	aux->prev = nuevo;
			//	origen = nuevo;
			//}
			////caso 2: nada de la lista es mayor al nuevo nodo. 
			//else if (aux->next == NULL) {
			//	nuevo->prev = aux;
			//	nuevo->next = NULL;
			//	aux->next = nuevo;
			//}
			//else {
			//	//caso 3: Hay algo mayor que el nuevo nodo y se coloca en medio
			//	nuevo->prev = aux->prev;
			//	nuevo->next = aux->next;
			//	aux->next->prev = nuevo;
			//	aux->prev->next = nuevo;
			//}
#pragma endregion

			this->lista = origen;
			//si no es igual a nada de la lista, no se repitio, y tampoco se inserto
			//asi que lo insertamos al final
			if (!repetido&&!inserted) {
				//insertar todo al final de la lista
				if (aux->next == NULL) {
					nuevo->prev = aux;
					nuevo->next = NULL;
					aux->next = nuevo;
					this->tail = nuevo;
					inserted = true;
				}
			}
			else {
				//revisa plis, no me hagas quedar mal
				return false;
			}
			//ya despues de todo el despapaye

			if (inserted&&!repetido) {
				return true;
			}
			else {
				return false;
				//algo fallo, revisa
			}
			
		}
	}
	//Revisar que funcione
	Node* creItem(char servicioC[], char mech[], char placas[], char cliente[],
		 char tel[],char shortfecha[],char hora[],bool attend,bool cancel) {
		Node* nuevo_nodo = new Node;
		nuevo_nodo->next = NULL;
		nuevo_nodo->prev = NULL;

		nuevo_nodo->ID = folioCita;
		folioCita++;
		
		strcpy(nuevo_nodo->mechName, mech);
		strcpy(nuevo_nodo->placas, placas);
		strcpy(nuevo_nodo->clientName, cliente);
		strcpy(nuevo_nodo->clientTel, tel);
		strcpy(nuevo_nodo->shortDate, shortfecha);
		strcpy(nuevo_nodo->shortHour, hora);
		strcpy(nuevo_nodo->specialC, servicioC);
		//------checkboxes
		nuevo_nodo->attend = attend;
		nuevo_nodo->canceled = cancel;

		//------------Ahora a llenar los datos de la fecha
		string date(shortfecha);
		int n = 0;
		n = date.find("/");
		string dia = date.substr(0, n);
		date = date.substr(n + 1, date.length());

		n = date.find("/");
		string mes = date.substr(0, n);
		date = date.substr(n + 1, date.length());

		string year = date;

		nuevo_nodo->date.dia = atoi(dia.c_str());
		nuevo_nodo->date.mes = atoi(mes.c_str());
		nuevo_nodo->date.año = atoi(year.c_str());

		//------------ahora la hora
		if (strcmp(hora, "8AM-12PM") == 0) {
			nuevo_nodo->date.hour = 8;
		}
		else if (strcmp(hora, "12PM-4PM") == 0) {
			nuevo_nodo->date.hour = 12;
		}
		else if (strcmp(hora, "4PM-8PM") == 0) {
			nuevo_nodo->date.hour = 16;
		}

		return nuevo_nodo;
	}
	bool CitsIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	//este edit funciona siempre y cuando no cambie los parametros del sort
	void editItem(Node*&edit ,char placas[], char cliente[],
		char tel[], bool attend, bool cancel, char canceldesc[]) {
		strcpy(edit->placas, placas);
		strcpy(edit->clientName, cliente);
		strcpy(edit->clientTel, tel);
		edit->attend = attend;
		edit->canceled = cancel;
		if (edit->canceled == true) {
			strcpy(edit->cancelMotive, canceldesc);
		}
	}
	int date_compare(Node* t1, Node* t2, int option) {
		// returns 1 if t1 greater than t2, -1 if t1 < t2, 0 if equal
		if (option == 1) {
			if (t1->date.año > t2->date.año) return(1);
			if (t1->date.año < t2->date.año) return(-1);
		}
		else if (option == 2) {
			// year matches. so check month
			if (t1->date.mes > t2->date.mes) return(1);
			if (t1->date.mes < t2->date.mes) return(-1);
		}
		else if (option == 3) {
			// month also matches. so check day
			if (t1->date.dia > t2->date.dia) return(1);
			if (t1->date.dia < t2->date.dia) return(-1);
			//day also matches, check hour
		}
		else if (option == 4) {
			if (t1->date.hour > t2->date.hour) return(1);
			if (t1->date.hour < t2->date.hour) return(-1);
		}
		// day also matches
		return(0);
	}
	bool lookDouble(char mech[],char date[],char hour[]) {
		//esto compara todos los char a ver si no estan repetidos
		if (!CitesIsEmpty()) {
			aux = origen;
			while (aux!= NULL) {
				if (strcmp(aux->mechName, mech) == 0) {
					if (strcmp(aux->shortDate, date) == 0) {
						if (strcmp(aux->shortHour, hour) == 0) {
							return true;
						}
					}
				}
				aux = aux->next;
				
			}
			return false;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return false;
		}

	}
	Node* findItem(char folio[]) {
		if (!CitsIsEmpty()) {
			int folioC = atoi(folio);
			aux = origen;
			while (aux != NULL) {
				if (aux->ID==folioC) {
					return aux;
				}
					aux = aux->next;
			}
			return NULL;
		}
		else {
			//si esta vacio, no hay nada que hacer
			return NULL;
		}
	}
	void delItem(Node*& var)
	{
		Node* auxD = var;

		if ((auxD->prev == NULL) && (auxD->next == NULL)) /*Caso de unico elemento*/ {
			delete auxD;
			origen = NULL;
			lista = NULL;
			tail = NULL;
		}
		else if (auxD->next == NULL) /*Caso de ultimo elemento*/ {
			this->tail = auxD->prev;
			this->tail->next = NULL;
			delete auxD;
		}
		else if (auxD->prev == NULL) /*Caso de primer elemento*/ {
			auxD->next->prev = NULL;
			origen = auxD->next;
			lista = origen;
			delete aux;
		}
		else { /*cualquier otro*/
			auxD->next->prev = auxD->prev;
			auxD->prev->next = auxD->next;
			delete auxD;
		}
	}
	bool CitesIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	Node* binarySearchM(Node* head, char mech[]) {
		Node* start = head;
		Node* last = NULL;

		do {
			Node* mid = middle(start, last);
			if (mid == NULL)
				return NULL;
			if (strcmp(mid->mechName, mech) == 0)
				return mid;
			else if (strcmp(mid->mechName, mech) < 0)
				start = mid->next;
			else
				last = mid;
		} while (last == NULL || last != start);
		return NULL;
	}
	Node* binarySearchS(Node* head, char special[]) {
		Node* start = head;
		Node* last = NULL;

		do {
			Node* mid = middle(start, last);
			if (mid == NULL)
				return NULL;
			if (strcmp(mid->specialC, special) == 0)
				return mid;
			else if (strcmp(mid->specialC, special) < 0)
				start = mid->next;
			else
				last = mid;
		} while (last == NULL || last != start);
		return NULL;
	}
	Node* middle(Node* start, Node* last) {
		if (start == NULL)
			return NULL;
		Node* slow = start;
		Node* fast = start->next;
		while (fast != last)
		{
			fast = fast->next;
			if (fast != last)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
		return slow;
	}
	void writeArchive() {
		CitasOArchv.open("Data\\citas.bin", ios::out | ios::trunc | ios::binary);
		if (!CitasOArchv.is_open()) {
			MessageBoxA(NULL, "No se GUARDO el archivo Citas", "error", MB_ICONASTERISK);
		}
		else {
			Node* temp = origen;
			while (temp != NULL) {
				CitasOArchv.write(reinterpret_cast<char*> (temp), sizeof(Node));
				temp = temp->next;
			}
			MessageBoxA(NULL, "Guardado de citas exitoso", "ok", MB_OK);
		}
		CitasOArchv.close();
	}
	void readArchive() {
		int foliomayor = folioCita;
		CitasArchv.open("Data\\citas.bin", ios::in | ios::binary | ios::ate);
		if (!CitasArchv.is_open()) {
			aux = origen = NULL;
			MessageBoxA(NULL, "No se ABRIO EL ARCHIVO CITAS", "error", MB_ICONASTERISK);
			CitasArchv.close();
		}
		else {
			int size = CitasArchv.tellg();
			if (size == 0) {
				MessageBoxA(NULL, "Archivo de citas vacio", "error", MB_ICONASTERISK);

			}
			for (unsigned int i = 0; i < (size / sizeof(Node)); i++) {

				if (origen == NULL) {
					origen = new Node;
					aux = origen;
					aux->prev = NULL;
					lista = origen;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Node;
					aux->next->prev = aux;
					aux = aux->next;
				}

				Node* Temp = new Node;
				CitasArchv.seekg(i * sizeof(Node));
				CitasArchv.read(reinterpret_cast<char*>(Temp), sizeof(Node));



				strcpy(aux->clientName, Temp->clientName);
				strcpy(aux->placas, Temp->placas);
				strcpy(aux->mechName, Temp->mechName);
				strcpy(aux->clientTel, Temp->clientTel);
				strcpy(aux->specialC, Temp->specialC);
				strcpy(aux->shortDate, Temp->shortDate);
				strcpy(aux->shortHour, Temp->shortHour);
				aux->ID = Temp->ID;
				if (aux->ID > foliomayor) {
					foliomayor = aux->ID;
				}
				aux->attend = Temp->attend;
				aux->canceled = Temp->canceled;
				aux->date.año = Temp->date.año;
				aux->date.mes = Temp->date.mes;
				aux->date.dia = Temp->date.dia;
				aux->date.hour = Temp->date.hour;


				delete reinterpret_cast<char*>(Temp);

				aux->next = NULL;
				aux = origen;
			}
			CitasArchv.close();
			if (origen != NULL) {
				while (aux->next != NULL) {
					aux = aux->next;
				}
				this->tail = aux;
				aux = origen;
				lista = origen;
			}
			folioCita = foliomayor + 1;
			MessageBoxA(NULL, "Cargado de citas exitoso", "OK", MB_OK);
		}
	}
	~Cits() {
		writeArchive();
		aux = lista;
		while (aux != NULL) {
			Node* borr = aux;
			aux = aux->next;
			delete borr;
		}
	}
}Citas;

class User {
private:
	
	string username="";
	string pass="";
	ifstream miTxt;
	string name = "";
	string picUs = "";
	string picApp = "";
public:
	char cUser[30];
	char cpicU[MAX_PATH];
	char cpicA[MAX_PATH];
	User() {
		//aqui se crea y se carga
		name = "";
		username = "";
		pass = "";

	}
	bool loadData() {
		miTxt.open("login.txt");
		if (!miTxt.is_open()) {
			/*miTxt.close();*/
			return false;
		}
		else {
			getline(miTxt, this->name);
			getline(miTxt, this->username);
			getline(miTxt,this-> pass);
			getline(miTxt,this-> picUs);
			getline(miTxt,this-> picApp);
			miTxt.close();
			strcpy(this->cUser, name.c_str());
			strcpy(this->cpicU, picUs.c_str());
			strcpy(this->cpicA, picApp.c_str());
			return true;
		}
	}
	bool compData(string user,string passe) {
		if (user.compare(username) == 0 && passe.compare(pass) == 0) {

			return true;
		}
		else {
			return false;
		}
	}
	void copyData(string&name,string&username,string&pass) {
		name = this->name;
		username = this->username;
		pass = this->pass;
	}
	
	~User() {
		//aqui se guarda y destruye
	}
}Usuario;

//cuidado, sin todas las banderas me cambia el destino del proyecto
void examinar(HWND Ventana, HWND hDestino)
{

	char Empty[MAX_PATH] = { "" };


	OPENFILENAME ofn;
	/*OPENFILENAME ofn;*/       // common dialog box structure
	char szFile[260];       // buffer for file name
							//HANDLE hf;            // file handle

							// Propiedades OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Ventana;
	ofn.lpstrFile = (LPWSTR)szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = (LPCWSTR)"Image files (*.jpg;*.png;*.bmp)\0*.jpg;*.png;*.bmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST| OFN_NOCHANGEDIR|OFN_READONLY;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{

		SendMessage(hDestino, WM_SETTEXT, 0, (LPARAM)ofn.lpstrFile);
		//folder = ofn.lpstrFile;
	}
	else
	{
		MessageBoxA(Ventana, "No eligio un archivo", "Aviso", MB_ICONEXCLAMATION);
		//folder = Empty;
		SendMessage(hDestino, WM_SETTEXT, 0, (LPARAM)"");
	}
}
void loadPic(HWND hImagen, char filebmp[MAX_PATH], int x, int y) {

	static HBITMAP bmp1;
	static HBITMAP bmp2;

	bmp1 = (HBITMAP)SendMessage(hImagen, STM_GETIMAGE, IMAGE_BITMAP, 0);
	bmp2 = (HBITMAP)LoadImageA(NULL, filebmp, IMAGE_BITMAP, x, y, LR_LOADFROMFILE);
	SendMessage(hImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);

}


#endif

