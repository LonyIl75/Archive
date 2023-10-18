// pch.cpp : fichier source correspondant à l'en-tête précompilé ; nécessaire au bon déroulement de la compilation

#include "pch.h"


// Faut faire une fonction qui supprime les données du fichier puis trie ,compte  et ecrit dans le fichier les char de la liste puis ecrit un \n a la fin de l'ecriture dans trad

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
		printf("La liste par defaut (vide) :%p a ete correctement initialise\n", maListe);
		return maListe;
	}
}
Cell * creaCell(const size_t oc, const char c)
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
		printf("La Cell: %p a ete correctement initialise\n", maCell);
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
		//maCell->val_c = EXIT_FAILURE ou (char)0 A VOIR
		//maCell->prec_L = prec; avec ajout prec arg fonction
		maCell->suiv_L = NULL;
		maCell->pere_A = NULL;
		maCell->filsD_A = min1;// le plus petit 
		maCell->filsG_A = min2;
		printf("Le node: %p a ete creer par defaut avec: %p et %p\n", maCell, min1, min2);
		//printf("%d , %p , %p , %p , %p", maCell->occurence, maCell->suiv_L, maCell->pere_A, maCell->filsD_A, maCell->filsG_A);
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
	if (beg->occurence < (C->occurence)) { return NULL; }
	while (beg->suiv_L != NULL && (((beg->suiv_L)->occurence) <= (C->occurence)))beg = beg->suiv_L;
	return beg;
}

/*
void location_append(Cell * cell_loca, Cell * cell_a) //ajoute apres la cell loca
{
	//relie cell_a au membre de la liste
	cell_a->suiv_L = cell_loca->suiv_L;

	cell_loca->suiv_L = cell_a; //relie loca au nouveau

}
void appendtri(Cell * cell_a, Liste* liste_n)
{
	if (is_emptyL(liste_n))
	{
		cell_a->suiv_L = NULL;

		liste_n->bege = cell_a;
		liste_n->ende = cell_a;
		//++nb_elem_L;//pour le debug apres enlever

	}
	else
	{
		location_append(chercheL(cell_a, liste_n), cell_a, liste_n);
	}


}*/

Liste* recupchar(FILE* fptr, FILE* trad) // recup les char les mets dans une liste et écrit le nombre different de char lu  dans un fichier
{

	fseek(fptr, 0, SEEK_SET);
	Liste* maliste = creaListe();
	size_t counter = 0;
	char huff = getc(fptr); // le char qui va nous servir a remplir la liste
	Cell* parcour = maliste->bege; // la cell qu va nous permettre de parcourir la liste qui se fait remplir 
	while (huff != EOF) // on fait la boucle tant qu il reste des char dans le fichier
	{

		while (parcour)// on test que parcour != NUll car si parcour ==NULL <=> one past the end => on a process toute la liste 
		{

			if (huff == (parcour->val_c)) { parcour->occurence += 1;  break; } // on a trouve huff dans la liste on increment le counter d occurence et on stop la boucle

			parcour = parcour->suiv_L; // on a pas trouve huff on increment parcour

		}
		if (!(parcour)) // parcour ==NULL donc huff a pas ete trouve 
		{
			default_append(creaCell(1, huff), maliste);// on cree une nouvel cell avec la valeur de huff et on l append dans la liste trie
			++counter;

		}
		parcour = maliste->bege;//on reset parcour 
		huff = getc(fptr);
	}
	fprintf(trad, "%d", counter);

	return maliste;// on return la liste complete 

}
void tri_liste(Liste* maListe)
{

	//trie liste


}
void listedel(Liste* liste)
{
	Cell*parcour = liste->bege;
	Cell* tmp = NULL;
	while (parcour)
	{
		tmp = parcour;
		parcour = parcour->suiv_L;
		printf("Le char :%c", tmp->val_c);
		free(tmp);
		printf("La cell %p : a ete delete\n", tmp);
		tmp = NULL;
	}
}
void print_caraL(FILE* trad, Liste*maliste)
{
	Cell * parcour = maliste->bege;
	size_t counter = 0;
	while (parcour)
	{
		fprintf(trad, "%c", parcour->val_c);
		parcour = parcour->suiv_L;

	}

}
void creaArbre(Liste* maListe)
{
	Cell* min1 = NULL, *min2 = NULL, *node = NULL;
	while ((maListe->bege) != (maListe->ende))// si la liste a plus d'un elem
	{
		min1 = maListe->bege;//min 1 et min 2 prennent les valeurs des premiers elem car la liste est trié 
		min2 = min1->suiv_L;
		maListe->bege = min2->suiv_L; // dans le cas 2 elem maListe->bege sera egal a NULL donc le isemptyliste va proc
		min1->suiv_L = min2->suiv_L = NULL;



		node = creaNode(min1, min2); //creer le node
		min1->pere_A = min2->pere_A = node;//  rattache le node a ses fils 

		default_append(node, maListe);// rattache le node a la liste
	}

}
void delArbre(Cell* parcour)
{
	if (!isfeuille(parcour)) {

		delArbre(parcour->filsD_A);
		delArbre(parcour->filsG_A);
		printf("L occurence :%d , le node : %p a ete delete\n ", parcour->occurence, parcour);
	}
	else { printf("Le char :%c , la cell : %p a ete delete\n ", parcour->val_c, parcour); }
	free(parcour);
	parcour = NULL;

}
void binary_char(Liste  * maListe, FILE* trad)
{
	if ((maListe->bege) != (maListe->ende)) { printf("Erreur vous ne pouvez pas appeler cette fonction sur une liste comptenant plus de 2 elem\n"); exit(EXIT_FAILURE); }
	else {
		parcourandbit2(maListe, trad);
	}

}

void parcourandbit2(Liste  * maListe, FILE*trad) {
	unsigned int p_bitG = 0;// a voir si on fait pas des alloc dynamique 
	unsigned int p_bitD = 0;
	size_t prof = 0;
	Cell* root = maListe->bege;
	parcourandbit(root, &p_bitG, &p_bitD, prof, trad);
	free(maListe);
	printf("La liste : %p a ete delete\n", maListe);
}


void parcourandbit(Cell  * maListe, unsigned int * p_bitG, unsigned int * p_bitD, size_t prof, FILE* trad)//parcoure la liste est ecrit pour chaque feuille sont codes binaire dans p_bit sans surplus de p_bit
{

	if (!isfeuille(maListe->filsD_A)) { // si c un node <=> pas une feuille 


		(setbitD(p_bitD));
		++prof;
		unsigned int p_bitG2 = *p_bitD;



		parcourandbit(maListe->filsD_A, &p_bitG2, p_bitD, prof, trad); //<=> parcourandbit(maListe->filsD_A, p_bitD ,&p_bitG2);

	}
	else {// si c une feuille 

		++prof;
		setbitD(p_bitD);
		print_bin_file(p_bitD, prof, trad);
		free(maListe->filsD_A);
		printf("La feuille droite :%p a ete delete\n", maListe->filsD_A);

	}
	if (!isfeuille(maListe->filsG_A)) {


		(setbitG(p_bitG));

		unsigned int p_bitD2 = *p_bitG;


		parcourandbit(maListe->filsG_A, &p_bitD2, p_bitG, prof, trad); //<=> parcourandbit(maListe->filsD_A, p_bitD ,&p_bitG2);
		free(maListe);
		printf("Le node : %p a ete delete\n", maListe);
	}
	else {
		setbitG(p_bitG);
		print_bin_file(p_bitG, prof, trad);
		free(maListe->filsG_A);
		printf("La feuille gauche :%p a ete delete\n", maListe->filsG_A);
		free(maListe);
		printf("Le node : %p a ete delete\n", maListe);

	}

	

}

void print_bin_file(int const *binary, size_t prof, FILE* recup)// ecrit le binaire dans un fichier avec un espace devant 
{
	char *str=malloc(prof+1) ;
	for (size_t i=0; i<prof; i++)
	{str[i] = (*binary & (1u <<(prof-i-1)) ? '1' : '0');
	}
	str[prof] = '\0';
	printf("%s\n", str);
	fprintf(recup, "%s ",str);

}


int** creatabfromfileR(FILE *trad) // sz row = nb_liste
{
    fseek(trad, 0, SEEK_SET);//flush le trad
	size_t sz_row = 0;
	fscanf_s(trad, "%d", &sz_row, sizeof(int));
	char recup = (char)0;
	int **tab = malloc(3 * sizeof(int)); // pour chaque caractere on creer une struct R et on la mets dans un tableau pour un acess + facile  remarque : marche aussi avec liste simplement chaine mais deja fait

	if (tab == NULL) {  // verif alloc bonne 
		printf("Description de l erreur :\n"); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);
	}
	else {
		for (size_t i = 0; i < 3; i++) {
			tab[i] = malloc(sz_row * sizeof(int));

		}
		for (size_t i = 0; i < sz_row && (recup = getc(trad)) && recup != EOF; i++) //not necessary EOF
		{
			tab[0][i] = (int)recup; // rempli les structs du tableau avec les caractere dans la premiere ligne du fichier 
			tab[1][i] = 0;
			tab[2][i] = 0;
			printf(" tab cara %c: \n", tab[0][i]);
		}
		// consome le saut de ligne volontaire getc(fptr);
		for (size_t i = 0; i < sz_row; i++) // not necessary i<sz_row
		{
		
			while( (recup = getc(trad)) && recup != EOF && !(isspace(recup)) ) {//&& recup!='\n'
				
				(((int)recup - 48) ? setbitG(&(tab[1][i])) : setbitD(&(tab[1][i])));
				++tab[2][i];
			
			}
			printf("Real: ");
			for (int k = tab[2][i] - 1; k >= 0; k--)
			{

				printf("%c", tab[1][i] & (1u << k) ? '1' : '0');
			}
			printf("\n");
		}

	}
	return tab;
}

void compress(const char* n_compress)
{

	FILE* fptr = open_fichier(n_compress, "a+"), *comp = open_fichier("compress.txt", "w+");// ouvre le fichier en mode ecriture + lecture et écrase son contenu

	Liste* maliste = recupchar(fptr, comp);//creer liste feuille

	print_caraL(comp, maliste);

	creaArbre(maliste);//creer arbre

	binary_char(maliste, comp);//creer fichier de trad
	
	//fflush(fptr); ou // flcose(comp); comp=open_fichier("compress.txt","r"); //meilleur solution avec comp en +w au debut 


	char mychar = '\0';
	size_t i = 0;

	int** tab = creatabfromfileR(comp); //creer un tab 
	//long int pos = ftell(comp);
	fseek(fptr, 0, SEEK_SET);


	while (!(feof(fptr))) //recupere les char du fichier non compresse
	{


		while (fscanf_s(fptr, "%c", &mychar, 1) != EOF)
		{
			//printf("cara1 :%c  ", mychar); // voir comment permet afficher \n 
			i = 0;
			while (mychar != (char)tab[0][i]) {
				i++; ;
			}
			//printf("indexe %d\n", i);// regarde a quel index se trouve mychar ( le caractere du fichier non compresse)


			for (int k = tab[2][i] - 1; k >= 0; k--)
			{
				fprintf(comp, "%c", (tab[1][i] & (1u << k) ? '1' : '0'));

			}
			// pour debug fprintf(comp, "%c", ' ');


		}



	}
	
	/*fprintf(comp, " %d", pos);*/
	
	for (size_t k = 0; k < 3; k++) // free le tab de struct R a voir si on fait pas une fonction : freetabR
	{

		free(tab[k]);
	}
	free(tab);
	
	fclose(comp);
	fclose(fptr);



}
/*unsigned short int bitwise(int tab, int binaire)
{
	while()
}*/
/*
void decompression(const char * compress)
{
	FILE *comp = open_fichier(compress, "r"), *result = open_fichier("resultat.txt", "w");
	int** tab=creatabfromfileR(comp);
	size_t i = 0;
	unsigned int binaire = 0;
	size_t taille_bin = 0;
	char mychar = (char)0;
	
	printf("%d\n", ftell(comp));

	while (!(feof(comp))) //recupere les char du fichier non compresse
	{
		
		taille_bin = 0;
		binaire = 0;
		i = 0;

		while ( fscanf_s(comp, "%c", &mychar, 1)!=EOF) 
		{
			
			
			
			(((int)mychar - 48) ? setbitG(&binaire) : setbitD(&binaire));
			++taille_bin;
			for (; taille_bin == tab[2][i] && bitwise(tab[1][i] , binaire); ++i); // ((tab[1][i] & binaire) != binaire
			if (tab[2][i] == taille_bin) { break; }
			
			//sinon on continue la comparaison  avec binaire avec 1 bit en plus

		}
		if (taille_bin > 0) {
			printf("\n cara %c", tab[0][i]);
			fprintf(result, "%c", tab[0][i]);
		}
		
	}
	for (size_t k = 0; k < 3; k++) 
	{

		free(tab[k]);
	}
	free(tab);
	// ferme les flux :
	fclose(comp);
	fclose(result);
}
*/