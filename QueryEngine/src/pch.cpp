// pch.cpp : fichier source correspondant à l'en-tête précompilé ; nécessaire au bon déroulement de la compilation

#include "pch.h"

//PASSAGE:
/*
PARSEUR FICHIER : INITIALISE LA TABLE AVEC LA VALEUR DU FICHIER 
	APPEL:
	1- CREA_TABID pour initialiser le tableau de table par defaut
	2- OPEN_FICHIER : ouvre le fichier contenant la valeur du tableau
	for loop: cond FICHIER==eof
		3- CREA_CLASS pour initialiser la table dont l'id est le nom recuperer dans le fichier
	fin de l'initialisation des table contenu dans le fichier

CREA_CLASS: INIIALISE UNE TABLE AVEC LE CONTENU D UN FICHIER jusqu'au delimiteur assigner a cette table
	APPEL:
	(Initialise le nom avec le nom passer en argument)
	1-GET_DESCRIPTION : initialise la description d'une table avec le contenu d'un fichier jusqu'au delimiteur assigner a cette description

	for loop: cond recup==fin_de_table 

		2-APPEND_CLASS: initialise une ligne d'un tableau de colonne contenu dans une table grace a un fichier , jusqu'au delimiteur associer a cette ligne
		++nb_ligne;
	fin de l'entree du tableau de colonne

GET_DESCRIPTION:
	for loop : cond recup == fin_description
		GETSTRING2 : permet de remplir une colonne de la description avec un fichier , la saisi s'arrete au delimiteur specifier .
		++nb_colonne
	fin de la description

APPEND_CLASS: INITIALISE UNE LIGNE
	APPEL IMPLICITE:
		for loop: cond recup==fin_de_ligne
			1- APPEND_COLONNE : initialise une colonne d une ligne d'une table grace au contenu d'un fichier , jusqu'au delimiteur associer a cette colonne
		fin de l'entree de la ligne


*/


//TODO: remplacer les void par des int 
//TODO : check si tout my erreur necessaire



//CONSTRUCTOR TABLE:
		//DEFAULT:
		Table * crea_default_table() // constructor par defaut d'une table  return l'objet alloue
{
	//VAR DYNAMIQUE :
		Table* select_table = NULL;
		my_erreur((select_table = (Table*)malloc(sizeof(Table))));

	//printf("LA wtf : %#010x\n",select_table);
	//NB _colonne:
	select_table->nb_attribut = 0;
	select_table->nb_element = 0;
	select_table->tab = NULL;
	select_table->name = NULL;
	select_table->description = NULL;
	return select_table;
}




//CONSTRUCTEUR :

void get_description(FILE* fptr, Table* my_class, const char fin_description, const size_t nb_attribut_max)//peut etre mettre return type en int avec un size_t i dans le for pour return nb valeur lu 
{
	//VAR STATIQUE SCOPE:

		char recup = '\0';
		char* buffer = NULL;

	//DEB DESCRIPTION :

	for (; recup != fin_description && my_class->nb_attribut < nb_attribut_max; ++my_class->nb_attribut, buffer = NULL) // descendre cond my_class->nb_attribut dans boucle et faire un realloc 
	{
		while ((recup = getc(fptr)) && recup != '"');//on se positionne au debut du nom de la colonne

		//RECUPERE LE NOM DE LA COLONNE dans le tableau stockant les descriptions :
		buffer = getString2(fptr, '"', my_class->description[my_class->nb_attribut * 2]);

		if (!testambiguite(buffer, my_class->description, my_class->nb_attribut * 2))my_class->description[my_class->nb_attribut * 2] = buffer;
		else exit(1);

		while ((recup = getc(fptr)) && recup != ':'&& recup != EOF); //on enleve le ':'

		while ((recup = getc(fptr)) && recup != '"');//on se positionne au debut du type de la colonne

		//RECUPERE LE TYPE DE LA COLONNE dans le tableau stockant les descriptions :

		my_class->description[my_class->nb_attribut * 2 + 1] = getString2(fptr, '"', my_class->description[my_class->nb_attribut * 2 + 1]); //recup le type

		while ((recup = getc(fptr)) && recup != ',' && recup != fin_description); //passe au prochain tuple ( "cle":"valeur") 

	}
	//SECURITE
	//if (my_class->nb_attribut < nb_attribut_max){
		//my_class->description[my_class->nb_attribut * 2] = NULL; my_class->description[my_class->nb_attribut * 2 + 1] = 0;
		//}
	//FIN DESCRIPTION ;
}
void append_class(Table* my_class, FILE* fptr, const int json, const size_t num, const char fin_table, const char fin_ligne, const char fin_colonne, const char fin_description, const char deb_table, const char deb_ligne, const char deb_colonne, const char deb_description)
{

	//VAR STATIQUE SCOPE:
		int incr = 0;
		char recup = (char)0;
		char* buffer = NULL;
		size_t k = 0;

	//VAR DYNAMIQUE:

	//DEBUT LIGNE:

	for (k = 0; recup != fin_ligne && recup != EOF && k < my_class->nb_attribut; ++k, free(buffer), buffer = NULL)
		{
			//DEBUT COLONNE:

				//SI JSON ALORS COMPARE LA CLEE AVEC LES NOMS DES COLONNES:
					if (json) {
						incr = 0;
						buffer = getString2(fptr, '"', buffer);
						incr = find_description(buffer, my_class->description, my_class->nb_attribut *2);
						if (incr < 0) {
							printf("Erreur JSON la colonne n'a pas ete trouvé\n");
							exit(1);
						}
						while ((recup = getc(fptr)) && recup != ':');// consome le ":"
						while ((recup = getc(fptr)) && recup != '"');
					}

				//SINON :

					else { incr = k; }

				test_str_or_int(fptr, my_class->description[2 * incr + 1], &my_class->tab[num*my_class->nb_attribut + incr]);

				while ((recup = getc(fptr)) && recup != fin_colonne && recup != fin_ligne); // on se place a la fin d'une colonne ou d'une ligne

			//FIN COLONNE

				if (recup == fin_colonne)while ((recup = getc(fptr)) && recup != '"');// si c'est une fin de colonne on se place au debut de la prochaine colonne

		}

		//initialise les colonnes attendu mais non spécifié a NULL
			for (; k < my_class->nb_attribut; k++)
			{
				init_str_or_int(&my_class->tab[num*my_class->nb_attribut + k], NULL);
			}
		//SECURITE test si recup!=fin_ligne et k==nb_colonne soit : ignore tout soit : erreur


	//FIN LIGNE 
}

void crea_class(Table * my_class, const int json, const char * name, size_t nb_attribut, FILE* fptr, const char fin_table, const char fin_ligne, const char fin_colonne, const char fin_description, const char deb_table, const char deb_ligne, const char deb_colonne, const char deb_description)
{

	//VAR DYNAMIQUE:

	//VAR TEMPORAIRE:
		char recup = '\0';


	//ALLOUE LE NOM :
		my_erreur((my_class->name = (char*)malloc((strlen(name) + 1))));
		//initialise le nom:
		strcpy_s(my_class->name, strlen(name) + 1, name);


	if (nb_attribut > 0)//verifie que le nombre de colonne soit positif
	{
		//ALLOUE DESCRIPTION
			my_erreur((my_class->description = (char**)malloc(sizeof(char*) * 2 * nb_attribut))); //nb_attribut devrait  majorer my_class ->nb_attribut 

			my_class->nb_attribut = 0;//initialise nombre colonne de la description TODO pas necessaire si default avant 


			if (deb_description != '\0') while ((recup = getc(fptr)) && recup != EOF && recup != deb_description); //on se positionne au debut de la description des colonnes

			//INITIALISE DESCRIPTION:

				get_description(fptr, my_class, fin_description, nb_attribut);


		//ALLOUE TABLEAU STR OR INT

			my_erreur((my_class->tab = (str_or_int*)malloc(sizeof(str_or_int)* my_class->nb_attribut * 1)));//pas necessaire surtout si default avant 
		

			my_class->nb_element = 0;//initialise le nombre de ligne

			if (deb_ligne != '\0')while ((recup = getc(fptr)) && recup != deb_ligne && recup != fin_table);//on se place au debut d'une ligne 

			//INITIALISE LE TABLEAU STR OR INT

				while (recup != fin_table)
				{
					//DEBUT LIGNE :

						//ALLOUE NOUVELLE LIGNE dans le tableau de colonne:

							my_erreur((my_class->tab = (str_or_int*)realloc(my_class->tab, ++my_class->nb_element*nb_attribut * sizeof(str_or_int))));


							while (recup != '"' && (recup = getc(fptr)));//on enleve le '"' 

							//INITIALISE LA LA LIGNE

								append_class(my_class, fptr, json, my_class->nb_element - 1, fin_table, fin_ligne, fin_colonne, fin_description, deb_table, deb_ligne, deb_colonne, deb_description);

					//FIN LIGNE;

					while ((recup = getc(fptr)) && recup != EOF && deb_ligne != '\0' && recup != deb_ligne && recup != fin_table); //on se positionne au debut d'une nouvelle ligne

				}

		//FIN TABLE
	}
}
TableauId* parseur_fichier(const char* n_fichier, const int json, const char fin_table, const char fin_ligne, const char fin_colonne, const char fin_description, const char deb_table, const char deb_ligne, const char deb_colonne, const char deb_description)
{

	//VAR STATIQUE SCOPE:

	char recup = '\0';
	size_t nb_attribut = 0;

	char  * buffer = NULL;
	FILE *fptr = open_fichier(n_fichier, "r");

	//VAR DYNAMIQUE:
		TableauId * my_tabId = creaTabId();


	while ((recup = getc(fptr)) && recup != '"');

	//PARSING FICHIER:

		//DEBUT ENTREE BDD :
			for (; !feof(fptr); free(buffer), buffer = NULL) {

			//DEBUT DE LA  TABLE:

				//RECUPERE NOM TABLE:

					my_erreur((buffer = getString2(fptr, '"', buffer)));

				while ((recup = getc(fptr)) && recup != ':');//on se positionne au debut du nombre de colonne

				//RECUPERE NB_COLONNE TABLE:

					fscanf_s(fptr, "%d", &nb_attribut);

				if (deb_table != '\0')while ((recup = getc(fptr)) && recup != EOF && recup != deb_table);// on se positionne au debut de la nouvelle table


				if (deb_table == '\0' || recup == deb_table) { //si la a une valeur dans le fichier 

					//RE-ALLOCATION DU TABLEAU DE TABLE pour acueillir une nouvelle table

						my_erreur((my_tabId->arr = (Table*)realloc((Table*)my_tabId->arr, ++my_tabId->taille*(sizeof(Table))))); //HEAP


					//INITIALISE LA NOUVELLE TABLE DANS LE TABLEAU
						crea_class(&my_tabId->arr[my_tabId->taille - 1], json, buffer, nb_attribut, fptr, fin_table, fin_ligne, fin_colonne, fin_description, deb_table, deb_ligne, deb_colonne, deb_description);

			//FIN TABLE ;
				

					while ((recup = getc(fptr)) && recup != '"' && recup != EOF); // on se position au début du nom de la prochaine table

				}
				else {
					//la table est soit considérer comme vide soit pas une table car ne respect pas la syntaxe a voir 
					//INITIALISE DEFAULT:
						//(my_tabId->arr+ my_tabId->taille-1)=crea_default_table();
				}

			}
		//FIN BDD
	//LIBERATION VAR STATIQUE :

		if (fclose(fptr) == EOF) {
			fprintf(stderr, "Erreur durant la fermeture du fichier");
			exit(1);
		}

	return my_tabId;
}
TableauId* creaTabId()
{
	//INITIALISE LA STRUCT:
		TableauId* my_tabId = NULL;
		my_erreur((my_tabId = (TableauId*)malloc(sizeof(TableauId) + sizeof(size_t))));
		my_tabId->arr = NULL;
		my_tabId->taille = 0;

	//LA RENVOIE:
		return my_tabId;
}

//UTILITAIRE:

char* getString2(FILE* fptr, const char delim, char* str) {


	//VAR STATIQUE:

	size_t sz_str = 7;
	char recup = (char)0;

	//VAR DYNAMIQUE:
		my_erreur(str = (char*)malloc(7));

	//RECUPERE LA CHAINE DE CARACTERE jusqu'au prochain '"'
		for (size_t i = 0; (recup = getc(fptr)) && recup != EOF; ++i)
		{
			//si le delimiteur a etait atteint TERMINE LA CHAINE DE CARACTERE AVEC '\0' et la renvoie
				if (recup == delim) {
					recup = '\0';
					str[i] = recup;
					//printf("%s\n", str);
					return str;
				}
			//SINON:

				//si la taille est trop petite:

				if (sz_str - 1 <= i) {
					sz_str = i * 2 + 1;
					my_erreur((str = (char*)realloc(str, sz_str)));//PROBLEME POUR FREE
				}

				str[i] = recup;

		}
}

int find_description(const char * colonne, char **  descri, const size_t taille) // trouve le nom d une colonne dans une description precise renvoie -1 si pas trouver sinon l'increment de la colonne dans cette descri
{
	size_t incr = 0;
	for (; incr < taille / 2 && strcmp(colonne, descri[incr * 2]); ++incr); //cherche colonnne dans descri
	return (incr == taille / 2 ? -1 : incr);
}
Table* search_id3(TableauId* tab, char* nom) // cherche le nom d'une table dans la tableau de table 
{
	size_t k = 0;
	for (; k < tab->taille && strcmp(tab->arr[k].name, nom); k++);
	if (k != tab->taille)return &tab->arr[k];
	else { //si k== taile
		printf("Table : %s  inexistante veuillez verifier votre requete\n", nom);
		exit(1);
	}
}


int testambiguite(const char * buffer, char** description, const size_t taille) { // confort pour afficher erreur , regarde si dans description il y a deja buffer sinon renvoie 0 : pas trouvé

	int indexe = 0;
	if ((indexe = find_description(buffer, description, taille)) >= 0)
	{
		printf("AMBIGUITER DANS LA DESCRIPTION : \n Le nom de colonne : %s  est deja present a l indexe %d  veuillez corriger la table \n", buffer, indexe);
		afficher_description(description, taille);
		return 1;
	}
	else return 0; //buffer non trouve dans description
}
FILE *open_fichier(const char* n_fichier, const char* param)
{
	FILE *fptr;
	errno_t err;
	if ((err = fopen_s(&fptr, n_fichier, param)) != 0)
	{
		char buffer[250] = "\0";
		strerror_s(buffer, 250, err);
		fprintf(stderr, "Error: %d file: %s error:%s\n", err, n_fichier, buffer);

		exit(EXIT_FAILURE);
	}
	else { return fptr; }
}
void my_erreur(void* ptr) {
	if (!ptr)
	{
		fprintf(stderr, "ERREUR : allocation mémoire\n");
		exit(EXIT_FAILURE);
	}
	//else
	//{
		//printf("Le bloc : %#010x a ete alloue avec succes ! \n\n", ptr);
	//}
}



//PROMPT:
	//JSON:
	void afficher_description(char** const descri, const size_t taille)
{
	for (size_t i = 0; i < taille; ++i)
	{
		printf("Nom %d : %s ", i / 2 , descri[i]);
		printf("Type: %s | ", descri[++i]);
	}
}
	void afficher_objetL(const Table * my_class, const size_t num)
{
	size_t taille = my_class->nb_attribut;
	char **  descri = my_class->description;
	str_or_int * tab_strOrInt = my_class->tab;
	for (size_t incr = 0; incr < taille; incr++)
	{
		if (!(strcmp(descri[2 * incr + 1], "string"))) {
			printf("\t\"%s\" : \"%s\"\n", descri[2 * incr], tab_strOrInt[num*taille + incr].str);
			
		}
		else if (!(strcmp(descri[2 * incr + 1], "int"))) {
			printf("\t\"%s\" : \"%d\"\n", descri[2 * incr], tab_strOrInt[num*taille + incr].nb);
		}

		else if (!(strcmp(descri[2 * incr + 1], "class"))) {
			printf("\t%s\n", "Class");
			//afficher_class(tab_strOrInt[incr / 2].class)
		}
		else {
			printf("\t%s\n", "WTF");
		}
	}

}
	void afficher_class(const Table* my_class)
{
	printf(" NOM : %s\n Nombre d'element actuellement stock : %d\n Nombre d'attribut : %d \n\n ",(my_class->name?my_class->name:"NULL"), my_class->nb_element, my_class->nb_attribut);

	afficher_description(my_class->description, my_class->nb_attribut * 2);

	printf("\nDEBBUT CLASS [\n");
	for (size_t i = 0; i < my_class->nb_element; i++)
	{
		printf("Element %d : \n\t{\n", i);
		afficher_objetL(my_class, i);
		printf("\t}\n\n");
	}
	printf("]\nFIN_CLASS\n");

}
	//CSV:
	void afficher_description2(char** const descri, const size_t taille)
	{
		size_t i = 0;
		for (; i < taille - 2; ++i)
		{
			printf("\"%s\":", descri[i]);
			printf("\"%s\",", descri[++i]);
		}

		printf("\"%s\":", descri[i]);
		printf("\"%s\"\n", descri[++i]);

	}
	void afficher_objetL2(const Table * my_class, const size_t num, const char fin_colonne, const char deb_colonne)
	{
		size_t taille = my_class->nb_attribut;
		char **  descri = my_class->description;
		str_or_int * tab_strOrInt = my_class->tab;
		size_t incr = 0;
		if (deb_colonne != '\0')printf("%c", deb_colonne);
		if (!(strcmp(descri[2 * incr + 1], "string"))) {

			printf("\"%s\"", tab_strOrInt[num*taille + incr].str);

		}
		else if (!(strcmp(descri[2 * incr + 1], "int"))) {
			printf("\"%d\"", tab_strOrInt[num*taille + incr].nb);
		}

		else if (!(strcmp(descri[2 * incr + 1], "class"))) {
			printf("\t%s\n", "Class");
			//afficher_class(tab_strOrInt[incr / 2].class)
		}
		else {
			printf("\t%s\n", "WTF");
		}

		for (; incr < taille - 2; incr++)
		{
			if (fin_colonne != '\0')printf("%c", fin_colonne);
			if (deb_colonne != '\0')printf("%c", deb_colonne);
			if (!(strcmp(descri[2 * incr + 1], "string"))) {
				printf("\"%s\"", tab_strOrInt[num*taille + incr].str);

			}
			else if (!(strcmp(descri[2 * incr + 1], "int"))) {
				printf("\"%d\"", tab_strOrInt[num*taille + incr].nb);
			}

			else if (!(strcmp(descri[2 * incr + 1], "class"))) {
				printf("\t%s\n", "Class");
				//afficher_class(tab_strOrInt[incr / 2].class)
			}
			else {
				printf("\t%s\n", "WTF");
			}
		}

		if (!(strcmp(descri[2 * incr + 1], "string"))) {
			if (tab_strOrInt[num*taille + incr].str) {
				if (fin_colonne != '\0')printf("%c", fin_colonne);
				printf("\"%s\"", tab_strOrInt[num*taille + incr].str);
			}


		}
		else if (!(strcmp(descri[2 * incr + 1], "int"))) {
			printf("\"%d\"", tab_strOrInt[num*taille + incr].nb);
			if (fin_colonne != '\0')printf("%c", fin_colonne);
		}

		else if (!(strcmp(descri[2 * incr + 1], "class"))) {
			printf("\t%s", "Class");
			if (fin_colonne != '\0')printf("%c", fin_colonne);
			//afficher_class(tab_strOrInt[incr / 2].class)
		}
		else {
			printf("\t%s", "WTF");
			if (fin_colonne != '\0')printf("%c", fin_colonne);
		}

	}
	void afficher_class2(const Table* my_class, const char fin_table, const char fin_ligne, const char fin_colonne, const char deb_ligne, const char deb_colonne, const char deb_table, const char deb_descri, const char fin_descri)
	{
		if (deb_table != '\0')printf("%c", deb_table);
		printf(" \"%s\":%d\n ", (my_class->name ? my_class->name : "NULL"), my_class->nb_attribut);
		if (fin_descri != '\0')printf("%c", fin_descri);
		if (deb_descri != '\0')printf("%c", deb_descri);
		afficher_description2(my_class->description, my_class->nb_attribut * 2);
		if (fin_descri != '\0')printf("%c", fin_descri);
		for (size_t i = 0; i < my_class->nb_element; i++)
		{
			if (deb_ligne != '\0')printf("%c", deb_ligne);
			afficher_objetL2(my_class, i, fin_colonne, deb_colonne);
			if (fin_ligne != '\0')printf("%c", fin_ligne);
		}
		if (fin_table != '\0')printf("%c", fin_table);

	}




//DELETE:

	int free_descri(char** descri, const size_t taille)
	{
		size_t i = 0;
		for (; i < taille; free(descri[i]), descri[i] = NULL, ++i);
		free(descri);
		return (i == taille ? 0 : 1);
	}
	int free_tab_strInt(str_or_int* tab, const size_t taille)
	{
		size_t i = 0;
		for (; i < taille; ++i) {
			if (tab[i].str) { free(tab[i].str); tab[i].str = NULL; }
		}
		free(tab);
		return (i == taille ? 0 : 1);
	}
	void free_class(Table* my_class)// LIBERE CONTENU TABLE
	{
		//LIBERETABLE
			if (my_class)
			{
				//LIBERE NOM TABLE

					free(my_class->name);
					my_class->name = NULL;

				//LIBERE TABLEAU DE NOM/TYPE DES COLONNES

					if (free_descri(my_class->description, my_class->nb_attribut * 2)) {
						printf("ERREUR DESALOC DESCRI\n");
						exit(1);
					}
					else {
						my_class->description = NULL;
					}

				//LIBERE COLONNE

					//LIBERE TABLEAU DE COLONNE

						if (free_tab_strInt(my_class->tab, my_class->nb_attribut*my_class->nb_element)) {
							printf("ERREUR DESALOC TAB\n");
							exit(1);
						}
						else {
							my_class->tab = NULL;
						}

			}

	}
	void del_TabId(TableauId*my_tabId)
	{
		//LIBERE LES TABLES CONTENU DANS LE TABLEAU

			for (size_t i = 0; i < my_tabId->taille; i++)
			{
				//AFFICHE TABLE
					//afficher_class(&tabId[i]);
				//DELETE CONTENU TABLE
					free_class(&my_tabId->arr[i]);

			}
		//FREE TABLEAU DE TABLE
			free(my_tabId->arr);
			my_tabId->arr = NULL;

		//FREE STRUCTURE CONTENANT LE TABLEAU
			free(my_tabId);
			my_tabId = NULL;

	}


//STR_OR_INT
	//CONSTRUCTOR STR_OR_INT

		void test_str_or_int(FILE* fptr, const char* descri, str_or_int * strInt) //init from file
	{
		//SI LA VALEUR EST UN STRING :

			if (!(strcmp(descri, "string"))) {//regarde si le type de la colonne considerer est "string"

				strInt->nb = 0;
				strInt->str = getString2(fptr, '"', strInt->str);

			}

		//SINON SI LA VALEUR EST UN INT

			else if (!(strcmp(descri, "int"))) {//regarde si le type de la colonne considerer est "int"
				strInt->str = NULL;
				fscanf_s(fptr, "%d", &strInt->nb);
		
				getc(fptr);//enleve guillemet

			}
		//SINON SI C EST UNE AUTRE TABLE

			else if (!(strcmp(descri, "class"))) {}

		//DEFAULT:
			else { }
	}

		void test_str_or_int2( char * buffer, const char* descri, str_or_int * strInt) // convertie le buffer en str_or_int
	{
		//SI LA VALEUR EST UN STRING :

			if (!(strcmp(descri, "string"))) {//regarde si le type de la colonne considerer est "string"
				strInt->nb = 0;
				strInt->str = buffer;


			}

		//SINON SI LA VALEUR EST UN INT

			else if (!(strcmp(descri, "int"))) {//regarde si le type de la colonne considerer est "int"
				strInt->str = NULL;
				strInt->nb = atoi(buffer);
				free(buffer);
			}

		//SINON SI C EST UNE AUTRE TABLE

		else if (!(strcmp(descri, "class"))) {

		}
		//DEFAULT:

			else {}
	}

	//operator :
		void init_str_or_int(str_or_int* modif, const str_or_int* source)// surcharge op de copie pour str or int 
	{
		// si != DEFAULT
			if (source) {

				//SI STRING
					if (source->str) {

						my_erreur(modif->str = (char*)malloc(strlen(source->str) + 1));
						//printf("modif : %#010x \n", modif->str);
						strcpy_s(modif->str, strlen(source->str) + 1, source->str);
						//free(source->str); source->str = NULL;
						modif->nb = 0;
					}
				//SINON (SI INT)
					
					else {		
						modif->nb = source->nb;
						modif->str = NULL;
					}
			}
		//SINON (si DEFAULT)

			else { 
				modif->nb = 0;
				modif->str = NULL;
			}
	}

	//DISPLAY:
		void affichage_str_or_int(const str_or_int* param) {
		if (param->str)
			printf("\"%s\"", param->str);
		else
			printf("%d", param->nb);
	}






//TODO: autre getstring2 pour les enlever les espaces devant ou fin PROTOTYPE:
char* getString3(FILE* fptr, const char delim, char* str) { // maybe passeer un delim a ignorer ou carrément des /*  */ ou // dans un tab

	//VAR DYNAMIQUE:
	my_erreur(str = (char*)malloc(7));
	//VAR STATIQUE:
	size_t sz_str = 7;
	char recup = (char)0;

	//RECUPERE LA CHAINE DE CARACTERE jusqu'au prochain '"'
	for (size_t i = 0; (recup = getc(fptr)) && recup != EOF; ++i)
	{
		//TERMINE LA CHAINE DE CARACTERE AVEC '\0' et la renvoie
		if (recup == delim) {
			recup = '\0';
			str[i] = recup;
			//printf("%s\n", str);
			return str;
		}

		if (sz_str - 1 <= i) {
			sz_str = i * 2 + 1;
			my_erreur((str = (char*)realloc(str, sz_str)));//PROBLEME POUR FREE
		}
		str[i] = recup;

	}
}


//TUPLE:

	//CONSTRUCTOR TUPLE:
		my_tuple* create_tuple(const char * delim, size_t deb_delim, size_t taille)
	{
		//VAR STATIQUE:
		//VAR DYNAMIQUE:
			my_tuple* tuple = NULL;
			my_erreur((tuple = (my_tuple*)malloc(sizeof(my_tuple))));

		tuple->delim = delim; // tableau de char : contenant les delimiteurs séparant les champs de la valeurs composé
		tuple->deb_delim = deb_delim;// indexe du debut des delimiteurs pouvant signifier la fin de l'entree d'une valeur
		tuple->taille = taille; // taille étant la taille du string , taille -2 l'indexe du delimiteur marquant la fin de l'entree de toute les valeurs

		return tuple;
	}
	//intialise tuple  avec fichier :
		int getString_arr(FILE* fptr, const char *  arr_delim, char** key_tuple, const size_t taille) // rempli un tableau de string grace a un fichier 
	{
		//VAR STATIQUE:

			int k = 0; // parcour delim
			size_t t = 0; // parcour tab
			size_t sz_str = 7;
			char recup = (char)0;

		//VAR DYNAMIQUE:
			char * buffer = NULL;
			my_erreur(buffer = (char*)malloc(sz_str));



		while ((recup = getc(fptr)) && recup == ' ');//enleve les espace devant 
		for (size_t i = 0; t < taille && recup != EOF; recup = getc(fptr)) // rempli le tuple de taille_tuple str au plus 
		{
			//TERMINE LA CHAINE DE CARACTERE AVEC '\0' et la renvoie
		
			for (k = t; arr_delim[k] != '\0' && (recup != arr_delim[k]); k++); // test si un delimiteur a etait trouver
			if (recup == arr_delim[k]) {//si le delim a etait trouver

				buffer[i] = '\0'; // complete le buffer


				for (; t < k && t < taille - 1; t++)key_tuple[t] = NULL; // rempli le tuple avec des NULL pour les positions non trouvés

				key_tuple[t++] = buffer; //affecte le buffer a la position voulu 
				//printf("%s\n", buffer);
				buffer = NULL;
				i = 0;
				if (k >= taille - 1) { // si le delimiteur correspond a un delim final
					//printf("%d\n", k);
					return k;
				}
				if (t != taille) { // si le tuple peut encore aceuillir des str initialise un nouveau buffer
					sz_str = 7;
					buffer = (char*)malloc(sz_str);
				}
				continue;

			}
			if (sz_str - 1 <= i) {//si le buffer n est pas assez grand 
				sz_str = i * 2 + 1;
				buffer = (char*)realloc(buffer, sz_str);
			}

			buffer[i++] = recup; // rempli le buffer

		}
		if (recup == EOF) { // si  le fichier est vide 

			buffer[0] = '\0'; key_tuple[t] = buffer;
			buffer = NULL;
			if (t == 0)k = -1; // si aucun delim n'a été trouvé 
			for (; t < taille; t++) { // si le tuple n a pas atteint la fin des delimiteurs 

				key_tuple[t] = NULL;
			}
		}
		return k; // return la position du dernier delim trouvé

	}
	//predicat :
		int isequal_tuple(char** tuple, char** tuple2, size_t taille) // test si 2 tuple sont égaux
	{
		size_t k = 0;
		for (; k < taille && (tuple[k] && tuple2[k] ? !strcmp(tuple[k], tuple2[k]) : 1); k++);

		return k == taille;
	}
	//DELETE TUPLE:
		void free_tuple(char** lol, size_t taille) // libere le tuple
	{
		for (int k = taille - 1; k > 0; k--)if (lol[k])free(lol[k]);
	}

//TABLEAU DE TUPLE:

	//CONSTRUCTOR PAR DEFAUT D UN TABLEAU DE TUPLE
		tab_tuple* defolt_arr_tuple(size_t taille_tuple) { // constructor par "defaut" de tuple avec la taille des tuples stocké : le nombre de valeur qui les composes 

		tab_tuple *tuple_arr = NULL;
		my_erreur((tuple_arr = (tab_tuple*)malloc(sizeof(tab_tuple))));
		tuple_arr->taille = 1;
		my_erreur((tuple_arr->tab = (char***)malloc(tuple_arr->taille * sizeof(char**))));
		tuple_arr->taille_tuple = taille_tuple;
		tuple_arr->cursor = 0;
		my_erreur((tuple_arr->tab[tuple_arr->cursor] = (char**)malloc(tuple_arr->taille_tuple * sizeof(char *))));
		return tuple_arr;
	}

	
		tab_tuple* create_tab_tuple(FILE* fptr, my_tuple* tuple)//initialise tableau tuple avec fichier
	{
		//VAR STATIQUE:
			int tmp = 0;
			char recup = '\0';

		//VAR DYNAMIQUE:

			tab_tuple* tuple_arr = NULL;

			//ALLOUE TABLEAU DE TUPLE POUR STOQUER CONTENU SELECT :

			my_erreur((tuple_arr = defolt_arr_tuple(2))); //creer un tableau de tuple ( comportant chacun 2 valeur )

		
		//DEBUT SELECT:
			while (tmp < tuple->taille - 2) { // tant qu'on finit pas la lecture des valeurs du select

				//DEBUT D UNE VALEUR DU SELECT

					//REALLOC :
						if (tuple_arr->taille - 1 <= tuple_arr->cursor)tuple_arr = tuple_arr_realoc(tuple_arr, 2 * tuple_arr->cursor + 1); // si le tableau de tuple est plein mais qu'il reste des valeurs : realloue
					//taille suffisante;

					//REMPLI
						tmp = getString_arr(fptr, tuple->delim, tuple_arr->tab[tuple_arr->cursor], tuple->deb_delim); // rempli la case pointé par le cursor 

					//TEST SI VALEUR REMPLI CORRECTE:

						// regarde SI dans l'actuel tableau [0;cursor[ il y a deja la valeur  tab[cursor]
						for (size_t k = 0; k < tuple_arr->cursor; k++) { 
						
							if ((isequal_tuple(tuple_arr->tab[tuple_arr->cursor], tuple_arr->tab[k], tuple_arr->taille_tuple))) { // si tel est le cas : vide tab[cursor]

								// vide tab[cursor]:
									free_tuple(tuple_arr->tab[tuple_arr->cursor], tuple_arr->taille_tuple);
									tuple_arr->tab[tuple_arr->cursor] = NULL;
								break;
							}
						}
					//SINON
						if (tuple_arr->tab[tuple_arr->cursor])tuple_arr->cursor++; // si la valeur/le tuple a été accepter ( il n'était pas deja present dans le tableau) incremente la taille du tableau actuel ( dont les cases sont rempli)
		
				//SE PLACE EN FIN DE VALEUR
				if (tmp < tuple->deb_delim) { // si nous n'avons pas atteint le caractere signifiant la fin d'une valeur , discard tous les caractères entre la fin de la valeur et le delimiteur attestant de sa fin 
					if (find_cara(tuple->delim + tuple->deb_delim, tuple->delim[tmp]) < 0)while ((recup = getc(fptr)) && recup != EOF && (tmp = find_cara(tuple->delim + tuple->deb_delim, recup)) < 0);
				}

			 //FIN DE LA VALEUR 

			
			}

		//FIN_SELECT
		
		//RENVOIE SELECT
		return tuple_arr;


	}

	//SECURE REALLOC
		tab_tuple * tuple_arr_realoc(tab_tuple* tuple_arr, const size_t resize) { // secur realloc pour tab_tuple
		//TODO FAIRE UN REALLOC POUR PILE POUR EVITER DE METTRE MAX DIRECT CHECK  impose taille actuel = cursor + size_t taille_reel + test cursor> taille

		tuple_arr->taille = resize;
		my_erreur(tuple_arr->tab = (char***)realloc(tuple_arr->tab, tuple_arr->taille * sizeof(char**)));
		for (size_t k = tuple_arr->cursor; k < tuple_arr->taille; k++)my_erreur((tuple_arr->tab[k] = (char**)malloc(tuple_arr->taille_tuple * sizeof(char *))));
		return tuple_arr;
	}

	//DESTRUCTOR TABLEAU de tuple
		void free_tab_tuple(tab_tuple* autre) { // libere le contenu du select 

		for (size_t k = 0; k < autre->cursor; k++)free_tuple(autre->tab[k], autre->taille_tuple); //ajouter libere funct agreg
		free(autre->tab);

	}


//CONDITION:
		//CONSTRUCTOR:
			
			//TODO : FAIRE UNE CONDITION N aire : pas lhs ou rhs mais n operande => tableau même traitement pour tous obligation que 1 est une table for loop test même operateur a chaque fois
			// TODO : accepter de n'avoir que 2 valeurs et gerer ce cas genre 20=20 => true
				cond * get_condition(FILE*fptr, cond* condition, from * my_pile, my_tuple * tuple, const char li) //constructor condition via fichier :
			{
				//VAR STATIQUE:
					char recup = '\0';
					int tmp = 0; //rapport erreur
					int selection = 0; // booleen : concerve type du lhs  : 0 = table 1= "valeur"


					char ** key_tuple = NULL;
					my_erreur(key_tuple = (char**)malloc(tuple->deb_delim * sizeof(char*)));
					char * buffer = NULL;

				//VAR DYNAMIQUE:
				
				my_erreur((condition = (cond*)malloc(sizeof(cond))));

				

				//DEBUT CONDITION:

								// TODO : gerer le cas du funct agreg 

					//DEBUT VALEUR:

						//DEBUT LHS:

							recup = getc(fptr);
													
							// SI le lhs n est pas une table 

								if (recup == '"') { 
								//idee : get le string comme si ct une table et si stop sur une paranthese + le string == str_to_enum si string!=str_to_enum alors strcat (buffer , getstring_arr) 
									buffer = getString2(fptr, '"', buffer);
									selection = 1;
								}
					
							//SINON (Si le lhs est une table ou d 'apres idee funct agreg 

								else {
						
									tmp = getString_arr(fptr, tuple->delim, key_tuple, tuple->deb_delim);

									//SI est une table + nom colonne alors search sinon si que colonne find_colonne
									(key_tuple[0]?search_id(my_pile, key_tuple, tuple->deb_delim, &condition->lhs) :find_colonne(my_pile, key_tuple, tuple->deb_delim, &condition->lhs)); 

									//condition->rhs.val.str = NULL;
									//condition->rhs.val.nb = 0;

									for (size_t l = 0; l < tuple->deb_delim; free(key_tuple[l]), key_tuple[l] = NULL, ++l);//reset buffer_tuple

								}
						//FIN LHS

						//GET OPERATEUR
						condition->operation = getString2(fptr, ' ', condition->operation);//get l operation de comparaison

						//DEBUT RHS:
						
							//SI le rhs n est pas une table  
				
								if ((recup = getc(fptr)) && recup == '"') { 

									if (selection) {//si le lhs n etait pas une table
										printf("Erreur la condition n'est pas valide veuillez la re ecrire \n");
										exit(1);
									}
									test_str_or_int(fptr, condition->lhs.id->id_table->adresse_table->description[2 * condition->lhs.id->indexe_colonne + 1], &condition->rhs.val); // rempli le lhs de la cond
									condition->rhs.id = NULL; //indique qu il n est pas une table 

									//VOIR SI PAS PAREIL QUE label: discard => mettre fin discard pour signer la fin valeur
								}

							//SINON (SI le rhs est une table )

							else {

								tmp = getString_arr(fptr, tuple->delim, key_tuple, tuple->deb_delim);
					
								//SI est une table + nom colonne alors search sinon si que colonne find_colonne
								(key_tuple[0] ? search_id(my_pile, key_tuple, tuple->deb_delim, &condition->rhs) : find_colonne(my_pile, key_tuple, tuple->deb_delim, &condition->rhs));

								//condition->rhs.val.str = NULL;
								//condition->rhs.val.nb = 0;

								if (selection) {//si le lhs n etait pas une table l initialise grace au info de la table du rhs TODO: gerer compatibilite avec funct agreg

									test_str_or_int2(buffer, condition->rhs.id->id_table->adresse_table->description[2 * condition->rhs.id->indexe_colonne + 1], &condition->lhs.val);
									condition->lhs.id = NULL;

								}
								for (size_t l = 0; l < tuple->deb_delim; free(key_tuple[l]), key_tuple[l] = NULL, ++l); //free buffer_tuple

								//discard: fin valeur
								if (tmp < tuple->deb_delim) { // si la valeur est terminer mais l'entree d'une nouvelle valeur METTRE A LA FIN 
									if (find_cara(tuple->delim + tuple->deb_delim, tuple->delim[tmp]) < 0)while ((recup = getc(fptr)) && recup != EOF && (tmp = find_cara(tuple->delim + tuple->deb_delim, recup)) < 0);
								}
							}
						//FIN RHS

					//FIN VALEUR
					free(key_tuple); //free emplacement buffer_tuple

					//reach condition limit :
					if (li != '\0' && tmp < tuple->taille - 1)while (recup != li && (recup = getc(fptr)) && recup != EOF);//si la fin de la porte du key_word n'a pas ete capter ainsi que  la fin des valeur  qui est normalement = au keyword fin TODO:harmoniser ,suppr generalisation , faire felxible

				//FIN CONDITION
				return condition;

			}

			//constructor Identifiant_or :
				void find_colonne(from* tab, char ** nom, size_t tae, Identifiant_or * result) // initialise un membre d une condtion dont seul la colonne a etait specifier 
			{
				//VAR STATIQUE:
					size_t k = 0, p = 0, z = 1;
					int tmp = 0;

				for (; z < tae && !nom[z]; z++); //cherche le premier str non nul " qui represente la colonne " , secondary_TODO: harmoniser avec autre utilisation genre NULL....colonne1.colonne2

				for (; k < tab->taille && (tmp = find_description(nom[z], tab->arr[k].adresse_table->description, tab->arr[k].adresse_table->nb_attribut * 2)) < 0; k++); // cherche nom dans  les descriptions des table dans la pile

				//TODO: PAS NECESSAIRE DEPUIS NOUVELLE UPDATE securite deja faite en ammont 
				for (p = k + 1; p < tab->taille && find_description(nom[z], tab->arr[p].adresse_table->description, tab->arr[p].adresse_table->nb_attribut * 2) < 0; p++);//si nom a ete trouve alors cherche nom dans la suite des descriptions de la pile
				if (p == tab->taille) {//si nom pas trouve dans la suite de la description <=> pas ambiguite
					my_erreur((result->id = (Identifiant*)malloc(sizeof(Identifiant)))); //alloue l'id de la table associ
					result->id->id_table = &tab->arr[k]; // recupere l adresse de la table 
					result->id->indexe_colonne = tmp;

				}
				else {
					printf("Ambiguite \n veuillez specifier la colonne : %s dans le fichier  ambiguite trouver avec : %s et %s \n", nom[z], tab->arr[k].rename, tab->arr[p].rename);
					exit(1);
				}

			}

		//PROMPT:
			void affichage_cond(const cond* condi) { //display condition avec format : si table ( nom_table : %s , colonne , %s) sinon (%s ou %d selon type valeur stockee)

				//SI LHS est une table
					if (condi->lhs.id)
					{

						printf("(nom_table : %s, colonne : %s )", condi->lhs.id->id_table->adresse_table->name, condi->lhs.id->id_table->adresse_table->description[2 * condi->lhs.id->indexe_colonne]);
					}

				//SINON (SI LHS est une valeur)
					else {
						affichage_str_or_int(&condi->lhs.val);
					}
				printf(" %s ", condi->operation);


				//SI RHS est une table
					if (condi->rhs.id)
					{
						printf(" (nom_table : %s, colonne : %s )", condi->rhs.id->id_table->adresse_table->name, condi->rhs.id->id_table->adresse_table->description[2 * condi->rhs.id->indexe_colonne]);
					}
				//SINON (SI RHS est une valeur)
					else {
						affichage_str_or_int(&condi->rhs.val);
					}

				printf("\n");
			}



//FUCK UP TREE:

	//CONSTRUCTOR NODE:
		Noeud * create_Node(Noeud * father)
		{
			//VAR STATIQUE:

			//VAR DYNAMIQUE:
				Noeud* node = NULL;
				my_erreur((node = (Noeud*)malloc(sizeof(Noeud))));

			node->filsD = NULL;
			node->filsG = NULL;
			node->pere = father;
			node->verif = NULL;
			node->val = NULL;
			node->poids = 0;
			return  node;
		}
		Noeud * create_NoeudD(char* efr , int poids, Noeud* fath) 
		{
			// VAR STATIQUE :

			//VAR DYNAMIQUE:
				Noeud* node = NULL;
				my_erreur((node = (Noeud*)malloc(sizeof(Noeud))));

			
			node->filsD = NULL;
			node->filsG = NULL;
			node->pere =fath;
			node->verif = NULL;
			node->val = efr;
			node->poids = poids;
			return  node;
		}

	//TODO: remplacer litteral par delim 

		Noeud * createNoeud(FILE * fptr, const char delim, Noeud* father, from * my_tab) { //CONSTRUCTEUR ARBRE via FILE:
		
		//VARIABLE STATIQUE:
			char recup = '\0';

		//VARIABLE DYNAMIQUE:
			char * buffer = NULL;
			Noeud *tmp_node = NULL, *root = NULL;

		//DEBUT FG:
	
			recup = getc(fptr);

			if (recup == delim || recup == '\n') {//?wut 
				while (recup != delim && (recup = getc(fptr)) );
			}
			else{
				//si fils gauche est un node / expression compose  ex: (A || (B && C))
					if (recup == '(')
					{

						root= createNoeud(fptr, ')', father, my_tab);
						//printf("recup2%c\n delim:%c\n", recup, delim);
						if (delim != ')')while ((recup = getc(fptr)) && recup != '\n' && recup != delim);

					}
				//SINON SI LE FG est une feuille ex: A
					else {
						if (delim != ')') {
							root= createleaf(fptr,father, my_tab, '\n');
						}
						else root= createleaf(fptr,father, my_tab, '\0');
					}
				//printf("root fg%d\n", root->poids);

		//FIN FG ;
			
			recup = getc(fptr);

			if (recup != delim) {//SI LE FG N EST PAS TOUT SEUL ex: A && B

				//GET OPERATEUR LOGIQUE:
					buffer = getString2(fptr, ' ', buffer);
					//printf("BUFF %s\n", buffer);

				//CREATE NOEUD AVEC OP LOGIQUE:
					tmp_node = create_NoeudD(buffer, root->poids, root->pere);

					//ASSIGNE FG au NOEUD:
							tmp_node->filsG = root; //FG op logique = le noeud precedemment get 
							root->pere = tmp_node;
				

				root = tmp_node;//LA RACINE DE L ARBE  DEVIENT L OP LOGIQUE
				//RESET le buffer
				buffer = NULL;

				while (recup != delim) { // comme une for loop regarder fin


					
				//FD:

					recup = getc(fptr);

					//printf(" cara2 %c\n", recup);
					//si FD est un node / expression compose  ex: (A || (B && C))
					if (recup == '(')
					{
						//GET EXPRESSION COMPOSE:
						root->filsD = createNoeud(fptr, ')', root, my_tab);
						//FIN_EXP compose

						//SI le root faisait partie d'une expression compose la termine
						if (delim != ')')while ((recup = getc(fptr)) && recup != '\n' && recup != delim);
					}
					//SINON (si FD est une feuille )

					else {root->filsD = (delim != ')' ? createleaf(fptr, root, my_tab, '\n') : createleaf(fptr, root, my_tab, '\0'));} //voir si pas enlever char 0 pour delim si oui alors tmp pour check si reach delimiteur

					//ACTUALISE POIDS

						root->poids += root->filsD->poids;
						//TRI LOCAL
							if (root->filsG->poids > root->filsD->poids)
							{
								tmp_node = root->filsG;
								root->filsG = root->filsD;
								root->filsD = tmp_node;
							}

					recup = getc(fptr);
					//printf("recup%c\n", recup);
					//SI L ARBRE N EST PAS FINI
						if (recup != delim) // (voir todo )check tmp 
						{
							//GET OPERATEUR
								buffer = getString2(fptr, ' ', buffer);
								//printf("BUFF%s\n", buffer);

							//CREATE NOEUD AVEC OP LOGIQUE:
							tmp_node = create_NoeudD(buffer, root->poids,root->pere);

							//ASSIGNE FG au NOEUD:
								tmp_node->filsG = root;
								root->pere = tmp_node;

							//LA RACINE DE L ARBE  DEVIENT L OP LOGIQUE
								root = tmp_node;

							//RESET BUFFER:
								buffer = NULL;
						}
				//TODO : FAUDRAIT EMPILER LA NEW VAL pour conserver tri 
				}

			}
		}
	return root;
}

	//CONSTRUCTOR LEAF:
		Noeud * createleaf(FILE* fptr, Noeud * father, from * my_tab, const char delim) // creer une feuille 
		{
			//VAR STATIQUE:
				my_tuple* tuple = create_tuple(".  \n", 2, 4); // creer un tuple qui correspond au delimiteur de la feuille  [field1.]field2[ ]soit[\n]
			//VAR DYNAMIQUE:
				Noeud* leaf = NULL;
				my_erreur(leaf = (Noeud*)malloc(sizeof(Noeud)));

			leaf->pere = father;
			leaf->filsD = NULL; leaf->filsG = NULL;
			leaf->val = NULL;
			leaf->verif = get_condition(fptr, leaf->verif, my_tab, tuple, delim); //rempli sa condition
			leaf->poids = (leaf->verif->lhs.id ? leaf->verif->lhs.id->id_table->adresse_table->nb_element : 1)*(leaf->verif->rhs.id ? leaf->verif->rhs.id->id_table->adresse_table->nb_element : 1); //changer poids pour poids valide formule prof
			free(tuple);
			//printf("poids leaf %d\n",leaf->poids);

			return leaf;

		}

	//DESCTRUCTOR TREE:	
			void free_arbre(Noeud * root)
		{
			if (root) // si arbre pas vide
			{
				free_arbre(root->filsG);// libere sag
				free_arbre(root->filsD);//libere sad
				if (root->verif) { // si le root est une leaf 

					//LHS
					if (root->verif->lhs.id) free(root->verif->lhs.id); // libere id de la table si lhs est une table 
					else if (root->verif->lhs.val.str) { free(root->verif->lhs.val.str); } // si lhs n'est pas une table et qu'il est un string : "Marc"= t2.nom 

					free(root->verif->operation); // libere l'operation : "<" , ">" ,... 

					//RHS
					if (root->verif->rhs.id) free(root->verif->rhs.id);
					else if (root->verif->rhs.val.str) { free(root->verif->rhs.val.str); } //SAME

					//libere condition : something op something2
					free(root->verif);
				}
				else { free(root->val); }// si c'est un node soit : "AND" ou "OR" libere le string associe

				free(root);// libere l emplacement du Noeud/leaf
			}
		}
			//TODO : faire destructor feuille et node pour alleger code free_arbre 

	//UTILITAIRE:
		int isnotFD(Noeud* parcour) //test si un noeud est le filsD de son pere
		{
			return (parcour->pere->filsD != parcour);
		}

	//PROMPT:
		void print_arbre(Noeud* racine) // parcour infixe
		{
			if (racine)
			{
				print_arbre(racine->filsG);
				if (racine->verif)affichage_cond(racine->verif);
				else { printf("%s et poids : %d pere :%s \n", racine->val, racine->poids, (racine->pere ? racine->pere->val : "null")); }
				print_arbre(racine->filsD);
			}
		}




// FAIRE UN SWITCH POUR LES MOTS CLES A APPELE A CHAQUE NEW LINE 

//PILE
	//CONSTRUCTOR :
		from * default_pile(size_t sz) {
			from* my_pile = NULL;
			my_erreur(my_pile = (from*)malloc(sizeof(from)));
			my_pile->taille = sz; //initialise taille
			my_erreur(my_pile->arr = (element*)malloc(sizeof(element)* my_pile->taille));
		
			return my_pile;
		}
		
		from * create_pile(FILE* fptr,TableauId* tab,my_tuple* tuple)//constructor pile via FILE
			{

				//VAR STATIQUE:
					int tmp = 0;//retour des get
					char* buffer = NULL;

				//VAR DYNAMIQUE:
					char *rename = NULL; //remplace par tuple
					//ALLOUE TABLEAU DE TUPLE:
					from* my_pile = default_pile(tab->taille);

				my_pile->taille = 0; // joue le role de cursor : taille reel

				//DEBUT VALEUR DU FROM:

					//INITIALISE TABLEAU D ELEMENT :

							for (char recup = '\0'; (tmp < tuple->taille -2)  /*&& !feof(fptr)*/ ; free(buffer), buffer = NULL, rename = NULL) { // tant que le delimiteur final : terminant l'entree de toutes les valeurs n'a pas était trouvé

							
									if ((tmp = getString_arr(fptr, tuple->delim, &buffer, tuple->deb_delim)) < tuple->deb_delim){// si la fin de la saisi de la valeur n'a pas proc 
			
										if ((tmp = (getString_arr(fptr, tuple->delim, &rename, tuple->deb_delim))) < tuple->deb_delim) { // si la fin de la saisi de la valeur n'a pas proc 
											while ((recup = getc(fptr)) && recup != EOF && ((tmp = find_cara(tuple->delim + tuple->deb_delim, recup)) < 0)); // discard les char du fichier jusqu'au prochain delim de fin de valeur
				
										}
		
									}
									//printf("%s\n", rename);
									append_tri(my_pile, create_element(&my_pile->arr[my_pile->taille], search_id3(tab, buffer), rename));//ajoute dans la pile l'element crée via dichotomie
									print_pile(my_pile);
								//FIN NOM TABLE (une valeur du FROM)

							}
				//FIN VALEUR FROM ;

				//my_erreur(my_pile->arr = (element*)realloc(my_pile->arr,sizeof(element)* my_pile->taille));
		
				return my_pile;
			}

	//OPERATOR:

		//surcharge operator  +=  pour pile :
			void append_tri(from* my_pile, element* elem) // ajoute la valeur via dichotomie dans la pile de table que constitue le from 
			{
				 
				//SI les elements de la sous pile avant elem n'est pas vide 

				if (my_pile->taille) { 

					// VAR STATIQUE: 
						int beg = 0;
						int ende = my_pile->taille - 1;//a verif
						int mid = mid = beg + (ende - beg) / 2;

					//VAR DYNAMIQUE:

						element *tmp = NULL;
					
					//DICHOTOMIE:
						
						while (beg <= ende) 
						{

							if (my_pile->arr[mid].adresse_table->nb_element > elem->adresse_table->nb_element)ende = mid - 1;
							else if (my_pile->arr[mid].adresse_table->nb_element < elem->adresse_table->nb_element)beg = mid + 1;
							else break;
							mid = beg + (ende - beg) / 2;
						}
						// NE MARCHE PAS IL FAUT FAIRE DE MY_pile->taille -1  à mid en sauvegardant le dernier a chaque fois car c l'elem
					if (my_pile->arr[mid].adresse_table->nb_element > elem->adresse_table->nb_element)++mid; // si l'element se trouve apres le mid increment le mid qui sera la position de l'element dans la file

					if (mid < my_pile->taille) { // si l'element n'était pas supérieur a tout ceux présent dans la pile comme supposé par défaut  alors il faut modifier la pile :

						if (mid == my_pile->taille - 1) { // si l'element est a insérer a la fin de la pile actuelle
							tmp = copy_element_init(&my_pile->arr[my_pile->taille - 1]); // sauvegarde l'element se trouvant en derniere position de la pile actuelle
						}
						else {
							//printf("mid:%d\n", mid);
							for (size_t i = mid; i < my_pile->taille - 1; i++) // fait de la place pour l element a inserer
							{
								my_cpy(&my_pile->arr[i], &my_pile->arr[i + 1]);

							}
						}
						my_cpy(elem, &my_pile->arr[mid]); // insere l'element a sa position 

						(tmp ? // si l'element etait a inserer dans la derniere position de la pile actuelle
							my_cpy(tmp, &my_pile->arr[my_pile->taille])  // mets le dernier element de l'ancienne pile a la fin de la nouvelle
							: my_cpy(&my_pile->arr[my_pile->taille - 1], &my_pile->arr[my_pile->taille])); // sinon fini l'iteration " normale" 

					}
					free(tmp);
				}
				++my_pile->taille; // increment le curseur de la pile 
				
				//print_pile(my_pile);



			}
	//DESTRUCTOR:
		void free_pile(from* my_pile) // libere la pile contenant les table par ordre croissant
		{
			for (size_t k = 0; k < my_pile->taille; k++) free(my_pile->arr[k].rename);

			free(my_pile->arr);
			free(my_pile);
		}

	//UTILITAIRE:
		void search_id(from * tab, char ** nom, const size_t taille_tuple, Identifiant_or * result)// cherche le nom d'une table  ainsi que le nom d'une colonne dans la pile pour initailiser result 
		{ // cas ou le tuple est plein pas deduction implicite table
			
		//INITIALISE L IDENTIFIANT DE  RESULT: 

			//VAR STATIQUE:
				size_t k = 0, p = 0;
				int tmp = 0;
			
			//INITIALISE ID_TABLE:

				my_erreur((result->id = (Identifiant*)malloc(sizeof(Identifiant))));
				result->id->id_table = search_id2(tab, nom[0]);//cherche le nom de la table dans la pile est renvoie sont adresse , harmonise pour autre utilisation : enleve litteral : rendre FLEXIBLE

			//TROUVE LA COLONNE:

				//Cherche la premiere colonne  non nulle dans la suite des string du tuple:

					for (p = 1; p < taille_tuple && !nom[p]; p++); 
					//GESTION ERREUR: Si aucune colonne n'est trouve 
						if (p == taille_tuple) {
							printf("Erreur la colonne  de la condition  impliquant la table : %s n'a pas ete trouve\n", nom[0]);
							exit(1);
						}

				//trouve colonne dans description:
				tmp = find_description(nom[p], result->id->id_table->adresse_table->description, result->id->id_table->adresse_table->nb_attribut * 2);


				//TODO: enelver securite deja faite en ammont dans le arr d'indice 
				//GESTION ERREUR :

					if (tmp < 0) {
						printf("Erreur la colonne : %s d e la condition n'a pas ete trouvé \n", nom[p]);
						exit(1);
					}

				//INITIALISE NUMERO DE LA COLONNE:

					else {
						//printf("COLONNE :%s indexe :%d \n ", nom[p],2*tmp);
						result->id->indexe_colonne = tmp;
					}

		//FIN INITIALISATION

		}

		element* search_id2(from* tab, char* nom)  // cherche le nom d'une table dans la pile  et renvoie son adresse dans tab
		{
			size_t k = 0;

			for (; k < tab->taille && strcmp(tab->arr[k].rename, nom); k++);
			if (k != tab->taille)return &tab->arr[k];
			else {
				printf("Table : %s  inexistante veuillez verifier votre requete\n", nom);
				exit(1);
			}
		}


	//PROMPT:
		void print_pile(from * my_pile)
	
	{
			printf("PILE , PILE_taille: %d\n", my_pile->taille);
			for (size_t i = 0; i < my_pile->taille; i++)
			{
				print_element(my_pile->arr + i);
				printf("\n");
			}
		}

//ELEMENT de la pile :

		//copy constructor element:
			element* copy_element_init(element* ele) //  constructeur par copy d'element : return l'objet crée 
			{
				//VAR STATIQUE:
				//VAR DYNAMIQUE:
					element* fill = NULL;

					//ALLOUE
						my_erreur(fill = (element*)malloc(sizeof(element)));
					//INITIALISE
						my_cpy(ele, fill); // surcharge de l'operateur de copy 

				return fill;
			}
			element* create_element(element* fill, Table* adresse, char* rename) // constructeur par copy/valeur pas d'allocation 
			{
				//INITIALISE ADRESSE_TABLE:
					fill->adresse_table = adresse;

				//INITIALISE RENAME:

					// SI RENAME:
						if (rename && strcmp(rename, "\0")) { // si un rename a ete operer dans le from
							fill->rename = rename;
							//printf("%#010x rename\n", fill->rename);
						}
					//SINON 
					else {// si aucun rename n'a ete operer prend le nom par defaut de la table dans le tableau de table 

						fill->rename = (char*)malloc(strlen(adresse->name) + 1);
						//printf("ici :%s\n", (adresse->name ? adresse->name : "null"));
						strcpy_s(fill->rename, strlen(adresse->name) + 1, adresse->name);

						free(rename);
					}

				//INTIALISE NUM DE LIGNE :
					fill->counter = 0;

				//INITIALISE A ETE EDITER
					fill->set = 0;

				return fill;
			}

			//OPERATOR
				
				void my_cpy(element* cotenu, element * contenant) // surcharge op de copy pour la classe elment
				{
					contenant->adresse_table = cotenu->adresse_table;
					contenant->rename = cotenu->rename;
					contenant->counter = cotenu->counter;
					contenant->set = cotenu->set;
	
				}
			
			//UTILITAIRE:
					int testcondition(cond* condition)//test une condition est renvoie le bool associe
					{
						//TODO implementer gestion des funct agreg

						//VARIABLE STATIQUE:
							str_or_int my_lhs, my_rhs; // buffer pour stocker lhs et rhs 
							int bol = 0; // booleen resultant de l'operation logique

						//VARIABLE DYNAMIQUE:

						init_str_or_int(&my_lhs, (condition->lhs.id ? &condition->lhs.id->id_table->adresse_table->tab[condition->lhs.id->id_table->counter*condition->lhs.id->id_table->adresse_table->nb_attribut + condition->lhs.id->indexe_colonne] : &condition->lhs.val));

						init_str_or_int(&my_rhs, (condition->rhs.id ? &condition->rhs.id->id_table->adresse_table->tab[condition->rhs.id->id_table->counter*condition->rhs.id->id_table->adresse_table->nb_attribut + condition->rhs.id->indexe_colonne] : &condition->rhs.val));

						//DEBUG : 
							//DISPLAY:
								affichage_str_or_int(&my_lhs);
								affichage_str_or_int(&my_rhs);
								printf("  ");
						//IS EQUAL:
							if (!(strcmp(condition->operation, "=")))
							{
								//IF  IS_STRING?
									if (my_lhs.str && my_rhs.str)
									{

										bol = !(strcmp(my_lhs.str, my_rhs.str));
										free(my_lhs.str); free(my_rhs.str);
										return bol;
									}
								//SINON IF IS_INT?
									else return  my_lhs.nb == my_rhs.nb;

							}
						//IS GREATER OR EQUAL:
							else if (!(strcmp(condition->operation, "<=")))
							{
								//IS STRING?==true
									if (my_lhs.str && my_rhs.str)
									{
										bol = (strcmp(my_lhs.str, my_rhs.str) <= 0);
										free(my_lhs.str); free(my_rhs.str);
										return bol;
									}
								//IS INT?==true
									else return my_lhs.nb <= my_rhs.nb;

							}
						//IS SMALLER OR EQUAL:
							else if (!(strcmp(condition->operation, ">=")))
							{
								//IS STRING?==true
									if (my_lhs.str && my_rhs.str)
									{
										bol = (strcmp(my_lhs.str, my_rhs.str) >= 0);
										free(my_lhs.str); free(my_rhs.str);
										return bol;
									}
								//IS INT?==true
									else return my_lhs.nb >= my_rhs.nb;

							}
						//IS GREATER ?
							else if (!(strcmp(condition->operation, "<")))
							{
								//IS STRING?==true
									if (my_lhs.str && my_rhs.str)
									{
										bol = (strcmp(my_lhs.str, my_rhs.str) < 0);
										free(my_lhs.str); free(my_rhs.str);
										return bol;
									}
								//IS INT?
									else return my_lhs.nb < my_rhs.nb;

							}
						//IS SMALLER?
						else if (!(strcmp(condition->operation, ">")))
						{
							//IS STRING?==true
								if (my_lhs.str && my_rhs.str)
								{
									bol = (strcmp(my_lhs.str, my_rhs.str) > 0);
									free(my_lhs.str); free(my_rhs.str);
									return bol;
								}
							//IS INTEGER?
							else return my_lhs.nb > my_rhs.nb;

						}
						//DEFAULT: si operateur pas definit
							else
							{
								printf("DESOLE OPERATION : %s NON IMPLEMENTE\n", condition->operation);
								return EXIT_FAILURE;
							}


					}

			//DISPLAY:
				void print_element(element * my_elem)
{
	printf("( Nom _table : %s , Counter :%d , Set : %s ) ", my_elem->rename, my_elem->counter, (my_elem->set ? "true" : "false"));
}




		//DEBUT WHERE:

			Table * my_where(FILE* fptr, from * my_pile, tab_tuple* autre)

			{
				//VARIABLE STATIQUE:

						int tmp = 0;	
						size_t rigolo = 0;// indexe de la colonne qui doit actuellement etre copie
						Noeud *racine = NULL; //va contenir l'arbre 
				//VARIABLE DYNAMIQUE:

					int* arr = NULL;//tableau stockant les indice des colonnes du SELECT dans la table resultante, faudrait aussi mettre les fonction d'agreg 
					my_erreur((arr = (int*)malloc(sizeof(int)*autre->cursor))); // alloue exactement le nombre de colonne capté dans le select
					for (size_t rol = 0; rol < autre->cursor; rol++)arr[rol] = -1; // initialise toutes les valeurs  + copier bettement fonction agreg

			//CONSTRUIT SELECT_TABLE:

				//ALLOUE TABLE:
				Table* select_table = crea_default_table();

				//INITIALISE ATTRIBUT SELECT TABLE:
			
					//INITIALISE NB COLONNE :
						for (size_t k = 0; k < my_pile->taille; k++)select_table->nb_attribut += my_pile->arr[k].adresse_table->nb_attribut; // incremente le nombre de colonne necessaire 

					//DESCRIPTION:
						
						//ALLOUE DESCRIPTION:
							my_erreur(select_table->description = (char**)malloc(sizeof(char*)* select_table->nb_attribut * 2));
			
						//INITIALISE DESCRIPTION :

							//TODO TODO TODO
							for (size_t k = 0; rigolo != select_table->nb_attribut * 2 && k < my_pile->taille; k++)
							{
								//ACTUALISE ARR:

									for (size_t rol = 0; rol < autre->cursor; rol++) {//A REFAIRE  ,  si un des nom_table.colonne dans le select  correspond a la table_k (k eme table du FROM ) si oui mettre son futur indexe dans le select dans arr

										// TODO : ne pas break une fois trouver car potentiellement equivalent si  : nom_colonne , name.nom_colonne  et rename.nom_colonne  
										if (!(autre->tab[rol][0]) || !strcmp(autre->tab[rol][0], my_pile->arr[k].rename) || !strcmp(autre->tab[rol][0], my_pile->arr[k].adresse_table->name)) {
											tmp = find_description(autre->tab[rol][1], my_pile->arr[k].adresse_table->description, my_pile->arr[k].adresse_table->nb_attribut * 2);

											//ERREUR :si colonne trouver dans table_K mais deja trouver dans autre table : Table_[0 , k[ 
												if (tmp >= 0 && arr[rol] >= 0) { 
													
													printf("AMBIGUITER avec la colonne %s \n", autre->tab[rol][1]);
													exit(1);
												}

											//SINON (SI LA COLONNE N A PAS DEJA ETAIT TROUVER)
												else if (tmp >= 0) arr[rol] = tmp + rigolo / 2; // si trouve mais pas deja trouver dans [0,k[
										}
									}

								//COPIE LA DESCRIPTION DE LA TABLE_K dans DESCRIPTION TABLE_SELECT:

									for (size_t i = 0; i < my_pile->arr[k].adresse_table->nb_attribut * 2; rigolo++, ++i) {
										
										//ALLOUE STRING :
											my_erreur(select_table->description[rigolo] = (char*)malloc(strlen(my_pile->arr[k].adresse_table->description[i]) + 1));

										//INITIALISE STRING:
											strcpy_s(select_table->description[rigolo], strlen(my_pile->arr[k].adresse_table->description[i]) + 1, my_pile->arr[k].adresse_table->description[i]);

								}


							}

						// TEST SI une colonne du SELECT n'a pas etait trouver dans les table du FROM :

							for (size_t rol = 0; rol < autre->cursor; rol++) {
								//SI colonne non set alors ERREUR :
									if (arr[rol] < 0) { 
										printf("ERREUR une colonne du select n'a pas ete trouvé : %s %s\n ", (autre->tab[rol][0] ? autre->tab[rol][0] : ""), autre->tab[rol][1]); 
										exit(1); 
									}
							}

	
			free_tab_tuple(autre);// libere les nom de colonne du SELECT

		

			//REMPLIT ARBRE:
				racine = createNoeud(fptr, ';', NULL, my_pile);

			//TRAITEMENT DU WHERE:
				lol(my_pile, racine, select_table);  // effectue le parcour de l'abre pour cond

			//DEBUG DISPLAY:
				//print_arbre(racine);
				//printf("\n\n");
				//afficher_description(select_table->description,select_table->nb_attribut*2);//affiche descri 
				//printf("ici \n");
			
			afficher_class(select_table);
			print_select(select_table, arr, autre->cursor);

			//LIBERE VARIABLE STATIQUE:

				free_arbre(racine);

			return select_table;

		}

			//FONCTION UTILE AU TRAITEMENT WHERE :

				//TABLEAU STR_OR_INT APPEND:

						void  append_ligne(str_or_int * buffer, str_or_int * big, const size_t taille_g)//rempli le tableau  pointe par buffer avec big[0;taille_g[  ou avec NULL si default
						{	
								//VARIABLE STATIQUE:
									size_t k = 0;
								//VARIABLE DYNAMIQUE:	

								if (big)//si  NORMAL
								{
									for (k = 0; k < taille_g; k++)
									{
										init_str_or_int(&buffer[k], &big[k]);
									}
								}
								else {// si DEFAUT
									init_str_or_int(&buffer[k], NULL); 
								}
						}

				//APPEND LIGNE PILE :
						void recursive_append(from* my_pile, Table * buffer,size_t k) // ajoute au buffer la ligne resultant de la concatenation des lignes designe par my_pile 
					{
							
							if( k < my_pile->taille)
							{
									//SI la k eme table de la pile  n a pas etait pris en compte dans l evaluation de la rqt :
									if (!(my_pile->arr[k].set)) {
									
											
											printf("cart: avec %s\n\n",my_pile->arr[k].rename);	
											//effectue le produit cartesien de la k eme table du FROM avec les autres tables validant la rqt :

												for (my_pile->arr[k].set = 1; my_pile->arr[k].counter < my_pile->arr[k].adresse_table->nb_element; ++my_pile->arr[k].counter) // desormais  la table_K est  considerer par prise en compte dans la rqt
												{
													printf("\t"); recursive_append(my_pile, buffer, k + 1);
												}

												// la table_K n est plus considerer comme prise en compte dans la rqt:

												my_pile->arr[k].set = 0; 
												my_pile->arr[k].counter = 0;
												printf("\n FIn cart :  avec %s\n\n", my_pile->arr[k].rename);

											//FIN PRODUIT CARTESIEN sur TABLE_K

									}
									//SINON (si la table etait deja prise en compte dans la rqt pas besoin de faire un produit cartesien dessus)
									else { recursive_append(my_pile, buffer, k + 1); }
		
							}
							// si toutes les tables contenu dans le FROM sont considerer comme prise en compte dans la rqt alors ajoute la ligne designe par la pile dans SELECT_TABLE( ici buffer)
							else { append(my_pile, buffer); }
	
					}

						void append(from* my_pile, Table * buffer)//ajoute une ligne au buffer grace au numero de ligne contenu dans la pile : concatenation des lignes de la pile 
					{
						//ALLOUE l espace memeoire associer a la ligne  designer par my_pile dans buffer/SELECT_TABLE->tab
							my_erreur((buffer->tab = (str_or_int*)realloc (buffer->tab, (buffer->nb_element + 1) * (buffer->nb_attribut) * sizeof(str_or_int)))); // realloc systematique car une ligne est potentiellement un tres gros elem

						//VARIABLE STATIQUE:
							str_or_int* cursor_buffer = &buffer->tab[buffer->nb_element*buffer->nb_attribut]; // place le curseur au debut de cette ligne 
						
						//Ajoute la ligne designer par my_pile ( par les counters/num ligne respectif de chaque table contenu dans le FROM )

							for (size_t k = 0; k < my_pile->taille; k++)//Ajout successif de la ligne appartenant a la table my_pile[k]
							{
									append_ligne(cursor_buffer, &my_pile->arr[k].adresse_table->tab[my_pile->arr[k].counter*my_pile->arr[k].adresse_table->nb_attribut], my_pile->arr[k].adresse_table->nb_attribut);
		
								//deplace le curseur de la taille de la ligne qui vient d etre ajouter 
								cursor_buffer += my_pile->arr[k].adresse_table->nb_attribut; 
							}

							//ici cursor_buffer= fin_ligne <=> fin buffer->tab 

						//FIN DE L AJOUT DE LA LIGNE

						++buffer->nb_element; // nb ligne +1
						//DEBUG:
							//DISPLAY:
								print_true(my_pile);
	
						// peut etre devrait return le dernier element ajouter : fin de l actuelle table buffer
					}


				//TRAITEMENT:
						void funct(from* my_pile, Table* buffer, Noeud * parcour, int cond) // permet de remonter dans l arbre en fonction de l evaluation d une condition
							{
								//TODO remplacer AND et OR par gauche et droite resp
							//VAR STATIQUE:
								Noeud * fls = parcour;
							//VAR DYNAMIQUE:


									if (cond) // si la condition est vrai 
									{
										// si l arbre n est pas vide alors remonte jusqu'au prochain ET
										if (parcour) for (parcour = parcour->pere; parcour && strcmp(parcour->val, "AND"); fls = parcour, parcour = parcour->pere);

											if (!parcour) {//si c la racine (<=> condition verifier pour un chemin menant a la racine ) alors ajoute ligne 
												recursive_append(my_pile, buffer, 0);

											}// SI un ET a ete trouve ? (ALORS) relance l evaluation sur l'arbre droit : (SINON)  relance la fonction pour trouver un autre ET
											else (isnotFD(fls) ? lol(my_pile, parcour->filsD, buffer) : funct(my_pile, buffer, parcour, cond));
			

									}
									else {//SINON (si la condition est faussse)

											// si l arbre n est pas vide alors remonte jusqu'au prochain OU
											if(parcour)for (parcour = parcour->pere; parcour && strcmp(parcour->val, "OR"); fls = parcour, parcour = parcour->pere);

											if (!parcour)print_false(my_pile);//si la racine a ete atteinte sans trouve de OU valide le chemin /ligne de pile est evaluer a cond : false

											else(isnotFD(fls) ? lol(my_pile, parcour->filsD, buffer) : funct(my_pile, buffer, parcour, cond)); // si un ou  a etait trouve ralance l evaluation si l arbre droit n a pas deja etait traite sinon remonte avec funct
			
									}

						}
						void lol(from * my_pile, Noeud * root, Table * buffer) //PARCOUR ARBRE ET ACTUALISE PILE 
							{
								//VARIABLE STATIQUE:
									Noeud * parcour = root;
								//VARIABLE DYNAMIQUE:


								// SI l arbre EST vide <=> WHERE VIDE alors produit cartesien des table dans le FROM
								if(!parcour)recursive_append(my_pile, buffer, 0);
				
								//SINON (SI l'arbre n'est PAS vide)
								else{

									// si l'arbre est un node parcour = le minimum de l arbre <=> plus haute priorite <=> plus petit poids
									if (!parcour->verif)for (; parcour->filsG; parcour = parcour->filsG);	 
									//ici :parcour == plus petite leaf du sous arbre considerer

										// SI  l operande GAUCHE EST une table ET qu elle n a PAS deja etait utilise dans la rqt

											if (parcour->verif->lhs.id && !parcour->verif->lhs.id->id_table->set) { 

												//pour chaque ligne de LHS : 
												for (parcour->verif->lhs.id->id_table->set = 1; parcour->verif->lhs.id->id_table->counter < parcour->verif->lhs.id->id_table->adresse_table->nb_element; ++parcour->verif->lhs.id->id_table->counter) {

													//SI l operande DROITE EST une table ET qu elle n a pas deja etait utilise

														if (parcour->verif->rhs.id && !parcour->verif->rhs.id->id_table->set)
														{
															//pour chaque ligne de RHS
															for (parcour->verif->rhs.id->id_table->set = 1; parcour->verif->rhs.id->id_table->counter < parcour->verif->rhs.id->id_table->adresse_table->nb_element; ++parcour->verif->rhs.id->id_table->counter) {

																//test la condition de la feuille parcour  et parcoure l arbre suivant le resultat ;
																funct(my_pile, buffer, parcour, testcondition(parcour->verif));//z et i
															}

															//RHS n est plus considerer comme utilise:
																parcour->verif->rhs.id->id_table->counter = 0;
																parcour->verif->rhs.id->id_table->set = 0;;// table rhs  plus considerer comme utilise dans rqt
														}
													//SI  l operande DROITE n'est PAS une table OU EST  deja utilise

														else { 
															//test la condition de la feuille parcour  et parcoure l arbre suivant le resultat ;
															funct(my_pile, buffer, parcour, testcondition(parcour->verif));//que i
														}

												}
												//LHS n est plus considerer comme utilise:
													parcour->verif->lhs.id->id_table->counter = 0;
													parcour->verif->lhs.id->id_table->set = 0;;// table lhs plus considerer comme utilise

											}

										//SINON (SI l'operande GAUCHE n est PAS une table OU EST DEJA UTILISE )

											//SI l operande droite EST un table ET n'est PAS utilise:
												else if (parcour->verif->rhs.id && !parcour->verif->rhs.id->id_table->set) //  sinon si  l operande droit est une table est qu elle n  a pas deja etait utilise
												{

													//pour chaque ligne de RHS
													for (parcour->verif->rhs.id->id_table->set = 1; parcour->verif->rhs.id->id_table->counter < parcour->verif->rhs.id->id_table->adresse_table->nb_element; ++parcour->verif->rhs.id->id_table->counter) {
														//test la condition de la feuille parcour  et parcoure l arbre suivant le resultat ;
														funct(my_pile, buffer, parcour, testcondition(parcour->verif));//que z

													}
													//RHS n est plus considerer comme utilise:
														parcour->verif->rhs.id->id_table->set = 0;// table rhs plus considerer comme utilise
														parcour->verif->rhs.id->id_table->counter = 0;
												}
											//SI les 2 operandes sont soit PAS des tables soit sont utilisés:

												else { 
													//test la condition de la feuille parcour  et parcoure l arbre suivant le resultat ;
													funct(my_pile, buffer, parcour, testcondition(parcour->verif)); // sinon si les 1 ou 2 table de la condition sont considerer comme utilise

												}
								}

							}

					//DISPLAY :
							void print_false(from* my_pile)
							{
								for (size_t k = 0; k < my_pile->taille; k++)
								{
									printf("table %s element %d ", my_pile->arr[k].adresse_table->name, my_pile->arr[k].counter);
								}
								printf(" <= ligne fausee \n");
							}
							void print_true(from* my_pile)
							{
								for (size_t k = 0; k < my_pile->taille; k++)
								{
									printf("table %s element %d ", my_pile->arr[k].adresse_table->name, my_pile->arr[k].counter);
								}
								printf(" <= ligne vrai\n");
							}
		//FIN WHERE:

//FIN SELECT :
void print_select(const Table *T, int* tab,size_t taille)//res tableau de tuple 
{
	
	//TODO : sorti dans table pour creer une table et non pas simplement print la table Select


	//PROMPT la description de la table select  grace au indice stocké dans tab ( indiquant les indexe des colonnes) 

	for (size_t i = 0; i < taille; i++)printf("Nom %d : \"%s\" ", i,T->description[tab[i]*2]); //*2 pour convertir indexe tab_colonne en indexe description_colonne

	printf("\n");

	for (size_t j = 0; j < T->nb_element; j++)//pour toutes les lignes de la select_table afficher les str_or_int /colonne marqué par le tab  
	{
		
		for (size_t i = 0; i < taille; i++) {
			printf("\t"); affichage_str_or_int(&T->tab[j*T->nb_attribut + tab[i]]);
		}
			
		printf("\n");
	}

}




//UTILITAIRE :


//TODO A METTRE DANS LE HEADER EN INLINE:
int find_cara(const char* str, char cara) // cherche un caractere dans un string
{
	for (size_t l = 0; str[l] != '\0'; l++) if (cara == str[l])return l;
	return -1;
}



//ANCIENNE NOTE:
		// faire un type pour les tuples avec taille_deb_fin , delim  [et taille delim ]
		// faire une recherche dans le select avec les tuples comme pour le where pour pas avoir d ambiguité dans select ca sera ID et ID tmp = getString_arr(fptr, delim, key_tuple, taille_deb_delim_fin);
		//mais grace a la pile la premiere ID sera rattaché a une table et l'autre a une autre  
		//si nom table specifier go direct dans sa range pour opti la recherche 

/*
pos = find_description(res[i], T->description, T->nb_attribut);
			if (pos < 0)
			{
				fprintf(stderr, "Erreur : Colonne introuvable ->%sl\n", res[j]);
				exit(EXIT_FAILURE);
			}
			else {
				for (k = j; k < T->nb_element && (pos2=find_description(res[i], T->description, T->nb_attribut)) < 0; j++);
				if (k < T->nb_element) {
					printf("Ambiguite \n veuillez specifier la colonne : %s dans le fichier  ambiguite trouver avec : %s et %s \n", res[i], T->description[pos] , T->description[pos2]);
					exit(1);
				}
			}
			*/