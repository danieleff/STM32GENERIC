import subprocess
import os

projects = [
    'SPI/src/',
    'Wire/src/',
    'HardwareTest/examples/AutomaticCompilation/Basic/',
    'HardwareTest/examples/AutomaticCompilation/SPI/',    
    'HardwareTest/examples/AutomaticCompilation/I2C/',
]

variants = os.listdir('makefiles')

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
            
            if p.returncode == 0:
                print '.',
            else:
                print
                print variant + 'COMPILATION ERROR ! ' + str(p.returncode)

print
print 'FINISH'
