﻿#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#define TAM 256
#define BUFFER_SIZE 100
#define BUFFER 10

#define MAX_NUM_PLAYERS 20
#define MAX_REGISTO 10
//Variaveis configuraveis para o jogo
#define MAX_NUM_TIJOLOS 30
#define MAX_NUM_BRINDES 30

#define CMD_MOVE_CIMA 1
#define CMD_MOVE_BAIXO 2
#define CMD_MOVE_ESQ 3 
#define CMD_MOVE_DIR 4
#define CMD_LOGIN 5
#define CMD_LOGOUT 6

#define PIPE_NAME TEXT("\\\\.\\pipe\\connect")

#define REGKEY TEXT("SOFTWARE\\Arkanoid")
LPCTSTR value = TEXT("Scores");

HKEY hKey;

//Mensagens (SINCRONIZAÇÃO)
HANDLE podeEscrever;
HANDLE podeLer;
HANDLE hMutexLer;
HANDLE hMutexEscrever;

//Objetos JOGO (SINCRONIZAÇÃO)
HANDLE mutex_bola;

//Arrays para controlar brindes e secalhar tijolos

//Memoria Partilhada 
TCHAR nomeMemoriaComandos[] = TEXT("Nome da Memoria Partilhada Comandos");
TCHAR nomeMemoriaJogo[] = TEXT("Nome da Mem�ria Partilhada Jogo");

TCHAR nomeSemaforoPodeEscrever[] = TEXT("Semaforo Pode Escrever");
TCHAR nomeSemaforoPodeLer[] = TEXT("Semaforo Pode Ler");

//EVENTOS
TCHAR nomeEventoComecoJogo[] = TEXT("EventoComeco");
TCHAR nomeEventoArrancaMemoria[] = TEXT("EventoMemoria");
TCHAR nomeEventoTerminaJogo[] = TEXT("EventoTermina");

//Tipo de cenas
enum Tipo_Tijolo{normal, resistente, magico};
enum Tipo_Brinde{speed_up, slow_down, vida_extra, triple, barreira}; //Adicionar outros brindes consoante a originalidade

//Estruturas
typedef struct {
	int id;
	COORD coord;
	int dimensao;
}Barreira;

//COMANDO PARTILHADO (COMANDO_SHARED)
typedef struct {
	int id;
	TCHAR nome[BUFFER_SIZE];
	Barreira barreira;
	int vidas;
	INT pontos;
	bool login; //Verificar se foi login
	HANDLE idHandle;
}Player;

typedef struct {
	int idUser;
	int tipo;
	bool login;
	HANDLE idHandle;
} COMANDO_SHARED;

typedef struct {
	COORD coord;
}Bola;


typedef struct {
	int id;
	COORD coord;
	int dimensao;
	Tipo_Tijolo tipo;
	int vida;
}Tijolo;

typedef struct {
	int id;
	COORD coord;
	DWORD threadId;
	int dimensao;
	Tipo_Brinde tipo;
	int velocidade;
	int duracao;
}Brinde;

typedef struct {
	Player jogadores[MAX_REGISTO];
}Scores;

typedef struct {
	Player players[MAX_NUM_PLAYERS];
	Scores ranking;
	Bola bola;  // Ver se só existe uma bola
	Tijolo tijolos[MAX_NUM_TIJOLOS];
	Brinde brindes[MAX_NUM_BRINDES];

}MensagemJogo; //Jogo

typedef struct {
	MensagemJogo jogo;
}DadosJogo;


//DADOS
typedef struct {
	COMANDO_SHARED PtrMemoria[BUFFER];
	int posRead, posWrite;
} Dados;

//DATACR
typedef struct {
	int posE, posL;
	Dados* shared;
	HANDLE hMapFileMSG;
	HANDLE hSemafroPodeEscrever;
	HANDLE hSemafroPodeLer;
	
	DadosJogo *sharedJogo;
	HANDLE hMapFileJogo;

}dataCr;


#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif


extern "C"
{
	//Váriavel global da DLL
	extern DLL_IMP_API int nDLL;

	//Protótipos Funções
	DLL_IMP_API bool createSharedMemory(dataCr* d);
	DLL_IMP_API bool openSharedMemory(dataCr* d);
	DLL_IMP_API void readMensagem(dataCr* d, COMANDO_SHARED* s);
	DLL_IMP_API void writeMensagem(dataCr* d, COMANDO_SHARED* s);

	DLL_IMP_API void createSharedMemoryJogo(dataCr* d);
	DLL_IMP_API bool openSharedMemoryJogo(dataCr* d);
	DLL_IMP_API void readMensagemJogo(dataCr* d, MensagemJogo* s);
	DLL_IMP_API void writeMensagemJogo(dataCr* d, MensagemJogo* s);

}