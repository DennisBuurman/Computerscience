#
# Skeleton code for use with Computer Architecture 2018 assignment 3,
# LIACS, Leiden University.
#
# From Dennis Buurman (s2027100) and Jerry Schonenberg (s2041022)
#

CC = gcc
CFLAGS = -Wall -O3 -std=gnu99 -mavx -march=native
LDFLAGS = -lm
PNGFLAGS = `pkg-config --cflags --libs libpng`

# Comment these lines to disable the timing code. (macOS does not
# provide clock_gettime().
CFLAGS += -DENABLE_TIMING
LDFLAGS += -lrt

NVCC = nvcc
NVCCFLAGS = -O3 -DENABLE_TIMING --use_fast_math -g

#TARGETS FOR DIFFERENT FILES
TARGETS = imglab-base
TARGETS += imglab-cuda
TARGETS += imglab-version1
TARGETS += imglab-version2
TARGETS += imglab-version3
TARGETS += imglab-version4
TARGETS += imglab-version5
TARGETS += imglab-version6
TARGETS += imglab-version7

TARGET_BASE = imglab-base
TARGET_BASE += imglab-cuda

TARGET_VERSION1 = imglab-version1
TARGET_VERSION1 += imglab-cuda

TARGET_VERSION2 = imglab-version2
TARGET_VERSION2 += imglab-cuda

TARGET_VERSION3 = imglab-version3
TARGET_VERSION3 += imglab-cuda

TARGET_VERSION4 = imglab-version4
TARGET_VERSION4 += imglab-cuda

TARGET_VERSION5 = imglab-version5
TARGET_VERSION5 += imglab-cuda

TARGET_VERSION6 = imglab-version6
TARGET_VERSION6 += imglab-cuda

TARGET_VERSION7 = imglab-version7
TARGET_VERSION7 += imglab-cuda

TARGET_CUDA1 = imglab-base
TARGET_CUDA1 += imglab-cuda1

COMMON_SRC = main.c image.c timing.c

all:			$(TARGETS)
base:			$(TARGET_BASE)
version1:	$(TARGET_VERSION1)
version2: $(TARGET_VERSION2)
version3: $(TARGET_VERSION3)
version4: $(TARGET_VERSION4)
version5: $(TARGET_VERSION5)
version6: $(TARGET_VERSION6)
version7: $(TARGET_VERSION7)
cuda1:		$(TARGET_CUDA1)

imglab-base: main.c $(COMMON_SRC) ops-baseline.c
			$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-cuda:		main.c $(COMMON_SRC) ops-cuda.cu
			$(NVCC) $(NVCCFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)
			
imglab-version1: main.c $(COMMON_SRC) ops-version1.c
			$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-version2: main.c $(COMMON_SRC) ops-version2.c
			$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-version3: main.c $(COMMON_SRC) ops-version3.c
			$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-version4: main.c $(COMMON_SRC) ops-version4.c
			$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-version5: main.c $(COMMON_SRC) ops-version5.c
			$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-version6: main.c $(COMMON_SRC) ops-version6.c
			$(CC) $(CFLAGS) -fopenmp -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

imglab-version7: main.c $(COMMON_SRC) ops-version7.c
			$(CC) $(CFLAGS) -fopenmp -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

clean:
			rm -f $(TARGETS)

