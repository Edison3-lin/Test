#include "include.h"

static bool UartBusy = FALSE;
u32 gSystemTickCount = 0;
uint16 ServoPwmDutyset[8] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
uint8 BuzzerState = 0;
uint16 keycount;
uint8_t Mode = 0;
uint16 BatteryVoltage;
uint16 error;

PS2X ps2X;

int InitPS2()
{
  error = ps2X.config_gamepad(A2, A4, A3, A5);
  return error;
}
void InitTimer2(void)
{
  TCCR2A = 0;
  TCCR2B = _BV(CS21) | _BV(CS20); //
  TIMSK2 = _BV(TOIE2);
  TCNT2 = 206;
  sei();
}

void LedFlip()
{
  if (digitalRead(LED) == HIGH)
  {
    digitalWrite(LED, LOW);
  }
  else
  {
    digitalWrite(LED, HIGH);
  }
}

uint16 GetADCResult(void)
{
  return analogRead(ADC_BAT);
}

void CheckBatteryVoltage(void)
{
  uint8 i;
  uint32 v = 0;
  for (i = 0; i < 8; i++)
  {
    v += GetADCResult();
  }
  v >>= 3;

  v = v * 1875 / 128;
  BatteryVoltage = v;
  // BatteryVoltage = 7000;
}

uint16 GetBatteryVoltage(void)
{ 
  return BatteryVoltage;
}

void Buzzer(void)
{ 
  static bool fBuzzer = FALSE;
  static uint32 t1 = 0;
  static uint32 t2 = 0;
  if (fBuzzer)
  {
    t1++;
    if (t1 <= 2)
    {
      digitalWrite(BUZZER, LOW);//2.5KHz
    }
    else if (t1 <= 4)
    {
      digitalWrite(BUZZER, HIGH);//2.5KHz
    }
    if (t1 == 4)
    {
      t1 = 0;
    }
  }

  if (BuzzerState == 0)
  {
    fBuzzer = FALSE;
    t2 = 0;
  }
  else if (BuzzerState == 1)
  {
    t2++;
    if (t2 < 5000)
    {
      fBuzzer = TRUE;
    }
    else if (t2 < 10000)
    {
      fBuzzer = FALSE;
    }
    else
    {
      t2 = 0;
    }
  }
}


bool manual = FALSE;
ISR(TIMER2_OVF_vect)
{
  TCNT2 = 206;

  static uint16 time = 0;
  static uint16 timeBattery = 0;
  static uint16 mytime = 0;
  Buzzer();
  if (++time >= 10)
  {
    time = 0;
    gSystemTickCount++;
    //		Ps2TimeCount++;
    if (GetBatteryVoltage() < 6600) 
    {
      timeBattery++;
      if (timeBattery > 5000)
      {
        BuzzerState = 1;
      }
    }
    else
    {
      timeBattery = 0;
      if (manual == TRUE)
      {
        BuzzerState  = 1;
        mytime++;
        if (mytime > 80 && mytime < 130)
        {
          manual = FALSE;
        }
        if (mytime >= 210)
        {
          mytime = 0;
          manual = FALSE;
        }
      } else
      {
        if (mytime != 0)
        {
          mytime++;
          if (mytime >= 130 && mytime < 210)
          {
            manual = TRUE;
            BuzzerState   = 1;
            return;
          }
        }
        BuzzerState = 0;
      }
    }
  }
}

void TaskTimeHandle(void)
{
  static uint32 time = 10;
  static uint32 times = 0;
  if (gSystemTickCount > time)
  {
    time += 10;
    times++;
    if (times % 2 == 0) //20ms
    {
      ServoPwmDutyCompare();
    }
  }
}

void ps2Handle() {                      
  static uint32_t Timer;   

  if (Timer > millis())                 
    return;

  ps2X.read_gamepad();                  

  if ( Mode == 1 )
  {
    if ( ps2X.Button( PSB_SELECT )   )
    {
      if ( ps2X.ButtonPressed( PSB_START ))
      {
        Mode = 0;
        manual = TRUE;
        FullActRun(0, 1);
        LedFlip();
        for (int i = 0 ; i < 8 ; i++)
        {
          ServoPwmDutyset[i] = 1500;
        }
        ServoSetPluseAndTime( 1, 1500, 1000 );
        ServoSetPluseAndTime( 2, 1500, 1000 );
        ServoSetPluseAndTime( 3, 1500, 1000 );
        ServoSetPluseAndTime( 4, 1500, 1000 );
        ServoSetPluseAndTime( 5, 1500, 1000 );
        ServoSetPluseAndTime( 6, 1500, 1000 );
        return;
      }
    }
    else {
      if (ps2X.ButtonPressed(PSB_START)) { 
        LedFlip();
        FullActRun(0, 1);
        Timer = millis() + 50;             
        return;       
      }
      if (ps2X.ButtonPressed(PSB_PAD_UP)) { 
        LedFlip();
        FullActRun(1, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_PAD_DOWN)) {  
        LedFlip();
        FullActRun(2, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_PAD_LEFT)) {  
        LedFlip();
        FullActRun(3, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_PAD_RIGHT)) { 
        LedFlip();
        FullActRun(4, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_GREEN)) {     
        LedFlip();
        FullActRun(5, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_BLUE)) {       
        LedFlip();
        FullActRun(6, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_PINK)) {       
        LedFlip();
        FullActRun(7, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_RED)) {       
        LedFlip();
        FullActRun(8, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_L1)) {        
        LedFlip();
        FullActRun(9, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_R1)) {        
        LedFlip();
        FullActRun(10, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_L2)) {       
        LedFlip();
        FullActRun(11, 1);
        Timer = millis() + 50;
        return;
      }
      if (ps2X.ButtonPressed(PSB_R2)) {          
        LedFlip();
        // FullActRun(12, 1);
        Timer = millis() + 50;

        //Edison [2021/03/22]-[15:06:42]-Start
        Edison_DBG++;
        if(Edison_DBG == 3) Edison_DBG = 0;
        Uart1SendData('\n');        
        if(Edison_DBG == 0) UART1SendDataPacket("0:No debug",10);
        if(Edison_DBG == 1) UART1SendDataPacket("1:Action Data",13);
        if(Edison_DBG == 2) UART1SendDataPacket("2:PWM Data",10);
        if(Edison_DBG == 3)
        {
          UART1SendDataPacket("Act count",9);
          Uart1SendData('\n');        
          Dump_Index();     
        }   
        Uart1SendData('\n');        

        //Edison [2021/03/22]-[15:06:42]-End   

        return;
      }
      Timer = millis() + 50;
    }
  }

  if (Mode == 0)
  {
    if ( ps2X.Button( PSB_SELECT )   )
    {
      if ( ps2X.ButtonPressed( PSB_START ))
      {
        Mode = 1;
        manual = TRUE;
        LedFlip();
        return;
      }
    }
    else
    {
      if (ps2X.Button(PSB_PAD_LEFT)) { 
        ServoPwmDutyset[6] += 30;
        if (ServoPwmDutyset[6] > 2500)
          ServoPwmDutyset[6] = 2500;
        ServoSetPluseAndTime( 6, ServoPwmDutyset[6], 60 );
      }
      if (ps2X.Button(PSB_PAD_RIGHT)) { 
        ServoPwmDutyset[6] -= 30;
        if (ServoPwmDutyset[6] < 500)
          ServoPwmDutyset[6] = 500;
        ServoSetPluseAndTime( 6, ServoPwmDutyset[6], 60 );
      }
      if (ps2X.Button(PSB_PAD_UP)) { 
        ServoPwmDutyset[5] += 30;
        if (ServoPwmDutyset[5] > 2500)
          ServoPwmDutyset[5] = 2500;
        ServoSetPluseAndTime( 5, ServoPwmDutyset[5], 60 );
      }
      if (ps2X.Button(PSB_PAD_DOWN)) { 
        ServoPwmDutyset[5] -= 30;
        if (ServoPwmDutyset[5] < 500)
          ServoPwmDutyset[5] = 500;
        ServoSetPluseAndTime( 5, ServoPwmDutyset[5], 60 );
      }
      if (ps2X.Button(PSB_GREEN)) { 
        ServoPwmDutyset[4] += 30;
        if (ServoPwmDutyset[4] > 2500)
          ServoPwmDutyset[4] = 2500;
        ServoSetPluseAndTime( 4, ServoPwmDutyset[4], 60 );
      }
      if (ps2X.Button(PSB_BLUE)) { 
        ServoPwmDutyset[4] -= 30;
        if (ServoPwmDutyset[4] < 500)
          ServoPwmDutyset[4] = 500;
        ServoSetPluseAndTime( 4, ServoPwmDutyset[4], 60 );
      }
      if (ps2X.Button(PSB_PINK)) { 
        ServoPwmDutyset[3] += 30;
        if (ServoPwmDutyset[3] > 2500)
          ServoPwmDutyset[3] = 2500;
        ServoSetPluseAndTime( 3, ServoPwmDutyset[3], 60 );
      }
      if (ps2X.Button(PSB_RED)) { 
        ServoPwmDutyset[3] -= 30;
        if (ServoPwmDutyset[3] < 500)
          ServoPwmDutyset[3] = 500;
        ServoSetPluseAndTime( 3, ServoPwmDutyset[3], 60 );
      }
      if (ps2X.Button(PSB_L1)) { 
        ServoPwmDutyset[2] += 30;
        if (ServoPwmDutyset[2] > 2500)
          ServoPwmDutyset[2] = 2500;
        ServoSetPluseAndTime( 2, ServoPwmDutyset[2] , 60 );
      }
      if (ps2X.Button(PSB_R1)) { 
        ServoPwmDutyset[2] -= 30;
        if (ServoPwmDutyset[2] < 500)
          ServoPwmDutyset[2] = 500;
        ServoSetPluseAndTime( 2, ServoPwmDutyset[2], 60 );
      }
      if (ps2X.Button(PSB_L2)) { 
        ServoPwmDutyset[1] += 30;
        if (ServoPwmDutyset[1] > 2500)
          ServoPwmDutyset[1] = 2500;
        ServoSetPluseAndTime( 1, ServoPwmDutyset[1], 60 );
      }
      if (ps2X.Button(PSB_R2)) { 
        ServoPwmDutyset[1] -= 30;
        if (ServoPwmDutyset[1] < 500)
          ServoPwmDutyset[1] = 500;
        ServoSetPluseAndTime( 1, ServoPwmDutyset[1], 60 );
      }
      if (ps2X.Analog(PSS_LX) > 200) 
      {
        ServoPwmDutyset[4] += 30;
        if (ServoPwmDutyset[4] > 2500)
          ServoPwmDutyset[4] = 2500;
        ServoSetPluseAndTime( 4, ServoPwmDutyset[4], 60 );
      }
      if (ps2X.Analog(PSS_LX) < 50)
      {
        ServoPwmDutyset[4] -= 30;
        if (ServoPwmDutyset[4] < 500)
          ServoPwmDutyset[4] = 500;
        ServoSetPluseAndTime( 4, ServoPwmDutyset[4], 60 );
      }
      if (ps2X.Analog(PSS_RY) > 200)
      {
        ServoPwmDutyset[3] += 30;
        if (ServoPwmDutyset[3] > 2500)
          ServoPwmDutyset[3] = 2500;
        ServoSetPluseAndTime( 3, ServoPwmDutyset[3], 60 );
      }
      if (ps2X.Analog(PSS_RY) < 50)
      {
        ServoPwmDutyset[3] -= 30;
        if (ServoPwmDutyset[3] < 500)
          ServoPwmDutyset[3] = 500;
        ServoSetPluseAndTime( 3, ServoPwmDutyset[3], 60 );
      }
      if (ps2X.Analog(PSS_RX) > 200) 
      {
        ServoPwmDutyset[6] += 30;
        if (ServoPwmDutyset[6] > 2500)
          ServoPwmDutyset[6] = 2500;
        ServoSetPluseAndTime( 6, ServoPwmDutyset[6], 60 );
      }
      if (ps2X.Analog(PSS_RX) < 50)
      {
        ServoPwmDutyset[6] -= 30;
        if (ServoPwmDutyset[6] < 500)
          ServoPwmDutyset[6] = 500;
        ServoSetPluseAndTime( 6, ServoPwmDutyset[6], 60 );
      }
      if (ps2X.Analog(PSS_LY) > 200)
      {
        ServoPwmDutyset[5] += 30;
        if (ServoPwmDutyset[5] > 2500)
          ServoPwmDutyset[5] = 2500;
        ServoSetPluseAndTime( 5, ServoPwmDutyset[5], 60 );
      }
      if (ps2X.Analog(PSS_LY) < 50)
      {
        ServoPwmDutyset[5] -= 30;
        if (ServoPwmDutyset[5] < 500)
          ServoPwmDutyset[5] = 500;
        ServoSetPluseAndTime( 5, ServoPwmDutyset[5], 60 );
      }
      if (ps2X.ButtonPressed(PSB_START)) { 
        LedFlip();
        for (int i = 0 ; i < 8 ; i++)
        {
          ServoPwmDutyset[i] = 1500;
        }
        ServoSetPluseAndTime( 1, 1500, 1000 );
        ServoSetPluseAndTime( 2, 1500, 1000 );
        ServoSetPluseAndTime( 3, 1500, 1000 );
        ServoSetPluseAndTime( 4, 1500, 1000 );
        ServoSetPluseAndTime( 5, 1500, 1000 );
        ServoSetPluseAndTime( 6, 1500, 1000 );
        return;
      }
    }
  }
  Timer = millis() + 50;
}

void TaskRun(int ps2_ok)
{
  static bool Ps2State = FALSE;
  uint8 PS2KeyValue;

  TaskTimeHandle();
  CheckBatteryVoltage();

  TaskPCMsgHandle();
//  TaskBLEMsgHandle();
  TaskRobotRun();

  if(analogRead(KEY) == 0)
  {
    keycount++;
  }
 else{
    if (keycount > 3000)
    {
      keycount = 0;
      FullActRun(100,0);
      return;
    }
    else if (keycount > 100)
    {
      keycount = 0;
      FullActRun(100,1);  
    }
  }
  if (ps2_ok == 0)
    ps2Handle();

}
