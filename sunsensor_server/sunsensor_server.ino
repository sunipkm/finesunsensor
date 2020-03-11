void setup()
{
    Serial.begin(115200);

    cli(); // clear interrupts
    //set timer1 interrupt at 10Hz
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0;  //initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 6249; // = (16*10^6) / (10*256) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 256 prescaler
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // enable interrupts
    sei();
}

ISR(TIMER1_COMPA_vect)
{   //timer1 interrupt 10Hz reads the sun sensor
    unsigned short a0 = analogRead(A0);
    unsigned short a1 = analogRead(A1);
    unsigned short a2 = analogRead(A2);
    unsigned short a3 = analogRead(A3);
    Serial.print(a0);
    Serial.print(",");
    Serial.print(a1);
    Serial.print(",");
    Serial.print(a2);
    Serial.print(",");
    Serial.print(a3);
    Serial.print("\n");
    Serial.flush();
}

void loop()
{
    // do nothing
}