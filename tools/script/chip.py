import xml.etree.ElementTree as ET
import os.path
from os import listdir
import sys
import re
import json

import config

header_lines = []

for serie in ['f0', 'f1', 'f2', 'f3', 'f4', 'f7', 'l0', 'l1', 'l4']:
    with open(config.system_dir + 'STM32' + serie.upper() + '/CMSIS_Inc/stm32' + serie + 'xx.h') as f:
        header_lines.extend(f.readlines())

# Returns product line for a chip: STM32F103CB -> STM32F103xB
def get_productline_for_include(name):
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
    
    return include


families_xml = os.path.join(config.cubemx_dir, 'db', 'mcu', 'families.xml')
if not os.path.exists(families_xml):
    print 'Could not find CubeMX, please set CubeMX directory'
    exit(-1)
families = ET.parse(families_xml).getroot()
ns = {'stm': 'http://mcd.rou.st.com/modules.php?name=mcu'}

mcus = families.findall(".//Mcu")

xml_cache = {}

def load_xml(path):
    if path in xml_cache:
        return xml_cache[path]
    
    ret = ET.parse(path)
    xml_cache[path] = ret
    return ret

class load_mcu:
    def __init__(self, name):
        self.mcu_name = name
        
        if families.find(".//Mcu[@RefName='" + self.mcu_name + "Tx']") is not None:
            self.mcu_xml_filename = families.find(".//Mcu[@RefName='" + self.mcu_name + "Tx']").attrib['Name'] + '.xml'
        else:
            self.mcu_xml_filename = families.find(".//Mcu[@RPN='" + self.mcu_name + "']").attrib['Name'] + '.xml'
        
        self.mcu_xml = load_xml(os.path.join(config.cubemx_dir, 'db', 'mcu', self.mcu_xml_filename)).getroot()
        self.remap_xml_filename = 'GPIO-' + self.mcu_xml.find("stm:IP[@Name='GPIO']", ns).attrib['Version'] + '_Modes.xml'

        self.remap_xml = load_xml(os.path.join(config.cubemx_dir, 'db', 'mcu', 'IP', self.remap_xml_filename )).getroot()

        self.mcu_pins = self.mcu_xml.findall('stm:Pin',  ns);

        self.pin_to_instance_signals = {}
        
        self.instance_signal_to_pin_to_af = {}
        self.instance_signal_to_default_pin = {}
        
        self.af_functions=[]
        self.af_function_used=[]
        self.pins = []
        self.instances = []
        self.peripheral_to_signals = {}
        self.peripheral_to_instances = {}
        self.peripheral_signal_to_instance_signals = {}
        
        self.load_rcc()
        self.find_remaps()
        self.process_pins()

        self.instances.sort()
        self.af_functions.sort()
        

    def load_rcc(self):
        self.rcc_xml = 'RCC-' + self.mcu_xml.find("stm:IP[@Name='RCC']", ns).attrib['Version'] + '_Modes.xml'
        
        self.rcc_root = load_xml(os.path.join(config.cubemx_dir, 'db', 'mcu', 'IP', self.rcc_xml )).getroot()
        
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
        for pin_element in self.mcu_xml.findall('stm:Pin',  ns):
            for signal_element in pin_element.findall('stm:Signal',  ns):
                pin_name = pin_element.attrib['Name']
                pin_name = re.search('^(\D*\d*)', pin_name).group(1)
                
                instance_signal = signal_element.attrib['Name']
                
                self.pin_to_instance_signals.setdefault(pin_name, []).append(instance_signal)
        
        for pin in self.remap_xml.findall('stm:GPIO_Pin',  ns):
            pin_name = pin.attrib['Name']
            
            pin_name = re.search('^(\D*\d*)', pin_name).group(1)
            
            gpio_signals = pin.findall('stm:PinSignal',  ns)
            for gpio_signal in gpio_signals:
                instance_signal = gpio_signal.attrib['Name']
                
                if pin_name not in self.pin_to_instance_signals or instance_signal not in self.pin_to_instance_signals[pin_name]:
                    continue
                
                instance = instance_signal.split('_')[0]
                if instance_signal not in self.instance_signal_to_pin_to_af:
                    self.instance_signal_to_pin_to_af[instance_signal] = {}
                
                if self.mcu_name.startswith('STM32F1'):
                    remap_function = False
                    
                    remap_block = gpio_signal.find('stm:RemapBlock', ns)
                    if remap_block is not None:
                        if 'DefaultRemap' in remap_block.attrib:
                            remap_function = 'AF__HAL_AFIO_REMAP_' + instance + '_DISABLE'
                            self.instance_signal_to_default_pin[instance_signal] = pin_name
                        else:
                            remap_function = 'AF' + remap_block.find('stm:SpecificParameter', ns).find('stm:PossibleValue', ns).text
                    else:
                        remap_function = 'AF__NO_REMAP'
                        if instance_signal not in self.instance_signal_to_default_pin:
                            self.instance_signal_to_default_pin[instance_signal] = pin_name
                            
                    self.instance_signal_to_pin_to_af[instance_signal][pin_name] = remap_function
                    if remap_function not in self.af_functions:
                        self.af_functions.append(remap_function)
                    
                else:
                    gpio_af = gpio_signal.find("stm:SpecificParameter[@Name='GPIO_AF']", ns)
                    self.instance_signal_to_pin_to_af[instance_signal][pin_name] = gpio_af.find("stm:PossibleValue", ns).text

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
                    and not instance_signal.startswith('ADC') \
                    and not instance_signal.startswith('ADC_') \
                    and not instance_signal.startswith('I2S') \
                    and not instance_signal.startswith('SDIO') \
                    and not instance_signal.startswith('SDMMC'):
                    pass
                    #continue
                
                if '_' not in instance_signal: continue
                #if 'ETH' in instance_signal: print instance_signal
                
                (instance, sig) = instance_signal.split('_', 1)

                if instance == 'SDIO':
                    instance = 'SDIO1'
                if instance == 'SDMMC1':
                    instance = 'SDIO1'
                if instance == 'SDMMC2':
                    instance = 'SDIO2'
                    
                if instance[-1].isdigit() and instance[-2].isdigit():
                    periph = instance[:-2]
                elif instance[-1].isdigit():
                    periph = instance[:-1]
                else:
                    periph = instance
                
                if instance not in self.instances:
                    self.instances.append(instance)

                self.peripheral_to_instances.setdefault(periph, [])
                if instance not in self.peripheral_to_instances[periph]:
                    self.peripheral_to_instances[periph].append(instance)
                
                self.peripheral_to_signals.setdefault(periph, []).append(sig)
                
                self.peripheral_signal_to_instance_signals.setdefault(periph + '_' + sig, []).append(instance_signal)
                
                if instance_signal not in self.instance_signal_to_default_pin:
                    self.instance_signal_to_default_pin[instance_signal] = pin_name

                if instance_signal not in self.instance_signal_to_pin_to_af:
                    self.instance_signal_to_pin_to_af[instance_signal] = {}
                    self.instance_signal_to_pin_to_af[instance_signal][pin_name] = 'AF_NO_REMAP'
