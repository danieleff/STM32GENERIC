# Compile from command line

# Example: python run_build.py -v MapleMini_F103CB -s STM32\libraries\HardwareTest\examples\AutomaticCompilation\Basic\Basic.ino -i

import sys
sys.dont_write_bytecode = True

import subprocess
import os
import fnmatch
import shutil
import errno
import argparse
import board
import json
import hashlib
import collections

#print(board.boards)
#exit(-1)

hardware_root = os.path.abspath('../../../')
stm32generic_root = os.path.abspath('../../')
arch_root = os.path.join(stm32generic_root, 'STM32')
tools_root = os.path.join(stm32generic_root, 'tools')

arduino_builder_default = "C:\\Program Files (x86)\\Arduino\\arduino-builder.exe"
arm_gcc_path_default = "C:\\Users\\" + str(os.environ.get('USERNAME')) + "\\AppData\\Local\\Arduino15\\packages\\arduino\\tools\\arm-none-eabi-gcc\\4.8.3-2014q1"

git_hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode("ascii").strip()
git_date = subprocess.check_output(['git', 'show', '-s', '--format=%ci']).decode("ascii").strip()

def build(args, build_target, sketch_path, build_path, include_external_libraries):
    print("Building: ", sketch_path) 
    print("Variant: ", build_target) 
    print("Build path: ", build_path) 
    
    result_filename = os.path.join(build_path, "info.json")
    
    if os.path.exists(result_filename) and args.ignore_already_compiled:
        return
    
    if os.path.exists(build_path):
        shutil.rmtree(build_path)
    
    os.makedirs(build_path, exist_ok = True)

    command = []
    command.append(args.arduino_builder)
    
    command.append("-hardware")
    command.append(hardware_root)
    
    command.append("-hardware")
    command.append(os.path.dirname(args.arduino_builder) + "/hardware")
    
    command.append("-tools")
    command.append(os.path.dirname(args.arduino_builder) + "/tools-builder")
    
    command.append("-ide-version=10606")
    
    command.append("-fqbn")
    command.append(build_target)
    
    
    #command.append("-warnings=all")
    
    command.append("-verbose")
    
    command.append("-build-path")
    command.append(os.path.abspath(build_path))
    
    if include_external_libraries:
        command.append("-libraries")
        command.append(args.external_libraries)
    
    command.append("-prefs")
    if 'F7' in build_target:
        command.append("runtime.tools.arm-none-eabi-gcc.path=" + args.arm_gcc_path_f7)
    else:
        command.append("runtime.tools.arm-none-eabi-gcc.path=" + args.arm_gcc_path)
    
    command.append(sketch_path)
    
    print(command)
    
    with open(os.path.join(build_path, "stdout.txt"), "w") as stdout, open(os.path.join(build_path, "stderr.txt"), "w") as stderr:
        proc = subprocess.Popen(command, stdout = stdout, stderr = stderr)
        #proc = subprocess.Popen(command)
    
    proc.wait()
    
    for dir in os.listdir(build_path):
        if os.path.isdir(os.path.join(build_path, dir)):
            shutil.rmtree(os.path.join(build_path, dir))
    
    with open(os.path.join(build_path, "disassembly.asm"), "w") as f:
        proc = subprocess.Popen([os.path.join(args.arm_gcc_path, "bin", "arm-none-eabi-objdump"), '-d', os.path.join(build_path, os.path.basename(sketch_path) + ".elf")], stdout = f)
    
    with open(os.path.join(build_path, "nm.txt"), "w") as f:
        proc = subprocess.Popen([os.path.join(args.arm_gcc_path, "bin", "arm-none-eabi-nm"), '-S', '--size-sort', '-t', 'd', os.path.join(build_path, os.path.basename(sketch_path) + ".elf")], stdout = f)
        
    with open(result_filename, "w") as f:
        info = {"build_target": build_target, "sketch_path": sketch_path, "build_path": build_path, "git_hash": git_hash}
        json.dump(info, f, indent=4)

def find_inos(path):
    ret = []
    for root, dirnames, filenames in os.walk(path):
        for filename in fnmatch.filter(filenames, '*.ino'):
            ret.append(os.path.join(root, filename))
    return sorted(ret)
    
            
def run(args, build_target, sketch, include_external_libraries = False):
    
    sketch_path = sketch
    
    if not sketch.startswith('/') and ":\\" not in sketch:
        sketch_path = stm32generic_root + '/' + sketch
        
    sketch = os.path.relpath(sketch, stm32generic_root)
    
    hash = hashlib.sha256(sketch.encode('utf-8')).hexdigest()[:10].strip()
        
    build_path = os.path.join(os.path.abspath("."), "build", git_hash, build_target.replace(":", "_").replace(",", "_").replace("=", "_"), hash)
            
    build(args, build_target, sketch_path, build_path, include_external_libraries)
    
    
def run_manual(args):
    for variant in args.variant:
        for sketch in args.sketch:
            run(args, "STM32GENERIC:STM32:" + variant, sketch)
            

def run_auto(args):
    
    # Common stuff, should mostly work on all boards
    variants = [
        "MapleMini_F103CB:upload_method=STLinkMethod,usb=Disabled",
        "MapleMini_F103CB:upload_method=STLinkMethod,usb=SerialUSB",
        "MapleMini_F103CB:upload_method=DFUUploadMethod2,usb=Disabled",
        "MapleMini_F103CB:upload_method=DFUUploadMethod2,usb=SerialUSB",
        
        "BluePill:upload_method=STLinkMethod,usb=SerialUSB",
        
        "BLACK_F407XX:upload_method=STLinkMethod,usb=Disabled,subboard=BLACK_F407VE",
        "BLACK_F407XX:upload_method=STLinkMethod,usb=SerialUSB,subboard=BLACK_F407VE",
    ]
    
    for variant in board.boards:
        if 'Nucleo' in variant:
            if variant[11] == 'R': # Nucleo 64
                variants.append('NUCLEO_64:subboard=' + variant)
            else:
                variants.append('NUCLEO_144:subboard=' + variant)
                
        if 'DISCO' in variant:
            variants.append(variant + ":usb=Disabled,serial=SerialUART1") # TODO sometimes serial=SerialUART2

    for variant in variants:
        inos = []
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "HardwareTest", "examples", "AutomaticCompilation")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32/libraries/BoardExamples/examples/all_boards")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "HardwareTimer", "examples")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "FreeRTOS", "examples")))
        inos.extend(find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "EEPROM", "examples")))
        
        for sketch in inos:
            run(args, "STM32GENERIC:STM32:" + variant, sketch)
            
    # Board specific stuff:
    inos = find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "BoardExamples", "examples", "BlackF407VE"))
    for sketch in inos:
        run(args, "STM32GENERIC:STM32:BLACK_F407XX:upload_method=STLinkMethod,usb=Disabled,subboard=BLACK_F407VE", sketch, True)
    
    inos = find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "BoardExamples", "examples", "BluePillF103"))
    for sketch in inos:
        run(args, "STM32GENERIC:STM32:BluePill:upload_method=STLinkMethod,usb=SerialUSB", sketch, True)
    
    inos = find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "BoardExamples", "examples", "Discovery407VG"))
    for sketch in inos:
        run(args, "STM32GENERIC:STM32:DISCOVERY_F407VG:usb=Disabled,serial=SerialUART2", sketch, True)
    
    inos = find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "BoardExamples", "examples", "Discovery429ZI"))
    for sketch in inos:
        run(args, "STM32GENERIC:STM32:DISCOVERY_F429ZI:usb=Disabled,serial=SerialUART1", sketch, True)
    
    inos = find_inos(os.path.join(stm32generic_root, "STM32", "libraries", "BoardExamples", "examples", "Discovery746NG"))
    for sketch in inos:
        run(args, "STM32GENERIC:STM32:DISCOVERY_F746NG:usb=Disabled,serial=SerialUART1", sketch, True)
    
    
    # all libraries:
    if args.external_libraries:
        inos = find_inos(os.path.join(stm32generic_root, args.external_libraries))
        for sketch in inos:
            run(args, "STM32GENERIC:STM32:MapleMini_F103CB:upload_method=STLinkMethod,usb=Disabled", sketch, True)

parser = argparse.ArgumentParser(description='Process some integers.')

parser.add_argument('-v', '--variant', choices = board.boards, action = 'append', default = [])
parser.add_argument('-s', '--sketch', action = 'append', default = [])
parser.add_argument('-b', '--arduino_builder', default = arduino_builder_default)
parser.add_argument('-g', '--arm_gcc_path', default = arm_gcc_path_default)
parser.add_argument('--arm_gcc_path_f7', default = arm_gcc_path_default)
parser.add_argument('-i', '--ignore_already_compiled', action="store_true")
parser.add_argument('-a', '--auto', action="store_true")
parser.add_argument('-o', '--output')
parser.add_argument('-e', '--external_libraries')

args = parser.parse_args()

if not args.auto and (not args.variant or not args.sketch):
    print("error: Please specify either -a or -v + -s")
    parser.print_help()
    exit(-1)
    
if not os.path.exists(args.arduino_builder):
    print("error: " + args.arduino_builder + " does not exist")
    print("       please use -b to set")
    exit(-1)
    
if not os.path.exists(args.arduino_builder):
    print("error: " + args.arm_gcc_path + " does not exist")
    print("       please use -g to set")
    exit(-1)

results = collections.OrderedDict()
if not args.auto:
    run_manual(args)
else:
    run_auto(args)
    
compiler = os.path.basename(os.path.dirname(args.arm_gcc_path)) + " " + os.path.basename(args.arm_gcc_path)


#json_data = {"core": "STM32GENERIC", "compiler": compiler, "git_date": git_date, "git_hash": git_hash, "results": results}

#with open("build/STM32GENERIC-" + git_hash + ".json", "w") as f:
#    json.dump(json_data, f, indent=4)
    
#if args.output:
#    with open(args.output, "w") as f:
#        json.dump(json_data, f, indent=4)
