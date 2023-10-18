// pch.cpp : fichier source correspondant à l'en-tête précompilé ; nécessaire au bon déroulement de la compilation

#include "pch.h"


// Faut faire une fonction qui supprime les données du fichier puis trie ,compte  et ecrit dans le fichier les char de la liste puis ecrit un \n a la fin de l'ecriture dans trad
size_t nb_liste = 0; // pour oral mais c'est pas recommandé 
size_t longeur_codage = 0;
void tri_insertion(Liste* maliste)
{

	Cell* beg = maliste->bege, *tmp = NULL;



	for (Cell* parcour = maliste->bege; parcour->suiv_L; parcour = tmp) {

		beg = maliste->bege;
		tmp = parcour->suiv_L;
		//marche
		if (beg->occurence >(parcour->suiv_L->occurence)) {
			
			location_append2(NULL, parcour, maliste);


			

		}

		else {
			
			
			for (; beg != parcour && (((beg->suiv_L->occurence) <= (parcour->suiv_L->occurence))); beg = beg->suiv_L);
			if (beg != parcour)location_append2(beg, parcour, maliste);
			

		}


	}

}


void location_append2(Cell * cell_loca, Cell * cell_a, Liste* maliste) //ajoute apres la cell loca
{
	//relie cell_a au membre de la liste
	Cell* tmp = NULL;
	if (cell_a->suiv_L != maliste->ende)tmp = (cell_a->suiv_L)->suiv_L;
	else maliste->ende = cell_a;
	if (cell_loca == NULL) {
		(cell_a->suiv_L)->suiv_L = maliste->bege;
		maliste->bege = cell_a->suiv_L;



	}
	else if (cell_loca->suiv_L) {


		cell_a->suiv_L->suiv_L = cell_loca->suiv_L;


		cell_loca->suiv_L = cell_a->suiv_L; //relie loca au nouveau
		if (!(cell_loca->suiv_L)) { //printf("fin: %p\n",cell_a->suiv_L); 
		maliste->ende = cell_a->suiv_L; }


	}

	cell_a->suiv_L = tmp;




}



	FILE *open_fichier(const char* n_fichier, const char* param) // open le file avec minimum securite
	{
		FILE *fptr;
		errno_t err;

		if ((err = fopen_s(&fptr, n_fichier, param)) != 0)
		{
			printf("Error: %d file: %s  do not exist or corrupt\n", err, n_fichier/*, sterror(err)*/);

			exit(EXIT_FAILURE);
		}
		else { return fptr; }
}
Binaire * creaBinaire() {
	Binaire* monBinaire = NULL;
	if (!(monBinaire = malloc(sizeof(Binaire)))) //libéré  au bit
	{

		/*printf("Description de l erreur : %s\n", strerror(errno)); // faudrait  longjump vers un switch avec une id pour code*/
		printf("Description de l erreur :\n");
		exit(EXIT_FAILURE);

	}
	else
	{
		monBinaire->cara = (char)0;
		monBinaire->bin = 0;
		monBinaire->taille = 1;
		if (!(monBinaire->b_tab = (int*)malloc(1 * sizeof(int)))) {
			printf("ERREUR , pas assez de memoire \n");
			exit(EXIT_FAILURE);
		}
		return monBinaire;
		
	}
}
Liste * creaListe()
{
	Liste * maListe = NULL;

	if (!(maListe = malloc(sizeof(Liste)))) //libéré  au bit
	{

		/*printf("Description de l erreur : %s\n", strerror(errno)); // faudrait  longjump vers un switch avec une id pour code*/
		printf("Description de l erreur :\n");
		exit(EXIT_FAILURE);

	}
	else
	{
		maListe->bege = maListe->ende = NULL; //liste vide
		//printf("La liste par defaut (vide) :%p a ete correctement initialise\n", maListe);//gt
		return maListe;
	}
}
Cell * creaCell(const size_t oc, const unsigned int c)
{
	Cell * maCell = NULL;
	if (!(maCell = malloc(sizeof(Cell))))
	{
		printf("Description de l erreur : \n"/*%s , strerror(errno)*/); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);




	}
	else {
		maCell->occurence = oc;
		maCell->val_c = c;
		//maCell->prec_L = prec; avec ajout prec arg fonction
		maCell->suiv_L = NULL;
		maCell->pere_A = NULL;
		maCell->filsD_A = NULL;
		maCell->filsG_A = NULL;
		//printf("La Cell: %p a ete correctement initialise\n", maCell);//gt
		return maCell;

	}
}

Cell * creaNode(Cell * min1, Cell   * min2)//creer un noeud avec les 2 fils connu 
{
	Cell * maCell = NULL;
	if (!(maCell = malloc(sizeof(Cell))))
	{
		printf("Description de l erreur :\n"/*%s , strerror(errno)*/); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);


	}
	else {
		maCell->occurence = min1->occurence + min2->occurence;

		maCell->val_c = (isfeuille(min1) ? (isfeuille(min2) ? 2 : min2->val_c + 1) : (isfeuille(min2) ? min1->val_c + 1 : min1->val_c + min2->val_c));

		//printf("%d\n", maCell->val_c);//gt
		maCell->suiv_L = NULL;
		maCell->pere_A = NULL;

		maCell->filsG_A = min1;
		maCell->filsD_A = min2;// le plus grand
		
		//printf("Le node: %p a ete creer par defaut avec: %p et %p\n", maCell, min1, min2);//gt
		//printf("%d , %p , %p , %p , %p", maCell->occurence, maCell->suiv_L, maCell->pere_A, maCell->filsD_A, maCell->filsG_A); //gt
		return maCell;

	}
}
void default_append(Cell* c, Liste* maliste)
{

	if (is_emptyL(maliste)) {
		maliste->ende = c;
		maliste->bege = c;
	}
	else {
		(maliste->ende->suiv_L) = c;
		maliste->ende = c;
	}
}
Cell* chercheL(Cell *C, Liste* L)// return l'antecedent de C->occurence dans la liste
{
	Cell* beg = L->bege;
	
	/*if (beg->occurence > C->occurence || beg->occurence == (C->occurence) && (isfeuille(beg)||!isfeuille(beg) && (beg->val_c > C->val_c))){ 
		return NULL;//(isfeuille(beg) && beg->occurence == (C->occurence)) a voir 
	}*/ //evolutif

	for (; beg->suiv_L != NULL &&( beg->suiv_L->occurence < C->occurence || beg->suiv_L->occurence == (C->occurence) && ( !isfeuille(beg->suiv_L) || isfeuille(beg->suiv_L) && (beg->suiv_L->val_c < C->val_c)));beg = beg->suiv_L);
	//debugg
	return beg;
}



void location_append(Cell * cell_loca, Cell * cell_a,Liste* maliste) //ajoute apres la cell loca
{
	//relie cell_a au membre de la liste 
	//evolutif
/*	if(cell_loca==NULL) {
		cell_a->suiv_L = maliste->bege;
		maliste->bege = cell_a;
		

	}
	else */if (cell_loca->suiv_L) {


		cell_a->suiv_L = cell_loca->suiv_L;


		cell_loca->suiv_L = cell_a; //relie loca au nouveau
	

	}
	else 
	{
		

		cell_loca->suiv_L = cell_a;


		maliste->ende = cell_a;
	

	}
	


}
void appendtri(Cell * cell_a,Cell* min2, Liste* liste_n)
{

	if (is_emptyL(liste_n))
	{
		min2->suiv_L = cell_a;
		cell_a->suiv_L = NULL;

		liste_n->bege = cell_a;
		liste_n->ende = cell_a;
	
		//++nb_elem_L;//pour le debug apres enlever

	}
	else if(liste_n->bege->occurence > cell_a->occurence || liste_n->bege->occurence == (cell_a->occurence) && (isfeuille(liste_n->bege) || !isfeuille(liste_n->bege) && (liste_n->bege->val_c > cell_a->val_c))) {
		min2->suiv_L = cell_a;
		cell_a->suiv_L = liste_n->bege;
		liste_n->bege = cell_a;
	}
	else
	{
		location_append(chercheL(cell_a, liste_n), cell_a,liste_n);
	}
	
	int i = 1;
	/*for (Cell* parcour = liste_n->bege; parcour; parcour = parcour->suiv_L,++i)
	{
		if (isfeuille(parcour))
		{
			printf("La feuille : %p a pour caractere: %c occurence: %d suivant :%p \n", parcour, parcour->val_c, parcour->occurence, parcour->suiv_L);
		}
		else {

			printf("Le node : %p a pour nb_feuille %d occurence: %d suivant :%p  filsD: %p filsG: %p \n", parcour, parcour->val_c, parcour->occurence, parcour->suiv_L, parcour->filsD_A ,parcour->filsG_A);
		}
  }
	printf("FIN APPEND \n \n");*///gt
	

}

Liste* recupchar(FILE* fptr, FILE* trad) // recup les char les mets dans une liste et écrit le nombre different de char lu  dans un fichier
{

	fseek(fptr, 0, SEEK_SET);
	Liste* maliste = creaListe();
	size_t counter = 0;
	char huff = getc(fptr); // le char qui va nous servir a remplir la liste
	Cell* parcour = maliste->bege; // la cell qu va nous permettre de parcourir la liste qui se fait remplir 
	while (huff != EOF) // on fait la boucle tant qu il reste des char dans le fichier
	{

		for (parcour = maliste->bege; parcour && huff != (parcour->val_c); parcour = parcour->suiv_L);
		
		if(parcour)++parcour->occurence;
		else{
			default_append(creaCell(1, huff), maliste);// on cree une nouvel cell avec la valeur de huff et on l append dans la liste trie
			++counter;
		}
		++nb_liste;
		huff = getc(fptr);
	}
	fprintf(trad, "%d", counter);
	return maliste;// on return la liste complete 

}


void creaArbre(Liste* maListe)
{
	Cell* min1 = NULL, *min2 = NULL, *node = NULL;
	Cell* beg_arbre = maListe->bege;
	while ((maListe->bege) != (maListe->ende))// si la liste a plus d'un elem
	{
		min1 = maListe->bege;//min 1 et min 2 prennent les valeurs des premiers elem car la liste est trié 
		min2 = min1->suiv_L;
		maListe->bege = min2->suiv_L; // dans le cas 2 elem maListe->bege sera egal a NULL donc le isemptyliste va proc
		//min1->suiv_L = min2->suiv_L = NULL;



		node = creaNode(min1, min2); //creer le node
		min1->pere_A = min2->pere_A = node;//  rattache le node a ses fils 

		
	 appendtri(node,min2, maListe);// rattache le node a la liste
	}
	

}
void printArbre(Liste* maliste)
{
	printf("Root :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f \n", maliste->bege, maliste->bege->filsG_A, maliste->bege->filsD_A,(float)maliste->bege->occurence/nb_liste);
	Cell* root = maliste->bege,*curr=root->filsG_A;
	if (isfeuille(curr)) {
		printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", curr,curr->pere_A, curr->val_c, (float)curr->occurence / nb_liste);
		if (isfeuille(curr->suiv_L)) {
			printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", curr->suiv_L,curr->suiv_L->pere_A, curr->suiv_L->val_c, (float)curr->suiv_L->occurence / nb_liste);
			return;
		}
		else {
			printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", curr->suiv_L,curr->suiv_L->filsG_A, curr->suiv_L->filsD_A,(float)curr->suiv_L->occurence / nb_liste);
			curr = curr->suiv_L;

		}
	}
	else {
		printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", curr,curr->filsG_A,curr->filsD_A,(float)curr->occurence / nb_liste);
		if (isfeuille(curr->suiv_L)) printf("Feuille4 :%p cara :%c occurence :%f\n ", curr->suiv_L, curr->suiv_L->val_c, (float)curr->suiv_L->occurence / nb_liste);
		else printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", curr->suiv_L, curr->suiv_L->filsG_A, curr->suiv_L->filsD_A, (float)curr->suiv_L->occurence / nb_liste);
	}
	printArbre2(root,curr);
	
}//a refaire propre
void printArbre2(Cell* root,Cell*curr)
{
	if (curr != NULL){
		Cell* parcour = curr->filsG_A;
		
	if (isfeuille(parcour)) {
		for (; parcour != root->filsG_A && isfeuille(parcour) ; parcour = parcour->suiv_L)
		{
			printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", parcour,parcour->pere_A, parcour->val_c,(float)parcour->occurence / nb_liste);
		}
		if (parcour == root->filsG_A)return;
	}
	
	Cell* tmp = parcour;
	for (; parcour != root->filsG_A; parcour = parcour->suiv_L)
	{
	
		if(isfeuille(parcour))printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", parcour, parcour->pere_A, parcour->val_c, (float)parcour->occurence / nb_liste);
		else printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", parcour,parcour->filsG_A, parcour->filsD_A, (float)parcour->occurence / nb_liste);
	}
	printArbre2(curr,tmp);
	}
}
void binary_char(Liste  * maListe, FILE* trad)
{
	if ((maListe->bege) != (maListe->ende)) { printf("Erreur vous ne pouvez pas appeler cette fonction sur une liste comptenant plus de 2 elem\n"); exit(EXIT_FAILURE); }
	else {
		parcourandbit2(maListe, trad);
	}

}

void parcourandbit2(Liste  * maListe, FILE*trad) {
	
	Cell* root = maListe->bege;
	char prof = 0,cursor=0;
	unsigned int p_bit = 0;
	parcourandbit(root, trad, &p_bit, prof,&cursor);
	printf("%d\n", cursor);
	p_bit = (p_bit << 32 - cursor);
	fprintf(trad, "%c%d", '\0',p_bit);
	printf("P_BIT: %zu\n", p_bit);
	
	for (char i =1; i <= 32; ++i)
	{
		printf("%c", (p_bit & (1u << 32-i)) ? '1' : '0');
	}
	printf("\n");
	free(maListe);
	//printf("La liste : %p a ete delete\n", maListe);//gt
	maListe = NULL;
}


void parcourandbit(Cell  * maListe, FILE* trad ,int* p_bit,char prof2,char* cursor )//parcoure la liste est ecrit pour chaque feuille sont codes binaire dans p_bit sans surplus de p_bit
{
	
	
	if (!isfeuille(maListe->filsD_A)) { // si c un node <=> pas une feuille 

		setbitD(p_bit); ++prof2; ++(*cursor);
		if ((cursor) == 32){
			print_bin(p_bit, *cursor);
			printf(" PBIT : %zu\n", *p_bit);
			fprintf(trad, "%c%d",'\0', *p_bit);
			*p_bit = 0; *cursor = 0;
		}
		parcourandbit(maListe->filsD_A, trad,p_bit,prof2,cursor); //<=> parcourandbit(maListe->filsD_A, p_bitD ,&p_bitG2);


	}
	else {// si c une feuille 
		setbitD(p_bit); ++prof2; ++(*cursor);
		longeur_codage += (int)prof2;
		if ((*cursor)==32) {
			print_bin(p_bit, *cursor);// ATTENTION si sequence commence par NULL -> taille après sequence si prof >256 danger
			printf(" PBIT : %zu\n", *p_bit);
			fprintf(trad, "%c%d", '\0', *p_bit);
			*p_bit = 0; *cursor = 0;
		}
		fprintf(trad, "%c", maListe->filsD_A->val_c);
		fprintf(trad, "%c", prof2);
		printf("Caractere:%c  prof:%d codage:%d\n", maListe->filsD_A->val_c, prof2,*p_bit);//pour oral
		printf("\n");//pour oral
		free(maListe->filsD_A);
		//printf("La feuille droite :%p  avec cara %c et prof :%d a ete delete\n", maListe->filsD_A,(char) (maListe->filsD_A->val_c), prof);//gt

		maListe->filsD_A = NULL;
		
	}
	if (!isfeuille(maListe->filsG_A)) {


		setbitG(p_bit); ++(*cursor);
		//printf("p_bitG : %zu\n", *p_bit);
		if ((*cursor) == 32) {
			print_bin(p_bit, *cursor);// ATTENTION si sequence commence par NULL -> taille après sequence si prof >256 danger
			printf(" PBIT : %zu\n", *p_bit);
			fprintf(trad, "%c%d", '\0', *p_bit);
			*p_bit = 0; *cursor = 0;
		}

		parcourandbit(maListe->filsG_A,trad,p_bit,prof2,cursor); //<=> parcourandbit(maListe->filsD_A, p_bitD ,&p_bitG2);
		free(maListe);
		//printf("Le  node : %p a ete delete\n", maListe);//gt
		maListe = NULL;
		
	}
	else {
		setbitG(p_bit); ++(*cursor);
		longeur_codage += (int)prof2;
		if ((*cursor) == 32) {
			print_bin(p_bit, *cursor);// ATTENTION si sequence commence par NULL -> taille après sequence si prof >256 danger
			printf(" PBIT : %zu\n", *p_bit);
			fprintf(trad, "%c%d", '\0', *p_bit);
			*p_bit = 0; *cursor = 0;
		}
		fprintf(trad, "%c", maListe->filsG_A->val_c);
		fprintf(trad, "%c", prof2);
		printf("Caractere :%c  prof:%d codage:%d\n", maListe->filsG_A->val_c, prof2, *p_bit);//pour oral
		
		printf("\n");//pour oral
		//printf("La feuille gauche :%p  avec cara %c et prof : %d a ete delete\n", maListe->filsG_A, (char)(maListe->filsG_A->val_c),prof );//gt
		free(maListe->filsG_A);
		maListe->filsG_A = NULL;
	
		free(maListe);
		//printf("Le  node  : %p a ete delete\n", maListe);//gt
		maListe = NULL;

		

	}



}

void print_bin(unsigned int const *binary, const char prof)// ecrit le binaire dans un fichier avec un espace devant 
{
	 //pour oral
	/*char *str=malloc(prof+1) ;
	for (size_t i=0; i<prof; i++)
	{str[i] = (*binary & (1u <<(prof-i-1)) ? '1' : '0');
	}
		str[prof] = '\0';
	printf("%s\n", str);
	fprintf(recup, "%s ",str);*/
	
	for (char i = prof-1; i >=0 ; --i)
	{
		printf("%c", (*binary & (1u << i)) ? '1' : '0');
	}
	printf("\n");
	//fprintf(recup, "%c", *binary);
    
}
R* creaR(const char str, const int*tab, const size_t taille,const size_t curr)
{
	R * mastruct = NULL;
	if (!(mastruct = malloc(sizeof(R))))
	{
		printf("Description de l erreur : \n"); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);

	}
	else {
		mastruct->cara = str;
	    mastruct->moninte = NULL;
		mastruct->taille = taille;
		//printf("taill creae %d et moi %d\n", taille, mastruct->taille);
		mastruct->cursor = curr;
		//printf("er %d\n", curr);
			if (!(mastruct->moninte = malloc(taille*sizeof(int))))
			{
				printf("Description de l erreur : \n"); // faudrait  longjump vers un switch avec une id pour code
				exit(EXIT_FAILURE);

			}
			if(taille)
			{
			for (size_t i = 0; i < taille; i++)mastruct->moninte[i] = tab[i];
			
			mastruct->cursor +=(taille - 1) * 32;
			
		}
	}
		return mastruct;
}
void print_masque(R* masque)
{
	if(masque->cara!='\0')printf("CARA MASQUE:%c\n", masque->cara);
	else { printf("KEY \n"); }
	size_t p = 0;
	printf("LOL : %d taille %d\n", p, masque->taille);
	for ( ;p < masque->taille-1; p++)
	{
		for (size_t k = 0; k <32; k++)
		{
			(masque->moninte[p] & (1u << 31 - k)) ? printf("%c", '1') : printf("%c", '0');
		}
		printf(" ");
	}
	printf(" ");
	for (size_t k = 0; k < masque->cursor; k++) {
		(masque->moninte[p] & (1u << 31 - k)) ? printf("%c", '1') : printf("%c", '0');
	}
	printf("<-- masque\n");
}
void change_masque(R* masque, const int* tab,const size_t taille, const size_t cursor1 ,const size_t cursor2)
{
	//cursor 1 nb_bit inclue dans first int <=> cursor1 à 32 
	//cursor 2 nb_bit dans last int  <=> de 0 à cursor2

	if (!masque->taille) // if masque = first key
	{
		//printf("je crea masque : %d taille\n",taille);
		masque->cara = '\0';
		masque->cursor = 0;
		if (!(realloc(masque->moninte,taille * sizeof(int))))
		{
			printf("Description de l erreur : \n"); // faudrait  longjump vers un switch avec une id pour code
			exit(EXIT_FAILURE);

		}
		masque->taille = taille;
		if (taille) {
			for (size_t i = 0; i < taille; i++) { masque->moninte[i] = tab[i]; }
			masque->cursor = (taille - 1) * 32;
		}
	    masque->cursor += cursor2;
		//printf("lolioki %d\n", cursor2);
		
	}
	else {
		masque->moninte[masque->taille - 1] &= ~(1u << 32 - masque->cursor);
		/*print_masque(masque);
		printf("\n");*/
		//printf("c1 : %d\n", masque->cursor);
		/*for (size_t k = 1; k <= 32; k++)
		{
			(~(1u << 32- masque->cursor) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
		}*/
		if (cursor1>cursor2)//si cursor2 se trouve dans autre int 
		{
			int rest = cursor1 - masque->cursor;
			if (rest>0 && rest > cursor2)//si les bits restant dans masque sont strictement suffisant pour stocker cursor1
			{
					masque->moninte[masque->taille - 1] |= ((~0 << 32 - masque->cursor) ^ (~0 << 32 - masque->cursor - (cursor2 - (cursor1 + 1))));
		
			}
			else {//rest<=0 ou rest<=cursor2
				masque->moninte[masque->taille - 1] |= (~0 >> masque->cursor);
				masque->cursor += 32 - masque->cursor;
				if (rest <= 0 && 32 - cursor2 + rest <= 0)
				{
					realloc(masque->moninte, (masque->taille + 2) * sizeof(int));
					~(masque->moninte[masque->taille++] &= 0);
					masque->moninte[masque->taille++] |= (~0 << 32 + 32 - cursor2 + rest);
					masque->cursor += cursor2 - rest;

				}
				else {

					realloc(masque->moninte, (masque->taille + 1) * sizeof(int));
					masque->moninte[masque->taille++] |= (~0 << 32 - (cursor2 - rest));
					masque->cursor += cursor2 - rest;
				}
			}
			if (taille > 3)
			{

			}

		}
		else {
			print_masque(masque);
			printf("LOL\n");
			while (!(masque->moninte[masque->taille - 1] & (1u << 32-masque->cursor))) {
				--masque->cursor; //printf("lol %d\n", masque->cursor);
				if (!(masque->cursor % 32)) {
					--masque->taille;
					if (masque->taille > 0) {
						masque->cursor = 32;
						printf("masque _taille : %d masque cursor reset : %d\n", masque->taille, masque->cursor);
					}
					else { printf("FIN\n"); }
				}
			}
		}
	}
}

R** traite_tab(FILE *trad, const size_t sz_row)
{

	char cara = (char)0;
	size_t o = 0;
	char  recup[65] = "\0"; // diminuer 65 stringe
	int me_int[7] = { 0 }; // car taille char max 256 donc 8 *4bytes
	R **tab = malloc(sz_row * sizeof(R*));
	//printf("%d size _row \n", sz_row);
	if (tab == NULL)
	{  // verif alloc bonne 
		printf("Description de l erreur :\n"); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);
	}

	else {
		size_t k = 0, i = 0, z = 0, cursor_int1 = 0, cursor_int2 = 1;
		//printf("k = %d\n", k);
		R*masque = creaR('\0', NULL, k, 1);//juke
		//printf("%p", masque->moninte);

		while (i < sz_row)
		{
			o = 0;
			while ((cara = getc(trad)) && cara == '\0') {//voir si pas do while pour evite refaire fscan
				fscanf_s(trad, "%d", &(me_int[k++]), sizeof(int));
				printf(" LOL %d\n", me_int[k - 1]);
			}


			do
			{
				recup[o++] = cara;
				//printf("cara: %d\n", cara);
				printf("str : %s\n ", recup);

			} while ((cara = getc(trad)) && cara != '\0');

			fscanf_s(trad, "%d", &(me_int[k]), sizeof(int));

			//printf("%d\n", me_int[k]);
			printf("O: %d\n", o);
			//tab[i] = creaR(recup, me_int, k);
			cursor_int2 = ((recup[1] - (masque->cursor-1)) - (32 - cursor_int1)) % 32;
			change_masque(masque, me_int,k+1, cursor_int1, cursor_int2);
			cursor_int1 = cursor_int2;
			tab[i++] = creaR(recup[0], masque->moninte, masque->taille, recup[1]);
			printf("print tab[ %d ] : cursor %d : \n ",i, tab[i-1]->cursor); print_masque(tab[i-1]);
			//FAIRE CURSOR 1 : beg donc 1=0 indice beg et CURSOR 2 : end donc one past the end
			for (size_t pos_str = 1; recup[2 * pos_str] != '\0'; pos_str++)
			{
				//printf("masque cursor %d  C2:%d  C1:%d  taille: %d\n ", masque->cursor ,cursor_int2 , cursor_int1, recup[2 * pos_str + 1]);
				cursor_int2 = cursor_int1 + (recup[2 * pos_str + 1] - (masque->cursor-1));
				//printf("masque cursor %d  C2:%d  C1:%d\n ", masque->cursor, cursor_int2, cursor_int1);
				//printf("recup taille : %d cursor2:%d , me_int : %d , cursor_int 1 : %d\n", recup[2 * pos_str + 1], cursor_int2, me_int[k], cursor_int1);
				change_masque(masque, &(me_int[k]), 1, cursor_int1, cursor_int2);
		
				cursor_int1 = cursor_int2;
				tab[i++] = creaR(recup[2 * pos_str], masque->moninte, masque->taille, recup[2 * pos_str + 1]);
	
				printf("print tab[i] : cursor %d : ",tab[i-1]->cursor); print_masque(tab[i-1]);
			}
			for (size_t h = 0; h < k; h++) {
				me_int[h] = 0;
			}
			if (masque->taille && cursor_int1 < 32)//eneleve et mettre boucle au dessus pour traité en 2 temps les cursor
			{
				me_int[0] = me_int[k];
				me_int[k] = 0;
				k = 1;
			}
			else {
				cursor_int1 = 0;
				me_int[k] = 0;
				k = 0; }
			printf("I :%d\n\n", i);
			
		}
		tab[i] = creaR('\0',NULL,0,0);
		//free(masque->moninte);
		//free(masque);
	}

		return tab;
}
Binaire** creatabfromfileB(FILE *trad, size_t sz_row) // sz row = nb_liste
{
	char recup = (char)0;
	Binaire** tab = malloc(sz_row * sizeof(Binaire*)); // pour chaque caractere on creer une struct R et on la mets dans un tableau pour un acess + facile  remarque : marche aussi avec liste simplement chaine mais deja fait

	if (tab == NULL) {  // verif alloc bonne 
		printf("Description de l erreur :\n"); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);
	}
	else {
	
		size_t j = 0, lol = 0, counter = 0;
		// consome le saut de ligne volontaire getc(fptr);
		for (size_t i = 0; i < sz_row; i++) // not necessary i<sz_row
		{
			tab[i] = creaBinaire();
			j = 0;
			counter = 0;
			fscanf_s(trad, "%d", &(lol), sizeof(int));
			while ((recup = getc(trad)) && (recup == ':')) {
				++counter;
				if (counter >= tab[i]->taille) {
					if (!(tab[i]->b_tab = (int*)realloc(tab[i]->b_tab, (2 * counter) * sizeof(int)))) {
						printf("ERREUR , pas assez de memoire \n");
						exit(EXIT_FAILURE);
					}
					else
					{
						tab[i]->taille = 2 * counter;
					}
				}
				tab[i]->b_tab[j++] = lol;
				//printf("valeur tab append : %d \n", tab[i]->b_tab[j - 1]);
				fscanf_s(trad, "%d", &(lol), sizeof(int));

			}
			tab[i]->b_tab[j++] = 1;
			for (; j < tab[i]->taille; j++)tab[i]->b_tab[j] = 0;
			//for (size_t o = 0; o < tab[i]->taille; o++)printf("indexe : %d valeur: %d", o, tab[i]->b_tab[o]);//gt
			
			tab[i]->taille = lol;
			tab[i]->cara = getc(trad);
			//printf(" char prec ; %c  taille :%d\n", recup, tab[i]->taille);//gt
			for (size_t k = 0; k < tab[i]->taille && (recup = getc(trad)) && recup != EOF; k++) {//&& recup!='\n'

				(((int)recup - 48) ? setbitD(&(tab[i]->bin)) : setbitG(&(tab[i]->bin)));
				//printf(" current char ; %c \n", recup);//gt
			}
			/*printf("Real: ");	
			for (int k = (tab[i]->taille) - 1; k >= 0; k--)
			{

				printf("%c", tab[i]->bin & (1u << k) ? '1' : '0');
			}
			printf("\n");*///gt
			
			

		}

	}
	return tab;
}

void compress(const char* n_compress)
{

	FILE* fptr = open_fichier(n_compress, "a+"), *comp = open_fichier("compress.txt", "w+");// ouvre le fichier en mode ecriture + lecture et écrase son contenu

	Liste* maliste = recupchar(fptr, comp);//creer liste feuille
	tri_insertion(maliste);
	//mergeSort(&(maliste->bege));
	
	//reverse_liste(maliste);
	//print_caraL(comp, maliste);
	
    creaArbre(maliste);//creer arbre
	printArbre(maliste); //oral
	
	binary_char(maliste, comp);//creer fichier de trad
	fseek(comp, 0, SEEK_SET);//flush le trad
	size_t sz_row = 0;
	fscanf_s(comp, "%d", &sz_row, sizeof(unsigned int));
	R**tab = traite_tab(comp, sz_row);
	/*for (size_t y = 0; y < sz_row; y++)
	{
		print_masque(tab[y]);
	}*/
	
	/*
	//fflush(fptr); ou // flcose(comp); comp=open_fichier("compress.txt","r"); //meilleur solution avec comp en +w au debut 
	

	R** tab = creatabfromfile(comp,sz_row); //creer un tab 

	char mychar = '\0';
	size_t i = 0, k = 0, taille_suiv = 0, taille_prec = 0, diff_c = 0, diff_nc = 0, taille_global = 0, inter = 0;
	int tmp = 0;
	unsigned int boo = 1;
	//long int pos = ftell(comp);
	fseek(fptr, 0, SEEK_SET);
	

	while (!(feof(fptr))) //recupere les char du fichier non compresse
	{


		while (fscanf_s(fptr, "%c", &mychar, 1) != EOF)
		{
			printf("cara1 :%c  ", mychar); // voir comment permet afficher \n 
			boo=1; 
			taille_suiv = 0, taille_prec = 0, diff_c = 0,diff_nc=0, taille_global = 0, inter = 0;
			for(i=0;boo;++i)
			{
				for ( k = 0; tab[i]->stringe[2 * k] != '\0'; ++k)
				{
					printf("cara2 : %c\n", tab[i]->stringe[2 * k]);
					if (tab[i]->stringe[2 * k] == mychar) {

						boo = 0;
						break;
					}
				}
			}
			for (size_t z = 0; tab[i]->stringe[2 * z] != '\0'; ++z)taille_suiv += tab[i]->stringe[2 * z + 1];
			for (size_t z = 0; z<k ; ++z)taille_prec += tab[i]->stringe[2 * z + 1];
			taille_global = taille_suiv + (taille_prec % 32);
			diff_c = 32 - taille_global;
			if (k == 0)
			{
				diff_nc = tab[i]->stringe[1] -(taille_global-taille_suiv); // nb bit non contenu dans le string 
				if (diff_nc < 32 && i) {
					inter = tab[i - 1]->moninte[(tab[i - 1]->taille) - 1]; //cherche contient le précédent int 
					for (size_t p = 0; p < diff_nc; p++)
					{
						(inter & (1u << (diff_nc - p))) ? setbitD(&tmp) : setbitG(&tmp); // les diff premier bit 
					}
					
				}
				else if(diff_nc) // diff>32
				{
					for (size_t p = 0; p < tab[i]->taille - 1; p++)
					{
						fprintf(comp, "%d", tab[i]->moninte[p]);
					}

				}
				for (size_t var =diff_c-taille_prec; var>=taille_suiv; var--)
				{
					(tab[2 * k]->moninte[0] & (1u << (var))) ? setbitD(&tmp) : setbitG(&tmp);
				}
			}
			else {
				inter = tab[i]->moninte[taille_prec / 32];
				taille_prec %= 32;//enleve autre bit
				for (size_t p = diff_c - taille_prec; p >= taille_prec - tab[i]->stringe[2 * k + 1]; p--)
				{
					(inter & (1u << p)) ? setbitD(&tmp) : setbitG(&tmp);
				}
			}
			fprintf(comp, "%d", tmp);

		}



	}
	*/
	/*
	for(size_t w = 0; w < sz_row; w++) // free le tab de struct R a voir si on fait pas une fonction : freetabR
	{
		free(tab[w]->moninte);
		free(tab[w]);
		tab[w] = NULL;
	}
	free(tab);
	tab = NULL;
	*/
	fclose(comp);
	comp = NULL;
	fclose(fptr);
	fptr = NULL;



} 


void decompression(const char * compress,const char* decompress)
{
	FILE *comp = open_fichier(compress, "r"), *result = open_fichier(decompress, "w");
	size_t sz_row = 0;
	fscanf_s(comp, "%d", &sz_row, sizeof(int));
	getc(comp);
	Binaire** tab = creatabfromfileB(comp, sz_row);
	size_t i = 0;
	//unsigned int binaire = 0;
	size_t taille_bin = 0;
	char mychar = (char)0;
	size_t k = 0;
	
	
	while (!(feof(comp))) //recupere les char du fichier non compresse
	{


		//binaire = 0;
		i = 0;
		taille_bin = 0;
		k = 0;
		while ((tab[i]->taille != taille_bin )&&(mychar = getc(comp)) && mychar != EOF )
		{


			++taille_bin;
			//printf("taille: %d k_bin: %d  capter :%c \n", taille_bin, k,mychar); //gt
			//(((int)mychar - 48) ? setbitG(&binaire) : setbitD(&binaire));

			//for(; taille_bin == tab[2][i] && (tab[1][i] & binaire)!=binaire;i+=tab[3][i]); // ((tab[1][i] & binaire) != binaire
			//printf("%d : iter\n", i); //gt
			
			if(!(bitwise(tab[i]->bin, mychar, taille_bin, tab[i]->taille)))
			{
				
				//printf("%d : shift\n", tab[i]->b_tab[k]);//gt
			    i += tab[i]->b_tab[k];
				
				k = 0;
				//printf("%d : iter2\n", i);//gt
			}else k++;
			
			//sinon on continue la comparaison  avec binaire avec 1 bit en plus

		}
		if (taille_bin > 0) {
			//printf("cara %c\n", tab[i]->cara);//gt
			fprintf(result, "%c", tab[i]->cara);
		}


	}
	for (size_t k = 0; k < sz_row; k++)
	{

		free((tab[k]->b_tab));
		tab[k]->b_tab = NULL;
		free(tab[k]);
		tab[k] = NULL;


	}
	free(tab);
	tab = NULL;
	// ferme les flux :
	fclose(comp);
	comp = NULL;
	fclose(result);
	result = NULL;
}

//inutile:

void  reverse_liste(Liste* maliste)
{
	Cell* beg = maliste->bege, *endee = maliste->ende;
	if (maliste->bege->suiv_L) {
		beg = reverse_liste2(beg);
		beg->suiv_L = NULL;
		maliste->ende = beg;
		maliste->bege = endee;
	}

}
Cell* reverse_liste2(Cell* beg)
{
	return(beg->suiv_L == NULL ? link(NULL, beg) : link(reverse_liste2(beg->suiv_L), beg));
}
Cell* link(Cell*prec, Cell* suiv)
{
	if (prec != NULL)prec->suiv_L = suiv;
	return suiv;
}

Cell* mergeList(Cell* first, Cell* second) {
	//merging two sorted list
	Cell* new = NULL;
	//base cases
	if (first == NULL)
		return second;
	if (second == NULL)
		return first;
	//recursively merge
	if (first->occurence <= second->occurence) {
		new = first;
		new->suiv_L = mergeList(first->suiv_L, second);
	}
	else {
		new = second;
		new->suiv_L = mergeList(first, second->suiv_L);
	}

	return new;

}

void splitList(Cell* head, Cell** first, Cell** second) {
	//similar to flyod's tortoise algorithm
	Cell* curr_first = head;
	Cell* curr_second = head->suiv_L;

	while (curr_second != NULL) {
		curr_second = curr_second->suiv_L;
		if (curr_second != NULL) {
			curr_first = curr_first->suiv_L;
			curr_second = curr_second->suiv_L;
		}
	}

	*first = head;
	*second = curr_first->suiv_L;
	//spliting
	curr_first->suiv_L = NULL;
}

void mergeSort(Cell** curr) {
	Cell* head = *curr;
	Cell* first, *second;
	//base case
	if (head == NULL || head->suiv_L == NULL) {
		return;
	}
	//split the list in two halves
	splitList(head, &first, &second);
	//recursively sort the two halves
	mergeSort(&first);
	mergeSort(&second);
	//merge two sorted list
	*curr = mergeList(first, second);


	return;

}

void print_caraL(FILE* trad, Liste*maliste)
{

	Cell * parcour = maliste->bege;
	for (; parcour->suiv_L; parcour = parcour->suiv_L)fprintf(trad, "%c", parcour->val_c);//gt
	fprintf(trad, "%c", parcour->val_c);//gt
	//maliste->ende = parcour;





}
