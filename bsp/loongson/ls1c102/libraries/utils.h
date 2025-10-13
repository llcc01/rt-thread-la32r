#ifndef __TEST_H__
#define __TEST_H__

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __cplusplus
  #define     __I     volatile                /*!< defines 'read only' permissions      */
#else
  #define     __I     volatile const          /*!< defines 'read only' permissions      */
#endif
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */
/* Exported types ------------------------------------------------------------*/
typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short INT16U;
typedef signed short INT16S;
typedef unsigned int INT32U; //32λ
typedef signed int INT32S;
typedef unsigned long long INT64U; //64λ
typedef signed long long INT64S;
typedef float FP32;
typedef float FP64;

typedef unsigned char BOOL;
typedef unsigned char bool;
#define NO_INIT volatile
#define NULL ((void *)0)


#define MAX_UINT (-1U)
typedef unsigned int size_t;
	 
typedef int 			int32_t;
typedef short   		int16_t;

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//defineed in boot/test.h

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef struct _test_entry {
  int (*entry_func)(void);
  char test_name[60];
} test_entry_t;

#define test_entry(fn, name)  \
static test_entry_t __attribute__ ((section(".text.init"))) _this_test = {fn, name};

typedef unsigned int  const ucint32_t;  /* Read Only */
typedef unsigned short const ucint16_t;  /* Read Only */
typedef unsigned char  const ucint8_t;   /* Read Only */
typedef volatile unsigned int vuint32_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned char vuint8_t;
typedef unsigned int uint32_t;
typedef unsigned char uchar_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#define TRUE  1
#define FALSE 0


#define READ_REG32(addr) (*(volatile uint32_t*)addr)
#define WRITE_REG32(addr, value) *(volatile uint32_t*)addr = value

#define VIRT_TO_PHYS(addr) ((uint32_t)addr & 0x7fffffff)

#define read_reg32(var, BASE, OFFSET) var = *(volatile uint32_t*)(BASE+OFFSET)
//    __asm__ __volatile__("ld.w %0, $0, %1":"=r" (var):"imm"(BASE+OFFSET));   // LS: LA compiler does not support __asm__

#define write_reg32(var, BASE, OFFSET) *(volatile uint32_t*)(BASE+OFFSET) = var
//    __asm__ __volatile__("st.w %0, $0, %1"::"r" (var), "imm"(BASE+OFFSET));

#define read_reg32_v(var, ADDR, OFFS) var = *(volatile uint32_t*)(ADDR+OFFS)
//    __asm__ __volatile__("ld.w %0, %1, %2":"=r" (var):"r"(ADDR), "imm"(OFFS));

#define write_reg32_v(var, ADDR, OFFS) *(volatile uint32_t*)(ADDR+OFFS) = var
//    __asm__ __volatile__("st.w %0, %1, %2"::"r" (var), "r"(ADDR), "imm"(OFFS));

#define read_reg8_v(var, ADDR, OFFS) var = *(volatile uint8_t*)(ADDR+OFFS)
//    __asm__ __volatile__("ld.b %0, %1, %2":"=r" (var):"r"(ADDR), "imm"(OFFS));

#define write_reg8_v(var, ADDR, OFFS) *(volatile uint8_t*)(ADDR+OFFS) = var
//    __asm__ __volatile__("st.b %0, %1, %2"::"r" (var), "r"(ADDR), "imm"(OFFS));

#define modify(reg, msb, lsb, v) { reg = (reg & ~( (0xffffffff << lsb) & (0xffffffff >> (31-msb)) ) | (v<<lsb) & ( (0xffffffff << lsb) & (0xffffffff >> (31-msb)) )); }

#define get_fld_shift(reg, msb, lsb) ((reg >> (lsb)) & (~(0xffffffff<<(msb+1))))

//extern uint32_t pmu_intr;
extern uint32_t ext_intr;
//extern uint32_t cpu_intr;
extern uint32_t wdt_caller;
extern uint32_t nmi_caller;
extern int cfg_waitop_sleep;

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


#ifdef __cplusplus
} 
#endif

#endif

