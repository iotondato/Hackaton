//
// curltest.cpp 
// 

#include <string> 
#include <curl/curl.h> 
#include "jsonxx.h"
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <curses.h>
#define Nmesas 16


using namespace std;
using namespace jsonxx;

typedef struct{
    char cadeiras[4];
}mesas;
//
// For this demonstration, the contents of the web page are put 
// into this global variable. 
// 
string contents; 

// 
// This is the callback function that is called by 
// curl_easy_perform(curl) 
//
void imprimeMapa(mesas* M, char placa);
void opcao(mesas* M);
size_t handle_data(void *ptr, size_t size, size_t nmemb, void *stream) 
    {
        contents.clear();
    int numbytes = size*nmemb; 
    // The data is not null-terminated, so get the last character, and replace 
    // it with '\0'. 
    char lastchar = *((char *) ptr + numbytes - 1); 
    *((char *) ptr + numbytes - 1) = '\0'; 
    contents.append((char *)ptr); 
    contents.append(1,lastchar); 
    *((char *) ptr + numbytes - 1) = lastchar;  // Might not be necessary. 
    return size*nmemb; 
    }
string atualizar(mesas* M){
       CURL* curl = curl_easy_init();
    
    if(curl)
    {
        curl_easy_setopt(curl,CURLOPT_FRESH_CONNECT, 1);
        curl_easy_setopt(curl,CURLOPT_URL, "http://dca.telefonicabeta.com/m2m/v2/services/kitkitiot4G_9f05fa41/assets/kit-iot-4g/");
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, handle_data);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    istringstream input(contents);
    Object o;
    o.parse(input);
    string n =o.get<Object>("data").get<Array>("sensorData").get<Object>(3).get<Object>("ms").get<String>("v");
    return n;
}
void verDisp(mesas* M){
    system("clear");
    int um=0, dois=0, tres=0, quatro=0,disp=0;

        for(int i=0; i<Nmesas; i++){
            for(int j=0; j<4;j++){
                if(M[i].cadeiras[j]=='O') disp++;
                    
            }
            if(disp==1) um++;
            else if(disp==2) dois++;
            else if(disp==3) tres++;
            else if(disp==4) quatro++;
            disp=0;
        }
    printf("Mesas com uma cadeira disponivel: %d\nMesas com duas cadeiras disponiveis: %d\nMesas com tres cadeiras disponiveis: %d\nMesas com quatro cadeiras disponiveis: %d\n", um,dois,tres,quatro);
    opcao(M);
}

void opcao(mesas* M){
    int escolha;
    printf("1 - Verificar disponibilidade.\n2 - Atualizar mapa.\n0 - Sair.\n");
    cin>>escolha;
    if(escolha==2){
             string resp = atualizar(M).c_str();
        system("clear");
        if(resp=="true") imprimeMapa(M,'O');
        else imprimeMapa(M, 'X');
    }
    if(escolha==1){
        verDisp(M);
    }
    if(escolha==0){
        free(M);
        exit(0);
    }
}

void imprimeMapa(mesas* M, char placa)
{
    for(int i=0; i<Nmesas; i++){
        for(int j=0; j<4;j++){
             M[i].cadeiras[j]='O';
            
        }
    }
    M[0].cadeiras[0]=placa;
    int contCad = 64, verM = 0, contMesa = 0;
    for(int i=0; i<Nmesas; i++){
        for(int j=0; j<4; j++){
            if(M[i].cadeiras[j]=='X') {
                contCad--;
                verM++;
            }
        }
        if(verM==0)contMesa++;
        verM=0;
    }
    
    
    printf("Mesas vazias: %d\n", contMesa);
    printf("Cadeiras vazias: %d\n", contCad);
    printf("  __      __      __      __\n");
    printf("%c|1 |%c  %c|2 |%c  %c|3 |%c  %c|4 |%c\n", M[0].cadeiras[0], M[0].cadeiras[1], M[1].cadeiras[0], M[1].cadeiras[1], M[2].cadeiras[0], M[2].cadeiras[1], M[3].cadeiras[0],M[3].cadeiras[1]);
    printf("%c|__|%c  %c|__|%c  %c|__|%c  %c|__|%c", M[0].cadeiras[2], M[0].cadeiras[3], M[1].cadeiras[2], M[1].cadeiras[3], M[2].cadeiras[2], M[2].cadeiras[3], M[3].cadeiras[2],M[3].cadeiras[3]);
    
    printf("\n  __      __      __      __\n");
    printf("%c|5 |%c  %c|6 |%c  %c|7 |%c  %c|8 |%c\n", M[4].cadeiras[0], M[4].cadeiras[1], M[5].cadeiras[0], M[5].cadeiras[1], M[6].cadeiras[0], M[6].cadeiras[1], M[7].cadeiras[0],M[7].cadeiras[1]);
    printf("%c|__|%c  %c|__|%c  %c|__|%c  %c|__|%c", M[4].cadeiras[2], M[4].cadeiras[3], M[5].cadeiras[2], M[5].cadeiras[3], M[6].cadeiras[2], M[6].cadeiras[3], M[7].cadeiras[2],M[7].cadeiras[3]);
    
    printf("\n  __      __      __      __\n");
    printf("%c|9 |%c  %c|10|%c  %c|11|%c  %c|12|%c\n", M[8].cadeiras[0], M[8].cadeiras[1], M[9].cadeiras[0], M[9].cadeiras[1], M[10].cadeiras[0], M[10].cadeiras[1], M[11].cadeiras[0],M[11].cadeiras[1]);
    printf("%c|__|%c  %c|__|%c  %c|__|%c  %c|__|%c", M[8].cadeiras[2], M[8].cadeiras[3], M[9].cadeiras[2], M[9].cadeiras[3], M[10].cadeiras[2], M[10].cadeiras[3], M[11].cadeiras[2],M[11].cadeiras[3]);
    
    printf("\n  __      __      __      __\n");
    printf("%c|13|%c  %c|14|%c  %c|15|%c  %c|16|%c\n", M[12].cadeiras[0], M[12].cadeiras[1], M[13].cadeiras[0], M[13].cadeiras[1], M[14].cadeiras[0], M[14].cadeiras[1], M[15].cadeiras[0],M[15].cadeiras[1]);
    printf("%c|__|%c  %c|__|%c  %c|__|%c  %c|__|%c\n\n", M[12].cadeiras[2], M[12].cadeiras[3], M[13].cadeiras[2], M[13].cadeiras[3], M[14].cadeiras[2], M[14].cadeiras[3], M[15].cadeiras[2],M[15].cadeiras[3]);
    
    printf("'O' = Cadeira vazia.\n'X' = Cadeira ocupada.\n\n");
    opcao(M);
}


int main(int argc, char *argv[])
    { 
        srand (time(NULL));
        mesas *M = (mesas*)malloc(sizeof(mesas)*Nmesas);
        
        
    CURL* curl = curl_easy_init();
    if(curl) 
        {
        curl_easy_setopt(curl,CURLOPT_URL, "http://dca.telefonicabeta.com/m2m/v2/services/kitkitiot4G_9f05fa41/assets/kit-iot-4g/");
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,handle_data);
        CURLcode res = curl_easy_perform(curl); 
        curl_easy_cleanup(curl); 
        if (res == 0) 
        	1;
        else 
            cerr << "Error: " << res << endl; 
        }
        
    istringstream input(contents);
    Object o;
    o.parse(input);
    string n =o.get<Object>("data").get<Array>("sensorData").get<Object>(3).get<Object>("ms").get<String>("v");

        char placa;
        if(n=="false") placa = 'X';
        else placa = 'O';
        imprimeMapa(M, placa);
    free(M);
    return 0; 
    }  
