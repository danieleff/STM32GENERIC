# Create stm32XXxx_hal_YYY.c files with #ifdefs

import os

serieses = ['f0', 'f1', 'f2', 'f3', 'f4', 'f7', 'l0', 'l1', 'l4']

all = []

for series in serieses:
    files = os.listdir('../../STM32/system/STM32' + series.upper() + '/HAL_Src')

    generic = [file.replace(series, 'XX') for file in files]
    
    #print files
    #print generic
    
    all.extend(generic)

source = ""
source_ex = ""
    
for series in serieses:

    source += '#ifdef STM32' + series.upper() + '\n'
    source_ex += '#ifdef STM32' + series.upper() + '\n'
    
    for x in sorted(set(all)):
        if 'system' in x or 'template' in x:
            continue
        
        if os.path.isfile('../../STM32/system/STM32' + series.upper() + '/HAL_Src/' + x.replace('XX', series)):
            if 'ex.c' in x:
                source_ex += '    #include "' + x.replace('XX', series) + '"\n'
            else:
                source += '    #include "' + x.replace('XX', series) + '"\n'
            
    source += '#endif\n'
    source_ex += '#endif\n'
    
with open('stm32XXxx_hal.c', 'w') as file_hal:
    file_hal.write('#include "stm32_build_defines.h"\n\n')
    file_hal.write(source)

with open('stm32XXxx_hal_ex.c', 'w') as file_hal_ex:
    file_hal_ex.write('#include "stm32_build_defines.h"\n\n')
    file_hal_ex.write(source_ex)
    