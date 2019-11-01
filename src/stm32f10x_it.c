/**
 ******************************************************************************
 * @file    USART/HyperTerminal_HwFlowControl/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "_write.c"

uint64_t ppmChannel0 = 0, ppmChannel1;
uint32_t left, right;

extern uint64_t timer;
extern uint16_t Timer3Period;
extern uint8_t enableL, enableR;
extern uint8_t autonomous_mode;
extern uint32_t adc_init_status;

/** @addtogroup STM32F10x_StdPeriph_Examples
 * @{
 */

/** @addtogroup USART_HyperTerminal_HwFlowControl
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**0
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	timer++;
}

/**
 * @brief  This function handles External line 0 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
	int max_value = Timer3Period - 60;
	if (EXTI_GetITStatus(EXTI_Line0) != RESET && !autonomous_mode)
	{
		if (ppmChannel0 == 0)
		{
			ppmChannel0 = timer;
		}
		else
		{
			int actual = timer;
			volatile int diff = actual - ppmChannel0;

			// Turn right
			if (diff > 1750 && diff < 2450 && enableR && ( ((diff - 1750) / 600.0) > 0.1 ))
			{
				volatile double valor = Timer3Period * ((diff - 1750) / 600.0);
				TIM_SetCompare1(TIM3, (valor >= Timer3Period) ? (Timer3Period - 1) :  valor);
				TIM_SetCompare2(TIM3, 0);
			}
			// Turn left
			else if (diff < 1730 && diff > 1000 && enableL && ( ((1730 - diff) / 730.0) > 0.1 ) )
			{
				volatile double valor = Timer3Period * ((1730 - diff) / 730.0);
				TIM_SetCompare2(TIM3, (valor >= Timer3Period) ? (Timer3Period-1) : valor);
				TIM_SetCompare1(TIM3, 0);
			}

			ppmChannel0 = actual;
		}

		/* Clear the  EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/*
* @brief  This function handles External line 0 interrupt request.
* @param  None
* @retval None
*/
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET && !autonomous_mode)
	{
		if (ppmChannel1 == 0)
		{
			ppmChannel1 = timer;
		}
		else
		{
			int actual = timer;
			volatile int diff = actual - ppmChannel1;

			if (diff < 2380 && diff > 1780) {
				 TIM_SetCompare3(TIM3, Timer3Period * ((diff - 1787) / 570.0));
			 }

			ppmChannel1 = actual;
		}

		/* Clear the  EXTI line 1 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

/*
* @brief  This function handles DMA1 interrupt.
* @param  None
* @retval None
*/
void DMA1_Channel1_IRQHandler(void)
{
	//Test on DMA1 Channel1 Transfer Complete interrupt
	if (DMA_GetITStatus(DMA1_IT_TC1))
	{
		adc_init_status = 1;
		//Clear DMA1 interrupt pending bits
		DMA_ClearITPendingBit(DMA1_IT_GL1);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
