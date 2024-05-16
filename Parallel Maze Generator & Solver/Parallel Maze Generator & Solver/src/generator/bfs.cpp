#include "bfs.hpp"
#include<bits/stdc++.h>
// #include<mpi.h>
using namespace std;

// mt19937_64 rand_gen((unsigned int)chrono::steac_a2_clock::now().time_since_epoch().count());
mt19937_64 rand_gen((unsigned int) chrono::steady_clock::now().time_since_epoch().count());

const int LN = 31 ;
const int N = 64;

void bfs_generator(int world_rank) {

    // MPI_Init(NULL, NULL);
    // int world_size, world_rank;
    // MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    bool visited_nodes[LN][LN];
    int  queue_pst[LN][LN];
    char ftnr_arr[LN*LN];

    for (int p = 0; p < LN; p++) {
        for (int q = 0; q < LN; q++) {
            visited_nodes[p][q] = false;
            queue_pst[p][q] = -1;
            ftnr_arr[p*LN+q] = '*';
        }
    }

    pair<int,int> start;
    if (world_rank == 0) {
        start = { 0 , LN-1}  ;
        int c13 = rand_gen() % 31 ;
        int c02 = rand_gen() % 31 ;
        int r10 = rand_gen() % 31 ;
        int r32 = rand_gen() % 31 ;

    }
    else if(world_rank==3){
        start = { LN-1 , 0};
    }
    else{
        int cl = rand_gen() % 31 ;
        int r  = rand_gen() % 31 ;
        start = { r , cl };
    }
    pair<int, int> frontier[LN * LN];
    int ind = 0;
   

    if (start.second+1 < LN ) {queue_pst[start.first][start.second+1] = ind; frontier[ind++] = { start.first , start.second+1};}
    if (start.second-1 >= 0 ) {queue_pst[start.first][start.second-1] = ind; frontier[ind++] = { start.first , start.second-1};}
    if (start.first+1 < LN ) {queue_pst[start.first+1][start.second] = ind; frontier[ind++] = { start.first+1 , start.second};}
    if (start.first-1 >= 0 ) {queue_pst[start.first-1][start.second] = ind; frontier[ind++] = { start.first-1 , start.second};}


    ftnr_arr[start.first*LN+start.second] = ' ';
    visited_nodes[start.first][start.second] = true;

    while (ind!=0) {
        int vs_count = 0;
        int rand_ind = rand_gen() % ind;
        auto [a1, a2] = frontier[rand_ind];
        swap(frontier[rand_ind], frontier[ind - 1]);
        ind--;

        if (a2+1 < LN && visited_nodes[a1][a2+1]) {vs_count++;}
        if (a2-1 >= 0 && visited_nodes[a1][a2-1]) {vs_count++;}
        if (a1+1 < LN && visited_nodes[a1+1][a2]) {vs_count++;}
        if (a1-1 >= 0 && visited_nodes[a1-1][a2]) {vs_count++;}


        if (vs_count <= 1) {
            ftnr_arr[a1*LN+a2] = ' ';
            visited_nodes[a1][a2] = true;
            if (a2+1 < LN && queue_pst[a1][a2+1]==-1) {queue_pst[a1][a2+1] = ind; frontier[ind++] = {a1, a2+1};}
            if (a2-1 >= 0 && queue_pst[a1][a2-1]==-1) {queue_pst[a1][a2-1] = ind; frontier[ind++] = {a1, a2-1};}
            if (a1+1 < LN && queue_pst[a1+1][a2]==-1) {queue_pst[a1+1][a2] = ind; frontier[ind++] = {a1+1, a2};}
            if (a1-1 >= 0 && queue_pst[a1-1][a2]==-1) {queue_pst[a1-1][a2] = ind; frontier[ind++] = {a1-1, a2};}
        }
    }


    // Print the final result
    if(world_rank != 0){
        MPI_Send(ftnr_arr,LN*LN,MPI_CHAR,0,0,MPI_COMM_WORLD);
    }
    if (world_rank == 0) {

        char graph[N][N];

        for(int i = 0 ;i<N;i++) for (int j = 0 ; j<N ;j++) graph[i][j] = '*';

        char recv_buf_1[LN*LN];
        MPI_Recv(recv_buf_1,LN*LN,MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        char recv_buf_2[LN*LN];
        MPI_Recv(recv_buf_2,LN*LN,MPI_CHAR,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        char recv_buf_3[LN*LN];
        MPI_Recv(recv_buf_3,LN*LN,MPI_CHAR,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        for(int i=0;i<LN;i++){
            for(int j=0;j<LN;j++){
                graph[i][j] = recv_buf_1[i*LN+j];
                graph[i+33][j] = recv_buf_3[i*LN+j];
                graph[i][j+33] = ftnr_arr[i*LN+j];
                graph[i+33][j+33] = recv_buf_2[i*LN+j];
            }
        }

        int rn1 = 0;
        if(rn1==0){
            // partition 1
            for(int i=33;i<64;i++ ){
                if(graph[30][i]==' '){
                    graph[31][i] = ' ';
                    // graph[32][i] = ' ';
                    int j= i;
                    while(j<64 && graph[33][j] != ' '){
                        graph[32][j] = ' ';
                        j++;
                    }
                    graph[32][j] = ' ';
                    break;
                }
                else{
                    continue;
                }
            }
            // partition 2
            for(int i=33;i<64;i++ ){
                if(graph[i][30]==' '){
                    graph[i][31] = ' ';
                    // graph[32][i] = ' ';
                    int j= i;
                    while(j<64 && graph[j][33] != ' '){
                        graph[j][32] = ' ';
                        j++;
                    }
                    graph[j][32] = ' ';
                    break;
                }
                else{
                    continue;
                }
            }
            // partition 3
            for(int i=0;i<31;i++ ){
                if(graph[30][i]==' '){
                    graph[31][i] = ' ';
                    // graph[32][i] = ' ';
                    int j= i;
                    while(j<31 && graph[33][j] != ' '){
                        graph[32][j] = ' ';
                        j++;
                    }
                    graph[32][j] = ' ';
                    break;
                }
                else{
                    continue;
                }
            }

        }
        // for(int i = 0 ;i<N;i++) {
        //     for (int j = 0 ; j<N ;j++)
        //     if(graph[i][j]=='*')cout<<"*";
        //     else cout<<" ";
        //     cout<<"\n";
        // }

        ofstream outputFile("output.txt");
        if (outputFile.is_open()) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (graph[i][j] == '*') {
                        outputFile << "*";
                    } else {
                        outputFile << " ";
                    }
                }
                outputFile << "\n";
            }
            outputFile.close();
            // cout << "Array written to file successfully.\n";
        } else {
            cout << "Error opening the file.\n";
        }

    }

    // MPI_Finalize();
    return;
}