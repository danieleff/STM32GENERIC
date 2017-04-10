import xml.etree.ElementTree as ET
import os.path
from os import listdir
import sys
import re

cubemx_dir = 'D:\programs\STM32Cube\STM32CubeMX'

#if len(sys.argv) == 1:
#    print 'Usage: ' + __file__ + ' STM32_MCU_NAME'
#    print 'For example: ' + __file__ + ' STM32F103C8'
#    exit(-1)

families_xml = os.path.join(cubemx_dir, 'db', 'mcu', 'families.xml')
if not os.path.exists(families_xml):
    print 'Could not find CubeMX, please set CubeMX directory'
    exit(-1)
families = ET.parse(families_xml).getroot()
ns = {'stm': 'http://mcd.rou.st.com/modules.php?name=mcu'}


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
                periph = signal.split('_')[0]
                if periph == 'GPIO':
                    continue
                
                if periph not in self.peripherals:
                    self.peripherals.append(periph)
                    
                if signal not in self.defaultremaps:
                    self.defaultremaps[signal] = pin_name

                if signal not in self.remaps:
                    self.remaps[signal] = {}
                    self.remaps[signal][pin_name] = 'AF_NO_REMAP'

    def generate_source_code(self):
        self.peripherals.sort()

        self.af_functions.sort()

        source_code = '';
        #source_code = '//DO NOT MODIFY, THIS FILE WAS AUTOMATICALLY GENERATED\n'
        #source_code += '//MCU name: ' + self.mcu_name+ '\n'
        #source_code += '//MCU  xml: ' + self.mcu_xml+ '\n'
        #source_code += '//GPIO remap/alternate function xml: ' + self.remap_xml+ '\n'
        source_code += '#include "stm32f1xx_hal.h"\n'
        source_code += '#include "spi_com.h"\n'
        source_code += '#include "twi.h"\n'
        source_code += '#include "uart.h"\n'
        source_code += '#include "analog.h"\n'
        source_code += '#include "variant.h"\n'
        source_code += '\n'
        
        gpio_pins = [pin for pin in self.pins if pin.startswith('P') and pin != 'PDR_ON']
        
        source_code += 'extern const PinDescription g_APinDescription[]=\n'
        source_code += '{\n'
        
        for pin in sorted(gpio_pins, key=lambda x: (x[0:2], int(x[2:]))):
        
            source_code += '  { ' + pin.ljust(4) + ', GPIO_PIN_' + pin[2:].ljust(2) + ', GPIO' +  pin[1:2] + ', GPIO_PIN_IO, false},\n'
            
        source_code += '};\n'
        source_code += '\n'
        
        source_code += '\n'
        
        if len(self.af_functions) > 0:
            source_code += 'static void AF_NO_REMAP (void) { }\n'
            
        for af_function in self.af_functions:
            if 'SPI' in af_function:
                source_code += 'static void ' + af_function + '(void) { ' + af_function[2:] + '(); }\n'

        source_code += '\n'
        source_code += 'spi_init_info_t spi_init_info[] = { \n'
        
        for periph in self.peripherals:
            if periph.startswith('SPI'):
            
                if periph+"_MOSI" not in self.defaultremaps: 
                    continue 
                    
                source_code += '  { \n'
                
                source_code +=  '    .init_done = 0, \n'
                source_code +=  '    .spi_instance = ' + periph + ', \n'
                
                if self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .spi_alternate = ' + self.remaps[periph+"_MOSI"][self.defaultremaps[periph+"_MOSI"]] + ',\n'
                    
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .mosi_alternate = ' + self.remaps[periph+"_MOSI"][self.defaultremaps[periph+"_MOSI"]] + ',\n'
                source_code +=  '    .mosi_port = GPIO' + self.defaultremaps[periph+"_MOSI"][1:2] + ', \n'
                source_code +=  '    .mosi_pin =  GPIO_PIN_' + self.defaultremaps[periph+"_MOSI"][2:] + ', \n'
                
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .miso_alternate = ' + self.remaps[periph+"_MISO"][self.defaultremaps[periph+"_MISO"]] + ',\n'
                source_code +=  '    .miso_port = GPIO' + self.defaultremaps[periph+"_MISO"][1:2] + ', \n'
                source_code +=  '    .miso_pin = GPIO_PIN_' + self.defaultremaps[periph+"_MISO"][2:] + ', \n'
                
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .sck_alternate = ' + self.remaps[periph+"_SCK"][self.defaultremaps[periph+"_SCK"]] + ',\n'
                source_code +=  '    .sck_port = GPIO' + self.defaultremaps[periph+"_SCK"][1:2] + ', \n'
                source_code +=  '    .sck_pin = GPIO_PIN_' + self.defaultremaps[periph+"_SCK"][2:] + ', \n'
                
                source_code += '  },\n'
        source_code += '}; \n'
        
        source_code += '\n'
        source_code += 'uint8_t NB_SPI_INSTANCES = sizeof(spi_init_info) / sizeof(spi_init_info[0]);\n'
        
        
        for af_function in self.af_functions:
            if 'I2C' in af_function:
                source_code += 'static void ' + af_function + '(void) { ' + af_function[2:] + '(); }\n'

        source_code += '\n'
        source_code += 'i2c_init_info_t g_i2c_init_info[] = {\n'
        
        for periph in self.peripherals:
            if periph.startswith('I2C'):
                source_code += '  {\n'
                
                source_code +=  '    .init_done = 0,\n'
                source_code +=  '    .i2c_instance = ' + periph + ',\n'
                source_code +=  '    .irq = ' + periph + '_EV_IRQn,\n'
                
                if self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .i2c_alternate = ' + self.remaps[periph+"_SDA"][self.defaultremaps[periph+"_SDA"]] + ',\n'
                
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .sda_alternate = ' + self.remaps[periph+"_SDA"][self.defaultremaps[periph+"_SDA"]] + ',\n'
                source_code +=  '    .sda_port = GPIO' + self.defaultremaps[periph+"_SDA"][1:2] + ',\n'
                source_code +=  '    .sda_pin = GPIO_PIN_' + self.defaultremaps[periph+"_SDA"][2:] + ',\n'
                
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .scl_alternate = ' + self.remaps[periph+"_SCL"][self.defaultremaps[periph+"_SCL"]] + ',\n'
                source_code +=  '    .scl_port = GPIO' + self.defaultremaps[periph+"_SCL"][1:2] + ',\n'
                source_code +=  '    .scl_pin = GPIO_PIN_' + self.defaultremaps[periph+"_SCL"][2:] + ',\n'
                
                source_code +=  '    .i2c_onSlaveReceive = NULL,\n'
                source_code +=  '    .i2c_onSlaveTransmit = NULL,\n'
                source_code +=  '    .i2cTxRxBufferSize = 0\n'
                
                source_code += '  },\n'
                
        source_code += '};\n'
        source_code += '\n'
        
        source_code += 'uint8_t NB_I2C_INSTANCES = sizeof(g_i2c_init_info) / sizeof(g_i2c_init_info[0]);\n'
        
        source_code += '\n'
        
        for af_function in self.af_functions:
            if 'USART' in af_function:
                source_code += 'static void ' + af_function + '(void) { ' + af_function[2:] + '(); }\n'

        source_code += '\n'
        source_code += 'uart_conf_t g_uart_config[] = {\n'
        for periph in self.peripherals:
            if periph.startswith('USART'):
                if periph+"_RX" not in self.defaultremaps: 
                    continue # TX only USART
                    
                source_code += '  {\n'
                source_code += '    //UART ID and IRQ\n'
                
                source_code +=  '    .usart_typedef = ' + periph + ', .irqtype = ' + periph + '_IRQn,\n'
                source_code +=  '    .tx_port = GPIO' + self.defaultremaps[periph+"_TX"][1:2] + ', .tx_pin = GPIO_PIN_' + self.defaultremaps[periph+"_TX"][2:] + ',\n'
                source_code +=  '    .rx_port = GPIO' + self.defaultremaps[periph+"_RX"][1:2] + ', .rx_pin = GPIO_PIN_' + self.defaultremaps[periph+"_RX"][2:] + ',\n'
                
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .uart_af_remap = ' + self.remaps[periph+"_TX"][self.defaultremaps[periph+"_TX"]] + ',\n'
                
                if not self.mcu_name.startswith('STM32F1'):
                    source_code +=  '    .tx_alternate  = ' + self.remaps[periph+"_TX"][self.defaultremaps[periph+"_TX"]] + ',\n'
                    source_code +=  '    .rx_alternate  = ' + self.remaps[periph+"_RX"][self.defaultremaps[periph+"_RX"]] + ',\n'
                
                source_code += '    .data_available = 0,\n'
                source_code += '    .begin = 0,\n'
                source_code += '    .end = 0,\n'
                source_code += '    .uart_option = NATIVE_UART_E\n'
                
                source_code += '  },\n'
        source_code += '};\n'
        
        source_code += '\n'
        source_code += 'uint8_t NB_UART_MANAGED = sizeof(g_uart_config) / sizeof(g_uart_config[0]);\n'
        source_code += '\n'
        source_code += 'UART_HandleTypeDef g_UartHandle[sizeof(g_uart_config) / sizeof(g_uart_config[0])];\n'
        source_code += '\n'
        
        for af_function in self.af_functions:
            if 'TIM' in af_function:
                source_code += 'static void ' + af_function + '(void) { ' + af_function[2:] + '(); }\n'

        source_code += '\n'
        source_code += '#define SAMPLINGTIME  ADC_SAMPLETIME_1CYCLE_5  /*!< ADC conversions sampling time. */\n'
        source_code += '\n'
        
        source_code += 'adc_config_str g_adc_config[] = {\n'
          
        for periph in self.peripherals:
            if periph.startswith('ADC1'):
                for signal in sorted(self.defaultremaps.keys()):
                    if 'EXTI' in signal:
                        continue
                    if signal.startswith(periph):
                        source_code += '  {\n'
                        
                        source_code +=  '    .port = GPIO' + self.defaultremaps[signal][1:2] + ',\n'
                        source_code +=  '    .pin = GPIO_PIN_' + self.defaultremaps[signal][2:] + ',\n'
                        source_code +=  '    .adcInstance = ' + periph + ',\n'
                        source_code +=  '    .adcChannelConf = { .Channel = ADC_CHANNEL_' + signal[7:] + ', .Rank = ADC_REGULAR_RANK_1, .SamplingTime = SAMPLINGTIME }\n'
                        
                        source_code += '  },\n'
        source_code += '};\n'
        
        
        source_code += '#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101xE) || defined (STM32F101xG) || defined (STM32F103xE) || defined (STM32F103xG) || defined (STM32F105xC) || defined (STM32F107xC)\n'
        source_code += 'dac_config_str g_dac_config[] = {\n'
        source_code += '    \n'
        source_code += '};\n'
        source_code += '#endif\n'

        source_code += '\n'
        source_code += 'pwm_config_str g_pwm_config[] = {\n'
        
        for periph in self.peripherals:
            if periph.startswith('TIM'):
                for signal in sorted(self.defaultremaps.keys()):
                    if not 'CH' in signal:
                        continue
                    if signal.startswith(periph + "_"):
                        if self.defaultremaps[signal] not in self.remaps[signal]:
                            continue #TODO check STM32L151VD / TIM9_CH2 / PE6
                        
                        channel = signal.split('_')[1]
                            
                        source_code += '  {\n'
                        
                        source_code +=  '    .port = GPIO' + self.defaultremaps[signal][1:2] + ',\n'
                        source_code +=  '    .pin = GPIO_PIN_' + self.defaultremaps[signal][2:] + ',\n'
                        source_code +=  '    .alFunction = ' + self.remaps[signal][self.defaultremaps[signal]] + ',\n'
                        source_code +=  '    .timInstance = ' + periph + ',\n'
                        source_code +=  '    .timChannel = TIM_CHANNEL_' + channel[2:3] + ',\n'
                        
                        source_code +=  '    .useNchannel = ' + ('1' if channel[3:] else '0') + ',\n'
                        
                        source_code +=  '    .timConfig = {\n'
                        source_code +=  '      .OCMode       = TIM_OCMODE_PWM1,\n'
                        source_code +=  '      .OCPolarity   = TIM_OCPOLARITY_HIGH,\n'
                        source_code +=  '      .OCFastMode   = TIM_OCFAST_DISABLE,\n'
                        source_code +=  '      .OCNPolarity  = TIM_OCNPOLARITY_HIGH,\n'
                        source_code +=  '      .OCNIdleState = TIM_OCNIDLESTATE_RESET,\n'
                        source_code +=  '      .OCIdleState  = TIM_OCIDLESTATE_RESET\n'
                        source_code += '     },\n'
                        source_code += '     .timHandle = {}\n'
                        source_code += '  },\n'
                        
        source_code += '};\n'
        source_code += '\n'
        source_code += 'uint8_t NB_ADC_INSTANCES = sizeof(g_adc_config) / sizeof(g_adc_config[0]);\n'
        source_code += '#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101xE) || defined (STM32F101xG) || defined (STM32F103xE) || defined (STM32F103xG) || defined (STM32F105xC) || defined (STM32F107xC)\n'
        source_code += 'uint8_t NB_DAC_INSTANCES = sizeof(g_dac_config) / sizeof(g_dac_config[0]);\n'
        source_code += '#endif\n'
        source_code += 'uint8_t NB_PWM_INSTANCES = sizeof(g_pwm_config) / sizeof(g_pwm_config[0]);\n'
        
        source_code += '\n'
        source_code += 'remaps_t remaps {\n'
        
        for signal in sorted(self.remaps):
            if signal.startswith('SPI') or signal.startswith('I2C') or signal.startswith('USART') or signal.startswith('TIM'):
                for pin in self.remaps[signal]:
                    if pin in self.pins:
                        remap = self.remaps[signal][pin]
                        
                        split = signal.split('_')
                        periph = split[0]
                        periph_signal = split[1]
                        
                        if periph_signal not in ['SDA', 'SCL', 'TX', 'RX', 'MISO', 'MOSI', 'SCK'] and not periph_signal.startswith('CH'):
                            continue
                        
                        source_code += '    { ' + periph.ljust(6) + ', ' + periph_signal.ljust(5) + ', GPIO' + pin[1:2] + ', GPIO_PIN_' + pin[2:].ljust(3) + ', ' + remap.ljust(35) + '},\n'
        source_code += '};\n'
        
        return source_code
     
    def generate_header_code(self, labels = {}):
        source_header = '';
        #source_header += '#ifndef PINDEFINITIONS_H\n'
        #source_header += '#define PINDEFINITIONS_H\n'
        source_header += '\n'
        
        gpio_pins = [pin for pin in self.pins if pin.startswith('P') and pin != 'PDR_ON']
        
        periphs = []
        
        if labels:
            periphs.append('BOARD LABEL')
            
        periphs.extend([p for p in self.peripherals if ('SPI' in p or 'I2C' in p or 'USART' in p)])
        periphs.append('ADC1')
        periphs.append('TIM')
        periphs.append('USB')
        periphs.append('SDIO')
        periphs.append('RCC')
        periphs.append('SYS')
        
        print_signals = [
            'BOARD LABEL',
            'MISO', 'MOSI', 'SCK', #SPI
            'SDA', 'SCL', #I2C
            'TX', 'RX', #USART
            '_IN', #ADC
            '_CH', #TIM PWM
            'DM', 'DP', #USB
            'OSC', #Exterlan Oscillator
            'JT', 'SW', 'TRACE', #J-TAG / ST-Link
            ]
        column_sizes = [0] * len(periphs)
        
        for pin in sorted(gpio_pins, key=lambda x: (x[0:2], int(x[2:]))):
            column = 0;
            for periph in periphs:
                x = ''
                for signal in self.remaps:
                    if periph != 'SDIO' and not any(sig in signal for sig in print_signals):
                        continue
                    
                    if periph in signal:
                        for signal_pin in self.remaps[signal]:
                            if pin == signal_pin:
                                x += '?'
                                if 'TIM' in signal or '_' not in signal:
                                    x += signal + ' '
                                else:
                                    x += signal[signal.index('_') + 1:] + ' '
                if periph == 'BOARD LABEL' and pin in labels:
                    x = labels[pin]
                    
                column_sizes[column] = max(column_sizes[column], len(x.strip()))
                column = column + 1;

        source_header += 'enum {\n'
        source_header += '//*=default , +=alternate\n'
        source_header += '//PIN    '
        column = -1
        for periph in periphs:
            column = column + 1
            if column_sizes[column] == 0:
                continue
            column_sizes[column] = max(column_sizes[column], len(periph))
            if periph == 'TIM':
                periph += ' (PWM)'
            source_header += '|' + periph.ljust(column_sizes[column])
                
        source_header += '\n'
        for pin in sorted(gpio_pins, key=lambda x: (x[0:2], int(x[2:]))):
        
            source_header += pin.ljust(4) + ', // '
            
            column = -1
            for periph in periphs:
                column = column + 1
                if column_sizes[column] == 0:
                    continue
                x = ''
                for signal in self.remaps:
                    if periph != 'SDIO' and not any(sig in signal for sig in print_signals):
                        continue
                    
                    if periph in signal:
                        for signal_pin in sorted(self.remaps[signal]):
                            if pin == signal_pin:
                                if signal in self.defaultremaps and pin == self.defaultremaps[signal]:
                                    x += '*'
                                else:
                                    x += '+'
                                if 'TIM' in signal or '_' not in signal:
                                    x += signal + ' '
                                else:
                                    x += signal[signal.index('_') + 1:] + ' '
                if periph == 'BOARD LABEL' and pin in labels:
                    x = labels[pin]
                source_header += '|' + x.strip().ljust(column_sizes[column])
            source_header += '\n'
        
        source_header += 'NB_DIGITAL_PINS,\n'
        source_header += '};\n'
        source_header += '\n'
        #source_header += '#endif\n'
        
        return source_header

def generate_pindefinitions(mcu, variant, labels = {}, filename = 'pindefinitions'):
    mcu_parser = load_mcu(mcu)

    mcu_parser.find_remaps()

    mcu_parser.process_pins()

    source_code = mcu_parser.generate_source_code()

    header_code = mcu_parser.generate_header_code(labels)

    with open(os.path.join('variants', variant, filename + '.c'), 'w') as file:
        file.write(header_code)
        file.write(source_code)

    #with open(os.path.join('variants', variant, filename + '.h'), 'w') as file:
    #    file.write(header_code)

mcus=[]
for mcu_element in families.findall(".//Mcu"):
    mcus.append(mcu_element.attrib['RPN'].split('-')[0]);
mcus = sorted(set(mcus))


remaps_for_cpu = {}

for mcu in mcus:
    #if 'R' not in mcu:
    #    continue
    
    mcu_parser = load_mcu(mcu)
    
    mcu_parser.find_remaps()
    
    mcu_parser.process_pins()
    
    #print sorted(mcu_parser.remaps.keys())
    #print 
    
    for signal_pin in mcu_parser.defaultremaps:
        if 'SPI1_MOSI' not in signal_pin:
            continue
        
        key = signal_pin + '_' + mcu_parser.defaultremaps[signal_pin]
        
        if key not in remaps_for_cpu:
            remaps_for_cpu[key] = []
            
        remaps_for_cpu[key].append(mcu)

    
for key, value in remaps_for_cpu.iteritems():
    print key
    print '  ', value
    print

exit()
    
official_boards_dir = os.path.join(cubemx_dir, "db", "plugins", "boardmanager", "boards")
official_boards = os.listdir(official_boards_dir)
for board in official_boards:
    if 'AllConfig' not in board:
        continue
    
    if 'Evaluation' in board:
        name = board.split('_')[3]
    else:
        name = board.split('_')[2]
        
    mcu = False

    labels = {}
    
    with open(os.path.join(official_boards_dir, board)) as f:
        for line in f.readlines():
            if line.startswith('Mcu.UserName='):
                mcu = line[line.index('=') + 1:].strip()[0:11]
            if 'GPIO_Label=' in line:
                pin_name = re.search('^(\D*\d*)', line).group(1)
                pin_label = line[line.index('=') + 1:].strip()
                labels[pin_name] = pin_label

    #print name, mcu, labels
    generate_pindefinitions(mcu, 'pindefinition_examples', labels, 'Official ' + name)

generate_pindefinitions('STM32F103C8', 'pindefinition_examples', {'PC13':'LED'}, 'System Development Blue Board - Blue Pill')
generate_pindefinitions('STM32F103CB', 'pindefinition_examples', {'PB1':'LED', 'PB8': 'BOOT0/BUTTON', 'PB9': 'USB DISC'}, 'Leaflabs Maple Mini')
generate_pindefinitions('STM32F103VE', 'pindefinition_examples', {}, 'Vcc-gnd.com STM32F103VE')
generate_pindefinitions('STM32F103ZE', 'pindefinition_examples', {}, 'Vcc-gnd.com STM32F103ZE')
generate_pindefinitions('STM32F407VE', 'pindefinition_examples', {}, 'Vcc-gnd.com STM32F407VE')
