// Программа контроллера углового положения AS5045 по RS-485
// Все устройства после первоначального программировании получают адрес 150.
// Команды мастера для контроллера углового положения AS5045:
// Команда задания сетевого адреса устройству.
// Запрос: <01><xxx> - задание адреса устройства, ххх - адрес в диапазоне 0..255
// Пример запроса <01><160> - 0х96 0х97 0хC2 0х01 0хA0 0х0B 0хA9
// Ответ: <xxx> - полученный адрес. Пример ответа: 0х96 0х97 0х41 0хA0 0х89 0хA9
// Ответ приходит со старого адреса, после этого происходит смена адреса
// и дальнейшие запросы должны идти уже с новым адресом
// Команда запроса углового положения и статуса AS5045.
// Запрос: <02> - запрос данных об угловом положении.
// Пример запроса по адресу 160: <02> - 0х96 0хА0 0х81 0х02 0хDC 0xA9
// Ответ: <Байт_1><Байт_2><Байт_3> 
// Пример ответа: 0х96 0хA0 0х03 0хB4 0х98 0х40 0х30 0хA9
// 0хB4 0х98 0х40 - информация из AS5045, из них только старшие 18 бит являются
// значащими, т.е.:
// 0хB49 - 12-бит, угловое положение (2889) или 0,08 х 2889 = 231,12 град.
// 0х84  - 6 бит, статус  (OCF, COF, LIN, MagINC, MagDEC, EvenPAR), где
// OCF = 1, COF = 0, LIN = 0, MagINC = 0, MagDEC = 0, EvenPAR = 0
// при правильной работе биты OCF, COF, LIN, MagINC, MagDEC  = 10000x--,
// при неправильном положении (или отсутствии) магнита MagINC = 1, MagDEC = 1,
// т.е. правильными вариантами являются - 0x80 или 0х84 в зависимости от EvenPAR
// Все другие команды считаются ошибочными, и на них контроллер отвечает
// одним байтом - 'E'(0x45) от слова "Error".
// Настройки порта  - 57600/8/1/N.
// НЕ ЗАБЫВАЕМ МЕНЯТЬ СЕТЕВЫЕ АДРЕСА УСТРОЙСТВА RS-485, ЕСЛИ ОНИ БУДУТ РАБОТАТЬ
// В ОДНОЙ СЕТИ.
// КОНФИГУРИРОВАНИЕ СИСТЕМЫ: 1)ПОДКЛЮЧАЕМ ПЕРВОЕ УСТРОЙСТВО (ТОЛЬКО ЕГО ОДНО),
// ОБРАЩАЕМСЯ К НЕМУ ПО АДРЕСУ 150 И МЕНЯЕМ КОМАНДОЙ <01> АДРЕС НА НУЖНЫЙ.
// ПОДКЛЮЧАЕМ К СЕТИ ВТОРОЕ УСТРОЙСТВО И АНАЛОГИЧНО МЕНЯЕМ ЕГО АДРЕС.

/*********************** Протокол обмена по RS-485 *****************************
PACKAGE:
--------
START_BYTE 0x96
ADDRESS
DATALEN
[DATA1]            // if exists
[DATA2]            // if exists
[DATA3]            // if exists
CRC
STOP_BYTE  0xA9

DATALEN bits
------------
bit7 = 1  MASTER SENDS
       0  SLAVE  SENDS
bit6 = 1  ADDRESS WAS XORed with 1, IT WAS EQUAL TO START_BYTE or STOP_BYTE
       0  ADDRESS UNCHANGED
bit5 = 0  FIXED
bit4 = 1  DATA3 (if exists) WAS XORed with 1, IT WAS EQUAL TO START_BYTE or STOP_BYTE
       0  DATA3 (if exists) UNCHANGED
bit3 = 1  DATA2 (if exists) WAS XORed with 1, IT WAS EQUAL TO START_BYTE or STOP_BYTE
       0  DATA2 (if exists) UNCHANGED
bit2 = 1  DATA1 (if exists) WAS XORed with 1, IT WAS EQUAL TO START_BYTE or STOP_BYTE
       0  DATA1 (if exists) UNCHANGED
bit1...bit0 = 0 to 3 NUMBER OF DATA BYTES SEND

CRC generation :
----------------
crc_send  = datalen ^ address;
crc_send ^= data[0];    // if exists
crc_send ^= data[1];    // if exists
crc_send ^= data[2];    // if exists
crc_send = ~crc_send;
if ((crc_send == START_BYTE) || (crc_send == STOP_BYTE))
   crc_send++;

NOTE: DATALEN<4..0> can not take the START_BYTE<4..0> or STOP_BYTE<4..0> values.
*******************************************************************************/

// SPI работает на частоте Fosc/64 = 691,2 кГц (для AS5045 <= 1 МГц)

void Read_AS5045 (void);

//------------------------------------------------------------------------------
//  Параметры таймера TMR2:
//  - прескайлер = 16,
//  - постскайлер = 16,
//  - предел счета = 98
//  Тактовая частота Fosc = 11,0592 МГц
//  Tcyc = 1/(Fosc/4) = 0,4 мкс
//  Период возникновения прерывания  TMR2  равен 16 * 16 * 97 * Tcyc = 10 ms
//------------------------------------------------------------------------------
#define PR2_CONST       97-1
#define TMR2_PRS        0x03                         // prs = 1:16
#define TMR2_POST       0x0F                         // post = 1:16
#define T2CON_CONST     (TMR2_POST<<3) | TMR2_PRS

sbit CS_bit at RC2_bit;
sbit CS_Direction_bit at TRISC2_bit;

sbit  rs485_rxtx_pin  at RC1_bit;
sbit  rs485_rxtx_pin_direction at TRISC1_bit;

char dat_in[30], i;
char dat_out[10];
unsigned short k, flag_1_sec, cmd, addr;
unsigned short Data_AS5045[3] = {0,0,0}, dat[6];
unsigned int cnt = 0;


void interrupt (){
  volatile unsigned short tmp;

   if (TMR2IF_bit){    // прерывание по переполнению TMR2 каждые 10 мс
    TMR2IF_bit = 0;
    cnt++;
    if(cnt > 100){
     cnt = 0;
     flag_1_sec = 1;
    }
  }

  
  if (RCIF_bit) {
      if(OERR_bit || FERR_bit){

       if(FERR_bit){
        tmp = RCREG;
       }

       if(OERR_bit){
         CREN_bit = 0;
         CREN_bit = 1;
        }
      }
      else{
       RS485Slave_Receive(dat_in);
      }
  }
}


void main() {

 ANSEL  = 0;                                     // Configure AN pins as digital I/O
 ANSELH = 0;
 C1ON_bit = 0;                                   // Disable comparators
 C2ON_bit = 0;

 TRISC6_bit = 0;
 LATC6_bit = 0;
 
 // Настройка TMR2 - переполнение каждые 10 мс
 PR2 = PR2_CONST;
 T2CON = T2CON_CONST | 0x04;
 TMR2IE_bit = 1;

 //EEPROM_Write(0, 150);
 //Delay_ms(20);

 UART1_Init(57600);
 Delay_ms(100);
 addr = EEPROM_Read(0);
 if (addr == 0xFF){
  addr = 150;
 }
 RS485Slave_Init(addr);
 //RS485Master_Init();

 CS_bit = 1;                             // Deselect Chip before initalization
 CS_Direction_bit = 0;                   // Set direction
 // SPI Initialization, Fosc/64, when idle SPI High, Data transmit on low to high edge
 SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_END, _SPI_CLK_IDLE_HIGH, _SPI_LOW_2_HIGH);
 
 dat_out[0] = 0;
 dat_out[1] = 0;
 dat_out[2] = 0;
 flag_1_sec = 0;
 
 PEIE_bit = 1;
 RCIE_bit = 1;
 GIE_bit = 1;
 
 if(TO__bit == 0)     // проверка на срабатывание WDT
  LATC6_bit = 1;
 
 SWDTEN_bit = 1;   //включаем WDT, время срабатывания WDT = ~4 сек

 for(;;){
 
 
 /*//////////////////////////////////////////////
 for(;;){
  if(flag_1_sec){
     flag_1_sec = 0;
     dat[0] = 2;
     dat[1] = 0;
     dat[2] = 0;
     dat[4] = 0;                          // ensure that message received flag is 0
     dat[5] = 0;                          // ensure that error flag is 0
     dat[6] = 0;

     RS485Master_Send(dat,1,160);      // отправляем данные
     
  }
 }
 */////////////////////////////////////////////
 


  if (dat_in[5])  {             // выявлена ошибка приема
      dat_in[5] = 0;
     // здесь, если требуется должен быть код обработки ошибки приема
  }

  if (dat_in[4]) {              // в буфере находятся правильные данные
      dat_in[4] = 0;


       cmd = dat_in[0];         // в dat_in[0] находится принятая команда
       switch(cmd){
         case 1:
                   addr = dat_in[1];
                   EEPROM_Write(0, addr);
                   delay_ms(20);
                   dat_out[0] = addr;
                   RS485Slave_Send(dat_out,1);
                   asm reset
         case 2:
                   Read_AS5045();
                   dat_out[0] = Data_AS5045[0];
                   dat_out[1] = Data_AS5045[1];
                   dat_out[2] = Data_AS5045[2];
                   i = 3;
                   break;
         /*
         case '9':
                  Read_AS5045();
                  dat_out[0] = Data_AS5045[1] << 1;
                  dat_out[0] &= 0x1E;
                  dat_out[0]  |= (Data_AS5045[1] >> 7);
                  i = 1;
                  break;
         */
         default:
                dat_out[0]  = 'E';
                i = 1;
         }
       delay_ms(1);
       RS485Slave_Send(dat_out,i);  // отправляем данные
       asm CLRWDT                   // если не будет отправки, то будет ресет
  }


  /*
   // тест передачи
  if(flag_1_sec){
     flag_1_sec = 0;
     
     Data_AS5045[0] = 1;
     Data_AS5045[1] = 2;
     Data_AS5045[2] = 3;
     
     //Read_AS5045();
     RS485Slave_Send(Data_AS5045,3);      // отправляем данные
  }
  */

  //LATC6_bit = 0;
  //asm nop
  //LATC6_bit = 1;
  //asm nop

 }
}

//==============================================================================
void Read_AS5045 (void){
  // получение данных от AS5045
  CS_bit = 0;
  for(k = 0; k < 3; k++){
     SSPBUF = 0x55;
     while(!BF_bit) continue;
     Data_AS5045[k] = SSPBUF;
  }
  CS_bit = 1;
}