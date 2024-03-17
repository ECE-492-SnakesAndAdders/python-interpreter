###############################################################################
# Description: Automates the process of verifying tests
# Author(s): Isaac Joffe
# Copyright: University of Alberta, 2024
# License: CC-BY-4.0
###############################################################################


import argparse
import subprocess


def parse_options():
    parser = argparse.ArgumentParser()
    parser.add_argument('--tests-path', '--tests', '--test', '--t', nargs=1, type=str, required=True, help='path to the file containing the tests to run')
    parser.add_argument('--program-path', '--program', '--prog', '--p', nargs=1, type=str, required=True, help='path to the program to run the tests on')
    parser.add_argument('--input-path', '--input', '--in', '--i', nargs=1, type=str, required=True, help='path to the file to save the inputs of the tests in')
    parser.add_argument('--output-path', '--output', '--out', '--o', nargs=1, type=str, required=True, help='path to the file to save the outputs of the tests in')
    parser.add_argument('--generated-path', '--generated', '--gen', '--g', nargs=1, type=str, required=True, help='path to the file to save the results of the tests in')
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
            outputs.append(line.split('`')[3].strip())

    # save this data into files for human inspection if desired
    with open(options.input_path[0], 'w') as f:
        for input_command in inputs:
            f.write(input_command)
            f.write('\n')
    with open(options.output_path[0], 'w') as f:
        for output_result in outputs:
            f.write(output_result)
            f.write('\n')

    # first verify all tests are correct in normal CPython
    print('Verifying all tests pass in CPython...')
    error_count = 0
    for i in range(len(inputs)):
        is_incorrect = True
        try:
            is_incorrect = (str(eval(inputs[i])) != outputs[i].replace('\'', ''))
            # account for None / null-print case
            if (str(eval(inputs[i])) == 'None') and (outputs[i] ==''):
                continue
            # account for empty string case
            if (str(eval(inputs[i])) == '') and (outputs[i].replace('\'', '') == '\'\''):
                continue
        # catch errors for those tests
        except TypeError:
            is_incorrect = (outputs[i] != 'TypeError')
        except ZeroDivisionError:
            is_incorrect = (outputs[i] != 'ZeroDivisionError')
        # print error information to help localize
        if is_incorrect:
            error_count += 1
            print(f'Error in test #{i}:')
            print(f'\tInput: {inputs[i]}')
            print(f'\tExpected Output: {outputs[i]}')
            print(f'\tActual Output: {eval(inputs[i])}')
    # print information about tests to user
    if error_count:
        print(f'\t{error_count} tests failed.')
    else:
        print('\tAll tests passed!')

    # now verify that outputs are correct in C3Python
    print('Verifying all tests pass in C3Python...')
    error_count = 0
    # run the tests with file redirection as a subprocess
    command = f'{options.program_path[0]} < {options.input_path[0]} > {options.generated_path[0]}'.split(' ')
    try:
        subprocess.run(command,
                       shell=True,
                       timeout=0.05,
        )
    # let the timer expire so that the process terminates
    except subprocess.TimeoutExpired:
        pass
    # read the data written to the output log file
    with open(options.generated_path[0], 'r') as f:
        data = f.readlines()
    # compare the output file to the desired outputs
    test_number = 0
    for i in range(len(data)):
        if test_number == len(inputs):
            break
        line = data[i].strip()
        if line.startswith('>>> '):
            line = line.split('>>> ')
            # simple case where each output printed on a separate line
            if len(line) == 2:
                if line[1] != outputs[test_number]:
                    # account for errors printed over multiple lines
                    if line[1].startswith('Traceback'):
                        message = data[i + 2].split(':')[0]
                        if message == outputs[test_number]:
                            test_number += 1
                            continue
                    # print error information to help localize
                    error_count += 1
                    print(f'Error in test #{test_number}:')
                    print(f'\tInput: {inputs[test_number]}')
                    print(f'\tExpected Output: {outputs[test_number]}')
                    print(f'\tActual Output: {line[1]}')
                test_number += 1
            # account for case where nothing is printed (so multiple on one line)
            elif len(line) < 10:
                for part in line[1:]:
                    if part != outputs[test_number]:
                        error_count += 1
                        print(f'Error in test #{test_number}:')
                        print(f'\tInput: {inputs[test_number]}')
                        print(f'\tExpected Output: {outputs[test_number]}')
                        print(f'\tActual Output: {line[0]}')
                    test_number += 1
            else:
                break
    # print information about tests to user
    if error_count:
        print(f'\t{error_count} tests failed.')
    else:
        print('\tAll tests passed!')

    return


if __name__ == '__main__':
    main()
