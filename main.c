/*
 * LPG_Leakage_Detection.c
 *
 * Author: Balija Neelasri
 * Target: ATmega328P
 * Architecture: Bare-Metal Embedded C
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define GAS_SENSOR_PIN PC0
#define BUZZER_PIN PB5
#define GAS_THRESHOLD 400

// Function declarations
void ADC_Init(void);
uint16_t ADC_Read(uint8_t ch);
void UART_Init(uint32_t baud);
void UART_Transmit(uint8_t data);
void UART_SendString(const char* str);
void Send_Emergency_SMS(void);

int main(void)
{
    uint16_t gas_level = 0;
    uint8_t alert_sent = 0;

    ADC_Init();
    UART_Init(9600);

    DDRB |= (1 << BUZZER_PIN);
    PORTB &= ~(1 << BUZZER_PIN);

    while (1)
    {
        gas_level = ADC_Read(GAS_SENSOR_PIN);

        if (gas_level > GAS_THRESHOLD)
        {
            PORTB |= (1 << BUZZER_PIN);

            if (alert_sent == 0)
            {
                Send_Emergency_SMS();
                alert_sent = 1;
            }
        }
        else
        {
            PORTB &= ~(1 << BUZZER_PIN);
            alert_sent = 0;
        }

        _delay_ms(500);
    }

    return 0;
}

void ADC_Init(void)
{
    ADMUX = (1 << REFS0);

    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}

uint16_t ADC_Read(uint8_t ch)
{
    ch &= 0x07;

    ADMUX = (ADMUX & 0xF0) | ch;

    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC))
    {
    }

    return ADC;
}

void UART_Init(uint32_t baud)
{
    uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1;

    UBRR0H = (unsigned char)(ubrr_value >> 8);
    UBRR0L = (unsigned char)ubrr_value;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }

    UDR0 = data;
}

void UART_SendString(const char* str)
{
    while (*str)
    {
        UART_Transmit(*str++);
    }
}

void Send_Emergency_SMS(void)
{
    UART_SendString("AT\r\n");
    _delay_ms(200);

    UART_SendString("AT+CMGF=1\r\n");
    _delay_ms(200);

    UART_SendString("AT+CMGS=\"+91XXXXXXXXXX\"\r\n");
    _delay_ms(200);

    UART_SendString(
        "ALERT: Critical LPG Gas Leakage Detected! "
        "Please check the location manually.\r\n"
    );

    _delay_ms(200);

    UART_Transmit(26);
    _delay_ms(1000);
}