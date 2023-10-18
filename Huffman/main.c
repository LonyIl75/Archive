
#include "pch.h"



int main()
{

	compress("lol.txt");
	/*FILE * tr = open_fichier("tr.txt", "a+");
	//fprintf(tr, "azer%c", '\0');
	fprintf(tr, "%zu", 4182331081);
	fprintf(tr, "%c%c%c%c%c",'l',' ','\0','y', '\0');
	fclose(tr);
	tr = open_fichier("tr.txt", "r");
	char cara;
	int lolor;
	
	fscanf_s(tr,"%zu",&lolor,sizeof(int));
	while ((cara = getc(tr)) && cara != '\0')printf("cara: %c\n", cara);
	printf("%zu", lolor);
	*/
	//decompression("compress.txt","resultat.txt");
	
	return 0;
	
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
/*
int borne_inf = (32 - (masque->cursor % 32)) - (cursor2 - cursor1 - 1);
			if (borne_inf>0)
			{
				masque->moninte[masque->taille - 1] |= ((~0 >> 32 - masque->cursor));
				borne_inf= 32 - (masque->cursor % 32);

				for (size_t k = 1; k < taille - 1; k++)
				{
					~(masque->moninte[masque->taille++] & 0); // que des 1 taille ex: 0,1,2 , taille =3
				}
				if(borne_inf)
			}
			else
			{
				masque->moninte[masque->taille - 1] |= ((~0 >> 32 - masque->cursor));
				for (size_t k = 1; k <= 32; k++)
				{
					((~0 >> 32 - masque->cursor) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
				}
				printf("\n");*/
/*
if (taille > 1)
		{
			//traite 2 temps cursor
			print_masque(masque);
			printf("\n");
			int ajoute = masque->cursor - cursor1;
			masque->moninte[masque->taille - 1] |= ((~0 << 32 - masque->cursor) ^ (~0 << 32 - masque->cursor - (32- (cursor1 + 1))));
			masque->cursor += 32 - (cursor1 + 1);
			print_masque(masque);
			printf("taille : %d\n",taille);
			realloc(masque->moninte, (masque->taille + taille-1) * sizeof(int));//voir pour taille car 1 de trop je crois
			//printf("new masque taille : %d\n",masque->taille+taille);
			if (taille > 3)
			{
				for (size_t k = 1; k < taille - 1; k++)
				{
					~(masque->moninte[masque->taille++] & 0); // que des 1 taille ex: 0,1,2 , taille =3
				}
			}
			masque->cursor += (taille - 1) * 32;
			printf("\n");
			print_masque(masque);
			printf("\n");
			masque->moninte[masque->taille++] |= (~0 << 32 - cursor2); // set de 32 a cursor2 les bits masque
			for (size_t k = 0; k < 32; k++)
			{
				((~0 << 32 - cursor2) & (1u << 31 - k)) ? printf("%c", '1') : printf("%c", '0');
			}
			printf("\n");
			if (ajoute < 0) {
				if (cursor2 - ajoute > 0) {
					masque->moninte[masque->taille - 1] << (-ajoute);
					for (size_t k = 1; k < 32; k++)
					{
						((masque->moninte[masque->taille-1]) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
					}
					printf("\n");
					masque->cursor = cursor2 - ajoute;
				}
				else {
					(masque->moninte[--masque->taille] & 0);
					masque->moninte[masque->taille - 1] << (ajoute-cursor2);
					for (size_t k = 1; k < 32; k++)
					{
						((masque->moninte[masque->taille - 1]) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
					}
					printf("\n");
					masque->cursor = 32-(ajoute - cursor2);
				}
				int borne_inf = (32 - (masque->cursor % 32)) - (cursor2 - cursor1 - 1);
				if (borne_inf > 0)
				{
					masque->moninte[masque->taille - 1] |= ((~0 >> 32 - masque->cursor));
					borne_inf = 32 - (masque->cursor % 32);

					for (size_t k = 1; k < taille - 1; k++)
					{
						~(masque->moninte[masque->taille++] & 0); // que des 1 taille ex: 0,1,2 , taille =3
					}
					if (borne_inf)
				}
				else
				{
					masque->moninte[masque->taille - 1] |= ((~0 >> 32 - masque->cursor));
					for (size_t k = 1; k <= 32; k++)
					{
						((~0 >> 32 - masque->cursor) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
					}
					printf("\n");
			}
			//printf("\n");
			masque->cursor += cursor2;
			print_masque(masque);
			printf("\n");
		}
		else if ((cursor1 + 1) < cursor2)
		{
		int borne_inf = (32 - (masque->cursor % 32)) - (cursor2 - cursor1 - 1);
		if (borne_inf > 0)
		{
			masque->moninte[masque->taille - 1] |= ((~0 >> 32 - masque->cursor));
			borne_inf = 32 - (masque->cursor % 32);

			for (size_t k = 1; k < taille - 1; k++)
			{
				~(masque->moninte[masque->taille++] & 0); // que des 1 taille ex: 0,1,2 , taille =3
			}
			if (borne_inf)
		}
		else
		{
			masque->moninte[masque->taille - 1] |= ((~0 >> 32 - masque->cursor));
			for (size_t k = 1; k <= 32; k++)
			{
				((~0 >> 32 - masque->cursor) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
			}
			printf("\n");
			print_masque(masque);
			printf("LOL\n");
			//entre cursor1+1 et cursor2
			//printf("%d cursor2 :%d\n\n", cursor1, cursor2);
			//print_masque(masque);
			masque->moninte[masque->taille - 1] |= ((~0 << 32 - masque->cursor) ^ (~0 << 32 - masque->cursor - (cursor2 - (cursor1 + 1))));
			for (size_t k = 1; k < 32; k++)
			{
				(((~0 << 32 - masque->cursor) ^ (~0 << 32 - masque->cursor - (cursor2 - (cursor1+1)))) & (1u << 32 - k)) ? printf("%c", '1') : printf("%c", '0');
			}
			printf("\n");
			//unset the n-cursor1+1  //unset the n-cursor2
			masque->cursor += cursor2 - (cursor1 + 1);
			print_masque(masque);
			printf("\n");
			//printf("maze:%d\n", masq*/