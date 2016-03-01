#!/usr/bin/python

import sys
#print(sys.version_info)

suffix = '.fix'

if len(sys.argv) < 2:
    sys.exit("Please provide the name of a data file")

for filename in sys.argv[1:]:
    print('Processing ', filename, '->', filename + suffix, '... ', end='')
    first = True
    with open(filename) as f_in:
        with open(filename + suffix, 'w') as f_out:
            for line in f_in.readlines():
                split = line.split()
                if len(split) == 2:
                    [new1, new2] = split
                    if first:
                        old1, old2 = 0, new2
                        first = False
                    f_out.write(str(new1) + '\t' + str(old2) + '\n')
                    f_out.write(str(new1) + '\t' + str(new2) + '\n')
                    old1, old2 = new1, new2
                else:
                    f_out.write('\n')
                    first = True
    print('Done')
print('All tasks done')