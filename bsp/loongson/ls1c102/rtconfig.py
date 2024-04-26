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
	EXEC_PATH   = "/home/lc/tools/loongarch32-linux-gnu-2022-04-15/bin"
	# EXEC_PATH   = r'D:\mipsel-none-elf-win-20180903\bin'
else:
    print('================ERROR===========================')
    print('Not support %s yet!' % CROSS_TOOL)
    print('=================================================')
    exit(0)

if os.getenv('RTT_EXEC_PATH'):
	EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD       = 'debug'

PREFIX = 'loongarch32-linux-gnu-'
CC = PREFIX + 'gcc'
AS = PREFIX + 'as'
AR = PREFIX + 'ar'
LINK = PREFIX + 'ld'
TARGET_EXT = 'elf'
SIZE = PREFIX + 'size'
OBJDUMP = PREFIX + 'objdump'
OBJCPY = PREFIX + 'objcopy'
READELF = PREFIX + 'readelf'

DEVICE = ' -msoft-float'
CFLAGS = DEVICE + ' -D_GNU_SOURCE -D_TIMEVAL_DEFINED -O0 -ffunction-sections -fdata-sections -fsched-pressure -fno-builtin'
AFLAGS = ' -c' + DEVICE + ' -fno-pic -fno-builtin -x assembler-with-cpp -DSYSTEM_STACK=0xa00003fc'
LFLAGS = ' --gc-sections -Map=rtthread.map -T ls1c102.lds -G 0 -static'

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
