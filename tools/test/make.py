import subprocess
import os
import json

projects = [
    'SPI/src/',
    'Wire/src/',
    'I2S/src/',
    'SDIO/src/',
    'HardwareTest/examples/AutomaticCompilation/Basic/',
    'HardwareTest/examples/AutomaticCompilation/SPI/',    
    'HardwareTest/examples/AutomaticCompilation/I2C/',
    'HardwareTest/examples/AutomaticCompilation/UART/',
    'HardwareTest/examples/AutomaticCompilation/SDIO/',
    'HardwareTest/examples/AutomaticCompilation/I2S/',
    'HardwareTest/examples/AutomaticCompilation/USB/',
]

variants = os.listdir('makefiles')

results = {}

for project in projects:
    print 'Compiling: ' + project
    subprocesses = {}
    
    for variant in variants:
        
        path = 'build/' + variant + '/' + project
        
        if not os.path.exists(path):
            os.makedirs(path)
        
        
        with open(path + '/stdout.txt', 'w') as out, open(path + '/stderr.txt', 'w') as err:
            p = subprocess.Popen(['make', 'VARIANT=' + variant, 'PROJECT=' + project], stdout = out, stderr = err)
            subprocesses[variant] = p
    
    #for variant, p in subprocesses.iteritems():
            p.wait()
            
            results.setdefault(project, {})[variant] = (p.returncode == 0)
            
            if p.returncode == 0:
                print '.',
            else:
                print
                print variant + 'COMPILATION ERROR ! ' + str(p.returncode)

print
print 'FINISH'

print results

with open('result.json', 'w') as file:
    file.write(json.dumps(results))
