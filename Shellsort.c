#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELD_SIZE 100

typedef struct {
    char nome[MAX_FIELD_SIZE];
    char formato[MAX_FIELD_SIZE];
    char duracao[MAX_FIELD_SIZE];
    char pais[MAX_FIELD_SIZE];
    char idioma[MAX_FIELD_SIZE];
    char emissora[MAX_FIELD_SIZE];
    char transmissao[MAX_FIELD_SIZE];
    int num_temporadas;
    int num_episodios;
} Serie;

char *remove_line_break(char *line) {
    while (*line != '\r' && *line != '\n') line++;
    *line = '\0';
    return line;
}

char *freadline(char *line, int max_size, FILE *file) {
    return remove_line_break(fgets(line, max_size, file));
}

char *readline(char *line, int max_size) {
    return freadline(line, max_size, stdin);
}

void print_serie(Serie *serie) {
    printf("%s %s %s %s %s %s %s %d %d\n",
        serie->nome,
        serie->formato,
        serie->duracao,
        serie->pais,
        serie->idioma,
        serie->emissora,
        serie->transmissao,
        serie->num_temporadas,
        serie->num_episodios
    );
}

// Retorna o tamanho em bytes de um arquivo.
long tam_arquivo(FILE *file) {
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

// Retorna todo o conteúdo do arquivo numa string.
char *ler_html(char filename[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Falha ao abrir arquivo %s\n", filename);
        exit(1);
    }
    long tam = tam_arquivo(file);
    char *html = (char *) malloc(sizeof(char) * (tam + 1));
    fread(html, 1, tam, file);
    fclose(file);
    html[tam] = '\0';
    return html;
}

/**
 * @brief Extrai os textos de uma tag html.
 * 
 * @param html Ponteiro para o caractere que abre a tag '<'.
 * @param texto Ponteiro para onde o texto deve ser colocado.
 * 
 * @return Ponteiro para o texto extraído.
 */
char *extrair_texto(char *html, char *texto) {
    char *start = texto;
    int contagem = 0;
    while (*html != '\0') {
        if (*html == '<') {
            if (
                (*(html + 1) == 'p') ||
                (*(html + 1) == 'b' && *(html + 2) == 'r') ||
                (*(html + 1) == '/' && *(html + 2) == 'h' && *(html + 3) == '1') ||
                (*(html + 1) == '/' && *(html + 2) == 't' && *(html + 3) == 'h') ||
                (*(html + 1) == '/' && *(html + 2) == 't' && *(html + 3) == 'd')
            ) break;
            else contagem++;
        }
        else if (*html == '>') contagem--;
        else if (contagem == 0 && *html != '"') {
            if (*html == '&') html = strchr(html, ';');
            else if (*html != '\r' && *html != '\n') *texto++ = *html;
        }
        html++;
    }
    *texto = '\0';
    return *start == ' ' ? start + 1 : start;
}

void ler_serie(Serie *serie, char *html) {
    char texto[MAX_FIELD_SIZE];

    char *ptr = strstr(html, "<h1");
    extrair_texto(ptr, texto);

    char *parenteses_ptr = strchr(texto, '(');
    if (parenteses_ptr != NULL) *(parenteses_ptr - 1) = '\0';

    strcpy(serie->nome, texto);

    ptr = strstr(ptr, "<table class=\"infobox_v2\"");

    ptr = strstr(ptr, "Formato");
    ptr = strstr(ptr, "<td");
    strcpy(serie->formato, extrair_texto(ptr, texto));

    ptr = strstr(ptr, "Duração");
    ptr = strstr(ptr, "<td");
    strcpy(serie->duracao, extrair_texto(ptr, texto));

    ptr = strstr(ptr, "País de origem");
    ptr = strstr(ptr, "<td");
    strcpy(serie->pais, extrair_texto(ptr, texto));
    if(strstr(serie->pais,"Estados Unidos ")!=NULL){
        strcpy(serie->pais, "Estados Unidos");
    }

    ptr = strstr(ptr, "Idioma original");
    ptr = strstr(ptr, "<td");
    strcpy(serie->idioma, extrair_texto(ptr, texto));

    ptr = strstr(ptr, "Emissora de televisão original");
    ptr = strstr(ptr, "<td");
    strcpy(serie->emissora, extrair_texto(ptr, texto));

    ptr = strstr(ptr, "Transmissão original");
    ptr = strstr(ptr, "<td");
    strcpy(serie->transmissao, extrair_texto(ptr, texto));

    ptr = strstr(ptr, "N.º de temporadas");
    ptr = strstr(ptr, "<td");
    sscanf(extrair_texto(ptr, texto), "%d", &serie->num_temporadas);

    ptr = strstr(ptr, "N.º de episódios");
    ptr = strstr(ptr, "<td");
    sscanf(extrair_texto(ptr, texto), "%d", &serie->num_episodios);
}

#define MAXTAM 100


Serie series[MAXTAM];
int n;
void start(){ 
	n = 0;
}

int isFim(char line[]) {
    return line[0] == 'F' && line[1] == 'I' && line[2] == 'M';
}

void imprimir(Serie serie){
printf ( "%s %s %s %s %s %s %s %d %d\n", serie.nome, serie.formato, serie.duracao, serie.pais, serie.idioma, serie.emissora, serie.transmissao, serie.num_temporadas, serie.num_episodios );

}

void mostrar (){
    int i = 0;
	for ( i = 0; i < n; i++){
		imprimir(series[i]);
	}

}

    
 void swap(int x, int y){
            Serie tmp = series[x];
            series[x] = series[y];
            series[y] = tmp;
  }

void selecao() {
    int i = 0;
    for (i=0;i < (n - 1); i++) {
      int menor = i;
      int j = (i + 1);
      for (j= (i+1);j < n; j++){
         if (strcmp (series[menor].pais,series[j].pais)>0 || ((strcmp (series[menor].pais,series[j].pais)==0 && strcmp (series[menor].nome,series[j].nome)>0))){
            menor = j;
         }
      }
      swap(menor, i);
   }
}

void insercaoPorCor(int cor, int h){
    int i = (h + cor);
    for (i = (h + cor); i < n; i+=h) {
        Serie tmp = series[i];
        int j = i - h;
        while ((j >= 0) && ((strcmp(series[j].idioma,tmp.idioma)>0) || ((strcmp(series[j].idioma,tmp.idioma)==0 && strcmp(series[j].nome,tmp.nome)>0 )))) {
            series[j + h] = series[j];
            j-=h;
        }
        series[j + h] = tmp;
    }
}
//=============================================================================
void shellsort() {
    int h = 1;
    int cor = 0;
    do { h = (h * 3) + 1; } while (h < n);

    do {
        h /= 3;

        for(cor = 0; cor < h; cor++){
            insercaoPorCor(cor, h);
        }
    } while (h != 1);
}


void Inserirserie(Serie x){
series[n]=x;
n++;
}

#define MAX_LINE_SIZE 250
 #define PREFIXO "/tmp/series/"
// #define PREFIXO ""
// #define PREFIXO "series/"


int main() {
    start();
    Serie serie;
    size_t tam_prefixo = strlen(PREFIXO);
    char line[MAX_LINE_SIZE];

    strcpy(line, PREFIXO);
    readline(line + tam_prefixo, MAX_LINE_SIZE);

    while (!isFim(line + tam_prefixo)) {
        char *html = ler_html(line);
        ler_serie(&serie, html);
        free(html);
        Inserirserie(serie);
        // print_serie(&serie.seleção());
        readline(line + tam_prefixo, MAX_LINE_SIZE);
    }
   shellsort();
   mostrar();
    return EXIT_SUCCESS;
}
 
