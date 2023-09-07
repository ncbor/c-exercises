#include <stdio.h>
#include <stdint.h>

typedef uint32_t u32;
#define M UINT32_MAX

int main(int argc, char *argv[])
{
  //O PROGRAMA PEGA OS ARQUIVOS "encrypted" E "key", SEM PRECISAR DE ARGUMENTOS
  //CASO ESSES ARQUIVOS NÃO EXISTAM, O PROGRAMA NÃO VAI RODAR, OU SEJA
  //É NECESSPARIO USAR O crypt.exe ANTES!!!

  FILE *tg; FILE *k;

  //error handling and file opening start
  if(!(tg = fopen("encrypted", "r")))
  {
    printf("Cifra faltando!\n");
    return 1;
  }
  else if(!(k = fopen("key","r")))
  {
    printf("Chave faltando!\n");
    return 1;
  }
  //error handling and file opening finish

  else
  {
    u32 c, key;
    FILE *f = fopen("decrypted","w+");//abre arquivo alvo ("decrypted")
    while (fscanf(tg,"%x\n",&c)!=EOF)//le a cifra
    {
      fscanf(k,"%x\n",&key);//le a chave
      fprintf(f, "%c",c ^ key);//decripta e já escreve no arquivo alvo ("decrypted")
    }
  }
  return 0;
}
