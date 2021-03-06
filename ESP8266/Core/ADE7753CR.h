/*
  ADE7753CR.h - Library for use with ADE7753.
  Created by Cristian F. Ritter, Octuber 18, 2020.
  Released into the public domain.
*/
#ifndef ADE7753CR_H
  #define ADE7753CR_H

  #include "Arduino.h"
  #include "SPI.h"
  
  //------------------------
  // Lista de registradores 
  //------------------------
  /*This read-only register contains the sampled waveform
  data from either Channel 1, Channel 2, or the active power signal. The data
  source and the length of the waveform registers are selected by data
  Bits 14 and 13 in the mode register */
  #define WAVEFORM_S 0x01   
  /*Active Energy Register. Active power is accumulated (integrated) over time
  in this 24-bit, read-only register*/
  #define AENERGY_S 0x02    
  /*Same as the active energy register except that the register is reset to 0
  following a read operation.*/ 
  #define RAENERGY_S 0x03   
  /*Line Accumulation Active Energy Register. The instantaneous active power
  is accumulated in this read-only register over the LINECYC number of half
  line cycles.*/
  #define LAENERGY_S 0x04   
  /*Apparent Energy Register. Apparent power is accumulated over time in this
  read-only register. */
  #define VAENERGY_U 0x05   
  /*Same as the VAENERGY register except that the register is reset to 0
  following a read operation.*/ 
  #define RVAENERGY_U 0x06  
  /*Line Accumulation Apparent Energy Register. The instantaneous real power
  is accumulated in this read-only register over the LINECYC number of half
  line cycles. */
  #define LVAENERGY_U 0x07  
  /*Line Accumulation Reactive Energy Register. The instantaneous reactive
  power is accumulated in this read-only register over the LINECYC number
  of half line cycles.*/ 
  #define LVARENERGY_S 0x08 
  /*Mode Register. This is a 16-bit register through which most of the ADE7753
  functionality is accessed. Signal sample rates, filter enabling, and
  calibration modes are selected by writing to this register. The contents can
  be read at any time*/
  #define MODE_U 0x09       
  /*Interrupt Enable Register. ADE7753 interrupts can be deactivated at any time
  by setting the corresponding bit in this 16- bit enable register to Logic 0.
  The status register continues to register an interrupt event even if disabled.
  However, the IRQ output is not activated*/
  #define IRQEN_U 0x0A      
  /*Interrupt Status Register. This is an 16-bit read-only register. The status
  register contains information regarding the source of ADE7753
  interrupts*/
  #define STATUS_U 0x0B     
  /*Same as the interrupt status register except that the register contents are
  reset to 0 (all flags cleared) after a read operation. */
  #define RSTSTATUS_U 0x0C  
  /* Channel 1 Offset Adjust. Bit 6 is not used. Writing to Bits 0 to 5 allows
  offsets on Channel 1 to be removed—see the Analog Inputs and CH1OS
  Register (0x0D) sections. Writing a Logic 1 to the MSB of this register
  enables the digital integrator on Channel 1, a Logic 0 disables the
  integrator. The default value of this bit is 0. */
  #define CH1OS_SS 0x0D      
  /* Channel 2 Offset Adjust. Bits 6 and 7 are not used. Writing to Bits 0 to 5 of
  this register allows any offsets on Channel 2 to be removed—see the
  Analog Inputs section. Note that the CH2OS register is inverted. To apply a
  positive offset, a negative number is written to this register. 
  */
  #define CH2OS_SS 0x0E
  /* PGA Gain Adjust. This 8-bit register is used to adjust the gain selection for
  the PGA in Channels 1 and 2.
  */
  #define GAIN_U 0x0F
  /* Phase Calibration Register. The phase relationship between Channel 1 and
  2 can be adjusted by writing to this 6-bit register. The valid content of this
  twos compliment register is between 0x1D to 0x21. At a line frequency of
  60 Hz, this is a range from –2.06° to +0.7°
  */
  #define PHCAL_S 0x10
  /* Active Power Offset Correction. This 16-bit register allows small offsets in
  the active power calculation to be removed—see the Active Power
  Calculation section. 
  */
  #define APOS_S 0x11
  /* Power Gain Adjust. This is a 12-bit register. The active power calculation can
  be calibrated by writing to this register. The calibration range is ±50% of
  the nominal full-scale active power. The resolution of the gain adjust is
  0.0244%/LSB
  */
  #define WGAIN_S 0x12
  /* Active Energy Divider Register. The internal active energy register is divided
  by the value of this register before being stored in the AENERGY register. 
  */
  #define WDIV_U 0x13
  /* CF Frequency Divider Numerator Register. The output frequency on the CF
  pin is adjusted by writing to this 12-bit read/write register
  */
  #define CFNUM_U 0x14
  /* CF Frequency Divider Denominator Register. The output frequency on the
  CF pin is adjusted by writing to this 12-bit read/write register
  */
  #define CFDEN_U 0x15
  /* Channel 1 RMS Value (Current Channel). 
  */
  #define IRMS_U 0x16
  /* Channel 2 RMS Value (Voltage Channel). 
  */
  #define VRMS_U 0x17
  /* Channel 1 RMS Offset Correction Register. 
  */
  #define IRMSOS_S 0x18
  /* Channel 2 RMS Offset Correction Register. 
  */
  #define VRMSOS_S 0x19
  /* Apparent Gain Register. Apparent power calculation can be calibrated by
  writing to this register. The calibration range is 50% of the nominal fullscale real power. The resolution of the gain adjust is 0.02444%/LSB. 
  */
  #define VAGAIN_S 0x1A
  /* Apparent Energy Divider Register. The internal apparent energy register is
  divided by the value of this register before being stored in the VAENERGY
  register. 
  */
  #define VADIV_U 0x1B
  /* Line Cycle Energy Accumulation Mode Line-Cycle Register. This 16-bit
  register is used during line cycle energy accumulation mode to set the
  number of half line cycles for energy accumulation
  */
  #define LINECYC_U 0x1C
  /* Zero-Crossing Timeout. If no zero crossings are detected on Channel 2
  within a time period specified by this 12-bit register, the interrupt request
  line (IRQ) is activated
  */
  #define ZXTOUT_U 0x1D
  /* Sag Line Cycle Register. This 8-bit register specifies the number of
  consecutive line cycles the signal on Channel 2 must be below SAGLVL
  before the SAG output is activated
  */
  #define SAGCYC_U 0x1E
  /* Sag Voltage Level. An 8-bit write to this register determines at what peak
  signal level on Channel 2 the SAG pin becomes active. The signal must
  remain low for the number of cycles specified in the SAGCYC register
  before the SAG pin is activated
  */
  #define SAGLVL_U 0x1F
  /* Channel 1 Peak Level Threshold (Current Channel). This register sets the
  level of the current peak detection. If the Channel 1 input exceeds this
  level, the PKI flag in the status register is set. 
  */
  #define IPKLVL_U 0x20
  /* Channel 2 Peak Level Threshold (Voltage Channel). This register sets the
  level of the voltage peak detection. If the Channel 2 input exceeds this
  level, the PKV flag in the status register is set. 
  */
  #define VPKLVL_U 0x21
  /* Channel 1 Peak Register. The maximum input value of the current channel
  since the last read of the register is stored in this register. 
  */
  #define IPEAK_U 0x22
  /* Same as Channel 1 Peak Register except that the register contents are reset
  to 0 after read.
  */
  #define RSTIPEAK_U 0x23
  /* Channel 2 Peak Register. The maximum input value of the voltage channel
  since the last read of the register is stored in this register. 
  */
  #define VPEAK_U 0x24
  /* Same as Channel 2 Peak Register except that the register contents are reset
  to 0 after a read. 
  */
  #define RSTVPEAK_U 0x25
  /* Temperature Register. This is an 8-bit register which contains the result of
  the latest temperature conversion
  */
  #define TEMP_S 0x26
  /* Period of the Channel 2 (Voltage Channel) Input Estimated by ZeroCrossing Processing. The MSB of this register is always zero. 
  */
  #define PERIOD_U 0x27
  /* Checksum Register. This 6-bit read-only register is equal to the sum of all
  the ones in the previous read
  */
  #define CHKSUM_U 0x3E 
  /* Die Revision Register. This 8-bit read-only register contains the revision
  number of the silicon. 
  */ 
#define DIEREV_U 0x3F
  
  //---------------------
  //MODE REGISTERS 16bits
  //---------------------

  #define DISHPF   0b0000000000000001 // HPF (high-pass filter) in Channel 1 is disabled when this bit is set
  #define DISLPF2  0b0000000000000010 // LPF (low-pass filter) after the multiplier (LPF2) is disabled when this bit is set. 
  #define DISCF    0b0000000000000100 // Frequency output CF is disabled when this bit is set
  #define DISSAG   0b0000000000001000 // Line voltage sag detection is disabled when this bit is set. 
  /* By setting this bit to Logic 1, both ADE7753 A/D converters can be turned off.
  In normal operation, this bit should be left at Logic 0. All digital functionality can be
  stopped by suspending the clock signal at CLKIN pin. 
  */
  #define ASUSPEND 0b0000000000010000 
  /* Temperature conversion starts when this bit is set to 1. This bit is automatically reset to 0
  when the temperature conversion is finished. 
  */
  #define TEMPSEL  0b0000000000100000
  /* Software Chip Reset. A data transfer should not take place to the ADE7753 for at least
  18 μs after a software reset. 
  */
  #define SWRST    0b0000000001000000
  #define CYCMODE  0b0000000010000000 // Setting this bit to Logic 1 places the chip into line cycle energy accumulation mode. 
  #define DISCH1   0b0000000100000000 // ADC 1 (Channel 1) inputs are internally shorted together. 
  #define DISCH2   0b0000001000000000 // ADC 2 (Channel 2) inputs are internally shorted together. 
  /* By setting this bit to Logic 1 the analog inputs V2P and V2N are connected to ADC 1 and
  the analog inputs V1P and V1N are connected to ADC 2. 
  */
  #define SWAP     0b0000010000000000
  /* These bits are used to select the waveform register update rate.
      |  DTRT1 |DTRT0  |  Update Rate              |
      |  0     | 0     |  27.9 kSPS (CLKIN/128)    |
      |  0     | 1     |  14 kSPS (CLKIN/256)      |
      |  1     | 0     |   7 kSPS (CLKIN/512)      |
      |  1     | 1     |   3.5 kSPS (CLKIN/1024)   | 
  */
  #define DTRT10      0b0001100000000000
  #define DTRT10_00   0b0000000000000000 //27.9 kSPS (CLKIN/128)
  #define DTRT10_01   0b0000100000000000 //14 kSPS (CLKIN/256)
  #define DTRT10_10   0b0001000000000000 // 7 kSPS (CLKIN/512)
  #define DTRT10_11   0b0001100000000000 //3.5 kSPS (CLKIN/1024)

  /* These bits are used to select the source of the sampled data for the waveform register.
      | WAVSEL1,0  | Length | Source
      | 0          | 0      | 24 bits active power signal (output of LPF2)
      | 0          | 1      | Reserved
      | 1          | 0      | 24 bits Channel 1
      | 1          | 1      | 24 bits Channel 2 
  */
  #define WAVSEL10    0b0110000000000000
  #define WAVSEL10_00 0b0000000000000000
  #define WAVSEL10_10 0b0100000000000000
  #define WAVSEL10_11 0b0110000000000000

  #define POAM     0b1000000000000000 //Writing Logic 1 to this bit allows only positive active power to be accumulated in the ADE7753.

  //---------------------------
  //INTERRUPTS REGISTERS 16bits
  //---------------------------

  #define AEHF   0b0000000000000001 // Indicates that an interrupt occurred because the active energy register, AENERGY, is more than half full. 
  #define SAG    0b0000000000000010 // Indicates that an interrupt was caused by a SAG on the line voltage. 
  #define CYCEND 0b0000000000000100
  /* Indicates the end of energy accumulation over an integer number of half line cycles as defined by
  the content of the LINECYC register
  */
  #define WSMP   0b0000000000001000 // Indicates that new data is present in the waveform register. 
  #define ZX     0b0000000000010000 // This status bit is set to Logic 0 on the rising and falling edge of the the voltage waveform. 
  #define TEMP   0b0000000000100000 // Indicates that a temperature conversion result is available in the temperature register. 
  /* Indicates the end of a reset (for both software or hardware reset). The corresponding enable bit has no
  function in the interrupt enable register, i.e., this status bit is set at the end of a reset, but it cannot
  be enabled to cause an interrupt. 
  */
  #define RESET  0b0000000001000000
  #define AEOF   0b0000000010000000 // Indicates that the active energy register has overflowed. 
  #define PKV    0b0000000100000000 // Indicates that waveform sample from Channel 2 has exceeded the VPKLVL value. 
  #define PKI    0b0000001000000000 // Indicates that waveform sample from Channel 1 has exceeded the IPKLVL value. 
  #define VAEHF  0b0000010000000000 // Indicates that an interrupt occurred because the active energy register, VAENERGY, is more than half full. 
  #define VAEOF  0b0000100000000000 // Indicates that the apparent energy register has overflowed. 
  #define ZXTO   0b0001000000000000 
  /* Indicates that an interrupt was caused by a missing zero crossing on the line voltage for the specified
  number of line cycles—see the Zero-Crossing Timeout section. 
  */
  #define PPOS   0b0010000000000000 // Indicates that the power has gone from negative to positive.
  #define PNEG   0b0100000000000000 // Indicates that the power has gone from positive to negative. 

  //CH1OS REGISTERS 8bits
  #define CH1OFFSET     0b00111111
  #define CH1INTEGRATOR 0b10000000

  //CH1OS REGISTERS 8bits
  #define CH2OFFSET     0b00111111


  class ADE7753
  {
    public:
      typedef struct {
        float voltage = 0;
        float current = 0;
        float frequency = 0;
        float FP = 0;
        short active_power = 0;
        short reactive_power = 0;
        short aparent_power = 0;
        short active_energy = 0;
        short reactive_energy = 0;
        short apparent_energy = 0;
        unsigned long timestamp = 0;
        byte events = 0;
        byte temp;
      } Measurement;

      void Init(int CSpin);
      void Closes();
      void EnableHPF();
      void DisableHPF();  
      void EnableLPF2();
      void DisableLPF2();
      void EnableCF();
      void DisableCF();
      void EnableSAGDetection(); 
      void DisableSAGDetection();   
      void DisableADConverters();
      void EnableADConverters();
      void StartTemperatureMeasurement();
      void SoftReset();
      unsigned long EnableAccumulationMode();
      void DisableAccumulationMode();
      void DisableCH1(); //Puts inputs internally in short
      void DisableCH2();
      void EnableCH1();
      void EnableCH2();
      void DisableSwap();
      void EnableSwap();

      /*
      0 -> 27.9 kSPS [Default]
      1 -> 14   kSPS
      2 ->  7   kSPS
      3 ->  3.5 kSPS
      */
      void SelectWaveformDataRate(int datarate);  
      /*
      0 -> 24 bits active power signal (output of LPF2) [Default]
      2 -> 24 bits Channel 1
      3 -> 24 bits Channel 2
      */
      void SelectWaveformDataSource(int datasource); 

      void EnableOnlyPositive();
      void DisableOnlyPositive();
      void EnableIRQActiveEnergyHalfFull();
      void DisableIRQActiveEnergyHalfFull();
      void EnableIRQSAG();
      void DisableIRQSAG();
      void EnableIRQCycleEnergyAccumulationEnd();
      void DisableIRQCycleEnergyAccumulationEnd();
      void EnableIRQNewWaveformData();
      void DisableIRQNewWaveformData();
      void EnableIRQZeroCrossing();
      void DisableIRQZeroCrossing();
      void EnableIRQTemperatureResults();
      void DisableIRQTemperatureResults();
      void EnableIRQActiveEnergyOverflow();
      void DisableIRQActiveEnergyOverflow();
      void EnableIRQCH2VlvlPeek();
      void DisableIRQCH2VlvlPeek();
      void EnableIRQCH1IlvlPeek();
      void DisableIRQCH1IlvlPeek();
      void EnableIRQAparentEnergyHalfFull();
      void DisableIRQAparentEnergyHalfFull();
      void EnableIRQAparentEnergyOverflow();
      void DisableIRQAparentEnergyOverflow();
      void EnableIRQZeroCrossingTimeout();
      void DisableIRQZeroCrossingTimeout();
      void EnablesIRQPowerChangeToPos();
      void DisablesIRQPowerChangeToPos();
      void EnablesIRQPowerChangeToNeg();
      void DisablesIRQPowerChangeToNeg();
      bool CheckandResetActiveEnergyHalfFull();
      bool CheckandResetSAG();
      bool CheckandResetCycleEnergyAccumulationEnd();
      bool CheckandResetNewWaveformData();
      bool CheckandResetZeroCrossing();  //returns
      bool CheckandResetZeroCrossingTimeout();  //returns 
      bool CheckZeroCrossingError();
      bool CheckandResetTemperatureResults();
      bool CheckandResetActiveEnergyOverflow();
      bool CheckandResetCH2VlvlPeek();
      bool CheckandResetCH1IlvlPeek();
      bool CheckandResetAparentEnergyHalfFull();
      bool CheckandResetAparentEnergyOverflow();
      bool CheckandResetPowerChangeToPos();
      bool CheckandResetPowerChangeToNeg();
      bool CheckResetEnds();
      
      long SetSAGLVL(long value);
      long SetSAGCYC(long value);
      long SetIPKLVL(long value);
      long SetVPKLVL(long value);
      long SetVRMSOS(long value);
      long SetIRMSOS(long value);
      unsigned long SetLINECYC(unsigned long value);

      unsigned long ResetStatusReg();     
      
      unsigned long ReadModeReg();
      unsigned long ReadStatusReg();
      float ReadVRMS();
      float ReadIRMS();
      float ReadPERIOD(int CLKIN);
      float ReadActiveEnergy();
      float ReadApparentEnergy();
      float ReadReactiveEnergy();
      float ReadandResetApparentEnergy();
      float ReadandResetActiveEnergy();
      void ReadFP(int half_line_cycles, float* power_factor, short* reactive_power);
      void DisplayBufferUpdate(Measurement* data, char dest, int view = 1, bool next = 0);
      int GetDisplayPosition();
      byte GetTemperature();
      
    private:
      int _CSpin;
      void Enable();
      void Disable();
      void SetBits(char reg, unsigned long value);
      void UnsetBits(char reg, unsigned long value);
      unsigned long Read8(char reg);
      unsigned long Read16(char reg);
      unsigned long Read24(char reg);
      void Write8(char reg, unsigned long value);
      void Write16(char reg, unsigned long value);
      int WaitZeroCross();
      long Signed24toSigned32(long value);
      int display_position = 1;
  };
#endif
