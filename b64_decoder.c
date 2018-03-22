#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ARGS 2
#define octet_pair unsigned char

#define ALPHABET "=BCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

int findLowestMultiple(int n, int multiple){
  
  int correct = n;
  while(correct %multiple != 0){
    correct--;
  }
  return correct;
}

int findIndex(char c){
  size_t walk = 0;
  while(walk < strlen(ALPHABET)){
    if(ALPHABET[walk] == c){
      return walk;
    }
    walk++;
  }
  //This is an error. This should never happen
  return -1;
}

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
    size_t iterations = 0;
    size_t input_len = strlen(inputString);

    size_t current_index;
    char next_char;

    while(1){
      current_index = input_len -1 -iterations;
      next_char = inputString[current_index];
      if(next_char != '='){
        break;
      }
      inputString[current_index] = '\0';
      iterations++;
    }
    //print_bits(len,inputString);
    current_index = 0;
    next_char = findIndex(inputString[current_index]);
    j = 0;
    size_t k =0;
    i = 0;
    unsigned char bitToRead = 0x20;
    unsigned char bitToInsert = 0x80;
    size_t shift_amount = 5;
    size_t closestMultiple = findLowestMultiple(strlen(inputString)*6,8);
    while(1){
      unsigned char next_bit = ( next_char & bitToRead ) >> shift_amount;
      bitToRead = bitToRead >> 1;
      bitToInsert = bitToInsert >> 1;
      next_pair = next_pair << 1;
      next_pair = (next_pair | next_bit);
      shift_amount--;
      /*
       printf("adding a %d\n",next_bit);
       print_bits(sizeof(next_pair),&next_pair);
       */
      j++;
      k++;
      num_bits++;
      if(j == 6){
       current_index++;
       next_char = findIndex(inputString[current_index]);
       bitToRead = 0x20;
       shift_amount = 5;
       j = 0;
      }
      if(k == 8){
       b64string[i] = next_pair;
       i++;
       k = 0;
       next_pair = 0;
       bitToInsert = 0x80;
       num_bits = 0;
      }
      if(i >= closestMultiple/8){
        break;
      }
    }
    printf("%s\n",b64string);
  }
}
