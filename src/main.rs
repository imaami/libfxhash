// SPDX-License-Identifier: MIT

extern crate rustc_hash;

use rustc_hash::FxHasher;
use std::env;
use std::hash::Hasher;

fn main() {
	let args: Vec<String> = env::args().collect();

	for arg in &args[1..] {
		let mut hasher = FxHasher::default();
		hasher.write(arg.as_bytes());
		let result = hasher.finish();

		#[cfg(target_pointer_width = "32")]
		println!("{:08x}", result);

		#[cfg(target_pointer_width = "64")]
		println!("{:016x}", result);
	}
}
