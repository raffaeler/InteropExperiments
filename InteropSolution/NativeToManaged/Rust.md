# How to create the Rust project

Create the basic folders and files:
```
cargo new RustClientConsole
cd RustClientConsole

md .windows

cargo new --lib bindings
```

Edit the `Cargo.toml` file in the root folder and add:

```
[dependencies]
bindings = { path=-"bindings" }
``` 

Edit the `Cargo.toml` file in the `bindings` folder and add:
```
[dependencies]
windows = "0.4.0"

[build-dependencies]
windows = "0.4.0"
```

Create the `build.rs` file in the `bindings` folder and add:
```
fn main() {
    windows::build!(
        CalcComponent::*
        windows::ui::Colors,
        windows::ui::Color
    );

}
```

Replace all the content of the `lib.rs` file in the `bindings\src` folder with the following code:
```
::windows::include bindings!();
```

