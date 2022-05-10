/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013, 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_rtc.h
* Version      : 
* Device(s)    : RL78/I1C
* Tool-Chain   : CCRL
* Description  : This file implements device driver for RTC module.
* Creation Date: 
***********************************************************************************************************************/
#ifndef RTC_H
#define RTC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Format of RTC Control Register 1 (RCR1) 
*/
/* Periodic Interrupt Select (PES) */
#define _00_RTC_INTRTCPRD_CLEAR                            (0x00U) /* INTRTCRPD clear */
#define _60_RTC_INTRTCPRD_CLOCK_0                          (0x60U) /* every 1/256 second */
#define _70_RTC_INTRTCPRD_CLOCK_1                          (0x70U) /* every 1/128 second */
#define _80_RTC_INTRTCPRD_CLOCK_2                          (0x80U) /* every 1/64 second */
#define _90_RTC_INTRTCPRD_CLOCK_3                          (0x90U) /* every 1/32 second */
#define _A0_RTC_INTRTCPRD_CLOCK_4                          (0xA0U) /* every 1/16 second */
#define _B0_RTC_INTRTCPRD_CLOCK_5                          (0xB0U) /* every 1/8 second */
#define _C0_RTC_INTRTCPRD_CLOCK_6                          (0xC0U) /* every 1/4 second */
#define _D0_RTC_INTRTCPRD_CLOCK_7                          (0xD0U) /* every 1/2 second */
#define _E0_RTC_INTRTCPRD_CLOCK_8                          (0xE0U) /* every 1 second */
#define _F0_RTC_INTRTCPRD_CLOCK_9                          (0xF0U) /* every 2 seconds */
/* RTCOUT Output Select (RTCOS) */
#define _00_RTC_1HZ_OUTPUT                                 (0x00U) /* RTCOUT outputs 1 Hz */
#define _08_RTC_64HZ_OUTPUT                                (0x08U) /* RTCOUT outputs 64 Hz */
/* Periodic Interrupt Control (PIE) */
#define _00_RTC_PERIODIC_DISABLE                           (0x00U) /* a periodic interrupt request is disabled */
#define _04_RTC_PERIODIC_ENABLE                            (0x04U) /* a periodic interrupt request is enabled */
/* Alarm Interrupt Control (AIE) */
#define _00_RTC_ALARM_DISABLE                              (0x00U) /* an alarm interrupt request is disabled */
#define _01_RTC_ALARM_ENABLE                               (0x01U) /* an alarm interrupt request is disabled */

/*
    RTC Control Register 2 (RCR2) 
*/
/* Count Mode Select (CNTMD) */
#define _00_RTC_CALENDER_MODE                              (0x00U) /* the calendar count mode */
#define _80_RTC_BINARY_MODE                                (0x80U) /* the binary count mode */
/* Hours Mode (HR24) */
#define _00_RTC_12HOUR_MODE                                (0x00U) /* the RTC operates in 12-hour mode */
#define _40_RTC_24HOUR_MODE                                (0x40U) /* the RTC operates in 24-hour mode */
/* Automatic Adjustment Period Select (AADJP) */
#define _00_RTC_CALENDER_MINUTE                            (0x00U) /* canlendar adjustment period:the prescaler every minute */
#define _20_RTC_CALENDER_10SECONDS                         (0x20U) /* canlendar adjustment period:the prescaler every 10 seconds */
#define _00_RTC_BINARY_32SECONDS                           (0x00U) /* binary adjustment period:the prescaler count value every 32 seconds */
#define _20_RTC_BINARY_8SECONDS                            (0x20U) /* binary adjustment period:the prescaler count value every 8 seconds */
/* Automatic Adjustment Control (AADJE) */
#define _00_RTC_ADJUSTMENT_DISABLE                         (0x00U) /* automatic adjustment is disabled. */
#define _10_RTC_ADJUSTMENT_ENABLE                          (0x10U) /* automatic adjustment is enabled */
/* RTCOUT Output Control (RTCOE) */
#define _00_RTC_RTCOUT_DISABLE                             (0x00U) /* RTCOUT output disabled */
#define _08_RTC_RTCOUT_ENABLE                              (0x08U) /* RTCOUT output enabled */
/* 30-Second Adjustment Control (ADJ30) */
#define _00_RTC_WRITER_INVALID                             (0x00U) /* writing is invalid */
#define _04_RTC_WRITER_EXECUTED                            (0x04U) /* 30-second adjustment is executed */
#define _00_RTC_ADJUSTMENT_COMPLETED                       (0x00U) /* in normal time operation, or 30-second adjustment has completed */
#define _04_RTC_ADJUSTMENT_DUING                           (0x04U) /* during 30-second adjustment */
/* RTC Software Reset Control (RESET) */
#define _00_RTC_RESET_WRITER_INVALID                       (0x00U) /* writing is invalid */
#define _02_RTC_RESET_WRITER_INITIALIZED                   (0x02U) /* the prescaler and the target registers for RTC software reset are initialized */
#define _00_RTC_RESET_COMPLETED                            (0x00U) /* in normal time operation, or an RTC software reset has completed */
#define _02_RTC_RESET_DUING                                (0x02U) /* during an RTC software reset */
/* Prescaler or time counter operation control (START) */
#define _00_RTC_COUNTER_STOP                               (0x00U) /* prescaler and time counter are stopped */
#define _01_RTC_COUNTER_NORMAL                             (0x01U) /* prescaler and time counter operate normally */

/*
    RTC Control Register 3 (RCR3) 
*/
/* time capture event input (RTCICn) enable/disable (RTCICEN) */
#define _00_RTC_INPUT_DISABLE                              (0x00U) /* RTCICn input is disabled */
#define _01_RTC_INPUT_ENABLE                               (0x01U) /* RTCICn input is enabled */

/*
    RTC Control Register 4 (RCR4) 
*/
/* Selection of count source clock (RCKSEL) */
#define _00_RTC_SELECT_FSUB                                (0x00U) /* sub-clock (fSX) */

/*
    RTC Control Register 5 (RCR5) 
*/
/* selection of realtime clock count source (RCR5) */
#define _00_RTC_SOURCE_FSUB                                (0x00U) /* select the sub clock (fSX) as RTC count source */

/*
    Format of Alarm Second register (RSECAR) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_COUNTER_SEC_NOTCOMPARE            (0x00U) /* the register value is not compared with the RSECCNT counter value */
#define _80_RTC_CALENDER_COUNTER_SEC_COMPARE               (0x80U) /* the register value is compared with the RSECCNT counter value */

/*
    Format of Alarm Minute register (RMINAR) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_RMINCNT_MIN_NOTCOMPARE            (0x00U) /* the register value is not compared with the RMINCNT counter value */
#define _80_RTC_CALENDER_RMINCNT_MIN_COMPARE               (0x80U) /* the register value is compared with the RMINCNT counter value */

/*
    Format of Alarm Hour register (RHRAR) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_RHRCNT_HOUR_NOTCOMPARE            (0x00U) /* the register value is not compared with the RHRCNT counter value */
#define _80_RTC_CALENDER_RHRCNT_HOUR_COMPARE               (0x80U) /* the register value is compared with the RHRCNT counter value */
/* PM (PM) */
#define _00_RTC_CALENDER_ALARM_AM                          (0x00U) /* a.m */
#define _40_RTC_CALENDER_ALARM_PM                          (0x40U) /* p.m */

/*
    Format of Alarm Day-of-Week register (RWKAR) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_RWKCNT_WEEK_NOTCOMPARE            (0x00U) /* the register value is not compared with the RWKCNT counter value */
#define _80_RTC_CALENDER_RWKCNT_WEEK_COMPARE               (0x80U) /* the register value is compared with the RWKCNT counter value */
/* Day-of-Week Counting (DAYW) */
#define _00_RTC_CALENDER_ALARM_SUNWEEK                     (0x00U) /* sunday */
#define _01_RTC_CALENDER_ALARM_MONWEEK                     (0x01U) /* monday */
#define _02_RTC_CALENDER_ALARM_TUESWEEK                    (0x02U) /* tuesday */
#define _03_RTC_CALENDER_ALARM_WEDWEEK                     (0x03U) /* wednesday */
#define _04_RTC_CALENDER_ALARM_THURWEEK                    (0x04U) /* thursday */
#define _05_RTC_CALENDER_ALARM_FRIWEEK                     (0x05U) /* friday */
#define _06_RTC_CALENDER_ALARM_SATURWEEK                   (0x06U) /* saturday */

/*
    Format of Alarm Date register (RDAYAR) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_RDAYCNT_DAY_NOTCOMPARE            (0x00U) /* the register value is not compared with the RDAYCNT counter value */
#define _80_RTC_CALENDER_RDAYCNT_DAY_COMPARE               (0x80U) /* the register value is compared with the RDAYCNT counter value */

/*
    Format of Alarm Month register (RMONAR) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_RMONCNT_MONTH_NOTCOMPARE          (0x00U) /* the register value is not compared with the RMONCNT counter value */
#define _80_RTC_CALENDER_RMONCNT_MONTH_COMPARE             (0x80U) /* the register value is compared with the RMONCNT counter value */
/* 10 Months (MON10) */
#define _00_RTC_CALENDER_ALARM_NOTHAVE_10MON               (0x00U) 
#define _10_RTC_CALENDER_ALARM_10MON                       (0x10U) 

/*
    Format of Year Alarm Enable Register (RYRAREN) 
*/
/* ENB (ENB) */
#define _00_RTC_CALENDER_RYRCNT_YEAR_NOTCOMPARE            (0x00U) /* the register value is not compared with the RYRCNT counter value */
#define _80_RTC_CALENDER_RYRCNT_YEAR_COMPARE               (0x80U) /* the register value is compared with the RYRCNT counter value */

/*
    Time Capture Control Register 0 (RTCCR0) 
*/
/* Time Capture Event Input Pin Control (TCEN) */
#define _00_RTC_RTCICN0_DISABLE                            (0x00U) /* the RTCIC0 pin is disabled */
#define _80_RTC_RTCICN0_ENABLE                             (0x80U) /* the RTCIC0 pin is enabled */
/* Time Capture Noise Filter Control (TCNF) */
#define _00_RTC_FILTER0_OFF                                (0x00U) /* the noise filter is off */
#define _20_RTC_FILTER0_COUNT_ON                           (0x20U) /* the noise filter is on (count source) */
#define _30_RTC_FILTER0_COUNT32_ON                         (0x30U) /* the noise filter is on (count source by divided by 32) */
/* Time Capture Status (TCST) */
#define _00_RTC_EVENT0_NOTDETECTED                         (0x00U) /* no event is detected */
#define _04_RTC_EVENT0_DETECTED                            (0x04U) /* an event is detected */
/* Time Capture Control (TCCT) */
#define _00_RTC_CAPTURE0_NOEVENT                           (0x00U) /* no event is detected */
#define _01_RTC_CAPTURE0_RISING                            (0x01U) /* rising edge is detected */
#define _02_RTC_CAPTURE0_FALLING                           (0x02U) /* falling edge is detected */
#define _03_RTC_CAPTURE0_BOTH                              (0x03U) /* both edges are detected */

/*
    Time Capture Control Register 1 (RTCCR1) 
*/
/* Time Capture Event Input Pin Control (TCEN) */
#define _00_RTC_RTCICN1_DISABLE                            (0x00U) /* the RTCIC1 pin is disabled */
#define _80_RTC_RTCICN1_ENABLE                             (0x80U) /* the RTCIC1 pin is enabled */
/* Time Capture Noise Filter Control (TCNF) */
#define _00_RTC_FILTER1_OFF                                (0x00U) /* the noise filter is off */
#define _20_RTC_FILTER1_COUNT_ON                           (0x20U) /* the noise filter is on (count source) */
#define _30_RTC_FILTER1_COUNT32_ON                         (0x30U) /* the noise filter is on (count source by divided by 32) */
/* Time Capture Status (TCST) */
#define _00_RTC_EVENT1_NOTDETECTED                         (0x00U) /* no event is detected */
#define _04_RTC_EVENT1_DETECTED                            (0x04U) /* an event is detected */
/* Time Capture Control (TCCT) */
#define _00_RTC_CAPTURE1_NOEVENT                           (0x00U) /* no event is detected */
#define _01_RTC_CAPTURE1_RISING                            (0x01U) /* rising edge is detected */
#define _02_RTC_CAPTURE1_FALLING                           (0x02U) /* falling edge is detected */
#define _03_RTC_CAPTURE1_BOTH                              (0x03U) /* both edges are detected */

/*
    Time Capture Control Register 2 (RTCCR2) 
*/
/* Time Capture Event Input Pin Control (TCEN) */
#define _00_RTC_RTCICN2_DISABLE                            (0x00U) /* the RTCIC2 pin is disabled */
#define _80_RTC_RTCICN2_ENABLE                             (0x80U) /* the RTCIC2 pin is enabled */
/* Time Capture Noise Filter Control (TCNF) */
#define _00_RTC_FILTER2_OFF                                (0x00U) /* the noise filter is off */
#define _20_RTC_FILTER2_COUNT_ON                           (0x20U) /* the noise filter is on (count source) */
#define _30_RTC_FILTER2_COUNT32_ON                         (0x30U) /* the noise filter is on (count source by divided by 32) */
/* Time Capture Status (TCST) */
#define _00_RTC_EVENT2_NOTDETECTED                         (0x00U) /* no event is detected */
#define _04_RTC_EVENT2_DETECTED                            (0x04U) /* an event is detected */
/* Time Capture Control (TCCT) */
#define _00_RTC_CAPTURE2_NOEVENT                           (0x00U) /* no event is detected */
#define _01_RTC_CAPTURE2_RISING                            (0x01U) /* rising edge is detected */
#define _02_RTC_CAPTURE2_FALLING                           (0x02U) /* falling edge is detected */
#define _03_RTC_CAPTURE2_BOTH                              (0x03U) /* both edges are detected */

/*
    Time Capture Event Input Noise Filter Enable Register (RTCICNFEN) 
*/
/* Use of noise filter of RTCIC2 pin (RTCIC2NF1,RTCIC2NF0) */
#define _00_RTC_RTCIC2_FILTER_OFF                          (0x00U) /* noise filter OFF */
#define _40_RTC_RTCIC2_FILTER_250ON                        (0x40U) /* noise filter ON (count source = 250 ms) */
#define _44_RTC_RTCIC2_FILTER_500ON                        (0x44U) /* noise filter ON (count source = 500 ms) */
/* Use of noise filter of RTCIC1 pin (RTCIC1NF1,RTCIC1NF0) */
#define _00_RTC_RTCIC1_FILTER_OFF                          (0x00U) /* noise filter OFF */
#define _20_RTC_RTCIC1_FILTER_250ON                        (0x20U) /* noise filter ON (count source = 250 ms) */
#define _22_RTC_RTCIC1_FILTER_500ON                        (0x22U) /* noise filter ON (count source = 500 ms) */
/* Use of noise filter of RTCIC0 pin (RTCIC0NF1,RTCIC0NF0) */
#define _00_RTC_RTCIC0_FILTER_OFF                          (0x00U) /* noise filter OFF */
#define _10_RTC_RTCIC0_FILTER_250ON                        (0x10U) /* noise filter ON (count source = 250 ms) */
#define _11_RTC_RTCIC0_FILTER_500ON                        (0x11U) /* noise filter ON (count source = 500 ms) */

/*
    RTC Status Register (RSR) 
*/
/* Periodic interrupt flag (PF) */
#define _00_RTC_PERIODIC_NOINTERRUPT                       (0x00U) /* no periodic interrupt occurs */
#define _04_RTC_PERIODIC_INTERRUPT                         (0x04U) /* periodic interrupt occurs */
/* Carry flag (CF) */
#define _00_RTC_SECOND_NOCARRY                             (0x00U) /* no carry occurs */
#define _02_RTC_SECOND_CARRY                               (0x02U) /* carry occurs */
/* Alarm flag (AF) */
#define _00_RTC_COUNTER_NOTMATCH                           (0x00U) /* the counter does not match the alarm registers */
#define _01_RTC_COUNTER_MATCH                              (0x01U) /* the counter matches the alarm registers */

/*
    Timer Error Adjustment Register (RADJ)
*/
/* Plus-Minus (PMADJ[7:6]) */
#define _40_RTC_TIMER_ERROR_ADJUST_PLUS                    (0x40U)
#define _80_RTC_TIMER_ERROR_ADJUST_MINUS                   (0x80U)
#define _C0_RTC_TIMER_ERROR_ADJUST_ALL                     (0xC0U)
/* Adjustment Value ADJ[5:0] */
#define _00_RTC_TIMER_ERROR_ADJUST_VALUE_NONE              (0x00U)
#define _3F_RTC_TIMER_ERROR_ADJUST_VALUE_ALL               (0x3FU)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _00_RTC_GUARD_VALUE                                (0x00U) /* control of guard function for RTC control register 5 */
#define _00_RTC_CALENDER_ALARM_10SEC                       (0x00U) /* 10-Seconds  */

#define _00_RTC_CALENDER_ALARM_1SEC                        (0x00U) /* 1-Seconds  */

#define _80_RTC_BINARY_ALARM_SEC                           (0x80U) /* Binary Counter 0 */
#define _00_RTC_CALENDER_ALARM_10MIN                       (0x00U) /* 10 Minutes */

#define _00_RTC_CALENDER_ALARM_1MIN                        (0x00U) /* 1 Minutes */
#define _00_RTC_CALENDER_ALARM_10HOUR                      (0x00U) /* 10 Hours */

#define _00_RTC_CALENDER_ALARM_1HOUR                       (0x00U) /* 1 Hours */
#define _00_RTC_CALENDER_ALARM_10DAY                       (0x00U) /* 10 Days */

#define _00_RTC_CALENDER_ALARM_1DAY                        (0x00U) /* 1 Days */
#define _00_RTC_CALENDER_ALARM_1MON                        (0x00U) /* 1 Months */
#define RTC_STARTWAITTIME                                  (1U)
#define RTC_RESETWAITTIME                                  (350U)
#define RTC_WAITTIME                                       (320U)
#define RTC_6CLOCKWAITTIME                                 (496U)
#define RTC_WAITTIME_ARSET                                 (62499U)
#define RTC_WAITTIME_AROFF                                 (15624U)
#define RTC_RSECAR_ENB                                     (0x01U)
#define RTC_RMINAR_ENB                                     (0x02U)
#define RTC_RHRAR_ENB                                      (0x04U)
#define RTC_RWKAR_ENB                                      (0x08U)
#define RTC_RDAYAR_ENB                                     (0x10U)
#define RTC_RMONAR_ENB                                     (0x20U)
#define RTC_RYRAREN_ENB                                    (0x40U)
#define _09_RTC_CLEAR_PERIOD                               (0x09U)

#define RTCCR0_OPERATION_STATE                             (_00_RTC_RTCICN0_DISABLE)
#define RTCCR1_OPERATION_STATE                             (_00_RTC_RTCICN1_DISABLE)
#define RTCCR2_OPERATION_STATE                             (_00_RTC_RTCICN2_DISABLE)

#define RTCCR0_NOISE_FILTER_CONFIG                         (_00_RTC_FILTER0_OFF)
#define RTCCR1_NOISE_FILTER_CONFIG                         (_00_RTC_FILTER1_OFF)
#define RTCCR2_NOISE_FILTER_CONFIG                         (_00_RTC_FILTER2_OFF)

#define RTCCR0_CAPTURE_EVENT_EDGE                          (_00_RTC_CAPTURE0_NOEVENT)
#define RTCCR1_CAPTURE_EVENT_EDGE                          (_00_RTC_CAPTURE1_NOEVENT)
#define RTCCR2_CAPTURE_EVENT_EDGE                          (_00_RTC_CAPTURE2_NOEVENT)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct 
{
    uint8_t rseccnt;
    uint8_t rmincnt;
    uint8_t rhrcnt;
    uint8_t rdaycnt;
    uint8_t rwkcnt;
    uint8_t rmoncnt;
    uint16_t ryrcnt;
} rtc_calendarcounter_value_t;

typedef struct
{
    uint8_t sec_enb;
    uint8_t min_enb;
    uint8_t hr_enb;
    uint8_t day_enb;
    uint8_t wk_enb;
    uint8_t mon_enb;
    uint8_t yr_enb;
} rtc_alarm_enable_t;

typedef struct 
{
    uint8_t rseccnt;
    uint8_t rmincnt;
    uint8_t rhrcnt;
    uint8_t rdaycnt;
    uint8_t rmoncnt;
} rtc_timecapture_value_t;

typedef enum
{
    PES_1_256_SEC = 0x60U,
    PES_1_128_SEC = 0x70U,
    PES_1_64_SEC = 0x80U,
    PES_1_32_SEC = 0x90U,
    PES_1_16_SEC = 0xA0U,
    PES_1_8_SEC = 0xB0U,
    PES_1_4_SEC = 0xC0U,
    PES_1_2_SEC = 0xD0U,
    PES_1_SEC = 0xE0U,
    PES_2_SEC = 0xF0U
} rtc_int_period_t;

typedef enum
{
    RTCOS_FREQ_1HZ = 0x00U,
    RTCOS_FREQ_64HZ = 0x08U,
} rtc_clock_out_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_RTC_Create(void);
void R_RTC_Start(void);
void R_RTC_Stop(void);
void R_RTC_Restart(rtc_calendarcounter_value_t counter_write_val);
void R_RTC_Get_CalendarCounterValue(rtc_calendarcounter_value_t * const counter_read_val);
void R_RTC_Set_CalendarCounterValue(rtc_calendarcounter_value_t counter_write_val);
void R_RTC_Get_TimeCapture0Value(rtc_timecapture_value_t * const counter_read_val);
void R_RTC_Get_TimeCapture1Value(rtc_timecapture_value_t * const counter_read_val);
void R_RTC_Get_TimeCapture2Value(rtc_timecapture_value_t * const counter_read_val);
void R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period);
void R_RTC_Set_ConstPeriodInterruptOff(void);
void R_RTC_Set_ClockOutputOn(rtc_clock_out_t clk_out);
void R_RTC_Set_ClockOutputOff(void);
void R_RTC_Compensate(float32_t ppm);
void R_RTC_StopCompensation(void);
void R_RTC_Set_AlarmValue(rtc_calendarcounter_value_t time_value, rtc_alarm_enable_t control);
void R_RTC_Get_AlarmValue(rtc_calendarcounter_value_t * const time_value, rtc_alarm_enable_t * const control);
static void r_rtc_callback_alarm(void);
 void r_rtc_callback_periodic(void);
/* Start user code for function. Do not edit comment generated here */
unsigned char CheckForSync(volatile unsigned char *ptr, unsigned char val, unsigned char cntr);
void DelayMs(unsigned int d);
/* End user code. Do not edit comment generated here */
#endif /* RTC_H */