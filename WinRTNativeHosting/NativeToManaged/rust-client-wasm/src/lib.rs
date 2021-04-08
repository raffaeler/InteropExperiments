mod utils;

use bindings::{
    calc_component::Calculator,
};

use wasm_bindgen::prelude::*;

// When the `wee_alloc` feature is enabled, use `wee_alloc` as the global
// allocator.
#[cfg(feature = "wee_alloc")]
#[global_allocator]
static ALLOC: wee_alloc::WeeAlloc = wee_alloc::WeeAlloc::INIT;

#[wasm_bindgen]
extern {
    fn alert(s: &str);
}

#[wasm_bindgen]
pub fn greet() {
    
    let calculator = Calculator::new()?;
    let res = calculator.add(1.2, 3.3)?;
    let str = res.to_string();

    alert("Hello, {}", str);
}
