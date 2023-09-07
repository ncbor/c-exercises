#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


typedef uint32_t u32;
#define M UINT32_MAX
//AQUI É DEFINIDO O TAMANHO DO HASHCODE, NO CASO 32 BITS
//PARA UTILIZAR 16 BITS, POR EXEMPLO, TROCAR UINT32 por UINT16


int hash(char, u32*, u32*); //função para o hash
int string_hash(char*, u32*, u32*); //aplica o hash em uma string
int file_hash(FILE*, u32*, u32*); //aplica o hash em um arquivo

int main(int argc, char *argv[])
{
  //utilizacao: ./hash.exe <string/arquivo>
  //o arquivo ou string a ser criado o hash deve ser colocado como argumento
  //o programa aceita apenas um argumento
  if(argc<=1){printf("faltam argumentos ok\n");return 1;}//checa o input do usuário

  u32 xi=0, x0 = 4000000;//inicializa as variaveis, x0 é a seed

  if (access(argv[1], F_OK) == 0)
  {
    FILE *f = fopen(argv[1], "r");
    file_hash(f,&xi,&x0);
    printf("O hash do arquivo \"%s\" é: %x\n",argv[1], xi);
  }//para o caso de ser um arquivo

  else
  {
    char *p = argv[1];
    string_hash(p,&xi,&x0);
    printf("O hash de \"%s\" é: %x\n",argv[1],xi);
  }//para o caso de ser uma string

  return 0;
}

int hash(char p, u32 *xi, u32 *x0)
{
  // x0 é a seed e P indica o ascii do caractere em questão
  u32 a = 13, c = 31;
  // M = Hash Size (32 BITS)
  // A e C arbritarios, primos conferem mais precisão, de mercenne mais ainda
  (*xi) = ( a *( (*x0) +p ) +c ) % M;
  //Xi = A(X0+P)+C % M [gerador de congruencia linear]
  return 0;
}

int string_hash(char *c, u32 *xi, u32 *x0)
{
  for(;*c;++c)
  {
    hash(*c, xi, x0);
    x0=xi;
  }//aplica o hash para todos o caractéres de uma string
  return 0;
}

int file_hash(FILE *f, u32 *xi, u32 *x0)
{
  char line[1000];//tamanho limite da linha
  while (fgets(line, sizeof(line), f)!=NULL)
  {
    string_hash(line, xi, x0);
    x0=xi;
  }//aplica o hash consecutivamente para todas linhas do arquivo
  return 0;
}
