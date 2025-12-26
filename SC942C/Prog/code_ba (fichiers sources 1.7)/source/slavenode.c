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
// Include Files
//--------------------------------------------------------------------
#include <pic.h>
//#include <string.h>
//#include <ctype.h>
//#include <math.h>
//#include <stdlib.h>
//#include <stdio.h>
#include "constantes.h"
#include "struct.h"
#include "global.h"
#include "hard.h"
#include "crc.h"

//--------------------------------------------------------------------
// Function Prototypes
//--------------------------------------------------------------------
void vd_I2C_Setup(void);
void WriteI2C(char data);
char ReadI2C(void);
void SSP_Handler(void);

extern void vd_Sauve_Etat (void);

//--------------------------------------------------------------------
//Constant Definitions
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// Buffer Length Definitions
//--------------------------------------------------------------------
#define RX_BUF_LEN      25 // Length of receive buffer
#define CMD_BUF_LEN     25 // Length of buffer for command data.

//--------------------------------------------------------------------
// Variable declarations
//--------------------------------------------------------------------
char RXBuffer[RX_BUF_LEN]; // Holds incoming bytes from master
                            // device.
char CmdBuf[CMD_BUF_LEN]; //
//xxxchar RXChecksum; //
unsigned short us_CRC;
unsigned char
    RXBufferIndex, // Index to received bytes.
    RXByteCount, // Number of bytes received
    SensBufIndex, // Index to sensor data table
    CmdBufIndex; //
union INTVAL
{
char b[2];
int i;
};
//char TXChecksum; // Holds checksum of bytes sent to

union I2CSTAT
{
    struct{
        unsigned BF:1;
        unsigned UA:1;
        unsigned R_nW:1;
        unsigned S:1;
        unsigned P:1;
        unsigned D_nA:1;
        unsigned CKE:1;
        unsigned SMP:1;
    } b_stat ;
    unsigned char b;
} I2CStat ;

union TXBUF 
{ // to be sent to master.
    struct{
        unsigned code:8;        // code de trame reçue juste avant

        unsigned chkfail:1;
        unsigned rxerror:1;
        unsigned ovflw:1;
        unsigned sspov:1;
        unsigned noactivity:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned r_w:1;
    } comm_stat ;
    unsigned char b[4];
} TxBuf ;
//
unsigned char uc_I2C_activity;

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

enum enum_CODE_TRAMES
{
	C_FORCAGE_SORTIES = 1,
	C_CONF_SORTIES,
	C_TPS_EXTINCTION,
	C_TPS_ACTION,
	C_ACTIONS
};



//--------------------------------------------------------------------
// void SSP_Handler(void)
//--------------------------------------------------------------------
void SSP_Handler(void)
{
unsigned char i,j;
unsigned short l_us_CRCRecu;

    I2CStat.b = SSPSTAT;
    uc_I2C_activity = 60;           // relance la surveillance d'ativité

    //--------------------------------------------------------------------
    // STATE 1: If this is a WRITE operation and last byte was an ADDRESS
    //--------------------------------------------------------------------
//    if(!D_nA && !R_nW && BF && I2C_START)
    if(!I2CStat.b_stat.D_nA && !I2CStat.b_stat.R_nW && I2CStat.b_stat.BF && I2CStat.b_stat.S)
    {
        // Clear WDT and disable clearing in the main program loop. The
        // WDT will be used to reset the device if an I2C message exceeds
        // the expected amount of time.
        CLRWDT();
        stat.wdtdis = 1;
        // clear the receive buffer and the index. Put the received data
        // in the first buffer location.
        RXByteCount = RX_BUF_LEN;
        RXBuffer[0] = ReadI2C();
        // Initialize the receive checksum.
        //xxxRXChecksum = 0; //le 1er octet est l'adresse qui n'est pas prise en compte dans la checksum
        // Increment the buffer index
        RXBufferIndex =1;
        TxBuf.b[0] = 0;
       // Reset the communication status byte. 
        TxBuf.b[1] = 0x00;      // xxx 0x02
        // Check to make sure an SSP overflow has not occurred.
        if(SSPOV)
        {
            TxBuf.comm_stat.sspov = 1;
            TxBuf.comm_stat.rxerror = 1;
            SSPOV = 0;
        }
    }
    //--------------------------------------------------------------------
    // STATE 2: If this is a WRITE operation and the last byte was DATA
    //--------------------------------------------------------------------
//    else if(D_nA && !R_nW && BF)
    else if(I2CStat.b_stat.D_nA && !I2CStat.b_stat.R_nW && I2CStat.b_stat.BF)
    {
        // Check the number of data bytes received.
        if(RXBufferIndex == RX_BUF_LEN)
        {
            TxBuf.comm_stat.ovflw = 1;
            TxBuf.comm_stat.rxerror = 1;
            RXBufferIndex = RX_BUF_LEN - 1;
        }
        // Check to see if SSP overflow occurred.
        if(SSPOV)
        {
            TxBuf.comm_stat.sspov = 1;
            TxBuf.comm_stat.rxerror = 1;
            SSPOV = 0;
        }
        // Get the incoming byte of data.
        RXBuffer[RXBufferIndex] = ReadI2C();
        // Add the received value to the checksum.
        //xxxRXChecksum += RXBuffer[RXBufferIndex];
        // Check to see if the current byte is the DATA_LEN byte. If it is,
        // check the MSb to see if this is a data write or data request.
        if(RXBufferIndex == 1)
        {
            // le code de la trame fixe la longueur de la trame
            
            switch ( RXBuffer[RXBufferIndex])
            {
                case C_FORCAGE_SORTIES :
                    RXByteCount = 11;
                break;
                   
                case C_CONF_SORTIES :
                    // RXByteCount = 3;
                    RXByteCount = 11;   // 12/03/13/IR configuration reçue sur 1 octet
                break;
                case C_TPS_EXTINCTION :
                    RXByteCount = 19;
                break;
                case C_TPS_ACTION :
                    RXByteCount = 11;
                break;
                case C_ACTIONS :
                    RXByteCount = 4;
                break;
                default :   // code de trame inconnu
                    TxBuf.comm_stat.ovflw = 1;
                    TxBuf.comm_stat.rxerror = 1;
                break;
            }

            TxBuf.b[0] = RXBuffer[RXBufferIndex];
            TxBuf.comm_stat.r_w = 0;
            if(RXByteCount > RX_BUF_LEN)
            {
                TxBuf.comm_stat.ovflw = 1;
                TxBuf.comm_stat.rxerror = 1;
            }
        }
        else if(RXBufferIndex > RXByteCount)
        {       // sécurité
            TxBuf.comm_stat.ovflw = 1;
            TxBuf.comm_stat.rxerror = 1;
        }

        // If the master is doing a data write to the slave, we must check
        // for the end of the data string so we can do the checksum.
        else if(RXBufferIndex == RXByteCount)
        {
            //xxxif(RXChecksum)
            us_CRC = us_CalculerCRCSurTrame(RXBuffer+1,RXBufferIndex-2);
            l_us_CRCRecu = (RXBuffer[RXBufferIndex] << 8) + RXBuffer[RXBufferIndex-1];
            if(us_CRC != l_us_CRCRecu)
            {
                us_CRC = 0;
                TxBuf.comm_stat.chkfail = 1;
                TxBuf.comm_stat.rxerror = 1;
            }
            else
            {
                // Checksum was OK,
                TxBuf.comm_stat.chkfail = 0;
                
                if (CmdBuf[0] == 0)
                {
                    // copy the data in receive buffer
                    // into the command buffer.
                    for(i=1, j = 0; i <= RXBufferIndex; i++,j++)
                    {
                        if(j == CMD_BUF_LEN) j--;
                        CmdBuf[j] = RXBuffer[i];
                    }
                    TxBuf.comm_stat.rxerror = 0;
                  
                }
                else
                {
                    // buffer de commande non disponible
                    TxBuf.comm_stat.rxerror = 1;
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
//    else if(!D_nA && R_nW && !BF && I2C_START)
    else if(!I2CStat.b_stat.D_nA && I2CStat.b_stat.R_nW && !I2CStat.b_stat.BF && I2CStat.b_stat.S)
    {
        // Clear the buffer index to the sensor data.
        SensBufIndex = 0;
        // Initialize the transmit checksum
        //xxxTXChecksum = TxBuf.b[0];
        // Send the data code byte.
        WriteI2C(TxBuf.b[0]);
    }
    //--------------------------------------------------------------------
    // STATE 4: If this is a READ operation and the last byte was DATA
    //--------------------------------------------------------------------
//    else if(D_nA && R_nW && !BF)
    else if(I2CStat.b_stat.D_nA && I2CStat.b_stat.R_nW && !I2CStat.b_stat.BF)
    {
        // If we haven?t transmitted all the required data yet,
        // get the next byte out of the TXBuffer and increment
        // the buffer index. Also, add the transmitted byte to
        // the checksum
        switch (SensBufIndex)
        {
            case 0 :
                // Send the communication status byte.
                TxBuf.b[1] = (us_CRC & 0xFF);
                WriteI2C(TxBuf.b[1]);
                //xxxTXChecksum += (us_CRC & 0xFF);//TxBuf.b[1];
                SensBufIndex++;
            break;

            case 1 :
                // Send the communication status byte.
                TxBuf.b[2] = ((us_CRC>>8) & 0xFF);
                WriteI2C(TxBuf.b[2]);
                //xxxTXChecksum += (us_CRC & 0xFF);//TxBuf.b[1];
                SensBufIndex++;
            break;
            
            case 2 :
                // send the checksum
                //TXChecksum = ~TXChecksum;
                //TXChecksum++;
                //WriteI2C(TXChecksum);
                us_CRC = us_CalculerCRCSurTrame(TxBuf.b,3);
                WriteI2C((us_CRC & 0xFF));
                SensBufIndex++;
            break;

            case 3 :
                // send the checksum
                //TXChecksum = ~TXChecksum;
                //TXChecksum++;
                //WriteI2C(TXChecksum);
                WriteI2C(((us_CRC>>8) & 0xFF));
                SensBufIndex++;
            break;

            default :
               // Otherwise, just send dummy data back to the master.
               TxBuf.comm_stat.r_w = 1;
               WriteI2C(0x55);
            break;
        }
     }

    //--------------------------------------------------------------------
    // STATE 5: A NACK from the master device is used to indicate that a
    // complete transmission has occurred. The clearing of the
    // WDT is reenabled in the main loop at this time.
    //--------------------------------------------------------------------
//    else if(D_nA && !R_nW && !BF)
    else if(I2CStat.b_stat.D_nA && !I2CStat.b_stat.R_nW && !I2CStat.b_stat.BF)
    {
        stat.wdtdis = 0;
        CLRWDT();
    }
    else;
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
/*
void main(void)
{
    I2C_Setup();
    while(1)
    {
        // Check WDT software flag to see if we need to clear the WDT. The
        // clearing of the WDT is disabled by this flag during I2C events to
        // increase reliablility of the slave I2C function. In the event that
        // a sequence on the I2C bus takes longer than expected, the WDT will
        // reset the device (and SSP module).
        if(!stat.wdtdis)
        CLRWDT();

    } // end while(1);
}
*/

//--------------------------------------------------------------------
// void vd_I2C_Setup(void)
//
// Initializes I2C program variables
//--------------------------------------------------------------------
void vd_I2C_Setup(void)
{
    RXBufferIndex = 0; // Clear software variables
    SensBufIndex = 0;
    CmdBufIndex = 0;
    //xxxTXChecksum = 0;
    //xxxRXChecksum = 0;
    CmdBuf[0] = 0;
}

//--------------------------------------------------------------------
// void vd_I2C_Setup(void)
//
// Initializes I2C program variables
//--------------------------------------------------------------------
void vd_Traitement_I2C(void)
{
    
unsigned char l_uc_Compteur, l_uc_masque;
unsigned char l_uc_RelaisSens1, l_uc_RelaisSens2;
    
    // surveillance réception d'une trame
    if (CmdBuf[0] != 0)
    {
        // code, longueur de trame et checksum déja vérifiés
        
        // interprétation des trames
        switch (CmdBuf[0]) 
        {
            case C_FORCAGE_SORTIES :
                // forçage des sorties simples
                // extinction prioritaire
                us_SortiesRelais |= (CmdBuf[2] + (CmdBuf[4] << 8));     // allume
                us_SortiesRelais &= ~(CmdBuf[1] + (CmdBuf[3] << 8));    // éteint

                // forçage des sorties variateurs
                // extinction prioritaire
                for (l_uc_Compteur = 0, l_uc_masque = 0x01; l_uc_Compteur < VARIATEUR_NB; l_uc_Compteur++, l_uc_masque= l_uc_masque<<1)
                {
                    // passe la commande comme une action des entrées locales
                    if ((CmdBuf[6] & l_uc_masque) != 0)
                    {
                       st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_ON;
                    }
                    if ((CmdBuf[5] & l_uc_masque) != 0)
                    {
                       st_EntreesVariateur[l_uc_Compteur].ub3_EtatCommande = uc_VARIATEUR_OFF;
                    }
                }
                
                // forçage des sorties volets / stores
                // montée / ouverture prioritaire
                for (l_uc_Compteur = 0, l_uc_masque = 0x01; l_uc_Compteur < VOLET_NB; l_uc_Compteur++, l_uc_masque= l_uc_masque<<1)
                {
                    // passe la commande comme une action des entrées locales
                    l_uc_RelaisSens1 = l_uc_Compteur *2;		// 2 relais par volet
                    l_uc_RelaisSens2 = l_uc_RelaisSens1 +1;		// 2 relais par volet

                    if ((CmdBuf[7] & l_uc_masque)!= 0)
                    {
                        st_EntreesVoletRoulant[l_uc_RelaisSens1].ub3_EtatCommande = uc_VOLET_ON;
                    }
                    else
                    {
                        if ((CmdBuf[8] & l_uc_masque) != 0)
                        {
                            st_EntreesVoletRoulant[l_uc_RelaisSens2].ub3_EtatCommande = uc_VOLET_ON;
                        }
                    }
                }

                CmdBuf[0] = 0;      // traitement terminé
           break;
                   
            // configuration des sorties variateur : TOR ou gradateur
            case C_CONF_SORTIES :
                
                // 12/03/13/IR configuration variateur sur 1 octet
                //for (l_uc_Compteur = 0, l_uc_masque = 0x01; l_uc_Compteur < VARIATEUR_NB; l_uc_Compteur++, l_uc_masque= l_uc_masque<<1)
                //{
                //    if ((CmdBuf[1] & l_uc_masque)!= 0)
                //    {
                //        st_EntreesVariateur[l_uc_Compteur].ub1_EntreeMode = uc_MODE_VARIATEUR;
                //    }
                //    else
                //    {
                //        st_EntreesVariateur[l_uc_Compteur].ub1_EntreeMode = uc_MODE_TOR;
                //    }
                //}

                for (l_uc_Compteur = 0; l_uc_Compteur < VARIATEUR_NB; l_uc_Compteur++)
                {
                    if (st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode != (CmdBuf[l_uc_Compteur+1] & 0x07))
                    {
                        st_EntreesVariateur[l_uc_Compteur].ub3_EntreeMode = CmdBuf[l_uc_Compteur+1] & 0x07;
                        // sauvegarde en eeprom
                        eeprom_write((l_uc_Compteur *3)+0x0014,CmdBuf[l_uc_Compteur+1]);
                    }
                }
                CmdBuf[0] = 0;      // traitement terminé
            break;       
            
            case C_TPS_EXTINCTION :

                for (l_uc_Compteur = 0; l_uc_Compteur < RM_NB; l_uc_Compteur++)
                {
                    if (uc_TempsCdeLampe[l_uc_Compteur] != CmdBuf[l_uc_Compteur+1])
                    {
                        uc_TempsCdeLampe[l_uc_Compteur] = CmdBuf[l_uc_Compteur+1];
                        // sauvegarde en eeprom
                        eeprom_write(l_uc_Compteur+0x0032, uc_TempsCdeLampe[l_uc_Compteur]);
                    }
                }
                CmdBuf[0] = 0;      // traitement terminé
            break;
            
            case C_TPS_ACTION :

                for (l_uc_Compteur = 0; l_uc_Compteur < VOLET_NB; l_uc_Compteur++)
                {
                    if (uc_TempsCdeVolet[l_uc_Compteur] != CmdBuf[l_uc_Compteur+1])
                    {
                        uc_TempsCdeVolet[l_uc_Compteur] = CmdBuf[l_uc_Compteur+1];
                        // sauvegarde en eeprom
                        eeprom_write(l_uc_Compteur+0x002A, uc_TempsCdeVolet[l_uc_Compteur]);
                    }
                }
                CmdBuf[0] = 0;      // traitement terminé
            break;
            
            case C_ACTIONS :
                // IR 14/03/2013 Sauvegarde effectuée sur front uniquement
                if (st_Commandes.ub1.Sauvegarde ==0)
                {
                    st_Commandes.o = CmdBuf[1];
                    if (st_Commandes.ub1.Sauvegarde ==1)
                    {
                        vd_Sauve_Etat ();
                    }
                }
                else
                {
                    st_Commandes.o = CmdBuf[1];
                }
                CmdBuf[0] = 0;      // traitement terminé
            break;

            default :   // code de trame inconnu

                CmdBuf[0] = 0;      // traitement terminé
            break;
        }
    }
        
    // surveillance activité sur bus I2C
    if (b1_BT_1S == 1)			// flag seconde : à 1 chaque seconde pendant 1 cycle
    {
        if (uc_I2C_activity >0)
        {
            TxBuf.comm_stat.noactivity = 0;
            uc_I2C_activity --;
        }
        else
        {
            TxBuf.comm_stat.noactivity = 1;
        }
    }

    //xxx visualisation de l'état de la com sur la led : pour tests
    // pilote la led en fonction de l'état de la com
    /*if (TxBuf.b[1] == 0)
    {
        // com I2C OK
        // Fait flasher la led d'état périodiquement
	if(uc_CompteurLED < uc_LED_CADENCEMENT)
	{
		LED_OFF;
		uc_CompteurLED++;
	}
	else
	{
		LED_ON;
		uc_CompteurLED = 0;
	}
    }
    else
    {
        if (TxBuf.comm_stat.noactivity == 1)
        {
            // absence de com
            // clignotement lent
            if(uc_CompteurLED < uc_LED_CADENCEMENT)
            {
		LED_OFF;
            }
            else
            {
		LED_ON;
            }
            uc_CompteurLED++;
            if(uc_CompteurLED >= (uc_LED_CADENCEMENT<<1))
            {
  		uc_CompteurLED = 0;
            }
        }
        else
        {
            // erreur de com
            // clignotement rapide
            if(uc_CompteurLED < (uc_LED_CADENCEMENT>>1))
            {
		LED_OFF;
            }
            else
            {
		LED_ON;
            }
            uc_CompteurLED++;
            if(uc_CompteurLED >= uc_LED_CADENCEMENT)
            {
  		uc_CompteurLED = 0;
            }
        }
    }*/
    //xxx fin visualisation de l'état de la com sur la led : pour tests

}
