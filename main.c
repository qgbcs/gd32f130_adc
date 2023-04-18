#include "gd32f1x0.h"

#include <stdio.h>
int fputc(int ch, FILE* stream){
    usart_data_transmit(USART1,ch);
	while (usart_flag_get(USART1, USART_FLAG_TBE) == RESET) {}
    return ch;
}
void _sys_exit(int x){
	x = x;

}
FILE __stdout;//must or not run main qgb

#define uart1_p(v)  usart_data_transmit(USART1,0x30+v);\
				while (usart_flag_get(USART1, USART_FLAG_TBE) == RESET) {}

#define U USART1
				
void USART_config(uint32_t usart_periph,uint32_t baudval)
{
	rcu_usart_clock_config(CK_APB2);         
	rcu_periph_clock_enable(RCU_USART0) ;    
	rcu_periph_clock_enable(RCU_USART1) ;    //??USART1????
	
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2 );
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
   
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3 );  
			gpio_output_options_set(GPIOA,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	
	
	// nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	
	usart_deinit(usart_periph);    //复位USART
   
	usart_baudrate_set(usart_periph,baudval);   //设置波特率
	usart_enable(usart_periph);                                 //使能USART(UEN=1)
	usart_transmit_config(usart_periph,USART_TRANSMIT_ENABLE);  //使能发送器(TEN=1)
	usart_receive_config(usart_periph,USART_RECEIVE_ENABLE);    //使能接收器(REN=1)

}

#define AVE_NUM    2
#define ADC_Num    6
uint16_t adc_value[ADC_Num];

void rcu_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC); //qgb
	/* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
}

void adc_config(void)
{
	/* gpio config */
	gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, 
	// GPIO_PIN_0 | 
	// GPIO_PIN_1 |
	// GPIO_PIN_2 |											
	// GPIO_PIN_3 | 
	GPIO_PIN_4 
	| GPIO_PIN_5
	| GPIO_PIN_6 
	| GPIO_PIN_7
	// | GPIO_PIN_8
	// | GPIO_PIN_9
	// | GPIO_PIN_10
	);
	
	// gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, 
	// GPIO_PIN_1 |
	// GPIO_PIN_11
	// );
	
	/* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL,ADC_Num);
	
    /* ADC regular channel config */
	// for (uint8_t i=0; i<ADC_Num; i++){
	  // adc_regular_channel_config(i,(uint8_t)i,ADC_SAMPLETIME_13POINT5);
		
	// }
	// adc_regular_channel_config(0,ADC_CHANNEL_0,ADC_SAMPLETIME_13POINT5);
	// adc_regular_channel_config(1,ADC_CHANNEL_1,ADC_SAMPLETIME_13POINT5);
	// adc_regular_channel_config(2,ADC_CHANNEL_2,ADC_SAMPLETIME_13POINT5);
	// adc_regular_channel_config(3,ADC_CHANNEL_3,ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(4,ADC_CHANNEL_4,ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(5,ADC_CHANNEL_5,ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(6,ADC_CHANNEL_6,ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(7,ADC_CHANNEL_7,ADC_SAMPLETIME_13POINT5);
	// adc_regular_channel_config(8,ADC_CHANNEL_9,ADC_SAMPLETIME_13POINT5);
	
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
	
    /* ADC external trigger source config */
    // adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_SWRCST);
	
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
	
	/* ADC discontinuous mode */
    adc_discontinuous_mode_config(ADC_REGULAR_CHANNEL,4);
	
    /* enable ADC interface */
    adc_enable();
	
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
	
    /* ADC DMA function enable */
    adc_dma_mode_enable();
    /* ADC contineous function enable */
   adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);
    /* ADC software trigger enable */
//    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
	
	
}

void dma_config(void)
{
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
	
	/* ADC_DMA_channel configuration */
    dma_deinit(DMA_CH0);
    dma_periph_address_config(DMA_CH0,(uint32_t)(&ADC_RDATA));
    dma_memory_address_config(DMA_CH0,(uint32_t)(&adc_value));
    dma_transfer_direction_config(DMA_CH0,DMA_PERIPHERAL_TO_MEMORY);
    dma_memory_width_config(DMA_CH0,DMA_MEMORY_WIDTH_16BIT);
    dma_periph_width_config(DMA_CH0,DMA_PERIPHERAL_WIDTH_16BIT);
    dma_priority_config(DMA_CH0,DMA_PRIORITY_HIGH);
    dma_transfer_number_config(DMA_CH0,ADC_Num);
    dma_periph_increase_disable(DMA_CH0);
    dma_memory_increase_enable(DMA_CH0);
    dma_circulation_enable(DMA_CH0);
    dma_channel_enable(DMA_CH0);
}

int main(void)
{
	uint8_t i;
	static uint8_t  adc_date_realdy_flag = 0;
	static uint16_t adc_sum_count    = 0;
	static uint16_t adc_date_buffer[AVE_NUM][ADC_Num] = {0};
	static uint32_t adc_sum[ADC_Num]  = {0};
	static uint16_t adc_ave[ADC_Num] = {0};
	
	rcu_config();
    /* ADC configuration */
    adc_config();
	dma_config();
	
	// GPIO_config();
	USART_config(U,57600);
	
    while(1)
	{
		/*adc software trigger*/
		adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
		
		/* get adc value, 8 times*/
		if (adc_sum_count < AVE_NUM)
		{
			/* 9 ch */
			for (i=0; i<ADC_Num; i++)
			{
				adc_date_buffer[adc_sum_count][i]  = adc_value[i];
			}
			adc_sum_count++;
		}
		else
		{
			adc_sum_count = 0;
			
			adc_date_realdy_flag = 1;
		}
		
		/* frist times get complete data after boot */
		if (adc_date_realdy_flag == 1){
			for(i=0; i<ADC_Num; i++)
			{
				/* calculate sum data of ch */
				adc_sum[i] = adc_date_buffer[0][i] + adc_date_buffer[1][i] + adc_date_buffer[2][i] + adc_date_buffer[3][i] 
							+ adc_date_buffer[4][i] + adc_date_buffer[5][i] + adc_date_buffer[6][i] + adc_date_buffer[7][i];
				
				/* Calculated average*/
				adc_ave[i] = adc_sum[i] >> 3;
			}
		}
		//qgb===============
		if (usart_flag_get(U, USART_FLAG_RBNE)== 1) {
			uint16_t n=(uint16_t)usart_data_receive(U);
			usart_data_transmit(U,n+0x20);
			while (usart_flag_get(U, USART_FLAG_TBE) == RESET) {}
			if(n==0x0d){
				printf("=============\r\n");
				
				// usart_data_transmit(USART1,0x0d);// \r
				// while (usart_flag_get(USART1, USART_FLAG_TBE) == RESET) {}
				// usart_data_transmit(USART1,0x0a);// \n
				// while (usart_flag_get(USART1, USART_FLAG_TBE) == RESET) {}
				for (i=0; i<ADC_Num; i++){
					// uart1_p(adc_value[i]);
					printf("%04X ",adc_value[i]);
				}
				
				// printf("\r\n");
				// for (i=0; i<ADC_Num; i++){
					// printf("%04X ",adc_sum[i]);
				// }
				
				// printf("\r\n");
				// for (i=0; i<ADC_Num; i++){
					// printf("%04X ",adc_ave[i]);
				// }
				
				
				
			}
				  
			
		}	
		
	}
}

