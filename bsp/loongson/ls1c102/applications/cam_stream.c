#include <lwip/sockets.h>
#include <rtthread.h>

#include "drv_ddr_dma.h"
#include "drv_jpeg.h"
#include "lwip/inet.h"
#include "rttypes.h"

#define LOG_TAG "drv.jpeg"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define STREAM_BLOCK_SIZE 1024

void cs_send_frame(int sock, struct sockaddr *sockaddr) {
  ddr_dma_reset_wr_ch(1);
  rt_thread_delay(1);
  jpeg_start();
  while (jpeg_get_status() == 1) {
    LOG_D("JPEG status: %d block_cnt: %d", jpeg_get_status(),
          jpeg_get_block_cnt());
  }
  rt_uint32_t jpeg_size = jpeg_get_size();
  LOG_I("JPEG size: %d", jpeg_size);

  rt_uint8_t *block = rt_malloc(STREAM_BLOCK_SIZE);
  LOG_D("block %p", block);
  rt_uint32_t word_num = jpeg_size / 4 + (jpeg_size % 4 != 0);
  rt_uint32_t block_word_cnt = 0;
  rt_uint32_t send_size = 0;
  int i = 0;
  while (i < word_num) {
    ((rt_uint32_t *)block)[block_word_cnt] = JPEG_DATA_PTR[i];
    block_word_cnt++;
    i++;
    if (block_word_cnt == STREAM_BLOCK_SIZE / 4 || i == word_num) {
      send_size = block_word_cnt * 4;
      LOG_I("block %d, data %08x ...", i, *(rt_uint32_t *)block);
      sendto(sock, block, send_size, 0, sockaddr, sizeof(struct sockaddr));
      block_word_cnt = 0;
    }
  }
  rt_free(block);
}

static rt_uint8_t run = 0;

void udp_server_thread_entry() {
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    LOG_E("socket error");
    return;
  }

  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(54321);
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
    LOG_E("bind error");
    closesocket(sock);
    return;
  }

  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(12345);
  sockaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  // wait udp packet for broadcast
  LOG_I("waiting for broadcast");

  char *buf = rt_malloc(1024);
  while (1) {
    struct sockaddr_in sockaddr;
    int addr_len = sizeof(struct sockaddr);
    int recv_len = recvfrom(sock, buf, sizeof(buf), 0,
                            (struct sockaddr *)&sockaddr, &addr_len);
    if (recv_len > 0) {
      LOG_D("received %d bytes from %s:%d", recv_len,
            inet_ntoa(sockaddr.sin_addr), ntohs(sockaddr.sin_port));
      break;
    }
  }
  rt_free(buf);

  cs_send_frame(sock, (struct sockaddr *)&sockaddr);

  closesocket(sock);

  LOG_I("udp server finished");
}

static int cmd_udp_server(int argc, char *argv[]) {
  udp_server_thread_entry();
  return 0;
}
MSH_CMD_EXPORT(cmd_udp_server, udp_server);
