sbit LCD_RS at PORTB2_bit;
sbit LCD_EN at PORTB3_bit;
sbit LCD_D4 at PORTB4_bit;
sbit LCD_D5 at PORTB5_bit;
sbit LCD_D6 at PORTB6_bit;
sbit LCD_D7 at PORTB7_bit;

sbit LCD_RS_Direction at DDB2_bit;
sbit LCD_EN_Direction at DDB3_bit;
sbit LCD_D4_Direction at DDB4_bit;
sbit LCD_D5_Direction at DDB5_bit;
sbit LCD_D6_Direction at DDB6_bit;
sbit LCD_D7_Direction at DDB7_bit;


void setup();
void adc_avgs(unsigned long *V_channel, unsigned long *I_channel);
void lcd_print(unsigned char x_pos, unsigned char y_pos, signed int value, unsigned char type);


void main()
{
     unsigned long v = 0;
     unsigned long i = 0;
     unsigned long p = 0;
     unsigned long i_zero = 0;
     
     setup();
     
     adc_avgs(&v, &i_zero);

     while(1)
     {
         adc_avgs(&v, &i);
         v *= 2.48;
         i = ((4621.8 * (i - i_zero)) / (696.9 - i_zero));
         p = v;
         p *= i;
         p /= 1000.0;
         lcd_print(1, 1, v, 0);
         lcd_print(9, 1, i, 1);
         lcd_print(5, 2, p, 2);
         delay_ms(100);
     };
}


void setup()
{
    CLKPR = 0x80;
    CLKPR = 0x00;
    PORTB=0x00;
    DDRB = 0x00;
    PORTC = 0x00;
    DDRC = 0x00;
    PORTD = 0x00;
    DDRD = 0x0F;
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    OCR0A = 0x00;
    OCR0B = 0x00;
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    ICR1H = 0x00;
    ICR1L = 0x00;
    OCR1AH = 0x00;
    OCR1AL = 0x00;
    OCR1BH = 0x00;
    OCR1BL = 0x00;
    ASSR = 0x00;
    TCCR2A = 0x00;
    TCCR2B = 0x00;
    TCNT2 = 0x00;
    OCR2A = 0x00;
    OCR2B = 0x00;
    EICRA = 0x00;
    EIMSK = 0x00;
    PCICR = 0x00;
    TIMSK0 = 0x00;
    TIMSK1 = 0x00;
    TIMSK2 = 0x00;
    UCSR0B = 0x00;
    ACSR = 0x80;
    ADCSRB = 0x00;
    DIDR1 = 0x00;
    ADCSRA = 0x00;
    SPCR = 0x00;
    TWCR = 0x00;
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    ADC_Init();
}


void adc_avgs(unsigned long *V_channel, unsigned long *I_channel)
{
    unsigned int samples = 512;
    
    unsigned long avg_I = 0;
    unsigned long avg_V = 0;

    while(samples > 0)
    {
        avg_V += adc_read(0);
        delay_us(4);
        avg_I += adc_read(1);
        delay_us(4);
        samples--;
    }

    *V_channel = (avg_V >> 9);
    *I_channel = (avg_I >> 9);
}


void lcd_print(unsigned char x_pos, unsigned char y_pos, signed int value, unsigned char type)
{
     unsigned int tmp = 0;
     unsigned char ch = 0;

     if(value < 0)
     {
         value *= -1;
         LCD_out(y_pos, x_pos, "-");
     }
     else
     {
         LCD_out(y_pos, x_pos, " ");
     }
     
     ch = (value / 1000);
     lcd_chr_cp(ch + 48);
     if(type == 1)
     {
        lcd_chr_cp(46);
     }
     ch = ((value / 100) % 10);
     lcd_chr_cp(ch + 48);
     if((type == 0) || (type == 2))
     {
        lcd_chr_cp(46);
     }
     ch = ((value / 10) % 10);
     lcd_chr_cp(ch + 48);
     ch = (value % 10);
     lcd_chr_cp(ch + 48);
     
     if(type == 0)
     {
         lcd_chr_cp('V');
     }
     else if(type == 1)
     {
         lcd_chr_cp('A');
     }
     else
     {
         lcd_chr_cp('W');
     }
}