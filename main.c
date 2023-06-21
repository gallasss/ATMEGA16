#define F_CPU 8000000L
#include <avr/io.h>

void ADC_init()
{
	ADMUX = (1 << REFS0) | (7 & 0x07); // Memilih saluran ADC 7 dan menggunakan AVCC sebagai tegangan referensi
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Mengaktifkan ADC dengan preskaler 128
}

uint16_t pinADC(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Memilih saluran ADC
	ADCSRA |= (1 << ADSC); // Memulai konversi ADC
	while (ADCSRA & (1 << ADSC)); // Menunggu hingga konversi selesai
	return ADC;
}

void PWM_init()
{
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10) | (1 << WGM11); // Mode Fast PWM pada PD4 dan PD5
	TCCR1B = (1 << WGM12) | (1 << CS10); // Mode Fast PWM, preskaler 1
	DDRD |= (1 << PD4) | (1 << PD5); // Mengatur PD4 dan PD5 sebagai output
}

void pwmduty(uint8_t dutyCyclePD4, uint8_t dutyCyclePD5)
{
	OCR1A = dutyCyclePD5; // Mengatur siklus tugas dengan mengubah OCR1A
	OCR1B = dutyCyclePD4; // Mengatur siklus tugas dengan mengubah OCR1B
}

int main(void)
{
	DDRD |= (1 << PD4) | (1 << PD5); // Mengatur PD4 dan PD5 sebagai output
	ADC_init(); // Menginisialisasi ADC
	PWM_init();

	while (1)
	{
		uint16_t adcValue = pinADC(7); // Membaca nilai ADC dari saluran 7 (PA7)

		if (adcValue < 512) // Jika nilai ADC kurang dari 512 (gelap)
		{
			pwmduty(255, 0); // Mengatur siklus tugas PD4 menjadi maksimum (100%) dan PD5 menjadi 0% (mati)
		}
		else // Jika nilai ADC lebih besar atau sama dengan 512 (terang)
		{
			pwmduty(0, 255); // Mengatur siklus tugas PD4 menjadi 0% (mati) dan PD5 menjadi maksimum (100%)
		}
	}
}
