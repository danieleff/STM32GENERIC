import os

serieses = ['F0', 'F1', 'F2', 'F3', 'F4', 'F7', 'L0', 'L1', 'L4']

handlers = {}

for series in serieses:
    path = '../../STM32/system/STM32' + series + '/CMSIS_Src';
    
    files = os.listdir(path)
    for file in files:
        with open(path + '/' + file) as f:
            for line in f.readlines():
                if '.thumb_set' in line:
                    line = line.replace('.thumb_set', '')
                    line = line.replace(',Default_Handler', '').strip()
                    handlers.setdefault(line, []).append(series)

files = {}

for handler in sorted(set(handlers.keys())):
    src = ''
    src += '// ' + ', '.join(sorted(set(handlers[handler]))) + '\n';
    src += 'void ' + handler + "() {\n\n}\n\n";
    
    name = 'stm32_irq_other.c'
    
    if handler.startswith('DMA') and not handler.startswith('DMA2D'):
        name = 'stm32_irq_dma.c' 
        
    elif handler.startswith('UART') or handler.startswith('USART'):
        name = 'stm32_irq_usart.c' 
        
    elif handler.startswith('SPI'):
        name = 'stm32_irq_spi.c' 
        
    elif handler.startswith('I2C'):
        name = 'stm32_irq_i2c.c' 
        
    elif handler.startswith('TIM'):
        name = 'stm32_irq_tim.c' 
        
    elif handler.startswith('EXTI'):
        name = 'stm32_irq_exti.c' 
        
    elif handler.startswith('USB') or handler.startswith('OTG'):
        name = 'stm32_irq_usb.c' 
    
    elif handler.startswith('ADC'):
        name = 'stm32_irq_adc.c' 
    
    elif handler.startswith('CAN'):
        name = 'stm32_irq_can.c' 
        
    files.setdefault(name, '')
    files[name] += src

for (file, src) in files.iteritems():
    with open(file, "w") as f:
        f.write(src)
