import re
import fileinput

# Function to read data from the text file
def read_data(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

# Function to parse filename and extract relevant information
def parse_filename(filename):
    #pattern = r'\d{2}_\d{2}_\d{2}_[\w]+_[\w]+_([\w\d]+)_\w_([\d+\w+])_([\w\d\-]+)_([\w\d]+)_Run_(\d+)[.-\_]+'
    pattern = r'\d{2}_\d{2}_\d{2}_[\w]'
    match = re.match(pattern, filename)
    if match:
        identifier, lambda_value, abs_unit, abs_mantissa, run_number = match.groups()
        return {
            "identifier": identifier,
            "lambda_value": lambda_value,
            "abs_unit": abs_unit,
            "abs_mantissa": abs_mantissa,
            "run_number": run_number
        }
    else:
        return None

# Function to update or append the text file with new data
def update_file(filename, parsed_info):
    with open(filename, 'r') as file:
        lines = file.readlines()
    
    skip_count = 0
    startData = 0
    lambdafound = False
    IDFound = False
    AbsFound = False
    RunFound = False

    for i, line in enumerate(lines):
        if skip_count < 74:
            skip_count += 1
            continue

        if "lambda" in line.lower():
            line = "lambda:\t" + parsed_info["lambda_value"]
            lambdafound = True
            continue
        if "ID" in line.upper():
            line = "ID:\t" + parsed_info["identifier"]
            IDFound = True
            continue
        if "abs" in line.lower():
            line = "Abs:\t" + parsed_info["abs_unit"] + "." + parsed_info["abs_mantissa"]
            AbsFound = True
            continue
        if "run" in line.lower():
            line = "Run:\t" + parsed_info["run_number"]
            RunFound = True
            continue

        if "#" in line:
            startData = i
            break
        
        if len(line.strip().split()) == 2 and all(word.isdigit() for word in line.strip().split()):
            # Insert "#" before the line
            lines.insert(i, "#\n")
            startData = i
            break
        
        if not RunFound:
            lines.insert(startData, "Run:\t" + parsed_info["run_number"])
        if not IDFound:
            lines.insert(startData, "ID:\t" + parsed_info["identifier"])
        if not AbsFound:
            lines.insert(startData, "Abs:\t" + parsed_info["abs_unit"] + "." + parsed_info["abs_mantissa"])
        if not lambdafound:
            lines.insert(startData, "lambda:\t" + parsed_info["lambda_value"])

    with open(filename, 'w') as file:
        for line in lines:
            file.write(line)

# Example usage
filename = '10_04_24_vetro oro gold_ID_T_0_800_-0_0001_Run_2_1.txt'
example = 'Lambda:\t800\nAbs:\t\t-0.0001\nID:\t\t0\nRun:\t\t2\n#'

parsed_info = parse_filename(filename)
if parsed_info:

    update_file(filename, parsed_info)
    print("File updated successfully!")
else:
    print("Filename does not match the expected pattern.")
