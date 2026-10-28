/*
 * LPG_Leakage_Detection_System.c
 * Target Microcontroller: ATmega328P
 * Architecture: Bare-metal Embedded C (AVR)
 */

#define F_CPU 16000000UL // 16 MHz Clock Speed
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define GAS_SENSOR_PIN PC0  // MQ-6 Sensor connected to ADC Channel 0
#define BUZZER_PIN     PB5  // Active Buzzer connected to Digital Pin 13
#define GAS_THRESHOLD  400  // Predefined safety limit for gas concentration

// Function Declarations
void ADC_Init(void);
uint16_t ADC_Read(uint8_t ch);
void UART_Init(uint32_t baud);
void UART_Transmit(char data);
void UART_SendString(char* str);
void Send_Emergency_SMS(void);

int main(void) {
    uint16_t gas_level = 0;
    
    // Initialize Subsystems
    ADC_Init();
    UART_Init(9600); // Standard Baud Rate for SIM808 GSM
    
    // Configure BUZZER_PIN as an Output
    DDRB |= (1 << BUZZER_PIN);
    PORTB &= ~(1 << BUZZER_PIN); // Turn off buzzer initially

    while (1) {
        // 1. Data Acquisition (Read raw analog value from MQ-6 sensor)
        gas_level = ADC_Read(GAS_SENSOR_PIN);
        
        // 2. Control Logic & Limit-Evaluation
        if (gas_level > GAS_THRESHOLD) {
            PORTB |= (1 << BUZZER_PIN);  // Trigger local hardware alarm (Buzzer ON)
            Send_Emergency_SMS();         // Execute automated cellular alert system
            _delay_ms(5000);              // Cool down delay to avoid duplicate messages
        } else {
            PORTB &= ~(1 << BUZZER_PIN); // Reset alarm (Buzzer OFF)
        }
        
        _delay_ms(500); // Sample ambient gas data every 500ms
    }
    return 0;
}

// Initialize internal Analog-to-Digital Converter hardware
void ADC_Init(void) {
    // Select Vref = AVcc with external capacitor at AREF pin
    ADMUX = (1 << REFS0);
    // Enable ADC and set Prescaler to 128 (16MHz/128 = 125kHz ADC clock)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// Read raw data from specified ADC channel
uint16_t ADC_Read(uint8_t ch) {
    ch &= 0x07; // Ensure channel bounds (0-7)
    ADMUX = (ADMUX & 0xF0) | ch; // Clear old channel and set new channel
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion complete flag
    return (ADC);
}

// Initialize UART Peripheral for GSM communication
void UART_Init(uint32_t baud) {
    uint16_t ubrr_value = (F_CPU / (16 * baud)) - 1;
    UBRR0H = (unsigned char)(ubrr_value >> 8);
    UBRR0L = (unsigned char)ubrr_value;
    UCSR0B = (1 << TXEN0); // Enable transmitter transmission line
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8-bit data, 1 stop bit
}

// Send single character via UART Tx register
void UART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Put data into buffer, sends the data
}

// Send complete string data packet via UART Tx
void UART_SendString(char* str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}

// Sequence to broadcast automated SMS alert via SIM808 AT Commands
void Send_Emergency_SMS(void) {
    UART_SendString("AT\r\n"); // Check communication stability
    _delay_ms(200);
    UART_SendString("AT+CMGF=1\r\n"); // Set GSM module to Text Mode
    _delay_ms(200);
    UART_SendString("AT+CMGS=\"+917981801543\"\r\n"); 
    _delay_ms(200);
    UART_SendString("ALERT: Critical LPG Gas Leakage Detected! Location Coordinates: Lat 17.3850, Lon 78.4867.\r\n");
    _delay_ms(200);
    UART_Transmit(26); // Send Ctrl+Z character to transmit text package over network
    _delay_ms(1000);
}
