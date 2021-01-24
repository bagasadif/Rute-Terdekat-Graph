/*
Nama Program    : Mencari Rute Terdekat Graph
Author          : Bagas Adi Firdaus
NPM             : 140810190037
Tanggal Buat    : 02 Juni 2020
Deskripsi       : - Program ini akan mencari rute terdekat menggunakan algoritma djikstra dari 
                    graph yang memiliki 2 bobot pada edgenya
                  - Program ini menggunakan struktur data linked list
                  - Graph dalam program ini mempunyai 12 node dan 21 edge
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
using namespace std;

// Struct node adjacency list
struct AdjListNode
{
    int nama;                 //nama = node asal
    int tujuan;               //node yang dituju
    int jarak;                //bobot jaraknya
    int kemacetan;            //bobot kemacetannya
    int hasil_kali;           //hasil perkalian bobot jarak dan kemacetan
    struct AdjListNode *next; //pointer untuk array
};

// Struct adjacency listnya
struct AdjList
{
    struct AdjListNode *head; // pointer ke node head dari list
};

// Struct graph. Graph adalah sebuah array dari adjacency list
// Besar arraynya = V (jumlah node atau vertex di graph)
struct Graph
{
    int V;
    struct AdjList *array;  // Pointer ke struct AdjList yang akan berbentuk array
};

// Untuk membuat adjacency list baru
struct AdjListNode *newAdjListNode(int tujuan, int jarak, int kemacetan)
{
    // Buat AdjListNode baru
    struct AdjListNode *newNode =
        (struct AdjListNode *)malloc(sizeof(struct AdjListNode));   
    
    // Isi data-datanya
    newNode->tujuan = tujuan;
    newNode->jarak = jarak;
    newNode->kemacetan = kemacetan;
    newNode->hasil_kali = jarak * kemacetan;

    return newNode;
}

// Untuk membuat graph dari V node
struct Graph *createGraph(int V)
{
    // Buat Graph baru
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    // Membuat array dari ajacency list. Jumlah array = V
    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));

    // Inisialisasi setiap adjacency list kosong dengan membuat head = NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Untuk menambahkan sebuah edge ke dalam graph
void addEdge(struct Graph *graph, int asal, int tujuan, int jarak, int kemacetan)
{
    // Menambahkan sebuah edge dari asal ke tujuan.  Node baru ditambahkan ke adjacency
    // Node yang barunya akan ditambahkan ke awal array (insert first)
    struct AdjListNode *newNode = newAdjListNode(tujuan, jarak, kemacetan);
    newNode->nama = asal;
    newNode->next = graph->array[asal].head;
    graph->array[asal].head = newNode;
}

// Struct untuk node min heap
struct MinHeapNode
{
    int v;          // Jumlah node di graph
    int hasil_kali; // Hasil kali bobot kemacetan dan jarak di node ajacency list
};

// Struct min heap
struct MinHeap
{
    int size;     // Jumlah dari node min heap yang baru ditampilkan
    int capacity; // Kapasitas dari min heap
    int *pos;     // Digunakan untuk fungsi decreaseKey()
    struct MinHeapNode **array;
};

// Untuk membuat node min heap yang baru
struct MinHeapNode *newMinHeapNode(int v, int hasil_kali)
{
    // Buat Node Min Heap baru
    struct MinHeapNode *minHeapNode =
        (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));

    // Isi data-datanya
    minHeapNode->v = v;
    minHeapNode->hasil_kali = hasil_kali;

    return minHeapNode;
}

// Untuk membuat min heap baru
struct MinHeap *createMinHeap(int capacity)
{
    // Buat Min Heap baru
    struct MinHeap *minHeap =
        (struct MinHeap *)malloc(sizeof(struct MinHeap));
    
    // Isi data-datanya
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
        (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));

    return minHeap;
}

// Digunakan untuk menukar 2 node di min heap. Digunakan di minHeapify()
void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b)
{
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

// Fungsi Heapify (sort sebuah heap) di idx
// Fungsi ini juga mengupdate posisi node saat mereka ditukan
// Posisi diperlukan untuk decreaseKey()
void minHeapify(struct MinHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    // Jika sebelah kiri lebih kecil maka smallest = left
    if (left < minHeap->size &&
        minHeap->array[left]->hasil_kali < minHeap->array[smallest]->hasil_kali)
        smallest = left;

    // Jika sebelah kanan lebih kecil maka samllest = right
    if (right < minHeap->size &&
        minHeap->array[right]->hasil_kali < minHeap->array[smallest]->hasil_kali)
        smallest = right;

    if (smallest != idx)
    {
        // Node yang akan ditukar di min heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        // Tukar posisinya
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Tukar nodenya
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// Untuk mengecek apakah sebuah min heap kosong atau tidak
int isEmpty(struct MinHeap *minHeap)
{
    return minHeap->size == 0;
}

// Untuk mengextract node minimun dari heap
struct MinHeapNode *extractMin(struct MinHeap *minHeap)
{
    // Jika Min Heap kosong
    if (isEmpty(minHeap))
        return NULL;

    // Menyimpan node root
    struct MinHeapNode *root = minHeap->array[0];

    // Ganti node root dengan node terakhir
    struct MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update posisi dari node terakhir
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Mengurangi size heap dan mengheapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Void untuk deacreasy nilai hasil_kali dari vertex v yang diberikan
// Void ini menggunakan pos[] dari min heap untuk mendapaktan index node di min heap
void decreaseKey(struct MinHeap *minHeap, int v, int hasil_kali)
{
    // Ambil index v di array heap
    int i = minHeap->pos[v];

    // Ambil nodenya dan update nilai hasil_kalinya
    minHeap->array[i]->hasil_kali = hasil_kali;

    // Pergi ke atas selama keseluruhan pohon belum hepified
    while (i && minHeap->array[i]->hasil_kali < minHeap->array[(i - 1) / 2]->hasil_kali)
    {
        // Tukar node dengan parentnya
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // Pindahkan index parent
        i = (i - 1) / 2;
    }
}

// Fungsi untuk mengecek apakah vertex atau node "v" adalah min heap atau tidak
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    // Jika node "v" adalah min heap
    if (minHeap->pos[v] < minHeap->size)
        return true;

    // Jika node "v" bukan min heap    
    return false;
}

// Untuk mengeprint solusi (hasil algoritma djikstranya)
void printSolution(int v, int pred[], struct Graph *graph, int nodeAsal, int nodeTujuan)
{
    int jarakNode = 0;     // Untuk menghitung jarak dari Node Asal ke Node Tujuan berdasarkan rute terdekat
    int kemacetanNode = 0; // Untuk menghitung tingkat kemacetan dari Node Asal ke Node Tujuan berdasarkan rute terdekat
    int j = nodeTujuan;    // Integer pembantu

    // Hitung jarak dan tingkat kemacetan dari Node Asal ke Node Tujuan
    while (j != nodeAsal)
    {
        struct AdjListNode *temp = graph->array[pred[j]].head;
        while (temp->tujuan != j)
        {
            temp = temp->next;
        }
        jarakNode = jarakNode + temp->jarak;
        kemacetanNode = kemacetanNode + temp->kemacetan;
        j = pred[j];
    }

    // Cetak hasilnya ke layar
    cout << "RUTE TERDEKAT DARI NODE " << char(nodeAsal + 65) << " KE NODE " << char(nodeTujuan + 65) << endl
         << "Jarak = " << jarakNode << endl
         << "Tingkat Kemacetan = " << kemacetanNode << endl
         << "Hasil kali jarak dengan tingkat kemacetannya = " << jarakNode * kemacetanNode << endl
         << "Rute = " << char(nodeTujuan + 65);
    j = nodeTujuan;
    do
    {
        j = pred[j];
        cout << " <- " << char(65 + j);
    } while (j != nodeAsal);
}

// Fungsi algoritma djikstranya
void dijkstra(struct Graph *graph, int nodeAsal, int nodeTujuan)
{
    int V = graph->V;  // Jumlah node atau vertex di graph
    int hasil_kali[V]; // Untuk menentukan hasil_kali yang paling minimum
    int pred[V];       // Untuk rute dari node Asal ke node Tujuan

    // buat min heap baru
    struct MinHeap *minHeap = createMinHeap(V);

    // Inisialisasi min heap dengan semua node/vertex dan nilai hasil_kali semua node/vertex
    for (int v = 0; v < V; ++v)
    {
        hasil_kali[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, hasil_kali[v]);
        minHeap->pos[v] = v;
    }

    // Nilai hasil_kali node asal akan selalu = 0 lalu kita extract
    minHeap->array[nodeAsal] = newMinHeapNode(nodeAsal, hasil_kali[nodeAsal]);
    minHeap->pos[nodeAsal] = nodeAsal;
    hasil_kali[nodeAsal] = 0;
    decreaseKey(minHeap, nodeAsal, hasil_kali[nodeAsal]);

    // Size min heap akan sama dengan V
    minHeap->size = V;

    // Pada loop ini, min heap akan berisi semua node yang hasil_kalinya masih berzona abu-abu
    // (belum hasil_kali terkecil)
    while (!isEmpty(minHeap))
    {
        // Extract vertexnya dengan nilai minimum hasil_kali
        struct MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Menyimpan nomor vertex yang diextract

        // Traversal seluruh adjacency verex u (vertex yang diextract) dan update hasil_kalinya
        struct AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->tujuan;

            // Jika hasil_kali minimum ke V masih zona abu-abu dan hasil_kali ke v dari u
            // leboh kecil dari yang dihitung sebelumnya maka:
            if (isInMinHeap(minHeap, v) && hasil_kali[u] != INT_MAX &&
                pCrawl->hasil_kali + hasil_kali[u] < hasil_kali[v])
            {
                hasil_kali[v] = hasil_kali[u] + pCrawl->hasil_kali;

                // Update hasil_kali di min heapnya juga
                decreaseKey(minHeap, v, hasil_kali[v]);

                // Catat rutenya
                pred[v] = pCrawl->nama;
            }
            pCrawl = pCrawl->next;
        }
    }

    // Print solusinya ke tampilan layar
    printSolution(V, pred, graph, nodeAsal, nodeTujuan);
}

//void check untuk mengecheck inputan user (error handling)
void check(char &c, int V)
{
     //jika user menginput huruf bukan A s.d L maka looping akan berjalan yaitu user menginput charnya lagi
    while (c < 65 || (c > 64 + V && c < 97) || c > 96 + V)
    {
        cout << "Masukkan Huruf A s.d L : ";
        cin >> c;
    }
}

// Main program
int main()
{
    int V = 12;                           // Jumlah node atau vertex di graph
    struct Graph *graph = createGraph(V); // Buat graph kosong

    // Tambahkan edge-edge
    addEdge(graph, 0, 1, 1, 6);
    addEdge(graph, 1, 2, 4, 4);
    addEdge(graph, 1, 3, 3, 6);
    addEdge(graph, 2, 0, 5, 7);
    addEdge(graph, 2, 3, 8, 3);
    addEdge(graph, 2, 6, 5, 9);
    addEdge(graph, 2, 7, 9, 3);
    addEdge(graph, 3, 5, 7, 2);
    addEdge(graph, 3, 7, 6, 4);
    addEdge(graph, 4, 1, 4, 9);
    addEdge(graph, 5, 4, 2, 3);
    addEdge(graph, 5, 8, 6, 4);
    addEdge(graph, 6, 9, 10, 4);
    addEdge(graph, 7, 5, 7, 2);
    addEdge(graph, 7, 9, 3, 7);
    addEdge(graph, 8, 9, 10, 2);
    addEdge(graph, 8, 11, 4, 10);
    addEdge(graph, 9, 10, 3, 3);
    addEdge(graph, 10, 8, 6, 2);
    addEdge(graph, 11, 4, 5, 8);
    addEdge(graph, 11, 10, 9, 6);
    // Disini graphnya sudah selesai dibuat

    char cAsal, cTujuan;      // Char untuk inputan user untuk menentukan node Asal dan node Tujuan yang akan dicari rute terdekat
    int nodeAsal, nodeTujuan; // Integer dari char yang diinput user
    int pilih = 0;            // Integer untuk program switch di akhir

    while (1) // Untuk infinity looping selama user tidak ingin keluar dari program
    {
        cout << endl // Tampilan awal program
             << "=====================================================================================================================================" << endl
             << "PROGRAM MENCARI RUTE YANG MEMPUNYAI HASIL PERKALIAN JARAK DENGAN TINGKAT KEMACETAN PALING KECIL DENGAN MENGGUNAKAN ALGORITMA DJIKSTRA" << endl
             << "=====================================================================================================================================" << endl
             << endl // Kita tampilkan representasi adjacency matrix dari edge yang telah kita buat
             << "Berikut adalah representasi adjacency matrix dari graphnya" << endl
             << "keterangan : "<< endl
             << "A s.d L = nama node" << endl
             << "(jarak,kemacetan)" << endl
             << endl
             << "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\tK\tL" << endl;

        for (int i = 0; i < V; i++)
        {
            cout << char(i + 65);
            for (int j = 0; j < V; j++)
            {
                AdjListNode *temp = graph->array[i].head;
                while (temp->tujuan != j && temp->next != NULL)
                {
                    temp = temp->next;
                }
                if (temp->tujuan == j)
                {
                    cout << "\t(" << temp->jarak << "," << temp->kemacetan << ")";
                }
                else
                {
                    cout << "\t(0,0)";
                }
            }
            cout << endl;
        }
        // Disini adjacency matrixnya telah selesai ditampilkan

        cout << endl
             << "Masukkan Node Asal (A s.d L) : ";
        cin >> cAsal;    // User input node asalnya
        check(cAsal, V); // Kita mengecek apakah yg diinput user huruf A sd. L atau tidak
        cout << "Masukkan Node Tujuan (A s.d L) : ";
        cin >> cTujuan;    // User input node tujuannya
        check(cTujuan, V); // Kita mengecek apakah yg diinput user huruf A sd. L atau tidak
        cout << endl;

        // Kita buat integer dari 0-11 sesuai dengan char yang diinput user, A = 0, B = 1 dsb
        if (cTujuan <= 65 + V) // Jika user menginputnya dengan huruf kapital
        {
            nodeAsal = cAsal - 65;
            nodeTujuan = cTujuan - 65;
        }

        else if (cTujuan >= 97) // Jika user menginputnya tidak dengan huruf kapital
        {
            nodeAsal = cAsal - 97;
            nodeTujuan = cTujuan - 97;
        }

        dijkstra(graph, nodeAsal, nodeTujuan); // Kita cari rutenya dengan void dijkstra

        cout << endl
             << endl

             // Di akhir program, user dapat memilih untuk keluar program atau tidak
             << "Apakah anda ingin menjalankan program lagi?" << endl
             << "1. Ya" << endl
             << "2. Tidak" << endl
             << "Pilih : ";

        // Looping akan berjalan jikalau user memasukkan angka selain 1 atau 2
        do
        {
            cin >> pilih; // User memasukkan pilihannya
            switch (pilih)
            {
            case 1:            // Case jika user ingin menjalankan program lagi
                system("cls"); // Clear tampilan di cmd
                break;

            case 2: // Case jika user ingin keluar dari program
                cout << endl

                     //tampilan akhir program
                     << "================" << endl
                     << "PROGRAM BERAKHIR" << endl
                     << "================" << endl;
                return 0;
                break;

            default: // Case jika user memasukkan selain angka 1 atau 2
                cout << "MASUKKAN ANGKA 1 ATAU 2 : ";
                break;
            }
        } while (pilih != 1); // Kalau user memasukkan angka selain 1 atau 2 maka looping berjalan
    }
}
