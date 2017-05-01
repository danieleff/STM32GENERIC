PATH_COMPILER = D:\programs\gcc-arm
#PATH_COMPILER = C:\Users\$(USERNAME)\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\4.8.3-2014q1

PATH_ROOT = ../../STM32/
PATH_PROJECT = $(PATH_ROOT)/libraries/$(PROJECT)
PATH_CORE = $(PATH_ROOT)/cores/arduino/
PATH_VARIANT = $(PATH_ROOT)variants/$(build.variant)/
PATH_BUILD_ROOT = build/$(VARIANT)/
PATH_BUILD_CORE = $(PATH_BUILD_ROOT)/core/
PATH_BUILD_PROJECT = $(PATH_BUILD_ROOT)/$(PROJECT)/

include makefiles/$(VARIANT)

### Included libraries

cat := $(if $(filter $(OS),Windows_NT),type,cat)
LIBRARIES = $(shell $(cat) $(subst /,\\,$(PATH_PROJECT)/build-include.txt))

#### Source files

PROJECT_SOURCES_C += $(wildcard $(PATH_PROJECT)*.c) 
PROJECT_SOURCES_CPP += $(wildcard $(PATH_PROJECT)*.cpp) 
PROJECT_SOURCES_INO += $(wildcard $(PATH_PROJECT)*.ino)

CORE_SOURCES_C = $(wildcard $(PATH_CORE)*.c)
CORE_SOURCES_C += $(wildcard $(PATH_CORE)stm32/*.c)
CORE_SOURCES_C += $(wildcard $(PATH_CORE)stm32_HAL/*.c)
CORE_SOURCES_C += $(wildcard $(PATH_CORE)usb/*.c)
CORE_SOURCES_CPP += $(wildcard $(PATH_CORE)*.cpp)
CORE_SOURCES_S += $(wildcard $(PATH_CORE)stm32_HAL/*.S)

VARIANT_SOURCES_C += $(wildcard $(PATH_VARIANT)*.c)
VARIANT_SOURCES_CPP += $(wildcard $(PATH_VARIANT)*.cpp)

#### Object files

OBJECTS = $(patsubst $(PATH_PROJECT)%,$(PATH_BUILD_PROJECT)%,$(PROJECT_SOURCES_C:.c=.c.o))
OBJECTS += $(patsubst $(PATH_PROJECT)%,$(PATH_BUILD_PROJECT)%,$(PROJECT_SOURCES_CPP:.cpp=.cpp.o))
OBJECTS += $(patsubst $(PATH_PROJECT)%,$(PATH_BUILD_PROJECT)%,$(PROJECT_SOURCES_INO:.ino=.ino.cpp.o))

OBJECTS += $(patsubst $(PATH_CORE)%,$(PATH_BUILD_CORE)%,$(CORE_SOURCES_C:.c=.c.o))
OBJECTS += $(patsubst $(PATH_CORE)%,$(PATH_BUILD_CORE)%,$(CORE_SOURCES_CPP:.cpp=.cpp.o))
OBJECTS += $(patsubst $(PATH_CORE)%,$(PATH_BUILD_CORE)%,$(CORE_SOURCES_S:.S=.s.o))

OBJECTS += $(patsubst $(PATH_VARIANT)%,$(PATH_BUILD_CORE)%,$(VARIANT_SOURCES_C:.c=.c.o))
OBJECTS += $(patsubst $(PATH_VARIANT)%,$(PATH_BUILD_CORE)%,$(VARIANT_SOURCES_CPP:.cpp=.cpp.o))


LIBRARY_OBJ_DIRS += $(patsubst $(PATH_ROOT)libraries%,$(PATH_BUILD_ROOT)%,$(LIBRARIES))
OBJECTS += $(foreach dir,$(LIBRARY_OBJ_DIRS),$(wildcard $(dir)/*.o))

ifneq ($(wildcard $(PATH_PROJECT)/*.ino),) 
	ELF = $(PATH_BUILD_CORE)build.elf
	BIN = $(PATH_BUILD_CORE)build.bin
endif

INCLUDES = "-I$(PATH_ROOT)cores/arduino/"
INCLUDES += "-I$(PATH_ROOT)cores/arduino/stm32"
INCLUDES += "-I$(PATH_ROOT)cores/arduino/stm32_HAL"
INCLUDES += "-I$(PATH_ROOT)cores/arduino/usb"
INCLUDES += "-I$(PATH_ROOT)system/CMSIS"
INCLUDES += "-I$(PATH_ROOT)system/STM32F1/CMSIS_Inc"
INCLUDES += "-I$(PATH_ROOT)system/STM32F1/CMSIS_Src"
INCLUDES += "-I$(PATH_ROOT)system/STM32F1/HAL_Inc"
INCLUDES += "-I$(PATH_ROOT)system/STM32F1/HAL_Src"
INCLUDES += "-I$(PATH_ROOT)system/STM32F1/stm32_chip"
INCLUDES += "-I$(PATH_VARIANT)"
INCLUDES += $(LIBRARIES:%=-I%)

all: $(OBJECTS) $(BIN)
	$(info BUILD SUCCESSFUL)

$(BIN): $(ELF)
	$(recipe.objcopy.bin.pattern)
	
$(ELF):
	$(recipe.c.combine.pattern)

$(PATH_BUILD_PROJECT)%.cpp.o: $(PATH_PROJECT)%.cpp
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.cpp.o.pattern)

$(PATH_BUILD_PROJECT)%.ino.cpp.o: $(PATH_PROJECT)%.ino
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(subst -c,-c -include Arduino.h -x c++,$(recipe.cpp.o.pattern) )
	
$(PATH_BUILD_PROJECT)%.c.o: $(PATH_PROJECT)%.c
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.c.o.pattern)

$(PATH_BUILD_CORE)%.cpp.o: $(PATH_CORE)%.cpp
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.cpp.o.pattern)

$(PATH_BUILD_CORE)%.c.o: $(PATH_CORE)%.c
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.c.o.pattern)

$(PATH_BUILD_CORE)%.s.o: $(PATH_CORE)%.S
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.S.o.pattern)

$(PATH_BUILD_CORE)%.cpp.o: $(PATH_VARIANT)%.cpp
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.cpp.o.pattern)

$(PATH_BUILD_CORE)%.c.o: $(PATH_VARIANT)%.c
	-@mkdir $(subst /,\\,$(@D)) 2> build/ignore
	$(recipe.c.o.pattern)

clean:
	rm -rf $(PATH_BUILD)
	rm -f $(ELF)
	rm -f $(BIN)
