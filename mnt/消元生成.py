import random

# Constants
NUM_ROWS = 53
NUM_COLS = 254
NUM_ELIMINATORS = 106

# Generate random data for the files
def generate_random_row(num_cols):
    return sorted(random.sample(range(num_cols), random.randint(1, 20)))

# Generate rows for 被消元行 and 消元子 files
beixiaoyuans = [generate_random_row(NUM_COLS) for _ in range(NUM_ROWS)]
xiaoyuans = [generate_random_row(NUM_COLS) for _ in range(NUM_ELIMINATORS)]

# Save to files
def save_to_file(filename, data):
    with open(filename, 'w') as f:
        for row in data:
            f.write(' '.join(map(str, row)) + '\n')

# File paths
beixiaoyuan_file_path = '/mnt/data/被消元行.txt'
xiaoyuan_file_path = '/mnt/data/消元子.txt'

# Save the generated data to files
save_to_file(beixiaoyuan_file_path, beixiaoyuans)
save_to_file(xiaoyuan_file_path, xiaoyuans)
