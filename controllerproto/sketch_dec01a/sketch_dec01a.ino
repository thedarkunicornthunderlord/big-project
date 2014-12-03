void setup()
int Data = 7;
int Strobe = 6;
int Clock = 5;

void setup()
{
   pinMode(Data,INPUT);    
   digitalWrite(Data,HIGH);
   pinMode(Strobe,OUTPUT);  
   digitalWrite(Strobe,HIGH);
   pinMode(Clock,OUTPUT);   
   digitalWrite(Clock,HIGH);
}
void loop ()
{
  
}

void ()
{
  ...
  digitalWrite(Strobe,LOW);
  buttons  = readNesBit(buttonA);
  buttons |= readNesBit(buttonB);
  buttons |= readNesBit(buttonSelect);
  buttons |= readNesBit(buttonStart);
  dpad  = readNesBit(0x01);
  dpad |= readNesBit(0x02);
  dpad |= readNesBit(0x04);
  dpad |= readNesBit(0x08);
  digitalWrite(Strobe,HIGH);
}
static byte readNesBit(byte button)
{
  const bool b = (digitalRead(NES_DATA)==LOW);
  digitalWrite(NES_CLOCK,LOW);
  digitalWrite(NES_CLOCK,HIGH);
  return (b)? button: 0;
}

