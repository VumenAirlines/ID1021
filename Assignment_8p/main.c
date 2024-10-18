#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define BUFFER 256

typedef struct node {
    bool valid;
    struct node *next[27];
} node;

typedef struct trie {
  node *root;
} trie;


//a has character code 97
int code(wchar_t w) {
 if(w==113||w==119){
        printf("strange character in word: %d\n", w);
        return -1;
    }
    if(w>96&&w<113)
        return w-97;
    if(w>113&&w<119)
        return w-98;
    if(w>119&&w<123)
        return w-99;
    switch (w) {
        case 229 :
            return 24;
        case 228 :
            return 25;
        case 246 :
            return 26;       
    }
    printf("strange character in word: %d\n", w);
    return -1;;
}
wchar_t character(int c) {
  
    if(c>=0&&c<=15)
        return c+97;
    if(c>15&&c<=20)
        return c+98;
    if(c>20&&c<=23)
        return c+99;
    switch (c) {
        case 24 :
            return 229;
        case 25 :
            return 229;
        case 26 :
            return 246;       
    }
    printf("strange character in woord: %d\n", c);
    return -1;
}
int key_to_index(char key) {
    if(key<49||key>57)
        return -1;
    return key-49;
}
char character_to_key(wchar_t w) {
    if(w==113||w==119||w<97||(w>123 && (w!=229&&w!=228&&w!=246))){
        printf("strange character in word: %d\n", w);
        return -100;
    }
    return (code(w)/3 + 1)+48;
}

node *add(node *nd, wchar_t *rest) {
    if (nd == NULL) {
        nd = (node*)malloc(sizeof(node));
        for (int i = 0; i < 27; i++ ) {
            nd->next[i] = NULL;
        }
        nd->valid = false;
    }
   
    if (rest[0] == '\n') nd->valid = true;   
    else {
        int c = code((int)*rest);
        nd->next[c] = add(nd->next[c], (rest + 1));
    }
    return nd;
}
trie *dict() {
    setlocale(LC_ALL,"en_US.UTF-8");
    // Open the dictionary in read mode
    FILE *fptr = fopen("kelly.txt", "r");
    if (fptr == NULL)
        return NULL;
    trie *kelly = (trie*)malloc(sizeof(trie));
    kelly->root = NULL;
    wchar_t ws[BUFFER];
    while (fgetws(ws, BUFFER, fptr) != NULL) {
        printf("adding %S", ws);
        kelly->root = add(kelly->root, ws);
    }
    fclose(fptr);
    return kelly;
} 

void collect(node *nd, char *keys, int step, wchar_t* seq, int *count, wchar_t** valid_words){
    if(keys[step]=='\0'){
        if(nd->valid){
            //seq[step]=L'\0';
            
            valid_words[(*count)]=(wchar_t*)malloc(sizeof(wchar_t)*(step + 1));
            wcscpy(valid_words[(*count)++],seq);
            printf("Valid: %S, %d \n", seq, step);
        } 
        return;
        
    }
    int index = key_to_index(keys[step]);
    for (int i = 0; i < 3; i++)
    {
        int chcode=index*3+i;
       //printf("%d, %d, %d,%d\n", chcode, index,step, keys[step]);
        if(nd->next[chcode]==NULL) continue;      
        wchar_t* tmp = (wchar_t*) malloc(sizeof(wchar_t)*(step+2));
        wcsncpy(tmp,seq,step);     
        tmp[step] = character(chcode); 
        tmp[step+1]= L'\0';
        collect(nd->next[chcode],keys,step + 1,tmp,count,valid_words);
        free(tmp);
    }
}

wchar_t** decode(trie* trie, char* keys) {
    
    wchar_t **valid_words = (wchar_t**)malloc(sizeof(wchar_t*)*50);
    wchar_t *seq =(wchar_t*)malloc(sizeof(wchar_t));
    int count = 0;
    collect(trie->root, keys, 0, seq, &count, valid_words);
    return valid_words;
}
char* text_to_keys(wchar_t* w) {
    char* text = (char*)malloc(sizeof(char)*wcslen(w));
    int i = 0;
    while(*w != L'\0') {
        text[i++] = character_to_key(*w);
        w++;
    }
    text[i]='\0';
    return text;
}

int main(){
    setlocale(LC_ALL,"en_US.UTF-8");    
    trie* tr = dict();
    char* keys = text_to_keys(L"eld");
    printf("%s \n", keys);
    wchar_t** results = decode(tr, keys);
    printf("%ls\n", *(results));

    return 0;
}