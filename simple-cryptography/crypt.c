#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

typedef uint32_t u32; //DEFININDO 32 BITS
#define M UINT32_MAX

u32 hash(int, u32*, u32*); // MESMA FUNÇÃO UTILIZADA ANTERIORMENTE

int main(int argc, char *argv[])
{ // AQUI O PROGRAMA DEVE SER UTILIZADO COMO ./crypt.exe <arquivo alvo>
  // O PRÓPRIO PROGRAMA VAI GERAR O ARQUIVO ENCRIPTADO E A CHAVE

  if(argc<=1){printf("faltam argumentos ok\n");return 1;}
  //checa o input do usuário

  if (access(argv[1], F_OK) != 0)
  {
    printf("Arquivo inexistente.\n");
    return 1;
  }
  //CHECANDO SE O ARQUIVO A SER ENCRIPTADO EXISTE
  else
  {
    u32 xi=0, x0 = 4000000;//inicializa as variaveis, x0 é a seed
    FILE *tg = fopen("encrypted", "w+"), *k = fopen("key","w+");
    //ARQUIVOS A SEREM ESCRITOS
    FILE *f = fopen(argv[1], "r");
    fseek(f,0L,SEEK_END);
    int sz=ftell(f);//QUANTIDADE DE CARACTERES A SEREM ENCRIPTADOS
    rewind(f);
    u32 key[sz], crypt;
    for(int i = 0; i<sz; ++i)
    {
      int c = fgetc(f);//Pega um caractére
      key[i]=hash(c,&xi,&x0);//Cria a chave correspondente a ele
      crypt = c ^ key[i];//Criptografia utilizando XOR
      fprintf(k,"%x\n",key[i]);//Escreve a chave
      fprintf(tg,"%x\n",crypt);//Escreve a cifra
    }
  }
  return 0;
}

u32 hash(int p, u32 *xi, u32 *x0)
{
  u32 a = 13, c = 31;
  (*xi) = ( a *( (*x0) +p ) +c ) % M;
  (*x0)=(*xi);
  return (*xi);
}
