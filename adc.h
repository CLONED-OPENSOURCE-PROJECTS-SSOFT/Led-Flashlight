#define ADMUX_ADC0 0x00
#define ADMUX_ADC1 0x01
#define ADMUX_ADC2 0x02
#define ADMUX_ADC3 0x03
#define ADMUX_ADC4 0x04
#define ADMUX_ADC5 0x05
#define ADMUX_ADC6 0x06
#define ADMUX_ADC7 0x07
#define ADMUX_ADC8 0x08
#define ADMUX_ADC9 0x09
#define ADMUX_ADC10 0x0A
#define SLEEP 1
#define BACKGROUND 2
#define WAIT 3
extern unsigned int adc[];
void adc_enable(unsigned char admux,char mode);
void adc_disable(void);
