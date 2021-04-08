use std::env;
use std::fs;
use std::path::Path;

fn main() -> std::io::Result<()> {
    windows::build!(
        Windows::Foundation::*,
        Windows::Foundation::Collections::IVectorView,
        ManagedComponent::*,

        // windows::foundation::collections::IVector,
        // windows::foundation::{IAsyncOperationWithProgress, Uri},
        // windows::web::syndication::{
        //     ISyndicationText, RetrievalProgress, SyndicationClient, SyndicationFeed, SyndicationItem,
        // },
    );

    // the name of the executable (without extension) is in the Cargo.toml
    // section "[package]" - "name" => rust_client_console

    let bindings_dir = env::var_os("CARGO_MANIFEST_DIR").unwrap();
    //let out_dir = env::var_os("OUT_DIR").unwrap(); // useless
    let current_dir = env::current_dir().unwrap();
    let profile = std::env::var("PROFILE").unwrap();
    let target_dir = Path::new(&current_dir).join("../target").join(profile).canonicalize().unwrap();
    //println!("cargo:warning={:?}", target_dir);   // print a warning

    let source = Path::new(&bindings_dir).join("rust_client_console.exe.manifest");
    let target = Path::new(&target_dir).join("rust_client_console.exe.manifest");
    fs::copy(source, target)?;

    // let source_cd = Path::new(&bindings_dir).join("cd.xml");
    // let target_cd = Path::new(&target_dir).join("cd.xml");
    // fs::copy(source_cd, target_cd)?;

    // print all the env variables
    // for (k, v) in std::env::vars() {
    //     println!("cargo:warning={} -> {}", k, v);
    // }
    
    Ok(())
}