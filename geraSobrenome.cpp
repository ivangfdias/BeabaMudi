#include <bits/stdc++.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
using namespace std;

#define pb push_back
#define mp make_pair
const int ALFABETO = 'z'-'a'+1;
const int MAXN = 4009;
const int SAMP = 1e4+9;

int cnt=0;
vector<string> V, ultimas,trava_lingua; // V armazena texto dividido em silabas para montagem do grafo
vector<int> Escolha, verso, prov; //Escolha eh vetor usado como espaco amostral para sorteio de silabas
vector<int> comeco_anag[ALFABETO];//guardamos silabas de inicio de palavra para cada letra do alfabeto
map<string,int> M;
set<string> S; //usado para buscar palavras geradas, impedindo que sejam iguais a palavras da Train Data
map<int,string> Mrev;
double prob[MAXN][MAXN], soma[MAXN], prob_rev[MAXN][MAXN], soma_rev[MAXN]; //matriz de adjacencias e soma de linhas para calculo de probabilidades
//O primeiro vetor carrega o codigo ASCII das letras acentuadas e cecedilla; o segundo carrega o codigo ANSI (?) das letras acentuadas e cecedilla
int ascii_special[] = { 192, 193, 194, 195, 199, 201, 202, 205, 211, 212, 213, 218 };
int string_special[] = { 224, 225, 226, 227, 231, 233, 234, 237, 243, 244, 245, 250 };

void nome(int barra, int quantidade) {
	int versos[4] = { 4,4,3,3 };

	int rima[4][4]; set<int>rimas;
	rima[0][0] = M[ultimas[rand() % (int)ultimas.size()]];
	rimas.insert(rima[0][0]);
	rima[0][3] = rima[0][0]; rima[1][0] = rima[0][0]; rima[1][3] = rima[0][0];

	do { rima[0][1] = M[ultimas[rand() % (int)ultimas.size()]]; } while (rimas.find(rima[0][1]) != rimas.end());
	rimas.insert(rima[0][1]);
	rima[0][2] = rima[0][1]; rima[1][1] = rima[0][1]; rima[1][2] = rima[0][1];

	do { rima[2][0] = M[ultimas[rand() % (int)ultimas.size()]]; } while (rimas.find(rima[2][0]) != rimas.end());
	rimas.insert(rima[2][0]);
	rima[2][2] = rima[2][0]; rima[3][1] = rima[2][0];

	do { rima[2][1] = M[ultimas[rand() % (int)ultimas.size()]]; } while (rimas.find(rima[2][1]) != rimas.end());
	rima[3][0] = rima[2][1]; rima[3][2] = rima[2][1];

	// o esquema de rimas do soneto eh abba abba cdc dcd
	vector<string> para_salvar;
	for (int i = 0; i < quantidade; i ++){
		verso.clear();
		bool flag = true;
		int atual;
		int tamanho = rand() % 7 + 1;
		while ((int)verso.size() < tamanho) {
			if (flag) atual = rima[0][0];
			else atual = barra;
			prov.clear();
			do {
				if (atual != barra)prov.pb(atual);
				Escolha.clear();
				for (int l = 0; l < cnt; l++)
					for (int k = 1; k <= prob_rev[atual][l]; k++)
						Escolha.pb(l); //preenchemos nosso espaco amostral com base nas probabilidades
				if (Escolha.size() == 0)atual = barra;
				else atual = Escolha[rand() % (int)Escolha.size()];
			} while (atual != barra);

			if (prov.size() >= 1 && prov.size() <= 5 && (int)prov.size() + (int)verso.size() < 2 + tamanho) {
				for (int k = 0; k < (int)prov.size(); k++) {
					verso.pb(prov[k]);
				}
				verso.pb(barra); flag = false;
			}
		}
		string aux; aux.clear();
		for (int k = (int)verso.size() - 2; k >= 0; k--) {
			if (verso[k] == barra) break;
			aux += Mrev[verso[k]];
		}
		//conversao letra corrompida -> letra acentuada no txt
		for (int k = 0; k < aux.length(); k++) {
			for (int w = 0; w < 12; w++) {
				if ((int)(unsigned char)aux[k] == string_special[w]) {
					aux[k] = (unsigned char)ascii_special[w];
				}
			}
		}
		para_salvar.pb(aux);
	}
	ofstream file_;
	file_.open("SobrenomeGerado.txt");
	
	for (int i = 0; i < (int)para_salvar.size(); i++) {
		if (i == 4 || i == 8 || i == 11)file_ << " \n";
		file_ << (para_salvar[i]) << "\n";
	
	}

	file_.close();
	//system("lp TextoGerado.txt");

}




int main(){
	//wiringPiSetup();
	//PinMode(0, INPUT);
	//PinMode(1, INPUT);
	int quantidade;
    setlocale(LC_ALL, "Portuguese");
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	ifstream Arquivo; 
    string ler;
    Arquivo.open("sobrenomes.txt");
    V.pb("/"); //barras indicam inicio e fim de palavras
	while (Arquivo >> ler) {
		string silaba, palavra;
		for (int i = 0; i < (int)ler.size(); i++) {
			ler[i] = tolower(ler[i]); //Trabalhamos apenas com letras minusculas
			if (ler[i] == '-') {
				V.pb(silaba);
				silaba.clear();
			}
			else {
				if (ler[i] != '.') {
						silaba += ler[i];
						palavra += ler[i];
				}
			}
		}
		V.pb(silaba);
		ultimas.pb(silaba);//silaba final de palavra, usaremos para rimar
		V.pb("/");
		S.insert(palavra);
		ler.clear();
	}
	int barra;
	long int check;
	for(long int i=0; i<(int)V.size(); i++){
		if(M.find(V[i])==M.end()){
			if(V[i]=="/")barra = cnt;
			M.insert(mp(V[i],cnt)); // sorteamos inteiros, depois usamos seu equivalente em string para imprimir texto
			Mrev.insert(mp(cnt,V[i]));
			cnt++;
		}
	}
	for(int i=1; i<(int)V.size(); i++){
		string s1 = V[i-1];
		string s2 = V[i];
		int v1 = M[s1]; 
		int v2 = M[s2];
		prob[v1][v2]++; soma[v1]++;
		prob_rev[v2][v1]++; soma_rev[v2]++;
		if(v1==barra && s2[0]-'a'>=0){//estamos no comeco de uma palavra, fazemos pre-processamento para geracao de anagramas
			comeco_anag[s2[0]-'a'].pb(v2);
		}
	}
	for(int i=1; i<cnt; i++){
		for(int j=1; j<cnt; j++){
			prob[i][j] = (prob[i][j]/soma[i])*SAMP; //calculamos probabilidades para sorteio
			prob_rev[i][j] = (prob_rev[i][j]/soma_rev[i])*SAMP;
		}
	}	
	srand(time(NULL));
	quantidade = rand() % 3 + 1;
	nome(barra, quantidade);
	Arquivo.close();
    return 0;
}
