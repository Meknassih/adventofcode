use core::panic;
use std::{path::Path, fs::File};
use std::io::{BufRead, BufReader};

fn main() {
    let path = Path::new("input/calibrations.txt");
    let file = match File::open(&path) {
        Ok(file) => file,
        Err(e) => panic!("could not open input file: {e}"),
    };
    let mut reader = BufReader::new(file);
    let mut buf = String::new();
    while let Ok(n) = reader.read_line(&mut buf) {
        if n == 0 {
            break;
        }
        println!("{buf}");
    }
}

