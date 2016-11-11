##########################################################
##########################################################

TARGET := bin/hobd-signal-viewer

SRCS := src/render_hobd_obd_time.c \
	src/render_hobd_obd1.c \
	src/render_hobd_obd2.c \
	src/render_hobd_obd3.c \
	src/render_hobd_heartbeat.c \
	src/render_hobd_gps_time1.c \
	src/render_hobd_gps_time2.c \
	src/render_hobd_imu_sample_time.c \
	src/render_hobd_imu_time1.c \
	src/render_hobd_imu_time2.c \
	src/render_hobd_gps_baseline_ned1.c \
	src/render_hobd_gps_pos_llh1.c \
	src/render_hobd_gps_pos_llh2.c \
	src/render_hobd_gps_pos_llh3.c \
	src/render_hobd_gps_pos_llh4.c \
	src/render_hobd_imu_utc_time1.c \
	src/render_hobd_imu_utc_time2.c \
	src/render_hobd_imu_rate_of_turn1.c \
	src/render_hobd_imu_rate_of_turn2.c \
	src/render_page1.c \
	src/render_page2.c \
	src/render_page3.c \
	src/render_page4.c \
	src/render_page5.c \
	src/render.c \
	src/time_domain.c \
	src/signal_table.c \
	src/display_manager.c \
	src/can.c \
	src/can_replay.c \
	src/main.c

OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.dep)
XDEPS := $(wildcard $(DEPS))

CC = gcc

CCFLAGS = -std=gnu99

CCFLAGS += -Wall -Wextra \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wnested-externs -Wmissing-include-dirs

# include GL EXT
CCFLAGS += -DGL_GLEXT_PROTOTYPES -DGLX_GLXEXT_PROTOTYPES

LDFLAGS =

INCLUDES = -Iinclude -I../../firmware/hobd_common/include

LIBS = -lrt -lglut -lGLU -lGL -lX11 -lm -lcanlib

# CAN replay module uses PolySync
PSYNC_HOME ?= /usr/local/polysync
INCLUDES += -I$(PSYNC_HOME)/include \
            -I$(PSYNC_HOME)/include/deps \
            -I$(PSYNC_HOME)/include/deps/xsens \
            -I$(PSYNC_HOME)/include/deps/dcps/C/SAC/ \
            -I$(PSYNC_HOME)/include/deps/sys \
            -I$(PSYNC_HOME)/pdm \
            -I/usr/include/libxml2 \
            `pkg-config --cflags gmodule-2.0`
LIBS += -L$(PSYNC_HOME)/lib -lpolysync -lpolysync_data_model `pkg-config --libs gmodule-2.0` -lgthread-2.0

all: dirs $(TARGET)

ifneq ($(XDEPS),)
include $(XDEPS)
endif

dirs::
	mkdir -p bin

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS): %.o: %.c %.dep
	$(CC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

$(DEPS): %.dep: %.c Makefile
	$(CC) $(CCFLAGS) $(INCLUDES) -MM $< > $@

install:
	cp $(TARGET) /usr/local/bin/

clean:
	-rm -f src/*.o
	-rm -f src/*.dep
	-rm -f $(TARGET)
