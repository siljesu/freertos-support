/*
 * Copyright 2019 ,2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTE1 (number 2), J2[20]/UART1_RX_TGTMCU
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_DEBUG_UART_RX_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_DEBUG_UART_RX_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_DEBUG_UART_RX_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                     /* @} */

/*! @name PORTC1 (number 44), J24[6]/LLWU_P6/ADC0_SE15/PUSH_BUTTON2
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SW2_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_SW2_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_SW2_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                           /* @} */

/*! @name PORTC3 (number 46), J1[14]
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_CLKOUT_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_CLKOUT_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_CLKOUT_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                              /* @} */

/*! @name PORTC4 (number 49), J8[P2]/J24[9]/uSD_card_CS
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_SD_CARD_DAT3_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_SD_CARD_DAT3_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_SD_CARD_DAT3_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                    /* @} */

/*! @name PORTD4 (number 61), J2[6]/SPI0_PCS1/LLWU_P14
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_RF_CS_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_RF_CS_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_RF_CS_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                             /* @} */

/*! @name PORTD5 (number 62), J2[12]/BLUE_LED
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_LEDRGB_BLUE_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_LEDRGB_BLUE_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_LEDRGB_BLUE_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name PORTA2 (number 24), J1[8]/GREEN_LED
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_LEDRGB_GREEN_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_LEDRGB_GREEN_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_LEDRGB_GREEN_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
