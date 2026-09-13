#ifndef STANDARD_PLC_H
#define STANDARD_PLC_H

#include <stdbool.h>
#include <stdint.h>
#include <limits.h> // Para UINT32_MAX

// =================================================================
// 1. BLOCOS DE TEMPORIZAÇÃO (TON, TOF, TP)
// =================================================================

/**
 * @brief Estrutura para o bloco de função TON (Timer ON Delay).
 */
typedef struct {
    uint32_t PT;           // Tempo Predefinido (Preset Time) em ms
    uint32_t ET;           // Tempo Decorrido (Elapsed Time) em ms
    bool Q;                // Saída do temporizador (BOOL)
    uint32_t start_time;   // Marcação de tempo interno para iniciar a contagem
} TON_Block;

/**
 * @brief Processa a lógica do temporizador TON (ON Delay).
 * @param timer Ponteiro para a estrutura TON_Block.
 * @param IN Estado da entrada (TRUE para iniciar/manter a contagem).
 * @param current_time O tempo atual do sistema em milissegundos (ms).
 */
void ProcessarTON(TON_Block *timer, bool IN, uint32_t current_time);

/**
 * @brief Estrutura para o bloco de função TOF (Timer OFF Delay).
 */
typedef struct {
    uint32_t PT;           // Tempo Predefinido (Preset Time) em ms
    uint32_t ET;           // Tempo Decorrido (Elapsed Time) em ms
    bool Q;                // Saída do temporizador (BOOL)
    uint32_t start_time;   // Marcação de tempo interno para iniciar a contagem (no desligamento de IN)
} TOF_Block;

/**
 * @brief Processa a lógica do temporizador TOF (OFF Delay).
 * @param timer Ponteiro para a estrutura TOF_Block.
 * @param IN Estado da entrada (TRUE para manter Q ativo).
 * @param current_time O tempo atual do sistema em milissegundos (ms).
 */
void ProcessarTOF(TOF_Block *timer, bool IN, uint32_t current_time);

/**
 * @brief Estrutura para o bloco de função TP (Timer Pulse).
 */
typedef struct {
    uint32_t PT;           // Duração do Pulso (Preset Time) em ms
    uint32_t ET;           // Tempo Decorrido (Elapsed Time) em ms
    bool Q;                // Saída do temporizador (BOOL)
    uint32_t start_time;   // Marcação de tempo interno para iniciar o pulso
    bool IN_old;           // Estado anterior da entrada IN (para detectar a borda de subida)
} TP_Block;

/**
 * @brief Processa a lógica do temporizador TP (Pulso).
 * @param timer Ponteiro para a estrutura TP_Block.
 * @param IN Estado da entrada (Gatilho para iniciar o pulso).
 * @param current_time O tempo atual do sistema em milissegundos (ms).
 */
void ProcessarTP(TP_Block *timer, bool IN, uint32_t current_time);

// =================================================================
// 2. BLOCOS DE CONTAGEM (CTU, CTD, CTUD)
// =================================================================

/**
 * @brief Estrutura para o bloco de função CTU (Counter Up).
 */
typedef struct {
    uint32_t PV;           // Valor Predefinido
    uint32_t CV;           // Valor Atual da Contagem
    bool Q;                // Saída (BOOL)
    bool CU_old;           // Estado anterior da entrada CU
} CTU_Block;

/**
 * @brief Processa a lógica do contador CTU.
 * @param counter Ponteiro para a estrutura CTU_Block.
 * @param CU Entrada de Contagem Crescente (ativa em borda de subida).
 * @param R Entrada de Reset (TRUE para zerar o contador).
 */
void ProcessarCTU(CTU_Block *counter, bool CU, bool R);

/**
 * @brief Estrutura para o bloco de função CTD (Counter Down).
 */
typedef struct {
    uint32_t PV;           // Valor Predefinido (Valor de Carregamento Inicial)
    uint32_t CV;           // Valor Atual da Contagem
    bool Q;                // Saída (BOOL)
    bool CD_old;           // Estado anterior da entrada CD
} CTD_Block;

/**
 * @brief Processa a lógica do contador CTD.
 * @param counter Ponteiro para a estrutura CTD_Block.
 * @param CD Entrada de Contagem Decrescente (ativa em borda de subida).
 * @param LD Entrada de Load (TRUE para carregar CV com PV).
 */
void ProcessarCTD(CTD_Block *counter, bool CD, bool LD);

/**
 * @brief Estrutura para o bloco de função CTUD (Counter Up/Down).
 */
typedef struct {
    uint32_t PV;           // Valor Predefinido
    uint32_t CV;           // Valor Atual da Contagem
    bool QU;               // Saída Crescente (Q Up)
    bool QD;               // Saída Decrescente (Q Down)
    bool CU_old;           // Estado anterior da entrada CU
    bool CD_old;           // Estado anterior da entrada CD
} CTUD_Block;

/**
 * @brief Processa a lógica do contador CTUD.
 * @param counter Ponteiro para a estrutura CTUD_Block.
 * @param CU Entrada de Contagem Crescente (ativa em borda de subida).
 * @param CD Entrada de Contagem Decrescente (ativa em borda de subida).
 * @param R Entrada de Reset (TRUE para zerar o contador).
 * @param LD Entrada de Load (TRUE para carregar CV com PV).
 */
void ProcessarCTUD(CTUD_Block *counter, bool CU, bool CD, bool R, bool LD);

// =================================================================
// 3. BLOCOS DE DETECÇÃO DE BORDA (R_TRIG, F_TRIG)
// =================================================================

/**
 * @brief Estrutura para o bloco de função R_TRIG (Rising Edge Trigger).
 */
typedef struct {
    bool Q;     // Saída (TRUE por um ciclo de execução após a borda)
    bool M;     // Memória do estado anterior da entrada CLK
} R_TRIG_Block;

/**
 * @brief Processa a lógica de detecção de borda de subida (R_TRIG).
 * @param trigger Ponteiro para a estrutura R_TRIG_Block.
 * @param CLK Entrada de relógio ou sinal a ser monitorado.
 */
void ProcessarR_TRIG(R_TRIG_Block *trigger, bool CLK);

/**
 * @brief Estrutura para o bloco de função F_TRIG (Falling Edge Trigger).
 */
typedef struct {
    bool Q;     // Saída (TRUE por um ciclo de execução após a borda)
    bool M;     // Memória do estado anterior da entrada CLK
} F_TRIG_Block;

/**
 * @brief Processa a lógica de detecção de borda de descida (F_TRIG).
 * @param trigger Ponteiro para a estrutura F_TRIG_Block.
 * @param CLK Entrada de relógio ou sinal a ser monitorado.
 */
void ProcessarF_TRIG(F_TRIG_Block *trigger, bool CLK);

#endif // STANDARD_PLC_H