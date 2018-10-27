#!/usr/bin/python

import random, sys
import fileinput
from optparse import OptionParser

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def shuffleLines(self):
        random.shuffle(self.lines)
        return self.lines

def main():
    version_msg = "%prog 1.0"
    usage_msg = """%prog [OPTIONS]... FILE

Shuffles its input by outputting a random permutation of its input lines. Each output permutation is equally likely."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--head-count",
                      action="store", dest="count", default=-1,
                      help="Output at most count lines. By default, all input lines are output.")
    parser.add_option("-e", "--echo",
                      action="store_true", dest="echo", default=False,
                      help="Treat each command-line operand as an input line..")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="Repeat output values, that is, select with replacement. With this option the output is not a permutation of the input; instead, each output line is randomly chosen from all the inputs. This option is typically combined with --head-count; if --head-count is not given, shuf repeats indefinitely.")
    options, args = parser.parse_args(sys.argv[1:])

    lines = []
    std = False

    if len(args) == 0 or args[0] == '-':
        std = True
    else:
        input_file = args[0]

    try:
        if bool(options.echo):
            for i in args:
                lines.append(i + "\n")
        elif std:
            lines = sys.stdin.readlines()
        else:
            f = open(input_file, 'r')
            lines = f.readlines()
            f.close()
        random.shuffle(lines)

        x = int(options.count)
        repAgain = True;
        
        if bool(options.repeat):
            while repAgain:
                sys.stdout.write(random.choice(lines))
                if x != -1:
                    x = x - 1
                if x == 0:
                    break

        else:
            if x == -1:
                x = len(lines)
            for index in range(x):
                sys.stdout.write(lines[index])
    except IOError as err:
    # except IOError as (errno, strerror):
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()