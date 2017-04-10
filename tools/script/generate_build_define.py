# This file is a mess, but does the job

import xml.etree.ElementTree as ET
import os.path
from os import listdir
import sys
import re

cubemx_dir = 'D:\programs\STM32Cube\STM32CubeMX'

stm32_dir = '../../STM32/cores/arduino/stm32/'
system_dir = '../../STM32/system/'
header_lines = []

with open(system_dir + 'STM32F0/CMSIS_Inc/stm32f0xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32F1/CMSIS_Inc/stm32f1xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32F2/CMSIS_Inc/stm32f2xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32F3/CMSIS_Inc/stm32f3xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32F4/CMSIS_Inc/stm32f4xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32F7/CMSIS_Inc/stm32f7xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32L0/CMSIS_Inc/stm32l0xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32L1/CMSIS_Inc/stm32l1xx.h') as f:
    header_lines.extend(f.readlines())
with open(system_dir + 'STM32L4/CMSIS_Inc/stm32l4xx.h') as f:
    header_lines.extend(f.readlines())
    
def find_header(name):
    include = False
    define = False
    for line in header_lines:
        if '/* #define' in line:
            define = line[line.index('#define ')+8:][:11]
        
        if '#endif' in line:
            define = False
            
        if not define:
            continue
            
        if (name in line):
            include = define
            break
        if ((name[:-2] + "xx") in line):
            include = define
            break
        if ((name[:-2] + "x" + name[-1:]) in line):
            include = define
            break
   
    if not include:
        raise Exception(name)
    
    return include
    
    
families_xml = os.path.join(cubemx_dir, 'db', 'mcu', 'families.xml')
if not os.path.exists(families_xml):
    print 'Could not find CubeMX, please set CubeMX directory'
    exit(-1)
families = ET.parse(families_xml).getroot()
ns = {'stm': 'http://mcd.rou.st.com/modules.php?name=mcu'}

mcus = families.findall(".//Mcu")


class load_mcu:
    def __init__(self, name):
        print name
        
        self.mcu_name = name
        
        if families.find(".//Mcu[@RefName='" + self.mcu_name + "Tx']") is not None:
            self.mcu_xml = families.find(".//Mcu[@RefName='" + self.mcu_name + "Tx']").attrib['Name'] + '.xml'
        else:
            self.mcu_xml = families.find(".//Mcu[@RPN='" + self.mcu_name + "']").attrib['Name'] + '.xml'
        
        self.mcu = ET.parse(os.path.join(cubemx_dir, 'db', 'mcu', self.mcu_xml)).getroot()
        self.remap_xml = 'GPIO-' + self.mcu.find("stm:IP[@Name='GPIO']", ns).attrib['Version'] + '_Modes.xml'

        self.remap_root = ET.parse(os.path.join(cubemx_dir, 'db', 'mcu', 'IP', self.remap_xml )).getroot()

        self.mcu_pins = self.mcu.findall('stm:Pin',  ns);

        self.remaps = {}
        self.defaultremaps = {}
        self.af_functions=[]    
        self.pins = []
        self.peripherals = []
        self.groups = {}
        self.group_signals = {}

    def find_remaps(self):
        for pin in self.remap_root.findall('stm:GPIO_Pin',  ns):
            pin_name = pin.attrib['Name']
            
            pin_name = re.search('^(\D*\d*)', pin_name).group(1)
            
            gpio_signals = pin.findall('stm:PinSignal',  ns)
            for gpio_signal in gpio_signals:
                signal = gpio_signal.attrib['Name']
                
                periph = signal.split('_')[0]
                if signal not in self.remaps:
                    self.remaps[signal] = {}
                
                if self.mcu_name.startswith('STM32F1'):
                    remap_function = False
                    
                    remap_block = gpio_signal.find('stm:RemapBlock', ns)
                    if remap_block is not None:
                        if 'DefaultRemap' in remap_block.attrib:
                            remap_function = 'AF__HAL_AFIO_REMAP_' + periph + '_DISABLE'
                            self.defaultremaps[signal] = pin_name
                        else:
                            remap_function = 'AF' + remap_block.find('stm:SpecificParameter', ns).find('stm:PossibleValue', ns).text
                    else:
                        remap_function = 'AF__NO_REMAP'
                        if signal not in self.defaultremaps:
                            self.defaultremaps[signal] = pin_name
                            
                    self.remaps[signal][pin_name] = remap_function
                    if remap_function not in self.af_functions:
                        self.af_functions.append(remap_function)
                    
                else:
                    gpio_af = gpio_signal.find("stm:SpecificParameter[@Name='GPIO_AF']", ns)
                    self.remaps[signal][pin_name] = gpio_af.find("stm:PossibleValue", ns).text
            
    def process_pins(self):
        for pin in self.mcu_pins:
            pin_name = pin.attrib['Name']
            
            pin_name = re.search('^(\D*\d*)', pin_name).group(1)
            
            self.pins.append(pin_name)
        
            if not pin.attrib['Type'] == 'I/O':
                continue
            
            signals = pin.findall('stm:Signal',  ns)
            for signal_element in signals:
                signal = signal_element.attrib['Name']
                if not signal.startswith('USART') \
                    and not signal.startswith('SPI') \
                    and not signal.startswith('I2C') \
                    and not signal.startswith('TIM'):
                    continue
                
                (periph, sig) = signal.split('_', 1)
                
                if periph not in self.peripherals:
                    self.peripherals.append(periph)
                
                self.groups.setdefault(periph[:-1], []).append(sig)
                
                self.group_signals.setdefault(periph[:-1] + '_' + sig, []).append(signal)
                
                if signal not in self.defaultremaps:
                    self.defaultremaps[signal] = pin_name

                if signal not in self.remaps:
                    self.remaps[signal] = {}
                    self.remaps[signal][pin_name] = 'AF_NO_REMAP'

    def generate_source_code(self):
        self.peripherals.sort()

        self.af_functions.sort()

        source_code = '';
        source_code = '//Autogenerated file\n'
        source_code += '//MCU name: ' + self.mcu_name+ '\n'
        source_code += '//MCU  xml: ' + self.mcu_xml+ '\n'
        source_code += '//GPIO remap/alternate function xml: ' + self.remap_xml+ '\n'
        
        source_code += '\n'
        
        gpio_pins = [pin for pin in self.pins if pin.startswith('P') and pin != 'PDR_ON']
        
        source_code += '/*\n'
        source_code += 'const stm32_port_pin_type port_pin_list[] = {\n'
        
        for pin in sorted(gpio_pins, key=lambda x: (x[0:2], int(x[2:]))):
             source_code += '  { GPIO' +  pin[1:2] + ', GPIO_PIN_' + pin[2:].ljust(2) + '},\n'
        
        source_code += '};\n'
        source_code += '*/\n'
        source_code += '\n'
        source_code += '/*\n'
        source_code += 'enum {\n'
        for pin in sorted(gpio_pins, key=lambda x: (x[0:2], int(x[2:]))):
             source_code += '   ' + pin.ljust(4) + ',\n'
        source_code += 'NUM_PINS,\n'
        source_code += '};\n'
        source_code += '*/\n'
        
        if len(self.af_functions) > 0:
            source_code += 'static void AF_NO_REMAP (void) { }\n'
            
        for af_function in self.af_functions:
            source_code += 'static void ' + af_function + '(void) { ' + af_function[2:] + '(); }\n'


        for periph in sorted(self.groups):
            for sig in sorted(set(self.groups[periph])):

                if sig not in ['SDA', 'SCL', 'TX', 'RX', 'MISO', 'MOSI', 'SCK'] and not periph_signal.startswith('CH'):
                    continue
                
                source_code += '\n'
                source_code += 'const alternate_pin_type alternate_'+(periph + '_' + sig).lower()+' [] = {\n'
                #print self.group_signals
                old = False
                for signal in sorted(set(self.group_signals[periph + '_' +sig])):
                    if old != signal.split('_')[0]:
                        source_code += '//' + signal.split('_')[0] + '\n'
                    p = signal.split('_')[0]
                    for pin in sorted(set(self.remaps[signal])):
                        if pin in self.pins:
                            remap = self.remaps[signal][pin]
                            
                            split = signal.split('_')
                            p = split[0]
                            periph_signal = split[1]
                            
                            source_code += '    { ' + p.ljust(6) + ', GPIO' + pin[1:2] + ', GPIO_PIN_' + pin[2:].ljust(3) + ', ' + remap.ljust(15) + '}, \n'
                    
                source_code += '}; \n'
            
        # for signal in sorted(self.remaps):
            # if signal.startswith('SPI') or signal.startswith('I2C') or signal.startswith('USART'): # or signal.startswith('TIM'):
                
                # source_code += '\n'
                # source_code += 'const alternate_'+signal.lower()+' alternate_list[] = {\n'
                # for pin in self.remaps[signal]:
                    # if pin in self.pins:
                        # remap = self.remaps[signal][pin]
                        
                        # split = signal.split('_')
                        # periph = split[0]
                        # periph_signal = split[1]
                        
                        # if periph_signal not in ['SDA', 'SCL', 'TX', 'RX', 'MISO', 'MOSI', 'SCK'] and not periph_signal.startswith('CH'):
                            # continue
                        
                        # source_code += '    { ' + periph.ljust(6) + ', GPIO' + pin[1:2] + ', GPIO_PIN_' + pin[2:].ljust(3) + ', ' + remap.ljust(15) + '}, // ' + periph_signal.ljust(5) + '\n'
                # source_code += '}; \n'
                        
        # for periph in self.peripherals:
            # for signal in self.signals[periph]:
                # print signal
                
                # if signal not in self.defaultremaps: 
                   # continue 
                    
                # source_code +=  ' {' + periph + ', '
                
                # source_code +=  '    .mosi_port = GPIO' + self.defaultremaps[signal][1:2] + ', \n'
                # source_code +=  '    .mosi_pin =  GPIO_PIN_' + self.defaultremaps[signal][2:] + ', \n'
                
                # if self.mcu_name.startswith('STM32F1'):
                    # source_code +=  '    .spi_alternate = ' + self.remaps[signal][self.defaultremaps[signal]] + ',\n'
                    
                # if not self.mcu_name.startswith('STM32F1'):
                    # source_code +=  '    .mosi_alternate = ' + self.remaps[signal][self.defaultremaps[signal]] + ',\n'
                    
                # source_code += '  },\n'
        
        #print source_code
        
        dir = system_dir + name[:7] + "/stm32_chip/"
        with open(dir + 'stm32_' + self.mcu_name + '.h', 'w') as file:
            file.write(source_code)
        

with open(stm32_dir + 'stm32_build_defines.h', 'w') as file:
    file.write('//Autogenerated file\n')
    file.write('\n')
    file.write('#ifndef STM32_BUILD_DEFINES_H\n')
    file.write('#define STM32_BUILD_DEFINES_H\n')
    file.write('\n')
    
    name_processed = []
    for mcu in mcus:
        name = mcu.attrib['RefName'] 
        name = name[:11]
        
        if name in name_processed:
            continue
        name_processed.append(name)
        
        dir = system_dir + name[:7] + "/CMSIS_Inc/"
        #filename = name.lower()
        define = find_header(name)
        
        file.write('\n');
        file.write('#if defined(' + name + ')\n')
        file.write('  #define ' + define + '\n')
        file.write('  #define CMSIS_STARTUP_ASSEMBLY "startup_' + define.lower() + '.s"\n')
        file.write('  #define CHIP_PERIPHERAL_INCLUDE "stm32_' + name + '.h"\n')
        file.write('#endif\n')
        
        mcu = load_mcu(name)
        mcu.find_remaps()
        mcu.process_pins()
        mcu.generate_source_code()
      
    file.write('\n')
    file.write('#endif')

    
    