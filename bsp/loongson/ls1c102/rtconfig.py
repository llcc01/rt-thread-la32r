import os

# CPU options
ARCH='loongarch'
CPU ='la132'

# toolchains options
CROSS_TOOL  = 'gcc'

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')

if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    # EXEC_PATH   = "/home/lc/tools/loongarch32r-linux-gnusf-2022-05-20/bin"
    EXEC_PATH   = "/home/lc/tools/loongson-gnu-toolchain-8.3-x86_64-loongarch32r-linux-gnusf-v2.0/bin"
    # EXEC_PATH   = "/home/lc/tools/loongarch32-linux-gnu-2022-04-15/bin"
    # EXEC_PATH   = r'D:\mipsel-none-elf-win-20180903\bin'
else:
    print('================ERROR===========================')
    print('Not support %s yet!' % CROSS_TOOL)
    print('=================================================')
    exit(0)

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

# BUILD       = 'debug'
BUILD       = 'release'

# PREFIX = 'loongarch32-linux-gnu-'
PREFIX = 'loongarch32r-linux-gnusf-'
CC = PREFIX + 'gcc'
AS = PREFIX + 'as'
AR = PREFIX + 'ar'
LINK = PREFIX + 'gcc'
# LINK = PREFIX + 'ld'
TARGET_EXT = 'elf'
SIZE = PREFIX + 'size'
OBJDUMP = PREFIX + 'objdump'
OBJCPY = PREFIX + 'objcopy'
READELF = PREFIX + 'readelf'

DEVICE = ' -msoft-float'
# CFLAGS = DEVICE + ' -D_GNU_SOURCE -D_TIMEVAL_DEFINED -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -fomit-frame-pointer'
CFLAGS = DEVICE + ' -D_GNU_SOURCE -D_TIMEVAL_DEFINED -Diovec=iovec -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -fdiagnostics-color=always'
CFLAGS += ' -fno-omit-frame-pointer'
AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp'
LFLAGS = ' -nostartfiles -static -Wl,--gc-sections,-Map=rtthread.map -T ls1c102.lds'
# LFLAGS = '-Map=rtthread.map -T ls1c102.lds --gc-sections'

CPATH = ''
LPATH = ''

if BUILD == 'debug':
    CFLAGS += ' -O0 -gdwarf-2'
    AFLAGS += ' -gdwarf-2'
else:
    CFLAGS += ' -O2'

DUMP_ACTION = OBJDUMP + ' -D -S $TARGET > rtt.asm\n'
READELF_ACTION = READELF + ' -a $TARGET > rtt.map\n'
POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'

POST_ACTION += DUMP_ACTION