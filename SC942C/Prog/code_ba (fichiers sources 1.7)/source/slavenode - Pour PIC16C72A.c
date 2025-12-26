//
//--------------------------------------------------------------------
// File: slavnode.c
//
// Written By: Stephen Bowling, Microchip Technology
// for a PIC16C72A device
//
// Version: 1.00
//
// Compiled using HiTech PICC Compiler, V. 7.85
//
// This code implements the slave node network protocol for an I2C slave
// device with the SSP module.
//
// The following files should be included in the MPLAB project:
//
// slavnode.c -- Main source code file
//
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Constant Definitions
//--------------------------------------------------------------------
#define CCP_HBYTE 0x03 // Set Compare timeout to 1msec
#define CCP_LBYTE 0xe8
#define COUNT_10MSEC 10 // Number of Compare timeouts for 10ms
#define COUNT_100MSEC10 // Number of Compare timeouts for 100ms
#define COUNT_1000MSEC10 // Number of Compare timeouts for 1000ms
#define TEMP_OFFSET 58 // Offset value for temperature table
#define NODE_ADDR 0x18 // I2C address of this node
#define ADRES ADRESH // Redefine for 10-bit A/D
#define ON 1
#define TRUE 1
#define OFF 0
#define FALSE 0
//--------------------------------------------------------------------
// Buffer Length Definitions
//--------------------------------------------------------------------
#define RX_BUF_LEN 8 // Length of receive buffer
#define SENS_BUF_LEN 12 // Length of buffer for sensor data.
#define CMD_BUF_LEN 4 // Length of buffer for command data.
//--------------------------------------------------------------------
// Receive Buffer Index Values
//--------------------------------------------------------------------
#define SLAVE_ADDR 0 //
#define DATA_OFFS 2 //
#define DATA_LEN 1 //
#define RX_DATA 3 //
//--------------------------------------------------------------------
// Sensor Buffer Index Values
//--------------------------------------------------------------------
#define COMM_STAT 0 // Communication status byte
#define SENSOR_DATA 3 // Start index for sensor data
#define STATUS0 1 // Sensor out-of-range status bits
#define STATUS1 2 // "
#define TEMP0 3 // Temperature (A/D CH4)
#define TACH0 4 // Fan tachometer #1
#define ADRES0 5 // A/D CH0
#define ADRES1 6 // A/D CH1
#define ADRES2 7 // A/D CH2
#define ADRES3 8 // A/D CH3
#define TACH1 9 // Fan tachometer #2
#define TACH2 10 // Fan tachometer #3
#define TACH3 11 // Fan tachometer #4
//--------------------------------------------------------------------
// Command Buffer Index Values
//--------------------------------------------------------------------
#define CMD_BYTE0 0
#define CMD_BYTE1 1
#define CMD_BYTE2 2
#define CMD_BYTE3 3
//--------------------------------------------------------------------
// Pin Definitions
//--------------------------------------------------------------------
#define TACH_IN0 0x10 // Mask values for fan tach
#define TACH_IN1 0x20 // input pins
#define TACH_IN2 0x40
#define TACH_IN3 0x80
#define LED_0 RB0 // Pin definitions for general
#define LED_1 RB1 // purpose I/O pins
#define FAN_CNTRL RC2

//--------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------
#include <pic.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//--------------------------------------------------------------------
// Function Prototypes
//--------------------------------------------------------------------
void Setup(void);
interrupt void ISR_Handler(void);
void WriteI2C(char data);
char ReadI2C(void);
void SSP_Handler(void);
void AD_Handler(void);
void CCP2_Handler(void);
//--------------------------------------------------------------------
// Variable declarations
//--------------------------------------------------------------------
unsigned charCount_10m, // Holds number of compare timeouts
        Count_100m, // Holds number of compare timeouts
        Count_1000m, // "
        Count_Tach0, // Holds number of accumulated pulses
        Count_Tach1, // for fan speed measurements.
        Count_Tach2, //
        Count_Tach3; // "
char RXBuffer[RX_BUF_LEN]; // Holds incoming bytes from master
                            // device.
char CmdBuf[CMD_BUF_LEN]; //
char RXChecksum; //
unsigned char
    RXBufferIndex, // Index to received bytes.
    RXByteCount, // Number of bytes received
    SensBufIndex, // Index to sensor data table
    CmdBufIndex, //
    PORTBold, // Holds previous value of PORTB
    temp;
union INTVAL
{
char b[2];
int i;
};
union INTVAL TXChecksum; // Holds checksum of bytes sent to
// master
union SENSORBUF // Holds sensor data and other bytes
{ // to be sent to master.
struct{
    unsigned chkfail:1;
    unsigned rxerror:1;
    unsigned ovflw:1;
    unsigned sspov:1;
    unsigned bit4:1;
    unsigned bit5:1;
    unsigned bit6:1;
    unsigned r_w:1;
} comm_stat ;
unsigned charb[SENS_BUF_LEN];
} SensorBuf ;
struct{ // Flags for program
    unsigned msec10:1; // 10msec time flag
    unsigned msec100:1; // 100msec time flag
    unsigned msec1000:1; // 1000msec time flag
    unsigned bit3:1;
    unsigned wdtdis:1; // Watchdog Timer disable flag
    unsigned bit5:1;
    unsigned bit6:1;
    unsigned bit7:1;
} stat ;
const char temperature[] = {32,32,32,32,33,33,34,34,35,35,35,36,
                            36,37,37,37,38,38,39,39,40,41,41,42,
                            43,43,44,44,45,45,46,46,47,48,49,50,
                            51,52,53,54,55,55,56,57,58,59,59,60,
                            61,61,62,63,63,63,64,65,66,67,68,68,
                            69,70,71,71,72,72,73,73,74,75,76,77,
                            78,79,80,81,81,82,82,83,84,84,85,86,
                            87,88,89,90,91,91,92,93,94,95,96,97,
                            98,99,99,99 };


//--------------------------------------------------------------------
// Interrupt Code
//--------------------------------------------------------------------
interrupt void ISR_Handler(void)
{
    if(SSPIF)
    {
        LED_0 = ON; // Turn on LED to indicate I2C activity.
        SSPIF = 0; // Clear the interrupt flag.
        SSP_Handler(); // Do I2C communication
    }
    if(CCP2IF)
    {
        CCP2IF = 0; // Clear the interrupt flag.
        CCP2_Handler(); // Do timekeeping and sample tach inputs.
    }
    if(ADIF)
    {
        ADIF = 0; // Clear the interrupt flag.
        AD_Handler(); // Get A/D data ready and change channel.
    }
}
//--------------------------------------------------------------------
// void SSP_Handler(void)
//--------------------------------------------------------------------
void SSP_Handler(void)
{
unsigned char i,j;
    //--------------------------------------------------------------------
    // STATE 1: If this is a WRITE operation and last byte was an ADDRESS
    //--------------------------------------------------------------------
    if(!STAT_DA && !STAT_RW && STAT_BF && STAT_S)
    {
        // Clear WDT and disable clearing in the main program loop. The
        // WDT will be used to reset the device if an I2C message exceeds
        // the expected amount of time.
        CLRWDT();
        stat.wdtdis = 1;
        // Since the address byte was the last byte received, clear
        // the receive buffer and the index. Put the received data
        // in the first buffer location.
        RXBufferIndex = SLAVE_ADDR;
        RXByteCount = 0;
        RXBuffer[RXBufferIndex] = ReadI2C();
        // Initialize the receive checksum.
        RXChecksum = RXBuffer[RXBufferIndex];
        // Increment the buffer index
        RXBufferIndex++;
        // Reset the communication status byte. The rxerror bit remains
        // set until a valid data request has taken place.
        SensorBuf.b[COMM_STAT] = 0x02;
        // Check to make sure an SSP overflow has not occurred.
        if(SSPOV)
        {
            SensorBuf.comm_stat.sspov = 1;
            SSPOV = 0;
        }
    }
    //--------------------------------------------------------------------
    // STATE 2: If this is a WRITE operation and the last byte was DATA
    //--------------------------------------------------------------------
    else if(STAT_DA && !STAT_RW && STAT_BF)
    {
        // Check the number of data bytes received.
        if(RXBufferIndex == RX_BUF_LEN)
        {
            SensorBuf.comm_stat.ovflw = 1;
            RXBufferIndex = RX_BUF_LEN - 1;
        }
        // Check to see if SSP overflow occurred.
        if(SSPOV)
        {
            SensorBuf.comm_stat.sspov = 1;
            SSPOV = 0;
        }
        // Get the incoming byte of data.
        RXBuffer[RXBufferIndex] = ReadI2C();
        // Add the received value to the checksum.
        RXChecksum += RXBuffer[RXBufferIndex];
        // Check to see if the current byte is the DATA_LEN byte. If it is,
        // check the MSb to see if this is a data write or data request.
        if(RXBufferIndex == DATA_LEN)
        {
            if(RXBuffer[DATA_LEN] & 0x80)
            {
                // This will be a data request, so the master should send
                // a total of 4 bytes: SLAVE_ADDR, DATA_LEN, DATA_OFFS,
                // and an 8 bit checksum value.
                // Mask out the R/W bit in the DATA_LEN byte to simplify
                // further calculations.
                RXBuffer[DATA_LEN] &= 0x7f;
                // Set the R/W bit in COMM_STAT byte to indicate a data
                // request.
                SensorBuf.comm_stat.r_w = 1;
                RXByteCount = 3;
            }
            else
            {
                // This will be a data write, so the master should send the
                // four bytes used for a data request, plus the number of
                // bytes specified by the DATA_LEN byte. If the total
                // number of bytes to be written exceeds the slave receive
                // buffer, the error flag needs to be set.
                SensorBuf.comm_stat.r_w = 0;
                RXByteCount = RXBuffer[DATA_LEN] + 3;
                if(RXByteCount > RX_BUF_LEN)
                {
                    SensorBuf.comm_stat.rxerror = 1;
                    SensorBuf.comm_stat.ovflw = 1;
                }
            }
        }
        // If not the DATA_LEN byte, check to see if the current byte
        // is the DATA_OFFS byte.
        else if(RXBufferIndex == DATA_OFFS)
        {
            // If this is a data request command.
            if(SensorBuf.comm_stat.r_w)
            {
                // Is the range of sensor data requested within the limits of the
                // sensor data buffer? If so, set the appropriate flags.
                if (RXBuffer[DATA_LEN] + RXBuffer[DATA_OFFS] > SENS_BUF_LEN - 1)
                {
                    SensorBuf.comm_stat.rxerror = 1;
                    SensorBuf.comm_stat.ovflw = 1;
                }
                else
                {
                    SensorBuf.comm_stat.rxerror = 0;
                    SensorBuf.comm_stat.ovflw = 0;
                }
            }
            // Otherwise, this is a data write command.
            else
            {
                // Is the master requesting to write more bytes than are available
                // in the command buffer?
                if(RXBuffer[DATA_LEN] + RXBuffer[DATA_OFFS] > CMD_BUF_LEN - 1)
                {
                    SensorBuf.comm_stat.rxerror = 1;
                    SensorBuf.comm_stat.ovflw = 1;
                }
                else
                {
                    SensorBuf.comm_stat.rxerror = 0;
                    SensorBuf.comm_stat.ovflw = 0;
                }
            }
        }
        // If the master is doing a data write to the slave, we must check
        // for the end of the data string so we can do the checksum.
        else if(RXBufferIndex == RXByteCount)
        {
            // Is this a data request?
            if(SensorBuf.comm_stat.r_w)
            {
                if(RXChecksum)
                    SensorBuf.comm_stat.chkfail = 1;
                else
                    SensorBuf.comm_stat.chkfail = 0;
            }
            // Was this a data write?
            else
            {
                if(RXChecksum)
                    SensorBuf.comm_stat.chkfail = 1;
                else
                {
                    // Checksum was OK, so copy the data in receive buffer
                    // into the command buffer.
                    for(i=RXBuffer[DATA_OFFS]+3, j = 0;
                        i < (RXBuffer[DATA_LEN] + RXBuffer[DATA_OFFS] + 3);
                        i++,j++)
                    {
                        if(j == CMD_BUF_LEN) j--;
                        CmdBuf[j] = RXBuffer[i];
                    }
                    SensorBuf.comm_stat.chkfail = 0;
                }
            }
        }
        else;

        // Increment the receive buffer index.
        RXBufferIndex++;
    }
    //--------------------------------------------------------------------
    // STATE 3: If this is a READ operation and last byte was an ADDRESS
    //--------------------------------------------------------------------
    else if(!STAT_DA && STAT_RW && !STAT_BF && STAT_S)
    {
        // Clear the buffer index to the sensor data.
        SensBufIndex = 0;
        // Initialize the transmit checksum
        TXChecksum.i = (int)SensorBuf.b[COMM_STAT];
        // Send the communication status byte.
        WriteI2C(SensorBuf.b[COMM_STAT]);
    }
    //--------------------------------------------------------------------
    // STATE 4: If this is a READ operation and the last byte was DATA
    //--------------------------------------------------------------------
    else if(STAT_DA && STAT_RW && !STAT_BF)
    {
        // If we haven?t transmitted all the required data yet,
        // get the next byte out of the TXBuffer and increment
        // the buffer index. Also, add the transmitted byte to
        // the checksum
        if(SensBufIndex < RXBuffer[DATA_LEN])
        {
            WriteI2C(SensorBuf.b[SensBufIndex + RXBuffer[DATA_OFFS]]);
            TXChecksum.i += (int)ReadI2C();
            SensBufIndex++;
        }
        // If all the data bytes have been sent, invert the checksum
        // value and send the first byte.
        else
            if(SensBufIndex == RXBuffer[DATA_LEN])
            {
                TXChecksum.i = ~TXChecksum.i;
                TXChecksum.i++;
                WriteI2C(TXChecksum.b[0]);
                SensBufIndex++;
            }
            // Send the second byte of the checksum value.
            else
                if(SensBufIndex == (RXBuffer[DATA_LEN] + 1))
                {
                    WriteI2C(TXChecksum.b[1]);
                    SensBufIndex++;
                }
                // Otherwise, just send dummy data back to the master.
                else
                {
                    WriteI2C(0x55);
                }
    }
    //--------------------------------------------------------------------
    // STATE 5: A NACK from the master device is used to indicate that a
    // complete transmission has occurred. The clearing of the
    // WDT is reenabled in the main loop at this time.
    //--------------------------------------------------------------------
    else if(STAT_DA && !STAT_RW && !STAT_BF)
    {
        stat.wdtdis = 0;
        CLRWDT();
    }
    else;
}


//--------------------------------------------------------------------
// void CCP2_Handler(void)
//
// At each CCP2 interrupt, the tachometer inputs are sampled to see
// if a pin change occurred since the last interrupt. If so, the count
// value for that tach input is incremented. Count values are also
// maintained to determine when 10ms, 100msec, and 1000msec have
// elapsed.
//--------------------------------------------------------------------
void CCP2_Handler(void)
{
    TMR1L = 0; // Clear Timer1
    TMR1H = 0;
    temp = PORTB; // Get present PORTB value
    PORTBold ^= temp; // XOR to get pin changes
    if(PORTBold & TACH_IN3) Count_Tach3++; // Test each input to see if pin
    if(PORTBold & TACH_IN2) Count_Tach2++; // changed.
    if(PORTBold & TACH_IN1) Count_Tach1++;
    if(PORTBold & TACH_IN0) Count_Tach0++;
    PORTBold = temp; // Store present PORTB value for
    // next sample time.
    Count_10m++; // Increment 10msec count.
    if(Count_10m == COUNT_10MSEC) // Set flag and zero count if
    { // 10msec have elapsed.
        Count_10m = 0;
        Count_100m++;
        stat.msec10 = 1;
    }
    if(Count_100m == COUNT_100MSEC) // Set flag and zero count if
    { // 100msec have elapsed.
        Count_100m = 0;
        Count_1000m++;
        stat.msec100 = 1;
    }
    if(Count_1000m == COUNT_1000MSEC) // Set flag and zero count if
    { // 1000msec have elapsed.
        Count_1000m = 0;
        stat.msec1000 = 1;
    }
}


//--------------------------------------------------------------------
// void AD_Handler(void)
//
// This routine gets the data that is ready in the ADRES register and
// changes the A/D channel to the next source.
//--------------------------------------------------------------------
void AD_Handler(void)
{
    switch(ADCON0 & 0x38) // Get current A/D channel
    {
        case 0x00: SensorBuf.b[ADRES0] = ADRES;
            CHS0 = 1; // Change to CH1
            CHS1 = 0;
            CHS2 = 0;
        break;
        case 0x08: SensorBuf.b[ADRES1] = ADRES;
            CHS0 = 0; // Change to CH2
            CHS1 = 1;
            CHS2 = 0;
        break;
        case 0x10: SensorBuf.b[ADRES2] = ADRES;
            CHS0 = 1; // Change to CH3
            CHS1 = 1;
            CHS2 = 0;
        break;
        case 0x18: SensorBuf.b[ADRES3] = ADRES;
            CHS0 = 0; // Change to CH4
            CHS1 = 0;
            CHS2 = 1;
        break;
        case 0x20:
            if(ADRES < TEMP_OFFSET || ADRES > (100 + TEMP_OFFSET))
                SensorBuf.b[TEMP0] = 0;
            else
                SensorBuf.b[TEMP0] = temperature[ADRES - TEMP_OFFSET];
            CHS0 = 0; // Change to CH0
            CHS1 = 0;
            CHS2 = 0;
        break;
        default: CHS0 = 0; // Change to CH0
            CHS1 = 0;
            CHS2 = 0;
        break;
    }
}

//--------------------------------------------------------------------
// void WriteI2C(char data)
//--------------------------------------------------------------------
void WriteI2C(char data)
{
    do
    {
        WCOL = 0;
        SSPBUF = data;
    } while(WCOL);
    // Release the clock.
    CKP = 1;
}

//--------------------------------------------------------------------
// char ReadI2C(void)
//--------------------------------------------------------------------
char ReadI2C(void)
{
    return(SSPBUF);
}

//--------------------------------------------------------------------
// void main(void)
//--------------------------------------------------------------------
void main(void)
{
    Setup();
    while(1)
    {
        // Check WDT software flag to see if we need to clear the WDT. The
        // clearing of the WDT is disabled by this flag during I2C events to
        // increase reliablility of the slave I2C function. In the event that
        // a sequence on the I2C bus takes longer than expected, the WDT will
        // reset the device (and SSP module).
        if(!stat.wdtdis)
        CLRWDT();

        // The 10msec flag is used to start a new A/D conversion. When the
        // conversion is complete, the AD_Handler() function called from the
        // ISR will get the conversion results and select the next A/D channel.
        // Therefore, each A/D result will get updated every 10msec x (number of
        // channels used).
        if(stat.msec10)
        {
            // Start the next A/D conversion.
            ADGO = 1;
            // Clear the 10 msec time flag
            stat.msec10 = 0;
        }
        // The 100msec time flag is used to update new values that have been
        // written to the command buffer.
        if(stat.msec100)
        {
            if(CmdBuf[0]) FAN_CNTRL = ON;
            else FAN_CNTRL = OFF;
            // Clear the activity LEDs
            LED_0 = OFF;
            LED_1 = OFF;
            // Clear the 100msec time flag
            stat.msec100 = 0;
        }
        // The 1000msec time flag is used to update the tachometer values in the
        // SensorBuf array.
        if(stat.msec1000)
        {
            SensorBuf.b[TACH0] = Count_Tach0;
            Count_Tach0 = 0;
            SensorBuf.b[TACH1] = Count_Tach1;
            Count_Tach1 = 0;
            SensorBuf.b[TACH2] = Count_Tach2;
            Count_Tach2 = 0;
            SensorBuf.b[TACH3] = Count_Tach3;
            Count_Tach3 = 0;
            // Clear the 1000msec time flag
            stat.msec1000 = 0;
        }
    } // end while(1);
}

//--------------------------------------------------------------------
// void Setup(void)
//
// Initializes program variables and peripheral registers.
//--------------------------------------------------------------------
void Setup(void)
{
    stat.msec10 = 0; // Clear the software status bits.
    stat.msec100 = 0;
    stat.msec1000 = 0;
    stat.wdtdis = 0;
    stat.button = 0;
    stat.b_latch = 0;
    RXBufferIndex = 0; // Clear software variables
    SensBufIndex = 0;
    CmdBufIndex = 0;
    TXChecksum.i = 0;
    RXChecksum = 0;
    Count_10m = 0;
    Count_100m = 0;
    Count_Tach0 = 0;
    Count_Tach1 = 0;
    Count_Tach2 = 0;
    Count_Tach3 = 0;
    CmdBuf[0] = 0;
    PORTA = 0xff;
    TRISA = 0xff;
    TRISB = 0xf0;
    TRISC = 0x18;
    OPTION = 0x78; // Weak pullups on, WDT prescaler 2:1
    SSPADD = NODE_ADDR; // Configure SSP module
    SSPSTAT = 0;
    SSPCON = 0;
    SSPCON = 0x36;
    CCPR2L = CCP_LBYTE; // Setup CCP2 for event timing
    CCPR2H = CCP_HBYTE;
    CCP2CON = 0x0a; // Compare mode, no output
    TMR1L = 0; // Timer1 is CCP1 timebase
    TMR1H = 0;
    T1CON = 0x01;
    ADCON1 = 0x02; // Setup A/D converter
    ADCON0 = 0x81;
    if(!TO)
        LED_1 = 1; // Set status LED to indicate WDT
    // timeout has occured.
    else
    {
        PORTB = 0; // Don?t clear port values on WDT
        PORTC = 0; //
    }
    CLRWDT();
    CCP2IF = 0;
    CCP2IE = 1;
    ADIF = 0;
    ADIE = 1;
    SSPIF = 0;
    SSPIE = 1;
    PEIE = 1;
    GIE = 1;
}
