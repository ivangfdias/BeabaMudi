#include <bits/stdc++.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <io.h>
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
/*
std::wstring utf8_to_utf16(const std::string& utf8){
    std::vector<unsigned long> unicode;
    size_t i = 0;
    while (i < utf8.size())
    {
        unsigned long uni;
        size_t todo;
        bool error = false;
        unsigned char ch = utf8[i++];
        if (ch <= 0x7F)
        {
            uni = ch;
            todo = 0;
        }
        else if (ch <= 0xBF)
        {
            throw std::logic_error("not a UTF-8 string");
        }
        else if (ch <= 0xDF)
        {
            uni = ch&0x1F;
            todo = 1;
        }
        else if (ch <= 0xEF)
        {
            uni = ch&0x0F;
            todo = 2;
        }
        else if (ch <= 0xF7)
        {
            uni = ch&0x07;
            todo = 3;
        }
        else
        {
            throw std::logic_error("not a UTF-8 string");
        }
        for (size_t j = 0; j < todo; ++j)
        {
            if (i == utf8.size())
                throw std::logic_error("not a UTF-8 string");
            unsigned char ch = utf8[i++];
            if (ch < 0x80 || ch > 0xBF)
                throw std::logic_error("not a UTF-8 string");
            uni <<= 6;
            uni += ch & 0x3F;
        }
        if (uni >= 0xD800 && uni <= 0xDFFF)
            throw std::logic_error("not a UTF-8 string");
        if (uni > 0x10FFFF)
            throw std::logic_error("not a UTF-8 string");
        unicode.push_back(uni);
    }
    std::wstring utf16;
    for (size_t i = 0; i < unicode.size(); ++i)
    {
        unsigned long uni = unicode[i];
        if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}
*/
/*
void acronimo(int barra) {
	string palavra;
	cout << "Ok, Digite a palavra: "; cout.flush();
	cin >> palavra;
	cout << "\n"; cout.flush();
	vector<string> para_salvar;
	for (int i = 0; i < (int)palavra.size(); i++) {
		palavra[i] = tolower(palavra[i]);
		int silaba_inicial = comeco_anag[palavra[i] - 'a'][rand() % (int)comeco_anag[palavra[i] - 'a'].size()];
		string silaba = Mrev[silaba_inicial];
		int atual = silaba_inicial;
		int minimo_silabas = 0;
		string anagrama; anagrama.clear();
		do {
			if (atual != barra)anagrama += silaba;
			minimo_silabas++;
			Escolha.clear();
			for (int j = 0; j < cnt; j++)
				for (int k = 1; k <= prob[atual][j]; k++)
					Escolha.pb(j); //preenchemos nosso espaco amostral com base nas probabilidades
			if (Escolha.size() == 0)break;
			atual = Escolha[rand() % (int)Escolha.size()];
			silaba = Mrev[atual];
		} while (atual != barra);
		if (anagrama.size() < 3 || anagrama.size() > 8 || S.find(anagrama) != S.end()) {
			i--;
		}
		else {
			anagrama[0] = toupper(anagrama[0]);
			cout << anagrama << endl; cout.flush();
			para_salvar.pb(anagrama);
		}
	}
	printf("\n");
	ofstream file_;
	file_.open("TextoGerado.txt");
	for (int i = 0; i < (int)para_salvar.size(); i++)
		file_ << para_salvar[i] << "\n";
	file_.close();
	//system("lp TextoGerado.txt");
}
*/
void poema(int barra) {
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
	cout << "\n"; cout.flush();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < versos[i]; j++) {
			verso.clear();
			bool flag = true;
			int atual;
			while ((int)verso.size() < 10) {
				if (flag)atual = rima[i][j];
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

				if (prov.size() >= 1 && prov.size() <= 5 && (int)prov.size() + (int)verso.size() < 12) {
					for (int k = 0; k < (int)prov.size(); k++) {
						verso.pb(prov[k]);
					}
					verso.pb(barra); flag = false;
				}
			}
			string aux; aux.clear();
			for (int k = (int)verso.size() - 2; k >= 0; k--) {
				if (verso[k] == barra) { aux += " "; cout << " "; cout.flush(); continue; }
				aux += Mrev[verso[k]];
				cout << Mrev[verso[k]];
				cout.flush();
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
			cout << endl; cout.flush();
		}
		cout << endl; cout.flush();
	}
	ofstream file_;
	file_.open("TextoGerado.txt");
	
	for (int i = 0; i < (int)para_salvar.size(); i++) {
		if (i == 4 || i == 8 || i == 11)file_ << " \n";
		file_ << (para_salvar[i]) << "\n";
	
	}

	file_.close();
	//system("lp TextoGerado.txt");

}
/*
void travalingua(int barra) {
	set<int> proibido;
	proibido.insert('w' - 'a'); proibido.insert('y' - 'a');
	proibido.insert('x' - 'a'); proibido.insert('k' - 'a');
	proibido.insert('w' - 'a'); proibido.insert('q' - 'a');
	proibido.insert('z' - 'a'); proibido.insert('j' - 'a');
	proibido.insert('u' - 'a'); proibido.insert('o' - 'a');
	proibido.insert('e' - 'a'); proibido.insert('h' - 'a');
	proibido.insert('i' - 'a'); proibido.insert('a' - 'a');
	int letra_rep;
	do { letra_rep = rand() % ALFABETO; } while (proibido.find(letra_rep) != proibido.end());
	int inicio_rep[20];
	for (int i = 0; i < 20; i++) {
		inicio_rep[i] = comeco_anag[letra_rep][rand() % (int)comeco_anag[letra_rep].size()];
	}
	trava_lingua.clear();
	set<string> ja_foi;
	while ((int)trava_lingua.size() < 5) {
		int atual = inicio_rep[rand() % 20];
		string palavra; palavra.clear();
		while (true) {
			if (atual == barra)break;
			palavra += Mrev[atual];
			Escolha.clear();
			for (int i = 0; i < cnt; i++)
				for (int k = 1; k <= prob[atual][i]; k++)
					Escolha.pb(i);
			if (Escolha.size() == 0)atual = barra;
			else atual = Escolha[rand() % (int)Escolha.size()];
		}
		if (palavra.size() < 6 && palavra.size() > 2 && ja_foi.find(palavra) == ja_foi.end()) {
			ja_foi.insert(palavra);
			palavra[0] = toupper(palavra[0]);
			trava_lingua.pb(palavra);
		}
	}
	for (int i = 0; i < (int)trava_lingua.size(); i++) {
		cout << trava_lingua[i] << " "; cout.flush();
	}
	cout << endl; cout.flush();
	ofstream file_;
	file_.open("TextoGerado.txt");
	for (int i = 0; i < (int)trava_lingua.size(); i++) {
		file_ << trava_lingua[i] << " ";
	}
	file_.close();
	system("lp TextoGerado.txt");
	cout << endl; cout.flush();
}
*/
void informacoes() {
	ofstream file_;
	file_.open("Informacoes.txt");
	//IMPRIME INFORMACOES.TXT
	cout << "Imprimindo Informacoes.txt ..." << endl;
	//system("lp TextoGerado.txt");
	file_.close();
}



int main(){
	//wiringPiSetup();
	//PinMode(0, INPUT);
	//PinMode(1, INPUT);
    setlocale(LC_ALL, "Portuguese");
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	ifstream Arquivo; 
    string ler;
    Arquivo.open("nuBank.txt");
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
	cout << "cnt" << cnt << endl;
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
    cout<<"Bem vindo ao Projeto BEABA 2.0\n"; cout.flush();
	while(true){
    	cout<<"1 -> Gerar acronimo\n"; cout.flush();
    	cout<<"0 -> Informacoes\n"; cout.flush();
		cout << "4 -> Sair \n"; cout.flush();
		int x;
		cin >> x;
		if (/*digitalRead(0) == 1 && digitalRead(1) == 0*/ x == 1) system("lp informacoes.txt");
		else if (/*digitalRead(1) == 1 && digitalRead(0) == 0*/ x == 0) poema(barra);
		else if (/*digitalRead(1) == 1 && digitalRead(1) == 1*/ x == 2) {
			/*system("lp aperture.txt");*/
			cout << (unsigned char)192 << endl;
		}
		else if (x == 4) break;
    	
		cout<<"Obrigado por utilizar o Beaba!\n"; cout.flush();
	}
    return 0;
}
