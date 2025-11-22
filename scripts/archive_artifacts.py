import argparse
import os
import zipfile
import time

def make_rel_archive(a_args):
    zip_path = os.path.join(a_args.out_dir, a_args.name + ".zip")
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as archive:

        def do_write(a_path):
            os.utime(a_path, (time.time(), time.time()))
            archive.write(a_path, f"F4SE/Plugins/{os.path.basename(a_path)}")

        do_write(a_args.dll)

def parse_arguments():
    parser = argparse.ArgumentParser(description="archive build artifacts for distribution")
    parser.add_argument("--dll", type=str, help="the full dll path", required=True)
    parser.add_argument("--name", type=str, help="the project name", required=True)
    parser.add_argument("--out-dir", type=str, help="output directory", required=True)
    parser.add_argument("--src-dir", type=str, help="source directory", required=True)
    return parser.parse_args()

def main():
    args = parse_arguments()

    os.makedirs(args.out_dir, exist_ok=True)
    make_rel_archive(args)

if __name__ == "__main__":
    main()
