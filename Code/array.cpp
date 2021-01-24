/*
Nama Program    : Mencari Rute Terdekat Graph Berbobot
Author          : Bagas Adi Firdaus
NPM             : 140810190037
Tanggal Buat    : 02 Juni 2020
Deskripsi       : Program ini akan mencari rute terdekat menggunakan algoritma djikstra dari 
                    graph yang memiliki 2 bobot pada edgenya
*/

#include <iostream>
#include <stdio.h>
#include <windows.h>
using namespace std;

#define INFINITY 9999 //define INFINITY = 9999
#define jumlahNode 12 //jumlah node (jumlah vertex) yang ada di program ini

//deklarasi void-void yang akan digunakan
void dijkstra(int jarak[jumlahNode][jumlahNode], int kemacetan[jumlahNode][jumlahNode], int nodeAwal, int nodeAkhir);
//void dijkstra untuk mencari rute terdekat dari node awal dan node tujuan (node akhir)
void check(char &c);
//void check untuk mengecheck inputan user (error handling)

int main() //main program
{
    char cAwal, cAkhir;      //char untuk inputan user untuk menentukan node Awal dan node Tujuan yang akan dicari rute terdekat
    int nodeAwal, nodeAkhir; //integer dari char yang diinput user
    int pilih = 0;           //integer untuk program switch di akhir

    int jarak[jumlahNode][jumlahNode] = {//edge yang berisikan jarak dari 1 node ke node lainnya [disini terdapat 12 node]
                                         {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                         {0, 0, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0},
                                         {5, 0, 0, 8, 0, 0, 5, 9, 0, 0, 0, 0},
                                         {0, 0, 0, 0, 0, 7, 0, 6, 0, 0, 0, 0},
                                         {0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                         {0, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0},
                                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0},
                                         {0, 0, 0, 0, 0, 7, 0, 0, 0, 3, 0, 0},
                                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 4},
                                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0},
                                         {0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0},
                                         {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 9, 0}};

    int kemacetan[jumlahNode][jumlahNode] = {//edge yang berisikan tingkat kemacetan dari 1 node ke node lainnya [disini terdapat 12 node]
                                             {0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 0, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {7, 0, 0, 3, 0, 0, 9, 3, 0, 0, 0, 0},
                                             {0, 0, 0, 0, 0, 2, 0, 4, 0, 0, 0, 0},
                                             {0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0},
                                             {0, 0, 0, 0, 0, 2, 0, 0, 0, 7, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 10},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
                                             {0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 9, 0}};

    while (1) //untuk infinity looping selama user tidak ingin keluar dari program
    {
        cout << endl //tampilan awal program
             << "=====================================================================================================================================" << endl
             << "PROGRAM MENCARI RUTE YANG MEMPUNYAI HASIL PERKALIAN JARAK DENGAN TINGKAT KEMACETAN PALING KECIL DENGAN MENGGUNAKAN ALGORITMA DJIKSTRA" << endl
             << "=====================================================================================================================================" << endl
             << endl //kita tampilkan representasi adjacency matrix dari edge yang telah kita buat
             << "Berikut adalah representasi adjacency matrix" << endl
             << "A s.d L = nama node" << endl
             << "(jarak,kemacetan)" << endl
             << endl
             << "\tA\tB\tC\tD\tE\tF\tG\tH\tI\tJ\tK\tL" << endl;

        for (int i = 0; i < jumlahNode; i++)
        {
            cout << char(i + 65);
            for (int j = 0; j < jumlahNode; j++)
            {
                cout << "\t(" << jarak[i][j] << "," << kemacetan[i][j] << ")";
            }
            cout << endl;
        }
        //disini adjacency matrixnya telah selesai ditampilkan

        cout << endl
             << "Masukkan node awal : ";
        cin >> cAwal; //user input node awalnya
        check(cAwal); //kita mengecek apakah yg diinput user huruf A sd. L atau tidak
        cout << "Masukkan node tujuan : ";
        cin >> cAkhir; //user input node tujuannya
        check(cAkhir); //kita mengecek apakah yg diinput user huruf A sd. L atau tidak
        cout << endl;

        //kita buat integer dari 0-11 sesuai dengan char yang diinput user, A = 0, B = 1 dsb
        if (cAkhir <= 65 + jumlahNode) //jika user menginputnya dengan huruf kapital
        {
            nodeAwal = cAwal - 65;
            nodeAkhir = cAkhir - 65;
        }

        else if (cAkhir >= 97) //jika user menginputnya tidak dengan huruf kapital
        {
            nodeAwal = cAwal - 97;
            nodeAkhir = cAkhir - 97;
        }

        dijkstra(jarak, kemacetan, nodeAwal, nodeAkhir); //kita cari rutenya dengan void dijkstra

        cout << endl
             << endl
             //di akhir program, user dapat memilih untuk keluar program atau tidak
             << "Apakah anda ingin menjalankan program lagi?" << endl
             << "1. Ya" << endl
             << "2. Tidak" << endl
             << "Pilih : ";
        do
        {                 //looping jikalau user memasukkan angka selain 1 atau 2
            cin >> pilih; //user memasukkan pilihannya
            switch (pilih)
            {
            case 1:            //case jika user ingin menjalankan program lagi
                system("cls"); //clear tampilan di cmd
                break;

            case 2: //case jika user ingin keluar dari program
                cout << endl
                     << "================" << endl
                     << "PROGRAM BERAKHIR" << endl
                     << "================" << endl; //tampilan akhir program
                return 0;
                break;

            default: //case jika user memasukkan selain angka 1 atau 2
                cout << "MASUKKAN ANGKA 1 ATAU 2 : ";
                break;
            }
        } while (pilih != 1); //kalau user memasukkan angka selain 1 atau 2 maka looping berjalan
    }
}

void dijkstra(int jarak[jumlahNode][jumlahNode], int kemacetan[jumlahNode][jumlahNode], int nodeAwal, int nodeAkhir)
{ //void dijkstra untuk mencari rute terdekat dari node awal dan node tujuan (node akhir)
    //kita passing edge yg berisi jarak, edge berisi kemacetan, inputan node awal user dan inputan node akhir user dari main

    int cost[jumlahNode][jumlahNode];       //digunakan untuk perhitungan mencari rute terdekat
    int distance[jumlahNode];               //ini kalau rute terdekat dari node awal ke suatu node sudah diketahui (sudah zona hijau)
    int pred[jumlahNode];                   //ini untuk mencatat node sebelumnya untuk mencapai suatu node dari node awal (untuk rute)
    int visited[jumlahNode];                //untuk mencatat apakah node selain node awal sudah kita hitung jaraknya atau tidak
    int count, mindistance, prevNode, i, j; //integer2 lainnya untuk membantu perhitungan
    int total[jumlahNode][jumlahNode];      //edge yang berisi hasil kali dari jarak dan kemacetan (algoritma dijkstranya digunakan di edge ini)
    int jarakNode;                          //integer untuk mencatat jarak dari node Awal ke node Akhir berdasarkan rute yang terdekat
    int kemacetanNode;                      //integer untuk mencatat tingkat kemacetan dari node Awal ke node Akhir berdasarkan rute yang terdekat

    //kita isi edge totalnya dengan hasil perkalian jarak dengan tingkat kemacetan
    for (int i = 0; i < jumlahNode; i++)
    {
        for (int j = 0; j < jumlahNode; j++)
        {
            total[i][j] = jarak[i][j] * kemacetan[i][j];
        }
    }
    //edge totalnya sudah terisi

    //disini kita mulai algoritma djikstranya dengan edge total
    for (i = 0; i < jumlahNode; i++)
        for (j = 0; j < jumlahNode; j++)
            if (total[i][j] == 0)
                cost[i][j] = INFINITY; //kalau misalnya rutenya tidak ada (di edge = 0) maka costnya diset infinity
            else
                cost[i][j] = total[i][j]; //kalau misalnya rutenya ada maka costnya diisi sesuai dengan nilai awal

    //kita masukin nilai awal distancenya
    for (i = 0; i < jumlahNode; i++)
    {
        distance[i] = cost[nodeAwal][i]; //distancenya masih sama dengan nilai awalnya
        pred[i] = nodeAwal;              //rute sebelumnya masih node awal
        visited[i] = 0;                  //visitednya = 0 berarti belum divisit (dikunjungi)
    }

    distance[nodeAwal] = 0; //jarak dari node awal ke node awal pasti = 0
    visited[nodeAwal] = 1;  //kita buat node awal sudah divisit
    count = 1;              //count disini untuk mencatat berapa node yang sudah diketahui jarak terdekatnya

    //disini kita cari rute terdekat dari node awal ke semua node lainnya
    while (count < jumlahNode - 1) /*jumlahNode-1 karena arraynya hanya sampe jumlahNode - 1 karena array mulai dari 0. 
                                    jika jumlahNode = 12 maka arraynya dari 0-11*/
    {
        mindistance = INFINITY; //set mindistance = infinity
        for (i = 0; i < jumlahNode; i++)
            if (distance[i] < mindistance && !visited[i]) //jika distance ke suatu node < mindisntace && nodenya belum pernah dikunjungi
            {
                mindistance = distance[i]; //mindistance = distance ke suatu node tersebut
                prevNode = i;              //set nextnodenya node tersebut
                //disini kita masih dalam perhitungan zona abu-abu (belum ditentukan pasti rute terdekatnya)
            }
        visited[prevNode] = 1; //node tersebut berarti sudah kita visit

        for (i = 0; i < jumlahNode; i++)
            if (!visited[i]) //jika nodenya belum pernah divisit
                if (mindistance + cost[prevNode][i] < distance[i])
                {                                                  //jika mindisntace + cost dari node sebelumnya ke node tersebut < distance maka distancenya = mindistance + cost dari prevNode ke node tersebut
                    distance[i] = mindistance + cost[prevNode][i]; //disini distancenya sudah menjadi zona hijau
                    pred[i] = prevNode;                            //kita catat di array pred[i] yaitu array untuk rutenya = prevNode
                }
        count++; // incerement count
    }
    //disini kita sudah tau rute-rute terdekat ke node lain dari node Awal

    //kita hitung total jarak dari node Awal ke node Akhir sesuai rute yang sudah kita dapatkan
    jarakNode = 0; //set jaraknya = 0 terlebih dahulu
    j = nodeAkhir; //integer pembantu j set ke nodeAkhir (jadi nanti itung jaraknya dari belakang [node Akhir sampe ke node Awal])
    while (j != nodeAwal)
    {
        jarakNode = jarakNode + jarak[pred[j]][j]; //jaraknya = jarak-jarak dari node yang merupakan rute untuk menuju node tersebut
        j = pred[j];                               //j nya ke rute node sebelumnya sampai tiba ke nodeAwal baru looping berhenti
    }
    //disini kita sudah menemukan jarak dari node Awal ke node Akhir

    //kita hitung total jarak dari node Awal ke node Akhir sesuai rute yang sudah kita dapatkan
    kemacetanNode = 0; //set tingkat kemacetan = 0 terlebih dahulu
    j = nodeAkhir;     //integer pembantu j set ke nodeAkhir (jadi nanti itung tingkat kemacetannya dari belakang [node Akhir sampe ke node Awal])
    while (j != nodeAwal)
    {
        kemacetanNode = kemacetanNode + kemacetan[pred[j]][j]; //tingkat kemacetannya = total tingkat kemacetan dari node yang merupakan rute untuk menuju node tersebut
        j = pred[j];                                           //j nya ke rute node sebelumnya sampai tiba ke nodeAwal baru looping berhenti
    }
    //disini kita sudah menemukan tingkat kemacetan dari node Awal ke node Akhir

    //lalu kita print hasilnya di layar
    cout << "RUTE TERDEKAT DARI NODE " << char(65 + nodeAwal) << " KE NODE " << char(65 + nodeAkhir) << endl
         //kita tampilkan huruf nodenya lagi berdasarkan angkanya. 0 = A, 1 = B dst
         << "Jarak = " << jarakNode << endl                                                        //tampilkan jaraknya
         << "Tingkat Kemacetan = " << kemacetanNode << endl                                        //tampilkan tingkat kemacetannya
         << "Hasil kali jarak dengan tingkat kemacetannya = " << jarakNode * kemacetanNode << endl //tampilkan hasil perkalian jarak dengan tingkat kemacetan
                                                                                                   //disini kita tampilkan rutenya menggunakan array pred[j]
         << "Rute = " << char(65 + nodeAkhir);
    j = nodeAkhir; //dari node akhir sampe ke node awal (dari belakang lagi)
    do
    {
        j = pred[j];
        cout << " <- " << char(65 + j);
    } while (j != nodeAwal);
    //hasilnya sudah ditampilkan di layar kembali ke main
}

//void check untuk mengecheck inputan user (error handling)
void check(char &c)
{
    while (c < 65 || (c > 64 + jumlahNode && c < 97) || c > 96 + jumlahNode)
    //jika user menginput huruf bukan A s.d L maka looping akan berjalan yaitu user menginput charnya lagi
    {
        cout << "Masukkan Huruf A s.d L" << endl;
        cin >> c;
    }
}