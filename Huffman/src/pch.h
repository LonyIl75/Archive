
			#ifndef PCH_H
			#define PCH_H
			#include<stdlib.h>
			#include <stdio.h>
			#include <string.h>
			#include <conio.h>
			#include <errno.h>
			#include <ctype.h>
            #include <math.h>
			// int car word dans chaque  compputer = int  donc meme si une struct bit avec un nb de bit précis ou un string serait plus faibnle en memoire l'acces et l'optimisation serait moins grande paralelle sizeèt int pour taille 
			// toujours int plus efficace 
			 // voir si tpydef / enum / ou autre pour simuler type bool ou encore include bool qui <=> enum mais avec tab
			//size_t nb_elem_L = 0, /*nb_elem_A = 0*/; //pour le debug apres enlever
			typedef struct C
			{
				size_t occurence;
				char val_c;

				struct C* suiv_L;
				struct C * pere_A, *filsD_A, *filsG_A;


			}Cell;
			typedef struct L
			{

				Cell*bege, *ende;


			}Liste;
			typedef struct B
			{
				char cara;
				int bin;
				size_t taille;
				int* b_tab;

			}Binaire;
			typedef struct tabe
			{
				char cara;
				int * moninte;
				size_t taille; // nb d'int
				size_t cursor; // curseur du dernier int

			}R;


			R* creaR(const char str, const int*tab, const size_t taille, const size_t curr);
			
			void tri_insertion(Liste* maliste);
			R** traite_tab(FILE *trad, const size_t sz_row);
			void change_masque(R* masque, const int* tab, const size_t taille, const size_t cursor1, const size_t cursor2);
			void print_masque(R* masque);
			Binaire * creaBinaire();
			void printArbre(Liste* root);
			void printArbre2(Cell* maliste,Cell*);
			Liste * creaListe();

		    inline unsigned int is_emptyL(Liste const * liste)
			{
				return ( liste->bege==NULL );
			}

			void location_append(Cell * , Cell *,Liste*);
			void location_append2(Cell * , Cell * , Liste*);
			Cell* chercheL(Cell *, Liste* );

			void appendtri(Cell * , Cell* , Liste* );


			Cell * creaCell(const size_t , const unsigned int);
			Cell * creaNode(Cell  *, Cell  *);//creer un noeud avec les 2 fils connu 
			 inline unsigned isfeuille(Cell* macell)
			{
				return (macell->filsD_A || macell->filsG_A ? 0 : 1);
			}

			void creaArbre(Liste* maListe);
		
			void binary_char(Liste  * maListe, FILE*);

			void parcourandbit2(Liste * maListe, FILE*);
	        void parcourandbit(Cell  * maListe, FILE* trad, int* p_bit, char* prof2, char* cursor);//parcoure la liste est ecrit pour chaque feuille sont codes binaire dans p_bit sans surplus de p_bit

			void print_bin(unsigned int const *binary, const char);// ecrit le binaire dans un fichier avec un espace devant 

			void setbitD(int * bit) { *bit = (*bit) << 1; (*bit) |= 1; }

			void setbitG(int *bit) { *bit = (*bit) << 1; }
			static inline int bitwise(int tab, char mychar, size_t taille, size_t taille_tab) {
				/*printf("elem tab entier :");
				for (int k = taille_tab - taille; k >= 0; k--)
				{
					printf( "%c", (tab & (1u << k) ? '1' : '0'));

				}
				printf("\ntab test: %c mychar : %c \n", tab & (1u << taille_tab - taille) ? '1' : '0', mychar);
				printf("%creturn value \n", (tab & (1u << taille_tab - taille)) ? (((int)mychar - 48) ? '1' : '0') : (((int)mychar - 48) ? '0' : '1'));*/ //gt
				return ((tab & (1u << taille_tab - taille)) ? (((int)mychar - 48) ? 1 : 0) : (((int)mychar - 48) ? 0 : 1));
			}

			void compress(const char* n_compress);

			FILE *open_fichier(const char* n_fichier, const char* param); // open le file avec minimum securite

			void default_append(Cell* c, Liste* maliste);
			
			Liste* recupchar(FILE *,FILE*);
			

			



			R**  creatabfromfile(FILE *trad,const size_t);
			Binaire** creatabfromfileB(FILE *trad, size_t);

			void decompression(const char*,const char*);







			//inutile:

			Cell* mergeList(Cell*, Cell*);
			void splitList(Cell*, Cell**, Cell**);
			void  mergeSort(Cell** curr);

			Cell* reverse_liste2(Cell* beg);
			void  reverse_liste(Liste* maliste);
			Cell* link(Cell*prec, Cell* suiv);
			void print_caraL(FILE* trad, Liste*maliste);
				// AJOUTER DES REALLOC POUR MODIFIER TAILLE APRES CONAISSANCE
			

			#endif //PCH_H

			