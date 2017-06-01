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
        if ((name[:-2] + "x" + name[-1:]) in line):
            include = define
            break
    
    if not include:
        define = False
        for line in header_lines:
            if '/* #define' in line:
                define = line[line.index('#define ')+8:][:11]
            
            if '#endif' in line:
                define = False
                
            if not define:
                continue
                
            if ((name[:-2] + "xx") in line):
                include = define
                break
    
    #if not include:
    #    raise Exception(name)
    
    return include
    
    
families_xml = os.path.join(cubemx_dir, 'db', 'mcu', 'families.xml')
if not os.path.exists(families_xml):
    print 'Could not find CubeMX, please set CubeMX directory'
    exit(-1)
families = ET.parse(families_xml).getroot()
ns = {'stm': 'http://mcd.rou.st.com/modules.php?name=mcu'}

mcus = families.findall(".//Mcu")


def natural_sort_key(s, _nsre=re.compile('([0-9]+)')):
    return [int(text) if text.isdigit() else text.lower()
            for text in re.split(_nsre, s)] 
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

        self.available_pin_signals = {}
        
        self.remaps = {}
        self.defaultremaps = {}
        self.af_functions=[]    
        self.af_function_used=[]    
        self.pins = []
        self.peripherals = []
        self.groups = {}
        self.group_signals = {}
        
    def load_rcc(self):
        self.rcc_xml = 'RCC-' + self.mcu.find("stm:IP[@Name='RCC']", ns).attrib['Version'] + '_Modes.xml'
        
        self.rcc_root = ET.parse(os.path.join(cubemx_dir, 'db', 'mcu', 'IP', self.rcc_xml )).getroot()
        
        pclk1 = self.rcc_root.find(".//stm:RefParameter[@Name='APB1Freq_Value']",  ns)
        pclk2 = self.rcc_root.find("stm:RefParameter[@Name='APB2Freq_Value']",  ns)
        
        if pclk1 is not None:
            self.PCLK1_PERIPHERALS = pclk1.attrib['IP'].split(',')
        else:
            self.PCLK1_PERIPHERALS = []
            
        if pclk2 is not None:
            self.PCLK2_PERIPHERALS = pclk2.attrib['IP'].split(',')
        else:
            self.PCLK2_PERIPHERALS = []
        
    def find_remaps(self):
        for pin_element in self.mcu.findall('stm:Pin',  ns):
            for signal_element in pin_element.findall('stm:Signal',  ns):
                pin = pin_element.attrib['Name']
                instance_signal = signal_element.attrib['Name']
                
                self.available_pin_signals.setdefault(pin, []).append(instance_signal)
        
        
        for pin in self.remap_root.findall('stm:GPIO_Pin',  ns):
            pin_name = pin.attrib['Name']
            
            pin_name = re.search('^(\D*\d*)', pin_name).group(1)
            
            gpio_signals = pin.findall('stm:PinSignal',  ns)
            for gpio_signal in gpio_signals:
                instance_signal = gpio_signal.attrib['Name']
                
                if pin_name not in self.available_pin_signals or instance_signal not in self.available_pin_signals[pin_name]:
                    continue
                
                instance = instance_signal.split('_')[0]
                if instance_signal not in self.remaps:
                    self.remaps[instance_signal] = {}
                
                if self.mcu_name.startswith('STM32F1'):
                    remap_function = False
                    
                    remap_block = gpio_signal.find('stm:RemapBlock', ns)
                    if remap_block is not None:
                        if 'DefaultRemap' in remap_block.attrib:
                            remap_function = 'AF__HAL_AFIO_REMAP_' + instance + '_DISABLE'
                            self.defaultremaps[instance_signal] = pin_name
                        else:
                            remap_function = 'AF' + remap_block.find('stm:SpecificParameter', ns).find('stm:PossibleValue', ns).text
                    else:
                        remap_function = 'AF__NO_REMAP'
                        if instance_signal not in self.defaultremaps:
                            self.defaultremaps[instance_signal] = pin_name
                            
                    self.remaps[instance_signal][pin_name] = remap_function
                    if remap_function not in self.af_functions:
                        self.af_functions.append(remap_function)
                    
                else:
                    gpio_af = gpio_signal.find("stm:SpecificParameter[@Name='GPIO_AF']", ns)
                    self.remaps[instance_signal][pin_name] = gpio_af.find("stm:PossibleValue", ns).text
            
    def process_pins(self):
        for pin in self.mcu_pins:
            pin_name = pin.attrib['Name']
            
            pin_name = re.search('^(\D*\d*)', pin_name).group(1)
            
            self.pins.append(pin_name)
        
            if not pin.attrib['Type'] == 'I/O':
                continue
            
            instance_signal_elements = pin.findall('stm:Signal',  ns)
            for instance_signal_element in instance_signal_elements:
                instance_signal = instance_signal_element.attrib['Name']
                
                if not instance_signal.startswith('USART') \
                    and not instance_signal.startswith('SPI') \
                    and not instance_signal.startswith('I2C') \
                    and not instance_signal.startswith('TIM') \
                    and not instance_signal.startswith('ADC1') \
                    and not instance_signal.startswith('ADC_') \
                    and not instance_signal.startswith('I2S') \
                    and not instance_signal.startswith('SDIO') \
                    and not instance_signal.startswith('SDMMC'):
                    continue
                
                (instance, sig) = instance_signal.split('_', 1)

                if instance == 'SDIO':
                    instance = 'SDIO1'
                if instance == 'SDMMC1':
                    instance = 'SDIO1'
                if instance == 'SDMMC2':
                    instance = 'SDIO2'
                    
                if instance[-1].isdigit():
                    periph = instance[:-1]
                else:
                    periph = instance
                
                if instance not in self.peripherals:
                    self.peripherals.append(instance)
                
                self.groups.setdefault(periph, []).append(sig)
                
                self.group_signals.setdefault(periph + '_' + sig, []).append(instance_signal)
                
                if instance_signal not in self.defaultremaps:
                    self.defaultremaps[instance_signal] = pin_name

                if instance_signal not in self.remaps:
                    self.remaps[instance_signal] = {}
                    self.remaps[instance_signal][pin_name] = 'AF_NO_REMAP'

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
        
        source_code += '\n'
        source_code += '#define VARIANT_PIN_LIST_DEFAULT \\\n'
        
        self.default_pin_list = '#define CHIP_PIN_LIST';
        for pin in sorted(gpio_pins, key=lambda x: (x[0:2], int(x[2:]))):
             source_code += '   PIN(' + pin[1] + ',' + pin[2:] + '), \\\n'
             self.default_pin_list += ' PIN(' + pin[1] + ',' + pin[2:] + '),'
        self.default_pin_list += '\n'
        
        source_code += '\n'
        source_code += '\n'
        
        periph_source_code = "";
        
        for periph in sorted(self.groups):
            periph_source_code += '\n'
            periph_source_code += '// --------------------' + periph + '--------------------'
            periph_source_code += '\n'
            if 'SDIO' in periph:
                periph_source_code += '#define STM32_CHIP_HAS_SDIO'
                periph_source_code += '\n'
            if 'I2S' in periph:
                periph_source_code += '#define STM32_CHIP_HAS_I2S'
                periph_source_code += '\n'
            
            for sig in sorted(set(self.groups[periph])):
            
                if 'SDIO' not in periph and 'I2S' not in periph and sig not in ['SDA', 'SCL', 'TX', 'RX', 'MISO', 'MOSI', 'SCK', 'NSS']:
                    continue
                    
                if 'I2S' == periph and sig == 'CKIN':
                    continue
                    
                periph_source_code += '\n'
                periph_source_code += 'const stm32_af_pin_list_type chip_af_'+(periph + '_' + sig).lower()+' [] = {\n'
                #print self.group_signals
                old = False
                for signal in sorted(set(self.group_signals[periph + '_' +sig])):
                    if old != signal.split('_')[0]:
                        periph_source_code += '//' + signal.split('_')[0] + '\n'
                    p = signal.split('_')[0]
                    for pin in sorted(set(self.remaps[signal]), key = natural_sort_key):
                        if pin in self.pins:
                            remap = self.remaps[signal][pin]
                            self.af_function_used.append(remap)
                            
                            split = signal.split('_')
                            p = split[0]
                            
                            p = p.replace('I2S', 'SPI');
                
                            periph_source_code += '    { ' + p.ljust(6) + ', GPIO' + pin[1:2] + ', GPIO_PIN_' + pin[2:].ljust(3) + ', ' + remap.ljust(15) + '}, \n'
                    
                periph_source_code += '}; \n'
        
        periph_source_code += '\n'
        
        if len(self.af_functions) > 0:
            source_code += 'static void AF_NO_REMAP (void) { }\n'
            
        for af_function in self.af_functions:
            if af_function in self.af_function_used:
                source_code += 'static void ' + af_function + '(void) { ' + af_function[2:] + '(); }\n'
        
        source_code += periph_source_code
        
        source_code += 'const stm32_chip_adc1_channel_type chip_adc1_channel[] = {\n'
        
        for instance_signal in sorted(self.defaultremaps.keys(), key = natural_sort_key):
            if 'ADC1_IN' not in instance_signal and 'ADC_IN' not in instance_signal:
                continue
                
            (_, channel) = instance_signal.split('_', 1)
            if not channel[2:].isdigit():
                continue
                
            pin = self.defaultremaps[instance_signal]
            source_code += '    { GPIO' + pin[1:2] + ', GPIO_PIN_' + pin[2:].ljust(3) + ', ADC_CHANNEL_' + channel[2:].ljust(3) + '}, \n'
        
        source_code += '};\n'
        source_code += '\n'
        
        source_code += 'const stm32_clock_freq_list_type chip_clock_freq_list[] = {\n'
        for periph in sorted(self.PCLK1_PERIPHERALS):
            if not periph.startswith('USART') and not periph.startswith('SPI') and not periph.startswith('I2C'):
                continue
            
            if periph not in self.peripherals:
                continue
            
            source_code += '    {' + periph.ljust(6) + ', HAL_RCC_GetPCLK1Freq },  \n'
        
        source_code += '\n'
        
        for periph in sorted(self.PCLK2_PERIPHERALS):
            if not periph.startswith('USART') and not periph.startswith('SPI') and not periph.startswith('I2C'):
                continue
            
            if periph not in self.peripherals:
                continue
                
            source_code += '    {' + periph.ljust(6) + ', HAL_RCC_GetPCLK2Freq },  \n'
        
        source_code += '};\n'
        source_code += '\n'
        
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
    
    file.write('#if __IGNORE\n')
        
    for mcu in mcus:
        name = mcu.attrib['RefName'] 
        name = name[:11]
        
        if name in name_processed:
            continue
        name_processed.append(name)
        
        dir = system_dir + name[:7] + "/CMSIS_Inc/"
        #filename = name.lower()
        define = find_header(name)
        if not define:
            continue
        
        file.write('\n');
        file.write('#elif defined(' + name + ')\n')
        file.write('  #define ' + define + ' 1\n')
        file.write('  #define CMSIS_STARTUP_ASSEMBLY "startup_' + define.lower() + '.s"\n')
        file.write('  #define CHIP_PERIPHERAL_INCLUDE "stm32_' + name + '.h"\n')
        
        mcu = load_mcu(name)
        mcu.load_rcc()
        mcu.find_remaps()
        mcu.process_pins()
        mcu.generate_source_code()
        
        file.write('  ' + mcu.default_pin_list)
        
    file.write('#else \n')
    file.write('#error UNKNOWN CHIP \n')
    file.write('#endif\n')
    
    file.write('\n')
    file.write('#endif')

    
    