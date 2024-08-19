#include "drv_pano.h"
#include "rtthread.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PANO_BASE 0xbfec0000

#define PANO_REG_W1 (*(volatile rt_uint32_t *)(PANO_BASE + 0x80))
#define PANO_REG_H1 (*(volatile rt_uint32_t *)(PANO_BASE + 0x84))
#define PANO_REG_X0 (*(volatile rt_uint32_t *)(PANO_BASE + 0x88))
#define PANO_REG_Y0 (*(volatile rt_uint32_t *)(PANO_BASE + 0x8c))
#define PANO_REG_W2 (*(volatile rt_uint32_t *)(PANO_BASE + 0x90))
#define PANO_REG_H2 (*(volatile rt_uint32_t *)(PANO_BASE + 0x94))
#define PANO_REG_R_MIN (*(volatile rt_uint32_t *)(PANO_BASE + 0x98))
#define PANO_REG_FLIPY (*(volatile rt_uint32_t *)(PANO_BASE + 0x9c))
#define PANO_REG_DELTA_PHI_PI (*(volatile rt_uint32_t *)(PANO_BASE + 0xa0))
#define PANO_REG_H2_DECAY (*(volatile rt_uint32_t *)(PANO_BASE + 0xa4))
#define PANO_REG_I1_BASE_ADDR (*(volatile rt_uint32_t *)(PANO_BASE + 0xb0))
#define PANO_REG_I2_BASE_ADDR (*(volatile rt_uint32_t *)(PANO_BASE + 0xb4))
#define PANO_REG_RSTN (*(volatile rt_uint32_t *)(PANO_BASE + 0xc0))
#define PANO_REG_EN (*(volatile rt_uint32_t *)(PANO_BASE + 0xc4))
#define PANO_REG_LOAD_EN (*(volatile rt_uint32_t *)(PANO_BASE + 0xc8))
#define PANO_REG_FORCE_LOAD (*(volatile rt_uint32_t *)(PANO_BASE + 0xcc))

int cmd_pano(int argc, char *argv[]) {

  PANO_REG_LOAD_EN = 0;
  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];
    char *p = strchr(arg, '=');
    if (p == NULL) {
      rt_kprintf(
          "usage : cmd_pano w1=0 h1=0 x0=0 y0=0 w2=0 h2=0 r_min=0 flipy=0 "
          "delta_phi_pi=0 h2_decay=0 i1_base_addr=0 i2_base_addr=0\n");
      return 0;
    }
    *p = '\0';
    if (strcmp(arg, "w1") == 0) {
      PANO_REG_W1 = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "h1") == 0) {
      PANO_REG_H1 = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "x0") == 0) {
      PANO_REG_X0 = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "y0") == 0) {
      PANO_REG_Y0 = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "w2") == 0) {
      rt_uint32_t w2 = strtoul(p + 1, NULL, 0);
      PANO_REG_W2 = w2;
      PANO_REG_DELTA_PHI_PI = 512 * 16 / w2;
    } else if (strcmp(arg, "h2") == 0) {
      PANO_REG_H2 = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "r_min") == 0) {
      PANO_REG_R_MIN = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "flipy") == 0) {
      PANO_REG_FLIPY = strtoul(p + 1, NULL, 0);
      // } else if (strcmp(arg, "delta_phi_pi") == 0) {
      //   PANO_REG_DELTA_PHI_PI = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "h2_decay") == 0) {
      PANO_REG_H2_DECAY = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "i1_base_addr") == 0) {
      PANO_REG_I1_BASE_ADDR = strtoul(p + 1, NULL, 0);
    } else if (strcmp(arg, "i2_base_addr") == 0) {
      PANO_REG_I2_BASE_ADDR = strtoul(p + 1, NULL, 0);
    } else {
      rt_kprintf(
          "usage : cmd_pano w1=0 h1=0 x0=0 y0=0 w2=0 h2=0 r_min=0 flipy=0 "
          "h2_decay=0 i1_base_addr=0 i2_base_addr=0\n");
      return 0;
    }
  }
  PANO_REG_LOAD_EN = 1;

  return 0;
}
MSH_CMD_EXPORT(cmd_pano, cmd_pano);
