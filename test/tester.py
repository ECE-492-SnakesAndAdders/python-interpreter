###############################################################################
# Description: Automates the process of verifying tests
# Author(s): Isaac Joffe
# Copyright: University of Alberta, 2024
# License: CC-BY-4.0
###############################################################################


import argparse


def parse_options():
    parser = argparse.ArgumentParser()
    parser.add_argument('--tests-path', '--tests', '--test', '--t', nargs=1, type=str, required=True, help='path to the file containing the tests to run')
    parser.add_argument('--program-path', '--program', '--prog', '--p', nargs=1, type=str, required=True, help='path to the program to run the tests on')
    options = parser.parse_args()
    return options


def main():
    options = parse_options()

    # read in the tests as raw text from the input file
    tests_text = []
    with open(options.tests_path[0], 'r') as f:
        tests_text = f.readlines()

    # parse this text into input-output pairs
    inputs = []
    outputs = []
    for line in tests_text:
        if line.startswith('| `'):
            inputs.append(line.split('`')[1].replace('\\', ''))
            outputs.append(line.split('`')[3].replace('\'', ''))

    # first verify all tests are correct in normal CPython
    for i in range(len(inputs)):
        is_incorrect = True
        try:
            is_incorrect = (str(eval(inputs[i])) != outputs[i])
            # account for None / null-print case
            if (str(eval(inputs[i])) == "None") and (outputs[i] == " "):
                continue
            # account for empty string case
            if (str(eval(inputs[i])) == "") and (outputs[i] == ""):
                continue
        # catch errors for those tests
        except TypeError:
            is_incorrect = (outputs[i] != "TypeError")
        except ZeroDivisionError:
            is_incorrect = (outputs[i] != "ZeroDivisionError")
        # print error information to help localize
        if is_incorrect:
            print(f'Error in test #{i}:')
            print(f'\tInput: {inputs[i]}')
            print(f'\tExpected Output: {outputs[i]}')
            print(f'\tActual Output: {eval(inputs[i])}')
    
    # now verify that outputs are correct in C3Python

    return


if __name__ == "__main__":
    main()
