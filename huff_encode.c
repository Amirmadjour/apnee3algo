
#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];

void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {

    int c;
    int i;

    for (i = 0; i < 256; i++) {
        TableOcc->tab[i] = 0;
    }

    c = fgetc(fichier);
    while (c != EOF) {
        if (c >= 0 && c < 256 && c != '\n' && c != ' ') {
            TableOcc->tab[c]++;
        }
        c = fgetc(fichier);
    };


    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

fap InitHuffman(TableOcc_t *TableOcc) {
    fap file = creer_fap_vide();
    int i;
    Arbre feuille;

    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] > 0) {
            feuille = NouveauNoeud(ArbreVide(), (Element)i, ArbreVide());
            file = inserer(file, feuille, TableOcc->tab[i]);
        }
    }

    return file;
}

Arbre ConstruireArbre(fap file) {
    Arbre gauche, droite;
    Arbre nouveau_noeud;
    int priorite_gauche, priorite_droite;
    int nouvelle_priorite;

    while (!est_fap_vide(file) && file->prochain != NULL) {
        file = extraire(file, &gauche, &priorite_gauche);
        file = extraire(file, &droite, &priorite_droite);

        nouvelle_priorite = priorite_gauche + priorite_droite;
        
        /* Utiliser 0 comme caractère pour les nœuds internes */
        nouveau_noeud = NouveauNoeud(gauche, (Element)0, droite);
        
        file = inserer(file, nouveau_noeud, nouvelle_priorite);
    }

    if (!est_fap_vide(file)) {
        file = extraire(file, &nouveau_noeud, &nouvelle_priorite);
        return nouveau_noeud;
    }

    return ArbreVide();
}


void ConstruireCodeRec(Arbre huff, int code[], int longueur) {
    if (EstVide(huff)) {
        return;
    }
    
    /* Si c'est une feuille (les deux enfants sont vides) */
    if (EstVide(FilsGauche(huff)) && EstVide(FilsDroit(huff))) {
        Element caractere = Etiq(huff);
        /* Stocker le code pour ce caractère */
        HuffmanCode[caractere].lg = longueur;
        int i;
        for (i = 0; i < longueur; i++) {
            HuffmanCode[caractere].code[i] = code[i];
        }
        return;
    }
    
    /* Parcourir le fils gauche (ajouter 0) */
    if (!EstVide(FilsGauche(huff))) {
        code[longueur] = 0;
        ConstruireCodeRec(FilsGauche(huff), code, longueur + 1);
    }
    
    /* Parcourir le fils droit (ajouter 1) */
    if (!EstVide(FilsDroit(huff))) {
        code[longueur] = 1;
        ConstruireCodeRec(FilsDroit(huff), code, longueur + 1);
    }
}

void ConstruireCode(Arbre huff) {
    int i;
    int code[256];
    
    /* Initialiser tous les codes à une longueur de 0 */
    for (i = 0; i < 256; i++) {
        HuffmanCode[i].lg = 0;
    }
    
    /* Construire les codes en parcourant l'arbre */
    if (!EstVide(huff)) {
        ConstruireCodeRec(huff, code, 0);
    }
}

void AfficherTableHuffman() {
    int i, j;
    printf("\n=== Table de codage Huffman ===\n");
    for (i = 0; i < 256; i++) {
        if (HuffmanCode[i].lg > 0) {
            printf("Caractere '%c' (code ASCII %d) : ", i, i);
            printf("longueur = %d, code = ", HuffmanCode[i].lg);
            for (j = 0; j < HuffmanCode[i].lg; j++) {
                printf("%d", HuffmanCode[i].code[j]);
            }
            printf("\n");
        }
    }
    printf("================================\n\n");
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    /* A COMPLETER */
    printf("Programme non realise (Encoder)\n");
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

        AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);
    
    /* Afficher la table Huffman pour debug */
    AfficherTableHuffman();

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
