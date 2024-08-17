/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-07-20     thread-liu        the first version
 */

#include <rtthread.h>
#if defined(BSP_USING_GBE)

#include "drv_eth.h"
#include "exception.h"
#include <stdint.h>

#ifdef RT_USING_LWIP

#include <netif/ethernetif.h>

#endif /* RT_USING_LWIP */

// Register definition
typedef struct {
  union {
    unsigned int ETH_TX_DATA[384]; /* 0x000~0x5ff transmit data registers */
    unsigned int ETH_RX_DATA[384]; /* 0x000~0x5ff receive data registers */
  };
  unsigned int ETH_TX_LENGTH; /* 0x600 transmit data length, bit[10:0]        */
  unsigned int ETH_TX_EN;     /* 0x604 transmit enable, bit[0]                */
  unsigned int ETH_TX_FAIL;   /* 0x608 transmit fail, bit[2,1,0]              */
  unsigned int ETH_TX_IS;     /* 0x60c transmit interrupt status, bit[0]      */
  unsigned int ETH_TX_IC;     /* 0x610 transmit interrupt clear, bit[0]       */
  unsigned int ETH_TX_IE;     /* 0x614 transmit interrupt enable, bit[0]      */
  unsigned int RESERVED_1[26]; /* 0x618~0x67f reserved */
  unsigned int ETH_RX_LENGTH; /* 0x680 receive data length                    */
  unsigned int ETH_RX_IS;     /* 0x684 receive interrupt status, bit[0]       */
  unsigned int ETH_RX_IE;     /* 0x688 receive interrupt enable, bit[0]       */
  unsigned int ETH_RX_IC;     /* 0x68c receive interrupt clear, bit[0]        */
  unsigned int RESERVED_2[28]; /* 0x690~0x6ff reserved */
  unsigned int MIIM_OP_MODE;  /* 0x700 MIIM operation mode, bit[0]            */
  unsigned int MIIM_PHY_ADDR; /* 0x704 MIIM physical address, bit[4:0]        */
  unsigned int MIIM_REG_ADDR; /* 0x708 MIIM register address, bit[4:0]        */
  unsigned int MIIM_WR_DATA;  /* 0x70c MIIM write data, bit[15:0]             */
  unsigned int MIIM_RD_DATA;  /* 0x710 MIIM read data, bit[15:0]              */
  unsigned int MIIM_IS;       /* 0x714 MIIM interrupt status, bit[1,0]        */
  unsigned int MIIM_IE;       /* 0x718 MIIM interrupt enable, bit[1,0]        */
  unsigned int MIIM_IC;       /* 0x71c MIIM interrupt clear, bit[1,0]         */
  unsigned int MIIM_OP_EN;    /* 0x720 MIIM operation enable, bit[0]          */
  unsigned int ETH_MODE;      /* 0x724 Ethernet operation mode, bit[2,1,0]    */
} ETHERNET_RegDef;

// Memory map definition
#define ETHERNET_BASE 0xa1100000

// Device definitions
#define ETHERNET ((ETHERNET_RegDef *)ETHERNET_BASE)

// Bit definition
#define BIT0 (1UL << 0)

#define ETH_TX_LATE_COL (1UL << 2)
#define ETH_TX_EXCESSIVE_COL (1UL << 1)
#define ETH_TX_FAIL_FAIL (1UL << 0)

#define MIIM_OP_END_IS (1UL << 1)
#define MIIM_RD_DATA_VALID_IS (1UL << 0)

#define MIIM_OP_END_IE (1UL << 1)
#define MIIM_RD_DATA_VALID_IE (1UL << 0)

#define MIIM_OP_END_IC (1UL << 1)
#define MIIM_RD_DATA_VALID_IC (1UL << 0)

#define ETH_DUPLEX_MODE (1UL << 2)

#define ETH_FULL_DUPLEX_100M 0UL
#define ETH_FULL_DUPLEX_1000M 1UL
#define ETH_FULL_DUPLEX_10M 2UL
#define ETH_HALF_DUPLEX_100M 4UL
#define ETH_HALF_DUPLEX_10M 6UL

#define ETH_RX 0
#define ETH_TX 1

#define IRQ_ETH S_CSR_ESTAT_HW3
#define VECTOR_ETH_SOURCE IRQ_TO_VECTOR(IRQ_ETH)

/* OUI 00-80-E1 STMICROELECTRONICS. */
static rt_uint8_t mac_addr[6] = {0x00, 0x80, 0xE1, 0x11, 0xcc, 0x01};

#define DRV_DEBUG
// #define ETH_RX_DUMP
// #define ETH_TX_DUMP
#define LOG_TAG "drv.emac"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#undef PHY_FULL_DUPLEX
#undef PHY_HALF_DUPLEX
#define PHY_LINK (1 << 0)
#define PHY_10M (1 << 1)
#define PHY_100M (1 << 2)
#define PHY_1000M (1 << 3)
#define PHY_FULL_DUPLEX (1 << 4)
#define PHY_HALF_DUPLEX (1 << 5)

#define MAX_ADDR_LEN 6
rt_base_t level;

/* transmit buffer */
static rt_uint8_t txBuffer[ETH_TXBUFNB][ETH_TX_BUF_SIZE];
/* Receive buffer */
static rt_uint8_t rxBuffer[ETH_RXBUFNB][ETH_RX_BUF_SIZE];
/* Transmit DMA descriptors */
static TxDmaDesc txDmaDesc[ETH_TXBUFNB];
/* Receive DMA descriptors */
static RxDmaDesc rxDmaDesc[ETH_RXBUFNB];

/* Current transmit descriptor */
static rt_uint8_t txIndex = 0;
/* Current receive descriptor */
static rt_uint8_t rxIndex = 0;

/* eth rx event */
static struct rt_event rx_event = {0};

#define ETH_TIME_OUT (RT_TICK_PER_SECOND)

struct rt_bsp_eth {
#ifdef RT_USING_LWIP
  /* inherit from ethernet device */
  struct eth_device parent;
#endif
#ifndef PHY_USING_INTERRUPT_MODE
  rt_timer_t poll_link_timer;
#endif
  /* interface address info, hw address */
  rt_uint8_t dev_addr[MAX_ADDR_LEN];
  /* eth speed */
  rt_uint32_t eth_speed;
  /* eth duplex mode */
  rt_uint32_t eth_mode;
};
static struct rt_bsp_eth bsp_eth_device = {0};

#if defined(ETH_RX_DUMP) || defined(ETH_TX_DUMP)
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
static void dump_hex(const rt_uint8_t *ptr, rt_size_t buflen) {
  unsigned char *buf = (unsigned char *)ptr;
  int i, j;

  for (i = 0; i < buflen; i += 16) {
    rt_kprintf("%08X: ", i);

    for (j = 0; j < 16; j++) {
      if (i + j < buflen) {
        rt_kprintf("%02X ", buf[i + j]);
      } else {
        rt_kprintf("   ");
      }
    }
    rt_kprintf(" ");

    for (j = 0; j < 16; j++) {
      if (i + j < buflen) {
        rt_kprintf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
      }
    }
    rt_kprintf("\n");
  }
}
#endif

static rt_err_t phy_write_reg(rt_uint8_t phy_addr, rt_uint8_t reg_addr,
                              rt_uint16_t reg_value) {
  rt_uint32_t temp;
  volatile rt_uint32_t tickstart = 0;

  // ETHERNET->MIIM_IE = 0x03;
  ETHERNET->MIIM_OP_MODE = 1;
  ETHERNET->MIIM_PHY_ADDR = phy_addr;
  ETHERNET->MIIM_REG_ADDR = reg_addr;
  ETHERNET->MIIM_WR_DATA = reg_value;
  ETHERNET->MIIM_OP_EN = 1;
  /* Wait for the write to complete */
  tickstart = rt_tick_get();
  while ((ETHERNET->MIIM_IS & MIIM_OP_END_IS) == 0) {
    /* judge timeout */
    if ((rt_tick_get() - tickstart) > ETH_TIME_OUT) {
      LOG_E("PHY write reg %02x date %04x timeout!", reg_addr, reg_value);
      return -RT_ETIMEOUT;
    }
  }

  /* Clear the interrupt */
  ETHERNET->MIIM_IC = MIIM_OP_END_IC;

  return RT_EOK;
}

static rt_err_t phy_write_paged_reg(rt_uint8_t phy_addr, rt_uint16_t page,
                                    rt_uint8_t reg_addr,
                                    rt_uint16_t reg_value) {
  rt_uint32_t status = 0;
  volatile rt_uint32_t tickstart = 0;

  /* Write the page number */
  status = phy_write_reg(phy_addr, RTL8211F_PAGSR, page);
  if (status != RT_EOK) {
    LOG_E("PHY write page %d failed!", page);
    return -RT_ERROR;
  }

  /* Write the register value */
  status = phy_write_reg(phy_addr, reg_addr, reg_value);
  if (status != RT_EOK) {
    LOG_E("PHY write reg %02x date %04x failed!", reg_addr, reg_value);
    return -RT_ERROR;
  }

  /* Restore the page number */
  status = phy_write_reg(phy_addr, RTL8211F_PAGSR, 0);
  if (status != RT_EOK) {
    LOG_E("PHY restore page failed!");
    return -RT_ERROR;
  }

  return RT_EOK;
}

static rt_ssize_t phy_read_reg(rt_uint8_t phy_addr, rt_uint8_t reg_addr) {
  rt_uint16_t reg_value = 0;
  rt_uint32_t status = 0;
  volatile rt_uint32_t tickstart = 0;

  ETHERNET->MIIM_OP_MODE = 0;
  ETHERNET->MIIM_PHY_ADDR = phy_addr;
  ETHERNET->MIIM_REG_ADDR = reg_addr;
  ETHERNET->MIIM_OP_EN = 1;

  tickstart = rt_tick_get();
  while (((ETHERNET->MIIM_IS & MIIM_OP_END_IS) == 0) ||
         ((ETHERNET->MIIM_IS & MIIM_RD_DATA_VALID_IS) == 0)) {
    /* judge timeout */
    if ((rt_tick_get() - tickstart) > ETH_TIME_OUT) {
      LOG_E("PHY read reg %02x timeout!", reg_addr);
      return -RT_ETIMEOUT;
    }
  }

  /* Get register value */
  reg_value = ETHERNET->MIIM_RD_DATA;

  /* Clear the interrupt */
  ETHERNET->MIIM_IC = MIIM_OP_END_IC | MIIM_RD_DATA_VALID_IC;

  return reg_value;
}

static rt_err_t update_mac_mode(rt_uint32_t eth_speed, rt_uint32_t eth_mode) {
  rt_uint32_t status;

  if (eth_speed == PHY_1000M) {
    ETHERNET->ETH_MODE = ETH_FULL_DUPLEX_1000M;
  } else if ((eth_speed == PHY_100M) && (eth_mode == PHY_FULL_DUPLEX)) {
    ETHERNET->ETH_MODE = ETH_FULL_DUPLEX_100M;
  } else if ((eth_speed == PHY_100M) && (eth_mode == PHY_HALF_DUPLEX)) {
    ETHERNET->ETH_MODE = ETH_HALF_DUPLEX_100M;
  } else if ((eth_speed == PHY_10M) && (eth_mode == PHY_FULL_DUPLEX)) {
    ETHERNET->ETH_MODE = ETH_FULL_DUPLEX_10M;
  } else if ((eth_speed == PHY_10M) && (eth_mode == PHY_HALF_DUPLEX)) {
    ETHERNET->ETH_MODE = ETH_HALF_DUPLEX_10M;
  } else {
    LOG_E("Invalid speed and duplex mode!");
    return -RT_ERROR;
  }

  return RT_EOK;
}

static void HAL_ETH_MspInit(void) {
  ETHERNET->ETH_TX_LENGTH = 0;
  ETHERNET->ETH_TX_EN = 0;
  ETHERNET->ETH_TX_FAIL = 0;
  ETHERNET->ETH_TX_IE = 0;
  ETHERNET->ETH_RX_IE = 1;
  ETHERNET->MIIM_OP_MODE = 0;
  ETHERNET->MIIM_PHY_ADDR = 0;
  ETHERNET->MIIM_REG_ADDR = 0;
  ETHERNET->MIIM_WR_DATA = 0;
  ETHERNET->MIIM_IE = 0;
  ETHERNET->MIIM_OP_EN = 0;
  ETHERNET->ETH_MODE = 0;
}

#ifdef RT_USING_LWIP
static rt_err_t rt_bsp_eth_init(rt_device_t dev) {
  RT_ASSERT(dev != RT_NULL);
#else
int rt_bsp_eth_init(void) {
#endif

  rt_ssize_t status, i;
  volatile rt_uint32_t tickstart = 0;
  rt_uint8_t *macAddr = &bsp_eth_device.dev_addr[0];

  /* Initialize TX descriptor index */
  txIndex = 0;
  for (rt_size_t i = 0; i < ETH_TXBUFNB; i++) {
    txDmaDesc[i].valid = 0;
  }
  /* Initialize RX descriptor index */
  rxIndex = 0;
  for (rt_size_t i = 0; i < ETH_RXBUFNB; i++) {
    rxDmaDesc[i].valid = 0;
  }

  HAL_ETH_MspInit();

  /* Reset PHY transceiver */
  phy_write_reg(RTL8211F_PHY_ADDR, RTL8211F_BMCR, RTL8211F_BMCR_RESET);

  status = phy_read_reg(RTL8211F_PHY_ADDR, RTL8211F_BMCR);
  /* Wait for the reset to complete */
  tickstart = rt_tick_get();
  while (status & RTL8211F_BMCR_RESET) {
    if ((rt_tick_get() - tickstart) > ETH_TIME_OUT) {
      LOG_E("PHY software reset timeout!");
      return -RT_ETIMEOUT;
    } else {
      status = phy_read_reg(RTL8211F_PHY_ADDR, RTL8211F_BMCR);
    }
  }

  /* The PHY will generate interrupts when link status changes are detected */
  phy_write_reg(RTL8211F_PHY_ADDR, RTL8211F_INER,
                RTL8211F_INER_AN_COMPLETE | RTL8211F_INER_LINK_STATUS);

  /* Set LED mode */
  phy_write_paged_reg(RTL8211F_PHY_ADDR, 0x0d04, RTL8211F_PAGE_LCR, 0x6d00);
  phy_write_paged_reg(RTL8211F_PHY_ADDR, 0x0d04, RTL8211F_PAGE_EEELCR, 0);

  return RT_EOK;
}
#ifndef RT_USING_LWIP
INIT_APP_EXPORT(rt_bsp_eth_init);
#endif

static rt_err_t rt_bsp_eth_open(rt_device_t dev, rt_uint16_t oflag) {
  LOG_D("emac open");
  return RT_EOK;
}

static rt_err_t rt_bsp_eth_close(rt_device_t dev) {
  LOG_D("emac close");
  return RT_EOK;
}

static rt_ssize_t rt_bsp_eth_read(rt_device_t dev, rt_off_t pos, void *buffer,
                                  rt_size_t size) {
  LOG_D("emac read");
  rt_set_errno(-RT_ENOSYS);
  return 0;
}

static rt_ssize_t rt_bsp_eth_write(rt_device_t dev, rt_off_t pos,
                                   const void *buffer, rt_size_t size) {
  LOG_D("emac write");
  rt_set_errno(-RT_ENOSYS);
  return 0;
}

#ifdef RT_USING_LWIP

static rt_err_t rt_bsp_eth_control(rt_device_t dev, int cmd, void *args) {
  switch (cmd) {
  case NIOCTL_GADDR:
    /* get mac address */
    if (args) {
      rt_memcpy(args, bsp_eth_device.dev_addr, 6);
    } else {
      return -RT_ERROR;
    }
    break;

  default:
    break;
  }

  return RT_EOK;
}

rt_err_t rt_bsp_eth_tx(rt_device_t dev, struct pbuf *p) {
  rt_uint32_t framelen = 0;
  struct pbuf *q = RT_NULL;
  volatile rt_uint32_t tickstart = 0;

  /* Copy user data to the transmit buffer */
  for (q = p; q != NULL; q = q->next) {
    /* Make sure the current buffer is available for writing */
    if (txDmaDesc[txIndex].valid) {
      LOG_D("buffer not valid");
      return ERR_USE;
    }

    level = rt_hw_interrupt_disable();
    rt_memcpy(&txBuffer[txIndex][framelen], q->payload, q->len);
    framelen += q->len;
    rt_hw_interrupt_enable(level);

    /* Check the frame length */
    if (framelen > ETH_TX_BUF_SIZE - 1) {
      LOG_D(" tx buffer frame length over : %d", framelen);
      return ERR_USE;
    }
  }

#ifdef ETH_TX_DUMP
  rt_kprintf("Tx dump, len= %d\r\n", framelen);
  dump_hex(txBuffer[txIndex], framelen);
#endif

  // /* Set the start address of the buffer */
  // txDmaDesc[txIndex].tdes0 = (rt_uint32_t)txBuffer[txIndex];
  // /* Write the number of bytes to send */
  // txDmaDesc[txIndex].tdes2 = ETH_TDES2_IOC | (framelen & ETH_TDES2_B1L);
  // /* Give the ownership of the descriptor to the DMA */
  // txDmaDesc[txIndex].tdes3 = ETH_TDES3_OWN | ETH_TDES3_FD | ETH_TDES3_LD;

  // fake DMA

  ETHERNET->ETH_TX_LENGTH = framelen;

  rt_uint32_t *tx_message_ptr = (rt_uint32_t *)(txBuffer[txIndex]);
  rt_uint32_t tx_word_num = framelen / 4 + (framelen % 4 ? 1 : 0);

  for (int i = 0; i < tx_word_num; i++) {
    ETHERNET->ETH_TX_DATA[i] = tx_message_ptr[i];
  }

  ETHERNET->ETH_TX_EN |= BIT0;

  tickstart = rt_tick_get();
  while ((ETHERNET->ETH_TX_IS & 0x1) == 0) {
    /* judge timeout */
    if ((rt_tick_get() - tickstart) > ETH_TIME_OUT) {
      LOG_E("ETH send timeout!");
      return -RT_ETIMEOUT;
    }
  }

  /* Clear the interrupt */
  ETHERNET->ETH_TX_IC |= 0x1;

  txIndex = (txIndex + 1) % ETH_TXBUFNB;

  return ERR_OK;
}

struct pbuf *rt_bsp_eth_rx(rt_device_t dev) {
  rt_uint32_t framelength = 0;
  rt_uint32_t framelen = 0;
  struct pbuf *p = RT_NULL, *q = RT_NULL;

  /* The current buffer is available for reading */
  if (rxDmaDesc[rxIndex].valid) {

    /* Retrieve the length of the frame */
    framelength = rxDmaDesc[rxIndex].len;
    /* check the frame length */
    framelength =
        (framelength > ETH_RX_BUF_SIZE) ? ETH_RX_BUF_SIZE : framelength;

    p = pbuf_alloc(PBUF_RAW, framelength, PBUF_RAM);
    if (p != NULL) {
      for (q = p; q != NULL; q = q->next) {
        level = rt_hw_interrupt_disable();
        rt_memcpy(q->payload, &rxBuffer[rxIndex][framelen], q->len);
        framelen += q->len;
        rt_hw_interrupt_enable(level);

        if (framelen > framelength) {
          LOG_E("frame len is too long!");
          return RT_NULL;
        }
      }
    }

    /* Set the start address of the buffer */
    rxDmaDesc[rxIndex].buf = rxBuffer[rxIndex];
    rxDmaDesc[rxIndex].valid = 0;

#ifdef ETH_RX_DUMP
    rt_kprintf("Rx dump, len= %d\r\n", framelen);
    dump_hex(rxBuffer[rxIndex], framelen);
#endif
    /* Increment index and wrap around if necessary */
    rxIndex = (rxIndex + 1) % ETH_RXBUFNB;

    // /* Clear RBU flag to resume processing */
    // ETH->DMAC0SR = ETH_DMAC0SR_RBU;
    // /* Instruct the DMA to poll the receive descriptor list */
    // ETH->DMAC0RXDTPR = 0;
  }

  return p;
}

#endif /* RT_USING_LWIP */

void ETH1_IRQHandler(int vector, void *param) {
  rt_interrupt_enter();

  /* Frame received */
  if (ETHERNET->ETH_RX_IS) {
    ETHERNET->ETH_RX_IE = 0;
    // LOG_D("emac interrupt rx");
    rt_event_send(&rx_event, 1);
  } else {
    LOG_D("emac interrupt unknown");
  }

  rt_interrupt_leave();
}

static void phy_linkchange() {
  rt_ssize_t status = 0;

  /* Read status register to acknowledge the interrupt */
  status = phy_read_reg(RTL8211F_PHY_ADDR, RTL8211F_INSR);

  if (status & (RTL8211F_BMSR_LINK_STATUS | RTL8211F_INSR_AN_COMPLETE)) {
    status = phy_read_reg(RTL8211F_PHY_ADDR, RTL8211F_BMSR);
    status = phy_read_reg(RTL8211F_PHY_ADDR, RTL8211F_BMSR);
    if (status & RTL8211F_BMSR_LINK_STATUS) {
      LOG_I("link up");

      status = phy_read_reg(RTL8211F_PHY_ADDR, RTL8211F_PHYSR);
      switch (status & RTL8211F_PHYSR_SPEED) {
      case RTL8211F_PHYSR_SPEED_10MBPS: {
        LOG_D("speed: 10M");
        bsp_eth_device.eth_speed = PHY_10M;
      } break;

      case RTL8211F_PHYSR_SPEED_100MBPS: {
        LOG_D("speed: 100M");
        bsp_eth_device.eth_speed = PHY_100M;
      } break;

      case RTL8211F_PHYSR_SPEED_1000MBPS: {
        LOG_D("speed: 1000M");
        bsp_eth_device.eth_speed = PHY_1000M;
      } break;

      /* Unknown speed */
      default:
        rt_kprintf("Invalid speed.");
        break;
      }

      bsp_eth_device.eth_mode =
          (status & RTL8211F_PHYSR_DUPLEX) ? PHY_FULL_DUPLEX : PHY_HALF_DUPLEX;

      update_mac_mode(bsp_eth_device.eth_speed, bsp_eth_device.eth_mode);
#ifdef RT_USING_LWIP
      /* send link up. */
      eth_device_linkchange(&bsp_eth_device.parent, RT_TRUE);
#endif
    } else {
      LOG_I("link down");
#ifdef RT_USING_LWIP
      eth_device_linkchange(&bsp_eth_device.parent, RT_FALSE);
#endif
    }
  }
}

#ifdef PHY_USING_INTERRUPT_MODE
static void eth_phy_isr(void *args) {
  rt_uint32_t status = 0;

  phy_read_reg(RTL8211F_PHY_ADDR, PHY_INTERRUPT_FLAG_REG,
               (rt_uint32_t *)&status);
  LOG_D("phy interrupt status reg is 0x%X", status);

  phy_linkchange();
}
#endif /* PHY_USING_INTERRUPT_MODE */

static void phy_monitor_thread_entry(void *parameter) {
  rt_uint32_t status = 0;
  phy_linkchange();
#ifdef PHY_USING_INTERRUPT_MODE
  /* configuration intterrupt pin */
  rt_pin_mode(PHY_INT_PIN, PIN_MODE_INPUT_PULLUP);
  rt_pin_attach_irq(PHY_INT_PIN, PIN_IRQ_MODE_FALLING, eth_phy_isr,
                    (void *)"callbackargs");
  rt_pin_irq_enable(PHY_INT_PIN, PIN_IRQ_ENABLE);

  /* enable phy interrupt */
  phy_write_reg(RTL8211F_PHY_ADDR, PHY_INTERRUPT_MASK_REG, PHY_INT_MASK);
#if defined(PHY_INTERRUPT_CTRL_REG)
  phy_write_reg(RTL8211F_PHY_ADDR, PHY_INTERRUPT_CTRL_REG, PHY_INTERRUPT_EN);
#endif
#else  /* PHY_USING_INTERRUPT_MODE */
  bsp_eth_device.poll_link_timer =
      rt_timer_create("phylnk", (void (*)(void *))phy_linkchange, NULL,
                      RT_TICK_PER_SECOND, RT_TIMER_FLAG_PERIODIC);
  if (!bsp_eth_device.poll_link_timer ||
      rt_timer_start(bsp_eth_device.poll_link_timer) != RT_EOK) {
    LOG_E("Start link change detection timer failed");
  }
#endif /* PHY_USING_INTERRUPT_MODE */
  while (1) {
    if (rt_event_recv(&rx_event, 0xffffffff,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &status) == RT_EOK) {
      // fake DMA
      /* check dma rx buffer */
      if (!rxDmaDesc[rxIndex].valid) {
        rt_uint8_t *rx_message = rxBuffer[rxIndex];
        rt_uint32_t *rx_data_length = &(rxDmaDesc[rxIndex].len);

        if (rx_message == NULL) {
          return;
        }

        rt_uint32_t rx_length = ETHERNET->ETH_RX_LENGTH;
        *rx_data_length = rx_length;

        rt_uint8_t *rx_buffer = (rt_uint8_t *)(ETHERNET->ETH_RX_DATA);
        for (int i = 0; i < rx_length; i++) {
          rx_message[i] = rx_buffer[i];
        }
        rxDmaDesc[rxIndex].valid = 1;

#ifndef RT_USING_LWIP
        rt_thread_delay(RT_TICK_PER_SECOND / 10);
#ifdef ETH_RX_DUMP
        rt_kprintf("Rx dump, len= %d\r\n", rx_length);
        dump_hex(rx_buffer, rx_length);
#endif
#endif

        /* Clear the interrupt */
        ETHERNET->ETH_RX_IC = 0x1;
        ETHERNET->ETH_RX_IE = 0x1;

        /* Process all pending packets */
        while (rxDmaDesc[rxIndex].valid) {
#ifdef RT_USING_LWIP
          /* trigger lwip receive thread */
          eth_device_ready(&(bsp_eth_device.parent));
#else
          rt_thread_delay(RT_TICK_PER_SECOND / 10);
          rxDmaDesc[rxIndex].valid = 0;
#endif
        }
      }
    }
  }
}

/* Register the EMAC device */
static int rt_hw_bsp_eth_init(void) {
  rt_err_t state = RT_EOK;

  // /* OUI 00-80-E1 STMICROELECTRONICS. */
  // bsp_eth_device.dev_addr[0] = 0x00;
  // bsp_eth_device.dev_addr[1] = 0x80;
  // bsp_eth_device.dev_addr[2] = 0xE1;
  // /* generate MAC addr from 96bit unique ID (only for test). */
  // bsp_eth_device.dev_addr[3] = *(rt_uint8_t *)(UID_BASE + 4);
  // bsp_eth_device.dev_addr[4] = *(rt_uint8_t *)(UID_BASE + 2);
  // bsp_eth_device.dev_addr[5] = *(rt_uint8_t *)(UID_BASE + 0);
  rt_memcpy(bsp_eth_device.dev_addr, mac_addr, 6);

#ifdef RT_USING_LWIP
  bsp_eth_device.parent.parent.init = rt_bsp_eth_init;
  bsp_eth_device.parent.parent.open = rt_bsp_eth_open;
  bsp_eth_device.parent.parent.close = rt_bsp_eth_close;
  bsp_eth_device.parent.parent.read = rt_bsp_eth_read;
  bsp_eth_device.parent.parent.write = rt_bsp_eth_write;
  bsp_eth_device.parent.parent.control = rt_bsp_eth_control;
  bsp_eth_device.parent.parent.user_data = RT_NULL;

  bsp_eth_device.parent.eth_rx = rt_bsp_eth_rx;
  bsp_eth_device.parent.eth_tx = rt_bsp_eth_tx;
#endif

  rt_event_init(&rx_event, "eth_rx", RT_IPC_FLAG_FIFO);

#ifdef RT_USING_LWIP
  /* register eth device */
  state = eth_device_init(&(bsp_eth_device.parent), "e0");
  if (RT_EOK == state) {
    LOG_I("emac device init success");
  } else {
    LOG_E("emac device init faild: %d", state);
    state = -RT_ERROR;
  }
#endif

  /* start phy monitor */
  rt_thread_t tid;
  tid = rt_thread_create("phy", phy_monitor_thread_entry, RT_NULL, 1024,
                         RT_THREAD_PRIORITY_MAX - 2, 2);
  if (tid != RT_NULL) {
    rt_thread_startup(tid);
  } else {
    state = -RT_ERROR;
  }

  rt_hw_interrupt_install(VECTOR_ETH_SOURCE, ETH1_IRQHandler, RT_NULL, "eth");
  rt_hw_interrupt_umask(IRQ_ETH);

  return state;
}
INIT_DEVICE_EXPORT(rt_hw_bsp_eth_init);

#endif
