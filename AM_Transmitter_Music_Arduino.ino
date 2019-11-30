#define ANTENA_PIN PB3 //Arduino Nano/Uno D11

// 
void setup() {
  Serial.begin(115200);

  //Geração de frequência da portadora
  uint32_t fTransmit = 600;                         //KHz
  DDRB |= (1 << ANTENA_PIN);
  TCCR2A = (0 << COM2A1) + (1 << COM2A0);           //Toggle OC0A 
  TCCR2A |= (1 << WGM21) + (0 << WGM20);            //CTC
  TCCR2B = (0 << CS22) + (0 << CS21) + (1 << CS20); //Sem Prescaling
  OCR2A = F_CPU / (2000 * fTransmit) - 1;
  
  char strbuf[255];
  sprintf(strbuf, "Transmitindo a %d KHz", (F_CPU / (2 * (1 + OCR2A)) / 1000));
  Serial.println(strbuf);

  //PWM Signal generation
  //TCCR1A |= (0 << WGM11) + (1 << WGM10);            //Fast PWM 8 Bit
  //TCCR1A |= (1 << WGM11) + (0 << WGM10);            //Fast PWM 9 Bit
  TCCR1A |= (1 << WGM11) + (1 << WGM10);              //Fast PWM 10 Bit
  TCCR1B = (1 << WGM12);          
  TCCR1B |= (0 << CS12) + (0 << CS11) + (1 << CS10);  //Sem Prescaling
  TIMSK1 = (1 << OCIE1A) + (1 << TOIE1);

  //ADC Settings
  ADMUX = (1 << REFS1) + (1 << REFS0);                  //Referencia interna 1.1V
  ADCSRA = (1 << ADEN) + (1 << ADSC) + (1 << ADATE);    //Auto Trigger abilitado, free running
  ADCSRA |= (1 << ADPS2) + (1 << ADPS1) + (0 << ADPS0); //Divide por 64 -> 18.5k Taxa de Amostragem
  DIDR0 = (1 << ADC0D);
}

ISR(TIMER1_OVF_vect) {
  uint8_t adcl = ADCL;
  uint8_t adch = ADCH;
  OCR1A = (adch << 8) + adcl;
  DDRB |= (1 << ANTENA_PIN);
}

ISR(TIMER1_COMPA_vect) {
  DDRB &= ~(1 << ANTENA_PIN);
}

void loop() {
}
