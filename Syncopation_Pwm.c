/*
 * MSST_Pwm.c
 *
 *  Created on: Oct 18, 2016
 *      Author: Yang Lei
 */

#include "F28x_Project.h"

#define PWM_PRD      1562
#define PWM_CMP      781
#define PWM_DB       200

void Epwm1Init()
{
    EPwm1Regs.TBPRD = PWM_PRD;

    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;
    EPwm1Regs.TBCTL.bit.PHSEN = 0;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm1Regs.AQCTLA.bit.CAU = 2;
    EPwm1Regs.AQCTLA.bit.CAD = 1;

    EPwm1Regs.DBRED.bit.DBRED = PWM_DB-1;
    EPwm1Regs.DBFED.bit.DBFED = PWM_DB-1;
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;
    EPwm1Regs.DBCTL.bit.POLSEL = 2;
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;

    EPwm1Regs.CMPA.bit.CMPA = PWM_CMP;

    EPwm1Regs.TBCTL.bit.CTRMODE = 2; // UP-DOWN mode, start the counter

    EPwm1Regs.ETSEL.bit.SOCASEL = 1; // SOCA at counter equals to 0
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;  // Enable SOCA
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;  // Pulse at every event
}

void Epwm2Init()
{
    EPwm2Regs.TBPRD = PWM_PRD;

    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0;
    EPwm2Regs.TBCTL.bit.PHSEN = 1;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm2Regs.AQCTLA.bit.CAU = 2;
    EPwm2Regs.AQCTLA.bit.CAD = 1;

    EPwm2Regs.DBRED.bit.DBRED = PWM_DB-1;
    EPwm2Regs.DBFED.bit.DBFED = PWM_DB-1;
    EPwm2Regs.DBCTL.bit.IN_MODE = 0;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;

    EPwm2Regs.CMPA.bit.CMPA = PWM_CMP;

    EPwm2Regs.TBCTL.bit.CTRMODE = 2;

    EALLOW;
    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EDIS;
}

void Epwm3Init()
{
    EPwm3Regs.TBPRD = PWM_PRD;

    EPwm3Regs.TBCTL.bit.SYNCOSEL = 0;
    EPwm3Regs.TBCTL.bit.PHSEN = 1;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;

    EPwm3Regs.AQCTLA.bit.CAU = 2;
    EPwm3Regs.AQCTLA.bit.CAD = 1;

    EPwm3Regs.DBRED.bit.DBRED = PWM_DB-1;
    EPwm3Regs.DBFED.bit.DBFED = PWM_DB-1;
    EPwm3Regs.DBCTL.bit.IN_MODE = 0;
    EPwm3Regs.DBCTL.bit.POLSEL = 2;
    EPwm3Regs.DBCTL.bit.OUT_MODE = 3;
//    EPwm3Regs.DBCTL.bit.OUTSWAP = 3;

    EPwm3Regs.CMPA.bit.CMPA = PWM_CMP;

    EPwm3Regs.TBCTL.bit.CTRMODE = 2;

    EALLOW;
    EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EDIS;
}

void EpwmInit()
{
    Epwm1Init();
    Epwm2Init();
    Epwm3Init();
}

void Dab_EN()
{
    EALLOW;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm3Regs.TZCLR.bit.OST = 1;
    EDIS;
}

void Dab_DIS()
{
    EALLOW;
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    EDIS;
}

Uint16 dab_prd = PWM_PRD;
int16 dab_phs = 0;

void DabFreq_INC()
{
    if(dab_prd > 1000)
        dab_prd-=10;
}

void DabFreq_DEC()
{
    if(dab_prd < 10000)
        dab_prd+=10;
}

void DabPhs_INC()
{
    if(dab_phs > -1000)
        dab_phs--;
}

void DabPhs_DEC()
{
    if(dab_phs < 1000)
        dab_phs++;
}

void DabPhs_SET(int16 arg)
{
    if((dab_phs > -1000) && (dab_phs < 1000))
        dab_phs = arg;
}

void Dab_Update()
{
    EPwm1Regs.TBPRD = dab_prd;
    EPwm2Regs.TBPRD = dab_prd;
    EPwm3Regs.TBPRD = dab_prd;

    Uint16 cmp = dab_prd >> 1;

    EPwm2Regs.CMPA.bit.CMPA = cmp;
    EPwm3Regs.CMPA.bit.CMPA = cmp;

    if(dab_phs >= 0)
    {
        EPwm3Regs.TBPHS.bit.TBPHS = dab_phs;
        EPwm3Regs.TBCTL.bit.PHSDIR = 0;
    }
    else
    {
        EPwm3Regs.TBPHS.bit.TBPHS = -dab_phs;
        EPwm3Regs.TBCTL.bit.PHSDIR = 1;
    }
}

