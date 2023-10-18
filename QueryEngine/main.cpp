// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
*/

int main()
{
	/*
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	*/
	TableauId* tab= parseur_fichier("lol.txt", 0, '~', '\n', ',', '\n', '\0', '\0', '\0', '\0'); // active le parseur csv
	/*str_or_int* ligne = (str_or_int *) malloc(sizeof(str_or_int) * 3);
	append_ligne(tab[0].arr->tab, ligne, 2);*/
	FILE * fptr = open_fichier("tester.txt", "r");

	//create_BigBoy(&tab->arr[0], tab);


	char bufr[255]="\0";
	char* buffer = NULL;
	char recup = (char)0;
	tab_tuple* lol = NULL;
	my_tuple* autre = create_tuple(". ,\n",2,5);
	size_t i = 0;
	for (; !feof(fptr) && (strcmp((buffer = getString2(fptr, ' ', buffer)), "SELECT")); free(buffer), buffer = NULL);
	if (!feof(fptr)) {
		free(buffer); buffer = NULL;
		lol= create_tab_tuple(fptr, autre);
	}
		for (; !feof(fptr) && (strcmp((buffer = getString2(fptr, ' ', buffer)), "FROM")); free(buffer), buffer = NULL);
		if (!feof(fptr)) {
			free(buffer); buffer = NULL;
			my_tuple* tuple = create_tuple(" ,\n", 1, 4);
			from* pile = create_pile(fptr, tab, tuple);
			free(tuple);
			//while (recup != EOF && (recup = getc(fptr)))printf("recup:%c\n", recup);
			for (; !feof(fptr) && (strcmp((buffer = getString2(fptr, ' ', buffer)), "WHERE")); free(buffer), buffer = NULL);
			if (!feof(fptr)) {
				
				free(buffer);
				buffer = NULL;
				Table* selec = my_where(fptr, pile,lol);
				//afficher_class2(selec, '~', '\n', ',', '\n', '\0', '\0', '\0', '\0');
				free_pile(pile);
				free_class(selec);
				free(selec);
			}
			//else { return 1; }

		}
	//else { return 1; }*/
	fclose(fptr);
	
	del_TabId(tab);
	
	//_CrtDumpMemoryLeaks();
}

/**/


//
//
//Noeud* RcreateNoeud( delim )
//{
//	if recup == '(' then  FG = createNode(delim = ')')//FG    A && ( B || ( C && D) )   // B || (C && D)
//	else Createleaf(FG, delim = ' ')
//
//		//FG get
//
//		Create_root(, [' ']); // root  create_node
//
//	if recup == '(' then FD=createNode( delim = ')') //FD
//	else Createleaf(FD, delim = ' ')
//
//}
//
//str_or_int * concat_ligne_check(Pile_tab* my_tab, char ** description, str_or_int*  ligne, cond ma_cond)
//{
//	// si dans ma_cond lhs ou rhs nom_table n'est pas dans pile trie alors :
//	concat(ligne->description lhs ou rhs);
//	resize(ligne);
//
//	nb_attribut_cumule = search(/*+=*/lhs->nom, pile_tab/*id*/);
//	nb_attribut_cumule2 = search(/*+=*/rhs->nom, pile_tab/*id*/);
//
//	if (operate(ligne[nb_attribut_cumule), ma_cond.op, ligne[nb_attribut_cumule2))return ligne;
//	else return NULL;
//
//
//
//}
//
//Table * disjonction(TableauId * arr_table, rqt arr_cond)
//{
//
//
//	for (size_t k = 0; (ligne = (pile_de_table, resul->description, ligne, arr_cond[k])) && k < taille_tableaucond; k++);
//	if (k == taille_tableaucond) {
//		set(ligne);//NULL TOUT LES PILE DE TABLE == NULL
//		append_ligne(result, ligne);
//	}
//
//}
