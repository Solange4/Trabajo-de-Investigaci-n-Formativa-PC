#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
 
const int MAX = 3e5; // Número máximo de estados
int cnt[505];
 
struct AhoCorasick{
    int node;    
    int trie[MAX][26] ; // trie, matriz que representa el arbol de patrones
    int failure[MAX]; // Esta función almacena todas las aristas que se siguen 
                        // cuando el carácter actual no tiene arista en Trie. 
                        // Se representa como una matriz 1D f[] donde almacenamos el 
                        // siguiente estado para el estado actual.
    vector<int>output[MAX]; // identificar los patrones 

    // Inicializamos todos los datos de la estructura
    void init(){
        fill(trie[0],trie[0]+26,-1);  // Llena trie con -1s
        output[0].clear(); // Elimina todos los elementos de output
        node = 0;
    }

    // Convertimos el char en entero
    int getID(char ch){
        int id = ch-'a';
        return id;
    }

    void Insert(string s,int idx){  // patron, i
        int state = 0; // va creciendo a medida que recorremos el patron
        int n = s.size(); // tamaño del patron
        for(int i=0;i<n;i++){ 
            int id = getID(s[i]); // transformamos el char a int (alfabeto ingles 26 letras)
            if(trie[state][id] == -1){  // recorremos el trie de acuerdo a los nodos
                trie[state][id] = ++node;
                output[node].clear();
                fill(trie[node],trie[node]+26,-1); // llenamos desde trie[node] 
                                                    // hasta el final con -1s
            }
            state = trie[state][id];
        }
        output[state].push_back(idx); //
    }

    void computeFailure(){
        // Hacemos uno de una la cola q
        queue<int>q;
        // 0 no esta definido
        failure[0] = 0; // arreglo Fallo empieza con 0
        for(int i=0;i<26;i++){  // recorremos la primera fila de Trie
            if(trie[0][i] != -1){
                // si algun dato es diferente da Fallo (-1)
                failure[trie[0][i]] = 0; // posicion del dato en Fallo=0
                q.push(trie[0][i]); // añadimos el dato a q
            }
            else // si es que es fallo cambiamos -1 por 0 en Trie
                trie[0][i] = 0; 
        }

        while(!q.empty()){ // mientras q no este vacia
            int u = q.front(); // sacar cada dato de q
            q.pop();
            for(int id : output[failure[u]]) // desde del inicio de output hasya el fin
                output[u].push_back(id);
            for(int i=0;i<26;i++){
                if(trie[u][i] != -1){
                    failure[trie[u][i]] = trie[failure[u]][i];
                    q.push(trie[u][i]);
                }
                else
                    trie[u][i] = trie[failure[u]][i];
            }
        }
    }
    void imprimirT(){
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                cout <<trie[i][j]<<",";
            }
            cout << endl;
        }
    }
    void imprimir(){
        for (int i = 0; i < 15; i++)
        {
            cout<<"fila "<<++i<<endl;
            for (int j = 0; j < output[i].size(); j++)
            {
                cout <<j<<' '<<output[i][j]<<",";
            }
        }
        cout << endl;
    }
    void imprimirF(){
        for (int j = 0; j < 10; j++)
        {
                cout <<failure[j]<<",";
        }
        cout << endl;
    }
} automata;
 
void countFreq(string s){
    int state = 0 , n = s.size();
    for(int i=0;i<n;i++){
        int id = automata.getID(s[i]);
        state = automata.trie[state][id];
        for(int idx : automata.output[state])
            cnt[idx]++;
    }
}
 
int main(){
    int casos,cas=1;
    cin >> casos;
    while(casos--){
        int n;
        string s,p;
        cin >> n >> s;
        automata.init();
        for(int i=0;i<n;i++){
            cin >> p;
            automata.Insert(p,i); // Insertamos cada patron al automata
        }
        // automata.imprimir();
       // automata.imprimirT();
        automata.computeFailure(); // 
        // automata.imprimirF();
        // cout<<"Trie"<<endl;
        //automata.imprimirT();
        // cout<<"output"<<endl;
        // automata.imprimir();
        memset(cnt,0,sizeof cnt); // Llena cnt de ceros 
        countFreq(s);
        cout << "Case " << cas++ << ":\n";
        for(int i=0;i<n;i++)
            cout << cnt[i] << endl;
    }
}

// 2
// 5
// ababacbabc
// aba
// ba
// ac
// a
// abc
// 3
// lightoj
// oj
// light
// lit