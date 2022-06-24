#ifndef  _mechs_
#define _mechs_
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>

#pragma warning(disable : 4996) 
using namespace std;
int folioEmp = 700000;

class Mechs {
public:
	struct Nodo
	{
		int idEmp;
		char name[70];
		char pic[300];
		char telephone[12];
		char occupation[30];
		struct sched {
			int weekSche = 0;
			int hourSche = 0;
		}empSched;
		Nodo* next;
		Nodo* prev;
	}*lista, * origen, * tail, * aux;

	struct leaf
	{
		int idEmp;
		char name[70];
		char pic[300];
		char telephone[12];
		char occupation[30];
		struct sched {
			int weekSche = 0;
			int hourSche = 0;
		}empSched;
		leaf* der;
		leaf* izq;
		leaf* parent;
	}*tree,*auxleaf;

	//variables para guardar archivos
	ifstream mechsArch;
	ofstream mechsOArch;

	Mechs() {
		lista = tail = origen = aux = NULL;
		readArchive();
		tree=auxleaf = NULL;
		cargarArbol(tree,lista);
	}
	//funciones para la lista
	Nodo* creItem(char name[], char pic[], char tel[], char ocup[], char wsched[], char hsched[]) {
		Nodo* nuevo_nodo = new Nodo;
		strcpy_s(nuevo_nodo->name, name);
		strcpy_s(nuevo_nodo->pic, pic);
		strcpy_s(nuevo_nodo->telephone, tel);
		strcpy_s(nuevo_nodo->occupation, ocup);

		nuevo_nodo->idEmp = folioEmp;
		folioEmp++;

		//agregar el horario semanal 1=lun a viernes, 2 el sabado
		if (strcmp(wsched, "LUNES A VIERNES") == 0) {
			nuevo_nodo->empSched.weekSche = 1;
		}

		//como recien se creo la eservialidad, los horarios estan vacios
		//cuando se llenen 1 es de 8am -12pm , 2 es de 12-4 pm y 3 es de 4-8pm
		
		if (strcmp(hsched, "8AM-12PM")==0) {
			nuevo_nodo->empSched.hourSche = 1;
		}
		else if (strcmp(hsched, "12PM-4PM")==0) {
			nuevo_nodo->empSched.hourSche = 2;
		}
		else if (strcmp(hsched, "4PM-8PM")==0) {
			nuevo_nodo->empSched.hourSche = 3;
		}

		nuevo_nodo->prev = NULL;
		nuevo_nodo->next = NULL;

		//nomas va a haber 1 espacio de trabajo por cada especialidad, nada de 2 mecanicos
		//con la misma especialidad, atendiendo el mismo dia y horario, en lados diferentes
		//no caben
		return nuevo_nodo;
	}
	void editItem(Nodo*& edit,char name[], char pic[], char tel[], char ocup[], char wsched[], char hsched[]) {
		if (strcmp(edit->name, name) == 0) {
			//keeps the same
		}
		else {
			strcpy(edit->name, name);
		}

		if (strcmp(edit->pic, pic) == 0) {

		}
		else {
			strcpy(edit->pic, pic);
		}

		if (strcmp(edit->telephone, tel) == 0) {

		}
		else {
			strcpy(edit->telephone, tel);
		}

		if (strcmp(wsched, "LUNES A VIERNES") == 0) {
			edit->empSched.weekSche = 1;
		}
		

		//como recien se creo la eservialidad, los horarios estan vacios
		//cuando se llenen 1 es de 8am -12pm , 2 es de 12-4 pm y 3 es de 4-8pm
		//de ser 2, el horario es el sabado de 8am-2pm con el numero 4
		if (strcmp(hsched, "8AM-12PM")==0) {
			edit->empSched.hourSche = 1;
		}
		else if (strcmp(hsched, "12PM-4PM")==0) {
			edit->empSched.hourSche = 2;
		}
		else if (strcmp(hsched, "4PM-8PM")==0) {
			edit->empSched.hourSche = 3;
		}
		
	


		//modificar en el arbol
		leaf* editHojita = buscarHoja(tree, edit->idEmp);
		if (editHojita != NULL) {
			editLeaf(editHojita, edit);
		}
	}
	void inItem(Nodo*& nuevo)
	{
		leaf* nuevoH = crearHoja(nuevo);

		aux = origen;
		if (origen == NULL)
		{
			origen = nuevo;
			nuevo->next = NULL;
			tail = nuevo;
			lista = origen;

		}
		else
		{
			//insertar todo al final de la lista
			tail->next = nuevo;
			nuevo->prev = tail;
			nuevo->next = NULL;
			tail = nuevo;
		}

		insertLeaf(tree,nuevoH);
		balanceMyTree();
	}
	bool MechsIsEmpty() {
		if (lista == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	bool lookDouble(char mechName[]) {
		if (!MechsIsEmpty()) {
			aux = origen;
			while (aux != NULL) {
				if (strcmp(aux->name, mechName) == 0) {
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
	Nodo* findItem(char mecName[]) {
		if (!MechsIsEmpty()) {
			aux = origen;
			while (aux != NULL) {
				if (strcmp(aux->name, mecName) == 0) {
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
	Nodo* findItemI(int nomina){
		if (!MechsIsEmpty()) {
			aux = origen;
		
			while (aux != NULL) {
				if (aux->idEmp==nomina) {
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
    void  delItem(Nodo*&mec)
	{
		Nodo*auxD = mec;
		//eliminamos primero de el arbol
		leaf* delHoja = NULL;
		delHoja = buscarHoja(tree, mec->idEmp);

		if (delHoja != NULL) {
			eliminarNodo(delHoja);
		}
		//ahora eliminamos de la lista ligada
		if ((auxD->prev == NULL) && (auxD->next == NULL)) /*Caso de unico elemento*/ {
			delete auxD;
			origen = NULL;
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
			delete aux;
		}
		else { /*cualquier otro*/
			auxD->next->prev = auxD->prev;
			auxD->prev->next = auxD->next;
			delete auxD;
		}
	}
	Nodo* binarySearchList(Nodo*head,int mat) {
		Nodo* start = head;
		Nodo* last = NULL;

		do {
			Nodo* mid = middle(start, last);
			if (mid == NULL)
				return NULL;
			if( mid->idEmp== mat)
				return mid;
			else if (mid->idEmp< mat)
				start = mid->next;
			else
				last = mid;
		} while (last == NULL || last != start);
		return NULL;
	}
	Nodo* middle(Nodo*start,Nodo*last) {
		if (start == NULL)
			return NULL;
		Nodo* slow = start;
		Nodo* fast = start->next;
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
	//-------------tree functions
	leaf* crearHoja(Nodo*nuevo) {
		leaf* nueva_hoja = new leaf();

		nueva_hoja->idEmp = nuevo->idEmp;
		nueva_hoja->empSched.weekSche = nuevo->empSched.weekSche;
		nueva_hoja->empSched.hourSche = nuevo->empSched.hourSche;
		strcpy(nueva_hoja->name, nuevo->name);
		strcpy(nueva_hoja->occupation, nuevo->occupation);
		strcpy(nueva_hoja->telephone, nuevo->telephone);

		nueva_hoja->izq = NULL;
		nueva_hoja->der = NULL;

		return nueva_hoja;

	}
	void PreOrden(leaf* arbol) {
		if (arbol == NULL) {
			return;
		}
		else {
			//cout arbol->dato
			PreOrden(arbol->izq);
			PreOrden(arbol->der);
		}

	}
	//que los inserte desbalanceados, luego se arregla
	void insertLeaf(leaf*&arbol,leaf *nuevoH) {
		if (tree == NULL) {
			tree = nuevoH;
			tree->parent = NULL;
		}
		else {
			InsertLeaftoTree(&tree, nuevoH, 0);
		}
	}
	void InsertLeaftoTree(leaf** nodo, leaf* nuevoH, leaf* padre) {
		leaf* aux = *nodo;
		if (aux == NULL)
		{
			*nodo = nuevoH;
			nuevoH->parent = padre;
		}
		else
		{
			int valorRaiz = aux->idEmp;

			if (valorRaiz < nuevoH->idEmp)
				InsertLeaftoTree(&(aux->der), nuevoH, aux);
			else
				InsertLeaftoTree(&(aux->izq), nuevoH, aux);

			
		}
	}
	void cargarArbol(leaf* tree, Nodo* lista) {

		if( !lista )
			return;


		Nodo* slow = lista,*fast = lista;

		while (fast != NULL && fast->next != NULL) {
			slow = slow->next;
			fast = fast->next->next;
		}

		//slow = mitad
		Nodo* tIzq = slow, *tDer = slow->next;
		while ( tIzq || tDer ){
			if ( tIzq ){
				leaf* newleaf = crearHoja(tIzq);
				insertLeaf( tree, newleaf );
				tIzq = tIzq->prev;
			}

			if ( tDer ){
				leaf* newleaf = crearHoja(tDer);
				insertLeaf( tree, newleaf );
				tDer = tDer->next;
			}
		}

	/*	balanceMyTree();*/
	}
	leaf* buscarHoja(leaf*tree,int mat){
		if (tree == NULL) {
			return NULL;
		}
		else if (tree->idEmp == mat) {
			return tree;
		}
		else if (mat < tree->idEmp) {
			return buscarHoja(tree->izq, mat);
		}
		else {
			return buscarHoja(tree->der,mat);
		}

	}
	void editLeaf(leaf* hojita, Nodo* nodolista) {
		strcpy(hojita->name, nodolista->name);
		strcpy(hojita->occupation, nodolista->occupation);
		strcpy(hojita->telephone, nodolista->telephone);
		strcpy(hojita->pic, nodolista->pic);
		hojita->idEmp = nodolista->idEmp;
		hojita->empSched.weekSche = nodolista->empSched.weekSche;
		hojita->empSched.hourSche = nodolista->empSched.hourSche;
	}
	leaf* minimo(leaf* arbol) {
		if (arbol == NULL) {
			return NULL;
		}
		else if (arbol->izq) {
			return minimo(arbol->izq);
		}
		else {
			return arbol;
		}
	}
	void eliminarNodo(leaf* nodoEliminar) {
		if (nodoEliminar->izq && nodoEliminar->der) {
			leaf* menor = minimo(nodoEliminar->der);
			nodoEliminar = menor;
			eliminarNodo(menor);
		}
		else if (nodoEliminar->izq) {
			reemplazar(nodoEliminar, nodoEliminar->izq);
			destruirNodo(nodoEliminar);
		}
		else if (nodoEliminar->der) {
			reemplazar(nodoEliminar, nodoEliminar->der);
			destruirNodo(nodoEliminar);
		}
		else {
			if (nodoEliminar == this->tree) {
				tree = NULL;
			}
			else {
				reemplazar(nodoEliminar, NULL);
				destruirNodo(nodoEliminar);
			}
		}


	}
	void destruirNodo(leaf* node) {
		node->izq = NULL;
		node->der = NULL;

		delete node;
	}
	void reemplazar(leaf* arbol, leaf* nuevo_nodo) {
		if (arbol->parent) {
			if (arbol->idEmp == arbol->parent->izq->idEmp) {
				arbol->parent->izq = nuevo_nodo;
			}
			else if (arbol->idEmp == arbol->parent->der->idEmp) {
				arbol->parent->der = nuevo_nodo;
			}
		}
		if (nuevo_nodo) {
			nuevo_nodo->parent = arbol->parent;
		}
	}
	void _deleteTree(leaf* node)
	{
		if (node == NULL) return;

		/* first delete both subtrees */
		_deleteTree(node->izq);
		_deleteTree(node->der);

		/* then delete the node */
		delete node;
	}
	void deleteTree(leaf** node_ref)
	{
		_deleteTree(*node_ref);
		*node_ref = NULL;
	}

	//-----balancear mi arbol feo
	void storeBSTNodes(leaf* root, vector<leaf*>& nodes)
	{
		
		// Base case
		if (root == NULL)
			return;

		// Store nodes in Inorder (which is sorted
		// order for BST)
		storeBSTNodes(root->izq, nodes);
		nodes.push_back(root);
		storeBSTNodes(root->der, nodes);
	}
	/* Recursive function to construct binary tree */
	leaf* buildTreeUtil(vector<leaf*>& nodes, int start,
		int end)
	{
		// base case
		if (start > end)
			return NULL;

		/* Get the middle element and make it root */
		int mid = (start + end) / 2;
		leaf* root = nodes[mid];

		/* Using index in Inorder traversal, construct
		   left and right subtress */
		root->izq = buildTreeUtil(nodes, start, mid - 1);
		root->der = buildTreeUtil(nodes, mid + 1, end);

		return root;
	}
	// This functions converts an unbalanced BST to
	// a balanced BST
	leaf* buildTree(leaf* root)
	{
		// Store nodes of given BST in sorted order
		vector<leaf*> nodes;
		storeBSTNodes(root, nodes);

		// Constucts BST from nodes[]
		int n = nodes.size();
		return buildTreeUtil(nodes, 0, n - 1);
	}
	void balanceMyTree() {
		tree = buildTree(tree);
	}
	
	//-----------------pal reporte de mecanicos
	//nota, esto se tiene que implementar en el codigo pa modificar el listBox
	//e imprimirlo en un archivo de texto
	int sizeOfTree(leaf* node)
	{
		int c = 1;

		if (node == NULL)
			return 0;
		else
		{
			c += sizeOfTree(node->izq);
			c += sizeOfTree(node->der);
			return c;
		}
	}
	int AddToArray(leaf* node, int arr[], int i)
	{
		if (node == NULL)
			return i;


		arr[i] = node->idEmp;
		i++;
		if (node->izq != NULL)
			i = AddToArray(node->izq, arr, i);
		if (node->der != NULL)
			i = AddToArray(node->der, arr, i);

		return i;
	}

	//-------quicksort
	int partition(int* a, int start, int end)
	{
		int pivot = a[end];
		int P_index = start;
		int i;
		int d;
		for (i = start; i < end; i++)
		{
			if (a[i] <= pivot)
			{
				d = a[i];
				a[i] = a[P_index];
				a[P_index] = d;
				P_index++;
			}
		}
		d = a[end];
		a[end] = a[P_index];
		a[P_index] = d;
		return P_index;
	}
	void Quicksort(int* a, int start, int end)
	{
		if (start < end)
		{
			int P_index = partition(a, start, end);
			Quicksort(a, start, P_index - 1);
			Quicksort(a, P_index + 1, end);
		}
	}
	
	//-----------guardar en archivos binarios
	void writeArchive() {
		mechsOArch.open("Data\\mechs.bin", ios::out | ios::trunc | ios::binary);
		if (!mechsOArch.is_open()) {
			MessageBoxA(NULL, "No se GUARDO el archivo DE MECANICOS", "error", MB_ICONASTERISK);
		}
		else {
			Nodo* temp = origen;
			while (temp != NULL) {
				mechsOArch.write(reinterpret_cast<char*> (temp), sizeof(Nodo));
				temp = temp->next;
			}
			MessageBoxA(NULL, "Guardado de mecánicos exitoso", "ok", MB_OK);
		}
		mechsOArch.close();
		
	}
	void readArchive() {
		mechsArch.open("Data\\mechs.bin", ios::in | ios::binary | ios::ate);
		if (!mechsArch.is_open()) {
			aux = origen = NULL;
			MessageBoxA(NULL, "No se cargo el archivo", "error", MB_ICONASTERISK);
			mechsArch.close();
		}
		else {
			int size = mechsArch.tellg();
			if (size == 0) {
				MessageBoxA(NULL, "Archivo de mecanicos vacio", "error", MB_ICONASTERISK);

			}
			for (unsigned int i = 0; i < (size / sizeof(Nodo)); i++) {

				if (origen == NULL) {
					origen = new Nodo;
					aux = origen;
					aux->prev = NULL;
					lista = origen;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Nodo;
					aux->next->prev = aux;
					aux = aux->next;
				}

				Nodo* Temp = new Nodo;
				mechsArch.seekg(i * sizeof(Nodo));
				mechsArch.read(reinterpret_cast<char*>(Temp), sizeof(Nodo));


				aux->idEmp = Temp->idEmp;
				strcpy(aux->name, Temp->name);
				strcpy(aux->occupation, Temp->occupation);
				strcpy(aux->pic, Temp->pic);
				strcpy(aux->telephone, Temp->telephone);
				aux->empSched.weekSche = Temp->empSched.weekSche;
				aux->empSched.hourSche = Temp->empSched.hourSche;

				delete reinterpret_cast<char*>(Temp);

				aux->next = NULL;
				aux = origen;
			}
			mechsArch.close();
			//aqui hay que reemplazar el global id con el ultimo id MIENTRAS NO ESTE VACIO
			if (origen != NULL) {
				aux = origen;
				this->lista = origen;
				while (aux != NULL) {
					if (aux->next == NULL) {
						folioEmp = aux->idEmp;
						folioEmp++;
					}
					aux = aux->next;
				}
				aux = origen;
				while (aux->next != NULL) {
					aux = aux->next;
				}
				this->tail = aux;
				aux = origen;
			}
			MessageBoxA(NULL, "Cargado de mecanicos exitoso", "OK", MB_OK);
		}
	}
	~Mechs() {
		writeArchive();
		aux = lista;
		while (aux != NULL) {
			Nodo* borr = aux;
			aux = aux->next;
			delete borr;
		}
		deleteTree(&tree);

	}


} *Mechanics=NULL;

#endif

