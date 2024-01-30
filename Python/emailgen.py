import os
import glob

def read_index_file(index_file_path):
    mapping = {}
    with open(index_file_path, 'r') as file:
        for line in file:
            parts = line.strip().split(': ')
            if len(parts) == 2:
                mapping[parts[0]] = parts[1]
    return mapping

def skip_action(line):
    return line.strip()

def process_line(line, email_mapping):
    if line.startswith('//'):
        return skip_action(line)

    parts = line.strip().split(' ')
    if len(parts) == 2:
        email, identifier = parts
        if identifier in email_mapping and email_mapping[identifier] != 'SKIP':
            return email + email_mapping[identifier]
        elif identifier in ['SKIP', 'DELETE', 'SAVE']:
            return skip_action()
    print(parts)
    return line + " (Error: Invalid format or unknown identifier)"

def process_file(input_path, output_path, email_mapping):
    processed_emails = set()
    if os.path.exists(output_path):
        with open(output_path, 'r') as output_file:
            for line in output_file:
                processed_emails.add(line.strip().split(' ')[0])

    with open(input_path, 'r') as input_file, open(output_path, 'a') as output_file:
        for line in input_file:
            result = process_line(line, email_mapping)
            if result and not result.startswith("Error") and result.split(' ')[0] not in processed_emails:
                output_file.write(result + '\n')
                processed_emails.add(result.split(' ')[0])
            elif result and result.startswith("Error"):
                output_file.write(result + '\n')

def main():
    index_file_path = 'index.txt'
    email_mapping = read_index_file(index_file_path)

    input_prefix = 'list'
    output_dir = 'emails'
    output_prefix = 'output'

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for input_file in glob.glob(input_prefix + '*'):
        file_number = input_file[len(input_prefix):].split('.')[0]
        output_file = os.path.join(output_dir, output_prefix + file_number + '.txt')

        process_file(input_file, output_file, email_mapping)
        print(f"Processed {input_file} and appended to {output_file}")

if __name__ == "__main__":
    main()
