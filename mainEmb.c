#include<proc/pic18f4520.h>
#include"adc.h"
#include"bits.h"
#include"config.h"
#include"serial.h"
#include"ds1307.h"
#include"keypad.h"
#include"lcd.h"
#include"ssd.h"
#include"timer.h"
#include"pwm.h"
#include"rgb.h"
#include"delay.h"

//defines para o LCD
#define L_ON 0x0F
#define L_OFF 0X08
#define L_CLR 0x01
#define L_L1 0x80
#define L_L2 0xC0

//escolha do serial
unsigned int tempIde; //temperatura ideal
unsigned long int tempMax; //temp máxima
unsigned long int pot; //potenciometro
unsigned char op; //opcao
unsigned long int temperatura;

//realizando leitura do pot e atualizando LCD
void leTemperatura(){ 
    unsigned long int degree;
    pot = adcRead(0);
    if(degree != pot){ //checa se mudou
        
        lcdPosition(1, 9);
        lcdString("T: ");
        lcdChar((((30*pot)/1023))/10 %10 + 48);
        lcdChar((((30*pot)/1023)) %10 + 48);
        lcdPosition(1,15);
        lcdString("C");
    }//end if
    degree = pot;
}//end temperatura

void mostraDisplay() {
    int v0,v1,v2,v3;
    v0 = ((tempMax / 1000) % 10);
    v1 = ((tempMax / 100) % 10);
    v2 = ((tempMax / 10) % 10);
    v3 = ((tempMax / 1) % 10);
    ssdDigit(v0,0);
    ssdDigit(v1,1);
    ssdDigit(v2,2);
    ssdDigit(v3,3);
}//end mostra display

void apagaDisplay(){
    ssdDigit(0,0); //desliga ssd
    ssdDigit(0,1); //desliga ssd
    ssdDigit(0,2); //desliga ssd
    ssdDigit(0,3); //desliga ssd
}

void leTeclado(void) {
    unsigned char tecla;
    kpDebounce();
    if (kpRead() != tecla) {
        tecla = kpRead();
        switch (kpReadKey()) {
            case 'U': //imprime BEBIDA 1
                lcdCommand(L_L1);
                lcdString("MALBEC   I: 12 C");
                lcdCommand(L_L2);
                lcdString("QTD: 12");
                tempMax = 24;
                tempIde = 12; //temperatura ideal para consumir
                break;
            case 'L': //imprime BEBIDA 2
                lcdCommand(L_L1);
                lcdString("MERLOT   I:  9 C");
                lcdCommand(L_L2);
                lcdString("QTD: 19");
                tempMax = 22;
                tempIde = 9;
                break;
            case 'D': //imprime BEBIDA 3
                lcdCommand(L_L1);
                lcdString("RIESLING I:  9 C");
                lcdCommand(L_L2);
                lcdString("QTD: 5 ");
                tempMax = 9;
                tempIde = 7;
                break;
            case 'R': //imprime BEBIDA 4
                lcdCommand(L_L1);
                lcdString("ROSE     I: 10 C");
                lcdCommand(L_L2);
                lcdString("QTD: 14");
                tempMax = 21;
                tempIde = 10;
                break;
            case 'S': //imprime BEBIDA 5
                lcdCommand(L_L1);
                lcdString("CABERNET I:  6 C");
                lcdCommand(L_L2);
                lcdString("QTD: 8 ");
                tempMax = 8;
                tempIde = 6;
                break;
            default:
                break;
        }//end switch
    }
}//end le teclado

serialcom(){
    op = serialRead();
    if(op != 0){
        switch (op) {
            serialSend(op);
            case 'U': //imprime BEBIDA 1
                lcdCommand(L_L1);
                lcdString("MALBEC   I: 12 C");
                lcdCommand(L_L2);
                lcdString("QTD: 12");
                tempMax = 24;
                tempIde = 12; //temperatura ideal para consumir
                break;
            case 'L': //imprime BEBIDA 2
                lcdCommand(L_L1);
                lcdString("MERLOT   I:  9 C");
                lcdCommand(L_L2);
                lcdString("QTD: 19");
                tempMax = 22;
                tempIde = 9;
                break;
            case 'D': //imprime BEBIDA 3
                lcdCommand(L_L1);
                lcdString("RIESLING I:  9 C");
                lcdCommand(L_L2);
                lcdString("QTD: 5 ");
                tempMax = 9;
                tempIde = 7;
                break;
            case 'R': //imprime BEBIDA 4
                lcdCommand(L_L1);
                lcdString("ROSE     I: 10 C");
                lcdCommand(L_L2);
                lcdString("QTD: 14");
                tempMax = 21;
                tempIde = 10;
                break;
            case 'S': //imprime BEBIDA 5
                lcdCommand(L_L1);
                lcdString("CABERNET I:  6 C");
                lcdCommand(L_L2);
                lcdString("QTD: 8 ");
                tempMax = 8;
                tempIde = 6;
                break;    
            default:
                break;
        }//end switch
    }//end if
    
    pot = adcRead(0);
    serialSend('T');
    serialSend(':');
    serialSend(' ');
    serialSend((((30*pot)/1023))/10 %10 + 48);
    serialSend((((30*pot)/1023)) %10 + 48);
    serialSend('\n');

}//end serial

void calcula(){

    temperatura = (30*pot)/1024;
    if(temperatura >= tempMax){ //teste de temperatura vs temp maxima
        mostraDisplay();
    }
    else if(temperatura < tempMax){
        apagaDisplay();
    }
}

void main(void) {
    char slot;
    lcdInit(); //inicia perifericos
    kpInit();
    ssdInit();
    timerInit();
    serialInit();
    adcInit();
    lcdString("   WELCOME TO   ");
    lcdCommand(L_L2);
    lcdString("   SMART ADEGA  ");
    
    timed(10);
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("FERNANDO'S ADEGA");
    
    for (;;) {
        timerReset(6000);
        switch (slot) {
            case 0:
                leTeclado();
                ssdUpdate();
                slot = 1;
                break;
            case 1:
                serialcom();
                slot = 2;
                break;
            case 2:
                leTemperatura();
                slot = 3;
                break;
            case 3:
                calcula();
                ssdUpdate();
                slot = 4;
                break;
            default:
                slot = 0;
                break;
        }//end switch

        timerWait();
    }//end for
}//end main


