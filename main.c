#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define OFFSETBITS 5
#define LENGTHBITS (8-OFFSETBITS)
#define OFFSETMASK ((1 << (OFFSETBITS)) - 1)
#define LENGTHMASK ((1 << (LENGTHBITS)) - 1)
#define GETOFFSET(x) (x >> LENGTHBITS)
#define GETLENGTH(x) (x & LENGTHMASK)
#define OFFSETLENGTH(x,y) (x << LENGTHBITS | y)
#define MAX_TREE_HT 100
char bindizi[256][10];
int sayacbin=0;
int kackarakter;//bindizide kac tane eleman oldugunu burda tutuyoruz
int index=0;
char karakterdizi[256];
struct token
{
    uint8_t offset_len;
    char c;
};
struct MinHeapNode
{
    char data;
    unsigned siklikdizi;
    struct MinHeapNode *left, *right;
};
struct MinHeap
{
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

struct MinHeapNode* newNode(char data, unsigned siklikdizi)
{
    struct MinHeapNode* temp= (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->siklikdizi = siklikdizi;

    return temp;
}
struct MinHeap* createMinHeap(unsigned capacity)
{

    struct MinHeap* minHeap= (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array= (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->
            siklikdizi < minHeap->array[smallest]->siklikdizi)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->siklikdizi < minHeap->array[smallest]->siklikdizi)
        smallest = right;
    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap)
{

    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]= minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap* minHeap,struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->siklikdizi< minHeap->array[(i - 1) / 2]->siklikdizi)
    {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap)
{

    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(int askidizi[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
    printf("%d", askidizi[i]);
    printf("\n");

    for (i = 0; i < n; ++i){
        bindizi[sayacbin][i]=askidizi[i]+48;
    }
    bindizi[sayacbin][n]='\0';
    sayacbin++;
}

int isLeaf(struct MinHeapNode* root)

{
    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int siklikdizi[], int size)
{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], siklikdizi[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int siklikdizi[], int size)

{
    struct MinHeapNode *left, *right, *top;


    struct MinHeap* minHeap = createAndBuildMinHeap(data, siklikdizi, size);


    while (!isSizeOne(minHeap))
    {

        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->siklikdizi + right->siklikdizi);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}
void printCodes(struct MinHeapNode* root, int askidizi[], int top)
{

    if (root->left)
    {

        askidizi[top] = 0;
        printCodes(root->left, askidizi, top + 1);
    }

    if (root->right)
    {

        askidizi[top] = 1;
        printCodes(root->right, askidizi, top + 1);
    }

    if (isLeaf(root))
    {

        printf("%c: ", root->data);
        karakterdizi[index]=root->data;
        index++;
        printArr(askidizi, top);
    }
}

void HuffmanCodes(char data[], int siklikdizi[], int size)
{

    struct MinHeapNode* root= buildHuffmanTree(data, siklikdizi, size);


    int askidizi[MAX_TREE_HT], top = 0;

    printCodes(root, askidizi, top);
}
int prefix_match_length(char *s1, char *s2, int limit)
{
    int len = 0;

    while (*s1++ == *s2++ && len < limit)
        len++;

    return len;
}

void lz77memcpy(char *s1, char *s2, int size)
{
    while (size--)
        *s1++ = *s2++;
}


struct token *encode(char *text, int limit, int *numTokens)
{
    int cap = 1 << 5;
    int _numTokens = 0;
    struct token t;
    char *lookahead, *search;
    struct token *encoded = malloc(cap * sizeof(struct token));
    for (lookahead = text; lookahead < text + limit; lookahead++)
    {
        search = lookahead - OFFSETMASK;
        if (search < text)
            search = text;
        int max_len = 0;
        char *max_match = lookahead;
        for (; search < lookahead; search++)
        {
            int len = prefix_match_length(search, lookahead, LENGTHMASK);

            if (len > max_len)
            {
                max_len = len;
                max_match = search;
            }
        }
        if (lookahead + max_len >= text + limit)
        {
            max_len = text + limit - lookahead - 1;
        }
        t.offset_len = OFFSETLENGTH(lookahead - max_match, max_len);
        lookahead += max_len;
        t.c = *lookahead;
        if (_numTokens + 1 > cap)
        {
            cap = cap << 1;
            encoded = realloc(encoded, cap * sizeof(struct token));
        }
        encoded[_numTokens++] = t;
    }
    if (numTokens)
        *numTokens = _numTokens;

    return encoded;
}
char *file_read(FILE *f, int *size)
{
    char *content;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    content = malloc(*size);
    fseek(f, 0, SEEK_SET);
    fread(content, 1, *size, f);
    return content;
}

int main(void)
{
    FILE *f;
    int metin_boyutu = 57, token_sayisi;
    char *kaynak_metin = "";
    struct token *encoded_metin;

    if (f = fopen("metin.txt", "rb"))
    {
        kaynak_metin = file_read(f, &metin_boyutu);
        fclose(f);
    }
    encoded_metin = encode(kaynak_metin, metin_boyutu, &token_sayisi);

    if (f = fopen("encoded.txt", "wb"))
    {
        fwrite(encoded_metin, sizeof(struct token), token_sayisi, f);
        fclose(f);
    }


    FILE *dosya;

    int sayac=0;
    int id;
    int i,j=0,l,k,m;
    char c,z;
    char askidizi[225];
    int siklikdizi[224];
    for(int i=0; i<224; i++)
    {
        siklikdizi[i]=0;
    }
    if ((dosya = fopen ("encoded.txt", "r")) == NULL)
    {
        printf("Dosya açma hatasi!");
        exit(1);
    }

    fseek(dosya, 0, SEEK_END);
    int boyut = ftell(dosya);
    fseek(dosya, 0, SEEK_SET);
   // printf("%d",boyut);
    char okumadizi[boyut+1];
    for(i=32; i<257; i++)
    {
        char c=i;
        askidizi[j]=c;
        j++;
    }
    for(k=0; k<boyut; k++)
    {
        char z=fgetc(dosya);
        okumadizi[k]=z;
    }
    printf("\n");
    for(l=0; l<boyut; l++)
    {
        for(m=0; m<224; m++)
        {
            if(okumadizi[l]==askidizi[m])
            {

                siklikdizi[m]++;
                break;
            }
        }
    }

    int o=0;
    for(int i=0; i<225; i++)
    {
        if(siklikdizi[i]!=0)
        {
            sayac++;
        }
    }
    kackarakter=sayac;
    int yenisiklik[sayac];
    char yeniaski[sayac];
    for(int i=0; i<225; i++)
    {
        if(siklikdizi[i]!=0)
        {
            yenisiklik[o]=siklikdizi[i];
            yeniaski[o]=askidizi[i];
            o++;
        }
    }
    fclose(dosya);
    HuffmanCodes(yeniaski, yenisiklik, sayac);
FILE *encoded2;



    if ((encoded2 = fopen ("encoded2.txt", "w+")) == NULL) {
         printf("Dosya açma hatası!");
         exit(1);
    }

    for(int i=0;i<boyut;i++){
        for(int j=0;j<sayac;j++){
            if(okumadizi[i]==karakterdizi[j]){

//printf("%s",bindizi[j]);
fputs(bindizi[j],encoded2);

                break;
            }
        }
    }fseek(encoded2, 0, SEEK_END);
    int boyut2 = ftell(encoded2);
    fseek(encoded2, 0, SEEK_SET);
     printf("Orjinal Boyut: %d,\n \nEncode Edilmis Boyutu: %d\n", metin_boyutu, token_sayisi * sizeof(struct token));
    printf("\nDeflate Sikistirilmis Metin Boyutu : %d\n",boyut2);
    return 0;

}

