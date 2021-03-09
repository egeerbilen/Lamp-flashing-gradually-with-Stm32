#include "stm32f10x.h"

int par = 0;
int main()
{
	RCC->APB2ENR |= (1 << 2) | 1;
	GPIOA->CRH &= 0XFFFFF0FF;
	GPIOA->CRH |= 0X00000B00;

	GPIOA->CRL |= (1 << 3);
	GPIOA->CRL &= ~((1 << 2) | (1 << 1) | (1 << 0));
	GPIOA->CRL |= (1 << 7);
	GPIOA->CRL &= ~((1 << 4) | (1 << 5) | (1 << 6));

	RCC->APB2ENR |= (1 << 11); //timer aktif etme
	TIM1->CCMR2 |= (6 << 4);   //PA_10 için pwm mode 1
	TIM1->CCER |= (1 << 8);	   //çikis ayarlandi (kanal 1 için)
	TIM1->BDTR |= (1 << 15);
	TIM1->ARR = 3600; //20kh
	TIM1->PSC = 9;	  //(ARR*(PSC+1))/72000000
	TIM1->CR1 |= 1;	  //timer aktif

	while (1)
	{
		if ((GPIOA->IDR & 1) && (GPIOA->IDR & (1 << 1)))
		{
			TIM1->CCR3 = 1800; //parlaklik
		}
		else
		{
			if (GPIOA->IDR & 1)
			{
				par = par + 360;
				TIM1->CCR3 = par; //parlaklik
				for (int j = 0; j < 3000000; j++)
					;
			}
			else if (!(GPIOA->IDR & 1))
			{
				TIM1->CCR3 = par; //parlaklik
			}

			if (GPIOA->IDR & (1 << 1))
			{
				par = par - 360;
				TIM1->CCR3 = par; //parlaklik
				for (int j = 0; j < 3000000; j++)
					;
			}
			else if (!(GPIOA->IDR & (1 << 1)))
			{
				TIM1->CCR3 = par; //parlaklik
			}
		}
	}
}
