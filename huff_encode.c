
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


void ConstruireCode(Arbre huff) {
    /* A COMPLETER */
    printf("Programme non realise (ConstruireCode)\n");
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

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
