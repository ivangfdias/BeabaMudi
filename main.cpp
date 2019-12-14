#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
//#include <io.h> //requer Visual Studio
#include <iostream>
#ifdef __arm__ // Raspberry Pi
#include "wiringPi.h"
#elif _WIN32 // Windows
#include <Windows.h>
#else // Outras plataformas

#endif
using namespace std;

#define pb push_back
#define mp make_pair
const int N_BOTOES = 2;
const int ALFABETO = 'z'-'a'+1;
const int MAXN = 4009;
const int SAMP = 1e4+9;

int cnt=0;
vector<string> V, ultimas,trava_lingua; // V armazena texto dividido em silabas para montagem do grafo
vector<int> Escolha, verso, prov; //Escolha eh vetor usado como espaco amostral para sorteio de silabas
vector<int> comeco_anag[ALFABETO];//guardamos silabas de inicio de palavra para cada letra do alfabeto
map<string,int> M;
map<int,string> Mrev;
double prob_rev[MAXN][MAXN], soma_rev[MAXN]; //matriz de adjacencias e soma de linhas para calculo de probabilidades
//O primeiro vetor carrega o codigo ASCII das letras acentuadas e cecedilla; o segundo carrega o codigo ANSI das letras acentuadas e cecedilla
int ascii_special[] = { 192, 193, 194, 195, 199, 201, 202, 205, 211, 212, 213, 218 };
int string_special[] = { 224, 225, 226, 227, 231, 233, 234, 237, 243, 244, 245, 250 };

struct botao {
    int n;
    int estado;
    int atraso;
};
const int PERIODO_MS = 10; // Intervalo em que botoes sao lidos
const int ATRASO_BOUNCE = 40/PERIODO_MS;
const int ATRASO_2BOTOES = 55/PERIODO_MS;
const int ATRASO_IMPR_MS = 300;
enum estado_botao { APERTADO, SOLTO, ATIVAR, DESAPERTADO };

void poema(int barra);
int leitura_botao(int n_botao);
void atualiza_botao(botao* b);
void informacoes();
bool dois_botoes( botao b1, botao b2 );

void poema(int barra)
{
    int versos[4] = { 4,4,3,3 };

    int rima[4][4];
    set<int>rimas;
    rima[0][0] = M[ultimas[rand() % (int)ultimas.size()]];
    rimas.insert(rima[0][0]);
    rima[0][3] = rima[0][0];
    rima[1][0] = rima[0][0];
    rima[1][3] = rima[0][0];

    do {
        rima[0][1] = M[ultimas[rand() % (int)ultimas.size()]];
    } while (rimas.find(rima[0][1]) != rimas.end());
    rimas.insert(rima[0][1]);
    rima[0][2] = rima[0][1];
    rima[1][1] = rima[0][1];
    rima[1][2] = rima[0][1];

    do {
        rima[2][0] = M[ultimas[rand() % (int)ultimas.size()]];
    } while (rimas.find(rima[2][0]) != rimas.end());
    rimas.insert(rima[2][0]);
    rima[2][2] = rima[2][0];
    rima[3][1] = rima[2][0];

    do {
        rima[2][1] = M[ultimas[rand() % (int)ultimas.size()]];
    } while (rimas.find(rima[2][1]) != rimas.end());
    rima[3][0] = rima[2][1];
    rima[3][2] = rima[2][1];

    // o esquema de rimas do soneto eh abba abba cdc dcd
    vector<string> para_salvar;
    cout << "\n";
    cout.flush();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < versos[i]; j++) {
            verso.clear();
            bool flag = true;
            int atual;
            while ((int)verso.size() < 10) {
                if (flag)
                    atual = rima[i][j];
                else
                    atual = barra;
                prov.clear();
                do {
                    if (atual != barra)
                        prov.pb(atual);
                    Escolha.clear();
                    for (int l = 0; l < cnt; l++)
                        for (int k = 1; k <= prob_rev[atual][l]; k++)
                            Escolha.pb(l); //preenchemos nosso espaco amostral com base nas probabilidades
                    if (Escolha.size() == 0)
                        atual = barra;
                    else
                        atual = Escolha[rand() % (int)Escolha.size()];
                } while (atual != barra);

                if (prov.size() >= 1 && prov.size() <= 5 && (int)prov.size() + (int)verso.size() < 12) {
                    for (int k = 0; k < (int)prov.size(); k++) {
                        verso.pb(prov[k]);
                    }
                    verso.pb(barra);
                    flag = false;
                }
            }
            string aux;
            aux.clear();
            for (int k = (int)verso.size() - 2; k >= 0; k--) {
                if (verso[k] == barra) {
                    aux += " ";
                    cout << " ";
                    cout.flush();
                    continue;
                }
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
            cout << endl;
            cout.flush();
        }
        cout << endl;
        cout.flush();
    }
    ofstream file_;
    file_.open("TextoGerado.txt");

    for (int i = 0; i < (int)para_salvar.size(); i++) {
        if (i == 4 || i == 8 || i == 11)
            file_ << " \n";
        file_ << (para_salvar[i]) << "\n";

    }

    file_.close();
    system("imprime-poema.sh");

}

void sr_ovo1()
{
    // Imprime segredo 1
    system("imprime-segredo1.sh");
}

void informacoes()
{
    //Cabe espaço pra rodar o programa de latexificação das informações ou a gente faz direto em latex
    system("imprime-informacoes.sh");
}

void atualiza_botao(botao* b)
{
    switch(b->estado) {
    case APERTADO:
        b->atraso++;
        if( b->atraso > ATRASO_BOUNCE && leitura_botao(b->n) == 0 ) {
            b->estado = SOLTO;
            b->atraso = 0;
        }
        break;
    case SOLTO:
        b->atraso++;
        if ( b->atraso > ATRASO_2BOTOES ) {
            b->estado = ATIVAR;
            b->atraso = 0;
        }
        break;
    case ATIVAR:
        b->estado = DESAPERTADO;
        b->atraso = 0;
        break;
    case DESAPERTADO:
        b->atraso++;
        if( b->atraso > ATRASO_BOUNCE && leitura_botao(b->n) == 1 ) {
            b->estado = APERTADO;
            b->atraso = 0;
        }
        break;
    default:
        break;
    }
}

int leitura_botao(int n_botao)
{
#ifdef _WIN32
    //cout << "Leitura..." << endl;
    switch (n_botao) {
    case 0:
        if(GetKeyState('H') & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            return 1;
        }
        return 0;
        break;
    case 1:
        if(GetKeyState('J') & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            return 1;
        }
        return 0;
        break;
    }
#elif defined(__arm__)
    int pino;
    switch (n_botao) {
    case 0:
        pino = 0;
        break;
    case 1:
        pino = 1;
        break;
    }
    return digitalRead(pino);
#endif // __arm__
    return 0;
}

bool dois_botoes( botao b1, botao b2 )
{
    return ( ( b1.estado == ATIVAR && b2.estado == SOLTO ) ||
             ( b2.estado == ATIVAR && b1.estado == SOLTO ) ||
             ( b1.estado == ATIVAR && b2.estado == ATIVAR ) );
}

void debug_print( string s )
{
#ifdef DEBUG
    cout << s << endl;
#endif // DEBUG
    return;
}

int main()
{
#ifdef __arm__
    wiringPiSetup();
    pinMode(0, INPUT);
    pinMode(1, INPUT);
#endif // __arm__
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
            } else {
                if (ler[i] != '.') {
                    silaba += ler[i];
                    palavra += ler[i];
                }
            }
        }
        V.pb(silaba);
        ultimas.pb(silaba);//silaba final de palavra, usaremos para rimar
        V.pb("/");
        ler.clear();
    }
    int barra;
    long int check;
    for(long int i=0; i<(int)V.size(); i++) {
        if(M.find(V[i])==M.end()) {
            if(V[i]=="/")
                barra = cnt;
            M.insert(mp(V[i],cnt)); // sorteamos inteiros, depois usamos seu equivalente em string para imprimir texto
            Mrev.insert(mp(cnt,V[i]));
            cnt++;
        }
    }
    for(int i=1; i<(int)V.size(); i++) {
        string s1 = V[i-1];
        string s2 = V[i];
        int v1 = M[s1];
        int v2 = M[s2];
        prob_rev[v2][v1]++;
        soma_rev[v2]++;
        if(v1==barra && s2[0]-'a'>=0) { //estamos no comeco de uma palavra, fazemos pre-processamento para geracao de anagramas
            comeco_anag[s2[0]-'a'].pb(v2);
        }
    }
    for(int i=1; i<cnt; i++) {
        for(int j=1; j<cnt; j++) {
            prob_rev[i][j] = (prob_rev[i][j]/soma_rev[i])*SAMP; //calculamos probabilidades para sorteio
        }
    }
    srand(time(NULL));

    struct botao botoes[N_BOTOES];
    for (int i = 0; i < N_BOTOES; i++ ) {
        botoes[i].n = i;
        botoes[i].estado = DESAPERTADO;
        botoes[i].atraso = 0;
    }

    while(true) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(PERIODO_MS));
        // Estados do botao:
        // DESAPERTADO -> APERTADO -> SOLTO -> ATIVAR -> DESAPERTADO
        // Funcao do botao ativada em ATIVAR

        for (int i = 0; i < N_BOTOES; i++ ) {
            atualiza_botao(&botoes[i]);
        }

        // Dois botoes soltos ao mesmo tempo
        if ( dois_botoes(botoes[0], botoes[1]) ) {
            cout << "segredo1" << endl;
            //sr_ovo1();
            botoes[0].estado = DESAPERTADO;
            botoes[1].estado = DESAPERTADO;
            //std::this_thread::sleep_for(std::chrono::milliseconds(ATRASO_IMPR_MS));
        } else if ( botoes[0].estado == ATIVAR ) {
            cout << "poema" << endl;
            poema(barra);
            //std::this_thread::sleep_for(std::chrono::milliseconds(ATRASO_IMPR_MS));
            botoes[1].estado = DESAPERTADO;
        } else if ( botoes[1].estado == ATIVAR ) {
            cout << "informacoes" << endl;
            //informacoes();
           // std::this_thread::sleep_for(std::chrono::milliseconds(ATRASO_IMPR_MS));
            botoes[0].estado = DESAPERTADO;
        }
    }
    return 0;
}
