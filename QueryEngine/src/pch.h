#ifndef PCH_H
#define PCH_H


#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

//TODO:remplacer const char x 100  par const my_tuple*  a rentrer avec a chaque fois description du caractere attendu
//TODO: convention ordre des param dans signature fonction


//TODO: free correctement

//TODO: enleve les warnings

//TODO: merge FROM and WHERE and SELECT "parsing" with function traitement dans general funct call : select ,from ,where ,  query 

//TODO: faire arbre correctement trie + reduction ( tautologie , CU , ... )

struct str_or_int {
	char* str;
	int nb;
};

//STR_OR_INT

	//CONSTRUCTOR STR_OR_INT pour tableau donc ne return pas ptr ( a changer)

		void test_str_or_int(FILE* fptr, const char* descri, str_or_int * strInt); //init from file
		void test_str_or_int2(char * buffer, const char* descri, str_or_int * strInt); // convertie le buffer en str_or_int
		void init_str_or_int(str_or_int* modif, const str_or_int* source);//CONSTRUCTEUR par copie/par defaut si second param ==NULL ;

	//Display:

		void affichage_str_or_int(const str_or_int* param);

//FIN STR_OR_INT 

struct Table {

	char** description;
	char* name;
	size_t nb_attribut;
	struct str_or_int * tab;
	size_t nb_element;

};

//TABLE:
	//CONSTRUCTEUR:

		Table * crea_default_table(); // constructor par defaut d'une table  return l'objet alloue
		void crea_class(Table * , const int , const char * , size_t, FILE* , const char, const char , const char , const char , const char , const char , const char , const char );//creer une table via fichier  avec son ptr + nom  comme id 2 premier param

	
	//INITIALISEUR:

		//DESCRIPTION:
			void get_description(FILE* fptr, Table* my_table, const char delim, const size_t);//INITIALISE LA DESCRIPTION VIA UN FORMAT DE FICHIER

		//COLONNE:


		//LIGNE:
			void  append_ligne(str_or_int * buffer, str_or_int * big, const size_t taille_g);//rempli le tableau  pointe par buffer de str_or_int buffer[0;taille_g[ avec big[0;taille_g[  ou avec NULL si default

		//TABLEAU STR OR INT
			void append_class(Table*, FILE*, const  int, const size_t, const char, const char, const char, const char, const char, const char, const char, const char);//initialise un tab de str_or_int appartenant a une table via fichier

	//DESTRUCTEUR;

		void free_class(Table*);//LIBERE TABLE
		int free_descri(char** descri, const size_t taille);//LIBERE la description
		int free_tab_strInt(str_or_int* tab, const size_t taille); //LIBERE TAB

	//DEBUG:

		//DISPLAY:

			//JSON:

				void afficher_description(char** const descri, const size_t taille);
				void afficher_objetL(const Table * my_class, const size_t num);
				void afficher_class(const Table* my_class);

			//CSV:

				void afficher_description2(char** const descri, const size_t taille);
				void afficher_objetL2(const Table * my_class, const size_t num, const char fin_colonne, const char deb_colonne);
				void afficher_class2(const Table* my_class, const char fin_table, const char fin_ligne, const char fin_colonne, const char deb_ligne, const char deb_colonne, const char deb_table, const char deb_descri, const char fin_descri);

//FIN_TABLE

struct TableauId
{
	struct  Table* arr;
	size_t taille;
};

//TABLEAU DE TABLE: PARSEUR

	//CONSTRUCTEUR :	

		TableauId* creaTabId(); //DEFAULT
		TableauId* parseur_fichier(const char*, const int, const char, const char, const char, const char, const char, const char, const char, const char);//VIA UN FORMAT DE FICHIER :

	//DESCTRUCTEUR:

		void del_TabId(TableauId*my_tabId);




//UTILITAIRE:
		
			//ERREUR
			inline void my_erreur(void* ptr);
			//FICHIER
			FILE *open_fichier(const char* n_fichier, const char* param);

			//PARSEUR
			char* getString2(FILE* fptr, const char delim, char* str);

			int getString_arr(FILE* fptr, const char *  arr_delim, char** key_tuple, const size_t taille);// rempli un tableau de string grace a un fichier , le tableau de delimiteur contient :
			//des delimiteurs dont l'indexe correspond à l'indexe de la valeur dans le tableau de string
			// et des delimiteurs final qui stoppe la saisi et dont l'indexe est de taille-1 dans le tableau de string et dont l'indexe est de [taille-1; strlen(arr_delim)] dans  le tableau de delimiteur
			//renvoie l'indexe du dernier delimiteur trouvé 

			//tableau de cond pour && 


			int find_description(const char * colonne, char **  descri, const size_t taille); // trouve le nom d une colonne dans une description precise renvoie -1 si pas trouver sinon l'increment de la colonne dans cette descri

			int testambiguite(const char * buffer, char** description, const size_t taille); // confort pour afficher erreur , regarde si dans description il y a deja buffer sinon renvoie 0 : pas trouvé

			Table* search_id3(TableauId* tab, char* nom);// cherche le nom d'une table dans la tableau de table 

			//TODO: autre getstring2 pour les enlever les espaces devant ou fin PROTOTYPE:
			char* getString3(FILE* fptr, const char delim, char* str);// maybe passeer un delim a ignorer ou carrément des /*  */ ou // dans un tab

//UTILITAIRE :


			int find_cara(const char* str, char cara);




			
	struct my_tuple {// ajouter enum:function{MAX , MIN , ...} par defaut NULL , fin function :"(" deb_delim-1 = ")" ou sinon balek continu jusqu'a ","  

			const char* delim;
			size_t deb_delim;
			size_t taille;

	};

//TUPLE:

	//CONSTRUCTOR TUPLE:

		my_tuple* create_tuple(const char * delim, size_t deb_delim, size_t taille);

	//DELETE TUPLE:

		void free_tuple(char** lol, size_t taille); // libere le tuple

	//SURCHARGE OP :

		//PREDICAT:
			int isequal_tuple(char** tuple, char** tuple2, size_t taille);// test si 2 tuple sont égaux

			
//FIN TUPLE ;


		struct tab_tuple {
				char*** tab;
				size_t taille;
				size_t taille_tuple;
				size_t cursor;
		};

//TABLEAU_TUPLE:

	//CONSTRUCTOR :

		tab_tuple* defolt_arr_tuple(size_t taille_tuple); // constructor par "defaut" de tuple avec la taille des tuples stocké : le nombre de valeur qui les composes 

		tab_tuple* create_tab_tuple(FILE* fptr, my_tuple* tuple);//initialise tableau tuple avec fichier

	//SECURE REALLOC

		tab_tuple * tuple_arr_realoc(tab_tuple* tuple_arr, const size_t resize); // secur realloc pour tab_tuple

	//DESTRUCTOR TABLEAU de tuple

		void free_tab_tuple(tab_tuple* autre); // libere le contenu du select 

//FIN TABLEAU_TUPLE


// DEBUT SELECT :

		//TRAITEMENT SELECT:

			//tab_tuple* create_tab_tuple(FILE* fptr, my_tuple* tuple);//initialise tableau tuple avec fichier

		//DISPLAY:

		void print_select(const Table *T, int* tab, size_t taille);//res tableau de tuple 

//FIN SELECT


			



struct Identifiant {

	size_t indexe_colonne;
	struct element * id_table;
};

//IDENTIFIANT:

	//CONSTRUCTOR:
	//DESTRUCTOR:
	//DISPLAY:

//FIN_IDENTIFIANT

struct Identifiant_or {
	Identifiant * id;
	str_or_int val;
};

//IDENTIFIANT_OR:

	//CONSTRUCTOR:
	//DESTRUCTOR:
	//DISPLAY:

//FIN_IDENTIFIANT_OR


struct element {
	Table* adresse_table;
	char* rename;
	int set;
	size_t counter;
};

//ELEMENT de la pile :

	//CONSTRUCTOR:

		element* copy_element_init(element* ele); //  constructeur par copy d'element : return l'objet crée 
		element* create_element(element* fill, Table* adresse, char* rename);// constructeur par copy/valeur pas d'allocation 


	//OPERATOR:

		void my_cpy(element* cotenu, element * contenant);// surcharge op de copy pour la classe elment


	//DEBUG:

		//DIPSPLAY:

			void print_element(element * my_elem);



struct from {
	element* arr;
	size_t taille;
};

//PILE

	//CONSTRUCTOR :

		from * create_pile(FILE* fptr, TableauId* tab, my_tuple* tuple);//constructor pile via FILE
		from * default_pile(size_t);
	
	//DESTRUCTOR:

		void free_pile(from* my_pile); // libere la pile contenant les table par ordre croissant


	//OPERATOR:

		//surcharge operator  +=  pour pile 
		void append_tri(from* my_pile, element* elem); // ajoute la valeur via dichotomie dans la pile de table que constitue le from 



	//UTILITAIRE:

		void search_id(from * tab, char ** nom, const size_t taille_tuple, Identifiant_or * result);// cherche le nom d'une table  ainsi que le nom d'une colonne dans la pile pour initailiser result 
		element* search_id2(from* tab, char* nom); // cherche le nom d'une table dans la pile  et renvoie son adresse dans tab
	
	//DEBUG:

		//DISPLAY:

			void print_pile(from * my_pile);

//FIN_PILE



//DEBUT FROM :

		//TRAITEMENT FROM:

			//from * create_pile(FILE* fptr, TableauId* tab, my_tuple* tuple);//constructor pile via FILE

		//DISPLAY:

			//void print_pile(from * my_pile);

//FIN FROM

struct cond {
	char * operation;
	Identifiant_or lhs, rhs; // peut changer en un seul mais perte sens lhs rhs 
};


//CONDITION:

	//CONSTRUCTOR:

		cond * get_condition(FILE*fptr, cond* condition, from * my_pile, my_tuple * tuple, const char li);//constructor condition via fichier 

		//constructor Identifiant_or :
		void find_colonne(from* tab, char ** nom, size_t tae, Identifiant_or * result);// initialise un membre d une condtion dont seul la colonne a etait specifier 

	//DISPLAY:

		void affichage_cond(const cond* condi);

		//UTILITAIRE:

			int testcondition(cond* condition);//test une condition est renvoie le bool associe


//FIN_CONDITION




struct Noeud {
		struct Noeud * pere;
		struct Noeud * filsG;
		struct Noeud * filsD;
		char* val; // pour noeud
		struct cond * verif; // pour leaf
		int poids;
	};


//FUCK UP TREE:

	//CONSTRUCTOR LEAF:

		Noeud * createleaf(FILE* fptr, Noeud * father, from * my_tab, const char delim);//construit une feuille via un fichier

	//DESTRUCTOR LEAF:


	//CONSTRUCTOR NODE:

		Noeud * create_Node(Noeud * father);
		Noeud * create_NoeudD(char* efr, int poids, Noeud* fath);

	//DESTRUCTOR NODE:
		

	//CONSTRUCTOR TREE:

		Noeud * createNoeud(FILE * fptr, const char delim, Noeud* father, from * my_tab);//CONSTRUCTEUR ARBRE via FILE

	//DESCTRUCTOR TREE:

		void free_arbre(Noeud * root);

	//UTILITAIRE:

		int isnotFD(Noeud* parcour);//test si un noeud est le filsD de son pere

	//DEBUG:

		//DISPLAY:

		void print_arbre(Noeud* racine);// parcour infixe

//FIN_TREE


// FAIRE UN SWITCH POUR LES MOTS CLES A APPELE A CHAQUE NEW LINE 






//DEBUT WHERE:

	Table * my_where(FILE* fptr, from * my_pile, tab_tuple* autre);

	//APPEND LIGNE PILE :

		void append(from* my_pile, Table * buffer);//ajoute une ligne au buffer grace au numero de ligne contenu dans la pile : concatenation des lignes de la pile 
		void recursive_append(from* my_pile, Table * buffer, size_t k); // ajoute au buffer la ligne resultant de la concatenation des lignes designe par my_pile

	//TRAITEMENT WHERE :

		void funct(from* my_pile, Table* buffer, Noeud * parcour, int cond); //remonte dans l'arbre 
		void lol(from * my_pile, Noeud * root, Table * buffer); // descends dans l'arbre 

	//DISPLAY :

		void print_false(from* my_pile);
		void print_true(from* my_pile);


//FIN WHERE:



		/*typedef enum { RIEN, ET, OU }logical_operation;






const static struct {
	logical_operation     val;
	const char *str;
} traduction[] = {
	{RIEN, "NULL"},
	{ET, "&&"},
	{OU, "||"},
};*/



#endif //PCH_H


