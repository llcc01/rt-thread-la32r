#include "drv_pano.h"

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

static const char *shortopts = "l:ng";

struct option longopts[] = {
    {"w1", required_argument, NULL, 'w'},
    {"h1", required_argument, NULL, 'h'},
    {"x0", required_argument, NULL, 'x'},
    {"y0", required_argument, NULL, 'y'},
    {"w2", required_argument, NULL, 'W'},
    {"h2", required_argument, NULL, 'H'},
    {"r_min", required_argument, NULL, 'r'},
    {"flipy", required_argument, NULL, 'f'},
    {"delta_phi_pi", required_argument, NULL, 'd'},
    {"h2_decay", required_argument, NULL, 'D'},
    {"i1_base_addr", required_argument, NULL, 'i'},
    {"i2_base_addr", required_argument, NULL, 'I'},
    {"rstn", required_argument, NULL, 'R'},
    {"en", required_argument, NULL, 'e'},
    {"load_en", required_argument, NULL, 'L'},
    {"force_load", required_argument, NULL, 'F'},
    {0, 0, 0, 0},
};

int cmd_pano(int argc, char *argv[]) {
  int c;
  while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
    switch (c) {
    case 'w':
      PANO_REG_W1 = strtol(optarg, NULL, 0);
      break;
    case 'h':
      PANO_REG_H1 = strtol(optarg, NULL, 0);
      break;
    case 'x':
      PANO_REG_X0 = strtol(optarg, NULL, 0);
      break;
    case 'y':
      PANO_REG_Y0 = strtol(optarg, NULL, 0);
      break;
    case 'W':
      PANO_REG_W2 = strtol(optarg, NULL, 0);
      break;
    case 'H':
      PANO_REG_H2 = strtol(optarg, NULL, 0);
      break;
    case 'r':
      PANO_REG_R_MIN = strtol(optarg, NULL, 0);
      break;
    case 'f':
      PANO_REG_FLIPY = strtol(optarg, NULL, 0);
      break;
    case 'd':
      PANO_REG_DELTA_PHI_PI = strtol(optarg, NULL, 0);
      break;
    case 'D':
      PANO_REG_H2_DECAY = strtol(optarg, NULL, 0);
      break;
    case 'i':
      PANO_REG_I1_BASE_ADDR = strtol(optarg, NULL, 0);
      break;
    case 'I':
      PANO_REG_I2_BASE_ADDR = strtol(optarg, NULL, 0);
      break;
    case 'R':
      PANO_REG_RSTN = strtol(optarg, NULL, 0);
      break;
    case 'e':
      PANO_REG_EN = strtol(optarg, NULL, 0);
      break;
    case 'L':
      PANO_REG_LOAD_EN = strtol(optarg, NULL, 0);
      break;
    case 'F':
      PANO_REG_FORCE_LOAD = strtol(optarg, NULL, 0);
      break;
    default:
      break;
    }
  }

  return 0;
}
MSH_CMD_EXPORT(cmd_pano, cmd_pano);