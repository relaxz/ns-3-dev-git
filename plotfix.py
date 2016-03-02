#!/usr/bin/env python

import sys, argparse
#print(sys.version_info)

suffix = '.fix'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('file', nargs='+', type=argparse.FileType('r'),
                        help='data file to process')
    args = parser.parse_args()
    for file in args.file:
        process(file)
        print('All tasks done')

def process(file):
    print('Processing ', file.name, '->', file.name + suffix, '... ', end='')
    first = True
    with open(file.name + suffix, 'w') as f_out:
        for line in file:
            split = line.split()
            if len(split) == 2:
                [new1, new2] = split
                if first:
                    old1, old2 = 0, new2
                    f_out.write(str(0) + '\t' + str(new2) + '\n')
                    first = False
                else:
                    f_out.write(str(new1) + '\t' + str(old2) + '\n')
                f_out.write(str(new1) + '\t' + str(new2) + '\n')
                old1, old2 = new1, new2
            else:
                f_out.write('\n')
                first = True
    print('Done')

if __name__ == '__main__':
    main()