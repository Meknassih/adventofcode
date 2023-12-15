use core::panic;
use std::collections::HashMap;
use std::io::{BufRead, BufReader};
use std::{fs::File, path::Path};

fn main() {
    let mut sum = 0;
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
        let parsed_line = transform_numbers(&buf);
        let mut numbers = Vec::new();
        println!("{parsed_line}");
        for c in parsed_line.chars() {
            if c.is_numeric() {
                numbers.push(c);
            }
        }
        let formed_number = numbers[0].to_string() + &numbers[numbers.len() - 1].to_string();
        let parsed_number = formed_number.parse::<u32>().unwrap();
        println!("\t{parsed_number}");
        sum += parsed_number;
        buf.clear();
    }
    println!("{sum}");
}

fn transform_numbers(buffer: &String) -> String {
    let mut text_numbers = HashMap::new();
    text_numbers.insert("one", "1");
    text_numbers.insert("two", "2");
    text_numbers.insert("three", "3");
    text_numbers.insert("four", "4");
    text_numbers.insert("five", "5");
    text_numbers.insert("six", "6");
    text_numbers.insert("seven", "7");
    text_numbers.insert("eight", "8");
    text_numbers.insert("nine", "9");
    let mut i = 0;
    let mut skip_counter = 0;
    let mut build = String::new();
    // Check if we can match a keyword at each char of the line
    while i < buffer.len() - 1 {
        for (word, num) in &text_numbers {
            let upper_bound = if i + word.len() > buffer.len() {
                buffer.len()
            } else {
                i + word.len()
            };
            // Compare a substring the length of the current keyword
            if &buffer[i..upper_bound] == *word {
                // Set how much chars we need to ignore next
                skip_counter = word.len();
                // Append corresponding digit to new string
                build.push_str(num);
                break;
            }
        }
        if skip_counter > 0 {
            // We are still inside a previously matched keyword, ignore
            skip_counter -= 1;
        } else {
            // Either a random char or a digit, append as is
            build.push(buffer.chars().nth(i).unwrap());
        }
        i += 1;
    }
    build.to_string()
}
