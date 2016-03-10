from subprocess import Popen
import time, os


def test_blocks_unit(input_file, block_size, output_file):
    file_size = os.path.getsize(input_file)
    start = time.time()
    Process = Popen(['./write_blocks %s %s' % (output_file, block_size)], shell=True)
    end = time.time()
    s_spent = end - start
    return s_spent


def test_lines_unit(output_file):
    start = time.time()
    Process = Popen(['./write_lines %s' % output_file], shell=True)
    end = time.time()
    s_spent = end - start
    return s_spent


def test_blocks():
    block_sizes = [512, 1024, 4096, 8192, 16384, 32768, 1048576, 2097152, 4194304]
    time_spent = []
    input_file = 'test_dataset/g_plusAnonymized.csv'
    output_file = 'records.dat'
    for size in block_sizes:
        ms_spent = test_blocks_unit(input_file, size, output_file)*1000
        print "block size "+str(size)+": "+str(ms_spent)
        time_spent.append(ms_spent)


def test_lines():
    output_file = 'records.dat'
    ms_spent = test_lines_unit(output_file)*1000
    print "write lines:", ms_spent


if __name__ == '__main__':
    test_blocks()
    test_lines()
