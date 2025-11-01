#include "standard_plc.h"

// Função auxiliar para lidar com o wrap-around (estouro) do uint32_t
uint32_t calculate_elapsed_time(uint32_t current_time, uint32_t start_time) {
    if (current_time >= start_time) {
        return current_time - start_time;
    } else {
        // Lida com o 'wrap-around'
        return (UINT32_MAX - start_time) + current_time;
    }
}

// =================================================================
// IMPLEMENTAÇÕES DE TEMPORIZADORES
// =================================================================

void ProcessarTON(TON_Block *timer, bool IN, uint32_t current_time) {
    if (IN) {
        if (!timer->Q) {
            if (timer->start_time == 0) {
                timer->start_time = current_time;
                timer->ET = 0;
            } else {
                timer->ET = calculate_elapsed_time(current_time, timer->start_time);
                
                if (timer->ET >= timer->PT) {
                    timer->Q = true;
                    timer->ET = timer->PT;
                }
            }
        } else {
            timer->ET = timer->PT; // Travado em PT quando Q é TRUE e IN é TRUE
        }
    } else {
        // Reset
        timer->start_time = 0;
        timer->ET = 0;
        timer->Q = false;
    }
}

void ProcessarTOF(TOF_Block *timer, bool IN, uint32_t current_time) {
    if (IN) {
        // Ativação imediata e reset da contagem
        timer->Q = true;
        timer->ET = 0;
        timer->start_time = 0; 
        
    } else {
        // Contagem (Delay OFF)
        if (timer->Q) {
            if (timer->start_time == 0) {
                // Inicia a contagem na borda de descida de IN
                timer->start_time = current_time;
                timer->ET = 0;
            } else {
                timer->ET = calculate_elapsed_time(current_time, timer->start_time);
                
                if (timer->ET >= timer->PT) {
                    timer->Q = false; // Desliga a saída
                    timer->ET = timer->PT;
                }
            }
        } else {
            // Estado de espera (Q já está FALSE)
            timer->Q = false;
            timer->ET = timer->PT;
            timer->start_time = 0;
        }
    }
}

void ProcessarTP(TP_Block *timer, bool IN, uint32_t current_time) {
    bool rising_edge = (IN && !timer->IN_old);

    if (rising_edge) {
        if (!timer->Q) {
            // Inicia o pulso SOMENTE se não houver um pulso em andamento
            timer->Q = true;
            timer->start_time = current_time;
            timer->ET = 0;
        }
    }
    
    if (timer->Q) {
        timer->ET = calculate_elapsed_time(current_time, timer->start_time);
        
        if (timer->ET >= timer->PT) {
            timer->Q = false; // Fim do Pulso
            timer->ET = timer->PT;
            timer->start_time = 0;
        }
    } else {
        timer->ET = 0;
    }
    
    timer->IN_old = IN; // Atualiza para detecção de borda no próximo ciclo
}

// =================================================================
// IMPLEMENTAÇÕES DE CONTADORES
// =================================================================

void ProcessarCTU(CTU_Block *counter, bool CU, bool R) {
    bool rising_edge_CU = (CU && !counter->CU_old);

    if (R) {
        counter->CV = 0;
    } else {
        if (rising_edge_CU) {
            if (counter->CV < UINT32_MAX) {
                counter->CV++;
            }
        }
    }
    
    counter->Q = (counter->CV >= counter->PV);
    counter->CU_old = CU;
}

void ProcessarCTD(CTD_Block *counter, bool CD, bool LD) {
    bool rising_edge_CD = (CD && !counter->CD_old);

    if (LD) {
        counter->CV = counter->PV;
    } else {
        if (rising_edge_CD) {
            if (counter->CV > 0) {
                counter->CV--;
            }
        }
    }
    
    counter->Q = (counter->CV == 0);
    counter->CD_old = CD;
}

void ProcessarCTUD(CTUD_Block *counter, bool CU, bool CD, bool R, bool LD) {
    bool rising_edge_CU = (CU && !counter->CU_old);
    bool rising_edge_CD = (CD && !counter->CD_old);

    if (R) {
        counter->CV = 0;
    } else if (LD) {
        counter->CV = counter->PV;
    } else {
        // Prioriza a anulação ou ignora se CU e CD estiverem ativos simultaneamente
        if (rising_edge_CU && !rising_edge_CD) {
            if (counter->CV < UINT32_MAX) {
                counter->CV++;
            }
        } else if (rising_edge_CD && !rising_edge_CU) {
            if (counter->CV > 0) {
                counter->CV--;
            }
        }
    }

    counter->QU = (counter->CV >= counter->PV);
    counter->QD = (counter->CV == 0); 
    
    counter->CU_old = CU;
    counter->CD_old = CD;
}

// =================================================================
// IMPLEMENTAÇÕES DE DETECTORES DE BORDA
// =================================================================

void ProcessarR_TRIG(R_TRIG_Block *trigger, bool CLK) {
    trigger->Q = (CLK && !trigger->M);
    trigger->M = CLK;
}

void ProcessarF_TRIG(F_TRIG_Block *trigger, bool CLK) {
    trigger->Q = (!CLK && trigger->M);
    trigger->M = CLK;
}