#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ARGS 2
#define octet_pair unsigned char

#define ALPHABET "=BCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

void print_error(void){
  printf("wrong num args (fix this error message)");
}

void print_bits(size_t const size, void const* const ptr){
  unsigned char * b= (unsigned char *)ptr;
  unsigned char byte;
  int i,j;

  for(i=size; i>=0; i--){
    for(j=7; j>=0; j--){
      byte = (b[i] >> j) & 1;
      printf("%u",byte);
      if(j == 4){
        printf("_");
      }
    }
  }
  printf(" | ");
}

int main(int argc, char** argv){
  size_t i,j,num_bits,b64Index;
  char* b64string = (char*)malloc(10000);
  char* inputString = (char*)malloc(10000);
  if(argc != NUM_ARGS){
    print_error();
  }else{
    i = 0;
    j = 0;
    num_bits = 0;
    b64Index = 0;
    memset(b64string,0,10000);
    memset(inputString,0,10000);

    size_t len = strlen(argv[1]);
    size_t carry_over = (argv[1][i] & 0x03) << 6;
    size_t octet = argv[1][i] >> 2;
    size_t padding_zeros = (len << 3) % 6 == 4 ? 1 : (len << 3) % 6;
    octet_pair* octets = (octet_pair *)malloc(sizeof(octet_pair) * len);
    
    memcpy(inputString,argv[1],len);
    octet_pair next_pair = 0;
    //printf("len before padding %d\n",len);
    len = len + padding_zeros;
    //printf("adding %d padding: len is now %d\n",padding_zeros,len);
    unsigned char current = inputString[i];

    //print_bits(len,inputString);
    while(1){
      unsigned char next_bit = ( current & 0x80 ) >> 7;
      next_pair = next_pair << 1;
      next_pair = (next_pair | next_bit);
      current = current << 1;
      /*
       printf("adding a %d\n",next_bit);
       print_bits(sizeof(next_pair),&next_pair);
       */
      j++;
      num_bits++;
      if(j >= 6){
        //print_bits(sizeof(next_pair),&next_pair);
        b64string[b64Index] = ALPHABET[next_pair];
        b64Index++;
        next_pair=0;
        j=0;
      }
      if(num_bits >= 8){
       i++;
       current = argv[1][i];
       num_bits = 0;
      }
      if(i >= len){
        break;
      }
    }
while(padding_zeros > 0){
      size_t final_len = strlen(b64string);
      size_t selectedIndex = final_len - padding_zeros;
      //printf("padding at %d which is %c\n",selectedIndex,b64string[selectedIndex]);
      b64string[selectedIndex] = '=';
      padding_zeros--;
    }
    printf("%s\n",b64string);
  }
}
