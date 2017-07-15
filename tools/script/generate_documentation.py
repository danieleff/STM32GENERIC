import sys
sys.dont_write_bytecode = True

import re
from collections import OrderedDict

import board
import chip

with open("../documentation/mkdocs/build_macros.md", "w") as f:
    
    f.write('When compiling, the following build macro definitions are set:\n')

    f.write('### **`ARDUINO_ARCH_STM32`**:\n')

    f.write('Meaning: \n')
    f.write('\n')
    f.write('* The code is compiling on Arduino\n')
    f.write('* The code is compiling for STM32 chip\n')
    f.write('* CMSIS is present and useable. (For example: `GPIOA->BSRR`)\n')
    f.write('* The STM32 HAL API is present and useable. (For example: `HAL_GPIO_TogglePin(port, pin)`)\n')
    f.write('\n')
    f.write('This macro does not imply the avaibility of the STM32GENERIC private functions.\n')

    f.write('### Macros specific to board:\n')
    f.write('These can be used to differentiate between boards\n')
    f.write('Meaning: The code is compiled for that chip, board and form factor.\n')
    f.write('\n')

    f.write("Board | Build macro\n");
    f.write("------|-------\n");
    for id in sorted(board.boards):
        #print '------'
        #print id
        b = board.boards[id]
        name = b["name"]
        build_board = b["build.board"]
        mcu = b["build.extra_flags"].split(" ")[0][2:]
        f.write(name.ljust(30) + " | **`ARDUINO_" + build_board + "`** \n")

def natural_sort_key(s, _nsre=re.compile('([0-9]+)')):
    return [int(text) if text.isdigit() else text.lower()
            for text in re.split(_nsre, s)] 

def get_board_pin_number(b, pin):
    if pin in b["pins"]:
        return str(b["pins"][pin]) + ' (' + pin + ')'
    else:
        return pin

def get_pins(mcu, b, instance_signal):
    ret = []
    try:
        default_pin = sorted(mcu.instance_signal_to_pin_to_af[instance_signal], key=natural_sort_key)[0]
        
        if (instance_signal == 'SPI1_MISO'): default_pin = b["macros"]["MISO"]
        if (instance_signal == 'SPI1_MOSI'): default_pin = b["macros"]["MOSI"]
        if (instance_signal == 'SPI1_SCK'): default_pin = b["macros"]["SCK"]
        if (instance_signal == 'I2C1_SDA'): default_pin = b["macros"]["SDA"]
        if (instance_signal == 'I2C1_SCL'): default_pin = b["macros"]["SCL"]
        
        for pin in sorted(mcu.instance_signal_to_pin_to_af[instance_signal], key=natural_sort_key):
            if pin == default_pin:
                ret.append('**' + get_board_pin_number(b, pin) + '**')
            else:
                ret.append(get_board_pin_number(b, pin))
        
        if 'TIM' in instance_signal:
            for pin in sorted(mcu.instance_signal_to_pin_to_af[instance_signal + 'N'], key=natural_sort_key):
                if pin == default_pin:
                    ret.append('**<span style="text-decoration: overline">' + get_board_pin_number(b, pin) + '</span>**')
                else:
                    ret.append('<span style="text-decoration: overline">' + get_board_pin_number(b, pin) + '</span>')
            
        
    except:
        pass
        
    return ", ".join(ret)

def create_board_file(id):
    b = board.boards[id]
    c = b["build.extra_flags"][2:13]
    mcu = chip.load_mcu(c)
    
    with open("../documentation/mkdocs/board_" + id + ".md", "w") as f:
        f.write("#" + b["name"] + "\n")
        f.write("\n")
        f.write("Below are the pins usable for the peripherals. Pins in **bold** are the default.\n")
        
        periphs = OrderedDict()
        periphs["SPI"] = ["MOSI", "MISO", "SCK"]
        periphs["I2C"] = ["SDA", "SCL"]
        periphs["USART"] = ["RX", "TX"]
        periphs["I2S"] = ["CK", "SD", "WS", "MCK"]
        
        periphs["TIM"] = ["CH1", "CH2", "CH3", "CH4"]
        
        for (periph, signals) in periphs.iteritems():
            if periph not in mcu.peripheral_to_instances:
                continue
            
            f.write("\n")
            f.write("## " + periph + "\n")
            f.write("\n")
            f.write("Instance |")
            for signal in signals:
                f.write(signal + "|")
            f.write("\n")
            
            f.write("-|")
            for signal in signals:
                f.write("-|")
            f.write("\n")
            
            for instance in sorted(mcu.instances, key = natural_sort_key):
                if not instance.startswith(periph):
                    if not periph == 'USART' or not instance.startswith('UART'):
                        continue
                
                if instance == 'I2S': #TODO check how this got in here
                    continue
                        
                f.write(instance + "|")
                for signal in signals:
                    f.write(get_pins(mcu, b, instance + '_' + signal) + "|")
                f.write("\n")
            
        f.write('\n')
        f.write('## ADC \n')
        f.write('\n')
        f.write('Instance | Channel | Pin\n')
        f.write('-|-|-\n')
            
        for instance in sorted(mcu.instances):
            if 'ADC' not in instance:
                continue
            
            for instance_signal in sorted(mcu.instance_signal_to_default_pin, key=natural_sort_key):
                if 'IN' not in instance_signal:
                    continue
                    
                if not instance_signal.startswith(instance):
                    continue
                
                channel = instance_signal.split('_')[1]
                
                f.write(instance + '|')
                f.write(channel + '|')
                pin = mcu.instance_signal_to_default_pin[instance_signal]
                f.write(get_board_pin_number(b, pin) + '|\n')
        
        f.write('\n')
        f.write('## GPIO \n')
        f.write('\n')
        f.write('Pin | Peripheral signal available on the pin | Board macro\n')
        f.write('-|-|-\n')
        
        pins = sorted(mcu.pins, key = natural_sort_key)
        if len(b["pins"]) > 0:
            pins = b["pins"]
        
        for pin in pins:
            if not pin.startswith('P'):
                continue
            f.write(get_board_pin_number(b, pin) + " |")
            instance_signals = []
            
            for instance_signal in sorted(mcu.instance_signal_to_pin_to_af, key = natural_sort_key):
                if pin in mcu.instance_signal_to_pin_to_af[instance_signal]:
                    instance_signals.append(instance_signal)
            
            f.write(", ".join(instance_signals) + "|")
            
            board_macros = []
            for (key, value) in b["macros"].iteritems():
                if value == pin:
                    board_macros.append('**' + key + '**')
            
            f.write(", ".join(sorted(board_macros)) + "|\n")


with open("../documentation/mkdocs.yml") as f:
    mkdocs_yml = f.readlines()

mkdocs_yml_new = []

i = 0
index = 0
for x in mkdocs_yml:
    i = i + 1
    if ': board_' not in x and 'boards:' not in x:
        mkdocs_yml_new.append(x)
    if '- STM32 Boards:' in x:
        index = i

for name in board.tree.keys():
    if isinstance(board.tree[name], str):
        mkdocs_yml_new.insert(index, '    - ' + name + ": board_" + board.tree[name] + ".md\n")
        create_board_file(board.tree[name])
        
        index = index + 1
    else:
        mkdocs_yml_new.insert(index, '    - ' + name + ":\n")
        index = index + 1
        for subname in board.tree[name]:
            mkdocs_yml_new.insert(index, '        - ' + subname + ": board_" + board.tree[name][subname] + ".md\n")
            create_board_file(board.tree[name][subname])
            index = index + 1

with open("../documentation/mkdocs.yml", "w") as f:
    f.write("".join(mkdocs_yml_new))
