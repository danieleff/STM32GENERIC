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
    
for x in sorted(set(all)):
    if 'system' in x or 'template' in x:
        continue
    
    source = '#include "stm32_build_defines.h"\n'
    source += '\n'
    for series in serieses:
        source += '#ifdef STM32' + series.upper() + '\n'
        if os.path.isfile('../../STM32/system/STM32' + series.upper() + '/HAL_Src/' + x.replace('XX', series)):
            source += '    #include "' + x.replace('XX', series) + '"\n'
        else:
            source += '    \n'
        
        source += '#endif\n'
    print source

    with open(x, 'w') as file:
        file.write(source)

    