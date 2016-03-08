#!/usr/bin/env python3
import sys, os, argparse
#print(sys.version_info) # known to work on python 3.5

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('datafile', type=argparse.FileType('r'),
                        help='data file to process')
    parser.add_argument('plotscript',
                        help='path to a gnuplot script to run when done')
    args = parser.parse_args()
    try:
        process(args.datafile, args.plotscript)
    except OSError as err:
        print('\n' + err, file=sys.stderr)

def process(file, script):
    suffix = '.tmp'
    print('Processing', file.name, '->', file.name + suffix, '... ', end='')
    first = True
    with open(file.name + suffix, 'w') as f_out:
        for line in file:
            # Because the speed is constant and gnuplot likes to draw straight
            # lines between the data points, we need to tell gnuplot to hold
            # each speed value and then instantly change speed.
            split = line.split()
            if len(split) == 2:
                [new1, new2] = split
                if first:
                    # Data is missing at simulation start.
                    # This happens because we can't hook our probes to the
                    # mobility models until we have created them.
                    old1, old2 = 0, new2
                    first = False
                f_out.write(str(new1) + '\t' + str(old2) + '\n')
                f_out.write(str(new1) + '\t' + str(new2) + '\n')
                old1, old2 = new1, new2
            else:
                f_out.write('\n')
                first = True
    print('Done')
    # backup the old file
    backup_name = file.name + '.bak'
    print('Renaming', file.name, '->', backup_name, '... ', end='')
    os.rename(file.name, backup_name)
    print('Done')
    print('Renaming', file.name + suffix, '->', file.name, '... ', end='')
    os.rename(file.name + suffix, file.name)
    print('Done')
    print('Running gnuplot', script, '... ', end='')
    os.system('gnuplot ' + script)
    print('Done')

if __name__ == '__main__':
    main()
