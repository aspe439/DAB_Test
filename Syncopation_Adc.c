/*
 * MSST_ADC.c
 *
 *  Created on: Dec 14, 2016
 *      Author: Yang Lei
 */

#include "F28x_Project.h"
#include "Syncopation_Data.h"
#include "Syncopation_Pwm.h"

#define SAMPLE_WINDOW 49
#define ADC_TRIG_SELECT  5  // EPWM1 SOCA

void Adc_A_Init();
void Adc_B_Init();
void Adc_C_Init();
void Adc_D_Init();

interrupt void ControlLoop(void);

void AdcInit()
{
    Adc_A_Init();
    Adc_B_Init();
    Adc_C_Init();
    Adc_D_Init();
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // ADC-A interrupt 1
    EALLOW;
    PieVectTable.ADCA1_INT = &ControlLoop;
    AnalogSubsysRegs.TSNSCTL.bit.ENABLE = 1;
    EDIS;
}

void Adc_A_Init()
{
    volatile struct ADC_REGS *adc_module = &AdcaRegs;
    EALLOW;
    adc_module->ADCCTL1.bit.ADCPWDNZ = 1;  // ADC-A power up
    adc_module->ADCCTL1.bit.INTPULSEPOS = 1; // Interrupt position at the end of conversion
    adc_module->ADCCTL2.bit.PRESCALE = 2; // ADC Clock = SYSCLK / 2;
    adc_module->ADCCTL2.bit.RESOLUTION = 0; // 12-bit resolution
    adc_module->ADCCTL2.bit.SIGNALMODE = 0; // Single-ended signal mode
    adc_module->ADCBURSTCTL.bit.BURSTEN = 0; // Disable burst mode
    adc_module->ADCINTFLGCLR.all = 15; // Clear all the interrupt flags
    adc_module->ADCINTOVFCLR.all = 15; // Clear all the interrupt overflow flags
    adc_module->ADCINTSEL1N2.all = 0x0023; //  Enable ADCINT1 and it is triggered by EOC3, disable ADCINT2
    adc_module->ADCINTSEL3N4.all = 0; //  Disable ADCINT3 and ADCINT4
    adc_module->ADCSOCPRICTL.all = 0; // Round robin control. Conversion starts from SOC0.
    adc_module->ADCINTSOCSEL1.all = 0; // ADC interrupt doesn't trigger any SOC
    adc_module->ADCINTSOCSEL2.all = 0; // ADC interrupt doesn't trigger any SOC

    adc_module->ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC0CTL.bit.CHSEL = 0; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC1CTL.bit.CHSEL = 1; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC1CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-2 software start
    adc_module->ADCSOC2CTL.bit.CHSEL = 2; // SOC-2 convert channel 4, which is ADC_A4 pin
    adc_module->ADCSOC2CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-3 software start
    adc_module->ADCSOC3CTL.bit.CHSEL = 3; // SOC-3 convert channel 5, which is ADC_A5 pin
    adc_module->ADCSOC3CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC4CTL.bit.CHSEL = 4; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC4CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC5CTL.bit.CHSEL = 5; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC5CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC6CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-2 software start
    adc_module->ADCSOC6CTL.bit.CHSEL = 13; // SOC-2 convert channel 4, which is ADC_A4 pin
    adc_module->ADCSOC6CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCCTL1.bit.INTPULSEPOS = 1;    // ADCINT1 trips after AdcResults latch
    adc_module->ADCINTSEL1N2.bit.INT1E     = 1;    // Enabled ADCINT1
    adc_module->ADCINTSEL1N2.bit.INT1CONT  = 0;    // Disable ADCINT1 Continuous mode
    adc_module->ADCINTSEL1N2.bit.INT1SEL   = 6;    // setup EOC3 to trigger ADCINT1 to fire
    EDIS;
}

void Adc_B_Init()
{
    volatile struct ADC_REGS *adc_module = &AdcbRegs;
    EALLOW;
    adc_module->ADCCTL1.bit.ADCPWDNZ = 1;  // ADC-A power up
    adc_module->ADCCTL1.bit.INTPULSEPOS = 1; // Interrupt position at the end of conversion
    adc_module->ADCCTL2.bit.PRESCALE = 2; // ADC Clock = SYSCLK / 2;
    adc_module->ADCCTL2.bit.RESOLUTION = 0; // 12-bit resolution
    adc_module->ADCCTL2.bit.SIGNALMODE = 0; // Single-ended signal mode
    adc_module->ADCBURSTCTL.bit.BURSTEN = 0; // Disable burst mode
    adc_module->ADCINTFLGCLR.all = 15; // Clear all the interrupt flags
    adc_module->ADCINTOVFCLR.all = 15; // Clear all the interrupt overflow flags
    adc_module->ADCINTSEL1N2.all = 0x0003; //  Enable ADCINT1 and it is triggered by EOC3, disable ADCINT2
    adc_module->ADCINTSEL3N4.all = 0; //  Disable ADCINT3 and ADCINT4
    adc_module->ADCSOCPRICTL.all = 0; // Round robin control. Conversion starts from SOC0.
    adc_module->ADCINTSOCSEL1.all = 0; // ADC interrupt doesn't trigger any SOC
    adc_module->ADCINTSOCSEL2.all = 0; // ADC interrupt doesn't trigger any SOC

    adc_module->ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC0CTL.bit.CHSEL = 0; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC1CTL.bit.CHSEL = 1; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC1CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-2 software start
    adc_module->ADCSOC2CTL.bit.CHSEL = 2; // SOC-2 convert channel 4, which is ADC_A4 pin
    adc_module->ADCSOC2CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-3 software start
    adc_module->ADCSOC3CTL.bit.CHSEL = 3; // SOC-3 convert channel 5, which is ADC_A5 pin
    adc_module->ADCSOC3CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC4CTL.bit.CHSEL = 4; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC4CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC5CTL.bit.CHSEL = 5; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC5CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    EDIS;
}
void Adc_C_Init()
{
    volatile struct ADC_REGS *adc_module = &AdccRegs;
    EALLOW;
    adc_module->ADCCTL1.bit.ADCPWDNZ = 1;  // ADC-A power up
    adc_module->ADCCTL1.bit.INTPULSEPOS = 1; // Interrupt position at the end of conversion
    adc_module->ADCCTL2.bit.PRESCALE = 2; // ADC Clock = SYSCLK / 2;
    adc_module->ADCCTL2.bit.RESOLUTION = 0; // 12-bit resolution
    adc_module->ADCCTL2.bit.SIGNALMODE = 0; // Single-ended signal mode
    adc_module->ADCBURSTCTL.bit.BURSTEN = 0; // Disable burst mode
    adc_module->ADCINTFLGCLR.all = 15; // Clear all the interrupt flags
    adc_module->ADCINTOVFCLR.all = 15; // Clear all the interrupt overflow flags
    adc_module->ADCINTSEL1N2.all = 0x0003; //  Enable ADCINT1 and it is triggered by EOC3, disable ADCINT2
    adc_module->ADCINTSEL3N4.all = 0; //  Disable ADCINT3 and ADCINT4
    adc_module->ADCSOCPRICTL.all = 0; // Round robin control. Conversion starts from SOC0.
    adc_module->ADCINTSOCSEL1.all = 0; // ADC interrupt doesn't trigger any SOC
    adc_module->ADCINTSOCSEL2.all = 0; // ADC interrupt doesn't trigger any SOC

    adc_module->ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC0CTL.bit.CHSEL = 14; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC1CTL.bit.CHSEL = 15; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC1CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-2 software start
    adc_module->ADCSOC2CTL.bit.CHSEL = 2; // SOC-2 convert channel 4, which is ADC_A4 pin
    adc_module->ADCSOC2CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-3 software start
    adc_module->ADCSOC3CTL.bit.CHSEL = 3; // SOC-3 convert channel 5, which is ADC_A5 pin
    adc_module->ADCSOC3CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC4CTL.bit.CHSEL = 4; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC4CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC5CTL.bit.CHSEL = 5; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC5CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    EDIS;
}

void Adc_D_Init()
{
    volatile struct ADC_REGS *adc_module = &AdcdRegs;
    EALLOW;
    adc_module->ADCCTL1.bit.ADCPWDNZ = 1;  // ADC-A power up
    adc_module->ADCCTL1.bit.INTPULSEPOS = 1; // Interrupt position at the end of conversion
    adc_module->ADCCTL2.bit.PRESCALE = 2; // ADC Clock = SYSCLK / 2;
    adc_module->ADCCTL2.bit.RESOLUTION = 0; // 12-bit resolution
    adc_module->ADCCTL2.bit.SIGNALMODE = 0; // Single-ended signal mode
    adc_module->ADCBURSTCTL.bit.BURSTEN = 0; // Disable burst mode
    adc_module->ADCINTFLGCLR.all = 15; // Clear all the interrupt flags
    adc_module->ADCINTOVFCLR.all = 15; // Clear all the interrupt overflow flags
    adc_module->ADCINTSEL1N2.all = 0x0003; //  Enable ADCINT1 and it is triggered by EOC3, disable ADCINT2
    adc_module->ADCINTSEL3N4.all = 0; //  Disable ADCINT3 and ADCINT4
    adc_module->ADCSOCPRICTL.all = 0; // Round robin control. Conversion starts from SOC0.
    adc_module->ADCINTSOCSEL1.all = 0; // ADC interrupt doesn't trigger any SOC
    adc_module->ADCINTSOCSEL2.all = 0; // ADC interrupt doesn't trigger any SOC

    adc_module->ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC0CTL.bit.CHSEL = 0; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC1CTL.bit.CHSEL = 1; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC1CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-2 software start
    adc_module->ADCSOC2CTL.bit.CHSEL = 2; // SOC-2 convert channel 4, which is ADC_A4 pin
    adc_module->ADCSOC2CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-3 software start
    adc_module->ADCSOC3CTL.bit.CHSEL = 3; // SOC-3 convert channel 5, which is ADC_A5 pin
    adc_module->ADCSOC3CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-0 software start
    adc_module->ADCSOC4CTL.bit.CHSEL = 4; // SOC-0 convert channel 2, which is ADC_A2 pin
    adc_module->ADCSOC4CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    adc_module->ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SELECT; // SOC-1 software start
    adc_module->ADCSOC5CTL.bit.CHSEL = 5; // SOC-1 convert channel 3, which is ADC_A3 pin
    adc_module->ADCSOC5CTL.bit.ACQPS = SAMPLE_WINDOW; // Sample window is SAMPLE_WINDOW+1 clock cycles

    EDIS;
}

#pragma CODE_SECTION(ControlLoop, ".TI.ramfunc");

Uint16 AdcResult[24];
Uint16 TempSensor;

Uint16 vac_reading = 0;
Uint16 iac_reading = 0;

extern float SinglePhasePLL(float Vac, float *Freq, float *Vac_amp);

float Vac,Iac;
float Freq, Vac_amp;
float Theta;

Uint16 index = 0;
Uint16 state = 0;

Uint16 fault_status = 0;

interrupt void ControlLoop(void)
{
    AdcResult[0]  = AdcaResultRegs.ADCRESULT0;
    AdcResult[1]  = AdcaResultRegs.ADCRESULT1;
    AdcResult[2]  = AdcaResultRegs.ADCRESULT2;
    AdcResult[3]  = AdcaResultRegs.ADCRESULT3;
    AdcResult[4]  = AdcaResultRegs.ADCRESULT4;
    AdcResult[5]  = AdcaResultRegs.ADCRESULT5;
    TempSensor    = AdcaResultRegs.ADCRESULT6;
    AdcResult[6]  = AdcbResultRegs.ADCRESULT0;
    AdcResult[7]  = AdcbResultRegs.ADCRESULT1;
    AdcResult[8]  = AdcbResultRegs.ADCRESULT2;
    AdcResult[9]  = AdcbResultRegs.ADCRESULT3;
    AdcResult[10] = AdcbResultRegs.ADCRESULT4;
    AdcResult[11] = AdcbResultRegs.ADCRESULT5;
    AdcResult[12] = AdccResultRegs.ADCRESULT0;
    AdcResult[13] = AdccResultRegs.ADCRESULT1;
    AdcResult[14] = AdccResultRegs.ADCRESULT2;
    AdcResult[15] = AdccResultRegs.ADCRESULT3;
    AdcResult[16] = AdccResultRegs.ADCRESULT4;
    AdcResult[17] = AdccResultRegs.ADCRESULT5;
    AdcResult[18] = AdcdResultRegs.ADCRESULT0;
    AdcResult[19] = AdcdResultRegs.ADCRESULT1;
    AdcResult[20] = AdcdResultRegs.ADCRESULT2;
    AdcResult[21] = AdcdResultRegs.ADCRESULT3;
    AdcResult[22] = AdcdResultRegs.ADCRESULT4;
    AdcResult[23] = AdcdResultRegs.ADCRESULT5;

    Dab_Update();


    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;	//Clear ADCINT1 flag reinitialize for next SOC
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

interrupt void Xint1Isr()
{
//    fault_status = 1;
    Dab_DIS();
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

