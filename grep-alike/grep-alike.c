#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Declaracoes das funcoes a serem usadas
int search_lines_c(char[], char[]); //ignora caps
int search_lines(char[], char[]); //case sensitive

//bloco principal do programa
//MODO DE USO:
// ./arquivo.exe [-caps] string arquivo1 arquivo2 (...) arquivoN
int main(int argc, char *argv[]){
  //Filter for safe program
  if(argc<3){printf("ERRO NO NÚMERO DE ARGUMENTOS\nMODO DE USO:\n\
./arquivo.exe [-caps] string arquivo1 arquivo2 (...) arquivoN\n");exit(1);}
  //Caso tenha a opcao -caps, vai usar a devida funcao
  if(strcmp(argv[1],"-caps")==0){
    for(int i=3; i<argc; ++i){
      search_lines_c(argv[2],argv[i]);
    }
  }
  else{
    for(int i=2; i<argc; ++i){
      search_lines(argv[1],argv[i]);
    }
  }
  printf("\n");
  return 0;
}

//funcao para procurar uma string em um arquivo
//e imprimir as linhas encontradas
int search_lines(char busca[], char aqv[]){
  char  line[256];
  int lnum = 0, aux=0;
  FILE *target = fopen(aqv, "r");
  if(target == NULL){perror(aqv);exit(1);}
  printf("\n\e[4m%s\e[24m:\n",aqv);
  while(fgets(line, sizeof(line), target)){
    ++lnum;
    if(strstr(line,busca)){
      aux = 1;
      printf("[%d]: %s",lnum, line);
    }
  }
  //Caso nada foi encontrado, imprimir o seguinte aviso
  if(aux==0){printf(">>> O arquivo não contém a cadeia '%s'\n",busca);}
  return 0;
}

//copia da funcao acima, mas mudando "strstr" por "strcasestr"
//o que faz a funcao deixar de ser case sensitive
int search_lines_c(char busca[], char aqv[]){
  char  line[256];
  int lnum = 0, aux=0;
  FILE *target = fopen(aqv, "r");
  if(target == NULL){perror(aqv);exit(1);}
  printf("\n\e[4m%s\e[24m:\n",aqv);
  while(fgets(line, sizeof(line), target)){
    ++lnum;
    if(strcasestr(line,busca)){
      aux = 1;
      printf("[%d]: %s",lnum, line);
    }
  }
  if(aux==0){printf(">>> O arquivo não contém a cadeia '%s'\n",busca);}
  return 0;
}
